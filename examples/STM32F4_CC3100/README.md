# STM32F4 example project

This example shows how to use mongoose on STM32 boards.
To run it you will need:
- [STM32F429-Discovery](http://www.st.com/content/st_com/en/products/evaluation-tools/product-evaluation-tools/mcu-eval-tools/stm32-mcu-eval-tools/stm32-mcu-discovery-kits/32f429idiscovery.html) dev board
- [CC3100](http://www.ti.com/product/CC3100) WiFi network processor

## Wiring scheme

By default, example uses SPI4 for communication with CC3100 and UART1 for 
the debug output. All parameters are described in file `stm32f4xx_hal_msp.h`,
they can be changed to use another SPI and/or UART

To use default scheme connect (CC3100 -> STM32-DISCO) connect:
```
DO  -> PE5
DIN -> PE6
CLK -> PE2
CS  -> PB12
IRQ -> PA0
HIB -> PB0
```

## Building firmware

Change `user_params.h`, put correct WiFi SSID and password there,
also change `MQTT_BROKER_ADDRESS` to the real broker address. 

`make` in `mongoose/STM32F4_CC3100` will download required docker image and make
firmware. Result will be in `STM32F4_CC3100/out` folder.

## Uploading firmare

Uploading firmware method depends on how you connected STM32 board to your
computer. If it is connected via USB ST-LINK connected it is appears as
a flash drive and in order to upload firmware just copy `out/example.bin`
to that drive.

## Running

Compile two additional samples: `mqtt_broker` and `mqtt_client` and run them 
in different terminals.
Press `reset` (or repower) STM board.
The board will connect to broker and will start to publish its uptime in 
`/stuff` channel. `mqtt_client` is subscribed on this channel as well, so 
it should start to print 

```
Got incoming message /stuff: Current tick: 99000
Forwarding to /test
Got incoming message /stuff: Current tick: 100120
Forwarding to /test
Got incoming message /stuff: Current tick: 101250
Forwarding to /test
...

```

If you connect UART to serial port monitor (pin PA9 or ST-LINK device, like /dev/ttyACM0)
you should see device output:
```
**** Hello ****
Initializing CC3100 on SPI4
Starting WiFi connect
Done, waiting for events
Connected to WiFi
Got IP
Connected to 192.168.1.108:1883
Connected to broker
Subscribing to /test
Subscription acknowledged
Publishing message with tick=1
Got incoming message /test: Current tick: 4487
Publishing message with tick=2
Got incoming message /test: Current tick: 5597
Publishing message with tick=3
Got incoming message /test: Current tick: 6717
...

```

This output looks like this because the device sends messages `Current tick: ....`
into `/stuff` channel, `mqtt_client` receives all messages in this channel
and sends them to `/test` channel. But the device is subscribed to this channel
so, it receives it back.
