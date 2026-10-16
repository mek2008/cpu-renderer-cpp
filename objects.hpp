#ifndef OBJECTS
#define OBJECTS

#include "math.h"
#include <cfloat>

#include "math3D.hpp"
#include "frame_camera.hpp"



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
    // Texture = nullptr;
};

struct Hit{
    bool hitTV = false;
    float t = FLT_MAX;
    Vec3 cord;
    Vec3 normal; 
    Pixel color;
};



#endif