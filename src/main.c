#include <stdio.h>

#define UTILS_IMPL
#include "utils.h"

#include <X11/Xlib.h>

#include "shader.h"
#include "window.h"
#include "glad/gl.h"

#define STB_IMAGE_IMPLEMENTATION
#include "camera.h"
#include "state.h"
#include "stb_image.h"
#include "texture.h"
#include "tile.h"
#include "cglm/cglm.h"
#include "GLFW/glfw3.h"

static camera_s cam;
f32 dt = 0.0f;

void input(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        vec3 camera_dir = {0.0f, 0.0f, 0.0f};

        glm_vec3_muladds((vec3){0.0f, 0.0f, -1.0f}, cam.speed * dt, camera_dir);
        glm_vec3_add(cam.position, camera_dir, cam.position);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        vec3 camera_dir = {0.0f, 0.0f, 0.0f};

        glm_vec3_muladds((vec3){0.0f, 0.0f, -1.0f}, cam.speed * dt, camera_dir);
        glm_vec3_sub(cam.position, camera_dir, cam.position);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        vec3 res = {0.0f, 0.0f, 0.0f};
        vec3 cross_product = {0.0f, 0.0f, 0.0f};

        glm_cross((vec3){0.0f, 0.0f, -1.0f}, (vec3){0.0f, 1.0f, 0.0f}, cross_product);
        glm_normalize(cross_product);
        glm_vec3_muladds(cross_product, cam.speed * dt, res);
        glm_vec3_sub(cam.position, res, cam.position);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        vec3 res = {0.0f, 0.0f, 0.0f};
        vec3 cross_product = {0.0f, 0.0f, 0.0f};

        glm_cross((vec3){0.0f, 0.0f, -1.0f}, (vec3){0.0f, 1.0f, 0.0f}, cross_product);
        glm_normalize(cross_product);
        glm_vec3_muladds(cross_product, cam.speed * dt, res);
        glm_vec3_add(res, cam.position, cam.position);
    }

    vec3 camera_center = {0, 0, 0};

    glm_vec3_add(cam.position, (vec3){0.0f, 0.0f, -1.0f}, camera_center);

    /*if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        list_append(&cam.directions, (void*)NORTH);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        list_append(&cam.directions, (void*)SOUTH);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        list_append(&cam.directions, (void*)EAST);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        list_append(&cam.directions, (void*)WEST);
    }*/
}

i32 main(void) {
    state_s state = state_create();
    state_loop(&state);

    return 0;
}
