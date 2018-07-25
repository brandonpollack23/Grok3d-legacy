licenses = ["notice"]

# TODO filegroup?
EXTERNAL_DEPENDENCIES = [
    "@notstd//:notstd",
    "@glm//:glm",
    "@glfw3//:glfw3",
]

filegroup(
    name = "grok3d_source_files",
    srcs = glob(["src/**/*.cpp"]),
)

filegroup(
    name = "grok3d_headers",
    srcs = glob(["include/**/*.h"]),
)

filegroup(
    name = "visible_headers",
    srcs = [
        "include/grok3d.h",
        "include/grok3d_types.h",
    ],
)

cc_library(
    name = "grok3d_debug",
    srcs = [
        "grok3d_source_files",
        ":grok3d_headers",
    ],
    hdrs = [":visible_headers"],
    copts = [
        "-std=c++17",
        "-g",
        "-O0",
        "-Wall",
        "-fno-exceptions",
    ],
    includes = ["include"],
    strip_include_prefix = "include",
    visibility = ["//visibility:public"],
    deps = EXTERNAL_DEPENDENCIES,
)

cc_library(
    name = "grok3d",
    srcs = [
        ":grok3d_headers",
        ":grok3d_source_files",
    ],
    hdrs = [":visible_headers"],
    copts = [
        "-std=c++17",
        "-O3",
        "-Wall",
        "-fno-exceptions",
    ],
    includes = ["include"],
    strip_include_prefix = "include",
    visibility = ["//visibility:public"],
    deps = EXTERNAL_DEPENDENCIES,
)
