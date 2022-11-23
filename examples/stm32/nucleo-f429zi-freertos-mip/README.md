# MIP webserver over FreeRTOS on NUCLEO-F429ZI

This firmware uses MIP, an experimental TCP/IP stack of the Mongoose Network Library, running as a FreeRTOS task.
It implements the following:

- Minimal elementary web server, as simple as possible
- Interrupt-driven [Ethernet driver](../../../drivers/mip_driver_stm32.c)
- Basic HAL header [mcu.h](mcu.h) based on CMSIS
- Blue LED blinky, based on another FreeRTOS task
- Debug log on UART3 (st-link)

## Requirements

- [GNU make](http://mongoose.ws/tutorials/tools/#gnu-make)
- [ARM GCC](http://mongoose.ws/tutorials/tools/#arm-gcc)
- [stlink](http://mongoose.ws/tutorials/tools/#stlink) for flashing

The Makefile defaults to using Docker for the compiler, so you don't actually need to install it if you are using a Linux/Mac workstation. If you are not, or you want to run your local ARM compiler, just append `DOCKER=` to the make commands depicted below to call the compiler directly; it must be in your executable path.

In any case, the links above will send you to tutorials on how to install each of those tools in your workstation for Linux, Mac, and Windows.

You'll also need _git_ so the Makefile can clone the FreeRTOS-Kernel repository. We assume you have it since you got to get this repository somehow. If you don't, and don't want to install it, just get the proper [FreeRTOS-Kernel](https://github.com/FreeRTOS/FreeRTOS-Kernel) version (see the Makefile) from its repository, as you did with the Mongoose repository.

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

There is also a [detailed tutorial on this example](http://mongoose.ws/tutorials/stm32/nucleo-f429zi-freertos-mip/)

For more details and benchmark data on MIP, check the [F746ZG example](../nucleo-f746zg-baremetal/)
