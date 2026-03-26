#include "tile.h"

#define TILE_IMPL(name, texture_x, texture_y)\
    const f32 front_##name##_vertices[] = {\
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,\
         0.5f, -0.5f, -0.5f,  texture_x / ATLAS_WIDTH, 0.0f,\
         0.5f,  0.5f, -0.5f,  texture_x / ATLAS_WIDTH, texture_y / ATLAS_HEIGHT,\
         0.5f,  0.5f, -0.5f,  texture_x / ATLAS_WIDTH, texture_y / ATLAS_HEIGHT,\
        -0.5f,  0.5f, -0.5f,  0.0f, texture_y / ATLAS_HEIGHT,\
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,\
    };\
    const f32 back_##name##_vertices[] = {\
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,\
         0.5f, -0.5f,  0.5f,  texture_x / ATLAS_WIDTH, 0.0f,\
         0.5f,  0.5f,  0.5f,  texture_x / ATLAS_WIDTH, texture_y / ATLAS_HEIGHT,\
         0.5f,  0.5f,  0.5f,  texture_x / ATLAS_WIDTH, texture_y / ATLAS_HEIGHT,\
        -0.5f,  0.5f,  0.5f,  0.0f, texture_y / ATLAS_HEIGHT,\
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,\
    };\
    const f32 left_##name##_vertices[] = {\
        -0.5f,  0.5f,  0.5f,  texture_x / ATLAS_WIDTH, 0.0f,\
        -0.5f,  0.5f, -0.5f,  texture_x / ATLAS_WIDTH, texture_y / ATLAS_HEIGHT,\
        -0.5f, -0.5f, -0.5f,  0.0f, texture_y / ATLAS_HEIGHT,\
        -0.5f, -0.5f, -0.5f,  0.0f, texture_y / ATLAS_HEIGHT,\
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,\
        -0.5f,  0.5f,  0.5f,  texture_x / ATLAS_WIDTH, 0.0f,\
    };\
    const f32 right_##name##_vertices[] = {\
         0.5f,  0.5f,  0.5f,  texture_x / ATLAS_WIDTH, 0.0f,\
         0.5f,  0.5f, -0.5f,  texture_x / ATLAS_WIDTH, texture_y / ATLAS_HEIGHT,\
         0.5f, -0.5f, -0.5f,  0.0f, texture_y / ATLAS_HEIGHT,\
         0.5f, -0.5f, -0.5f,  0.0f, texture_y / ATLAS_HEIGHT,\
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,\
         0.5f,  0.5f,  0.5f,  texture_x / ATLAS_WIDTH, 0.0f,\
    };\
    const f32 bottom_##name##_vertices[] = {\
        -0.5f, -0.5f, -0.5f,  0.0f, texture_y / ATLAS_HEIGHT,\
         0.5f, -0.5f, -0.5f,  texture_x / ATLAS_WIDTH, texture_y / ATLAS_HEIGHT,\
         0.5f, -0.5f,  0.5f,  texture_x / ATLAS_WIDTH, 0.0f,\
         0.5f, -0.5f,  0.5f,  texture_x / ATLAS_WIDTH, 0.0f,\
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,\
        -0.5f, -0.5f, -0.5f,  0.0f, texture_y / ATLAS_HEIGHT,\
    };\
    const f32 top_##name##_vertices[] = {\
        -0.5f,  0.5f, -0.5f,  0.0f, texture_y / ATLAS_HEIGHT,\
         0.5f,  0.5f, -0.5f,  texture_x / ATLAS_WIDTH, texture_y / ATLAS_HEIGHT,\
         0.5f,  0.5f,  0.5f,  texture_x / ATLAS_WIDTH, 0.0f,\
         0.5f,  0.5f,  0.5f,  texture_x / ATLAS_WIDTH, 0.0f,\
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,\
        -0.5f,  0.5f, -0.5f,  0.0f, texture_y / ATLAS_HEIGHT\
    };


TILE_IMPL(grass, 8.0f, 8.0f);
