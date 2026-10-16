#ifndef FRAME_CAMERA
#define FRAME_CAMERA

#include "math3D.hpp"



struct Pixel{ unsigned char r, g, b, a; }; static_assert(sizeof(Pixel) == 4, "Pixel must be 4 bytes");


struct Frame{
    int width, height;
    Pixel *framev;
    bool includeAlpha;

    Frame(int w_, int h_, bool atv_);
    
    ~Frame();
};


class Perspective{
public:
    Vec3 cordinates;
private:
    Vec3 i, j, k;
public:
    float aspect;
    float focalLength;

    Perspective(Vec3 cordinates_, Vec3 i_, Vec3 j_, Vec3 k_, float aspect_, float focalLength_);

    void iRotation(float rad);
    void jRotation(float rad);
    void kRotation(float rad);

    void reOrthonormalization();

};



#endif