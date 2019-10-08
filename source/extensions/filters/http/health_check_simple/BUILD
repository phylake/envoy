licenses(["notice"])  # Apache 2

load(
    "//bazel:envoy_build_system.bzl",
    "envoy_cc_library",
    "envoy_package",
)

envoy_package()

envoy_cc_library(
    name = "health_check_simple_lib",
    srcs = ["health_check_simple.cc"],
    hdrs = ["health_check_simple.h"],
    deps = [
        "//include/envoy/http:filter_interface",
        "//include/envoy/server:filter_config_interface",
        "//source/common/http:codes_lib",
        "//source/common/http:header_utility_lib",
    ],
)

envoy_cc_library(
    name = "config",
    srcs = ["config.cc"],
    deps = [
        ":health_check_simple_lib",
        "//include/envoy/registry",
        "//source/extensions/filters/http/common:factory_base_lib",
        "@envoy_api//envoy/api/v2/route:route_cc",
        "@envoy_api//envoy/config/filter/http/health_check_simple/v2:health_check_simple_cc",
    ],
)
