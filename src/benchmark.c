#include "types.h"
#include "wavelet.h"
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

typedef void (*BenchFunction)(void *arg, size_t test_count);

static size_t bench_num = 1;

static void benchmark_ms(const char *const name, void *const arg,
			 const BenchFunction fn, const size_t test_count)
{
	// Determina cuántas veces se repite el benchmark para sacar el promedio
	static const size_t REPEAT_COUNT = 5;

	printf("Bench %zu [%s]: ...", bench_num, name);
	fflush(stdout);

	long double avg_elapsed_ms = 0;

	for (size_t i = 0; i < REPEAT_COUNT; ++i) {
		struct timespec start;
		struct timespec end;

		clock_gettime(CLOCK_MONOTONIC, &start);
		fn(arg, test_count);
		clock_gettime(CLOCK_MONOTONIC, &end);

		const long double elapsed_s =
			(long double)(end.tv_sec - start.tv_sec) +
			((long double)(end.tv_nsec - start.tv_nsec) / 1e9);

		const long double elapsed_ms = elapsed_s * 1000;
		avg_elapsed_ms += elapsed_ms / REPEAT_COUNT;
	}

	printf("\rBench %zu [%s]: %.2Lf ms\n", bench_num, name, avg_elapsed_ms);

	++bench_num;
}

typedef struct BenchCtx {
	WaveletTree *wavelet;
	size_t data_size;
} BenchCtx;

static void bench_access(void *const arg, const size_t test_count)
{
	BenchCtx *const ctx = arg;

	volatile u32 result = 0;

	for (size_t i = 0; i < test_count; ++i)
		result = wavelet_at(ctx->wavelet, i % ctx->data_size);
}

static void bench_rank(void *const arg, const size_t test_count)
{
	BenchCtx *const ctx = arg;

	volatile u32 result = 0;

	for (size_t i = 0; i < test_count; ++i)
		result = wavelet_rank(ctx->wavelet, 0, ctx->data_size, 'e');
}

static void bench_ksmall(void *const arg, const size_t test_count)
{
	BenchCtx *const ctx = arg;

	volatile u32 result = 0;

	for (size_t i = 0; i < test_count; ++i)
		result = wavelet_kth_smallest(ctx->wavelet, 0, ctx->data_size,
					      i % ctx->data_size);
}

static void bench_leq(void *const arg, const size_t test_count)
{
	BenchCtx *const ctx = arg;

	volatile u32 result = 0;

	for (size_t i = 0; i < test_count; ++i)
		result = wavelet_leq(ctx->wavelet, 0, ctx->data_size, 'e');
}

