#include "dns.h"
#include "log.h"
#include "printf.h"
#include "str.h"
#include "timer.h"
#include "url.h"
#include "util.h"

struct dns_data {
  struct dns_data *next;
  struct mg_connection *c;
  uint64_t expire;
  uint16_t txnid;
};

static void mg_sendnsreq(struct mg_connection *, struct mg_str *, int,
                         struct mg_dns *, bool);

static void mg_dns_free(struct dns_data **head, struct dns_data *d) {
  LIST_DELETE(struct dns_data, head, d);
  free(d);
}

void mg_resolve_cancel(struct mg_connection *c) {
  struct dns_data *tmp, *d;
  struct dns_data **head = (struct dns_data **) &c->mgr->active_dns_requests;
  for (d = *head; d != NULL; d = tmp) {
    tmp = d->next;
    if (d->c == c) mg_dns_free(head, d);
  }
}

static size_t mg_dns_parse_name_depth(const uint8_t *s, size_t len, size_t ofs,
                                      char *to, size_t tolen, size_t j,
                                      int depth) {
  size_t i = 0;
  if (tolen > 0 && depth == 0) to[0] = '\0';
  if (depth > 5) return 0;
  // MG_INFO(("ofs %lx %x %x", (unsigned long) ofs, s[ofs], s[ofs + 1]));
  while (ofs + i + 1 < len) {
    size_t n = s[ofs + i];
    if (n == 0) {
      i++;
      break;
    }
    if (n & 0xc0) {
      size_t ptr = (((n & 0x3f) << 8) | s[ofs + i + 1]);  // 12 is hdr len
      // MG_INFO(("PTR %lx", (unsigned long) ptr));
      if (ptr + 1 < len && (s[ptr] & 0xc0) == 0 &&
          mg_dns_parse_name_depth(s, len, ptr, to, tolen, j, depth + 1) == 0)
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
    // MG_INFO(("--> [%s]", to));
  }
  if (tolen > 0) to[tolen - 1] = '\0';  // Make sure make sure it is nul-term
  return i;
}

static size_t mg_dns_parse_name(const uint8_t *s, size_t n, size_t ofs,
                                char *dst, size_t dstlen) {
  return mg_dns_parse_name_depth(s, n, ofs, dst, dstlen, 0, 0);
}

size_t mg_dns_parse_rr(const uint8_t *buf, size_t len, size_t ofs,
                       bool is_question, struct mg_dns_rr *rr) {
  const uint8_t *s = buf + ofs, *e = &buf[len];

  memset(rr, 0, sizeof(*rr));
  if (len < sizeof(struct mg_dns_header)) return 0;  // Too small
  if (len > 512) return 0;  //  Too large, we don't expect that
  if (s >= e) return 0;     //  Overflow

  if ((rr->nlen = (uint16_t) mg_dns_parse_name(buf, len, ofs, NULL, 0)) == 0)
    return 0;
  s += rr->nlen + 4;
  if (s > e) return 0;
  rr->atype = (uint16_t) (((uint16_t) s[-4] << 8) | s[-3]);
  rr->aclass = (uint16_t) (((uint16_t) s[-2] << 8) | s[-1]);
  if (is_question) return (size_t) (rr->nlen + 4);

  s += 6;
  if (s > e) return 0;
  rr->alen = (uint16_t) (((uint16_t) s[-2] << 8) | s[-1]);
  if (s + rr->alen > e) return 0;
  return (size_t) (rr->nlen + rr->alen + 10);
}

bool mg_dns_parse(const uint8_t *buf, size_t len, struct mg_dns_message *dm) {
  const struct mg_dns_header *h = (struct mg_dns_header *) buf;
  struct mg_dns_rr rr;
  size_t i, n, num_answers, ofs = sizeof(*h);
  bool is_response;
  memset(dm, 0, sizeof(*dm));

  if (len < sizeof(*h)) return 0;                // Too small, headers dont fit
  if (mg_ntohs(h->num_questions) > 1) return 0;  // Sanity
  num_answers = mg_ntohs(h->num_answers);
  if (num_answers > 10) {
    MG_DEBUG(("Got %u answers, ignoring beyond 10th one", num_answers));
    num_answers = 10;  // Sanity cap
  }
  dm->txnid = mg_ntohs(h->txnid);
  is_response = mg_ntohs(h->flags) & 0x8000;

  for (i = 0; i < mg_ntohs(h->num_questions); i++) {
    if ((n = mg_dns_parse_rr(buf, len, ofs, true, &rr)) == 0) return false;
    // MG_INFO(("Q %lu %lu %hu/%hu", ofs, n, rr.atype, rr.aclass));
    mg_dns_parse_name(buf, len, ofs, dm->name, sizeof(dm->name));
    ofs += n;
  }

  if (!is_response) {
    // For queries, there is no need to parse the answers. In this way,
    // we also ensure the domain name (dm->name) is parsed from
    // the question field.
    return true;
  }

  for (i = 0; i < num_answers; i++) {
    if ((n = mg_dns_parse_rr(buf, len, ofs, false, &rr)) == 0) return false;
    // MG_INFO(("A -- %lu %lu %hu/%hu %s", ofs, n, rr.atype, rr.aclass,
    // dm->name));
    mg_dns_parse_name(buf, len, ofs, dm->name, sizeof(dm->name));
    ofs += n;

    if (rr.alen == 4 && rr.atype == 1 && rr.aclass == 1) {
      dm->addr.is_ip6 = false;
      memcpy(&dm->addr.ip, &buf[ofs - 4], 4);
      dm->resolved = true;
      break;  // Return success
    } else if (rr.alen == 16 && rr.atype == 28 && rr.aclass == 1) {
      dm->addr.is_ip6 = true;
      memcpy(&dm->addr.ip, &buf[ofs - 16], 16);
      dm->resolved = true;
      break;  // Return success
    }
  }
  return true;
}

static void dns_cb(struct mg_connection *c, int ev, void *ev_data) {
  struct dns_data *d, *tmp;
  struct dns_data **head = (struct dns_data **) &c->mgr->active_dns_requests;
  if (ev == MG_EV_POLL) {
    uint64_t now = *(uint64_t *) ev_data;
    for (d = *head; d != NULL; d = tmp) {
      tmp = d->next;
      // MG_DEBUG ("%lu %lu dns poll", d->expire, now));
      if (now > d->expire) mg_error(d->c, "DNS timeout");
    }
  } else if (ev == MG_EV_READ) {
    struct mg_dns_message dm;
    int resolved = 0;
    if (mg_dns_parse(c->recv.buf, c->recv.len, &dm) == false) {
      MG_ERROR(("Unexpected DNS response:"));
      mg_hexdump(c->recv.buf, c->recv.len);
    } else {
      // MG_VERBOSE(("%s %d", dm.name, dm.resolved));
      for (d = *head; d != NULL; d = tmp) {
        tmp = d->next;
        // MG_INFO(("d %p %hu %hu", d, d->txnid, dm.txnid));
        if (dm.txnid != d->txnid) continue;
        if (d->c->is_resolving) {
          if (dm.resolved) {
            dm.addr.port = d->c->rem.port;  // Save port
            d->c->rem = dm.addr;            // Copy resolved address
            MG_DEBUG(
                ("%lu %s is %M", d->c->id, dm.name, mg_print_ip, &d->c->rem));
            mg_connect_resolved(d->c);
#if MG_ENABLE_IPV6
          } else if (dm.addr.is_ip6 == false && dm.name[0] != '\0' &&
                     c->mgr->use_dns6 == false) {
            struct mg_str x = mg_str(dm.name);
            mg_sendnsreq(d->c, &x, c->mgr->dnstimeout, &c->mgr->dns6, true);
#endif
          } else {
            mg_error(d->c, "%s DNS lookup failed", dm.name);
          }
        } else {
          MG_ERROR(("%lu already resolved", d->c->id));
        }
        mg_dns_free(head, d);
        resolved = 1;
      }
    }
    if (!resolved) MG_ERROR(("stray DNS reply"));
    c->recv.len = 0;
  } else if (ev == MG_EV_CLOSE) {
    for (d = *head; d != NULL; d = tmp) {
      tmp = d->next;
      mg_error(d->c, "DNS error");
      mg_dns_free(head, d);
    }
  }
}

static bool mg_dns_send(struct mg_connection *c, const struct mg_str *name,
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
    if (name->buf[i] == '.' || i >= name->len) {
      pkt.data[n] = (uint8_t) (i - n);
      memcpy(&pkt.data[n + 1], name->buf + n, i - n);
      n = i + 1;
    }
    if (i >= name->len) break;
  }
  memcpy(&pkt.data[n], "\x00\x00\x01\x00\x01", 5);  // A query
  n += 5;
  if (ipv6) pkt.data[n - 3] = 0x1c;  // AAAA query
  // memcpy(&pkt.data[n], "\xc0\x0c\x00\x1c\x00\x01", 6);  // AAAA query
  // n += 6;
  return mg_send(c, &pkt, sizeof(pkt.header) + n);
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
      // dnsc->c->is_hexdumping = 1;
    }
  }
  if (dnsc->c == NULL) {
    mg_error(c, "resolver");
  } else if ((d = (struct dns_data *) calloc(1, sizeof(*d))) == NULL) {
    mg_error(c, "resolve OOM");
  } else {
    struct dns_data *reqs = (struct dns_data *) c->mgr->active_dns_requests;
    d->txnid = reqs ? (uint16_t) (reqs->txnid + 1) : 1;
    d->next = (struct dns_data *) c->mgr->active_dns_requests;
    c->mgr->active_dns_requests = d;
    d->expire = mg_millis() + (uint64_t) ms;
    d->c = c;
    c->is_resolving = 1;
    MG_VERBOSE(("%lu resolving %.*s @ %s, txnid %hu", c->id, (int) name->len,
                name->buf, dnsc->url, d->txnid));
    if (!mg_dns_send(dnsc->c, name, d->txnid, ipv6)) {
      mg_error(dnsc->c, "DNS send");
    }
  }
}

