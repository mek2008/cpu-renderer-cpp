#include "frame_camera.hpp"



Frame::Frame(int w_, int h_, bool atv_) : width(w_), height(h_), includeAlpha(atv_) {
    framev = new Pixel[width * height]();
}
Frame::~Frame() {
    delete[] framev;
}


Perspective::Perspective(Vec3 cordinates_, Vec3 i_, Vec3 j_, Vec3 k_, float aspect_, float focalLength_) 
    : cordinates(cordinates_), i(i_), j(j_), k(k_), aspect(aspect_), focalLength(focalLength_) {}

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