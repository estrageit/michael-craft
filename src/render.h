#pragma once

typedef struct vertex_t{
    float pos[3];
    float normal[3];
    float texcoord[2];
} vertex_t;

typedef struct rcbbuf_t{
    unsigned int count;
    unsigned int *indices;
    vertex_t *vertices;
    unsigned int ver_s, ibo_s;
} rcbbuf_t;

typedef struct rchunk_t{
    unsigned int count, ibo, vbo;
} rchunk_t;

typedef struct render_t{
    unsigned int sh, vao, tx;
    void *u_proj, *u_view;
    unsigned int chunkc;
    rchunk_t** chunks;
    rcbbuf_t** cbbuf;
} render_t;

render_t* render_make();
void render_refresh(render_t* r, void* scene);
void render_offset(render_t* r, void* scene, int x, int z);

void render_bind(render_t* r);
void render_draw(render_t* r);
void render_flush(render_t* r);