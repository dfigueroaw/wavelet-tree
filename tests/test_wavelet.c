#include <stdio.h>
#include <string.h>
#include "unity.h"
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

    u32 elem = 0;

    elem = wavelet_at(wavelet, 0);
    TEST_ASSERT_EQUAL_INT(5, elem);

    elem = wavelet_at(wavelet, 1);
    TEST_ASSERT_EQUAL_INT(1, elem);

    elem = wavelet_at(wavelet, 2);
    TEST_ASSERT_EQUAL_INT(9, elem);

    elem = wavelet_at(wavelet, 3);
    TEST_ASSERT_EQUAL_INT(3, elem);

    elem = wavelet_at(wavelet, 4);
    TEST_ASSERT_EQUAL_INT(7, elem);

    elem = wavelet_at(wavelet, 5);
    TEST_ASSERT_EQUAL_INT(2, elem);

    elem = wavelet_at(wavelet, 6);
    TEST_ASSERT_EQUAL_INT(8, elem);

    elem = wavelet_at(wavelet, 7);
    TEST_ASSERT_EQUAL_INT(6, elem);

    elem = wavelet_at(wavelet, 8);
    TEST_ASSERT_EQUAL_INT(4, elem);

    elem = wavelet_at(wavelet, 9);
    TEST_ASSERT_EQUAL_INT(10, elem);

    u32 rank = 0;

    rank = wavelet_rank(wavelet, 0, n - 1, 1);
    TEST_ASSERT_EQUAL_INT(1, rank);

    rank = wavelet_rank(wavelet, 0, n - 1, 2);
    TEST_ASSERT_EQUAL_INT(1, rank);

    rank = wavelet_rank(wavelet, 0, n - 1, 3);
    TEST_ASSERT_EQUAL_INT(1, rank);

    rank = wavelet_rank(wavelet, 0, n - 1, 9);
    TEST_ASSERT_EQUAL_INT(1, rank);

    rank = wavelet_rank(wavelet, 0, n - 1, 10);
    TEST_ASSERT_EQUAL_INT(1, rank);

    rank = wavelet_rank(wavelet, 0, n - 1, 11);
    TEST_ASSERT_EQUAL_INT(0, rank);

    rank = wavelet_rank(wavelet, 0, 4, 5);
    TEST_ASSERT_EQUAL_INT(1, rank);

    rank = wavelet_rank(wavelet, 0, 4, 9);
    TEST_ASSERT_EQUAL_INT(1, rank);

    rank = wavelet_rank(wavelet, 6, 9, 2);
    TEST_ASSERT_EQUAL_INT(0, rank);

    rank = wavelet_rank(wavelet, 3, 7, 7);
    TEST_ASSERT_EQUAL_INT(1, rank);

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

    u32 selem = 0;

    selem = (char)wavelet_at(str_wavelet, 0);
    TEST_ASSERT_EQUAL_CHAR('w', selem);

    selem = (char)wavelet_at(str_wavelet, 1);
    TEST_ASSERT_EQUAL_CHAR('a', selem);

    selem = (char)wavelet_at(str_wavelet, 2);
    TEST_ASSERT_EQUAL_CHAR('v', selem);

    selem = (char)wavelet_at(str_wavelet, 3);
    TEST_ASSERT_EQUAL_CHAR('e', selem);

    selem = (char)wavelet_at(str_wavelet, 4);
    TEST_ASSERT_EQUAL_CHAR('l', selem);

    selem = (char)wavelet_at(str_wavelet, 5);
    TEST_ASSERT_EQUAL_CHAR('e', selem);

    selem = (char)wavelet_at(str_wavelet, 6);
    TEST_ASSERT_EQUAL_CHAR('t', selem);

    selem = (char)wavelet_at(str_wavelet, 7);
    TEST_ASSERT_EQUAL_CHAR('t', selem);

    selem = (char)wavelet_at(str_wavelet, 8);
    TEST_ASSERT_EQUAL_CHAR('r', selem);

    selem = (char)wavelet_at(str_wavelet, 9);
    TEST_ASSERT_EQUAL_CHAR('e', selem);

    selem = (char)wavelet_at(str_wavelet, 10);
    TEST_ASSERT_EQUAL_CHAR('e', selem);

    u32 srank = 0;

    srank = wavelet_rank(str_wavelet, 0, len - 1, 'e');
    TEST_ASSERT_EQUAL_INT(4, srank);

    srank = wavelet_rank(str_wavelet, 0, len - 1, 't');
    TEST_ASSERT_EQUAL_INT(2, srank);

    srank = wavelet_rank(str_wavelet, 0, len - 1, 'r');
    TEST_ASSERT_EQUAL_INT(1, srank);

    srank = wavelet_rank(str_wavelet, 0, len - 1, 'w');
    TEST_ASSERT_EQUAL_INT(1, srank);

    srank = wavelet_rank(str_wavelet, 0, len - 1, 'a');
    TEST_ASSERT_EQUAL_INT(1, srank);

    srank = wavelet_rank(str_wavelet, 0, len - 1, 's');
    TEST_ASSERT_EQUAL_INT(0, srank);

    srank = wavelet_rank(str_wavelet, 0, 6, 'e');
    TEST_ASSERT_EQUAL_INT(2, srank);

    srank = wavelet_rank(str_wavelet, 0, 6, 't');
    TEST_ASSERT_EQUAL_INT(1, srank);

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
