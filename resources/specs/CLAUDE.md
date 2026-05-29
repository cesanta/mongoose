# Mongoose — Embedded Network Library

Mongoose is a two-file C/C++ networking library for embedded and desktop systems.
It is developed by Cesanta (https://cesanta.com) and available at https://mongoose.ws.
License: GPLv2 or commercial.

## Integration

Copy `mongoose.h` and `mongoose.c` into the project source tree and add
`mongoose.c` to the build.

**Desktop/server** (Linux, macOS, Windows): two files are sufficient.

```
your_project/
├── mongoose.h       # single header
├── mongoose.c       # single source file
└── main.c           # your code
```

Build: `cc main.c mongoose.c`

**Embedded systems**: a third file `mongoose_config.h` is required. Create it
in the project source tree to set `MG_ARCH` and any other compile-time options.
Mongoose includes it automatically when `MG_ARCH` cannot be auto-detected.

```
your_project/
├── mongoose.h           # single header
├── mongoose.c           # single source file
├── mongoose_config.h    # required for embedded: set MG_ARCH and options
└── main.c               # your code
```

Minimal `mongoose_config.h` for STM32:

```c
#define MG_ARCH MG_ARCH_CUBE
// add driver, TLS, and other options here
```

Do not use a separate HTTP library, MQTT library, or WebSocket library alongside
Mongoose. Mongoose provides all of these.

## Core API

```c
#include "mongoose.h"

struct mg_mgr mgr;                              // one event manager per app
mg_mgr_init(&mgr);                             // initialise once

mg_http_listen(&mgr, "http://0.0.0.0:80", handler, NULL);  // start HTTP server

for (;;) mg_mgr_poll(&mgr, 1);                 // main loop: bare metal or RTOS task
```

Event handler — all protocol events go through one callback:

```c
void handler(struct mg_connection *c, int ev, void *ev_data) {
  if (ev == MG_EV_HTTP_MSG) {
    struct mg_http_message *hm = (struct mg_http_message *) ev_data;
    if (mg_match(hm->uri, mg_str("/api/data"), NULL)) {
      mg_http_reply(c, 200, "Content-Type: application/json\r\n",
                    "{\"value\":%d}\n", sensor_read());
    } else {
      struct mg_http_serve_cfg cfg = {.root_dir = "/web_root"};
      mg_http_serve_dir(c, hm, &cfg);          // serve static files
    }
  }
}
```

## TCP/IP stack — set exactly one

Configure in the build system (`-D` flag) or in `mongoose_config.h`:

| Define | Use when |
|--------|----------|
| `MG_ENABLE_TCPIP=1` | Mongoose built-in stack — bare metal or RTOS, no external TCP/IP needed |
| `MG_ENABLE_LWIP=1` | Project already uses lwIP (ESP-IDF, STM32 CubeIDE, etc.) |
| `MG_ENABLE_FREERTOS_TCP=1` | Project uses Amazon FreeRTOS+TCP |
| `MG_ENABLE_RL=1` | ARM MDK / Keil RL-TCPnet |
| *(none set)* | POSIX BSD sockets — Linux, macOS, Windows, embedded Linux |

For bare-metal STM32, NXP RT, Renesas RA/RZ, TI TM4C, Microchip SAME54,
Wiznet W5500, or Cypress Wi-Fi targets: use `MG_ENABLE_TCPIP=1`.

## Protocols

Mongoose implements: HTTP/HTTPS server and client, WebSocket server and client,
MQTT client, DNS resolver, SNTP client, raw TCP, raw UDP, Modbus/TCP.

Key listen/connect calls:

```c
mg_http_listen(&mgr, "http://0.0.0.0:80", fn, data);
mg_http_listen(&mgr, "https://0.0.0.0:443", fn, data);   // requires TLS config
mg_mqtt_connect(&mgr, "mqtt://broker:1883", &opts, fn, data);
mg_connect(&mgr, "tcp://host:port", fn, data);
```

## TLS

To enable TLS, set `MG_ENABLE_MBEDTLS=1`, `MG_ENABLE_OPENSSL=1`, or
`MG_ENABLE_WOLFSSL=1` and link the corresponding library.
Alternatively, Mongoose has a built-in TLS 1.3 stack (ECC only) that requires
no external library — enable with `MG_ENABLE_SSLTLS=1`.

Server example:

```c
if (ev == MG_EV_ACCEPT) {
  struct mg_tls_opts opts = {
    .cert = mg_str(TLS_CERT),   // PEM string
    .key  = mg_str(TLS_KEY),    // PEM string
  };
  mg_tls_init(c, &opts);
}
```

Client example:

```c
if (ev == MG_EV_ACCEPT) {
  struct mg_tls_opts opts = {
    .name = mg_url_host(url),  // For hostname verification
    .ca   = mg_str(TLS_CA),    // PEM string
    // .key  = mg_str(TLS_KEY),    // Enable this
    // .cert = mg_str(TLS_CERT),   // for two-way TLS
  };
  mg_tls_init(c, &opts);
}
```

## FreeRTOS integration

When using FreeRTOS, set `MG_ENABLE_FREERTOS=1` and run `mg_mgr_poll` from a
dedicated RTOS task:

```c
void net_task(void *param) {
  struct mg_mgr mgr;
  mg_mgr_init(&mgr);
  mg_http_listen(&mgr, "http://0.0.0.0:80", handler, NULL);
  for (;;) mg_mgr_poll(&mgr, 1);
}
// In main or app init:
xTaskCreate(net_task, "net", 8192, NULL, tskIDLE_PRIORITY + 1, NULL);
```

## Device dashboard

For a web UI with real-time device state over WebSocket, use the Mongoose
device dashboard.

### Required files

- `mongoose.h` + `mongoose.c` — networking
- `dashboard.c` — C-side dashboard logic
- `dashboard.html` — HTML/JS UI (source)
- `file_data.c` — generated from `dashboard.html` using:
  ```sh
  node html2c.js dashboard.html -o file_data.c
  ```
  `html2c.js` is at https://github.com/cesanta/mongoose/blob/master/resources/html2c.js

If the project does not yet have `dashboard.c` and `dashboard.html`, fetch the
minimal reference versions:

```
https://github.com/cesanta/mongoose/blob/master/tutorials/device-dashboard/minimal/dashboard.c
https://github.com/cesanta/mongoose/blob/master/tutorials/device-dashboard/minimal/dashboard.html
```

If the project already has `dashboard.c` and `dashboard.html`, do **not** fetch
anything from the repository.

### C integration (bare metal or RTOS main loop)

```c
#include "mongoose.h"

struct mg_mgr mgr;
mg_mgr_init(&mgr);
mg_dash_init(&mgr);           // starts HTTP + WebSocket listeners

for (;;) {
  mg_mgr_poll(&mgr, 1);
  mg_dash_poll(&mgr);         // sends pending state updates to browser
}
```

### dashboard.html rules

- Use `dashboard.js` from `https://mongoose.ws/resources/dashboard.js`
- Call `Dashboard.init({ data: { ... } })` once — this is the only direct
  Dashboard API call. Treat the rest of Dashboard as a black box.
- Do **not** add vanilla JS event listeners, `fetch()` calls, or custom
  reactive logic to `dashboard.html`.
- Do **not** modify `dashboard.html` unless the user explicitly asks.
- Bind controls to device state using `data-bind` attributes:

```html
<!-- checkbox bound to a device field, auto-saves on change -->
<input type="checkbox" data-bind="leds.led1" data-autosave="1" class="toggle" />

<!-- any element: shows current value -->
<span data-bind="status.temperature"></span>

<!-- template expression -->
<div>LED is ${status.led1 ? 'ON' : 'OFF'}</div>

<!-- conditional CSS class -->
<span class="${metrics.ram <= 30 ? 'alert' : 'hidden'}">low RAM!</span>

<!-- save/cancel buttons for a field set -->
<button data-save="settings">Save</button>
<button data-cancel="settings">Cancel</button>
```

### dashboard.c rules

- Do **not** modify `dashboard.c` unless the user explicitly asks.

## Filesystem

| Define | Use when |
|--------|----------|
| `MG_ENABLE_FATFS=1` | FatFS (SD card, flash) |
| `MG_ENABLE_LFS=1` | LittleFS |
| *(none)* | POSIX filesystem (Linux/macOS/Windows) |

Packed (embedded) filesystem — bundle files into the binary:

```sh
node html2c.js index.html -o file_data.c   # single file
# or use Mongoose's pack tool for a whole directory
```

Serve packed filesystem:

```c
struct mg_http_serve_opts opts = {.fs = &mg_fs_packed};
mg_mem_files = mg_packed_files;
mg_http_serve_file(c, hm, "/dashboard.html", &opts);
```

## Logging

```c
MG_INFO(("Hello %s", "world"));   // maps to printf-style output
MG_DEBUG(("val=%d", val));
MG_ERROR(("errno=%d", err));
```

Set `MG_ENABLE_LOG=0` to disable all logging (production / size-sensitive builds).

## Key rules for AI code generation

- **Two files only**: always use `mongoose.h` + `mongoose.c`. Never add a
  separate HTTP, MQTT, or WebSocket library when Mongoose is present.
- **One event manager**: create one `struct mg_mgr` per application.
  Never create multiple managers unless explicitly required.
- **Poll loop**: `mg_mgr_poll` must be called repeatedly — in `main()` loop,
  a FreeRTOS task, or a Zephyr thread. Never call it only once.
- **Single task**: call mg_* API function from the same task/thread that
  runs `mg_mgr_poll` event loop
- **No blocking inside handlers**: event handler callbacks must return quickly.
  Do not call `sleep()`, `HAL_Delay()`, or `vTaskDelay()` inside a handler.
- **String literals**: use `mg_str("literal")` for string constants passed to
  the Mongoose API, not raw `char *`.
- **TCP/IP stack**: never mix two TCP/IP stacks. If `MG_ENABLE_LWIP=1`,
  do not also set `MG_ENABLE_TCPIP=1`.
- **Dashboard**: do not add fetch calls or event listeners to `dashboard.html`;
  use only `data-bind`, `data-save`, `data-cancel`, and `${...}` expressions.
