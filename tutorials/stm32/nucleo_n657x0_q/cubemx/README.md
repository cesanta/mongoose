# Mongoose CubeMX integration

This is a step-by-step guide on how to bring up a production-grade Web UI
dashboard on STM32 Nucleo-N657X0-Q development board.

A pre-configured project .zip is available for download at https://mongoose.ws.
However for those who want to configure the project from scratch, follow the steps
below.

## Generate .ioc file

This directory already has a pre-configured `.ioc` file so you can just open it
in your CubeMX and skip this step. However for those who want to create the
project from scratch, here are the steps:

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

## Generate project code
- Clone this repo to your workstation. Open the .ioc file in your CubeMX
- Click on "Project Manager"
  - If you're using CubeIDE, select "CubeIDE" in the toolchain/IDE dropdown
  - If you're using VSCode, select "Cmake" in the toolchain/IDE dropdown
  - If you're using Makefile, select "Makefile" in the toolchain/IDE dropdown
- Click "Generate Code" button on the top right

## Configure .bin generation

Normally, IDEs generate .elf firmware file, cause it is easier for a debugger
to load it to a device. However, for the firmware OTA update, the .bin file
is required. The .bin file is the firmware image that gets copied to the
beginning of the microcontroller's flash memory and gets executed after the
system boot.

So, we need to tell the IDE to generate .bin firmware flash image file.

- If you're using CubeIDE, select Project / Properties / C/C++ build / Settings
  / MCU/MPU Post build outputs, enable "Convert to binary file (-O binary)"
- If you're using VSCode, add a section to the end of the top-level `CMakeLists.txt`:
```cmake
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
  COMMAND ${CMAKE_OBJCOPY} -O binary
          ${PROJECT_NAME}.elf
          ${PROJECT_NAME}.bin
)
```

## Call Mongoose from main.c

Edit `Core/Src/main.c` and update the following snippets - notice the `USER CODE BEGIN` / `USER CODE END` placeholders. At the top of the file:

```c
/* USER CODE BEGIN Includes */
#include "mongoose_glue.h"
/* USER CODE END Includes */
```

Before the `main()` function:
```c
/* USER CODE BEGIN 0 */
int _write(int fd, unsigned char *buf, int len) {
  HAL_UART_Transmit(&huart1, buf, len, HAL_MAX_DELAY);
  return len;
}

static void http_ev_handler(struct mg_connection *c, int ev, void *ev_data) {
  if (ev == MG_EV_HTTP_MSG) {  // New HTTP request received
    struct mg_http_message *hm = (struct mg_http_message *) ev_data;
    if (mg_match(hm->uri, mg_str("/api/tick"), NULL)) {
      mg_http_reply(c, 200, "", "{%m:%llu}\n", MG_ESC("tick"), HAL_GetTick());
    } else {
      mg_http_reply(c, 200, "", "Hi from Mongoose, tick %llu\n", HAL_GetTick());
    }
  }
}

// This blocks forever. Call it at the end of main(), or in a
// separate RTOS task. Give that task 8k stack space.
void run_mongoose(void) {
  struct mg_mgr mgr;
  mg_mgr_init(&mgr);
  mg_http_listen(&mgr, "http://0.0.0.0", http_ev_handler, NULL);

  for (;;) {
    mg_mgr_poll(&mgr, 0);
  }
}
/* USER CODE END 0 */
```

Inside the `main()` function:
```c
/* USER CODE BEGIN WHILE */
HAL_GPIO_WritePin(GPIOG, GPIO_PIN_0, 1);
HAL_GPIO_WritePin(GPIOG, GPIO_PIN_8, 1);
HAL_GPIO_WritePin(GPIOG, GPIO_PIN_10, 1);
run_mongoose();
while (1)
{
  /* USER CODE END WHILE */
```

## Build, flash, test

Start serial console.
Use STM32CubeProgrammer to flash the firmware. Don't forget to specify
the `MX25UM51245G_STM32N6570-NUCLEO.stldr` external loader for this board.
