################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../component/serial_manager/fsl_component_serial_manager.c \
../component/serial_manager/fsl_component_serial_port_uart.c 

C_DEPS += \
./component/serial_manager/fsl_component_serial_manager.d \
./component/serial_manager/fsl_component_serial_port_uart.d 

OBJS += \
./component/serial_manager/fsl_component_serial_manager.o \
./component/serial_manager/fsl_component_serial_port_uart.o 


# Each subdirectory must supply rules for building sources it contributes
component/serial_manager/%.o: ../component/serial_manager/%.c component/serial_manager/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -DCPU_MIMXRT1062DVL6B -DCPU_MIMXRT1062DVL6B_cm7 -DSDK_OS_BAREMETAL -DXIP_EXTERNAL_FLASH=1 -DXIP_BOOT_HEADER_ENABLE=1 -DSDK_DEBUGCONSOLE=1 -DSERIAL_PORT_TYPE_UART=1 -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -D__NEWLIB__ -I"/root/Documents/MCUXpresso_11.8.1_1197/workspace/rt1060-evk-xpresso-baremetal-builtin/board" -I"/root/Documents/MCUXpresso_11.8.1_1197/workspace/rt1060-evk-xpresso-baremetal-builtin/source" -I"/root/Documents/MCUXpresso_11.8.1_1197/workspace/rt1060-evk-xpresso-baremetal-builtin/drivers" -I"/root/Documents/MCUXpresso_11.8.1_1197/workspace/rt1060-evk-xpresso-baremetal-builtin/xip" -I"/root/Documents/MCUXpresso_11.8.1_1197/workspace/rt1060-evk-xpresso-baremetal-builtin/CMSIS" -I"/root/Documents/MCUXpresso_11.8.1_1197/workspace/rt1060-evk-xpresso-baremetal-builtin/device" -I"/root/Documents/MCUXpresso_11.8.1_1197/workspace/rt1060-evk-xpresso-baremetal-builtin/component/serial_manager" -I"/root/Documents/MCUXpresso_11.8.1_1197/workspace/rt1060-evk-xpresso-baremetal-builtin/component/uart" -I"/root/Documents/MCUXpresso_11.8.1_1197/workspace/rt1060-evk-xpresso-baremetal-builtin/utilities" -I"/root/Documents/MCUXpresso_11.8.1_1197/workspace/rt1060-evk-xpresso-baremetal-builtin/component/lists" -O0 -fno-common -g3 -Wall -c -ffunction-sections -fdata-sections -ffreestanding -fno-builtin -fmerge-constants -fmacro-prefix-map="$(<D)/"= -mcpu=cortex-m7 -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -D__NEWLIB__ -fstack-usage -specs=nano.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-component-2f-serial_manager

clean-component-2f-serial_manager:
	-$(RM) ./component/serial_manager/fsl_component_serial_manager.d ./component/serial_manager/fsl_component_serial_manager.o ./component/serial_manager/fsl_component_serial_port_uart.d ./component/serial_manager/fsl_component_serial_port_uart.o

.PHONY: clean-component-2f-serial_manager

