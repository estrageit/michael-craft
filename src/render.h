#pragma once

typedef struct render_t{
    unsigned int sh, vao, ibo, vbo, tx, count;
    float *u_proj, *u_view, *u_model;
} render_t;

render_t* render_make();
void render_update(render_t* r, void* scene);

void render_bind(render_t* r);
void render_draw(render_t* r);