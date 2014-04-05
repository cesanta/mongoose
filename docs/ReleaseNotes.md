# Mongoose Release Notes

## Release 5.4

Changes in Libmongoose library:
   * Added `hexdump_file` option for low-level request/reply debugging
   * Added `mg_template()` API function for generating HTML pages from
     templates with expansions
   * Fixed `struct mg_connection::local_ip` handling, `mg_set_option()`
     behavior with NULL values
   * Removed Lua support

Changes in pre-compiled binaries:
   * Created HTML administration console
   * When server is started, browser is started automatically
   * Fixed directory listing bug when directory contains `#` character
   * Removed built-in Lua Server Pages in the binary, and instead
     added Mongoose + Lua developer bundle which has Lua Server Pages support.
     That also solves external Lua modules loading problem.


## Release 5.3, March 10 2014

Changes in Libmongoose library:

   * Moved to the evented API. Updated API documentation is at
       http://cesanta.com/docs/Embed.shtml
       http://cesanta.com/docs/API.shtml
   *  Added `MG_LUA` event for exporting custom variables to the Lua environment
   *  Added virtual hosts capability, see `url_rewrites` option description at
       http://cesanta.com/docs/Options.shtml
   *  Added mjpg serving example
   *  Cleaned up and documented HTTP client API, with unit tests
   *  Added `mg_wakeup_server()` to awaken `mg_poll_server()`
      from another thread
   *  Moved Mongoose IO core to [https://github.com/cesanta/net_skeleton](Net Skeleton)
   *  Added connection hexdump functionality for developers
   *  Bug fixes

Changes in pre-compiled binaries:

   *  New awesome Mongoose logos buy our designer Katrin - thanks Katrin!
        Check them out at http://cesanta.com/products.shtml
   *  Added Lua Server Pages support to the free version, quick intro is at
        http://cesanta.com/docs/Lua.shtml
   *  Added quick "Set shared directory" menu item to set `document_root`
   *  Added SSI support to the Pro version
   *  Removed SSL support from the Pro version

## Release 5.2, Feb 1 2014

   *  Windows binary made fully UNICODE aware. In previous versions,
      the presence of non-ASCII chars in document root, CGI script name,
      or directory name might have broken Mongoose as stand-alone
      or as Windows service. Now Mongoose works with non-ASCII paths properly.
      Internally, Mongoose uses UTF8 encoding. When making WinAPI calls,
      mongoose converts UTF8 strings to wide chars and calls UNICODE API.
   *  Enhanced authorization API by providing `mg_set_auth_handler()` and
      `mg_authorize_digest()`
   *  Removed `mg_add_uri_handler()`, added `mg_set_request_handler()`.
      There is only oneURI handler that handles all requests, just like in 4.x.
      The reason for this change is to provide an ability to catch all URIs,
      and at the same time signal Mongoose to continue handling specific URIs.
  *  Added `mg_parse_multipart()` API for file uploads.
      Note that the restriction on uploading huge files still exists,
      and will be eliminated in the next release.
  *  Allowing mongoose to bind to port 0, in which case it'll bind to any
      random unused port.
  *  Moved `idle_timeout_ms` run-time option to compile-time flag
  *  Added asynchronous HTTP client, not documented yet. Documentation and
      examples are coming in the next couple of weeks. Async Websocket client
      is scheduled for the next release. See usage examples at `unit_test.c`
  *  Windows and MacOS pre-built binaries are now split to free and paid ones,
    paid binaries include CGI, SSL, Lua, Sqlite, support and updates.
    Linux pre-built binary includes all functionality and is free, and will
    continue to be free.  Source code for Windows and MacOS GUI is closed.
    Disclaimer: source code for the command line stand-alone server,
    as well as Mongoose library itself, will never be closed.
  *  Multiple bug fixes and minor enhancements

## Release 5.1, Jan 10 2014

   *  CGI-related bugs where fixed, primarily for Windows platform
   *  Bugs on Windows related to UNICODE support were fixed
   *  Added a feature to support "error pages" through redirect.
      Done using `-url_redirects` option, details are on
      http://cesanta.com/docs/Options.shtml

## Release 5.0, Jan 6 2014

   *  Internal core has been changed from blocking, thread-per-connection to
      non-blocking, asynchronous, one thread for all.
   *  API modification for server creation and response creation. That allowed
     keep-alive support for dynamic requests, boosting the embedded performance
     to 100+ thousands requests per second on a single core
     (as measured on my development MacBook laptop)
   * Unified handling of POST requests and Websocket requests by putting a
    payload into `conn->content`, `conn->content_len` attributes.
    That simplified user code and eliminated the need of `mg_read()`,
    since mongoose buffers all data prior to calling the callback
   * keep-alive support is the default
   * Dropped SSI support and throttling support
   *  Several configuraition parameters are gone:
    *  `cgi_environment` (replaced with MONGOOSE_CGI),
    *  `protect_uri` (not useful)
    *  `ssi_pattern` (SSI support is gone)
    *  `throttle` (throttling support is gone)
    *  `error_log_file` (not used)
    *   `enable_keep_alive` (enabled by default)
    *   `listening_ports` (renamed to listening_port)
    *   `num_threads` (core has changed to single thread)
    *   `put_delete_auth_file` (renamed to dav_auth_file)
    *   `authentication_domain` (renamed to auth_domain)
   * Due to the async, non-blocking nature of the core, few restrictions
      are now in place:
    * user callbacks must not block
    * POST and Websocket data are now buffered, and cannot be huge
   * mongoose is now capable on listening on only one port

## Release 4.1, Oct 2014
## Release 4.0, Oct 2014
## Release 3.8, Sep 2014

## Release 3.7, Feb 2 2013

  *  Added "redirect to SSL port" functionality, e.g. if you specify
      `-listening_ports 8080r,8043s`
      then all requests to HTTP port 8080 will be redirected to HTTPS port 8043
  *  Added `mg_download()` API, an HTTP client interface!
  *  Lua server pages now must output HTTP headers -- full control for Lua
  *  Added pre-built binary for MacOS, with initial GUI support
  *  API change: got rid of events, moved to struct `mg_callbacks`
  *  Bugfixes, thanks to contributors


## Release 3.7, Jan 18 2013
  *  Fixed source code archive (main.c was missing)
  *  Extended Windows GUI functionality:
   * Added "Start browser" systray popup menu item
   *  Enhanced configuration editor
  *  Renamed config options:
   * `put_delete_passwords_file` -> `put_delete_auth_file`
   * `global_passwords_file` -> `global_auth_file`
  *  `select()` changed to `poll()`, to avoid big file descriptor
  `FD_SET` problem on UNIX
  *  Couple of bugfixes, thanks to contributors


Eearlier release notes could be found by searching
[Mongoose mailing list](https://groups.google.com/forum/#!forum/mongoose-users)
