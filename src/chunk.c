#include "chunk.h"

#include <glad/gl.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

unsigned int chunk_v2s(unsigned int x, unsigned int y, unsigned int z){
    return x + z * 16 + y * 16 * 16;
}

void chunk_s2v(unsigned int* v, unsigned int scalar){
    v[1] = scalar / (16 * 16);
    v[2] = (scalar % (16 * 16)) / 16;
    v[0] = (scalar % (16 * 16)) % 16;
}

/* void chunk_s2v(unsigned int* x, unsigned int* y, unsigned int* z, unsigned int scalar){
    *x = scalar / (16 * 16);
    *y = (scalar % (16 * 16)) / 16;
    *z = (scalar % (16 * 16)) % 16;
}*/

// make this faster by not converting back and forth
char chunk_aircheck(char* data, int x, int y, int z){
    return (x < 0 || x > 15 || y < 0 || y > 255 || z < 0 || z > 15 ||
     data[chunk_v2s(x, y, z)] == 0);
}  

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

void chunk_update(chunk_t* chunk){
    unsigned int facec = 0;
    for (int i = 0; i < 16 * 16 * 256; i++){
        if (chunk->data[i] != 0){
            unsigned int p[3];
            chunk_s2v(p, i);
            facec += chunk_aircheck(chunk->data, p[0]+1, p[1], p[2]);
            facec += chunk_aircheck(chunk->data, p[0]-1, p[1], p[2]);
            facec += chunk_aircheck(chunk->data, p[0], p[1]+1, p[2]);
            facec += chunk_aircheck(chunk->data, p[0], p[1]-1, p[2]);
            facec += chunk_aircheck(chunk->data, p[0], p[1], p[2]+1);
            facec += chunk_aircheck(chunk->data, p[0], p[1], p[2]-1);
        }
    }
    vertex_t* v = malloc(sizeof(vertex_t) * facec * 4);
    unsigned int* indices = malloc(6 * facec * sizeof(unsigned int));

    for (int i = 0; i < facec; i++){
        indices[0 + i * 6] = 0 + i * 4;
        indices[1 + i * 6] = 1 + i * 4;
        indices[2 + i * 6] = 2 + i * 4;
        indices[3 + i * 6] = 2 + i * 4;
        indices[4 + i * 6] = 3 + i * 4;
        indices[5 + i * 6] = 0 + i * 4;
    }

    printf("%d %ld\n", facec, sizeof(vertex_t) * facec * 4);

    unsigned int curf = 0;
    for (int i = 0; i < 16 * 16 * 256; i++){
        if (chunk->data[i] != 0){
            unsigned int p[3];
            chunk_s2v(p, i);
            if(chunk_aircheck(chunk->data, p[0]+1, p[1], p[2])){
                vert_set(  v+curf, p[0]+1,   p[1],   p[2], 1, 0, 0, (chunk->data[i] + 1) * (1/256.0f), 2/6.f);
                vert_set(v+1+curf, p[0]+1, p[1]+1,   p[2], 1, 0, 0, (chunk->data[i] + 1) * (1/256.0f), 1/6.f);
                vert_set(v+2+curf, p[0]+1, p[1]+1, p[2]+1, 1, 0, 0, (chunk->data[i] + 0) * (1/256.0f), 1/6.f);
                vert_set(v+3+curf, p[0]+1,   p[1], p[2]+1, 1, 0, 0, (chunk->data[i] + 0) * (1/256.0f), 2/6.f);
                curf += 4;
            }
            if(chunk_aircheck(chunk->data, p[0]-1, p[1], p[2])){ 
                vert_set(  v+curf, p[0],   p[1],   p[2],-1, 0, 0, (chunk->data[i] + 1) * (1/256.0f), 3/6.f);
                vert_set(v+1+curf, p[0], p[1]+1,   p[2],-1, 0, 0, (chunk->data[i] + 1) * (1/256.0f), 2/6.f);
                vert_set(v+2+curf, p[0], p[1]+1, p[2]+1,-1, 0, 0, (chunk->data[i] + 0) * (1/256.0f), 2/6.f);
                vert_set(v+3+curf, p[0],   p[1], p[2]+1,-1, 0, 0, (chunk->data[i] + 0) * (1/256.0f), 3/6.f);
                curf += 4;
            }
            if(chunk_aircheck(chunk->data, p[0], p[1]+1, p[2])){ 
                vert_set(  v+curf,   p[0], p[1]+1,   p[2], 0, 1, 0, (chunk->data[i] + 0) * (1/256.0f), 0.f);
                vert_set(v+1+curf, p[0]+1, p[1]+1,   p[2], 0, 1, 0, (chunk->data[i] + 1) * (1/256.0f), 0.f);
                vert_set(v+2+curf, p[0]+1, p[1]+1, p[2]+1, 0, 1, 0, (chunk->data[i] + 1) * (1/256.0f), 1/6.f);
                vert_set(v+3+curf,   p[0], p[1]+1, p[2]+1, 0, 1, 0, (chunk->data[i] + 0) * (1/256.0f), 1/6.f);
                curf += 4;
            }
            if(chunk_aircheck(chunk->data, p[0], p[1]-1, p[2])){ 
                vert_set(  v+curf,   p[0], p[1],   p[2], 0,-1, 0, (chunk->data[i] + 0) * (1/256.0f), 5/6.f);
                vert_set(v+1+curf, p[0]+1, p[1],   p[2], 0,-1, 0, (chunk->data[i] + 1) * (1/256.0f), 5/6.f);
                vert_set(v+2+curf, p[0]+1, p[1], p[2]+1, 0,-1, 0, (chunk->data[i] + 1) * (1/256.0f),   1.f);
                vert_set(v+3+curf,   p[0], p[1], p[2]+1, 0,-1, 0, (chunk->data[i] + 0) * (1/256.0f),   1.f);
                curf += 4;
            }
            if(chunk_aircheck(chunk->data, p[0], p[1], p[2]+1)){ 
                vert_set(  v+curf,   p[0],   p[1], p[2]+1, 0, 0, 1, (chunk->data[i] + 1) * (1/256.0f), 4/6.f);
                vert_set(v+1+curf, p[0]+1,   p[1], p[2]+1, 0, 0, 1, (chunk->data[i] + 1) * (1/256.0f), 4/6.f);
                vert_set(v+2+curf, p[0]+1, p[1]+1, p[2]+1, 0, 0, 1, (chunk->data[i] + 0) * (1/256.0f), 3/6.f);
                vert_set(v+3+curf,   p[0], p[1]+1, p[2]+1, 0, 0, 1, (chunk->data[i] + 0) * (1/256.0f), 3/6.f);
                curf += 4;
            }
            if(chunk_aircheck(chunk->data, p[0], p[1], p[2]-1)){
                vert_set(  v+curf,   p[0],   p[1], p[2], 0, 0,-1, (chunk->data[i] + 0) * (1/256.0f), 5/6.f);
                vert_set(v+1+curf, p[0]+1,   p[1], p[2], 0, 0,-1, (chunk->data[i] + 1) * (1/256.0f), 5/6.f);
                vert_set(v+2+curf, p[0]+1, p[1]+1, p[2], 0, 0,-1, (chunk->data[i] + 1) * (1/256.0f), 4/6.f);
                vert_set(v+3+curf,   p[0], p[1]+1, p[2], 0, 0,-1, (chunk->data[i] + 0) * (1/256.0f), 4/6.f);
                curf += 4;
            }
        }
    }
    //glDeleteBuffers(1, &chunk->ibo);
    //glDeleteBuffers(1, &chunk->vbo);

    //glGenBuffers(1, &chunk->ibo);
    //glGenBuffers(1, &chunk->vbo);

    glBindBuffer(GL_ARRAY_BUFFER, chunk->vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, chunk->ibo);

    chunk->count = 6 * facec;

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_t) * facec * 4, v, GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * facec * sizeof(unsigned int), indices, GL_STATIC_DRAW);

    free(v);
    free(indices);
}

chunk_t* chunk_make(int x, int z){
    chunk_t* r = malloc(sizeof(chunk_t));

    glGenBuffers(1, &r->ibo);
    glGenBuffers(1, &r->vbo);

    glBindBuffer(GL_ARRAY_BUFFER, r->vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, r->ibo);

    glBufferData(GL_ARRAY_BUFFER, 0, (void*)0, GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 0, (void*)0, GL_STATIC_DRAW);

    memset(r->data, 0, sizeof(r->data));
    r->count = 0;

    return r;
}

void chunk_bind(chunk_t* chunk){
    glBindBuffer(GL_ARRAY_BUFFER, chunk->vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, chunk->ibo);
}

void chunk_draw(chunk_t* chunk){
    glDrawElements(GL_TRIANGLES, chunk->count, GL_UNSIGNED_INT, NULL);
}