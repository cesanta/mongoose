# Design Concept

Mongoose has three basic data structures:

- `struct mg_mgr` is an event manager that holds all active connections
- `struct mg_connection` describes a connection
- `struct mbuf` describes data buffer (received or sent data)

Connections could be either *listening*, *outbound* or *inbound*. Outbound
connections are created by the `mg_connect()` call. Listening connections are
created by the `mg_bind()` call. Inbound connections are those accepted by a
listening connection. Each connection is described by the `struct mg_connection`
structure, which has a number of fields like socket, event handler function,
send/receive buffer, flags, etc.

An application that uses mongoose should follow a standard pattern of
event-driven application:

1. declare and initialise event manager:

    ```c
    struct mg_mgr mgr;
    mg_mgr_init(&mgr, NULL);
    ```
2. Create connections. For example, a server application should create
   listening connections:

   ```c
    struct mg_connection *c = mg_bind(&mgr, "80", ev_handler_function);
    mg_set_protocol_http_websocket(c);
   ```

3. create an event loop by calling `mg_mgr_poll()` in a loop:

    ```c
    for (;;) {
      mg_mgr_poll(&mgr, 1000);
    }
    ```

`mg_mgr_poll()` iterates over all sockets, accepts new connections, sends and
receives data, closes connections and calls event handler functions for the
respective events. For the full example, see
[Usage Example](#/overview/usage-example.md/)
which implements TCP echo server.
