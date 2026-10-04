#include "math3d.h"
#include <math.h>


inline float dotProduct3v(const Vec3 a, const Vec3 b){ //static
return a.x*b.x + a.y*b.y + a.z*b.z;
};


inline Vec3 scalar3v(Vec3 const vector, float const scalar){
return (Vec3){vector.x * scalar, vector.y * scalar, vector.z * scalar}; //        .x = v.x * s, .y = v.y * s, .z = v.z * s
}


inline Vec3 crossProduct3v(Vec3 const a, Vec3 const b){
return (Vec3){a.y*b.z - a.z*b.y, a.z*b.x - a.x*b.z, a.x*b.y - a.y*b.x};
}


inline Vec3 add3v(Vec3 const a, Vec3 const b){
return (Vec3){a.x+b.x, a.y+b.y, a.z+b.z };
}


inline Vec3 subtract3v(Vec3 const a, Vec3 const b){ //a-b = change
return (Vec3){a.x-b.x, a.y-b.y, a.z-b.z };
}


inline Vec3 normalize3v(Vec3 const vector){
    float length = vector.x*vector.x + vector.y*vector.y + vector.z*vector.z;

    if(length > EPSILON){       length = sqrtf(length);
    return (Vec3){vector.x/length, vector.y/length, vector.z/length};   }
    
    return (Vec3){0, 0, 0};
}


float degreesToRadians(float const degrees) {return degrees * (PI / 180.0f);}


Vec3 rodriguesRotation(Vec3 const u, Vec3 const v, float const radrotation)
{
    return add3v(
        add3v(
            scalar3v(v, cosf(radrotation)),
            scalar3v(crossProduct3v(u, v), sinf(radrotation))
        ),
        scalar3v(u, dotProduct3v(u, v) * (1.0f - cosf(radrotation)))
    );
}


void rotatetioni(float const radrotation, Vec3 *const i, Vec3 *const j, Vec3 *const k){
*j = rodriguesRotation(*i, *j, radrotation);
*k = rodriguesRotation(*i, *k, radrotation);
}

void rotatetionj(float const radrotation, Vec3 *const j, Vec3 *const i, Vec3 *const k){
*i = rodriguesRotation(*j, *i, radrotation);
*k = rodriguesRotation(*j, *k, radrotation);
}

void rotatetionk(float const radrotation, Vec3 *const k, Vec3 *const i, Vec3 *const j){
*i = rodriguesRotation(*k, *i, radrotation);
*j = rodriguesRotation(*k, *j, radrotation);
}


void reOrthonormalization(Vec3 *const i, Vec3 *const j, Vec3 *const k){
*i = normalize3v(*i);
*j = normalize3v(*j);

*k = normalize3v(crossProduct3v(*i, *j));
*j = normalize3v(crossProduct3v(*k, *i));
}


inline Vec3 localToGlobal(Vec3 const world, Vec3 const i, Vec3 const j, Vec3 const k){
    return (Vec3){
        i.x * world.x + j.x * world.y + k.x * world.z,
        i.y * world.x + j.y * world.y + k.y * world.z,
        i.z * world.x + j.z * world.y + k.z * world.z
    };
}


inline Vec3 localToGlobalXYZ(float const xo, float const yo, float const zo, Vec3 const i, Vec3 const j, Vec3 const k){
    return (Vec3){
        i.x * xo + j.x * yo + k.x * zo,
        i.y * xo + j.y * yo + k.y * zo,
        i.z * xo + j.z * yo + k.z * zo
    };
}


inline Vec3 globalToLocal(Vec3 const world, Vec3 const i, Vec3 const j, Vec3 const k){
    return (Vec3){
        dotProduct3v(world, i),
        dotProduct3v(world, j),
        dotProduct3v(world, k)
    };
}

