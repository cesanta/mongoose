#include "net_builtin.h"
#include "drivers/usb.h"
#include "log.h"
#include "printf.h"
#include "util.h"

// USB Ethernet needs the TCP/IP stack, the serial console does not
#if (MG_ENABLE_TCPIP || !MG_ENABLE_USB_ETHERNET) &&           \
    (MG_ENABLE_DRIVER_USB_RP || MG_ENABLE_DRIVER_USB_NRF52 || \
     MG_ENABLE_DRIVER_USB_DWC2) &&                            \
    (MG_ENABLE_USB_ETHERNET || MG_ENABLE_USB_SERIAL)

// Interfaces: ACM control and data 0/1 if enabled, then NCM control and data.
// The serial port comes first, so that its name on macOS does not depend on
// Ethernet: /dev/cu.usbmodem...01
#define MGI_USB_ETH_IF (MG_USB_IF_ETH_DATA - 1)
#define MGI_USB_NUM_IFS (2 * MG_ENABLE_USB_ETHERNET + 2 * MG_ENABLE_USB_SERIAL)
#define MGI_USB_CONFIG_LEN \
  (9 + 85 * MG_ENABLE_USB_ETHERNET + 66 * MG_ENABLE_USB_SERIAL)
#if MG_ENABLE_USB_ETHERNET
#define MGI_USB_PRODUCT "MongooseUsbEthernet"
#else
#define MGI_USB_PRODUCT "MongooseUsbSerial"
#endif

// VID 0xadde, PID 0x40e4: an example, for development only
static const uint8_t s_device_desc[] = {
    18, 1, 1, 2, 0xef, 2, 1, 64, 0xde, 0xad, 0xe4, 0x40, 0, 1, 1, 2, 3, 1};

// NCM: endpoints 0x81, 0x02/0x82. ACM: endpoints 0x83, 0x04/0x84
// clang-format off
static const uint8_t s_config_desc[] = {
    9, 2, MGI_USB_CONFIG_LEN, 0, MGI_USB_NUM_IFS, 1, 0, 0x80,
    50,                                             // Config
#if MG_ENABLE_USB_SERIAL
    8,  11,   0,    2,    2,  2,    1,  6,          // ACM association
    9,  4,    0,    0,    1,  2,    2,  1,    6,    // ACM control interface
    5,  0x24, 0,    0x20, 1,                        // CDC header
    5,  0x24, 1,    0,    1,                        // Call management
    4,  0x24, 2,    2,                              // ACM
    5,  0x24, 6,    0,    1,                        // Union
    7,  5,    0x83, 3,    16, 0,    16,             // Notification endpoint
    9,  4,    1,    0,    2,  10,   0,  0,    0,    // Data interface
    7,  5,    0x04, 2,    64, 0,    0,              // Data OUT
    7,  5,    0x84, 2,    64, 0,    0,              // Data IN
#endif
#if MG_ENABLE_USB_ETHERNET
    8, 11, MGI_USB_ETH_IF, 2, 2, 13, 0, 4,          // NCM association
    9, 4, MGI_USB_ETH_IF, 0, 1, 2, 13, 0, 4,        // NCM control interface
    5, 0x24, 0, 0x20, 1,                            // CDC header
    5, 0x24, 6, MGI_USB_ETH_IF, MGI_USB_ETH_IF + 1, // Union
    13, 0x24, 15, 5, 0, 0, 0, 0, 0xea,
    5, 0, 0, 0,                                     // Eth, 1514 bytes
    6, 0x24, 0x1a, 0, 1, 0x21,                      // NCM 1.0
    7, 5, 0x81, 3, 16, 0, 16,                       // Notification endpoint
    9, 4, MGI_USB_ETH_IF + 1, 0, 0, 10, 0, 1, 0,    // Data interface, off
    9, 4, MGI_USB_ETH_IF + 1, 1, 2, 10, 0, 1, 0,    // Data interface, on
    7, 5, 0x02, 2, 64, 0, 0,                        // Data OUT
    7, 5, 0x82, 2, 64, 0, 0,                        // Data IN
#endif
};
// clang-format on

// Endpoint addresses, for CLEAR_FEATURE(ENDPOINT_HALT)
static const uint8_t s_endpoints[] = {
#if MG_ENABLE_USB_ETHERNET
    0x81, 0x02, 0x82,
#endif
#if MG_ENABLE_USB_SERIAL
    0x83, 0x04, 0x84,
#endif
};

