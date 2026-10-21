#include "l2.h"
#include "net_builtin.h"

#if MG_ENABLE_TCPIP

// Radio frame: 802.15.4 MAC header, 15 bytes: data frame, PAN ID compression,
// broadcast destination, our EUI-64 made from MAC as source. Then 2 bytes:
// tag, fragment index << 4 | last fragment index. Then data. 125 bytes max,
// 127 minus FCS
#define MGI_802154_HDR 17
#define MGI_802154_DATA (125 - MGI_802154_HDR)

size_t mg_l2_802154_tx(struct mg_tcpip_if *ifp, const void *buf, size_t len,
                       size_t (*send)(const uint8_t *, size_t,
                                      struct mg_tcpip_if *)) {
  struct mg_l2_802154_data *d =
      (struct mg_l2_802154_data *) ifp->driver_data;
  const uint8_t *p = (const uint8_t *) buf, *m = ifp->mac;
  uint8_t f[MGI_802154_HDR + MGI_802154_DATA];
  size_t i, n, last = (len - 1) / MGI_802154_DATA;
  if (len == 0 || last > 15) return 0;
  d->tag++;
  for (i = 0; i <= last; i++) {
    n = i == last ? len - i * MGI_802154_DATA : MGI_802154_DATA;
    f[0] = 0x41, f[1] = 0xd8, f[2] = d->seq++;  // 2006 frame, see above
    f[3] = (uint8_t) d->pan_id, f[4] = (uint8_t) (d->pan_id >> 8);
    f[5] = f[6] = 0xff;
    f[7] = m[5], f[8] = m[4], f[9] = m[3], f[10] = 0xfe, f[11] = 0xff;
    f[12] = m[2], f[13] = m[1], f[14] = m[0];  // Little endian EUI-64
    f[15] = d->tag, f[16] = (uint8_t) ((i << 4) | last);
    memcpy(f + MGI_802154_HDR, p + i * MGI_802154_DATA, n);
    if (send(f, MGI_802154_HDR + n, ifp) == 0) return 0;
  }
  return len;
}

size_t mg_l2_802154_rx(struct mg_tcpip_if *ifp, const uint8_t *f, size_t n,
                       void *buf, size_t len) {
  struct mg_l2_802154_data *d =
      (struct mg_l2_802154_data *) ifp->driver_data;
  size_t idx, last;
  if (n <= MGI_802154_HDR || f[0] != 0x41 || f[1] != 0xd8 ||
      f[3] != (uint8_t) d->pan_id || f[4] != (uint8_t) (d->pan_id >> 8))
    return 0;  // Not ours
  idx = f[16] >> 4, last = f[16] & 15;
  if (idx == 0) d->rxtag = f[15], d->rxnext = 0, d->rxlen = 0;
  if (idx == 0) memcpy(d->rxsrc, f + 7, sizeof(d->rxsrc));
  n -= MGI_802154_HDR;
  if (idx != d->rxnext || idx > last || f[15] != d->rxtag ||
      memcmp(d->rxsrc, f + 7, sizeof(d->rxsrc)) != 0 ||
      d->rxlen + n > sizeof(d->rxbuf))
    return 0;  // Missed a fragment, or from another sender
  memcpy(d->rxbuf + d->rxlen, f + MGI_802154_HDR, n);
  d->rxlen = (uint16_t) (d->rxlen + n), d->rxnext++;
  if (idx < last || d->rxlen > len) return 0;
  memcpy(buf, d->rxbuf, d->rxlen);
  return d->rxlen;
}

#endif
