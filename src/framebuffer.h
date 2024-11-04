//
// Created by Jiyao on 2024/11/2.
//

#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include "stype.h"
#include "math_utils.h"

struct FrameBuffer{
    unsigned width;
    unsigned height;
    unsigned char *buffer;
};


FrameBuffer& CreateFrameBuffer(const unsigned width, const unsigned height){
    auto *buffer = new unsigned char[width * height * 3]{0};
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


void RenderFrameBuffer(const FrameBuffer& frameBuffer){
    glRasterPos2i(-1, -1);      // set glDrawPixels' begin pos to left-bottom
    glDrawPixels(frameBuffer.width, frameBuffer.height, GL_RGB, GL_UNSIGNED_BYTE, frameBuffer.buffer);
}


void ClearFrameBuffer(FrameBuffer& frameBuffer, const SNormColor3& color){
    for(auto i = 0; i < frameBuffer.width * frameBuffer.height; ++i){
        *(frameBuffer.buffer + i * 3) = color.r;
        *(frameBuffer.buffer + i * 3 + 1) = color.g;
        *(frameBuffer.buffer + i * 3 + 2) = color.b;
    }
}


struct SwapFrameBuffer{
    FrameBuffer *buffer;
};


inline void DrawPixel(FrameBuffer &frameBuffer, const SVector2Int& p1, const SNormColor3& color){
    auto index = (p1.y * frameBuffer.width + p1.x) * 3;
    frameBuffer.buffer[index] = color.r;
    frameBuffer.buffer[index + 1] = color.g;
    frameBuffer.buffer[index + 2] = color.b;
}


void DrawLine(FrameBuffer &frameBuffer, const glm::ivec2& p1, const glm::ivec2& p2, const SNormColor3& color){
    float k = (float)(p2.y - p1.y) / (float)(p2.x - p1.x);
    for(int x = p1.x; x <= p2.x; ++x){
        int y = (int)glm::round(k * (x - p1.x) + p1.y);
        DrawPixel(frameBuffer, SVector2Int(x, y), color);
    }
}


void DrawTriangle(FrameBuffer &frameBuffer, const SVector2Int& p0, const SVector2Int& p1, const SVector2Int& p2, const SNormColor3 color){
    auto lx = std::min(std::min(p0.x, p1.x), p2.x);
    auto rx = std::max(std::min(p0.x, p1.x), p2.x);
    auto by = std::max(std::max(p0.y, p1.y), p2.y);
    auto ty = std::min(std::min(p0.y, p1.y), p2.y);
    for(int x = lx; x <= rx; ++x)
    {
        for(int y = ty; y <= by; ++y)
        {
            SVector2Int point(x, y);

            auto crs0 = icross2(p1 - p0, point - p0);
            auto crs1 = icross2(p2 - p1, point - p1);
            auto crs2 = icross2(p0 - p2, point - p2);
            if(crs0 >= 0 && crs1 >= 0 && crs2 >= 0 ||
                crs0 <= 0 && crs1 <= 0 && crs2 <= 0)
            {
                 DrawPixel(frameBuffer, SVector2Int(x, y), color);
            }
        }
    }
}


#endif //FRAMEBUFFER_H
