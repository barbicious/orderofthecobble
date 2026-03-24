#include <stdio.h>

#define UTILS_IMPL
#include "utils.h"

#include <X11/Xlib.h>

#include "shader.h"
#include "window.h"
#include "glad/gl.h"

i32 main(void) {
    window_s window = window_create(string_create("Order of the Stone"), 1280, 720);

    window_make_gl_context_current(&window);

    XEvent event;
    KeySym keysym;

    Atom delete_window = XInternAtom(window.display, "WM_DELETE_WINDOW", False);
    XSetWMProtocols(window.display, window.window, &delete_window, 1);

    shader_s shader = shader_create(string_create("src/main.vert"), string_create("src/main.frag"));

    shader_bind(&shader);


    f32 vertices[] = {
        -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
        0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f,
        0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f,
        -0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f,
        -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
    };

    u32 VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    u32 VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(f32), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(f32), (void*)(3 * sizeof(f32)));
    glEnableVertexAttribArray(1);


    while (!window.should_close) {
        XNextEvent(window.display, &event);
        switch (event.type) {
            case KeyPress: {
                printf("KeyPress\n");
            } break;
            case ClientMessage: {
                if (event.xclient.data.l[0] == delete_window) {
                    window.should_close = true;
                }
            } break;
        }

        glClearColor(0.3f, 0.4f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        shader_bind(&shader);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        window_display(&window);
    }

    shader_destroy(&shader);
    window_destroy(&window);

    return 0;
}