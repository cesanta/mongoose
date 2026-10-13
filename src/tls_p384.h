// P-384 from BearSSL, https://bearssl.org commit 7bea48e
// P-384 signature verification, used by built-in TLS for certificates and
// CertificateVerify. Shared bignum and ECDSA code is in tls_p256.c
//
// Copyright (c) 2016 Thomas Pornin <pornin@bolet.org>
//
// Permission is hereby granted, free of charge, to any person obtaining
// a copy of this software and associated documentation files (the
// "Software"), to deal in the Software without restriction, including
// without limitation the rights to use, copy, modify, merge, publish,
// distribute, sublicense, and/or sell copies of the Software, and to
// permit persons to whom the Software is furnished to do so, subject to
// the following conditions:
//
// The above copyright notice and this permission notice shall be
// included in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
// NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
// BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
// ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
// CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//
// Mongoose changes: br_ / BR_ names are prefixed with mg_ / MG_, static
// functions with p384_. Trimmed to P-384. Structs are not typedef-ed
#pragma once

#include "arch.h"
#include "config.h"
#include "tls.h"

#if MG_TLS == MG_TLS_BUILTIN || defined(MG_OTA_PUBLIC_KEY)
// Verify ECDSA P-384 signature r || s over hash. Public key is X || Y.
// Return 1 on success, 0 on failure
int mg_p384_verify(const uint8_t public_key[96], const uint8_t *hash,
                   size_t hash_size, const uint8_t signature[96]);
#endif  // MG_TLS == MG_TLS_BUILTIN || defined(MG_OTA_PUBLIC_KEY)
