#include <iostream>
#include <cmath>

#include "camera.hpp"
#include "BVH.hpp"
#include "objects.hpp"
#include "settings.hpp"

using std::cout;

// ---------- tiny scene-building helpers(temporary) ----------
static const int MAX_TRIS = 512;
static Triangle *tris = nullptr;
static int triCount = 0;

static void addTri(Vec3 a, Vec3 b, Vec3 c, Pixel color, float reflectivity = 0.0f){
    if(triCount >= MAX_TRIS) return;
    Triangle &t = tris[triCount++];
    t.Triangleinit(a, b, c);
    t.data->color = color;
    t.data->reflectivity = reflectivity;
    t.data->reflectionSpread = 0.0f;
    t.data->density = 1.0f;
    t.data->apsnormal = t.data->normal;
    t.data->bpsnormal = t.data->normal;
    t.data->cpsnormal = t.data->normal;
}

// quad corners must go around the edge in order (a,b,c,d)
static void addQuad(Vec3 a, Vec3 b, Vec3 c, Vec3 d, Pixel color, float reflectivity = 0.0f){
    addTri(a, b, c, color, reflectivity);
    addTri(a, c, d, color, reflectivity);
}

// box centered at c with half-sizes h, rotated around the y axis by yaw
static void addBox(Vec3 c, Vec3 h, float yaw, Pixel color, float reflectivity = 0.0f){
    float cs = cosf(yaw), sn = sinf(yaw);
    Vec3 p[8];
    for(int i = 0; i < 8; i++){
        float x = (i & 1) ? h.x : -h.x;
        float y = (i & 2) ? h.y : -h.y;
        float z = (i & 4) ? h.z : -h.z;
        p[i] = {c.x + x * cs + z * sn, c.y + y, c.z - x * sn + z * cs};
    }
    addQuad(p[0], p[1], p[3], p[2], color, reflectivity);
    addQuad(p[4], p[5], p[7], p[6], color, reflectivity);
    addQuad(p[0], p[2], p[6], p[4], color, reflectivity);
    addQuad(p[1], p[3], p[7], p[5], color, reflectivity);
    addQuad(p[0], p[1], p[5], p[4], color, reflectivity);
    addQuad(p[2], p[3], p[7], p[6], color, reflectivity);
}

// square-based pyramid, base center on y = baseY
static void addPyramid(Vec3 c, float halfBase, float height, Pixel color, float reflectivity = 0.0f){
    Vec3 b0 = {c.x - halfBase, c.y, c.z - halfBase};
    Vec3 b1 = {c.x + halfBase, c.y, c.z - halfBase};
    Vec3 b2 = {c.x + halfBase, c.y, c.z + halfBase};
    Vec3 b3 = {c.x - halfBase, c.y, c.z + halfBase};
    Vec3 apex = {c.x, c.y + height, c.z};
    addTri(b0, b1, apex, color, reflectivity);
    addTri(b1, b2, apex, color, reflectivity);
    addTri(b2, b3, apex, color, reflectivity);
    addTri(b3, b0, apex, color, reflectivity);
    addQuad(b0, b1, b2, b3, color, reflectivity);
}

// split the tree until leaves have fewer than 2*BVH_LEAF_MIN_TRIANGLES triangles
static void buildBVH(BVHNode *node){
    if(node->triangleCount < BVH_LEAF_MIN_TRIANGLES * 2) return;
    BVHsplit(node);
    buildBVH(node->subNodes[0]);
    buildBVH(node->subNodes[1]);
}

int main(){
    // false = no alpha channel in the BMP, so the background is not see-through
    Frame framebuffer_1(1600, 850, false);
    Perspective camera_1({0.0f, 2.2f, -8.0f}, &framebuffer_1, 1.7f);
    camera_1.iRotation(0.2f);              // tilt the camera down a little
    camera_1.antialiasing = 3;             // try 2 or 3 for smooth edges (slower)
    camera_1.transparencyMaxDepth = 6;
    camera_1.reflectionMaxDepth = 4;
    camera_1.reflectionSamplesPerAxis = 3;
    camera_1.reflectionSamplesReduction = 1;

    cout << "pre-prossesing\n";

    tris = new Triangle[MAX_TRIS];

    // checkerboard floor (y = -1), slightly glossy
    const float tile = 2.5f;
    const float floorY = -1.0f;
    for(int ix = 0; ix < 10; ix++){
        for(int iz = 0; iz < 8; iz++){
            float x0 = -12.5f + static_cast<float>(ix) * tile;
            float z0 = -7.5f + static_cast<float>(iz) * tile;
            bool dark = ((ix + iz) % 2) == 0;
            Pixel col = dark ? Pixel{70, 72, 90, 255} : Pixel{215, 215, 220, 255};
            addQuad({x0, floorY, z0}, {x0 + tile, floorY, z0},
                    {x0 + tile, floorY, z0 + tile}, {x0, floorY, z0 + tile}, col, 0.15f);
        }
    }

    // back wall and a mirror on it
    addQuad({-12.5f, floorY, 12.5f}, {12.5f, floorY, 12.5f}, {12.5f, 14.0f, 12.5f}, {-12.5f, 14.0f, 12.5f},
            Pixel{120, 130, 170, 255});
    addQuad({-5.5f, 0.0f, 12.4f}, {5.5f, 0.0f, 12.4f}, {5.5f, 8.0f, 12.4f}, {-5.5f, 8.0f, 12.4f},
            Pixel{235, 240, 255, 255}, 0.85f);

    // objects
    addBox({-3.0f, 0.0f, 3.0f}, {1.0f, 1.0f, 1.0f}, 0.5f, Pixel{210, 70, 50, 255});          // red box
    addBox({-0.2f, -0.4f, 7.0f}, {0.6f, 0.6f, 0.6f}, -0.4f, Pixel{230, 230, 240, 255}, 0.7f); // mirror cube
    addPyramid({3.0f, floorY, 3.5f}, 1.4f, 3.0f, Pixel{40, 170, 150, 255});                   // teal pyramid

    // glass pane (alpha 90 of 255)
    addQuad({-1.3f, floorY, 0.5f}, {1.3f, floorY, 0.5f}, {1.3f, 1.8f, 0.5f}, {-1.3f, 1.8f, 0.5f},
            Pixel{120, 200, 255, 90});

    cout << "triangles: " << triCount << "\n";

    // BVH
    BVHNode scene(tris, triCount);
    scene.sceneNode = true;
    if(BVH_TV) buildBVH(&scene);

    // Lights: warm key light + cool fill light (no distance falloff by default)
    const int lightcount = 2;
    LightSource lights[lightcount] = {
        {{-6.0f, 9.0f, -6.0f}, 3.0f, 2.7f, 2.3f, 1.0f},
        {{ 7.0f, 5.0f, -5.0f}, 0.9f, 1.0f, 1.5f, 1.0f}
    };

    cout << "prossesing\n";
    camera_1.render(&scene, lights, lightcount);

    cout << "post-prossesing\n";
    bmpPrint(camera_1.frame);

    return 0;
}