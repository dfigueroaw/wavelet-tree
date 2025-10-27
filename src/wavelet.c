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

WaveletTree* wavelet_from_vec(const u32* const restrict vals, const size_t n)
{
    u32 max = 0;
    u32 min = UINT32_MAX;

    for (size_t i = 0; i < n; ++i) {
        if (max < vals[i]) max = vals[i];
        if (min > vals[i]) min = vals[i];
    }

    WaveletTree* const wavelet = malloc(sizeof(*wavelet));
    if (wavelet == NULL) return NULL;

    *wavelet = (WaveletTree){
        .left = NULL,
        .right = NULL,
        .low = min,
        .high = max,
        .data = calloc(n + 1, sizeof(*wavelet->data)),
    };

    if (wavelet->data == NULL) {
        free(wavelet);
        return NULL;
    }

    if (min == max || n == 0) return wavelet;

    const u32 mid = (min + max) / 2;

    for (size_t i = 0; i < n; ++i)
        wavelet->data[i + 1] = wavelet->data[i] + (vals[i] <= mid);

    u32* const temp = calloc(n, sizeof(*temp));
    if (temp == NULL) {
        wavelet_destroy(wavelet);
        return NULL;
    }

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

WaveletTree* wavelet_from_string(const char* const restrict str)
{
    const size_t n = strlen(str);

    u32* const vals = calloc(n, sizeof(*vals));
    if (vals == NULL) return NULL;

    for (size_t i = 0; i < n; ++i)
        vals[i] = (u32)str[i];

    WaveletTree* const wavelet = wavelet_from_vec(vals, n);
    free(vals);

    return wavelet;
}

void wavelet_destroy(WaveletTree* const restrict wavelet)
{
    if (wavelet == NULL) return;

    wavelet_destroy(wavelet->left);
    wavelet_destroy(wavelet->right);

    free(wavelet->data);
    free(wavelet);
}

u32 wavelet_at(const WaveletTree* const restrict wavelet, const size_t i)
{
    if (wavelet->low == wavelet->high) return wavelet->low;

    const u32 in_left = wavelet->data[i + 1] - wavelet->data[i];
    const u32 rank = wavelet->data[i];

    if (in_left) return wavelet_at(wavelet->left, rank);
    return wavelet_at(wavelet->right, i - rank);
}

u32 wavelet_rank(const WaveletTree* const restrict wavelet, const u32 l, const u32 r, const u32 k)
{
    if (l > r) return 0;
    if (k < wavelet->low || k > wavelet->high) return 0;
    if (wavelet->low == wavelet->high) return r - l + 1;

    const u32 mid = (wavelet->low + wavelet->high) / 2;
    const u32 lb = wavelet->data[l];
    const u32 rb = wavelet->data[r + 1];

    if (k <= mid) return wavelet_rank(wavelet->left, lb, rb - 1, k);
    return wavelet_rank(wavelet->right, l - lb, r - rb, k);
}

u32 wavelet_kth_smallest(const WaveletTree* const restrict wavelet,
                         const u32 l,
                         const u32 r,
                         const u32 k)
{
    if (l > r) return 0;
    if (wavelet->low == wavelet->high) return wavelet->low;

    const u32 in_left = wavelet->data[r + 1] - wavelet->data[l];
    const u32 lb = wavelet->data[l];
    const u32 rb = wavelet->data[r + 1];

    if (k <= in_left) return wavelet_kth_smallest(wavelet->left, lb, rb - 1, k);
    return wavelet_kth_smallest(wavelet->right, l - lb, r - rb, k - in_left);
}

u32 wavelet_leq(const WaveletTree* const restrict wavelet, const u32 l, const u32 r, const u32 k)
{
    if (l > r || wavelet->low > k) return 0;
    if (wavelet->high <= k) return r - l + 1;

    const u32 lb = wavelet->data[l];
    const u32 rb = wavelet->data[r + 1];

    return wavelet_leq(wavelet->left, lb, rb - 1, k) +
           wavelet_leq(wavelet->right, l - lb, r - rb, k);
}
