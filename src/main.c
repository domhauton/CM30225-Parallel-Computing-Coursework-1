#include <stdio.h>
#include <stdlib.h>
#include "matrix/matrix.h"
#include "matrix/matrixFactory.h"

int main() {
    Matrix *matrix = MatrixFactory_initRandom(5, 3);
    Matrix *matrix2 = MatrixFactory_initEmpty(5, 3);
    Matrix_print(matrix);
    printf("\n");
    Matrix_print(matrix2);
    Matrix_copyEdge(matrix, matrix2);
    printf("\n");
    Matrix_print(matrix2);
    Matrix_destroy(matrix);
    Matrix_destroy(matrix2);
    return 0;
}




