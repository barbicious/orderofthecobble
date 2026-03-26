#pragma once

#include "window.h"
#include "camera.h"
#include "keyboard.h"
#include "renderer.h"

typedef struct state {
    f32 dt;
    camera_s camera;
    keyboard_s keyboard;
    window_s window;
    renderer_s renderer;
} state_s;

state_s state_create();
void state_loop(state_s* state);
