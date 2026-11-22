#include "drivers/usb.h"
#include "net_builtin.h"

#if MG_ENABLE_USB

// Interfaces: NCM control and data 0/1 if enabled, then ACM control and data
#define MG_USB_SERIAL_IF (2 * MG_ENABLE_USB_ETHERNET)
#define MG_USB_NUM_IFS (2 * MG_ENABLE_USB_ETHERNET + 2 * MG_ENABLE_USB_SERIAL)
#define MG_USB_CONFIG_LEN \
  (9 + 85 * MG_ENABLE_USB_ETHERNET + 66 * MG_ENABLE_USB_SERIAL)
#if MG_ENABLE_USB_ETHERNET
#define MG_USB_PRODUCT "MongooseUsbEthernet"
#else
#define MG_USB_PRODUCT "MongooseUsbSerial"
#endif

// 0xdead is our example VID, for development only
static const uint8_t s_device_desc[] = {
    18, 1, 1, 2, 0xef, 2, 1, 64, 0xde, 0xad, 0xe4, 0x40, 0, 1, 1, 2, 3, 1};

// NCM: endpoints 0x81, 0x02/0x82. ACM: endpoints 0x83, 0x04/0x84
static const uint8_t s_config_desc[] = {
    9, 2, MG_USB_CONFIG_LEN, 0, MG_USB_NUM_IFS, 1, 0, 0x80, 50,  // Config
#if MG_ENABLE_USB_ETHERNET
    8,  11,   0,    2,    2,  13,   0,  4,         // NCM association
    9,  4,    0,    0,    1,  2,    13, 0,    4,   // NCM control interface
    5,  0x24, 0,    0x20, 1,                       // CDC header
    5,  0x24, 6,    0,    1,                       // Union
    13, 0x24, 15,   5,    0,  0,    0,  0,    0xea,
    5,  0,    0,    0,                            // Eth, 1514 bytes
    6,  0x24, 0x1a, 0,    1,  0x21,               // NCM 1.0
    7,  5,    0x81, 3,    16, 0,    16,           // Notification endpoint
    9,  4,    1,    0,    0,  10,   0,  1,    0,  // Data interface, off
    9,  4,    1,    1,    2,  10,   0,  1,    0,  // Data interface, on
    7,  5,    0x02, 2,    64, 0,    0,            // Data OUT
    7,  5,    0x82, 2,    64, 0,    0,            // Data IN
#endif
#if MG_ENABLE_USB_SERIAL
    8, 11, MG_USB_SERIAL_IF, 2, 2, 2, 1, 6,          // ACM association
    9, 4, MG_USB_SERIAL_IF, 0, 1, 2, 2, 1, 6,        // ACM control interface
    5, 0x24, 0, 0x20, 1,                             // CDC header
    5, 0x24, 1, 0, MG_USB_SERIAL_IF + 1,             // Call management
    4, 0x24, 2, 2,                                   // ACM
    5, 0x24, 6, MG_USB_SERIAL_IF, MG_USB_SERIAL_IF + 1,  // Union
    7, 5, 0x83, 3, 16, 0, 16,                        // Notification endpoint
    9, 4, MG_USB_SERIAL_IF + 1, 0, 2, 10, 0, 0, 0,   // Data interface
    7, 5, 0x04, 2, 64, 0, 0,                         // Data OUT
    7, 5, 0x84, 2, 64, 0, 0,                         // Data IN
#endif
};

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
static const uint8_t s_ms_os_desc[] = {
    10, 0, 0, 0, 0,   0,   3,   6,   46,  0,    // Set header, Windows 8.1+
    8,  0, 1, 0, 0,   0,   36,  0,              // Configuration subset
    8,  0, 2, 0, 0,   0,   28,  0,              // Function subset, NCM
    20, 0, 3, 0, 'W', 'I', 'N', 'N', 'C', 'M',  // Compatible ID
    0,  0, 0, 0, 0,   0,   0,   0,   0,   0};
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
static const uint8_t s_speed[] = {0xa1, 0x2a, 0,    0, 0, 0,    8,    0,
                                  0,    0x1b, 0xb7, 0, 0, 0x1b, 0xb7, 0};
static const uint8_t s_up[] = {0xa1, 0, 1, 0, 0, 0, 0, 0};
static const uint8_t s_down[] = {0xa1, 0, 0, 0, 0, 0, 0, 0};

