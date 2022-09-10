#include "chunk.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

unsigned int chunk_v2s(unsigned int x, unsigned int y, unsigned int z){
    return x + z * 16 + y * 16 * 16;
}

void chunk_s2v(unsigned int* v, unsigned int scalar){
    v[1] = scalar / (16 * 16);
    v[2] = (scalar % (16 * 16)) / 16;
    v[0] = (scalar % (16 * 16)) % 16;
}

void chunk_b2c(int x, int y, int z, int* xc, int* ic){
    xc[0] = floorf(x / 16.0f);
    xc[1] = floorf(z / 16.0f);

    ic[0] = x - (xc[0] * 16);
    ic[1] = y;
    ic[2] = z - (xc[1] * 16);
}