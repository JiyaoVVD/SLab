#ifndef UNICODE
#define UNICODE
#endif

#include <windows.h>
#include <iostream>
#include <conio.h>
#include <gl/GL.h>
#include "GLFW/glfw3.h"

#include "stype.h"
#include "renderer.h"

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
    // disable window resize
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    // create window with glfw
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "SLab", NULL, NULL);
    if(!window){
        std::cerr << "Failed to create GLFW window." << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    glViewport(0, 0, WIDTH, HEIGHT);

    Renderer renderer(WIDTH, HEIGHT, window, Renderer::SLAB_LINE_MODE);
    auto camera = new Camera(SVector3(0, 0, S_CONST_FLOAT(-2.0)), SVector3(0, 0, 0), SVector3(0, 1, 0));
    camera->setPerspective(S_CONST_FLOAT(90.0), S_CONST_FLOAT(1.0), S_CONST_FLOAT(0.01), S_CONST_FLOAT(100.0));
    renderer.setCamera(camera);
    while(!glfwWindowShouldClose(window)){
        processInput(window);

        renderer.render();

        glfwPollEvents();
    }

    glfwTerminate();
    renderer.setCamera(nullptr);
    delete camera;
    return 0;
}
