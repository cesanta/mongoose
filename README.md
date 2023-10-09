# Mongoose - Embedded Web Server / Embedded Networking Library

[![License: GPLv2/Commercial](https://img.shields.io/badge/License-GPLv2%20or%20Commercial-green.svg)](https://opensource.org/licenses/gpl-2.0.php)
[![Build Status]( https://github.com/cesanta/mongoose/workflows/build/badge.svg)](https://github.com/cesanta/mongoose/actions)
[![Code Coverage](https://codecov.io/gh/cesanta/mongoose/branch/master/graph/badge.svg)](https://codecov.io/gh/cesanta/mongoose)
[![Fuzzing Status](https://oss-fuzz-build-logs.storage.googleapis.com/badges/mongoose.svg)](https://bugs.chromium.org/p/oss-fuzz/issues/list?sort=-opened&can=1&q=proj:mongoose)

Mongoose is a network library for C/C++. 
It implements event-driven
non-blocking APIs for TCP, UDP, HTTP, WebSocket, MQTT.  It is designed for
connecting devices and bringing them online. On the market since 2004, used by
vast number of open source and commercial products - it even runs on the
International Space Station!  Mongoose makes embedded network programming fast,
robust, and easy. Features include:

- Cross-platform:
  - works on Linux/UNIX, MacOS, Windows, Android
  - works on STM32, NXP, ESP32, NRF52, TI, Microchip, and other
  - write code once - and it'll work everywhere
  - ideal for the unification of the network infrastructure code across company
- Built-in protocols: plain TCP/UDP, SNTP, HTTP, MQTT, Websocket
- SSL/TLS support: mbedTLS, OpenSSL or custom (via API)
- Asynchronous DNS resolver
- Tiny static and run-time footprint
- Source code is both ISO C and ISO C++ compliant
- Very easy to integrate: just copy `mongoose.c` and `mongoose.h` files to your source tree. See
  [exact steps](https://mongoose.ws/documentation/#2-minute-integration-guide)
- Works with any network stack with socket API, like LwIP or FreeRTOS-Plus-TCP
- Built-in TCP/IP stack with drivers for bare metal or RTOS systems
   - Available drivers: STM32 F4, F7, H5, H7; NXP RT1020; TI TM4C; Microchip SAME54; Wiznet W5500
   - A complete Web device dashboard on bare metal
  [Nucleo-F429ZI](examples/stm32/nucleo-f429zi-baremetal) is only 6 files
   - For comparison, a CubeIDE generated HTTP example is 400+ files
- Built-in firmware updates for STM32H5, STM32H7, and more coming
- Detailed [user guide, API reference and tons of tutorials](https://mongoose.ws/documentation/) available


# Commercial use
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


# Security

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


# Contributions

Contributions are welcome! Please follow the guidelines below:

- Sign [Cesanta CLA](https://cesanta.com/cla.html) and send GitHub pull request
- Make sure that PRs have only one commit, and deal with one issue only
