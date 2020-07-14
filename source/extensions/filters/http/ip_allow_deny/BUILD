licenses(["notice"])  # Apache 2

load(
    "//bazel:envoy_build_system.bzl",
    "envoy_cc_library",
    "envoy_package",
)

envoy_package()

envoy_cc_library(
    name = "ip_allow_deny_lib",
    srcs = ["ip_allow_deny.cc"],
    hdrs = ["ip_allow_deny.h"],
    deps = [
        "//include/envoy/http:filter_interface",
        "//include/envoy/stats:stats_interface",
        "//include/envoy/stats:stats_macros",
        "//source/common/common:assert_lib",
        "//source/common/http:codes_lib",
        "//source/common/http:headers_lib",
        "//source/common/network:cidr_range_lib",
        "//source/extensions/filters/http:well_known_names",
        "@envoy_api//envoy/config/filter/network/ip_allow_deny/v2:pkg_cc_proto",
    ],
)

envoy_cc_library(
    name = "config",
    srcs = ["config.cc"],
    deps = [
        ":ip_allow_deny_lib",
        "//include/envoy/registry",
        "//source/extensions/filters/http:well_known_names",
        "//source/extensions/filters/http/common:factory_base_lib",
    ],
)
