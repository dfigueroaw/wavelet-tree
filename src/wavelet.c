#include "wavelet.h"
#include <stdlib.h>
#include <string.h>

void wavelet_tree_from_vec(WaveletTree* wavelet, const u32* restrict vals, const size_t n)
{
    wavelet->left = NULL;
    wavelet->right = NULL;

    u32 max = 0;
    u32 min = UINT32_MAX;

    for (size_t i = 0; i < n; ++i) {
        if (max < vals[i]) max = vals[i];
        if (min > vals[i]) min = vals[i];
    }

    wavelet->low = min;
    wavelet->high = max;
    wavelet->data = calloc(n + 1, sizeof(*wavelet->data));

    if (min == max || n == 0) return;
    u32 mid = (min + max) / 2;

    for (size_t i = 0; i < n; ++i) {
        wavelet->data[i + 1] = wavelet->data[i] + (vals[i] <= mid);
    }

    u32* temp = calloc(n, sizeof(*temp));

    size_t left_cnt = 0;
    for (size_t i = 0; i < n; ++i) {
        if (vals[i] <= mid) temp[left_cnt++] = vals[i];
    }

    size_t right_cnt = 0;
    for (size_t i = 0; i < n; ++i) {
        if (vals[i] > mid) temp[left_cnt + right_cnt++] = vals[i];
    }

    wavelet->left = malloc(sizeof(WaveletTree));
    wavelet_tree_from_vec(wavelet->left, temp, left_cnt);

    wavelet->right = malloc(sizeof(WaveletTree));
    wavelet_tree_from_vec(wavelet->right, temp + left_cnt, right_cnt);

    free(temp);
}

void wavelet_tree_from_string(WaveletTree* wavelet, const char* restrict str)
{
    size_t n = strlen(str);
    u32* vals = calloc(n, sizeof(*vals));

    for (size_t i = 0; i < n; ++i) {
        vals[i] = (u32)str[i];
    }

    wavelet_tree_from_vec(wavelet, vals, n);
    free(vals);
}

void wavelet_tree_free(WaveletTree* wavelet)
{
    if (wavelet == NULL) return;

    wavelet_tree_free(wavelet->left);
    wavelet_tree_free(wavelet->right);

    free(wavelet->data);
    free(wavelet);
}

u32 wavelet_tree_kth(const WaveletTree* wavelet, const u32 l, const u32 r, const u32 k)
{
    if (l > r) return 0;
    if (wavelet->low == wavelet->high) return wavelet->low;

    u32 inLeft = wavelet->data[r] - wavelet->data[l - 1];
    u32 lb = wavelet->data[l - 1];
    u32 rb = wavelet->data[r];

    if (k <= inLeft) return wavelet_tree_kth(wavelet->left, lb + 1, rb, k);
    return wavelet_tree_kth(wavelet->right, l - lb, r - rb, k - inLeft);
}

u32 wavelet_tree_leq(const WaveletTree* wavelet, const u32 l, const u32 r, const u32 k)
{
    if (l > r || wavelet->low > k) return 0;
    if (wavelet->high <= k) return r - l + 1;

    u32 lb = wavelet->data[l - 1];
    u32 rb = wavelet->data[r];

    return wavelet_tree_leq(wavelet->left, lb + 1, rb, k) +
           wavelet_tree_leq(wavelet->right, l - lb, r - rb, k);
}
