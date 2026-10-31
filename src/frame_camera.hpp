#ifndef FRAME_CAMERA_HPP
#define FRAME_CAMERA_HPP

#include "math3D.hpp"



struct Pixel{ unsigned char r, g, b, a; }; static_assert(sizeof(Pixel) == 4, "Pixel must be 4 bytes");


struct Frame{
    int width, height;
    Pixel *framev;
    bool includeAlpha;

    Frame(int w_, int h_, bool atv_);

    Pixel& At(int x, int y);
    
    ~Frame();
};


class Perspective{
public:
    Vec3 cordinates;
private:
    Vec3 i={1.0f, 0.0f, 0.0f}, j = {0.0f, 1.0f, 0.0f}, k = {0.0f, 0.0f, 1.0f};
public:
    float aspect;
    float focalLength;

    Perspective(Vec3 cordinates_, float aspect_, float focalLength_);

    void iRotation(float rad);
    void jRotation(float rad);
    void kRotation(float rad);

    void reOrthonormalization();
    Vec3 localToGlobalVec(Vec3 const world);
    Vec3 localToGlobal(float const x, float const y, float const z);

};



#endif