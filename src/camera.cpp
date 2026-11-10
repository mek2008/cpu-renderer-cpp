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
    k = normalize3v(crossProduct3v(i, j));
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


Pixel lightSolving(floatPixel colorOriginal, ToneMapping mode, float ambiance);


void Perspective::render(BVHNode const * const scene, LightSource const * const lights, int const &lightcount){
    int const aa = std::max(1, antialiasing);
    float antialiasingConst = 1.0f / static_cast<float>(antialiasing);
    float antialiasingConstSQ = 1.0f / (static_cast<float>(antialiasing) *static_cast<float>(antialiasing));

for(int y = 0; y < frame->height; y++){
for(int x = 0; x < frame->width; x++){

    floatPixel pixelColor;
    Pixel color;
    
        for (int suby = 0; suby < antialiasing; suby++){
        for (int subx = 0; subx < antialiasing; subx++){
        
            float x_sample = static_cast<float>(x) + (static_cast<float>(subx) + 0.5f) * antialiasingConst;
            float y_sample = static_cast<float>(y) + (static_cast<float>(suby) + 0.5f) * antialiasingConst;

            float u = (2.0f * (x_sample / static_cast<float>(frame->width)) - 1.0f) * frame->aspect;
            float v = 1.0f - 2 * (y_sample / static_cast<float>(frame->height));
                    
            
            Ray ray;
            ray.origin = cordinates; 
            ray.direction = normalize3v( localToGlobal(u, v, focalLength) );

            floatPixel subPixelColor = recursiveRay(scene, lights, lightcount, ray, transparencyMaxDepth, reflectionMaxDepth);
            
            pixelColor.r += subPixelColor.r * subPixelColor.a * antialiasingConstSQ;
            pixelColor.g += subPixelColor.g * subPixelColor.a * antialiasingConstSQ;
            pixelColor.b += subPixelColor.b * subPixelColor.a * antialiasingConstSQ;
            pixelColor.a += subPixelColor.a * antialiasingConstSQ;
        }
        }
        
        if(pixelColor.a > EPSILON){
            pixelColor.r /= pixelColor.a;
            pixelColor.g /= pixelColor.a;
            pixelColor.b /= pixelColor.a;
        }
    color = lightSolving(pixelColor, mode, ambiance);
        

    frame->At(x, y) = color;

}
}

}






