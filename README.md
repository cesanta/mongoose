# Mongoose - Embedded Web Server / Embedded Networking Library

![License: GPLv2](https://img.shields.io/badge/license-GPL_2-green.svg "License")
[![Build Status]( https://github.com/cesanta/mongoose/workflows/build/badge.svg)](https://github.com/cesanta/mongoose/actions)
[![Code Coverage](https://codecov.io/gh/cesanta/mongoose/branch/master/graph/badge.svg)](https://codecov.io/gh/cesanta/mongoose)
[![Fuzzing Status](https://oss-fuzz-build-logs.storage.googleapis.com/badges/mongoose.svg)](https://bugs.chromium.org/p/oss-fuzz/issues/list?sort=-opened&can=1&q=proj:mongoose)

Mongoose is a networking library for C/C++. It implements event-driven
non-blocking APIs for TCP, UDP, HTTP, WebSocket, MQTT.  It has been designed
for connecting devices and bringing them online. On the market since 2004, used
by vast number of open source and commercial products - it even runs on the
International Space Station!  Mongoose makes embedded network programming fast,
robust, and easy.

- [Download Mongoose Source Code here](https://www.cesanta.com/download.html)
- [Read Mongoose documentation](https://cesanta.com/docs/)

Connectivity is vital for software and embedded devices, but there are many
pitfalls to consider when embedding a web server.  This white paper breaks down
on the top 9 things to avoid when embedding a web server.

- [Download “9 Things NOT to do when embedding a web server” white paper here](https://www.cesanta.com/whitepaper.html)

Looking for a complete IoT solution? Check out
- [VCON](https://vcon.io) - Arduino compatible MCU modules with built-in firmware OTA updates
- [Mongoose OS](https://mongoose-os.com) - open source embedded operating system for low-power connected microcontrollers. Secure, designed for commercial Internet of Things products

# Support
- [Study Mongoose examples](https://github.com/cesanta/mongoose/tree/master/examples)
- [Support Forum - ask your technical questions here](https://forum.cesanta.com/c/mongoose-library/6)
- [Commercial licensing and support available](https://www.cesanta.com/licensing.html)
- [Check out latest releases](https://github.com/cesanta/mongoose/releases)

# Features

* Cross-platform: works on Linux/UNIX, MacOS, QNX, eCos, Windows, Android,
  iPhone, FreeRTOS, etc
* Supported embedded architectures: ESP32, ESP8266, TI CC3200, TI MSP432, NRF52, STM32, PIC32, NXP, and more
* Builtin protocols: plain TCP/UDP, HTTP, MQTT, Websocket
* SSL/TLS support: mbedTLS, OpenSSL or custom via API
* Asynchronous DNS resolver
* Tiny static and run-time footprint
* Source code is both ISO C and ISO C++ compliant
* Very easy to integrate: just copy
  [mongoose.c](https://raw.githubusercontent.com/cesanta/mongoose/master/mongoose.c) and
  [mongoose.h](https://raw.githubusercontent.com/cesanta/mongoose/master/mongoose.h)
  files to your build tree

# Licensing

Mongoose is released under Commercial and [GNU GPL v.2](http://www.gnu.org/licenses/old-licenses/gpl-2.0.html) open source licenses.

Commercial Projects: [Contact us for commercial license.](https://www.cesanta.com/contact.html)

# Dashboard Example

Mongoose is often used to implement device dashboards and real-time
data exchange over Websocket. Here is a dashboard example that illustrates
the functionality:

![](http://www.cesanta.com/images/dashboard.png)

[Developing a new product? Contact us today to discuss how Mongoose can help.](https://www.cesanta.com/contact.html)

# Contributions

Contributions are welcome! Please follow the guidelines below:

- Sign [Cesanta CLA](https://cesanta.com/cla.html) and send GitHub pull request
- When making pull requests, please make sure that it has only one commit,
 and imlements/fixes only one piece of functionality

# Looking for a pre-compiled Mongoose web server Windows or Mac binary?
- [Download pre-compiled Mongoose web server binary.](https://www.cesanta.com/binary.html)
