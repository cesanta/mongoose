# A complete device dashboard

This example is a demonstration of how Mongoose Library could be integrated
into an embedded device and provide a complete device dashboard with the
following features:

- Authentication: login-protected dashboard
- Multiple logins with different permissions (admin and user)
- Web UI is fully embedded into the server/firmware binary, and does not
  need a filesystem to serve it. UI is resilient to FS problems
- Administrators can change server settings
- All changes are propagated to all connected clients
- The device is connected to the external MQTT server
- Logged in clients can send/receive messages via MQTT

## Screenshots

This is a login screen that prompts for user/password

![](screenshots/login.webp)

## Main dashboard

The main dashboard page shows the interactive MQTT console

![](screenshots/dashboard.webp)


See a detailed tutorial at https://mongoose.ws/tutorials/device-dashboard/
