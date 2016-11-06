#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "matrix/matrix.h"
#include "matrix/matrixFactory.h"

int main() {
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
    return 0;
}




