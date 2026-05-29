# Mongoose CubeMX pack

This pack provides an easy to use CubeMX integration.

## Mongoose Networking Core

This module installs Mongoose Library itself:  `mogoose.c`, `mongoose.h` and
`mongoose_config.h`. It is required for any other module to work. Below are
the steps to make it work with you CubeMX application:

## Configure peripherals

In CubeMX, configure USART (for debug), RNG (for TLS), ETH. 

## Enable debug logs

Open `Core/Src/main.c` and add this snippet:

```c
/* USER CODE BEGIN 0 */
int _write(int fd, unsigned char *buf, int len) {
  HAL_UART_Transmit(&huart3, buf, len, HAL_MAX_DELAY);
  return len;
}
/* USER CODE END 0 */
```

## Call Mongoose

Open `Core/Src/main.c`. Add at the top of the file:

```c
/* USER CODE BEGIN Includes */
#include "mongoose.h"
/* USER CODE END Includes */
```

Inside main() function:

```c
Inside the `main()` function:

```c
/* USER CODE BEGIN WHILE */
struct mg_mgr mgr;
mg_mgr_init(&mgr);

while (1)
{
  mg_mgr_poll(&mgr, 1);
/* USER CODE END WHILE */
```

This starts the TCP/IP stack. In the serial console logs, you should
see something like this:

```text
0      2 mongoose.c:26153:mg_phy_init   PHY ID: 0x07 0xc131 (LAN87x)
6      2 mongoose.c:5834:mg_mgr_init    Driver: stm32h, MAC: 2a:37:94:03:d5:74
d      3 mongoose.c:5841:mg_mgr_init    MG_IO_SIZE: 512, TLS: builtin
13     3 mongoose.c:5758:mg_listen      1 0 http://0.0.0.0:80
19     1 mongoose.c:6260:onstatechange  Link down
7d6    3 mongoose.c:28424:mg_tcpip_driv Link is 100M full-duplex
7db    3 mongoose.c:6404:tx_dhcp_discov DHCP discover sent. Our MAC: 2a:37:94:03:d5:74
831    3 mongoose.c:6382:tx_dhcp_reques DHCP req sent
836    2 mongoose.c:6559:rx_dhcp_client Lease: 3600 sec (3602)
83c    2 mongoose.c:6249:onstatechange  READY, IP: 192.168.2.31
842    2 mongoose.c:6250:onstatechange         GW: 192.168.2.1
847    2 mongoose.c:6253:onstatechange        MAC: 2a:37:94:03:d5:74
```

Now, you should be able to ping your device.
However, it runs no network services.

## Add hello world HTTP server

Let's add the simplest web server possible. After the `mg_mgr_init()`
call, add one more line to start the HTTP listener:

```c
mg_mgr_init(&mgr);
mg_http_listen(&mgr, "http://0.0.0.0:80", http_ev_handler, NULL);
```

And after the `_write()` override, add this HTTP event handler function:

```c
static void http_ev_handler(struct mg_connection *c, int ev, void *ev_data) {
  if (ev == MG_EV_HTTP_MSG) {
    mg_http_reply(c, 200, "", "Hi from Mongoose! Tick %lu\r\n", HAL_GetTick());
  }
}
```

Rebuild, reflash your board, open the device's IP address in the browser,
and see the message displayed, something like:

```text
Hi from Mongoose! Tick 38187
```

## Add full device dashboard

Remove the hello world server if you added it.

Create `Mongoose` directory in the project root and copy
the following files there:

- [dashboard.c](https://raw.githubusercontent.com/cesanta/mongoose/refs/heads/master/tutorials/device-dashboard/full/dashboard.c)
- [dashboard.html](https://raw.githubusercontent.com/cesanta/mongoose/refs/heads/master/tutorials/device-dashboard/full/dashboard.html)
- [html2c.js](https://raw.githubusercontent.com/cesanta/mongoose/refs/heads/master/resources/html2c.js)
 
`html2c.js` is a node.js script that generates `file_data.c` using this command:
`node html2c.js dashboard.html -o file_data.c`. This script first inlines all
external resources into an amalgamated HTML file - so it does not depend on
anything external, and then zips it and packs into a Mongoose embedded
filesystem. This way it takes the minimum space on flash.

In top-level `CMakeLists.txt`, add two extra source files to the build,
and add the rule to generate `file_data.c` from `dashboard.html`.
Note that requires Nodejs installed:

```text
# Generate file_data.c from dashboard.html
add_custom_command(
    OUTPUT ${CMAKE_SOURCE_DIR}/Mongoose/file_data.c
    COMMAND node ${CMAKE_SOURCE_DIR}/Mongoose/html2c.js ${CMAKE_SOURCE_DIR}/Mongoose/dashboard.html -o ${CMAKE_SOURCE_DIR}/Mongoose/file_data.c
    DEPENDS ${CMAKE_SOURCE_DIR}/Mongoose/dashboard.html
    VERBATIM
)

# Add sources to executable
target_sources(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user sources here
    Mongoose/dashboard.c
    Mongoose/file_data.c
)
```


Modify your while loop to look like this:

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

Rebuild and reflash your board, then open the device's IP address in your
browser. Log in using `admin` as the password. You should see the full
dashboard, including the status panel, device settings, firmware update, and
file manager. You can easily customize it for your product, but we will start
by implementing a simple LED control dashboard to demonstrate the principles.

## Simple LED control dashboard

Copy a minimal LED control dashboard example to your `Mongoose/` directory:

- [dashboard.c](https://raw.githubusercontent.com/cesanta/mongoose/refs/heads/master/tutorials/device-dashboard/minimal/dashboard.c)
- [dashboard.html](https://raw.githubusercontent.com/cesanta/mongoose/refs/heads/master/tutorials/device-dashboard/minimal/dashboard.html)

Rebuild and reflash


## AI agentic development

For agentic development, use https://github.com/cesanta/mongoose/tree/master/resources/specs/CLAUDE.md
Use a prompt like this:

> Read and apply https://github.com/cesanta/mongoose/tree/master/resources/specs/CLAUDE.md


## Add MQTT Client


Copy [mqtt.c](https://raw.githubusercontent.com/cesanta/mongoose/refs/heads/master/tutorials/mqtt/mqtt-client/mqtt.c)
to the `Mongoose/` directory. Add it to the extra source files in `CMakeLists.txt`

Add MQTT init and poll to your while loop:

```c
/* USER CODE BEGIN WHILE */
struct mg_mgr mgr;
mg_mgr_init(&mgr);
mg_mqtt_init(&mgr);

while (1)
{
  mg_mgr_poll(&mgr, 1);
  mg_mqtt_poll(&mgr);
/* USER CODE END WHILE */
```

Rebuild, reflash your board. This MQTT Client connects to the
HiveMQ public MQTT broker.
Also it add a JSON-RPC "ota.update" function via MQTT, so you can
update your firmware over MQTT, see https://mongoose.ws/mqtt/
