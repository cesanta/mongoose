# <img src="http://cesanta.com/images/mongoose_logo.png" width="64" height="64"> Mongoose Web Server

Mongoose is easy to use, powerful, embeddable web server.
   * [Mailing list](http://groups.google.com/group/mongoose-users)
   * [Downloads](http://cesanta.com/mongoose.shtml) and
   [Documentation](http://cesanta.com/docs.shtml)
    * [How to use Lua Server Pages](http://cesanta.com/docs/Lua.shtml)
    * [How to create basic website with Mongoose](http://cesanta.com/docs/BasicWebsite.shtml)
    * [How to create PHP website with Mongoose](http://cesanta.com/docs/PhpWebsite.shtml)
    * [Embedding Guide](http://cesanta.com/docs/Embed.shtml)

# Features

- Works on Windows, Mac, UNIX/Linux, iPhone, Android eCos, QNX and many other platforms
- CGI, SSI, SSL, Digest auth, Websocket, WEbDAV, Resumed download, URL rewrite, file blacklist, Custom error pages, Virtual hosts, IP-based ACL, Windows service
- Lua Server Pages (PHP-like scripting in [Lua](http://lua.org) ), see [dirscan.lp](https://github.com/cesanta/mongoose/blob/master/examples/lua/dirscan.lp)
- Simple and clean
  [embedding API](https://github.com/cesanta/mongoose/blob/master/mongoose.h).
  The source is in single
  [mongoose.c](https://github.com/cesanta/mongoose/blob/master/mongoose.c) file
  to make embedding easy
- Extremely lightweight, has a core of under 40kB and tiny runtime footprint
- Asynchronous, non-blocking core supporting single- or multi-threaded usage
- HTTP client functionality
- On the market since 2004, stable, mature and tested,
  has several man-years invested in continuous improvement and refinement
- Professional support and consultancy available from
[Cesanta Software](http://cesanta.com), developer of Mongoose

# Screenshots

Download, double-click to start, run browser -- that's all!

![shot1](http://cesanta.com/images/tut_sharing/tut1.png)
![shot2](http://cesanta.com/images/tut_sharing/tut2.png)

![shot3](http://cesanta.com/images/tut_sharing/tut3.png)
![shot4](http://cesanta.com/images/tut_sharing/tut4.png)

# Acknowledgements

Mongoose made better thanks to the contribution of following people:

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

Mongoose is released under
[GNU GPL v.2](http://www.gnu.org/licenses/old-licenses/gpl-2.0.html).
Businesses have an option to get non-restrictive, royalty-free commercial
license and professional support from
[Cesanta Software](http://cesanta.com).

[Frozen JSON parser](https://github.com/cesanta/frozen),
[Super Light Regular Expression library](https://github.com/cesanta/slre),
[Super Light DNS Resolver](https://github.com/cesanta/sldr)
are other projects by Cesanta Software, developed with the same philosophy
of functionality and simplicity.
