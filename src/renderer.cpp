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
    return color;
}


Renderer::Renderer(unsigned width, unsigned height, GLFWwindow* window, RenderMode renderMode):
    width(width), height(height),
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

            {
                    {S_CONST_FLOAT(-1.0), S_CONST_FLOAT(-1.0), S_CONST_FLOAT(-1.0)},
                    {S_CONST_FLOAT(0.0), S_CONST_FLOAT(0.0), S_CONST_FLOAT(0.0)},
                    RandomColor()
            },
            {
                    {S_CONST_FLOAT(-1.0), S_CONST_FLOAT(1.0), S_CONST_FLOAT(-1.0)},
                    {S_CONST_FLOAT(0.0), S_CONST_FLOAT(0.0), S_CONST_FLOAT(0.0)},
                    RandomColor()
            },
            {
                    {S_CONST_FLOAT(1.0), S_CONST_FLOAT(1.0), S_CONST_FLOAT(-1.0)},
                    {S_CONST_FLOAT(0.0), S_CONST_FLOAT(0.0), S_CONST_FLOAT(0.0)},
                    RandomColor()
            },
            {
                    {S_CONST_FLOAT(1.0), S_CONST_FLOAT(-1.0), S_CONST_FLOAT(-1.0)},
                    {S_CONST_FLOAT(0.0), S_CONST_FLOAT(0.0), S_CONST_FLOAT(0.0)},
                    RandomColor()
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

        10, 9, 8,
        11, 10, 8,
    };
    setVertices(testVertices, 12, indices, 42);
}


Renderer::Renderer(unsigned width, unsigned height, GLFWwindow* window, RenderMode renderMode, SByte r, SByte g, SByte b):
    width(width), height(height),
    background(r, g, b),
    window(window),
    frameBuffer(
        CreateFrameBuffer(width, height)),
    renderMode(renderMode){
}


Renderer::Renderer(unsigned width, unsigned height, GLFWwindow* window, RenderMode renderMode, SNormColor3 background):
    width(width), height(height),
    background(background),
    window(window),
    frameBuffer(
        CreateFrameBuffer(width, height)),
    renderMode(renderMode){
}


Renderer::Renderer(unsigned int width, unsigned int height, GLFWwindow* window, RenderMode renderMode, SColor3 background):
    width(width), height(height),
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
    fragBuffer.clear();

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
Renderer::rasterizeTriangle(
        const SVertex& v0,
        const SVertex& v1,
        const SVertex& v2) {
    auto p0 = (SVector2Int) v0.position.xy();
    auto p1 = (SVector2Int) v1.position.xy();
    auto p2 = (SVector2Int) v2.position.xy();

    auto lx = std::min(std::min(p0.x, p1.x), p2.x);
    auto rx = std::max(std::max(p0.x, p1.x), p2.x);
    auto by = std::max(std::max(p0.y, p1.y), p2.y);
    auto ty = std::min(std::min(p0.y, p1.y), p2.y);

    auto normal = glm::normalize(v0.normal + v1.normal + v2.normal);

    for (int x = lx; x <= rx; ++x) {
        for (int y = ty; y <= by; ++y) {
            SVector2Int point(x, y);

            auto crs0 = icross2(p1 - p0, point - p0);
            auto crs1 = icross2(p2 - p1, point - p1);
            auto crs2 = icross2(p0 - p2, point - p2);
            if ((crs0 >= 0 && crs1 >= 0 && crs2 >= 0)) // ||
                //(crs0 <= 0 && crs1 <= 0 && crs2 <= 0))
            {
                // TODO: to be optimized
                auto w0 = (SFloat) (-(x - p1.x) * (p2.y - p1.y) + (y - p1.y) * (p2.x - p1.x)) /
                          (-(p0.x - p1.x) * (p2.y - p1.y) + (p0.y - p1.y) * (p2.x - p1.x));
                auto w1 = (SFloat) (-(x - p2.x) * (p0.y - p2.y) + (y - p2.y) * (p0.x - p2.x)) /
                          (-(p1.x - p2.x) * (p0.y - p2.y) + (p1.y - p2.y) * (p0.x - p2.x));
                auto w2 = S_CONST_FLOAT(1.0) - w0 - w1;
                auto color = v0.color.rgb() * w0 + v1.color.rgb() * w1 + v2.color.rgb() * w2;
                auto alpha = v0.color.a * w0 + v1.color.a * w1 + v2.color.a * w2;
                auto depth = v0.position.z * w0 + v1.position.z * w1 + v2.position.z * w2;
                fragBuffer.emplace_back(SFragment{
                        {x, y},
                        {normal},
                        {color, alpha},
                        depth
                });
            }
        }
    }
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


inline SVertex
Renderer::processVertex(const SVertex &vertex) {
    SVector4 p(vertex.position, S_CONST_FLOAT(1.0));
    p = camera->view() * p;
    p = camera->projection() * p;
    SFloat w = p.w;
    SVector3 pNdc(p.x / w, p.y / w, p.z);

    return {
            {
                (pNdc.x * (SFloat)frameBuffer->width) / 2 + (SFloat)frameBuffer->width / 2,
                (pNdc.y * (SFloat)frameBuffer->height) / 2 + (SFloat)frameBuffer->height / 2,
                S_CONST_FLOAT(1.0) / pNdc.z},
            {vertex.normal},
            {vertex.color},
    };
}


void
Renderer::drawTriangleMode() {
    std::vector<SVertex> vertexOut;
    for(auto vertex: vertexBuffer){
        vertexOut.emplace_back(processVertex(vertex));
    }
    auto sizeIndices = indexBuffer.size();
    assert(!(sizeIndices % 3));
    for(int i = 0; i < sizeIndices; i += 3){
        SIndex i0 = indexBuffer[i];
        SIndex i1 = indexBuffer[i + 1];
        SIndex i2 = indexBuffer[i + 2];
        const SVertex& v0 = vertexOut[i0];
        const SVertex& v1 = vertexOut[i1];
        const SVertex& v2 = vertexOut[i2];
        rasterizeTriangle(v0, v1, v2);
    }

    for(auto frag: fragBuffer){
        DrawPixel(frameBuffer, frag.coord, NORMALIZE_COLOR_3(frag.color), frag.depth);
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