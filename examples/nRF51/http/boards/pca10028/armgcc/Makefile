PROJECT_NAME := iot_lwip_tcp_server_pca10028

export OUTPUT_FILENAME
#MAKEFILE_NAME := $(CURDIR)/$(word $(words $(MAKEFILE_LIST)),$(MAKEFILE_LIST))
MAKEFILE_NAME := $(MAKEFILE_LIST)
MAKEFILE_DIR := $(dir $(MAKEFILE_NAME) ) 

TEMPLATE_PATH = ../../../../nrf51_iot_sdk/components/toolchain/gcc
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
CC       		:= "$(GNU_INSTALL_ROOT)/bin/$(GNU_PREFIX)-gcc"
AS       		:= "$(GNU_INSTALL_ROOT)/bin/$(GNU_PREFIX)-as"
AR       		:= "$(GNU_INSTALL_ROOT)/bin/$(GNU_PREFIX)-ar" -r
LD       		:= "$(GNU_INSTALL_ROOT)/bin/$(GNU_PREFIX)-ld"
NM       		:= "$(GNU_INSTALL_ROOT)/bin/$(GNU_PREFIX)-nm"
OBJDUMP  		:= "$(GNU_INSTALL_ROOT)/bin/$(GNU_PREFIX)-objdump"
OBJCOPY  		:= "$(GNU_INSTALL_ROOT)/bin/$(GNU_PREFIX)-objcopy"
SIZE    		:= "$(GNU_INSTALL_ROOT)/bin/$(GNU_PREFIX)-size"

#function for removing duplicates in a list
remduplicates = $(strip $(if $1,$(firstword $1) $(call remduplicates,$(filter-out $(firstword $1),$1))))

#source common to all targets
C_SOURCE_FILES_SDK = \
../../../../nrf51_iot_sdk/components/libraries/button/app_button.c \
../../../../nrf51_iot_sdk/components/libraries/util/app_error.c \
../../../../nrf51_iot_sdk/components/libraries/fifo/app_fifo.c \
../../../../nrf51_iot_sdk/components/libraries/scheduler/app_scheduler.c \
../../../../nrf51_iot_sdk/components/libraries/timer/app_timer.c \
../../../../nrf51_iot_sdk/components/libraries/timer/app_timer_appsh.c \
../../../../nrf51_iot_sdk/components/libraries/trace/app_trace.c \
../../../../nrf51_iot_sdk/components/libraries/mem_manager/mem_manager.c \
../../../../nrf51_iot_sdk/components/libraries/util/nrf_assert.c \
../../../../nrf51_iot_sdk/external/lwip/src/port/nrf_platform_port.c \
../../../../nrf51_iot_sdk/components/drivers_nrf/uart/app_uart_fifo.c \
../../../../nrf51_iot_sdk/components/drivers_nrf/hal/nrf_delay.c \
../../../../nrf51_iot_sdk/components/drivers_nrf/common/nrf_drv_common.c \
../../../../nrf51_iot_sdk/components/drivers_nrf/gpiote/nrf_drv_gpiote.c \
../../../../nrf51_iot_sdk/components/ble/common/ble_advdata.c \
../../../../nrf51_iot_sdk/components/ble/common/ble_srv_common.c \
../../../../nrf51_iot_sdk/components/iot/context_manager/iot_context_manager.c \
../../../../nrf51_iot_sdk/components/toolchain/system_nrf51.c \
../../../../nrf51_iot_sdk/components/softdevice/common/softdevice_handler/softdevice_handler.c \
../../../../nrf51_iot_sdk/components/softdevice/common/softdevice_handler/softdevice_handler_appsh.c \

C_SOURCE_FILES_APP = \
../../../main.c \
../../../bleconfig.c \
../../../rtt/RTT/SEGGER_RTT.c \
../../../rtt/RTT/SEGGER_RTT_printf.c \
../../../rtt/Syscalls/RTT_Syscalls_GCC.c \
../../../../../../mongoose.c \

C_SOURCE_FILES = $(C_SOURCE_FILES_SDK) $(C_SOURCE_FILES_APP)

#assembly files common to all targets
ASM_SOURCE_FILES  = ../../../../nrf51_iot_sdk/components/toolchain/gcc/gcc_startup_nrf51.s

#assembly files common to all targets
LIBS  = ../../../../nrf51_iot_sdk/external/lwip/lib/liblwip.a
LIBS += ../../../../nrf51_iot_sdk/components/iot/ble_6lowpan/lib/ble_6lowpan.a

