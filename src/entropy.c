#include "compressor.h"
#include "wavelet.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

static const char banner[] = "\
 _____       _                   _           \n\
| ____|_ __ | |_ _ __ ___  _ __ (_) __ _     \n\
|  _| | '_ \\| __| '__/ _ \\| '_ \\| |/ _` | \n\
| |___| | | | |_| | | (_) | |_) | | (_| |    \n\
|_____|_| |_|\\__|_|  \\___/| .__/|_|\\__,_| \n\
                          |_|                \n\
";

static const char *const test_files[] = {
	"high_100mb.bin",  "high_50mb.bin",    "high_10mb.bin",
	"high_1mb.bin",	   "medium_100mb.bin", "medium_50mb.bin",
	"medium_10mb.bin", "medium_1mb.bin",   "low_100mb.bin",
	"low_50mb.bin",	   "low_10mb.bin",     "low_1mb.bin",
};
static const size_t N_TEST_FILES = sizeof(test_files) / sizeof(*test_files);

static const char s1[] = "abcdefg";
static const char s2[] = "abcdefghijklmno";
static const char s3[] = "abcdefghijklmnopqrstuvwxyz";

#define BUF_LEN 2048
static char buf[BUF_LEN];

typedef struct {
	const char *name;
	double size;
	double overhead;
	i32 ok;
} CompResult;

// NOLINTBEGIN
static const Compressor *const compressors[] = {
	&GZIP_COMPRESSOR,
	&BZIP2_COMPRESSOR,
	&XZ_COMPRESSOR,
	&ZSTD_COMPRESSOR,
};
// NOLINTEND
static const size_t N_COMP = sizeof(compressors) / sizeof(*compressors);

static void compare_compressors(const char *const restrict path)
{
	printf("Comparación para archivo: %s\n", path);

	size_t n = 0;
	WaveletTree *wavelet = wavelet_from_file(path, &n);
	double H = wavelet_entropy(wavelet, 0, n);
	double S_min = (H * (double)n) / 8.0;

	printf("Tamaño del archivo: %zu\n", n);
	printf("Entropía del archivo: %f\n", H);
	printf("Tamaño mínimo teórico: %f bytes\n", S_min);

	CompResult results[N_COMP + 1];
	results[N_COMP] = (CompResult){
		.name = "teórico",
		.size = S_min,
		.overhead = 1,
		.ok = 1,
	};

	for (size_t i = 0; i < N_COMP; ++i) {
		const Compressor *c = compressors[i];
		size_t out = c->compress(path);

		if (out == 0) {
			results[i] = (CompResult){
				.name = c->name,
				.size = 0.0,
				.overhead = 0.0,
				.ok = 0,
			};
			continue;
		}

		double overhead = (double)out / S_min;

		results[i] = (CompResult){
			.name = c->name,
			.size = (double)out,
			.overhead = overhead,
			.ok = 1,
		};
	}

	printf("%-12s | %-12s | %-12s\n", "Método", "Tamaño", "Overhead");
	printf("=========================================\n");

	for (size_t i = 0; i < N_COMP; ++i) {
		if (!results[i].ok) {
			printf("%-12s | %-12s | %-12s\n", results[i].name,
			       "error", "error");
		} else {
			printf("%-12s | %-12.2f | %-12f\n", results[i].name,
			       results[i].size, results[i].overhead);
		}
	}

	printf("\n");
	wavelet_destroy(wavelet);
}

int main()
{
	printf("=======================================================\n");
	printf("%s", banner);
	printf("=======================================================\n");
	printf("\n");

	printf("Escoge una opción:\n");
	printf("0. Salir\n");
	printf("1. String corto\n");
	printf("2. String medio\n");
	printf("3. String largo\n");
	printf("4. String personalizado\n");
	printf("5. Comparar compresores en archivo\n");
	printf("6. Comparar compresores en archivos de prueba\n");
	printf("\n");

	int choice = -1;

	do {
		printf("> ");
		if (scanf("%i", &choice) == 0) {
			puts("error inesperado al leer selección");
			return 1;
		}
	} while (choice < 0 || choice > 6);

	printf("\n");

	const char *selected_str = NULL;

	switch (choice) {
	case 0:
		return 0;
	case 1:
		selected_str = s1;
		break;
	case 2:
		selected_str = s2;
		break;
	case 3:
		selected_str = s3;
		break;
	case 4:
		printf("Escribe el string:\n");
		printf("> ");
		if (scanf("%s", buf) == 0) {
			puts("error inesperado al leer string");
			return 1;
		};
		selected_str = buf;
		break;
	case 5:
		printf("Escribe la ruta del archivo:\n");
		printf("> ");
		if (scanf("%s", buf) == 0) {
			puts("error inesperado al leer string");
			return 1;
		};
		selected_str = buf;
		break;
	case 6:
		break;
	default:
		// Unreachable
		puts("Selección incorrecta.");
		return 1;
	}

	if (choice == 5) {
		compare_compressors(buf);
		return 0;
	}

	if (choice == 6) {
		for (size_t i = 0; i < N_TEST_FILES; ++i) {
			char path[512];
			snprintf(path, sizeof(path), "./data/%s",
				 test_files[i]);
			compare_compressors(path);
		}
		return 0;
	}

	printf("string seleccionado: %s\n", selected_str);

	WaveletTree *wavelet = wavelet_from_string(selected_str);

	size_t l = 0;
	size_t r = 0;

	while (1) {
		printf("Seleccione un rango ([0, 0) para finalizar):\n");
		printf("> ");
		if (scanf("%zu %zu", &l, &r) == 0) {
			puts("error inesperado al leer rango");
			return 1;
		}

		if (l == 0 && r == 0)
			break;

		printf("la entropía de este string en el rango [%zu,%zu) es: %f\n",
		       l, r, wavelet_entropy(wavelet, l, r));
	}

	wavelet_destroy(wavelet);
	wavelet = NULL;

	return 0;
}
