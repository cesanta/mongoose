cmake_minimum_required(VERSION 3.13)
include(pico-sdk/pico_sdk_init.cmake)

project(firmware)
pico_sdk_init()

add_executable(firmware
  main.c
  ../../../mongoose.c
  ../../device-dashboard/net.c
  ../../device-dashboard/packed_fs.c
  pico-sdk//lib/tinyusb/examples/device/net_lwip_webserver/src/usb_descriptors.c
  pico-sdk/lib/tinyusb/lib/networking/rndis_reports.c)

target_include_directories(firmware PUBLIC
  .
  ../../..
  pico-sdk/lib/tinyusb/lib/networking)

target_link_libraries(firmware pico_stdlib hardware_spi tinyusb_device)
pico_add_extra_outputs(firmware)

pico_enable_stdio_usb(firmware 0)    # Route stdio
pico_enable_stdio_uart(firmware 1)   # to the UART

# Mongoose build flags
add_definitions(-DMG_ENABLE_MIP=1)
add_definitions(-DMG_ENABLE_PACKED_FS=1)
add_definitions(-DMG_ENABLE_FILE=0)
add_definitions(-DDISABLE_ROUTING=1)
