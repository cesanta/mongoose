# A complete device dashboard

This example is a demonstration of how Mongoose Library could be integrated
into an embedded device and provide a complete device dashboard with the
following features:

- Authentication: login-protected dashboard
- Multiple logins with different permissions (admin and user)
- Web UI is fully embedded into the server/firmware binary, and does not
  need a filesystem to serve it. UI is resilient to FS problems
- View and change server settings
- All changes are propagates to all connected clients
- Live interactive chat that demonstrates bi-directional data exchange

# Screenshots

This is a login screen that prompts for user/password

![](screenshots/login.png)

# Main dashboard

A main dashboard page shows device settings form, and live chat

![](screenshots/dashboard.png)
