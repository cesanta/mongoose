cmake_minimum_required(VERSION 3.13)

# If you will set board type here, do it BEFORE including the SDK
# set(PICO_BOARD pico2)
include(pico_sdk_import.cmake)

project(firmware)
pico_sdk_init()

add_executable(firmware
  main.c
  msc_disk.c
  usb_descriptors.c
  mongoose/mongoose.c
  pico-sdk/lib/tinyusb/lib/networking/rndis_reports.c)

target_include_directories(firmware PRIVATE
  .
  mongoose
  pico-sdk/lib/tinyusb/lib/networking)

target_link_libraries(firmware pico_stdlib pico_rand hardware_spi tinyusb_device)
pico_add_extra_outputs(firmware)     # create map/bin/hex file etc.

pico_enable_stdio_usb(firmware 0)    # Route stdio
pico_enable_stdio_uart(firmware 1)   # to the UART

# Extra build flags (enable if needed)
add_definitions(-DDUAL_CONFIG=0)

# Prevent extreme optimization from breaking OTA
target_compile_options(firmware PRIVATE -fno-tree-loop-distribute-patterns)