#if MG_ENABLE_USB_ETHERNET
// Windows 11 binds its NCM driver through a Microsoft OS 2.0 compatible ID
// clang-format off
static const uint8_t s_ms_os_desc[] = {
    10, 0, 0, 0, 0,   0,   3,   6,   46,  0,    // Set header, Windows 8.1+
    8,  0, 1, 0, 0,   0,   36,  0,              // Configuration subset
    8,  0, 2, 0, MGI_USB_ETH_IF, 0, 28, 0,      // Function subset, NCM
    20, 0, 3, 0, 'W', 'I', 'N', 'N', 'C', 'M',  // Compatible ID
    0,  0, 0, 0, 0,   0,   0,   0,   0,   0};
// clang-format on
static const uint8_t s_bos_desc[] = {
    5,    15,   33,   0,    1,    28,   16,   5,    0,    0xdf,
    0x60, 0xdd, 0xd8, 0x89, 0x45, 0xc7, 0x4c, 0x9c, 0xd2, 0x65,
    0x9d, 0x9e, 0x64, 0x8a, 0x9f, 0,    0,    3,    6,    sizeof(s_ms_os_desc),
    0,    1,    0};

// GET_NTB_PARAMETERS reply: NTB16 only, same limits both ways
static const uint8_t s_ntb_params[] = {
    28, 0, 1,  0, MG_USB_NTB_SIZE & 255, MG_USB_NTB_SIZE >> 8, 0, 0, 4, 0, 0, 0,
    4,  0, 0,  0, MG_USB_NTB_SIZE & 255, MG_USB_NTB_SIZE >> 8, 0, 0, 4, 0, 0, 0,
    4,  0, 16, 0};

// Link notifications: speed 12 Mbit/s both ways, connected, disconnected
static const uint8_t s_speed[] = {
    0xa1, 0x2a, 0, 0, MGI_USB_ETH_IF, 0, 8, 0,
    0, 0x1b, 0xb7, 0, 0, 0x1b, 0xb7, 0};
static const uint8_t s_up[] = {0xa1, 0, 1, 0, MGI_USB_ETH_IF, 0, 0, 0};
static const uint8_t s_down[] = {0xa1, 0, 0, 0, MGI_USB_ETH_IF, 0, 0, 0};

// Transfer block header for one frame: NTH16, NDP16 with one datagram at
// offset 30, so that the IP header after the 14-byte Ethernet header lands
// on a 4-byte boundary
static const uint8_t s_nth[30] = {'N', 'C', 'M', 'H', 12,  0,   0,  0, 0, 0,
                                  12,  0,   'N', 'C', 'M', '0', 16, 0, 0, 0,
                                  30,  0,   0,   0,   0,   0,   0,  0, 0, 0};
#endif

// An IN transfer on an endpoint, sent a packet at a time
struct mgi_usb_in {
  const uint8_t *data;  // Bytes still to send
  size_t left;          // How many
  unsigned packets;     // Packets still to send: a short or empty one ends it
  bool busy;            // A packet is waiting for the host
};

// The USB device. There is one
static struct mgi_usb {
  const struct mg_usb_hw *hw;  // Controller, NULL until mg_usb_init()
  struct mgi_usb_in in[5];     // IN transfers, by endpoint
  char serial[17];             // Serial number, from the controller
  bool configured;             // Host has set the configuration
  uint8_t buf[66];             // Generated control replies
#if MG_ENABLE_USB_ETHERNET
  struct mg_tcpip_if *ifp;       // Ethernet interface, NULL: serial only
  bool alt;                      // Ethernet open: data interface setting 1
  uint16_t filter, sequence;     // Host packet filter, next IN block number
  uint32_t input_size;           // Largest IN block the host accepts
  uint8_t notify;                // Pending link notifications
  struct mg_queue txq;           // Transfer blocks for the host, see tx
  char txq_buf[MG_USB_TX_SIZE];  // Its storage
  uint8_t rx[MG_USB_NTB_SIZE];   // OUT transfer block being received
  size_t rxlen;                  // How much of it arrived
  bool discard;                  // It is too long: drop it
#endif
#if MG_ENABLE_USB_SERIAL
  bool dtr;                   // Terminal open
  uint8_t line[7];            // Serial line coding
  char log[MG_USB_LOG_SIZE];  // Log ring
  unsigned head, tail;        // Log ring write and read counters
  char out[64];               // Log output being sent
#endif
} s_usb;

