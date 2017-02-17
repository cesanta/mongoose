# simplest basel (http://basel.io) BUILD file for mongoose

package(default_visibility = ["//visibility:public"],)

filegroup(
    name = "mongoose_sources",
    srcs = [
        "mongoose.c",
        "mongoose.h",
    ],
)

cc_library(
    name = "mongoose_lib",
    srcs = ["mongoose.c"],
    hdrs = ["mongoose.h"],
    copts = [
        "-DNO_CGI",
    ],
    defines = [
        "NO_SSL",
        "USE_WEBSOCKET",
    ],
)

cc_binary(
    name = "simplest_web_server",
    srcs = [
      "examples/simplest_web_server/simplest_web_server.c",
    ],
    deps = [
      ":mongoose_lib",
    ],
)
