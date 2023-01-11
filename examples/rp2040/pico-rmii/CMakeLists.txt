cmake_minimum_required(VERSION 3.13)
include(pico-sdk/pico_sdk_init.cmake)

project(pico_rmii_ethernet)

# initialize the Pico SDK
pico_sdk_init()

add_compile_definitions(
	MIP_DEBUG=1 
	MG_ENABLE_PACKED_FS=1
	MG_ENABLE_MIP=1
)

add_executable(firmware
        driver_rp2040_rmii.c
	main.c
	mongoose.c
        )

# enable usb output, disable uart output
pico_enable_stdio_usb(firmware 1)
pico_enable_stdio_uart(firmware 0)

target_link_libraries(firmware hardware_pio hardware_dma pico_stdlib)

# create map/bin/hex file etc.
pico_add_extra_outputs(firmware)
