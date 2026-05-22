# Nucleo-U5A5ZJ-Q Mongoose Integration

This guide explains how to bring up a production-grade Web UI dashboard on the
Nucleo-U5A5ZJ-Q board with the X-NUCLEO-67W61M1 Wi-Fi
expansion board for STM32 Nucleo boards, based on the ST67W611M1 Wi-Fi module.

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

- Open CubeMX. If you are starting a new project, select the STM32U5A5ZJT6Q
  microcontroller and create a new project.
- In Clock Configuration, set the maximum clock frequency to 160 MHz.
- In Pinout & Configuration / Connectivity, enable USART1 in asynchronous mode.
  Configure pins PA9 and PA10 for USART.

## Wi-Fi module firmware setup

Before running this project, the correct firmware must be flashed onto the Wi-Fi board.

Steps:

1. Install the X-CUBE-ST67W61 package.

   Open `STM32CubeMX`, then navigate to:
   `Help -> Manage embedded software packages`.

   In the `STMicroelectronics` tab, locate `X-CUBE-ST67W61`.
   Select the latest version and click `Install`.

2. Connect the X-NUCLEO-67W61M1 board to the Nucleo-U5A5ZJ-Q board using the
   Arduino connectors CN7, CN8, CN9 and CN10.

   See Figure 5 in the Wi-Fi board
   [user manual](https://www.st.com/resource/en/user_manual/um3475-getting-started-with-xcubest67w61-stmicroelectronics.pdf)
   for the connector layout.

3. Program the firmware to the ST67W61 module.

   Connect the Nucleo board to your PC using a USB cable.

   Navigate to the STM32Cube repository directory. The installed package is located at:

   `Packs/STMicroelectronics/X-CUBE-ST67W61/<version>/`

   Then navigate to:

   `<version>/Projects/ST67W6X_Utilities/Binaries`

   Run the appropriate script depending on your operating system:

   - `NCP_update_mission_profile_t02.sh` (Linux)
   - `NCP_update_mission_profile_t02.bat` (Windows)

   Ensure that `STM32CubeProgrammer` is installed and available in your system `PATH`.

   For more details on the programming process, see Section 4.3 and Table 2 of the
   Wi-Fi board
   [user manual](https://www.st.com/resource/en/user_manual/um3475-getting-started-with-xcubest67w61-stmicroelectronics.pdf).
