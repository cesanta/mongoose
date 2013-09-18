# Mongoose Server Embedding Guide And API Reference

Embedding Mongoose is easy. Copy
[mongoose.c](https://github.com/cesanta/mongoose/blob/master/mongoose.c) and
[mongoose.h](https://github.com/cesanta/mongoose/blob/master/mongoose.h)
to your application's source tree and include them in the build. For
example, your application's code lives in C file `my_app.c`, then on UNIX
this command embeds Mongoose:

    $ ls
    my_app.c mongoose.c mongoose.h
    $ gcc my_app.c mongoose.c -o my_app -ldl -pthread

Somewhere in the application code, call `mg_start()` to start the server.
Pass configuration options and event handlers to `mg_start()`.
Mongoose then calls handlers when certain events happen.
For example, when new request arrives, Mongoose calls `begin_request`
handler function to let user handle the request. In the handler, user code
can get all information about the request -- parsed headers, etcetera.

Mongoose API is logically divided in three categories: server setup/shutdown
functions, functions to be used by user-written event handlers, and
convenience utility functions.

### Starting and stopping embedded web server
To start the embedded web server, call `mg_start()`. To stop it, call
`mg_stop()`.

    // This structure needs to be passed to mg_start(), to let mongoose know
    // which callbacks to invoke.
    struct mg_callbacks {
      int  (*begin_request)(struct mg_connection *);
      void (*end_request)(const struct mg_connection *, int reply_status_code);
      int  (*log_message)(const struct mg_connection *, const char *message);
      int  (*init_ssl)(void *ssl_context);
      int (*websocket_connect)(const struct mg_connection *);
      void (*websocket_ready)(struct mg_connection *);
      int  (*websocket_data)(struct mg_connection *);
      const char * (*open_file)(const struct mg_connection *,
                                 const char *path, size_t *data_len);
      void (*init_lua)(struct mg_connection *, void *lua_context);
      void (*upload)(struct mg_connection *, const char *file_name);
      int  (*http_error)(struct mg_connection *, int status);
    };

[hello.c](https://github.com/cesanta/mongoose/blob/master/examples/hello.c)
provides a minimalistic example.

Common pattern is to implement `begin_request` callback, and serve static files
from memory, and/or construct dynamic replies on the fly. Here is
my [embed.c](https://gist.github.com/valenok/4714740) gist
that shows how to easily any data can be embedded
directly into the executable. If such data needs to be encrypted, then
encrypted database or encryption dongles would be a better choice.


