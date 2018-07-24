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
        "-O0",
        "-Wall",
        "-fno-exceptions",
    ],
    includes = ["include"],
    strip_include_prefix = "include",
    deps = EXTERNAL_DEPENDENCIES,
)
