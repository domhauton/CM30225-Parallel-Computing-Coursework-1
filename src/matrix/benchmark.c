//
// Created by dominic on 16/11/16.
//

#include <bits/time.h>
#include <sys/time.h>
#include <wchar.h>
#include <stdio.h>
#include "matrix.h"
#include "matrixFactory.h"
#include "matrixParallel.h"
#include "benchmark.h"

double Benchmark_smoothUntilLimitBenchmark(int size, double precision, int threads) {
    struct timeval tv_start, tv_end;

    Matrix *matrix1 = MatrixFactory_initRandom(size, size);
    Matrix *matrix2 = MatrixFactory_initRandom(size, size);
    gettimeofday(&tv_start, NULL);
    MatrixParallel_smoothUntilLimit(matrix1, matrix2, precision, threads);
    gettimeofday(&tv_end, NULL);
    Matrix_destroy(matrix1);
    Matrix_destroy(matrix2);

    double time_spent = (double) (tv_end.tv_usec - tv_start.tv_usec) / 1000000
                        + (double) (tv_end.tv_sec - tv_start.tv_sec);
    printf("%05d,%03d,%f,%f\n", size, threads, precision, time_spent);
    return time_spent;
}