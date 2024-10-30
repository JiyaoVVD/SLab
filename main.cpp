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
//    for(int i = 0; i < frameBuffer.height; i++){
//        for(int j = 0; j < frameBuffer.width; j++){
//            putpixel(j, i, frameBuffer.buffer[i * frameBuffer.width + j]);
//        }
//    }
    glRasterPos2i(0, 0);
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


void DrawLine(FrameBuffer &frameBuffer, const glm::ivec2& p1, const glm::ivec2& p2, const HEX_COLOR& color){
    float k = (float)(p2.y - p1.y) / (float)(p2.x - p1.x);
    for(int x = p1.x; x <= p2.x; ++x){
        int y = (int)glm::round(k * (x - p1.x) + p1.y);
        frameBuffer.buffer[y * frameBuffer.width + x] = color;
    }
}


void DrawPixel(FrameBuffer &frameBuffer, const glm::ivec2& p1, const SNormColor3& color){
    auto index = p1.y * frameBuffer.width * 3 + p1.x * 3;
    frameBuffer.buffer[index] = color.r;
    frameBuffer.buffer[index + 1] = color.g;
    frameBuffer.buffer[index + 2] = color.b;
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
            if(crs0 >= 0 && crs1 >= 0 && crs2 >= 0 ||
                crs0 <= 0 && crs1 <= 0 && crs2 <= 0)
            {
                 frameBuffer.buffer[y * frameBuffer.width + x] = color;
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

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "SLab", NULL, NULL);
    if(!window){
        std::cerr << "Failed to create GLFW window." << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    glViewport(0, 0, WIDTH, HEIGHT);
//    glEnable(GL_DEPTH_TEST);

    FrameBuffer& frameBuffer = CreateFrameBuffer(WIDTH, HEIGHT);
    ClearFrameBuffer(frameBuffer, SNormColor3(255, 0, 0));

    while(!glfwWindowShouldClose(window)){
        processInput(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glRasterPos2i(0, 0);
        glDrawPixels(WIDTH, HEIGHT, GL_RGB, GL_UNSIGNED_BYTE, frameBuffer.buffer);
        // RenderFrameBuffer(frameBuffer);
        glfwSwapBuffers(window);

        glfwPollEvents();
    }

    ReleaseFrameBuffer(&frameBuffer);
    glfwTerminate();
    return 0;
}


//int main(){
//    // EasyX's init function
//    FrameBuffer& frameBuffer = CreateFrameBuffer(WIDTH, HEIGHT);
//    // ClearFrameBuffer(frameBuffer, RGB_TO_HEX_COLOR(255, 0, 0));
//    DrawTriangle(frameBuffer, glm::ivec2(0, 0), glm::ivec2(0, HEIGHT-1), glm::ivec2(WIDTH-1, HEIGHT-1), RGB_TO_HEX_COLOR(255, 0, 0));
//    initgraph(WIDTH, HEIGHT, EW_SHOWCONSOLE | EW_DBLCLKS);
//
//
//    // receive keyboard event and exit
//    while(!_kbhit()){
//        RenderFrameBuffer(frameBuffer);
//        Sleep(20);
//    }
//
//    ReleaseFrameBuffer(&frameBuffer);
//    // close EasyX
//    closegraph();
//    return 0;
//}
