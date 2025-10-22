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

void wavelet_tree_from_vec(WaveletTree* wavelet, const u32* restrict vals, size_t n);
void wavelet_tree_from_string(WaveletTree* wavelet, const char* restrict str);
void wavelet_tree_free(WaveletTree* wavelet);

u32 wavelet_tree_kth(const WaveletTree* wavelet, u32 l, u32 r, u32 k);
u32 wavelet_tree_leq(const WaveletTree* wavelet, u32 l, u32 r, u32 k);

#endif
