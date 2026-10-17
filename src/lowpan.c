#include "l2.h"
#include "log.h"
#include "net.h"
#include "net_builtin.h"
#include "printf.h"
#include "util.h"

// IPv6 over IEEE 802.15.4, 6LoWPAN. RFC 4944: frame format, fragmentation.
// RFC 6282: IPHC header compression, UDP next header compression.
// Driver tx()/rx() carry raw 802.15.4 frames: MAC header, payload, no FCS.
//
// L2 addresses are 8 bytes, struct mg_l2addr. Extended address: EUI-64, big
// endian. Short address S: 0000:00ff:fe00:SSSS, which is also its IPv6
// interface identifier, RFC 6282 3.2.2. Broadcast is short address 0xffff.
// No link-layer security: secured frames are dropped

#if MG_ENABLE_TCPIP && MG_ENABLE_IPV6

#define MGI_LOWPAN_FRAME_MAX 125   // 802.15.4 frame without FCS: 127 - 2
#define MGI_LOWPAN_MTU 1280        // IPv6 minimum MTU, RFC 8200 5
#define MGI_LOWPAN_REASM_MS 5000   // Drop incomplete datagrams after this
#define MGI_FCF_DATA 1U            // Frame control: data frame
#define MGI_FCF_SECURITY 0x08U     // Frame control: security enabled
#define MGI_FCF_ACK_REQUEST 0x20U  // Frame control: acknowledgment request
#define MGI_FCF_PANID_COMP 0x40U   // Frame control: PAN ID compression
#define MGI_FCF_IE 0x200U          // Frame control: IE present, 2015 frames
#define MGI_FCF_VERSION_2006 0x1000U
#define MGI_AM_SHORT 2U            // Addressing mode: 16-bit short address
#define MGI_AM_EXT 3U              // Addressing mode: 64-bit extended address
#define MGI_DISPATCH_IPV6 0x41     // Uncompressed IPv6, RFC 4944 5.1
#define MGI_DISPATCH_FRAG1 0xc0    // First fragment, 11000xxx
#define MGI_DISPATCH_FRAGN 0xe0    // Subsequent fragment, 11100xxx

// Parsed 802.15.4 MAC header
struct lowpan_mhr {
  uint8_t dst[8], src[8];  // Addresses, struct mg_l2addr format
  uint16_t dst_pan;        // Destination PAN ID
  size_t len;              // MAC header length, without security header
  bool secured;            // Security enabled
};

static struct mg_l2addr s_rxsrc;  // Source address returned by getaddr()

static void set_short(uint8_t *a, uint16_t s) {
  a[0] = a[1] = a[2] = a[5] = 0, a[3] = 0xff, a[4] = 0xfe;
  a[6] = (uint8_t) (s >> 8), a[7] = (uint8_t) s;
}

static bool is_zero(const uint8_t *p, size_t n) {
  while (n-- > 0)
    if (*p++ != 0) return false;
  return true;
}

// Read an address field, over-the-air byte order is little endian
static size_t read_addr(const uint8_t *p, unsigned mode, uint8_t *a) {
  int i;
  if (mode == MGI_AM_SHORT) {
    set_short(a, (uint16_t) (p[0] | (p[1] << 8)));
    return 2;
  }
  for (i = 0; i < 8; i++) a[i] = p[7 - i];
  return 8;
}

// Parse MAC header of a data frame, 2003/2006 format
static bool parse_mhr(const uint8_t *f, size_t n, struct lowpan_mhr *h) {
  unsigned fcf, dm, sm, ver;
  size_t i = 3;
  if (n < 3) return false;
  fcf = (unsigned) (f[0] | (f[1] << 8));
  dm = (fcf >> 10) & 3, sm = (fcf >> 14) & 3, ver = (fcf >> 12) & 3;
  if ((fcf & 7) != MGI_FCF_DATA || (fcf & MGI_FCF_IE) || ver > 1) return false;
  h->secured = (fcf & MGI_FCF_SECURITY) != 0;
  if (dm < MGI_AM_SHORT || sm < MGI_AM_SHORT) return false;  // Need both addresses
  if (i + 2 > n) return false;
  h->dst_pan = (uint16_t) (f[i] | (f[i + 1] << 8)), i += 2;
  if (i + (dm == MGI_AM_EXT ? 8 : 2) > n) return false;
  i += read_addr(f + i, dm, h->dst);
  if ((fcf & MGI_FCF_PANID_COMP) == 0) i += 2;  // Source PAN ID, if present
  if (i + (sm == MGI_AM_EXT ? 8 : 2) > n) return false;
  i += read_addr(f + i, sm, h->src);
  h->len = i;
  return true;
}

