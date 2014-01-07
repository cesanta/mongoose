# Mongoose Internals

Mongoose has single-threaded, event-driven, asynchronous, non-blocking core.
`mg_create_server()` creates a web server instance. An instance is a container
for the config options and list of active connections. To do the actual
serving, user must call `mg_poll_server()`, which iterates over all
active connections, performing `select()` syscall on all sockets with a
timeout of specified number of milliseconds. When `select()` returns, Mongoose
does an IO for each socket that has data to be sent or received. Application
code must call `mg_poll_server()` in a loop.

Mongoose server instance is designed to be used by a single thread.
It is an error to have more then
one thread calling `mg_poll_server()`, `mg_set_option()` or any other function
that take `struct mg_server *` parameter. Mongoose does not
mutex-protect `struct mg_server *`, therefore the best practice is
to call server management functions from the same thread (an IO thread).
On a multi-core systems, many server instances can be created, sharing the
same listening socket and managed by separate threads (see [multi_threaded.c](https://github.com/cesanta/mongoose/blob/master/examples/multi_threaded.c))
example.

It is an error to pass and store `struct mg_connection *` pointers for
later use to send data. The reason is that they can be invalidated by the
next `mg_poll_server()` call. For such a task,
there is `mg_iterate_over_connections()` API
exists, which sends a callback function to the IO thread, then IO thread
calls specified function for all active connection.

When mongoose buffers in HTTP request and successfully parses it, it calls
appropriate URI handler immediately for GET requests. For POST requests,
Mongoose delays the call until the whole POST request is buffered in memory.
POST data is available to the callback as `struct mg_connection::content`,
and POST data length is in `struct mg_connection::content_len`.

Note that websocket connections are treated the same way. Mongoose buffers
websocket frame in memory, and calls URI handler when frame is fully
buffered. Frame data is available `struct mg_connection::content`, and
data length is in `struct mg_connection::content_len`, i.e. very similar to
the POST request. `struct mg_connection::is_websocket` flag indicates
whether the request is websocket or not. Also, for websocket requests,
there is `struct mg_connection::wsbits` field which contains first byte
of the websocket frame which URI handler can examine. Note that to
reply to the websocket client, `mg_websocket_write()` should be used.
To reply to the plain HTTP client, `mg_write()` should be used.
