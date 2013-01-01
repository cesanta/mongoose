Project Mission
---------------

Project mission is to provide the community with simple and functional
web server library and web server executable built on top of that library.


The target audience are application or device developers looking to implement
web interface for their applications or devices and web developers looking for
a simple development platform.

Overview
--------

To accomplish it's mission, Mongoose keeps balance on functionality and
simplicity by carefully selected list of features:

- Liberal [MIT license](http://en.wikipedia.org/wiki/MIT_License), which is great for commercial use
- Crossplatform - works on Windows, Mac and UNIX
- CGI, SSL, SSI, Digest (MD5) authorization, Websocket, WEbDAV support
- Lua server pages support (PHP-like functionality using Lua), see [page.lp](https://github.com/valenok/mongoose/blob/master/test/page.lp)
- Resumed download, URL rewrite, IP-based ACL, Windows service
- Excluding files from serving by URI pattern (file blacklist)
- Download speed limit based on client subnet or URI pattern
- Small footprint: executable size is 50 kB on Linux 2.6 i386 system
- 130 kilobytes Windows executable with all of the above and no dependencies
- Embeddable with simple and clean API ([mongoose.h](https://github.com/valenok/mongoose/blob/master/mongoose.h)). The source is in single [mongoose.c](https://github.com/valenok/mongoose/blob/master/mongoose.c) file to make things easy.
- Embedding examples: [hello.c](https://github.com/valenok/mongoose/blob/master/examples/hello.c), [post.c](https://github.com/valenok/mongoose/blob/master/examples/post.c), [upload.c](https://github.com/valenok/mongoose/blob/master/examples/upload.c), [websocket.c](https://github.com/valenok/mongoose/blob/master/examples/websocket.c)


See [manual](https://github.com/valenok/mongoose/wiki/Manual) for more information.


Mailing list
------------

You can read it online, subscribe to, or send a message at [mongoose-users](http://groups.google.com/group/mongoose-users).


Keep Sergey happy
-----------------

I have a [books wishlist](http://amzn.com/w/1OC2ZCPTQYIEP?sort=priority) on
Amazon. If you feel brave, you can buy me a book! Many thanks to all who
already did so: T.Barmann, D.Hughes, J.C.Sloan, R.Romeo and 4 others.
Appreciated, guys, you keep my brains going!