static bool is_short(const uint8_t *a) {
  return a[0] == 0 && a[1] == 0 && a[2] == 0 && a[3] == 0xff && a[4] == 0xfe &&
         a[5] == 0;
}

static bool is_bcast(const uint8_t *a) {
  return is_short(a) && a[6] == 0xff && a[7] == 0xff;
}

// IPv6 interface identifier for an L2 address, RFC 6282 3.2.2
static void make_iid(const uint8_t *a, uint8_t *iid) {
  memcpy(iid, a, 8);
  if (!is_short(a)) iid[0] ^= 2;  // EUI-64: flip universal/local bit
}

static size_t write_addr(uint8_t *p, const uint8_t *a) {
  int i;
  if (is_short(a)) {
    p[0] = a[7], p[1] = a[6];
    return 2;
  }
  for (i = 0; i < 8; i++) p[i] = a[7 - i];
  return 8;
}

// Write MAC header for a data frame from us to dst. Return its length
static size_t write_mhr(struct mg_tcpip_if *ifp, const uint8_t *dst,
                        uint8_t *f) {
  struct lowpan_data *d = &ifp->l2data.lowpan;
  unsigned fcf = MGI_FCF_DATA | MGI_FCF_PANID_COMP | MGI_FCF_VERSION_2006;
  size_t i = 3;
  if (!is_bcast(dst)) fcf |= MGI_FCF_ACK_REQUEST;  // Unicast: MAC retries
  fcf |= (is_short(dst) ? MGI_AM_SHORT : MGI_AM_EXT) << 10;
  fcf |= (is_short(ifp->mac) ? MGI_AM_SHORT : MGI_AM_EXT) << 14;
  f[0] = (uint8_t) fcf, f[1] = (uint8_t) (fcf >> 8), f[2] = d->seq++;
  f[i++] = (uint8_t) d->pan_id, f[i++] = (uint8_t) (d->pan_id >> 8);
  i += write_addr(f + i, dst);
  i += write_addr(f + i, ifp->mac);
  return i;
}

// Compress a unicast IPv6 address: 2 bits for the SAM/DAM field, inline
// bytes go to out. RFC 6282 3.1.1, SAC/DAC = 0
static unsigned compress_addr(const uint8_t *ip, const uint8_t *l2,
                              uint8_t *out, size_t *n) {
  static const uint8_t ll[8] = {0xfe, 0x80, 0, 0, 0, 0, 0, 0};
  uint8_t iid[8];
  if (memcmp(ip, ll, 8) != 0) {
    memcpy(out + *n, ip, 16), *n += 16;
    return 0;
  }
  make_iid(l2, iid);
  if (memcmp(ip + 8, iid, 8) == 0) return 3;  // Derived from L2 address
  if (is_short(ip + 8)) {                    // 0000:00ff:fe00:XXXX
    memcpy(out + *n, ip + 14, 2), *n += 2;
    return 2;
  }
  memcpy(out + *n, ip + 8, 8), *n += 8;
  return 1;
}

