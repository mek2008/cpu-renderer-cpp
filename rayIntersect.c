#include <stddef.h>
#include <math.h>
#include "math3d.h"
#include "object.h"
#include <stdbool.h>
#include <sys/types.h>
#include "rayIntersect.h"

Hit triangleRayIntersect(Ray const ray, Triangle const * const tri) { //Vec3 b, Vec3 c unused
//Vec3 e1 = subtract3v(b, a); optimization
//Vec3 e2 = subtract3v(c, a); optimization
//Vec3 h = cross(ray.direction, e2);
//float det = dot(e1, h);
Vec3 q = crossProduct3v(tri->data->e1, ray.direction);
float D = dotProduct3v(tri->data->e2, q);

if (fabsf(D) < 0.00001f) return (Hit){false, -1.0f, (Vec3){0,0,0}, (Vec3){0,0,0}, (Pixel){0,0,0,0}};
float invD = 1.0f / D;

Vec3 B = subtract3v(ray.origin, tri->a);

float v = (dotProduct3v(B, q)) * invD; 
if (v < 0.0f || v > 1.0f) return (Hit){false, -1.0f, (Vec3){0,0,0}, (Vec3){0,0,0}, (Pixel){0,0,0,0}};

Vec3 p = crossProduct3v(tri->data->e2, B);

float u = (dotProduct3v(ray.direction, p)) * invD;
if (u < 0.0f || u + v > 1.0f) return (Hit){false, -1.0f, (Vec3){0,0,0}, (Vec3){0,0,0}, (Pixel){0,0,0,0}};

float t = (dotProduct3v(tri->data->e1, p)) * invD;
if(t < 0.0001f) return (Hit){false, -1.0f, (Vec3){0,0,0}, (Vec3){0,0,0}, (Pixel){0,0,0,0}};

Vec3 cord = add3v(ray.origin, scalar3v(ray.direction, t)); 

Vec3 finalNormal = tri->data->normal;

if (dotProduct3v(finalNormal, ray.direction) > 0) finalNormal = scalar3v(finalNormal, -1);
return (Hit){true, t, cord, finalNormal, tri->data->color};
}



float intersectAABB(BVHNode const * const node, Ray const * const ray){
float invDirX = 1.0f / ray->direction.x;
float invDirY = 1.0f / ray->direction.y;
float invDirZ = 1.0f / ray->direction.z;

float tx1 = (node->minX - ray->origin.x) * invDirX;
float tx2 = (node->maxX - ray->origin.x) * invDirX;
float tmin = fminf(tx1, tx2);
float tmax = fmaxf(tx1, tx2);

float ty1 = (node->minY - ray->origin.y) * invDirY;
float ty2 = (node->maxY - ray->origin.y) * invDirY;
tmin = fmaxf(tmin, fminf(ty1, ty2));
tmax = fminf(tmax, fmaxf(ty1, ty2));

float tz1 = (node->minZ - ray->origin.z) * invDirZ;
float tz2 = (node->maxZ - ray->origin.z) * invDirZ;
tmin = fmaxf(tmin, fminf(tz1, tz2));
tmax = fminf(tmax, fmaxf(tz1, tz2));

if(tmax >= tmin && tmax > 0.0f){
    return (tmin < 0.0f) ? 0.0f : tmin;
}
return -1.0f;
}



//check  if a node intersects, then its children and so on, for every valid check its trinagles or use the closest t value


static void bvhTraverse(BVHNode const * const node, Ray const * const ray, Hit *best){
float tBox = intersectAABB(node, ray);
if(tBox < 0.0f || tBox > best->t) return;

if(node->endNodeTV){
    for(int i = 0; i < node->triangleCount; i++){
        Hit h = triangleRayIntersect(*ray, &node->triangles[i]);
        if(h.hitTV && h.t < best->t) *best = h;
    }
    return;
}
for(int i = 0; i < 8; i++) if(node->subNodes[i]) bvhTraverse(node->subNodes[i], ray, best);
}

Hit rayIntersectBVH(BVHNode const * const node, Ray const * const ray){
    Hit best;
    initHit(&best);
    bvhTraverse(node, ray, &best);
    return best;
}



Hit shadowRay(BVHNode const * const node, Ray const * const ray){
Hit hit;


return hit;
};
