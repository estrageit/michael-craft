#include "scene.h"

#include <cglm/cglm.h>

#include "chunk.h"
#include "world.h"
#include "player.h"

unsigned int scene_v2s(unsigned int vd, int x, int z){
    return x + vd + (z + vd) * (1 + 2 * vd);
}

void scene_updatechunks(scene_t* scene, world_t* world){
    for (int x = -scene->vd; x <= (int)scene->vd; x++){
        for (int z = -scene->vd; z <= (int)scene->vd; z++){
            scene->chunks[scene_v2s(scene->vd, x, z)] = world_getchunk(world, x + scene->cx, z + scene->cz);
        }
    }
}

scene_t* scene_make(void* world, unsigned int view_distance){
    scene_t* r = malloc(sizeof(scene_t));

    r->vd = view_distance;
    r->chunks = malloc(sizeof(char*) * (1 + 2 * view_distance) * (1 + 2 * view_distance));
    r->cx = 0; r->cz = 0;
    scene_updatechunks(r, world);

    return r;
}

void scene_update(scene_t* scene, void* world, void* player){
    int cp[2], bp[3];
    chunk_b2c(((player_t*)player)->pos[0], ((player_t*)player)->pos[1], ((player_t*)player)->pos[2], cp, bp);
    if (cp[0] != scene->cx || cp[1] != scene->cz){
        scene->cx = cp[0]; scene->cz = cp[1];
        scene_updatechunks(scene, world);
    }
}

char scene_getblock(scene_t* scene, int x, int y, int z){
    return scene->chunks[x / 16 + ((z / 16) * (1 + 2 * scene->vd))]
     [chunk_v2s(x % 16, y, z % 16)];
}

char scene_aircheck(scene_t* scene, int x, int y, int z){
    return (x < 0 || x > 16 * (scene->vd * 2 + 1) - 1 || y < 0 ||
     y > 255 || z < 0 || z > 16 * (scene->vd * 2 + 1) - 1 ||
     scene_getblock(scene, x, y, z) == 0);
}

void scene_setmodel(scene_t* scene, void* u_model){
    float diff = scene->vd * 16 ;
    glm_mat4_identity(u_model);
    glm_translate(u_model, (vec3){-diff, 0, -diff});
}