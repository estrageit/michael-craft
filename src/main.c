#include <stdio.h>
#include <string.h>

#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <cglm/cglm.h>

#include "chunk.h"
#include "shader.h"
#include "texture.h"

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

    window = glfwCreateWindow(wwidth, wheight, "Michael Craft", NULL, NULL);
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
    unsigned int vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    unsigned int shader = shader_make_from_file("res/shaders/chunk.glsl", NULL);
    glUseProgram(shader);

    unsigned int texture = texture_load("res/textures/blocks.png");
    glBindTexture(GL_TEXTURE_2D, texture);

    chunk_t* chunk = chunk_make(0, 0);
    memset(chunk->data, 1, sizeof(chunk->data));
    chunk->data[chunk_v2s(12, 60, 2)] = 3;
    chunk->data[chunk_v2s(12, 60, 3)] = 1;
    double t1 = glfwGetTime();
    chunk_update(chunk);
    printf("chunk update time: %fs\n", glfwGetTime() - t1);
    chunk_bind(chunk);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex_t), (void*)offsetof(vertex_t, pos));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vertex_t), (void*)offsetof(vertex_t, normal));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(vertex_t), (void*)offsetof(vertex_t, texcoord));

    while (!glfwWindowShouldClose(window)){
        if (glfwGetKey(window, GLFW_KEY_ESCAPE))
            break;
        if (glfwGetKey(window, GLFW_KEY_E)){
            t1 = glfwGetTime();
            chunk_update(chunk);
            printf("chunk update time: %fs\n", glfwGetTime() - t1);
        }

        mat4 u_proj, u_view, u_model;

        glm_mat4_identity(u_model);
        glm_lookat((vec3){sin(glfwGetTime()) * 5, 62, cos(glfwGetTime()) * 5}, (vec3){12, 60, 2}, (vec3){0, 1, 0}, u_view);
        glm_perspective(glm_rad(90.0f), ((float)wwidth)/wheight, 0.1f, 100.0f, u_proj);

        glUniform3f(glGetUniformLocation(shader, "u_lightdir"), -1, -3, -2);
        glUniformMatrix4fv(glGetUniformLocation(shader, "u_proj"), 1, 0, (float*)u_proj);
        glUniformMatrix4fv(glGetUniformLocation(shader, "u_view"), 1, 0, (float*)u_view);
        glUniformMatrix4fv(glGetUniformLocation(shader, "u_model"), 1, 0, (float*)u_model);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //draw
        chunk_bind(chunk);
        printf("bound %d\n", glGetError());
        chunk_draw(chunk);
        printf("draw\n");

        glfwSwapBuffers(window);

        glfwPollEvents();
    }
    glDeleteVertexArrays(1, &vao);
    glDeleteProgram(shader);

    glfwTerminate();

    return 0;
}