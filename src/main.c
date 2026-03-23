#include <stdio.h>

#define UTILS_IMPL
#include "utils.h"

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>

#include "glad/gl.h"
#include "GL/glx.h"

i32 main(void) {
    printf("ORDER OF THE STONE!\n");

    Display* display = XOpenDisplay(NULL);
    if(display == NULL) {
        printf("Unable to connect to X server.\n");
        return 1;
    }

    XVisualInfo* visual_info = glXChooseVisual(display, DefaultScreen(display), (i32[]){
        GLX_RGBA,
    });

    GLXContext gl = glXCreateContext(display, visual_info, NULL, GL_TRUE);

    i32 screen = DefaultScreen(display);
    Window window = XCreateSimpleWindow(display, DefaultRootWindow(display), 0, 0, 1280, 720, 5, BlackPixel(display, screen), WhitePixel(display, screen));

    glXMakeCurrent(display, window, gl);

    if (!gladLoadGL((GLADloadfunc)glXGetProcAddress)) {
        printf("Unable to initialize OpenGL context.\n");
        return 1;
    }

    XSetStandardProperties(display, window, "Order of the Stone", "Hey.", None, NULL, 0, NULL);

    XClearWindow(display, window);
    XMapRaised(display, window);


    XSelectInput(display, window, StructureNotifyMask | KeyPressMask | KeyReleaseMask | ButtonPressMask | ButtonReleaseMask | ExposureMask);
    XEvent event;
    KeySym keysym;

    for (;;) {
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(0.3f, 0.4f, 0.5f, 1.0f);

        glXSwapBuffers(display, window);
    }

    return 0;
}