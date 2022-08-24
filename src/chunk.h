#pragma once

typedef struct chunk_t{

    char data[16 * 16 * 256];
}chunk_t;

unsigned int chunk_v2s(unsigned int x, unsigned int y, unsigned int z);
void chunk_s2v(unsigned int* v, unsigned int scalar);
//void chunk_s2v(unsigned int* x, unsigned int* y, unsigned int* z, unsigned int scalar);