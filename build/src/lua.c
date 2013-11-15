#include "internal.h"

#ifdef USE_LUA
#ifdef _WIN32
static void *mmap(void *addr, int64_t len, int prot, int flags, int fd,
                  int offset) {
  HANDLE fh = (HANDLE) _get_osfhandle(fd);
  HANDLE mh = CreateFileMapping(fh, 0, PAGE_READONLY, 0, 0, 0);
  void *p = MapViewOfFile(mh, FILE_MAP_READ, 0, 0, (size_t) len);
  CloseHandle(mh);
  return p;
}
#define munmap(x, y)  UnmapViewOfFile(x)
#define MAP_FAILED NULL
#define MAP_PRIVATE 0
#define PROT_READ 0
#else
#include <sys/mman.h>
#endif

static const char *LUASOCKET = "luasocket";

// Forward declarations
static int handle_lsp_request(struct mg_connection *, const char *,
                              struct file *, struct lua_State *);

static void reg_string(struct lua_State *L, const char *name, const char *val) {
  lua_pushstring(L, name);
  lua_pushstring(L, val);
  lua_rawset(L, -3);
}

static void reg_int(struct lua_State *L, const char *name, int val) {
  lua_pushstring(L, name);
  lua_pushinteger(L, val);
  lua_rawset(L, -3);
}

static void reg_function(struct lua_State *L, const char *name,
                         lua_CFunction func, struct mg_connection *conn) {
  lua_pushstring(L, name);
  lua_pushlightuserdata(L, conn);
  lua_pushcclosure(L, func, 1);
  lua_rawset(L, -3);
}

static int lsp_sock_close(lua_State *L) {
  if (lua_gettop(L) > 0 && lua_istable(L, -1)) {
    lua_getfield(L, -1, "sock");
    closesocket((SOCKET) lua_tonumber(L, -1));
  } else {
    return luaL_error(L, "invalid :close() call");
  }
  return 1;
}

static int lsp_sock_recv(lua_State *L) {
  char buf[2000];
  int n;

  if (lua_gettop(L) > 0 && lua_istable(L, -1)) {
    lua_getfield(L, -1, "sock");
    n = recv((SOCKET) lua_tonumber(L, -1), buf, sizeof(buf), 0);
    if (n <= 0) {
      lua_pushnil(L);
    } else {
      lua_pushlstring(L, buf, n);
    }
  } else {
    return luaL_error(L, "invalid :close() call");
  }
  return 1;
}

static int lsp_sock_send(lua_State *L) {
  const char *buf;
  size_t len, sent = 0;
  int n, sock;

  if (lua_gettop(L) > 1 && lua_istable(L, -2) && lua_isstring(L, -1)) {
    buf = lua_tolstring(L, -1, &len);
    lua_getfield(L, -2, "sock");
    sock = (int) lua_tonumber(L, -1);
    while (sent < len) {
      if ((n = send(sock, buf + sent, len - sent, 0)) <= 0) {
        break;
      }
      sent += n;
    }
    lua_pushnumber(L, n);
  } else {
    return luaL_error(L, "invalid :close() call");
  }
  return 1;
}

static const struct luaL_Reg luasocket_methods[] = {
  {"close", lsp_sock_close},
  {"send", lsp_sock_send},
  {"recv", lsp_sock_recv},
  {NULL, NULL}
};

static int lsp_connect(lua_State *L) {
  char ebuf[100];
  SOCKET sock;

  if (lua_isstring(L, -3) && lua_isnumber(L, -2) && lua_isnumber(L, -1)) {
    sock = conn2(lua_tostring(L, -3), (int) lua_tonumber(L, -2),
                 (int) lua_tonumber(L, -1), ebuf, sizeof(ebuf));
    if (sock == INVALID_SOCKET) {
      return luaL_error(L, ebuf);
    } else {
      lua_newtable(L);
      reg_int(L, "sock", sock);
      reg_string(L, "host", lua_tostring(L, -4));
      luaL_getmetatable(L, LUASOCKET);
      lua_setmetatable(L, -2);
    }
  } else {
    return luaL_error(L, "connect(host,port,is_ssl): invalid parameter given.");
  }
  return 1;
}

static int lsp_error(lua_State *L) {
  lua_getglobal(L, "mg");
  lua_getfield(L, -1, "onerror");
  lua_pushvalue(L, -3);
  lua_pcall(L, 1, 0, 0);
  return 0;
}

