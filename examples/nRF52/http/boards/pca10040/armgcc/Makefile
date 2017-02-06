PROJECT_NAME := iot_lwip_tcp_server_pca10040

export OUTPUT_FILENAME
#MAKEFILE_NAME := $(CURDIR)/$(word $(words $(MAKEFILE_LIST)),$(MAKEFILE_LIST))
MAKEFILE_NAME := $(MAKEFILE_LIST)
MAKEFILE_DIR := $(dir $(MAKEFILE_NAME) ) 

TEMPLATE_PATH = ../../../../nrf5_iot_sdk/components/toolchain/gcc
ifeq ($(OS),Windows_NT)
include $(TEMPLATE_PATH)/Makefile.windows
else
include $(TEMPLATE_PATH)/Makefile.posix
endif

MK := mkdir
RM := rm -rf

#echo suspend
ifeq ("$(VERBOSE)","1")
NO_ECHO := 
else
NO_ECHO := @
endif

# Toolchain commands
CC              := '$(GNU_INSTALL_ROOT)/bin/$(GNU_PREFIX)-gcc'
AS              := '$(GNU_INSTALL_ROOT)/bin/$(GNU_PREFIX)-as'
AR              := '$(GNU_INSTALL_ROOT)/bin/$(GNU_PREFIX)-ar' -r
LD              := '$(GNU_INSTALL_ROOT)/bin/$(GNU_PREFIX)-ld'
NM              := '$(GNU_INSTALL_ROOT)/bin/$(GNU_PREFIX)-nm'
OBJDUMP         := '$(GNU_INSTALL_ROOT)/bin/$(GNU_PREFIX)-objdump'
OBJCOPY         := '$(GNU_INSTALL_ROOT)/bin/$(GNU_PREFIX)-objcopy'
SIZE            := '$(GNU_INSTALL_ROOT)/bin/$(GNU_PREFIX)-size'

#function for removing duplicates in a list
remduplicates = $(strip $(if $1,$(firstword $1) $(call remduplicates,$(filter-out $(firstword $1),$1))))

