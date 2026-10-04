#ifndef MATH3D_H
#define MATH3D_H

#define EPSILON 1e-5f
#define PI 3.14159265358979323846f

typedef struct Vec3 {
    float x, y, z;
} Vec3;
typedef struct Ray { Vec3 origin, direction; } Ray;
typedef struct Pixel { 
    unsigned char r, g, b, a;
} Pixel;

float dotProduct3v(Vec3 const a, Vec3 const b);
Vec3 scalar3v(Vec3 const vector, float const scalar);
Vec3 crossProduct3v(Vec3 const a, Vec3 const b);
Vec3 add3v(Vec3 const a, Vec3 const b);
Vec3 subtract3v(Vec3 const a, Vec3 const b);
Vec3 normalize3v(Vec3 const vector);

float degreesToRadians(float const degrees);

Vec3 rodriguesRotation(Vec3 const axis, Vec3 const vector, float const radians);

void rotatetioni(float const radians, Vec3 *const i, Vec3 *const j, Vec3 *const k);
void rotatetionj(float const radians, Vec3 *const j, Vec3 *const i, Vec3 *const k);
void rotatetionk(float const radians, Vec3 *const k, Vec3 *const i, Vec3 *const j);

void reOrthonormalization(Vec3 *const i, Vec3 *const j, Vec3 *const k);

Vec3 localToGlobal(Vec3 const local, Vec3 const i, Vec3 const j, Vec3 const k);
Vec3 localToGlobalXYZ(float const x, float const y, float const z, Vec3 const i, Vec3 const j, Vec3 const k);
Vec3 globalToLocal(Vec3 const local, Vec3 const i, Vec3 const j, Vec3 const k);

#endif