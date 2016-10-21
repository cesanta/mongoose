/*
 * Copyright (c) 2014 Cesanta Software Limited
 * All rights reserved
 */

#ifndef CS_MONGOOSE_EXAMPLES_API_SERVER_DB_PLUGIN_H_
#define CS_MONGOOSE_EXAMPLES_API_SERVER_DB_PLUGIN_H_

#include "../../mongoose.h"

void *db_open(const char *db_path);
void db_close(void **db_handle);

enum { API_OP_GET, API_OP_SET, API_OP_DEL };

void db_op(struct mg_connection *nc, const struct http_message *hm,
           const struct mg_str *key, void *db, int op);

#endif /* CS_MONGOOSE_EXAMPLES_API_SERVER_DB_PLUGIN_H_ */
