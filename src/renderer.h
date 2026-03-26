#pragma once
#include "shader.h"
#include "texture.h"


typedef struct renderer {
    shader_s primary_shader;
    texture_s texture_atlas;
} renderer_s;

renderer_s renderer_create();
void renderer_blit(renderer_s* renderer);