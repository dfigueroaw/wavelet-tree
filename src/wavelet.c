#include "wavelet.h"
#include <stdlib.h>
#include <string.h>

void wavelet_tree_from_vec(WaveletTree* self, const u32* restrict vals, const size_t n)
{
    self->left = NULL;
    self->right = NULL;

    u32 max = 0;
    u32 min = UINT32_MAX;

    for (size_t i = 0; i < n; ++i) {
        if (max < vals[i]) max = vals[i];
        if (min > vals[i]) min = vals[i];
    }

    self->low = min;
    self->high = max;
    self->data = calloc(n + 1, sizeof(*self->data));

    if (min == max || n == 0) return;
    u32 mid = (min + max) / 2;

    for (size_t i = 0; i < n; ++i) {
        self->data[i + 1] = self->data[i] + (vals[i] <= mid);
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

    self->left = malloc(sizeof(WaveletTree));
    wavelet_tree_from_vec(self->left, temp, left_cnt);

    self->right = malloc(sizeof(WaveletTree));
    wavelet_tree_from_vec(self->right, temp + left_cnt, right_cnt);

    free(temp);
}

void wavelet_tree_from_string(WaveletTree* self, const char* restrict str)
{
    size_t n = strlen(str);
    u32* vals = calloc(n, sizeof(*vals));

    for (size_t i = 0; i < n; ++i) {
        vals[i] = (u32)str[i];
    }

    wavelet_tree_from_vec(self, vals, n);
    free(vals);
}

void wavelet_tree_free(WaveletTree* self)
{
    if (self == NULL) return;

    wavelet_tree_free(self->left);
    wavelet_tree_free(self->right);

    free(self->data);
    free(self);
}

u32 wavelet_tree_kth(const WaveletTree* self, const u32 l, const u32 r, const u32 k)
{
    if (l > r) return 0;
    if (self->low == self->high) return self->low;

    u32 inLeft = self->data[r] - self->data[l - 1];
    u32 lb = self->data[l - 1];
    u32 rb = self->data[r];

    if (k <= inLeft) return wavelet_tree_kth(self->left, lb + 1, rb, k);
    return wavelet_tree_kth(self->right, l - lb, r - rb, k - inLeft);
}

u32 wavelet_tree_leq(const WaveletTree* self, const u32 l, const u32 r, const u32 k)
{
    if (l > r || self->low > k) return 0;
    if (self->high <= k) return r - l + 1;

    u32 lb = self->data[l - 1];
    u32 rb = self->data[r];

    return wavelet_tree_leq(self->left, lb + 1, rb, k) +
           wavelet_tree_leq(self->right, l - lb, r - rb, k);
}
