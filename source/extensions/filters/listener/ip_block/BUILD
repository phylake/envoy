licenses(["notice"])  # Apache 2

# IP blocking listener filter
# Public docs: docs/root/configuration/listener_filters/ip_block_filter.rst

load(
    "//bazel:envoy_build_system.bzl",
    "envoy_cc_library",
    "envoy_package",
)

envoy_package()

envoy_cc_library(
    name = "ip_block_lib",
    srcs = ["ip_block.cc"],
    hdrs = ["ip_block.h"],
    deps = [
        "//include/envoy/network:filter_interface",
        "//include/envoy/network:listen_socket_interface",
        "//source/common/network:cidr_range_lib",
        "//source/common/network:utility_lib",
        "@envoy_api//envoy/config/filter/network/ip_block/v2:ip_block_cc",
    ],
)

envoy_cc_library(
    name = "config",
    srcs = ["config.cc"],
    deps = [
        ":ip_block_lib",
        "//include/envoy/registry",
        "//include/envoy/server:filter_config_interface",
        "//source/extensions/filters/listener:well_known_names",
    ],
)
