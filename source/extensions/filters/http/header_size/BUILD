licenses(["notice"])  # Apache 2

load(
    "//bazel:envoy_build_system.bzl",
    "envoy_cc_library",
    "envoy_package",
)

envoy_package()

envoy_cc_library(
    name = "header_size_lib",
    srcs = ["header_size.cc"],
    hdrs = ["header_size.h"],
    deps = [
        "//include/envoy/http:codes_interface",
        "//include/envoy/http:filter_interface",
        "//include/envoy/stats:stats_interface",
        "//include/envoy/stats:stats_macros",
        "//source/common/common:assert_lib",
        "//source/common/http:codes_lib",
        "//source/common/http:header_map_lib",
        "//source/common/http:headers_lib",
        "//source/common/http:utility_lib",
        "//source/extensions/filters/http:well_known_names",
        "@envoy_api//envoy/config/filter/http/header_size/v2:header_size_cc",
    ],
)

envoy_cc_library(
    name = "config",
    srcs = ["config.cc"],
    hdrs = ["config.h"],
    deps = [
        "//include/envoy/registry",
        "//source/common/config:filter_json_lib",
        "//source/extensions/filters/http:well_known_names",
        "//source/extensions/filters/http/header_size:header_size_lib",
        "//source/extensions/filters/http/common:factory_base_lib",
    ],
)
