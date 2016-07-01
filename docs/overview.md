---
title: Overview
---

Mongoose is a swiss army knife for embedded network programming.
It implements event-driven non-blocking API for TCP, UDP, HTTP,
WebSocket, CoAP, MQTT for both client and server mode.
Features include:

- Cross-platform: works on Linux/UNIX, MacOS, QNX, eCos, Windows, Android,
  iPhone, FreeRTOS (TI CC3200, ESP8266), etc
- Single-threaded, asynchronous, non-blocking core with simple event-based API
- Native support for [PicoTCP embedded TCP/IP stack](http://www.picotcp.com),
  [LWIP embedded TCP/IP stack](https://en.wikipedia.org/wiki/LwIP)
- Builtin protocols:
   - plain TCP, plain UDP, SSL/TLS (over TCP, one-way or two-way)
   - HTTP client and server
   - WebSocket client and server
   - MQTT client and server
   - CoAP client and server
   - DNS client and server
   - asynchronous DNS resolver
- Tiny static and run-time footprint
- Source code is both ISO C and ISO C++ compliant
- Very easy to integrate: just copy
  [mongoose.c](https://raw.githubusercontent.com/cesanta/mongoose/master/mongoose.c) and
  [mongoose.h](https://raw.githubusercontent.com/cesanta/mongoose/master/mongoose.h)
  files to your build tree
