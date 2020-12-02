#!/bin/bash

if (( $# != 2 )); then
  echo "$0 <mtx data path> <cpu_spmv | gpu_spmv [--device=...]>"
  exit 0
fi

echo "num_vectors, file, num_rows, num_cols, num_nonzeros, row_length_mean, row_length_std_dev, row_length_variation, row_length_skewness, method_name, setup_ms, avg_spmv_ms, gflops, effective_GBs"

MTX_PATH=$1

shift

for i in `seq 200 200 3000`
do
    printf "%d, " $i
    ./$@ --quiet --mtx=$MTX_PATH --num_vectors=$i
done
