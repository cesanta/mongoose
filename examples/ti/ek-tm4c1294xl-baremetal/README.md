# Baremetal webserver on EK-TM4C1294XL

This firmware uses experimental TCP/IP stack of the Mongoose Network Library,
which implements the following:

- A complete [HTTP device dashboard](../../device-dashboard) with:
  - User authentication
  - Real-time device data graph
  - Coninfiguration display and update
  - MQTT communication with a remote MQTT server
- No dependencies: no HAL, no CMSIS, no RTOS
- Hand-written [mcu.h](mcu.h) header based on the [datasheet](https://www.ti.com/lit/pdf/spms433)
- Interrupt-driven [Ethernet driver](../../../drivers/mip_driver_tm4c.c)
- LED blinky, based on SysTick interrupt
- User button handler, turns off/on an LED, based on GPIO IRQs (interrupt-driven)
- HardFault handler alternate blinks LEDs
- Debug log on UART0 (ICDI)

## Requirements

- GNU make
- [ARM GCC](https://developer.arm.com/tools-and-software/open-source-software/developer-tools/gnu-toolchain/gnu-rm) toolchain for build
- [Uniflash](https://www.ti.com/tool/UNIFLASH) for flashing

## Usage

Plugin your board into USB, and attach an Ethernet cable.
To build:

```sh
$ make clean nuild
```
To flash: use Uniflash

To see debug log, use any serial monitor program like `picocom` at 115200 bps and configure it to insert carriage returns after line feeds:

```sh
$ picocom /dev/ttyACM0 -i -b 115200 --imap=lfcrlf
```

For more details and benchmark data on MIP, check the [F746ZG example](../nucleo-f746zg-baremetal/)