// Compress IPv6 header to IPHC, RFC 6282 3.1. Next header is carried inline.
// Return compressed header length
static size_t iphc_compress(const uint8_t *ip, const uint8_t *src_l2,
                            const uint8_t *dst_l2, uint8_t *out) {
  unsigned tc = (unsigned) (((ip[0] & 0x0f) << 4) | (ip[1] >> 4));
  unsigned ecn = tc & 3, dscp = tc >> 2, tf, hl;
  uint32_t flow = ((uint32_t) (ip[1] & 0x0f) << 16) |
                  ((uint32_t) ip[2] << 8) | ip[3];
  size_t n = 2;
  if (tc == 0 && flow == 0) {
    tf = 3;
  } else if (flow == 0) {
    tf = 2, out[n++] = (uint8_t) ((ecn << 6) | dscp);
  } else if (dscp == 0) {
    tf = 1, out[n++] = (uint8_t) ((ecn << 6) | (flow >> 16));
    out[n++] = (uint8_t) (flow >> 8), out[n++] = (uint8_t) flow;
  } else {
    tf = 0, out[n++] = (uint8_t) ((ecn << 6) | dscp);
    out[n++] = (uint8_t) (flow >> 16), out[n++] = (uint8_t) (flow >> 8);
    out[n++] = (uint8_t) flow;
  }
  out[n++] = ip[6];  // Next header, inline
  hl = ip[7] == 1 ? 1 : ip[7] == 64 ? 2 : ip[7] == 255 ? 3 : 0;
  if (hl == 0) out[n++] = ip[7];
  out[0] = (uint8_t) (0x60 | (tf << 3) | hl);
  out[1] = 0;
  if (is_zero(ip + 8, 16)) {
    out[1] |= 0x40;  // SAC = 1, SAM = 00: unspecified address
  } else {
    out[1] |= (uint8_t) (compress_addr(ip + 8, src_l2, out, &n) << 4);
  }
  if (ip[24] == 0xff) {  // Multicast, M = 1
    out[1] |= 0x08;
    if (ip[25] == 0x02 && is_zero(ip + 26, 13)) {  // ff02::00XX
      out[1] |= 3, out[n++] = ip[39];
    } else if (is_zero(ip + 26, 11)) {  // ffXX::00XX:XXXX
      out[1] |= 2, out[n++] = ip[25];
      memcpy(out + n, ip + 37, 3), n += 3;
    } else if (is_zero(ip + 26, 9)) {  // ffXX::00XX:XXXX:XXXX
      out[1] |= 1, out[n++] = ip[25];
      memcpy(out + n, ip + 35, 5), n += 5;
    } else {
      memcpy(out + n, ip + 24, 16), n += 16;
    }
  } else {
    out[1] |= (uint8_t) compress_addr(ip + 24, dst_l2, out, &n);
  }
  return n;
}

// Decompress a unicast address. Prefix is the context prefix for SAC/DAC = 1,
// or NULL for link-local. Return inline bytes used
static size_t decompress_addr(unsigned mode, const uint8_t *p, size_t n,
                              const uint8_t *l2, const uint8_t *prefix,
                              uint8_t *ip) {
  static const size_t sizes[] = {16, 8, 2, 0};
  if (sizes[mode] > n) return n + 1;  // Truncated: caller sees i > n
  if (mode == 0) {
    memcpy(ip, p, 16);
  } else {
    memset(ip, 0, 16), ip[0] = 0xfe, ip[1] = 0x80;
    if (prefix != NULL) memcpy(ip, prefix, 8);
    if (mode == 1) memcpy(ip + 8, p, 8);
    if (mode == 2) set_short(ip + 8, (uint16_t) ((p[0] << 8) | p[1]));
    if (mode == 3) make_iid(l2, ip + 8);
  }
  return sizes[mode];
}

