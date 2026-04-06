#include "rtsp.h"

#define RTSP_TRACK_PREFIX "/trackID="
#define RTSP_VIDEO_TRACK 0U
#define RTSP_AUDIO_TRACK 1U
#define RTSP_TRACK_COUNT 2U
#define RTSP_VIDEO_RTP_PT 96U
#define RTSP_AUDIO_RTP_PT 97U
#define RTSP_VIDEO_RTP_CLOCK 90000U
#define RTSP_RTP_PAYLOAD_MAX 1400U
#define RTSP_MAX_NALS 16U

#define H264_MAX_AU (4U * 1024U * 1024U)
#define H264_MAX_RBSP (4U * 1024U * 1024U)

struct rtsp_request {
  struct mg_str method;
  struct mg_str uri;
  struct mg_str cseq;
  struct mg_str transport;
  struct mg_str session;
  size_t msg_len;
};

struct rtsp_nal {
  const uint8_t *ptr;
  size_t len;
};

struct rtsp_server {
  struct mg_rtsp_opts opts;
  char *path;
  bool has_audio;
  char sdp[1024];
};

struct rtsp_track_conn {
  bool setup_done;
  bool ended;
  uint16_t rtp_seq;
  uint32_t ssrc;
  unsigned rtp_channel;
  unsigned rtcp_channel;
  uint64_t deadline_ms;
};

struct rtsp_conn {
  struct rtsp_server *srv;
  bool playing;
  struct rtsp_track_conn tracks[RTSP_TRACK_COUNT];
  char session_id[24];
};

struct h264_bw {
  uint8_t *buf;
  size_t len;
  size_t cap;
  unsigned bitpos;
  bool oom;
};

struct h264_color {
  uint8_t y;
  uint8_t cb;
  uint8_t cr;
};

struct h264_testsrc_state {
  bool inited;
  unsigned width;
  unsigned height;
  unsigned fps;
  unsigned mb_width;
  unsigned mb_height;
  unsigned box_w;
  unsigned box_h;
  int box_x;
  int box_y;
  int dx;
  int dy;
  uint64_t frame_no;
  uint8_t sps[64];
  uint8_t pps[32];
  size_t sps_len;
  size_t pps_len;
  uint8_t rbsp[H264_MAX_RBSP];
  uint8_t au[H264_MAX_AU];
};

#define MP4_TAG(a, b, c, d)                                      \
  (((uint32_t) (uint8_t) (a) << 24) | ((uint32_t) (uint8_t) (b) << 16) | \
   ((uint32_t) (uint8_t) (c) << 8) | (uint32_t) (uint8_t) (d))

struct mp4_box {
  const uint8_t *buf;
  size_t len;
  uint32_t type;
};

struct rtsp_mp4_stsc {
  uint32_t first_chunk;
  uint32_t samples_per_chunk;
};

struct rtsp_mp4_sample {
  uint64_t offset;
  uint32_t size;
  uint64_t ts;
  uint64_t pts_us;
  bool keyframe;
};

struct rtsp_mp4_state {
  struct rtsp_mp4_state *next;
  char *path;
  struct mg_str file;
  struct rtsp_mp4_sample *samples;
  size_t sample_count;
  size_t start_index;
  size_t sample_index;
  uint32_t width;
  uint32_t height;
  unsigned fps;
  uint32_t timescale;
  uint8_t nal_len_size;
  uint8_t *param_sets;
  size_t param_sets_len;
  uint8_t *sps;
  size_t sps_len;
  uint8_t *pps;
  size_t pps_len;
  uint8_t *au;
  size_t au_cap;
  struct rtsp_mp4_sample *audio_samples;
  size_t audio_sample_count;
  size_t audio_sample_index;
  uint32_t audio_timescale;
  unsigned audio_rate;
  unsigned audio_channels;
  uint8_t *audio_config;
  size_t audio_config_len;
};

static struct rtsp_mp4_state *s_rtsp_mp4_states;

static int rtsp_tolower(int ch) {
  return ch >= 'A' && ch <= 'Z' ? ch + ('a' - 'A') : ch;
}

static bool rtsp_memeq(const void *ptr1, const void *ptr2, size_t len) {
  return len == 0 || memcmp(ptr1, ptr2, len) == 0;
}

static bool rtsp_caseeq(struct mg_str s, const char *lit) {
  size_t i, len = strlen(lit);
  if (s.len != len) return false;
  for (i = 0; i < len; i++) {
    if (rtsp_tolower((unsigned char) s.buf[i]) != rtsp_tolower((unsigned char) lit[i])) {
      return false;
    }
  }
  return true;
}

static bool rtsp_casencontains(struct mg_str s, const char *lit) {
  size_t i, j, slen = s.len, llen = strlen(lit);
  if (llen == 0 || llen > slen) return false;
  for (i = 0; i + llen <= slen; i++) {
    for (j = 0; j < llen; j++) {
      if (rtsp_tolower((unsigned char) s.buf[i + j]) !=
          rtsp_tolower((unsigned char) lit[j])) {
        break;
      }
    }
    if (j == llen) return true;
  }
  return false;
}

static struct mg_str rtsp_trim(struct mg_str s) {
  while (s.len > 0 &&
         (s.buf[0] == ' ' || s.buf[0] == '\t' || s.buf[0] == '\r' ||
          s.buf[0] == '\n')) {
    s.buf++;
    s.len--;
  }
  while (s.len > 0 &&
         (s.buf[s.len - 1] == ' ' || s.buf[s.len - 1] == '\t' ||
          s.buf[s.len - 1] == '\r' || s.buf[s.len - 1] == '\n')) {
    s.len--;
  }
  return s;
}

static bool rtsp_parse_u32(struct mg_str s, uint32_t *value) {
  size_t i;
  uint32_t n = 0;
  s = rtsp_trim(s);
  if (s.len == 0) return false;
  for (i = 0; i < s.len; i++) {
    unsigned ch = (unsigned char) s.buf[i];
    if (ch < '0' || ch > '9') return false;
    n = n * 10U + (uint32_t) (ch - '0');
  }
  *value = n;
  return true;
}

static char *rtsp_strdup(const char *s) {
  size_t len = strlen(s);
  char *copy = (char *) mg_calloc(len + 1U, 1U);
  if (copy != NULL) memcpy(copy, s, len);
  return copy;
}

static struct mg_str rtsp_uri_path(struct mg_str uri) {
  const char *p = uri.buf, *end = uri.buf + uri.len, *scheme = NULL;
  size_t i;

  for (i = 0; i + 2 < uri.len; i++) {
    if (uri.buf[i] == ':' && uri.buf[i + 1] == '/' && uri.buf[i + 2] == '/') {
      scheme = uri.buf + i;
      break;
    }
  }
  if (scheme != NULL) {
    p = scheme + 3;
    while (p < end && *p != '/') p++;
    if (p >= end) return mg_str("/");
  }
  if (p < end && *p != '/') {
    while (p < end && *p != '/') p++;
    if (p >= end) return mg_str("/");
  }
  if (p >= end) return mg_str("/");
  end = p;
  while (end < uri.buf + uri.len && *end != '?') end++;
  return mg_str_n(p, (size_t) (end - p));
}

static bool rtsp_path_eq(struct mg_str uri, const char *path) {
  struct mg_str upath = rtsp_uri_path(uri);
  size_t path_len = strlen(path);
  return upath.len == path_len && rtsp_memeq(upath.buf, path, path_len);
}

static int rtsp_track_path_eq(struct mg_str uri, const char *path) {
  struct mg_str upath = rtsp_uri_path(uri);
  struct mg_str id;
  uint32_t track_id;
  size_t path_len = strlen(path), prefix_len = strlen(RTSP_TRACK_PREFIX);

  if (upath.len <= path_len + prefix_len) return -1;
  if (!rtsp_memeq(upath.buf, path, path_len) ||
      !rtsp_memeq(upath.buf + path_len, RTSP_TRACK_PREFIX, prefix_len)) {
    return -1;
  }
  id = mg_str_n(upath.buf + path_len + prefix_len,
                upath.len - path_len - prefix_len);
  if (!rtsp_parse_u32(id, &track_id)) return -1;
  return (int) track_id;
}

static bool rtsp_next_line(const char **cursor, const char *end,
                           struct mg_str *line) {
  const char *p = *cursor;
  if (p >= end) return false;
  while (p < end && *p != '\n') p++;
  if (p >= end) return false;
  line->buf = (char *) *cursor;
  line->len = (size_t) (p - *cursor);
  if (line->len > 0 && line->buf[line->len - 1] == '\r') line->len--;
  *cursor = p + 1;
  return true;
}

static int rtsp_parse_request(const char *buf, size_t len,
                              struct rtsp_request *req) {
  const char *cursor = buf, *end;
  struct mg_str line;
  int header_len = mg_http_get_request_len((const unsigned char *) buf, len);
  uint32_t content_length = 0;

  memset(req, 0, sizeof(*req));
  if (header_len <= 0) return header_len;
  if ((size_t) header_len > len) return 0;
  end = buf + header_len;
  if (!rtsp_next_line(&cursor, end, &line)) return -1;

  {
    const char *p = line.buf, *line_end = line.buf + line.len;
    while (p < line_end && *p == ' ') p++;
    req->method.buf = (char *) p;
    while (p < line_end && *p != ' ') p++;
    req->method.len = (size_t) (p - req->method.buf);
    while (p < line_end && *p == ' ') p++;
    req->uri.buf = (char *) p;
    while (p < line_end && *p != ' ') p++;
    req->uri.len = (size_t) (p - req->uri.buf);
    while (p < line_end && *p == ' ') p++;
    if (!rtsp_caseeq(mg_str_n(p, (size_t) (line_end - p)), "RTSP/1.0")) return -1;
    if (req->method.len == 0 || req->uri.len == 0) return -1;
  }

  while (rtsp_next_line(&cursor, end, &line)) {
    struct mg_str name, value;
    char *colon;
    if (line.len == 0) break;
    colon = (char *) memchr(line.buf, ':', line.len);
    if (colon == NULL) return -1;
    name = mg_str_n(line.buf, (size_t) (colon - line.buf));
    value = rtsp_trim(mg_str_n(colon + 1, line.len - (size_t) (colon - line.buf) - 1));
    if (rtsp_caseeq(name, "CSeq")) {
      req->cseq = value;
    } else if (rtsp_caseeq(name, "Transport")) {
      req->transport = value;
    } else if (rtsp_caseeq(name, "Session")) {
      req->session = value;
    } else if (rtsp_caseeq(name, "Content-Length")) {
      if (!rtsp_parse_u32(value, &content_length)) return -1;
    }
  }

  req->msg_len = (size_t) header_len + (size_t) content_length;
  if (req->msg_len > len) return 0;
  return req->cseq.len > 0 ? 1 : -1;
}

