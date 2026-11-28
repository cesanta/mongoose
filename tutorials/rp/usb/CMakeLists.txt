cmake_minimum_required(VERSION 3.13)
set(PICO_BOARD pico2 CACHE STRING "Target board")
include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)
project(usb_ncm C CXX ASM)
pico_sdk_init()
add_executable(firmware main.c ../../../mongoose.c
  ../../device-dashboard/full/dashboard.c
  ../../device-dashboard/full/file_data.c)
target_include_directories(firmware PRIVATE . ../../..)
target_link_libraries(firmware pico_stdlib pico_unique_id pico_rand hardware_resets)
pico_enable_stdio_usb(firmware 0)
pico_enable_stdio_uart(firmware 0)
pico_add_extra_outputs(firmware)
