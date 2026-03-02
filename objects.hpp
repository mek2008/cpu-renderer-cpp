#pragma once
#include <limits>
#include <vector>

using std::vector;

struct Pixel { unsigned char r, g, b, a; };
static_assert(sizeof(Pixel) == 4, "Pixel must be 4 bytes");

struct Vec3 { float x, y, z; };


struct Rotation3matrix { Vec3 x, y, z; }; //x, y, z

struct locPixelv{ Pixel color; Vec3 cordinates; Vec3 normal; float t; };

struct Ray { Vec3 origin, direction; };

class ClosestP{ public:
locPixelv colCord = {{0, 0, 0, 0}, {0, 0, 0}, {0, 0, 0}, std::numeric_limits<float>::max()};
bool hit = 0;

void closestH(locPixelv newColCord);
};

//for later optimization
//struct RelevantObjects{vector<Objects>;};

//struct RelevantLightSources{vector<LightSource>;};




float dotProduct3v(const Vec3 &a, const Vec3 &b);

Vec3 scelar3v(const Vec3 &vector, float scelar);

Vec3 crossProduct3v(const Vec3 &a, Vec3 const &b);

Vec3 add3v(const Vec3 &a, const Vec3 &b);

Vec3 subtract3v(const Vec3 &a, const Vec3 &b);

Vec3 normalize3v(Vec3 vector);

float degreesToRadians(float degrees);

Vec3 rodriguesRotation(Vec3 u, Vec3 v);



class Perspective{ public:
Vec3 position;
Vec3 i{1, 0, 0}, j{0, 1, 0}, k{0, 0, 1}; //rotation matrix
float focalLength;


Perspective(Vec3 position, float focalLength);

void rotatetioni(float radrotation);

void rotatetionj(float radrotation);

void rotatetionk(float radrotation);

void reOrthonormalization();

Vec3 globaToLocalv(Vec3 world);
Vec3 globaToLocal(float xo, float yo, float zo);

Vec3 localToGlobalv(Vec3 local);
Vec3 localToGlobal(float xo, float yo, float zo);
};


class LightSource{public:
    Vec3 cordinates;
    unsigned char brightness;
    Pixel color;

LightSource(Vec3 cordinates, unsigned char brightness, Pixel color);

};




class Sphere{ public:
Vec3 location;
float radius; 
Pixel color;

Sphere(Vec3 location, float radius, Pixel color);

locPixelv hitRR(Ray ray);
};





Pixel postLighting(const LightSource &light, const locPixelv &point );