# Device dashboard

This is the AI specification file for the Web device dashboard.

## General rules

Your goal is to produce a dashboard.c and dashboard.html files

Fetch the initial implementation of these two files from:

https://github.com/cesanta/mongoose/blob/master/tutorials/device-dashboard/minimal/dashboard.c
https://github.com/cesanta/mongoose/blob/master/tutorials/device-dashboard/minimal/dashboard.html

The C API for dashboard.c is described at https://mongoose.ws/docs/api/dash/

The C API header file is at https://github.com/cesanta/mongoose/blob/master/src/dash.h

The minimal reference project is at https://github.com/cesanta/mongoose/blob/master/tutorials/device-dashboard/minimal/

The full reference project is at https://github.com/cesanta/mongoose/blob/master/tutorials/device-dashboard/full/

For editing dashboard.html, use only Mongoose `data-bind` and `${...}` template syntax

No vanilla JS event listeners, no fetch calls, no custom reactive logic.

## Functionality

### LED controls

On a page, create single control panel titled "LED control"

On that control, add 3 LED toggles: LED1, LED2, LED3

Update Dashboard.init call accordingly. Update dashboard.c accordingly - have 3 LEDs in the "leds" data set.


## dashboard.html

Start with this empty skeleton file:

```html
<!doctype html>
<html lang="en">
  <head>
    <meta charset="utf-8" />
    <meta name="viewport" content="width=device-width, initial-scale=1" />
    <script src="https://cdn.jsdelivr.net/gh/cesanta/mongoose@master/resources/dashboard.js"></script>
    <script>
      Dashboard.init({
        debug: true,
        data: { status: { led1: false, version: '1.0.0' } },
      });
    </script>
  </head>
  <body>
  </body>
</html>
```

Add a visually appealing toggle button - use an <input type="checkbox" />
for it, stylised as a toggle button.

Add "data-bind" and "data-autosave" attributes to the toggle control
in order to bind it to the device: <input type="checkbox" data-bind="status.led1" data-autosave="1" />

## dashboard.c

Don't touch it
