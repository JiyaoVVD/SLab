//
// Created by tangrui01 on 2024/11/5.
//
#include "framebuffer.h"


FrameBuffer* CreateFrameBuffer(const unsigned width, const unsigned height){
    auto *buffer = new unsigned char[width * height * 3]{0};
    auto *depthBuffer = new SFloat[width * height]{0};
    auto *frameBuffer = new FrameBuffer{
            width,
            height,
            buffer,
            depthBuffer,
    };
    return frameBuffer;
}


void ReleaseFrameBuffer(FrameBuffer* frameBuffer){
    delete frameBuffer->buffer;
    delete frameBuffer->depthBuffer;
    delete frameBuffer;
}


void RenderFrameBuffer(const FrameBuffer* frameBuffer){
    glRasterPos2i(-1, -1);      // set glDrawPixels' begin pos to left-bottom
    glDrawPixels((GLsizei)frameBuffer->width, (GLsizei)frameBuffer->height, GL_RGB, GL_UNSIGNED_BYTE, frameBuffer->buffer);
}


void ClearFrameBuffer(FrameBuffer* frameBuffer, const SNormColor3& color){
    for(auto i = 0; i < frameBuffer->width * frameBuffer->height; ++i){
        *(frameBuffer->buffer + i * 3) = color.r;
        *(frameBuffer->buffer + i * 3 + 1) = color.g;
        *(frameBuffer->buffer + i * 3 + 2) = color.b;
        *(frameBuffer->depthBuffer + i) = S_CONST_FLOAT(1000.0);
    }
    // memset(frameBuffer->depthBuffer, 1000.0, sizeof(SFloat) * frameBuffer->width * frameBuffer->height);
}


//inline void DrawPixel(FrameBuffer *frameBuffer, const SVector2Int& p, const SNormColor3& color){
//    if(
//        p.x < 0 || p.x >= frameBuffer->width || p.y < 0 || p.y >= frameBuffer->height
//    )
//        return;
//    auto index = (p.y * frameBuffer->width + p.x) * 3;
//    frameBuffer->buffer[index] = color.r;
//    frameBuffer->buffer[index + 1] = color.g;
//    frameBuffer->buffer[index + 2] = color.b;
//}


inline void DrawPixel(FrameBuffer *frameBuffer, const SVector2Int& p, const SNormColor3& color, SFloat depth){
    if(
            p.x < 0 || p.x >= frameBuffer->width || p.y < 0 || p.y >= frameBuffer->height
    )
        return;
    auto index = (p.y * frameBuffer->width + p.x);
    auto colorIndex = index * 3;
    frameBuffer->buffer[colorIndex] = color.r;
    frameBuffer->buffer[colorIndex + 1] = color.g;
    frameBuffer->buffer[colorIndex + 2] = color.b;
    frameBuffer->depthBuffer[index] = depth;
}


void DrawLine(FrameBuffer *frameBuffer, const SVector2Int& p0, const SVector2Int& p1, const SNormColor3& c0, const SNormColor3& c1){
    int x0 = p0.x, y0 = p0.y, x1 = p1.x, y1 = p1.y;
    int dx = glm::abs(x1 - x0);
    int dy = glm::abs(y1 - y0);
    int sx = (x0 < x1) ? 1 : -1;
    int sy = (y0 < y1) ? 1 : -1;
    int err = dx - dy;
    SFloat len = glm::length((SVector2)(p1 - p0));


    while(true){
        auto color = glm::mix((SVector3)c0, (SVector3)c1, glm::length(SVector2(x0, y0) - (SVector2)p0) / len);
        DrawPixel(frameBuffer, SVector2Int(x0, y0), color, 0);
        if(x0 == x1 && y0 == y1) break;
        int e2 = 2 * err;
        if(e2 > -dy){
            err -= dy;
            x0 += sx;
        }
        if(e2 < dx){
            err += dx;
            y0 += sy;
        }
    }
//    SFloat k = (SFloat)(p2.y - p1.y) / (SFloat)(p2.x - p1.x);
//    for(int x = p1.x; x <= p2.x; ++x){
//        int y = (int)glm::round(k * (SFloat)(x - p1.x) + (SFloat)p1.y);
//        DrawPixel(frameBuffer, SVector2Int(x, y), color);
//    }
}


void DrawTriangle(
        FrameBuffer *frameBuffer,
        const SVector2Int& p0,
        const SVector2Int& p1,
        const SVector2Int& p2,
        const SNormColor3& c0,
        const SNormColor3& c1,
        const SNormColor3& c2){
    auto lx = std::min(std::min(p0.x, p1.x), p2.x);
    auto rx = std::max(std::max(p0.x, p1.x), p2.x);
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
            if((crs0 >= 0 && crs1 >= 0 && crs2 >= 0)) // ||
               //(crs0 <= 0 && crs1 <= 0 && crs2 <= 0))
            {
                // TODO: to be optimized
                auto w0 = (SFloat)(-(x - p1.x) * (p2.y - p1.y) + (y - p1.y) * (p2.x - p1.x)) / (-(p0.x - p1.x) * (p2.y - p1.y) + (p0.y - p1.y) * (p2.x - p1.x));
                auto w1 = (SFloat)(-(x - p2.x) * (p0.y - p2.y) + (y - p2.y) * (p0.x - p2.x)) / (-(p1.x - p2.x) * (p0.y - p2.y) + (p1.y - p2.y) * (p0.x - p2.x));
                auto w2 = S_CONST_FLOAT(1.0) - w0 - w1;
                auto color = (SVector3)c0 * w0 + (SVector3) c1 * w1 + (SVector3) c2 * w2;;
                DrawPixel(frameBuffer, SVector2Int(x, y), (SNormColor3)color, 0);
            }
        }
    }
}