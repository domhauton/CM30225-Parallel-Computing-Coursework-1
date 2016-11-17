//
// Created by dominic on 16/11/16.
//

#include <bits/time.h>
#include <sys/time.h>
#include <stdio.h>
#include "matrix.h"
#include "matrixFactory.h"
#include "matrixParallel.h"
#include "benchmark.h"

double Benchmark_smoothUntilLimit(int size, double precision, int threads) {
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
    printf("01,%05d,%03d,%f,%f\n", size, threads, precision, time_spent);
    return time_spent;
}

double Benchmark_smoothUntilLimitPooled(int size, double precision, unsigned int threads) {
    struct timeval tv_start, tv_end;

    Matrix *matrix1 = MatrixFactory_initRandom(size, size);
    Matrix *matrix2 = MatrixFactory_initRandom(size, size);
    gettimeofday(&tv_start, NULL);
    MatrixParallel_smoothUntilLimitPooled(matrix1, matrix2, precision, threads);
    gettimeofday(&tv_end, NULL);
    Matrix_destroy(matrix1);
    Matrix_destroy(matrix2);

    double time_spent = (double) (tv_end.tv_usec - tv_start.tv_usec) / 1000000
                        + (double) (tv_end.tv_sec - tv_start.tv_sec);
    printf("02,%05d,%03d,%f,%f\n", size, threads, precision, time_spent);
    return time_spent;
}

double Benchmark_smoothUntilLimitPooledCut(int size, double precision, unsigned int threads) {
    struct timeval tv_start, tv_end;

    Matrix *matrix1 = MatrixFactory_initRandom(size, size);
    Matrix *matrix2 = MatrixFactory_initRandom(size, size);
    gettimeofday(&tv_start, NULL);
    MatrixParallel_smoothUntilLimitPooledCut(matrix1, matrix2, precision, threads);
    gettimeofday(&tv_end, NULL);
    Matrix_destroy(matrix1);
    Matrix_destroy(matrix2);

    double time_spent = (double) (tv_end.tv_usec - tv_start.tv_usec) / 1000000
                        + (double) (tv_end.tv_sec - tv_start.tv_sec);
    printf("03,%05d,%03d,%f,%f\n", size, threads, precision, time_spent);
    return time_spent;
}

double Benchmark_serial(int size, double precision) {
    struct timeval tv_start, tv_end;

    Matrix *matrix1 = MatrixFactory_initRandom(size, size);
    Matrix *matrix2 = MatrixFactory_initRandom(size, size);
    gettimeofday(&tv_start, NULL);
    Matrix_smoothUntilLimit(matrix1, matrix2, precision);
    gettimeofday(&tv_end, NULL);
    Matrix_destroy(matrix1);
    Matrix_destroy(matrix2);

    double time_spent = (double) (tv_end.tv_usec - tv_start.tv_usec) / 1000000
                        + (double) (tv_end.tv_sec - tv_start.tv_sec);
    printf("00,%05d,%03d,%f,%f\n", size, 0, precision, time_spent);
    return time_spent;
}