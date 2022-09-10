#pragma once

typedef struct scene_t{
    unsigned int vd;
    char** chunks;
    int cx, cz;
} scene_t;

unsigned int scene_v2s(unsigned int vd, int x, int z);

scene_t* scene_make(void* world, unsigned int view_distance);
void scene_update(scene_t* scene, void* world, void* player);

char scene_getblock(scene_t* scene, int x, int y, int z);
char scene_aircheck(scene_t* scene, int x, int y, int z);

void scene_setmodel(scene_t* scene, void* u_model);