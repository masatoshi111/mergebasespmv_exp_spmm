#include <omp.h>

#include <stdio.h>
#include <vector>
#include <algorithm>
#include <cstdio>
#include <fstream>
#include <sstream>
#include <iostream>
#include <limits>
#include <numa.h>

#include <mkl.h>

#include "sparse_matrix.h"
#include "utils.h"

inline void axpy_(int size, double a, double* vector_x, double* vector_y){
    for(int i=0; i<size; i++){
        vector_y[i] += a * vector_x[i];
    }
}

inline void axpy_2(int size, double a, double* vector_x, double* vector_y){
    while( size-- ){
        *(vector_y++) = a * *(vector_x++);
    }
}

inline void* copy(int size, double a, double* vector_x, double* vector_y){
    const double* x = (const double*) vector_x;
    double*       y = (double*)       vector_y;
    while( size-- ){ *(y++) = *(x++); }
    return vector_x;
}

void test(int size){
    size_t iter = 1e8 / size;
    int flops = size*2;
    double *vector_x, *vector_y;
    vector_x = (double*) numa_alloc_onnode(sizeof(double) * size, 0);
    vector_y = (double*) numa_alloc_onnode(sizeof(double) * size*2, 0);
    for (int i=0; i<size; i++){
        vector_x[i] = i % 14;
    }

    for (size_t i=0; i<100; i++){
        cblas_daxpy(size, 2.0, vector_x, 1, vector_y, 1);
    }

    float elapsed_ms = 0.0;
    CpuTimer timer;
    timer.Start();
    for (size_t i=0; i<iter; i++){
        cblas_daxpy(size, 2.0, vector_x, 1, vector_y, 1);
    }
    timer.Stop();
    elapsed_ms += timer.ElapsedMillis();

    printf("MKL axpy : %lf Gflop/s\n", flops*10e-6/(elapsed_ms/iter));

    for (size_t i=0; i<iter; i++){
        // copy(size, 2.0, vector_x, vector_y);
        memcpy(vector_y, vector_x, size);
    }

    elapsed_ms = 0.0;
    CpuTimer timer2;
    timer2.Start();
    for (size_t i=0; i<iter; i++){
        copy(size, 2.0, vector_x, vector_y);
    }
    timer2.Stop();
    elapsed_ms += timer2.ElapsedMillis();
    printf("copy : %lf Gflop/s\n", flops*10e-6/(elapsed_ms/iter));

    for (size_t i=0; i<iter; i++){
        axpy_2(size, 2.0, vector_x, vector_y);
    }

    elapsed_ms = 0.0;
    CpuTimer timer3;
    timer3.Start();
    for (size_t i=0; i<iter; i++){
        axpy_2(size, 2.0, vector_x, vector_y);
    }
    timer3.Stop();
    elapsed_ms += timer3.ElapsedMillis();
    printf("axpy_2 : %lf Gflop/s\n", flops*10e-6/(elapsed_ms/iter));
    
}

int main(){
    int size_list[4] = {15, 16, 30, 32};
    for(int i=2; i<18; i+=1){
        int size = std::pow(2,i);
        printf("%d\n", size);
        test(size);
    }
}