static void rtsp_send_response(struct mg_connection *c,
                               const struct rtsp_request *req, int status,
                               const char *reason, const char *headers,
                               const void *body, size_t body_len) {
  const char *extra = headers == NULL ? "" : headers;
  mg_printf(c,
            "RTSP/1.0 %d %s\r\n"
            "CSeq: %.*s\r\n"
            "Server: Mongoose RTSP\r\n"
            "%s"
            "Content-Length: %lu\r\n"
            "\r\n",
            status, reason, (int) req->cseq.len, req->cseq.buf, extra,
            (unsigned long) body_len);
  if (body_len > 0) mg_send(c, body, body_len);
}

static void rtsp_send_status(struct mg_connection *c,
                             const struct rtsp_request *req, int status,
                             const char *reason, const char *headers) {
  rtsp_send_response(c, req, status, reason, headers, NULL, 0);
}

static size_t h264_start_code_len(const uint8_t *buf, size_t len, size_t off) {
  if (off + 3 <= len && buf[off] == 0 && buf[off + 1] == 0 &&
      buf[off + 2] == 1) {
    return 3;
  }
  if (off + 4 <= len && buf[off] == 0 && buf[off + 1] == 0 &&
      buf[off + 2] == 0 && buf[off + 3] == 1) {
    return 4;
  }
  return 0;
}

static size_t h264_split_annexb(const uint8_t *buf, size_t len,
                                struct rtsp_nal *nals, size_t max_nals) {
  size_t i = 0, count = 0;

  while (i < len) {
    size_t sc = h264_start_code_len(buf, len, i);
    if (sc == 0) {
      i++;
      continue;
    }
    i += sc;
    if (i >= len) break;
    {
      size_t start = i;
      while (i < len && h264_start_code_len(buf, len, i) == 0) i++;
      if (start < i && count < max_nals) {
        nals[count].ptr = buf + start;
        nals[count].len = i - start;
        count++;
      }
    }
  }

  if (count == 0 && len > 0 && max_nals > 0) {
    nals[0].ptr = buf;
    nals[0].len = len;
    count = 1;
  }
  return count;
}

static void rtsp_parse_interleaved_channels(struct mg_str transport,
                                            unsigned *rtp_channel,
                                            unsigned *rtcp_channel) {
  size_t i;
  *rtp_channel = 0;
  *rtcp_channel = 1;
  for (i = 0; i + 11 < transport.len; i++) {
    if (rtsp_tolower((unsigned char) transport.buf[i]) == 'i' &&
        rtsp_tolower((unsigned char) transport.buf[i + 1]) == 'n' &&
        rtsp_tolower((unsigned char) transport.buf[i + 2]) == 't' &&
        rtsp_tolower((unsigned char) transport.buf[i + 3]) == 'e' &&
        rtsp_tolower((unsigned char) transport.buf[i + 4]) == 'r' &&
        rtsp_tolower((unsigned char) transport.buf[i + 5]) == 'l' &&
        rtsp_tolower((unsigned char) transport.buf[i + 6]) == 'e' &&
        rtsp_tolower((unsigned char) transport.buf[i + 7]) == 'a' &&
        rtsp_tolower((unsigned char) transport.buf[i + 8]) == 'v' &&
        rtsp_tolower((unsigned char) transport.buf[i + 9]) == 'e' &&
        rtsp_tolower((unsigned char) transport.buf[i + 10]) == 'd' &&
        transport.buf[i + 11] == '=') {
      uint32_t a = 0, b = 1;
      size_t j = i + 12;
      struct mg_str s1, s2;
      s1.buf = &transport.buf[j];
      s1.len = 0;
      while (j < transport.len && transport.buf[j] >= '0' && transport.buf[j] <= '9') {
        s1.len++;
        j++;
      }
      if (j >= transport.len || transport.buf[j] != '-') return;
      j++;
      s2.buf = &transport.buf[j];
      s2.len = 0;
      while (j < transport.len && transport.buf[j] >= '0' && transport.buf[j] <= '9') {
        s2.len++;
        j++;
      }
      if (rtsp_parse_u32(s1, &a) && rtsp_parse_u32(s2, &b)) {
        *rtp_channel = (unsigned) a;
        *rtcp_channel = (unsigned) b;
      }
      return;
    }
  }
}

static bool rtsp_session_matches(const struct rtsp_conn *rc, struct mg_str session) {
  size_t i;
  session = rtsp_trim(session);
  for (i = 0; i < session.len; i++) {
    if (session.buf[i] == ';') {
      session.len = i;
      break;
    }
  }
  return session.len == strlen(rc->session_id) &&
         rtsp_memeq(session.buf, rc->session_id, session.len);
}

static uint16_t rtsp_mp4_u16(const uint8_t *p) {
  return (uint16_t) (((uint16_t) p[0] << 8) | p[1]);
}

static uint32_t rtsp_mp4_u32(const uint8_t *p) {
  return ((uint32_t) p[0] << 24) | ((uint32_t) p[1] << 16) |
         ((uint32_t) p[2] << 8) | p[3];
}

static uint64_t rtsp_mp4_u64(const uint8_t *p) {
  return ((uint64_t) rtsp_mp4_u32(p) << 32) | rtsp_mp4_u32(p + 4);
}

static uint64_t rtsp_scale_us(uint64_t value, uint32_t timescale) {
  if (timescale == 0) return 0;
  return (value / timescale) * 1000000ULL +
         (((value % timescale) * 1000000ULL) + timescale / 2U) / timescale;
}

static bool rtsp_mp4_next_box(const uint8_t *buf, size_t len, size_t *ofs,
                              struct mp4_box *box) {
  uint64_t start = *ofs, size, head = 8;
  if (start + 8U > len) return false;
  size = rtsp_mp4_u32(buf + start);
  box->type = rtsp_mp4_u32(buf + start + 4U);
  if (size == 1U) {
    if (start + 16U > len) return false;
    size = rtsp_mp4_u64(buf + start + 8U);
    head = 16;
  } else if (size == 0) {
    size = len - start;
  }
  if (size < head || start + size > len) return false;
  box->buf = buf + start + head;
  box->len = (size_t) (size - head);
  *ofs = (size_t) (start + size);
  return true;
}

static bool rtsp_mp4_find_child(const uint8_t *buf, size_t len, uint32_t type,
                                struct mp4_box *box) {
  size_t ofs = 0;
  struct mp4_box tmp;
  while (rtsp_mp4_next_box(buf, len, &ofs, &tmp)) {
    if (tmp.type == type) {
      *box = tmp;
      return true;
    }
  }
  return false;
}

static uint32_t rtsp_mp4_track_type(const struct mp4_box *mdia) {
  struct mp4_box hdlr;
  return rtsp_mp4_find_child(mdia->buf, mdia->len, MP4_TAG('h', 'd', 'l', 'r'),
                             &hdlr) &&
                 hdlr.len >= 12U
             ? rtsp_mp4_u32(hdlr.buf + 8U)
             : 0;
}

static void rtsp_mp4_free_state(struct rtsp_mp4_state *st) {
  if (st == NULL) return;
  mg_free(st->path);
  mg_free(st->file.buf);
  mg_free(st->samples);
  mg_free(st->audio_samples);
  mg_free(st->param_sets);
  mg_free(st->sps);
  mg_free(st->pps);
  mg_free(st->au);
  mg_free(st->audio_config);
  mg_free(st);
}

static bool rtsp_mp4_copy_blob(uint8_t **dst, size_t *dst_len, const uint8_t *src,
                               size_t len) {
  uint8_t *copy = (uint8_t *) mg_calloc(len == 0 ? 1U : len, 1U);
  if (copy == NULL) return false;
  if (len > 0) memcpy(copy, src, len);
  mg_free(*dst);
  *dst = copy;
  *dst_len = len;
  return true;
}

static bool rtsp_mp4_parse_avcc(struct rtsp_mp4_state *st, const uint8_t *buf,
                                size_t len) {
  size_t i, pos, out_len = 0, out_cap;
  uint8_t num_pps;
  uint8_t *out;
  if (len < 7U) return false;

  st->nal_len_size = (uint8_t) ((buf[4] & 3U) + 1U);
  out_cap = len * 2U + 32U;
  out = (uint8_t *) mg_calloc(out_cap, 1U);
  if (out == NULL) return false;

  pos = 6U;
  for (i = 0; i < (size_t) (buf[5] & 0x1fU); i++) {
    uint16_t n;
    if (pos + 2U > len) goto fail;
    n = rtsp_mp4_u16(buf + pos);
    pos += 2U;
    if (n == 0 || pos + n > len || out_len + 4U + n > out_cap) goto fail;
    if (st->sps == NULL && !rtsp_mp4_copy_blob(&st->sps, &st->sps_len, buf + pos, n))
      goto fail;
    memcpy(out + out_len, "\0\0\0\1", 4U);
    out_len += 4U;
    memcpy(out + out_len, buf + pos, n);
    out_len += n;
    pos += n;
  }
  if (pos + 1U > len) goto fail;
  num_pps = buf[pos++];
  for (i = 0; i < num_pps; i++) {
    uint16_t n;
    if (pos + 2U > len) goto fail;
    n = rtsp_mp4_u16(buf + pos);
    pos += 2U;
    if (n == 0 || pos + n > len || out_len + 4U + n > out_cap) goto fail;
    if (st->pps == NULL && !rtsp_mp4_copy_blob(&st->pps, &st->pps_len, buf + pos, n))
      goto fail;
    memcpy(out + out_len, "\0\0\0\1", 4U);
    out_len += 4U;
    memcpy(out + out_len, buf + pos, n);
    out_len += n;
    pos += n;
  }
  if (st->sps == NULL || st->pps == NULL || out_len == 0) goto fail;
  st->param_sets = out;
  st->param_sets_len = out_len;
  return true;

fail:
  mg_free(out);
  return false;
}

static bool rtsp_mp4_next_desc(const uint8_t *buf, size_t len, size_t *ofs,
                               uint8_t *tag, const uint8_t **body,
                               size_t *body_len) {
  size_t pos = *ofs, i;
  uint32_t size = 0;

  if (pos >= len) return false;
  *tag = buf[pos++];
  for (i = 0; i < 4; i++) {
    uint8_t b;
    if (pos >= len) return false;
    b = buf[pos++];
    size = (size << 7) | (uint32_t) (b & 0x7fU);
    if ((b & 0x80U) == 0) break;
  }
  if (pos + size > len) return false;
  *body = buf + pos;
  *body_len = size;
  *ofs = pos + size;
  return true;
}

