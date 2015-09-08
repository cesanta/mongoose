# Arduino Restful Server

This example demonstrates how to use [Mongoose](https://www.cesanta.com/mongoose) to control Arduino
using HTTP requests.
Example just blinks by LED when Mongoose receives HTTP command, but it can be modified to execute any user-specific code.

At the moment this example supports [Arduino Mega 2560](http://arduino.cc/en/Main/ArduinoBoardMega2560) board (and compatible) with either W5100-based
network shield (like [Arduino Ethernet Shield](http://arduino.cc/en/Main/ArduinoEthernetShield)) or [CC3000](http://www.ti.com/product/cc3000)-based WIFI Shield.

## Build and run instructions:

###To run with Arduino Ethernet (W5100) shield:
1. Add (Sketch->Add file...) the following files to sketch:
     - /mongoose/mongoose.h
     - /mongoose/mongoose.c
     - /mongoose/platforms/arduino_ethernet_W5100/avrsupport.h
     - /mongoose/platforms/arduino_ethernet_W5100/avrsupport.cpp
2. Make `board_ip` and `board_mac` variables suitable for your network and board
3. Uncomment line `#include <Ethernet.h>`
4. Compile and flash sketch
5. Run `curl http://<board_ip/blink`
   LED attached to PIN 13 will blink and board free memory size and board uptime will be displayed.


###To run with Adafruit WiFi (CC3000) shield:
1. Add (Sketch->Add files...) the following files to sketch:
     - /mongoose/mongoose.h
     - /mongoose/mongoose.c
     - /mongoose/platforms/arduino_ethernet_W5100/avrsupport.h
     - /mongoose/platforms/arduino_ethernet_W5100/avrsupport.cpp
2. Import Adafruit CC3000 library for mongoose
    (select Sketch->Import Library...->Add library... and point
     /mongoose/platforms/arduino_wifi_CC3000/adafruit_CC3000_lib_mongoose folder)
3. Make the following variables suitable for your network
     - `board_ip`
     - `subnet_mask`
     - `gateway`
     - `dns`
     - `wlan_ssid`
     - `wlan_pwd`
     - `wlan_security`
5. Uncomment line `#include <Adafruit_CC3000.h>`
4. Compile and flash sketch
5. Run curl `http://<board_ip/blink`
   LED attached to PIN 13 will blink and board free memory size and board uptime will be displayed.
