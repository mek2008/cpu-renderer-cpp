#include <stdio.h>
//#include <string.h>
#include <stdbool.h>
#include <stdlib.h>


#include "math3d.h"
#include "object.h"
#include "multiThread.h"


int main() {
//--------------pre-processing-----------------

printf("%s\n", "initilizing pre-processing"); 

BVHNode scene;
BVHNodeInit(&scene);


//------settings------
Frame mainframe;

mainframe.height = 900;
mainframe.width = 1600;
mainframe.frame = calloc(mainframe.height * mainframe.width, sizeof(Pixel)); //mainframe.frame = malloc(mainframe.height * mainframe.width * sizeof(unsigned char));


Perspective camera = {
    .cordinates  = {0.0f, 0.0f, 0.0f},
    .i = {1.0f, 0.0f, 0.0f},
    .j = {0.0f, 1.0f, 0.0f},
    .k = {0.0f, 0.0f, 1.0f},
    .aspect      = (float)mainframe.width / mainframe.height,
    .focalLength = 1.0f
};
float ambient = 0.1f;

int threadCount = 12;

//----------------processing-------------------
printf("%s\n", "initilizing processing"); 

bool running = true;
pthread_t *threads = malloc(threadCount * sizeof(pthread_t));

while(running){


_Atomic int jobIndex = 0;

RenderWorkData data = {
    .frame = &mainframe,
    .scene = &scene,
    .ambient = ambient,
    .camera = &camera,
    .jobIndex = &jobIndex
};



for(int i = 0; i < threadCount; i++) {
    pthread_create(&threads[i], NULL, render8rThread, &data);
}


for(int i = 0; i < threadCount; i++) {
    pthread_join(threads[i], NULL);
}

running = false;

}



//--------------post-processing----------------
printf("%s\n", "initilizing post-processing"); 

free(mainframe.frame);
mainframe.frame = NULL;
    return 0;
}