// Decompress IPHC or uncompressed IPv6 header, RFC 6282 3.1, 4.3.
// Write IPv6 header (and UDP header if compressed) to ip. Return its length,
// or 0 on error. Set *used to the number of compressed bytes consumed
static size_t iphc_decompress(const struct lowpan_mhr *h, const uint8_t *p,
                              size_t n, uint8_t *ip, size_t *used, bool *udp) {
  static const size_t tf_sizes[] = {4, 3, 1, 0};
  unsigned b0, b1, tf, tc = 0, sam, dam;
  uint32_t flow = 0;
  size_t i = 2;
  *udp = false;
  if (n > 0 && p[0] == MGI_DISPATCH_IPV6) {
    if (n < 41) return 0;
    memcpy(ip, p + 1, 40);
    *used = 41;
    return 40;
  }
  if (n < 2 || (p[0] & 0xe0) != 0x60) return 0;
  b0 = p[0], b1 = p[1], tf = (b0 >> 3) & 3, sam = (b1 >> 4) & 3, dam = b1 & 3;
  if (b1 & 0x80) return 0;  // CID: no compression contexts
  if (i + tf_sizes[tf] + ((b0 & 4) ? 0 : 1) + ((b0 & 3) ? 0 : 1) > n)
    return 0;  // TF, next header, hop limit
  if (tf == 0 || tf == 2) tc = (unsigned) (((p[i] & 0x3f) << 2) | (p[i] >> 6));
  if (tf == 1) tc = p[i] >> 6;
  if (tf == 0) flow = ((uint32_t) (p[i + 1] & 0x0f) << 16) |
                      ((uint32_t) p[i + 2] << 8) | p[i + 3];
  if (tf == 1) flow = ((uint32_t) (p[i] & 0x0f) << 16) |
                      ((uint32_t) p[i + 1] << 8) | p[i + 2];
  i += tf_sizes[tf];
  ip[0] = (uint8_t) (0x60 | (tc >> 4));
  ip[1] = (uint8_t) ((tc << 4) | (flow >> 16));
  ip[2] = (uint8_t) (flow >> 8), ip[3] = (uint8_t) flow;
  ip[4] = ip[5] = 0;  // Payload length, set by caller
  if ((b0 & 4) == 0) ip[6] = p[i++];  // Next header inline
  if ((b0 & 3) == 0) ip[7] = p[i++];
  if ((b0 & 3) != 0) ip[7] = (b0 & 3) == 1 ? 1 : (b0 & 3) == 2 ? 64 : 255;
  // Source address
  if ((b1 & 0x40) && sam == 0) {  // SAC = 1, SAM = 00: unspecified address
    memset(ip + 8, 0, 16);
  } else {
    if (b1 & 0x40) return 0;  // SAC = 1: no compression contexts
    i += decompress_addr(sam, p + i, n - i, h->src, NULL, ip + 8);
    if (i > n) return 0;
  }
  // Destination address
  if ((b1 & 0x0c) == 0x08) {  // Multicast, stateless
    static const size_t sizes[] = {16, 6, 4, 1};
    if (i + sizes[dam] > n) return 0;
    memset(ip + 24, 0, 16), ip[24] = 0xff;
    if (dam == 0) memcpy(ip + 24, p + i, 16);
    if (dam == 1) ip[25] = p[i], memcpy(ip + 35, p + i + 1, 5);
    if (dam == 2) ip[25] = p[i], memcpy(ip + 37, p + i + 1, 3);
    if (dam == 3) ip[25] = 2, ip[39] = p[i];
    i += sizes[dam];
  } else if ((b1 & 0x08) == 0) {  // Unicast
    if (b1 & 0x04) return 0;  // DAC = 1: no compression contexts
    i += decompress_addr(dam, p + i, n - i, h->dst, NULL, ip + 24);
    if (i > n) return 0;
  } else {
    return 0;  // Stateful multicast is not supported
  }
  if (b0 & 4) {  // Next header compressed: only UDP, RFC 6282 4.3
    static const size_t port_sizes[] = {4, 3, 3, 1};
    uint8_t *u = ip + 40;
    unsigned nhc;
    if (i >= n || (p[i] & 0xf8) != 0xf0) return 0;
    nhc = p[i++];
    if (nhc & 4) return 0;  // Elided checksum is not supported
    if (i + port_sizes[nhc & 3] + 2 > n) return 0;  // Ports, checksum
    switch (nhc & 3) {
      case 0: memcpy(u, p + i, 4), i += 4; break;
      case 1: memcpy(u, p + i, 2), u[2] = 0xf0, u[3] = p[i + 2], i += 3; break;
      case 2: u[0] = 0xf0, u[1] = p[i], memcpy(u + 2, p + i + 1, 2), i += 3; break;
      default:
        u[0] = u[2] = 0xf0, u[1] = (uint8_t) (0xb0 | (p[i] >> 4));
        u[3] = (uint8_t) (0xb0 | (p[i] & 0x0f)), i += 1;
        break;
    }
    u[4] = u[5] = 0;                    // UDP length, set by caller
    memcpy(u + 6, p + i, 2), i += 2;    // Checksum
    ip[6] = 17;
    *udp = true;
  }
  if (i > n) return 0;
  *used = i;
  return *udp ? 48 : 40;
}

