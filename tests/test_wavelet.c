#include <stdio.h>
#include <string.h>
#include <unity.h>
#include "unity_internals.h"
#include "wavelet.h"

void setUp(void) {}
void tearDown(void) {}

void test_wavelet_from_vec(void)
{
    u32 vals[] = {5, 1, 9, 3, 7, 2, 8, 6, 4, 10};
    size_t n = sizeof(vals) / sizeof(*vals);

    WaveletTree* wavelet = wavelet_from_vec(vals, n);

    u32 kth = 0;

    kth = wavelet_kth(wavelet, 2, 7, 2);
    TEST_ASSERT_EQUAL_INT(3, kth);

    kth = wavelet_kth(wavelet, 0, 9, 5);
    TEST_ASSERT_EQUAL_INT(5, kth);

    kth = wavelet_kth(wavelet, 3, 6, 3);
    TEST_ASSERT_EQUAL_INT(7, kth);

    u32 leq = 0;

    leq = wavelet_leq(wavelet, 2, 7, 5);
    TEST_ASSERT_EQUAL_INT(2, leq);

    leq = wavelet_leq(wavelet, 0, 9, 7);
    TEST_ASSERT_EQUAL_INT(7, leq);

    leq = wavelet_leq(wavelet, 3, 6, 9);
    TEST_ASSERT_EQUAL_INT(4, leq);

    wavelet_destroy(wavelet);
    wavelet = NULL;
}

void test_wavelet_from_str(void)
{
    const char* str = "wavelettree";
    const size_t len = strlen(str);

    WaveletTree* str_wavelet = wavelet_from_string(str);

    char skth = 0;

    skth = (char)wavelet_kth(str_wavelet, 0, len - 1, 1);
    TEST_ASSERT_EQUAL_CHAR('a', skth);

    skth = (char)wavelet_kth(str_wavelet, 0, len - 1, 3);
    TEST_ASSERT_EQUAL_CHAR('e', skth);

    skth = (char)wavelet_kth(str_wavelet, 2, 7, 3);
    TEST_ASSERT_EQUAL_CHAR('l', skth);

    u32 sleq = 0;

    sleq = wavelet_leq(str_wavelet, 0, len - 1, 'm');
    TEST_ASSERT_EQUAL_INT(6, sleq);

    sleq = wavelet_leq(str_wavelet, 1, 8, 't');
    TEST_ASSERT_EQUAL_INT(7, sleq);

    sleq = wavelet_leq(str_wavelet, 0, len - 1, 'a');
    TEST_ASSERT_EQUAL_INT(1, sleq);

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
