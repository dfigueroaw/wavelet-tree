#!/usr/bin/env bash
SIZE="$1"
H_TARGET="$2"
OUT="$3"

P=$(awk -v H="$H_TARGET" 'BEGIN{printf "%f", H/8.0}')
RANDOM_BYTES=$(printf "%.0f" "$(awk -v S="$SIZE" -v P="$P" 'BEGIN{printf S*P}')")
ZEROS_BYTES=$((SIZE - RANDOM_BYTES))

dd if=/dev/urandom bs=1 count="$RANDOM_BYTES" status=none >"$OUT"
dd if=/dev/zero bs=1 count="$ZEROS_BYTES" status=none >>"$OUT"
