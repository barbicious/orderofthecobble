const std = @import("std");
const State = @import("State.zig");
const zalgebra = @import("zalgebra");

pub fn main(init: std.process.Init) !void {
    const allocator = init.arena.allocator();
    const io = init.io;

    var state = try State.init(allocator, io);
    state.window.installUserPointer();
    defer state.deinit();
    try state.run(allocator);
}
