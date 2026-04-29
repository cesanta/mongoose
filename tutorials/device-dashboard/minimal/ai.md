# Device dashboard

This is the AI specification file for the Web device dashboard.

## General rules

Follow these sections of the guide:
  - For editing HTML, https://mongoose.ws/docs/guides/web-dashboard-guide/#step-2-develop-dashboardhtml
  - For editing C, https://mongoose.ws/docs/guides/web-dashboard-guide/#step-3-develop-dashboardc

Never call Dashboard.call / Dashboard.on functions

For editing HTML file, only Mongoose `data-bind` and `${...}` template syntax,

No vanilla JS event listeners, no fetch calls, no custom reactive logic.


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
