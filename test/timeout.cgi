#!/usr/bin/env perl

use Cwd;
use CGI;

use vars '%in';
CGI::ReadParse();

print "Content-Type: text/html\r\n\r\n";

# This script outputs some content, then sleeps for 5 seconds, then exits.
# Web server should return the content immediately after it is sent,
# not waiting until the script exits.
print "Some data";
flush STDOUT;
sleep 5;
