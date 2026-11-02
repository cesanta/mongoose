// Copyright (c) 2026 Cesanta Software Limited
// All rights reserved

#include "mongoose.h"

// Radio header followed by type and fragment index/last index.
#define MGI_FRAME_HDR_LEN (MG_802154_HEADER_LEN + 2)

#define MGI_ETH_MAX 1514
#define MGI_TX_DST_LEN 2

#define MGI_RX_TIMEOUT_US 1000000
#define MGI_TX_QUEUE_SIZE 4096
#define MGI_DATA_TAG \
  0x80u  // High bit identifies DATA, low seven bits tag a burst

struct mgi_mesh_a_rx {
  struct mgi_mesh_a_rx *next;
  uint64_t expires;
  uint16_t src;
  uint8_t tag, index, last;
  size_t len;
  uint8_t buf[MGI_ETH_MAX];
};

struct mgi_mesh_a_data {
  struct mg_mesh_node *node;
  struct mg_tcpip_if *ethernet;
  struct mgi_mesh_a_rx *rx;
  uint8_t tag;
  struct mg_queue tx;
  char tx_buf[MGI_TX_QUEUE_SIZE];
  size_t tx_offset;
  uint64_t tx_at;
};

static const struct mg_radio *const s_radio = &mg_radio_802154;

static uint64_t mgi_mesh_a_airtime(size_t len) {
  const struct mg_radio *radio = &mg_radio_802154;
  return ((uint64_t) (len + radio->overhead) * 8000000 + radio->bitrate - 1) /
         radio->bitrate;
}

// Each queue record holds a destination and a complete burst.
static size_t mgi_mesh_a_queue(struct mg_mesh_node *node, struct mg_queue *q,
                     const void *buf, size_t len, uint16_t dst, uint8_t type) {
  const struct mg_radio *radio = &mg_radio_802154;
  const uint8_t *eth = (const uint8_t *) buf;
  size_t hdr = MGI_FRAME_HDR_LEN, cap = radio->max_frame_size - hdr;
  size_t frames, size, i;
  char *record;
  uint8_t *out;
  if (len < 14 || len > MGI_ETH_MAX) return 0;
  frames = (len + cap - 1) / cap;
  size = MGI_TX_DST_LEN + len + frames * (1 + hdr);
  if (mg_queue_next(q, NULL) == 0) mg_queue_init(q, q->buf, q->size);
  if (mg_queue_book(q, &record, size) < size) return 0;
  out = (uint8_t *) record;
  *out++ = (uint8_t) (dst >> 8);
  *out++ = (uint8_t) dst;
  for (i = 0; i < frames; i++) {
    size_t chunk = len - i * cap;
    if (chunk > cap) chunk = cap;
    *out++ = (uint8_t) (hdr + chunk);
    radio->print_hdr(radio, out, dst, node->radio_address);
    out[radio->header_len] = type;
    out[radio->header_len + 1] = (uint8_t) (i << 4 | (frames - 1));
    memcpy(out + hdr, eth + i * cap, chunk);
    out += hdr + chunk;
  }
  mg_queue_add(q, size);
  return len;
}

static size_t mgi_mesh_a_send(struct mgi_mesh_a_data *data, const void *buf,
                              size_t len) {
  uint8_t tag = (uint8_t) ((data->tag + 1) & 0x7f);
  size_t queued = mgi_mesh_a_queue(data->node, &data->tx, buf, len, 0xffffu,
                                (uint8_t) (MGI_DATA_TAG | tag));
  if (queued != 0) data->tag = tag;
  return queued;
}

static void mgi_mesh_a_tx(struct mgi_mesh_a_data *data, uint64_t now) {
  struct mg_tcpip_if *radio = data->node->radio;
  char *record;
  size_t size = mg_queue_next(&data->tx, &record);
  if (size == 0) return;
  if (now >= data->tx_at) {
    const uint8_t *part = (const uint8_t *) record + data->tx_offset;
    size_t len = part[0];
    if (now >= data->node->tx_at &&
        data->node->tx(radio, part + 1, len) == len) {
      data->tx_offset += len + 1;
      if (data->tx_offset == size) {
        mg_queue_del(&data->tx, size);
        data->tx_offset = MGI_TX_DST_LEN;
      }
      data->tx_at = now + mgi_mesh_a_airtime(len) + s_radio->ifs_us;
    } else {
      data->tx_at = now + s_radio->slot_us;
    }
  }
  if (mg_queue_next(&data->tx, NULL) != 0) data->node->next_tick = data->tx_at;
}

static size_t mgi_mesh_a_enqueue(struct mg_mesh_node *node, const void *buf,
                                size_t len) {
  return mgi_mesh_a_send(node->data, buf, len);
}

static void mgi_mesh_a_deliver(struct mgi_mesh_a_data *data, uint8_t *buf,
                               size_t len) {
  struct mg_tcpip_if *lan = data->node->lan;
  if (len < 14) return;
  if (lan != NULL) {
    lan->driver->tx(buf, len, lan);
  } else {
    struct mg_tcpip_if *ifp = data->node->radio;
    if (data->node->rx_len != 0 || len > ifp->recv_queue.size) {
      ifp->ndrop++;
    } else {
      memcpy(ifp->recv_queue.buf, buf, len);
      data->node->rx_len = len;
    }
  }
}

