---
title: Platform specific
---

Mongoose tries to detect the target platform whenever possible, but in some cases you have
to explicitly declare some peculiarities of your target, such as:

- `MG_CC3200`: enable workarounds for the TI CC3200 target.
- `MG_ESP8266`: enable workarounds for the ESP8266 target, add `RTOS_SDK` to specify the RTOS SDK flavour.
