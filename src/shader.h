#pragma once
#include "utils.h"
#include "cglm/mat4.h"

typedef struct shader {
    i32 id;
} shader_s;

shader_s shader_create(string_s vpath, string_s fpath);

void shader_bind(const shader_s *shader);

void shader_unbind();

void shader_set_matrix(const shader_s *shader, string_s name, const mat4 *matrix);

void shader_destroy(shader_s *shader);
