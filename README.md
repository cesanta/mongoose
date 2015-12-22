# <img src="http://www.cesanta.com/hubfs/www.cesanta.com/Images/mongoose_library.png" width="64" height="64"> Mongoose - Embedded Web Server

![](https://img.shields.io/badge/license-GPL_2-green.svg "License")

Mongoose is ideal for embedded environments. It has been designed
for connecting devices and bringing them online.
[Mongoose](https://www.cesanta.com/products)
has been on the market since 2004 and attracted
over 1 million cumulative downloads.

It provides easy to use event-driven interface that allows to implement
network protocols or scalable network applications  with little effort.
Mongoose helps developers to manage the complexity of network programming
and let them concentrate on the logic, saving time and money.

- [Download Mongoose Source Code here](http://hubs.ly/H01bWvx0)
- [Ask questions on support forum](http://forum.cesanta.com/index.php?p=/categories/mongoose)
- [Read User Guide and API reference](https://docs.cesanta.com/mongoose)
- [Study mongoose example code](https://github.com/cesanta/mongoose/tree/master/examples) 

> Nothing overdone. Nothing less. So unbelievably easy to use.
> Just how good software should be! -- Pritin Tyagaraj, SAP


# Features

* Cross-platform: works on Linux/UNIX, MacOS, QNX, eCos, Windows, Android,
  iPhone, FreeRTOS (TI CC3200, ESP8266), etc
* Single-threaded, asynchronous, non-blocking core with simple event-based API
* Native support for [PicoTCP embedded TCP/IP stack](http://www.picotcp.com), 
  [LWIP embedded TCP/IP stack](https://en.wikipedia.org/wiki/LwIP)
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

# Dashboard Example

Mongoose is often used to implement device dashboards and real-time
data exchange over Websocket. Here is a dashboard example that illustrates
the functionality:

![](http://www.cesanta.com/hubfs/www.cesanta.com/diagrams/dash_mongoose_diagram.png)

[Developing a new product? Contact us today to discuss how Mongoose can help
](https://www.cesanta.com/contact)

# Contributions

People who have agreed to the
[Cesanta CLA](https://docs.cesanta.com/contributors_la.shtml)
can make contributions. Note that the CLA isn't a copyright
_assigment_ but rather a copyright _license_.
You retain the copyright on your contributions.

# Licensing

You are using Mongoose under
[GPLv2](http://www.gnu.org/licenses/old-licenses/gpl-2.0.en.html)
and until you commercialize your
application, can continue to use Mongoose free of charge.

Once you commercialize your product, in order to comply with GPLv2
you need to either open the source code of your product fully or
alternatively purchase a commercial license.
We offer [three levels of commercial licensing](https://www.cesanta.com/products):

- Mongoose Lite: cross platform integration, SSL/TLS, HTTP support
- Mongoose: Mongoose Lite + HTTP CGI, HTTP SSI, HTTP Digest Auth,
  WebSocket support
- Mongoose Premium: Mongoose + Plain TCP, UDP, JSON-RPC, MQTT, DNS,
  CoAP support


[Enquire about commercial licensing here](https://www.cesanta.com/contact)

# Mongoose Binary

This is our easy to use web server for web developers (PHP, Ruby, Python, etc)
and web designers. Available in three editions to suit your needs: free, pro
(USD 5) and dev edition (from USD 8). To install, simply download, double-click
to start and run browser - that's all!

[Download Mongoose Binary here](https://www.cesanta.com/products#binary)

# Other products by Cesanta

- [Smart.js](https://github.com/cesanta/smart.js) -
    Generic, hardware independent, full-stack IoT software platform
- [V7](https://github.com/cesanta/v7) - Embedded JavaScript engine
- [Frozen](https://github.com/cesanta/frozen) - JSON parser and generator
- [SLRE](https://github.com/cesanta/slre) - Super Light Regular Expression
  library
