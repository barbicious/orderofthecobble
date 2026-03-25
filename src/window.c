#include "window.h"

#include <stdio.h>

#include "glad/gl.h"
#include "GLFW/glfw3.h"

static void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
}

window_s window_create(string_s title, const i32 width, const i32 height) {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // Initialize GLFW and OpenGL
    GLFWwindow *window = glfwCreateWindow(width, height, title.data, NULL, NULL);
    if (!window) {
        perror("glfwCreateWindow");
        exit(-1);
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGL((GLADloadfunc)glfwGetProcAddress)) {
        perror("gladLoadGL");
        exit(-1);
    }

    glViewport(0, 0, width, height);

    // Setup callbacks
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    return (window_s){
        .width = width,
        .height = height,
        .window = window,
    };
}

void window_flush(const window_s *window) {
    glfwSwapBuffers(window->window);
    glfwPollEvents();
}

bool window_should_close(const window_s *window) {
    return glfwWindowShouldClose(window->window) || window->should_close;
}

void window_destroy(window_s *window) {
    glfwDestroyWindow(window->window);
    glfwTerminate();
}
