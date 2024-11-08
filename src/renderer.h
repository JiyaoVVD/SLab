//
// Created by tangrui01 on 2024/11/4.
//
#pragma once
#ifndef SLAB_RENDERER_H
#define SLAB_RENDERER_H

#include <GLFW/glfw3.h>
#include <vector>

#include "stype.h"
#include "framebuffer.h"
#include "camera.h"

class Renderer {
public:
    enum RenderMode{
        SLAB_TRIANGLE_MODE,
        SLAB_LINE_MODE,
        SLAB_LINE_LOOP_MODE,
    };
public:
    Renderer(unsigned width, unsigned height, GLFWwindow* window, RenderMode renderMode);
    Renderer(unsigned width, unsigned height, GLFWwindow* window, RenderMode renderMode, SByte r, SByte g, SByte b);
    Renderer(unsigned width, unsigned height, GLFWwindow* window, RenderMode renderMode, SNormColor3 background);
    Renderer(unsigned width, unsigned height, GLFWwindow* window, RenderMode renderMode, SColor3 background);
    ~Renderer();
    void render();
    void setVertices(std::vector<SVertex> vertices, std::vector<SIndex> indices);
    void setVertices(SVertex* vertices, size_t numVertices, SIndex* indices, size_t numIndices);
    void setVertices(std::vector<SVertex> vertices);
    void setVertices(SVertex* vertices, size_t numVertices);
    void setRenderMode(RenderMode renderMode);
    void setCamera(const Camera* camera);
private:
    void drawLineMode();
    void drawLineLoopMode();
    void drawTriangleMode();
private:
    FrameBuffer* frameBuffer;
    GLFWwindow* window;
    SNormColor3 background;

    const Camera* camera;

    RenderMode renderMode;

    std::vector<SVertex> vertexBuffer;
    std::vector<SIndex> indexBuffer;
};


#endif //SLAB_RENDERER_H
