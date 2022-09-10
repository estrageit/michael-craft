#include "vertex.h"

#include <stdio.h>

void vert_set(vertex_t* v, float px, float py, float pz, float nx, float ny, float nz, float tx, float ty){
    v->pos[0] = px;
    v->pos[1] = py;
    v->pos[2] = pz;

    v->normal[0] = nx;
    v->normal[1] = ny;
    v->normal[2] = nz;

    v->texcoord[0] = tx;
    v->texcoord[1] = ty;
}

void vert_print(vertex_t* v){
    printf("(%f, %f, %f), (%f, %f, %f), (%f, %f)\n",
    v->pos[0], v->pos[1], v->pos[2],
    v->normal[0], v->normal[1], v->normal[2],
    v->texcoord[0], v->texcoord[1]);
}