static bool rtsp_mp4_parse_esds(struct rtsp_mp4_state *st, const uint8_t *buf,
                                size_t len) {
  const uint8_t *body, *config;
  size_t body_len, config_len, ofs;
  uint8_t tag;

  if (len < 4U) return false;
  ofs = 0;
  if (!rtsp_mp4_next_desc(buf + 4U, len - 4U, &ofs, &tag, &body, &body_len) ||
      tag != 0x03U || body_len < 3U) {
    return false;
  }
  ofs = 3U;
  if ((body[2] & 0x80U) != 0) ofs += 2U;
  if ((body[2] & 0x40U) != 0) {
    if (ofs >= body_len) return false;
    ofs += 1U + body[ofs];
  }
  if ((body[2] & 0x20U) != 0) ofs += 2U;
  if (ofs > body_len ||
      !rtsp_mp4_next_desc(body, body_len, &ofs, &tag, &body, &body_len) ||
      tag != 0x04U || body_len < 13U) {
    return false;
  }
  ofs = 13U;
  if (!rtsp_mp4_next_desc(body, body_len, &ofs, &tag, &config, &config_len) ||
      tag != 0x05U || config_len == 0) {
    return false;
  }
  while (config_len > 0 && config[config_len - 1] == 0) config_len--;
  return config_len > 0 &&
         rtsp_mp4_copy_blob(&st->audio_config, &st->audio_config_len, config,
                            config_len);
}

static bool rtsp_mp4_parse_stsd(struct rtsp_mp4_state *st,
                                const struct mp4_box *stsd) {
  size_t ofs = 8U;
  struct mp4_box entry, avcc;
  if (stsd->len < 8U || rtsp_mp4_u32(stsd->buf + 4U) == 0) return false;
  while (rtsp_mp4_next_box(stsd->buf, stsd->len, &ofs, &entry)) {
    if (entry.type != MP4_TAG('a', 'v', 'c', '1') &&
        entry.type != MP4_TAG('a', 'v', 'c', '3')) {
      continue;
    }
    if (entry.len < 78U) return false;
    st->width = rtsp_mp4_u16(entry.buf + 24U);
    st->height = rtsp_mp4_u16(entry.buf + 26U);
    if (!rtsp_mp4_find_child(entry.buf + 78U, entry.len - 78U,
                             MP4_TAG('a', 'v', 'c', 'C'), &avcc)) {
      return false;
    }
    return rtsp_mp4_parse_avcc(st, avcc.buf, avcc.len);
  }
  return false;
}

static bool rtsp_mp4_parse_audio_stsd(struct rtsp_mp4_state *st,
                                      const struct mp4_box *stsd) {
  size_t ofs = 8U;
  struct mp4_box entry, esds;

  if (stsd->len < 8U || rtsp_mp4_u32(stsd->buf + 4U) == 0) return false;
  while (rtsp_mp4_next_box(stsd->buf, stsd->len, &ofs, &entry)) {
    uint16_t version;
    size_t child_ofs = 28U;

    if (entry.type != MP4_TAG('m', 'p', '4', 'a')) continue;
    if (entry.len < 28U) return false;
    version = rtsp_mp4_u16(entry.buf + 8U);
    if (version == 1U) {
      child_ofs = 44U;
    } else if (version == 2U) {
      child_ofs = 64U;
    }
    if (entry.len < child_ofs) return false;
    st->audio_channels = rtsp_mp4_u16(entry.buf + 16U);
    st->audio_rate = rtsp_mp4_u32(entry.buf + 24U) >> 16;
    if (st->audio_channels == 0 || st->audio_rate == 0) return false;
    if (!rtsp_mp4_find_child(entry.buf + child_ofs, entry.len - child_ofs,
                             MP4_TAG('e', 's', 'd', 's'), &esds)) {
      return false;
    }
    return rtsp_mp4_parse_esds(st, esds.buf, esds.len);
  }
  return false;
}

static bool rtsp_mp4_parse_mdhd(struct rtsp_mp4_state *st,
                                const struct mp4_box *mdhd) {
  if (mdhd->len < 16U) return false;
  if (mdhd->buf[0] == 1) {
    if (mdhd->len < 24U) return false;
    st->timescale = rtsp_mp4_u32(mdhd->buf + 20U);
  } else {
    st->timescale = rtsp_mp4_u32(mdhd->buf + 12U);
  }
  return st->timescale > 0;
}

static bool rtsp_mp4_parse_audio_mdhd(struct rtsp_mp4_state *st,
                                      const struct mp4_box *mdhd) {
  if (mdhd->len < 16U) return false;
  if (mdhd->buf[0] == 1) {
    if (mdhd->len < 24U) return false;
    st->audio_timescale = rtsp_mp4_u32(mdhd->buf + 20U);
  } else {
    st->audio_timescale = rtsp_mp4_u32(mdhd->buf + 12U);
  }
  return st->audio_timescale > 0;
}

static uint32_t *rtsp_mp4_parse_stsz(const struct mp4_box *stsz,
                                     size_t *sample_count) {
  uint32_t fixed_size, count;
  uint32_t *sizes;
  size_t i;
  if (stsz->len < 12U) return NULL;
  fixed_size = rtsp_mp4_u32(stsz->buf + 4U);
  count = rtsp_mp4_u32(stsz->buf + 8U);
  if (count == 0) return NULL;
  if (fixed_size == 0 && stsz->len < 12U + (size_t) count * 4U) return NULL;

  sizes = (uint32_t *) mg_calloc(count, sizeof(*sizes));
  if (sizes == NULL) return NULL;
  if (fixed_size != 0) {
    for (i = 0; i < count; i++) sizes[i] = fixed_size;
  } else {
    for (i = 0; i < count; i++) sizes[i] = rtsp_mp4_u32(stsz->buf + 12U + i * 4U);
  }
  *sample_count = count;
  return sizes;
}

static uint64_t *rtsp_mp4_parse_chunk_offsets(const struct mp4_box *box,
                                              size_t *count) {
  uint32_t n;
  size_t i;
  uint64_t *offsets;
  bool is64 = box->type == MP4_TAG('c', 'o', '6', '4');
  if (box->len < 8U) return NULL;
  n = rtsp_mp4_u32(box->buf + 4U);
  if (n == 0) return NULL;
  if (is64) {
    if (box->len < 8U + (size_t) n * 8U) return NULL;
  } else {
    if (box->len < 8U + (size_t) n * 4U) return NULL;
  }
  offsets = (uint64_t *) mg_calloc(n, sizeof(*offsets));
  if (offsets == NULL) return NULL;
  for (i = 0; i < n; i++) {
    offsets[i] =
        is64 ? rtsp_mp4_u64(box->buf + 8U + i * 8U)
             : (uint64_t) rtsp_mp4_u32(box->buf + 8U + i * 4U);
  }
  *count = n;
  return offsets;
}

static struct rtsp_mp4_stsc *rtsp_mp4_parse_stsc(const struct mp4_box *stsc,
                                                 size_t *count) {
  uint32_t n;
  size_t i;
  struct rtsp_mp4_stsc *entries;
  if (stsc->len < 8U) return NULL;
  n = rtsp_mp4_u32(stsc->buf + 4U);
  if (n == 0 || stsc->len < 8U + (size_t) n * 12U) return NULL;
  entries = (struct rtsp_mp4_stsc *) mg_calloc(n, sizeof(*entries));
  if (entries == NULL) return NULL;
  for (i = 0; i < n; i++) {
    const uint8_t *p = stsc->buf + 8U + i * 12U;
    entries[i].first_chunk = rtsp_mp4_u32(p);
    entries[i].samples_per_chunk = rtsp_mp4_u32(p + 4U);
  }
  *count = n;
  return entries;
}

static int64_t *rtsp_mp4_parse_pts(const struct mp4_box *stts,
                                   const struct mp4_box *ctts,
                                   size_t sample_count,
                                   uint64_t *duration_units) {
  int64_t *pts;
  uint64_t dts = 0;
  uint32_t entry_count;
  size_t i, j, idx = 0, pos;
  (void) ctts;
  if (stts->len < 8U) return NULL;
  entry_count = rtsp_mp4_u32(stts->buf + 4U);
  pts = (int64_t *) mg_calloc(sample_count, sizeof(*pts));
  if (pts == NULL) return NULL;

  pos = 8U;
  for (i = 0; i < entry_count; i++, pos += 8U) {
    uint32_t count, delta;
    if (pos + 8U > stts->len) goto fail;
    count = rtsp_mp4_u32(stts->buf + pos);
    delta = rtsp_mp4_u32(stts->buf + pos + 4U);
    for (j = 0; j < count; j++) {
      if (idx >= sample_count) goto fail;
      pts[idx++] = (int64_t) dts;
      dts += delta;
    }
  }
  if (idx != sample_count) goto fail;
  *duration_units = dts;
  return pts;

fail:
  mg_free(pts);
  return NULL;
}

