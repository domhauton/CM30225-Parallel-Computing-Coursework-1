//
// Created by dominic on 15/11/16.
//

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include "matrixParallel.h"

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
        //long totItrCnt = 0;
        for (int i = 0; i < threadCount; i++) {
            if (pthread_join(threads[i], NULL)) {
                printf("Error joining threads again.\n");
            }
            smoothDiffExceeded = smoothDiffExceeded
                                 || MatSmoother_exceedDiff(smoothers[i]);
            //totItrCnt += MatSmoother_getIterations(smoothers[i]);
            MatSmoother_destroy(smoothers[i]);
        }
        //printf("Smooth Ops (sqrt): %f\n", sqrt(totItrCnt));
        tmp = target;
        target = source;
        source = tmp;
        resultFlipped = !resultFlipped;
        ctr++;
    } while (smoothDiffExceeded);

    printf("%05li,", ctr);

    return resultFlipped ? source : target;
}

struct parallelArguments {
    MatSmoother *matSmoother;
    pthread_barrier_t *startWorkBarrier;
    pthread_barrier_t *endWorkBarrier;
    bool alive;
};

void *parallelMutexSmoothHelper(void *param) {
    struct parallelArguments *arguments = param;
    pthread_barrier_wait(arguments->startWorkBarrier);

    while(arguments->alive) {
        if (arguments->matSmoother != NULL) {
            MatSmoother_smooth(arguments->matSmoother);
        }
        pthread_barrier_wait(arguments->endWorkBarrier);
        pthread_barrier_wait(arguments->startWorkBarrier);
    }
    return NULL;
}

Matrix *MatrixParallel_smoothUntilLimitPooled(Matrix *source, Matrix *target, double limit, unsigned int threadCount) {
    bool smoothDiffExceeded;
    bool resultFlipped = false;
    long ctr = 0;
    Matrix *tmp;
    pthread_t threads[threadCount];
    pthread_barrier_t *startWorkBarrier = malloc(sizeof(pthread_barrier_t));
    pthread_barrier_t *endWorkBarrier = malloc(sizeof(pthread_barrier_t));
    struct parallelArguments pArgs[threadCount];

    pthread_barrier_init(startWorkBarrier, NULL, threadCount + 1);

    for (int i = 0; i < threadCount; i++) {
        pArgs[i].startWorkBarrier = startWorkBarrier;
        pArgs[i].endWorkBarrier = endWorkBarrier;
        pArgs[i].alive = true;
        pthread_create(&threads[i], NULL, parallelMutexSmoothHelper, &pArgs[i]);
    }

    do {
        // Create Jobs
        pArgs[0].matSmoother = Matrix_getSmoother(source, target, limit);
        for (int pos = 2; pos <= threadCount; pos++) {
            pArgs[pos - 1].matSmoother = MatSmoother_split(pArgs[(pos - (pos >> 1)) - 1].matSmoother);
        }

        pthread_barrier_init(endWorkBarrier, NULL, threadCount + 1);

        // Start work & reset
        pthread_barrier_wait(startWorkBarrier);
        pthread_barrier_destroy(startWorkBarrier);
        pthread_barrier_init(startWorkBarrier, NULL, threadCount + 1);

        // End work & reset
        pthread_barrier_wait(endWorkBarrier);
        pthread_barrier_destroy(endWorkBarrier);

        // Reduce
        smoothDiffExceeded = false;
        for (int i = 0; i < threadCount; i++) {
            smoothDiffExceeded = smoothDiffExceeded
                                 || MatSmoother_exceedDiff(pArgs[i].matSmoother);
            MatSmoother_destroy(pArgs[i].matSmoother);
        }

        tmp = target;
        target = source;
        source = tmp;
        resultFlipped = !resultFlipped;
        ctr++;
    } while (smoothDiffExceeded);

    // Tidy and join
    for (int i = 0; i < threadCount; i++) {
        pArgs[i].alive = false;
    }

    pthread_barrier_wait(startWorkBarrier);
    pthread_barrier_destroy(startWorkBarrier);

    free(startWorkBarrier);
    free(endWorkBarrier);

    for (int i = 0; i < threadCount; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("%05li,", ctr);

    return resultFlipped ? source : target;
}

Matrix *MatrixParallel_smoothUntilLimitPooledCut(Matrix *source,
                                                 Matrix *target,
                                                 double limit,
                                                 unsigned int threadCount) {
    bool smoothDiffExceeded;
    bool resultFlipped = false;
    long ctr = 0;
    Matrix *tmp;
    pthread_t threads[threadCount];
    pthread_barrier_t *startWorkBarrier = malloc(sizeof(pthread_barrier_t));
    pthread_barrier_t *endWorkBarrier = malloc(sizeof(pthread_barrier_t));
    struct parallelArguments pArgs[threadCount];

    MatSmoother *smootherPtrs[threadCount];

    pthread_barrier_init(endWorkBarrier, NULL, threadCount + 1);
    pthread_barrier_init(startWorkBarrier, NULL, threadCount + 1);

    for (int i = 0; i < threadCount; i++) {
        pArgs[i].startWorkBarrier = startWorkBarrier;
        pArgs[i].endWorkBarrier = endWorkBarrier;
        pArgs[i].alive = true;
        pthread_create(&threads[i], NULL, parallelMutexSmoothHelper, &pArgs[i]);
    }

    do {

        // Create Jobs
        //printf("Ctr: %ld\n", ctr);
        Matrix_getSmootherCut(source, target, limit, threadCount, smootherPtrs);
        for (int i = 0; i < threadCount; i++) {
            pArgs[i].matSmoother = smootherPtrs[i];
        }

        pthread_barrier_init(endWorkBarrier, NULL, threadCount + 1);

        // Start work & reset
        pthread_barrier_wait(startWorkBarrier);
        pthread_barrier_destroy(startWorkBarrier);
        pthread_barrier_init(startWorkBarrier, NULL, threadCount + 1);

        // End work & reset
        pthread_barrier_wait(endWorkBarrier);
        pthread_barrier_destroy(endWorkBarrier);

        // Reduce
        smoothDiffExceeded = false;
        for (int i = 0; i < threadCount; i++) {
            smoothDiffExceeded = smoothDiffExceeded
                                 || MatSmoother_exceedDiff(pArgs[i].matSmoother);
            MatSmoother_destroy(pArgs[i].matSmoother);
        }
        tmp = target;
        target = source;
        source = tmp;
        resultFlipped = !resultFlipped;
        ctr++;
    } while (smoothDiffExceeded);

    // Tidy and join
    for (int i = 0; i < threadCount; i++) {
        pArgs[i].alive = false;
    }

    pthread_barrier_wait(startWorkBarrier);
    pthread_barrier_destroy(startWorkBarrier);
    for (int i = 0; i < threadCount; i++) {
        pthread_join(threads[i], NULL);
    }

    free(startWorkBarrier);
    free(endWorkBarrier);

    printf("%05li,", ctr);

    return resultFlipped ? source : target;
}


