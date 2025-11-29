#include "wavelet.h"
#include <stddef.h>
#include <stdio.h>

static const char banner[] = "\
 _____       _                   _           \n\
| ____|_ __ | |_ _ __ ___  _ __ (_) __ _     \n\
|  _| | '_ \\| __| '__/ _ \\| '_ \\| |/ _` | \n\
| |___| | | | |_| | | (_) | |_) | | (_| |    \n\
|_____|_| |_|\\__|_|  \\___/| .__/|_|\\__,_| \n\
                          |_|                \n\
";

static const char s1[] = "abcdefg";
static const char s2[] = "abcdefghijklmno";
static const char s3[] = "abcdefghijklmnopqrstuvwxyz";

#define BUF_LEN 2048
static char buf[BUF_LEN];

int main(void)
{
	printf("=======================================================\n");
	printf("%s", banner);
	printf("=======================================================\n");
	printf("\n");

	printf("Escoge una opción:\n");
	printf("1. String corto\n");
	printf("2. String medio\n");
	printf("3. String largo\n");
	printf("4. String personalizado\n");
	printf("\n");

	int choice = -1;

	do {
		printf("> ");
		if (scanf("%i", &choice) == 0) {
			puts("error inesperado al leer selección");
			return 1;
		}
	} while (choice < 1 || choice > 4);

	printf("\n");

	const char *selected_str = NULL;

	switch (choice) {
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
	default:
		// Unreachable
		puts("Selección incorrecta.");
		return 1;
	}

	printf("string seleccionado: %s\n", selected_str);

	WaveletTree *wavelet = wavelet_from_string(selected_str);

	size_t l = 0;
	size_t r = 0;

	while (1) {
		printf("Seleccione un rango\n");
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
