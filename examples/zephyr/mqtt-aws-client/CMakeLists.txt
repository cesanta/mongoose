# Copyright (c) 2022 Cesanta Software Limited
# Mail: support@cesanta.com
#
# SPDX-License-Identifier: GPL2.0 or commercial

cmake_minimum_required(VERSION 3.20.0)
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(mqtt_aws_client)
#add_definitions(-DMG_ENABLE_LINES=1)
add_definitions(-DMG_ENABLE_MBEDTLS=1 -DMG_MBEDTLS_DEBUG_LEVEL=4)
target_sources(app PRIVATE src/main.c src/mongoose.c)
