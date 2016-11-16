//
// Created by dominic on 01/11/16.
//

#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include "matrixIterator.h"

typedef struct matIterator {
    double *currentPtr;
    double *rowEnd;
    double *areaEnd;
    long nextRowJump;
    long areaWidth;
    int itrStep;
} MatIterator;

bool advanceRow(MatIterator *matIterator) {
    matIterator->currentPtr += matIterator->nextRowJump;
    matIterator->rowEnd += matIterator->nextRowJump + matIterator->areaWidth;
    return true;
}

MatIterator *MatIterator_init(double *dataPtr, long fullWidth, long areaWidth, long areaHeight) {
    MatIterator *matIterator = malloc(sizeof(MatIterator));
    matIterator->currentPtr = dataPtr;
    matIterator->nextRowJump = fullWidth - areaWidth;
    matIterator->areaEnd = matIterator->currentPtr + (fullWidth * (areaHeight - 1)) + areaWidth;
    matIterator->areaWidth = areaWidth;
    matIterator->rowEnd = matIterator->currentPtr + matIterator->areaWidth - 1;
    matIterator->itrStep = 1;
    return matIterator;
}

MatIterator *MatIterator_split(MatIterator* matIterator1) {
    MatIterator *matIterator2 = malloc(sizeof(MatIterator));
    matIterator2->currentPtr = matIterator1->currentPtr;
    matIterator2->rowEnd = matIterator1->rowEnd;
    matIterator2->areaEnd = matIterator1->areaEnd;
    matIterator2->nextRowJump = matIterator1->nextRowJump;
    matIterator2->areaWidth = matIterator1->areaWidth;
    matIterator2->itrStep = matIterator1->itrStep;

    MatIterator_next(matIterator2);

    matIterator1->itrStep *= 2;
    matIterator2->itrStep *= 2;
    return matIterator2;
}

double *MatIterator_step(MatIterator *matIterator) {
    if (MatIterator_hasNext(matIterator)) {
        if (matIterator->currentPtr > matIterator->rowEnd) {
            advanceRow(matIterator);
        }
        return matIterator->currentPtr++;
    } else {
        return NULL;
    }
}

double *MatIterator_next(MatIterator *matIterator) {
    double* retPtr = MatIterator_step(matIterator);
    for(int i = matIterator->itrStep; i > 1; i--) {
        MatIterator_step(matIterator);
    }
    return retPtr;
}

bool MatIterator_hasNext(MatIterator *matIterator) {
    return matIterator->currentPtr < matIterator->areaEnd;
}

void MatIterator_destroy(MatIterator *matIterator) {
    free(matIterator);
}