// Send the next packet of the IN transfer on ep. False if it is over
static bool mgi_usb_in_next(unsigned ep) {
  struct mgi_usb_in *e = &s_usb.in[ep];
  size_t n = e->left > 64 ? 64 : e->left;
  if (e->packets == 0) return false;
  s_usb.hw->in((enum mg_usb_ep) ep, e->data, n);
  e->data += n, e->left -= n, e->packets--;
  return true;
}

// Start an IN transfer. zlp: end a multiple of 64 bytes with an empty packet
static void mgi_usb_in_start(unsigned ep, const void *data, size_t len,
                             bool zlp) {
  struct mgi_usb_in *e = &s_usb.in[ep];
  e->data = (const uint8_t *) data, e->left = len;
  e->packets = (unsigned) (len / 64 + (len % 64 || len == 0 || zlp));
  e->busy = mgi_usb_in_next(ep);
}

// Cancel the transfers on ep
static void mgi_usb_idle(unsigned ep) {
  memset(&s_usb.in[ep], 0, sizeof(s_usb.in[ep]));
#if MG_ENABLE_USB_ETHERNET
  if (ep == MG_USB_EP_ETH_DATA) s_usb.rxlen = 0, s_usb.discard = false;
#endif
}

// Unconfigured, on start, bus reset and SET_CONFIGURATION. Keeps the log
static void mgi_usb_unconfigure(void) {
#if MG_ENABLE_USB_SERIAL
  static const uint8_t line[7] = {0, 0xc2, 1, 0, 0, 0, 8};  // 115200 8N1
  s_usb.dtr = false;
  memcpy(s_usb.line, line, sizeof(line));
#endif
#if MG_ENABLE_USB_ETHERNET
  s_usb.alt = false, s_usb.filter = s_usb.sequence = 0, s_usb.notify = 0;
  s_usb.input_size = MG_USB_NTB_SIZE;
#endif
  s_usb.configured = false;
}

// Build string descriptor i in s_usb.buf. Returns its length, 0 if none
static int mgi_usb_string(unsigned i) {
  static const char *strings[] = {NULL,        "Mongoose",     MGI_USB_PRODUCT,
                                  NULL,        "USB Ethernet", NULL,
                                  "USB Serial"};
  const char *s = i < 7 ? strings[i] : NULL;
  char tmp[33];  // Built strings: product name, host MAC
  uint8_t *b = s_usb.buf;
  size_t k, n;
  if (i == 0) return b[0] = 4, b[1] = 3, b[2] = 9, b[3] = 4;
  if (i == 2) {  // Serial's last 4 digits: tell devices apart
    size_t len = strlen(s_usb.serial);
    mg_snprintf(tmp, sizeof(tmp), "%s_%s", MGI_USB_PRODUCT,
                s_usb.serial + (len > 4 ? len - 4 : 0));
    s = tmp;
  }
  if (i == 3) s = s_usb.serial;
#if MG_ENABLE_USB_ETHERNET
  if (i == 5 && s_usb.ifp != NULL) {  // Host end: our MAC, first byte changed
    uint8_t *m = s_usb.ifp->mac;
    mg_snprintf(tmp, sizeof(tmp), "%02X%02X%02X%02X%02X%02X",
                ((m[0] ^ 4) | 2) & ~1, m[1], m[2], m[3], m[4], m[5]);
    s = tmp;
  }
#endif
  if (s == NULL) return 0;
  n = strlen(s) > 32 ? 32 : strlen(s);
  memset(b, 0, sizeof(s_usb.buf));
  for (k = 0; k < n; k++) b[2 + k * 2] = (uint8_t) s[k];
  b[0] = (uint8_t) (2 + n * 2), b[1] = 3;
  return b[0];
}

