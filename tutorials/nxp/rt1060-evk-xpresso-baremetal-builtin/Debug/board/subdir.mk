################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../board/board.c \
../board/clock_config.c \
../board/dcd.c \
../board/peripherals.c \
../board/pin_mux.c 

C_DEPS += \
./board/board.d \
./board/clock_config.d \
./board/dcd.d \
./board/peripherals.d \
./board/pin_mux.d 

OBJS += \
./board/board.o \
./board/clock_config.o \
./board/dcd.o \
./board/peripherals.o \
./board/pin_mux.o 


# Each subdirectory must supply rules for building sources it contributes
board/%.o: ../board/%.c board/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -DCPU_MIMXRT1062DVL6B -DCPU_MIMXRT1062DVL6B_cm7 -DSDK_OS_BAREMETAL -DXIP_EXTERNAL_FLASH=1 -DXIP_BOOT_HEADER_ENABLE=1 -DSDK_DEBUGCONSOLE=1 -DSERIAL_PORT_TYPE_UART=1 -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -D__NEWLIB__ -I"/root/Documents/MCUXpresso_11.8.1_1197/workspace/rt1060-evk-xpresso-baremetal-builtin/board" -I"/root/Documents/MCUXpresso_11.8.1_1197/workspace/rt1060-evk-xpresso-baremetal-builtin/source" -I"/root/Documents/MCUXpresso_11.8.1_1197/workspace/rt1060-evk-xpresso-baremetal-builtin/drivers" -I"/root/Documents/MCUXpresso_11.8.1_1197/workspace/rt1060-evk-xpresso-baremetal-builtin/xip" -I"/root/Documents/MCUXpresso_11.8.1_1197/workspace/rt1060-evk-xpresso-baremetal-builtin/CMSIS" -I"/root/Documents/MCUXpresso_11.8.1_1197/workspace/rt1060-evk-xpresso-baremetal-builtin/device" -I"/root/Documents/MCUXpresso_11.8.1_1197/workspace/rt1060-evk-xpresso-baremetal-builtin/component/serial_manager" -I"/root/Documents/MCUXpresso_11.8.1_1197/workspace/rt1060-evk-xpresso-baremetal-builtin/component/uart" -I"/root/Documents/MCUXpresso_11.8.1_1197/workspace/rt1060-evk-xpresso-baremetal-builtin/utilities" -I"/root/Documents/MCUXpresso_11.8.1_1197/workspace/rt1060-evk-xpresso-baremetal-builtin/component/lists" -O0 -fno-common -g3 -Wall -c -ffunction-sections -fdata-sections -ffreestanding -fno-builtin -fmerge-constants -fmacro-prefix-map="$(<D)/"= -mcpu=cortex-m7 -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -D__NEWLIB__ -fstack-usage -specs=nano.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-board

clean-board:
	-$(RM) ./board/board.d ./board/board.o ./board/clock_config.d ./board/clock_config.o ./board/dcd.d ./board/dcd.o ./board/peripherals.d ./board/peripherals.o ./board/pin_mux.d ./board/pin_mux.o

.PHONY: clean-board