static bool rtsp_mp4_parse_samples(struct rtsp_mp4_state *st,
                                   const struct mp4_box *stbl) {
  struct mp4_box stsz, stsc, stts, ctts, stss, chunk_box;
  uint32_t *sizes = NULL;
  uint64_t *chunk_offsets = NULL, duration_units = 0;
  struct rtsp_mp4_stsc *stsc_entries = NULL;
  int64_t *pts = NULL, min_pts;
  size_t sample_count = 0, chunk_count = 0, stsc_count = 0;
  size_t i, chunk = 0, sample = 0, start_index = 0, active_count;
  uint64_t duration_us;
  bool has_ctts, has_stss;

  if (!rtsp_mp4_find_child(stbl->buf, stbl->len, MP4_TAG('s', 't', 's', 'z'),
                           &stsz) ||
      !rtsp_mp4_find_child(stbl->buf, stbl->len, MP4_TAG('s', 't', 's', 'c'),
                           &stsc) ||
      !rtsp_mp4_find_child(stbl->buf, stbl->len, MP4_TAG('s', 't', 't', 's'),
                           &stts)) {
    return false;
  }
  has_ctts =
      rtsp_mp4_find_child(stbl->buf, stbl->len, MP4_TAG('c', 't', 't', 's'),
                          &ctts);
  has_stss =
      rtsp_mp4_find_child(stbl->buf, stbl->len, MP4_TAG('s', 't', 's', 's'),
                          &stss);
  if (!rtsp_mp4_find_child(stbl->buf, stbl->len, MP4_TAG('s', 't', 'c', 'o'),
                           &chunk_box) &&
      !rtsp_mp4_find_child(stbl->buf, stbl->len, MP4_TAG('c', 'o', '6', '4'),
                           &chunk_box)) {
    return false;
  }

  sizes = rtsp_mp4_parse_stsz(&stsz, &sample_count);
  chunk_offsets = rtsp_mp4_parse_chunk_offsets(&chunk_box, &chunk_count);
  stsc_entries = rtsp_mp4_parse_stsc(&stsc, &stsc_count);
  pts = rtsp_mp4_parse_pts(&stts, has_ctts ? &ctts : NULL, sample_count,
                           &duration_units);
  st->samples = (struct rtsp_mp4_sample *) mg_calloc(sample_count, sizeof(*st->samples));
  if (sizes == NULL || chunk_offsets == NULL || stsc_entries == NULL || pts == NULL ||
      st->samples == NULL) {
    goto fail;
  }

  for (i = 0; i < stsc_count; i++) {
    uint32_t first_chunk = stsc_entries[i].first_chunk;
    uint32_t last_chunk =
        i + 1U < stsc_count ? stsc_entries[i + 1U].first_chunk - 1U
                            : (uint32_t) chunk_count;
    uint32_t chunk_no;
    if (first_chunk == 0 || first_chunk > last_chunk) goto fail;
    for (chunk_no = first_chunk; chunk_no <= last_chunk; chunk_no++) {
      uint64_t offset;
      uint32_t sample_no;
      if ((size_t) chunk_no > chunk_count) goto fail;
      offset = chunk_offsets[chunk_no - 1U];
      for (sample_no = 0; sample_no < stsc_entries[i].samples_per_chunk; sample_no++) {
        if (sample >= sample_count) goto fail;
        st->samples[sample].offset = offset;
        st->samples[sample].size = sizes[sample];
        if (offset > st->file.len || sizes[sample] > st->file.len - (size_t) offset)
          goto fail;
        offset += sizes[sample];
        sample++;
      }
      chunk++;
    }
  }
  if (sample != sample_count || chunk != chunk_count) goto fail;

  for (i = 0; i < sample_count; i++) st->samples[i].keyframe = !has_stss;
  if (has_stss) {
    uint32_t sync_count;
    size_t pos = 8U;
    if (stss.len < 8U) goto fail;
    sync_count = rtsp_mp4_u32(stss.buf + 4U);
    for (i = 0; i < sync_count; i++, pos += 4U) {
      uint32_t sample_no;
      if (pos + 4U > stss.len) goto fail;
      sample_no = rtsp_mp4_u32(stss.buf + pos);
      if (sample_no > 0 && sample_no <= sample_count)
        st->samples[sample_no - 1U].keyframe = true;
    }
  }

  min_pts = pts[0];
  for (i = 0; i < sample_count; i++) {
    if (pts[i] < min_pts) min_pts = pts[i];
  }
  for (i = 0; i < sample_count; i++) {
    st->samples[i].ts = (uint64_t) (pts[i] - min_pts);
    st->samples[i].pts_us = rtsp_scale_us(st->samples[i].ts, st->timescale);
  }

  while (start_index < sample_count && !st->samples[start_index].keyframe) start_index++;
  if (start_index >= sample_count) start_index = 0;

  st->sample_count = sample_count;
  st->start_index = start_index;
  st->sample_index = start_index;
  duration_us =
      rtsp_scale_us(duration_units, st->timescale) > st->samples[start_index].pts_us
          ? rtsp_scale_us(duration_units, st->timescale) -
                st->samples[start_index].pts_us
          : rtsp_scale_us(duration_units, st->timescale);
  active_count = sample_count - start_index;
  if (active_count == 0) active_count = sample_count;
  if (duration_us == 0) duration_us = 1000000ULL / 30ULL;
  st->fps = (unsigned) ((active_count * 1000000ULL + duration_us / 2U) / duration_us);
  if (st->fps == 0) st->fps = 30U;

  mg_free(sizes);
  mg_free(chunk_offsets);
  mg_free(stsc_entries);
  mg_free(pts);
  return true;

fail:
  mg_free(sizes);
  mg_free(chunk_offsets);
  mg_free(stsc_entries);
  mg_free(pts);
  mg_free(st->samples);
  st->samples = NULL;
  return false;
}

static bool rtsp_mp4_parse_audio_samples(struct rtsp_mp4_state *st,
                                         const struct mp4_box *stbl) {
  struct mp4_box stsz, stsc, stts, ctts, chunk_box;
  uint32_t *sizes = NULL;
  uint64_t *chunk_offsets = NULL, duration_units = 0;
  struct rtsp_mp4_stsc *stsc_entries = NULL;
  int64_t *pts = NULL, min_pts;
  size_t sample_count = 0, chunk_count = 0, stsc_count = 0;
  size_t i, chunk = 0, sample = 0;
  bool has_ctts;

  if (!rtsp_mp4_find_child(stbl->buf, stbl->len, MP4_TAG('s', 't', 's', 'z'),
                           &stsz) ||
      !rtsp_mp4_find_child(stbl->buf, stbl->len, MP4_TAG('s', 't', 's', 'c'),
                           &stsc) ||
      !rtsp_mp4_find_child(stbl->buf, stbl->len, MP4_TAG('s', 't', 't', 's'),
                           &stts)) {
    return false;
  }
  has_ctts =
      rtsp_mp4_find_child(stbl->buf, stbl->len, MP4_TAG('c', 't', 't', 's'),
                          &ctts);
  if (!rtsp_mp4_find_child(stbl->buf, stbl->len, MP4_TAG('s', 't', 'c', 'o'),
                           &chunk_box) &&
      !rtsp_mp4_find_child(stbl->buf, stbl->len, MP4_TAG('c', 'o', '6', '4'),
                           &chunk_box)) {
    return false;
  }

  sizes = rtsp_mp4_parse_stsz(&stsz, &sample_count);
  chunk_offsets = rtsp_mp4_parse_chunk_offsets(&chunk_box, &chunk_count);
  stsc_entries = rtsp_mp4_parse_stsc(&stsc, &stsc_count);
  pts = rtsp_mp4_parse_pts(&stts, has_ctts ? &ctts : NULL, sample_count,
                           &duration_units);
  st->audio_samples =
      (struct rtsp_mp4_sample *) mg_calloc(sample_count, sizeof(*st->audio_samples));
  if (sizes == NULL || chunk_offsets == NULL || stsc_entries == NULL || pts == NULL ||
      st->audio_samples == NULL) {
    goto fail;
  }

  for (i = 0; i < stsc_count; i++) {
    uint32_t first_chunk = stsc_entries[i].first_chunk;
    uint32_t last_chunk =
        i + 1U < stsc_count ? stsc_entries[i + 1U].first_chunk - 1U
                            : (uint32_t) chunk_count;
    uint32_t chunk_no;
    if (first_chunk == 0 || first_chunk > last_chunk) goto fail;
    for (chunk_no = first_chunk; chunk_no <= last_chunk; chunk_no++) {
      uint64_t offset;
      uint32_t sample_no;
      if ((size_t) chunk_no > chunk_count) goto fail;
      offset = chunk_offsets[chunk_no - 1U];
      for (sample_no = 0; sample_no < stsc_entries[i].samples_per_chunk; sample_no++) {
        if (sample >= sample_count) goto fail;
        st->audio_samples[sample].offset = offset;
        st->audio_samples[sample].size = sizes[sample];
        st->audio_samples[sample].keyframe = true;
        if (offset > st->file.len || sizes[sample] > st->file.len - (size_t) offset) {
          goto fail;
        }
        offset += sizes[sample];
        sample++;
      }
      chunk++;
    }
  }
  if (sample != sample_count || chunk != chunk_count) goto fail;

  min_pts = pts[0];
  for (i = 0; i < sample_count; i++) {
    if (pts[i] < min_pts) min_pts = pts[i];
  }
  for (i = 0; i < sample_count; i++) {
    st->audio_samples[i].ts = (uint64_t) (pts[i] - min_pts);
    st->audio_samples[i].pts_us =
        rtsp_scale_us(st->audio_samples[i].ts, st->audio_timescale);
  }
  st->audio_sample_count = sample_count;
  st->audio_sample_index = 0;

  mg_free(sizes);
  mg_free(chunk_offsets);
  mg_free(stsc_entries);
  mg_free(pts);
  return true;

fail:
  mg_free(sizes);
  mg_free(chunk_offsets);
  mg_free(stsc_entries);
  mg_free(pts);
  mg_free(st->audio_samples);
  st->audio_samples = NULL;
  return false;
}

static bool rtsp_mp4_parse_file(struct rtsp_mp4_state *st) {
  struct mp4_box root_moov, trak;
  size_t ofs = 0;
  if (!rtsp_mp4_find_child((const uint8_t *) st->file.buf, st->file.len,
                           MP4_TAG('m', 'o', 'o', 'v'), &root_moov)) {
    return false;
  }
  while (rtsp_mp4_next_box(root_moov.buf, root_moov.len, &ofs, &trak)) {
    struct mp4_box mdia, mdhd, minf, stbl, stsd;
    uint32_t track_type;
    if (trak.type != MP4_TAG('t', 'r', 'a', 'k')) continue;
    if (!rtsp_mp4_find_child(trak.buf, trak.len, MP4_TAG('m', 'd', 'i', 'a'),
                             &mdia) ||
        !rtsp_mp4_find_child(mdia.buf, mdia.len, MP4_TAG('m', 'd', 'h', 'd'),
                             &mdhd) ||
        !rtsp_mp4_find_child(mdia.buf, mdia.len, MP4_TAG('m', 'i', 'n', 'f'),
                             &minf) ||
        !rtsp_mp4_find_child(minf.buf, minf.len, MP4_TAG('s', 't', 'b', 'l'),
                             &stbl) ||
        !rtsp_mp4_find_child(stbl.buf, stbl.len, MP4_TAG('s', 't', 's', 'd'),
                             &stsd)) {
      continue;
    }
    track_type = rtsp_mp4_track_type(&mdia);
    if (track_type == MP4_TAG('v', 'i', 'd', 'e') && st->samples == NULL) {
      if (!rtsp_mp4_parse_mdhd(st, &mdhd) || !rtsp_mp4_parse_stsd(st, &stsd) ||
          !rtsp_mp4_parse_samples(st, &stbl)) {
        return false;
      }
    } else if (track_type == MP4_TAG('s', 'o', 'u', 'n') &&
               st->audio_samples == NULL) {
      if (!rtsp_mp4_parse_audio_mdhd(st, &mdhd) ||
          !rtsp_mp4_parse_audio_stsd(st, &stsd) ||
          !rtsp_mp4_parse_audio_samples(st, &stbl)) {
        st->audio_rate = 0;
        st->audio_channels = 0;
        mg_free(st->audio_config);
        st->audio_config = NULL;
        st->audio_config_len = 0;
        mg_free(st->audio_samples);
        st->audio_samples = NULL;
        st->audio_sample_count = 0;
        st->audio_sample_index = 0;
      }
    }
  }
  return st->samples != NULL && st->sps != NULL && st->pps != NULL;
}