floatPixel Perspective::recursiveRay(BVHNode const * const scene, LightSource const * const lights, int const &lightcount, Ray const &ray, 
                                        int const &transparencyDepthRemaining, int const &reflectionDepthRemaining){
    floatPixel pixelColor;
    Hit hit = BVHtraversial(*scene, ray);
    if(!hit.hitTV) return {0.0f, 0.0f, 0.0f, 0.0f};

    floatPixel surfaceColor = shadow(hit, scene, lights, lightcount, mode, ambiance); //currently just the normal color with shadows included
    
    pixelColor = surfaceColor;
    float alpha = static_cast<float>(hit.color.a) / 255.0f;
    pixelColor.a = alpha;

//reflection
    floatPixel reflectionColor;

    if(reflectionDepthRemaining > 0 && hit.reflectivity > EPSILON){

        Vec3 reflectionDir = rodriguesRotation(hit.normal, scalar3v(ray.direction, -1.0f), PI);
        Vec3 temp = crossProduct3v(reflectionDir, normalize3v(hit.cord));
        if(dotProduct3v(temp, temp) < EPSILON * EPSILON) temp = crossProduct3v(reflectionDir, hit.normal);

        Vec3 temp1 = normalize3v(temp);
        Vec3 temp2 = normalize3v(crossProduct3v(reflectionDir, temp1));


        int samplesPerAxis = reflectionSamplesPerAxis - reflectionSamplesReduction * (reflectionMaxDepth - reflectionDepthRemaining);
        if(samplesPerAxis < 1) samplesPerAxis = 1;
        float sampleWeight = 1.0f / static_cast<float>(samplesPerAxis * samplesPerAxis);

        for(int y = 0; y < samplesPerAxis; y++){
            for(int x = 0; x < samplesPerAxis; x++){
                float sampleY = ((static_cast<float>(y) + 0.5f) / static_cast<float>(samplesPerAxis) - 0.5f) * hit.reflectionSpread;
                float sampleX = ((static_cast<float>(x) + 0.5f) / static_cast<float>(samplesPerAxis) - 0.5f) * hit.reflectionSpread;

                Ray reflectionRay;
                reflectionRay.origin = add3v(hit.cord, scalar3v(hit.normal, EPSILON));
                reflectionRay.direction = normalize3v(add3v(reflectionDir,  add3v(scalar3v(temp1, sampleY), scalar3v(temp2, sampleX))  ));

                    Vec3 dir = normalize3v(add3v(reflectionDir,
                    add3v(scalar3v(temp1, sampleY), scalar3v(temp2, sampleX))));
                    float const minDot = 0.05f;                       // about 3 degrees above the surface
                    float d = dotProduct3v(dir, hit.normal);
                    if(d < minDot) reflectionRay.direction = normalize3v(add3v(dir, scalar3v(hit.normal, minDot - d)));

                int nextReflectionDepth = reflectionDepthRemaining - 1;
                floatPixel currentReflectionColor = recursiveRay(scene, lights, lightcount, reflectionRay, transparencyDepthRemaining, nextReflectionDepth);
                    reflectionColor.r += currentReflectionColor.r * sampleWeight * currentReflectionColor.a;
                    reflectionColor.g += currentReflectionColor.g * sampleWeight * currentReflectionColor.a;
                    reflectionColor.b += currentReflectionColor.b * sampleWeight * currentReflectionColor.a;
                    reflectionColor.a += currentReflectionColor.a * sampleWeight;
            }
        }
        
        if(reflectionColor.a > EPSILON){
            reflectionColor.r /= reflectionColor.a;
            reflectionColor.g /= reflectionColor.a;
            reflectionColor.b /= reflectionColor.a;
        }
    }

    float effectiveReflectivity = hit.reflectivity * reflectionColor.a;
    pixelColor.r = surfaceColor.r * (1.0f - effectiveReflectivity) + reflectionColor.r * effectiveReflectivity;
    pixelColor.g = surfaceColor.g * (1.0f - effectiveReflectivity) + reflectionColor.g * effectiveReflectivity;
    pixelColor.b = surfaceColor.b * (1.0f - effectiveReflectivity) + reflectionColor.b * effectiveReflectivity;
//    pixelColor.a = surfaceColor.a * (1.0f - effectiveReflectivity) + reflectionColor.a * effectiveReflectivity;


//transparency
    float transparency = 1.0f - alpha;
    floatPixel transparencyColor;
    if(transparencyDepthRemaining > 0 && transparency > EPSILON){
        Ray transparencyRay;
        transparencyRay.origin = add3v(hit.cord, scalar3v(ray.direction, EPSILON));
        transparencyRay.direction = ray.direction; //should be changed for refractions

        int nextTransparencyDepth = transparencyDepthRemaining - 1;
        transparencyColor = recursiveRay(scene, lights, lightcount, transparencyRay, nextTransparencyDepth, reflectionDepthRemaining);

        float backgroundWeight = transparencyColor.a * transparency;
        float outputAlpha = alpha + backgroundWeight;

        if(outputAlpha > EPSILON){
            pixelColor.r = (pixelColor.r * alpha + transparencyColor.r * backgroundWeight) / outputAlpha;
            pixelColor.g = (pixelColor.g * alpha + transparencyColor.g * backgroundWeight) / outputAlpha;
            pixelColor.b = (pixelColor.b * alpha + transparencyColor.b * backgroundWeight) / outputAlpha;
        }else{
            pixelColor.r = 0.0f;
            pixelColor.g = 0.0f;
            pixelColor.b = 0.0f;
        }
    pixelColor.a = outputAlpha;
    }

    return pixelColor;
}




