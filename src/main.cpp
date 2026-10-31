#include <iostream>
//#include <vector>

#include "bmp_writer.hpp"
#include "frame_camera.hpp"
#include "BVH.hpp"
#include "objects.hpp"
#include "settings.hpp"


using std::cout;
//using std::vector;




int main(){

    Frame framebuffer(RENDER_WIDTH, RENDER_HEIGHT, true); //unsafe to copy
    bool bvh = false;
    
    Perspective camera({0.0f, 0.0f, 0.0f},
        static_cast<float>(framebuffer.width) / static_cast<float>(framebuffer.height),
        1.0f);

    

cout << "pre-prossesing\n";

    Triangle *tris = new Triangle[2];
    tris[0].a.x=13.0f;
    tris[1].a.x=2.0f;

    BVHNode scene(tris, 2);
    scene.sceneNode = true;
    cout << scene.max.x;




cout << "prossesing\n";


//float antiAliasingmsq = 1.0f / static_cast<float>(antialiasing * antialiasing);
//ray generation

float antialiasingConst = (1 / static_cast<float>(ANTIALIASING))-0.5;


for(int y = 0; y < framebuffer.height; y++){
for(int x = 0; x < framebuffer.width; x++){

    
    if(ANTIALIASING > 1){  
        for (int suby = 0; suby < ANTIALIASING; suby++){
        for (int subx = 0; subx < ANTIALIASING; subx++){

        float u = (2.0f * (static_cast<float>(x) + (static_cast<float>(suby) * antialiasingConst)) 
                    / static_cast<float>(framebuffer.width) - 1.0f) * camera.aspect; //(2.0f * (x + suby) / framebuffer.width - 1.0f) * camera.aspect;
        float v = (1.0f - 2.0f * (static_cast<float>(y) + (static_cast<float>(subx) * antialiasingConst)) 
                    / static_cast<float>(framebuffer.height));  //1.0f - (2.0f * (y + (subx * antialiasingConst)) / framebuffer.height);
            
        Ray ray;
        ray.origin = camera.cordinates; 
        ray.direction = normalize3v( camera.localToGlobal(u, v, camera.focalLength) );

        Hit hitClosest;
        //Bvh Ray trevarisal to return node used, dont forget to add

        if(!bvh){
        }




        }
        }
    }


    float u = (2.0f * static_cast<float>(x) / static_cast<float>(framebuffer.width) - 1.0f) * camera.aspect;
    float v = (1.0f - 2.0f * static_cast<float>(y) / static_cast<float>(framebuffer.height));
    
    Ray ray;
    ray.origin = camera.cordinates; 
    ray.direction = normalize3v( camera.localToGlobal(u, v, camera.focalLength) );


}
}







cout << "post-prossesing\n";
bmpPrint(&framebuffer);



return 0;
}