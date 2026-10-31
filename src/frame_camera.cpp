#include "frame_camera.hpp"



Frame::Frame(int w_, int h_, bool atv_) : width(w_), height(h_), includeAlpha(atv_) {
    framev = new Pixel[width * height]();
}
Pixel& Frame::At(int x, int y){
return framev[y * width + x];
}
Frame::~Frame() {
    delete[] framev;
}


Perspective::Perspective(Vec3 cordinates_, float aspect_, float focalLength_) 
    : cordinates(cordinates_), aspect(aspect_), focalLength(focalLength_) {}

void Perspective::iRotation(float radrotation){
    j = rodriguesRotation(i, j, radrotation);
    k = rodriguesRotation(i, k, radrotation);
}
void Perspective::jRotation(float radrotation){
    i = rodriguesRotation(j, i, radrotation);
    k = rodriguesRotation(j, k, radrotation);
}
void Perspective::kRotation(float radrotation){
    i = rodriguesRotation(k, i, radrotation);
    j = rodriguesRotation(k, j, radrotation);
}

void Perspective::reOrthonormalization(){
    i = normalize3v(i);
    j = normalize3v(j);
    k = crossProduct3v(i, j);
    j = crossProduct3v(k, i); 
}

Vec3 Perspective::localToGlobalVec(Vec3 const world){
    return{
        i.x * world.x + j.x * world.y + k.x * world.z,
        i.y * world.x + j.y * world.y + k.y * world.z,
        i.z * world.x + j.z * world.y + k.z * world.z
    };
}

Vec3 Perspective::localToGlobal(float const x, float const y, float const z){
    return{
        i.x * x + j.x * y + k.x * z,
        i.y * x + j.y * y + k.y * z,
        i.z * x + j.z * y + k.z * z
    };
}