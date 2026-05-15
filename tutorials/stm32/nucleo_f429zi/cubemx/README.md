# Mongoose CubeMX integration

This is a step-by-step guide on how to bring up a production-grade Web UI
dashboard on STM32 Nucleo-F429ZI development board.

A pre-configured project .zip is available for download at https://mongoose.ws.
However for those who want to configure the project from scratch, follow the steps
below.

## Generate project code
- Open the .ioc file in your CubeMX
- Click on "Project Manager"
  - If you're using CubeIDE, select "CubeIDE" in the toolchain/IDE dropdown
  - If you're using VSCode, select "Cmake" in the toolchain/IDE dropdown
  - If you're using Makefile, select "Makefile" in the toolchain/IDE dropdown
- Click "Generate Code" button on the top right

This should generate project code in this directory the directory where .ioc file lives.
Open this directory in your IDE.

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
  HAL_UART_Transmit(&huart3, buf, len, HAL_MAX_DELAY);
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
run_mongoose();  // <----------------- Add this line
while (1)
{
  /* USER CODE END WHILE */
```

## Build, flash, test

Start serial console.
Use STM32CubeProgrammer to flash the firmware.