// Transfer block header for one frame: NTH16, NDP16 with one datagram at
// offset 30, so that the IP header after the 14-byte Ethernet header lands
// on a 4-byte boundary
static const uint8_t s_nth[30] = {'N', 'C', 'M', 'H', 12,  0,   0,  0, 0, 0,
                                  12,  0,   'N', 'C', 'M', '0', 16, 0, 0, 0,
                                  30,  0,   0,   0,   0,   0,   0,  0, 0, 0};
#endif

// Build string descriptor i in u->buf. Returns its length, 0 if none
static int string_desc(struct mg_usb *u, unsigned i) {
  static const char *strings[] = {
      NULL,         "Mongoose", MG_USB_PRODUCT, NULL,
      "USB Ethernet", NULL,     "USB Serial"};
  const char *s = i < 7 ? strings[i] : NULL;
#if MG_ENABLE_USB_ETHERNET
  char mac[13];
#endif
  size_t k, n;
  if (i == 0) return u->buf[0] = 4, u->buf[1] = 3, u->buf[2] = 9, u->buf[3] = 4;
  if (i == 3) s = u->serial;
#if MG_ENABLE_USB_ETHERNET
  if (i == 5) {  // Host end of the link: the device MAC, first byte changed
    uint8_t *m = u->ifp->mac;
    mg_snprintf(mac, sizeof(mac), "%02X%02X%02X%02X%02X%02X",
                ((m[0] ^ 4) | 2) & ~1, m[1], m[2], m[3], m[4], m[5]);
    s = mac;
  }
#endif
  if (s == NULL) return 0;
  n = strlen(s) > 32 ? 32 : strlen(s);
  memset(u->buf, 0, sizeof(u->buf));
  for (k = 0; k < n; k++) u->buf[2 + k * 2] = (uint8_t) s[k];
  u->buf[0] = (uint8_t) (2 + n * 2), u->buf[1] = 3;
  return u->buf[0];
}

void mg_usb_reset(struct mg_usb *u) {
#if MG_ENABLE_USB_SERIAL
  static const uint8_t line[7] = {0, 0xc2, 1, 0, 0, 0, 8};  // 115200 8N1
  u->dtr = false;
  memcpy(u->line, line, sizeof(line));
#endif
#if MG_ENABLE_USB_ETHERNET
  u->alt = false, u->filter = u->sequence = 0, u->notify = 0;
  u->input_size = MG_USB_NTB_SIZE;
#endif
  u->configured = false;
}

