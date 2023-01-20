# Baremetal webserver on NUCLEO-F429ZI

This firmware uses MIP, an experimental TCP/IP stack of the Mongoose Network Library,
which implements the following:

- A complete [HTTP device dashboard](../../device-dashboard) with:
  - User authentication
  - Real-time device data graph
  - Coninfiguration display and update
  - MQTT communication with a remote MQTT server
- No dependencies: no HAL, no CMSIS, no RTOS
- Hand-written [mcu.h](mcu.h) header based on a [datasheet](https://www.st.com/resource/en/reference_manual/rm0090-stm32f405415-stm32f407417-stm32f427437-and-stm32f429439-advanced-armbased-32bit-mcus-stmicroelectronics.pdf)
- Interrupt-driven [Ethernet driver](../../../drivers/mip_driver_stm32.c)
- Blue LED blinky, based on SysTick interrupt
- User button handler, turns off/on green LED, based on EXTI, interrupt-driven 
- HardFault handler that blinks red LED
- Debug log on UART3 (st-link)

## Requirements

- [GNU make](http://mongoose.ws/tutorials/tools/#gnu-make)
- [ARM GCC](http://mongoose.ws/tutorials/tools/#arm-gcc)
- [stlink](http://mongoose.ws/tutorials/tools/#stlink) for flashing

The links above will send you to tutorials on how to install each of those tools in your workstation for Linux, Mac, and Windows.

## Usage

Plugin your Nucleo board into USB, and attach an Ethernet cable.
To build and flash:

```sh
$ make clean flash
```

To see debug log, use any serial monitor program like `picocom` at 115200 bps and configure it to insert carriage returns after line feeds:

```sh
$ picocom /dev/ttyACM0 -i -b 115200 --imap=lfcrlf
```

There is also a [detailed tutorial on this example](https://mongoose.ws/tutorials/stm32/nucleo-f746zg-baremetal/) but for the NUCLEO-F746ZG board

For more details and benchmark data on MIP, check the [F746ZG example](../nucleo-f746zg-baremetal/)
