//
// Created by dominic on 05/11/16.
//

#include <stdlib.h>
#include "matrix.h"

const long RNG_SEED = 31413241L;

Matrix *MatrixFactory_initEmpty(long xSize, long ySize) {
    double *data = calloc(sizeof(double), sizeof(double) * xSize * ySize);
    return Matrix_initSet(data, xSize, ySize);
}

Matrix *MatrixFactory_initRandom(long xSize, long ySize) {
    Matrix *matrix = MatrixFactory_initEmpty(xSize, ySize);
    srand48(RNG_SEED);

    MatEdgeIterator *matEdgeIterator = Matrix_getEdgeIterator(matrix);
    while(MatEdgeIterator_hasNext(matEdgeIterator)) {
        *MatEdgeIterator_next(matEdgeIterator) = drand48();
    }
    MatEdgeIterator_destroy(matEdgeIterator);

    return matrix;
}