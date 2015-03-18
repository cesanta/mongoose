# Mongoose Release Notes

## Release 5.6, 2015-03-17

Changes in Libmongoose library:

- Added `-dav_root` configuration option that gives an ability to mount
  a different root directory (not document_root)
- Fixes for build under Win23 and MinGW
- Bugfix: Double dots removal
- Bugfix: final chunked response double-send
- Fixed compilation in 64-bit environments
- Added OS/2 compatibility
- Added `getaddrinfo()` call and `NS_ENABLE_GETADDRINFO`
- Various SSL-related fixes
- Added integer overflow protection in `iobuf_append()` and `deliver_websocket_frame()`
- Fixed NetBSD build
- Enabled `NS_ENABLE_IPV6` build for Visual Studio 2008+
- Enhanced comma detection in `parse_header()`
- Fixed unchanged memory accesses on ARM
- Added ability to use custom memory allocator through NS_MALLOC, NS_FREE, NS_REALLOC

Changes in Mongoose binary:

- Added `-start_browser` option to disable automatic browser launch
- Added experimental SSL support. To listen on HTTPS port, use `ssl://PORT:SSL_CERT` format. For example, to listen on HTTP port 8080 and HTTPS port 8043, use `-listening_port 8080,ssl://8043:ssl_cert.pem`

## Release 5.5, October 28 2014

Changes in Libmongoose library:

- Added new API function: `mg_forward()` for proxying functionality
- Added new API function: `mg_send_file_data()` for sending file data
- Added new utility API functions: `mg_mmap() and mg_munmap()`
- Changed the way SSL settings are handled: removed `ssl_certificate` and
  `ssl_ca_certificate` options, and instead made `listening_port` accept
  `ssl://PORT:SSL_CERT:CA_CERT` notation
- Added ability to listen on multiple ports, see `listening_port` documentation
- Added `enable_proxy` option
- Added [cookie_authentication](https://github.com/cesanta/mongoose/tree/master/examples/cookie_authentication) example
- Added [websocket\_ssl\_proxy](https://github.com/cesanta/mongoose/tree/master/examples/websocket_ssl_proxy) example
- Added [http_client](https://github.com/cesanta/mongoose/tree/master/examples/http_client) example
- Increased default 'idle connection' timeout from 30 to 300 seconds
- Fixed MinGW build
- Refactored all examples, put each in it's own directory with dedicated build
- Many smaller bugfixed, including SSL, CGI, API, proxy, etc

Changes in pre-compiled binaries:

- Support for multiple listening ports
- Fixed CGI handling for scripts that specify interpreter in the hashbang line

## Release 5.4, July 28 2014

Changes in Libmongoose library:

- Added `hexdump_file` option for low-level request/reply debugging
- Added `mg_template()` API function for generating HTML pages from
    templates with expansions
- Fixed `struct mg_connection::local_ip` handling, `mg_set_option()`
    behavior with NULL values
- Added `mg_send_file()` call to send arbitrary file to the client
- Added `mg_terminate_ssl()` for SSL termination functionality
- Added HTTP proxy support, `enable_proxy` config option
- Added `mg_next()` for iterating over existing active connections
- Added client-side SSL auth, `ssl_ca_certificate` option
- Added `mg_wakeup_server_ex()` for pushing messages to existing connections
- Added `MG_WS_HANDSHAKE` and `MG_WS_CONNECT` events that are sent on
     Websocket handshake is connection establishment, respectively
- Removed server-side Lua support
- Filesystem access, reading from socket/SSL performance improvements
- DAV PROPFIND memory leak fixed
- Added `big_upload.c` and enhanced `upload.c` example
- Added `proxy.c` example that demonstrates proxy functionality and SSE pushes
- Added `websocket2.c` example that shows simple web chat implementation
    over websockets
- Various minor fixes


Changes in pre-compiled binaries:

- Created HTML administration console
- When server is started, browser is started automatically
- Fixed directory listing bug when directory contains `#` character
- Removed built-in Lua Server Pages in the binary, and instead
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

   *  New awesome Mongoose logos by our designer Katrin - thanks Katrin!
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

## Release 4.1, Oct 2013
## Release 4.0, Oct 2013
## Release 3.8, Sep 2013

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
