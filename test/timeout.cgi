#!/usr/bin/env perl

# Make stdout unbuffered
use FileHandle;
STDOUT->autoflush(1);

# This script outputs some content, then sleeps for 5 seconds, then exits.
# Web server should return the content immediately after it is sent,
# not waiting until the script exits.
print "Content-Type: text/html\r\n\r\n";
print "Some data";
sleep 3;
