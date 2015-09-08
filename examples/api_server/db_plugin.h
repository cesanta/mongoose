/*
 * Copyright (c) 2014 Cesanta Software Limited
 * All rights reserved
 */

#ifndef DB_PLUGIN_HEADER_DEFINED
#define DB_PLUGIN_HEADER_DEFINED

#include "../../mongoose.h"

void *db_open(const char *db_path);
void db_close(void **db_handle);

enum { API_OP_GET, API_OP_SET, API_OP_DEL };

void db_op(struct mg_connection *nc, const struct http_message *hm,
           const struct mg_str *key, void *db, int op);

#endif /* DB_PLUGIN_HEADER_DEFINED */
