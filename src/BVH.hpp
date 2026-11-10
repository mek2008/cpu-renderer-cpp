#ifndef BVH_HPP
#define BVH_HPP

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
    bool sceneNode = false;

    BVHNode(Triangle triangles_[], int triangleCount_);

    bool intersectAABB(Ray const &ray);

    ~BVHNode();

    void Clear();
};

Hit closestTriangle(BVHNode const &scene, Ray const &ray);
void nthTriangleSplitX(Triangle triangles[], int const &triangleCount, int const &n);
void nthTriangleSplitY(Triangle triangles[], int const &triangleCount, int const &n);
void nthTriangleSplitZ(Triangle triangles[], int const &triangleCount, int const &n);

void BVHsplit(BVHNode * original);

Hit BVHtraversial(BVHNode const &node, Ray const &ray);

#endif