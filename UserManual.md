Overview
--------

Mongoose is small and easy to use web server. It is self-contained, and does
not require any external software to run. Mongoose can be configured via
command line options, or via configuration file. If no command line
options are given, mongoose searches for a configuration file
called "mongoose.conf" in the same directory where mongoose binary is
located.

Configuration file is a sequence of lines, each line containing
command line option name and it's value. Lines beginning with '#'
and empty lines are ignored. Here is 

    # mongoose.conf file
    document_root c:\www
    listening_ports 8080,8043s
    ssh_certificate c:\mongoose\ssl_cert.pem


Usage Examples
--------------
- How to share a Windows folder  

Command Line Options
--------------------

Common Problems
---------------

Embedding
---------

Other Resources
---------------
- Presentation made by Arnout Vandecappelle at FOSDEM 2011 on 2011-02-06
  in Brussels, Belgium, called
  "Creating secure web based user interfaces for Embedded Devices"
  ([pdf](http://mind.be/content/110206_Web-ui.pdf) |
   [odp](http://mind.be/content/110206_Web-ui.odp))
- Linux Journal article by Michel J.Hammel, 2010-04-01, called
  [Mongoose: an Embeddable Web Server in C](http://www.linuxjournal.com/article/10680)
