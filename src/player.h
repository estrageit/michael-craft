#pragma once

typedef struct player_t{
    float pos[3];
    float rot[3];
    float vel[3];
} player_t;

player_t* player_make();
void player_update(player_t* player, void* input, float delta);
void player_setview(player_t* player, void* u_view);