int main(void)
{
	const char *const str_1 =
		"Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.";
	const char *const str_2 =
		"Sed ut perspiciatis unde omnis iste natus error sit voluptatem accusantium doloremque laudantium, totam rem aperiam, eaque ipsa quae ab illo inventore veritatis et quasi architecto beatae vitae dicta sunt explicabo. Nemo enim ipsam voluptatem quia voluptas sit aspernatur aut odit aut fugit, sed quia consequuntur magni dolores eos qui ratione voluptatem sequi nesciunt. Neque porro quisquam est, qui dolorem ipsum quia dolor sit amet, consectetur, adipisci velit, sed quia non numquam eius modi tempora incidunt ut labore et dolore magnam aliquam quaerat voluptatem. Ut enim ad minima veniam, quis nostrum exercitationem ullam corporis suscipit laboriosam, nisi ut aliquid ex ea commodi consequatur? Quis autem vel eum iure reprehenderit qui in ea voluptate velit esse quam nihil molestiae consequatur, vel illum qui dolorem eum fugiat quo voluptas nulla pariatur?";
	const char *const str_3 =
		"Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum. Sed ut perspiciatis unde omnis iste natus error sit voluptatem accusantium doloremque laudantium, totam rem aperiam, eaque ipsa quae ab illo inventore veritatis et quasi architecto beatae vitae dicta sunt explicabo. Nemo enim ipsam voluptatem quia voluptas sit aspernatur aut odit aut fugit, sed quia consequuntur magni dolores eos qui ratione voluptatem sequi nesciunt. Neque porro quisquam est, qui dolorem ipsum quia dolor sit amet, consectetur, adipisci velit, sed quia non numquam eius modi tempora incidunt ut labore et dolore magnam aliquam quaerat voluptatem. Ut enim ad minima veniam, quis nostrum exercitationem ullam corporis suscipit laboriosam, nisi ut aliquid ex ea commodi consequatur? Quis autem vel eum iure reprehenderit qui in ea voluptate velit esse quam nihil molestiae consequatur, vel illum qui dolorem eum fugiat quo voluptas nulla pariatur?";

	BenchCtx ctx_1 = {
		.data_size = strlen(str_1),
		.wavelet = wavelet_from_string(str_1),
	};

	BenchCtx ctx_2 = {
		.data_size = strlen(str_2),
		.wavelet = wavelet_from_string(str_2),
	};

	BenchCtx ctx_3 = {
		.data_size = strlen(str_3),
		.wavelet = wavelet_from_string(str_3),
	};

	printf("========================================\n");
	printf("Tamaño de inputs\n");
	printf("========================================\n");
	printf("str_1: %zu\n", ctx_1.data_size);
	printf("str_2: %zu\n", ctx_2.data_size);
	printf("str_3: %zu\n", ctx_3.data_size);
	printf("\n");

	printf("========================================\n");
	printf("Benchmarks\n");
	printf("========================================\n");
	printf("\n");

	printf("Access:\n");
	benchmark_ms("access(str_1) - 1e6", &ctx_1, bench_access, 1000000);
	benchmark_ms("access(str_2) - 1e6", &ctx_2, bench_access, 1000000);
	benchmark_ms("access(str_3) - 1e6", &ctx_3, bench_access, 1000000);
	benchmark_ms("access(str_1) - 1e7", &ctx_1, bench_access, 10000000);
	benchmark_ms("access(str_2) - 1e7", &ctx_2, bench_access, 10000000);
	benchmark_ms("access(str_3) - 1e7", &ctx_3, bench_access, 10000000);
	benchmark_ms("access(str_1) - 1e8", &ctx_1, bench_access, 100000000);
	benchmark_ms("access(str_2) - 1e8", &ctx_2, bench_access, 100000000);
	benchmark_ms("access(str_3) - 1e8", &ctx_3, bench_access, 100000000);
	printf("\n");

	printf("Rank:\n");
	benchmark_ms("rank(str_1) - 1e6", &ctx_1, bench_rank, 1000000);
	benchmark_ms("rank(str_2) - 1e6", &ctx_2, bench_rank, 1000000);
	benchmark_ms("rank(str_3) - 1e6", &ctx_3, bench_rank, 1000000);
	benchmark_ms("rank(str_1) - 1e7", &ctx_1, bench_rank, 10000000);
	benchmark_ms("rank(str_2) - 1e7", &ctx_2, bench_rank, 10000000);
	benchmark_ms("rank(str_3) - 1e7", &ctx_3, bench_rank, 10000000);
	benchmark_ms("rank(str_1) - 1e8", &ctx_1, bench_rank, 100000000);
	benchmark_ms("rank(str_2) - 1e8", &ctx_2, bench_rank, 100000000);
	benchmark_ms("rank(str_3) - 1e8", &ctx_3, bench_rank, 100000000);
	printf("\n");

	printf("Kth-smallest:\n");
	benchmark_ms("ksmall(str_1) - 1e6", &ctx_1, bench_ksmall, 1000000);
	benchmark_ms("ksmall(str_2) - 1e6", &ctx_2, bench_ksmall, 1000000);
	benchmark_ms("ksmall(str_3) - 1e6", &ctx_3, bench_ksmall, 1000000);
	benchmark_ms("ksmall(str_1) - 1e7", &ctx_1, bench_ksmall, 10000000);
	benchmark_ms("ksmall(str_2) - 1e7", &ctx_2, bench_ksmall, 10000000);
	benchmark_ms("ksmall(str_3) - 1e7", &ctx_3, bench_ksmall, 10000000);
	benchmark_ms("ksmall(str_1) - 1e8", &ctx_1, bench_ksmall, 100000000);
	benchmark_ms("ksmall(str_2) - 1e8", &ctx_2, bench_ksmall, 100000000);
	benchmark_ms("ksmall(str_3) - 1e8", &ctx_3, bench_ksmall, 100000000);
	printf("\n");

	printf("Leq:\n");
	benchmark_ms("leq(str_1) - 1e6", &ctx_1, bench_leq, 1000000);
	benchmark_ms("leq(str_2) - 1e6", &ctx_2, bench_leq, 1000000);
	benchmark_ms("leq(str_3) - 1e6", &ctx_3, bench_leq, 1000000);
	benchmark_ms("leq(str_1) - 1e7", &ctx_1, bench_leq, 10000000);
	benchmark_ms("leq(str_2) - 1e7", &ctx_2, bench_leq, 10000000);
	benchmark_ms("leq(str_3) - 1e7", &ctx_3, bench_leq, 10000000);
	benchmark_ms("leq(str_1) - 1e8", &ctx_1, bench_leq, 100000000);
	benchmark_ms("leq(str_2) - 1e8", &ctx_2, bench_leq, 100000000);
	benchmark_ms("leq(str_3) - 1e8", &ctx_3, bench_leq, 100000000);
	printf("\n");

	wavelet_destroy(ctx_1.wavelet);
	wavelet_destroy(ctx_2.wavelet);
	wavelet_destroy(ctx_3.wavelet);

	return 0;
}
