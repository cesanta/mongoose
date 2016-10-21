---
title: Build Options
items:
  - { type: file, name: enabling-flags.md }
  - { type: file, name: disabling-flags.md }
  - { type: file, name: platform-spec.md }
  - { type: file, name: tunables.md }
---

Mongoose source code ships in a single .c file that contains functionality
for all supported protocols (modules). Modules can be disabled at compile
time which reduces the executable's size. That can be done by setting preprocessor
flags. Also, some preprocessor flags can be used to tune internal Mongoose
parameters.

To set a preprocessor flag during compile time, use the `-D <PREPROCESSOR_FLAG>`
compiler option. For example, to disable both MQTT and CoAP,
compile the application `my_app.c` like this (assumed UNIX system):

```
  $ cc my_app.c mongoose.c -D MG_DISABLE_MQTT -D MG_DISABLE_COAP
```
