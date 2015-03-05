# Mongoose API Reference

    struct mg_server *mg_create_server(void *server_param, mg_handler_t handler);

Creates web server instance. Returns opaque instance pointer, or NULL if
there is not enough memory. `server_param`: Could be any pointer, or NULL.
This pointer will be passed
to the callback functions as `struct mg_connection::server_param` field.
A common use case is to pass `this` pointer of the C++ wrapper class
as `user_param`, to let the callback get the pointer to the C++ object.

Note that this function doesn't make the
server instance to serve. Serving is done by `mg_poll_server()` function.
Mongoose has single-threaded, event-driven, asynchronous, non-blocking core.
When server instance is created, it contains an information about
the configuration and the state of each connection.
Server instance is capable on listening on only one port. After creation,
`struct mg_server` has a list
of active connections and configuration parameters.

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

Sets a particular server option. Note that at least one option,
`listening_port`, must be specified. To serve static files, `document_root`
must be specified too. If `document_root` option is left unset, Mongoose
will not access filesystem at all. `mg_set_option()` returns NULL if option was
set successfully, otherwise it returns human-readable error string. It is
allowed to call `mg_set_option()` by the same thread that does
`mg_poll_server()` (Mongoose thread) and change server configuration while it
is serving, in between `mg_poll_server()` calls.

    int mg_poll_server(struct mg_server *server, int milliseconds);

Performs one iteration of IO loop by iterating over all
active connections, performing `select()` syscall on all sockets with a timeout
of `milliseconds`. When `select()` returns, Mongoose
does an IO for each socket that has data to be sent or received. Application
code must call `mg_poll_server()` in a loop. It is an error to have more then
one thread calling `mg_poll_server()`, `mg_set_option()` or any other function
that take `struct mg_server *` parameter. Mongoose does not
mutex-protect `struct mg_server *`, therefore only single thread
(Mongoose thread) should make Mongoose calls.

`mg_poll_server()` calls user-specified event handler when certain events
occur. Sequence of events for the accepted connection is this:

   * `MG_AUTH` - Mongoose asks whether this connection is authorized. If event
      handler returns `MG_FALSE`, then Mongoose does not serve the request but
      sends authorization request to the client. If `MG_TRUE` is returned,
      then Mongoose continues on with the request.
   * `MG_REQUEST` - Mongoose asks event handler to serve the request. If
      event handler serves the request by sending a reply,
      it should return `MG_TRUE`. Otherwise,
      it should return `MG_FALSE` which tells Mongoose that request is not
      served and Mongoose should serve it. For example, event handler might
      choose to serve only RESTful API requests with URIs that start with
      certain prefix, and let Mongoose serve all static files.
      If event handler decides to serve the request, but doesn't have
      all the data at the moment, it should return `MG_MORE`. That tells
      Mongoose to keep the connection open after callback returns.

      `mg_connection::connection_param` pointer is a placeholder to keep
      user-specific data. For example, handler could decide to open a DB
      connection and store DB connection handle in `connection_param`.
   * `MG_POLL` is sent to every connection on every iteration of
      `mg_poll_server()`. Event handler should return `MG_FALSE` to ignore
      this event. If event handler returns `MG_TRUE`, then Mongoose assumes
      that event handler has finished sending data, and Mongoose will
      close the connection.
   * `MG_HTTP_ERROR` sent when Mongoose is about to send HTTP error back
      to the client. Event handler can choose to send a reply itself, in which
      case event handler must return `MG_TRUE`. Otherwise, event handler must
      return `MG_FALSE`.
   * `MG_CLOSE` is sent when the connection is closed. This event is used
      to cleanup per-connection state stored in `connection_param`
      if it was allocated. Event handler return value is ignored.

