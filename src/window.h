#pragma once
#include "glad/gl.h"
#include <X11/Xlib.h>

#include "utils.h"

typedef struct window {
    b8 should_close;
    i32 screen;
    Window window;
    Display* display;
} window_s;

window_s window_create(string_s title, i32 width, i32 height);
void window_make_gl_context_current(window_s* window);
void window_poll(window_s* window);
void window_display(const window_s* window);
void window_destroy(window_s* window);