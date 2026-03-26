#pragma once

#include "utils.h"

typedef struct GLFWwindow GLFWwindow;

typedef struct window {
    bool should_close;
    i32 width;
    i32 height;
    GLFWwindow* window;
} window_s;

window_s window_create(string_s title, i32 width, i32 height);

void window_flush(const window_s* window);

bool window_should_close(const window_s* window);

void window_destroy(window_s* window);
