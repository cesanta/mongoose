# Mongoose - Full-stack embedded networking library

[![License: GPLv2/Commercial](https://img.shields.io/badge/License-GPLv2%20or%20Commercial-green.svg)](https://opensource.org/licenses/gpl-2.0.php)
[![Build Status]( https://github.com/cesanta/mongoose/actions/workflows/quicktest.yml/badge.svg)](https://github.com/cesanta/mongoose/actions)
[![Code Coverage](https://codecov.io/gh/cesanta/mongoose/branch/master/graph/badge.svg)](https://codecov.io/gh/cesanta/mongoose)
[![Fuzzing Status](https://oss-fuzz-build-logs.storage.googleapis.com/badges/mongoose.svg)](https://issues.oss-fuzz.com/issues?sort=-opened&can=1&q=proj:mongoose)

<img src="https://mongoose.ws/images/logo.svg" width="48" height="48"
align="left" style="float:left;" />
Mongoose is a full-stack embedded networking library: built-in TCP/IP stack
with drivers, TLS 1.3, HTTP/WebSocket, MQTT and firmware OTA updates in a
2-file C/C++ library (`mongoose.c`, `mongoose.h`), for STM32, NXP i.MX RT and
other microcontrollers, on any RTOS or bare metal.

Use it instead of lwIP + mbedTLS + httpd + an MQTT client + OTA glue code, or
run it on top of lwIP or Zephyr.

On the market since 2004, used by a vast number of open source and commercial
products - it even runs on the International Space Station! Features include:

- Cross-platform:
  - works on Linux/UNIX, MacOS, Windows, Android, STM32, NXP, ESP32, Nordic, TI, Microchip, Infineon, Renesas and other chips
  - write code once - and it'll work everywhere
  - ideal for the unification of the network infrastructure code across company
- Built-in protocols: plain TCP/UDP, SNTP, HTTP, MQTT, Websocket, Modbus-TCP, and other
- Asynchronous DNS resolver
- Tiny static and run-time footprint
- Source code is both ISO C and ISO C++ compliant
- Very easy to integrate
- Own TCP/IP stack for bare metal / RTOS, or run on top of any existing one (lwIP, Zephyr, ...)
- Built-in TLS 1.3 ECC stack. Also can use external TLS libraries - mbedTLS, OpenSSL, or other
- Does not depend on any other software to implement networking
- Built-in firmware updates for STM32 H5, STM32 H7, NXP IMXRT, RP2040/2350, ESP32 and more
- Commercial support with proactive security updates; [CRA-compliant](https://mongoose.ws/security/cra-compliance/)

Mongoose is ideal for IoT devices, embedded web servers, OTA updates, MQTT telemetry, secure device dashboards, REST APIs on microcontrollers

See https://mongoose.ws/ for complete documentation, videos, case studies, etc.

## Supported platforms

Mongoose has its own built-in TCP/IP stack with drivers, so it needs no
additional software to implement networking. The built-in stack runs on bare
metal and under any RTOS. Platforms supported by the Mongoose built-in TCP/IP
stack:

| MCU | Connectivity | Notes |
| :-- | :----------- | :---- |
| **STM32** | Built-in Ethernet | Ethernet-capable STM32F, STM32H5, STM32H7, STM32N6, STM32C5 variants |
| **NXP i.MX RT** | Built-in Ethernet | RT102x, RT104x, RT105x, RT106x, RT117x, RT118x (NETC) |
| **NXP MCX** | Built-in Ethernet | MCXN94x, MCXE247 |
| **NXP RW612** | Eth, WiFi, 802.15.4 | Separate Ethernet and radio drivers |
| **Microchip ATSAME54** | Built-in Ethernet | GMAC driver |
| **Renesas RA** | Built-in Ethernet | RA6M, RA8M |
| **Infineon XMC** | Built-in Ethernet | XMC4, XMC7 |
| **Texas Instruments** | Built-in Ethernet | TM4C, TMS570 |
| **RP2040, RP2350, STM32, others** | CYW43xx Wi-Fi chips | SPI or SDIO; Pico W, Pico 2 W, Pico RM2, Arduino Portenta H7 |
| **STM32, others** | ST67W6 Wi-Fi modules | SPI; for example, STM32U5 with the ST67W611M1 module |
| **MCUs with an NXP Wi-Fi SDK port** | NXP Wi-Fi | Requires the SDK's Wi-Fi port implementing the driver's network hooks |
| **NXP MCXW71** | Built-in 802.15.4 radio | IPv4 over IEEE 802.15.4 |
| **NXP RW612** | Built-in 802.15.4 radio | IPv4 over IEEE 802.15.4; requires NXP radio firmware |
| **Any MCU with SPI** | W5500 or W5100 Ethernet chips | External Wiznet MAC+PHY |
| **MCUs connected to a cellular modem** | Cellular | AT commands and PPP; for example, SIM800 |
| **MCUs with CMSIS Ethernet drivers** | Ethernet via CMSIS-Driver | Requires CMSIS Ethernet MAC and PHY drivers |

Mongoose can also run on top of any TCP/IP stack that supports the BSD
sockets API. Platforms supported via 3rd party TCP/IP stacks:

| TCP/IP stack    | Notes |
| :-------------- | :------- |
| **lwIP**        | All devices running lwIP, for example ESP32, ESP32S3, ESP32C3, ESP32C6, etc |
| **Zephyr**      | All devices supported by Zephyr |
| **Other**       | Any other TCP/IP stack that supports BSD socket API, for example Amazon FreeRTOS-TCP |
| **Linux, Mac, Windows**   | Workstations, server, single board computers, embedded Linux devices running on MPUs or FPGAs |

## Usage Examples

Below are quick snippets that should give an idea how simple the API is and
how easy it is to create applications with it.

Create a simple web server that serves a directory. The behavior of the
HTTP server is specified by its event handler function:

```c
#include "mongoose.h"   // To build, run: cc main.c mongoose.c

// HTTP server event handler function
void ev_handler(struct mg_connection *c, int ev, void *ev_data) {
  if (ev == MG_EV_HTTP_MSG) {
    struct mg_http_message *hm = (struct mg_http_message *) ev_data;
    struct mg_http_serve_opts opts = { .root_dir = "./web_root/" };
    mg_http_serve_dir(c, hm, &opts);
  }
}

int main(void) {
  struct mg_mgr mgr;  // Declare event manager
  mg_mgr_init(&mgr);  // Initialise event manager
  mg_http_listen(&mgr, "http://0.0.0.0:8000", ev_handler, NULL);  // Setup listener
  for (;;) {          // Run an infinite event loop
    mg_mgr_poll(&mgr, 1000);
  }
  return 0;
}
```

HTTP server implements a REST API that returns current time. JSON formatting:
```c
static void ev_handler(struct mg_connection *c, int ev, void *ev_data) {
  if (ev == MG_EV_HTTP_MSG) {
    struct mg_http_message *hm = (struct mg_http_message *) ev_data;
    if (mg_match(hm->uri, mg_str("/api/time/get"), NULL)) {
      mg_http_reply(c, 200, "", "{%m:%lu}\n", MG_ESC("time"), time(NULL));
    } else {
      mg_http_reply(c, 500, "", "{%m:%m}\n", MG_ESC("error"), MG_ESC("Unsupported URI")); 
    }
  }
}
```

MQTT client that subscribes to a topic `device1/rx` and
echoes incoming messages to `device1/tx`:

```c
#include "mongoose.h"

static const char *s_mqtt_url = "mqtt://broker.hivemq.com:1883";
static struct mg_connection *s_mqtt_conn = NULL;

// MQTT connection event handler function
static void ev_handler(struct mg_connection *c, int ev, void *ev_data) {
  if (ev == MG_EV_OPEN) {
    MG_INFO(("%lu created, connecting to %s ...", c->id, s_mqtt_url));
  } else if (ev == MG_EV_MQTT_OPEN) {
    struct mg_mqtt_opts opts = {.qos = 1, .topic = mg_str("device1/rx")};
    mg_mqtt_sub(c, &opts);
    MG_INFO(("%lu connected, subscribing to %s", c->id, opts.topic.buf));
  } else if (ev == MG_EV_MQTT_MSG) {
    char response[100];
    struct mg_mqtt_message *mm = (struct mg_mqtt_message *) ev_data;
    struct mg_mqtt_opts opts = {.qos = 1, .topic = mg_str("device1/tx")};
    mg_snprintf(response, sizeof(response), "Received [%.*s] / [%.*s]",
                mm->topic.len, mm->topic.buf, mm->data.len, mm->data.buf);
    opts.message = mg_str(response);
    mg_mqtt_pub(c, &opts);
  } else if (ev == MG_EV_CLOSE) {
    MG_INFO(("%u closing", c->id));
    s_mqtt_conn = NULL;
  }
}

// Reconnection timer function. If we get disconnected, reconnect again
static void timer_fn(void *arg) {
  struct mg_mgr *mgr = (struct mg_mgr *) arg;
  if (s_mqtt_conn == NULL) {
    struct mg_mqtt_opts opts = {.clean = true};
    s_mqtt_conn = mg_mqtt_connect(mgr, s_mqtt_url, &opts, ev_handler, NULL);
  }
}

int main() {
  struct mg_mgr mgr;  // Mongoose event manager. Holds all connections
  mg_mgr_init(&mgr);  // Initialise event manager
  mg_timer_add(&mgr, 3000, MG_TIMER_REPEAT | MG_TIMER_RUN_NOW, timer_fn, &mgr);
  for (;;) {
    mg_mgr_poll(&mgr, 1000);  // Infinite event loop
  }
  return 0;
}
```

## Licensing

Mongoose is dual-licensed:
- GPLv2: free for open source projects. Many popular open source projects use
  Mongoose, for example
  [SWUpdate](https://github.com/sbabic/swupdate/tree/master/mongoose), the
  embedded Linux update framework
- Commercial: for closed-source products. Includes support directly from the
  engineers who wrote the whole stack, and early security fixes.
  See [licensing](https://mongoose.ws/licensing/)

## Commercial use
- Mongoose is used by hundreds of businesses, from Fortune500 giants like
  Siemens, Schneider Electric, Broadcom, Bosch, Google, Samsung, Qualcomm, Caterpillar to the small businesses
- Used to solve a wide range of business needs, like implementing Web UI
  interface on devices, RESTful API services, telemetry data exchange, remote
  control for a product, remote software updates, remote monitoring, and others
- Deployed to hundreds of millions devices in production environment worldwide
- See [Case Studies](https://mongoose.ws/case-studies/) from our respected
  customers like [Schneider Electric](https://mongoose.ws/case-studies/schneider-electric/) (industrial automation), [Broadcom](https://mongoose.ws/case-studies/broadcom/) (semiconductors), [Pilz](https://mongoose.ws/case-studies/pilz/) (industrial automation), and others
- See [Testimonials](https://mongoose.ws/testimonials/) from engineers that integrated Mongoose in their commercial products
- We provide [Evaluation and Commercial licensing](https://mongoose.ws/licensing/), [support](https://mongoose.ws/support/), consultancy and [integration
  services](https://mongoose.ws/integration/) - don't hesitate to [contact us](https://mongoose.ws/contact/)


## Security

We take security seriously:
1. Mongoose repository runs a
  [continuous integration test powered by GitHub](https://github.com/cesanta/mongoose/actions),
  which runs through hundreds of unit tests on every commit to the repository.
  Our [unit tests](https://github.com/cesanta/mongoose/tree/master/test)
  are built with modern address sanitizer technologies, which help to find
  security vulnerabilities early
2. Mongoose repository is integrated into Google's
  [oss-fuzz continuous fuzzer](https://bugs.chromium.org/p/oss-fuzz/issues/list?sort=-opened&can=1&q=proj:mongoose)
  which scans for potential vulnerabilities continuously
3.  We receive periodic vulnerability reports from the independent security
  groups like
  [Cisco Talos](https://www.cisco.com/c/en/us/products/security/talos.html),
  [Microsoft Security Response Center](https://www.microsoft.com/en-us/msrc),
  [MITRE Corporation](https://www.mitre.org/),
  [Compass Security](https://www.compass-security.com/en/) and others.
  In case of the vulnerability found, we act according to the industry best
  practice: hold on to the publication, fix the software and notify all our
  customers that have an appropriate subscription
4. Some of our customers (for example NASA)
  have specific security requirements and run independent security audits,
  of which we get notified and in case of any issue, act similar to (3).

## How to report security vulnerabilities

Please report via https://github.com/cesanta/mongoose/security/advisories.
Do NOT create a github issue.

## Articles

Technical guides and deep dives into embedded web servers, WebUI integration and embedded networking technologies:
- [Embedded Web Server: A Comprehensive Guide for Modern Connected Devices](https://mongoose.ws/articles/embedded-web-server-a-comprehensive-guide-for-modern-connected-devices/)
- [Building Embedded Web Device Dashboards](https://mongoose.ws/articles/building-embedded-web-device-dashboard/)
- [ESP32 Device Dashboard: A Step-by-Step Guide for Developers](https://mongoose.ws/articles/esp32-device-dashboard/)
- [How to build an STM32 Web Dashboard](https://mongoose.ws/articles/stm32-device-dashboard/)
- [STM32 WebSocket Guide](https://mongoose.ws/articles/stm32-websocket-guide/)
- [Web File Manager on STM32, ESP32 and Embedded Linux](https://mongoose.ws/articles/building-a-web-file-manager-on-stm32-esp32-embedded-linux/)
- [Web dashboard on Zephyr RTOS](https://mongoose.ws/articles/web-dashboard-on-zephyr-rtos/)
- [Limiting TCP/IP RAM usage on STM32](https://mongoose.ws/articles/limiting-tcpip-ram-usage-on-stm32/)
- [STM32 Ethernet explained](https://mongoose.ws/articles/stm32-ethernet-explained/)
- [MQTT on a Microcontroller](https://mongoose.ws/articles/mqtt-on-a-microcontroller/)
- [STM32 OTA Firmware Update](https://mongoose.ws/articles/stm32-ota-firmware-update/)
- [RP2350 OTA Firmware Update](https://mongoose.ws/articles/rp2350-ota-firmware-update/)
- [STM32 Ethernet and caches](https://mongoose.ws/articles/stm32-ethernet-and-cache/)
- [NXP RW612 OTA Firmware Update](https://mongoose.ws/articles/rw612-ota-firmware-update/)
- [lwIP vs Mongoose - TCP/IP Stack Integration Benchmark](https://mongoose.ws/articles/lwip-vs-mongoose-tcpip-stack-integration/)
- [Embedded TCP/IP Stacks Comparison](https://mongoose.ws/embedded-tcpip-stack-comparison/)
- [STM32H5 File System on Internal Flash: LittleFS + Mongoose](https://mongoose.ws/articles/stm32h5-file-system-on-internal-flash/)
- [Firmware OTA and remote control on NXP FRDM-RW612](https://mongoose.ws/articles/mdash/)
- [25000 WebSocket messages per second on STM32 Nucleo-H723ZG](https://mongoose.ws/articles/stm32-websocket-benchmark/)
- [How to connect STM32 to AWS IoT Core](https://mongoose.ws/articles/stm32-aws-iot/)
- [How to connect STM32 to Azure IoT Hub](https://mongoose.ws/articles/stm32-azure-iot-hub/)
- [How to connect STM32 to EMQX](https://mongoose.ws/articles/stm32-emqx/)
- [STM32 Device Management deep dive: Secure OTA, Remote Control, health monitor](https://mongoose.ws/articles/stm32-mdash-deep-dive/)
- [STM32 TCP/IP footprint: how much flash and RAM does Mongoose take?](https://mongoose.ws/articles/tcp-ip-footprint/)


## Contributions

Contributions are welcome! Please follow the guidelines below:

- Sign [Cesanta CLA](https://cesanta.com/cla.html) and send GitHub pull request
- Make sure that PRs have only one commit, and deal with one issue only
