# FreeRTOS/{FreeRTOS-Kernel V10.4.3,FreeRTOS-Plus-TCP V2.3.2}
PROG = firmware
ARCH ?= stm32f1
ROOT = $(realpath $(CURDIR)/../..)
ENV ?= docker run -it --rm -v $(ROOT):$(ROOT) -w $(CURDIR) mdashnet/armgcc
MFLAGS = -DMG_ENABLE_LINES=1 -DMG_ARCH=MG_ARCH_FREERTOS_TCP -DMG_ENABLE_FS=0
ifeq "$(ARCH)" "stm32f1"
MCU = -mcpu=cortex-m3 -mthumb -mfloat-abi=soft
else ifeq "$(ARCH)" "stm32f7"
MCU = -mcpu=cortex-m7 -mthumb -mfpu=fpv5-sp-d16 -mfloat-abi=hard
endif
INCS = -I$(ARCH) -I. -I../.. -Ifreertos-kernel/include -Ifreertos-tcp/include -Ifreertos-tcp/portable/Compiler/GCC
CFLAGS = -W -Wall -Og $(MCU) -fdata-sections -ffunction-sections $(INCS) $(MFLAGS) $(EXTRA)
AFLAGS = --warn --fatal-warnings $(MCU)
LDFLAGS = $(MCU) -specs=nano.specs -T$(ARCH)/link.ld -nostartfiles -nostdlib -lc -lm -lnosys -lgcc #-Wl,-Map=obj/$(PROG).map,--cref -Wl,--gc-sections
A_SRC = $(ARCH)/boot.s
C_SRC = main.c # $(wildcard freertos-tcp/*.c)
C_SRC += freertos-kernel/portable/MemMang/heap_4.c $(ARCH)/port.c
C_SRC += freertos-kernel/list.c freertos-kernel/tasks.c freertos-kernel/queue.c
OBJS := $(C_SRC:%.c=obj/%.o) $(A_SRC:%.s=obj/%.o) #obj/mongoose.o

all: $(PROG).hex

$(PROG).bin: $(PROG).elf
	$(ENV) arm-none-eabi-objcopy -O binary $< $@

$(PROG).hex: $(PROG).bin
	$(ENV) arm-none-eabi-objcopy -I binary -O ihex --change-address 0x8000000 $< $@

$(PROG).elf: $(OBJS)
	$(ENV) arm-none-eabi-gcc $^ $(LDFLAGS) -o $@

obj/%.o: %.c
	@mkdir -p $(dir $@)
	$(ENV) arm-none-eabi-gcc $(CFLAGS) -c $< -o $@

obj/%.o: %.s
	@mkdir -p $(dir $@)
	$(ENV) arm-none-eabi-as $(AFLAGS) $< -o $@

obj/mongoose.o: 
	$(ENV) arm-none-eabi-gcc $(CFLAGS) -c ../../mongoose.c -o $@

clean:
	@rm -rf *.{bin,elf,map,lst,tgz,zip,hex} obj