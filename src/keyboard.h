#pragma once

#include "utils.h"

#define MAX_KEYS 512

typedef struct GLFWwindow GLFWwindow;

typedef struct keyboard {
    bool keys_down_now[MAX_KEYS];
    bool keys_down_last[MAX_KEYS];
} keyboard_s;

keyboard_s keyboard_create();
bool keyboard_key_down(const keyboard_s* keyboard, i32 key);
bool keyboard_key_pressed(const keyboard_s* keyboard, i32 key);