floatPixel shadow(Hit const &hit, BVHNode const * const scene, LightSource const * const lights, int const &lightcount,
                    ToneMapping mode, float ambiance){

if(!hit.hitTV) return {0.0f, 0.0f, 0.0f, 0.0f};
float tR = 0.0f;
float tG = 0.0f;
float tB = 0.0f;

    for(int i = 0; i < lightcount; i++){
        Ray shadowray;
        shadowray.origin = add3v(hit.cord, scalar3v(hit.normal, EPSILON));
        shadowray.direction = normalize3v(subtract3v(lights[i].cord, hit.cord));
        float lightDistance = length3v(subtract3v(lights[i].cord, hit.cord));

        Hit temphit = BVHtraversial(*scene, shadowray);

            float distanceloss = (lights[i].distancelossTV && lightDistance > EPSILON) ? 1.0f :
                                    distanceloss = lights[i].distancelossConst / ((lightDistance * lightDistance));
            float normalaAlighnment = std::max(0.0f, dotProduct3v(hit.normal, shadowray.direction));
            float transmit = (!temphit.hitTV || temphit.t >= lightDistance) ? 1.0f : 
                                1.0f - static_cast<float>(temphit.color.a) / 255.0f;
            float brightness = normalaAlighnment * distanceloss * transmit;

                tR += lights[i].r * static_cast<float>(hit.color.r) * brightness;
                tG += lights[i].g * static_cast<float>(hit.color.g) * brightness;
                tB += lights[i].b * static_cast<float>(hit.color.b) * brightness;
    }
    //add alpha see trough posibilities with a variable for see trough ness and a color for the color change
    //reflection and refraction if blocked
return {tR, tG, tB, 0.0f};
}

Pixel lightSolving(floatPixel colorOriginal, ToneMapping mode, float ambiance){
float tR = colorOriginal.r;
float tG = colorOriginal.g;
float tB = colorOriginal.b;
float tA = colorOriginal.a;

    switch(mode){
        case ToneMapping::Clamp:
            //not tested or recommended
            tR = std::max(ambiance * 255.0f, std::min(tR * 255.0f, 255.0f));
            tG = std::max(ambiance * 255.0f, std::min(tG * 255.0f, 255.0f));
            tB = std::max(ambiance * 255.0f, std::min(tB * 255.0f, 255.0f));
            tA = std::max(0.0f, std::min(tA * 255.0f, 255.0f));
            break;

        case ToneMapping::Reinhard:
            tR = std::max(ambiance, tR/255.0f / (tR/255.0f + 1.0f)) * 255.0f;
            tG = std::max(ambiance, tG/255.0f / (tG/255.0f + 1.0f)) * 255.0f;
            tB = std::max(ambiance, tB/255.0f / (tB/255.0f + 1.0f)) * 255.0f;
            tA = std::max(0.0f, std::min(tA * 255.0f, 255.0f));
            break;
//        case ToneMapping::Normalize:
//            break;
        default:
            DEBUG_LOG("error on light settings");
            tR = std::max(ambiance, (tR/255.0f / (tR/255.0f + 1.0f)) * 2.0f) * 255.0f;
            tG = std::max(ambiance, (tG/255.0f / (tG/255.0f + 1.0f)) * 2.0f) * 255.0f;
            tB = std::max(ambiance, (tB/255.0f / (tB/255.0f + 1.0f)) * 2.0f) * 255.0f;
            tA = std::max(0.0f, std::min(tA * 255.0f, 255.0f));
            break;
    }
    return {static_cast<unsigned char>(tR), static_cast<unsigned char>(tG), static_cast<unsigned char>(tB), static_cast<unsigned char>(tA)};
}