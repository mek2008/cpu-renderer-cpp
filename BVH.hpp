#ifndef BVH
#define BVH

#include "math.h"
#include "objects.hpp"


struct BVHNode {
    Vec3 max = {-FLT_MAX, -FLT_MAX, -FLT_MAX};
    Vec3 min = {FLT_MAX, FLT_MAX, FLT_MAX};
    //struct BVHNode *parrentNode;
    BVHNode *subNodes[2] = {nullptr, nullptr};
    Triangle *triangles = nullptr;
    int triangleCount = 0;
    bool endNodeTV = true;

    BVHNode(Triangle triangles_[], int triangleCount_);

    float intersectAABB(Ray const &ray, Hit &hit);
};


#endif