// Answer a control request r, its OUT data at r + 8. Returns -1 to stall,
// else the IN reply length in *reply, 0 for status only
static int mgi_usb_request(const uint8_t *r, const void **reply) {
  unsigned value = MG_LOAD_LE16(r + 2), index = MG_LOAD_LE16(r + 4);
  unsigned wlen = MG_LOAD_LE16(r + 6), i;
  const void *d = s_usb.buf;
  int n = 0;
  switch ((r[0] << 8) | r[1]) {
    case 0x8006:  // GET_DESCRIPTOR
      if (value >> 8 == 1) d = s_device_desc, n = sizeof(s_device_desc);
      if (value >> 8 == 2) d = s_config_desc, n = sizeof(s_config_desc);
#if MG_ENABLE_USB_ETHERNET
      if (value >> 8 == 15) d = s_bos_desc, n = sizeof(s_bos_desc);
#endif
      if (value >> 8 == 3) n = mgi_usb_string(value & 255);
      if (n == 0) return -1;
      break;
    case 0x0005:  // SET_ADDRESS, applied by the controller
      if (value > 127) return -1;
      break;
    case 0x0009:  // SET_CONFIGURATION
      if (value > 1) return -1;
      mgi_usb_unconfigure();
      for (i = 1; i < 5; i++) mgi_usb_idle(i);
      s_usb.configured = value == 1;
      break;
    case 0x8008:  // GET_CONFIGURATION
      s_usb.buf[0] = s_usb.configured, n = 1;
      break;
    case 0x010b:  // SET_INTERFACE: only the Ethernet data interface has 1
      if (index >= MGI_USB_NUM_IFS ||
          value > (unsigned) (MG_ENABLE_USB_ETHERNET &&
                              index == MG_USB_IF_ETH_DATA))
        return -1;
#if MG_ENABLE_USB_ETHERNET
      if (index == MG_USB_IF_ETH_DATA) {
        s_usb.alt = value == 1, s_usb.notify = 1;
        // Directed and broadcast by default: Windows may not set a filter
        if (s_usb.alt) s_usb.filter = 0x0c;
        mgi_usb_idle(MG_USB_EP_ETH_DATA);
      }
#endif
      break;
    case 0x810a:  // GET_INTERFACE
      s_usb.buf[0] = 0, n = 1;
#if MG_ENABLE_USB_ETHERNET
      if (index == MG_USB_IF_ETH_DATA) s_usb.buf[0] = s_usb.alt;
#endif
      break;
#if MG_ENABLE_USB_ETHERNET
    case 0xa183:  // GET_NTB_FORMAT: NTB16
#endif
    case 0x8000:  // GET_STATUS: device, interface, endpoint
    case 0x8100:
    case 0x8200:
      s_usb.buf[0] = s_usb.buf[1] = 0, n = 2;
      break;
    case 0x0201:  // CLEAR_FEATURE(ENDPOINT_HALT), applied by the controller
      if (value || index > 255 ||
          !memchr(s_endpoints, (int) index, sizeof(s_endpoints)))
        return -1;
      break;
#if MG_ENABLE_USB_ETHERNET
    case 0xc001:  // Microsoft OS 2.0 descriptor set
      if (index != 7) return -1;
      d = s_ms_os_desc, n = sizeof(s_ms_os_desc);
      break;
    case 0x2143:  // SET_ETHERNET_PACKET_FILTER
      s_usb.filter = (uint16_t) value;
      break;
    case 0xa180:  // GET_NTB_PARAMETERS
      d = s_ntb_params, n = sizeof(s_ntb_params);
      break;
    case 0x2184:  // SET_NTB_FORMAT: NTB16 only
      if (value) return -1;
      break;
    case 0xa185:  // GET_NTB_INPUT_SIZE, one datagram per block
      memset(s_usb.buf, 0, 8);
      MG_STORE_LE32(s_usb.buf, s_usb.input_size);
      s_usb.buf[4] = 1, n = 8;
      break;
    case 0x2186:  // SET_NTB_INPUT_SIZE
      if ((wlen != 4 && wlen != 8) || MG_LOAD_LE32(r + 8) < 2048 ||
          MG_LOAD_LE32(r + 8) > 65535)
        return -1;
      s_usb.input_size = MG_LOAD_LE32(r + 8);
      if (s_usb.input_size > MG_USB_NTB_SIZE)
        s_usb.input_size = MG_USB_NTB_SIZE;
      break;
#endif
#if MG_ENABLE_USB_SERIAL
    case 0xa121:  // GET_LINE_CODING
      d = s_usb.line, n = sizeof(s_usb.line);
      break;
    case 0x2120:  // SET_LINE_CODING
      if (wlen != sizeof(s_usb.line)) return -1;
      memcpy(s_usb.line, r + 8, sizeof(s_usb.line));
      break;
    case 0x2122:  // SET_CONTROL_LINE_STATE
      s_usb.dtr = value & 1;
      break;
    case 0x2123:  // SEND_BREAK
      break;
#endif
    default:
      return -1;
  }
  *reply = d;
  return n > (int) wlen ? (int) wlen : n;
}

