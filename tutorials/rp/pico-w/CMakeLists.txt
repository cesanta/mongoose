cmake_minimum_required(VERSION 3.13)

# If you will set board type here, do it BEFORE including the SDK
set(PICO_BOARD pico_w)
include(pico_sdk_import.cmake)

project(firmware)
pico_sdk_init()

add_executable(firmware main.c wifi.c mongoose/mongoose.c)
target_include_directories(firmware PRIVATE mongoose)

target_link_libraries(firmware pico_stdlib hardware_spi pico_rand pico_cyw43_driver pico_cyw43_arch_none hardware_flash pico_bootrom)
pico_add_extra_outputs(firmware)     # create map/bin/hex file etc.
pico_enable_stdio_usb(firmware 1)    # Route stdio to USB

# Prevent extreme optimization from breaking OTA
target_compile_options(firmware PRIVATE -fno-tree-loop-distribute-patterns)

# Wi-Fi credentials from the environment, e.g. make WIFI_NAME=... WIFI_PASS=...
# Read when build/ is configured: run "make clean" after changing them
if (DEFINED ENV{WIFI_NAME})
  target_compile_definitions(firmware PRIVATE "WIFI_NAME=\"$ENV{WIFI_NAME}\"")
endif()
if (DEFINED ENV{WIFI_PASS})
  target_compile_definitions(firmware PRIVATE "WIFI_PASS=\"$ENV{WIFI_PASS}\"")
endif()
