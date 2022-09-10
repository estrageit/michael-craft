#include "render.h"

#include <glad/gl.h>

#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>

#include "shader.h"
#include "texture.h"
#include "scene.h"
#include "vertex.h"

render_t* render_make(){
    render_t* r = malloc(sizeof(render_t));

    glGenBuffers(1, &r->vbo);
    glGenBuffers(1, &r->ibo);

    glGenVertexArrays(1, &r->vao);

    r->sh = shader_make_from_file("res/shaders/chunk.glsl", NULL);
    r->tx = texture_load("res/textures/blocks.png");
    r->count = 0;

    r->u_model = malloc(sizeof(float) * 16);
    r->u_proj = malloc(sizeof(float) * 16);
    r->u_view = malloc(sizeof(float) * 16);

    return r;
}

void render_update(render_t* r, void* scene){
    unsigned int facec = 0;
    for (int x = 0; x < (((scene_t*)scene)->vd * 2 + 1) * 16; x++){
        for (int z = 0; z < (((scene_t*)scene)->vd * 2 + 1) * 16; z++){
            for (int y = 0; y < 256; y++){
                if(scene_getblock(scene, x, y, z)){
                    facec+=scene_aircheck(scene, x+1, y, z);
                    facec+=scene_aircheck(scene, x-1, y, z);
                    facec+=scene_aircheck(scene, x, y+1, z);
                    facec+=scene_aircheck(scene, x, y-1, z);
                    facec+=scene_aircheck(scene, x, y, z+1);
                    facec+=scene_aircheck(scene, x, y, z-1);
                }}}}
    unsigned int ibo_s = 6 * facec * sizeof(unsigned int);
    unsigned int ver_s = sizeof(vertex_t) * facec * 4;

    r->count = 6 * facec;

    unsigned int* indices = malloc(ibo_s);
    vertex_t* v = malloc(ver_s);

    for (int i = 0; i < facec; i++){
        indices[0 + i * 6] = 0 + i * 4;
        indices[1 + i * 6] = 1 + i * 4;
        indices[2 + i * 6] = 2 + i * 4;
        indices[3 + i * 6] = 2 + i * 4;
        indices[4 + i * 6] = 3 + i * 4;
        indices[5 + i * 6] = 0 + i * 4;
    }

    printf("[INFO] Updated the scene with %d faces (%d, %d)\n", facec, ver_s, ibo_s);

    unsigned int curf = 0;
    for (int x = 0; x < (((scene_t*)scene)->vd * 2 + 1) * 16; x++){
        for (int z = 0; z < (((scene_t*)scene)->vd * 2 + 1) * 16; z++){
            for (int y = 0; y < 256; y++){
                char b = scene_getblock(scene, x, y, z);
                if(b){
                    if(scene_aircheck(scene, x+1, y, z)){  
                        vert_set(  v+curf, x+1,   y,   z, 1, 0, 0, (b + 1) * (1/256.0f), 2/6.f);
                        vert_set(v+1+curf, x+1, y+1,   z, 1, 0, 0, (b + 1) * (1/256.0f), 1/6.f);
                        vert_set(v+2+curf, x+1, y+1, z+1, 1, 0, 0, (b + 0) * (1/256.0f), 1/6.f);
                        vert_set(v+3+curf, x+1,   y, z+1, 1, 0, 0, (b + 0) * (1/256.0f), 2/6.f);
                        curf += 4;
                    }
                    if(scene_aircheck(scene, x-1, y, z)){   
                        vert_set(  v+curf,   x,   y,   z,-1, 0, 0, (b + 1) * (1/256.0f), 3/6.f);
                        vert_set(v+1+curf,   x, y+1,   z,-1, 0, 0, (b + 1) * (1/256.0f), 2/6.f);
                        vert_set(v+2+curf,   x, y+1, z+1,-1, 0, 0, (b + 0) * (1/256.0f), 2/6.f);
                        vert_set(v+3+curf,   x,   y, z+1,-1, 0, 0, (b + 0) * (1/256.0f), 3/6.f);
                        curf += 4;
                    }
                    if(scene_aircheck(scene, x, y+1, z)){   
                        vert_set(  v+curf,   x, y+1,   z, 0, 1, 0, (b + 0) * (1/256.0f), 0.f);
                        vert_set(v+1+curf, x+1, y+1,   z, 0, 1, 0, (b + 1) * (1/256.0f), 0.f);
                        vert_set(v+2+curf, x+1, y+1, z+1, 0, 1, 0, (b + 1) * (1/256.0f), 1/6.f);
                        vert_set(v+3+curf,   x, y+1, z+1, 0, 1, 0, (b + 0) * (1/256.0f), 1/6.f);
                        curf += 4;
                    }
                    if(scene_aircheck(scene, x, y-1, z)){   
                        vert_set(  v+curf,   x,   y,   z, 0,-1, 0, (b + 0) * (1/256.0f), 5/6.f);
                        vert_set(v+1+curf, x+1,   y,   z, 0,-1, 0, (b + 1) * (1/256.0f), 5/6.f);
                        vert_set(v+2+curf, x+1,   y, z+1, 0,-1, 0, (b + 1) * (1/256.0f),   1.f);
                        vert_set(v+3+curf,   x,   y, z+1, 0,-1, 0, (b + 0) * (1/256.0f),   1.f);
                        curf += 4;
                    }
                    if(scene_aircheck(scene, x, y, z+1)){   
                        vert_set(  v+curf,   x,   y, z+1, 0, 0, 1, (b + 0) * (1/256.0f), 4/6.f);
                        vert_set(v+1+curf, x+1,   y, z+1, 0, 0, 1, (b + 1) * (1/256.0f), 4/6.f);
                        vert_set(v+2+curf, x+1, y+1, z+1, 0, 0, 1, (b + 1) * (1/256.0f), 3/6.f);
                        vert_set(v+3+curf,   x, y+1, z+1, 0, 0, 1, (b + 0) * (1/256.0f), 3/6.f);
                        curf += 4;
                    }
                    if(scene_aircheck(scene, x, y, z-1)){   
                        vert_set(  v+curf,   x,   y,   z, 0, 0,-1, (b + 0) * (1/256.0f), 5/6.f);
                        vert_set(v+1+curf, x+1,   y,   z, 0, 0,-1, (b + 1) * (1/256.0f), 5/6.f);
                        vert_set(v+2+curf, x+1, y+1,   z, 0, 0,-1, (b + 1) * (1/256.0f), 4/6.f);
                        vert_set(v+3+curf,   x, y+1,   z, 0, 0,-1, (b + 0) * (1/256.0f), 4/6.f);
                        curf += 4;
                    }
                }}}}

    glBindBuffer(GL_ARRAY_BUFFER, r->vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, r->ibo);

    glBufferData(GL_ARRAY_BUFFER, ver_s, v, GL_DYNAMIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, ibo_s, indices, GL_DYNAMIC_DRAW);

    glBindVertexArray(r->vao);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex_t), (void*)offsetof(vertex_t, pos));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vertex_t), (void*)offsetof(vertex_t, normal));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(vertex_t), (void*)offsetof(vertex_t, texcoord));

    free(v);
    free(indices);
}

void render_bind(render_t* r){
    glBindVertexArray(r->vao);
    glBindBuffer(GL_ARRAY_BUFFER, r->vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, r->ibo);
    glUseProgram(r->sh);
    glUniform3f(glGetUniformLocation(r->sh, "u_lightdir"), 1, 3, 2);
    glUniform1i(glGetUniformLocation(r->sh, "u_tex"), 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, r->tx);
}

void render_draw(render_t* r){
    glUniformMatrix4fv(glGetUniformLocation(r->sh, "u_proj"), 1, 0, r->u_proj);
    glUniformMatrix4fv(glGetUniformLocation(r->sh, "u_view"), 1, 0, r->u_view);
    glUniformMatrix4fv(glGetUniformLocation(r->sh, "u_model"), 1, 0, r->u_model);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //printf("c%d\n", glGetError());
    glDrawElements(GL_TRIANGLES, r->count, GL_UNSIGNED_INT, (void*)0);
}