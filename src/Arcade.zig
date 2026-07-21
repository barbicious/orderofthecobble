const std = @import("std");
const Clod = @import("Clod.zig");
const State = @import("State.zig");
const zalgebra = @import("zalgebra");

const render_distance: i32 = 1;

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

    var iter = clods.iterator();
    while (iter.next()) |clod| {
        try clod.value_ptr.*.generateMesh(allocator, clods);
    }

    return .{
        .clods = clods
    };
}

pub fn blit(self: Arcade) void {
    var iter = self.clods.iterator();
    while (iter.next()) |clod| {
        clod.value_ptr.*.blit();
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
                const clod_position = Clod.Position {
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

    var it = self.clods.iterator();
    while (it.next()) |entry| {
        if (entry.value_ptr.*.dirty) {
            _ = self.clods.remove(entry.key_ptr.*);
        }
    }

    var i = self.clods.valueIterator();
    while (i.next()) |clod| {
        try clod.*.generateMesh(allocator, self.clods);
    }
}