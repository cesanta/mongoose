# FreeRTOS/{FreeRTOS-Kernel V10.4.3,FreeRTOS-Plus-TCP V2.3.2}
PROG = firmware
ARCH ?= stm32f1
ROOT = $(realpath $(CURDIR)/../..)
DOCKER ?= docker run -it --rm -v $(ROOT):$(ROOT) -w $(CURDIR) mdashnet/armgcc
MONGOOSE_OPTS = -DMG_ENABLE_LINES=1 -DMG_ARCH=MG_ARCH_FREERTOS_TCP -DMG_ENABLE_FS=0
ifeq "$(ARCH)" "stm32f1"
MCU = -mcpu=cortex-m3 -mthumb -mfloat-abi=soft
else ifeq "$(ARCH)" "stm32f7"
MCU = -mcpu=cortex-m7 -mthumb -mfpu=fpv5-sp-d16 -mfloat-abi=hard
endif
INCS = -I$(ARCH) -I. -I../.. -Ifreertos-kernel/include -Ifreertos-tcp/include -Ifreertos-tcp/portable/Compiler/GCC
CFLAGS = -W -Wall -Os -g $(MCU) -fdata-sections -ffunction-sections $(INCS) $(MONGOOSE_OPTS) $(EXTRA)
LDFLAGS = $(MCU) -specs=nano.specs -Tobj/link.ld -nostartfiles -nostdlib -lc -lm -lnosys -lgcc #-Wl,-Map=obj/$(PROG).map,--cref -Wl,--gc-sections
SRCS = main.c # $(wildcard freertos-tcp/*.c)
# SRCS += freertos-kernel/portable/MemMang/heap_4.c $(ARCH)/port.c
# SRCS += freertos-kernel/list.c freertos-kernel/tasks.c freertos-kernel/queue.c
OBJS = obj/boot.o $(SRCS:%.c=obj/%.o) #obj/mongoose.o  # ORDER MATTERS - boot (vector table) first!

all: $(PROG).hex

$(PROG).bin: $(PROG).elf
	$(DOCKER) arm-none-eabi-objcopy -O binary $< $@

$(PROG).hex: $(PROG).bin
	$(DOCKER) arm-none-eabi-objcopy -I binary -O ihex --change-address 0x8000000 $< $@

obj/link.ld: $(ARCH)/link.ld
	$(DOCKER) arm-none-eabi-cpp -P -imacros $(ARCH)/device.h $< > $@

$(PROG).elf: $(OBJS) obj/link.ld
	$(DOCKER) arm-none-eabi-gcc $(OBJS) $(LDFLAGS) -o $@

obj/%.o: %.c
	@mkdir -p $(dir $@)
	$(DOCKER) arm-none-eabi-gcc $(CFLAGS) -c $< -o $@

obj/boot.o:
	@mkdir -p $(dir $@)
	$(DOCKER) arm-none-eabi-as --warn --fatal-warnings $(MCU) $(ARCH)/boot.s -o $@

obj/mongoose.o: 
	$(DOCKER) arm-none-eabi-gcc $(CFLAGS) -c ../../mongoose.c -o $@

flash: $(PROG).bin
	st-flash write $< 0x8000000

gdb: $(PROG).elf
	arm-none-eabi-gdb \
		-ex 'set confirm off' \
        -ex 'target extended-remote :4242' \
        -ex 'monitor reset halt' \
        -ex 'monitor reset init' \
		-ex 'b main' \
		-ex 'r' \
        $<

clean:
	@rm -rf *.{bin,elf,map,lst,tgz,zip,hex} obj