// Set IPv6 payload length and, for compressed UDP, UDP length
static void set_lengths(uint8_t *ip, size_t len, bool udp) {
  size_t plen = len - 40;
  ip[4] = (uint8_t) (plen >> 8), ip[5] = (uint8_t) plen;
  if (udp) ip[44] = (uint8_t) (plen >> 8), ip[45] = (uint8_t) plen;
}

void mg_l2_lowpan_init(struct mg_tcpip_if *ifp) {
  struct lowpan_data *d = &ifp->l2data.lowpan;
  if (is_zero(ifp->mac, 8)) {
    ifp->mac[0] = 0x02;  // Locally administered EUI-64
    mg_random(&ifp->mac[1], 7);
    MG_INFO(("EUI-64 not set. Generated random: %M", mg_print_hex, 8,
             ifp->mac));
  }
  ifp->l2mtu = MGI_LOWPAN_MTU;
  ifp->framesize = MGI_LOWPAN_MTU;  // TX buffer holds an uncompressed packet
  // Polling drivers receive one raw 802.15.4 frame at a time into the queue
  // buffer, so it does not need to be framesize: 127 bytes max
  if (ifp->recv_queue.size == 0) ifp->recv_queue.size = 128;
  d->short_addr = 0xfffe;
  d->buf = (uint8_t *) mg_calloc(1, MGI_LOWPAN_MTU);
  if (d->buf == NULL) MG_ERROR(("OOM"));
}

bool mg_l2_lowpan_poll(struct mg_tcpip_if *ifp, bool expired_1000ms) {
  struct lowpan_data *d = &ifp->l2data.lowpan;
  if (expired_1000ms && d->size != 0 && ifp->now > d->expire) d->size = 0;
  return true;
}

uint8_t *mg_l2_lowpan_header(struct mg_tcpip_if *ifp, enum mg_l2proto proto,
                             struct mg_l2addr *src, struct mg_l2addr *dst,
                             uint8_t *frame) {
  struct lowpan_data *d = &ifp->l2data.lowpan;
  d->proto = (uint8_t) proto;
  memcpy(d->dst, dst, sizeof(d->dst));
  (void) src;
  return frame;  // Uncompressed packet starts at the frame start
}

// Compress the packet, send all fragments but the last directly, put the
// last fragment into the TX buffer for the stack to send
size_t mg_l2_lowpan_trailer(struct mg_tcpip_if *ifp, size_t len,
                            uint8_t *cur) {
  struct lowpan_data *d = &ifp->l2data.lowpan;
  uint8_t *ip = cur - len, hdr[48], f[MGI_LOWPAN_FRAME_MAX];
  const uint8_t *dst = d->dst;
  size_t hlen, mlen, avail, chunk, off, n;
  if (d->proto != MG_TCPIP_L2PROTO_IPV6 || len < 40 || len > MGI_LOWPAN_MTU)
    return 0;
  hlen = iphc_compress(ip, ifp->mac, dst, hdr);
  mlen = write_mhr(ifp, dst, f);
  avail = MGI_LOWPAN_FRAME_MAX - mlen;
  if (hlen + len - 40 <= avail) {  // Fits into one frame
    memcpy(f + mlen, hdr, hlen);
    memcpy(f + mlen + hlen, ip + 40, len - 40);
    n = mlen + hlen + len - 40;
  } else {  // Fragment, RFC 4944 5.3. Offsets are in uncompressed bytes
    uint16_t tag = d->tag++;
    chunk = (avail - 4 - hlen) & ~(size_t) 7;  // 40 + chunk: multiple of 8
    f[mlen] = (uint8_t) (MGI_DISPATCH_FRAG1 | (len >> 8));
    f[mlen + 1] = (uint8_t) len;
    f[mlen + 2] = (uint8_t) (tag >> 8), f[mlen + 3] = (uint8_t) tag;
    memcpy(f + mlen + 4, hdr, hlen);
    memcpy(f + mlen + 4 + hlen, ip + 40, chunk);
    n = mlen + 4 + hlen + chunk;
    ifp->driver->tx(f, n, ifp);
    for (off = 40 + chunk;;) {
      mlen = write_mhr(ifp, dst, f);
      chunk = len - off;
      if (chunk > ((avail - 5) & ~(size_t) 7)) chunk = (avail - 5) & ~(size_t) 7;
      f[mlen] = (uint8_t) (MGI_DISPATCH_FRAGN | (len >> 8));
      f[mlen + 1] = (uint8_t) len;
      f[mlen + 2] = (uint8_t) (tag >> 8), f[mlen + 3] = (uint8_t) tag;
      f[mlen + 4] = (uint8_t) (off / 8);
      memcpy(f + mlen + 5, ip + off, chunk);
      n = mlen + 5 + chunk;
      off += chunk;
      if (off >= len) break;
      ifp->driver->tx(f, n, ifp);
    }
  }
  memcpy(ifp->tx.buf, f, n);
  return n;
}

