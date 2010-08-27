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
    as first argument

  Mongoose: wraps all functions that accept struct mg_context pointer as
    first argument. All valid option names, settable via mg_set_option(),
    are settable/gettable as the attributes of the Mongoose object.
    In addition to those, two attributes are available:
       'version': string, contains server version
       'options': array of all known options.

  Creating Mongoose object automatically starts server, deleting object
  automatically stops it. There is no need to call mg_start() or mg_stop().
"""


import ctypes
import os


MG_ERROR = 0
MG_SUCCESS = 1
MG_NOT_FOUND = 2
MG_BUFFER_TOO_SMALL = 3


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
		('log_message', ctypes.c_char_p),
		('remote_ip', ctypes.c_long),
		('remote_port', ctypes.c_int),
		('status_code', ctypes.c_int),
		('is_ssl', ctypes.c_int),
		('num_headers', ctypes.c_int),
		('http_headers', mg_header * 64),
	]


mg_callback_t = ctypes.CFUNCTYPE(ctypes.c_int,
				 ctypes.c_voidp,
				 ctypes.POINTER(mg_request_info))


class mg_config(ctypes.Structure):
	"""A wrapper for struct mg_config."""
	_fields_ = [
		('document_root', ctypes.c_char_p),
		('index_files', ctypes.c_char_p),
		('ssl_certificate', ctypes.c_char_p),
		('listening_ports', ctypes.c_char_p),
		('cgi_extensions', ctypes.c_char_p),
		('cgi_interpreter', ctypes.c_char_p),
		('cgi_environment', ctypes.c_char_p),
		('ssi_extensions', ctypes.c_char_p),
		('auth_domain', ctypes.c_char_p),
		('protect', ctypes.c_char_p),
		('global_passwords_file', ctypes.c_char_p),
		('put_delete_passwords_file', ctypes.c_char_p),
		('access_log_file', ctypes.c_char_p),
		('error_log_file', ctypes.c_char_p),
		('acl', ctypes.c_char_p),
		('uid', ctypes.c_char_p),
		('mime_types', ctypes.c_char_p),
		('enable_directory_listing', ctypes.c_char_p),
		('num_threads', ctypes.c_char_p),
		('new_request_handler', mg_callback_t),
		('http_error_handler', mg_callback_t),
		('event_log_handler', mg_callback_t),
		('ssl_password_handler', mg_callback_t),
	]


class Connection(object):
	"""A wrapper class for all functions that take
	struct mg_connection * as the first argument."""

	def __init__(self, mongoose, connection):
		self.m = mongoose
		self.conn = ctypes.c_voidp(connection)

	def get_header(self, name):
		val = self.m.dll.mg_get_header(self.conn, name)
		return ctypes.c_char_p(val).value

	def get_var(self, data, name):
		size = len(data)
		buf = ctypes.create_string_buffer(size)
		n = self.m.dll.mg_get_var(data, size, name, buf, size)
		return n == MG_SUCCESS and buf or None
	
	def get_qsvar(self, request_info, name):
		qs = request_info.query_string
		return qs and self.get_var(qs, name) or None

	def printf(self, fmt, *args):
		val = self.m.dll.mg_printf(self.conn, fmt, *args)
		return ctypes.c_int(val).value

	def write(self, data):
		val = self.m.dll.mg_write(self.conn, data, len(data))
		return ctypes.c_int(val).value
	
	def read(self, size):
		buf = ctypes.create_string_buffer(size)
		n = self.m.dll.mg_read(self.conn, buf, size)
		print size, buf, n
		return n <= 0 and None or buf[:n]


class Mongoose(object):
	"""A wrapper class for Mongoose shared library."""

	def __init__(self, **kwargs):
		dll_extension = os.name == 'nt' and 'dll' or 'so'
		self.dll = ctypes.CDLL('_mongoose.%s' % dll_extension)

		self.dll.mg_start.restype = ctypes.c_void_p
		self.dll.mg_modify_passwords_file.restype = ctypes.c_int
		self.dll.mg_read.restype = ctypes.c_int
		self.dll.mg_write.restype = ctypes.c_int
		self.dll.mg_printf.restype = ctypes.c_int
		self.dll.mg_get_header.restype = ctypes.c_char_p
		self.dll.mg_get_var.restype = ctypes.c_int
		self.dll.mg_get_qsvar.restype = ctypes.c_int
		self.dll.mg_get_cookie.restype = ctypes.c_int

		self.callbacks = []
		self.config = mg_config(num_threads='5',
					enable_directory_listing='yes',
					listening_ports='8080',
					document_root='.',
					auth_domain='mydomain.com')
		for key, value in kwargs.iteritems():
			if key in ('new_request_handler',
				   'http_error_handler',
				   'event_log_handler',
				   'ssl_password_handler'):
				cb = self.MakeHandler(value)
				setattr(self.config, key, cb)
			else:
				setattr(self.config, key, str(value))
		self.ctx = self.dll.mg_start(ctypes.byref(self.config))

	def __del__(self):
		"""Destructor, stop Mongoose instance."""
		self.dll.mg_stop(ctypes.c_void_p(self.ctx))

	def MakeHandler(self, python_func):
		"""Return C callback from given Python callback."""

		# Create a closure that will be called by the  shared library.
		def func(connection, request_info):
			# Wrap connection pointer into the connection
			# object and call Python callback
			conn = Connection(self, connection)
			status = python_func(conn, request_info.contents)
			return status == MG_SUCCESS and MG_SUCCESS or MG_ERROR

		# Convert the closure into C callable object
		c_func = mg_callback_t(func)
		c_func.restype = ctypes.c_int

		# Store created callback in the list, so it is kept alive
		# during context lifetime. Otherwise, python can garbage
		# collect it, and C code will crash trying to call it.
		self.callbacks.append(c_func)

		return c_func
