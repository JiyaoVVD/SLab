#ifndef UNICODE
#define UNICODE
#endif

#include <windows.h>
#include <graphics.h>
#include <iostream>
#include <glm.hpp>
#include <conio.h>

#include "stype.h"

#define WIDTH 640
#define HEIGHT 640


struct FrameBuffer{
    unsigned width;
    unsigned height;
    HEX_COLOR *buffer;
};


FrameBuffer& CreateFrameBuffer(unsigned width, unsigned height){
    auto *buffer = new HEX_COLOR[width * height]{0};
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


void DrawLine(FrameBuffer &frameBuffer, const glm::ivec2& p1, const glm::ivec2& p2, const HEX_COLOR& color){
    float k = (float)(p2.y - p1.y) / (float)(p2.x - p1.x);
    for(int x = p1.x; x <= p2.x; ++x){
        int y = (int)glm::round(k * (x - p1.x) + p1.y);
        frameBuffer.buffer[y * frameBuffer.width + x] = color;
    }
}


void DrawPixel(FrameBuffer &frameBuffer, const glm::ivec2& p1, const HEX_COLOR& color){
    frameBuffer.buffer[p1.y * frameBuffer.width + p1.x] = color;
}


inline int64_t icross2(const glm::ivec2& v0, const glm::ivec2& v1)
{
    return (int64_t)v0.x * v1.y - (int64_t)v0.y * v1.x;
}


void DrawTriangle(FrameBuffer &frameBuffer, const glm::ivec2 p0, const glm::ivec2 p1, const glm::ivec2 p2, const HEX_COLOR& color){
    auto lx = std::min(std::min(p0.x, p1.x), p2.x);
    auto rx = std::max(std::min(p0.x, p1.x), p2.x);
    auto by = std::max(std::max(p0.y, p1.y), p2.y);
    auto ty = std::min(std::min(p0.y, p1.y), p2.y);
    for(int x = lx; x <= rx; ++x)
    {
        for(int y = ty; y <= by; ++y)
        {
            glm::ivec2 point(x, y);

            auto crs0 = icross2(p1 - p0, point - p0);
            auto crs1 = icross2(p2 - p1, point - p1);
            auto crs2 = icross2(p0 - p2, point - p2);
            if(crs0 * crs1 >= 0 && crs1 * crs2 >= 0 && crs0 * crs2 >= 0)
            {
                // std::cout << x << ',' << y << std::endl;
                frameBuffer.buffer[y * frameBuffer.width + x] = color;
            }
        }
    }
}


int main(){
    // EasyX's init function
    initgraph(WIDTH, HEIGHT, EW_SHOWCONSOLE | EW_DBLCLKS);

    FrameBuffer& frameBuffer = CreateFrameBuffer(WIDTH, HEIGHT);

    // receive keyboard event and exit
//    DrawTriangle(frameBuffer, glm::ivec2(0, 0), glm::ivec2(WIDTH, 0), glm::ivec2(WIDTH, HEIGHT), RGB_TO_HEX_COLOR(0, 0, 255));
//    DrawTriangle(frameBuffer, glm::ivec2(0, 0), glm::ivec2(0, HEIGHT), glm::ivec2(WIDTH, HEIGHT), RGB_TO_HEX_COLOR(255, 0, 0));
    while(!_kbhit()){
        DrawLine(frameBuffer, glm::ivec2(0, 0), glm::ivec2(WIDTH, HEIGHT), RGB_TO_HEX_COLOR(255, 255, 255));
        DrawLine(frameBuffer, glm::ivec2(0, HEIGHT), glm::ivec2(WIDTH, 0), RGB_TO_HEX_COLOR(255, 255, 255));
        RenderFrameBuffer(frameBuffer);
        Sleep(20);
    }

    ReleaseFrameBuffer(&frameBuffer);
    // close EasyX
    closegraph();
    return 0;
}
