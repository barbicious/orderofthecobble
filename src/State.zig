const c = @import("c.zig").c;
const std = @import("std");
const Window = @import("Window.zig");
const Shader = @import("Shader.zig");
const zalgebra = @import("zalgebra");
const Texture = @import("Texture.zig");
const Camera = @import("Camera.zig");
const Clod = @import("Clod.zig");

const State = @This();

window: Window,
shader: Shader,
texture: Texture,
camera: Camera,
delta_time: f32,
clod: *Clod,

pub fn init(allocator: std.mem.Allocator, io: std.Io) !State {
    var window = try Window.init("Order of the Cobble", 1280, 720);

    c.glEnable(c.GL_DEPTH_TEST);

    const shader = try Shader.init(allocator, io, "res/world.vert", "res/world.frag");
    shader.bind();

    const fov: f32 = 45.0;

    const projection = zalgebra.perspective(zalgebra.toDegrees(fov), window.aspectRatio(), 0.1, 100.0);
    shader.setMat4(projection, "u_projection");

    const view = zalgebra.Mat4x4(f32).identity().translate(.fromSlice(&[_]f32{ 0.0, 0.0, -3.0 }));
    shader.setMat4(view, "u_view");

    const model = zalgebra.Mat4x4(f32).identity();
    shader.setMat4(model, "u_model");

    const texture = try Texture.init("res/oftc_atlas.png");
    texture.bind();

    const clod = try Clod.init(allocator, 0, 0, 0);

    return .{ .window = window, .shader = shader, .texture = texture, .camera = .init(0.0, 0.0, 3.0), .delta_time = 0.0, .clod = clod };
}

pub fn deinit(self: *State) void {
    self.window.deinit();
    self.shader.deinit();
}

pub fn run(self: *State) void {
    var last_time: f32 = 0.0;

    while (!self.window.shouldClose()) {
        self.delta_time = @as(f32, @floatCast(c.glfwGetTime())) - last_time;
        last_time = @as(f32, @floatCast(c.glfwGetTime()));

        if (self.window.isKeyDown(c.GLFW_KEY_ESCAPE)) {
            self.window.close();
        }

        self.camera.tick(self);

        c.glClearColor(0.1, 0.4, 0.4, 1.0);
        c.glClear(c.GL_COLOR_BUFFER_BIT | c.GL_DEPTH_BUFFER_BIT);

        c.glDrawArrays(c.GL_TRIANGLES, 0, @intCast(self.clod.vertices.items.len));

        self.shader.setMat4(self.camera.viewMatrix(), "u_view");

        self.window.flush();
    }
}
