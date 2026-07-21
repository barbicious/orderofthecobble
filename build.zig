const std = @import("std");

pub fn build(b: *std.Build) void {
    const target = b.standardTargetOptions(.{});

    const glfw_dep = b.dependency("glfw_zig", .{
        .target = target,
        .optimize = .ReleaseFast
    });

    const glad_dep = b.dependency("zig_glad", .{
        .target = target,
        .optimize = .ReleaseFast,
    });

    const zalgebra_dep = b.dependency("zalgebra", .{
        .target = target,
        .optimize = .ReleaseFast,
    });

    const exe = b.addExecutable(.{
        .name = "orderofthecobble",
        .root_module = b.createModule(.{
            .root_source_file = b.path("src/main.zig"),
            .target = target,
            .optimize = .ReleaseFast,
            .link_libc = true,
            .imports = &.{
                .{ .name = "zalgebra", .module = zalgebra_dep.module("zalgebra") }
            }
        }),
    });

    exe.root_module.linkLibrary(glad_dep.artifact("glad"));
    exe.root_module.linkLibrary(glfw_dep.artifact("glfw"));

    exe.root_module.addCSourceFile(.{ .file = b.path("lib/stb_image.c") });
    exe.root_module.addIncludePath(b.path("lib"));

    b.installArtifact(exe);

    const run_step = b.step("run", "Run the app");

    const run_cmd = b.addRunArtifact(exe);
    run_step.dependOn(&run_cmd.step);

    run_cmd.step.dependOn(b.getInstallStep());

    if (b.args) |args| {
        run_cmd.addArgs(args);
    }

    const exe_tests = b.addTest(.{
        .root_module = exe.root_module,
    });

    const run_exe_tests = b.addRunArtifact(exe_tests);

    const test_step = b.step("test", "Run tests");
    test_step.dependOn(&run_exe_tests.step);
}
