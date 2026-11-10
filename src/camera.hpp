#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "BVH.hpp"
#include "math3D.hpp"

enum class ToneMapping{
    Clamp,
    Reinhard,
//    Normalize
};



class Perspective{
public:
    Vec3 cordinates;
    Frame *frame;
    float focalLength = 1.0f;
    int antialiasing = 1; //1 is off
    float ambiance = 0.1f;
    int transparencyMaxDepth, reflectionMaxDepth, reflectionSamplesPerAxis, reflectionSamplesReduction;
    ToneMapping mode = ToneMapping::Reinhard;
private:
    Vec3 i={1.0f, 0.0f, 0.0f}, j = {0.0f, 1.0f, 0.0f}, k = {0.0f, 0.0f, 1.0f};
public:

    Perspective(Vec3 cordinates_, Frame * frame_, float focalLength_);

    void iRotation(float rad);
    void jRotation(float rad);
    void kRotation(float rad);

    void reOrthonormalization();
    Vec3 localToGlobalVec(Vec3 const world) const;
    Vec3 localToGlobal(float const x, float const y, float const z) const;
    
    void render(BVHNode const * const scene, LightSource const * const lights, int const &lightcount);


    floatPixel recursiveRay(BVHNode const * const scene, LightSource const * const lights, int const &lightcount, Ray const &ray, int const &currentORC, int const &currentRRC);

};



#endif