# CMSIS-Driver example

Mongoose includes a driver for CMSIS-Driver, that is, Mongoose built-in TCP/IP stack can run over any (ARM) chip that has a CMSIS Driver for its Ethernet controller, and uses a PHY that also has a CMSIS Driver. You can follow this example to use Mongoose there, the Makefile in this example performs the following list of actions.

Actions:

- Pull CMSIS core, this also includes the basic support for CMSIS Driver
- Pull CMSIS Driver, this repository has driver code for several widely used PHYs and some Ethernet chips (stand-alone controllers, not MCUs)
- Pull the device family CMSIS Pack, this includes CMSIS headers and also includes CMSIS Drivers for those peripherals that have Middleware (Ethernet, CAN, UART...) available
- The rest of the job is to find and solve the driver dependencies. In this example, the CMSIS Driver for the STM32F746 uses the STM32 HAL, so we have copied some files from an STM32CubeMX generated project in order to have some defines available, then picked the correct sources to compile, and provided a HAL_GetTick() function to satisfy those parts of the HAL that use it, without having to include more pieces of the HAL, as we already have a time base in place.
- Finally,
  - enable the driver by defining `MG_ENABLE_DRIVER_CMSIS=1`
  - select it in your `main.c`

    ```c
    struct mg_tcpip_if mif = {.driver = &mg_tcpip_driver_cmsis}
    ```
