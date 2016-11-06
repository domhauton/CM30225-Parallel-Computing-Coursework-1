//
// Created by dominic on 05/11/16.
//

#include <malloc.h>
#include "matrixSmoother.h"

typedef struct matrixSmoother {
    MatIterator *target;
    MatIterator *srcUp;
    MatIterator *srcDown;
    MatIterator *srcLeft;
    MatIterator *srcRight;
} MatSmoother;

MatSmoother *MatSmoother_init(MatIterator *target,
                              MatIterator *srcUp,
                              MatIterator *srcDown,
                              MatIterator *srcLeft,
                              MatIterator *srcRight) {
    MatSmoother *matSmoother = malloc(sizeof(matSmoother));
    matSmoother->target = target;
    matSmoother->srcUp = srcUp;
    matSmoother->srcDown = srcDown;
    matSmoother->srcLeft = srcLeft;
    matSmoother->srcRight = srcRight;
    return matSmoother;
}

void MatSmoother_smooth(MatSmoother *matSmoother) {
    while (MatIterator_hasNext(matSmoother->target)) {
        *MatIterator_next(matSmoother->target) = (*MatIterator_next(matSmoother->srcUp) +
                                                  *MatIterator_next(matSmoother->srcDown) +
                                                  *MatIterator_next(matSmoother->srcLeft) +
                                                  *MatIterator_next(matSmoother->srcRight)) / 4;
    }
}

void MatSmoother_destroy(MatSmoother *matSmoother) {
    free(matSmoother);
}