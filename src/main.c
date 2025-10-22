#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "wavelet.h"

int main(void)
{
    u32 vals[] = {5, 1, 9, 3, 7, 2, 8, 6, 4, 10};
    size_t n = sizeof(vals) / sizeof(*vals);

    WaveletTree* wavelet = wavelet_from_vec(vals, n);

    u32 kth = 0;

    kth = wavelet_kth(wavelet, 3, 8, 2);
    printf("%d\n", kth);
    kth = wavelet_kth(wavelet, 1, 10, 5);
    printf("%d\n", kth);
    kth = wavelet_kth(wavelet, 4, 7, 3);
    printf("%d\n", kth);

    u32 leq = 0;

    leq = wavelet_leq(wavelet, 3, 8, 5);
    printf("%d\n", leq);
    leq = wavelet_leq(wavelet, 1, 10, 7);
    printf("%d\n", leq);
    leq = wavelet_leq(wavelet, 4, 7, 9);
    printf("%d\n", leq);

    wavelet_destroy(wavelet);
    wavelet = NULL;

    const char* str = "wavelettree";
    size_t len = strlen(str);

    WaveletTree* str_wavelet = wavelet_from_string(str);

    char skth = 0;

    skth = (char)wavelet_kth(str_wavelet, 1, len, 1);
    printf("%c\n", skth);
    skth = (char)wavelet_kth(str_wavelet, 1, len, 3);
    printf("%c\n", skth);
    skth = (char)wavelet_kth(str_wavelet, 3, 8, 2);
    printf("%c\n", skth);

    u32 sleq = 0;

    sleq = wavelet_leq(str_wavelet, 1, len, 'm');
    printf("%d\n", sleq);
    sleq = wavelet_leq(str_wavelet, 2, 9, 't');
    printf("%d\n", sleq);
    sleq = wavelet_leq(str_wavelet, 1, len, 'a');
    printf("%d\n", sleq);

    wavelet_destroy(str_wavelet);
    str_wavelet = NULL;

    return 0;
}
