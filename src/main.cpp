#ifndef UNICODE
#define UNICODE
#endif

#include <windows.h>
#include <iostream>
#include "glm.hpp"
#include <conio.h>
// #include <oneapi/tbb/parallel_for.h>
#include <chrono>
#include <gl/GL.h>
#include "GLFW/glfw3.h"
#include <algorithm>

#include "stype.h"
#include "framebuffer.h"

#define WIDTH 400LL
#define HEIGHT 400LL


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
