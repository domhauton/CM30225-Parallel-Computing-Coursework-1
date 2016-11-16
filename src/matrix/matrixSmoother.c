//
// Created by dominic on 05/11/16.
//

#include <malloc.h>
#include "matrixSmoother.h"

typedef struct matrixSmoother {
    MatIterator *target;
    MatIterator *srcMid;
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
    matSmoother->srcMid = srcCenter;
    matSmoother->srcUp = srcUp;
    matSmoother->srcDown = srcDown;
    matSmoother->srcLeft = srcLeft;
    matSmoother->srcRight = srcRight;
    matSmoother->diffLimit = diffLimit;
    matSmoother->overLimit = false;
    return matSmoother;
}

MatSmoother *MatSmoother_split(MatSmoother *matSmoother1) {
    MatSmoother *matSmoother2 = malloc(sizeof(MatSmoother));
    matSmoother2->target = MatIterator_split(matSmoother1->target);
    matSmoother2->srcMid = MatIterator_split(matSmoother1->srcMid);
    matSmoother2->srcUp = MatIterator_split(matSmoother1->srcUp);
    matSmoother2->srcDown = MatIterator_split(matSmoother1->srcDown);
    matSmoother2->srcLeft = MatIterator_split(matSmoother1->srcLeft);
    matSmoother2->srcRight = MatIterator_split(matSmoother1->srcRight);
    matSmoother2->diffLimit = matSmoother1->diffLimit;
    matSmoother2->overLimit = matSmoother1->overLimit;
    return matSmoother2;
}

void MatSmoother_smoothIgnoreDiff(MatSmoother *matSmoother) {
    while (MatIterator_hasNext(matSmoother->srcDown)) {
        *MatIterator_next(matSmoother->target) = (*MatIterator_next(matSmoother->srcUp) +
                                                  *MatIterator_next(matSmoother->srcDown) +
                                                  *MatIterator_next(matSmoother->srcLeft) +
                                                  *MatIterator_next(matSmoother->srcRight)) / 4;
    }
}

void MatSmoother_smooth(MatSmoother *matSmoother) {
    while (!matSmoother->overLimit && MatIterator_hasNext(matSmoother->srcDown)) {
        double* currentResPtr = MatIterator_next(matSmoother->target);
        *currentResPtr = (*MatIterator_next(matSmoother->srcUp) +
                                                  *MatIterator_next(matSmoother->srcDown) +
                                                  *MatIterator_next(matSmoother->srcLeft) +
                                                  *MatIterator_next(matSmoother->srcRight)) / 4;
        double diff = *currentResPtr - *MatIterator_next(matSmoother->srcMid);
        matSmoother->overLimit = matSmoother->diffLimit < diff && -diff < matSmoother->diffLimit;
    }
    MatSmoother_smoothIgnoreDiff(matSmoother);
}

bool MatSmoother_exceedDiff(MatSmoother *matSmoother){
    return matSmoother->overLimit;
};

void MatSmoother_destroy(MatSmoother *matSmoother) {
    MatIterator_destroy(matSmoother->srcMid);
    MatIterator_destroy(matSmoother->srcUp);
    MatIterator_destroy(matSmoother->srcDown);
    MatIterator_destroy(matSmoother->srcLeft);
    MatIterator_destroy(matSmoother->srcRight);
    MatIterator_destroy(matSmoother->target);
    free(matSmoother);
}