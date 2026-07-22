const size: f32 = 8.0 / 256.0;

pub const Type = enum { air, grass, stone, bark, leaves };

pub const num_vertices: usize = 36;

pub const elements_per_face: usize = 30;
pub const total_elements: usize = elements_per_face * 6;

pub const Position = struct {
    x: usize,
    y: usize,
    z: usize,
};

pub const Face = enum {
    top,
    bottom,
    back,
    front,
    left,
    right,
};

pub fn getVertices(block_type: Type, face: Face, x: f32, y: f32, z: f32) [elements_per_face]f32 {
    const u: f32 = switch (block_type) {
        .grass => 0.0 / 256.0,
        .stone => 8.0 / 256.0,
        .bark => 24.0 / 256.0,
        .leaves => 32.0 / 256.0,
        .air => @panic("Attempted to get vertices from air tile!"),
    };

    const v: f32 = switch (block_type) {
        .grass => 0.0 / 256.0,
        .stone => 0.0 / 256.0,
        .bark => 0.0 / 256.0,
        .leaves => 0.0 / 256.0,
        .air => @panic("Attempted to get vertices from air tile!"),
    };

    switch (face) {
        .front => {
            return [_]f32{
                1.0 + x, 0.0 + y, 0.0 + z, u + size, v,
                0.0 + x, 0.0 + y, 0.0 + z, u,        v,
                1.0 + x, 1.0 + y, 0.0 + z, u + size, v + size,
                0.0 + x, 0.0 + y, 0.0 + z, u,        v,
                0.0 + x, 1.0 + y, 0.0 + z, u,        v + size,
                1.0 + x, 1.0 + y, 0.0 + z, u + size, v + size,
            };
        },
        .back => {
            return [_]f32{
                0.0 + x, 0.0 + y, 1.0 + z, u,        v,
                1.0 + x, 0.0 + y, 1.0 + z, u + size, v,
                1.0 + x, 1.0 + y, 1.0 + z, u + size, v + size,
                1.0 + x, 1.0 + y, 1.0 + z, u + size, v + size,
                0.0 + x, 1.0 + y, 1.0 + z, u,        v + size,
                0.0 + x, 0.0 + y, 1.0 + z, u,        v,
            };
        },
        .left => {
            return [_]f32{
                0.0 + x, 1.0 + y, 1.0 + z, u + size, v,
                0.0 + x, 1.0 + y, 0.0 + z, u + size, v + size,
                0.0 + x, 0.0 + y, 0.0 + z, u,        v + size,
                0.0 + x, 0.0 + y, 0.0 + z, u,        v + size,
                0.0 + x, 0.0 + y, 1.0 + z, u,        v,
                0.0 + x, 1.0 + y, 1.0 + z, u + size, v,
            };
        },
        .right => {
            return [_]f32{
                1.0 + x, 1.0 + y, 1.0 + z, u + size, v,
                1.0 + x, 0.0 + y, 0.0 + z, u,        v + size,
                1.0 + x, 1.0 + y, 0.0 + z, u + size, v + size,
                1.0 + x, 1.0 + y, 1.0 + z, u + size, v,
                1.0 + x, 0.0 + y, 1.0 + z, u,        v,
                1.0 + x, 0.0 + y, 0.0 + z, u,        v + size,
            };
        },
        .bottom => {
            return [_]f32{
                0.0 + x, 0.0 + y, 0.0 + z, u,        v + size,
                1.0 + x, 0.0 + y, 0.0 + z, u + size, v + size,
                1.0 + x, 0.0 + y, 1.0 + z, u + size, v,
                1.0 + x, 0.0 + y, 1.0 + z, u + size, v,
                0.0 + x, 0.0 + y, 1.0 + z, u,        v,
                0.0 + x, 0.0 + y, 0.0 + z, u,        v + size,
            };
        },
        .top => {
            return [_]f32{
                1.0 + x, 1.0 + y, 1.0 + z, u + size, v,
                1.0 + x, 1.0 + y, 0.0 + z, u + size, v + size,
                0.0 + x, 1.0 + y, 0.0 + z, u,        v + size,
                0.0 + x, 1.0 + y, 1.0 + z, u,        v,
                1.0 + x, 1.0 + y, 1.0 + z, u + size, v,
                0.0 + x, 1.0 + y, 0.0 + z, u,        v + size,
            };
        },
    }
}
