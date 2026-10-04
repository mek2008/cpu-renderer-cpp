#include <stdlib.h>
#include <stdio.h>
#include "math3d.h"
#include <math.h>
#include <string.h>
#include "object.h"


void initFrame(Frame * const frame){
frame->frame = calloc(frame->height * frame->width, sizeof(Pixel));
}

void initHit(Hit * const hit){
    hit->hitTV = false;
    hit->t = FLT_MAX;
    hit->cord = (Vec3){0.0f, 0.0f, 0.0f};
    hit->normal = (Vec3){0.0f, 0.0f, 0.0f}; 
    hit->color = (Pixel){0, 0, 0, 0};
}


void initTriangle(Triangle * const tri, TriangleData *data, Vec3 a, Vec3 b, Vec3 c, Pixel color){
    tri->a = a;
    tri->b = b;
    tri->c = c;

    data->e1 = subtract3v(b, a);
    data->e2 = subtract3v(c, a);
    data->normal = normalize3v(crossProduct3v(data->e1, data->e2));
    data->color = color;

    tri->data = data;
}



void dotAddNormal(Vec3 const normal, tempDot * const dot){
dot->triangles++;
Vec3 *temp = realloc(dot->tempNormals->normals, dot->triangles * sizeof(Vec3));

if (temp == NULL) {    dot->triangles--;    return;    }

dot->tempNormals->normals = temp;
dot->tempNormals->normals[dot->triangles - 1] = normal;
}



Ray rayGeneration(Perspective const *const perspective, int const height, int const width, int const screenx, int const screeny){
float u  = (2.0f * (screenx + 0.5f) / width - 1.0f) * perspective->aspect;
float v = (1.0f - 2.0f * (screeny + 0.5f) / height);

return (Ray){
perspective->cordinates,
normalize3v( localToGlobalXYZ(u, v, perspective->focalLength, perspective->i, perspective->j ,perspective->k))
};
}


void BVHNodeInit(BVHNode *node){
node->minX = FLT_MAX;
node->minY = FLT_MAX;
node->minZ = FLT_MAX;
node->maxX = -FLT_MAX;
node->maxY = -FLT_MAX;
node->maxZ = -FLT_MAX;

node->parrentNode = NULL;
node->triangles = NULL;
node->triangleCount = 0;
node->endNodeTV = true;

for (int i = 0; i < 8; i++) { node->subNodes[i] = NULL; }
}

void BVHNodeAxis(BVHNode *node){
for(int i = 0; i < node->triangleCount; i++){

node->maxX = fmaxf(node->maxX, fmaxf(node->triangles[i].a.x, fmaxf(node->triangles[i].b.x, node->triangles[i].c.x)));
node->minX = fminf(node->minX, fminf(node->triangles[i].a.x, fminf(node->triangles[i].b.x, node->triangles[i].c.x)));

node->maxY = fmaxf(node->maxY, fmaxf(node->triangles[i].a.y, fmaxf(node->triangles[i].b.y, node->triangles[i].c.y)));
node->minY = fminf(node->minY, fminf(node->triangles[i].a.y, fminf(node->triangles[i].b.y, node->triangles[i].c.y)));

node->maxZ = fmaxf(node->maxZ, fmaxf(node->triangles[i].a.z, fmaxf(node->triangles[i].b.z, node->triangles[i].c.z)));
node->minZ = fminf(node->minZ, fminf(node->triangles[i].a.z, fminf(node->triangles[i].b.z, node->triangles[i].c.z)));
}
}


void BVHsplit(BVHNode * original){
BVHNodeAxis(original);
if(original->triangleCount < 16){
    printf("%s\n", "BVH with sub 16 triangles atempted"); 
    return;
}

float varianceX = original->maxX - original->minX;
float varianceY = original->maxY - original->minY;
float varianceZ = original->maxZ - original->minZ;

if(varianceX > varianceY && varianceX > varianceZ){ sortTrianglesX(original->triangles, original->triangleCount);}
else if(varianceX < varianceY && varianceY > varianceZ){ sortTrianglesY(original->triangles, original->triangleCount);}
else if(varianceX < varianceZ && varianceY < varianceZ){ sortTrianglesZ(original->triangles, original->triangleCount);}
else{printf("%s\n", "equal variance or error"); sortTrianglesX(original->triangles, original->triangleCount);}

BVHNode *first = malloc(sizeof(BVHNode));

if (first == NULL) {
printf("%s\n", "first faild to allocate");
    return;
}

BVHNodeInit(first);
BVHNode *second = malloc(sizeof(BVHNode));

if (second == NULL) {
printf("%s\n", "second faild to allocate");
    return;
}

BVHNodeInit(second);

double half = (double)original->triangleCount / 2.0;
size_t firstCount = (size_t)floor(half);
size_t secondCount = (size_t)ceil(half);

first->triangles = malloc(firstCount * sizeof(Triangle));
second->triangles = malloc(secondCount * sizeof(Triangle));

memcpy(first->triangles, original->triangles, firstCount * sizeof(Triangle));
memcpy(second->triangles, original->triangles + firstCount, secondCount * sizeof(Triangle));

first->triangleCount = firstCount;
second->triangleCount = secondCount;
//malloc(sizeof(BVHNode) * 2); ptr+1; if you decide to not use [8]
BVHNodeAxis(first);
BVHNodeAxis(second);

original->subNodes[0] = first;
original->subNodes[1] = second;

original->subNodes[0]->parrentNode = original;
original->subNodes[1]->parrentNode = original;
original->triangleCount = 0;
original->endNodeTV = false;
return;
}



void sortTrianglesX(Triangle *const triangles, int const triangleCount){
Triangle temp;
bool clear = false;
int passes = 0;

while(!clear){
    clear = true;
    for(int i = 0; i < ((triangleCount - passes) -1); i++){
        if((triangles[i].a.x + triangles[i].b.x + triangles[i].c.x) > (triangles[i+1].a.x + triangles[i+1].b.x + triangles[i+1].c.x)){
            clear = false;
            temp = triangles[i];
            triangles[i] = triangles[i+1];
            triangles[i+1] = temp;
        }
    }
passes++;
}
}


void sortTrianglesY(Triangle *triangles, int triangleCount){
Triangle temp;
bool clear = false;
int passes = 0;

while(!clear){
    clear = true;
    for(int i = 0; i < ((triangleCount - passes) -1); i++){
        if((triangles[i].a.y + triangles[i].b.y + triangles[i].c.y) > (triangles[i+1].a.y + triangles[i+1].b.y + triangles[i+1].c.y)){
            clear = false;
            temp = triangles[i];
            triangles[i] = triangles[i+1];
            triangles[i+1] = temp;
        }
    }
passes++;
}
}


void sortTrianglesZ(Triangle *triangles, int triangleCount){
Triangle temp;
bool clear = false;
int passes = 0;

while(!clear){
    clear = true;
    for(int i = 0; i < ((triangleCount - passes) -1); i++){
        if((triangles[i].a.z + triangles[i].b.z + triangles[i].c.z) > (triangles[i+1].a.z + triangles[i+1].b.z + triangles[i+1].c.z)){
            clear = false;
            temp = triangles[i];
            triangles[i] = triangles[i+1];
            triangles[i+1] = temp;
        }
    }
passes++;
}
}




