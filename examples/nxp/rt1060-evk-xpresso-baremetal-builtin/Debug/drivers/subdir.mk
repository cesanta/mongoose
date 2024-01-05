################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../drivers/fsl_clock.c \
../drivers/fsl_common.c \
../drivers/fsl_common_arm.c \
../drivers/fsl_gpio.c \
../drivers/fsl_lpuart.c \
../drivers/fsl_trng.c 

C_DEPS += \
./drivers/fsl_clock.d \
./drivers/fsl_common.d \
./drivers/fsl_common_arm.d \
./drivers/fsl_gpio.d \
./drivers/fsl_lpuart.d \
./drivers/fsl_trng.d 

OBJS += \
./drivers/fsl_clock.o \
./drivers/fsl_common.o \
./drivers/fsl_common_arm.o \
./drivers/fsl_gpio.o \
./drivers/fsl_lpuart.o \
./drivers/fsl_trng.o 


# Each subdirectory must supply rules for building sources it contributes
drivers/%.o: ../drivers/%.c drivers/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -DCPU_MIMXRT1062DVL6B -DCPU_MIMXRT1062DVL6B_cm7 -DSDK_OS_BAREMETAL -DXIP_EXTERNAL_FLASH=1 -DXIP_BOOT_HEADER_ENABLE=1 -DSDK_DEBUGCONSOLE=1 -DSERIAL_PORT_TYPE_UART=1 -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -D__NEWLIB__ -I"/root/Documents/MCUXpresso_11.8.1_1197/workspace/rt1060-evk-xpresso-baremetal-builtin/board" -I"/root/Documents/MCUXpresso_11.8.1_1197/workspace/rt1060-evk-xpresso-baremetal-builtin/source" -I"/root/Documents/MCUXpresso_11.8.1_1197/workspace/rt1060-evk-xpresso-baremetal-builtin/drivers" -I"/root/Documents/MCUXpresso_11.8.1_1197/workspace/rt1060-evk-xpresso-baremetal-builtin/xip" -I"/root/Documents/MCUXpresso_11.8.1_1197/workspace/rt1060-evk-xpresso-baremetal-builtin/CMSIS" -I"/root/Documents/MCUXpresso_11.8.1_1197/workspace/rt1060-evk-xpresso-baremetal-builtin/device" -I"/root/Documents/MCUXpresso_11.8.1_1197/workspace/rt1060-evk-xpresso-baremetal-builtin/component/serial_manager" -I"/root/Documents/MCUXpresso_11.8.1_1197/workspace/rt1060-evk-xpresso-baremetal-builtin/component/uart" -I"/root/Documents/MCUXpresso_11.8.1_1197/workspace/rt1060-evk-xpresso-baremetal-builtin/utilities" -I"/root/Documents/MCUXpresso_11.8.1_1197/workspace/rt1060-evk-xpresso-baremetal-builtin/component/lists" -O0 -fno-common -g3 -Wall -c -ffunction-sections -fdata-sections -ffreestanding -fno-builtin -fmerge-constants -fmacro-prefix-map="$(<D)/"= -mcpu=cortex-m7 -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -D__NEWLIB__ -fstack-usage -specs=nano.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-drivers

clean-drivers:
	-$(RM) ./drivers/fsl_clock.d ./drivers/fsl_clock.o ./drivers/fsl_common.d ./drivers/fsl_common.o ./drivers/fsl_common_arm.d ./drivers/fsl_common_arm.o ./drivers/fsl_gpio.d ./drivers/fsl_gpio.o ./drivers/fsl_lpuart.d ./drivers/fsl_lpuart.o ./drivers/fsl_trng.d ./drivers/fsl_trng.o

.PHONY: clean-drivers

