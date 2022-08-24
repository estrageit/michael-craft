#include <stdio.h>

#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include "chunk.h"

int wwidth = 640, wheight = 480;

void framebuffer_size_callback(GLFWwindow *window, int width, int height){
    glViewport(0, 0, width, height);
    wwidth = width;
    wheight = height;
}

int main(void){
    GLFWwindow* window;

    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(wwidth, wheight, "Deez", NULL, NULL);
    if (!window){
        glfwTerminate();
        return -1;
    }    

    glfwMakeContextCurrent(window);
    gladLoadGL(glfwGetProcAddress);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    glEnable(GL_DEPTH_TEST);  

    printf("[INFO] OpenGL Version: %s\n", glGetString(GL_VERSION));

    /*
    unsigned int s = chunk_v2s(6, 144, 10);
    printf("%d\n", s);
    unsigned int v[3];
    chunk_s2v(v, s);
    printf("%d %d %d\n", v[0], v[1], v[2]);
    */
   
    while (!glfwWindowShouldClose(window)){
        if(glfwGetKey(window, GLFW_KEY_ESCAPE))
            break;

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //draw

        glfwSwapBuffers(window);

        glfwPollEvents();
    }

    glfwTerminate();

    return 0;
}