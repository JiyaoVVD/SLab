//
// Created by tangrui01 on 2024/11/4.
//

#include "renderer.h"


Renderer::Renderer(unsigned width, unsigned height, GLFWwindow* window):
    background(0, 0, 0),
    window(window),
    frameBuffer(
        CreateFrameBuffer(width, height)){}


Renderer::Renderer(unsigned width, unsigned height, GLFWwindow* window, SByte r, SByte g, SByte b):
    background(r, g, b),
    window(window),
    frameBuffer(
        CreateFrameBuffer(width, height)){}


Renderer::Renderer(unsigned width, unsigned height, GLFWwindow* window, SNormColor3 background):
    background(background),
    window(window),
    frameBuffer(
        CreateFrameBuffer(width, height)){}


Renderer::Renderer(unsigned int width, unsigned int height, GLFWwindow* window, SColor3 background):
    background(NORMALIZE_COLOR_3(background)),
    window(window),
    frameBuffer(
        CreateFrameBuffer(width, height)){}


Renderer::~Renderer(){
    ReleaseFrameBuffer(frameBuffer);
}


void
Renderer::render(){
    glClear(GL_COLOR_BUFFER_BIT);
    ClearFrameBuffer(frameBuffer, background);
    RenderFrameBuffer(frameBuffer);
    glfwSwapBuffers(window);
}