// Silently stop processing chunks.
static void lsp_abort(lua_State *L) {
  int top = lua_gettop(L);
  lua_getglobal(L, "mg");
  lua_pushnil(L);
  lua_setfield(L, -2, "onerror");
  lua_settop(L, top);
  lua_pushstring(L, "aborting");
  lua_error(L);
}

static int lsp(struct mg_connection *conn, const char *path,
               const char *p, int64_t len, lua_State *L) {
  int i, j, pos = 0, lines = 1, lualines = 0;
  char chunkname[MG_BUF_LEN];

  for (i = 0; i < len; i++) {
    if (p[i] == '\n') lines++;
    if (p[i] == '<' && p[i + 1] == '?') {
      for (j = i + 1; j < len ; j++) {
        if (p[j] == '\n') lualines++;
        if (p[j] == '?' && p[j + 1] == '>') {
          mg_write(conn, p + pos, i - pos);

          snprintf(chunkname, sizeof(chunkname), "@%s+%i", path, lines);
          lua_pushlightuserdata(L, conn);
          lua_pushcclosure(L, lsp_error, 1);
          if (luaL_loadbuffer(L, p + (i + 2), j - (i + 2), chunkname)) {
            // Syntax error or OOM. Error message is pushed on stack.
            lua_pcall(L, 1, 0, 0);
          } else {
            // Success loading chunk. Call it.
            lua_pcall(L, 0, 0, 1);
          }

          pos = j + 2;
          i = pos - 1;
          break;
        }
      }
      if (lualines > 0) {
        lines += lualines;
        lualines = 0;
      }
    }
  }

  if (i > pos) {
    mg_write(conn, p + pos, i - pos);
  }

  return 0;
}

static int lsp_write(lua_State *L) {
  int i, num_args;
  const char *str;
  size_t size;
  struct mg_connection *conn = lua_touserdata(L, lua_upvalueindex(1));

  num_args = lua_gettop(L);
  for (i = 1; i <= num_args; i++) {
    if (lua_isstring(L, i)) {
      str = lua_tolstring(L, i, &size);
      mg_write(conn, str, size);
    }
  }

  return 0;
}

static int lsp_read(lua_State *L) {
  struct mg_connection *conn = lua_touserdata(L, lua_upvalueindex(1));
  char buf[1024];
  int len = mg_read(conn, buf, sizeof(buf));

  if (len <= 0) return 0;
  lua_pushlstring(L, buf, len);

  return 1;
}

// mg.include: Include another .lp file
static int lsp_include(lua_State *L) {
  struct mg_connection *conn = lua_touserdata(L, lua_upvalueindex(1));
  struct file file = STRUCT_FILE_INITIALIZER;
  if (handle_lsp_request(conn, lua_tostring(L, -1), &file, L)) {
    // handle_lsp_request returned an error code, meaning an error occured in
    // the included page and mg.onerror returned non-zero. Stop processing.
    lsp_abort(L);
  }
  return 0;
}

// mg.cry: Log an error. Default value for mg.onerror.
static int lsp_cry(lua_State *L){
  struct mg_connection *conn = lua_touserdata(L, lua_upvalueindex(1));
  cry(conn, "%s", lua_tostring(L, -1));
  return 0;
}

// mg.redirect: Redirect the request (internally).
static int lsp_redirect(lua_State *L) {
  struct mg_connection *conn = lua_touserdata(L, lua_upvalueindex(1));
  conn->request_info.uri = lua_tostring(L, -1);
  handle_request(conn);
  lsp_abort(L);
  return 0;
}

