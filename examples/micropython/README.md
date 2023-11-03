# Mongoose as a MicroPython user module

The most basic usage is in `main.py`.

You'll have to configure Mongoose for your desired platform, this example runs on the `unix` port and Mongoose detects it as a known architecture and hence configures itself to use sockets and time base. For other platforms, you'll have to do your homework (there's plenty of Mongoose examples for several platforms); also check respective MicroPython build instructions.

Check [_thread](https://docs.micropython.org/en/latest/library/_thread.html) support for more advanced usage

https://docs.micropython.org/en/latest/develop/cmodules.html
