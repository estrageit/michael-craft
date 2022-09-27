#include "render.h"

#include <glad/gl.h>
#include <cglm/cglm.h>

#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <pthread.h>
#include <string.h>

#include "shader.h"
#include "texture.h"
#include "scene.h"

render_t* render_make(){
    render_t* r = malloc(sizeof(render_t));

    glGenVertexArrays(1, &r->vao);

    r->sh = shader_make_from_file("res/shaders/chunk.glsl", NULL);
    r->tx = texture_load("res/textures/blocks.png");

    r->chunkc = 0;
    r->chunks = NULL;
    r->cbbuf = NULL;

    r->u_proj = malloc(sizeof(mat4));
    r->u_view = malloc(sizeof(mat4));

    return r;
}

rchunk_t* render_chunkmake(){
    rchunk_t* r = malloc(sizeof(rchunk_t));
    r->count = 0;
    glGenBuffers(1, &r->ibo);
    glGenBuffers(1, &r->vbo);
    return r;
}

void render_chunkdestroy(rchunk_t* c){
    glDeleteBuffers(1, &c->ibo);
    glDeleteBuffers(1, &c->vbo);
    free(c);
}

void vert_set(vertex_t* v, float px, float py, float pz, float nx, float ny, float nz, float tx, float ty){
    v->pos[0] = px;
    v->pos[1] = py;
    v->pos[2] = pz;

    v->normal[0] = nx;
    v->normal[1] = ny;
    v->normal[2] = nz;

    v->texcoord[0] = tx;
    v->texcoord[1] = ty;
}

void vert_print(vertex_t* v){
    printf("(%f, %f, %f), (%f, %f, %f), (%f, %f)\n",
    v->pos[0], v->pos[1], v->pos[2],
    v->normal[0], v->normal[1], v->normal[2],
    v->texcoord[0], v->texcoord[1]);
}

