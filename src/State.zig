const c = @import("c.zig").c;
const std = @import("std");
const Window = @import("Window.zig");
const Shader = @import("Shader.zig");
const zalgebra = @import("zalgebra");
const Texture = @import("Texture.zig");
const Camera = @import("Camera.zig");

const State = @This();

window: Window,
shader: Shader,
texture: Texture,
camera: Camera,

pub fn init(allocator: std.mem.Allocator, io: std.Io) !State {
    var window = try Window.init("Order of the Cobble", 1280, 720);

    c.glEnable(c.GL_DEPTH_TEST);

    const size: f32 = 8.0 / 256.0;

    const vertices = [_]f32{
        0.0, 0.0, 0.0, 0.0,  0.0,
        1.0, 0.0, 0.0, size, 0.0,
        1.0, 1.0, 0.0, size, size,
        1.0, 1.0, 0.0, size, size,
        0.0, 1.0, 0.0, 0.0,  size,
        0.0, 0.0, 0.0, 0.0,  0.0,

        0.0, 0.0, 1.0, 0.0,  0.0,
        1.0, 0.0, 1.0, size, 0.0,
        1.0, 1.0, 1.0, size, size,
        1.0, 1.0, 1.0, size, size,
        0.0, 1.0, 1.0, 0.0,  size,
        0.0, 0.0, 1.0, 0.0,  0.0,

        0.0, 1.0, 1.0, size, 0.0,
        0.0, 1.0, 0.0, size, size,
        0.0, 0.0, 0.0, 0.0,  size,
        0.0, 0.0, 0.0, 0.0,  size,
        0.0, 0.0, 1.0, 0.0,  0.0,
        0.0, 1.0, 1.0, size, 0.0,

        1.0, 1.0, 1.0, size, 0.0,
        1.0, 1.0, 0.0, size, size,
        1.0, 0.0, 0.0, 0.0,  size,
        1.0, 0.0, 0.0, 0.0,  size,
        1.0, 0.0, 1.0, 0.0,  0.0,
        1.0, 1.0, 1.0, size, 0.0,

        0.0, 0.0, 0.0, 0.0,  size,
        1.0, 0.0, 0.0, size, size,
        1.0, 0.0, 1.0, size, 0.0,
        1.0, 0.0, 1.0, size, 0.0,
        0.0, 0.0, 1.0, 0.0,  0.0,
        0.0, 0.0, 0.0, 0.0,  size,

        0.0, 1.0, 0.0, 0.0,  size,
        1.0, 1.0, 0.0, size, size,
        1.0, 1.0, 1.0, size, 0.0,
        1.0, 1.0, 1.0, size, 0.0,
        0.0, 1.0, 1.0, 0.0,  0.0,
        0.0, 1.0, 0.0, 0.0,  size,
    };

    const shader = try Shader.init(allocator, io, "res/world.vert", "res/world.frag");
    shader.bind();

    const fov: f32 = 45.0;

    const projection = zalgebra.perspective(zalgebra.toDegrees(fov), window.aspectRatio(), 0.1, 100.0);
    shader.setMat4(projection, "u_projection");

    const view = zalgebra.Mat4x4(f32).identity().translate(.fromSlice(&[_]f32{ 0.0, 0.0, -3.0 }));
    shader.setMat4(view, "u_view");

    const model = zalgebra.Mat4x4(f32).identity();
    shader.setMat4(model, "u_model");

    var vao: u32 = 0;
    c.glGenVertexArrays(1, &vao);
    c.glBindVertexArray(vao);

    var vbo: u32 = 0;
    c.glGenBuffers(1, &vbo);
    c.glBindBuffer(c.GL_ARRAY_BUFFER, vbo);
    c.glBufferData(c.GL_ARRAY_BUFFER, @as(c_long, @intCast(vertices.len)) * @sizeOf(f32), @ptrCast(&vertices), c.GL_STATIC_DRAW);
    c.glVertexAttribPointer(0, 3, c.GL_FLOAT, c.GL_FALSE, 5 * @sizeOf(f32), @ptrFromInt(0));
    c.glEnableVertexAttribArray(0);
    c.glVertexAttribPointer(1, 2, c.GL_FLOAT, c.GL_FALSE, 5 * @sizeOf(f32), @ptrFromInt(3 * @sizeOf(f32)));
    c.glEnableVertexAttribArray(1);

    const texture = try Texture.init("res/oftc_atlas.png");
    texture.bind();

    return .{ .window = window, .shader = shader, .texture = texture, .camera = .init(0.0, 0.0, 3.0) };
}

pub fn deinit(self: *State) void {
    self.window.deinit();
    self.shader.deinit();
}

pub fn run(self: *State) void {
    while (!self.window.shouldClose()) {
        if (self.window.isKeyDown(c.GLFW_KEY_ESCAPE)) {
            self.window.close();
        }

        self.camera.tick(self);

        c.glClearColor(0.1, 0.4, 0.4, 1.0);
        c.glClear(c.GL_COLOR_BUFFER_BIT | c.GL_DEPTH_BUFFER_BIT);

        c.glDrawArrays(c.GL_TRIANGLES, 0, 36);

        self.shader.setMat4(self.camera.viewMatrix(), "u_view");

        self.window.flush();
    }
}