bool mg_l2_lowpan_rx(struct mg_tcpip_if *ifp, enum mg_l2proto *proto,
                     struct mg_str *pay, struct mg_str *raw) {
  struct lowpan_data *d = &ifp->l2data.lowpan;
  uint8_t *p = (uint8_t *) raw->buf;
  size_t n = raw->len, hlen, used, size, off;
  struct lowpan_mhr h;
  bool udp;
  if (d->buf == NULL || !parse_mhr(p, n, &h)) return false;
  if (h.dst_pan != d->pan_id && h.dst_pan != 0xffff) return false;
  if (!is_bcast(h.dst) && memcmp(h.dst, ifp->mac, 8) != 0 &&
      !(is_short(h.dst) && d->short_addr != 0xfffe &&
        MG_LOAD_BE16(h.dst + 6) == d->short_addr))
    return false;
  if (h.secured) return false;  // No link-layer security
  p += h.len, n -= h.len;
  if (n < 1) return false;
  if ((p[0] & 0xf8) == MGI_DISPATCH_FRAG1 || (p[0] & 0xf8) == MGI_DISPATCH_FRAGN) {
    bool first = (p[0] & 0xf8) == MGI_DISPATCH_FRAG1;
    uint16_t tag;
    if (n < (first ? 4U : 5U)) return false;
    size = (size_t) (((p[0] & 7) << 8) | p[1]);
    tag = (uint16_t) ((p[2] << 8) | p[3]);
    if (size < 40 || size > MGI_LOWPAN_MTU) return false;
    if (first) {
      p += 4, n -= 4;
      hlen = iphc_decompress(&h, p, n, d->buf, &used, &udp);
      if (hlen == 0 || hlen + n - used > size) return false;
      memcpy(d->buf + hlen, p + used, n - used);
      d->size = (uint16_t) size, d->tag_rx = tag, d->udp = udp;
      d->received = (uint16_t) (hlen + n - used);
      memcpy(d->src, h.src, sizeof(d->src));
      d->expire = ifp->now + MGI_LOWPAN_REASM_MS;
    } else {
      off = (size_t) p[4] * 8;
      p += 5, n -= 5;
      if (d->size != size || d->tag_rx != tag ||
          memcmp(d->src, h.src, sizeof(d->src)) != 0 || off + n > size)
        return false;  // Not the datagram being reassembled
      memcpy(d->buf + off, p, n);
      d->received = (uint16_t) (d->received + n);
    }
    if (d->size == 0 || d->received < d->size) return false;
    set_lengths(d->buf, d->size, d->udp);
    pay->buf = (char *) d->buf, pay->len = d->size;
    d->size = 0;
  } else {
    hlen = iphc_decompress(&h, p, n, d->buf, &used, &udp);
    if (hlen == 0 || hlen + n - used > MGI_LOWPAN_MTU) return false;
    memcpy(d->buf + hlen, p + used, n - used);
    set_lengths(d->buf, hlen + n - used, udp);
    pay->buf = (char *) d->buf, pay->len = hlen + n - used;
  }
  *proto = MG_TCPIP_L2PROTO_IPV6;
  return true;
}

