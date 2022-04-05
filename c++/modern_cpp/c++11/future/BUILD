# https://github.com/apache/incubator-brpc/blob/master/BUILD.bazel
COPTS = [
]

LINKOPTS = [
    "-lpthread",
    "-ldl",
]

cc_library(
    name = "librun_future",
    srcs = glob([
        "*.cc",
    ]),
    copts = COPTS,
    linkopts = LINKOPTS,
    visibility = ["//visibility:public"],
)

cc_binary(
    name = "main",
    visibility = [
	    "//visibility:public",
    ],
    deps = [
	    ":librun_future",
    ],
)
