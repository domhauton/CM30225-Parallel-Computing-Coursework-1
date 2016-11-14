#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "matrix/matrix.h"
#include "matrix/matrixFactory.h"

void matrixComputeTest() {
    Matrix *matrix = MatrixFactory_initRandom(1000, 1000);
    Matrix *matrix2 = MatrixFactory_initEmpty(1000, 1000);
    //Matrix_print(matrix);
    printf("\n");
    //Matrix_print(matrix2);
    Matrix_copyEdge(matrix, matrix2);
    printf("\n");
    //Matrix_print(matrix2);
    printf("\n");
    clock_t begin = clock();
    Matrix *tmp = Matrix_smoothUntilLimit(matrix, matrix2, 0.001f);
    clock_t end = clock();
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("Took %f seconds.\n", time_spent);
    //Matrix_print(tmp);
    Matrix_destroy(matrix);
    Matrix_destroy(matrix2);
}

void matrixOverwriteTest() {
    Matrix *matrix = MatrixFactory_initEmpty(7, 7);
    MatIterator* matIterator = Matrix_getIterator(matrix, 1, 1, 1, 5);
    while(MatIterator_hasNext(matIterator)) {
        *MatIterator_next(matIterator) = 1.111111;
    }
    MatIterator_destroy(matIterator);
    Matrix_print(matrix);
    Matrix_destroy(matrix);
}

int main() {
    matrixOverwriteTest();
    return 0;
}




