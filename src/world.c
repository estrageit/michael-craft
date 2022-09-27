#include "world.h"

#include "link.h"
#include "chunk.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

world_t* world_make(){
    world_t* r = malloc(sizeof(world_t));
    r->chunks = NULL;
    return r;
}

void* world_getchunk(world_t* world, int x, int z){
    chunk_t* cur = world->chunks;
    while (cur != NULL){
        if (cur->x == x && cur->z == z){
            return cur->data;
        }
        cur = cur->next;
    }
    //read world data

    //generate chunk
    printf("[INFO] Generating new chunk at (%d, %d)\n", x, z);
    chunk_t* node = malloc(sizeof(chunk_t));
    node->next = NULL;
    node->x = x;
    node->z = z;

    //chunk generation
    memset(node->data, 0, sizeof(node->data));
    memset(node->data, 1, (16*16*(50 + abs(x) + abs(z)) + 1));

    lnk_push((lnk**)&world->chunks, (lnk*)node);
    return node->data;
}

char world_getblock(world_t* world, int x, int y, int z){
    chunk_t* cur = world->chunks;
    int cp[2], bp[3];
    chunk_b2c(x, y, z, cp, bp);
    while (cur != NULL){
        if (cur->x == cp[0] && cur->z == cp[1])
            return cur->data[chunk_v2s(bp[0], bp[1],  bp[2])];
        cur = cur->next;
    }
    printf("[ERROR] Tried to get block that doesn't exist (%d, %d, %d)\n", x, y, z);
    return 0;
}

void world_setblock(world_t* world, int x, int y, int z, char block){
    chunk_t* cur = world->chunks;
    int cp[2], bp[3];
    chunk_b2c(x, y, z, cp, bp);
    while (cur != NULL){
        if (cur->x == cp[0] && cur->z == cp[1]){
            cur->data[chunk_v2s(bp[0], bp[1],  bp[2])] = block;
            return;
        }
        cur = cur->next;
    }
    printf("[ERROR] Tried to set block that doesn't exist (%d, %d, %d)\n", x, y, z);
    return;
}

void world_destroy(world_t* world){
    lnk_destroy((lnk*)world->chunks);
    free(world);
}