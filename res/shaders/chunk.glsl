$SHADER_TYPE VERT

#version 330 core
layout (location = 0) in vec3 a_pos;
layout (location = 1) in vec3 a_normal;
layout (location = 2) in vec2 a_texcoord;
  
uniform mat4 u_proj;
uniform mat4 u_view;
uniform mat4 u_model;

out vec3 v_normal;
out vec3 v_fpos;
out vec2 v_texcoord;

void main(){
    gl_Position = u_proj * u_view * u_model * vec4(a_pos, 1.0);
    v_normal = mat3(transpose(inverse(u_model))) * a_normal;
    v_fpos = vec3(u_model * vec4(a_pos, 1.0));
    v_texcoord = a_texcoord;
}

$SHADER_TYPE FRAG

#version 330 core
out vec4 FragColor;

uniform sampler2D u_tex;
uniform vec3 u_lightdir;

in vec3 v_normal;
in vec3 v_fpos;
in vec2 v_texcoord;

void main(){
    vec3 n = normalize(v_normal);
    vec3 ld = normalize(u_lightdir);
    float l = max(dot(n, ld), 0.0);
    FragColor = (0.5 + l * 0.5) * vec4(texture2D(u_tex, v_texcoord));
}