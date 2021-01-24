#include "dns.h"
#include "log.h"
#include "private.h"
#include "str.h"
#include "timer.h"
#include "util.h"

struct dns_data {
  struct dns_data *next;
  struct mg_connection *c;
  unsigned long expire;
  uint16_t txnid;
};

static struct dns_data *s_reqs;  // Active DNS requests

static void mg_sendnsreq(struct mg_connection *, struct mg_str *, int,
                         struct mg_dns *, bool);

static void mg_dns_free(struct dns_data *d) {
  LIST_DELETE(struct dns_data, &s_reqs, d);
  free(d);
}

void mg_resolve_cancel(struct mg_connection *c) {
  struct dns_data *tmp, *d;
  for (d = s_reqs; d != NULL; d = tmp) {
    tmp = d->next;
    if (d->c == c) mg_dns_free(d);
  }
}

static size_t mg_dns_parse_name_depth(const uint8_t *s, size_t len, size_t ofs,
                                      char *to, size_t tolen, int depth) {
  size_t i = 0, j = 0;
  if (tolen > 0) to[0] = '\0';
  if (depth > 5) return 0;
  while (ofs + i + 1 < len) {
    size_t n = s[ofs + i];
    if (n == 0) {
      i++;
      break;
    }
    if (n & 0xc0) {
      size_t ptr = (((n & 0x3f) << 8) | s[ofs + i + 1]);  // 12 is hdr len
      if (ptr + 1 < len && (s[ptr] & 0xc0) == 0 &&
          mg_dns_parse_name_depth(s, len, ptr, to, tolen, depth + 1) == 0)
        return 0;
      i += 2;
      break;
    }
    if (ofs + i + n + 1 >= len) return 0;
    if (j > 0) {
      if (j < tolen) to[j] = '.';
      j++;
    }
    if (j + n < tolen) memcpy(&to[j], &s[ofs + i + 1], n);
    j += n;
    i += n + 1;
    if (j < tolen) to[j] = '\0';  // Zero-terminate this chunk
  }
  if (tolen > 0) to[tolen - 1] = '\0';  // Make sure make sure it is nul-term
  return i;
}

size_t mg_dns_parse_name(const uint8_t *s, size_t n, size_t ofs, char *dst,
                         size_t dstlen) {
  return mg_dns_parse_name_depth(s, n, ofs, dst, dstlen, 0);
}

size_t mg_dns_parse_rr(const uint8_t *buf, size_t len, size_t ofs,
                       bool is_question, struct mg_dns_rr *rr) {
  const struct mg_dns_header *h = (struct mg_dns_header *) buf;
  const uint8_t *s = buf + ofs, *e = &buf[len];

  memset(rr, 0, sizeof(*rr));
  if (len < sizeof(*h)) return 0;  // Too small, headers dont fit
  if (len > 512) return 0;         //  Too large, we don't expect that
  if (s >= e) return 0;            //  Overflow

  if ((rr->nlen = (uint16_t) mg_dns_parse_name(buf, len, ofs, NULL, 0)) == 0)
    return 0;
  s += rr->nlen + 4;
  if (s > e) return 0;
  rr->atype = ((uint16_t) s[-4] << 8) | s[-3];
  rr->aclass = ((uint16_t) s[-2] << 8) | s[-1];
  if (is_question) return rr->nlen + 4;

  s += 6;
  if (s > e) return 0;
  rr->alen = ((uint16_t) s[-2] << 8) | s[-1];
  if (s + rr->alen > e) return 0;
  return rr->nlen + rr->alen + 10;
}

bool mg_dns_parse(const uint8_t *buf, size_t len, struct mg_dns_message *dm) {
  const struct mg_dns_header *h = (struct mg_dns_header *) buf;
  struct mg_dns_rr rr;
  size_t i, n, ofs = sizeof(*h);
  memset(dm, 0, sizeof(*dm));

  if (len < sizeof(*h)) return 0;                // Too small, headers dont fit
  if (mg_ntohs(h->num_questions) > 1) return 0;  // Sanity
  if (mg_ntohs(h->num_answers) > 10) return 0;   // Sanity
  dm->txnid = mg_ntohs(h->txnid);

  for (i = 0; i < mg_ntohs(h->num_questions); i++) {
    if ((n = mg_dns_parse_rr(buf, len, ofs, true, &rr)) == 0) return false;
    // LOG(LL_INFO, ("Q %zu %zu", ofs, n));
    ofs += n;
  }
  for (i = 0; i < mg_ntohs(h->num_answers); i++) {
    // LOG(LL_INFO, ("A -- %zu %zu %s", ofs, n, dm->name));
    if ((n = mg_dns_parse_rr(buf, len, ofs, false, &rr)) == 0) return false;
    mg_dns_parse_name(buf, len, ofs, dm->name, sizeof(dm->name));
    ofs += n;

    if (rr.alen == 4 && rr.atype == 1 && rr.aclass == 1) {
      dm->addr.is_ip6 = false;
      memcpy(&dm->addr.ip, &buf[ofs - 4], 4);
      dm->resolved = true;
      break;  // Return success
    } else if (rr.alen == 16 && rr.atype == 28 && rr.aclass == 1) {
      dm->addr.is_ip6 = true;
      memcpy(&dm->addr.ip6, &buf[ofs - 16], 16);
      dm->resolved = true;
      break;  // Return success
    }
  }
  return true;
}

