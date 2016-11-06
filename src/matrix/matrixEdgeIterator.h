//
// Created by dominic on 06/11/16.
//

#ifndef PARALLEL_COMPUTATION_CW1_MATRIXEDGEITERATOR_H
#define PARALLEL_COMPUTATION_CW1_MATRIXEDGEITERATOR_H

#include <stdbool.h>

typedef struct matEdgeIterator MatEdgeIterator;

MatEdgeIterator *MatEdgeIterator_init(double *startPtr, long areaWidth, long areaHeight);

double *MatEdgeIterator_next(MatEdgeIterator *edgeIterator);

bool MatEdgeIterator_hasNext(MatEdgeIterator *edgeIterator);

void MatEdgeIterator_destroy(MatEdgeIterator *edgeIterator);

#endif //PARALLEL_COMPUTATION_CW1_MATRIXEDGEITERATOR_H
