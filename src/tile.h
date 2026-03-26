#pragma once

#include "utils.h"

#define ATLAS_WIDTH 256.0f
#define ATLAS_HEIGHT 256.0f

#define TILE_SIZE (180)

#define TILE_DEFINE(name)\
    extern const f32 front_##name##_vertices[TILE_SIZE];\
    extern const f32 back_##name##_vertices[TILE_SIZE];\
    extern const f32 top_##name##_vertices[TILE_SIZE];\
    extern const f32 bottom_##name##_vertices[TILE_SIZE];\
    extern const f32 left_##name##_vertices[TILE_SIZE];\
    extern const f32 right_##name##_vertices[TILE_SIZE];
TILE_DEFINE(grass);
