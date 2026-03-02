#include <algorithm>
#include "objects.hpp"
#include <cmath>




void ClosestP::closestH(locPixelv newColCord){
if (colCord.t > newColCord.t && newColCord.t > 0) {
colCord = newColCord;
}
}



float dotProduct3v(const Vec3 &a, const Vec3 &b){
return a.x*b.x + a.y*b.y + a.z*b.z;
}

Vec3 scelar3v(const Vec3 &vector, float scelar){
return{vector.x * scelar, vector.y * scelar, vector.z * scelar};
}

Vec3 crossProduct3v(const Vec3 &a, Vec3 const &b){
return {a.y*b.z - a.z*b.y, a.z*b.x - a.x*b.z, a.x*b.y - a.y*b.x};
}
Vec3 add3v(const Vec3 &a, const Vec3 &b){
return {a.x+b.x, a.y+b.y, a.z+b.z };
}
Vec3 subtract3v(const Vec3 &a, const Vec3 &b){ //a-b = change
return {a.x-b.x, a.y-b.y, a.z-b.z };
}

Vec3 normalize3v(Vec3 vector) {
float lenght = sqrt(vector.x*vector.x + vector.y*vector.y + vector.z*vector.z);
if(lenght != 0){
return {vector.x/lenght, vector.y/lenght, vector.z/lenght};
} else {return {0, 0, 0};}
}


float degreesToRadians(float degrees) {return degrees * (3.141592 / 180.0f);}

Vec3 rodriguesRotation(const Vec3 &u, const Vec3 &v, float radrotation){ 
//v’ = v * cosθ + (u × v) * sinθ + u * (u · v) * (1 − cosθ)
 return add3v(
        add3v(
            scelar3v(v, cos(radrotation)), 
            scelar3v(crossProduct3v(u, v), sin(radrotation))
        ), 
        scelar3v(u, dotProduct3v(u, v) * (1 - cos(radrotation)))
);
} 








Perspective::Perspective(Vec3 position, float focalLength) : position(position), focalLength(focalLength) {}

void Perspective::rotatetioni(float radrotation){
j = rodriguesRotation(i, j, radrotation);
k = rodriguesRotation(i, k, radrotation);
}
void Perspective::rotatetionj(float radrotation){
i = rodriguesRotation(j, i, radrotation);
k = rodriguesRotation(j, k, radrotation);
}
void Perspective::rotatetionk(float radrotation){
i = rodriguesRotation(k, i, radrotation);
j = rodriguesRotation(k, j, radrotation);
}


void Perspective::reOrthonormalization(){
    i = normalize3v(i);
    j = normalize3v(j);
    k = crossProduct3v(i, j);
    j = crossProduct3v(k, i); 
}

// Vec3 version
Vec3 Perspective::globaToLocalv(Vec3 world) {
    return {
        i.x * world.x + j.x * world.y + k.x * world.z,
        i.y * world.x + j.y * world.y + k.y * world.z,
        i.z * world.x + j.z * world.y + k.z * world.z
    };
}
// Float version
Vec3 Perspective::globaToLocal(float xo, float yo, float zo) {
    return {
        i.x * xo + j.x * yo + k.x * zo,
        i.y * xo + j.y * yo + k.y * zo,
        i.z * xo + j.z * yo + k.z * zo
    };
}

// Vec3 version
Vec3 Perspective::localToGlobalv(Vec3 local) {
    return {
        i.x * local.x + j.x * local.y + k.x * local.z,
        i.y * local.x + j.y * local.y + k.y * local.z,
        i.z * local.x + j.z * local.y + k.z * local.z
    };
}
// Float version
Vec3 Perspective::localToGlobal(float xo, float yo, float zo) {
    return {
        i.x * xo + j.x * yo + k.x * zo,
        i.y * xo + j.y * yo + k.y * zo,
        i.z * xo + j.z * yo + k.z * zo
    };
}


LightSource::LightSource(Vec3 cordinates, unsigned char brightness, Pixel color)   :   cordinates(cordinates), brightness(brightness), color(color)   {}






Sphere::Sphere(Vec3 location, float radius, Pixel color)    :    location(location), radius(radius), color(color)    {}

locPixelv Sphere::hitRR(Ray ray){
//t = [−(a·b) ± sqrt((a·b)² − (b·b)((a·a) − r²))] / (b·b)
Vec3 oc = subtract3v(ray.origin, location);
float a = dotProduct3v(ray.direction, ray.direction );
float b = 2.0f * (dotProduct3v(oc, ray.direction)) ;
float c = dotProduct3v(oc, oc) - (radius * radius);

float discrimenant = b*b - 4.0f*a*c;

if (discrimenant <= 0.001f) {
return {{0,0,0,0}, {0,0,0}, {0,0,0}, -1};
}

float t1 = (-b - sqrt(discrimenant)) / (2.0f * a);
float t2 = (-b + sqrt(discrimenant)) / (2.0f * a);

Vec3 pointCordinates = add3v(ray.origin, scelar3v(ray.direction, std::min(t1, t2)));
Vec3 normal = subtract3v(pointCordinates, location);


return {color, pointCordinates, normal, std::min(t1, t2)}; //color, cordinates, t
}





Pixel postLighting(const LightSource &light, const locPixelv &point ){
Vec3 lightNormal = normalize3v(subtract3v(light.cordinates, point.cordinates));

float lightAlignment = std::max(dotProduct3v(point.normal, lightNormal), 0.1f);


return {(unsigned char)(point.color.r * lightAlignment),
        (unsigned char)(point.color.g * lightAlignment),
        (unsigned char)(point.color.b * lightAlignment), point.color.a };
}   

