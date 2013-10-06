# Project Mission

Project mission is to provide easy to use, powerful, embeddable web server.

# Downloads & Docs

Sources, binaries and bundles are available for download at
[http://cesanta.com/downloads.html](http://cesanta.com/downloads.html).
User documentation is available at
[http://cesanta.com/docs.html](http://cesanta.com/docs.html).

# Overview

Mongoose keeps the balance between functionality and
simplicity by carefully selected list of features:

- Works on Windows, Mac, UNIX, iPhone, Android, and many other platforms
- Scripting and database support (Lua Server Pages + Sqlite, see
  [page.lp](https://github.com/cesanta/mongoose/blob/master/test/page.lp) ),
  which provides ready to go, powerful web development platform in
  one single-click executable with **no dependencies**: forget LAMP!
- Support for CGI, SSL, SSI, Digest (MD5) authorization, Websocket, WEbDAV
- Resumed download, URL rewrite, file blacklist, IP-based ACL, Windows service
- Download speed limit based on client subnet or URI pattern
- Simple and clean embedding API,
  [mongoose.h](https://github.com/cesanta/mongoose/blob/master/mongoose.h).
  The source is in single
  [mongoose.c](https://github.com/cesanta/mongoose/blob/master/mongoose.c) file
  to make things easy. Embedding examples:
  [hello.c](https://github.com/cesanta/mongoose/blob/master/examples/hello.c),
  [post.c](https://github.com/cesanta/mongoose/blob/master/examples/post.c),
  [upload.c](https://github.com/cesanta/mongoose/blob/master/examples/upload.c),
  [websocket.c](https://github.com/cesanta/mongoose/blob/master/examples/websocket.c)
- HTTP client capable of sending arbitrary HTTP/HTTPS requests

Note that Windows and MacOS binaries have following 3rd party software
compiled in:
  <a href="http://wolfssl.com">WolfSSL lightweight SSL library</a>,
  <a href="http://sqlite.org">SQLite embedded database</a>,
  <a href="http://lua.org">Lua embedded scripting engine</a>.

Questions can be asked at
[mongoose-users@googlegroups.com](http://groups.google.com/group/mongoose-users)
mailing list.

# Acknowledgements

Author is grateful to the following people who made their contribution:

Arnout Vandecappelle, Benoît Amiaux, Boris Pek, Cody Hanson, Colin Leitner,
Daniel Oaks, Eric Bakan, Erik Oomen, Filipp Kovalev, Ger Hobbelt,
Hendrik Polczynski, Igor Okulist, Jay, Joe Mucchiello, John Safranek,
José Miguel Gonçalves, Shueng Chuan, Katerina Blinova, Konstantin Sorokin,
Marin Atanasov, Matt Healy, Mitch Hendrickson, Nigel Stewart, Pavel Khlebovich,
Sebastian Reinhard, Stefan Doehla, abadc0de, nullable.type,
T.Barmann, D.Hughes, J.C.Sloan, R.Romeo, L.E.Spencer, S.Kotay, R.M.Shorter,
W.Mar, J.Wilander, Santa from Memphis, S.Davies, C.Beck,
O.M.Vilhunen, C.Radik, G.Woodcock, M.Szczepkowski,
Eternal Lands Dev Team, T.Tollet, C.Tangerino, G.Karsai, A.Bourgett,
C.Blakemore, D.Fonaryov, T.Andrle, O.IJsselmuiden, R.Womack, M.Tomlinson,
A.Slåttå, L.Farrell, J.D.P.Ballestero, V.Albaev, B.Harker, T.Scheffel, H.Klein,
R.Merit, T.Bennett, H.Solis, A.Zincenko, M.S., S.Krul, K.Cooke, S.McCallum,
F.Morenius, and 10 others.

# Licensing

Mongoose is licensed under the terms of
[GNU GPLv.2](http://www.gnu.org/licenses/old-licenses/gpl-2.0.html)
license. Businesses have an option to get a non-restrictive,
royalty-free license and professional support from
[Cesanta Software](http://cesanta.com/products.html).

[Super Light Regular Expression library](https://github.com/cesanta/slre) is
another project by Cesanta Software, developed with the same philosophy
of functionality and simplicity.
