//
// Created by dominic on 06/11/16.
//

#include <stdbool.h>
#include <stdlib.h>
#include "matrixEdgeIterator.h"

typedef struct matEdgeIterator {
    double *currentPtr;
    double *endFirstRow;
    double *startFinalRow;
    double *endFinalRow;
    // Navigation
    bool isAtRowStart;
    long rowJumpSize;
} MatEdgeIterator;

MatEdgeIterator *MatEdgeIterator_init(double *startPtr, long areaWidth, long areaHeight) {
    MatEdgeIterator *edgeIterator = malloc(sizeof(MatEdgeIterator));

    edgeIterator->currentPtr = startPtr - 1;
    edgeIterator->endFirstRow = startPtr + areaWidth - 1;
    edgeIterator->startFinalRow = startPtr + (areaWidth * (areaHeight - 1));
    edgeIterator->endFinalRow = startPtr + (areaHeight * areaWidth) - 1;
    edgeIterator->isAtRowStart = true;
    edgeIterator->rowJumpSize = areaWidth - 1;

    return edgeIterator;
}

double *MatEdgeIterator_next(MatEdgeIterator *edgeIterator) {
    if (edgeIterator->currentPtr <= edgeIterator->endFirstRow) {
        return ++edgeIterator->currentPtr;
    } else if (edgeIterator->currentPtr < edgeIterator->startFinalRow) {
        if (edgeIterator->isAtRowStart) {
            edgeIterator->isAtRowStart = false;
            edgeIterator->currentPtr += edgeIterator->rowJumpSize;
            return edgeIterator->currentPtr;
        } else {
            edgeIterator->isAtRowStart = true;
            return ++edgeIterator->currentPtr;
        }
    } else if (edgeIterator->currentPtr < edgeIterator->endFinalRow) {
        return ++edgeIterator->currentPtr;
    } else {
        return NULL;
    }
}

bool MatEdgeIterator_hasNext(MatEdgeIterator *edgeIterator) {
    return edgeIterator->currentPtr < edgeIterator->endFinalRow;
}

void MatEdgeIterator_destroy(MatEdgeIterator *edgeIterator) {
    free(edgeIterator);
}