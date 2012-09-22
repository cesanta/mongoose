#  Copyright (c) 2004-2009 Sergey Lyubka
#
#  Permission is hereby granted, free of charge, to any person obtaining a copy
#  of this software and associated documentation files (the "Software"), to deal
#  in the Software without restriction, including without limitation the rights
#  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
#  copies of the Software, and to permit persons to whom the Software is
#  furnished to do so, subject to the following conditions:
#
#  The above copyright notice and this permission notice shall be included in
#  all copies or substantial portions of the Software.
#
#  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
#  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
#  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
#  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
#  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
#  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
#  THE SOFTWARE.
#
#  $Id: mongoose.py 471 2009-08-30 14:30:21Z valenok $

"""
This module provides python binding for the Mongoose web server.

There are two classes defined:

  Connection: - wraps all functions that accept struct mg_connection pointer
  as first argument.

  Mongoose: wraps all functions that accept struct mg_context pointer as
  first argument.

  Creating Mongoose object automatically starts server, deleting object
  automatically stops it. There is no need to call mg_start() or mg_stop().
"""


import ctypes
import os


NEW_REQUEST       = 0
REQUEST_COMPLETE  = 1
HTTP_ERROR        = 2
EVENT_LOG         = 3
INIT_SSL          = 4
WEBSOCKET_CONNECT = 5
WEBSOCKET_READY   = 6
WEBSOCKET_MESSAGE = 7
WEBSOCKET_CLOSE   = 8


class mg_header(ctypes.Structure):
  """A wrapper for struct mg_header."""
  _fields_ = [
    ('name', ctypes.c_char_p),
    ('value', ctypes.c_char_p),
  ]


class mg_request_info(ctypes.Structure):
  """A wrapper for struct mg_request_info."""
  _fields_ = [
    ('request_method', ctypes.c_char_p),
    ('uri', ctypes.c_char_p),
    ('http_version', ctypes.c_char_p),
    ('query_string', ctypes.c_char_p),
    ('remote_user', ctypes.c_char_p),
    ('remote_ip', ctypes.c_long),
    ('remote_port', ctypes.c_int),
    ('is_ssl', ctypes.c_int),
    ('num_headers', ctypes.c_int),
    ('http_headers', mg_header * 64),
  ]


mg_callback_t = ctypes.CFUNCTYPE(ctypes.c_void_p, ctypes.c_int, ctypes.c_void_p)


class Connection(object):
  """A wrapper class for all functions that take
  struct mg_connection * as the first argument."""

  def __init__(self, mongoose, connection):
    self.m = mongoose
    self.conn = ctypes.c_void_p(connection)
    self.info = self.m.dll.mg_get_request_info(self.conn).contents
    self.user_data = self.m.dll.mg_get_user_data(self.conn)
    self.log_message = self.m.dll.mg_get_log_message(self.conn)
    self.reply_status_code = self.m.dll.mg_get_reply_status_code(self.conn)
    self.ssl_context = self.m.dll.mg_get_ssl_context(self.conn)

  def get_header(self, name):
    val = self.m.dll.mg_get_header(self.conn, name)
    return ctypes.c_char_p(val).value

  def get_var(self, data, name):
    size = data and len(data) or 0
    buf = ctypes.create_string_buffer(size)
    n = self.m.dll.mg_get_var(data, size, name, buf, size)
    return n >= 0 and buf or None

  def printf(self, fmt, *args):
    val = self.m.dll.mg_printf(self.conn, fmt, *args)
    return ctypes.c_int(val).value

  def write(self, data):
    val = self.m.dll.mg_write(self.conn, data, len(data))
    return ctypes.c_int(val).value

  def read(self, size):
    buf = ctypes.create_string_buffer(size)
    n = self.m.dll.mg_read(self.conn, buf, size)
    return n <= 0 and None or buf[:n]

  def send_file(self, path):
    self.m.dll.mg_send_file(self.conn, path)


class Mongoose(object):
  """A wrapper class for Mongoose shared library."""

  def __init__(self, callback, **kwargs):
    if os.name == 'nt':
      self.dll = ctypes.WinDLL('mongoose.dll')
    else:
      self.dll = ctypes.CDLL('libmongoose.so')

    self.dll.mg_start.restype = ctypes.c_void_p
    self.dll.mg_modify_passwords_file.restype = ctypes.c_int
    self.dll.mg_read.restype = ctypes.c_int
    self.dll.mg_write.restype = ctypes.c_int
    self.dll.mg_printf.restype = ctypes.c_int
    self.dll.mg_get_header.restype = ctypes.c_char_p
    self.dll.mg_get_var.restype = ctypes.c_int
    self.dll.mg_get_cookie.restype = ctypes.c_int
    self.dll.mg_get_option.restype = ctypes.c_char_p
    self.dll.mg_get_request_info.restype = ctypes.POINTER(mg_request_info)
    self.dll.mg_get_user_data.restype = ctypes.c_void_p
    self.dll.mg_get_reply_status_code.restype = ctypes.c_int
    self.dll.mg_get_log_message.restype = ctypes.c_char_p
    self.dll.mg_get_ssl_context.restype = ctypes.c_void_p

    if callback:
      # Create a closure that will be called by the  shared library.
      def func(event, connection):
        # Wrap connection pointer into the connection
        # object and call Python callback
        conn = Connection(self, connection)
        return callback(event, conn) and 1 or 0

      # Convert the closure into C callable object
      self.callback = mg_callback_t(func)
      self.callback.restype = ctypes.c_char_p
    else:
      self.callback = ctypes.c_void_p(0)

    args = [y for x in kwargs.items() for y in x] + [None]
    options = (ctypes.c_char_p * len(args))(*args)

    ret = self.dll.mg_start(self.callback, 0, options)
    self.ctx = ctypes.c_void_p(ret)

  def __del__(self):
    """Destructor, stop Mongoose instance."""
    self.dll.mg_stop(self.ctx)

  def get_option(self, name):
    return self.dll.mg_get_option(self.ctx, name)
