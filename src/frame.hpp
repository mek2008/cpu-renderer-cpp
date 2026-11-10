#ifndef FRAME_HPP
#define FRAME_HPP


struct Pixel{ unsigned char r = 0, g = 0, b = 0, a = 0; }; static_assert(sizeof(Pixel) == 4, "Pixel must be 4 bytes");
struct floatPixel{ float r = 0.0f, g = 0.0f, b = 0.0f, a = 0.0f; }; static_assert(sizeof(floatPixel) == 16, "FloatPixel must be 16 bytes"); //optimization and accuracy

struct Frame{
    int width, height;
    Pixel *framev;
    float aspect;
    bool includeAlpha;

    Frame(int w_, int h_, bool atv_);

    Pixel& At(int x, int y);
};

void bmpPrint(Frame const * const frame, char const *filename = "out.bmp");




#endif
