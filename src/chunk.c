#include "chunk.h"

unsigned int chunk_v2s(unsigned int x, unsigned int y, unsigned int z){
    return x + z * 16 + y * 16 * 16;
}

void chunk_s2v(unsigned int* v, unsigned int scalar){
    v[1] = scalar / (16 * 16);
    v[2] = (scalar % (16 * 16)) / 16;
    v[0] = (scalar % (16 * 16)) % 16;
}

/*
void chunk_s2v(unsigned int* x, unsigned int* y, unsigned int* z, unsigned int scalar){
    *x = scalar / (16 * 16);
    *y = (scalar % (16 * 16)) / 16;
    *z = (scalar % (16 * 16)) % 16;
}*/