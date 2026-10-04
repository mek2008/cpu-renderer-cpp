#ifndef RAYINTERSECT_H
#define RAYINTERSECT_H

#include <math.h>
#include "math3d.h"
#include "object.h"
#include <stdbool.h>

Hit triangleRayIntersect(Ray const ray, Triangle const *const tri);

float intersectAABB(BVHNode const * const node, Ray const * const ray);

static void bvhTraverse(BVHNode const * const node, Ray const * const ray, Hit *best);
Hit rayIntersectBVH(BVHNode const * const node, Ray const * const ray);


#endif