#ifndef MULTITHREAD_H
#define MULTITHREAD_H

#include <stdbool.h>
#include <float.h>
#include "math3d.h"
#include "object.h"
#include <pthread.h>
#include <stdatomic.h>

typedef struct RenderWorkData{
    Frame *frame;
    BVHNode *scene;
    Perspective *camera;
    float ambient;

        _Atomic int *jobIndex;
} RenderWorkData;

void *render8rThread(void *arg);


#endif