# Copyright (c) 2022 Cesanta Software Limited
# Mail: support@cesanta.com
#
# SPDX-License-Identifier: GPL2.0 or commercial

cmake_minimum_required(VERSION 3.20.0)
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(device_dashboard)
add_definitions(-DMG_ENABLE_PACKED_FS=1)
target_sources(app PRIVATE src/main.c src/mongoose.c src/net.c src/packed_fs.c)
