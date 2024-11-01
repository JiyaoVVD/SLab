#ifndef UNICODE
#define UNICODE
#endif

#include <windows.h>
#include <graphics.h>
#include <iostream>
#include <glm.hpp>
#include <conio.h>
#include <oneapi/tbb/parallel_for.h>
#include <chrono>
#include <gl/GL.h>
#include <GLFW/glfw3.h>
#include <algorithm>

#include "stype.h"

#define WIDTH 400LL
#define HEIGHT 400LL


struct FrameBuffer{
    unsigned width;
    unsigned height;
    unsigned char *buffer;
};


FrameBuffer& CreateFrameBuffer(unsigned width, unsigned height){
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
    glRasterPos2i(-1, -1);
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


void DrawLine(FrameBuffer &frameBuffer, const glm::ivec2& p1, const glm::ivec2& p2, const HEX_COLOR& color){
    float k = (float)(p2.y - p1.y) / (float)(p2.x - p1.x);
    for(int x = p1.x; x <= p2.x; ++x){
        int y = (int)glm::round(k * (x - p1.x) + p1.y);
        frameBuffer.buffer[y * frameBuffer.width + x] = color;
    }
}


inline int64_t icross2(const SVector2Int& v0, const SVector2Int& v1)
{
    return (int64_t)v0.x * v1.y - (int64_t)v0.y * v1.x;
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


void processInput(GLFWwindow *window){
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
        glfwSetWindowShouldClose(window, true);
    }
}


int main(){
    if(!glfwInit()){
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    // set opengl version to 2.1, enable immediate mode
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "SLab", NULL, NULL);
    if(!window){
        std::cerr << "Failed to create GLFW window." << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    glViewport(0, 0, WIDTH, HEIGHT);

    FrameBuffer& frameBuffer = CreateFrameBuffer(WIDTH, HEIGHT);
    ClearFrameBuffer(frameBuffer, SNormColor3(0, 0, 0));

    DrawTriangle(frameBuffer, SVector2Int(0, HEIGHT - 1), SVector2Int(WIDTH - 1, HEIGHT - 1), SVector2Int(WIDTH - 1, 0), SNormColor3(255, 0, 0));

    while(!glfwWindowShouldClose(window)){
        processInput(window);
        glClear(GL_COLOR_BUFFER_BIT);

        RenderFrameBuffer(frameBuffer);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    ReleaseFrameBuffer(&frameBuffer);
    glfwTerminate();
    return 0;
}
