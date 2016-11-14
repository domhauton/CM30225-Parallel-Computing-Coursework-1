//
// Created by dominic on 05/11/16.
//

#include <malloc.h>
#include "matrixSmoother.h"

typedef struct matrixSmoother {
    MatIterator *target;
    MatIterator *srcCntr;
    MatIterator *srcUp;
    MatIterator *srcDown;
    MatIterator *srcLeft;
    MatIterator *srcRight;
    double diffLimit;
    bool overLimit;
} MatSmoother;

MatSmoother *MatSmoother_init(MatIterator *target,
                              MatIterator *srcCenter,
                              MatIterator *srcUp,
                              MatIterator *srcDown,
                              MatIterator *srcLeft,
                              MatIterator *srcRight,
                              double diffLimit) {
    MatSmoother *matSmoother = malloc(sizeof(MatSmoother));
    matSmoother->target = target;
    matSmoother->srcCntr = srcCenter;
    matSmoother->srcUp = srcUp;
    matSmoother->srcDown = srcDown;
    matSmoother->srcLeft = srcLeft;
    matSmoother->srcRight = srcRight;
    matSmoother->diffLimit = diffLimit;
    matSmoother->overLimit = false;
    return matSmoother;
}

void MatSmoother_smoothIgnoreDiff(MatSmoother *matSmoother) {
    while (MatIterator_hasNext(matSmoother->target)) {
        *MatIterator_next(matSmoother->target) = (*MatIterator_next(matSmoother->srcUp) +
                                                  *MatIterator_next(matSmoother->srcDown) +
                                                  *MatIterator_next(matSmoother->srcLeft) +
                                                  *MatIterator_next(matSmoother->srcRight)) / 4;
    }
}

void MatSmoother_smooth(MatSmoother *matSmoother) {
    while (!matSmoother->overLimit && MatIterator_hasNext(matSmoother->target)) {
        double* currentResPtr = MatIterator_next(matSmoother->target);
        *currentResPtr = (*MatIterator_next(matSmoother->srcUp) +
                                                  *MatIterator_next(matSmoother->srcDown) +
                                                  *MatIterator_next(matSmoother->srcLeft) +
                                                  *MatIterator_next(matSmoother->srcRight)) / 4;
        double diff = *currentResPtr - *MatIterator_next(matSmoother->srcCntr);
        matSmoother->overLimit = matSmoother->diffLimit < diff && -diff < matSmoother->diffLimit;
    }
    MatSmoother_smoothIgnoreDiff(matSmoother);
}

bool MatSmoother_exceedDiff(MatSmoother *matSmoother){
    return matSmoother->overLimit;
};

void MatSmoother_destroy(MatSmoother *matSmoother) {
    MatIterator_destroy(matSmoother->srcCntr);
    MatIterator_destroy(matSmoother->srcUp);
    MatIterator_destroy(matSmoother->srcDown);
    MatIterator_destroy(matSmoother->srcLeft);
    MatIterator_destroy(matSmoother->srcRight);
    MatIterator_destroy(matSmoother->target);
    free(matSmoother);
}