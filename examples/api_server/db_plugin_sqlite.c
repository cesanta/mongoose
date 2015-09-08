/*
 * Copyright (c) 2014 Cesanta Software Limited
 * All rights reserved
 */

#include "db_plugin.h"
#include "sqlite3.h"

void *db_open(const char *db_path) {
  sqlite3 *db = NULL;
  if (sqlite3_open_v2(db_path, &db, SQLITE_OPEN_READWRITE |
      SQLITE_OPEN_CREATE | SQLITE_OPEN_FULLMUTEX, NULL) == SQLITE_OK) {
    sqlite3_exec(db, "CREATE TABLE IF NOT EXISTS kv(key PRIMARY KEY, val);",
                 0, 0, 0);
  }
  return db;
}

void db_close(void **db_handle) {
  if (db_handle != NULL && *db_handle != NULL) {
    sqlite3_close(*db_handle);
    *db_handle = NULL;
  }
}

static void op_set(struct mg_connection *nc, const struct http_message *hm,
                   const struct mg_str *key, void *db) {
  sqlite3_stmt *stmt = NULL;
  char value[200];
  const struct mg_str *body = hm->query_string.len > 0 ?
    &hm->query_string : &hm->body;

  mg_get_http_var(body, "value", value, sizeof(value));
  if (sqlite3_prepare_v2(db, "INSERT OR REPLACE INTO kv VALUES (?, ?);",
      -1, &stmt, NULL) == SQLITE_OK) {
    sqlite3_bind_text(stmt, 1, key->p, key->len, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, value, strlen(value), SQLITE_STATIC);
    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
  }
  mg_printf(nc, "%s", "HTTP/1.1 200 OK\r\nContent-Length: 0\r\n\r\n");
}

static void op_get(struct mg_connection *nc, const struct http_message *hm,
                   const struct mg_str *key, void *db) {
  sqlite3_stmt *stmt = NULL;
  const char *data = NULL;
  int result;
  (void) hm;

  if (sqlite3_prepare_v2(db, "SELECT val FROM kv WHERE key = ?;",
      -1, &stmt, NULL) == SQLITE_OK) {
    sqlite3_bind_text(stmt, 1, key->p, key->len, SQLITE_STATIC);
    result = sqlite3_step(stmt);
    data = (char *) sqlite3_column_text(stmt, 0);
    if ((result == SQLITE_OK || result == SQLITE_ROW) && data != NULL) {
      mg_printf(nc, "HTTP/1.1 200 OK\r\n"
                "Content-Type: text/plain\r\n"
                "Content-Length: %d\r\n\r\n%s",
                (int) strlen(data), data);
    } else {
      mg_printf(nc, "%s", "HTTP/1.1 404 Not Found\r\n"
                "Content-Length: 0\r\n\r\n");
    }
    sqlite3_finalize(stmt);
  } else {
    mg_printf(nc, "%s", "HTTP/1.1 500 Server Error\r\n"
              "Content-Length: 0\r\n\r\n");
  }
}

static void op_del(struct mg_connection *nc, const struct http_message *hm,
                   const struct mg_str *key, void *db) {
  sqlite3_stmt *stmt = NULL;
  int result;
  (void) hm;

  if (sqlite3_prepare_v2(db, "DELETE FROM kv WHERE key = ?;",
      -1, &stmt, NULL) == SQLITE_OK) {
    sqlite3_bind_text(stmt, 1, key->p, key->len, SQLITE_STATIC);
    result = sqlite3_step(stmt);
    if (result == SQLITE_OK || result == SQLITE_ROW) {
      mg_printf(nc, "%s", "HTTP/1.1 200 OK\r\nContent-Length: 0\r\n\r\n");
    } else {
      mg_printf(nc, "%s", "HTTP/1.1 404 Not Found\r\n"
                "Content-Length: 0\r\n\r\n");
    }
    sqlite3_finalize(stmt);
  } else {
    mg_printf(nc, "%s", "HTTP/1.1 500 Server Error\r\n"
              "Content-Length: 0\r\n\r\n");
  }
}

void db_op(struct mg_connection *nc, const struct http_message *hm,
           const struct mg_str *key, void *db, int op) {
  switch (op) {
    case API_OP_GET: op_get(nc, hm, key, db); break;
    case API_OP_SET: op_set(nc, hm, key, db); break;
    case API_OP_DEL: op_del(nc, hm, key, db); break;
    default:
      mg_printf(nc, "%s", "HTTP/1.0 501 Not Implemented\r\n"
                "Content-Length: 0\r\n\r\n");
      break;
  }
}

