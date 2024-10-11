# Socket activated http server with systemd

This example demonstrates a configuration for an HTTP server that is started only upon network request using systemd sockets. Socket activation is particularly beneficial for services in embedded systems, as it minimizes resource usage when the service is not active.

## Setup

1. install `libsystemd-dev`
1. compile example
1. create systemd socket description at /etc/systemd/system/mongoose.socket
```
[Unit]
Description=Socket listener

[Socket]
ListenStream=0.0.0.0:8443
Accept=no

```
4. create systemd service description at /etc/systemd/system/mongoose.service
```
[Unit]
Description=Socket listener

[Socket]
ListenStream=0.0.0.0:8443
Accept=no

[Install]
WantedBy = sockets.target
```
5. `sudo systemctl start mongoose.socket`
1. check `systemctl status mongoose` and `systemctl status mongoose.socket`
1. connecting to https://\<ip\>:8443 will start the webserver and forward the connection to mongoose
