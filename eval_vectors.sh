#!/bin/bash

if (( $# != 2 )); then
  echo "$0 <mtx data path> <cpu_spmv | gpu_spmv [--device=...]>"
  exit 0
fi

echo "num_vectors, file, num_rows, num_cols, num_nonzeros, row_length_mean, row_length_std_dev, row_length_variation, row_length_skewness, method_name, setup_ms, avg_spmv_ms, gflops, effective_GBs"

MTX_DIR=$1

shift

for i in `find $MTX_DIR -name *.mtx`
do
# for j in `seq 1 1 12`
for j in 1 2 4 8 16 32 64 128 256 512 1024 2048 
do
    printf "%d, " $j
    ./$@ --quiet --mtx=$i --num_vectors=$j --threads=18
done
done