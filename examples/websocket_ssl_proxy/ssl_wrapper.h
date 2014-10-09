// Copyright (c) 2014 Cesanta Software Limited
// All rights reserved
//
// This software is dual-licensed: you can redistribute it and/or modify
// it under the terms of the GNU General Public License version 2 as
// published by the Free Software Foundation. For the terms of this
// license, see <http://www.gnu.org/licenses/>.
//
// You are free to use this software under the terms of the GNU General
// Public License, but WITHOUT ANY WARRANTY; without even the implied
// warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
// See the GNU General Public License for more details.
//
// Alternatively, you can license this software under a commercial
// license, as set out in <http://cesanta.com/products.html>.
//
// $Date$

#ifndef SSL_WRAPPER_HEADER_INCLUDED
#define SSL_WRAPPER_HEADER_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

void *ssl_wrapper_init(const char *listen_addr, const char *target_addr,
                       const char **err_msg);
void ssl_wrapper_serve(void *, volatile int *stop_marker);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // SSL_WRAPPER_HEADER_INCLUDED
