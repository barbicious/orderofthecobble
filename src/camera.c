#include "camera.h"

#include "window.h"
#include "GLFW/glfw3.h"
#include "state.h"

camera_s camera_create(vec3 position) {
    return (camera_s){
        .directions = list_create(),
        .speed = 2.5f,
        .position = {position[0], position[1], position[2]},
    };
}

void camera_tick(camera_s* cam, f32 dt, state_s* state) {
    if (glfwGetKey(state->window.window, GLFW_KEY_W) == GLFW_PRESS) {
        vec3 camera_dir = {0.0f, 0.0f, 0.0f};

        glm_vec3_muladds((vec3){0.0f, 0.0f, -1.0f}, cam->speed * dt, camera_dir);
        glm_vec3_add(cam->position, camera_dir, cam->position);
    }
    if (glfwGetKey(state->window.window, GLFW_KEY_S) == GLFW_PRESS) {
        vec3 camera_dir = {0.0f, 0.0f, 0.0f};

        glm_vec3_muladds((vec3){0.0f, 0.0f, -1.0f}, cam->speed * dt, camera_dir);
        glm_vec3_sub(cam->position, camera_dir, cam->position);
    }
    if (glfwGetKey(state->window.window, GLFW_KEY_A) == GLFW_PRESS) {
        vec3 res = {0.0f, 0.0f, 0.0f};
        vec3 cross_product = {0.0f, 0.0f, 0.0f};

        glm_cross((vec3){0.0f, 0.0f, -1.0f}, (vec3){0.0f, 1.0f, 0.0f}, cross_product);
        glm_normalize(cross_product);
        glm_vec3_muladds(cross_product, cam->speed * dt, res);
        glm_vec3_sub(cam->position, res, cam->position);
    }
    if (glfwGetKey(state->window.window, GLFW_KEY_D) == GLFW_PRESS) {
        vec3 res = {0.0f, 0.0f, 0.0f};
        vec3 cross_product = {0.0f, 0.0f, 0.0f};

        glm_cross((vec3){0.0f, 0.0f, -1.0f}, (vec3){0.0f, 1.0f, 0.0f}, cross_product);
        glm_normalize(cross_product);
        glm_vec3_muladds(cross_product, cam->speed * dt, res);
        glm_vec3_add(res, cam->position, cam->position);
    }

    vec3 camera_center = {0, 0, 0};

    glm_vec3_add(cam->position, (vec3){0.0f, 0.0f, -1.0f}, camera_center);
    glm_lookat_rh(cam->position, camera_center, (vec3){0.0f, 1.0f, 0.0f}, cam->view);
}
