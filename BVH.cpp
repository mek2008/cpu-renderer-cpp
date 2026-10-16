#include "BVH.hpp"
#include "objects.hpp"
#include <algorithm>



BVHNode::BVHNode(Triangle triangles_[], int triangleCount_){
    triangleCount = triangleCount_;
    triangles = triangles_;

    for(int i = 0; i < triangleCount; i++){
        max.x = std::max(std::max(max.x, triangles[i].c.x), std::max(triangles[i].a.x, triangles[i].b.x));
        max.y = std::max(std::max(max.y, triangles[i].c.y), std::max(triangles[i].a.y, triangles[i].b.y));
        max.z = std::max(std::max(max.z, triangles[i].c.z), std::max(triangles[i].a.z, triangles[i].b.z));

        min.x = std::min(std::min(min.x, triangles[i].c.x), std::min(triangles[i].a.x, triangles[i].b.x));
        min.y = std::min(std::min(min.y, triangles[i].c.y), std::min(triangles[i].a.y, triangles[i].b.y));
        min.z = std::min(std::min(min.z, triangles[i].c.z), std::min(triangles[i].a.z, triangles[i].b.z));
    }
}


float BVHNode::intersectAABB(Ray const &ray, Hit &hit){
float invDirX = 1.0f / ray.direction.x;
float invDirY = 1.0f / ray.direction.y;
float invDirZ = 1.0f / ray.direction.z;

float tx1 = (min.x - ray.origin.x) * invDirX;
float tx2 = (max.x - ray.origin.x) * invDirX;
float tmin = fminf(tx1, tx2);
float tmax = fmaxf(tx1, tx2);

float ty1 = (min.y - ray.origin.y) * invDirY;
float ty2 = (max.y - ray.origin.y) * invDirY;
tmin = fmaxf(tmin, fminf(ty1, ty2));
tmax = fminf(tmax, fmaxf(ty1, ty2));

float tz1 = (min.z - ray.origin.z) * invDirZ;
float tz2 = (max.z - ray.origin.z) * invDirZ;
tmin = fmaxf(tmin, fminf(tz1, tz2));
tmax = fminf(tmax, fmaxf(tz1, tz2));

if(tmax >= tmin && tmax > 0.0f){
    return (tmin < 0.0f) ? 0.0f : tmin;
}
return -1.0f;
}