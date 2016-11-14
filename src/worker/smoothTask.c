//
// Created by dominic on 13/11/16.
//

#include <malloc.h>
#include "../matrix/matrixSmoother.h"
#include "smoothTask.h"

enum taskType {SMOOTH, AGGREGATE};

typedef struct smoothTask {
    MatSmoother *smootherInput;
    bool exceedDiff;
    enum taskType taskType;
    int dependencies;
    SmoothTask *dependantTasks;
    int dependantTaskCount;
} SmoothTask;


SmoothTask* SmoothTask_initSmooth( MatSmoother *matSmoother,
                            int dependencies,
                            SmoothTask *dependantTasks,
                            int dependantTaskCount) {
    SmoothTask *smoothTask = malloc(sizeof(SmoothTask));
    smoothTask->taskType = SMOOTH;
    smoothTask->smootherInput = matSmoother;
    smoothTask->dependencies = dependencies;
    smoothTask->dependantTasks = dependantTasks;
    smoothTask->dependantTaskCount = dependantTaskCount;
    return smoothTask;
}

SmoothTask* SmoothTask_initAggregate(int dependencies,
                                   SmoothTask *dependantTasks,
                                   int dependantTaskCount) {
    SmoothTask *smoothTask = malloc(sizeof(SmoothTask));
    smoothTask->taskType = AGGREGATE;
    smoothTask->exceedDiff = false;
    smoothTask->dependencies = dependencies;
    smoothTask->dependantTasks = dependantTasks;
    smoothTask->dependantTaskCount = dependantTaskCount;
    return smoothTask;
}

void SmoothTask_decrementDependencies(SmoothTask *smoothTask, bool diffExceeded) {
    //FIXME Should lock access while writing
    if(smoothTask->taskType == AGGREGATE) {
        smoothTask->exceedDiff = smoothTask->exceedDiff || diffExceeded;
    }
    smoothTask->dependencies--;
}

int SmoothTask_remainingDependencies(SmoothTask *smoothTask) {
    return smoothTask->dependencies;
}

bool SmoothTask_execute(SmoothTask* smoothTask) {
    switch(smoothTask->taskType) {
        case AGGREGATE:
            return !(smoothTask->exceedDiff);
        case SMOOTH:
            MatSmoother_smooth(smoothTask->smootherInput);
            smoothTask->exceedDiff = MatSmoother_exceedDiff(smoothTask->smootherInput);
            MatSmoother_destroy(smoothTask->smootherInput);
            return false;
        default:
            printf("Error executing undefined task.");
            return false;
    }
}

void SmoothTask_destroy(SmoothTask* smoothTask) {
    SmoothTask *tmpPtr = smoothTask->dependantTasks;
    while(smoothTask->dependantTaskCount > 0) {
        SmoothTask_decrementDependencies(tmpPtr++, smoothTask->exceedDiff);
        smoothTask->dependantTaskCount--;
    }
    free(smoothTask->dependantTasks);
    free(smoothTask);
}