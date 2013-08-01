# Project Mission

Project mission is to provide easy to use, powerful, embeddable web server.

# Overview

Mongoose keeps the balance between functionality and
simplicity by carefully selected list of features:

- Liberal, commercial-friendly
  [MIT license](http://en.wikipedia.org/wiki/MIT_License)
- Works on Windows, Mac, UNIX, iPhone, Android, and many other platforms
- Scripting and database support (Lua Server Pages + Sqlite, see
  [page.lp](https://github.com/valenok/mongoose/blob/master/test/page.lp) ),
  which provides ready to go, powerful web development platform in
  one single-click executable with **no dependencies**: forget LAMP!
- Support for CGI, SSL, SSI, Digest (MD5) authorization, Websocket, WEbDAV
- Resumed download, URL rewrite, file blacklist, IP-based ACL, Windows service
- Download speed limit based on client subnet or URI pattern
- Simple and clean embedding API,
  [mongoose.h](https://github.com/valenok/mongoose/blob/master/mongoose.h).
  The source is in single
  [mongoose.c](https://github.com/valenok/mongoose/blob/master/mongoose.c) file
  to make things easy. Embedding examples:
  [hello.c](https://github.com/valenok/mongoose/blob/master/examples/hello.c),
  [post.c](https://github.com/valenok/mongoose/blob/master/examples/post.c),
  [upload.c](https://github.com/valenok/mongoose/blob/master/examples/upload.c),
  [websocket.c](https://github.com/valenok/mongoose/blob/master/examples/websocket.c)
- HTTP client capable of sending arbitrary HTTP/HTTPS requests
- [User Manual](https://github.com/valenok/mongoose/blob/master/UserManual.md)

Note that Windows and MacOS binaries have following 3rd party software
compiled in:
  <a href="http://wolfssl.com">WolfSSL lightweight SSL library</a>,
  <a href="http://sqlite.org">SQLite embedded database</a>,
  <a href="http://lua.org">Lua embedded scripting engine</a>.

Questions can be asked at
[mongoose-users@google.com](http://groups.google.com/group/mongoose-users)
mailing list.

[![&nbsp;](https://cruel-carlota.pagodabox.com/2a613890c1f4b60e5919a9c1dd3caca2 "githalytics.com")](http://githalytics.com/valenok/mongoose)


# Author

I am Sergey Lyubka, a software engineer from Galway, Ireland. I started
working on Mongoose in 2004, and since then continuously improve it,
investing thousands of hours of work. My other project I'm contributing to the
community for free is
[Super Light Regular Expression library](http://code.google.com/p/slre).
