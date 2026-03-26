//
// Created by barbicious on 3/25/26.
//

#include "state.h"

#include "renderer.h"
#include "tile.h"
#include "glad/gl.h"
#include "GLFW/glfw3.h"

state_s state_create() {
    window_s window = window_create(string_create("Order of the Cobble"), 1280, 720);
    camera_s camera = camera_create((vec3){0.0f, 0.0f, 3.0f});
    keyboard_s keyboard = keyboard_create();
    renderer_s renderer = renderer_create();

    return (state_s){
        .camera = camera,
        .keyboard = keyboard,
        .window = window,
        .dt = 0.0f,
        .renderer = renderer
    };
}

void state_loop(state_s* state) {
    shader_bind(&state->renderer.primary_shader);

    mat4 projection;
    glm_perspective(glm_rad(45.0f), (f32) state->window.width / (f32) state->window.height, 0.1f, 100.0f, projection);

    shader_set_matrix(&state->renderer.primary_shader, string_create("view"), &state->camera.view);
    shader_set_matrix(&state->renderer.primary_shader, string_create("projection"), &projection);

    u32 VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    u32 VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(bottom_grass_vertices), bottom_grass_vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(f32), (void*) 0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(f32), (void*) (3 * sizeof(f32)));
    glEnableVertexAttribArray(1);

    f32 last_frame = 0.0f;

    while (!window_should_close(&state->window)) {
        f32 current_frame = glfwGetTime();
        state->dt = current_frame - last_frame;
        last_frame = current_frame;


        camera_tick(&state->camera, state->dt, state);
        shader_set_matrix(&state->renderer.primary_shader, string_create("view"), &state->camera.view);


        glClearColor(0.3f, 0.4f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        shader_bind(&state->renderer.primary_shader);
        texture_bind(&state->renderer.texture_atlas);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        window_flush(&state->window);
    }

    shader_destroy(&state->renderer.primary_shader);
    window_destroy(&state->window);

}
