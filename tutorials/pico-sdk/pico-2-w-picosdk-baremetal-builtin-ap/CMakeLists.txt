cmake_minimum_required(VERSION 3.13)
include(pico_sdk_import.cmake)

project(firmware)
pico_sdk_init()

add_executable(firmware
	main.c
	mongoose.c
)

target_include_directories(firmware PUBLIC
  .
)

target_link_libraries(firmware pico_stdlib pico_rand pico_cyw43_driver pico_cyw43_arch_none)
pico_add_extra_outputs(firmware)     # create map/bin/hex file etc.

pico_enable_stdio_usb(firmware 1)    # Route stdio

add_definitions(-DMG_ENABLE_TCPIP_DRIVER_INIT=0)
