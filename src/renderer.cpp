//
// Created by tangrui01 on 2024/11/4.
//

#include "renderer.h"

#include <utility>
#include <random>


SColor4 RandomColor(){
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0.0, 1.0);
    SColor4 color{
        dis(gen),
        dis(gen),
        dis(gen),
        S_CONST_FLOAT(1.0)
    };
    PrintVector<4>(color);
    return color;
}


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
                    RandomColor()
            },
            {
                    {S_CONST_FLOAT(-0.5), S_CONST_FLOAT(0.5), S_CONST_FLOAT(-0.5)},
                    {S_CONST_FLOAT(0.0), S_CONST_FLOAT(0.0), S_CONST_FLOAT(0.0)},
                RandomColor()
            },
            {
                    {S_CONST_FLOAT(0.5), S_CONST_FLOAT(0.5), S_CONST_FLOAT(-0.5)},
                    {S_CONST_FLOAT(0.0), S_CONST_FLOAT(0.0), S_CONST_FLOAT(0.0)},
                RandomColor()
            },
            {
                    {S_CONST_FLOAT(0.5), S_CONST_FLOAT(-0.5), S_CONST_FLOAT(-0.5)},
                    {S_CONST_FLOAT(0.0), S_CONST_FLOAT(0.0), S_CONST_FLOAT(0.0)},
                RandomColor()
            },
            {
                    {S_CONST_FLOAT(-0.5), S_CONST_FLOAT(-0.5), S_CONST_FLOAT(0.5)},
                    {S_CONST_FLOAT(0.0), S_CONST_FLOAT(0.0), S_CONST_FLOAT(0.0)},
                RandomColor()
            },
            {
                    {S_CONST_FLOAT(-0.5), S_CONST_FLOAT(0.5), S_CONST_FLOAT(0.5)},
                    {S_CONST_FLOAT(0.0), S_CONST_FLOAT(0.0), S_CONST_FLOAT(0.0)},
                RandomColor()
            },
            {
                    {S_CONST_FLOAT(0.5), S_CONST_FLOAT(0.5), S_CONST_FLOAT(0.5)},
                    {S_CONST_FLOAT(0.0), S_CONST_FLOAT(0.0), S_CONST_FLOAT(0.0)},
                    {S_CONST_FLOAT(1.0), S_CONST_FLOAT(0.0), S_CONST_FLOAT(0.0), S_CONST_FLOAT(0.0)}
            },
            {
                    {S_CONST_FLOAT(0.5), S_CONST_FLOAT(-0.5), S_CONST_FLOAT(0.5)},
                    {S_CONST_FLOAT(0.0), S_CONST_FLOAT(0.0), S_CONST_FLOAT(0.0)},
                    {S_CONST_FLOAT(1.0), S_CONST_FLOAT(0.0), S_CONST_FLOAT(0.0), S_CONST_FLOAT(0.0)}
            },
    };
    auto indices = new SIndex[]{
        // front
        2, 1, 0,
        3, 2, 0,
        // back
        4, 5, 6,
        4, 6, 7,
        // top
        1, 6, 5,
        1, 2, 6,
        // bottom
        0, 4, 3,
        4, 7, 3,
        // left
        4, 0, 5,
        0, 1, 5,
        //right
        3, 6, 2,
        3, 7, 6,
    };
    setVertices(testVertices, 8, indices, 36);
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
        auto colorStart = NORMALIZE_COLOR_4(vertexBuffer[start].color);
        auto colorEnd = NORMALIZE_COLOR_4(vertexBuffer[end].color);
        DrawLine(frameBuffer, startPoint, endPoint, colorStart, colorEnd);
    }
}


void
Renderer::drawLineMode(){
    std::vector<SVector3> points;
    for(auto vertex: vertexBuffer){
        SVector4 p(vertex.position, S_CONST_FLOAT(1.0));
        p = camera->view() * p;
        p = camera->projection() * p;
        SFloat w = p.w;
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
        auto colorStart = NORMALIZE_COLOR_4(vertexBuffer[start].color);
        auto colorEnd = NORMALIZE_COLOR_4(vertexBuffer[end].color);
        DrawLine(frameBuffer, startPoint, endPoint, colorStart, colorEnd);
    }
}


void
Renderer::drawTriangleMode() {
    std::vector<SVector3> points;
    for(auto vertex: vertexBuffer){
        SVector4 p(vertex.position, S_CONST_FLOAT(1.0));
        p = camera->view() * p;
        p = camera->projection() * p;
        SFloat w = p.w;
        SVector3 pNdc(p.x / w, p.y / w, p.z);
        SVector3Int pScreen(
                (pNdc.x * (SFloat)frameBuffer->width) / 2 + (SFloat)frameBuffer->width / 2,
                (pNdc.y * (SFloat)frameBuffer->height) / 2 + (SFloat)frameBuffer->height / 2,
                pNdc.z
        );
        points.emplace_back(pScreen);
    }
    auto sizeIndices = indexBuffer.size();
    assert(!(sizeIndices % 3));
    for(int i = 0; i < indexBuffer.size(); i += 3){
        SIndex i0 = indexBuffer[i];
        SIndex i1 = indexBuffer[i + 1];
        SIndex i2 = indexBuffer[i + 2];
        SVector3Int v0 = points[i0];
        SVector3Int v1 = points[i1];
        SVector3Int v2 = points[i2];
        auto c0 = NORMALIZE_COLOR_4(vertexBuffer[i0].color);
        auto c1 = NORMALIZE_COLOR_4(vertexBuffer[i1].color);
        auto c2 = NORMALIZE_COLOR_4(vertexBuffer[i2].color);
        DrawTriangle(frameBuffer, v0, v1, v2, c0, c1, c2);
    }
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