int mg_usb_setup(struct mg_usb *u, const uint8_t *r, const void **reply) {
  unsigned value = MG_LOAD_LE16(r + 2), index = MG_LOAD_LE16(r + 4);
  unsigned wlen = MG_LOAD_LE16(r + 6);
  const void *d = u->buf;
  int n = 0;
  switch ((r[0] << 8) | r[1]) {
    case 0x8006:  // GET_DESCRIPTOR
      if (value >> 8 == 1) d = s_device_desc, n = sizeof(s_device_desc);
      if (value >> 8 == 2) d = s_config_desc, n = sizeof(s_config_desc);
#if MG_ENABLE_USB_ETHERNET
      if (value >> 8 == 15) d = s_bos_desc, n = sizeof(s_bos_desc);
#endif
      if (value >> 8 == 3) n = string_desc(u, value & 255);
      if (n == 0) return -1;
      break;
    case 0x0005:  // SET_ADDRESS, applied by the driver after the status
      if (value > 127) return -1;
      break;
    case 0x0009:  // SET_CONFIGURATION
      if (value > 1) return -1;
      mg_usb_reset(u);
      u->configured = value == 1;
      break;
    case 0x8008:  // GET_CONFIGURATION
      u->buf[0] = u->configured, n = 1;
      break;
    case 0x010b:  // SET_INTERFACE: only the Ethernet data interface has 1
      if (index >= MG_USB_NUM_IFS ||
          value > (unsigned) (MG_ENABLE_USB_ETHERNET && index == 1))
        return -1;
#if MG_ENABLE_USB_ETHERNET
      if (index == 1) u->alt = value == 1, u->notify = 1;
#endif
      break;
    case 0x810a:  // GET_INTERFACE
      u->buf[0] = 0, n = 1;
#if MG_ENABLE_USB_ETHERNET
      if (index == 1) u->buf[0] = u->alt;
#endif
      break;
#if MG_ENABLE_USB_ETHERNET
    case 0xa183:  // GET_NTB_FORMAT: NTB16
#endif
    case 0x8000:  // GET_STATUS: device, interface, endpoint
    case 0x8100:
    case 0x8200:
      u->buf[0] = u->buf[1] = 0, n = 2;
      break;
    case 0x0201:  // CLEAR_FEATURE(ENDPOINT_HALT), applied by the driver
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
      u->filter = (uint16_t) value;
      break;
    case 0xa180:  // GET_NTB_PARAMETERS
      d = s_ntb_params, n = sizeof(s_ntb_params);
      break;
    case 0x2184:  // SET_NTB_FORMAT: NTB16 only
      if (value) return -1;
      break;
    case 0xa185:  // GET_NTB_INPUT_SIZE, one datagram per block
      memset(u->buf, 0, 8);
      MG_STORE_LE32(u->buf, u->input_size);
      u->buf[4] = 1, n = 8;
      break;
    case 0x2186:  // SET_NTB_INPUT_SIZE
      if ((wlen != 4 && wlen != 8) || MG_LOAD_LE32(r + 8) < 2048 ||
          MG_LOAD_LE32(r + 8) > 65535)
        return -1;
      u->input_size = MG_LOAD_LE32(r + 8);
      if (u->input_size > MG_USB_NTB_SIZE) u->input_size = MG_USB_NTB_SIZE;
      break;
#endif
#if MG_ENABLE_USB_SERIAL
    case 0xa121:  // GET_LINE_CODING
      d = u->line, n = sizeof(u->line);
      break;
    case 0x2120:  // SET_LINE_CODING
      if (wlen != sizeof(u->line)) return -1;
      memcpy(u->line, r + 8, sizeof(u->line));
      break;
    case 0x2122:  // SET_CONTROL_LINE_STATE. Closed at 1200 baud: bootloader,
      u->dtr = value & 1;  // after a delay that lets this request complete
      if (!u->dtr && MG_LOAD_LE32(u->line) == 1200)
        u->reboot = mg_millis() + 100;
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
size_t mg_usb_notify(struct mg_usb *u, const void **msg) {
  if (!u->notify) return 0;
  if (u->notify == 1 && u->alt) return u->notify = 2, *msg = s_speed, 16;
  u->notify = 0, *msg = u->alt ? s_up : s_down;
  return 8;
}

size_t mg_usb_encode(struct mg_usb *u, uint8_t *p, const void *buf,
                     size_t len) {
  if (!u->alt || !u->filter || len < 14 || len > 1514 ||
      len + 30 > u->input_size)
    return 0;
  memcpy(p, s_nth, sizeof(s_nth));
  MG_STORE_LE16(p + 6, u->sequence);
  MG_STORE_LE16(p + 8, len + 30);
  MG_STORE_LE16(p + 22, len);
  memcpy(p + 30, buf, len);
  u->sequence++;
  return len + 30;
}

// Offsets come from the host: bound every datagram by the block, and follow
// at most 16 datagram tables so that a cyclic chain cannot loop forever
void mg_usb_decode(struct mg_usb *u, const uint8_t *p, size_t len) {
  size_t block, ndp, end, i, ofs, n, tables = 0;
  if (len < 12 || MG_LOAD_LE32(p) != 0x484d434e) return;  // "NCMH"
  block = MG_LOAD_LE16(p + 8), ndp = MG_LOAD_LE16(p + 10);
  if (block > len) return;
  while (ndp && tables++ < 16 && ndp + 16 <= block &&
         MG_LOAD_LE32(p + ndp) == 0x304d434e) {  // "NCM0"
    end = ndp + MG_LOAD_LE16(p + ndp + 4);
    for (i = ndp + 8; i + 4 <= end && end <= block; i += 4) {
      ofs = MG_LOAD_LE16(p + i), n = MG_LOAD_LE16(p + i + 2);
      if (ofs == 0 || n == 0) break;
      if (n >= 14 && n <= 1514 && ofs + n <= block)
        mg_tcpip_qwrite((void *) (p + ofs), n, u->ifp);
    }
    ndp = MG_LOAD_LE16(p + ndp + 6);
  }
}
#endif

#if MG_ENABLE_USB_SERIAL
void mg_usb_log(char ch, void *arg) {
  struct mg_usb *u = (struct mg_usb *) arg;
  if (u->head - u->tail >= MG_USB_LOG_SIZE) u->tail++;
  u->log[u->head++ % MG_USB_LOG_SIZE] = ch;
}

size_t mg_usb_log_read(struct mg_usb *u, void *buf, size_t len) {
  size_t n = 0;
  while (n < len && u->tail != u->head) {
    ((char *) buf)[n++] = u->log[u->tail++ % MG_USB_LOG_SIZE];
  }
  return n;
}
#endif

#endif
