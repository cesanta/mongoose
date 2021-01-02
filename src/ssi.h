#pragma once
#include "http.h"
void mg_http_serve_ssi(struct mg_connection *c, const char *root,
                       const char *fullpath);
