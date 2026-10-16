#include "objects.hpp"


void Triangle::Triangleinit(Vec3 a_, Vec3 b_, Vec3 c_){
    a = a_; b = b_; c = c_;

    data = new TriangleData;
    data->e1 = subtract3v(b, a);
    data->e2 = subtract3v(c, a);
    data->normal = normalize3v(crossProduct3v(data->e1, data->e2));
}  //(TrianglePoints *p, TriangleData *d) : point(p), data(d)


Hit triangleRayIntersect(Ray const &ray, Triangle const * const tri) {
    //Triangle const *tri = &triangles[4];
//Vec3 e1 = subtract3v(b, a); optimization
//Vec3 e2 = subtract3v(c, a); optimization
//Vec3 h = cross(ray.direction, e2);
//float det = dot(e1, h);
Hit failHit = {false, -1.0f, {0,0,0}, {0,0,0}, {0,0,0,0}};

    Vec3 q = crossProduct3v(tri->data->e1, ray.direction);
    float D = dotProduct3v(tri->data->e2, q);

    if (fabsf(D) < EPSILON) return failHit;
    float invD = 1.0f / D;

    Vec3 B = subtract3v(ray.origin, tri->a);

    float v = (dotProduct3v(B, q)) * invD; 
    if (v < 0.0f || v > 1.0f) return failHit;

    Vec3 p = crossProduct3v(tri->data->e2, B);

    float u = (dotProduct3v(ray.direction, p)) * invD;
    if (u < 0.0f || u + v > 1.0f) return failHit;

    float t = (dotProduct3v(tri->data->e1, p)) * invD;
    if(t < EPSILON) return failHit;

    Vec3 cord = add3v(ray.origin, scalar3v(ray.direction, t)); 

    Vec3 finalNormal = tri->data->normal;

    if (dotProduct3v(finalNormal, ray.direction) > 0) finalNormal = scalar3v(finalNormal, -1);
    return {true, t, cord, finalNormal, tri->data->color};
}