void mg_resolve(struct mg_connection *c, const char *url) {
  struct mg_str host = mg_url_host(url);
  c->rem.port = mg_htons(mg_url_port(url));
  if (mg_aton(host, &c->rem)) {
    // host is an IP address, do not fire name resolution
    mg_connect_resolved(c);
  } else {
    // host is not an IP, send DNS resolution request
    struct mg_dns *dns = c->mgr->use_dns6 ? &c->mgr->dns6 : &c->mgr->dns4;
    mg_sendnsreq(c, &host, c->mgr->dnstimeout, dns, c->mgr->use_dns6);
  }
}

static const uint8_t mdns_answer[] = {
    0, 1,          // 2 bytes - record type, A
    0, 1,          // 2 bytes - address class, INET
    0, 0, 0, 120,  // 4 bytes - TTL
    0, 4           // 2 bytes - address length
};

static void mdns_cb(struct mg_connection *c, int ev, void *ev_data) {
  if (ev == MG_EV_READ) {
    struct mg_dns_header *qh = (struct mg_dns_header *) c->recv.buf;
    if (c->recv.len > 12 && (qh->flags & mg_htons(0xF800)) == 0) {
      // flags -> !resp, opcode=0 => query; ignore other opcodes and responses
      struct mg_dns_rr rr;  // Parse first question, offset 12 is header size
      size_t n = mg_dns_parse_rr(c->recv.buf, c->recv.len, 12, true, &rr);
      MG_VERBOSE(("mDNS request parsed, result=%d", (int) n));
      if (n > 0) {
        // RFC-6762 Appendix C, RFC2181 11: m(n + 1-63), max 255 + 0x0
        // buf and h declared here to ease future expansion to DNS-SD
        uint8_t buf[sizeof(struct mg_dns_header) + 256 + sizeof(mdns_answer) + 4];
        struct mg_dns_header *h = (struct mg_dns_header *) buf;
        char local_name[63 + 7];  // name label + '.' + local label + '\0'
        uint8_t name_len = (uint8_t) strlen((char *)c->fn_data);
        struct mg_dns_message dm;
        bool unicast = (rr.aclass & MG_BIT(15)) != 0;  // QU
        // uint16_t q = mg_ntohs(qh->num_questions);
        rr.aclass &= (uint16_t) ~MG_BIT(15);  // remove "QU" (unicast response)
        qh->num_questions = mg_htons(1);      // parser sanity
        mg_dns_parse(c->recv.buf, c->recv.len, &dm);
        if (name_len > (sizeof(local_name) - 7))  // leave room for .local\0
          name_len = sizeof(local_name) - 7;
        memcpy(local_name, c->fn_data, name_len);
        strcpy(local_name + name_len, ".local");  // ensure proper name.local\0
        if (strcmp(local_name, dm.name) == 0) {
          uint8_t *p = &buf[sizeof(*h)];
          memset(h, 0, sizeof(*h));            // clear header
          h->txnid = unicast ? qh->txnid : 0;  // RFC-6762 18.1
          // RFC-6762 6: 0 questions, 1 Answer, 0 Auth, 0 Additional RRs
          h->num_answers = mg_htons(1);  // only one answer
          h->flags = mg_htons(0x8400);   // Authoritative response
          *p++ = name_len;               // label 1
          memcpy(p, c->fn_data, name_len), p += name_len;
          *p++ = 5;  // label 2
          memcpy(p, "local", 5), p += 5;
          *p++ = 0;  // no more labels
          memcpy(p, mdns_answer, sizeof(mdns_answer)), p += sizeof(mdns_answer);
#if MG_ENABLE_TCPIP
          memcpy(p, &c->mgr->ifp->ip, 4), p += 4;
#else
          memcpy(p, c->data, 4), p += 4;
#endif
          if (!unicast) memcpy(&c->rem, &c->loc, sizeof(c->rem));
          mg_send(c, buf, (size_t)(p - buf));  // And send it!
          MG_DEBUG(("mDNS %c response sent", unicast ? 'U' : 'M'));
        }
      }
    }
    mg_iobuf_del(&c->recv, 0, c->recv.len);
  }
  (void) ev_data;
}

void mg_multicast_add(struct mg_connection *c, char *ip);
struct mg_connection *mg_mdns_listen(struct mg_mgr *mgr, char *name) {
#if MG_ARCH == MG_ARCH_WIN32
  const char *mcast_url = "udp://0.0.0.0:5353";
#else
  const char *mcast_url = "udp://224.0.0.251:5353";
#endif
  struct mg_connection *c =
      mg_listen(mgr, mcast_url, mdns_cb, name);
  if (c != NULL) mg_multicast_add(c, (char *)"224.0.0.251");
  return c;
}
