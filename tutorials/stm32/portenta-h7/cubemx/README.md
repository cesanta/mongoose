# Portenta H7 Mongoose Integration

This guide explains how to bring up a production-grade Web UI dashboard on the
Portenta H7 board.

A pre-configured, ready-to-run project zip is available for download at
https://mongoose.ws.

To integrate the Web UI dashboard into an existing CubeMX project, or to create a
new project from scratch:

1. Configure the peripherals (described below).
2. Regenerate your project code and integrate Mongoose core by following
   https://mongoose.ws/docs/guides/stm32-tcpip-stack/.
3. Add the Web UI dashboard by following
   https://mongoose.ws/docs/guides/web-dashboard-guide/.

Note: to setup your WiFi login credentials, open the `wifi.c` file located in the
`Mongoose` directory and edit the `WIFI_NAME` and `WIFI_PASS` macros.

## Configure peripherals

An `.ioc` file is included in this directory for your reference. It was created
using the following steps:

- Open CubeMX. If you are starting a new project, select the STM32747XI
  microcontroller and create a new project.
- In Clock Configuration, set the maximum clock frequency to 200 MHz.
- In Pinout & Configuration / Connectivity, enable USART1 in asynchronous mode.
  Configure pins PA9 and PA10 for USART.
