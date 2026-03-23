cmake_minimum_required(VERSION 4.1)
project(orderofthestone C)

set(CMAKE_C_STANDARD 99)

add_executable(orderofthestone src/main.c
        vendor/glad/src/gl.c)

target_include_directories(orderofthestone PRIVATE vendor/glad/include)

target_link_libraries(orderofthestone PRIVATE X11 GL m)