#source common to all targets
C_SOURCE_FILES_SDK = \
$(abspath ../../../../nrf5_iot_sdk/components/libraries/button/app_button.c) \
$(abspath ../../../../nrf5_iot_sdk/components/libraries/util/app_error.c) \
$(abspath ../../../../nrf5_iot_sdk/components/libraries/fifo/app_fifo.c) \
$(abspath ../../../../nrf5_iot_sdk/components/libraries/scheduler/app_scheduler.c) \
$(abspath ../../../../nrf5_iot_sdk/components/libraries/timer/app_timer.c) \
$(abspath ../../../../nrf5_iot_sdk/components/libraries/timer/app_timer_appsh.c) \
$(abspath ../../../../nrf5_iot_sdk/components/libraries/trace/app_trace.c) \
$(abspath ../../../../nrf5_iot_sdk/components/libraries/mem_manager/mem_manager.c) \
$(abspath ../../../../nrf5_iot_sdk/components/libraries/util/nrf_assert.c) \
$(abspath ../../../../nrf5_iot_sdk/components/libraries/uart/retarget.c) \
$(abspath ../../../../nrf5_iot_sdk/external/lwip/src/core/def.c) \
$(abspath ../../../../nrf5_iot_sdk/external/lwip/src/core/dhcp.c) \
$(abspath ../../../../nrf5_iot_sdk/external/lwip/src/core/ipv6/dhcp6.c) \
$(abspath ../../../../nrf5_iot_sdk/external/lwip/src/core/dns.c) \
$(abspath ../../../../nrf5_iot_sdk/external/lwip/src/core/ipv4/icmp.c) \
$(abspath ../../../../nrf5_iot_sdk/external/lwip/src/core/ipv6/icmp6.c) \
$(abspath ../../../../nrf5_iot_sdk/external/lwip/src/core/ipv6/inet6.c) \
$(abspath ../../../../nrf5_iot_sdk/external/lwip/src/core/inet_chksum.c) \
$(abspath ../../../../nrf5_iot_sdk/external/lwip/src/core/init.c) \
$(abspath ../../../../nrf5_iot_sdk/external/lwip/src/core/ipv4/ip4.c) \
$(abspath ../../../../nrf5_iot_sdk/external/lwip/src/core/ipv4/ip4_addr.c) \
$(abspath ../../../../nrf5_iot_sdk/external/lwip/src/core/ipv6/ip6.c) \
$(abspath ../../../../nrf5_iot_sdk/external/lwip/src/core/ipv6/ip6_addr.c) \
$(abspath ../../../../nrf5_iot_sdk/external/lwip/src/core/memp.c) \
$(abspath ../../../../nrf5_iot_sdk/external/lwip/src/core/ipv6/mld6.c) \
$(abspath ../../../../nrf5_iot_sdk/external/lwip/src/core/ipv6/nd6.c) \
$(abspath ../../../../nrf5_iot_sdk/external/lwip/src/core/netif.c) \
$(abspath ../../../../nrf5_iot_sdk/external/lwip/src/port/nrf_platform_port.c) \
$(abspath ../../../../nrf5_iot_sdk/external/lwip/src/core/pbuf.c) \
$(abspath ../../../../nrf5_iot_sdk/external/lwip/src/core/raw.c) \
$(abspath ../../../../nrf5_iot_sdk/external/lwip/src/core/sys.c) \
$(abspath ../../../../nrf5_iot_sdk/external/lwip/src/core/tcp.c) \
$(abspath ../../../../nrf5_iot_sdk/external/lwip/src/core/tcp_in.c) \
$(abspath ../../../../nrf5_iot_sdk/external/lwip/src/core/tcp_out.c) \
$(abspath ../../../../nrf5_iot_sdk/external/lwip/src/core/timers.c) \
$(abspath ../../../../nrf5_iot_sdk/external/lwip/src/core/udp.c) \
$(abspath ../../../../nrf5_iot_sdk/components/libraries/uart/app_uart_fifo.c) \
$(abspath ../../../../nrf5_iot_sdk/components/drivers_nrf/delay/nrf_delay.c) \
$(abspath ../../../../nrf5_iot_sdk/components/drivers_nrf/common/nrf_drv_common.c) \
$(abspath ../../../../nrf5_iot_sdk/components/drivers_nrf/gpiote/nrf_drv_gpiote.c) \
$(abspath ../../../../nrf5_iot_sdk/components/drivers_nrf/uart/nrf_drv_uart.c) \
$(abspath ../../../../nrf5_iot_sdk/components/drivers_nrf/pstorage/pstorage.c) \
$(abspath ../../../../nrf5_iot_sdk/examples/bsp/bsp.c) \
$(abspath ../../../../nrf5_iot_sdk/components/ble/common/ble_advdata.c) \
$(abspath ../../../../nrf5_iot_sdk/components/ble/common/ble_conn_params.c) \
$(abspath ../../../../nrf5_iot_sdk/components/ble/common/ble_srv_common.c) \
$(abspath ../../../../nrf5_iot_sdk/components/iot/errno/errno.c) \
$(abspath ../../../../nrf5_iot_sdk/components/iot/context_manager/iot_context_manager.c) \
$(abspath ../../../../nrf5_iot_sdk/components/iot/iot_timer/iot_timer.c) \
$(abspath ../../../../nrf5_iot_sdk/components/iot/medium/ipv6_medium_ble.c) \
$(abspath ../../../../nrf5_iot_sdk/components/iot/common/ipv6_parse.c) \
$(abspath ../../../../nrf5_iot_sdk/components/toolchain/system_nrf52.c) \
$(abspath ../../../../nrf5_iot_sdk/components/iot/medium/ble_ncfgs/ble_ncfgs.c) \
$(abspath ../../../../nrf5_iot_sdk/components/iot/medium/commissioning/commissioning.c) \
$(abspath ../../../../nrf5_iot_sdk/components/softdevice/common/softdevice_handler/softdevice_handler.c) \
$(abspath ../../../../nrf5_iot_sdk/components/softdevice/common/softdevice_handler/softdevice_handler_appsh.c) \

C_SOURCE_FILES_APP = \
$(abspath ../../../main.c) \
$(abspath ../../../bleconfig.c) \
$(abspath ../../../../../../mongoose.c) \
$(abspath ../../../rtt/RTT/SEGGER_RTT.c) \
$(abspath ../../../rtt/RTT/SEGGER_RTT_printf.c) \
$(abspath ../../../rtt/Syscalls/RTT_Syscalls_GCC.c) \

C_SOURCE_FILES = $(C_SOURCE_FILES_APP) $(C_SOURCE_FILES_SDK)

#assembly files common to all targets
ASM_SOURCE_FILES  = $(abspath ../../../../nrf5_iot_sdk/components/toolchain/gcc/gcc_startup_nrf52.s)

#assembly files common to all targets
LIBS  = $(abspath ../../../../nrf5_iot_sdk/components/iot/ble_6lowpan/lib/ble_6lowpan.a)

