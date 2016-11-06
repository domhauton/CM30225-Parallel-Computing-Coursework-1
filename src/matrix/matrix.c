//
// Created by dominic on 31/10/16.
//

#include <stdlib.h>
#include <stdio.h>
#include "matrix.h"

typedef struct matrix {
    long xSize, ySize;
    double *data;
} Matrix;

Matrix *Matrix_init() {
    return malloc(sizeof(Matrix));
}

void Matrix_set(Matrix *matrix, double *values, long xSize, long ySize) {
    matrix->data = values;
    matrix->xSize = xSize;
    matrix->ySize = ySize;
}

Matrix *Matrix_initSet(double *values, long xSize, long ySize) {
    Matrix *matrix = Matrix_init();
    Matrix_set(matrix, values, xSize, ySize);
    return matrix;
}

double *Matrix_getDataPtr(Matrix *matrix, long x, long y) {
    return matrix->data + x + (matrix->xSize * y);
}

MatIterator *Matrix_getIterator(Matrix *matrix, long x, long y, long width, long height) {
    double *initialDataPtr = Matrix_getDataPtr(matrix, x, y);
    return MatIterator_init(initialDataPtr, matrix->xSize, width, height);
}

MatEdgeIterator *Matrix_getEdgeIterator(Matrix *matrix) {
    return MatEdgeIterator_init(matrix->data, matrix->xSize, matrix->ySize);
}

void Matrix_copyEdge(Matrix* source, Matrix* target) {
    MatEdgeIterator *edgeIterator1 = Matrix_getEdgeIterator(source);
    MatEdgeIterator *edgeIterator2 = Matrix_getEdgeIterator(target);
    while(MatEdgeIterator_hasNext(edgeIterator1)) {
        *MatEdgeIterator_next(edgeIterator2) = *MatEdgeIterator_next(edgeIterator1);
    }
    MatEdgeIterator_destroy(edgeIterator1);
    MatEdgeIterator_destroy(edgeIterator2);
}

void Matrix_print(Matrix *matrix) {
    long size = matrix->xSize * matrix->ySize;
    double *ptr = matrix->data;
    for (long i = 1; i <= size; i++) {
        if (i % matrix->xSize != 0) {
            printf("%f, ", *ptr++);
        } else {
            printf("%f\n", *ptr++);
        }
    }
}

void Matrix_destroy(Matrix *matrix) {
    free(matrix->data);
    free(matrix);
}
