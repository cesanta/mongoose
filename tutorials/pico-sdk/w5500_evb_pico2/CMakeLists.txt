cmake_minimum_required(VERSION 3.13)

# If you will set board type here, do it BEFORE including the SDK
set(PICO_BOARD pico2)
include(pico_sdk_import.cmake)

project(firmware)
pico_sdk_init()

# Sources are identical to ../w5500_evb_pico, we're just setting pico2 board
add_executable(firmware ../w5500_evb_pico/main.c mongoose.c)
target_include_directories(firmware PRIVATE . ../w5500_evb_pico)

target_link_libraries(firmware pico_stdlib hardware_spi pico_rand  hardware_flash pico_bootrom)
pico_add_extra_outputs(firmware)     # create map/bin/hex file etc.
pico_enable_stdio_usb(firmware 1)    # Route stdio

# Prevent extreme optimization from breaking OTA
target_compile_options(firmware PRIVATE -fno-tree-loop-distribute-patterns)
