# FreeRTOS/{FreeRTOS-Kernel V10.4.3,FreeRTOS-Plus-TCP V2.3.2}
PROG = firmware
ARCH = stm32f7
PROJECT_ROOT_PATH = $(realpath $(CURDIR)/../..)
DOCKER ?= docker run -it --rm -v $(PROJECT_ROOT_PATH):$(PROJECT_ROOT_PATH) -w $(CURDIR) mdashnet/armgcc
FREERTOS_KERNEL_PATH ?= $(PROJECT_ROOT_PATH)/test/freertos-kernel
FREERTOS_PLUS_TCP_PATH ?= $(PROJECT_ROOT_PATH)/test/freertos-tcp

MONGOOSE_FLAGS = -DMG_ARCH=MG_ARCH_FREERTOS_TCP -DMG_ENABLE_FS=1
MCU_FLAGS = -mcpu=cortex-m7 -mthumb -mfloat-abi=softfp -mfpu=vfpv4 
#-mcpu=cortex-m7 -mthumb -mfpu=fpv5-sp-d16 -mfloat-abi=hard

INCLUDES = -I$(PROJECT_ROOT_PATH) -I$(ARCH) -I$(FREERTOS_KERNEL_PATH)/include -I$(FREERTOS_PLUS_TCP_PATH)/include
INCLUDES += -I$(FREERTOS_PLUS_TCP_PATH)/tools/tcp_utilities/include
INCLUDES += -I$(FREERTOS_PLUS_TCP_PATH)/../Utilities/include
#INCLUDES += -IH2
#INCLUDES += -I/Users/lsm/src/htibosch/plus/stm32F7/ST_Library/include
INCLUDES += -IHAL
INCLUDES += -I$(FREERTOS_PLUS_TCP_PATH)/portable/Compiler/GCC
INCLUDES += -I$(FREERTOS_PLUS_TCP_PATH)/portable/NetworkInterface/STM32Fxx
INCLUDES += -I$(FREERTOS_PLUS_TCP_PATH)/portable/NetworkInterface/include
NETFLAGS = -DSTM32F7xx -DSTM32F746xx -Wno-sign-compare -Wno-unused-function #-Wno-cpp

CFLAGS = -g3 -O0 -W -Wall $(MCU_FLAGS)
CFLAGS += $(INCLUDES) $(MONGOOSE_FLAGS) $(NETFLAGS) $(EXTRA)

LINKFLAGS = -T$(ARCH)/link.ld -mcpu=cortex-m7 -mthumb -nostartfiles --specs rdimon.specs -Wl,--gc-sections

SOURCES = main.c $(PROJECT_ROOT_PATH)/mongoose.c
SOURCES += $(wildcard HAL/*.c)
#SOURCES += -I$(realpath $(FREERTOS_KERNEL_PATH)/../../stm32F7/ST_Library/sources)/

# FreeRTOS kernel sources
SOURCES += $(wildcard $(FREERTOS_KERNEL_PATH)/*.c)
SOURCES += $(FREERTOS_KERNEL_PATH)/portable/MemMang/heap_5.c
SOURCES += $(FREERTOS_KERNEL_PATH)/portable/GCC/ARM_CM7/r0p1/port.c

# FreeRTOS TCP stack sources
SOURCES += $(FREERTOS_PLUS_TCP_PATH)/FreeRTOS_ARP.c 
SOURCES += $(FREERTOS_PLUS_TCP_PATH)/FreeRTOS_DHCP.c 
SOURCES += $(FREERTOS_PLUS_TCP_PATH)/FreeRTOS_DNS.c 
SOURCES += $(FREERTOS_PLUS_TCP_PATH)/FreeRTOS_IP.c 
SOURCES += $(FREERTOS_PLUS_TCP_PATH)/FreeRTOS_Sockets.c 
SOURCES += $(FREERTOS_PLUS_TCP_PATH)/FreeRTOS_Stream_Buffer.c 
SOURCES += $(FREERTOS_PLUS_TCP_PATH)/FreeRTOS_TCP_IP.c
SOURCES += $(FREERTOS_PLUS_TCP_PATH)/FreeRTOS_TCP_WIN.c 
SOURCES += $(FREERTOS_PLUS_TCP_PATH)/FreeRTOS_UDP_IP.c 
SOURCES += $(FREERTOS_PLUS_TCP_PATH)/portable/NetworkInterface/Common/phyHandling.c
SOURCES += $(FREERTOS_PLUS_TCP_PATH)/portable/BufferManagement/BufferAllocation_1.c
SOURCES += $(FREERTOS_PLUS_TCP_PATH)/portable/NetworkInterface/STM32Fxx/NetworkInterface.c
SOURCES += $(FREERTOS_PLUS_TCP_PATH)/portable/NetworkInterface/STM32Fxx/stm32fxx_hal_eth.c 

OBJECTS = obj/boot.o $(SOURCES:%.c=obj/%.o)

all: $(PROG).hex

$(PROG).bin: $(PROG).elf
	$(DOCKER) arm-none-eabi-objcopy -O binary $< $@

$(PROG).hex: $(PROG).bin
	$(DOCKER) arm-none-eabi-objcopy -I binary -O ihex --change-address 0x8000000 $< $@

$(PROG).elf: $(OBJECTS) Makefile
	$(DOCKER) arm-none-eabi-gcc $(OBJECTS) $(LINKFLAGS) -o $@
	$(DOCKER) arm-none-eabi-size -A $@

obj/%.o: %.c
	@mkdir -p $(dir $@)
	$(DOCKER) arm-none-eabi-gcc $(CFLAGS) -c $< -o $@

obj/boot.o: $(ARCH)/boot.s
	@mkdir -p $(dir $@)
	$(DOCKER) arm-none-eabi-as -g3 --warn --fatal-warnings $(MCU_FLAGS) $< -o $@

flash: $(PROG).bin
	st-flash --reset write $< 0x8000000

openocd:
	openocd -f openocd.cfg

ELF ?= $(PROG).elf
gdb: #$(PROG).elf
	arm-none-eabi-gdb \
		-ex 'set confirm off' \
        -ex 'target extended-remote :3333' \
        -ex 'monitor arm semihosting enable' \
        -ex 'monitor reset halt' \
        -ex 'load' \
        -ex 'monitor reset init' \
        -ex '$(GDBCMD)' \
        -ex 'r' \
        $(ELF)

clean:
	@rm -rf *.{bin,elf,map,lst,tgz,zip,hex} obj
