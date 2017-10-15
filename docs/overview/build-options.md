# Build options

Mongoose source code ships in a single .c file that contains functionality
for all supported protocols (modules). Modules can be disabled at compile
time which reduces the executable's size. That can be done by setting preprocessor
flags. Also, some preprocessor flags can be used to tune internal Mongoose
parameters.

To set a preprocessor flag during compile time, use the `-D <PREPROCESSOR_FLAG>`
compiler option. For example, to disable both MQTT and CoAP,
compile the application `my_app.c` like this (assumed UNIX system):

```
  $ cc my_app.c mongoose.c -D MG_DISABLE_MQTT -D MG_DISABLE_COAP
```
## Enabling flags

- `MG_ENABLE_SSL` Enable [SSL/TLS support](https://docs.cesanta.com/mongoose/master/#/http/ssl.md/) (OpenSSL API)
- `MG_ENABLE_IPV6` Enable IPv6 support
- `MG_ENABLE_MQTT` enable [MQTT client](https://docs.cesanta.com/mongoose/master/#/mqtt/client_example.md/) (on by default, set to 0 to disable)
- `MG_ENABLE_MQTT_BROKER` enable [MQTT broker](https://docs.cesanta.com/mongoose/master/#/mqtt/server_example.md/)
- `MG_ENABLE_DNS_SERVER` enable DNS server
- `MG_ENABLE_COAP` enable CoAP protocol
- `MG_ENABLE_HTTP` Enable HTTP protocol support (on by default, set to 0 to disable)
- `MG_ENABLE_HTTP_CGI` Enable [CGI](https://docs.cesanta.com/mongoose/master/#/http/cgi.md/) support
- `MG_ENABLE_HTTP_SSI` Enable [Server Side Includes](https://docs.cesanta.com/mongoose/master/#/http/ssi.md/) support
- `MG_ENABLE_HTTP_SSI_EXEC` Enable SSI `exec` operator
- `MG_ENABLE_HTTP_WEBDAV` enable WebDAV extensions to HTTP
- `MG_ENABLE_HTTP_WEBSOCKET` enable WebSocket extension to HTTP (on by default, =0 to disable)
- `MG_ENABLE_BROADCAST` enable `mg_broadcast()` API
- `MG_ENABLE_GETADDRINFO` enable `getaddrinfo()` in `mg_resolve2()`
- `MG_ENABLE_THREADS` enable `mg_start_thread()` API

## Disabling flags

- `MG_DISABLE_HTTP_DIGEST_AUTH` disable HTTP Digest (MD5) authorisation support
- `CS_DISABLE_SHA1` disable SHA1 support (used by WebSocket)
- `CS_DISABLE_MD5` disable MD5 support (used by HTTP auth)
- `MG_DISABLE_HTTP_KEEP_ALIVE` useful for embedded systems to save resources

## Platform specific

Mongoose tries to detect the target platform whenever possible, but in some cases you have
to explicitly declare some peculiarities of your target, such as:

- `MG_CC3200`: enable workarounds for the TI CC3200 target.
- `MG_ESP8266`: enable workarounds for the ESP8266 target, add `RTOS_SDK` to specify the RTOS SDK flavour.

## Tunables

- `MG_MALLOC`, `MG_CALLOC`, `MG_REALLOC`, `MG_FREE` allow you to a use custom
  memory allocator, e.g. `-DMG_MALLOC=my_malloc`
- `MG_USE_READ_WRITE` when set replaces calls to `recv` with `read` and `send` with `write`,
  thus enabling to add any kind of file descriptor (files, serial devices) to an event manager.
- `MG_SSL_CRYPTO_MODERN`, `MG_SSL_CRYPTO_OLD` - choose either "Modern" or "Old" ciphers
  instead of the default "Intermediate" setting.
  See [this article](https://wiki.mozilla.org/Security/Server_Side_TLS#Recommended_configurations) for details.
- `MG_USER_FILE_FUNCTIONS` allow you to use custom file operation, by defining you own `mg_stat`, `mg_fopen`, `mg_open`, `mg_fread` and `mg_fwrite` functions

