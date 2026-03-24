#pragma once

/* TODO:
 * Memory Arena
 * Stack Allocator
 * JSON Serializer
 * Binary Serializer
 * Image loading
 * Window lib
 */

#pragma region types

#include <math.h>
#include <stdint.h>
#include <stdlib.h>

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef float f32;
typedef double f64;

typedef u8 b8;
typedef u32 b32;

typedef size_t usize;
typedef ssize_t isize;

#define true 1
#define false 0

// No _t suffix cuz it is basically an integral type any ways
typedef struct vector2 {
    f32 x, y;
} v2;

#pragma endregion types


#pragma region usefulmacros

/* compilers, debuggers, past me, and C++ purists beware */

#define RAND_RANG(min, max) (min + (rand() % (max - min + 1)))

#define RECT_COLLIDES(x1, y1, w1, h1, x2, y2, w2, h2)\
    (!!(\
    ((x1)  < ((x2) + (w2))) && \
    (((x1) + (w1)) > (x2))  && \
    ((y1)  < ((y2) + (h2))) && \
    (((y1) + (h1)) > (y2))     \
    ))

#pragma endregion usefulmacros


#pragma region usefulfunctions

// Interpolates between two numbers based on the weight
f32 lerp(f32 num_one, f32 num_two, f32 weight);

#ifdef UTILS_IMPL

f32 lerp(f32 num_one, f32 num_two, f32 weight) {
    return (num_two - num_one) * (3.0f - weight * 2.0f) * weight * weight + num_one;
}

#endif

#pragma endregion usefulfunctions


#pragma region array

#define DEFINE_ARRAY(type)\
typedef struct array_##type {\
    type* data;\
    size_t length;\
} array_##type##_s;\
\
type array_##type##_get(array_##type##_s* arr, usize idx);

DEFINE_ARRAY(i32);

#ifdef UTILS_IMPL

#define IMPL_ARRAY(type)\
    type array_##type##_get(array_##type##_s* arr, usize idx) {\
        if (idx < 0 || idx >= arr->length) {\
            printf("Array access out of bounds.\n");\
            exit(0);\
        }\
        return arr->data[idx];\
    }

IMPL_ARRAY(i32);

#endif

#pragma endregion array


#pragma region string

#include <stddef.h>

typedef struct string {
    char* data;
    size_t length;
} string_s;

string_s string_create(const char* str);
void string_free(string_s* string);

#ifdef UTILS_IMPL

#include <stdlib.h>
#include <string.h>

string_s string_create(const char* str) {
    string_s ret;
    ret.data = strdup(str);
    ret.length = strlen(str);
    return ret;
}

void string_free(string_s* string) {
    if (string != NULL) {
        free(string->data);
        string->data = NULL;
        string->length = 0;
    }
}

#endif

#pragma endregion string


#pragma region list

/*
 * Probably should do this in a non
 * "cast a billion things from void* way" but idc
 * It works for now and performance will probably be fine
 */

typedef struct list {
    void** items;
    size_t length;
    size_t capacity;
} list_s;

list_s list_create();
void list_append(list_s* list, void* item);
void* list_get(const list_s* list, size_t index);
void list_remove(list_s* list, size_t index);
void list_destroy(list_s* list);

#ifdef UTILS_IMPL

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

list_s list_create() {
    list_s ret;
    ret.length = 0;
    ret.capacity = 2;
    ret.items = malloc(sizeof(*ret.items) * ret.capacity);
    return ret;
}

void list_append(list_s* list, void* item) {
    if (list->length >= list->capacity) {
        list->capacity *= 2;
        list->items = realloc(list->items, list->capacity * sizeof(*list->items));
    }

    list->items[list->length++] = item;
}

void* list_get(const list_s* list, size_t index) {
    if (index >= list->length) {
        printf("Attempted out of bounds access on list.");
        exit(0);
    }

    return list->items[index];
}

void list_remove(list_s* list, size_t index) {
    if (index >= list->length) {
        printf("Attempted out of bounds access on list.");
        exit(0);
    }

    memmove(
        list->items + index,
        list->items + index + 1,
        (list->length - index - 1) * sizeof(*list->items)
    );
    list->length--;
}

void list_destroy(list_s* list) {
    // TODO: Log warning that i tried to free null list
    if (list != NULL && list->items != NULL) {
        free(list->items);
        list->items = NULL;
        list->length = 0;
        list->capacity = 0;
    }
}

#endif

#pragma endregion list

#pragma region noise

// Computes a psuedorandom gradient value for a given point
v2 random_gradient(i32 x, i32 y);
// Determines the distance/dot product between the corner normal, and the provided coordinate
f32 dot_grid_gradient(i32 x, i32 y, f32 fx, f32 fy);
// 2D Perlin noise function, returns a value 0-1
f32 perlin(f32 x, f32 y);

#ifdef UTILS_IMPL

#include <math.h>

v2 random_gradient(i32 x, i32 y) {
    unsigned w = 8 * sizeof(unsigned);
    unsigned s = w / 2;
    unsigned a = x, b = y;
    a *= 3284157443;

    b ^= a << s | a >> w - s;
    b *= 1911520717;

    a ^= b << s | b >> w - s;
    a *= 2048419325;

    f32 random = a * (3.14159265 / ~(~0u >> 1));

    // Create vector from the angle
    v2 vector2 = {
        .x = sin(random),
        .y = cos(random),
    };

    return vector2;
}

f32 dot_grid_gradient(i32 x, i32 y, f32 fx, f32 fy) {
    v2 gradient = random_gradient(x, y);

    // Get the distance vector
    f32 dx = fx - (f32)x;
    f32 dy = fy - (f32)y;

    // Dot product
    return (dx * gradient.x + dy * gradient.y);
}

f32 perlin(f32 x, f32 y) {
    // Get cell coordinate for each corner
    i32 x0 = (i32)x;
    i32 y0 = (i32)y;
    i32 x1 = x0 + 1;
    i32 y1 = y0 + 1;

    // Lerp weights
    f32 lx = x - (f32)x0;
    f32 ly = y - (f32)y0;

    // Compute and lerp top two corners
    f32 n0 = dot_grid_gradient(x0, y0, x, y);
    f32 n1 = dot_grid_gradient(x1, y0, x, y);
    f32 ix0 = lerp(n0, n1, lx);

    // Compute and lerp bottom two corners
    n0 = dot_grid_gradient(x0, y1, x, y);
    n1 = dot_grid_gradient(x1, y1, x, y);
    f32 ix1 = lerp(n0, n1, lx);

    // Lerp previous two values with y weight
    return lerp(ix0, ix1, ly);
}

#endif

#pragma endregion noise