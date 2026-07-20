const std = @import("std");
const c = @import("c.zig").c;
const zalgebra = @import("zalgebra");

const Error = error{ ShaderCompilationFailed, ShaderLinkingFailed };

const Shader = @This();

handle: u32,

pub fn init(allocator: std.mem.Allocator, io: std.Io, vertex_path: [:0]const u8, fragment_path: [:0]const u8) !Shader {
    const vertex_shader = try createShader(io, allocator, vertex_path, c.GL_VERTEX_SHADER);
    const fragment_shader = try createShader(io, allocator, fragment_path, c.GL_FRAGMENT_SHADER);

    const handle: u32 = c.glCreateProgram();
    c.glAttachShader(handle, vertex_shader);
    c.glAttachShader(handle, fragment_shader);
    c.glLinkProgram(handle);

    var success: i32 = 0;
    c.glGetProgramiv(handle, c.GL_LINK_STATUS, @ptrCast(&success));

    if (success != c.GL_TRUE) {
        var buffer: [1024]u8 = undefined;
        var info_len: u32 = undefined;

        c.glGetShaderiv(handle, c.GL_INFO_LOG_LENGTH, @ptrCast(&info_len));
        c.glGetShaderInfoLog(handle, buffer.len, @ptrCast(&info_len), &buffer);

        std.log.err("Shader linking failed! Error: {s}", .{buffer[0..info_len]});

        return Error.ShaderLinkingFailed;
    }

    return .{
        .handle = handle,
    };
}

pub fn deinit(self: *Shader) void {
    c.glDeleteProgram(self.handle);
}

pub fn bind(self: Shader) void {
    c.glUseProgram(self.handle);
}

pub fn setMat4(self: Shader, mat4: zalgebra.Mat4x4(f32), name: [:0]const u8) void {
    c.glUniformMatrix4fv(c.glGetUniformLocation(self.handle, name), 1, c.GL_FALSE, @ptrCast(&mat4.data[0][0]));
}

fn createShader(io: std.Io, allocator: std.mem.Allocator, path: []const u8, shader_type: u32) !u32 {
    const src = try allocator.dupeSentinel(u8, try std.Io.Dir.cwd().readFileAlloc(io, path, allocator, .limited(std.math.maxInt(usize))), 0);
    defer allocator.free(src);

    const shader = c.glCreateShader(shader_type);

    c.glShaderSource(shader, 1, @ptrCast(&src), null);
    c.glCompileShader(shader);

    var success: i32 = 0;
    c.glGetShaderiv(shader, c.GL_COMPILE_STATUS, @ptrCast(&success));

    if (success != c.GL_TRUE) {
        var buffer: [1024]u8 = undefined;
        var info_len: u32 = undefined;

        c.glGetShaderiv(shader, c.GL_INFO_LOG_LENGTH, @ptrCast(&info_len));
        c.glGetShaderInfoLog(shader, buffer.len, @ptrCast(&info_len), &buffer);

        std.log.err("Shader compilation failed! Error: {s}", .{buffer[0..info_len]});

        return Error.ShaderCompilationFailed;
    }

    return shader;
}
