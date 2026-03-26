cmake_minimum_required(VERSION 4.1)
project(orderofthestone C)

set(CMAKE_C_STANDARD 99)

add_subdirectory(vendor/glfw)

add_executable(orderofthestone src/main.c
        vendor/glad/src/gl.c
        src/window.c
        src/window.h
        src/shader.c
        src/shader.h
        src/texture.c
        src/texture.h
        src/renderer.c
        src/renderer.h
        src/tile.c
        src/tile.h
        src/camera.c
        src/camera.h
        src/keyboard.c
        src/keyboard.h
        src/state.c
        src/state.h
        src/clod.c
        src/clod.h)

target_include_directories(orderofthestone PRIVATE vendor/glad/include vendor/stbi vendor/cglm/include vendor/glfw)

target_link_libraries(orderofthestone PRIVATE glfw)

