const c = @import("c.zig").c;
const block = @import("block.zig");
const std = @import("std");
const Arcade = @import("Arcade.zig");
const fastnoise = @import("fastnoise.zig");

const noise: fastnoise.Noise(f32) = .{
    .seed = 1337,
    .noise_type = .cellular,
    .frequency = 0.025,
    .gain = 0.40,
    .fractal_type = .fbm,
    .lacunarity = 0.40,
    .cellular_distance = .euclidean,
    .cellular_return = .distance2,
    .cellular_jitter_mod = 0.88,
};

const Clod = @This();

pub const Position = struct {
    x: i32,
    y: i32,
    z: i32,

    pub fn add(self: Position, other: Position) Position {
        return .{
            .x = self.x + other.x,
            .y = self.y + other.y,
            .z = self.z + other.z,
        };
    }

    pub fn eql(self: Position, other: Position) bool {
        return self.x == other.x and self.y == other.y and self.z == other.z;
    }
};

const tree_height: usize = 3;

pub const width: usize = 16;
pub const height: usize = 16;
pub const depth: usize = 16;

const size = width * height * depth;

position: Position,
vao: u32,
blocks: [size]block.Type,
vertices: std.ArrayList(f32), // TODO: Change to fixed array
dirty: bool,

pub fn init(allocator: std.mem.Allocator, x: i32, y: i32, z: i32) !*Clod {
    var clod = try allocator.create(Clod);

    var blocks: [size]block.Type = undefined;

    for (0..depth) |block_z| {
        for (0..height) |block_y| {
            for (0..width) |block_x| {
                var block_type: block.Type = .air;
                const real_x = @as(i32, @intCast(block_x)) + x * @as(i32, @intCast(width));
                const real_y = @as(i32, @intCast(block_y)) + y * @as(i32, @intCast(height));
                const real_z = @as(i32, @intCast(block_z)) + z * @as(i32, @intCast(depth));

                const value = noise.genNoise3D(@floatFromInt(real_x), @floatFromInt(real_y * y * @as(i32, @intCast(height))), @floatFromInt(real_z));

                if (value < -0.4) {
                    block_type = .stone;
                } else if (value < -0.3) {
                    block_type = .grass;
                }

                blocks[idx(block_x, block_y, block_z)] = block_type;
            }
        }
    }

    const vertices = try std.ArrayList(f32).initCapacity(allocator, block.total_elements);

    var vao: u32 = 0;
    c.glGenVertexArrays(1, &vao);
    c.glBindVertexArray(vao);

    var vbo: u32 = 0;
    c.glGenBuffers(1, &vbo);
    c.glBindBuffer(c.GL_ARRAY_BUFFER, vbo);

    c.glBufferData(c.GL_ARRAY_BUFFER, @as(c_long, @intCast(vertices.items.len)) * @sizeOf(f32), @ptrCast(vertices.items), c.GL_STATIC_DRAW);
    c.glVertexAttribPointer(0, 3, c.GL_FLOAT, c.GL_FALSE, 6 * @sizeOf(f32), @ptrFromInt(0));
    c.glEnableVertexAttribArray(0);
    c.glVertexAttribPointer(1, 2, c.GL_FLOAT, c.GL_FALSE, 6 * @sizeOf(f32), @ptrFromInt(3 * @sizeOf(f32)));
    c.glEnableVertexAttribArray(1);
    c.glVertexAttribPointer(2, 1, c.GL_FLOAT, c.GL_FALSE, 6 * @sizeOf(f32), @ptrFromInt(5 * @sizeOf(f32)));
    c.glEnableVertexAttribArray(2);

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

pub fn populateLife(self: *Clod, arcade: *Arcade) !void {
    var iter = arcade.homeless_blocks.iterator();
    while (iter.next()) |entry| {
        const key = entry.key_ptr.*;

        if (key.clod_position.eql(self.position)) {
            self.blocks[idx(key.block_position.x, key.block_position.y, key.block_position.z)] = entry.value_ptr.*;

            _ = arcade.homeless_blocks.remove(key);
        }
    }

    if (self.position.y == 0) {
        try self.spawnTree(15, 9, 15, arcade);
    }
}

fn spawnTree(self: *Clod, x: usize, y: usize, z: usize, arcade: *Arcade) !void {
    var leaves_z: i32 = -2;
    while (leaves_z < 3) : (leaves_z += 1) {
        var leaves_y: i32 = 0;
        while (leaves_y < 5) : (leaves_y += 1) {
            var leaves_x: i32 = -2;
            while (leaves_x < 3) : (leaves_x += 1) {
                if (leaves_y > 2 and ((leaves_z > 1 or leaves_x > 1) or (leaves_z < -1 or leaves_x < -1))) {
                    continue;
                }

                if (leaves_x + @as(i32, @intCast(x)) < 0 or leaves_y + @as(i32, @intCast(tree_height - 1 + y)) < 0 or leaves_z + @as(i32, @intCast(z)) < 0 or leaves_x + @as(i32, @intCast(x)) > Clod.width - 1 or leaves_y + @as(i32, @intCast(y + tree_height - 1)) > Clod.height - 1 or leaves_z + @as(i32, @intCast(z)) > Clod.depth - 1) {
                    try arcade.addBlock(self.position, @intCast(@as(i32, @intCast(x)) + leaves_x), @intCast(@as(i32, @intCast(y + tree_height)) + leaves_y - 1), @intCast(@as(i32, @intCast(z)) + leaves_z), .leaves);
                    continue;
                }

                self.blocks[idx(@intCast(@as(i32, @intCast(x)) + leaves_x), @intCast(@as(i32, @intCast(y + tree_height)) + leaves_y - 1), @intCast(@as(i32, @intCast(z)) + leaves_z))] = .leaves;
            }
        }
    }

    for (0..tree_height) |tree_y| {
        self.blocks[idx(x, y + tree_y, z)] = .bark;
    }
}

pub fn generateMesh(self: *Clod, allocator: std.mem.Allocator, clods: std.AutoHashMap(Position, *Clod)) !void {
    self.vertices.clearAndFree(allocator);

    const left_clod = clods.get(self.position.add(.{ .x = -1, .y = 0, .z = 0 }));
    const right_clod = clods.get(self.position.add(.{ .x = 1, .y = 0, .z = 0 }));
    const top_clod = clods.get(self.position.add(.{ .x = 0, .y = 1, .z = 0 }));
    const bottom_clod = clods.get(self.position.add(.{ .x = 0, .y = -1, .z = 0 }));
    const front_clod = clods.get(self.position.add(.{ .x = 0, .y = 0, .z = -1 }));
    const back_clod = clods.get(self.position.add(.{ .x = 0, .y = 0, .z = 1 }));

    for (0..depth) |block_z| {
        for (0..height) |block_y| {
            for (0..width) |block_x| {
                const block_type = self.blocks[idx(block_x, block_y, block_z)];

                if (block_type == .air) {
                    continue;
                }

                const real_x = @as(f32, @floatFromInt(block_x)) + @as(f32, @floatFromInt(self.position.x * @as(i32, @intCast(width))));
                const real_y = @as(f32, @floatFromInt(block_y)) + @as(f32, @floatFromInt(self.position.y * @as(i32, @intCast(height))));
                const real_z = @as(f32, @floatFromInt(block_z)) + @as(f32, @floatFromInt(self.position.z * @as(i32, @intCast(depth))));

                const up_block = blk: {
                    if (block_y == height - 1) {
                        if (top_clod) |clod| {
                            break :blk clod.blocks[idx(block_x, 0, block_z)];
                        }

                        break :blk null;
                    }

                    break :blk self.blocks[idx(block_x, block_y + 1, block_z)];
                };

                if (up_block == null or up_block == .air) {
                    try self.vertices.appendSlice(allocator, &block.getVertices(block_type, .top, real_x, real_y, real_z));
                }

                const down_block = blk: {
                    if (block_y == 0) {
                        if (bottom_clod) |clod| {
                            break :blk clod.blocks[idx(block_x, height - 1, block_z)];
                        }

                        break :blk null;
                    }

                    break :blk self.blocks[idx(block_x, block_y - 1, block_z)];
                };

                if (down_block == null or down_block == .air) {
                    try self.vertices.appendSlice(allocator, &block.getVertices(block_type, .bottom, real_x, real_y, real_z));
                }

                const back_block = blk: {
                    if (block_z == depth - 1) {
                        if (back_clod) |clod| {
                            break :blk clod.blocks[idx(block_x, block_y, 0)];
                        }

                        break :blk null;
                    }

                    break :blk self.blocks[idx(block_x, block_y, block_z + 1)];
                };

                if (back_block == null or back_block == .air) {
                    try self.vertices.appendSlice(allocator, &block.getVertices(block_type, .back, real_x, real_y, real_z));
                }

                const front_block = blk: {
                    if (block_z == 0) {
                        if (front_clod) |clod| {
                            break :blk clod.blocks[idx(block_x, block_y, depth - 1)];
                        }

                        break :blk null;
                    }

                    break :blk self.blocks[idx(block_x, block_y, block_z - 1)];
                };

                if (front_block == null or front_block == .air) {
                    try self.vertices.appendSlice(allocator, &block.getVertices(block_type, .front, real_x, real_y, real_z));
                }

                const left_block = blk: {
                    if (block_x == 0) {
                        if (left_clod) |clod| {
                            break :blk clod.blocks[idx(width - 1, block_y, block_z)];
                        }

                        break :blk null;
                    }

                    break :blk self.blocks[idx(block_x - 1, block_y, block_z)];
                };

                if (left_block == null or left_block == .air) {
                    try self.vertices.appendSlice(allocator, &block.getVertices(block_type, .left, real_x, real_y, real_z));
                }

                const right_block = blk: {
                    if (block_x == width - 1) {
                        if (right_clod) |clod| {
                            break :blk clod.blocks[idx(0, block_y, block_z)];
                        }

                        break :blk null;
                    }

                    break :blk self.blocks[idx(block_x + 1, block_y, block_z)];
                };

                if (right_block == null or right_block == .air) {
                    try self.vertices.appendSlice(allocator, &block.getVertices(block_type, .right, real_x, real_y, real_z));
                }
            }
        }
    }

    // TODO: This is just spritebatching, so use glSubBufferData()

    c.glBindVertexArray(self.vao);

    var vbo: u32 = 0;
    c.glGenBuffers(1, &vbo);
    c.glBindBuffer(c.GL_ARRAY_BUFFER, vbo);

    c.glBufferData(c.GL_ARRAY_BUFFER, @as(c_long, @intCast(self.vertices.items.len)) * @sizeOf(f32), @ptrCast(self.vertices.items), c.GL_STATIC_DRAW);
    c.glVertexAttribPointer(0, 3, c.GL_FLOAT, c.GL_FALSE, 6 * @sizeOf(f32), @ptrFromInt(0));
    c.glEnableVertexAttribArray(0);
    c.glVertexAttribPointer(1, 2, c.GL_FLOAT, c.GL_FALSE, 6 * @sizeOf(f32), @ptrFromInt(3 * @sizeOf(f32)));
    c.glEnableVertexAttribArray(1);
    c.glVertexAttribPointer(2, 1, c.GL_FLOAT, c.GL_FALSE, 6 * @sizeOf(f32), @ptrFromInt(5 * @sizeOf(f32)));
    c.glEnableVertexAttribArray(2);
}

pub fn blit(self: Clod) void {
    c.glBindVertexArray(self.vao);
    c.glDrawArrays(c.GL_TRIANGLES, 0, @intCast(self.vertices.items.len));
}

pub fn idx(x: usize, y: usize, z: usize) usize {
    return (z * depth * height) + (y * height) + x;
}

pub fn handleNegative(n: f32) i32 {
    if (n < 0) {
        return @divTrunc(@as(i32, @intFromFloat(n)), @as(i32, @intCast(width))) - 1;
    } else {
        return @divTrunc(@as(i32, @intFromFloat(n)), @as(i32, @intCast(width)));
    }
}
