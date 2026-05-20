cmake_minimum_required(VERSION 3.13)

# If you will set board type here, do it BEFORE including the SDK
# set(PICO_BOARD pico2_w)
include(pico_sdk_import.cmake)

project(firmware)
pico_sdk_init()
add_executable(firmware main.c mongoose.c)

target_link_libraries(firmware pico_stdlib hardware_pio pico_rand hardware_flash pico_bootrom)
pico_add_extra_outputs(firmware)     # create map/bin/hex file etc.
pico_enable_stdio_usb(firmware 1)    # Route stdio

# Prevent extreme optimization from breaking OTA
target_compile_options(firmware PRIVATE -fno-tree-loop-distribute-patterns)
