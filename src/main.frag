#version 460 core

out vec4 o_frag_color;

in vec3 color;

void main() {
    o_frag_color = vec4(color, 1.0f);
}