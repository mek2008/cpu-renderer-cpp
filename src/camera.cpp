#include "camera.hpp"
#include "BVH.hpp"
#include "debug.hpp"
#include "frame.hpp"
#include "math3D.hpp"
#include "objects.hpp"
#include "settings.hpp"
//#include "BVH.hpp"


Perspective::Perspective(Vec3 cordinates_, Frame * frame_, float focalLength_) 
    : cordinates(cordinates_), frame(frame_), focalLength(focalLength_) {}

void Perspective::iRotation(float radrotation){
    j = rodriguesRotation(i, j, radrotation);
    k = rodriguesRotation(i, k, radrotation);
}
void Perspective::jRotation(float radrotation){
    i = rodriguesRotation(j, i, radrotation);
    k = rodriguesRotation(j, k, radrotation);
}
void Perspective::kRotation(float radrotation){
    i = rodriguesRotation(k, i, radrotation);
    j = rodriguesRotation(k, j, radrotation);
}

void Perspective::reOrthonormalization(){
    i = normalize3v(i);
    j = normalize3v(j);
    j = crossProduct3v(k, i); 
}

Vec3 Perspective::localToGlobalVec(Vec3 const world) const{
    return{
        i.x * world.x + j.x * world.y + k.x * world.z,
        i.y * world.x + j.y * world.y + k.y * world.z,
        i.z * world.x + j.z * world.y + k.z * world.z
    };
}

Vec3 Perspective::localToGlobal(float const x, float const y, float const z) const{
    return{
        i.x * x + j.x * y + k.x * z,
        i.y * x + j.y * y + k.y * z,
        i.z * x + j.z * y + k.z * z
    };
}


floatPixel shadow(Hit const &hit, BVHNode const * const scene, LightSource const * const lights, int const &lightcount,
             ToneMapping mode, float ambiance);


void Perspective::render(BVHNode const * const scene, LightSource const * const lights, int const &lightcount){
    float antialiasingConst = 1.0f / static_cast<float>(antialiasing);
    float antialiasingConstSQ = 1.0f / (static_cast<float>(antialiasing) *static_cast<float>(antialiasing));


for(int y = 0; y < frame->height; y++){
for(int x = 0; x < frame->width; x++){

    floatPixel pixelColor;
    
        for (int suby = 0; suby < antialiasing; suby++){
        for (int subx = 0; subx < antialiasing; subx++){
        
            float x_sample = static_cast<float>(x) + (static_cast<float>(subx) + 0.5f) * antialiasingConst;
            float y_sample = static_cast<float>(y) + (static_cast<float>(suby) + 0.5f) * antialiasingConst;

            float u = (2.0f * (x_sample / static_cast<float>(frame->width)) - 1.0f) * frame->aspect;
            float v = 1.0f - 2 * (y_sample / static_cast<float>(frame->height));
                    
            
            Ray ray;
            ray.origin = cordinates; 
            ray.direction = normalize3v( localToGlobal(u, v, focalLength) );

            Hit closestHit = BVHtraversial(*scene, ray);
            floatPixel color = recurisveRay(closestHit, scene, lights, lightcount, ray, reflectionRayCount, reflectionRayCount);
            
            pixelColor.r += color.r * antialiasingConstSQ;
            pixelColor.g += color.g * antialiasingConstSQ;
            pixelColor.b += color.b * antialiasingConstSQ;
            pixelColor.a += color.a * antialiasingConstSQ;
        }
        }

        if(pixelColor.r > 255.0f) pixelColor.r = 255.0f;
        if(pixelColor.g > 255.0f) pixelColor.g = 255.0f;
        if(pixelColor.b > 255.0f) pixelColor.b = 255.0f;
        if(pixelColor.a > 255.0f) pixelColor.a = 255.0f;

    frame->At(x, y) = {static_cast<unsigned char>(pixelColor.r), static_cast<unsigned char>(pixelColor.g), 
    static_cast<unsigned char>(pixelColor.b), static_cast<unsigned char>(pixelColor.a)};

}
}

}





