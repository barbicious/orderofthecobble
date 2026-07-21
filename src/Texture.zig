const c = @import("c.zig").c;
const std = @import("std");

const Error = error{StbiFailed};

const Texture = @This();

width: i32,
height: i32,
handle: u32,

pub fn init(texture_path: [:0]const u8) Error!Texture {
    var width: i32 = 0;
    var height: i32 = 0;
    var channels: i32 = 0;
    c.stbi_set_flip_vertically_on_load(@intFromBool(true));
    const data = c.stbi_load(texture_path, &width, &height, &channels, 4);
    if (data == null) {
        std.log.err("Failed to load file! stbi says... {s}", .{c.stbi_failure_reason()});
        return Error.StbiFailed;
    }

    var handle: u32 = 0;
    c.glGenTextures(1, &handle);
    c.glBindTexture(c.GL_TEXTURE_2D, handle);
    c.glTexParameteri(c.GL_TEXTURE_2D, c.GL_TEXTURE_WRAP_S, c.GL_REPEAT);
    c.glTexParameteri(c.GL_TEXTURE_2D, c.GL_TEXTURE_WRAP_T, c.GL_REPEAT);
    c.glTexParameteri(c.GL_TEXTURE_2D, c.GL_TEXTURE_MIN_FILTER, c.GL_NEAREST);
    c.glTexParameteri(c.GL_TEXTURE_2D, c.GL_TEXTURE_MAG_FILTER, c.GL_NEAREST);
    c.glTexImage2D(c.GL_TEXTURE_2D, 0, c.GL_RGBA, width, height, 0, c.GL_RGBA, c.GL_UNSIGNED_BYTE, data);
    c.glGenerateMipmap(c.GL_TEXTURE_2D);
    c.stbi_image_free(data);

    return .{
        .handle = handle,
        .width = width,
        .height = height,
    };
}

pub fn bind(self: Texture) void {
    c.glBindTexture(c.GL_TEXTURE_2D, self.handle);
}
