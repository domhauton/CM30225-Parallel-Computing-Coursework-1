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
} MatIterator;

bool advanceRow(MatIterator *matIterator) {
    matIterator->currentPtr += matIterator->nextRowJump;
    matIterator->rowEnd += matIterator->nextRowJump + matIterator->areaWidth;
    printf("Jump!\n");
    return true;
}

MatIterator *MatIterator_init(double *dataPtr, long fullWidth, long areaWidth, long areaHeight) {
    MatIterator *matIterator = malloc(sizeof(MatIterator));
    matIterator->currentPtr = dataPtr - 1;
    matIterator->nextRowJump = fullWidth - areaWidth;
    matIterator->areaEnd = matIterator->currentPtr + (fullWidth * (areaHeight - 1)) + areaWidth;
    matIterator->areaWidth = areaWidth;
    matIterator->rowEnd = matIterator->currentPtr + matIterator->areaWidth;
    return matIterator;
}

double *MatIterator_next(MatIterator *matIterator) {
    if (matIterator->currentPtr == matIterator->areaEnd) {
        return NULL;
    } else {
        if (matIterator->currentPtr == matIterator->rowEnd) {
            advanceRow(matIterator);
        }
        return ++matIterator->currentPtr;
    }
}

bool MatIterator_hasNext(MatIterator *matIterator) {
    return matIterator->currentPtr != matIterator->areaEnd;
}

void MatIterator_destroy(MatIterator *matIterator) {
    free(matIterator);
}





