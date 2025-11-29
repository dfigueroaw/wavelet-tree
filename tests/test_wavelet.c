#include <stdio.h>
#include <string.h>
#include "unity.h"
#include "unity_internals.h"
#include "wavelet.h"

void setUp(void)
{
}
void tearDown(void)
{
}

void test_wavelet_from_vec(void)
{
	const u32 vals[] = { 5, 1, 9, 3, 7, 2, 8, 6, 4, 10 };
	const size_t n = sizeof(vals) / sizeof(*vals);

	WaveletTree *wavelet = wavelet_from_vec(vals, n);

	TEST_ASSERT_EQUAL_INT(3, wavelet_kth_smallest(wavelet, 2, 7, 2));
	TEST_ASSERT_EQUAL_INT(5, wavelet_kth_smallest(wavelet, 0, 9, 5));
	TEST_ASSERT_EQUAL_INT(7, wavelet_kth_smallest(wavelet, 3, 6, 3));

	TEST_ASSERT_EQUAL_INT(2, wavelet_leq(wavelet, 2, 7, 5));
	TEST_ASSERT_EQUAL_INT(7, wavelet_leq(wavelet, 0, 9, 7));
	TEST_ASSERT_EQUAL_INT(4, wavelet_leq(wavelet, 3, 6, 9));

	TEST_ASSERT_EQUAL_INT(5, wavelet_at(wavelet, 0));
	TEST_ASSERT_EQUAL_INT(1, wavelet_at(wavelet, 1));
	TEST_ASSERT_EQUAL_INT(9, wavelet_at(wavelet, 2));
	TEST_ASSERT_EQUAL_INT(3, wavelet_at(wavelet, 3));
	TEST_ASSERT_EQUAL_INT(7, wavelet_at(wavelet, 4));
	TEST_ASSERT_EQUAL_INT(2, wavelet_at(wavelet, 5));
	TEST_ASSERT_EQUAL_INT(8, wavelet_at(wavelet, 6));
	TEST_ASSERT_EQUAL_INT(6, wavelet_at(wavelet, 7));
	TEST_ASSERT_EQUAL_INT(4, wavelet_at(wavelet, 8));
	TEST_ASSERT_EQUAL_INT(10, wavelet_at(wavelet, 9));

	TEST_ASSERT_EQUAL_INT(1, wavelet_rank(wavelet, 0, n - 1, 1));
	TEST_ASSERT_EQUAL_INT(1, wavelet_rank(wavelet, 0, n - 1, 2));
	TEST_ASSERT_EQUAL_INT(1, wavelet_rank(wavelet, 0, n - 1, 3));
	TEST_ASSERT_EQUAL_INT(1, wavelet_rank(wavelet, 0, n - 1, 9));
	TEST_ASSERT_EQUAL_INT(1, wavelet_rank(wavelet, 0, n - 1, 10));
	TEST_ASSERT_EQUAL_INT(0, wavelet_rank(wavelet, 0, n - 1, 11));
	TEST_ASSERT_EQUAL_INT(1, wavelet_rank(wavelet, 0, 4, 5));
	TEST_ASSERT_EQUAL_INT(1, wavelet_rank(wavelet, 0, 4, 9));
	TEST_ASSERT_EQUAL_INT(0, wavelet_rank(wavelet, 6, 9, 2));
	TEST_ASSERT_EQUAL_INT(1, wavelet_rank(wavelet, 3, 7, 7));

	TEST_ASSERT_EQUAL_FLOAT(3.321928, wavelet_entropy(wavelet, 0, n));

	wavelet_destroy(wavelet);
	wavelet = NULL;
}

void test_wavelet_from_str(void)
{
	const char *const restrict str = "wavelettree";
	const size_t len = strlen(str);

	WaveletTree *str_wavelet = wavelet_from_string(str);

	TEST_ASSERT_EQUAL_CHAR('a', (char)wavelet_kth_smallest(str_wavelet, 0,
							       len - 1, 1));
	TEST_ASSERT_EQUAL_CHAR('e', (char)wavelet_kth_smallest(str_wavelet, 0,
							       len - 1, 3));
	TEST_ASSERT_EQUAL_CHAR('l', (char)wavelet_kth_smallest(str_wavelet, 2,
							       7, 3));

	TEST_ASSERT_EQUAL_INT(6, wavelet_leq(str_wavelet, 0, len - 1, 'm'));
	TEST_ASSERT_EQUAL_INT(7, wavelet_leq(str_wavelet, 1, 8, 't'));
	TEST_ASSERT_EQUAL_INT(1, wavelet_leq(str_wavelet, 0, len - 1, 'a'));

	TEST_ASSERT_EQUAL_CHAR('w', (char)wavelet_at(str_wavelet, 0));
	TEST_ASSERT_EQUAL_CHAR('a', (char)wavelet_at(str_wavelet, 1));
	TEST_ASSERT_EQUAL_CHAR('v', (char)wavelet_at(str_wavelet, 2));
	TEST_ASSERT_EQUAL_CHAR('e', (char)wavelet_at(str_wavelet, 3));
	TEST_ASSERT_EQUAL_CHAR('l', (char)wavelet_at(str_wavelet, 4));
	TEST_ASSERT_EQUAL_CHAR('e', (char)wavelet_at(str_wavelet, 5));
	TEST_ASSERT_EQUAL_CHAR('t', (char)wavelet_at(str_wavelet, 6));
	TEST_ASSERT_EQUAL_CHAR('t', (char)wavelet_at(str_wavelet, 7));
	TEST_ASSERT_EQUAL_CHAR('r', (char)wavelet_at(str_wavelet, 8));
	TEST_ASSERT_EQUAL_CHAR('e', (char)wavelet_at(str_wavelet, 9));
	TEST_ASSERT_EQUAL_CHAR('e', (char)wavelet_at(str_wavelet, 10));

	TEST_ASSERT_EQUAL_INT(4, wavelet_rank(str_wavelet, 0, len - 1, 'e'));
	TEST_ASSERT_EQUAL_INT(2, wavelet_rank(str_wavelet, 0, len - 1, 't'));
	TEST_ASSERT_EQUAL_INT(1, wavelet_rank(str_wavelet, 0, len - 1, 'r'));
	TEST_ASSERT_EQUAL_INT(1, wavelet_rank(str_wavelet, 0, len - 1, 'w'));
	TEST_ASSERT_EQUAL_INT(1, wavelet_rank(str_wavelet, 0, len - 1, 'a'));
	TEST_ASSERT_EQUAL_INT(0, wavelet_rank(str_wavelet, 0, len - 1, 's'));
	TEST_ASSERT_EQUAL_INT(2, wavelet_rank(str_wavelet, 0, 6, 'e'));
	TEST_ASSERT_EQUAL_INT(1, wavelet_rank(str_wavelet, 0, 6, 't'));

	TEST_ASSERT_EQUAL_FLOAT(2.550341, wavelet_entropy(str_wavelet, 0, len));

	wavelet_destroy(str_wavelet);
	str_wavelet = NULL;
}

int main(void)
{
	UNITY_BEGIN();

	RUN_TEST(test_wavelet_from_vec);
	RUN_TEST(test_wavelet_from_str);

	return UNITY_END();
}
