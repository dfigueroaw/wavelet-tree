#ifndef WAVELET_H
#define WAVELET_H

#include <stddef.h>
#include "types.h"

typedef struct WaveletTree WaveletTree;

WaveletTree* wavelet_from_vec(const u32* restrict vals, size_t n);

WaveletTree* wavelet_from_string(const char* restrict str);

void wavelet_destroy(WaveletTree* wavelet);

u32 wavelet_at(const WaveletTree* wavelet, size_t i);

u32 wavelet_kth(const WaveletTree* wavelet, u32 l, u32 r, u32 k);

u32 wavelet_leq(const WaveletTree* wavelet, u32 l, u32 r, u32 k);

#endif