static struct rtsp_mp4_state *rtsp_mp4_find_state(const char *path) {
  struct rtsp_mp4_state *st;
  for (st = s_rtsp_mp4_states; st != NULL; st = st->next) {
    if (st->path != NULL && path != NULL && strcmp(st->path, path) == 0) return st;
  }
  return NULL;
}

static struct rtsp_mp4_state *rtsp_mp4_get_state(const char *path) {
  struct rtsp_mp4_state *st;
  if (path == NULL) return NULL;
  st = rtsp_mp4_find_state(path);
  if (st != NULL) return st;

  st = (struct rtsp_mp4_state *) mg_calloc(1, sizeof(*st));
  if (st == NULL) return NULL;
  st->path = rtsp_strdup(path);
  if (st->path == NULL) goto fail;
  st->file = mg_file_read(&mg_fs_posix, path);
  if (st->file.buf == NULL || st->file.len == 0) goto fail;
  if (!rtsp_mp4_parse_file(st)) goto fail;
  st->next = s_rtsp_mp4_states;
  s_rtsp_mp4_states = st;
  return st;

fail:
  rtsp_mp4_free_state(st);
  return NULL;
}

static void rtsp_mp4_reset(const char *path) {
  struct rtsp_mp4_state *st = rtsp_mp4_find_state(path);
  if (st == NULL) return;
  st->sample_index = st->start_index;
  st->audio_sample_index = 0;
}

static bool rtsp_mp4_ensure_au(struct rtsp_mp4_state *st, size_t cap) {
  uint8_t *buf;
  if (cap <= st->au_cap) return true;
  buf = (uint8_t *) mg_calloc(cap, 1U);
  if (buf == NULL) return false;
  mg_free(st->au);
  st->au = buf;
  st->au_cap = cap;
  return true;
}

bool mg_rtsp_get_mp4_frame(struct mg_rtsp_frame *frame, void *fn_data) {
  const char *path = (const char *) fn_data;
  struct rtsp_mp4_state *st = rtsp_mp4_get_state(path);
  struct rtsp_mp4_sample *sample;
  const uint8_t *src;
  size_t remaining, au_len = 0, prefix_len = 0;

  if (frame == NULL || st == NULL || st->sample_count == 0) return false;
  if (st->sample_index >= st->sample_count) return false;
  sample = &st->samples[st->sample_index];
  prefix_len = sample->keyframe ? st->param_sets_len : 0;
  if (!rtsp_mp4_ensure_au(st, prefix_len + sample->size * 2U + 16U))
    return false;

  if (prefix_len > 0) {
    memcpy(st->au, st->param_sets, prefix_len);
    au_len = prefix_len;
  }

  src = (const uint8_t *) st->file.buf + sample->offset;
  remaining = sample->size;
  while (remaining > (size_t) st->nal_len_size) {
    uint32_t nal_len = 0;
    unsigned i;
    for (i = 0; i < st->nal_len_size; i++) nal_len = (nal_len << 8) | src[i];
    src += st->nal_len_size;
    remaining -= st->nal_len_size;
    if (nal_len == 0 || nal_len > remaining || au_len + 4U + nal_len > st->au_cap)
      return false;
    memcpy(st->au + au_len, "\0\0\0\1", 4U);
    au_len += 4U;
    memcpy(st->au + au_len, src, nal_len);
    au_len += nal_len;
    src += nal_len;
    remaining -= nal_len;
  }
  if (au_len == 0) return false;

  frame->data = st->au;
  frame->len = au_len;
  frame->pts_us = sample->pts_us >= st->samples[st->start_index].pts_us
                      ? sample->pts_us - st->samples[st->start_index].pts_us
                      : 0;
  frame->keyframe = sample->keyframe;

  st->sample_index++;
  return true;
}

bool mg_rtsp_get_mp4_audio(struct mg_rtsp_audio *audio, void *fn_data) {
  const char *path = (const char *) fn_data;
  struct rtsp_mp4_state *st = rtsp_mp4_get_state(path);
  struct rtsp_mp4_sample *sample;

  if (audio == NULL || st == NULL || st->audio_samples == NULL ||
      st->audio_sample_count == 0 || st->audio_sample_index >= st->audio_sample_count) {
    return false;
  }

  sample = &st->audio_samples[st->audio_sample_index++];
  audio->data = (const uint8_t *) st->file.buf + sample->offset;
  audio->len = sample->size;
  audio->pts_us = sample->pts_us;
  return true;
}

static uint64_t rtsp_mp4_track_period_ms(const struct rtsp_mp4_sample *samples,
                                         size_t sample_count, size_t sample_index,
                                         uint64_t fallback_ms) {
  uint64_t delta_us;

  if (samples == NULL || sample_count == 0 || sample_index >= sample_count) {
    return fallback_ms == 0 ? 1U : fallback_ms;
  }
  if (sample_index + 1U < sample_count &&
      samples[sample_index + 1U].pts_us > samples[sample_index].pts_us) {
    delta_us = samples[sample_index + 1U].pts_us - samples[sample_index].pts_us;
  } else if (sample_index > 0 &&
             samples[sample_index].pts_us > samples[sample_index - 1U].pts_us) {
    delta_us = samples[sample_index].pts_us - samples[sample_index - 1U].pts_us;
  } else {
    delta_us = (fallback_ms == 0 ? 1U : fallback_ms) * 1000ULL;
  }
  return (delta_us + 500ULL) / 1000ULL == 0 ? 1U : (delta_us + 500ULL) / 1000ULL;
}

static void bw_init(struct h264_bw *bw, uint8_t *buf, size_t cap) {
  memset(bw, 0, sizeof(*bw));
  bw->buf = buf;
  bw->cap = cap;
}

static void bw_put_bit(struct h264_bw *bw, unsigned bit) {
  if (bw->oom) return;
  if (bw->len >= bw->cap) {
    bw->oom = true;
    return;
  }
  if (bw->bitpos == 0) bw->buf[bw->len] = 0;
  if (bit) bw->buf[bw->len] |= (uint8_t) (1U << (7U - bw->bitpos));
  bw->bitpos++;
  if (bw->bitpos == 8) {
    bw->bitpos = 0;
    bw->len++;
  }
}

static void bw_put_bits(struct h264_bw *bw, unsigned count, uint32_t value) {
  while (count > 0) {
    count--;
    bw_put_bit(bw, (value >> count) & 1U);
  }
}

static void bw_put_ue(struct h264_bw *bw, uint32_t value) {
  uint32_t code_num = value + 1U;
  unsigned leading_zero_bits = 0;
  uint32_t tmp = code_num;
  while ((tmp >>= 1U) != 0U) leading_zero_bits++;
  {
    unsigned i;
    for (i = 0; i < leading_zero_bits; i++) bw_put_bit(bw, 0);
  }
  bw_put_bits(bw, leading_zero_bits + 1U, code_num);
}

static void bw_put_se(struct h264_bw *bw, int32_t value) {
  uint32_t code_num = value <= 0 ? (uint32_t) (-value * 2) : (uint32_t) (value * 2 - 1);
  bw_put_ue(bw, code_num);
}

static void bw_align_zero(struct h264_bw *bw) {
  if (bw->oom) return;
  if (bw->bitpos != 0) {
    bw->bitpos = 0;
    bw->len++;
  }
}

static void bw_put_byte(struct h264_bw *bw, uint8_t byte) {
  if (bw->oom) return;
  bw_align_zero(bw);
  if (bw->len >= bw->cap) {
    bw->oom = true;
    return;
  }
  bw->buf[bw->len++] = byte;
}

static void bw_rbsp_trailing_bits(struct h264_bw *bw) {
  bw_put_bit(bw, 1U);
  while (bw->bitpos != 0) bw_put_bit(bw, 0U);
}

static size_t h264_nal_from_rbsp(uint8_t nal_header, const uint8_t *rbsp,
                                 size_t rbsp_len, uint8_t *out, size_t out_cap) {
  size_t i, len = 0, zeros = 0;
  if (out_cap == 0) return 0;
  out[len++] = nal_header;
  for (i = 0; i < rbsp_len && len < out_cap; i++) {
    uint8_t byte = rbsp[i];
    if (zeros >= 2 && byte <= 3) {
      if (len >= out_cap) return 0;
      out[len++] = 3;
      zeros = 0;
    }
    if (len >= out_cap) return 0;
    out[len++] = byte;
    zeros = byte == 0 ? zeros + 1 : 0;
  }
  return i == rbsp_len ? len : 0;
}

static bool h264_build_sps(struct h264_testsrc_state *st) {
  struct h264_bw bw;
  uint8_t rbsp[32];

  bw_init(&bw, rbsp, sizeof(rbsp));
  bw_put_bits(&bw, 8, 66U);
  bw_put_bit(&bw, 1U);
  bw_put_bit(&bw, 1U);
  bw_put_bit(&bw, 0U);
  bw_put_bit(&bw, 0U);
  bw_put_bit(&bw, 0U);
  bw_put_bit(&bw, 0U);
  bw_put_bits(&bw, 2, 0U);
  bw_put_bits(&bw, 8, 31U);
  bw_put_ue(&bw, 0U);
  bw_put_ue(&bw, 0U);
  bw_put_ue(&bw, 0U);
  bw_put_ue(&bw, 0U);
  bw_put_ue(&bw, 2U);
  bw_put_bit(&bw, 0U);
  bw_put_ue(&bw, st->mb_width - 1U);
  bw_put_ue(&bw, st->mb_height - 1U);
  bw_put_bit(&bw, 1U);
  bw_put_bit(&bw, 1U);
  bw_put_bit(&bw, 0U);
  bw_put_bit(&bw, 0U);
  bw_rbsp_trailing_bits(&bw);
  if (bw.oom) return false;

  st->sps_len = h264_nal_from_rbsp(0x67U, rbsp, bw.len, st->sps, sizeof(st->sps));
  return st->sps_len > 0;
}

static bool h264_build_pps(struct h264_testsrc_state *st) {
  struct h264_bw bw;
  uint8_t rbsp[16];

  bw_init(&bw, rbsp, sizeof(rbsp));
  bw_put_ue(&bw, 0U);
  bw_put_ue(&bw, 0U);
  bw_put_bit(&bw, 0U);
  bw_put_bit(&bw, 0U);
  bw_put_ue(&bw, 0U);
  bw_put_ue(&bw, 0U);
  bw_put_ue(&bw, 0U);
  bw_put_bit(&bw, 0U);
  bw_put_bits(&bw, 2, 0U);
  bw_put_se(&bw, 0);
  bw_put_se(&bw, 0);
  bw_put_se(&bw, 0);
  bw_put_bit(&bw, 1U);
  bw_put_bit(&bw, 0U);
  bw_put_bit(&bw, 0U);
  bw_rbsp_trailing_bits(&bw);
  if (bw.oom) return false;

  st->pps_len = h264_nal_from_rbsp(0x68U, rbsp, bw.len, st->pps, sizeof(st->pps));
  return st->pps_len > 0;
}

