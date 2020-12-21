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
  if (tolen > 0) to[0] = '\0';
  if (depth > 5) return 0;
  while (&s[off + i + 1] < e && s[off + i] > 0) {
    size_t n = s[off + i];
    if (n & 0xc0) {
      size_t ptr = (((n & 0x3f) << 8) | s[off + i + 1]) - 12;  // 12 is hdr len
      if (&s[ptr + 1] < e && (s[ptr] & 0xc0) == 0) {
        mg_dns_parse_name(s, e, ptr, to, tolen, depth + 1);
      }
      i++;
      break;
    }
    if (&s[off + i + n + 1] >= e) break;
    if (j + n + 1 >= tolen) return 0;  // Error - overflow
    if (j > 0) to[j++] = '.';
    memcpy(&to[j], &s[off + i + 1], n);
    j += n;
    i += n + 1;
    to[j] = '\0';  // Zero-terminate this chunk
    // LOG(LL_DEBUG, ("-- %zu/%zu %zu %zu", i, e - s, j, n));
  }
  if (tolen > 0) to[tolen - 1] = '\0';  // Make sure make sure it is nul-term
  return i;
}

//       txid  flags numQ  numA  numAP numOP
// 0000  00 01 81 80 00 01 00 01 00 00 00 00 07 63 65 73  .............ces
// 0010  61 6e 74 61 03 63 6f 6d 00 00 01 00 01 c0 0c 00  anta.com........
// 0020  01 00 01 00 00 02 57 00 04 94 fb 36 ec           ......W....6.
bool mg_dns_parse(const uint8_t *buf, size_t len, struct mg_dns_message *dm) {
  struct mg_dns_header *h = (struct mg_dns_header *) buf;
  const uint8_t *s = buf + sizeof(*h), *e = &buf[len];
  uint16_t atype, aclass;
  size_t i, j = 0, n;
  memset(dm, 0, sizeof(*dm));
  if (len < sizeof(*h)) return 0;  // Too small, headers dont fit
  if (len > 512) return 0;         //  Too large, we don't expect that
  if (mg_ntohs(h->num_questions) > 2) return 0;  // Sanity
  if (mg_ntohs(h->num_answers) > 5) return 0;    // Sanity
  dm->txnid = mg_ntohs(h->transaction_id);
  {
    // char *s = mg_hexdump(buf, len);
    // LOG(LL_DEBUG, ("--\n%s\n--\n", s));
    // free(s);
  }
  for (i = 0; i < mg_ntohs(h->num_questions); i++) {
    j += mg_dns_parse_name(s, e, j, dm->name, sizeof(dm->name), 0) + 5;
    // LOG(LL_INFO, ("QUE %zu %zu [%s]", i, j, dm->name));
  }
  for (i = 0; i < mg_ntohs(h->num_answers); i++) {
    j += mg_dns_parse_name(s, e, j, dm->name, sizeof(dm->name), 0) + 9;
    if (&s[j] + 2 > e) break;
    atype = ((int) s[j - 8] << 8) | s[j - 7];
    aclass = ((int) s[j - 6] << 8) | s[j - 5];
    n = ((int) s[j] << 8) | s[j + 1];
    // LOG(LL_DEBUG,
    //("NAME %s, IP len %zu t: %hu, c: %hu", dm->name, n, atype, aclass));
    if (&s[j] + 2 + n > e) break;
    if (n == 4 && atype == 1 && aclass == 1) {
      memcpy(&dm->ipaddr, &s[j + 2], 4);
      dm->resolved = true;
      break;  // Return success
    }
    j += 2 + n;
  }
  return true;
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
    if (mg_dns_parse(c->recv.buf, c->recv.len, &dm) == false) {
      char *s = mg_hexdump(c->recv.buf, c->recv.len);
      LOG(LL_ERROR, ("Unexpected DNS response:\n%s\n", s));
      free(s);
    } else {
      for (d = (struct dns_data *) c->pfn_data; d != NULL; d = tmp) {
        tmp = d->next;
        // LOG(LL_INFO, ("d %p %hu %hu", d, d->txnid, dm.txnid));
        if (dm.txnid != d->txnid) continue;
        if (d->c->is_resolving) {
          d->c->is_resolving = 0;
          if (dm.resolved) {
            d->c->peer.ip = dm.ipaddr;
            mg_connect_resolved(d->c);
          } else {
            mg_error(d->c, "%s DNS lookup failed", dm.name);
          }
        } else {
          LOG(LL_ERROR, ("%lu already resolved", d->c->id));
        }
        mg_dns_free(head, d);
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
  pkt.header.num_questions = mg_htons(2);
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
  memcpy(&pkt.data[n], "\xc0\x0c\x00\x1c\x00\x01", 6);  // AAAA query
  n += 6;
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
  if (mg_aton(*name, &c->peer)) {
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
      LOG(LL_DEBUG, ("%lu resolving %.*s, txnid %hu", c->id, (int) name->len,
                     name->ptr, d->txnid));
      mg_dns_send(mgr->dnsc, name, d->txnid);
    }
  }
}
