#include "player.h"

#include <cglm/cglm.h>

#include <string.h>
#include <math.h>

#include "input.h"

player_t* player_make(){
    player_t* r = malloc(sizeof(player_t));
    memset(r, 0, sizeof(player_t));
    r->rot[2] = 1;
    return r;
}

void player_update(player_t* player, void* input, float delta){
    vec3 mov = {0, 0, 0};
    vec3 rot = {((input_t*)input)->mx, ((input_t*)input)->my, 0};
    mat4 trans;

    glm_look((vec3){0, 0, 0}, player->rot, (vec3){0, 1, 0}, trans);
    glm_mat4_inv(trans, trans);

    if (input_getkey(input, I_K_MOV_FOWARD))
        glm_vec3_add((vec3){ 0, 0, 1}, mov, mov);
    if (input_getkey(input, I_K_MOV_BACK))
        glm_vec3_add((vec3){ 0, 0,-1}, mov, mov);
    if (input_getkey(input, I_K_MOV_RIGHT))
        glm_vec3_add((vec3){-1, 0, 0}, mov, mov);
    if (input_getkey(input, I_K_MOV_LEFT))
        glm_vec3_add((vec3){ 1, 0, 0}, mov, mov);

    glm_vec3_rotate_m4(trans, rot, rot);
    glm_vec3_add(player->rot, rot, rot);
    glm_vec3_normalize(rot);
    if (glm_vec3_angle((vec3){0, 1, 0}, rot) > 0.15f &&
        glm_vec3_angle((vec3){0, -1, 0}, rot) > 0.15f &&
        ((input_t*)input)->ml){
        glm_vec3_copy(rot, player->rot);
    }

    float speed = 4;
    if (input_getkey(input, I_K_MOV_SPRINT))
        speed = 16;

    glm_vec3_normalize(mov);
    glm_vec3_scale(mov, -speed * delta, mov);
    glm_vec3_rotate_m4(trans, mov, mov);
    glm_vec3_add(player->pos, mov, player->pos);
}

void player_setview(player_t* player, void* u_view){
    glm_look((vec3)
        {player->pos[0] - (floorf(player->pos[0] / 16.0f) * 16),
         player->pos[1],
         player->pos[2] - (floorf(player->pos[2] / 16.0f) * 16)},
     player->rot, (vec3){0, 1, 0}, u_view);
}