static void mgi_mesh_a_rx_free(struct mgi_mesh_a_data *data,
                               struct mgi_mesh_a_rx **p) {
  struct mgi_mesh_a_rx *r = *p;
  *p = r->next;
  data->node->ram_used -= sizeof(*r);
  free(r);
}

static void mgi_mesh_a_receive(struct mgi_mesh_a_data *data, uint8_t *frame,
                               size_t len, uint64_t now) {
  struct mgi_mesh_a_rx **p = &data->rx, *r;
  size_t cap = s_radio->max_frame_size - MGI_FRAME_HDR_LEN, chunk;
  uint16_t dst, src;
  uint8_t tag, index, last;
  if (len <= MGI_FRAME_HDR_LEN || len > s_radio->max_frame_size ||
      s_radio->parse_hdr(s_radio, frame, len, &dst, &src) !=
          s_radio->header_len ||
      (dst != 0xffffu && dst != data->node->radio_address))
    return;
  tag = frame[MGI_FRAME_HDR_LEN - 2];
  index = frame[MGI_FRAME_HDR_LEN - 1] >> 4;
  last = frame[MGI_FRAME_HDR_LEN - 1] & 15;
  chunk = len - MGI_FRAME_HDR_LEN;
  if ((tag & MGI_DATA_TAG) == 0 || index > last ||
      last > (MGI_ETH_MAX - 1) / cap || (index < last && chunk != cap))
    return;
  if (index == 0 &&
      (chunk < 14 ||
       (data->node->lan == NULL && (frame[MGI_FRAME_HDR_LEN] & 1) == 0 &&
        memcmp(frame + MGI_FRAME_HDR_LEN, data->ethernet->mac, 6) != 0)))
    return;
  while (*p != NULL && (*p)->src != src) p = &(*p)->next;
  if (index == 0) {
    if (*p != NULL) mgi_mesh_a_rx_free(data, p);
    if (last == 0) {
      mgi_mesh_a_deliver(data, frame + MGI_FRAME_HDR_LEN, chunk);
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
    mgi_mesh_a_rx_free(data, p);
    return;
  }
  memcpy(r->buf + r->len, frame + MGI_FRAME_HDR_LEN, chunk);
  r->len += chunk;
  r->index++;
  r->expires = now + MGI_RX_TIMEOUT_US;
  if (index == last) {
    mgi_mesh_a_deliver(data, r->buf, r->len);
    mgi_mesh_a_rx_free(data, p);
  }
}

static void mgi_mesh_a_input(struct mg_mesh_node *node, void *buf, size_t len) {
  uint64_t ready = node->now + node->strategy->radio->turnaround_us;
  if (node->tx_at < ready) node->tx_at = ready;
  node->next_tick = node->now;
  mgi_mesh_a_receive(node->data, buf, len, node->now);
}

static void mgi_mesh_a_tick(struct mg_mesh_node *node, uint64_t now) {
  struct mgi_mesh_a_data *data = node->data;
  struct mgi_mesh_a_rx **p = &data->rx;
  size_t len;
  node->now = now;
  node->next_tick = (uint64_t) -1;
  while (*p != NULL) {
    if ((*p)->expires <= now)
      mgi_mesh_a_rx_free(data, p);
    else
      p = &(*p)->next;
  }
  if (node->lan != NULL) {
    struct mg_queue *q = &node->lan->recv_queue;
    char *buf;
    while ((len = mg_queue_next(q, &buf)) > 0) {
      mgi_mesh_a_send(data, buf, len);
      mg_queue_del(q, len);
    }
  }
  mgi_mesh_a_tx(data, now);
  for (p = &data->rx; *p != NULL; p = &(*p)->next) {
    if ((*p)->expires < node->next_tick) node->next_tick = (*p)->expires;
  }
}

static void mgi_mesh_a_free(struct mg_mesh_node *node) {
  struct mgi_mesh_a_data *data = node->data;
  if (data == NULL) return;
  while (data->rx != NULL) mgi_mesh_a_rx_free(data, &data->rx);
  free(data);
  node->data = NULL;
}

static bool mgi_mesh_a_init(struct mg_mesh_node *node,
                            struct mg_tcpip_if *radio, struct mg_tcpip_if *lan,
                            uint16_t radio_address) {
  struct mgi_mesh_a_data *data = calloc(1, sizeof(*data));
  memset(node, 0, sizeof(*node));
  node->radio = radio;
  node->lan = lan;
  node->radio_address = radio_address;
  node->next_tick = (uint64_t) -1;
  node->ram_used = node->ram_peak = sizeof(*node);
  if (data == NULL) return false;
  node->ram_used += sizeof(*data);
  node->ram_peak = node->ram_used;
  data->node = node;
  data->tx_offset = MGI_TX_DST_LEN;
  node->data = data;
  mg_queue_init(&data->tx, data->tx_buf, sizeof(data->tx_buf));
  data->ethernet = radio;
  node->joined = lan == NULL;
  return true;
}

const struct mg_mesh mg_mesh_a = {
    "A", mgi_mesh_a_init, mgi_mesh_a_tick, mgi_mesh_a_free,
    mgi_mesh_a_enqueue, mgi_mesh_a_input, false, false, &mg_radio_802154,
};