struct mg_l2addr *mg_l2_lowpan_getaddr(struct mg_tcpip_if *ifp,
                                       uint8_t *frame) {
  struct lowpan_mhr h;
  memset(&s_rxsrc, 0, sizeof(s_rxsrc));
  if (parse_mhr(frame, MGI_LOWPAN_FRAME_MAX, &h))
    memcpy(&s_rxsrc, h.src, sizeof(s_rxsrc));
  (void) ifp;
  return &s_rxsrc;
}

extern struct mg_l2addr s_mapip;

// All broadcast and multicast goes to the 802.15.4 broadcast address
struct mg_l2addr *mg_l2_lowpan_mapip(enum mg_l2addrtype addrtype,
                                     struct mg_addr *addr) {
  set_short((uint8_t *) &s_mapip, 0xffff);
  (void) addrtype, (void) addr;
  return &s_mapip;
}

bool mg_l2_lowpan_genip6(uint64_t *ip6, uint8_t prefix_len,
                         struct mg_l2addr *l2addr) {
  if (prefix_len > 64) {
    MG_ERROR(("Prefix length > 64, UNSUPPORTED"));
    return false;
  }
  ip6[0] = 0;
  make_iid((uint8_t *) l2addr, (uint8_t *) &ip6[1]);  // RFC 6282 3.2.2
  return true;
}

// Link-layer address of a neighbour from its IPv6 address: its interface ID
// is made from it, RFC 6282 3.2.2. make_iid() is its own inverse
void mg_l2_lowpan_ip6l2(const uint64_t *ip6, uint8_t *l2) {
  make_iid((const uint8_t *) &ip6[1], l2);
}

// Link-layer address option, RFC 4944 8: length 2 for EUI-64, 1 for short
bool mg_l2_lowpan_ip6get(struct mg_l2addr *l2addr, uint8_t *opts,
                         uint8_t len) {
  if (len == 2) {
    memcpy(l2addr, opts, 8);
  } else if (len == 1) {
    set_short((uint8_t *) l2addr, (uint16_t) ((opts[0] << 8) | opts[1]));
  } else {
    return false;
  }
  return true;
}

uint8_t mg_l2_lowpan_ip6put(struct mg_l2addr *l2addr, uint8_t *opts) {
  uint8_t *a = (uint8_t *) l2addr;
  if (is_short(a)) {
    opts[0] = a[6], opts[1] = a[7];
    return 1;
  }
  memcpy(opts, a, 8);
  return 2;
}

#elif MG_ENABLE_TCPIP
// No IPv6: 6LoWPAN is not available. Stubs for the L2 dispatch tables
extern struct mg_l2addr s_mapip;

void mg_l2_lowpan_init(struct mg_tcpip_if *ifp) {
  MG_ERROR(("6LoWPAN needs MG_ENABLE_IPV6"));
  (void) ifp;
}

bool mg_l2_lowpan_poll(struct mg_tcpip_if *ifp, bool expired_1000ms) {
  (void) ifp, (void) expired_1000ms;
  return false;  // Link is never up
}

uint8_t *mg_l2_lowpan_header(struct mg_tcpip_if *ifp, enum mg_l2proto proto,
                             struct mg_l2addr *src, struct mg_l2addr *dst,
                             uint8_t *frame) {
  (void) ifp, (void) proto, (void) src, (void) dst;
  return frame;
}

size_t mg_l2_lowpan_trailer(struct mg_tcpip_if *ifp, size_t len,
                            uint8_t *cur) {
  (void) ifp, (void) len, (void) cur;
  return 0;  // Nothing to send
}

bool mg_l2_lowpan_rx(struct mg_tcpip_if *ifp, enum mg_l2proto *proto,
                     struct mg_str *pay, struct mg_str *raw) {
  (void) ifp, (void) proto, (void) pay, (void) raw;
  return false;
}

struct mg_l2addr *mg_l2_lowpan_getaddr(struct mg_tcpip_if *ifp,
                                       uint8_t *frame) {
  (void) ifp, (void) frame;
  return &s_mapip;
}

struct mg_l2addr *mg_l2_lowpan_mapip(enum mg_l2addrtype addrtype,
                                     struct mg_addr *addr) {
  (void) addrtype, (void) addr;
  return &s_mapip;
}
#endif
