#include "input.h"
#include "window.h"
#include "player.h"
#include "scene.h"
#include "render.h"
#include "world.h"

#include <stdio.h>

int main(int argc, char** argv){
    void* window = window_make();
    input_t* input = input_make(1, window);
    player_t* player = player_make();
    player->pos[0] = 8.5;
    player->pos[1] = 62.5;
    player->pos[2] = 8.5;
    world_t* world = world_make();
    scene_t* scene = scene_make(world, 3);
    render_t* render = render_make();

    float time = window_gettime();

    render_refresh(render, scene);
    printf("[INFO] Render update took %fs to complete\n", window_gettime() - time);

    render_bind(render);    

    while (!window_run(window)){
        float delta = window_gettime() - time;
        time = window_gettime();

        input_update(input, window);
        if (input_getkey(input, I_K_QUIT))
            break;

        player_update(player, input, delta);
        scene_update(scene, world, player, render);
        if (input_getkeydown(input, I_K_RELOAD)){
            float t1 = window_gettime();
            render_refresh(render, scene);
            printf("[INFO] Render update took %fs to complete\n", window_gettime() - t1);
            printf("Position and rotation:\n");
            printf("\t%f, %f, %f\n", player->pos[0], player->pos[1], player->pos[2]);
            printf("\t%f, %f, %f\n", player->rot[0], player->rot[1], player->rot[2]);
        }

        player_setview(player, render->u_view);
        window_setproj(window, render->u_proj);

        //draw
        render_flush(render);
        render_bind(render);
        render_draw(render);

        window_update(window);
    }
    input_destroy(input);
    world_destroy(world);

    window_terminate();

    return 0;
}