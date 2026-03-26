#pragma once
#include "utils.h"

typedef struct texture {
    u32 id;
    i32 width;
    i32 height;
    u8* data;
} texture_s;

texture_s texture_load(string_s path);

void texture_bind(texture_s* texture);
