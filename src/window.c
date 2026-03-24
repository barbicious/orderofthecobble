#include "window.h"
#include <GL/glx.h>
#include <X11/Xutil.h>

#include <stdio.h>

window_s window_create(string_s title, const i32 width, const i32 height) {
    Display* display = XOpenDisplay(NULL);
    if(display == NULL) {
        printf("Unable to connect to X server.\n");
        exit(-1);
    }

    i32 screen = DefaultScreen(display);
    Window window = XCreateSimpleWindow(display, DefaultRootWindow(display), 0, 0, width, height, 5, BlackPixel(display, screen), WhitePixel(display, screen));

    XSetStandardProperties(display, window, title.data, NULL, None, NULL, 0, NULL);

    string_free(&title);

    XClearWindow(display, window);
    XMapRaised(display, window);


    XSelectInput(display, window, StructureNotifyMask | KeyPressMask | KeyReleaseMask | ButtonPressMask | ButtonReleaseMask | ExposureMask);

    window_s ret = {
        .screen = screen,
        .window = window,
        .display = display,
        .should_close = false,
    };

    return ret;
}

void window_make_gl_context_current(window_s *window) {
    XVisualInfo* visual_info = glXChooseVisual(window->display, DefaultScreen(window->display), (i32[]){
        GLX_RGBA,
    });

    GLXContext gl = glXCreateContext(window->display, visual_info, NULL, GL_TRUE);

    glXMakeCurrent(window->display, window->window, gl);

    if (!gladLoadGL((GLADloadfunc)glXGetProcAddress)) {
        printf("Unable to initialize OpenGL context.\n");
        exit(-1);
    }
}

void window_display(const window_s *window) {
    glXSwapBuffers(window->display, window->window);
}

void window_destroy(window_s *window) {
    glXDestroyWindow(window->display, window->window);
}
