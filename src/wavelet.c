#include "wavelet.h"
#include <stdlib.h>
#include <string.h>

struct WaveletTree {
    u32 low;
    u32 high;
    u32* data;
    WaveletTree* left;
    WaveletTree* right;
};

WaveletTree* wavelet_from_vec(const u32* restrict vals, const size_t n)
{
    u32 max = 0;
    u32 min = UINT32_MAX;

    for (size_t i = 0; i < n; ++i) {
        if (max < vals[i]) max = vals[i];
        if (min > vals[i]) min = vals[i];
    }

    WaveletTree* wavelet = malloc(sizeof(*wavelet));
    wavelet->left = NULL;
    wavelet->right = NULL;

    wavelet->low = min;
    wavelet->high = max;
    wavelet->data = calloc(n + 1, sizeof(*wavelet->data));

    if (min == max || n == 0) return wavelet;

    u32 mid = (min + max) / 2;

    for (size_t i = 0; i < n; ++i)
        wavelet->data[i + 1] = wavelet->data[i] + (vals[i] <= mid);

    u32* temp = calloc(n, sizeof(*temp));

    size_t left_cnt = 0;
    for (size_t i = 0; i < n; ++i) {
        if (vals[i] <= mid) {
            temp[left_cnt] = vals[i];
            ++left_cnt;
        }
    }

    size_t right_cnt = 0;
    for (size_t i = 0; i < n; ++i) {
        if (vals[i] > mid) {
            temp[left_cnt + right_cnt] = vals[i];
            ++right_cnt;
        }
    }

    wavelet->left = wavelet_from_vec(temp, left_cnt);
    wavelet->right = wavelet_from_vec(temp + left_cnt, right_cnt);

    free(temp);
    return wavelet;
}

WaveletTree* wavelet_from_string(const char* restrict str)
{
    size_t n = strlen(str);
    u32* vals = calloc(n, sizeof(*vals));

    for (size_t i = 0; i < n; ++i)
        vals[i] = (u32)str[i];

    WaveletTree* wavelet = wavelet_from_vec(vals, n);
    free(vals);

    return wavelet;
}

void wavelet_destroy(WaveletTree* wavelet)
{
    if (wavelet == NULL) return;

    wavelet_destroy(wavelet->left);
    wavelet_destroy(wavelet->right);

    free(wavelet->data);
    free(wavelet);
}

u32 wavelet_at(const WaveletTree* wavelet, size_t i)
{
    if (wavelet->low == wavelet->high) return wavelet->low;

    u32 inLeft = wavelet->data[i + 1] - wavelet->data[i];
    u32 rank = wavelet->data[i + 1];

    if (inLeft) return wavelet_at(wavelet->left, rank - 1);
    return wavelet_at(wavelet->right, i - rank);
}

u32 wavelet_kth(const WaveletTree* wavelet, const u32 l, const u32 r, const u32 k)
{
    if (l > r) return 0;
    if (wavelet->low == wavelet->high) return wavelet->low;

    u32 inLeft = wavelet->data[r + 1] - wavelet->data[l];
    u32 lb = wavelet->data[l];
    u32 rb = wavelet->data[r + 1];

    if (k <= inLeft) return wavelet_kth(wavelet->left, lb, rb - 1, k);
    return wavelet_kth(wavelet->right, l - lb, r - rb, k - inLeft);
}

u32 wavelet_leq(const WaveletTree* wavelet, const u32 l, const u32 r, const u32 k)
{
    if (l > r || wavelet->low > k) return 0;
    if (wavelet->high <= k) return r - l + 1;

    u32 lb = wavelet->data[l];
    u32 rb = wavelet->data[r + 1];

    return wavelet_leq(wavelet->left, lb, rb - 1, k) +
           wavelet_leq(wavelet->right, l - lb, r - rb, k);
}
