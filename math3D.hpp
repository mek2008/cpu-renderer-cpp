#ifndef MATH3D
#define MATH3D

#include "math.h"


#define EPSILON 1e-5f
#define PI 3.14159265358979323846f



struct Vec3{ float x, y, z; };
struct Ray{ Vec3 origin, direction; };


inline float dotProduct3v(Vec3 const &a, Vec3 const &b){ //static  //remember no compound literal in c++
return a.x*b.x + a.y*b.y + a.z*b.z;
}

inline Vec3 scalar3v(Vec3 const &vector, float const &scalar){
return {vector.x * scalar, vector.y * scalar, vector.z * scalar}; //        .x = v.x * s, .y = v.y * s, .z = v.z * s
}

inline Vec3 crossProduct3v(Vec3 const &a, Vec3 const &b){
return {a.y*b.z - a.z*b.y, a.z*b.x - a.x*b.z, a.x*b.y - a.y*b.x};
}

inline Vec3 add3v(Vec3 const &a, Vec3 const &b){
return {a.x+b.x, a.y+b.y, a.z+b.z };
}

inline Vec3 subtract3v(Vec3 const &a, Vec3 const &b){ //a-b = change
return {a.x-b.x, a.y-b.y, a.z-b.z };
}

inline Vec3 normalize3v(Vec3 const &vector){
    float length = vector.x*vector.x + vector.y*vector.y + vector.z*vector.z;

    if(length > EPSILON){       length = sqrtf(length);
    return {vector.x/length, vector.y/length, vector.z/length};   }
    
    return {0, 0, 0};
}

inline Vec3 rodriguesRotation(Vec3 const &u, Vec3 const &v, float const &rad)
{
    return add3v(
        add3v(
            scalar3v(v, cosf(rad)),
            scalar3v(crossProduct3v(u, v), sinf(rad))
        ),
        scalar3v(u, dotProduct3v(u, v) * (1.0f - cosf(rad)))
    );
}


#endif