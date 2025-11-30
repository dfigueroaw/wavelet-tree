#!/usr/bin/env python

from pathlib import Path

import matplotlib.pyplot as plt
import numpy as np

OUTDIR = Path(__file__).resolve().parent.parent.parent / "figs"
OUTDIR.mkdir(exist_ok=True)

queries = np.array([1e6, 1e7, 1e8])
bench_at = {
    "str_1": np.array([8.73, 47.76, 429.66]),
    "str_2": np.array([9.20, 45.24, 471.70]),
    "str_3": np.array([9.25, 45.19, 460.87]),
}
bench_rank = {
    "str_1": np.array([10.34, 72.87, 526.43]),
    "str_2": np.array([12.29, 75.32, 656.41]),
    "str_3": np.array([12.30, 63.15, 649.74]),
}
bench_kth_smallest = {
    "str_1": np.array([5.27, 44.92, 244.11]),
    "str_2": np.array([6.10, 29.50, 323.13]),
    "str_3": np.array([6.35, 31.19, 328.41]),
}
bench_leq = {
    "str_1": np.array([9.59, 96.21, 963.03]),
    "str_2": np.array([11.11, 113.15, 1128.18]),
    "str_3": np.array([10.92, 109.74, 1111.57]),
}

compressors = ["gzip", "bzip2", "xz", "zstd"]
sizes = ["1MB", "10MB", "50MB", "100MB"]

overhead_high = {
    "gzip": [1.000203, 1.000158, 1.000154, 1.000160],
    "bzip2": [1.004679, 1.004457, 1.004443, 1.004467],
    "xz": [1.000135, 1.000058, 1.000049, 1.000048],
    "zstd": [1.000056, 1.000026, 1.000024, 1.000023],
}

overhead_medium = {
    "gzip": [0.804315, 0.804080, 0.803883, 0.803875],
    "bzip2": [0.807534, 0.806539, 0.806544, 0.806451],
    "xz": [0.804128, 0.803202, 0.803140, 0.803126],
    "zstd": [0.802984, 0.803004, 0.803013, 0.803001],
}

overhead_low = {
    "gzip": [0.655824, 0.655249, 0.655022, 0.654955],
    "bzip2": [0.655774, 0.653732, 0.653229, 0.653230],
    "xz": [0.652558, 0.651467, 0.651086, 0.651109],
    "zstd": [0.650791, 0.650613, 0.650503, 0.650532],
}


def save_fig(fig, fname, dpi=200):
    path = OUTDIR / fname
    fig.tight_layout()
    fig.savefig(path, dpi=dpi)
    print("Saved:", path)


def plot_time_per_operation(queries, bench_dict, opname):
    fig, ax = plt.subplots(figsize=(7, 4))

    for k, arr in bench_dict.items():
        ax.plot(queries, arr, marker="o", label=k)

    ax.set_xscale("log")
    ax.set_xlabel("# de consultas")
    ax.set_ylabel("Tiempo total (ms)")
    ax.set_title(f"Benchmarks — wavelet_{opname}")
    ax.grid(True, which="both", ls="--", alpha=0.3)
    ax.legend()
    save_fig(fig, f"queries_vs_time_{opname}.png")


def plot_compressors_overhead_per_size(en, es, overhead_dict):
    fig, ax = plt.subplots(figsize=(8, 4))

    for comp in compressors:
        ax.plot(sizes, overhead_dict[comp], marker="o", label=f"{comp}")

    ax.set_xlabel("Tamaño del archivo")
    ax.set_ylabel("Overhead")
    ax.set_title(f"Overhead compresores — Entropía {es}")
    ax.grid(True)
    ax.legend()
    save_fig(fig, f"overhead_{en}.png")


plot_time_per_operation(queries, bench_at, "at")
plot_time_per_operation(queries, bench_rank, "rank")
plot_time_per_operation(queries, bench_kth_smallest, "kth_smallest")
plot_time_per_operation(queries, bench_leq, "leq")

plot_compressors_overhead_per_size("high", "alta", overhead_high)
plot_compressors_overhead_per_size("medium", "media", overhead_medium)
plot_compressors_overhead_per_size("low", "baja", overhead_low)
