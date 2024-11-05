//
// Created by tangrui01 on 2024/11/4.
//
#pragma once
#ifndef SLAB_RENDERER_H
#define SLAB_RENDERER_H

#include <GLFW/glfw3.h>

#include "stype.h"
#include "framebuffer.h"
#include "camera.h"

class Renderer {
public:
    Renderer(unsigned width, unsigned height, GLFWwindow* window);
    Renderer(unsigned width, unsigned height, GLFWwindow* window, SByte r, SByte g, SByte b);
    Renderer(unsigned width, unsigned height, GLFWwindow* window, SNormColor3 background);
    Renderer(unsigned width, unsigned height, GLFWwindow* window, SColor3 background);
    ~Renderer();
    void render();
private:
    FrameBuffer *frameBuffer;
    SNormColor3 background;
    GLFWwindow* window;
};


#endif //SLAB_RENDERER_H
