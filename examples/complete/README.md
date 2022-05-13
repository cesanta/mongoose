# A complete device dashboard

This example is a demonstration of how Mongoose Library could be integrated
into an embedded device and provide a complete device dashboard with the
following features:

- Multiple logins with different permissions (admin and user)
- Login screen for non-authenticated connections
- A [preact](https://preactjs.com/)-based dashboard with multiple pages
- Web UI is fully embedded into the server/firmware binary, and does not
  need a filesystem to serve it
- View and change server settings
- All changes are propagates to all connected clients
- Live interactive chat that demonstrates bi-directional data exchange


# Screenshots

This is a login screen that prompts for user/password

![](screenshots/login.png)

# Main dashboard

A main dashboard page shows internal data, live video stream, and a form

![](screenshots/dashboard.png)

A live log page shows live log stream coming from device

![](screenshots/logs.png)
