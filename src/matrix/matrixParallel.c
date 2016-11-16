//
// Created by dominic on 15/11/16.
//

#include <pthread.h>
#include <stdio.h>
#include "matrixParallel.h"

void *parallelSmoothHelper(void *param) {
    MatSmoother_smooth(param);
    return NULL;
}


Matrix *MatrixParallel_smoothUntilLimit(Matrix *source, Matrix *target, double limit) {
    bool smoothDiffExceeded;
    bool resultFlipped = false;
    long ctr = 0;
    Matrix *tmp;
    pthread_t thread0;
    pthread_t thread1;
    do {
        MatSmoother *smoother0 = Matrix_getSmoother(source, target, limit);
        MatSmoother *smoother1 = MatSmoother_split(smoother0);
        pthread_create(&thread0, NULL, parallelSmoothHelper, smoother0);
        pthread_create(&thread1, NULL, parallelSmoothHelper, smoother1);
        if (pthread_join(thread0, NULL) || pthread_join(thread1, NULL)) {
            printf("Error joining threads again.\n");
        }
        smoothDiffExceeded = MatSmoother_exceedDiff(smoother0) || MatSmoother_exceedDiff(smoother1);
        MatSmoother_destroy(smoother0);
        MatSmoother_destroy(smoother1);
        tmp = target;
        target = source;
        source = tmp;
        resultFlipped = !resultFlipped;
        ctr++;
    } while (smoothDiffExceeded);

    printf("Looped %d times!\n", ctr);

    return resultFlipped ? source : target;
}