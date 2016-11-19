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
    //long ctr;
    double diffLimit;
    bool *overLimit;
    bool split;
} MatSmoother;

MatSmoother *MatSmoother_init(MatIterator *target,
                              MatIterator *srcCenter,
                              MatIterator *srcUp,
                              MatIterator *srcDown,
                              MatIterator *srcLeft,
                              MatIterator *srcRight,
                              bool *overLimit,
                              double diffLimit) {
    MatSmoother *matSmoother = malloc(sizeof(MatSmoother));
    matSmoother->target = target;
    matSmoother->srcMid = srcCenter;
    matSmoother->srcUp = srcUp;
    matSmoother->srcDown = srcDown;
    matSmoother->srcLeft = srcLeft;
    matSmoother->srcRight = srcRight;
    matSmoother->diffLimit = diffLimit;
    matSmoother->overLimit = overLimit;
    matSmoother->split = false;
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
    matSmoother2->split = true;
    matSmoother1->split = true;

    return matSmoother2;
}

void MatSmoother_smoothIgnoreDiff(MatSmoother *matSmoother) {
    if (matSmoother->split) {
        while (MatIterator_hasNext(matSmoother->srcDown)) {
            *MatIterator_nextUnsafe(matSmoother->target) = ((*MatIterator_nextUnsafe(matSmoother->srcUp) +
                                                             *MatIterator_nextUnsafe(matSmoother->srcDown)) +
                                                            (*MatIterator_nextUnsafe(matSmoother->srcLeft) +
                                                             *MatIterator_nextUnsafe(matSmoother->srcRight))) / 4;
        }
    } else {
        while (MatIterator_hasNext(matSmoother->srcDown)) {
            *MatIterator_stepUnsafe(matSmoother->target) = ((*MatIterator_stepUnsafe(matSmoother->srcUp) +
                                                             *MatIterator_stepUnsafe(matSmoother->srcDown)) +
                                                            (*MatIterator_stepUnsafe(matSmoother->srcLeft) +
                                                             *MatIterator_stepUnsafe(matSmoother->srcRight))) / 4;
        }
    }
}

void MatSmoother_smooth(MatSmoother *matSmoother) {
    if (matSmoother->split) {
        while (!*matSmoother->overLimit && MatIterator_hasNext(matSmoother->srcDown)) {
            double *currentResPtr = MatIterator_nextUnsafe(matSmoother->target);
            *currentResPtr = ((*MatIterator_nextUnsafe(matSmoother->srcUp) +
                               *MatIterator_nextUnsafe(matSmoother->srcDown)) +
                              (*MatIterator_nextUnsafe(matSmoother->srcLeft) +
                               *MatIterator_nextUnsafe(matSmoother->srcRight))) / 4;
            double diff = *currentResPtr - *MatIterator_nextUnsafe(matSmoother->srcMid);
            if (matSmoother->diffLimit < diff && -diff < matSmoother->diffLimit) {
                *matSmoother->overLimit = true;
            }
        }
    } else {
        while (!*matSmoother->overLimit && MatIterator_hasNext(matSmoother->srcDown)) {
            double *currentResPtr = MatIterator_stepUnsafe(matSmoother->target);
            *currentResPtr = ((*MatIterator_stepUnsafe(matSmoother->srcUp) +
                               *MatIterator_stepUnsafe(matSmoother->srcDown)) +
                              (*MatIterator_stepUnsafe(matSmoother->srcLeft) +
                               *MatIterator_stepUnsafe(matSmoother->srcRight))) / 4;
            double diff = *currentResPtr - *MatIterator_stepUnsafe(matSmoother->srcMid);
            if (matSmoother->diffLimit < diff && -diff < matSmoother->diffLimit) {
                *matSmoother->overLimit = true;
            }
        }
    }

    MatSmoother_smoothIgnoreDiff(matSmoother);
}

bool MatSmoother_exceedDiff(MatSmoother *matSmoother) {
    return *matSmoother->overLimit;
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