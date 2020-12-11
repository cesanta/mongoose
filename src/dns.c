#include "dns.h"
#include "log.h"
#include "private.h"
#include "str.h"
#include "timer.h"
#include "util.h"

struct mg_dns_header {
  uint16_t transaction_id;
  uint16_t flags;
  uint16_t num_questions;
  uint16_t num_answers;
  uint16_t num_authority_prs;
  uint16_t num_other_prs;
};

struct dns_data {
  struct dns_data *next;
  struct mg_connection *c;
  unsigned long expire;
  uint16_t txnid;
};

static void mg_dns_free(struct dns_data **head, struct dns_data *d) {
  LIST_DELETE(struct dns_data, head, d);
  // LOG(LL_INFO, ("--> %p %p %d", d, d->c, d->c->fd));
  free(d);
}

void mg_resolve_cancel(struct mg_mgr *mgr, struct mg_connection *c) {
  struct dns_data *tmp, *d, **head;
  head = mgr->dnsc == NULL ? NULL : (struct dns_data **) &mgr->dnsc->pfn_data;
  for (d = head == NULL ? NULL : *head; d != NULL; d = tmp) {
    tmp = d->next;
    if (d->c == c) mg_dns_free(head, d);
  }
}

static size_t mg_dns_parse_name(const uint8_t *s, const uint8_t *e, size_t off,
                                char *to, size_t tolen, int depth) {
  size_t i = 0, j = 0;
  if (depth > 5) return 0;
  while (&s[off + i + 1] < e && s[off + i] > 0) {
    size_t n = s[off + i];
    if (n & 0xc0) {
      size_t ptr = (((n & 0x3f) << 8) | s[off + i + 1]) - 12;  // 12 is hdr len
      if (&s[ptr + 1] < e && (s[ptr] & 0xc0) == 0) {
        j = mg_dns_parse_name(s, e, ptr, to, tolen, depth + 1);
      }
      i++;
      break;
    }
    if (&s[off + i + n + 1] >= e) break;
    if (j > 0 && j < tolen) to[j++] = '.';
    if (j + n < tolen) memcpy(&to[j], &s[off + i + 1], n);
    j += n;
    i += n + 1;
  }
  if (j < tolen) to[j] = '\0';  // Zero-terminate the name
  to[tolen - 1] = '\0';         // Just in case
  return i;
}

//       txid  flags numQ  numA  numAP numOP
// 0000  00 01 81 80 00 01 00 01 00 00 00 00 07 63 65 73  .............ces
// 0010  61 6e 74 61 03 63 6f 6d 00 00 01 00 01 c0 0c 00  anta.com........
// 0020  01 00 01 00 00 02 57 00 04 94 fb 36 ec           ......W....6.
int mg_dns_parse(const uint8_t *buf, size_t len, struct mg_dns_message *dm) {
  struct mg_dns_header *h = (struct mg_dns_header *) buf;
  const uint8_t *s = buf + sizeof(*h), *e = &buf[len];
  size_t i, j = 0, n, ok = 0;
  if (len < sizeof(*h)) return ok;
  for (i = 0; i < mg_ntohs(h->num_questions); i++) {
    j += mg_dns_parse_name(s, e, j, dm->name, sizeof(dm->name), 0) + 5;
    // LOG(LL_INFO, ("QUE [%s]", name));
  }
  for (i = 0; i < mg_ntohs(h->num_answers); i++) {
    j += mg_dns_parse_name(s, e, j, dm->name, sizeof(dm->name), 0) + 9;
    // LOG(LL_DEBUG, ("NAME %s", name));
    if (&s[j] + 2 > e) break;
    n = ((int) s[j] << 8) | s[j + 1];
    if (&s[j] + 2 + n > e) break;
    if (n == 4) {
      dm->txnid = mg_ntohs(h->transaction_id);
      memcpy(&dm->ipaddr, &s[j + 2], 4);
      ok = 1;
      break;
    }
    j += 2 + n;
  }
  return ok;
}

