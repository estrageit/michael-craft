#pragma once

typedef struct vertex_t{
    float pos[3];
    float normal[3];
    float texcoord[2];
} vertex_t;

void vert_set(vertex_t* v, float px, float py, float pz, float nx, float ny, float nz, float tx, float ty);
void vert_print(vertex_t* v);