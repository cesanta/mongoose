# Mongoose as a MicroPython user module on the ESP32

The most basic usage is:

```python
import network
import mongoose

sta_if = network.WLAN(network.STA_IF)
sta_if.active(True)                      
sta_if.connect("<SSID>", "<PASSWORD>")
while not sta_if.isconnected():
    pass

mongoose.init()
while True:
    mongoose.poll(1000)

```

Check [_thread](https://docs.micropython.org/en/latest/library/_thread.html) support for more advanced usage

https://docs.micropython.org/en/latest/develop/cmodules.html
