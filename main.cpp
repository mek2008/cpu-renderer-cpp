#include <iostream>
//#include <vector>

#include "bmp_writer.hpp"
#include "frame_camera.hpp"
#include "BVH.hpp"
#include "objects.hpp"


using std::cout;
//using std::vector;




int main(){

Frame framebuffer(1600, 850, true); //unsafe to copy


cout << "pre-prossesing\n";

Triangle tris[2];
tris[0].a.x=13.0f;
tris[1].a.x=2.0f;

BVHNode node(tris, 2);
cout << node.max.x;


cout << "prossesing\n";





cout << "post-prossesing\n";
bmpPrint(&framebuffer);



return 0;
}