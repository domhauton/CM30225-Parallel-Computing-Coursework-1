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
    bool resultFlipped = false;
    long ctr = 0;
    Matrix *tmp;
    pthread_t threads[threadCount];
    MatSmoother *smoothers[threadCount];
    bool *overLimit = calloc(false, sizeof(bool));
    do {
        *overLimit = false;
        smoothers[0] = Matrix_getSmoother(source, target, limit, overLimit);
        for (int pos = 2; pos <= threadCount; pos++) {
            smoothers[pos - 1] = MatSmoother_split(smoothers[(pos - (pos >> 1)) - 1]);
        }
        for (int i = 0; i < threadCount; i++) {
            pthread_create(&threads[i], NULL, parallelSmoothHelper, smoothers[i]);
        }
        //long totItrCnt = 0;
        for (int i = 0; i < threadCount; i++) {
            if (pthread_join(threads[i], NULL)) {
                printf("Error joining threads.\n");
            }
            //totItrCnt += MatSmoother_getIterations(smoothers[i]);
            MatSmoother_destroy(smoothers[i]);
        }
        //printf("Smooth Ops (sqrt): %f\n", sqrt(totItrCnt));
        tmp = target;
        target = source;
        source = tmp;
        resultFlipped = !resultFlipped;
        ctr++;
    } while (*overLimit);

    free(overLimit);
    printf("%08li,", ctr);

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
    while (arguments->alive) {
        if (arguments->matSmoother != NULL) {
            MatSmoother_smooth(arguments->matSmoother);
        }
        pthread_barrier_wait(arguments->endWorkBarrier);
        pthread_barrier_wait(arguments->startWorkBarrier);
    }
    return NULL;
}

Matrix *MatrixParallel_smoothUntilLimitPooled(Matrix *source, Matrix *target, double limit, unsigned int threadCount) {
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

    bool *overLimit = calloc(false, sizeof(bool));

    do {
        *overLimit = false;
        // Create Jobs
        pArgs[0].matSmoother = Matrix_getSmoother(source, target, limit, overLimit);
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
        for (int i = 0; i < threadCount; i++) {
            MatSmoother_destroy(pArgs[i].matSmoother);
        }

        tmp = target;
        target = source;
        source = tmp;
        resultFlipped = !resultFlipped;
        ctr++;
    } while (*overLimit);

    // Tidy and join
    for (int i = 0; i < threadCount; i++) {
        pArgs[i].alive = false;
    }

    pthread_barrier_wait(startWorkBarrier);
    pthread_barrier_destroy(startWorkBarrier);

    free(startWorkBarrier);
    free(endWorkBarrier);
    free(overLimit);

    for (int i = 0; i < threadCount; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("%08li,", ctr);

    return resultFlipped ? source : target;
}

Matrix *MatrixParallel_smoothUntilLimitPooledCut(Matrix *source,
                                                 Matrix *target,
                                                 double limit,
                                                 unsigned int threadCount) {
    bool resultFlipped = false;
    long ctr = 0;
    Matrix *tmp;
    pthread_t threads[threadCount];
    pthread_barrier_t *startWorkBarrier = malloc(sizeof(pthread_barrier_t));
    pthread_barrier_t *endWorkBarrier = malloc(sizeof(pthread_barrier_t));
    bool *overLimit = calloc(false, sizeof(bool));
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
        *overLimit = false;
        // Create Jobs
        Matrix_getSmootherCut(source, target, limit, overLimit, threadCount, smootherPtrs);
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
        //printf("[");
        for (int i = 0; i < threadCount; i++) {
            MatSmoother_destroy(pArgs[i].matSmoother);
            //printf("%f, ", pArgs[i].workTime);
        }
        //printf("]\n");
        tmp = target;
        target = source;
        source = tmp;
        resultFlipped = !resultFlipped;
        ctr++;
    } while (*overLimit);

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
    free(overLimit);

    printf("%08li,", ctr);

    return resultFlipped ? source : target;
}

Matrix *MatrixParallel_smoothUntilLimitManagedPool(Matrix *source,
                                                   Matrix *target,
                                                   double limit,
                                                   unsigned int threadCount,
                                                   unsigned int splitCount) {
    bool resultFlipped = false;
    long ctr = 0;
    Matrix *tmp;
    pthread_t threads[threadCount];
    pthread_barrier_t *startWorkBarrier = malloc(sizeof(pthread_barrier_t));
    pthread_barrier_t *endWorkBarrier = malloc(sizeof(pthread_barrier_t));
    bool *overLimit = calloc(false, sizeof(bool));
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
        *overLimit = false;
        // Create Jobs
        Matrix_getSmootherCut(source, target, limit, overLimit, threadCount, smootherPtrs);
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
        //printf("[");
        for (int i = 0; i < threadCount; i++) {
            MatSmoother_destroy(pArgs[i].matSmoother);
            //printf("%f, ", pArgs[i].workTime);
        }
        //printf("]\n");
        tmp = target;
        target = source;
        source = tmp;
        resultFlipped = !resultFlipped;
        ctr++;
    } while (*overLimit);

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
    free(overLimit);

    printf("%08li,", ctr);

    return resultFlipped ? source : target;
}