static bool h264_testsrc_init(struct h264_testsrc_state *st, unsigned width,
                              unsigned height, unsigned fps) {
  memset(st, 0, sizeof(*st));
  st->width = width == 0 ? 640U : width;
  st->height = height == 0 ? 480U : height;
  st->fps = fps == 0 ? 30U : fps;
  st->mb_width = (st->width + 15U) / 16U;
  st->mb_height = (st->height + 15U) / 16U;
  st->box_w = st->mb_width / 8U;
  st->box_h = st->mb_height / 8U;
  if (st->box_w < 4U) st->box_w = 4U;
  if (st->box_h < 4U) st->box_h = 4U;
  if (st->box_w > st->mb_width) st->box_w = st->mb_width;
  if (st->box_h > st->mb_height) st->box_h = st->mb_height;
  st->dx = 1;
  st->dy = 1;
  if (!h264_build_sps(st) || !h264_build_pps(st)) return false;
  st->inited = true;
  return true;
}

static void h264_append_start_code(uint8_t *buf, size_t *len) {
  buf[(*len)++] = 0;
  buf[(*len)++] = 0;
  buf[(*len)++] = 0;
  buf[(*len)++] = 1;
}

static void h264_emit_pcm_macroblock(struct h264_bw *bw,
                                     const struct h264_color *color) {
  unsigned i;
  bw_put_ue(bw, 25U);
  bw_align_zero(bw);
  for (i = 0; i < 16U * 16U; i++) bw_put_byte(bw, color->y);
  for (i = 0; i < 8U * 8U; i++) bw_put_byte(bw, color->cb);
  for (i = 0; i < 8U * 8U; i++) bw_put_byte(bw, color->cr);
}

static bool h264_build_access_unit(struct h264_testsrc_state *st,
                                   struct mg_rtsp_frame *frame) {
  static const struct h264_color kGreen = {145U, 54U, 34U};
  static const struct h264_color kYellow = {210U, 16U, 146U};
  struct h264_bw bw;
  uint8_t idr[H264_MAX_AU];
  size_t au_len = 0, idr_len;
  unsigned mb_x, mb_y;

  bw_init(&bw, st->rbsp, sizeof(st->rbsp));
  bw_put_ue(&bw, 0U);
  bw_put_ue(&bw, 2U);
  bw_put_ue(&bw, 0U);
  bw_put_bits(&bw, 4U, 0U);
  bw_put_ue(&bw, 0U);
  bw_put_bits(&bw, 4U, 0U);
  bw_put_bit(&bw, 0U);
  bw_put_bit(&bw, 0U);
  bw_put_se(&bw, 0);
  bw_put_ue(&bw, 1U);

  for (mb_y = 0; mb_y < st->mb_height; mb_y++) {
    for (mb_x = 0; mb_x < st->mb_width; mb_x++) {
      bool in_box = mb_x >= (unsigned) st->box_x &&
                    mb_x < (unsigned) st->box_x + st->box_w &&
                    mb_y >= (unsigned) st->box_y &&
                    mb_y < (unsigned) st->box_y + st->box_h;
      h264_emit_pcm_macroblock(&bw, in_box ? &kYellow : &kGreen);
    }
  }
  bw_rbsp_trailing_bits(&bw);
  if (bw.oom) return false;

  idr_len = h264_nal_from_rbsp(0x65U, st->rbsp, bw.len, idr, sizeof(idr));
  if (idr_len == 0) return false;

  h264_append_start_code(st->au, &au_len);
  memcpy(st->au + au_len, st->sps, st->sps_len);
  au_len += st->sps_len;
  h264_append_start_code(st->au, &au_len);
  memcpy(st->au + au_len, st->pps, st->pps_len);
  au_len += st->pps_len;
  h264_append_start_code(st->au, &au_len);
  memcpy(st->au + au_len, idr, idr_len);
  au_len += idr_len;

  frame->data = st->au;
  frame->len = au_len;
  frame->pts_us = (st->frame_no * 1000000ULL) / st->fps;
  frame->keyframe = true;
  st->frame_no++;

  st->box_x += st->dx;
  st->box_y += st->dy;
  if (st->box_x < 0) {
    st->box_x = 0;
    st->dx = 1;
  } else if ((unsigned) st->box_x + st->box_w >= st->mb_width) {
    st->box_x = (int) (st->mb_width - st->box_w);
    st->dx = -1;
  }
  if (st->box_y < 0) {
    st->box_y = 0;
    st->dy = 1;
  } else if ((unsigned) st->box_y + st->box_h >= st->mb_height) {
    st->box_y = (int) (st->mb_height - st->box_h);
    st->dy = -1;
  }
  return true;
}

bool mg_rtsp_h264_testsrc_next_frame(struct mg_rtsp_frame *frame, unsigned width,
                                     unsigned height, unsigned fps) {
  static struct h264_testsrc_state st;
  if (frame == NULL) return false;
  if (!st.inited || st.width != width || st.height != height || st.fps != fps) {
    if (!h264_testsrc_init(&st, width, height, fps)) return false;
  }
  return h264_build_access_unit(&st, frame);
}

static size_t rtsp_hex_encode(const uint8_t *buf, size_t len, char *dst,
                              size_t dst_len) {
  static const char *hex = "0123456789ABCDEF";
  size_t i, out = 0;
  if (dst_len == 0) return 0;
  for (i = 0; i < len && out + 2U < dst_len; i++) {
    dst[out++] = hex[buf[i] >> 4];
    dst[out++] = hex[buf[i] & 0x0fU];
  }
  dst[out] = '\0';
  return out;
}

static bool rtsp_aac_lc_config_parse(const uint8_t cfg[2], unsigned *rate,
                                     unsigned *channels) {
  static const unsigned rates[] = {96000U, 88200U, 64000U, 48000U, 44100U,
                                   32000U, 24000U, 22050U, 16000U, 12000U,
                                   11025U, 8000U,  7350U};
  static const unsigned channel_counts[] = {0U, 1U, 2U, 3U, 4U, 5U, 6U, 8U};
  unsigned aot = (cfg[0] >> 3) & 0x1fU;
  unsigned sfi = ((unsigned) (cfg[0] & 0x07U) << 1) | (cfg[1] >> 7);
  unsigned cc = (cfg[1] >> 3) & 0x0fU;

  if (rate == NULL || channels == NULL) return false;
  if (aot != 2U || sfi >= sizeof(rates) / sizeof(rates[0])) return false;
  if (cc == 0U || cc >= sizeof(channel_counts) / sizeof(channel_counts[0])) {
    return false;
  }

  *rate = rates[sfi];
  *channels = channel_counts[cc];
  return *channels > 0;
}

static uint64_t rtsp_aac_frame_period_ms(unsigned sample_rate) {
  uint64_t period_ms;

  if (sample_rate == 0U) return 20U;
  period_ms = (1024000ULL + sample_rate / 2U) / sample_rate;
  return period_ms == 0 ? 1U : period_ms;
}

static uint32_t rtsp_audio_rtp_timestamp(unsigned sample_rate, uint64_t pts_us) {
  if (sample_rate == 0U) return 0U;
  return (uint32_t) ((pts_us * sample_rate + 500000ULL) / 1000000ULL);
}

static bool rtsp_build_sdp(struct rtsp_server *srv) {
  char sps64[128], pps64[128], profile_level_id[7];
  char audio_cfg[128];
  struct mg_rtsp_frame frame;
  struct rtsp_nal nals[RTSP_MAX_NALS];
  const uint8_t *sps = NULL, *pps = NULL;
  struct rtsp_mp4_state *st = NULL, *audio_st = NULL;
  size_t i, sps_len = 0, pps_len = 0, nals_count;
  unsigned fps, audio_rate = 0, audio_channels = 0;

  srv->has_audio = srv->opts.audio_fn != NULL;
  if (srv->has_audio &&
      !rtsp_aac_lc_config_parse(srv->opts.aac_lc_config, &audio_rate,
                                &audio_channels)) {
    return false;
  }
  if (srv->opts.fn == mg_rtsp_get_mp4_frame && srv->opts.fn_data != NULL) {
    st = rtsp_mp4_get_state((const char *) srv->opts.fn_data);
    if (st == NULL || st->sps == NULL || st->pps == NULL) return false;
    if (srv->opts.width == 0) srv->opts.width = st->width;
    if (srv->opts.height == 0) srv->opts.height = st->height;
    if (srv->opts.fps == 0) srv->opts.fps = st->fps;
    sps = st->sps;
    pps = st->pps;
    sps_len = st->sps_len;
    pps_len = st->pps_len;
  } else {
    if (srv->opts.fn == NULL) return false;
    if (!srv->opts.fn(&frame, srv->opts.fn_data) || frame.data == NULL ||
        frame.len == 0) {
      return false;
    }

    nals_count = h264_split_annexb((const uint8_t *) frame.data, frame.len, nals,
                                   sizeof(nals) / sizeof(nals[0]));
    for (i = 0; i < nals_count; i++) {
      if (nals[i].len == 0) continue;
      switch (nals[i].ptr[0] & 0x1fU) {
        case 7:
          sps = nals[i].ptr;
          sps_len = nals[i].len;
          break;
        case 8:
          pps = nals[i].ptr;
          pps_len = nals[i].len;
          break;
        default:
          break;
      }
    }
    if (sps == NULL || pps == NULL || sps_len < 4) return false;
  }

  fps = srv->opts.fps == 0 ? 30U : srv->opts.fps;
  if (srv->has_audio && srv->opts.audio_fn == mg_rtsp_get_mp4_audio &&
      srv->opts.audio_fn_data != NULL) {
    audio_st = rtsp_mp4_get_state((const char *) srv->opts.audio_fn_data);
    if (audio_st == NULL || audio_st->audio_samples == NULL ||
        audio_st->audio_sample_count == 0) {
      return false;
    }
  }

  profile_level_id[0] = '\0';
  mg_snprintf(profile_level_id, sizeof(profile_level_id), "%02X%02X%02X", sps[1],
              sps[2], sps[3]);
  sps64[mg_base64_encode(sps, sps_len, sps64, sizeof(sps64) - 1U)] = '\0';
  pps64[mg_base64_encode(pps, pps_len, pps64, sizeof(pps64) - 1U)] = '\0';
  mg_snprintf(
      srv->sdp, sizeof(srv->sdp),
      "v=0\r\n"
      "o=- 0 0 IN IP4 0.0.0.0\r\n"
      "s=Mongoose RTSP Stream\r\n"
      "t=0 0\r\n"
      "a=control:*\r\n"
      "m=video 0 RTP/AVP %u\r\n"
      "c=IN IP4 0.0.0.0\r\n"
      "a=rtpmap:%u H264/90000\r\n"
      "a=fmtp:%u packetization-mode=1;profile-level-id=%s;"
      "sprop-parameter-sets=%s,%s\r\n"
      "a=framerate:%u\r\n"
      "a=framesize:%u %u-%u\r\n"
      "a=control:trackID=0\r\n",
      RTSP_VIDEO_RTP_PT, RTSP_VIDEO_RTP_PT, RTSP_VIDEO_RTP_PT, profile_level_id,
      sps64, pps64, fps, RTSP_VIDEO_RTP_PT, srv->opts.width, srv->opts.height);
  if (srv->has_audio) {
    rtsp_hex_encode(srv->opts.aac_lc_config, sizeof(srv->opts.aac_lc_config),
                    audio_cfg, sizeof(audio_cfg));
    mg_snprintf(
        srv->sdp + strlen(srv->sdp), sizeof(srv->sdp) - strlen(srv->sdp),
        "m=audio 0 RTP/AVP %u\r\n"
        "c=IN IP4 0.0.0.0\r\n"
        "a=rtpmap:%u MPEG4-GENERIC/%u/%u\r\n"
        "a=fmtp:%u streamtype=5;profile-level-id=1;mode=AAC-hbr;"
        "sizelength=13;indexlength=3;indexdeltalength=3;config=%s\r\n"
        "a=control:trackID=1\r\n",
        RTSP_AUDIO_RTP_PT, RTSP_AUDIO_RTP_PT, audio_rate, audio_channels,
        RTSP_AUDIO_RTP_PT, audio_cfg);
  }
  return true;
}

