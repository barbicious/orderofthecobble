#include "tile.h"

#define TILE_CREATE(name, texture_x, texture_y)\
    const f32 name##_vertices[] = {\
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,\
         0.5f, -0.5f, -0.5f,  texture_x / ATLAS_WIDTH, 0.0f,\
         0.5f,  0.5f, -0.5f,  texture_x / ATLAS_WIDTH, texture_y / ATLAS_HEIGHT,\
         0.5f,  0.5f, -0.5f,  texture_x / ATLAS_WIDTH, texture_y / ATLAS_HEIGHT,\
        -0.5f,  0.5f, -0.5f,  0.0f, texture_y / ATLAS_HEIGHT,\
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,\
    \
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,\
         0.5f, -0.5f,  0.5f,  texture_x / ATLAS_WIDTH, 0.0f,\
         0.5f,  0.5f,  0.5f,  texture_x / ATLAS_WIDTH, texture_y / ATLAS_HEIGHT,\
         0.5f,  0.5f,  0.5f,  texture_x / ATLAS_WIDTH, texture_y / ATLAS_HEIGHT,\
        -0.5f,  0.5f,  0.5f,  0.0f, texture_y / ATLAS_HEIGHT,\
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,\
    \
        -0.5f,  0.5f,  0.5f,  texture_x / ATLAS_WIDTH, 0.0f,\
        -0.5f,  0.5f, -0.5f,  texture_x / ATLAS_WIDTH, texture_y / ATLAS_HEIGHT,\
        -0.5f, -0.5f, -0.5f,  0.0f, texture_y / ATLAS_HEIGHT,\
        -0.5f, -0.5f, -0.5f,  0.0f, texture_y / ATLAS_HEIGHT,\
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,\
        -0.5f,  0.5f,  0.5f,  texture_x / ATLAS_WIDTH, 0.0f,\
    \
         0.5f,  0.5f,  0.5f,  texture_x / ATLAS_WIDTH, 0.0f,\
         0.5f,  0.5f, -0.5f,  texture_x / ATLAS_WIDTH, texture_y / ATLAS_HEIGHT,\
         0.5f, -0.5f, -0.5f,  0.0f, texture_y / ATLAS_HEIGHT,\
         0.5f, -0.5f, -0.5f,  0.0f, texture_y / ATLAS_HEIGHT,\
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,\
         0.5f,  0.5f,  0.5f,  texture_x / ATLAS_WIDTH, 0.0f,\
    \
        -0.5f, -0.5f, -0.5f,  0.0f, texture_y / ATLAS_HEIGHT,\
         0.5f, -0.5f, -0.5f,  texture_x / ATLAS_WIDTH, texture_y / ATLAS_HEIGHT,\
         0.5f, -0.5f,  0.5f,  texture_x / ATLAS_WIDTH, 0.0f,\
         0.5f, -0.5f,  0.5f,  texture_x / ATLAS_WIDTH, 0.0f,\
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,\
        -0.5f, -0.5f, -0.5f,  0.0f, texture_y / ATLAS_HEIGHT,\
    \
        -0.5f,  0.5f, -0.5f,  0.0f, texture_y / ATLAS_HEIGHT,\
         0.5f,  0.5f, -0.5f,  texture_x / ATLAS_WIDTH, texture_y / ATLAS_HEIGHT,\
         0.5f,  0.5f,  0.5f,  texture_x / ATLAS_WIDTH, 0.0f,\
         0.5f,  0.5f,  0.5f,  texture_x / ATLAS_WIDTH, 0.0f,\
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,\
        -0.5f,  0.5f, -0.5f,  0.0f, texture_y / ATLAS_HEIGHT\
    };


TILE_CREATE(grass, 8.0f, 8.0f);
