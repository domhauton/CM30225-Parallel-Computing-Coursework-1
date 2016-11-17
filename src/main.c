#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include "matrix/matrix.h"
#include "matrix/matrixFactory.h"
#include "matrix/benchmark.h"

void matrixComputeTest() {
    Matrix *matrix = MatrixFactory_initRandom(1000, 1000);
    Matrix *matrix2 = MatrixFactory_initEmpty(1000, 1000);
    //Matrix_print(matrix);
    printf("\n");
    //Matrix_print(matrix2);
    Matrix_copyEdge(matrix, matrix2);
    printf("\n");
    //Matrix_print(matrix2);
    printf("\n");
    clock_t begin = clock();
    Matrix *tmp = Matrix_smoothUntilLimit(matrix, matrix2, 0.01f);
    clock_t end = clock();
    double time_spent = (double) (end - begin) / CLOCKS_PER_SEC;
    printf("Took %f seconds.\n", time_spent);
    //Matrix_print(tmp);
    Matrix_destroy(matrix);
    Matrix_destroy(matrix2);
}

void matrixMultiIteratorOverwriteTest() {
    Matrix *matrix = MatrixFactory_initEmpty(7, 7);
    MatIterator *matIterator1 = Matrix_getIterator(matrix, 1, 1, 5, 5);
    MatIterator *matIterator2 = MatIterator_split(matIterator1);
    MatIterator *matIterator3 = MatIterator_split(matIterator2);
    while (MatIterator_hasNext(matIterator1)) {
        *MatIterator_next(matIterator1) = 1.111111;
    }
    while (MatIterator_hasNext(matIterator2)) {
        *MatIterator_next(matIterator2) = 2.222222;
    }
    while (MatIterator_hasNext(matIterator3)) {
        *MatIterator_next(matIterator3) = 3.333333;
    }
    MatIterator_destroy(matIterator1);
    MatIterator_destroy(matIterator2);
    MatIterator_destroy(matIterator3);
    Matrix_print(matrix);
    Matrix_destroy(matrix);
}


void matrixCompareRegeneration(int size) {
    Matrix *matrix1 = MatrixFactory_initRandom(size, size);
    Matrix *matrix2 = MatrixFactory_initRandom(size, size);
    if (Matrix_equals(matrix1, matrix2)) {
        printf("Matrices same\n");
    } else {
        printf("Matrices not same\n");
    }

    MatIterator *matIterator = Matrix_getIterator(matrix1, 0, 0, 1, 1);
    (*MatIterator_next(matIterator))++;
    MatIterator_destroy(matIterator);

    if (Matrix_equals(matrix1, matrix2)) {
        printf("Matrices same\n");
    } else {
        printf("Matrices not same\n");
    }

    Matrix_destroy(matrix1);
    Matrix_destroy(matrix2);
}

int main(int argc, char *argv[]) {

    if (argc == 4) {
        unsigned int threads = (unsigned int) atoi(argv[1]);
        int size = atoi(argv[2]);
        double precision = strtof(argv[3], NULL);
        Benchmark_smoothUntilLimitPooledCut(size, precision, threads);
    } else if (argc == 5) {
        unsigned int threads = (unsigned int) atoi(argv[1]);
        int size = atoi(argv[2]);
        int type = atoi(argv[2]);
        double precision = strtof(argv[3], NULL);
        switch(type) {
            case 1:
                Benchmark_smoothUntilLimit(size, precision, threads);
                break;
            case 2:
                Benchmark_smoothUntilLimitPooled(size, precision, threads);
                break;
            case 3:
                Benchmark_smoothUntilLimitPooledCut(size, precision, threads);
                break;
            default:
                Benchmark_serial(size, precision);
        }
        Benchmark_smoothUntilLimitPooledCut(size, precision, threads);
    } else {
        printf(" Ctr |Ty|Size |Thr|  Acc   |  Time\n");
        double precision = 0.001f;
        for (unsigned int threads = 1; threads <= sysconf(_SC_NPROCESSORS_ONLN); threads<<=1) {
            for (int matrixSize = (2 << 5); matrixSize <= (2 << 7); matrixSize <<= 1) {
                //Benchmark_smoothUntilLimit(matrixSize, precision, threads);
                //Benchmark_smoothUntilLimitPooled(matrixSize, precision, threads);
                Benchmark_smoothUntilLimitPooledCut(matrixSize, precision, threads);
            }
        }
    }
    return 0;
}




