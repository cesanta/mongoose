# Mongoose API Reference

    struct mg_server *mg_create_server(void *server_param);

Creates web server instance. Returns opaque instance pointer, or NULL if
there is not enough memory. `server_param`: Could be any pointer, or NULL.
This pointer will be passed
to the callback functions as `struct mg_connection::server_param` field.
A common use case is to pass `this` pointer of the C++ wrapper class
as `user_param`, to let the callback get the pointer to the C++
object.

Note that this function doesn't make the
server instance to serve. Serving is done by `mg_poll_server()` function.
Mongoose has single-threaded, event-driven, asynchronous, non-blocking core.
When server instance is created, it contains an information about
the configuration and the state of each connection.
Server instance is capable on listening on only one port. After creation,
`struct mg_server` has a list
of active connections, initially empty. It has a list of URI handlers,
initially empty, and configuration parameters. Configuration can be
altered by `mg_set_option()`, and new URI handlers could be added by
`mg_add_uri_handler()`.

Side-effect: on UNIX, `mg_create_server()` ignores SIGPIPE signals. If custom
processing is required SIGPIPE, signal handler must be set up after
calling `mg_create_server()`.

Important: Mongoose does not install `SIGCHLD` handler. If CGI is used,
`SIGCHLD` handler must be set up to reap CGI zombie processes.


    void mg_destroy_server(struct mg_server **server);

Deallocates web server instance, closes all pending connections, and makes
server pointer a NULL pointer.

    const char mg_set_option(struct mg_server *server, const char *name,
                             const char *value);

Sets a particular server option. Please refer to a separate documentation page
that lists all available option names. Note that at least one option,
`listening_port`, must be specified. To serve static files, `document_root`
must be specified too. If `document_root` option is left unset, Mongoose
will not access filesystem at all. This function returns NULL if option was
set successfully, otherwise it returns human-readable error string. It is
allowed to call `mg_set_option()` by the same thread that does
`mg_poll_server()` (an IO thread) and change server configuration while it
is serving, in between `mg_poll_server()` calls.

    void mg_poll_server(struct mg_server *server, int milliseconds);

This function performs one iteration of IO loop by iterating over all
active connections, performing `select()` syscall on all sockets with a timeout
of `milliseconds` number of milliseconds. When `select()` returns, Mongoose
does an IO for each socket that has data to be sent or received. Application
code must call `mg_poll_server()` in a loop. It is an error to have more then
one thread calling `mg_poll_server()`, `mg_set_option()` or any other function
that take `struct mg_server *` parameter. Mongoose does not
mutex-protect `struct mg_server *`, therefore the best practice is
to call server management functions from the same thread (an IO thread).

    void mg_add_uri_handler(struct mg_server *, const char *uri, mg_handler_t);

Adds an URI handler. If Mongoose gets a request and request's URI starts
with `uri`, then specified handler is called to serve the request. Thus, an
`uri` is a match prefix. For example, if `uri` is "/", then all requests will
be routed to the handler, because all URIs start with `/` character.

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

    void mg_set_http_error_handler(struct mg_server *, mg_handler_t);

Adds HTTP error handler. An actual HTTP error is passed as
`struct mg_connection::status_code` parameter. If handler returns 0, it
means a handler has not processed the connection, and mongoose proceeds
with sending HTTP error to the client. Otherwise, mongoose does nothing.

    const char **mg_get_valid_option_names(void);

Returns a NULL-terminated array of option names and their default values.
There are two entries per option in an array: an option name followed by a
default value. A default value could be NULL. A NULL name indicates an end
of the array.

    const char *mg_get_option(const struct mg_server *server, const char *name);

Returns the value of particular configuration parameter. If
given parameter name is not valid, NULL is returned. For valid names, return
value is guaranteed to be non-NULL. If parameter is not set, zero-length string
is returned.


    int mg_iterate_over_connections(struct mg_server *,
                                void (*func)(struct mg_connection *, void *),
                                void *param);

This is an interface primarily designed to push arbitrary data to websocket
connections at any time. This function could be called from any thread. When
it returns, an IO thread called `func()` on each active connection,
passing `param` as an extra parameter. It is allowed to call `mg_send_data()` or
`mg_websocket_write()` within a callback, cause `func` is executed in the
context of the IO thread.

    void mg_send_status(struct mg_connection *, int status_code);
    void mg_send_header(struct mg_connection *, const char *name,
                        const char *value);
    void mg_send_data(struct mg_connection *, const void *data, int data_len);
    void mg_printf_data(struct mg_connection *, const char *format, ...);

These functions are used to construct a response to the client. HTTP response
consists of three parts: a status line, zero or more HTTP headers,
a response body.  Mongoose provides functions for all three parts:
   * `mg_send_status()` is used to create status line. This function can be
      called zero or once. If `mg_send_status()` is not called, then Mongoose
      will send status 200 (success) implicitly.
   * `mg_send_header()` adds HTTP header to the response. This function could
      be called zero or more times.
   * `mg_send_data()` and `mg_printf_data()` are used to send data to the
     client. Note that Mongoose adds `Transfer-Encoding: chunked` header
     implicitly, and sends data in chunks. Therefore, it is not necessary to
     set `Content-Length` header.

<!-- -->

    int mg_websocket_write(struct mg_connection* conn, int opcode,
                           const char *data, size_t data_len);

Similar to `mg_write()`, but wraps the data into a websocket frame with a
given websocket `opcode`. This function is available when mongoose is
compiled with `-DUSE_WEBSOCKET`.  

    const char *mg_get_header(const struct mg_connection *, const char *name);

Get the value of particular HTTP header. This is a helper function.
It traverses http_headers array, and if the header is present in the array,
returns its value. If it is not present, NULL is returned.


    int mg_get_var(const struct mg_connection *conn, const char *var_name,
                   char *buf, size_t buf_len);

Gets HTTP form variable. Both POST buffer and query string are inspected.
Form variable is url-decoded and written to the buffer. On success, this
function returns the length of decoded variable. On error, -1 is returned if
variable not found, and -2 is returned if destination buffer is too small
to hold the variable. Destination buffer is guaranteed to be
'\0' - terminated if it is not NULL or zero length.

    int mg_parse_header(const char *hdr, const char *var_name, char *buf,
                        size_t buf_size);

This function parses HTTP header and fetches given variable's value in a buffer.
A header should be like `x=123, y=345, z="other value"`. This function is
designed to parse Cookie headers, Authorization headers, and similar. Returns
the length of the fetched value, or 0 if variable not found.

    int mg_modify_passwords_file(const char *passwords_file_name,
                                 const char *domain,
                                 const char *user,
                                 const char *password);

Add, edit or delete the entry in the passwords file.  
This function allows an application to manipulate .htpasswd files on the
fly by adding, deleting and changing user records. This is one of the
several ways of implementing authentication on the server side. For another,
cookie-based way please refer to the examples/chat.c in the source tree.  
If password is not NULL, entry is added (or modified if already exists).
If password is NULL, entry is deleted.  
Return: 1 on success, 0 on error.
