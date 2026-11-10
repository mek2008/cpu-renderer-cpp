#include "frame.hpp"
#include "debug.hpp"
#include <fstream>
#include <cstdint>




Frame::Frame(int w_, int h_, bool atv_) : width(w_), height(h_), includeAlpha(atv_) {
    framev = new Pixel[width * height]();
    aspect = static_cast<float>(width) / static_cast<float>(height);
}
Pixel& Frame::At(int x, int y){
return framev[y * width + x];
}






void bmpPrint(Frame const * const frame, const char *filename){
std::ofstream f(filename, std::ios::binary);

DEBUG_LOG_IF(!f, "failed to open file");
if(!f) return; 


int width = frame->width;
int height = frame->height;
bool includeAlpha = frame->includeAlpha;

int bytesPerPixel = includeAlpha ? 4 : 3;


int rawRowBytes = width * bytesPerPixel;
int paddedRowBytes = (rawRowBytes + 3) & ~3;
int padCount = paddedRowBytes - rawRowBytes;

int pixelDataSize = paddedRowBytes * height;

const int fileHeaderSize = 14;
const int infoHeaderSize = 40;

uint32_t fileSize = static_cast<uint32_t>(fileHeaderSize + infoHeaderSize + pixelDataSize);

f.write("BM", 2);  //.write(const char* data, std::streamsize count)

//bit shift, truncade, convert
f.put(static_cast<char>(static_cast<uint8_t>(fileSize)));
f.put(static_cast<char>(static_cast<uint8_t>(fileSize >> 8)));
f.put(static_cast<char>(static_cast<uint8_t>(fileSize >> 16)));
f.put(static_cast<char>(static_cast<uint8_t>(fileSize >> 24)));

f.put(static_cast<char>(0));
f.put(static_cast<char>(0));
f.put(static_cast<char>(0));
f.put(static_cast<char>(0));

f.put(static_cast<char>(static_cast<uint8_t>(54)));
f.put(static_cast<char>(static_cast<uint8_t>(54 >> 8)));
f.put(static_cast<char>(static_cast<uint8_t>(54 >> 16)));
f.put(static_cast<char>(static_cast<uint8_t>(54 >> 24)));

//----------------------------------------

f.put(static_cast<char>(static_cast<uint8_t>(40)));
f.put(static_cast<char>(static_cast<uint8_t>(40 >> 8)));
f.put(static_cast<char>(static_cast<uint8_t>(40 >> 16)));
f.put(static_cast<char>(static_cast<uint8_t>(40 >> 24)));

signed int swidth = width;
signed int sheight = height;

f.put(static_cast<char>(static_cast<uint8_t>(swidth)));
f.put(static_cast<char>(static_cast<uint8_t>(swidth >> 8)));
f.put(static_cast<char>(static_cast<uint8_t>(swidth >> 16)));
f.put(static_cast<char>(static_cast<uint8_t>(swidth >> 24)));
f.put(static_cast<char>(static_cast<uint8_t>(sheight)));
f.put(static_cast<char>(static_cast<uint8_t>(sheight >> 8)));
f.put(static_cast<char>(static_cast<uint8_t>(sheight >> 16)));
f.put(static_cast<char>(static_cast<uint8_t>(sheight >> 24)));

f.put(static_cast<char>(static_cast<uint8_t>(1)));
f.put(static_cast<char>(static_cast<uint8_t>(1 >> 8)));

f.put(static_cast<char>(static_cast<uint8_t>(bytesPerPixel*8)));
f.put(static_cast<char>(static_cast<uint8_t>(bytesPerPixel*8 >> 8)));

f.put(static_cast<char>(0));
f.put(static_cast<char>(0));
f.put(static_cast<char>(0));
f.put(static_cast<char>(0));

f.put(static_cast<char>(static_cast<uint8_t>(pixelDataSize)));
f.put(static_cast<char>(static_cast<uint8_t>(pixelDataSize >> 8)));
f.put(static_cast<char>(static_cast<uint8_t>(pixelDataSize >> 16)));
f.put(static_cast<char>(static_cast<uint8_t>(pixelDataSize >> 24)));

for(int i = 0; i < 4*4; ++i){ f.put(static_cast<char>(0)); }


for(int y = height - 1; y >= 0; --y){
    for(int x = 0; x < width; ++x){
    f.put(static_cast<char>(frame->framev[y*width+x].b));
    f.put(static_cast<char>(frame->framev[y*width+x].g));
    f.put(static_cast<char>(frame->framev[y*width+x].r));
    if(includeAlpha) f.put(static_cast<char>(frame->framev[y*width+x].a));
}
for(int i = 0; i < padCount; ++i){ f.put(static_cast<char>(0)); }    
}





    return;
}