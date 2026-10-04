#include <pthread.h>
#include <stdatomic.h>
#include "object.h"
#include "multiThread.h"
#include "rayIntersect.h"

void* render8rThread(void *arg){
    
RenderWorkData *data = arg;


int startY = atomic_fetch_add(data->jobIndex, 8);
if(startY >= data->frame->height) return NULL;
int endY = startY + 8;
if(endY > data->frame->height) endY = data->frame->height;

for (int y = startY; y < endY; y++){
    for (int x = 0; x < data->frame->width; x++){
        Ray ray = rayGeneration(data->camera, data->frame->height, data->frame->width, x, y);
        Triangle tri;
        Hit hit = triangleRayIntersect(ray, &tri);
    }



}

return NULL;

}

