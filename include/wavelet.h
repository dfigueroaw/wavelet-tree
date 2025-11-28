#ifndef WAVELET_H
#define WAVELET_H

#include <stddef.h>
#include "types.h"

typedef struct WaveletTree WaveletTree;

WaveletTree *wavelet_from_vec(const u32 *restrict vals, size_t n);

WaveletTree *wavelet_from_string(const char *restrict str);

void wavelet_destroy(WaveletTree *restrict wavelet);

u32 wavelet_at(const WaveletTree *restrict wavelet, size_t i);

u32 wavelet_rank(const WaveletTree *restrict wavelet, u32 l, u32 r, u32 k);

u32 wavelet_kth_smallest(const WaveletTree *restrict wavelet, u32 l, u32 r,
			 u32 k);

u32 wavelet_leq(const WaveletTree *restrict wavelet, u32 l, u32 r, u32 k);

double wavelet_entropy(const WaveletTree *restrict wavelet);

#endif
