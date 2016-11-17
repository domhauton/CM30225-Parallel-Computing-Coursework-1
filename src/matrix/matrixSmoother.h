//
// Created by dominic on 05/11/16.
//

#ifndef PARALLEL_COMPUTATION_CW1_MATSMOOTHER_H
#define PARALLEL_COMPUTATION_CW1_MATSMOOTHER_H

#include "matrixIterator.h"

typedef struct matrixSmoother MatSmoother;

MatSmoother *MatSmoother_init(MatIterator *target,
                              MatIterator *srcCenter,
                              MatIterator *srcUp,
                              MatIterator *srcDown,
                              MatIterator *srcLeft,
                              MatIterator *srcRight,
                              double diffLimit);

void MatSmoother_smooth(MatSmoother *matSmoother);

long MatSmoother_getIterations(MatSmoother *matSmoother);

MatSmoother *MatSmoother_split(MatSmoother *matSmoother1);

bool MatSmoother_exceedDiff(MatSmoother *matSmoother);

void MatSmoother_destroy(MatSmoother *matSmoother);

#endif //PARALLEL_COMPUTATION_CW1_MATSMOOTHER_H