static void dns_cb(struct mg_connection *c, int ev, void *ev_data,
                   void *fn_data) {
  struct dns_data *d, *tmp, **head = (struct dns_data **) &c->pfn_data;
  if (ev == MG_EV_POLL) {
    unsigned long now = *(unsigned long *) ev_data;
    for (d = (struct dns_data *) fn_data; d != NULL; d = tmp) {
      tmp = d->next;
      // LOG(LL_DEBUG, ("%lu %lu dns poll", d->expire, now));
      if (now > d->expire) mg_error(d->c, "DNS timeout");
    }
  } else if (ev == MG_EV_READ) {
    struct mg_dns_message dm;
    int resolved = 0;
    if (mg_dns_parse(c->recv.buf, c->recv.len, &dm)) {
      for (d = (struct dns_data *) c->pfn_data; d != NULL; d = tmp) {
        tmp = d->next;
        // LOG(LL_INFO, ("d %p %p", d, tmp));
        if (dm.txnid != d->txnid) continue;
        if (d->c->is_resolving) {
          d->c->is_resolving = 0;
          d->c->peer.ip = dm.ipaddr;
          mg_connect_resolved(d->c);
          mg_dns_free(head, d);
        } else {
          LOG(LL_ERROR, ("%p already resolved", d->c->fd));
        }
        resolved = 1;
      }
    }
    if (!resolved) LOG(LL_ERROR, ("stray DNS reply"));
    c->recv.len = 0;
  } else if (ev == MG_EV_CLOSE) {
    for (d = *head; d != NULL; d = tmp) {
      tmp = d->next;
      mg_dns_free(head, d);
    }
  }
}

void mg_dns_send(struct mg_connection *c, const struct mg_str *name,
                 uint16_t txnid) {
  struct {
    struct mg_dns_header header;
    uint8_t data[256];
  } pkt;
  size_t i, n;
  memset(&pkt, 0, sizeof(pkt));
  pkt.header.transaction_id = mg_htons(txnid);
  pkt.header.flags = mg_htons(0x100);
  pkt.header.num_questions = mg_htons(1);
  for (i = n = 0; i < sizeof(pkt.data) - 5; i++) {
    if (name->ptr[i] == '.' || i >= name->len) {
      pkt.data[n] = (uint8_t)(i - n);
      memcpy(&pkt.data[n + 1], name->ptr + n, i - n);
      n = i + 1;
    }
    if (i >= name->len) break;
  }
  memcpy(&pkt.data[n], "\x00\x00\x01\x00\x01", 5);  // A query
  n += 5;
  mg_send(c, &pkt, sizeof(pkt.header) + n);
#if 0
  // Immediately after A query, send AAAA query. Whatever reply comes first,
  // we'll use it. Note: we cannot send two queries in a single packet.
  // https://stackoverflow.com/questions/4082081/requesting-a-and-aaaa-records-in-single-dns-query
  pkt.data[n - 3] = 0x1c;  // AAAA query
  mg_send(c, &pkt, sizeof(pkt.header) + n);
#endif
}

void mg_resolve(struct mg_mgr *mgr, struct mg_connection *c,
                struct mg_str *name, int ms) {
  struct dns_data *d = NULL;
  int resolved = mg_aton(name->ptr, &c->peer.ip);

  // Try to parse name as IP address
  if (mg_vcmp(name, "localhost") == 0) {
    resolved = 1;
    c->peer.ip = mg_htonl(0x7f000001);
  }

  if (resolved) {
    // name is an IP address, do not fire name resolution
    mg_connect_resolved(c);
  } else {
    // name is not an IP, send DNS resolution request
    if (mgr->dnsc == NULL) {
      const char *srv = mgr->dnsserver ? mgr->dnsserver : "udp://8.8.8.8:53";
      mgr->dnsc = mg_connect(mgr, srv, NULL, NULL);
      if (mgr->dnsc != NULL) {
        mgr->dnsc->pfn = dns_cb;
        // mgr->dnsc->is_hexdumping = 1;
        snprintf(mgr->dnsc->label, sizeof(mgr->dnsc->label), "%s", "RESOLVER");
      }
    }
    if (mgr->dnsc == NULL) {
      mg_error(c, "resolver");
    } else if ((d = (struct dns_data *) calloc(1, sizeof(*d))) == NULL) {
      mg_error(c, "resolve OOM");
    } else {
      struct dns_data **head = (struct dns_data **) &mgr->dnsc->pfn_data;
      d->txnid = *head ? (*head)->txnid + 1 : 1;
      d->next = *head;
      *head = d;
      d->expire = mg_millis() + ms;
      d->c = c;
      c->is_resolving = 1;
      LOG(LL_DEBUG, ("%p resolving %.*s, txnid %hu", c->fd, (int) name->len,
                     name->ptr, d->txnid));
      mg_dns_send(mgr->dnsc, name, d->txnid);
    }
  }
}
