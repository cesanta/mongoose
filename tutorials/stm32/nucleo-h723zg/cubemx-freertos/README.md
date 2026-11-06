# Nucleo-H723ZG Mongoose Integration

This guide explains how to bring up a production-grade Web UI dashboard on the
STM32 Nucleo-H723ZG development board.

## Build, flash, and debug in VS Code

Connect the board's ST-LINK USB port. The tasks require CMake, Ninja,
`arm-none-eabi-gcc`, and `STM32_Programmer_CLI` on `PATH`. Debugging uses the
Cortex-Debug extension and its configured `cortex-debug.stlinkPath`,
`cortex-debug.stm32cubeprogrammer` (the programmer's `bin` directory),
`cortex-debug.armToolchainPath`, and `cortex-debug.gdbPath` settings.

- **Build:** press **Ctrl+Shift+B** to configure and build the Debug preset.
- **Flash and run:** choose **Terminal > Run Task > Flash STM32 Debug**. This
  builds the firmware, programs the ELF, verifies it, and resets the MCU.
- **Flash and debug:** select **NUCLEO-H723ZG: Flash and Debug (ST-Link)** in
  Run and Debug, then press **F5**. This builds and programs the firmware and
  stops at `main`. Set breakpoints and press **F5** to continue.
- **Attach:** select **NUCLEO-H723ZG: Attach (ST-Link)** to inspect firmware
  already on the board. Use the matching `build/Debug/nucleo-h723zg.elf` for
  symbols; this configuration does not build or download firmware.

Stop an active debug session before running the flash task so the probe is free.
The H723 Cortex-M7 uses debug access port **0** (`-m 0` for the GDB server,
`ap=0` for CubeProgrammer). Selecting `-m 1` causes the GDB server to fail with
`Failed to halt target`. Programming connects under hardware reset.

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

- Open CubeMX. If you are starting a new project, select the STM32H723ZG
  microcontroller and create a new project.
- In Clock Configuration, set the maximum clock frequency to 550 MHz.
- In the MCU pinout view, set pins PB0, PE1, and PB14 to GPIO_Output mode.
- In Pinout & Configuration / Connectivity, enable USART3 in asynchronous mode.
  Configure pins PD8 and PD9 for USART.
- In Pinout & Configuration / Connectivity, enable ETH in RMII mode and set pins
  PA1, PA2, PA7, PC1, PC4, PC5, PB13, PG11, and PG13 for Ethernet.
- In the "Project Manager", set heap size to 0x10000, and stack size to 0x2000
