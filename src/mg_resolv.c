/*
 * Copyright (c) 2014 Cesanta Software Limited
 * All rights reserved
 */

#if MG_ENABLE_ASYNC_RESOLVER

#include "mg_internal.h"
#include "mg_resolv.h"

#ifndef MG_DEFAULT_NAMESERVER
#define MG_DEFAULT_NAMESERVER "8.8.8.8"
#endif

struct mg_resolve_async_request {
  char name[1024];
  int query;
  mg_resolve_callback_t callback;
  void *data;
  time_t timeout;
  int max_retries;
  enum mg_resolve_err err;

  /* state */
  time_t last_time;
  int retries;
};

/*
 * Find what nameserver to use.
 *
 * Return 0 if OK, -1 if error
 */
static int mg_get_ip_address_of_nameserver(char *name, size_t name_len) {
  int ret = -1;

#ifdef _WIN32
  int i;
  LONG err;
  HKEY hKey, hSub;
  wchar_t subkey[512], value[128],
      *key = L"SYSTEM\\ControlSet001\\Services\\Tcpip\\Parameters\\Interfaces";

  if ((err = RegOpenKeyExW(HKEY_LOCAL_MACHINE, key, 0, KEY_READ, &hKey)) !=
      ERROR_SUCCESS) {
    fprintf(stderr, "cannot open reg key %S: %ld\n", key, err);
    ret = -1;
  } else {
    for (ret = -1, i = 0; 1; i++) {
      DWORD subkey_size = sizeof(subkey), type, len = sizeof(value);
      if (RegEnumKeyExW(hKey, i, subkey, &subkey_size, NULL, NULL, NULL,
                        NULL) != ERROR_SUCCESS) {
        break;
      }
      if (RegOpenKeyExW(hKey, subkey, 0, KEY_READ, &hSub) == ERROR_SUCCESS &&
          ((RegQueryValueExW(hSub, L"NameServer", 0, &type, (void *) value,
                             &len) == ERROR_SUCCESS &&
            value[0] != '\0') ||
           (RegQueryValueExW(hSub, L"DhcpNameServer", 0, &type, (void *) value,
                             &len) == ERROR_SUCCESS &&
            value[0] != '\0'))) {
        /*
         * See https://github.com/cesanta/mongoose/issues/176
         * The value taken from the registry can be empty, a single
         * IP address, or multiple IP addresses separated by comma.
         * If it's empty, check the next interface.
         * If it's multiple IP addresses, take the first one.
         */
        wchar_t *comma = wcschr(value, ',');
        if (comma != NULL) {
          *comma = '\0';
        }
        /* %S will convert wchar_t -> char */
        snprintf(name, name_len, "%S", value);
        ret = 0;
        RegCloseKey(hSub);
        break;
      }
    }
    RegCloseKey(hKey);
  }
#elif MG_ENABLE_FILESYSTEM && defined(MG_RESOLV_CONF_FILE_NAME)
  FILE *fp;
  char line[512];

  if ((fp = mg_fopen(MG_RESOLV_CONF_FILE_NAME, "r")) == NULL) {
    ret = -1;
  } else {
    /* Try to figure out what nameserver to use */
    for (ret = -1; fgets(line, sizeof(line), fp) != NULL;) {
      unsigned int a, b, c, d;
      if (sscanf(line, "nameserver %u.%u.%u.%u", &a, &b, &c, &d) == 4) {
        snprintf(name, name_len, "%u.%u.%u.%u", a, b, c, d);
        ret = 0;
        break;
      }
    }
    (void) fclose(fp);
  }
#else
  snprintf(name, name_len, "%s", MG_DEFAULT_NAMESERVER);
#endif /* _WIN32 */

  return ret;
}

int mg_resolve_from_hosts_file(const char *name, union socket_address *usa) {
#if MG_ENABLE_FILESYSTEM && defined(MG_HOSTS_FILE_NAME)
  /* TODO(mkm) cache /etc/hosts */
  FILE *fp;
  char line[1024];
  char *p;
  char alias[256];
  unsigned int a, b, c, d;
  int len = 0;

  if ((fp = mg_fopen(MG_HOSTS_FILE_NAME, "r")) == NULL) {
    return -1;
  }

  for (; fgets(line, sizeof(line), fp) != NULL;) {
    if (line[0] == '#') continue;

    if (sscanf(line, "%u.%u.%u.%u%n", &a, &b, &c, &d, &len) == 0) {
      /* TODO(mkm): handle ipv6 */
      continue;
    }
    for (p = line + len; sscanf(p, "%s%n", alias, &len) == 1; p += len) {
      if (strcmp(alias, name) == 0) {
        usa->sin.sin_addr.s_addr = htonl(a << 24 | b << 16 | c << 8 | d);
        fclose(fp);
        return 0;
      }
    }
  }

  fclose(fp);
#else
  (void) name;
  (void) usa;
#endif

  return -1;
}

