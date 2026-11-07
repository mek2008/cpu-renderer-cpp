#ifndef OBJECTS_HPP
#define OBJECTS_HPP

#include "math.h"
#include <cfloat>

#include "math3D.hpp"
#include "frame.hpp"


struct TriangleData;

struct Triangle{
    Vec3 a, b, c;
    TriangleData *data = nullptr;

    void Triangleinit(Vec3 a_, Vec3 b_, Vec3 c_); //no constructor for arrays
};

struct TriangleData{
    Vec3 e1, e2;
    Pixel color;
    Vec3 normal;
    Vec3 apsnormal, bpsnormal, cpsnormal;
    float density = 1.0f;
    // Texture = nullptr;
};

float triangleXtotal(Triangle const &triangle);
float triangleYtotal(Triangle const &triangle);
float triangleZtotal(Triangle const &triangle);

struct Hit{
    bool hitTV = false;
    float t = FLT_MAX;
    Vec3 cord;
    Vec3 normal; 
    Pixel color;
    // Texture = nullptr;

    void ClosestHit(Hit hit);
};

Hit triangleRayIntersect(Ray const &ray, Triangle const * const tri);

struct LightSource{
    Vec3 cord;
    float r, g, b, a; //intended max is 1 but more could be used for intresting effects
    bool distancelossTV = false;
    float distancelossConst = 1;
};




#endif