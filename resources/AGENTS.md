# Device dashboard

This is the AI specification file for the Mongoose based Web device dashboard functionality

Mongoose-base device dashboard requires following files:
- mongoose.h
- mongoose.c
- dashboard.c
- file_data.c

The `file_data.c` is a generated file from `dashboard.html` using Nodejs:
```sh
node html2c.js dashboard.html -o file_data.c
```
The `html2c.js` file can be taken from https://github.com/cesanta/mongoose/blob/master/resources/html2c.js


## General rules

Your goal is to produce a dashboard.c and dashboard.html files
If your project does not yet have those, fetch from here:

https://github.com/cesanta/mongoose/blob/master/tutorials/device-dashboard/minimal/dashboard.c
https://github.com/cesanta/mongoose/blob/master/tutorials/device-dashboard/minimal/dashboard.html

If your project already has dashboard.c and dashboard.html, do NOT fetch anything.

The C API header file is at https://github.com/cesanta/mongoose/blob/master/src/dash.h
The minimal reference project is at https://github.com/cesanta/mongoose/blob/master/tutorials/device-dashboard/minimal/
The full reference project is at https://github.com/cesanta/mongoose/blob/master/tutorials/device-dashboard/full/

## dashboard.html

For editing dashboard.html, use only Mongoose `data-bind` and `${...}` template syntax
No vanilla JS event listeners, no fetch calls, no custom reactive logic.
Do not make any changes to dashboard.html unless asked by a user.

## dashboard.c

Do not make any changes to dashboard.c unless asked by a user.

## CubeMX project

If you have installed I-CUBE-Mongoose pack, then all required files are installed
in the `Middlewares/Third_Party/Cesanta_Mongoose/` directory. Do not fetch
dashboard.html, or any other file, from the Mongoose repository.

Add `#include "mongoose.h"` to the "USER CODE BEGIN Includes" block

Add automatic `file_data.c` generation for the build step - either Makefile,
or CMakeLists.txt or whatever is appropriate for the selected IDE.
```sh
node html2c.js dashboard.html -o file_data.c
```


To the "USER CODE BEGIN 0" block, add debug print redirect. If syscalls.c already
has a weak _write override, ignore it and still add this.
```c
int _write(int fd, unsigned char *buf, int len) {
  HAL_UART_Transmit(&huart3, buf, len, HAL_MAX_DELAY);
  return len;
}
```

Make sure main() initialises and processes dashboard:

```c
/* USER CODE BEGIN WHILE */
struct mg_mgr mgr;
mg_mgr_init(&mgr);
mg_dash_init(&mgr);

while (1)
{
  mg_mgr_poll(&mgr, 1);
  mg_dash_poll(&mgr);
/* USER CODE END WHILE */
```

In order to verify the validity of your edits, compile `dashboard.c` using
embedded or host compiler in `Middlewares/Third_Party/Cesanta_Mongoose/`:

```sh
$(CC) -c dashboard.c
```