floatPixel Perspective::recurisveRay(Hit const &hit, BVHNode const * const scene, LightSource const * const lights, int const &lightcount, Ray const &ray, int const &currentORC, int const &currentRRC){
    floatPixel pixelColor;


    if(!hit.hitTV) return{0.0f, 0.0f, 0.0f, 0.0f};

    floatPixel shadowColor = shadow(hit, scene, lights, lightcount, mode, ambiance);

    float alpha = static_cast<float>(hit.color.a) / 255.0f;
    pixelColor.r += shadowColor.r * alpha;
    pixelColor.g += shadowColor.g * alpha;
    pixelColor.b += shadowColor.b * alpha;
    pixelColor.a += shadowColor.a * alpha;

    if(currentORC > 0 && alpha < 1.0f){
        Ray opacityRay;
        opacityRay.origin = hit.cord;
        opacityRay.direction = ray.direction;

        int nextORC = currentORC - 1;
        floatPixel opacityHit = recurisveRay(hit, scene, lights, lightcount, opacityRay, nextORC, currentRRC);

        float transparency = 1.0f - alpha;
        pixelColor.r += opacityHit.r * transparency;
        pixelColor.g += opacityHit.g * transparency;
        pixelColor.b += opacityHit.b * transparency;
        pixelColor.a += opacityHit.a * transparency;
    }

    if(currentRRC > 0){

        Vec3 reflectionDir = rodriguesRotation(hit.normal, scalar3v(ray.direction, -1.0f), PI);
        Vec3 temp = crossProduct3v(reflectionDir, normalize3v(hit.cord));
        if(dotProduct3v(temp, temp) < EPSILON * EPSILON) temp = crossProduct3v(reflectionDir, hit.normal);
        Vec3 temp1 = normalize3v(temp);
        Vec3 temp2 = normalize3v(crossProduct3v(reflectionDir, temp1));

        int RRCM = reflectionRayCount - reflectionRayCountminimizer * (reflectionRecursiuonCount - currentRRC);
        if(RRCM < 1) RRCM = 1;
        float RRCC = 1.0f / static_cast<float>(RRCM * RRCM);

        for(int y = 0; y < RRCM; y++){
            for(int x = 0; x < RRCM; x++){
                float sampleY = ((static_cast<float>(y) + 0.5f) / static_cast<float>(RRCM) - 0.5f) * reflectionspread;
                float sampleX = ((static_cast<float>(x) + 0.5f) / static_cast<float>(RRCM) - 0.5f) * reflectionspread;

                Ray reflectionRay;
                reflectionRay.origin = hit.cord;
                reflectionRay.direction = normalize3v(add3v(reflectionDir,  add3v(scalar3v(temp1, sampleY), scalar3v(temp2, sampleX))  ));

                int nextRRC = currentRRC - 1;
                floatPixel reflectionColor = recurisveRay(hit, scene, lights, lightcount, reflectionRay, currentORC, nextRRC);
                pixelColor.r += reflectionColor.r * RRCC;
                pixelColor.g += reflectionColor.g * RRCC;
                pixelColor.b += reflectionColor.b * RRCC;
                pixelColor.a += reflectionColor.a * RRCC;
            }
        }
    }
    return pixelColor;
}






floatPixel shadow(Hit const &hit, BVHNode const * const scene, LightSource const * const lights, int const &lightcount,
                    ToneMapping mode, float ambiance){

if(!hit.hitTV) return {0.0f, 0.0f, 0.0f, 0.0f};
float tR = 0.0f;
float tG = 0.0f;
float tB = 0.0f;
float tA = 0.0f;
bool blocked = false;
    for(int i = 0; i < lightcount; i++){
        Ray shadowray;
        shadowray.origin = hit.cord;
        shadowray.direction = normalize3v(subtract3v(lights[i].cord, hit.cord));
        float lightDistance = length3v(subtract3v(lights[i].cord, hit.cord));
        Hit temphit = BVHtraversial(*scene, shadowray);
            if(!temphit.hitTV || temphit.t >= lightDistance){
                float distanceloss = 1.0f;
                if(lights[i].distancelossTV) distanceloss = lights[i].distancelossConst / ((lightDistance * lightDistance)) ;
                float normalaAlighnment = fabs(dotProduct3v(hit.normal, shadowray.direction));
                float brightnessModifier = normalaAlighnment * distanceloss;

                tR += lights[i].r * static_cast<float>(hit.color.r) * brightnessModifier;
                tG += lights[i].g * static_cast<float>(hit.color.g) * brightnessModifier;
                tB += lights[i].b * static_cast<float>(hit.color.b) * brightnessModifier;
                tA += lights[i].a * static_cast<float>(hit.color.a) * brightnessModifier;
            }
    }
    //add alpha see trough posibilities with a variable for see trough ness and a color for the color change
    //reflection and refraction if blocked

    switch(mode){
        case ToneMapping::Clamp:

            tR = std::max(ambiance, std::min(tR, 255.0f));
            tG = std::max(ambiance, std::min(tG, 255.0f));
            tB = std::max(ambiance, std::min(tB, 255.0f));
            tA = std::max(ambiance, std::min(tA, 255.0f));
            break;

        case ToneMapping::Reinhard:
            tR = std::max(ambiance, (tR/(tR+1.0f))*255.0f);
            tG = std::max(ambiance, (tG/(tG+1.0f))*255.0f);
            tB = std::max(ambiance, (tB/(tB+1.0f))*255.0f);
            tA = std::max(ambiance, (tA/(tA+1.0f))*255.0f);
            break;
//        case ToneMapping::Normalize:
//            break;
        default:
            DEBUG_LOG("error on light settings");
            tR = std::max(ambiance, (tR/(tR+1.0f))*255.0f);
            tG = std::max(ambiance, (tG/(tG+1.0f))*255.0f);
            tB = std::max(ambiance, (tB/(tB+1.0f))*255.0f);
            tA = std::max(ambiance, (tA/(tA+1.0f))*255.0f);
            break;
    }
return {tR, tG, tB, tA};

}