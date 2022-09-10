#include "input.h"

#include <GLFW/glfw3.h>

#include <string.h>
#include <stdlib.h>

input_t* input_make(char mouse_lock, void* win){
    input_t* r = malloc(sizeof(input_t));
    memset(r, 0, sizeof(input_t));
    if (mouse_lock){
        int ww, wh;
        glfwGetWindowSize(win, &ww, &wh);
        r->ml = 1;
        glfwSetCursorPos(win, ww / 2.0, wh / 2.0);
        glfwSetInputMode(win, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
    }
    
    r->kmap[I_K_MOV_FOWARD] =   GLFW_KEY_W;
    r->kmap[I_K_MOV_BACK] =     GLFW_KEY_S;
    r->kmap[I_K_MOV_LEFT] =     GLFW_KEY_A;
    r->kmap[I_K_MOV_RIGHT] =    GLFW_KEY_D;
    r->kmap[I_K_RELOAD] =       GLFW_KEY_R;
    r->kmap[I_K_MOUSE_LOCK] =   GLFW_KEY_E;
    r->kmap[I_K_QUIT] =         GLFW_KEY_ESCAPE;
    r->kmap[I_K_MOV_SPRINT] =   GLFW_KEY_LEFT_SHIFT;

    r->mmap[I_M_LEFT] =         GLFW_MOUSE_BUTTON_1;
    r->mmap[I_M_RIGHT] =        GLFW_MOUSE_BUTTON_2;
    r->mmap[I_M_MIDDLE] =       GLFW_MOUSE_BUTTON_3;

    return r;
}

char input_getkeydown(input_t* input, unsigned int key){
    return (input->kd >> key) & 1;
}
char input_getkeyup(input_t* input, unsigned int key){
    return (input->ku >> key) & 1;
}
char input_getkey(input_t* input, unsigned int key){
    return (input->kp >> key) & 1;
}

char input_getmousedown(input_t* input, unsigned int button){
    return (input->md >> button) & 1;
}
char input_getmouseup(input_t* input, unsigned int button){
    return (input->mu >> button) & 1;
}
char input_getmouse(input_t* input, unsigned int button){
    return (input->mp >> button) & 1;
}

void input_update(input_t* input, void* win){
    input->kd = 0;
    for (int i = 0; i < I_K_COUNT; i++)
        input->kd |= (~input->kp) & (glfwGetKey(win, input->kmap[i]) << ((long long)i));
    input->ku = 0;
    for (int i = 0; i < I_K_COUNT; i++)
        input->ku |= input->kp & (~(glfwGetKey(win, input->kmap[i]) << ((long long)i)));
    input->kp = 0;
    for (int i = 0; i < I_K_COUNT; i++)
        input->kp |= glfwGetKey(win, input->kmap[i]) << ((long long)i);

    input->md = 0;
    for (int i = 0; i < I_M_COUNT; i++)
        input->md |= (~input->kp) & (glfwGetMouseButton(win, input->mmap[i]) << ((long long)i));
    input->mu = 0;
    for (int i = 0; i < I_M_COUNT; i++)
        input->mu |= input->kp & (~(glfwGetMouseButton(win, input->mmap[i]) << ((long long)i)));
    input->mp = 0;
    for (int i = 0; i < I_M_COUNT; i++)
        input->mp |= glfwGetMouseButton(win, input->mmap[i]) << ((long long)i);

    if (input_getkeydown(input, I_K_MOUSE_LOCK)){
        input->ml ^= 1;
        if (input->ml){
            int ww, wh;
            glfwGetWindowSize(win, &ww, &wh);
            glfwSetCursorPos(win, ww / 2.0, wh / 2.0);
            glfwSetInputMode(win, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
        } else {
            glfwSetInputMode(win, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }
    }

    int ww, wh;
    glfwGetWindowSize(win, &ww, &wh);
    double cx, cy;
    glfwGetCursorPos(win, &cx, &cy);
    input->mx = ((cx / ww) * 2.0f) - 1.0f;
    input->my = -(((cy / wh) * 2.0f) - 1.0f);
    if(input->ml)
        glfwSetCursorPos(win, ww / 2.0, wh / 2.0);
}

void input_destroy(input_t* input){
    free(input);
}