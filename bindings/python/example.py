# This is Python example on how to use Mongoose embeddable web server,
# http://code.google.com/p/mongoose
#
# Before using the mongoose module, make sure that Mongoose shared library is
# built and present in the current (or system library) directory

import mongoose
import sys

# This function is a "/foo" URI handler: it will be called each time
# HTTP request to http://this_machine:8080/foo made.
# It displays some request information.
def uri_handler(conn, info, data):
	conn.printf('%s', 'HTTP/1.0 200 OK\r\n')
	conn.printf('%s', 'Content-Type: text/plain\r\n\r\n')
	conn.printf('%s %s\n', info.request_method, info.uri)
	conn.printf('my_var: %s\n', conn.get_var('my_var') or '<not set>')
	conn.printf('HEADERS: \n')
	for header in info.http_headers[:info.num_headers]:
		conn.printf('  %s: %s\n', header.name, header.value)

# This function is 404 error handler: it is called each time requested
# document is not found by the server.
def error_404_handler(conn, info, data):
	conn.printf('%s', 'HTTP/1.0 200 OK\r\n')
	conn.printf('%s', 'Content-Type: text/plain\r\n\r\n')
	conn.printf('Document %s not found!\n', info.uri)

# Create mongoose object, and register '/foo' URI handler
# List of options may be specified in the contructor
server = mongoose.Mongoose(root='/tmp', ports='8080')

# Register custom URI and 404 error handler
server.set_uri_callback('/foo', uri_handler, 0)
server.set_error_callback(404, error_404_handler, 0)

# Any option may be set later on by setting an attribute of the  server object
server.ports = '8080,8081'   # Listen on port 8081 in addition to 8080

# Mongoose options can be retrieved by asking an attribute
print 'Starting Mongoose %s on port(s) %s ' % (server.version, server.ports)
print 'CGI extensions: %s' % server.cgi_ext

# Serve connections until 'enter' key is pressed on a console
sys.stdin.read(1)

# Deleting server object stops all serving threads
print 'Stopping server.'
del server
