//
// Created by dominic on 05/11/16.
//

#ifndef PARALLEL_COMPUTATION_CW1_MATSMOOTHER_H
#define PARALLEL_COMPUTATION_CW1_MATSMOOTHER_H

#include "matrixIterator.h"

typedef struct matrixSmoother MatSmoother;

MatSmoother *MatSmoother_init(MatIterator *target,
                              MatIterator *srcUp,
                              MatIterator *srcDown,
                              MatIterator *srcLeft,
                              MatIterator *srcRight);

void MatSmoother_smooth(MatSmoother *matSmoother);

void MatSmoother_destroy(MatSmoother *matSmoother);

#endif //PARALLEL_COMPUTATION_CW1_MATSMOOTHER_H
