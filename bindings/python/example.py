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
def uri_handler(conn, info):
	if info.uri != '/foo':
		return mongoose.MG_ERROR
	conn.printf('%s', 'HTTP/1.0 200 OK\r\n')
	conn.printf('%s', 'Content-Type: text/plain\r\n\r\n')
	conn.printf('%s %s\n', info.request_method, info.uri)
	conn.printf('my_var: %s\n',
		    conn.get_qsvar(info, 'my_var') or '<not set>')
	conn.printf('HEADERS: \n')
	for header in info.http_headers[:info.num_headers]:
		conn.printf('  %s: %s\n', header.name, header.value)
	return mongoose.MG_SUCCESS

# This function is 404 error handler: it is called each time requested
# document is not found by the server.
def error_handler(conn, info):
	conn.printf('%s', 'HTTP/1.0 200 OK\r\n')
	conn.printf('%s', 'Content-Type: text/plain\r\n\r\n')
	conn.printf('HTTP error: %d\n', info.status_code)
	return mongoose.MG_SUCCESS

# Create mongoose object, and register '/foo' URI handler
# List of options may be specified in the contructor
server = mongoose.Mongoose(document_root='/tmp',
			   new_request_handler=uri_handler,
			   http_error_handler=error_handler,
			   listening_ports='8080')

print 'Starting Mongoose server, press enter to quit'
sys.stdin.read(1)

# Deleting server object stops all serving threads
print 'Stopping server.'
del server
