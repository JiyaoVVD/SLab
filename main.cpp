#ifndef UNICODE
#define UNICODE
#endif

#include <windows.h>
#include <graphics.h>
#include <iostream>
#include <glm.hpp>
#include <conio.h>

typedef COLORREF Color;


#define WIDTH 640
#define HEIGHT 480


struct FrameBuffer{
    unsigned width;
    unsigned height;
    Color *buffer;
};


FrameBuffer& CreateFrameBuffer(unsigned width, unsigned height){
    auto *buffer = new Color[width * height]{0};
    auto *frameBuffer = new FrameBuffer{
            width,
            height,
            buffer
    };
    return *frameBuffer;
}

 
void RenderFrameBuffer(FrameBuffer& frameBuffer){
    for(int i = 0; i < frameBuffer.height; i++){
        for(int j = 0; j < frameBuffer.width; j++){
            putpixel(j, i, frameBuffer.buffer[i * frameBuffer.width + j]);
        }
    }
}


struct SwapFrameBuffer{
    FrameBuffer *buffer;
};


void DrawLine(FrameBuffer &frameBufer, const glm::ivec2& sPos, const glm::ivec2& ePos, const Color& color){

}


void DrawLine(FrameBuffer &frameBuffer, int x0, int y0, int x1, int y1, Color color){
    for (float t = 0; t < 1.; t += .1){
        int x = int(x0 * (1.f - t) + x1 * t);
        int y = int(y0 * (1.f - t) + y1 * t);
        frameBuffer.buffer[x * frameBuffer.width + y] = color;
        
    }
}

void DrawLine(const glm::ivec2& p1, const glm::ivec2& p2, const Color& color){
    float k = (float)(p2.y - p1.y) / (float)(p2.x - p1.x);
    for(int i = p1.x; i <= p2.x; ++i){
        int y = (int)(k * (i - p1.x) + p1.y);
        putpixel(i, y, color);
    }
}


int main(){
    // EasyX's init function
    initgraph(WIDTH, HEIGHT, EW_SHOWCONSOLE | EW_DBLCLKS);

    // receive keyboard event and exit
    while(!_kbhit()){
        DrawLine(glm::ivec2(100, 100), glm::ivec2(200, 200), Color(0xFFFFFF));
        Sleep(20);
    }

    // close EasyX
    closegraph();
    return 0;
}