rcbbuf_t* render_chunkupdate(void* scene, unsigned int cx, unsigned int cz){
    rcbbuf_t* r = malloc(sizeof(rcbbuf_t));
    int sx = cx * 16;
    int sz = cz * 16;
    unsigned int facec = 0;
    for (int x = sx; x < (cx + 1) * 16; x++){
        for (int z = sz; z < (cz + 1) * 16; z++){
            for (int y = 0; y < 256; y++){
                if(scene_getblock(scene, x, y, z)){
                    facec+=scene_aircheck(scene, x+1, y, z);
                    facec+=scene_aircheck(scene, x-1, y, z);
                    facec+=scene_aircheck(scene, x, y+1, z);
                    facec+=scene_aircheck(scene, x, y-1, z);
                    facec+=scene_aircheck(scene, x, y, z+1);
                    facec+=scene_aircheck(scene, x, y, z-1);
                }}}}
    r->ibo_s = 6 * facec * sizeof(unsigned int);
    r->ver_s = sizeof(vertex_t) * facec * 4;
    r->count = 6 * facec;

    r->indices = malloc(r->ibo_s);
    r->vertices = malloc(r->ver_s);

    for (int i = 0; i < facec; i++){
        r->indices[0 + i * 6] = 0 + i * 4;
        r->indices[1 + i * 6] = 1 + i * 4;
        r->indices[2 + i * 6] = 2 + i * 4;
        r->indices[3 + i * 6] = 2 + i * 4;
        r->indices[4 + i * 6] = 3 + i * 4;
        r->indices[5 + i * 6] = 0 + i * 4;
    }

    printf("[INFO] Updated the scene with %d faces (%d, %d)\n", facec, cx, cz);

    unsigned int curf = 0;
    for (int x = sx; x < (cx + 1) * 16; x++){
        for (int z = sz; z < (cz + 1) * 16; z++){
            for (int y = 0; y < 256; y++){
                char b = scene_getblock(scene, x, y, z);
                if(b){
                    if(scene_aircheck(scene, x+1, y, z)){  
                        vert_set(  r->vertices+curf, x+1-sx,   y,   z-sz, 1, 0, 0, (b + 1) * (1/256.0f), 2/6.f);
                        vert_set(r->vertices+1+curf, x+1-sx, y+1,   z-sz, 1, 0, 0, (b + 1) * (1/256.0f), 1/6.f);
                        vert_set(r->vertices+2+curf, x+1-sx, y+1, z+1-sz, 1, 0, 0, (b + 0) * (1/256.0f), 1/6.f);
                        vert_set(r->vertices+3+curf, x+1-sx,   y, z+1-sz, 1, 0, 0, (b + 0) * (1/256.0f), 2/6.f);
                        curf += 4;
                    }
                    if(scene_aircheck(scene, x-1, y, z)){   
                        vert_set(  r->vertices+curf,   x-sx,   y,   z-sz,-1, 0, 0, (b + 1) * (1/256.0f), 3/6.f);
                        vert_set(r->vertices+1+curf,   x-sx, y+1,   z-sz,-1, 0, 0, (b + 1) * (1/256.0f), 2/6.f);
                        vert_set(r->vertices+2+curf,   x-sx, y+1, z+1-sz,-1, 0, 0, (b + 0) * (1/256.0f), 2/6.f);
                        vert_set(r->vertices+3+curf,   x-sx,   y, z+1-sz,-1, 0, 0, (b + 0) * (1/256.0f), 3/6.f);
                        curf += 4;
                    }
                    if(scene_aircheck(scene, x, y+1, z)){   
                        vert_set(  r->vertices+curf,   x-sx, y+1,   z-sz, 0, 1, 0, (b + 0) * (1/256.0f), 0.f);
                        vert_set(r->vertices+1+curf, x+1-sx, y+1,   z-sz, 0, 1, 0, (b + 1) * (1/256.0f), 0.f);
                        vert_set(r->vertices+2+curf, x+1-sx, y+1, z+1-sz, 0, 1, 0, (b + 1) * (1/256.0f), 1/6.f);
                        vert_set(r->vertices+3+curf,   x-sx, y+1, z+1-sz, 0, 1, 0, (b + 0) * (1/256.0f), 1/6.f);
                        curf += 4;
                    }
                    if(scene_aircheck(scene, x, y-1, z)){   
                        vert_set(  r->vertices+curf,   x-sx,   y,   z-sz, 0,-1, 0, (b + 0) * (1/256.0f), 5/6.f);
                        vert_set(r->vertices+1+curf, x+1-sx,   y,   z-sz, 0,-1, 0, (b + 1) * (1/256.0f), 5/6.f);
                        vert_set(r->vertices+2+curf, x+1-sx,   y, z+1-sz, 0,-1, 0, (b + 1) * (1/256.0f),   1.f);
                        vert_set(r->vertices+3+curf,   x-sx,   y, z+1-sz, 0,-1, 0, (b + 0) * (1/256.0f),   1.f);
                        curf += 4;
                    }
                    if(scene_aircheck(scene, x, y, z+1)){   
                        vert_set(  r->vertices+curf,   x-sx,   y, z+1-sz, 0, 0, 1, (b + 0) * (1/256.0f), 4/6.f);
                        vert_set(r->vertices+1+curf, x+1-sx,   y, z+1-sz, 0, 0, 1, (b + 1) * (1/256.0f), 4/6.f);
                        vert_set(r->vertices+2+curf, x+1-sx, y+1, z+1-sz, 0, 0, 1, (b + 1) * (1/256.0f), 3/6.f);
                        vert_set(r->vertices+3+curf,   x-sx, y+1, z+1-sz, 0, 0, 1, (b + 0) * (1/256.0f), 3/6.f);
                        curf += 4;
                    }
                    if(scene_aircheck(scene, x, y, z-1)){   
                        vert_set(  r->vertices+curf,   x-sx,   y,   z-sz, 0, 0,-1, (b + 0) * (1/256.0f), 5/6.f);
                        vert_set(r->vertices+1+curf, x+1-sx,   y,   z-sz, 0, 0,-1, (b + 1) * (1/256.0f), 5/6.f);
                        vert_set(r->vertices+2+curf, x+1-sx, y+1,   z-sz, 0, 0,-1, (b + 1) * (1/256.0f), 4/6.f);
                        vert_set(r->vertices+3+curf,   x-sx, y+1,   z-sz, 0, 0,-1, (b + 0) * (1/256.0f), 4/6.f);
                        curf += 4;
                    }
                }}}}
    return r;
}

void render_cbbfdestroy(rcbbuf_t* b){
    free(b->indices);
    free(b->vertices);
    free(b);
}

void render_b2f(rcbbuf_t* b, rchunk_t* f){
    glBindBuffer(GL_ARRAY_BUFFER, f->vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, f->ibo);

    glBufferData(GL_ARRAY_BUFFER, b->ver_s, b->vertices, GL_DYNAMIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, b->ibo_s, b->indices, GL_DYNAMIC_DRAW);

    f->count = b->count;
}

