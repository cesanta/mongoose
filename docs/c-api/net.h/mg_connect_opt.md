---
title: "mg_connect_opt()"
decl_name: "mg_connect_opt"
symbol_kind: "func"
signature: |
  struct mg_connection *mg_connect_opt(struct mg_mgr *mgr, const char *address,
                                       mg_event_handler_t handler,
                                       struct mg_connect_opts opts);
---

Connect to a remote host.

`address` format is `[PROTO://]HOST:PORT`. `PROTO` could be `tcp` or `udp`.
`HOST` could be an IP address,
IPv6 address (if Mongoose is compiled with `-DMG_ENABLE_IPV6`), or a host
name. If `HOST` is a name, Mongoose will resolve it asynchronously. Examples
of valid addresses: `google.com:80`, `udp://1.2.3.4:53`, `10.0.0.1:443`,
`[::1]:80`

See the `mg_connect_opts` structure for a description of the optional
parameters.

Returns a new outbound connection, or `NULL` on error.

NOTE: Connection remains owned by the manager, do not free().

NOTE: To enable IPv6 addresses, `-DMG_ENABLE_IPV6` should be specified
in the compilation flags.

NOTE: New connection will receive `MG_EV_CONNECT` as it's first event
which will report connect success status.
If asynchronous resolution fail, or `connect()` syscall fail for whatever
reason (e.g. with `ECONNREFUSED` or `ENETUNREACH`), then `MG_EV_CONNECT`
event report failure. Code example below:

```c
static void ev_handler(struct mg_connection *nc, int ev, void *ev_data) {
  int connect_status;

  switch (ev) {
    case MG_EV_CONNECT:
      connect_status = * (int *) ev_data;
      if (connect_status == 0) {
        // Success
      } else  {
        // Error
        printf("connect() error: %s\n", strerror(connect_status));
      }
      break;
    ...
  }
}

  ...
  mg_connect(mgr, "my_site.com:80", ev_handler);
``` 

