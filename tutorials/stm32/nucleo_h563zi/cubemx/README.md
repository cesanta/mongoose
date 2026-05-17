# Nucleo-H563ZI Mongoose Integration

This guide explains how to bring up a production-grade Web UI dashboard on the
STM32 Nucleo-H563ZI development board.

A pre-configured, ready-to-run project zip is available for download at
https://mongoose.ws.

To integrate the Web UI dashboard into an existing CubeMX project, or to create a
new project from scratch:

1. Configure the peripherals (described below).
2. Regenerate your project code and integrate Mongoose core by following
   https://mongoose.ws/docs/guides/stm32-tcpip-stack/.
3. Add the Web UI dashboard by following
   https://mongoose.ws/docs/guides/web-dashboard-guide/.

## Configure peripherals

An `.ioc` file is included in this directory for your reference. It was created
using the following steps:

- Open CubeMX. If you are starting a new project, select the STM32H563ZI
  microcontroller and create a new project.
- In Pinout & Configuration / Security, enable RNG and select "Activated".
- In Clock Configuration, set the maximum clock frequency to 250 MHz.
- In the MCU pinout view, set pins PB0, PF4, and PG4 to GPIO_Output mode.
- In Pinout & Configuration / Connectivity, enable USART3 in asynchronous mode.
  Configure pins PD8 and PD9 for USART.
- In Pinout & Configuration / Connectivity, enable ETH in RMII mode and set pins
  PA1, PA2, PA7, PC1, PC4, PC5, PB15, PG11, and PG13 for Ethernet.
