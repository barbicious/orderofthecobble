#version 330 core

layout (location = 0) in vec3 i_pos;
layout (location = 1) in vec2 i_tex_coords;
layout (location = 2) in float i_light;

out vec2 v_tex_coords;
out float v_light;

uniform mat4 u_projection;
uniform mat4 u_view;
uniform mat4 u_model;

void main() {
    gl_Position = u_projection * u_view * u_model * vec4(i_pos, 1.0);
    v_tex_coords = i_tex_coords;
    v_light = i_light;
}