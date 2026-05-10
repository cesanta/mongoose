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

The `dashboard.html` file has to use `dashboard.js`, for example:

```html
<script src="https://mongoose.ws/resources/dashboard.js"></script>
<script>
  Dashboard.init({
    debug: true,  // Show websocket messages on the console
    data: {
      status: { led1: false, version: '1.0.0' },
    },
  });
  // Show dashboard only when all device data is reported
  Dashboard.on('ready', () => document.body.style.visibility = 'visible');
</script>
```

The `Dashboard.init()` call should be the only direct `Dashboard` API usage.
Do not generate anything that uses other `Dashboard` API - treat it as a black box.

The `Dashboard.init()` call has an argument that is a settings object which should
be edited. The `data`
attrubute specifies device state representation, and it is used in the expression
evaluation and `data-*` attributes described below.

For editing dashboard.html, use only Mongoose `data-bind` and `${...}` template syntax
No vanilla JS event listeners, no fetch calls, no custom reactive logic.
Do not make any changes to dashboard.html unless asked by a user.

- checkboxes, inputs, selects: add `data-bind="FIELDSET.FIELD"` to to bind them to a specific field. Optionally, specify `data-autosave="1"` to send a change immediately after modification, without pressing a "save" button
- any other element: add `data-bind="FIELDSET.FIELD"` to replace its contents with the curent value of the field. You can achieve the same with the `${...}` expression
- buttons: add `data-save="FIELDSET"` to create a "save" button for a given field set. If any value in the field set is edited by the bound control, a button would activate, otherwise it'll stay disabled
- buttons: add `data-cancel="FIELDSET"` to create a "cancel" button, similar to "save"
- `${...}` expressions: add those anywhere


Here is the example of binding a toggle button to a device variable:

```html
<input type="checkbox" data-bind="leds.led1" data-autosave="1" class="toggle" />
```

Here is the example of using `${...}` expressions:

```html
<div>LED ${status.led1 ? 'ON' : 'OFF'}</div>
```

When the expression is evaluated,
its evaluation context contains your Dashboard data, and all global JS variables like `window`.
For example, `${settings.volume + 1}` will be evaluated exactly as you think.

Expressions are useful for the conditional display: expression can add CSS classes, or
CSS styles depending on some conditions. For example, this shows or hides
the warning depedning on the `metrics.ram` condition:

```html
<span class="${metrics.ram <= 30 ? 'alert': 'hidden'}">low RAM !</span>
```


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