#if MG_ENABLE_USB_ETHERNET
// Queue the frames of a received transfer block to the TCP/IP stack.
// Offsets come from the host: bound every datagram by the block, and follow
// at most 16 datagram tables so that a cyclic chain cannot loop forever
static void mgi_usb_decode(const uint8_t *p, size_t len) {
  size_t block, ndp, end, i, ofs, n, tables = 0;
  if (s_usb.ifp == NULL || len < 12 || MG_LOAD_LE32(p) != 0x484d434e) return;
  block = MG_LOAD_LE16(p + 8), ndp = MG_LOAD_LE16(p + 10);
  if (block > len) return;
  while (ndp && tables++ < 16 && ndp + 16 <= block &&
         MG_LOAD_LE32(p + ndp) == 0x304d434e) {  // "NCM0"
    end = ndp + MG_LOAD_LE16(p + ndp + 4);
    for (i = ndp + 8; i + 4 <= end && end <= block; i += 4) {
      ofs = MG_LOAD_LE16(p + i), n = MG_LOAD_LE16(p + i + 2);
      if (ofs == 0 || n == 0) break;
      if (n >= 14 && n <= 1514 && ofs + n <= block)
        mg_tcpip_qwrite((void *) (p + ofs), n, s_usb.ifp);
    }
    ndp = MG_LOAD_LE16(p + ndp + 6);
  }
}

// If the Ethernet IN endpoint is idle, start sending the first queued
// transfer block. It stays queued until sent. Drop blocks the host cannot
// take now: the link closed, or it lowered its block size
static void mgi_usb_tx_next(void) {
  char *block;
  size_t len;
  while (!s_usb.in[MG_USB_EP_ETH_DATA].busy &&
         (len = mg_queue_next(&s_usb.txq, &block)) > 0) {
    if (s_usb.alt && s_usb.filter && len <= s_usb.input_size) {
      mgi_usb_in_start(MG_USB_EP_ETH_DATA, block, len, len < s_usb.input_size);
    } else {
      mg_queue_del(&s_usb.txq, len);
    }
  }
}
#endif

#if MG_ENABLE_USB_SERIAL
// Log output function: keep the latest MG_USB_LOG_SIZE characters
static void mgi_usb_log(char ch, void *arg) {
  if (s_usb.head - s_usb.tail >= MG_USB_LOG_SIZE) s_usb.tail++;
  s_usb.log[s_usb.head++ % MG_USB_LOG_SIZE] = ch;
  (void) arg;
}
#endif

// Controller events, see enum mg_usb_ev
static int mgi_usb_event(enum mg_usb_ev ev, enum mg_usb_ep ep, void *buf,
                         size_t len) {
  const uint8_t *p = (const uint8_t *) buf;
  unsigned i;
  if (ev == MG_USB_EV_RESET) {
    mgi_usb_unconfigure();
    for (i = 0; i < 5; i++) mgi_usb_idle(i);
  } else if (ev == MG_USB_EV_SETUP) {
    const void *reply;
    unsigned wlen = len < 8 ? 0 : MG_LOAD_LE16(p + 6);
    int n;
    mgi_usb_idle(MG_USB_EP_CTL);  // A SETUP ends the previous transfer
    if (len < 8 || (!(p[0] & 0x80) && len < 8 + wlen)) return -1;
    if ((n = mgi_usb_request(p, &reply)) < 0) return -1;
    if (!(p[0] & 0x80) || wlen == 0) return 0;
    mgi_usb_in_start(MG_USB_EP_CTL, reply, (size_t) n, n < (int) wlen);
    return 1;
  } else if (ev == MG_USB_EV_IN && s_usb.in[ep].busy) {
    s_usb.in[ep].busy = mgi_usb_in_next(ep);
#if MG_ENABLE_USB_ETHERNET
    if (ep == MG_USB_EP_ETH_DATA && !s_usb.in[ep].busy) {
      mg_queue_del(&s_usb.txq, mg_queue_next(&s_usb.txq, NULL));  // Sent
      mgi_usb_tx_next();
    }
#endif
    return s_usb.in[ep].busy;
#if MG_ENABLE_USB_ETHERNET
  } else if (ev == MG_USB_EV_OUT && ep == MG_USB_EP_ETH_DATA) {
    // Add the packet to the transfer block. At the advertised maximum OUT
    // size, the host need not send a ZLP. Other blocks end with a short
    // packet, possibly empty
    if (!s_usb.discard && s_usb.rxlen + len <= sizeof(s_usb.rx)) {
      memcpy(s_usb.rx + s_usb.rxlen, buf, len);
      s_usb.rxlen += len;
    } else {
      s_usb.discard = true;
    }
    if (len < 64 || s_usb.rxlen == sizeof(s_usb.rx)) {
      if (!s_usb.discard) mgi_usb_decode(s_usb.rx, s_usb.rxlen);
      s_usb.rxlen = 0, s_usb.discard = false;
    }
#endif
  }
  return 0;
}

