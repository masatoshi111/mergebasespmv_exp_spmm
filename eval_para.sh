#!/bin/bash
sleep 60
if (( $# != 2 )); then
  echo "$0 <mtx data path> <cpu_spmv | gpu_spmv [--device=...]>"
  exit 0
fi

echo "threads, file, num_rows, num_cols, num_nonzeros, row_length_mean, row_length_std_dev, row_length_variation, row_length_skewness, method_name, setup_ms, avg_spmv_ms, gflops, effective_GBs"

MTX_DIR=$1

shift

for i in `find $MTX_DIR -name *.mtx`
do
# for j in `seq 1 1 12`
for j in 1 2 4 6 8 10 12 14 16 17 18
do
    printf "%d, " $j
    ./$@ --quiet --mtx=$i --threads=$j --num_vectors=32
done
done