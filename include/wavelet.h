#ifndef WAVELET_H
#define WAVELET_H

#include <stddef.h>
#include "types.h"

typedef struct WaveletTree {
    u32 low, high;
    u32* data;
    struct WaveletTree* left;
    struct WaveletTree* right;
} WaveletTree;

void wavelet_tree_from_vec(WaveletTree* self, const u32* restrict vals, size_t n);
void wavelet_tree_from_string(WaveletTree* self, const char* restrict str);
void wavelet_tree_free(WaveletTree* self);

[[nodiscard]] u32 wavelet_tree_kth(const WaveletTree* self, u32 l, u32 r, u32 k);
[[nodiscard]] u32 wavelet_tree_leq(const WaveletTree* self, u32 l, u32 r, u32 k);

#endif
