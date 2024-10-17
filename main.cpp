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


void ReleaseFrameBuffer(FrameBuffer* frameBuffer){
    delete frameBuffer->buffer;
    delete frameBuffer;
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


void DrawLine(FrameBuffer &frameBuffer, const glm::ivec2& p1, const glm::ivec2& p2, const Color& color){
    float k = (float)(p2.y - p1.y) / (float)(p2.x - p1.x);
    for(int x = p1.x; x <= p2.x; ++x){
        int y = (int)(k * (x - p1.x) + p1.y);
        frameBuffer.buffer[y * frameBuffer.width + x] = color;
    }
}


void DrawPixel(FrameBuffer &frameBuffer, const glm::ivec2& p1, const Color& color){
    frameBuffer.buffer[p1.y * frameBuffer.width + p1.x] = color;
}


void DrawTriangle(FrameBuffer &frameBuffer, const glm::ivec2 p1, const glm::ivec2 p2, const glm::ivec2 p3, const Color& color){

}


int main(){
    // EasyX's init function
    initgraph(WIDTH, HEIGHT, EW_SHOWCONSOLE | EW_DBLCLKS);

    FrameBuffer& frameBuffer = CreateFrameBuffer(WIDTH, HEIGHT);

    // receive keyboard event and exit
    while(!_kbhit()){
        DrawLine(frameBuffer, glm::ivec2(100, 100), glm::ivec2(400, 300), Color(0xFFFFFF));
        RenderFrameBuffer(frameBuffer);
        Sleep(20);
    }

    ReleaseFrameBuffer(&frameBuffer);
    // close EasyX
    closegraph();
    return 0;
}
