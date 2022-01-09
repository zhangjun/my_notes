# https://github.com/apache/incubator-brpc/blob/master/BUILD.bazel
COPTS = [
]

LINKOPTS = [
    "-lpthread",
    "-ldl",
]

cc_library(
    name = "type_trait",
    srcs = glob([
        "*.cc",
    ]),
    copts = COPTS,
    linkopts = LINKOPTS,
    visibility = ["//visibility:public"],
)

cc_library(
    name = "type_main",
    visibility = [
	    "//visibility:public",
    ],
    deps = [
	    ":type_trait",
    ],
)
