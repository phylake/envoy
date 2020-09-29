licenses(["notice"])  # Apache 2

load(
    "//bazel:envoy_build_system.bzl",
    "envoy_cc_library",
    "envoy_package",
)

envoy_package()

envoy_cc_library(
    name = "protobufs_interface",
    hdrs = ["protobufs.h"],
    deps = ["//source/common/protobuf:utility_lib"],
)

envoy_cc_library(
    name = "equality_interface",
    hdrs = ["equality.h"],
    deps = [],
)
