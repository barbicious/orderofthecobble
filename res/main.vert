#version 460 core

layout(location=0) in vec3 i_pos;
layout(location=1) in vec2 i_uv;

out vec2 uv;

uniform mat4 view;
uniform mat4 projection;

void main() {
    gl_Position = projection * view * vec4(i_pos, 1.0);
    uv = i_uv;
}