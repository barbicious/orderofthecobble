#pragma once
#include "utils.h"

typedef struct shader {
    u32 id;
} shader_s;

shader_s shader_create(string_s vpath, string_s fpath);
void shader_bind(const shader_s* shader);
void shader_unbind();
void shader_destroy(shader_s* shader);