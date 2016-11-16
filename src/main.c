#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "matrix/matrix.h"
#include "matrix/matrixFactory.h"
#include "matrix/matrixSmoother.h"
#include "matrix/matrixParallel.h"

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
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("Took %f seconds.\n", time_spent);
    //Matrix_print(tmp);
    Matrix_destroy(matrix);
    Matrix_destroy(matrix2);
}

void matrixMultiIteratorOverwriteTest() {
    Matrix *matrix = MatrixFactory_initEmpty(7, 7);
    MatIterator* matIterator1 = Matrix_getIterator(matrix, 1, 1, 5, 5);
    MatIterator* matIterator2 = MatIterator_split(matIterator1);
    MatIterator* matIterator3 = MatIterator_split(matIterator2);
    while(MatIterator_hasNext(matIterator1)) {
        *MatIterator_next(matIterator1) = 1.111111;
    }
    while(MatIterator_hasNext(matIterator2)) {
        *MatIterator_next(matIterator2) = 2.222222;
    }
    while(MatIterator_hasNext(matIterator3)) {
        *MatIterator_next(matIterator3) = 3.333333;
    }
    MatIterator_destroy(matIterator1);
    MatIterator_destroy(matIterator2);
    MatIterator_destroy(matIterator3);
    Matrix_print(matrix);
    Matrix_destroy(matrix);
}

void matrixSmoothAndCompare(int size, double precision) {
    //Prep
    Matrix *matrix1 = MatrixFactory_initRandom(size, size);
    Matrix *matrix2 = MatrixFactory_initRandom(size, size);
    Matrix *matrix3 = MatrixFactory_initRandom(size, size);
    Matrix *matrix4 = MatrixFactory_initRandom(size, size);
    //Compute 1
    clock_t begin1 = clock();
    Matrix *result1 = Matrix_smoothUntilLimit(matrix1, matrix2, precision);
    clock_t end1 = clock();
    double time_spent1 = (double)(end1 - begin1) / CLOCKS_PER_SEC;
    //Compute 2
    clock_t begin2 = clock();
    Matrix *result2 = MatrixParallel_smoothUntilLimit(matrix3, matrix4, precision);
    clock_t end2 = clock();
    double time_spent2 = (double)(end2 - begin2) / CLOCKS_PER_SEC;
    //Print
    printf("Size %d - 1 Thread  - Took %f seconds.\n", size, time_spent1);
    printf("Size %d - 2 Threads - Took %f seconds.\n", size, time_spent2);
    if(Matrix_equals(result1, result2)) {
        printf("Matrices same\n");
    } else {
        printf("Matrices not same\n");
    }
    Matrix_destroy(matrix1);
    Matrix_destroy(matrix2);
    Matrix_destroy(matrix3);
    Matrix_destroy(matrix4);
}

void matrixCompareRegeneration(int size) {
    Matrix *matrix1 = MatrixFactory_initRandom(size, size);
    Matrix *matrix2 = MatrixFactory_initRandom(size, size);
    if(Matrix_equals(matrix1, matrix2)) {
        printf("Matrices same\n");
    } else {
        printf("Matrices not same\n");
    }

    MatIterator* matIterator = Matrix_getIterator(matrix1, 0, 0, 1, 1);
    (*MatIterator_next(matIterator))++;
    MatIterator_destroy(matIterator);

    if(Matrix_equals(matrix1, matrix2)) {
        printf("Matrices same\n");
    } else {
        printf("Matrices not same\n");
    }

    Matrix_destroy(matrix1);
    Matrix_destroy(matrix2);
}

int main() {
    matrixSmoothAndCompare(8,    0.001f);
    matrixSmoothAndCompare(16,   0.001f);
    matrixSmoothAndCompare(32,   0.001f);
    matrixSmoothAndCompare(64,   0.001f);
    matrixSmoothAndCompare(128,  0.001f);
    matrixSmoothAndCompare(256,  0.001f);
    matrixSmoothAndCompare(512,  0.001f);
    matrixSmoothAndCompare(1024, 0.001f);
    matrixSmoothAndCompare(2048, 0.001f);
    matrixSmoothAndCompare(4096, 0.001f);
    matrixSmoothAndCompare(8192, 0.001f);
    return 0;
}




