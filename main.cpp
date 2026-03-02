#include <iostream>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include "objects.hpp"
#include <vector>
#include <cmath>
#include <algorithm>








struct Framebuffer {
int width, height;
std::vector<Pixel> pixels;

Framebuffer(int w, int h)
    : width(w), height(h), pixels(w*h)  {}

Pixel& at(int x, int y) {return pixels[y * width + x]; }

};

struct SDLState {
    SDL_Window *window = nullptr;
    SDL_Renderer *renderer = nullptr;
    SDL_Texture* texture = nullptr;
    int width, height;
    SDLState(int width, int height) : width(width), height(height) {}
};

void cleanup(SDLState &state){
 if (state.texture) SDL_DestroyTexture(state.texture);
 if (state.renderer) SDL_DestroyRenderer(state.renderer);
 if (state.window) SDL_DestroyWindow(state.window);
    SDL_Quit();
}





int main(int argc, char *argv[])
{

//-------------------------------------
SDLState state(1600, 900); 
Perspective persptv({0, 0, 0}, 1);
persptv.rotatetionj(0.6);
//-------------------------------------

//SDL_Init(SDL_INIT_VIDEO);
if (!SDL_Init(SDL_INIT_VIDEO)) { SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", "Error initiatlizing SDL3", nullptr); return -1; }
state.window = SDL_CreateWindow("SDL3 demo", state.width, state.height, 0);

if (!state.window){ SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", "Error creating window", state.window); cleanup(state); return -1; }

// create the renderer
state.renderer = SDL_CreateRenderer(state.window, nullptr);
if(!state.renderer) { SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", "Error creating renderer", state.window); cleanup(state); return -1;}



Framebuffer framebuffer(state.width, state.height);

state.texture = SDL_CreateTexture(
    state.renderer,
    SDL_PIXELFORMAT_RGBA32,
    SDL_TEXTUREACCESS_STREAMING,
    state.width, state.height 
    );




bool running = true; while (running) {

    SDL_Event event{ 0 };
    while (SDL_PollEvent(&event)) {
    switch (event.type){
        case SDL_EVENT_QUIT:
        {
            running = false;
            break;
        }

    }
    }
 
//------------------------------------
SDL_SetRenderDrawColor(state.renderer, 0, 0, 0, 255);
SDL_RenderClear(state.renderer);
std::fill(framebuffer.pixels.begin(), framebuffer.pixels.end(), Pixel{0,0,0,255});

Sphere sphere({0, 0, 5}, 1.0f, {255, 0, 0, 255});

LightSource light({3, 5, 0}, 200, {255, 255, 255, 255});


float aspect = (float)framebuffer.width / framebuffer.height;

for (int y = 0; y < framebuffer.height; y++) {
for (int x = 0; x < framebuffer.width; x++) {

// ray initilization
float u = (2.0f * x / framebuffer.width - 1.0f) * aspect;
float v = (1.0f - 2.0f * y / framebuffer.height);

Ray ray;
ray.origin = persptv.position; 
ray.direction = persptv.localToGlobal(u, v, persptv.focalLength);
ray.direction = normalize3v(ray.direction);


//--------------------------------------------------

ClosestP closest;

locPixelv hit = sphere.hitRR(ray);
closest.closestH(hit);

if (closest.colCord.t < std::numeric_limits<float>::max()) {
    framebuffer.at(x, y) = postLighting(light, closest.colCord);
}
//--------------------------------------------------
}
}




SDL_UpdateTexture(
    state.texture,
    nullptr,
    framebuffer.pixels.data(),
    framebuffer.width * sizeof(Pixel)
);

SDL_RenderClear(state.renderer);
SDL_RenderTexture(state.renderer, state.texture, nullptr, nullptr);
SDL_RenderPresent(state.renderer);
}
    


    cleanup(state); return 0;
}