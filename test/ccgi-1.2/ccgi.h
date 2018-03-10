/*
 * C CGI Library version 1.2
 *
 * Copyright 2015 Stephen C. Losen.  Distributed under the terms
 * of the GNU Lesser General Public License (LGPL 2.1)
 */

#ifndef _CCGI_H
#define _CCGI_H

typedef struct CGI_varlist CGI_varlist;

typedef const char * const CGI_value;

char *CGI_decode_url(const char *p);

char *CGI_encode_url(const char *p, const char *keep);

char *CGI_encode_entity(const char *p);

char *CGI_encode_base64(const void *p, int len);

void *CGI_decode_base64(const char *p, int *len);

char *CGI_encode_hex(const void *p, int len);

void *CGI_decode_hex(const char *p, int *len);

char *CGI_encrypt(const void *p, int len, const char *password);

void *CGI_decrypt(const char *p, int *len, const char *password);

char *CGI_encode_query(const char *keep, ...);

char *CGI_encode_varlist(CGI_varlist *v, const char *keep);

CGI_varlist *CGI_decode_query(CGI_varlist *v, const char *query);

CGI_varlist *CGI_get_cookie(CGI_varlist *v);

CGI_varlist *CGI_get_query(CGI_varlist *v);

CGI_varlist *CGI_get_post(CGI_varlist *v, const char *template);

CGI_varlist *CGI_get_all(const char *template);

CGI_varlist *CGI_add_var(CGI_varlist *v, const char *varname,
    const char *value);

void CGI_free_varlist(CGI_varlist *v);

CGI_value *CGI_lookup_all(CGI_varlist *v, const char *varname);

const char *CGI_lookup(CGI_varlist *v, const char *varname);

const char *CGI_first_name(CGI_varlist *v);

const char *CGI_next_name(CGI_varlist *v);

void CGI_prefork_server(const char *host, int port, const char *pidfile,
    int maxproc, int minidle, int maxidle, int maxreq,
    void (*callback)(void));

#endif