Sequence of events for the client connection is this:

   * `MG_CONNECT` sent when Mongoose has connected to the remote host.
   This event is sent to the connection initiated by `mg_connect()` call.
   Connection status is held in `mg_connection::status_code`: if zero,
   then connection was successful, otherwise connection was not established.
   User should send a request upon successful connection.
   Event handler should return `MG_TRUE` if connection was successful and
   HTTP request has been sent. Otherwise, it should send `MG_FALSE`.
   * `MG_REPLY` is sent when response has been received from the remote host.
   If event handler sends another request, then it should return `MG_TRUE`.
   Otherwise it should return `MG_FALSE` and Mongoose will close the connection.
   * `MG_CLOSE` same as for the accepted connection.


When mongoose buffers in HTTP request and successfully parses it, it sends
`MG_REQUEST` event for GET requests immediately. For POST requests,
Mongoose delays the call until the whole POST request is buffered in memory.
POST data is available to the callback as `struct mg_connection::content`,
and POST data length is in `struct mg_connection::content_len`.

Note that websocket connections are treated the same way. Mongoose buffers
websocket frame in memory, and calls event handler when frame is fully
buffered. Frame data is available `struct mg_connection::content`, and
data length is in `struct mg_connection::content_len`, i.e. very similar to
the POST request. `struct mg_connection::is_websocket` flag indicates
whether the request is websocket or not. Also, for websocket requests,
there is `struct mg_connection::wsbits` field which contains first byte
of the websocket frame which URI handler can examine. Note that to
reply to the websocket client, `mg_websocket_write()` should be used.
To reply to the plain HTTP client, `mg_write_data()` should be used.

Return value: number of active connections.


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

    void mg_wakeup_server_ex(struct mg_server *, mg_handler_t func,
                             const char *fmt, ...);

Sends string message to a server. Function `func` is called for every active
connection. String message is passed in `struct mg_connection::callback_param`.
This function is designed to push data to the connected clients, and
can be called from any thread. There is a limitation on the length of
the message, currently at 8 kilobytes.

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
     set `Content-Length` header. Note that `mg_send_data()` and
     `mg_printf_data()` do not send data immediately. Instead, they spool
     data in memory, and Mongoose sends that data later after URI handler
     returns. If data to be sent is huge, an URI handler might
     send data in pieces by saving state in
     `struct mg_connection::connection_param` variable and returning `0`. Then
     Mongoose will call a handler repeatedly after each socket write.

<!-- -->

     void mg_send_file(struct mg_connection *, const char *path);

Tells Mongoose to serve given file. Mongoose handles file according to
it's extensions, i.e. Mongoose will invoke CGI script if `path` has CGI
extension, it'll render SSI file if `path` has SSI extension, etc. If `path`
points to a directory, Mongoose will show directory listing. If this function
is used, no calls to `mg_send*` or `mg_printf*` functions must be made, and
event handler must return `MG_MORE`.

    size_t mg_websocket_write(struct mg_connection* conn, int opcode,
                              const char *data, size_t data_len);
    size_t mg_websocket_printf(struct mg_connection* conn, int opcode,
                               const char *fmt, ...);


Similar to `mg_write()` and `mg_printf()`, but wraps the data into a
websocket frame with a given websocket `opcode`.

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
several ways of implementing authentication on the server side.
If password is not NULL, entry is added (or modified if already exists).
If password is NULL, entry is deleted.
Return: 1 on success, 0 on error.


    int mg_parse_multipart(const char *buf, int buf_len,
                           char *var_name, int var_name_len,
                           char *file_name, int file_name_len,
                           const char **data, int *data_len);

Parses a buffer that contains multipart form data. Stores chunk name
in a `var_name` buffer. If chunk is an uploaded file, then `file_name`
will have a file name. `data` and `data_len` will point to the chunk data.
Returns number of bytes to skip to the next chunk.

     struct mg_connection *mg_connect(struct mg_server *server,
                                      const char *host, int port, int use_ssl);

Create connection to the remote host. Returns `NULL` on error, non-null
if the connection has been scheduled for connection. Upon a connection,
Mongoose will send `MG_CONNECT` event to the event handler.