bool mg_usb_init(struct mg_tcpip_if *ifp, const struct mg_usb_hw *hw) {
  if (s_usb.hw == NULL) {
    s_usb.hw = hw;
    mgi_usb_unconfigure();
#if MG_ENABLE_USB_ETHERNET
    mg_queue_init(&s_usb.txq, s_usb.txq_buf, sizeof(s_usb.txq_buf));
#endif
#if MG_ENABLE_USB_SERIAL
    mg_log_set_fn(mgi_usb_log, NULL);  // Log to the USB serial port
#endif
    hw->init(s_usb.serial);
  }
#if MG_ENABLE_USB_ETHERNET
  if (ifp != NULL) s_usb.ifp = ifp;
#else
  (void) ifp;
#endif
  return true;
}

size_t mg_usb_tx(const void *buf, size_t len, struct mg_tcpip_if *ifp) {
#if MG_ENABLE_USB_ETHERNET
  // Queue the frame wrapped in its transfer block, ready to send
  size_t n = sizeof(s_nth) + len;
  char *p;
  if (!s_usb.alt || !s_usb.filter || len < 14 || len > 1514) return 0;
  if (mg_queue_next(&s_usb.txq, NULL) == 0) {
    mg_queue_init(&s_usb.txq, s_usb.txq.buf, s_usb.txq.size);  // Unwrap
  }
  if (mg_queue_book(&s_usb.txq, &p, n) < n) {
    ifp->nerr++;  // Queue full: the host is not reading
    return 0;
  }
  memcpy(p, s_nth, sizeof(s_nth));
  MG_STORE_LE16(p + 6, s_usb.sequence);
  s_usb.sequence++;
  MG_STORE_LE16(p + 8, n);
  MG_STORE_LE16(p + 22, len);
  memcpy(p + sizeof(s_nth), buf, len);
  mg_queue_add(&s_usb.txq, n);
  mgi_usb_tx_next();
  return len;
#else
  (void) ifp, (void) buf, (void) len;
  return 0;
#endif
}

bool mg_usb_poll(struct mg_tcpip_if *ifp, bool s1) {
  (void) ifp;
  if (s_usb.hw == NULL) return false;  // mg_usb_init() not called yet
  s_usb.hw->poll(mgi_usb_event);
#if MG_ENABLE_USB_SERIAL
  if (s_usb.configured && s_usb.dtr && !s_usb.in[MG_USB_EP_SER_DATA].busy) {
    size_t n = 0;
    while (n < sizeof(s_usb.out) && s_usb.tail != s_usb.head) {
      s_usb.out[n++] = s_usb.log[s_usb.tail++ % MG_USB_LOG_SIZE];
    }
    if (n > 0) mgi_usb_in_start(MG_USB_EP_SER_DATA, s_usb.out, n, true);
  }
#endif
#if MG_ENABLE_USB_ETHERNET
  if (s_usb.notify && !s_usb.in[MG_USB_EP_ETH_EV].busy) {
    // Link notifications: the speed first if the link is up, then the state
    if (s_usb.notify == 1 && s_usb.alt) {
      s_usb.notify = 2;
      mgi_usb_in_start(MG_USB_EP_ETH_EV, s_speed, sizeof(s_speed), false);
    } else {
      s_usb.notify = 0;
      mgi_usb_in_start(MG_USB_EP_ETH_EV, s_usb.alt ? s_up : s_down,
                       sizeof(s_up), false);
    }
  }
  mgi_usb_tx_next();
  return s1 && s_usb.alt && s_usb.filter;
#else
  (void) s1;
  return false;
#endif
}

#endif
