EXTERNAL_DEPENDENCIES = [
    "@notstd//:notstd",
    "@glm//:glm",
    "@glfw3//:glfw3",
]

CPP_FILES = glob(["src/**/*.cpp"])

HEADER_FILES = glob(["include/**/*.h"])

cc_library(
    name = "grok3d_debug",
    srcs = CPP_FILES + HEADER_FILES,
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
    srcs = CPP_FILES + HEADER_FILES,
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
