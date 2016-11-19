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

MatSmoother *getSmoother(Matrix *source, Matrix *target,
                         double limit, bool *overLimit,
                         long x, long y,
                         long xSize, long ySize) {
    MatIterator *resItr = Matrix_getIterator(target, x, y, xSize, ySize);
    MatIterator *ctrItr = Matrix_getIterator(source, x, y, xSize, ySize);
    MatIterator *topItr = Matrix_getIterator(source, x - 1, y, xSize, ySize);
    MatIterator *botItr = Matrix_getIterator(source, x + 1, y, xSize, ySize);
    MatIterator *lftItr = Matrix_getIterator(source, x, y - 1, xSize, ySize);
    MatIterator *rgtItr = Matrix_getIterator(source, x, y + 1, xSize, ySize);
    return MatSmoother_init(resItr, ctrItr, topItr, botItr, lftItr, rgtItr, overLimit, limit);
}

MatSmoother *Matrix_getSmoother(Matrix *source, Matrix *target, double limit, bool *overLimit) {
    long itrWidth = target->xSize - 2;
    long itrHeight = target->ySize - 2;
    return getSmoother(source, target, limit, overLimit, 1, 1, itrWidth, itrHeight);
}

void Matrix_getSmootherCut(Matrix *source,
                           Matrix *target,
                           double limit,
                           bool *overLimit,
                           unsigned int sections,
                           MatSmoother** smoothers){
    long smoothingHeight = target->ySize - 2;
    MatSmoother** tmpPtr = smoothers;

    if(smoothingHeight <= sections) { // 1 Thread per row
        for(int i = 1; i <= smoothingHeight; i++) {
            *tmpPtr++ = getSmoother(source, target, limit, overLimit, 1, i, target->xSize - 2, 1);
        }
    } else {
        long sectionHeight = smoothingHeight / sections;
        long longerSections = smoothingHeight % sections;
        long nextHeight = 1;
        for(int i = 0; i < sections; i++) {
            if(i < longerSections) {
                *tmpPtr++ = getSmoother(source, target, limit, overLimit, 1, nextHeight, target->xSize - 2, sectionHeight + 1);
                nextHeight += sectionHeight + 1;
            } else {
                *tmpPtr++ = getSmoother(source, target, limit, overLimit, 1, nextHeight, target->xSize - 2, sectionHeight);
                nextHeight += sectionHeight;
            }
        }
    }
}

Matrix *Matrix_smoothUntilLimit(Matrix *source, Matrix *target, double limit, bool *overLimit) {
    bool resultFlipped = false;
    long ctr = 0;
    Matrix *tmp;
    do {
        *overLimit = false;
        MatSmoother *smoother = Matrix_getSmoother(source, target, limit, overLimit);
        MatSmoother_smooth(smoother);
        MatSmoother_destroy(smoother);

        tmp = target;
        target = source;
        source = tmp;
        resultFlipped = !resultFlipped;

        ctr++;
    } while (*overLimit);

    printf("%08li,", ctr);

    return resultFlipped ? source : target;
}

union {
    double d; // C99 double == 64 bytes
    unsigned long long int ll; // C99 long long >= 64 bytes
} dubLongTypePunner;

unsigned long long int Matrix_getParity(Matrix *matrix) {
    unsigned long long int currentParity = 0ULL;
    double *endPtr = matrix->data + matrix->ySize*matrix->ySize;
    for(double *tmpPtr = matrix->data; tmpPtr < endPtr; tmpPtr++) {
        dubLongTypePunner.d = *tmpPtr;
        currentParity ^= dubLongTypePunner.ll;
    }
    return currentParity;
}

unsigned long long int Matrix_getCRC64(Matrix *matrix) {
    unsigned long long int currentCRC = 0ULL;
    double *endPtr = matrix->data + matrix->ySize*matrix->ySize;
    for(double *tmpPtr = matrix->data; tmpPtr < endPtr; tmpPtr++) {
        dubLongTypePunner.d = *tmpPtr;
        currentCRC += dubLongTypePunner.ll;
    }
    return currentCRC;
}

void Matrix_copyEdge(Matrix *source, Matrix *target) {
    MatEdgeIterator *edgeIterator1 = Matrix_getEdgeIterator(source);
    MatEdgeIterator *edgeIterator2 = Matrix_getEdgeIterator(target);
    while (MatEdgeIterator_hasNext(edgeIterator1)) {
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
    if (matrix1->ySize == matrix2->ySize && matrix1->xSize == matrix2->xSize) {
        bool match = true;
        MatIterator *matIterator1 = Matrix_getIterator(matrix1, 0, 0, matrix1->xSize, matrix1->ySize);
        MatIterator *matIterator2 = Matrix_getIterator(matrix2, 0, 0, matrix2->xSize, matrix2->ySize);
        while (MatIterator_hasNext(matIterator1) && match) {
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
