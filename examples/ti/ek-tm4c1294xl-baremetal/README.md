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

- [GNU make](http://mongoose.ws/tutorials/tools/#gnu-make)
- [ARM GCC](http://mongoose.ws/tutorials/tools/#arm-gcc)
- [Uniflash](https://mongoose.ws/tutorials/ti/ek-tm4c1294xl-baremetal/#build-and-run) for flashing

The Makefile defaults to using Docker for the compiler, so you don't actually need to install it if you are using a Linux/Mac workstation. If you are not, or you want to run your local ARM compiler, just append `DOCKER=` to the make commands depicted below to call the compiler directly; it must be in your executable path.

In any case, the links above will send you to tutorials on how to install each of those tools in your workstation for Linux, Mac, and Windows.


## Usage

Plugin your board into USB, and attach an Ethernet cable.
To build:

```sh
$ make clean build
```

To flash: use Uniflash

To see debug log, use any serial monitor program like `picocom` at 115200 bps and configure it to insert carriage returns after line feeds:

```sh
$ picocom /dev/ttyACM0 -i -b 115200 --imap=lfcrlf
```

There is also a [detailed tutorial on this example](https://mongoose.ws/tutorials/ti/ek-tm4c1294xl-baremetal/)

For more details and benchmark data on MIP, check the [F746ZG example](../nucleo-f746zg-baremetal/)