#includes common to all targets
INC_PATHS  = -I$(abspath ../../../config)
INC_PATHS += -I$(abspath ../../../../nrf5_iot_sdk/components/iot/errno)
INC_PATHS += -I$(abspath ../../../../nrf5_iot_sdk/components/softdevice/s1xx_iot/headers)
INC_PATHS += -I$(abspath ../../../../nrf5_iot_sdk/components/drivers_nrf/common)
INC_PATHS += -I$(abspath ../../../../nrf5_iot_sdk/components/softdevice/common/softdevice_handler)
INC_PATHS += -I$(abspath ../../../../nrf5_iot_sdk/external/lwip/src/include)
INC_PATHS += -I$(abspath ../../../../nrf5_iot_sdk/components/iot/ble_6lowpan)
INC_PATHS += -I$(abspath ../../../../nrf5_iot_sdk/components/drivers_nrf/hal)
INC_PATHS += -I$(abspath ../../../../nrf5_iot_sdk/components/ble/common)
INC_PATHS += -I$(abspath ../../../../nrf5_iot_sdk/external/lwip/src/port)
INC_PATHS += -I$(abspath ../../../../nrf5_iot_sdk/components/libraries/fifo)
INC_PATHS += -I$(abspath ../../../../nrf5_iot_sdk/components/libraries/trace)
INC_PATHS += -I$(abspath ../../../../nrf5_iot_sdk/components/device)
INC_PATHS += -I$(abspath ../../../../nrf5_iot_sdk/components/libraries/uart)
INC_PATHS += -I$(abspath ../../../../nrf5_iot_sdk/components/iot/medium)
INC_PATHS += -I$(abspath ../../../../nrf5_iot_sdk/components/drivers_nrf/config)
INC_PATHS += -I$(abspath ../../../../nrf5_iot_sdk/components/libraries/mem_manager)
INC_PATHS += -I$(abspath ../../../../nrf5_iot_sdk/components/drivers_nrf/uart)
INC_PATHS += -I$(abspath ../../../../nrf5_iot_sdk/components/iot/ble_ipsp)
INC_PATHS += -I$(abspath ../../../../nrf5_iot_sdk/components/libraries/scheduler)
INC_PATHS += -I$(abspath ../../../../nrf5_iot_sdk/components/iot/iot_timer)
INC_PATHS += -I$(abspath ../../../../nrf5_iot_sdk/external/lwip/src/include/netif)
INC_PATHS += -I$(abspath ../../../../nrf5_iot_sdk/components/iot/medium/commissioning)
INC_PATHS += -I$(abspath ../../../../nrf5_iot_sdk/components/iot/include)
INC_PATHS += -I$(abspath ../../../../nrf5_iot_sdk/components/drivers_nrf/pstorage)
INC_PATHS += -I$(abspath ../../..)
INC_PATHS += -I$(abspath ../../../../../..)
INC_PATHS += -I$(abspath ../../../rtt/RTT)
INC_PATHS += -I$(abspath ../../../../nrf5_iot_sdk/components/iot/context_manager)
INC_PATHS += -I$(abspath ../../../../nrf5_iot_sdk/components/toolchain/gcc)
INC_PATHS += -I$(abspath ../../../../nrf5_iot_sdk/components/drivers_nrf/delay)
INC_PATHS += -I$(abspath ../../../../nrf5_iot_sdk/components/iot/medium/ble_ncfgs)
INC_PATHS += -I$(abspath ../../../../nrf5_iot_sdk/components/drivers_nrf/gpiote)
INC_PATHS += -I$(abspath ../../../../nrf5_iot_sdk/components/softdevice/s1xx_iot/headers/nrf52)
INC_PATHS += -I$(abspath ../../../../nrf5_iot_sdk/external/lwip/src/include/lwip)
INC_PATHS += -I$(abspath ../../../../nrf5_iot_sdk/components/iot/common)
INC_PATHS += -I$(abspath ../../../../nrf5_iot_sdk/components/libraries/timer)
INC_PATHS += -I$(abspath ../../../../nrf5_iot_sdk/external/lwip/src/port/arch)
INC_PATHS += -I$(abspath ../../../../nrf5_iot_sdk/components/libraries/button)
INC_PATHS += -I$(abspath ../../../../nrf5_iot_sdk/components/libraries/util)
INC_PATHS += -I$(abspath ../../../../nrf5_iot_sdk/examples/bsp)
INC_PATHS += -I$(abspath ../../../../nrf5_iot_sdk/components/toolchain)
INC_PATHS += -I$(abspath ../../../../nrf5_iot_sdk/components/iot/medium/include)

