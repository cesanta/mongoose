# Nucleo-N657X0-Q Mongoose Integration

This guide explains how to bring up a production-grade Web UI dashboard on the
STM32 Nucleo-N657X0-Q development board.

A pre-configured, ready-to-run project archive is available for download at
https://mongoose.ws.

To integrate the Web UI dashboard into an existing CubeMX project, or to create a
new project from scratch:

1. Configure the peripherals (described below).
2. Regenerate your project code and integrate Mongoose core by following
   https://mongoose.ws/docs/guides/stm32-tcpip-stack/. After Mongoose core has
   been integrated, the protocol examples at
   https://github.com/cesanta/mongoose/tree/master/tutorials should run on your
   board. Copy the corresponding functionality from the example's `main.c` into
   your project's `main.c`.
3. Add the Web UI dashboard by following
   https://mongoose.ws/docs/guides/web-dashboard-guide/.

## Configure peripherals

An `.ioc` file is included in this directory for your reference. It was created
using the following steps:

- Open CubeMX, File / New project, type "STM32N657X0H3Q" in the part number, select
  it from the list, click "Start project" on the top right, choose "Secure domain only"
- Project Manager: Project name: "n657". Project location: path to this directory.
  Project strucure: "FSBL" only. Toolchain/IDE: pick ether Cmake or CubeIDE.
- Clock configuration: Find SYSB, enter 400 MHz, Resolve clock issues
- MCU pinout view: set PG0, PG8, PG10 to GPIO_Output mode
- Pinout & Configuration / System Core: click on "GPIO", then click on each
  of the PG0, PG8, PG10 pins and select Pin Context Assignment / First Stage Boot Loader
- Pinout & Configuration / Security: Enable RIF and RNG for FSBL, click on RNG, check Activate
- Pinout & Configuration / Connectivity: enable USART1 for FSBL, mode asynchronous,
  configure pins PE5 and PE6 for USART1 function
- Pinout & Configuration / Connectivity: enable ETH1 for FSBL, mode RMII,
  pins PF4, PF5, PF7, PF10, PF11, PF12, PF13, PF14, PF15, PG11 for Ethernet function
- RIF:
  - Peripherals (RISUP), enable "Privilege" on ETH1
  - Domains (RIMU): for ETH1, enable SECURE and PRIVILEGE
- Clock configuration: resolve clocks if required. Find "To ETH1" and make sure it has 100 MHz
