//
// Created by dominic on 16/11/16.
//

#include <bits/time.h>
#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include "matrix.h"
#include "matrixFactory.h"
#include "matrixParallel.h"
#include "benchmark.h"

double Benchmark_smoothUntilLimit(int size, double precision, int threads) {
    struct timeval tv_start, tv_end;

    Matrix *matrix1 = MatrixFactory_initRandom(size, size);
    Matrix *matrix2 = MatrixFactory_initRandom(size, size);
    gettimeofday(&tv_start, NULL);
    Matrix *retMat = MatrixParallel_smoothUntilLimit(matrix1, matrix2, precision, threads);
    gettimeofday(&tv_end, NULL);

    double time_spent = (double) (tv_end.tv_usec - tv_start.tv_usec) / 1000000
                        + (double) (tv_end.tv_sec - tv_start.tv_sec);

    unsigned long long parity = Matrix_getParity(retMat);
    unsigned long long crc64 = Matrix_getCRC64(retMat);

    printf("01,%05d,%03d,%f,%f,%016llx,%016llx\n", size, threads, precision, time_spent, parity, crc64);

    Matrix_destroy(matrix1);
    Matrix_destroy(matrix2);
    return time_spent;
}

double Benchmark_smoothUntilLimitPooled(int size, double precision, unsigned int threads) {
    struct timeval tv_start, tv_end;

    Matrix *matrix1 = MatrixFactory_initRandom(size, size);
    Matrix *matrix2 = MatrixFactory_initRandom(size, size);
    gettimeofday(&tv_start, NULL);
    Matrix *retMat = MatrixParallel_smoothUntilLimitPooled(matrix1, matrix2, precision, threads);
    gettimeofday(&tv_end, NULL);
    double time_spent = (double) (tv_end.tv_usec - tv_start.tv_usec) / 1000000
                        + (double) (tv_end.tv_sec - tv_start.tv_sec);

    unsigned long long parity = Matrix_getParity(retMat);
    unsigned long long crc64 = Matrix_getCRC64(retMat);

    printf("02,%05d,%03d,%f,%f,%016llx,%016llx\n", size, threads, precision, time_spent, parity, crc64);

    Matrix_destroy(matrix1);
    Matrix_destroy(matrix2);
    return time_spent;
}

double Benchmark_smoothUntilLimitPooledCut(int size, double precision, unsigned int threads) {
    struct timeval tv_start, tv_end;

    Matrix *matrix1 = MatrixFactory_initRandom(size, size);
    Matrix *matrix2 = MatrixFactory_initRandom(size, size);
    gettimeofday(&tv_start, NULL);
    Matrix *retMat = MatrixParallel_smoothUntilLimitPooledCut(matrix1, matrix2, precision, threads);
    gettimeofday(&tv_end, NULL);

    double time_spent = (double) (tv_end.tv_usec - tv_start.tv_usec) / 1000000
                        + (double) (tv_end.tv_sec - tv_start.tv_sec);

    unsigned long long parity = Matrix_getParity(retMat);
    unsigned long long crc64 = Matrix_getCRC64(retMat);

    printf("03,%05d,%03d,%f,%f,%016llx,%016llx\n", size, threads, precision, time_spent, parity, crc64);

    Matrix_destroy(matrix1);
    Matrix_destroy(matrix2);

    return time_spent;
}

double Benchmark_serial(int size, double precision) {
    struct timeval tv_start, tv_end;

    Matrix *matrix1 = MatrixFactory_initRandom(size, size);
    Matrix *matrix2 = MatrixFactory_initRandom(size, size);
    bool *overLimit = calloc(false, sizeof(bool));
    gettimeofday(&tv_start, NULL);
    Matrix *retMat = Matrix_smoothUntilLimit(matrix1, matrix2, precision, overLimit);
    gettimeofday(&tv_end, NULL);
    free(overLimit);

    double time_spent = (double) (tv_end.tv_usec - tv_start.tv_usec) / 1000000
                        + (double) (tv_end.tv_sec - tv_start.tv_sec);

    unsigned long long parity = Matrix_getParity(retMat);
    unsigned long long crc64 = Matrix_getCRC64(retMat);

    printf("00,%05d,%03d,%f,%f,%016llx,%016llx\n", size, 0, precision, time_spent, parity, crc64);

    Matrix_destroy(matrix1);
    Matrix_destroy(matrix2);
    return time_spent;
}