static void dns_cb(struct mg_connection *c, int ev, void *ev_data,
                   void *fn_data) {
  struct dns_data *d, *tmp;
  if (ev == MG_EV_POLL) {
    unsigned long now = *(unsigned long *) ev_data;
    for (d = s_reqs; d != NULL; d = tmp) {
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
      LOG(LL_VERBOSE_DEBUG, ("%s %d", dm.name, dm.resolved));
      for (d = s_reqs; d != NULL; d = tmp) {
        tmp = d->next;
        // LOG(LL_INFO, ("d %p %hu %hu", d, d->txnid, dm.txnid));
        if (dm.txnid != d->txnid) continue;
        if (d->c->is_resolving) {
          d->c->is_resolving = 0;
          if (dm.resolved) {
            char buf[100];
            dm.addr.port = d->c->peer.port;  // Save port
            d->c->peer = dm.addr;            // Copy resolved address
            LOG(LL_DEBUG, ("%lu %s resolved to %s", d->c->id, dm.name,
                           mg_ntoa(&d->c->peer, buf, sizeof(buf))));
            mg_connect_resolved(d->c);
#if MG_ENABLE_IPV6
          } else if (dm.addr.is_ip6 == false && dm.name[0] != '\0') {
            struct mg_str x = mg_str(dm.name);
            mg_sendnsreq(d->c, &x, c->mgr->dnstimeout, &c->mgr->dns6, true);
#endif
          } else {
            mg_error(d->c, "%s DNS lookup failed", dm.name);
          }
        } else {
          LOG(LL_ERROR, ("%lu already resolved", d->c->id));
        }
        mg_dns_free(d);
        resolved = 1;
      }
    }
    if (!resolved) LOG(LL_ERROR, ("stray DNS reply"));
    c->recv.len = 0;
  } else if (ev == MG_EV_CLOSE) {
    for (d = s_reqs; d != NULL; d = tmp) {
      tmp = d->next;
      mg_dns_free(d);
    }
  }
  (void) fn_data;
}

void mg_dns_send(struct mg_connection *c, const struct mg_str *name,
                 uint16_t txnid, bool ipv6) {
  struct {
    struct mg_dns_header header;
    uint8_t data[256];
  } pkt;
  size_t i, n;
  memset(&pkt, 0, sizeof(pkt));
  pkt.header.txnid = mg_htons(txnid);
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
  if (ipv6) pkt.data[n - 3] = 0x1c;  // AAAA query
  // memcpy(&pkt.data[n], "\xc0\x0c\x00\x1c\x00\x01", 6);  // AAAA query
  // n += 6;
  mg_send(c, &pkt, sizeof(pkt.header) + n);
#if 0
  // Immediately after A query, send AAAA query. Whatever reply comes first,
  // we'll use it. Note: we cannot send two queries in a single packet.
  // https://stackoverflow.com/questions/4082081/requesting-a-and-aaaa-records-in-single-dns-query
  pkt.data[n - 3] = 0x1c;  // AAAA query
  mg_send(c, &pkt, sizeof(pkt.header) + n);
#endif
}

static void mg_sendnsreq(struct mg_connection *c, struct mg_str *name, int ms,
                         struct mg_dns *dnsc, bool ipv6) {
  struct dns_data *d = NULL;
  if (dnsc->url == NULL) {
    mg_error(c, "DNS server URL is NULL. Call mg_mgr_init()");
  } else if (dnsc->c == NULL) {
    dnsc->c = mg_connect(c->mgr, dnsc->url, NULL, NULL);
    if (dnsc->c != NULL) {
      dnsc->c->pfn = dns_cb;
      snprintf(dnsc->c->label, sizeof(dnsc->c->label), "%s", "DNS");
      // dnsc->c->is_hexdumping = 1;
    }
  }
  if (dnsc->c == NULL) {
    mg_error(c, "resolver");
  } else if ((d = (struct dns_data *) calloc(1, sizeof(*d))) == NULL) {
    mg_error(c, "resolve OOM");
  } else {
    d->txnid = s_reqs ? s_reqs->txnid + 1 : 1;
    d->next = s_reqs;
    s_reqs = d;
    d->expire = mg_millis() + ms;
    d->c = c;
    c->is_resolving = 1;
    LOG(LL_VERBOSE_DEBUG, ("%lu resolving %.*s, txnid %hu", c->id,
                           (int) name->len, name->ptr, d->txnid));
    mg_dns_send(dnsc->c, name, d->txnid, ipv6);
  }
}

void mg_resolve(struct mg_connection *c, struct mg_str *name, int ms) {
  if (mg_aton(*name, &c->peer)) {
    // name is an IP address, do not fire name resolution
    mg_connect_resolved(c);
  } else {
    // name is not an IP, send DNS resolution request
    mg_sendnsreq(c, name, ms, &c->mgr->dns4, false);
  }
}
