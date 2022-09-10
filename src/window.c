#include "window.h"

#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <cglm/cglm.h>

#include <stdio.h>

void framebuffer_size_callback(GLFWwindow *window, int width, int height){
    glViewport(0, 0, width, height);
}

void* window_make(){
    GLFWwindow* window;

    if (!glfwInit()){
        printf("[FATAL] Could not start glfw\n");
        return NULL;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);  

    window = glfwCreateWindow(640, 480, "Michael Craft", NULL, NULL);
    if (!window){
        printf("[FATAL] Could not create window\n");
        glfwTerminate();
        return NULL;
    }    

    glfwMakeContextCurrent(window);
    gladLoadGL(glfwGetProcAddress);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    glEnable(GL_DEPTH_TEST);

    printf("[INFO] OpenGL Version: %s\n", glGetString(GL_VERSION));
    return window;
}

int window_run(void* window){
    return glfwWindowShouldClose(window);
}

void window_update(void* window){
    glfwSwapBuffers(window);
    glfwPollEvents();
}

void window_terminate(){
    glfwTerminate();
}

double window_gettime(){
    return glfwGetTime();
}

void window_setproj(void* window, void* u_proj){
    int wwidth, wheight;
    glfwGetWindowSize(window, &wwidth, &wheight);
    glm_perspective(glm_rad(90.0f), ((float)wwidth)/wheight, 0.1f, 1000.0f, u_proj);
}