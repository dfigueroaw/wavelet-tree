#ifndef WAVELET_TREE_H
#define WAVELET_TREE_H

#include <algorithm>
#include <cstdint>
#include <vector>

namespace
{

struct WaveletTree {
  using u32 = std::uint32_t;

  u32 lo, hi;
  WaveletTree *left = nullptr, *right = nullptr;
  std::vector<u32> b;

  WaveletTree(u32 *const from, u32 *const to, const u32 x, const u32 y)
      : lo(x), hi(y)
  {
    if (lo == hi || from >= to) return;
    u32 mid = (lo + hi) / 2;

    auto f = [mid](u32 x) { return x <= mid; };
    b.reserve(to - from + 1);
    b.push_back(0);
    for (auto it = from; it != to; ++it)
      b.push_back(b.back() + f(*it));

    auto pivot = std::stable_partition(from, to, f);
    left = new WaveletTree(from, pivot, lo, mid);
    right = new WaveletTree(pivot, to, mid + 1, hi);
  }

  ~WaveletTree()
  {
    delete left;
    delete right;
  }
};

}; // namespace

#endif
