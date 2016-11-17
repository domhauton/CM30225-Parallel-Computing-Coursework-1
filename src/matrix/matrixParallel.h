//
// Created by dominic on 15/11/16.
//

#ifndef PARALLEL_COMPUTATION_CW1_MATRIXPARALLEL_H
#define PARALLEL_COMPUTATION_CW1_MATRIXPARALLEL_H

#include "matrix.h"

Matrix *MatrixParallel_smoothUntilLimit(Matrix *source, Matrix *target, double limit, int threadCount);

Matrix *MatrixParallel_smoothUntilLimitPooled(Matrix *source, Matrix *target, double limit, unsigned int threadCount);

Matrix *MatrixParallel_smoothUntilLimitPooledCut(Matrix *source,
                                                 Matrix *target,
                                                 double limit,
                                                 unsigned int threadCount);

#endif //PARALLEL_COMPUTATION_CW1_MATRIXPARALLEL_H
