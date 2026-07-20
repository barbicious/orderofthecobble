const zalgebra = @import("zalgebra");
const c = @import("c.zig").c;
const State = @import("State.zig");
const std = @import("std");

const Camera = @This();

const up = zalgebra.Vec3.fromSlice(&[_]f32{ 0.0, 1.0, 0.0 });
const speed: f32 = 2.5;
const sensitivity: f32 = 0.1;

position: zalgebra.Vec3,
front: zalgebra.Vec3,
yaw: f32,
pitch: f32,

pub fn init(x: f32, y: f32, z: f32) Camera {
    const position = zalgebra.Vec3.fromSlice(&[_]f32{ x, y, z });
    const front = zalgebra.Vec3.fromSlice(&[_]f32{ 0.0, 0.0, -1.0 });

    return .{
        .position = position,
        .front = front,
        .yaw = -90.0,
        .pitch = 0.0,
    };
}

pub fn viewMatrix(self: Camera) zalgebra.Mat4x4(f32) {
    return zalgebra.Mat4x4(f32).lookAt(self.position, self.position.add(self.front), up);
}

pub fn tick(self: *Camera, state: *State) void {
    if (state.window.isKeyDown(c.GLFW_KEY_W)) {
        self.position = self.position.add(self.front.scale(speed * state.delta_time));
    }

    if (state.window.isKeyDown(c.GLFW_KEY_S)) {
        self.position = self.position.sub(self.front.scale(speed * state.delta_time));
    }

    const right = self.front.cross(up).norm();

    if (state.window.isKeyDown(c.GLFW_KEY_A)) {
        self.position = self.position.sub(right.scale(speed * state.delta_time));
    }

    if (state.window.isKeyDown(c.GLFW_KEY_D)) {
        self.position = self.position.add(right.scale(speed * state.delta_time));
    }

    if (state.window.mouse.delta_x != 0 or state.window.mouse.delta_y != 0) {
        state.window.mouse.delta_x *= sensitivity;
        state.window.mouse.delta_y *= sensitivity;

        self.yaw += @floatCast(state.window.mouse.delta_x);
        self.pitch += @floatCast(state.window.mouse.delta_y);

        if (self.pitch > 89.0) {
            self.pitch = 89.0;
        }

        if (self.pitch < -89.0) {
            self.pitch = -89.0;
        }

        const direction = zalgebra.Vec3.fromSlice(&[_]f32{ @cos(zalgebra.toRadians(self.yaw)) * @cos(zalgebra.toRadians(self.pitch)), @sin(zalgebra.toRadians(self.pitch)), @sin(zalgebra.toRadians(self.yaw)) * @cos(zalgebra.toRadians(self.pitch)) });
        self.front = direction;
    }
}
