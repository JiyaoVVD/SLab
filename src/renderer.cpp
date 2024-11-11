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
    auto testVertices = new SVertex[]{
            {
                    {S_CONST_FLOAT(-0.5), S_CONST_FLOAT(-0.5), S_CONST_FLOAT(-0.5)},
                    {S_CONST_FLOAT(0.0), S_CONST_FLOAT(0.0), S_CONST_FLOAT(0.0)},
                    {S_CONST_FLOAT(1.0), S_CONST_FLOAT(1.0), S_CONST_FLOAT(1.0)},
            },
            {
                    {S_CONST_FLOAT(-0.5), S_CONST_FLOAT(0.5), S_CONST_FLOAT(-0.5)},
                    {S_CONST_FLOAT(0.0), S_CONST_FLOAT(0.0), S_CONST_FLOAT(0.0)},
                    {S_CONST_FLOAT(1.0), S_CONST_FLOAT(1.0), S_CONST_FLOAT(1.0)},
            },
            {
                    {S_CONST_FLOAT(0.5), S_CONST_FLOAT(0.5), S_CONST_FLOAT(-0.5)},
                    {S_CONST_FLOAT(0.0), S_CONST_FLOAT(0.0), S_CONST_FLOAT(0.0)},
                    {S_CONST_FLOAT(1.0), S_CONST_FLOAT(1.0), S_CONST_FLOAT(1.0)},
            },
            {
                    {S_CONST_FLOAT(0.5), S_CONST_FLOAT(-0.5), S_CONST_FLOAT(-0.5)},
                    {S_CONST_FLOAT(0.0), S_CONST_FLOAT(0.0), S_CONST_FLOAT(0.0)},
                    {S_CONST_FLOAT(1.0), S_CONST_FLOAT(1.0), S_CONST_FLOAT(1.0)},
            },
            {
                    {S_CONST_FLOAT(-0.5), S_CONST_FLOAT(-0.5), S_CONST_FLOAT(0.5)},
                    {S_CONST_FLOAT(0.0), S_CONST_FLOAT(0.0), S_CONST_FLOAT(0.0)},
                    {S_CONST_FLOAT(1.0), S_CONST_FLOAT(1.0), S_CONST_FLOAT(1.0)},
            },
            {
                    {S_CONST_FLOAT(-0.5), S_CONST_FLOAT(0.5), S_CONST_FLOAT(0.5)},
                    {S_CONST_FLOAT(0.0), S_CONST_FLOAT(0.0), S_CONST_FLOAT(0.0)},
                    {S_CONST_FLOAT(1.0), S_CONST_FLOAT(1.0), S_CONST_FLOAT(1.0)},
            },
            {
                    {S_CONST_FLOAT(0.5), S_CONST_FLOAT(0.5), S_CONST_FLOAT(0.5)},
                    {S_CONST_FLOAT(0.0), S_CONST_FLOAT(0.0), S_CONST_FLOAT(0.0)},
                    {S_CONST_FLOAT(1.0), S_CONST_FLOAT(1.0), S_CONST_FLOAT(1.0)},
            },
            {
                    {S_CONST_FLOAT(0.5), S_CONST_FLOAT(-0.5), S_CONST_FLOAT(0.5)},
                    {S_CONST_FLOAT(0.0), S_CONST_FLOAT(0.0), S_CONST_FLOAT(0.0)},
                    {S_CONST_FLOAT(1.0), S_CONST_FLOAT(1.0), S_CONST_FLOAT(1.0)},
            },
    };
    auto indices = new SIndex[]{
        0, 1,
        1, 2,
        2, 3,
        3, 0,
        4, 5,
        5, 6,
        6, 7,
        7, 4,
//        0, 4,
//        1, 5,
//        2, 6,
//        3, 7
    };
    setVertices(testVertices, 8, indices, 16);
}


Renderer::Renderer(unsigned width, unsigned height, GLFWwindow* window, RenderMode renderMode, SByte r, SByte g, SByte b):
    background(r, g, b),
    window(window),
    frameBuffer(
        CreateFrameBuffer(width, height)),
    renderMode(renderMode){
}


