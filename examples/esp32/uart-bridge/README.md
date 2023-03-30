# A UART to network bridge for ESP32

![](../../uart-bridge/screenshots/dashboard.png)

- See detailed tutorial at https://mongoose.ws/tutorials/esp32/uart-bridge/

## Flash pre-built firmware

You can flash a pre-built firmware to the ESP32 device using the following
instructions:

1. Connect your ESP32 device to the workstation. It should be accessible
   via a serial port
2. Download and unzip ESP32 flashing tool from https://mongoose.ws/downloads/esputil.zip
3. Download a prebuilt firmware https://mongoose.ws/downloads/uart-bridge.hex into the unzipped directory
4. Start command prompt (or terminal on Mac/Linux). Run `cd
  PATH/TO/esputil`  to go into the unzipped `esputil/` directory. After that, run
  the following command (change `COMPORT` to the board's serial port):

  | OS      | Command |
  | ------- | ------- |
  | Windows | <pre><code class="language-bash">.\windows\esputil -p COMPORT flash uart-bridge.hex</code></pre>|
  | Linux   | <pre><code class="language-bash">./linux/esputil -p COMPORT flash uart-bridge.hex</pre> |
  | MacOS   | <pre><code class="language-bash">./macos/esputil -p COMPORT flash uart-bridge.hex</code></pre> |

Next step is to monitor and follow the instructions.

```sh
esputil -p COMPORT monitor
```

Note: if monitor command shows constant restarts, the flash parameters
settings can be wrong. Reflash your device with `-fp ...` flash parameters
settings. For example, WROOM-32 based boards use `-fp 0x220`:

```sh
esputil -p COMPORT -fp 0x220 flash uart-bridge.hex
```

For more on possible options for flash parameters, see
https://github.com/cpq/esputil#flash-parameters
