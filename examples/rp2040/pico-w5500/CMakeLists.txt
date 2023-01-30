cmake_minimum_required(VERSION 3.13)
include(pico-sdk/pico_sdk_init.cmake)

project(firmware)
pico_sdk_init()

file(GLOB TLS_SOURCES "mbedtls/library/*.c")
add_executable(firmware
  main.c
  ../../../mongoose.c
  ../../device-dashboard/net.c
  ../../device-dashboard/packed_fs.c
  ${TLS_SOURCES})

target_include_directories(firmware PUBLIC . ../../.. mbedtls/include)
target_link_libraries(firmware pico_stdlib hardware_spi)
pico_add_extra_outputs(firmware)

# Enable USB output. Comment out in order to use UART
pico_enable_stdio_usb(firmware 0)
pico_enable_stdio_uart(firmware 1)

# Mongoose build flags
add_definitions(-DMG_ENABLE_MIP=1)
add_definitions(-DMG_ENABLE_PACKED_FS=1)
add_definitions(-DMG_ENABLE_FILE=0)

add_definitions(-DMG_ENABLE_MBEDTLS=1)
add_definitions(-DMBEDTLS_CONFIG_FILE=\"tlsconf.h\")
#add_definitions(-DMG_MBEDTLS_DEBUG_LEVEL=1)
