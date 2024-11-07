//
// Created by tangrui01 on 2024/11/4.
//

#include "renderer.h"

#include <utility>


Renderer::Renderer(unsigned width, unsigned height, GLFWwindow* window, RenderMode renderMode):
    background(0, 0, 0),
    window(window),
    frameBuffer(
        CreateFrameBuffer(width, height)),
    renderMode(renderMode){
    camera = new Camera(SVector3(0, 0, -1), SVector3(0, 0, 0), SVector3(0, 1, 0));
    // camera->setOrthodox(S_CONST_FLOAT(-1.0), S_CONST_FLOAT(1.0), S_CONST_FLOAT(1.0), S_CONST_FLOAT(-1.0), S_CONST_FLOAT(0.01),
//                        S_CONST_FLOAT(10000.0));
    camera->setPerspective(S_CONST_FLOAT(90.0), S_CONST_FLOAT(1.0), S_CONST_FLOAT(0.01), S_CONST_FLOAT(100.0));
}


Renderer::Renderer(unsigned width, unsigned height, GLFWwindow* window, RenderMode renderMode, SByte r, SByte g, SByte b):
    background(r, g, b),
    window(window),
    frameBuffer(
        CreateFrameBuffer(width, height)),
    renderMode(renderMode){
    camera = new Camera(SVector3(0, 0, -1), SVector3(0, 0, 0), SVector3(0, 1, 0));
    camera->setPerspective(S_CONST_FLOAT(90.0), S_CONST_FLOAT(1.0), S_CONST_FLOAT(0.01), S_CONST_FLOAT(10000.0));
}


Renderer::Renderer(unsigned width, unsigned height, GLFWwindow* window, RenderMode renderMode, SNormColor3 background):
    background(background),
    window(window),
    frameBuffer(
        CreateFrameBuffer(width, height)),
    renderMode(renderMode){
    camera = new Camera(SVector3(0, 0, -1), SVector3(0, 0, 0), SVector3(0, 1, 0));
    camera->setPerspective(S_CONST_FLOAT(90.0), S_CONST_FLOAT(1.0), S_CONST_FLOAT(0.01), S_CONST_FLOAT(10000.0));
}


Renderer::Renderer(unsigned int width, unsigned int height, GLFWwindow* window, RenderMode renderMode, SColor3 background):
    background(NORMALIZE_COLOR_3(background)),
    window(window),
    frameBuffer(
        CreateFrameBuffer(width, height)),
    renderMode(renderMode){
    camera = new Camera(SVector3(0, 0, -1), SVector3(0, 0, 0), SVector3(0, 1, 0));
    camera->setPerspective(S_CONST_FLOAT(90.0), S_CONST_FLOAT(1.0), S_CONST_FLOAT(0.01), S_CONST_FLOAT(10000.0));
}


Renderer::~Renderer(){
    ReleaseFrameBuffer(frameBuffer);
    delete camera;
}


void
Renderer::render(){
    glClear(GL_COLOR_BUFFER_BIT);
    SVector4 vetice(0, S_CONST_FLOAT(0.5), 0, 1);
    SVector4 v = camera->view() * vetice;
    v = camera->projection() * v;
    SFloat w = v.w;
    SVector3 v_ndc(v.x / w, v.y / w, v.z / w);

    SVector3 v_screen(
            (v_ndc.x * frameBuffer->width) / 2 + frameBuffer->width / 2,
            (v_ndc.y * frameBuffer->height) / 2 + frameBuffer->height / 2,
            v_ndc.z
    );
    PrintVector<3>(v_screen);
    ClearFrameBuffer(frameBuffer, background);
    DrawPixel(frameBuffer, SVector2Int((int)v_screen.x, (int)v_screen.y), SNormColor3(255, 0, 0));
    RenderFrameBuffer(frameBuffer);
    glfwSwapBuffers(window);
}


void
Renderer::setRenderMode(Renderer::RenderMode renderMode) {
    renderMode = renderMode;
}


void
Renderer::setVertices(std::vector<SVertex> vertices) {
    vertexBuffer = std::move(vertices);
}

void
Renderer::setVertices(SVertex *vertices, size_t numVertices) {
    vertexBuffer.assign(vertices, vertices + numVertices);
}


void
Renderer::setVertices(std::vector<SVertex> vertices, std::vector<SIndex> indices) {
    vertexBuffer = std::move(vertices);
    indexBuffer = std::move(indices);
}


void
Renderer::setVertices(SVertex *vertices, size_t numVertices, SIndex *indices, size_t numIndices) {
    vertexBuffer.assign(vertices, vertices + numVertices);
    indexBuffer.assign(indices, indices + numIndices);
}