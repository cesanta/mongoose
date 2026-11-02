// Copyright (c) 2026 Cesanta Software Limited
// All rights reserved

#include "l2_mesh.h"

#define HDR_LEN (MG_802154_HEADER_LEN + MG_MESH_HDR_LEN)
#define ETH_FRAME_MAX 1514
#define RX_TIMEOUT_US 1000000
#define DATA_TAG 0x80u  // High bit identifies DATA, low seven bits tag a burst

struct star_rx {
  struct star_rx *next;
  uint64_t expires;
  uint16_t src;
  uint8_t tag, index, last;
  size_t len;
  uint8_t buf[ETH_FRAME_MAX];
};

struct star_data {
  struct mg_mesh_node *node;
  struct mg_tcpip_if ethernet;
  struct star_rx *rx;
  uint8_t tag;
};

static const struct mg_radio *const s_radio = &mg_radio_802154;

static size_t star_send(struct star_data *data, const void *buf, size_t len) {
  struct mg_tcpip_if *radio = data->node->radio;
  const uint8_t *payload = buf;
  uint8_t frame[HDR_LEN + ETH_FRAME_MAX];
  size_t cap = s_radio->max_frame_size - HDR_LEN, last, i;
  if (len < 14 || len > ETH_FRAME_MAX) return 0;
  last = (len - 1) / cap;
  data->tag = (uint8_t) ((data->tag + 1) & 0x7f);
  s_radio->print_hdr(s_radio, frame, UINT16_MAX, data->node->radio_address);
  frame[HDR_LEN - 2] = (uint8_t) (DATA_TAG | data->tag);
  for (i = 0; i <= last; i++) {
    size_t chunk = i == last ? len - i * cap : cap;
    frame[HDR_LEN - 1] = (uint8_t) (i << 4 | last);
    memcpy(frame + HDR_LEN, payload + i * cap, chunk);
    if (radio->driver->tx(frame, HDR_LEN + chunk, radio) != HDR_LEN + chunk)
      return 0;
  }
  return len;
}

static size_t ethernet_tx(const void *buf, size_t len, struct mg_tcpip_if *ifp) {
  return star_send(ifp->driver_data, buf, len);
}

static struct mg_tcpip_driver s_ethernet_driver = {NULL, ethernet_tx, NULL, NULL};

static void star_deliver(struct star_data *data, uint8_t *buf, size_t len) {
  struct mg_tcpip_if *lan = data->node->lan;
  if (len < 14) return;
  if (lan != NULL) {
    lan->driver->tx(buf, len, lan);
  } else {
    mg_tcpip_qwrite(buf, len, &data->ethernet);
  }
}

static void star_rx_free(struct star_data *data, struct star_rx **p) {
  struct star_rx *r = *p;
  *p = r->next;
  data->node->ram_used -= sizeof(*r);
  free(r);
}

static void star_receive(struct star_data *data, uint8_t *frame, size_t len,
                         uint64_t now) {
  struct star_rx **p = &data->rx, *r;
  size_t cap = s_radio->max_frame_size - HDR_LEN, chunk;
  uint16_t dst, src;
  uint8_t tag, index, last;
  if (len <= HDR_LEN || len > s_radio->max_frame_size ||
      s_radio->parse_hdr(s_radio, frame, len, &dst, &src) != s_radio->header_len ||
      (dst != UINT16_MAX && dst != data->node->radio_address)) return;
  tag = frame[HDR_LEN - 2];
  index = frame[HDR_LEN - 1] >> 4;
  last = frame[HDR_LEN - 1] & 15;
  chunk = len - HDR_LEN;
  if ((tag & DATA_TAG) == 0 || index > last || last > (ETH_FRAME_MAX - 1) / cap ||
      (index < last && chunk != cap)) return;
  if (index == 0 && (chunk < 14 ||
      (data->node->lan == NULL && (frame[HDR_LEN] & 1) == 0 &&
       memcmp(frame + HDR_LEN, data->ethernet.mac, 6) != 0))) return;
  while (*p != NULL && (*p)->src != src) p = &(*p)->next;
  if (index == 0) {
    if (*p != NULL) star_rx_free(data, p);
    if (last == 0) {
      star_deliver(data, frame + HDR_LEN, chunk);
      return;
    }
    r = calloc(1, sizeof(*r));
    if (r == NULL) return;
    data->node->ram_used += sizeof(*r);
    if (data->node->ram_peak < data->node->ram_used)
      data->node->ram_peak = data->node->ram_used;
    r->next = *p;
    *p = r;
    r->src = src;
    r->tag = tag;
    r->last = last;
  }
  r = *p;
  if (r == NULL || r->tag != tag) return;
  if (r->index != index || r->last != last || chunk > sizeof(r->buf) - r->len) {
    star_rx_free(data, p);
    return;
  }
  memcpy(r->buf + r->len, frame + HDR_LEN, chunk);
  r->len += chunk;
  r->index++;
  r->expires = now + RX_TIMEOUT_US;
  if (index == last) {
    star_deliver(data, r->buf, r->len);
    star_rx_free(data, p);
  }
}

static void star_tick(struct mg_mesh_node *node, uint64_t now) {
  struct star_data *data = node->data;
  struct mg_tcpip_if *radio = node->radio;
  struct star_rx **p = &data->rx;
  uint8_t frame[HDR_LEN + ETH_FRAME_MAX];
  size_t len;
  node->next_tick = UINT64_MAX;
  while (*p != NULL) {
    if ((*p)->expires <= now) star_rx_free(data, p);
    else p = &(*p)->next;
  }
  if (node->lan != NULL) {
    struct mg_queue *q = &node->lan->recv_queue;
    char *buf;
    while ((len = mg_queue_next(q, &buf)) > 0) {
      star_send(data, buf, len);
      mg_queue_del(q, len);
    }
  }
  while ((len = radio->driver->rx(frame, sizeof(frame), radio)) > 0) {
    star_receive(data, frame, len, now);
  }
  for (p = &data->rx; *p != NULL; p = &(*p)->next) {
    if ((*p)->expires < node->next_tick) node->next_tick = (*p)->expires;
  }
}

static void star_free(struct mg_mesh_node *node) {
  struct star_data *data = node->data;
  if (data == NULL) return;
  while (data->rx != NULL) star_rx_free(data, &data->rx);
  free(data);
  node->data = NULL;
}

static bool star_init(struct mg_mesh_node *node, struct mg_mgr *mgr,
                      struct mg_tcpip_if *radio, struct mg_tcpip_if *lan,
                      uint16_t radio_address) {
  struct star_data *data = calloc(1, sizeof(*data));
  mg_mesh_node_init(node, radio, lan, radio_address);
  if (data == NULL) return false;
  node->ram_used += sizeof(*data);
  node->ram_peak = node->ram_used;
  data->node = node;
  node->data = data;
  if (lan == NULL) {
    data->ethernet.driver = &s_ethernet_driver;
    data->ethernet.driver_data = data;
    memcpy(data->ethernet.mac, radio->mac, sizeof(data->ethernet.mac));
    mg_tcpip_init(mgr, &data->ethernet);
  }
  return true;
}

const struct mg_mesh mg_mesh_a = {
    "A",
    star_init,
    star_tick,
    star_free,
    false,
    false,
    &mg_radio_802154,
};
