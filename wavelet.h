#include <algorithm>
#include <cstdint>
#include <iostream>
#include <string>
#include <vector>

class WaveletTree
{
    using u32 = std::uint32_t;

    u32 lo, hi;
    WaveletTree *left = nullptr, *right = nullptr;
    std::vector<u32> data;

public:
    template<typename b_it, typename e_it>
    WaveletTree(b_it from, e_it to, const u32 x, const u32 y)
        : lo(x),
          hi(y)
    {
        if (lo == hi || from >= to) return;
        u32 mid = (lo + hi) / 2;

        auto f = [mid](u32 x) { return x <= mid; };
        data.reserve(to - from + 1);
        data.push_back(0);
        for (auto it = from; it != to; ++it) {
            data.push_back(data.back() + f(*it));
        }
        auto pivot = std::stable_partition(from, to, f);
        left = new WaveletTree(from, pivot, lo, mid);
        right = new WaveletTree(pivot, to, mid + 1, hi);
    }

    [[nodiscard]] static WaveletTree from_string(const std::string_view source)
    {
        std::vector<u32> vals;
        vals.resize(vals.size());

        for (std::size_t i = 0; i < source.size(); ++i) {
            vals[i] = (unsigned char)source[i];
        }

        WaveletTree new_tree(vals.begin(), vals.end(), 0, 1);

        return new_tree;
    }

    u32 kth(const u32 l, const u32 r, const u32 k)
    {
        if (l > r) return 0;
        if (lo == hi) return lo;
        u32 inLeft = data[r] - data[l - 1];
        u32 lb = data[l - 1];
        u32 rb = data[r];
        if (k <= inLeft) return left->kth(lb + 1, rb, k);
        return right->kth(l - lb, r - rb, k - inLeft);
    }

    u32 leq(const u32 l, const u32 r, const u32 k)
    {
        if (l > r || k < lo) return 0;
        if (hi <= k) return r - l + 1;
        u32 lb = data[l - 1];
        u32 rb = data[r];
        return left->leq(lb + 1, rb, k) + right->leq(l - lb, r - rb, k);
    }

    ~WaveletTree()
    {
        delete left;
        delete right;
    }
};
