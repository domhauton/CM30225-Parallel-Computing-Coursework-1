//
// Created by dominic on 01/11/16.
//

#ifndef PARALLEL_COMPUTATION_CW1_MATRIXSMOOTHER_H
#define PARALLEL_COMPUTATION_CW1_MATRIXSMOOTHER_H

#include <stdbool.h>

typedef struct matIterator MatIterator;

MatIterator *MatIterator_init(double *dataPtr, long fullWidth, long areaWidth, long areaHeight);

double *MatIterator_next(MatIterator *matIterator);

bool MatIterator_hasNext(MatIterator *matIterator);

void MatIterator_destroy(MatIterator *matIterator);

#endif //PARALLEL_COMPUTATION_CW1_MATRIXSMOOTHER_H
