#include "shader.h"
#include <unistd.h>

#include <stdio.h>
#include <string.h>

#include "glad/gl.h"
#include "cglm/cglm.h"

u32 shader_load(u32 type, string_s path) {
    char cwd[1024];
    getcwd(cwd, sizeof(cwd));
    cwd[strlen(cwd) - 17] = '\0';

    char abspath[1024];

    snprintf(abspath, sizeof(abspath), "%s%s", cwd, path.data);

    FILE* file = fopen(abspath, "rb");
    if (file == NULL) {
        fprintf(stderr, "Could not open %s\n", abspath);
        exit(-1);
    }

    fseek(file, 0, SEEK_END);
    usize file_size = ftell(file);
    rewind(file);

    char* buffer = malloc(file_size + 1);
    fread(buffer, sizeof(char), file_size, file);
    buffer[file_size] = '\0';
    fclose(file);


    u32 shader = glCreateShader(type);
    glShaderSource(shader, 1, (const char**) &buffer, NULL);
    glCompileShader(shader);

    b32 success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char info_log[1024];
        glGetShaderInfoLog(shader, 1024, NULL, info_log);
        fprintf(stderr, "Could not compile %s\nError: %s", abspath, info_log);
    }

    free(buffer);

    return shader;
}

shader_s shader_create(string_s vpath, string_s fpath) {
    shader_s shader = {0};

    u32 vshader = shader_load(GL_VERTEX_SHADER, vpath);
    u32 fshader = shader_load(GL_FRAGMENT_SHADER, fpath);

    shader.id = glCreateProgram();
    glAttachShader(shader.id, vshader);
    glAttachShader(shader.id, fshader);
    glLinkProgram(shader.id);


    b32 success;
    glGetProgramiv(shader.id, GL_LINK_STATUS, &success);
    if (!success) {
        char info_log[1024];
        glGetProgramInfoLog(shader.id, 1024, NULL, info_log);
        fprintf(stderr, "Could not link %d\nError: %s", shader.id, info_log);
    }

    glDeleteShader(vshader);
    glDeleteShader(fshader);

    return shader;
}

void shader_bind(const shader_s* shader) {
    glUseProgram(shader->id);
}

void shader_unbind() {
    glUseProgram(0);
}

void shader_set_matrix(const shader_s* shader, string_s name, const mat4* matrix) {
    glUniformMatrix4fv(glGetUniformLocation(shader->id, name.data), 1, false, (f32*) matrix);
}

void shader_destroy(shader_s* shader) {
    shader_unbind();
    glDeleteProgram(shader->id);
}