Renderer::Renderer(unsigned width, unsigned height, GLFWwindow* window, RenderMode renderMode, SNormColor3 background):
    background(background),
    window(window),
    frameBuffer(
        CreateFrameBuffer(width, height)),
    renderMode(renderMode){
}


Renderer::Renderer(unsigned int width, unsigned int height, GLFWwindow* window, RenderMode renderMode, SColor3 background):
    background(NORMALIZE_COLOR_3(background)),
    window(window),
    frameBuffer(
        CreateFrameBuffer(width, height)),
    renderMode(renderMode){
}


Renderer::~Renderer(){
    ReleaseFrameBuffer(frameBuffer);
    delete camera;
}


void
Renderer::render(){
    assert(camera);
    glClear(GL_COLOR_BUFFER_BIT);
    ClearFrameBuffer(frameBuffer, background);

    switch(renderMode){
        case SLAB_LINE_MODE:
            drawLineMode();
            break;
        case SLAB_LINE_LOOP_MODE:
            drawLineLoopMode();
            break;
        case SLAB_TRIANGLE_MODE:
            drawTriangleMode();
            break;
    }

    RenderFrameBuffer(frameBuffer);
    glfwSwapBuffers(window);
}


void
Renderer::drawLineLoopMode(){
    std::vector<SVector3> points;
    for(auto vertex: vertexBuffer){
        SVector4 p(vertex.position, S_CONST_FLOAT(1.0));
        p = camera->view() * p;
        p = camera->projection() * p;
        SFloat w = p.w;
        SVector3 pNdc(p.x / w, p.y / w, p.z / 2);
        SVector3Int pScreen(
                (pNdc.x * (SFloat)frameBuffer->width) / 2 + (SFloat)frameBuffer->width / 2,
                (pNdc.y * (SFloat)frameBuffer->height) / 2 + (SFloat)frameBuffer->height / 2,
                pNdc.z
        );
        points.emplace_back(pScreen);
    }
    auto sizeIndices = indexBuffer.size();
    for(int i = 0; i < indexBuffer.size(); ++i){
        SIndex start = indexBuffer[i];
        SIndex end = indexBuffer[(i + 1) % sizeIndices];
        SVector3Int startPoint = points[start];
        SVector3Int endPoint = points[end];
        DrawLine(frameBuffer, startPoint, endPoint, SNormColor3(255, 255, 255));
    }
}


void
Renderer::drawLineMode(){
    std::vector<SVector3> points;
    for(auto vertex: vertexBuffer){
        SVector4 p(vertex.position, S_CONST_FLOAT(1.0));
        p = camera->view() * p;
        SFloat w = p.z;
        p = camera->projection() * p;
        SVector3 pNdc(p.x / w, p.y / w, p.z);
        SVector3Int pScreen(
                (pNdc.x * (SFloat)frameBuffer->width) / 2 + (SFloat)frameBuffer->width / 2,
                (pNdc.y * (SFloat)frameBuffer->height) / 2 + (SFloat)frameBuffer->height / 2,
                pNdc.z
        );
        points.emplace_back(pScreen);
    }
    auto sizeIndices = indexBuffer.size();
    assert(!(sizeIndices % 2));
    for(int i = 0; i < indexBuffer.size(); i += 2){
        SIndex start = indexBuffer[i];
        SIndex end = indexBuffer[(i + 1) % sizeIndices];
        SVector3Int startPoint = points[start];
        SVector3Int endPoint = points[end];
        DrawLine(frameBuffer, startPoint, endPoint, SNormColor3(255, 255, 255));
    }
}


void
Renderer::drawTriangleMode() {

}


void
Renderer::setRenderMode(Renderer::RenderMode renderMode) {
    renderMode = renderMode;
}


void
Renderer::setVertices(std::vector<SVertex> vertices) {
    vertexBuffer = std::move(vertices);
    indexBuffer.clear();
    for(int i = 0; i < vertexBuffer.size(); ++i){
        indexBuffer.emplace_back(i);
    }
}

void
Renderer::setVertices(SVertex *vertices, size_t numVertices) {
    vertexBuffer.assign(vertices, vertices + numVertices);
    indexBuffer.clear();
    for(int i = 0; i < vertexBuffer.size(); ++i){
        indexBuffer.emplace_back(i);
    }
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

void
Renderer::setCamera(const Camera* cam){
    camera = cam;
}