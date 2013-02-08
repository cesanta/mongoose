# Project Mission

Project mission is to provide simple, functional, embeddable web server to
make it easy for application and device developers to implement web interface
for their application and devices, and to offer a simple web development
environment.

# Overview

To accomplish it's mission, Mongoose keeps balance on functionality and
simplicity by carefully selected list of features:

- Liberal, commercial-friendly
  [MIT license](http://en.wikipedia.org/wiki/MIT_License)
- Works on Windows, Mac, UNIX, iPhone, Android, and many other platforms
- Support for CGI, SSL, SSI, Digest (MD5) authorization, Websocket, WEbDAV
- Lua server pages (PHP-like functionality using Lua), see
  [page.lp](https://github.com/valenok/mongoose/blob/master/test/page.lp)
- Resumed download, URL rewrite, IP-based ACL, Windows service
- Excluding files from serving by URI pattern (file blacklist)
- Download speed limit based on client subnet or URI pattern
- Small footprint: executable size is 50 kB on Linux 2.6 i386 system
- 130 kilobytes Windows executable with all of the above and no dependencies
- Simple and clean embedding API,
  [mongoose.h](https://github.com/valenok/mongoose/blob/master/mongoose.h).
  The source is in single
  [mongoose.c](https://github.com/valenok/mongoose/blob/master/mongoose.c) file
  to make things easy
- Embedding examples:
  [hello.c](https://github.com/valenok/mongoose/blob/master/examples/hello.c),
  [post.c](https://github.com/valenok/mongoose/blob/master/examples/post.c),
  [upload.c](https://github.com/valenok/mongoose/blob/master/examples/upload.c),
  [websocket.c](https://github.com/valenok/mongoose/blob/master/examples/websocket.c)
- HTTP client functionality for embedded usage, capable of
  sending arbitrary HTTP/HTTPS requests
- [User Manual](https://github.com/valenok/mongoose/blob/master/UserManual.md)

Questions can be asked at
[mongoose-users@google.com](http://groups.google.com/group/mongoose-users)
mailing list.


# Keep Sergey happy

I am Sergey Lyubka, a software engineer from Galway, Ireland. I started
working on Mongoose in 2004, and since then continuously improve it,
investing thousands of hours of work. My other project I'm contributing to the
community for free is
[Super Light Regular Expression library](http://code.google.com/p/slre).

If you feel grateful for the stuff I've done, you can buy me a book from my
[Amazon wishlist](http://amzn.com/w/1OC2ZCPTQYIEP?sort=priority). Many thanks
to all who already did so: T.Barmann, D.Hughes, J.C.Sloan, R.Romeo,
L.E.Spencer, S.Kotay, B.Fjukstad and 6 others.
Appreciated guys, you keep my brains going! Cash is also appreciated indeed.
Press [<img src="http://www.paypalobjects.com/en_US/i/btn/btn_donate_SM.gif">](https://www.paypal.com/cgi-bin/webscr?cmd=_s-xclick&hosted_button_id=DGZ2FMP95TAL6)
button to donate. Donation progress: 24/1000 &euro;
(thanks to O.M.Vilhunen, C.Radik, G.Woodcock, M.Szczepkowski)

![Progress](http://chart.googleapis.com/chart?chxr=0,0,1000&chxt=x&chbh=30,0,0&chs=300x35&cht=bhs&chco=90c0f0&chd=t:2.4)