OBJECT_DIRECTORY = _build
LISTING_DIRECTORY = $(OBJECT_DIRECTORY)
OUTPUT_BINARY_DIRECTORY = $(OBJECT_DIRECTORY)

# Sorting removes duplicates
BUILD_DIRECTORIES := $(sort $(OBJECT_DIRECTORY) $(OUTPUT_BINARY_DIRECTORY) $(LISTING_DIRECTORY) )

# Mongoose features
MG_FEATURES_TINY = \
                   -DMG_DISABLE_HTTP_DIGEST_AUTH \
                   -DMG_DISABLE_MD5 \
                   -DMG_DISABLE_HTTP_KEEP_ALIVE \
                   -DMG_ENABLE_HTTP_SSI=0 \
                   -DMG_ENABLE_HTTP_STREAMING_MULTIPART

#flags common to all targets
CFLAGS  = -D__HEAP_SIZE=4096
#CFLAGS += -DCOMMISSIONING_ENABLED
CFLAGS += -DSWI_DISABLE0
CFLAGS += -DSOFTDEVICE_PRESENT
CFLAGS += -DBOARD_PCA10040
CFLAGS += -DNRF52
CFLAGS += -DCONFIG_GPIO_AS_PINRESET
CFLAGS += -DBLE_STACK_SUPPORT_REQD
CFLAGS += -Ds1xx
CFLAGS += -mcpu=cortex-m4
CFLAGS += -mthumb -mabi=aapcs --std=gnu99
CFLAGS += -g3 -O3
CFLAGS += -mfloat-abi=hard -mfpu=fpv4-sp-d16
# keep every function in separate section. This will allow linker to dump unused functions
CFLAGS += -ffunction-sections -fdata-sections -fno-strict-aliasing
CFLAGS += -fno-builtin --short-enums

CFLAGS += -DCS_ENABLE_STDIO

# This macro is needed for stdout to be retargeted to either UART or RTT
CFLAGS += -DENABLE_DEBUG_LOG_SUPPORT

# Don't retarget stdout to UART; it'll be retargeted to RTT
CFLAGS += -DNRF_LOG_USES_RTT

# Mongoose flags
CFLAGS += -DCS_PLATFORM=CS_P_NRF52 $(MG_FEATURES_TINY) -DMG_MODULE_LINES

# keep every function in separate section. This will allow linker to dump unused functions
LDFLAGS += -Xlinker -Map=$(LISTING_DIRECTORY)/$(OUTPUT_FILENAME).map
LDFLAGS += -mthumb -mabi=aapcs -L $(TEMPLATE_PATH) -T$(LINKER_SCRIPT)
LDFLAGS += -mcpu=cortex-m4
LDFLAGS += -mfloat-abi=hard -mfpu=fpv4-sp-d16
# let linker to dump unused sections
LDFLAGS += -Wl,--gc-sections
# use newlib in nano version
LDFLAGS += --specs=nano.specs -lc -lnosys

# Assembler flags
ASMFLAGS += -x assembler-with-cpp
ASMFLAGS += -D__HEAP_SIZE=4096
ASMFLAGS += -DSWI_DISABLE0
ASMFLAGS += -DSOFTDEVICE_PRESENT
ASMFLAGS += -DBOARD_PCA10040
ASMFLAGS += -DNRF52
ASMFLAGS += -DCONFIG_GPIO_AS_PINRESET
ASMFLAGS += -DENABLE_DEBUG_LOG_SUPPORT
ASMFLAGS += -DBLE_STACK_SUPPORT_REQD
ASMFLAGS += -Ds1xx
ASMFLAGS += -g3

#building all targets
all: nrf52832_xxaa_s1xx_iot

#target for printing all targets
help:
	@echo following targets are available:
	@echo 	nrf52832_xxaa_s1xx_iot

C_SOURCE_FILE_NAMES = $(notdir $(C_SOURCE_FILES))
C_PATHS = $(call remduplicates, $(dir $(C_SOURCE_FILES) ) )
C_OBJECTS = $(addprefix $(OBJECT_DIRECTORY)/, $(C_SOURCE_FILE_NAMES:.c=.o) )

