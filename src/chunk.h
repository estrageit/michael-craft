#pragma once

unsigned int chunk_v2s(unsigned int x, unsigned int y, unsigned int z);
void chunk_s2v(unsigned int* v, unsigned int scalar);

// x, y, z are input in world coordinates
// xc is 2 ints x,z of the chunk's position in world space
// ic is 3 ints x,y,z of the block in chunk space
void chunk_b2c(int x, int y, int z, int* xc, int* ic);