#version 460 core

out vec4 o_frag_color;

in vec2 uv;

uniform sampler2D texture_sampler;

void main() {
    o_frag_color = texture(texture_sampler, uv);
}