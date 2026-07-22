const std = @import("std");
const Clod = @import("Clod.zig");
const State = @import("State.zig");
const zalgebra = @import("zalgebra");
const block = @import("block.zig");

const render_distance: i32 = 2;

const Arcade = @This();

clods: std.AutoHashMap(Clod.Position, *Clod),

pub fn init(allocator: std.mem.Allocator) !Arcade {
    var clods = std.AutoHashMap(Clod.Position, *Clod).init(allocator);

    var z: i32 = -render_distance;
    while (z <= render_distance) : (z += 1) {
        var y: i32 = -render_distance;
        while (y <= render_distance) : (y += 1) {
            var x: i32 = -render_distance;
            while (x <= render_distance) : (x += 1) {
                const clod = try Clod.init(allocator, x, y, z);
                try clods.put(clod.position, clod);
            }
        }
    }

    var arcade: Arcade = .{ .clods = clods };

    var iter = arcade.clods.iterator();
    while (iter.next()) |clod| {
        clod.value_ptr.*.populateLife(&arcade);
    }

    iter = arcade.clods.iterator();
    while (iter.next()) |clod| {
        try clod.value_ptr.*.generateMesh(allocator, arcade.clods);
    }

    return arcade;
}

pub fn blit(self: Arcade) void {
    var iter = self.clods.iterator();
    while (iter.next()) |clod| {
        if (clod.value_ptr.*.dirty) {
            continue;
        }

        clod.value_ptr.*.blit();
    }
}

pub fn addBlock(self: *Arcade, clod_position: Clod.Position, x: i32, y: i32, z: i32, block_type: block.Type) void {
    var clod_dst = clod_position;

    var x_dst = x;
    var y_dst = y;
    var z_dst = z;

    std.log.debug("{d}", .{y});

    if (x_dst < 0) {
        clod_dst.x -= 1;
        x_dst += Clod.width;
    }

    // if (x_dst >= Clod.height - 1) {
    //     clod_dst.x += 1;
    //     x_dst -= Clod.height - 1;
    // }

    if (y_dst < 0) {
        clod_dst.y -= 1;
        y_dst += Clod.height;
    }

    // if (y_dst >= Clod.height - 1) {
    //     clod_dst.y += 1;
    //     y_dst -= Clod.height - 1;
    // }

    if (z_dst < 0) {
        clod_dst.z -= 1;
        z_dst += Clod.depth;
    }

    // if (z_dst >= Clod.depth - 1) {
    //     clod_dst.z += 1;
    //     z_dst -= Clod.depth - 1;
    // }

    if (self.clods.get(clod_dst)) |clod| {
        clod.blocks[Clod.idx(@intCast(x_dst), @intCast(y_dst), @intCast(z_dst))] = block_type;
    }
}

pub fn crossClodBoundary(self: *Arcade, allocator: std.mem.Allocator, player_x: i32, player_y: i32, player_z: i32) !void {
    var iter = self.clods.valueIterator();
    while (iter.next()) |clod| {
        clod.*.dirty = true;
    }

    var z: i32 = -render_distance + player_z;
    while (z <= render_distance + player_z) : (z += 1) {
        var y: i32 = -render_distance + player_y;
        while (y <= render_distance + player_y) : (y += 1) {
            var x: i32 = -render_distance + player_x;
            while (x <= render_distance + player_x) : (x += 1) {
                const clod_position = Clod.Position{
                    .x = x,
                    .y = y,
                    .z = z,
                };

                if (self.clods.get(clod_position)) |c| {
                    c.dirty = false;
                } else {
                    const c = try Clod.init(allocator, x, y, z);
                    try self.clods.put(c.position, c);
                }
            }
        }
    }



    iter = self.clods.valueIterator();

    while (iter.next()) |clod| {
        clod.*.populateLife(self);
    }

    iter = self.clods.valueIterator();

    while (iter.next()) |clod| {
        try clod.*.generateMesh(allocator, self.clods);
    }
}
