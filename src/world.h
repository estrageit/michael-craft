#pragma once

typedef struct chunk_t{
    struct chunk_t* next;
    int x, z;
    char data[16 * 16 * 256];
} chunk_t;

typedef struct world_t{
    chunk_t *chunks;
} world_t;

world_t* world_make();
void* world_getchunk(world_t* world, int x, int z);
char world_getblock(world_t* world, int x, int y, int z);
void world_setblock(world_t* world, int x, int y, int z, char block);
void world_destroy(world_t* world);