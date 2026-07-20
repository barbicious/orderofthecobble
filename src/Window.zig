const c = @import("c.zig").c;
const std = @import("std");
const builtin = @import("builtin");

const Error = error{ GlfwInitFailed, GlfwFailedToCreateWindow, GladFnLoadFailed };

const Version = struct {
    major: i32,
    minor: i32,
    revision: i32,
};

const Window = @This();

handle: *c.GLFWwindow,
width: u32,
height: u32,
mouse: struct {
    x: f64,
    y: f64,
    delta_x: f64,
    delta_y: f64,
},

pub fn init(title: [:0]const u8, width: u32, height: u32) Error!Window {
    if (c.glfwInit() != c.GLFW_TRUE) {
        std.log.err("Failed to init glfw!", .{});
        return Error.GlfwInitFailed;
    }

    errdefer c.glfwTerminate();

    c.glfwWindowHint(c.GLFW_CONTEXT_VERSION_MAJOR, 3);
    c.glfwWindowHint(c.GLFW_CONTEXT_VERSION_MINOR, 3);
    c.glfwWindowHint(c.GLFW_OPENGL_PROFILE, c.GLFW_OPENGL_CORE_PROFILE);

    var version: Version = undefined;

    c.glfwGetVersion(&version.major, &version.minor, &version.revision);

    std.log.info("GLFW Version {d}.{d}.{d}", .{ version.major, version.minor, version.revision });

    if (builtin.os.tag == .macos) {
        c.glfwWindowHint(c.GLFW_OPENGL_FORWARD_COMPAT, c.GL_TRUE);

        std.log.info("Running GLFW Forward Compatibility mode for MacOS.", .{});
    }

    const handle = c.glfwCreateWindow(@as(i32, @intCast(width)), @as(i32, @intCast(height)), title, null, null) orelse {
        std.log.err("Failed to create window!", .{});
        return Error.GlfwFailedToCreateWindow;
    };

    errdefer c.glfwDestroyWindow(handle);

    c.glfwMakeContextCurrent(handle);

    const glad_load_proc: c.GLADloadproc = @ptrCast(&c.glfwGetProcAddress);
    if (c.gladLoadGLLoader(glad_load_proc) == c.GL_FALSE) {
        std.log.err("Failed to load GL functions", .{});
        return Error.GladFnLoadFailed;
    }

    c.glViewport(0, 0, @as(i32, @intCast(width)), @as(i32, @intCast(height)));

    _ = c.glfwSetFramebufferSizeCallback(handle, framebufferSizeCallback);
    _ = c.glfwSetCursorPosCallback(handle, cursorPosCallback);

    c.glfwSetInputMode(handle, c.GLFW_CURSOR, c.GLFW_CURSOR_DISABLED);

    return .{ .handle = handle, .width = width, .height = height, .mouse = .{
        .x = 0.0,
        .y = 0.0,
        .delta_x = 0.0,
        .delta_y = 0.0,
    } };
}

pub fn isKeyDown(self: Window, key: i32) bool {
    return c.glfwGetKey(self.handle, key) == c.GLFW_PRESS;
}

pub fn close(self: Window) void {
    c.glfwSetWindowShouldClose(self.handle, @intFromBool(true));
}

pub fn deinit(self: Window) void {
    c.glfwDestroyWindow(self.handle);
    c.glfwTerminate();
}

pub fn aspectRatio(self: Window) f32 {
    return @as(f32, @floatFromInt(self.width)) / @as(f32, @floatFromInt(self.height));
}

pub fn flush(self: Window) void {
    c.glfwSwapBuffers(self.handle);
    c.glfwPollEvents();
}

pub fn shouldClose(self: Window) bool {
    return c.glfwWindowShouldClose(self.handle) == c.GLFW_TRUE;
}

pub fn installUserPointer(window_ptr: *Window) void {
    c.glfwSetWindowUserPointer(window_ptr.handle, @ptrCast(window_ptr));
}

fn framebufferSizeCallback(handle: ?*c.GLFWwindow, width: c_int, height: c_int) callconv(.c) void {
    var window: *Window = @ptrCast(@alignCast(c.glfwGetWindowUserPointer(handle) orelse @panic("framebufferSizeCallback glfw ptr is null!")));

    window.width = @intCast(width);
    window.height = @intCast(height);

    c.glViewport(0, 0, width, height);
}

fn cursorPosCallback(handle: ?*c.GLFWwindow, cursor_x: f64, cursor_y: f64) callconv(.c) void {
    var window: *Window = @ptrCast(@alignCast(c.glfwGetWindowUserPointer(handle) orelse @panic("cursorPosCallback glfw ptr is null!")));

    window.mouse.delta_x = cursor_x - window.mouse.x;
    window.mouse.delta_y = window.mouse.y - cursor_y;

    window.mouse.x = cursor_x;
    window.mouse.y = cursor_y;
}