static void rtsp_send_rtp_packet(struct mg_connection *c,
                                 struct rtsp_track_conn *tc, uint8_t pt,
                                 bool marker, uint32_t timestamp,
                                 const void *payload, size_t payload_len) {
  uint8_t packet[4 + 12 + RTSP_RTP_PAYLOAD_MAX + 2];
  uint16_t packet_len = (uint16_t) (12U + payload_len);
  uint16_t net16;
  uint32_t net32;

  if (payload_len > sizeof(packet) - 16U) return;
  packet[0] = '$';
  packet[1] = (uint8_t) tc->rtp_channel;
  net16 = mg_htons(packet_len);
  memcpy(packet + 2, &net16, sizeof(net16));
  packet[4] = 0x80U;
  packet[5] = (uint8_t) ((marker ? 0x80U : 0U) | pt);
  net16 = mg_htons(tc->rtp_seq++);
  memcpy(packet + 6, &net16, sizeof(net16));
  net32 = mg_htonl(timestamp);
  memcpy(packet + 8, &net32, sizeof(net32));
  net32 = mg_htonl(tc->ssrc);
  memcpy(packet + 12, &net32, sizeof(net32));
  memcpy(packet + 16, payload, payload_len);
  mg_send(c, packet, (size_t) packet_len + 4U);
}

static void rtsp_send_h264_nal(struct mg_connection *c,
                               struct rtsp_track_conn *tc, const uint8_t *nal,
                               size_t nal_len, bool last_nal,
                               uint32_t timestamp) {
  if (nal_len == 0) return;
  if (nal_len <= RTSP_RTP_PAYLOAD_MAX) {
    rtsp_send_rtp_packet(c, tc, RTSP_VIDEO_RTP_PT, last_nal, timestamp, nal,
                         nal_len);
  } else {
    size_t off = 1;
    uint8_t fu[2 + RTSP_RTP_PAYLOAD_MAX];
    uint8_t nal_type = nal[0] & 0x1fU;
    uint8_t nri = nal[0] & 0x60U;
    size_t max_chunk = RTSP_RTP_PAYLOAD_MAX - 2U;

    fu[0] = (uint8_t) (nri | 28U);
    while (off < nal_len) {
      size_t chunk = nal_len - off;
      bool first = off == 1U;
      bool last = off + chunk >= nal_len;
      if (chunk > max_chunk) chunk = max_chunk;
      last = off + chunk >= nal_len;
      fu[1] = (uint8_t) ((first ? 0x80U : 0U) | (last ? 0x40U : 0U) | nal_type);
      memcpy(fu + 2, nal + off, chunk);
      rtsp_send_rtp_packet(c, tc, RTSP_VIDEO_RTP_PT, last_nal && last, timestamp,
                           fu, chunk + 2U);
      off += chunk;
    }
  }
}

static void rtsp_send_access_unit(struct mg_connection *c,
                                  struct rtsp_track_conn *tc,
                                  const struct mg_rtsp_frame *frame) {
  struct rtsp_nal nals[RTSP_MAX_NALS];
  size_t i, count;
  uint32_t timestamp =
      (uint32_t) ((frame->pts_us * RTSP_VIDEO_RTP_CLOCK) / 1000000ULL);

  count = h264_split_annexb((const uint8_t *) frame->data, frame->len, nals,
                            sizeof(nals) / sizeof(nals[0]));
  for (i = 0; i < count; i++) {
    rtsp_send_h264_nal(c, tc, nals[i].ptr, nals[i].len, i + 1U == count,
                       timestamp);
  }
}

static void rtsp_send_audio_sample(struct mg_connection *c,
                                   struct rtsp_track_conn *tc,
                                   const uint8_t *sample, size_t sample_len,
                                   uint32_t timestamp) {
  uint8_t payload[4 + RTSP_RTP_PAYLOAD_MAX];

  if (sample == NULL || sample_len == 0 || sample_len + 4U > sizeof(payload)) return;
  payload[0] = 0;
  payload[1] = 16;
  payload[2] = (uint8_t) (sample_len >> 5);
  payload[3] = (uint8_t) ((sample_len & 0x1fU) << 3);
  memcpy(payload + 4, sample, sample_len);
  rtsp_send_rtp_packet(c, tc, RTSP_AUDIO_RTP_PT, true, timestamp, payload,
                       sample_len + 4U);
}

static void rtsp_handle_options(struct mg_connection *c, struct rtsp_conn *rc,
                                const struct rtsp_request *req) {
  char headers[192];
  (void) rc;
  mg_snprintf(headers, sizeof(headers),
              "Public: OPTIONS, DESCRIBE, SETUP, PLAY, PAUSE, TEARDOWN, "
              "GET_PARAMETER\r\n");
  rtsp_send_status(c, req, 200, "OK", headers);
}

static void rtsp_handle_describe(struct mg_connection *c, struct rtsp_conn *rc,
                                 const struct rtsp_request *req) {
  char headers[128];
  if (!rtsp_path_eq(req->uri, rc->srv->path)) {
    rtsp_send_status(c, req, 404, "Not Found", NULL);
    return;
  }
  mg_snprintf(headers, sizeof(headers),
              "Content-Type: application/sdp\r\n"
              "Content-Base: %.*s/\r\n",
              (int) req->uri.len, req->uri.buf);
  rtsp_send_response(c, req, 200, "OK", headers, rc->srv->sdp,
                     strlen(rc->srv->sdp));
}

static bool rtsp_any_setup_done(const struct rtsp_conn *rc) {
  size_t i;
  for (i = 0; i < RTSP_TRACK_COUNT; i++) {
    if (rc->tracks[i].setup_done) return true;
  }
  return false;
}

static void rtsp_handle_setup(struct mg_connection *c, struct rtsp_conn *rc,
                              const struct rtsp_request *req) {
  char headers[160];
  int track_id = rtsp_track_path_eq(req->uri, rc->srv->path);
  struct rtsp_track_conn *tc;

  if (track_id < 0 || (unsigned) track_id >= RTSP_TRACK_COUNT ||
      ((unsigned) track_id == RTSP_AUDIO_TRACK && !rc->srv->has_audio)) {
    rtsp_send_status(c, req, 404, "Not Found", NULL);
    return;
  }
  if (!rtsp_casencontains(req->transport, "RTP/AVP/TCP")) {
    rtsp_send_status(c, req, 461, "Unsupported Transport", NULL);
    return;
  }
  tc = &rc->tracks[track_id];
  rtsp_parse_interleaved_channels(req->transport, &tc->rtp_channel, &tc->rtcp_channel);
  tc->setup_done = true;
  tc->ended = false;
  mg_snprintf(headers, sizeof(headers),
              "Transport: RTP/AVP/TCP;unicast;interleaved=%u-%u\r\n"
              "Session: %s\r\n",
              tc->rtp_channel, tc->rtcp_channel, rc->session_id);
  rtsp_send_status(c, req, 200, "OK", headers);
}

static void rtsp_handle_play(struct mg_connection *c, struct rtsp_conn *rc,
                             const struct rtsp_request *req) {
  char headers[128];
  size_t i;
  uint64_t now_ms;

  if (!rtsp_any_setup_done(rc)) {
    rtsp_send_status(c, req, 455, "Method Not Valid In This State", NULL);
    return;
  }
  if (!rtsp_session_matches(rc, req->session)) {
    rtsp_send_status(c, req, 454, "Session Not Found", NULL);
    return;
  }
  rc->playing = true;
  now_ms = mg_millis();
  for (i = 0; i < RTSP_TRACK_COUNT; i++) {
    if (rc->tracks[i].setup_done) {
      rc->tracks[i].deadline_ms = now_ms;
      rc->tracks[i].ended = false;
    }
  }
  if (rc->srv->opts.fn == mg_rtsp_get_mp4_frame && rc->srv->opts.fn_data != NULL) {
    rtsp_mp4_reset((const char *) rc->srv->opts.fn_data);
  }
  if (rc->srv->opts.audio_fn == mg_rtsp_get_mp4_audio &&
      rc->srv->opts.audio_fn_data != NULL) {
    rtsp_mp4_reset((const char *) rc->srv->opts.audio_fn_data);
  }
  mg_snprintf(headers, sizeof(headers), "Session: %s\r\nRange: npt=0.000-\r\n",
              rc->session_id);
  rtsp_send_status(c, req, 200, "OK", headers);
}

static void rtsp_handle_pause(struct mg_connection *c, struct rtsp_conn *rc,
                              const struct rtsp_request *req) {
  char headers[64];
  if (!rtsp_session_matches(rc, req->session)) {
    rtsp_send_status(c, req, 454, "Session Not Found", NULL);
    return;
  }
  rc->playing = false;
  mg_snprintf(headers, sizeof(headers), "Session: %s\r\n", rc->session_id);
  rtsp_send_status(c, req, 200, "OK", headers);
}

