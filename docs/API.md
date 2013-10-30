# Mongoose Embedding Guide

Embedding Mongoose is done in two steps:

   1. Copy
    [mongoose.c](https://raw.github.com/cesanta/mongoose/master/mongoose.c) and
    [mongoose.h](https://raw.github.com/cesanta/mongoose/master/mongoose.h)
    to your application's source tree and include these two files in the build.
   2. Somewhere in the application code, call `mg_start()` to start the server.
    Pass configuration options and event handlers to `mg_start()`. Call
    `mg_stop()` when server needs to be stopped.

Mongoose calls event handlers when certain events happen.
For example, when new request arrives, Mongoose calls `begin_request`
handler to let user handle the request. In the handler, user code
can get all information about the request -- parsed headers, etcetera.
Here is a list of well-commented embedding examples:

   * [hello.c](https://github.com/cesanta/mongoose/blob/master/examples/hello.c)
    This is the most basic "Hello, world!" example
   * [post.c](https://github.com/cesanta/mongoose/blob/master/examples/post.c)
    This example shows how to handle form submission
   * [upload.c](https://github.com/cesanta/mongoose/blob/master/examples/upload.c)
    This example shows how to handle file upload
   * [websocket.c](https://github.com/cesanta/mongoose/blob/master/examples/websocket.c)
    This example shows how to handle websocket requests
   * [chat.c](https://github.com/cesanta/mongoose/blob/master/examples/chat.c), [main.js](https://github.com/cesanta/mongoose/blob/master/examples/html/main.js)
    An example of web chat application, with cookie-based user authentication,
    session support. All UI is done using static HTML/CSS. Interaction
    with backed is done using AJAX.



# API Reference

    struct mg_context *mg_start(const char **configuration_options
                                int (*event_handler_func)(struct mg_event *),
                                void *user_data);

Starts mongoose web server. This function starts a separate master thread,
which opens listening sockets, and `num_threads` worker threads, which are
used to handle incoming requests.

  `options`: NULL terminated list of option_name, option_value pairs that
            specify Mongoose configuration parameters.  
  `event_handler`: a function that will be called on specific events,
               see description below.  
  `user_data`: Opaque pointer, used by application developer to store
               global private data.  
  Return: web server context, or NULL on error.

  Side-effects: on UNIX, `mg_start()` ignores `SIGPIPE` signals. If custom
    processing is required `SIGPIPE`, signal handler must be set up
    after calling `mg_start()`.

 Important: Mongoose does not install `SIGCHLD` handler. If CGI is used,
 `SIGCHLD` handler must be set up to reap CGI zombie processes.


    void mg_stop(struct mg_context *);

Stop the web server. This function blocks until all Mongoose
threads are stopped. Context pointer becomes invalid.


## Events triggered by Mongoose

Every time an event happens, such as new connection being made,
Mongoose calls user-specified event handler. Mongoose passes `struct mg_event`
structure to the event handler, which event handler can use to find any
information required to handle an event:

    struct mg_event {
      int type;               // Event type
      void *user_data;        // User data pointer passed to mg_start()
      void *conn_data;        // Connection-specific, per-thread user data.
      void *event_param;      // Event-specific parameter
      struct mg_connection *conn;
      struct mg_request_info *request_info;
    };

Below is a list of all events triggered by Mongoose:

### MG\_REQUEST\_BEGIN

Called when Mongoose has received and successfully parsed new HTTP request.
`request_info`
attribute of `struct mg_event` contains parsed HTTP request. Return value tells
mongoose what to do next. If event handler returns 0, that means that the
handler did not process the request, did not send any data to the client, and
expects Mongoose to continue processing the request. Returning non-zero
tells Mongoose to stop doing any processing, cause callback already sent
valid reply to the client.

### MG\_REQUEST\_END

Called when mongoose has finished processing the request.
Could be used to implement custom request logging, request execution time
profiling, etcetera. Return value is ignored by Mongoose.

### MG\_HTTP\_ERROR

Called when Mongoose is about to send HTTP error to the client.
`event_param` attribute contains integer HTTP error code, that could be
accessed like this:  
`int status_code = (int) (long) event->event_param;`  
If handler returns zero, then Mongoose proceeds with sending error to the
client, otherwise Mongoose will not send anything.

### MG\_EVENT\_LOG

Called when Mongoose wants to log an error message.
Normally, error messages are logged to the error log file. If handler
returns 0, mongoose will not log to the log file. `event_param` holds
a message to be logged:  
`const char *message = (const char *) event->event_param;`

### MG\_THREAD\_BEGIN

Called when Mongoose starts a new thread. Handler will be executing
in the context of that new thread. It is used to perform any extra per-thread
initialization. Return value is ignored by Mongoose.

### MG\_THREAD\_END

Called when Mongoose is about to terminate a thread. Used to clean up
the state initialized by `MG_THREAD_BEGIN` handling. Return value is ignored.

    const char *mg_get_option(const struct mg_context *ctx, const char *name);

Get the value of particular configuration parameter.  The value returned is
read-only. Mongoose does not allow changing configuration at run time.  If
given parameter name is not valid, NULL is returned. For valid names, return
value is guaranteed to be non-NULL. If parameter is not set, zero-length string
is returned.

    const char **mg_get_valid_option_names(void);

Return array of strings that represent valid configuration options.  For each
option, option name and default value is returned, i.e. the number of entries
in the array equals to number_of_options x 2.  Array is NULL terminated.


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


    int mg_write(struct mg_connection *, const void *buf, int len);

Send data to the client. This function guarantees to send all requested data.
If more then one thread is writing to the connection, writes must be
serialized by e.g. using mutex.  
Return: number of bytes written to the client. If return value is less then
`len`, it is a failure, meaning that client has closed the connection.


    int mg_websocket_write(struct mg_connection* conn, int opcode,
                           const char *data, size_t data_len);

Send data to a websocket client. If more then one thread is writing to the
connection, writes must be serialized by e.g. using mutex. This function
guarantees to send all data (semantic is similar to `mg_write()`).
This function is available when mongoose is compiled with `-DUSE_WEBSOCKET`.  
Return: number of bytes written to the client. If return value is less then
`data_len`, it is a failure, meaning that client has closed the connection.


## Embedding Examples

The common pattern is to handle `MG_REQUEST_BEGIN` and serve static files
from memory, and/or construct dynamic replies on the fly. Here is
my [embed.c](https://gist.github.com/valenok/4714740) gist
that shows how to easily any data can be embedded
directly into the executable. If such data needs to be encrypted, then
encrypted database or encryption dongles would be a better choice.
