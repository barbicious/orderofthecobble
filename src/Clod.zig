const c = @import("c.zig").c;
const block = @import("block.zig");
const std = @import("std");

const Clod = @This();

const Position = struct {
    x: i32,
    y: i32,
    z: i32,
};

const width: usize = 16;
const height: usize = 16;
const depth: usize = 16;

const size = width * height * depth;

position: Position,
vao: u32,
blocks: [size]block.Type,
vertices: std.ArrayList(f32), // TODO: Change to fixed array

pub fn init(allocator: std.mem.Allocator, x: i32, y: i32, z: i32) !*Clod {
    var clod = try allocator.create(Clod);

    var blocks: [size]block.Type = undefined;

    for (0..depth) |block_z| {
        for (0..height) |block_y| {
            for (0..width) |block_x| {
                var block_type: block.Type = .air;

                if (block_y < 7) {
                    block_type = .stone;
                } else if (block_y < 9) {
                    block_type = .grass;
                }

                blocks[idx(block_x, block_y, block_z)] = block_type;
            }
        }
    }

    var vertices = try std.ArrayList(f32).initCapacity(allocator, block.total_elements);

    for (0..depth) |block_z| {
        for (0..height) |block_y| {
            for (0..width) |block_x| {
                const block_type = blocks[idx(block_x, block_y, block_z)];

                if (block_type == .air) {
                    continue;
                }

                const real_x = @as(f32, @floatFromInt(block_x)) + @as(f32, @floatFromInt(x * @as(i32, @intCast(width))));
                const real_y = @as(f32, @floatFromInt(block_y)) + @as(f32, @floatFromInt(y * @as(i32, @intCast(height))));
                const real_z = @as(f32, @floatFromInt(block_z)) + @as(f32, @floatFromInt(z * @as(i32, @intCast(depth))));

                try vertices.appendSlice(allocator, &block.getVertices(block_type, .top, real_x, real_y, real_z));
                try vertices.appendSlice(allocator, &block.getVertices(block_type, .bottom, real_x, real_y, real_z));
                try vertices.appendSlice(allocator, &block.getVertices(block_type, .back, real_x, real_y, real_z));
                try vertices.appendSlice(allocator, &block.getVertices(block_type, .front, real_x, real_y, real_z));
                try vertices.appendSlice(allocator, &block.getVertices(block_type, .left, real_x, real_y, real_z));
                try vertices.appendSlice(allocator, &block.getVertices(block_type, .right, real_x, real_y, real_z));
            }
        }
    }

    var vao: u32 = 0;
    c.glGenVertexArrays(1, &vao);
    c.glBindVertexArray(vao);

    var vbo: u32 = 0;
    c.glGenBuffers(1, &vbo);
    c.glBindBuffer(c.GL_ARRAY_BUFFER, vbo);

    c.glBufferData(c.GL_ARRAY_BUFFER, @as(c_long, @intCast(vertices.items.len)) * @sizeOf(f32), @ptrCast(vertices.items), c.GL_STATIC_DRAW);
    c.glVertexAttribPointer(0, 3, c.GL_FLOAT, c.GL_FALSE, 5 * @sizeOf(f32), @ptrFromInt(0));
    c.glEnableVertexAttribArray(0);
    c.glVertexAttribPointer(1, 2, c.GL_FLOAT, c.GL_FALSE, 5 * @sizeOf(f32), @ptrFromInt(3 * @sizeOf(f32)));
    c.glEnableVertexAttribArray(1);

    clod.blocks = blocks;
    clod.position = .{
        .x = x,
        .y = y,
        .z = z,
    };
    clod.vao = vao;
    clod.vertices = vertices;

    return clod;
}

pub fn generateMesh(self: Clod) void {
    var vao: u32 = 0;
    c.glGenVertexArrays(1, &vao);
    c.glBindVertexArray(vao);

    var vbo: u32 = 0;
    c.glGenBuffers(1, &vbo);
    c.glBindBuffer(c.GL_ARRAY_BUFFER, vbo);

    c.glBufferData(c.GL_ARRAY_BUFFER, @as(c_long, @intCast(self.vertices.items.len)) * @sizeOf(f32), @ptrCast(self.vertices.items), c.GL_STATIC_DRAW);
    c.glVertexAttribPointer(0, 3, c.GL_FLOAT, c.GL_FALSE, 5 * @sizeOf(f32), @ptrFromInt(0));
    c.glEnableVertexAttribArray(0);
    c.glVertexAttribPointer(1, 2, c.GL_FLOAT, c.GL_FALSE, 5 * @sizeOf(f32), @ptrFromInt(3 * @sizeOf(f32)));
    c.glEnableVertexAttribArray(1);
}

pub fn idx(x: usize, y: usize, z: usize) usize {
    return (z * depth * height) + (y * height) + x;
}
