#ifndef OBJECT_H
#define OBJECT_H

#include <stdbool.h>
#include <float.h>
#include "math3d.h"


typedef struct Frame{
int height, width;
Pixel *frame;
}Frame;

void initFrame(Frame * const frame);


typedef struct Hit {
    bool hitTV;
    float t;
    Vec3 cord;
    Vec3 normal; 
    Pixel color;
} Hit;

void initHit(Hit * const hit);

typedef struct tempDotNormals tempDotNormals;

typedef struct tempDot {
    int dotNum;
    int triangles;
    Vec3 normal;   //use double if phong shadign breaks when tiny and enourmus triangles intersect
    tempDotNormals *tempNormals;
    
} tempDot;

typedef struct tempDotNormals {
    Vec3 *normals;   
} tempDotNormals;

void dotAddNormal(Vec3 const normal, tempDot *const dot);


typedef struct TriangleData TriangleData;

typedef struct Triangle {
    Vec3 a, b, c;
    TriangleData *data;
} Triangle;

typedef struct TriangleData {
    Vec3 e1, e2;
    Pixel color;
    Vec3 normal;
    Vec3 apsnormal, bpsnormal, cpsnormal;
} TriangleData;

void initTriangle(Triangle *const tri, TriangleData *data, Vec3 a, Vec3 b, Vec3 c, Pixel color);

void sortTrianglesX(Triangle *const triangles, int const triangleCount);
void sortTrianglesY(Triangle *const triangles, int const triangleCount);
void sortTrianglesZ(Triangle *const triangles, int const triangleCount);

typedef struct BVHNode {
float minX, minY, minZ, maxX, maxY, maxZ;
struct BVHNode *parrentNode;
struct BVHNode *subNodes[8]; //6 extra pointers is worth allocationg memeory all the time
Triangle *triangles;
int triangleCount;
bool endNodeTV;
} BVHNode;
   
void BVHNodeInit(BVHNode *node);

typedef struct Perspective {
Vec3 cordinates;
Vec3 i, j,  k;
float aspect;
float focalLength;
} Perspective;

Ray rayGeneration(Perspective const  *const perspective, int const height, int const width, int const x, int const y);



#endif