static void prepare_lua_environment(struct mg_connection *conn, lua_State *L) {
  const struct mg_request_info *ri = &conn->request_info;
  extern void luaL_openlibs(lua_State *);
  int i;

  luaL_openlibs(L);
#ifdef USE_LUA_SQLITE3
  { extern int luaopen_lsqlite3(lua_State *); luaopen_lsqlite3(L); }
#endif

  luaL_newmetatable(L, LUASOCKET);
  lua_pushliteral(L, "__index");
  luaL_newlib(L, luasocket_methods);
  lua_rawset(L, -3);
  lua_pop(L, 1);
  lua_register(L, "connect", lsp_connect);

  if (conn == NULL) return;

  // Register mg module
  lua_newtable(L);

  reg_function(L, "read", lsp_read, conn);
  reg_function(L, "write", lsp_write, conn);
  reg_function(L, "cry", lsp_cry, conn);
  reg_function(L, "include", lsp_include, conn);
  reg_function(L, "redirect", lsp_redirect, conn);
  reg_string(L, "version", MONGOOSE_VERSION);

  // Export request_info
  lua_pushstring(L, "request_info");
  lua_newtable(L);
  reg_string(L, "request_method", ri->request_method);
  reg_string(L, "uri", ri->uri);
  reg_string(L, "http_version", ri->http_version);
  reg_string(L, "query_string", ri->query_string);
  reg_int(L, "remote_ip", ri->remote_ip);
  reg_int(L, "remote_port", ri->remote_port);
  reg_int(L, "num_headers", ri->num_headers);
  lua_pushstring(L, "http_headers");
  lua_newtable(L);
  for (i = 0; i < ri->num_headers; i++) {
    reg_string(L, ri->http_headers[i].name, ri->http_headers[i].value);
  }
  lua_rawset(L, -3);
  lua_rawset(L, -3);

  lua_setglobal(L, "mg");

  // Register default mg.onerror function
  luaL_dostring(L, "mg.onerror = function(e) mg.write('\\nLua error:\\n', "
                "debug.traceback(e, 1)) end");
}

static int lua_error_handler(lua_State *L) {
  const char *error_msg =  lua_isstring(L, -1) ?  lua_tostring(L, -1) : "?\n";

  lua_getglobal(L, "mg");
  if (!lua_isnil(L, -1)) {
    lua_getfield(L, -1, "write");   // call mg.write()
    lua_pushstring(L, error_msg);
    lua_pushliteral(L, "\n");
    lua_call(L, 2, 0);
    luaL_dostring(L, "mg.write(debug.traceback(), '\\n')");
  } else {
    printf("Lua error: [%s]\n", error_msg);
    luaL_dostring(L, "print(debug.traceback(), '\\n')");
  }
  // TODO(lsm): leave the stack balanced

  return 0;
}

void mg_exec_lua_script(struct mg_connection *conn, const char *path,
                        const void **exports) {
  int i;
  lua_State *L;

  if (path != NULL && (L = luaL_newstate()) != NULL) {
    prepare_lua_environment(conn, L);
    lua_pushcclosure(L, &lua_error_handler, 0);

    lua_pushglobaltable(L);
    if (exports != NULL) {
      for (i = 0; exports[i] != NULL && exports[i + 1] != NULL; i += 2) {
        lua_pushstring(L, exports[i]);
        lua_pushcclosure(L, (lua_CFunction) exports[i + 1], 0);
        lua_rawset(L, -3);
      }
    }

    if (luaL_loadfile(L, path) != 0) {
      lua_error_handler(L);
    }
    lua_pcall(L, 0, 0, -2);
    lua_close(L);
  }
}

static void lsp_send_err(struct mg_connection *conn, struct lua_State *L,
                         const char *fmt, ...) {
  char buf[MG_BUF_LEN];
  va_list ap;

  va_start(ap, fmt);
  vsnprintf(buf, sizeof(buf), fmt, ap);
  va_end(ap);

  if (L == NULL) {
    send_http_error(conn, 500, http_500_error, "%s", buf);
  } else {
    lua_pushstring(L, buf);
    lua_error(L);
  }
}

static int handle_lsp_request(struct mg_connection *conn, const char *path,
                               struct file *filep, struct lua_State *ls) {
  void *p = NULL;
  lua_State *L = NULL;
  FILE *fp = NULL;
  int error = 1;

  // We need both mg_stat to get file size, and mg_fopen to get fd
  if (!mg_stat(path, filep) || (fp = mg_fopen(path, "r")) == NULL) {
    lsp_send_err(conn, ls, "File [%s] not found", path);
  } else if ((p = mmap(NULL, (size_t) filep->size, PROT_READ, MAP_PRIVATE,
                       fileno(fp), 0)) == MAP_FAILED) {
    lsp_send_err(conn, ls, "mmap(%s, %zu, %d): %s", path, (size_t) filep->size,
              fileno(fp), strerror(errno));
  } else if ((L = ls != NULL ? ls : luaL_newstate()) == NULL) {
    send_http_error(conn, 500, http_500_error, "%s", "luaL_newstate failed");
  } else {
    // We're not sending HTTP headers here, Lua page must do it.
    if (ls == NULL) {
      prepare_lua_environment(conn, L);
    }
    error = lsp(conn, path, p, filep->size, L);
  }

  if (L != NULL && ls == NULL) lua_close(L);
  if (p != NULL) munmap(p, filep->size);
  fclose(fp);

  return error;
}
#endif // USE_LUA
