#pragma once

#include "utils.h"
#include "cglm/cglm.h"
#include "cglm/struct/mat4.h"

typedef struct state state_s;

typedef struct camera {
    f32 speed;
    vec3 position;
    list_s directions;
    mat4 view;
} camera_s;

camera_s camera_create(vec3 position);

void camera_tick(camera_s* cam, f32 dt, state_s* state);

mat4s camera_get_view_matrix(const camera_s* cam);
