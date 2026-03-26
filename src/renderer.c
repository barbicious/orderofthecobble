#include "renderer.h"

#include "glad/gl.h"

renderer_s renderer_create() {
    glEnable(GL_DEPTH_TEST);

    shader_s primary_shader = shader_create(string_create("res/main.vert"), string_create("res/main.frag"));
    texture_s texture = texture_load(string_create("res/ofts_atlas.png"));

    return (renderer_s) {
        .primary_shader = primary_shader,
        .texture_atlas = texture,
    };
}

void renderer_blit(renderer_s* renderer) {

}
