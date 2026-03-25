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
#include <stdbool.h>

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
    char *data;
    size_t length;
} string_s;

string_s string_create(char *str);

#ifdef UTILS_IMPL

#include <stdlib.h>
#include <string.h>

string_s string_create(char *str) {
    string_s ret = {0};
    ret.length = strlen(str);
    ret.data = str;
    return ret;
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
    void **items;
    size_t length;
    size_t capacity;
} list_s;

list_s list_create();

void list_append(list_s *list, void *item);

void *list_get(const list_s *list, size_t index);

void list_remove(list_s *list, size_t index);

void list_destroy(list_s *list);

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

void list_append(list_s *list, void *item) {
    if (list->length >= list->capacity) {
        list->capacity *= 2;
        list->items = realloc(list->items, list->capacity * sizeof(*list->items));
    }

    list->items[list->length++] = item;
}

void *list_get(const list_s *list, size_t index) {
    if (index >= list->length) {
        printf("Attempted out of bounds access on list.");
        exit(0);
    }

    return list->items[index];
}

void list_remove(list_s *list, size_t index) {
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

void list_destroy(list_s *list) {
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
    f32 dx = fx - (f32) x;
    f32 dy = fy - (f32) y;

    // Dot product
    return (dx * gradient.x + dy * gradient.y);
}

f32 perlin(f32 x, f32 y) {
    // Get cell coordinate for each corner
    i32 x0 = (i32) x;
    i32 y0 = (i32) y;
    i32 x1 = x0 + 1;
    i32 y1 = y0 + 1;

    // Lerp weights
    f32 lx = x - (f32) x0;
    f32 ly = y - (f32) y0;

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


#pragma region math

i32 is_prime(i32 n);
i32 next_prime(i32 n);

#ifdef UTILS_IMPL

#include <math.h>

/**
 * Is it a prime number.
 * @param n Number to be checked.
 * @return Whether it is a prime number or not. 1 for prime, 0 for not prime, -1 for undefined. (n < 2)
 */
i32 is_prime(const i32 n) {
    if (n < 2) {
        return -1;
    }

    if (n < 4) {
        return 1;
    }

    if ((n & (n - 1)) == 0) {
        return 0;
    }

    for (i32 i = 3; i <= floor(sqrt((f64) n)); i+= 2) {
        if ((n & (i - 1)) == 0) {
            return 0;
        }
    }

    return 1;
}

/**
 * Calculates the next prime number after n.
 * @param n Number to start with.
 * @return Next prime number.
 */
i32 next_prime(i32 n) {
    while (is_prime(n) == 1) {
        n++;
    }

    return n;
}

#endif


#pragma endregion math


#pragma region hashtable

typedef struct hashtable_item {
    char* key;
    void* value;
} hashtable_item_t;

typedef struct hashtable {
    hashtable_item_t** items;
    i32 base_size;
    i32 size;
    i32 count;
} hashtable_t;

/**
 * Creates a new instance of a hashtable.
 * @return Pointer to the hashtable.
 */
hashtable_t *hashtable_init();
void hashtable_free(hashtable_t* table);

void hashtable_insert(hashtable_t* table, char* key, void* value);
void *hashtable_get(hashtable_t* table, const char* key);
void hashtable_remove(hashtable_t* table, const char* key);


#ifdef UTILS_IMPL

#include <math.h>
#include <stdio.h>

#include <stdlib.h>
#include <string.h>

#define HASHTABLE_BASE_SIZE 53

#define HASHTABLE_PRIME_ONE 13
#define HASHTABLE_PRIME_TWO 5

static hashtable_item_t hashtable_null_item = {NULL, NULL};

/**
 * Creates a pointer to a new hashtable item type, and assigns the
 * key and value to it.
 * @param key Hashtable key for the value.
 * @param value Hashtable value that is held
 * @return Pointer to hashtable item
 */
static hashtable_item_t *hashtable_item_new(char *key, void *value) {
    hashtable_item_t *item = malloc(sizeof(hashtable_item_t));
    item->key = key;
    item->value = value;
    return item;
}

static void hashtable_item_free(hashtable_item_t *item) {
    free(item->key);
    free(item->value);
    free(item);
}

/**
 * Returns a usable index for the hashtable, should be unique.
 * @param str String for base of hashing.
 * @param seed Hashing seed, must be prime.
 * @param count Number of elements in hashtable.
 * @return Index.
 */
static i32 hashtable_hash(const char *str, const i32 seed, const i32 size) {
    i64 hash = 0;
    const i32 str_length = strlen(str);
    for (int i = 0; i < str_length; ++i) {
        hash += (i64)pow(seed, str_length - (i + 1) * str[i]);
        hash = hash & (size - 1);
    }

    return (i32)hash;
}

/**
 * Hash functions will produce collisions with input keys, so
 * they must be handled somehow. Here we use double hashing to calculate
 * a unique hash.
 * @return Unique hash.
 */
static i32 hashtable_get_hash(const char *str, const i32 size, const i32 attempt) {
    const i32 hash_a = hashtable_hash(str, HASHTABLE_PRIME_ONE, size);
    const i32 hash_b = hashtable_hash(str, HASHTABLE_PRIME_TWO, size);

    i32 hash_result = (hash_a + (attempt * (hash_b + 1)));

    return hash_result & (size - 1);
}

static hashtable_t *hashtable_realloc(const i32 base_size) {
    hashtable_t *hashtable = malloc(sizeof(hashtable_t));
    hashtable->base_size = base_size;

    hashtable->size = next_prime(hashtable->base_size);

    hashtable->count = 0;
    hashtable->items = calloc((size_t)hashtable->size, sizeof(hashtable_item_t *));
    return hashtable;
}

static void hashtable_resize(hashtable_t *hashtable, const i32 base_size) {
    if (base_size < HASHTABLE_BASE_SIZE) {
        return;
    }

    hashtable_t *new_hashtable = hashtable_realloc(base_size);
    // Copy over items
    for (i32 i = 0; i < hashtable->size; ++i) {
        hashtable_item_t *item = hashtable->items[i];
        if (item != NULL && item != &hashtable_null_item) {
            hashtable_insert(new_hashtable, item->key, item->value);
        }
    }

    hashtable->base_size = new_hashtable->base_size;
    hashtable->count = new_hashtable->count;

    hashtable->size = hashtable->size ^= new_hashtable->size;
    new_hashtable->size = new_hashtable->size ^= hashtable->size;
    hashtable->size = hashtable->size ^= new_hashtable->size;

    hashtable_item_t **temp_items = new_hashtable->items;
    hashtable->items = new_hashtable->items;
    new_hashtable->items = temp_items;

    hashtable_free(new_hashtable);
}

static void hashtable_resize_up(hashtable_t *hashtable) {
    const i32 new_size = hashtable->base_size * 2;
    hashtable_resize(hashtable, new_size);
}

static void hashtable_resize_down(hashtable_t *hashtable) {
    const i32 new_size = hashtable->base_size / 2;
    hashtable_resize(hashtable, new_size);
}

hashtable_t *hashtable_init() {
    return hashtable_realloc(HASHTABLE_BASE_SIZE);
}

void hashtable_free(hashtable_t *table) {
    for (int item_index = 0; item_index < table->size; ++item_index) {
        hashtable_item_t *item = table->items[item_index];
        if (item == NULL) {
            continue;
        }

        hashtable_item_free(item);
    }

    free(table->items);
    free(table);
}

void hashtable_insert(hashtable_t *hashtable, char *key, void *value) {
    const i32 load = hashtable->count * 100 / hashtable->size;

    if (load > 70) {
        hashtable_resize_up(hashtable);
    }

    const hashtable_item_t *item = hashtable_item_new(key, value);
    i32 index = hashtable_get_hash(item->key, hashtable->size, 0);
    hashtable_item_t *current_item = hashtable->items[index];

    // Loop over elements until we find open space
    i32 i = 1;
    while (current_item != NULL) {
        if (current_item != &hashtable_null_item) {
            if (strcmp(current_item->key, key) == 0) {
                hashtable_item_free(current_item);
                hashtable->items[index] = item;
                return;
            }
        }
        index = hashtable_get_hash(current_item->key, hashtable->size, i);
        current_item = hashtable->items[index];
        ++i;
    }

    hashtable->items[index] = item;
    hashtable->count++;
}

void *hashtable_get(hashtable_t *hashtable, const char *key) {
    // TODO: Make this O(1), or at least O(log*n)

    i32 index = hashtable_get_hash(key, hashtable->size, 0);
    hashtable_item_t *item = hashtable->items[index];

    // Loop over items until we find one that matches the desired key, O(n) time-complexity

    i32 i = 1;
    while (item != NULL) {
        if (item != &hashtable_null_item) {
            if (strcmp(item->key, key) == 0) {
                return item->value;
            }
        }

        index = hashtable_get_hash(item->key, hashtable->size, i);
        item = hashtable->items[index];
        ++i;
    }

    return NULL;
}

void hashtable_delete(hashtable_t *hashtable, const char *key) {
    const i32 load = hashtable->count * 100 / hashtable->size;
    if (load < 10) {
        hashtable_resize_down(hashtable);
    }

    i32 index = hashtable_get_hash(key, hashtable->size, 0);
    hashtable_item_t *item = hashtable->items[index];

    i32 i = 1;
    while (item != NULL) {
        if (item != &hashtable_null_item) {
            if (strcmp(item->key, key) == 0) {
                hashtable_item_free(item);
                return;
            }
        }

        index = hashtable_get_hash(item->key, hashtable->size, i);
        item = hashtable->items[index];
        ++i;
    }

    --hashtable->count;
}

#endif

#pragma endregion hashtable