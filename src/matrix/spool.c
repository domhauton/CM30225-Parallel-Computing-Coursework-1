//
// Created by dominic on 19/11/16.
//

#include "spool.h"
#include "matrixSmoother.h"
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>

typedef struct {
    MatSmoother *matSmoother;
    spool_job_t *next;
} spool_job_t;

typedef struct {
    spool_t smoothPool;
    bool running;
} spool_thread_t;

typedef struct {
    spool_job_t *nextJob;
    sem_t *jobWaitSem;
    pthread_spinlock_t *jobQueueAccessLock;
    pthread_t *threads;
    unsigned int threadCount;
} spool_t;

spool_job_t *spool_job_init(MatSmoother* smoother, ) {
    spool_job_t* spoolJob = malloc(sizeof(spool_job_t));
    spoolJob
}

/* Get current job and increment job pointer.
        Returns NULL if no job available.      */
spool_job_t *smoothing_pool_job_next(spool_t smoothPool) {
    spool_job_t *retJob;
    pthread_spin_lock(smoothPool.jobQueueAccessLock);
    retJob = smoothPool.nextJob;
    if (retJob != NULL) {
        smoothPool.nextJob = retJob->next;
    }
    pthread_spin_unlock(smoothPool.jobQueueAccessLock);
    return retJob;
}

/* Wait until jobs available in pool.
        Return job when available.      */
spool_job_t *smoothing_pool_job_wait(spool_thread_t *smoothThread) {
    sem_wait(smoothThread->smoothPool.jobWaitSem);
    return smoothing_pool_job_next(smoothThread->smoothPool);
}

/* Fetch jobs and execute jobs while running bool is true*/
void *runSmoothingThread(void *voidArgs) {
    spool_thread_t *smoothThread = voidArgs;
    while (smoothThread->running) {
        spool_job_t *smoothJob = smoothing_pool_job_wait(smoothThread);
        if (smoothJob->matSmoother != NULL) {
            MatSmoother_smooth(smoothJob->matSmoother);
        }
        // Todo indicate job done;
    }
    return NULL;
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