static void rtsp_handle_teardown(struct mg_connection *c, struct rtsp_conn *rc,
                                 const struct rtsp_request *req) {
  char headers[64];
  if (!rtsp_session_matches(rc, req->session)) {
    rtsp_send_status(c, req, 454, "Session Not Found", NULL);
    return;
  }
  rc->playing = false;
  mg_snprintf(headers, sizeof(headers), "Session: %s\r\n", rc->session_id);
  rtsp_send_status(c, req, 200, "OK", headers);
  c->is_draining = 1;
}

static void rtsp_handle_get_parameter(struct mg_connection *c,
                                      struct rtsp_conn *rc,
                                      const struct rtsp_request *req) {
  char headers[64];
  (void) rc;
  mg_snprintf(headers, sizeof(headers), "Session: %s\r\n", rc->session_id);
  rtsp_send_status(c, req, 200, "OK", headers);
}

static void rtsp_dispatch_request(struct mg_connection *c, struct rtsp_conn *rc,
                                  const struct rtsp_request *req) {
  if (rtsp_caseeq(req->method, "OPTIONS")) {
    rtsp_handle_options(c, rc, req);
  } else if (rtsp_caseeq(req->method, "DESCRIBE")) {
    rtsp_handle_describe(c, rc, req);
  } else if (rtsp_caseeq(req->method, "SETUP")) {
    rtsp_handle_setup(c, rc, req);
  } else if (rtsp_caseeq(req->method, "PLAY")) {
    rtsp_handle_play(c, rc, req);
  } else if (rtsp_caseeq(req->method, "PAUSE")) {
    rtsp_handle_pause(c, rc, req);
  } else if (rtsp_caseeq(req->method, "TEARDOWN")) {
    rtsp_handle_teardown(c, rc, req);
  } else if (rtsp_caseeq(req->method, "GET_PARAMETER")) {
    rtsp_handle_get_parameter(c, rc, req);
  } else {
    rtsp_send_status(c, req, 405, "Method Not Allowed", NULL);
  }
}

static void rtsp_handle_read(struct mg_connection *c) {
  struct rtsp_conn *rc = (struct rtsp_conn *) c->fn_data;

  for (;;) {
    if (c->recv.len >= 4 && ((uint8_t *) c->recv.buf)[0] == '$') {
      size_t frame_len =
          ((size_t) ((uint8_t *) c->recv.buf)[2] << 8) | ((uint8_t *) c->recv.buf)[3];
      if (c->recv.len < frame_len + 4U) return;
      mg_iobuf_del(&c->recv, 0, frame_len + 4U);
      continue;
    }

    {
      struct rtsp_request req;
      int result = rtsp_parse_request((const char *) c->recv.buf, c->recv.len, &req);
      if (result == 0) return;
      if (result < 0) {
        c->is_closing = 1;
        return;
      }
      rtsp_dispatch_request(c, rc, &req);
      mg_iobuf_del(&c->recv, 0, req.msg_len);
    }
  }
}

static void rtsp_handle_poll(struct mg_connection *c, uint64_t now_ms) {
  struct rtsp_conn *rc = (struct rtsp_conn *) c->fn_data;
  struct rtsp_mp4_state *video_st = NULL, *audio_st = NULL;
  unsigned audio_rate = 0, audio_channels = 0;
  unsigned fps = rc->srv->opts.fps == 0 ? 30U : rc->srv->opts.fps;
  uint64_t video_fallback_ms = 1000U / fps;
  uint64_t audio_fallback_ms = 20U;
  unsigned burst = 0, max_burst = 128U;
  size_t send_limit = 256U * 1024U;

  if (!rc->playing) return;
  if (!rc->tracks[RTSP_VIDEO_TRACK].setup_done &&
      !rc->tracks[RTSP_AUDIO_TRACK].setup_done) {
    return;
  }
  if (video_fallback_ms == 0) video_fallback_ms = 1;
  if (rc->srv->opts.fn == NULL) return;
  if (c->send.len >= send_limit) return;

  if (rc->srv->opts.fn == mg_rtsp_get_mp4_frame && rc->srv->opts.fn_data != NULL) {
    video_st = rtsp_mp4_get_state((const char *) rc->srv->opts.fn_data);
    if (video_st == NULL) {
      rc->playing = false;
      c->is_draining = 1;
      return;
    }
  }
  if (rc->srv->has_audio) {
    if (!rtsp_aac_lc_config_parse(rc->srv->opts.aac_lc_config, &audio_rate,
                                  &audio_channels)) {
      rc->playing = false;
      c->is_draining = 1;
      return;
    }
    (void) audio_channels;
    audio_fallback_ms = rtsp_aac_frame_period_ms(audio_rate);
    if (rc->srv->opts.audio_fn == mg_rtsp_get_mp4_audio &&
        rc->srv->opts.audio_fn_data != NULL) {
      audio_st = rtsp_mp4_get_state((const char *) rc->srv->opts.audio_fn_data);
      if (audio_st == NULL) {
        rc->playing = false;
        c->is_draining = 1;
        return;
      }
    }
  }

  if (rc->tracks[RTSP_VIDEO_TRACK].setup_done && video_st != NULL &&
      video_st->sample_index >= video_st->sample_count) {
    rc->tracks[RTSP_VIDEO_TRACK].ended = true;
  }
  if (rc->tracks[RTSP_AUDIO_TRACK].setup_done && audio_st != NULL &&
      (audio_st->audio_samples == NULL ||
       audio_st->audio_sample_index >= audio_st->audio_sample_count)) {
    rc->tracks[RTSP_AUDIO_TRACK].ended = true;
  }

  while (rc->tracks[RTSP_VIDEO_TRACK].setup_done &&
         !rc->tracks[RTSP_VIDEO_TRACK].ended && c->send.len < send_limit &&
         mg_timer_expired(&rc->tracks[RTSP_VIDEO_TRACK].deadline_ms,
                          video_st != NULL
                              ? rtsp_mp4_track_period_ms(
                                    video_st->samples, video_st->sample_count,
                                    video_st->sample_index, video_fallback_ms)
                              : video_fallback_ms,
                          now_ms)) {
    struct mg_rtsp_frame frame;
    if (!rc->srv->opts.fn(&frame, rc->srv->opts.fn_data) || frame.data == NULL ||
        frame.len == 0) {
      rc->tracks[RTSP_VIDEO_TRACK].ended = true;
      break;
    }
    rtsp_send_access_unit(c, &rc->tracks[RTSP_VIDEO_TRACK], &frame);
    if (++burst >= max_burst) break;
  }

  while (rc->srv->has_audio && rc->tracks[RTSP_AUDIO_TRACK].setup_done &&
         !rc->tracks[RTSP_AUDIO_TRACK].ended && c->send.len < send_limit &&
         mg_timer_expired(&rc->tracks[RTSP_AUDIO_TRACK].deadline_ms,
                          audio_st != NULL
                              ? rtsp_mp4_track_period_ms(
                                    audio_st->audio_samples,
                                    audio_st->audio_sample_count,
                                    audio_st->audio_sample_index,
                                    audio_fallback_ms)
                              : audio_fallback_ms,
                          now_ms)) {
    struct mg_rtsp_audio audio;
    if (rc->srv->opts.audio_fn == NULL ||
        !rc->srv->opts.audio_fn(&audio, rc->srv->opts.audio_fn_data) ||
        audio.data == NULL || audio.len == 0) {
      rc->tracks[RTSP_AUDIO_TRACK].ended = true;
      break;
    }
    rtsp_send_audio_sample(c, &rc->tracks[RTSP_AUDIO_TRACK], audio.data, audio.len,
                           rtsp_audio_rtp_timestamp(audio_rate, audio.pts_us));
    if (++burst >= max_burst) break;
  }

  if ((!rc->tracks[RTSP_VIDEO_TRACK].setup_done ||
       rc->tracks[RTSP_VIDEO_TRACK].ended) &&
      (!rc->tracks[RTSP_AUDIO_TRACK].setup_done ||
       rc->tracks[RTSP_AUDIO_TRACK].ended)) {
    rc->playing = false;
    c->is_draining = 1;
  }
}

static void rtsp_free_server(struct rtsp_server *srv) {
  if (srv == NULL) return;
  mg_free(srv->path);
  mg_free(srv);
}

static void rtsp_fn(struct mg_connection *c, int ev, void *ev_data) {
  if (c->is_listening) {
    if (ev == MG_EV_CLOSE) rtsp_free_server((struct rtsp_server *) c->fn_data);
    return;
  }

  switch (ev) {
    case MG_EV_ACCEPT: {
      struct rtsp_server *srv = (struct rtsp_server *) c->fn_data;
      struct rtsp_conn *rc =
          (struct rtsp_conn *) mg_calloc(1, sizeof(struct rtsp_conn));
      if (rc == NULL) {
        c->is_closing = 1;
        break;
      }
      rc->srv = srv;
      rc->tracks[RTSP_VIDEO_TRACK].rtp_channel = 0;
      rc->tracks[RTSP_VIDEO_TRACK].rtcp_channel = 1;
      rc->tracks[RTSP_VIDEO_TRACK].rtp_seq = (uint16_t) c->id;
      rc->tracks[RTSP_VIDEO_TRACK].ssrc = 0x13572468U ^ (uint32_t) c->id;
      rc->tracks[RTSP_AUDIO_TRACK].rtp_channel = 2;
      rc->tracks[RTSP_AUDIO_TRACK].rtcp_channel = 3;
      rc->tracks[RTSP_AUDIO_TRACK].rtp_seq = (uint16_t) (c->id ^ 0x4000U);
      rc->tracks[RTSP_AUDIO_TRACK].ssrc = 0x24681357U ^ (uint32_t) c->id;
      mg_snprintf(rc->session_id, sizeof(rc->session_id), "%lu", c->id);
      c->fn_data = rc;
      break;
    }
    case MG_EV_READ:
      rtsp_handle_read(c);
      break;
    case MG_EV_POLL:
      rtsp_handle_poll(c, *(uint64_t *) ev_data);
      break;
    case MG_EV_CLOSE:
      mg_free(c->fn_data);
      break;
    default:
      break;
  }
}

struct mg_connection *mg_rtsp_listen(struct mg_mgr *mgr, const char *url,
                                     const struct mg_rtsp_opts *opts) {
  struct mg_connection *c;
  struct rtsp_server *srv;
  if (mgr == NULL || url == NULL || opts == NULL || opts->fn == NULL) return NULL;

  srv = (struct rtsp_server *) mg_calloc(1, sizeof(*srv));
  if (srv == NULL) return NULL;
  srv->opts = *opts;
  srv->path = rtsp_strdup(mg_url_uri(url));
  if (srv->path == NULL || !rtsp_build_sdp(srv)) {
    rtsp_free_server(srv);
    return NULL;
  }

  c = mg_listen(mgr, url, rtsp_fn, srv);
  if (c == NULL) {
    rtsp_free_server(srv);
    return NULL;
  }
  return c;
}