#includes common to all targets
INC_PATHS  = -I../../../config
INC_PATHS += -I../../../../nrf51_iot_sdk/components/softdevice/s1xx_iot/headers
INC_PATHS += -I../../../../nrf51_iot_sdk/components/drivers_nrf/common
INC_PATHS += -I../../../../nrf51_iot_sdk/components/softdevice/common/softdevice_handler
INC_PATHS += -I../../../../nrf51_iot_sdk/external/lwip/src/include
INC_PATHS += -I../../../../nrf51_iot_sdk/components/iot/ble_6lowpan
INC_PATHS += -I../../../../nrf51_iot_sdk/components/drivers_nrf/hal
INC_PATHS += -I../../../../nrf51_iot_sdk/components/ble/common
INC_PATHS += -I../../../../nrf51_iot_sdk/external/lwip/src/port
INC_PATHS += -I../../../../nrf51_iot_sdk/components/libraries/fifo
INC_PATHS += -I../../../../nrf51_iot_sdk/components/libraries/trace
INC_PATHS += -I../../../../nrf51_iot_sdk/components/device
INC_PATHS += -I../../../../nrf51_iot_sdk/components/drivers_nrf/config
INC_PATHS += -I../../../../nrf51_iot_sdk/components/libraries/mem_manager
INC_PATHS += -I../../../../nrf51_iot_sdk/components/drivers_nrf/uart
INC_PATHS += -I../../../../nrf51_iot_sdk/components/iot/ble_ipsp
INC_PATHS += -I../../../../nrf51_iot_sdk/components/libraries/scheduler
INC_PATHS += -I../../../../nrf51_iot_sdk/external/lwip/src/include/netif
INC_PATHS += -I../../../../nrf51_iot_sdk/components/iot/include
INC_PATHS += -I../../..
INC_PATHS += -I../../../../../..
INC_PATHS += -I../../../rtt/RTT
INC_PATHS += -I../../../../nrf51_iot_sdk/components/iot/context_manager
INC_PATHS += -I../../../../nrf51_iot_sdk/components/toolchain/gcc
INC_PATHS += -I../../../../nrf51_iot_sdk/components/drivers_nrf/gpiote
INC_PATHS += -I../../../../nrf51_iot_sdk/components/libraries/timer
INC_PATHS += -I../../../../nrf51_iot_sdk/external/lwip/src/port/arch
INC_PATHS += -I../../../../nrf51_iot_sdk/components/libraries/button
INC_PATHS += -I../../../../nrf51_iot_sdk/components/libraries/util
INC_PATHS += -I../../../../nrf51_iot_sdk/examples/bsp
INC_PATHS += -I../../../../nrf51_iot_sdk/components/toolchain

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
CFLAGS  = -D__HEAP_SIZE=512
CFLAGS += -DSWI_DISABLE0
CFLAGS += -DBOARD_PCA10028
CFLAGS += -DSOFTDEVICE_PRESENT
CFLAGS += -DNRF51
CFLAGS += -DS110
CFLAGS += -DBLE_STACK_SUPPORT_REQD
CFLAGS += -DBSP_DEFINES_ONLY
CFLAGS += -mcpu=cortex-m0
CFLAGS += -mthumb -mabi=aapcs --std=gnu99
CFLAGS += -g3 -O3
CFLAGS += -mfloat-abi=soft
# keep every function in separate section. This will allow linker to dump unused functions
CFLAGS += -ffunction-sections -fdata-sections -fno-strict-aliasing
CFLAGS += -fno-builtin --short-enums

CFLAGS += -DCS_ENABLE_STDIO

# This macro is needed for stdout to be retargeted to either UART or RTT
CFLAGS += -DENABLE_DEBUG_LOG_SUPPORT

# Mongoose flags
CFLAGS += -DCS_PLATFORM=CS_P_NRF51 $(MG_FEATURES_TINY) -DMG_MODULE_LINES

# keep every function in separate section. This will allow linker to dump unused functions
LDFLAGS += -Xlinker -Map=$(LISTING_DIRECTORY)/$(OUTPUT_FILENAME).map
LDFLAGS += -mthumb -mabi=aapcs -L $(TEMPLATE_PATH) -T$(LINKER_SCRIPT)
LDFLAGS += -mcpu=cortex-m0
# let linker to dump unused sections
LDFLAGS += -Wl,--gc-sections
# use newlib in nano version
LDFLAGS += --specs=nano.specs -lc -lnosys

# Assembler flags
ASMFLAGS += -x assembler-with-cpp
ASMFLAGS += -D__HEAP_SIZE=512
ASMFLAGS += -DSWI_DISABLE0
ASMFLAGS += -DBOARD_PCA10028
ASMFLAGS += -DSOFTDEVICE_PRESENT
ASMFLAGS += -DNRF51
ASMFLAGS += -DS110
ASMFLAGS += -DBLE_STACK_SUPPORT_REQD
ASMFLAGS += -DBSP_DEFINES_ONLY
#default target - first one defined
default: clean nrf51422_xxac_s1xx_iot

#building all targets
all: clean
	$(NO_ECHO)$(MAKE) -f $(MAKEFILE_NAME) -C $(MAKEFILE_DIR) -e cleanobj
	$(NO_ECHO)$(MAKE) -f $(MAKEFILE_NAME) -C $(MAKEFILE_DIR) -e nrf51422_xxac_s1xx_iot

#target for printing all targets
help:
	@echo following targets are available:
	@echo 	nrf51422_xxac_s1xx_iot


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

nrf51422_xxac_s1xx_iot: OUTPUT_FILENAME := nrf51422_xxac_s1xx_iot
nrf51422_xxac_s1xx_iot: LINKER_SCRIPT=iot_lwip_tcp_server_gcc_nrf51.ld
nrf51422_xxac_s1xx_iot: $(BUILD_DIRECTORIES) $(OBJECTS)
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
	@echo Compiling file: $(notdir $<)
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
	-@echo ""
	$(NO_ECHO)$(SIZE) $(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_FILENAME).out
	-@echo ""

clean:
	$(RM) $(BUILD_DIRECTORIES)

cleanobj:
	$(RM) $(BUILD_DIRECTORIES)/*.o

flash: $(MAKECMDGOALS)
	@echo Flashing: $(OUTPUT_BINARY_DIRECTORY)/$<.hex
	nrfjprog --reset --program $(OUTPUT_BINARY_DIRECTORY)/$<.hex

## Flash softdevice
