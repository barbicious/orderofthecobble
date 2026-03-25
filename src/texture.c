#include "texture.h"

#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "stb_image.h"
#include "glad/gl.h"

texture_s texture_load(string_s path) {
    stbi_set_flip_vertically_on_load(true);

    u32 texture = 0;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    i32 width, height, channels;

    char cwd[1024];
    getcwd(cwd, sizeof(cwd));
    cwd[strlen(cwd) - 17] = '\0';

    char abspath[1024];

    snprintf(abspath, sizeof(abspath), "%s%s", cwd, path.data);

    u8 *data = stbi_load(abspath, &width, &height, &channels, 0);

    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        perror("stbi_load");
    }

    return (texture_s){
        .id = texture,
        .width = width,
        .height = height,
        .data = data,
    };
}

void texture_bind(texture_s *texture) {
    glBindTexture(GL_TEXTURE_2D, texture->id);
}