C_SOURCE_FILE_NAMES_SDK = $(notdir $(C_SOURCE_FILES_SDK))
C_SOURCE_FILE_NAMES_APP = $(notdir $(C_SOURCE_FILES_APP))
C_OBJECTS_SDK = $(addprefix $(OBJECT_DIRECTORY)/, $(C_SOURCE_FILE_NAMES_SDK:.c=.o) )
C_OBJECTS_APP = $(addprefix $(OBJECT_DIRECTORY)/, $(C_SOURCE_FILE_NAMES_APP:.c=.o) )

# Disable all warnings for SDK sources since we have no control of them
$(C_OBJECTS_SDK): CFLAGS += -w

# Enable all warnings for the application's files
$(C_OBJECTS_APP): CFLAGS += -Wall

ASM_SOURCE_FILE_NAMES = $(notdir $(ASM_SOURCE_FILES))
ASM_PATHS = $(call remduplicates, $(dir $(ASM_SOURCE_FILES) ))
ASM_OBJECTS = $(addprefix $(OBJECT_DIRECTORY)/, $(ASM_SOURCE_FILE_NAMES:.s=.o) )

vpath %.c $(C_PATHS)
vpath %.s $(ASM_PATHS)

OBJECTS = $(C_OBJECTS) $(ASM_OBJECTS)

nrf52832_xxaa_s1xx_iot: OUTPUT_FILENAME := nrf52832_xxaa_s1xx_iot
nrf52832_xxaa_s1xx_iot: LINKER_SCRIPT=iot_lwip_tcp_server_gcc_nrf52.ld

nrf52832_xxaa_s1xx_iot: $(BUILD_DIRECTORIES) $(OBJECTS)
	@echo Linking target: $(OUTPUT_FILENAME).out
	$(NO_ECHO)$(CC) $(LDFLAGS) $(OBJECTS) $(LIBS) -o $(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_FILENAME).out
	$(NO_ECHO)$(MAKE) -f $(MAKEFILE_NAME) -C $(MAKEFILE_DIR) -e finalize

## Create build directories
$(BUILD_DIRECTORIES):
	echo $(MAKEFILE_NAME)
	$(MK) $@

# Create objects from C SRC files
$(OBJECT_DIRECTORY)/%.o: %.c
	@echo Compiling file: $(notdir $<)
	$(NO_ECHO)$(CC) $(CFLAGS) $(INC_PATHS) -c -o $@ $<

# Assemble files
$(OBJECT_DIRECTORY)/%.o: %.s
	@echo Assembly file: $(notdir $<)
	$(NO_ECHO)$(CC) $(ASMFLAGS) $(INC_PATHS) -c -o $@ $<
# Link
$(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_FILENAME).out: $(BUILD_DIRECTORIES) $(OBJECTS)
	@echo Linking target: $(OUTPUT_FILENAME).out
	$(NO_ECHO)$(CC) $(LDFLAGS) $(OBJECTS) $(LIBS) -o $(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_FILENAME).out
## Create binary .bin file from the .out file
$(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_FILENAME).bin: $(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_FILENAME).out
	@echo Preparing: $(OUTPUT_FILENAME).bin
	$(NO_ECHO)$(OBJCOPY) -O binary $(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_FILENAME).out $(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_FILENAME).bin

## Create binary .hex file from the .out file
$(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_FILENAME).hex: $(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_FILENAME).out
	@echo Preparing: $(OUTPUT_FILENAME).hex
	$(NO_ECHO)$(OBJCOPY) -O ihex $(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_FILENAME).out $(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_FILENAME).hex

finalize: genbin genhex echosize

genbin:
	@echo Preparing: $(OUTPUT_FILENAME).bin
	$(NO_ECHO)$(OBJCOPY) -O binary $(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_FILENAME).out $(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_FILENAME).bin

## Create binary .hex file from the .out file
genhex: 
	@echo Preparing: $(OUTPUT_FILENAME).hex
	$(NO_ECHO)$(OBJCOPY) -O ihex $(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_FILENAME).out $(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_FILENAME).hex
echosize:
	-@echo ''
	$(NO_ECHO)$(SIZE) $(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_FILENAME).out
	-@echo ''

clean:
	$(RM) $(BUILD_DIRECTORIES)

cleanobj:
	$(RM) $(BUILD_DIRECTORIES)/*.o
flash: $(MAKECMDGOALS)
	@echo Flashing: $(OUTPUT_BINARY_DIRECTORY)/nrf52832_xxaa_s1xx_iot.hex
	nrfjprog --program $(OUTPUT_BINARY_DIRECTORY)/nrf52832_xxaa_s1xx_iot.hex -f nrf52 --sectorerase
	nrfjprog --reset -f nrf52

## Flash softdevice
