cmake_minimum_required(VERSION 3.13)
include(pico-sdk/pico_sdk_init.cmake)

project(example)
pico_sdk_init()

add_executable(example main.c mongoose.c)
target_link_libraries(example pico_stdlib hardware_spi)
pico_add_extra_outputs(example)

# Enable USB output. Comment out in order to use UART
pico_enable_stdio_usb(example 1)
pico_enable_stdio_uart(example 0)

# Mongoose build flags
add_definitions(-DMG_ENABLE_MIP=1)
