#include <stdio.h>

#define UTILS_IMPL
#include "utils.h"

#include <X11/Xlib.h>

#include "shader.h"
#include "window.h"
#include "glad/gl.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "texture.h"
#include "tile.h"
#include "cglm/cglm.h"
#include "GLFW/glfw3.h"

i32 main(void) {
    window_s window = window_create(string_create("Order of the Stone"), 1280, 720);

    shader_s shader = shader_create(string_create("res/main.vert"), string_create("res/main.frag"));

    shader_bind(&shader);

    texture_s texture = texture_load(string_create("res/ofts_atlas.png"));

    mat4 view;
    glm_mat4_identity(view);
    glm_translate(view, (vec3){0, 0, -5});
    glm_rotate(view, glm_rad(60.0f), (vec3){1, 1, 1});

    mat4 projection;
    glm_perspective(glm_rad(45.0f), (f32) window.width / (f32) window.height, 0.1f, 100.0f, projection);

    shader_set_matrix(&shader, string_create("view"), &view);
    shader_set_matrix(&shader, string_create("projection"), &projection);

    u32 VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    u32 VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(grass_vertices), grass_vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(f32), (void *) 0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(f32), (void *) (3 * sizeof(f32)));
    glEnableVertexAttribArray(1);

    glEnable(GL_DEPTH_TEST);

    while (!window_should_close(&window)) {

        glClearColor(0.3f, 0.4f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader_bind(&shader);
        texture_bind(&texture);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        window_flush(&window);
    }

    shader_destroy(&shader);
    window_destroy(&window);

    return 0;
}
