#pragma once

#define I_K_MOV_FOWARD  00
#define I_K_MOV_BACK    01
#define I_K_MOV_LEFT    02
#define I_K_MOV_RIGHT   03
#define I_K_RELOAD      04
#define I_K_MOUSE_LOCK  05
#define I_K_QUIT        06
#define I_K_MOV_SPRINT  07

#define I_M_RIGHT       00
#define I_M_LEFT        01
#define I_M_MIDDLE      02

#define I_K_COUNT       8
#define I_M_COUNT       03

typedef struct input_t{
    long long kd, ku, kp;
    char md, mu, mp;
    float mx, my;
    double ms;
    char ml;
    int kmap[I_K_COUNT];
    int mmap[I_M_COUNT];
} input_t;

input_t* input_make(char mouse_lock, void* win);
void input_update(input_t* input, void* win);
void input_destroy(input_t* input);

char input_getkeydown(input_t* input, unsigned int key);
char input_getkeyup(input_t* input, unsigned int key);
char input_getkey(input_t* input, unsigned int key);
char input_getmousedown(input_t* input, unsigned int button);
char input_getmouseup(input_t* input, unsigned int button);
char input_getmouse(input_t* input, unsigned int button);
