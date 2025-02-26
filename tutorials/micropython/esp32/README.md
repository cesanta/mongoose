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

[Please check the Linux example for more information and useful links](../README.md)
