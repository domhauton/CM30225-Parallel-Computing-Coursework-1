//
// Created by dominic on 31/10/16.
//

#ifndef PARALLEL_COMPUTATION_CW1_RELAXATIONMATRIX_H
#define PARALLEL_COMPUTATION_CW1_RELAXATIONMATRIX_H

#include "matrixIterator.h"
#include "matrixEdgeIterator.h"

typedef struct matrix Matrix;

Matrix *Matrix_init();

Matrix *Matrix_initSet(double *values, long xSize, long ySize);

double *Matrix_getDataPtr(Matrix *matrix, long x, long y);

MatIterator *Matrix_getIterator(Matrix *matrix, long x, long y, long width, long height);

MatEdgeIterator *Matrix_getEdgeIterator(Matrix *matrix);

void Matrix_copyEdge(Matrix* source, Matrix* target);

Matrix *Matrix_smoothUntilLimit(Matrix *source, Matrix *target, double limit);

void Matrix_print(Matrix *matrix);

void Matrix_destroy(Matrix *matrix);

#endif //PARALLEL_COMPUTATION_CW1_RELAXATIONMATRIX_H
