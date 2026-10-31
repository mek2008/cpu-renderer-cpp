#include "BVH.hpp"
#include "math3D.hpp"
#include "objects.hpp"
#include <algorithm>
#include "debug.hpp"



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

BVHNode::~BVHNode(){
    if(!endNodeTV) delete subNodes[0];
    if(!endNodeTV) delete subNodes[1];
    if(sceneNode) delete[] triangles;
}






void BVHsplit(BVHNode * original){
DEBUG_LOG_IF(triangleCount < 16, "sub 8 leaf atempt");

float varianceX = original->max.x - original->min.x;
float varianceY = original->max.y - original->min.y;
float varianceZ = original->max.z - original->min.z;

int firstCount = (original->triangleCount / 8) * 4;
int secondCount = original->triangleCount - firstCount;

Triangle *firstTriangles = nullptr;
Triangle *secondTriangles = nullptr;


if ((varianceX > varianceY) && (varianceX > varianceZ)){
nthTriangleSplitX(original->triangles, original->triangleCount, firstCount);
//firstTriangles = new Triangle[firstCount];
//secondTriangles = new Triangle[secondCount];
}else if((varianceY > varianceX) && (varianceY > varianceZ)){
nthTriangleSplitY(original->triangles, original->triangleCount, firstCount);
}else if((varianceZ > varianceX) && (varianceZ > varianceY)){
nthTriangleSplitZ(original->triangles, original->triangleCount, firstCount);
}else{
nthTriangleSplitX(original->triangles, original->triangleCount, firstCount);
}

original->endNodeTV = false;

firstTriangles = original->triangles;
secondTriangles = &original->triangles[firstCount];

original->subNodes[0] = new BVHNode(firstTriangles, firstCount);
original->subNodes[1] = new BVHNode(secondTriangles, secondCount);

DEBUG_LOG_IF(original->subNodes[0] = nullptr, "bvh allocation fail");
DEBUG_LOG_IF(original->subNodes[1] = nullptr, "bvh allocation fail");


    return;
}

void nthTriangleSplitX(Triangle triangles[], int const &triangleCount, int const &n){
    int pivot = triangleCount -1;
    int left = 0;
    int right = triangleCount -1;
    int point = left;

        while(true){
            for(int i = left; i < right; i++){
                if(triangleXtotal(triangles[pivot]) > triangleXtotal(triangles[i])){
                    Triangle temp = triangles[point];
                    triangles[point] = triangles[i];
                    triangles[i] = temp;

                    point++;
                }
            }
            Triangle temp = triangles[point];
            triangles[point] = triangles[pivot];
            triangles[pivot] = temp;
            if(point == n) return; 
            if(point < n) left = point +1;
            if(point > n) right = point -1;
            pivot = right;
            point = left;
        }
}

void nthTriangleSplitY(Triangle triangles[], int const &triangleCount, int const &n){
    int pivot = triangleCount -1;
    int left = 0;
    int right = triangleCount -1;
    int point = left;

        while(true){
            for(int i = left; i < right; i++){
                if(triangleYtotal(triangles[pivot]) > triangleYtotal(triangles[i])){
                    Triangle temp = triangles[point];
                    triangles[point] = triangles[i];
                    triangles[i] = temp;

                    point++;
                }
            }
            Triangle temp = triangles[point];
            triangles[point] = triangles[pivot];
            triangles[pivot] = temp;
            if(point == n) return; 
            if(point < n) left = point +1;
            if(point > n) right = point -1;
            pivot = right;
            point = left;
        }
}

void nthTriangleSplitZ(Triangle triangles[], int const &triangleCount, int const &n){
    int pivot = triangleCount -1;
    int left = 0;
    int right = triangleCount -1;
    int point = left;

        while(true){
            for(int i = left; i < right; i++){
                if(triangleZtotal(triangles[pivot]) > triangleZtotal(triangles[i])){
                    Triangle temp = triangles[point];
                    triangles[point] = triangles[i];
                    triangles[i] = temp;

                    point++;
                }
            }
            Triangle temp = triangles[point];
            triangles[point] = triangles[pivot];
            triangles[pivot] = temp;
            if(point == n) return; 
            if(point < n) left = point +1;
            if(point > n) right = point -1;
            pivot = right;
            point = left;
        }
}




bool BVHNode::intersectAABB(Ray const &ray){
    float invDirX = 1.0f / ray.direction.x;
    float invDirY = 1.0f / ray.direction.y;
    float invDirZ = 1.0f / ray.direction.z;
    float tmin = -FLT_MAX;
    float tmax = FLT_MAX;

    if(fabsf(ray.direction.x) < 1e-8f){
        if(ray.origin.x < min.x || ray.origin.x > max.x) return false;
    }else{
        float tx1 = (min.x - ray.origin.x) * invDirX;
        float tx2 = (max.x - ray.origin.x) * invDirX;
        tmin = fmaxf(tmin, fminf(tx1, tx2));
        tmax = fminf(tmax, fmaxf(tx1, tx2));
    }

    if(fabsf(ray.direction.y) < 1e-8f){
        if(ray.origin.y < min.y || ray.origin.y > max.y) return false;
    }else{
        float ty1 = (min.y - ray.origin.y) * invDirY;
        float ty2 = (max.y - ray.origin.y) * invDirY;
        tmin = fmaxf(tmin, fminf(ty1, ty2));
        tmax = fminf(tmax, fmaxf(ty1, ty2));
    }

    if(fabsf(ray.direction.z) < 1e-8f){
        if(ray.origin.z < min.z || ray.origin.z > max.z) return false;
    }else{
        float tz1 = (min.z - ray.origin.z) * invDirZ;
        float tz2 = (max.z - ray.origin.z) * invDirZ;
        tmin = fmaxf(tmin, fminf(tz1, tz2));
        tmax = fminf(tmax, fmaxf(tz1, tz2));
    }

    return tmax >= tmin && tmax > 0.0f;
}



Hit BVHtraversial(BVHNode const &node, Ray const &ray){
    if (node.endNodeTV) return closestTriangle(node, ray);
    Hit temp;
    if(node.subNodes[0]->intersectAABB(ray)) temp.ClosestHit(BVHtraversial(*node.subNodes[0], ray));
    if(node.subNodes[1]->intersectAABB(ray)) temp.ClosestHit(BVHtraversial(*node.subNodes[1], ray));

    return temp;
}

Hit closestTriangle(BVHNode const &scene, Ray const &ray){
    Hit hitClosest;

        for(int i = 0; i < scene.triangleCount; i++){
            Hit hit = triangleRayIntersect(ray, &scene.triangles[i]);
            hitClosest.ClosestHit(hit);
        }
    return hitClosest;
}