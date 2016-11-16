//
// Created by dominic on 31/10/16.
//

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include "matrix.h"
#include "matrixSmoother.h"

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

MatSmoother *Matrix_getSmoother(Matrix *source, Matrix *target, double limit) {
    long itrWidth = target->xSize - 2;
    long itrHeight = target->ySize - 2;
    MatIterator *resItr = Matrix_getIterator(target, 1, 1, itrWidth, itrHeight);
    MatIterator *ctrItr = Matrix_getIterator(source, 1, 1, itrWidth, itrHeight);
    MatIterator *topItr = Matrix_getIterator(source, 0, 1, itrWidth, itrHeight);
    MatIterator *botItr = Matrix_getIterator(source, 2, 1, itrWidth, itrHeight);
    MatIterator *lftItr = Matrix_getIterator(source, 1, 0, itrWidth, itrHeight);
    MatIterator *rgtItr = Matrix_getIterator(source, 1, 2, itrWidth, itrHeight);
    return MatSmoother_init(resItr, ctrItr, topItr, botItr, lftItr, rgtItr, limit);
}

Matrix *Matrix_smoothUntilLimit(Matrix *source, Matrix *target, double limit) {
    bool smoothDiffExceeded;
    bool resultFlipped = false;
    long ctr = 0;
    Matrix *tmp;
    do {
        MatSmoother *smoother = Matrix_getSmoother(source, target, limit);
        MatSmoother_smooth(smoother);
        smoothDiffExceeded = MatSmoother_exceedDiff(smoother);
        MatSmoother_destroy(smoother);

        tmp = target;
        target = source;
        source = tmp;
        resultFlipped = !resultFlipped;

        ctr++;
    } while (smoothDiffExceeded);

    printf("Looped %d times!\n", ctr);

    return resultFlipped ? source : target;
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

bool Matrix_equals(Matrix *matrix1, Matrix *matrix2) {
    if(matrix1->ySize == matrix2->ySize && matrix1->xSize == matrix2->xSize) {
        bool match = true;
        MatIterator *matIterator1 = Matrix_getIterator(matrix1, 0, 0, matrix1->xSize, matrix1->ySize);
        MatIterator *matIterator2 = Matrix_getIterator(matrix2, 0, 0, matrix2->xSize, matrix2->ySize);
        while(MatIterator_hasNext(matIterator1) && match) {
            match = *MatIterator_next(matIterator1) == *MatIterator_next(matIterator2);
        }
        MatIterator_destroy(matIterator1);
        MatIterator_destroy(matIterator2);
        return match;
    } else {
        return false;
    }
}


void Matrix_destroy(Matrix *matrix) {
    free(matrix->data);
    free(matrix);
}
