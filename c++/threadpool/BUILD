# https://github.com/apache/incubator-brpc/blob/master/BUILD.bazel
COPTS = [
	"-std=c++14",
]

LINKOPTS = [
    "-pthread",
]

cc_library(
    name = "libmain",
    srcs = glob([
        "*.cc",
    ]),
    hdrs = ['thread_pool.h'],
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
	    ":libmain",
    ],
)
