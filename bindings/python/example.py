# This is Python example on how to use Mongoose embeddable web server,
# http://code.google.com/p/mongoose
#
# Before using the mongoose module, make sure that Mongoose shared library is
# built and present in the current (or system library) directory

import mongoose
import sys

# Handle /show and /form URIs.
def EventHandler(event, conn):
    info = conn.info
    if event == mongoose.HTTP_ERROR:
        conn.printf('%s', 'HTTP/1.0 200 OK\r\n')
        conn.printf('%s', 'Content-Type: text/plain\r\n\r\n')
        conn.printf('HTTP error: %d\n', conn.reply_status_code)
        return True
    elif event == mongoose.NEW_REQUEST and info.uri == '/show':
        conn.printf('%s', 'HTTP/1.0 200 OK\r\n')
        conn.printf('%s', 'Content-Type: text/plain\r\n\r\n')
        conn.printf('%s %s\n', info.request_method, info.uri)
        if info.request_method == 'POST':
            content_len = conn.get_header('Content-Length')
            post_data = conn.read(int(content_len))
            my_var = conn.get_var(post_data, 'my_var')
        else:
            my_var = conn.get_var(info.query_string, 'my_var')
        conn.printf('my_var: %s\n', my_var or '<not set>')
        conn.printf('HEADERS: \n')
        for header in info.http_headers[:info.num_headers]:
            conn.printf('  %s: %s\n', header.name, header.value)
        return True
    elif event == mongoose.NEW_REQUEST and info.uri == '/form':
        conn.write('HTTP/1.0 200 OK\r\n'
               'Content-Type: text/html\r\n\r\n'
               'Use GET: <a href="/show?my_var=hello">link</a>'
               '<form action="/show" method="POST">'
               'Use POST: type text and submit: '
               '<input type="text" name="my_var"/>'
               '<input type="submit"/>'
               '</form>')
        return True
    elif event == mongoose.NEW_REQUEST and info.uri == '/secret':
        conn.send_file('/etc/passwd')
        return True
    else:
        return False


# Create mongoose object, and register '/foo' URI handler
# List of options may be specified in the contructor
server = mongoose.Mongoose(EventHandler,
                           document_root='/tmp',
                           listening_ports='8080')

print ('Mongoose started on port %s, press enter to quit'
       % server.get_option('listening_ports'))

sys.stdin.read(1)

# Deleting server object stops all serving threads
print 'Stopping server.'
del server