static void mg_resolve_async_eh(struct mg_connection *nc, int ev,
                                void *data MG_UD_ARG(void *user_data)) {
  time_t now = (time_t) mg_time();
  struct mg_resolve_async_request *req;
  struct mg_dns_message *msg;
  int first = 0;
#if !MG_ENABLE_CALLBACK_USERDATA
  void *user_data = nc->user_data;
#endif

  if (ev != MG_EV_POLL) DBG(("ev=%d user_data=%p", ev, user_data));

  req = (struct mg_resolve_async_request *) user_data;

  if (req == NULL) {
    return;
  }

  switch (ev) {
    case MG_EV_CONNECT:
      /* don't depend on timer not being at epoch for sending out first req */
      first = 1;
    /* fallthrough */
    case MG_EV_POLL:
      if (req->retries > req->max_retries) {
        req->err = MG_RESOLVE_EXCEEDED_RETRY_COUNT;
        nc->flags |= MG_F_CLOSE_IMMEDIATELY;
        break;
      }
      if (first || now - req->last_time >= req->timeout) {
        mg_send_dns_query(nc, req->name, req->query);
        req->last_time = now;
        req->retries++;
      }
      break;
    case MG_EV_RECV:
      msg = (struct mg_dns_message *) MG_MALLOC(sizeof(*msg));
      if (mg_parse_dns(nc->recv_mbuf.buf, *(int *) data, msg) == 0 &&
          msg->num_answers > 0) {
        req->callback(msg, req->data, MG_RESOLVE_OK);
        nc->user_data = NULL;
        MG_FREE(req);
      } else {
        req->err = MG_RESOLVE_NO_ANSWERS;
      }
      MG_FREE(msg);
      nc->flags |= MG_F_CLOSE_IMMEDIATELY;
      break;
    case MG_EV_SEND:
      /*
       * If a send error occurs, prevent closing of the connection by the core.
       * We will retry after timeout.
       */
      nc->flags &= ~MG_F_CLOSE_IMMEDIATELY;
      mbuf_remove(&nc->send_mbuf, nc->send_mbuf.len);
      break;
    case MG_EV_TIMER:
      req->err = MG_RESOLVE_TIMEOUT;
      nc->flags |= MG_F_CLOSE_IMMEDIATELY;
      break;
    case MG_EV_CLOSE:
      /* If we got here with request still not done, fire an error callback. */
      if (req != NULL) {
        char addr[32];
        mg_sock_addr_to_str(&nc->sa, addr, sizeof(addr), MG_SOCK_STRINGIFY_IP);
#ifdef MG_LOG_DNS_FAILURES
        LOG(LL_ERROR, ("Failed to resolve '%s', server %s", req->name, addr));
#endif
        req->callback(NULL, req->data, req->err);
        nc->user_data = NULL;
        MG_FREE(req);
      }
      break;
  }
}

int mg_resolve_async(struct mg_mgr *mgr, const char *name, int query,
                     mg_resolve_callback_t cb, void *data) {
  struct mg_resolve_async_opts opts;
  memset(&opts, 0, sizeof(opts));
  return mg_resolve_async_opt(mgr, name, query, cb, data, opts);
}

int mg_resolve_async_opt(struct mg_mgr *mgr, const char *name, int query,
                         mg_resolve_callback_t cb, void *data,
                         struct mg_resolve_async_opts opts) {
  struct mg_resolve_async_request *req;
  struct mg_connection *dns_nc;
  const char *nameserver = opts.nameserver;
  char dns_server_buff[17], nameserver_url[26];

  if (nameserver == NULL) {
    nameserver = mgr->nameserver;
  }

  DBG(("%s %d %p", name, query, opts.dns_conn));

  /* resolve with DNS */
  req = (struct mg_resolve_async_request *) MG_CALLOC(1, sizeof(*req));
  if (req == NULL) {
    return -1;
  }

  strncpy(req->name, name, sizeof(req->name));
  req->name[sizeof(req->name) - 1] = '\0';

  req->query = query;
  req->callback = cb;
  req->data = data;
  /* TODO(mkm): parse defaults out of resolve.conf */
  req->max_retries = opts.max_retries ? opts.max_retries : 2;
  req->timeout = opts.timeout ? opts.timeout : 5;

  /* Lazily initialize dns server */
  if (nameserver == NULL) {
    if (mg_get_ip_address_of_nameserver(dns_server_buff,
                                        sizeof(dns_server_buff)) != -1) {
      nameserver = dns_server_buff;
    } else {
      nameserver = MG_DEFAULT_NAMESERVER;
    }
  }

  snprintf(nameserver_url, sizeof(nameserver_url), "udp://%s:53", nameserver);

  dns_nc = mg_connect(mgr, nameserver_url, MG_CB(mg_resolve_async_eh, NULL));
  if (dns_nc == NULL) {
    MG_FREE(req);
    return -1;
  }
  dns_nc->user_data = req;
  if (opts.dns_conn != NULL) {
    *opts.dns_conn = dns_nc;
  }

  return 0;
}

void mg_set_nameserver(struct mg_mgr *mgr, const char *nameserver) {
  MG_FREE((char *) mgr->nameserver);
  mgr->nameserver = NULL;
  if (nameserver != NULL) {
    mgr->nameserver = strdup(nameserver);
  }
}

#endif /* MG_ENABLE_ASYNC_RESOLVER */
