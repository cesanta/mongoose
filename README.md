# <img src="http://www.cesanta.com/hubfs/www.cesanta.com/Images/mongoose_library.png" width="64" height="64"> Mongoose Embedded Web Server

[![Join the chat at https://gitter.im/cesanta/mongoose](https://badges.gitter.im/Join%20Chat.svg)](https://gitter.im/cesanta/mongoose?utm_source=badge&utm_medium=badge&utm_campaign=pr-badge&utm_content=badge)

![](https://img.shields.io/badge/license-GPL_2-green.svg "License")

[Mongoose](https://www.cesanta.com/mongoose) is a
multi-protocol networking library written in C.
It provides easy to use event-driven interface that allows to implement
network protocols or scalable network applications  with little effort.
Mongoose helps developers to manage the complexity of network programming
and let them concentrate on the logic, saving time and money.

Mongoose has built-in support for several protocols, like
HTTP, Websocket, MQTT, mDNS. Example applications include
Websocket-based chat server, JSON-RPC server,
database server with RESTful API, MQTT broker, netcat with SSL and hexdump,
Raspberry PI camera video feed + led control, and more.

Mongoose is ideal for the embedded environments, it has been designed as
an open source platform for connecting devices and bringing them online.

[Download Mongoose Source Code here](http://hubs.ly/H0150FK0)

Are you an embedded developer? Working on an embedded task?
Check out our [embedded development products](http://hubs.ly/H0150sY0)
to make the right choice for your project.

# Features

* Cross-platform: works on Linux/UNIX, QNX, eCos, Windows, Android, iPhone, etc
* Single-threaded, asynchronous, non-blocking core with simple event-based API
* Builtin protocols:
   - plain TCP, plain UDP, SSL/TLS (over TCP, one-way or two-way)
   - HTTP client, HTTP server
   - Websocket client, Websocket server
   - JSON-RPC client, JSON-RPC server
   - MQTT client, MQTT broker
   - CoAP client, CoAP server
   - DNS client, DNS server, async DNS resolver
* Tiny static and run-time footprint
* Source code is both ISO C and ISO C++ compliant
* Very easy to integrate: just copy
  [mongoose.c](https://raw.githubusercontent.com/cesanta/mongoose/master/mongoose.c) and
  [mongoose.h](https://raw.githubusercontent.com/cesanta/mongoose/master/mongoose.h)
  files to your build tree
* Extensively tested and production-ready, trusted by many blue chip businesses

# Examples & Documentation

- [User Guide](https://docs.cesanta.com/mongoose) - Detailed User Guide and API reference
- [examples](examples) - Collection of well-commented examples. To build any example,
go into respective directory and type `make`

# Dashboard Example

![](http://www.cesanta.com/hubfs/www.cesanta.com/diagrams/dash_mongoose_diagram.png)

[Developing a new product? Contact us today to discuss how Mongoose can help
](https://www.cesanta.com/contact)


# Licensing

Cesanta made Mongoose open source under GPLv2 for a reason. We are all
developers here and appreciate easy access to code and therefore seamless
integration. It's great to be able to play around with the software before
committing to it.

However, the GPLv2 open source license does not permit incorporating the
software into non-open source programs. In order to comply with GPLv2 licensing
you need to open the source code of your end product fully or alternatively
purchase a commercial license.

[Enquire about commercial licensing here](https://www.cesanta.com/contact)

# Mongoose Binary

This is our easy to use web server for web developers (PHP, Ruby, Python, etc)
and web designers. Available in three editions to suit your needs: free, pro
(USD 5) and dev edition (from USD 8). To install, simply download, double-click
to start and run browser - that's all!

[Download Mongoose Binary here](https://www.cesanta.com/mongoose)

# Other products by Cesanta

- [Smart.js](https://github.com/cesanta/smart.js) -
    Generic, hardware independent, full-stack IoT software platform
- [V7](https://github.com/cesanta/v7) - Embedded JavaScript engine
- [Frozen](https://github.com/cesanta/frozen) - JSON parser and generator
- [SLRE](https://github.com/cesanta/slre) - Super Light Regular Expression
  library
