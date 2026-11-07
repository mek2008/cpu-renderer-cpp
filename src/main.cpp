#include <iostream>
//#include <vector>

#include "camera.hpp"
#include "BVH.hpp"
#include "objects.hpp"
#include "settings.hpp"


using std::cout;
//using std::vector;




int main(){

    Frame framebuffer_1(1600, 850, true); //unsafe to copy
    Perspective camera_1({0.0f, 0.0f, 0.0f},
        &framebuffer_1,
        1.0f);

    

cout << "pre-prossesing\n";

    Triangle *tris = new Triangle[2];
    tris[0].a.x=13.0f;
    tris[1].a.x=2.0f;

    BVHNode scene(tris, 2);
    scene.sceneNode = true;
    if(BVH_TV) BVHsplit(&scene);




cout << "prossesing\n";


//float antiAliasingconst = 1.0f / static_cast<float>(antialiasing * antialiasing);
//ray generation








cout << "post-prossesing\n";
bmpPrint(&framebuffer_1);



return 0;
}