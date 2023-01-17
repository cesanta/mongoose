cmake_minimum_required(VERSION 3.13)
include(pico-sdk/pico_sdk_init.cmake)

project(pico_usb_rndis)

# initialize the Pico SDK
pico_sdk_init()

add_compile_options(-Wall
        -Wno-format          # int != int32_t as far as the compiler is concerned because gcc has int32_t as long int
        -Wno-unused-function # we have some for the docs that aren't called
        -Wno-maybe-uninitialized
        )

add_compile_definitions(
	MIP_DEBUG=1 
	MG_ENABLE_PACKED_FS=1
	MG_ENABLE_FILE=0
	MG_ENABLE_MIP=1
	DISABLE_ROUTING
	)

if (TARGET tinyusb_device)

add_executable(firmware
	main.c
	mongoose.c
	net.c
	packed_fs.c
	usb_descriptors.c
	pico-sdk/lib/tinyusb/lib/networking/rndis_reports.c
	)

# Make sure TinyUSB can find tusb_config.h and workaround SDK include paths
target_include_directories(firmware PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}
	${CMAKE_CURRENT_LIST_DIR}/pico-sdk/lib/tinyusb/lib/networking
	)

# enable uart output
pico_enable_stdio_usb(firmware 0)
pico_enable_stdio_uart(firmware 1)

else()
    message(WARNING "not building because TinyUSB submodule is not initialized in the SDK")
endif()

target_link_libraries(firmware PUBLIC
	pico_stdlib
	tinyusb_device
	tinyusb_board
	)

# create map/bin/hex file etc.
pico_add_extra_outputs(firmware)