void render_flush(render_t* r){
    for (int i = 0; i < r->chunkc; i++){
        if (r->cbbuf[i] != NULL){
            render_b2f(r->cbbuf[i], r->chunks[i]);
            render_cbbfdestroy(r->cbbuf[i]);
            r->cbbuf[i] = NULL;
        }
    }
}

void render_refresh(render_t* r, void* scene){
    if (r->chunkc != (((scene_t*)scene)->vd * 2 + 1) * (((scene_t*)scene)->vd * 2 + 1)){
        if(r->chunks != NULL){
            render_flush(r);
            for (int i = 0; i < r->chunkc; i++){
                render_chunkdestroy(r->chunks[i]);
            }
            free(r->cbbuf);
            free(r->chunks);
        }

        r->chunkc = (((scene_t*)scene)->vd * 2 + 1) * (((scene_t*)scene)->vd * 2 + 1);
        r->cbbuf = malloc(r->chunkc * sizeof(rcbbuf_t*));
        r->chunks = malloc(r->chunkc * sizeof(rchunk_t*));
        for (int i = 0; i < r->chunkc; i++){
            r->chunks[i] = render_chunkmake();
        }
    }
    for (int i = 0; i < r->chunkc; i++){
        unsigned int cx = i / (((scene_t*)scene)->vd * 2 + 1);
        unsigned int cz = i % (((scene_t*)scene)->vd * 2 + 1);
        r->cbbuf[i] = render_chunkupdate(scene, cx, cz);
    }
}

void render_offset(render_t* r, void* scene, int x, int z){
    printf("[INFO] Offset the current chunk by (%d, %d)\n", x, z);
    rchunk_t** tchunks = malloc(r->chunkc * sizeof(rchunk_t*));
    rchunk_t** schunks = malloc(0);
    memset(tchunks, 0, r->chunkc * sizeof(rchunk_t*));
    int s = 0;
    for (int i = 0; i < r->chunkc; i++){
        int cx = i / (((scene_t*)scene)->vd * 2 + 1);
        int cz = i % (((scene_t*)scene)->vd * 2 + 1);
        if(cx + x < 0 || cx + x >= (((scene_t*)scene)->vd * 2 + 1)
        || cz + z < 0 || cz + z >= (((scene_t*)scene)->vd * 2 + 1)){
            schunks = realloc(schunks, (s + 1) * sizeof(rchunk_t*));
            schunks[s] = r->chunks[i];
            s++;
            continue;
        }
        int tpos = (cx + x) * (((scene_t*)scene)->vd * 2 + 1) + (cz + z);
        tchunks[tpos] = r->chunks[i];
    }
    s = 0;
    for (int i = 0; i < r->chunkc; i++){
        if(tchunks[i] == NULL){
            r->chunks[i] = schunks[s];
            s++;
            int cx = i / (((scene_t*)scene)->vd * 2 + 1);
            int cz = i % (((scene_t*)scene)->vd * 2 + 1);
            r->cbbuf[i] = render_chunkupdate(scene, cx, cz);
            continue;
        }
        r->chunks[i] = tchunks[i];
    }
    render_flush(r);
    free(tchunks);
    free(schunks);
}

void render_bind(render_t* r){
    glBindVertexArray(r->vao);
    glUseProgram(r->sh);
    glUniform3f(glGetUniformLocation(r->sh, "u_lightdir"), 1, 3, 2);
    glUniform1i(glGetUniformLocation(r->sh, "u_tex"), 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, r->tx);
}

void render_draw(render_t* r){
    glUniformMatrix4fv(glGetUniformLocation(r->sh, "u_proj"), 1, 0, r->u_proj);
    glUniformMatrix4fv(glGetUniformLocation(r->sh, "u_view"), 1, 0, r->u_view);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    int side = sqrtf(r->chunkc);
    for (int i = 0; i < r->chunkc; i++){
        glBindBuffer(GL_ARRAY_BUFFER, r->chunks[i]->vbo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, r->chunks[i]->ibo);

        mat4 u_model;
        int cx = i / side;
        int cz = i % side;
        glm_mat4_identity(u_model);
        glm_translate(u_model, (vec3){(cx - side/2) * 16, 0, (cz - side/2) * 16});
        glUniformMatrix4fv(glGetUniformLocation(r->sh, "u_model"), 1, 0, (float*)u_model);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex_t), (void*)offsetof(vertex_t, pos));
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vertex_t), (void*)offsetof(vertex_t, normal));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(vertex_t), (void*)offsetof(vertex_t, texcoord));
        glDrawElements(GL_TRIANGLES, r->chunks[i]->count, GL_UNSIGNED_INT, (void*)0);
    }
}