#pragma once

typedef struct vertex_t{
    float pos[3];
    float normal[3];
    float texcoord[2];
} vertex_t;

typedef struct chunk_t{
    unsigned int ibo, vbo, count;
    char data[16 * 16 * 256];
}chunk_t;

unsigned int chunk_v2s(unsigned int x, unsigned int y, unsigned int z);
void chunk_s2v(unsigned int* v, unsigned int scalar);
//void chunk_s2v(unsigned int* x, unsigned int* y, unsigned int* z, unsigned int scalar);

void chunk_update(chunk_t* chunk);
void chunk_bind(chunk_t* chunk);
void chunk_draw(chunk_t* chunk);
chunk_t* chunk_make(int x, int z);