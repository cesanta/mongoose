
# RM2 with a NUCLEO-F746ZG

The RM2 is a breakout containing the Wi-Fi subsection of a Raspberry Pi Pico W (or Pico 2 W), that is, a CYW43439, related circuitry, and antenna.

Connect both modules as pin definitions in hal.h suggest, **keep short wires**


| Signal  | GPIO | CN7 pin |             | RM2 pin | RM2 signal |
|---------|------|---------|-------------|---------|------------|
| DATAPIN | PB5  | 13      | ───┐─────── | 9       | DAT        |
| MISOPIN | PB4  | 19      | ───┘        |         |            |
| CLKPIN  | PB3  | 15      | ─────────── | 8       | CLK        |
| CSPIN   | PA4  | 17      | ─────────── | 10      | CS         |
| PWRPIN  | PC7  | 11      | ─────────── | 11      | WL ON      |

| Signal  | GPIO | CN8 pin |             | RM2 pin | RM2 signal |
|---------|------|---------|-------------|---------|------------|
| +3V3    |      | 7       | ─────────── | 12      | 3V3        |
| GND     |      | 11      | ─────────── | 7       | (-)        |
