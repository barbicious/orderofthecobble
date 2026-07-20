#version 330 core

out vec4 o_color;

in vec2 v_tex_coords;

uniform sampler2D texture_sampler;

void main() {
    o_color = texture(texture_sampler, v_tex_coords);
}