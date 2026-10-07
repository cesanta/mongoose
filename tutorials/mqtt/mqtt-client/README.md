# Mongoose MQTT client

This MQTT client implementation implements the following:

- Connects to the MQTT server specified by MQTT_SERVER_URL
- When connected, subscribes to the topic MQTT_SUBSCRIBE_TOPIC
- When it receives a message, echoes it back to MQTT_PUBLISH_TOPIC
- Timer-based reconnection logic revives the connection when it is down
- Ping server periodically. When disconnected, a last will is published
- Implements "ota.update" for OTA updates, see https://mongoose.ws/mqtt/

By default, it uses HiveMQ public broker, and can be tested with
the [HiveMQ Websocket Client](https://www.hivemq.com/demos/websocket-client/).
Connect, subscribe to `mg/d3/#`, send a message to `mg/d3/rx`.

## Integrating into an embedded project

1. Copy `mongoose_mqtt.c` to your embedded project and add it to the build
2. Add `mg_mqtt_init(&mgr)` after `mg_mgr_init()`
3. Add `mg_mqtt_poll(&mgr)` after `mg_mgr_poll()`

## EMQX

See full walk-trough at https://mongoose.ws/articles/stm32-emqx/

## Amazon AWS IoT

1. Login to AWS console, choose AWS IoT
2. On the left menu, choose Connect / Domain configurations. Copy the domain
   name and update `MQTT_SERVER_URL`. Note to set "mqtts" and port 8883:
   ```c
   #define MQTT_SERVER_URL "mqtts://COPIED_DOMAIN:8883"
   #define MQTT_CLIENT_ID "d1"
   ```
3. Create EC CSR for device certificate:
   ```sh
   openssl req -new -newkey ec -pkeyopt ec_paramgen_curve:prime256v1   -pkeyopt ec_param_enc:named_curve -nodes   -keyout device.key -out device.csr -subj /CN=d1
   ```
3. On the left menu, click on Manage / All devices / Things,
   Create things, Create single thing, Set thing name to d1, No shadow,
   Next, Upload CSR (use device.csr created on a previous step), Next,
   Create Policy, name "AllowAll", set both action and resouce to `*`,
   Create, Attach AllowAll to the certificate, Create Thing.
4. Click on Things / d1 / Certificates, make the certificate active
5. Click on the certificate, download it, rename to "device.crt"
6. Set `TLS_CA`. Visit https://mongoose.ws/tls/, enter `DOMAIN_NAME:8883` into the CA field and click on the "Get CA Certificate" button. Enable the "Show as C/C++ constant", copy-paste to your code.
7. Set `TLS_KEY` and `TLS_CRT` with the output of this commands:
  ```sh
  sed 's/\r$//; s/.*/  "&\\n"/; $!s/$/ \\/' device.key
  sed 's/\r$//; s/.*/  "&\\n"/; $!s/$/ \\/' device.crt
  ```
8. Click on Test / MQTT test client, subscribe to topic `mg/#`, send a
   message to `mg/d1/rx`

## Microsoft Azure IoT Hub

See full walk-trough at https://mongoose.ws/articles/stm32-azure-iot-hub/

1. Create IoT Hub
2. Generate device self-signed certificates:
  ```sh
  openssl req -x509 -newkey ec -pkeyopt ec_paramgen_curve:prime256v1 -pkeyopt ec_param_enc:named_curve -nodes -keyout device.key -out device.crt -days 3650 -subj "/CN=MYDEVICE"
  ```
3. Show certificate thumbprint:
  ```sh
  openssl x509 -in device.crt -noout -fingerprint -sha256 | tr -d ':' | cut -d= -f2
  ```
4. Register a device with the thumbprint from above, and set the following:
  ```c
#define AZURE_HUB_NAME "HUB_NAME"    // Change this
#define AZURE_DEVICE_ID "DEVICE_ID"  // Change this

// Do not change this
#define MQTT_SERVER_URL "mqtts://" AZURE_HUB_NAME ".device.azure-devices.net"
#define MQTT_CLIENT_ID AZURE_DEVICE_ID
#define MQTT_USER AZURE_HUB_NAME ".azure-devices.net/" AZURE_DEVICE_ID "/?api-version=2021-04-12"
#define MQTT_PUBLISH_TOPIC "devices/" AZURE_DEVICE_ID "/messages/events/"
#define MQTT_SUBSCRIBE_TOPIC "devices/" AZURE_DEVICE_ID "/messages/devicebound/#"
  ```

5. Set `TLS_CA`. Visit https://mongoose.ws/tls/, enter "HUB_NAME.device.azure-devices.net:8883" into the CA field and click on the "Get CA Certificate" button. Enable the "Show as C/C++ constant", copy-paste to your code.
6. Set `TLS_KEY` and `TLS_CRT` with the output of this commands:

```sh
sed 's/\r$//; s/.*/  "&\\n"/; $!s/$/ \\/' device.key
sed 's/\r$//; s/.*/  "&\\n"/; $!s/$/ \\/' device.crt
```
7. Add to your `mongoose_config.h`:
```c
#define MG_ENABLE_CHACHA20 0
```

## Microsoft Azure Event Grid

1. Create Azure Event Grid (EG) instance

2. Generate device self-signed certificates:

```sh
openssl req -x509 -newkey ec -pkeyopt ec_paramgen_curve:prime256v1 -pkeyopt ec_param_enc:named_curve -nodes -keyout device.key -out device.crt -days 3650 -subj "/CN=MYDEVICE"
```

3. Show certificate thumbprint:

```sh
openssl x509 -in device.crt -noout -fingerprint -sha256 | tr -d ':' | cut -d= -f2
```

4. Register a client on EG: choose a name, and use the thumbprint from above

5. Set URL, username and client ID:

```c
#define MQTT_SERVER_URL "mqtts://INSTANCE.REGION.ts.eventgrid.azure.net:8883"
#define MQTT_CLIENT_ID "CLIENT_NAME"
#define MQTT_USER "CLIENT_NAME"
```

6. Set `TLS_CA`. Visit https://mongoose.ws/tls/, enter "INSTANCE.REGION.ts.eventgrid.azure.net:8883" into the CA field and click on the "Get CA Certificate" button. Enable the "Show as C/C++ constant", copy-paste to your code.
7. Set `TLS_KEY` and `TLS_CRT` with the output of this commands:

```sh
sed 's/\r$//; s/.*/  "&\\n"/; $!s/$/ \\/' device.key
sed 's/\r$//; s/.*/  "&\\n"/; $!s/$/ \\/' device.crt
```

8. In Azure EG, go to Instance / MQTT Broker / Topic spaces, add "space1" with `mg/#` pattern
9. In Azure EG, go to Instance / MQTT Broker / Permissions bindings, add binding1 and binding2:
  - $all , space1 , Publisher
  - $all , space1 , Subscriber
