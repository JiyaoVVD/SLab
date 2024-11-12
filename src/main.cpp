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


SVector3 calcCamPos(SVector3 center, SFloat deg, SFloat dist){
    SFloat rad = glm::radians(deg);
    SVector3 offset{
        glm::cos(rad) * dist,
        S_CONST_FLOAT(0.0),
        glm::sin(rad) * dist,
    };
    return center + offset;
}


int main(){
//    auto camera = new Camera(SVector3(S_CONST_FLOAT(0.0), S_CONST_FLOAT(0.0), S_CONST_FLOAT(-2.0)), SVector3(0, 0, 0), SVector3(0, 1, 0));
//    camera->setPerspective(S_CONST_FLOAT(60.0), (SFloat)WIDTH / HEIGHT, S_CONST_FLOAT(0.001), S_CONST_FLOAT(1000.0));
//    // camera->setOrthodox(S_CONST_FLOAT(-0.5), S_CONST_FLOAT(0.5), S_CONST_FLOAT(0.5), S_CONST_FLOAT(-0.5), S_CONST_FLOAT(0.001), S_CONST_FLOAT(100.0));
//    PrintMatrix<4, 4>(camera->projection());
//    PrintMatrix<4, 4>(glm::perspectiveLH(glm::radians(60.0f), 1.0f, 0.001f, 1000.0f));
//    auto p = camera->view() * SVector4(0, 0.5, 1.0, 1);
//    PrintVector<4>(p);
//    p = camera->projection() * p;
//    PrintVector<4>(p);

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

    auto deg = S_CONST_FLOAT(0.0);
    SVector3 center(0);
    SVector3 newPos;
    SFloat dist = S_CONST_FLOAT(2.0);

    Renderer renderer(WIDTH, HEIGHT, window, Renderer::SLAB_TRIANGLE_MODE);
    auto camera = new Camera(SVector3(S_CONST_FLOAT(0.0), S_CONST_FLOAT(0.0), S_CONST_FLOAT(-4.0)), SVector3(0, 0, 0), SVector3(0, 1, 0));
    camera->setPerspective(S_CONST_FLOAT(90.0), (SFloat)WIDTH / HEIGHT, S_CONST_FLOAT(0.01), S_CONST_FLOAT(1000.0));
    // camera->setOrthodox(S_CONST_FLOAT(-0.5), S_CONST_FLOAT(0.5), S_CONST_FLOAT(0.5), S_CONST_FLOAT(-0.5), S_CONST_FLOAT(0.001), S_CONST_FLOAT(100.0));
    renderer.setCamera(camera);
    while(!glfwWindowShouldClose(window)){
        processInput(window);

        deg += S_CONST_FLOAT(0.2);
        newPos = calcCamPos(center, deg, dist);
        camera->setPosition(newPos);
        camera->setLookat(center, SConst::up);

        renderer.render();

        glfwPollEvents();
    }

    glfwTerminate();
    renderer.setCamera(nullptr);
    delete camera;
    return 0;
}
