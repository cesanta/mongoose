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
  mg_free(d);
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
      memcpy(&dm->addr.addr.ip, &buf[ofs - 4], 4);
      dm->resolved = true;
      break;  // Return success
    } else if (rr.alen == 16 && rr.atype == 28 && rr.aclass == 1) {
      dm->addr.is_ip6 = true;
      memcpy(&dm->addr.addr.ip, &buf[ofs - 16], 16);
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

bool mg_dnsc_init(struct mg_mgr *mgr, struct mg_dns *dnsc);
bool mg_dnsc_init(struct mg_mgr *mgr, struct mg_dns *dnsc) {
  if (dnsc->url == NULL) {
    mg_error(0, "DNS server URL is NULL. Call mg_mgr_init()");
    return false;
  }
  if (dnsc->c == NULL) {
    dnsc->c = mg_connect(mgr, dnsc->url, NULL, NULL);
    if (dnsc->c == NULL) return false;
    dnsc->c->pfn = dns_cb;
  }
  return true;
}

static void mg_sendnsreq(struct mg_connection *c, struct mg_str *name, int ms,
                         struct mg_dns *dnsc, bool ipv6) {
  struct dns_data *d = NULL;
  if (!mg_dnsc_init(c->mgr, dnsc)) {
    mg_error(c, "resolver");
  } else if ((d = (struct dns_data *) mg_calloc(1, sizeof(*d))) == NULL) {
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

static uint8_t *build_name(struct mg_str *name, uint8_t *p) {
  *p++ = (uint8_t) name->len;  // label 1
  memcpy(p, name->buf, name->len), p += name->len;
  *p++ = 5;  // label 2
  memcpy(p, "local", 5), p += 5;
  *p++ = 0;  // no more labels
  return p;
}

static uint8_t *build_a_record(struct mg_connection *c, uint8_t *p) {
  memcpy(p, mdns_answer, sizeof(mdns_answer)), p += sizeof(mdns_answer);
#if MG_ENABLE_TCPIP
  memcpy(p, &c->mgr->ifp->ip, 4), p += 4;
#else
  memcpy(p, c->data, 4), p += 4;
#endif
  return p;
}

static uint8_t *build_srv_name(uint8_t *p, struct mg_dnssd_record *r) {
  *p++ = (uint8_t) r->srvcproto.len - 5;  // label 1, up to '._tcp'
  memcpy(p, r->srvcproto.buf, r->srvcproto.len), p += r->srvcproto.len;
  p[-5] = 4;  // label 2, '_tcp', overwrite '.'
  *p++ = 5;   // label 3
  memcpy(p, "local", 5), p += 5;
  *p++ = 0;  // no more labels
  return p;
}

#if 0
// TODO(): for listing
static uint8_t *build_mysrv_name(struct mg_str *name, uint8_t *p,
                                 struct mg_dnssd_record *r) {
  *p++ = name->len;  // label 1
  memcpy(p, name->buf, name->len), p += name->len;
  return build_srv_name(p, r);
}
#endif

static uint8_t *build_ptr_record(struct mg_str *name, uint8_t *p, uint16_t o) {
  uint16_t offset = mg_htons(o);
  memcpy(p, mdns_answer, sizeof(mdns_answer));
  p[1] = 12;  // overwrite record type
  p += sizeof(mdns_answer);
  p[-1] = (uint8_t) name->len +
          3;  // overwrite response length, label length + label + offset
  *p++ = (uint8_t) name->len;                       // response: label 1
  memcpy(p, name->buf, name->len), p += name->len;  // copy label
  memcpy(p, &offset, 2);
  *p |= 0xC0, p += 2;
  return p;
}

static uint8_t *build_srv_record(struct mg_str *name, uint8_t *p,
                                 struct mg_dnssd_record *r, uint16_t o) {
  uint16_t port = mg_htons(r->port);
  uint16_t offset = mg_htons(o);
  memcpy(p, mdns_answer, sizeof(mdns_answer));
  p[1] = 33;  // overwrite record type
  p += sizeof(mdns_answer);
  p[-1] = (uint8_t) name->len + 9;  // overwrite response length (4+2+1+2)
  *p++ = 0;                         // priority
  *p++ = 0;
  *p++ = 0;  // weight
  *p++ = 0;
  memcpy(p, &port, 2), p += 2;  // port
  *p++ = (uint8_t) name->len;   // label 1
  memcpy(p, name->buf, name->len), p += name->len;
  memcpy(p, &offset, 2);
  *p |= 0xC0, p += 2;
  return p;
}

static uint8_t *build_txt_record(uint8_t *p, struct mg_dnssd_record *r) {
  uint16_t len = mg_htons((uint16_t) r->txt.len);
  memcpy(p, mdns_answer, sizeof(mdns_answer));
  p[1] = 16;  // overwrite record type
  p += sizeof(mdns_answer);
  memcpy(p - 2, &len, 2);  // overwrite response length
  memcpy(p, r->txt.buf, r->txt.len), p += r->txt.len;  // copy record verbatim
  return p;
}

// RFC-6762 16: case-insensitivity --> RFC-1034, 1035

static void handle_mdns_record(struct mg_connection *c) {
  struct mg_dns_header *qh = (struct mg_dns_header *) c->recv.buf;
  struct mg_dns_rr rr;
  size_t n;
  // flags -> !resp, opcode=0 => query; ignore other opcodes and responses
  if (c->recv.len <= 12 || (qh->flags & mg_htons(0xF800)) != 0) return;
  // Parse first question, offset 12 is header size
  n = mg_dns_parse_rr(c->recv.buf, c->recv.len, 12, true, &rr);
  MG_VERBOSE(("mDNS request parsed, result=%d", (int) n));
  if (n > 0) {
    // RFC-6762 Appendix C, RFC2181 11: m(n + 1-63), max 255 + 0x0
    uint8_t buf[sizeof(struct mg_dns_header) + 256 + sizeof(mdns_answer) + 4];
    struct mg_dns_header *h = (struct mg_dns_header *) buf;
    uint8_t *p = &buf[sizeof(*h)];
    char name[256];
    uint8_t name_len;
    // uint16_t q = mg_ntohs(qh->num_questions);
    struct mg_str defname = mg_str((const char *) c->fn_data);
    struct mg_str *respname;
    struct mg_mdns_req req;
    memset(&req, 0, sizeof(req));
    req.is_unicast = (rr.aclass & MG_BIT(15)) != 0;  // QU
    rr.aclass &= (uint16_t) ~MG_BIT(15);  // remove "QU" (unicast response)
    qh->num_questions = mg_htons(1);      // parser sanity
    mg_dns_parse_name(c->recv.buf, c->recv.len, 12, name, sizeof(name));
    name_len = (uint8_t) strlen(name);  // verify it ends in .local
    if (strcmp(".local", &name[name_len - 6]) != 0 ||
        (rr.aclass != 1 && rr.aclass != 0xff))
      return;
    name[name_len -= 6] = '\0';  // remove .local
    MG_VERBOSE(("RR %u %u %s", (unsigned int) rr.atype,
                (unsigned int) rr.aclass, name));
    if (rr.atype == 1) {  // A
      // TODO(): ensure c->fn_data ends in \0
      // if we have a name to match, go; otherwise users will match and fill
      // req.r.name and set req.is_resp
      if (c->fn_data != NULL && mg_casecmp((char *) c->fn_data, name) != 0)
        return;
      req.is_resp = (c->fn_data != NULL);
      req.reqname = mg_str_n(name, name_len);
      mg_call(c, MG_EV_MDNS_A, &req);
    } else  // users have to match the request to something in their db, then
            // fill req.r and set req.is_resp
      if (rr.atype == 12) {  // PTR
        if (strcmp("_services._dns-sd._udp", name) == 0) req.is_listing = true;
        MG_DEBUG(
            ("PTR request for %s", req.is_listing ? "services listing" : name));
        req.reqname = mg_str_n(name, name_len);
        mg_call(c, MG_EV_MDNS_PTR, &req);
      } else if (rr.atype == 33 || rr.atype == 16) {  // SRV or TXT
        MG_DEBUG(("%s request for %s", rr.atype == 33 ? "SRV" : "TXT", name));
        // if possible, check it starts with our name, users will check it ends
        // in a service name they handle
        if (c->fn_data != NULL) {
          if (mg_strcasecmp(defname, mg_str_n(name, defname.len)) != 0 ||
              name[defname.len] != '.')
            return;
          req.reqname =
              mg_str_n(name + defname.len + 1, name_len - defname.len - 1);
          MG_DEBUG(
              ("That's us, handing %.*s", req.reqname.len, req.reqname.buf));
        } else {
          req.reqname = mg_str_n(name, name_len);
        }
        mg_call(c, rr.atype == 33 ? MG_EV_MDNS_SRV : MG_EV_MDNS_TXT, &req);
      } else {  // unhandled record
        return;
      }
    if (!req.is_resp) return;
    respname = req.respname.buf != NULL ? &req.respname : &defname;

    memset(h, 0, sizeof(*h));                   // clear header
    h->txnid = req.is_unicast ? qh->txnid : 0;  // RFC-6762 18.1
    h->num_answers = mg_htons(1);  // RFC-6762 6: 0 questions, 1 Answer
    h->flags = mg_htons(0x8400);   // Authoritative response
    if (req.is_listing) {
      // TODO(): RFC-6762 6: each responder SHOULD delay its response by a
      // random amount of time selected with uniform random distribution in the
      // range 20-120 ms.
      // TODO():
      return;
    } else if (rr.atype == 12) {  // PTR requested, serve PTR + SRV + TXT + A
      // TODO(): RFC-6762 6: each responder SHOULD delay its response by a
      // random amount of time selected with uniform random distribution in the
      // range 20-120 ms. Response to PTR is local_name._myservice._tcp.local
      uint8_t *o = p, *aux;
      uint16_t offset;
      if (respname->buf == NULL || respname->len == 0) return;
      h->num_other_prs = mg_htons(3);  // 3 additional records
      p = build_srv_name(p, req.r);
      aux = build_ptr_record(respname, p, (uint16_t) (o - buf));
      o = p + sizeof(mdns_answer);  // point to PTR response (full srvc name)
      offset = mg_htons((uint16_t) (o - buf));
      o = p - 7;  // point to '.local' label (\x05local\x00)
      p = aux;
      memcpy(p, &offset, 2);  // point to full srvc name, in record
      *p |= 0xC0, p += 2;
      aux = p;
      p = build_srv_record(respname, p, req.r, (uint16_t) (o - buf));
      o = aux + sizeof(mdns_answer) + 6;  // point to target in SRV
      memcpy(p, &offset, 2);              // point to full srvc name, in record
      *p |= 0xC0, p += 2;
      p = build_txt_record(p, req.r);
      offset = mg_htons((uint16_t) (o - buf));
      memcpy(p, &offset, 2);  // point to target name, in record
      *p |= 0xC0, p += 2;
      p = build_a_record(c, p);
    } else if (rr.atype == 16) {  // TXT requested
      p = build_srv_name(p, req.r);
      p = build_txt_record(p, req.r);
    } else if (rr.atype == 33) {  // SRV requested, serve SRV + A
      uint8_t *o, *aux;
      uint16_t offset;
      if (respname->buf == NULL || respname->len == 0) return;
      h->num_other_prs = mg_htons(1);  // 1 additional record
      p = build_srv_name(p, req.r);
      o = p - 7;  // point to '.local' label (\x05local\x00)
      aux = p;
      p = build_srv_record(respname, p, req.r, (uint16_t) (o - buf));
      o = aux + sizeof(mdns_answer) + 6;  // point to target in SRV
      offset = mg_htons((uint16_t) (o - buf));
      memcpy(p, &offset, 2);  // point to target name, in record
      *p |= 0xC0, p += 2;
      p = build_a_record(c, p);
    } else {  // A requested
      // RFC-6762 6: 0 Auth, 0 Additional RRs
      if (respname->buf == NULL || respname->len == 0) return;
      p = build_name(respname, p);
      p = build_a_record(c, p);
    }
    if (!req.is_unicast) memcpy(&c->rem, &c->loc, sizeof(c->rem));
    mg_send(c, buf, (size_t) (p - buf));  // And send it!
    MG_DEBUG(("mDNS %c response sent", req.is_unicast ? 'U' : 'M'));
  }
}

static void mdns_cb(struct mg_connection *c, int ev, void *ev_data) {
  if (ev == MG_EV_READ) {
    handle_mdns_record(c);
    mg_iobuf_del(&c->recv, 0, c->recv.len);
  }
  (void) ev_data;
}

void mg_multicast_add(struct mg_connection *c, char *ip);
struct mg_connection *mg_mdns_listen(struct mg_mgr *mgr, mg_event_handler_t fn,
                                     void *fn_data) {
  struct mg_connection *c =
      mg_listen(mgr, "udp://224.0.0.251:5353", fn, fn_data);
  if (c == NULL) return NULL;
  c->pfn = mdns_cb, c->pfn_data = fn_data;
  mg_multicast_add(c, (char *) "224.0.0.251");
  return c;
}
