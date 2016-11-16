//
// Created by dominic on 15/11/16.
//

#include <pthread.h>
#include <stdio.h>
#include <sys/time.h>
#include "matrixParallel.h"
#include "matrixFactory.h"

void *parallelSmoothHelper(void *param) {
    MatSmoother_smooth(param);
    return NULL;
}

Matrix *MatrixParallel_smoothUntilLimit(Matrix *source, Matrix *target, double limit, int threadCount) {
    bool smoothDiffExceeded;
    bool resultFlipped = false;
    long ctr = 0;
    Matrix *tmp;
    pthread_t threads[threadCount];
    MatSmoother *smoothers[threadCount];
    do {
        smoothers[0] = Matrix_getSmoother(source, target, limit);
        for (int pos = 2; pos <= threadCount; pos++) {
            smoothers[pos - 1] = MatSmoother_split(smoothers[(pos - (pos >> 1)) - 1]);
        }
        for (int i = 0; i < threadCount; i++) {
            pthread_create(&threads[i], NULL, parallelSmoothHelper, smoothers[i]);
        }
        smoothDiffExceeded = false;
        for (int i = 0; i < threadCount; i++) {
            if (pthread_join(threads[i], NULL)) {
                printf("Error joining threads again.\n");
            }
            smoothDiffExceeded = smoothDiffExceeded
                                 || MatSmoother_exceedDiff(smoothers[i]);
            MatSmoother_destroy(smoothers[i]);
        }
        tmp = target;
        target = source;
        source = tmp;
        resultFlipped = !resultFlipped;
        ctr++;
    } while (smoothDiffExceeded);

    printf("%05li,", ctr);

    return resultFlipped ? source : target;
}

