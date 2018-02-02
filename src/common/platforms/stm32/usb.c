#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <sys/time.h>
#include <sys/types.h>
#include <libusb.h>
#include <errno.h>
#include <unistd.h>

#include "stlink.h"

enum SCSI_Generic_Direction { SG_DXFER_TO_DEV = 0, SG_DXFER_FROM_DEV = 0x80 };

enum stlink_debug_commands {
  STLINK_DEBUG_ENTER_JTAG = 0x00,
  STLINK_DEBUG_GETSTATUS = 0x01,
  STLINK_DEBUG_FORCEDEBUG = 0x02,
  STLINK_DEBUG_RESETSYS = 0x03,
  STLINK_DEBUG_READALLREGS = 0x04,
  STLINK_DEBUG_READREG = 0x05,
  STLINK_DEBUG_WRITEREG = 0x06,
  STLINK_DEBUG_READMEM_32BIT = 0x07,
  STLINK_DEBUG_WRITEMEM_32BIT = 0x08,
  STLINK_DEBUG_RUNCORE = 0x09,
  STLINK_DEBUG_STEPCORE = 0x0a,
  STLINK_DEBUG_SETFP = 0x0b,
  STLINK_DEBUG_WRITEMEM_8BIT = 0x0d,
  STLINK_DEBUG_CLEARFP = 0x0e,
  STLINK_DEBUG_WRITEDEBUGREG = 0x0f,
  STLINK_DEBUG_ENTER = 0x20,
  STLINK_DEBUG_EXIT = 0x21,
  STLINK_DEBUG_READCOREID = 0x22,
  STLINK_DEBUG_ENTER_SWD = 0xa3
};

void _stlink_usb_close(stlink_t *sl) {
  if (!sl) return;

  struct stlink_libusb *const handle = sl->backend_data;
  // maybe we couldn't even get the usb device?
  if (handle != NULL) {
    if (handle->usb_handle != NULL) {
      libusb_close(handle->usb_handle);
    }

    libusb_exit(handle->libusb_ctx);
    free(handle);
  }
}

ssize_t send_recv(struct stlink_libusb *handle, int terminate,
                  unsigned char *txbuf, size_t txsize, unsigned char *rxbuf,
                  size_t rxsize) {
  /* note: txbuf and rxbuf can point to the same area */
  int res = 0;
  int t;

  t = libusb_bulk_transfer(handle->usb_handle, handle->ep_req, txbuf,
                           (int) txsize, &res, 3000);
  if (t) {
    printf("[!] send_recv send request failed: %s\n", libusb_error_name(t));
    return -1;
  } else if ((size_t) res != txsize) {
    printf("[!] send_recv send request wrote %u bytes (instead of %u).\n",
           (unsigned int) res, (unsigned int) txsize);
  }

  if (rxsize != 0) {
    t = libusb_bulk_transfer(handle->usb_handle, handle->ep_rep, rxbuf,
                             (int) rxsize, &res, 3000);
    if (t) {
      printf("[!] send_recv read reply failed: %s\n", libusb_error_name(t));
      return -1;
    }
  }

  if ((handle->protocoll == 1) && terminate) {
    /* Read the SG reply */
    unsigned char sg_buf[13];
    t = libusb_bulk_transfer(handle->usb_handle, handle->ep_rep, sg_buf, 13,
                             &res, 3000);
    if (t) {
      printf("[!] send_recv read storage failed: %s\n", libusb_error_name(t));
      return -1;
    }
    /* The STLink doesn't seem to evaluate the sequence number */
    handle->sg_transfer_idx++;
  }

  return res;
}

static inline int send_only(struct stlink_libusb *handle, int terminate,
                            unsigned char *txbuf, size_t txsize) {
  return (int) send_recv(handle, terminate, txbuf, txsize, NULL, 0);
}

static int fill_command(stlink_t *sl, enum SCSI_Generic_Direction dir,
                        uint32_t len) {
  struct stlink_libusb *const slu = sl->backend_data;
  unsigned char *const cmd = sl->c_buf;
  int i = 0;
  memset(cmd, 0, sizeof(sl->c_buf));
  if (slu->protocoll == 1) {
    cmd[i++] = 'U';
    cmd[i++] = 'S';
    cmd[i++] = 'B';
    cmd[i++] = 'C';
    write_uint32(&cmd[i], slu->sg_transfer_idx);
    write_uint32(&cmd[i + 4], len);
    i += 8;
    cmd[i++] = (dir == SG_DXFER_FROM_DEV) ? 0x80 : 0;
    cmd[i++] = 0;   /* Logical unit */
    cmd[i++] = 0xa; /* Command length */
  }
  return i;
}

int _stlink_usb_version(stlink_t *sl) {
  struct stlink_libusb *const slu = sl->backend_data;
  unsigned char *const data = sl->q_buf;
  unsigned char *const cmd = sl->c_buf;
  ssize_t size;
  uint32_t rep_len = 6;
  int i = fill_command(sl, SG_DXFER_FROM_DEV, rep_len);

  cmd[i++] = STLINK_GET_VERSION;

  size = send_recv(slu, 1, cmd, slu->cmd_len, data, rep_len);
  if (size == -1) {
    printf("[!] send_recv STLINK_GET_VERSION\n");
    return (int) size;
  }

  return 0;
}

int32_t _stlink_usb_target_voltage(stlink_t *sl) {
  struct stlink_libusb *const slu = sl->backend_data;
  unsigned char *const rdata = sl->q_buf;
  unsigned char *const cmd = sl->c_buf;
  ssize_t size;
  uint32_t rep_len = 8;
  int i = fill_command(sl, SG_DXFER_FROM_DEV, rep_len);
  uint32_t factor, reading;
  int voltage;

  cmd[i++] = STLINK_GET_TARGET_VOLTAGE;

  size = send_recv(slu, 1, cmd, slu->cmd_len, rdata, rep_len);
  if (size == -1) {
    printf("[!] send_recv STLINK_GET_TARGET_VOLTAGE\n");
    return -1;
  } else if (size != 8) {
    printf("[!] wrong length STLINK_GET_TARGET_VOLTAGE\n");
    return -1;
  }

  factor =
      (rdata[3] << 24) | (rdata[2] << 16) | (rdata[1] << 8) | (rdata[0] << 0);
  reading =
      (rdata[7] << 24) | (rdata[6] << 16) | (rdata[5] << 8) | (rdata[4] << 0);
  voltage = 2400 * reading / factor;

  return voltage;
}

int _stlink_usb_read_debug32(stlink_t *sl, uint32_t addr, uint32_t *data) {
  struct stlink_libusb *const slu = sl->backend_data;
  unsigned char *const rdata = sl->q_buf;
  unsigned char *const cmd = sl->c_buf;
  ssize_t size;
  const int rep_len = 8;

  int i = fill_command(sl, SG_DXFER_FROM_DEV, rep_len);
  cmd[i++] = STLINK_DEBUG_COMMAND;
  cmd[i++] = STLINK_JTAG_READDEBUG_32BIT;
  write_uint32(&cmd[i], addr);
  size = send_recv(slu, 1, cmd, slu->cmd_len, rdata, rep_len);
  if (size == -1) {
    printf("[!] send_recv STLINK_JTAG_READDEBUG_32BIT\n");
    return (int) size;
  }
  *data = read_uint32(rdata, 4);
  return 0;
}

int _stlink_usb_write_debug32(stlink_t *sl, uint32_t addr, uint32_t data) {
  struct stlink_libusb *const slu = sl->backend_data;
  unsigned char *const rdata = sl->q_buf;
  unsigned char *const cmd = sl->c_buf;
  ssize_t size;
  const int rep_len = 2;

  int i = fill_command(sl, SG_DXFER_FROM_DEV, rep_len);
  cmd[i++] = STLINK_DEBUG_COMMAND;
  cmd[i++] = STLINK_JTAG_WRITEDEBUG_32BIT;
  write_uint32(&cmd[i], addr);
  write_uint32(&cmd[i + 4], data);
  size = send_recv(slu, 1, cmd, slu->cmd_len, rdata, rep_len);
  if (size == -1) {
    printf("[!] send_recv STLINK_JTAG_WRITEDEBUG_32BIT\n");
    return (int) size;
  }

  return 0;
}

int _stlink_usb_write_mem32(stlink_t *sl, uint32_t addr, uint16_t len) {
  struct stlink_libusb *const slu = sl->backend_data;
  unsigned char *const data = sl->q_buf;
  unsigned char *const cmd = sl->c_buf;
  int i, ret;

  i = fill_command(sl, SG_DXFER_TO_DEV, len);
  cmd[i++] = STLINK_DEBUG_COMMAND;
  cmd[i++] = STLINK_DEBUG_WRITEMEM_32BIT;
  write_uint32(&cmd[i], addr);
  write_uint16(&cmd[i + 4], len);
  ret = send_only(slu, 0, cmd, slu->cmd_len);
  if (ret == -1) return ret;

  ret = send_only(slu, 1, data, len);
  if (ret == -1) return ret;

  return 0;
}

int _stlink_usb_write_mem8(stlink_t *sl, uint32_t addr, uint16_t len) {
  struct stlink_libusb *const slu = sl->backend_data;
  unsigned char *const data = sl->q_buf;
  unsigned char *const cmd = sl->c_buf;
  int i, ret;

  i = fill_command(sl, SG_DXFER_TO_DEV, 0);
  cmd[i++] = STLINK_DEBUG_COMMAND;
  cmd[i++] = STLINK_DEBUG_WRITEMEM_8BIT;
  write_uint32(&cmd[i], addr);
  write_uint16(&cmd[i + 4], len);
  ret = send_only(slu, 0, cmd, slu->cmd_len);
  if (ret == -1) return ret;

  ret = send_only(slu, 1, data, len);
  if (ret == -1) return ret;

  return 0;
}

int _stlink_usb_current_mode(stlink_t *sl) {
  struct stlink_libusb *const slu = sl->backend_data;
  unsigned char *const cmd = sl->c_buf;
  unsigned char *const data = sl->q_buf;
  ssize_t size;
  int rep_len = 2;
  int i = fill_command(sl, SG_DXFER_FROM_DEV, rep_len);

  cmd[i++] = STLINK_GET_CURRENT_MODE;
  size = send_recv(slu, 1, cmd, slu->cmd_len, data, rep_len);
  if (size == -1) {
    printf("[!] send_recv STLINK_GET_CURRENT_MODE\n");
    return -1;
  }
  return sl->q_buf[0];
}

int _stlink_usb_core_id(stlink_t *sl) {
  struct stlink_libusb *const slu = sl->backend_data;
  unsigned char *const cmd = sl->c_buf;
  unsigned char *const data = sl->q_buf;
  ssize_t size;
  int rep_len = 4;
  int i = fill_command(sl, SG_DXFER_FROM_DEV, rep_len);

  cmd[i++] = STLINK_DEBUG_COMMAND;
  cmd[i++] = STLINK_DEBUG_READCOREID;

  size = send_recv(slu, 1, cmd, slu->cmd_len, data, rep_len);
  if (size == -1) {
    printf("[!] send_recv STLINK_DEBUG_READCOREID\n");
    return -1;
  }

  sl->core_id = read_uint32(data, 0);
  return 0;
}

int _stlink_usb_status(stlink_t *sl) {
  struct stlink_libusb *const slu = sl->backend_data;
  unsigned char *const data = sl->q_buf;
  unsigned char *const cmd = sl->c_buf;
  ssize_t size;
  int rep_len = 2;
  int i = fill_command(sl, SG_DXFER_FROM_DEV, rep_len);

  cmd[i++] = STLINK_DEBUG_COMMAND;
  cmd[i++] = STLINK_DEBUG_GETSTATUS;

  size = send_recv(slu, 1, cmd, slu->cmd_len, data, rep_len);
  if (size == -1) {
    printf("[!] send_recv STLINK_DEBUG_GETSTATUS\n");
    return (int) size;
  }
  sl->q_len = (int) size;

  return 0;
}

int _stlink_usb_force_debug(stlink_t *sl) {
  struct stlink_libusb *slu = sl->backend_data;
  unsigned char *const data = sl->q_buf;
  unsigned char *const cmd = sl->c_buf;
  ssize_t size;
  int rep_len = 2;
  int i = fill_command(sl, SG_DXFER_FROM_DEV, rep_len);

  cmd[i++] = STLINK_DEBUG_COMMAND;
  cmd[i++] = STLINK_DEBUG_FORCEDEBUG;
  size = send_recv(slu, 1, cmd, slu->cmd_len, data, rep_len);
  if (size == -1) {
    printf("[!] send_recv STLINK_DEBUG_FORCEDEBUG\n");
    return (int) size;
  }

  return 0;
}

int _stlink_usb_enter_swd_mode(stlink_t *sl) {
  struct stlink_libusb *const slu = sl->backend_data;
  unsigned char *const cmd = sl->c_buf;
  ssize_t size;
  const int rep_len = 0;
  int i = fill_command(sl, SG_DXFER_FROM_DEV, rep_len);

  cmd[i++] = STLINK_DEBUG_COMMAND;
  cmd[i++] = STLINK_DEBUG_ENTER;
  cmd[i++] = STLINK_DEBUG_ENTER_SWD;

  size = send_only(slu, 1, cmd, slu->cmd_len);
  if (size == -1) {
    printf("[!] send_recv STLINK_DEBUG_ENTER\n");
    return (int) size;
  }

  return 0;
}

int _stlink_usb_exit_dfu_mode(stlink_t *sl) {
  struct stlink_libusb *const slu = sl->backend_data;
  unsigned char *const cmd = sl->c_buf;
  ssize_t size;
  int i = fill_command(sl, SG_DXFER_FROM_DEV, 0);

  cmd[i++] = STLINK_DFU_COMMAND;
  cmd[i++] = STLINK_DFU_EXIT;

  size = send_only(slu, 1, cmd, slu->cmd_len);
  if (size == -1) {
    printf("[!] send_recv STLINK_DFU_EXIT\n");
    return (int) size;
  }

  return 0;
}

/**
 * TODO - not convinced this does anything...
 * @param sl
 */
int _stlink_usb_reset(stlink_t *sl) {
  struct stlink_libusb *const slu = sl->backend_data;
  unsigned char *const data = sl->q_buf;
  unsigned char *const cmd = sl->c_buf;
  ssize_t size;
  int rep_len = 2;
  int i = fill_command(sl, SG_DXFER_FROM_DEV, rep_len);

  cmd[i++] = STLINK_DEBUG_COMMAND;
  cmd[i++] = STLINK_DEBUG_RESETSYS;

  size = send_recv(slu, 1, cmd, slu->cmd_len, data, rep_len);
  if (size == -1) {
    printf("[!] send_recv STLINK_DEBUG_RESETSYS\n");
    return (int) size;
  }

  return 0;
}

int _stlink_usb_jtag_reset(stlink_t *sl, int value) {
  struct stlink_libusb *const slu = sl->backend_data;
  unsigned char *const data = sl->q_buf;
  unsigned char *const cmd = sl->c_buf;
  ssize_t size;
  int rep_len = 2;
  int i = fill_command(sl, SG_DXFER_FROM_DEV, rep_len);

  cmd[i++] = STLINK_DEBUG_COMMAND;
  cmd[i++] = STLINK_JTAG_DRIVE_NRST;
  cmd[i++] = value;

  size = send_recv(slu, 1, cmd, slu->cmd_len, data, rep_len);
  if (size == -1) {
    printf("[!] send_recv STLINK_JTAG_DRIVE_NRST\n");
    return (int) size;
  }

  return 0;
}

int _stlink_usb_step(stlink_t *sl) {
  struct stlink_libusb *const slu = sl->backend_data;
  unsigned char *const data = sl->q_buf;
  unsigned char *const cmd = sl->c_buf;
  ssize_t size;
  int rep_len = 2;
  int i = fill_command(sl, SG_DXFER_FROM_DEV, rep_len);

  cmd[i++] = STLINK_DEBUG_COMMAND;
  cmd[i++] = STLINK_DEBUG_STEPCORE;

  size = send_recv(slu, 1, cmd, slu->cmd_len, data, rep_len);
  if (size == -1) {
    printf("[!] send_recv STLINK_DEBUG_STEPCORE\n");
    return (int) size;
  }

  return 0;
}

/**
 * This seems to do a good job of restarting things from the beginning?
 * @param sl
 */
int _stlink_usb_run(stlink_t *sl) {
  struct stlink_libusb *const slu = sl->backend_data;
  unsigned char *const data = sl->q_buf;
  unsigned char *const cmd = sl->c_buf;
  ssize_t size;
  int rep_len = 2;
  int i = fill_command(sl, SG_DXFER_FROM_DEV, rep_len);

  cmd[i++] = STLINK_DEBUG_COMMAND;
  cmd[i++] = STLINK_DEBUG_RUNCORE;

  size = send_recv(slu, 1, cmd, slu->cmd_len, data, rep_len);
  if (size == -1) {
    printf("[!] send_recv STLINK_DEBUG_RUNCORE\n");
    return (int) size;
  }

  return 0;
}

int _stlink_usb_set_swdclk(stlink_t *sl, uint16_t clk_divisor) {
  struct stlink_libusb *const slu = sl->backend_data;
  unsigned char *const data = sl->q_buf;
  unsigned char *const cmd = sl->c_buf;
  ssize_t size;
  int rep_len = 2;
  int i;

  // clock speed only supported by stlink/v2 and for firmware >= 22
  if (sl->version.stlink_v >= 2 && sl->version.jtag_v >= 22) {
    i = fill_command(sl, SG_DXFER_FROM_DEV, rep_len);

    cmd[i++] = STLINK_DEBUG_COMMAND;
    cmd[i++] = STLINK_DEBUG_APIV2_SWD_SET_FREQ;
    cmd[i++] = clk_divisor & 0xFF;
    cmd[i++] = (clk_divisor >> 8) & 0xFF;

    size = send_recv(slu, 1, cmd, slu->cmd_len, data, rep_len);
    if (size == -1) {
      printf("[!] send_recv STLINK_DEBUG_APIV2_SWD_SET_FREQ\n");
      return (int) size;
    }

    return 0;
  } else {
    return -1;
  }
}

int _stlink_usb_exit_debug_mode(stlink_t *sl) {
  struct stlink_libusb *const slu = sl->backend_data;
  unsigned char *const cmd = sl->c_buf;
  ssize_t size;
  int i = fill_command(sl, SG_DXFER_FROM_DEV, 0);

  cmd[i++] = STLINK_DEBUG_COMMAND;
  cmd[i++] = STLINK_DEBUG_EXIT;

  size = send_only(slu, 1, cmd, slu->cmd_len);
  if (size == -1) {
    printf("[!] send_only STLINK_DEBUG_EXIT\n");
    return (int) size;
  }

  return 0;
}

int _stlink_usb_read_mem32(stlink_t *sl, uint32_t addr, uint16_t len) {
  struct stlink_libusb *const slu = sl->backend_data;
  unsigned char *const data = sl->q_buf;
  unsigned char *const cmd = sl->c_buf;
  ssize_t size;
  int i = fill_command(sl, SG_DXFER_FROM_DEV, len);

  cmd[i++] = STLINK_DEBUG_COMMAND;
  cmd[i++] = STLINK_DEBUG_READMEM_32BIT;
  write_uint32(&cmd[i], addr);
  write_uint16(&cmd[i + 4], len);

  size = send_recv(slu, 1, cmd, slu->cmd_len, data, len);
  if (size == -1) {
    printf("[!] send_recv STLINK_DEBUG_READMEM_32BIT\n");
    return (int) size;
  }

  sl->q_len = (int) size;

  return 0;
}

int _stlink_usb_read_reg(stlink_t *sl, int r_idx, struct stlink_reg *regp) {
  struct stlink_libusb *const slu = sl->backend_data;
  unsigned char *const data = sl->q_buf;
  unsigned char *const cmd = sl->c_buf;
  ssize_t size;
  uint32_t r;
  uint32_t rep_len = 4;
  int i = fill_command(sl, SG_DXFER_FROM_DEV, rep_len);

  cmd[i++] = STLINK_DEBUG_COMMAND;
  cmd[i++] = STLINK_DEBUG_READREG;
  cmd[i++] = (uint8_t) r_idx;
  size = send_recv(slu, 1, cmd, slu->cmd_len, data, rep_len);
  if (size == -1) {
    printf("[!] send_recv STLINK_DEBUG_READREG\n");
    return (int) size;
  }
  sl->q_len = (int) size;
  r = read_uint32(sl->q_buf, 0);

  switch (r_idx) {
    case 16:
      regp->xpsr = r;
      break;
    case 17:
      regp->main_sp = r;
      break;
    case 18:
      regp->process_sp = r;
      break;
    case 19:
      regp->rw = r; /* XXX ?(primask, basemask etc.) */
      break;
    case 20:
      regp->rw2 = r; /* XXX ?(primask, basemask etc.) */
      break;
    default:
      regp->r[r_idx] = r;
  }

  return 0;
}

int _stlink_usb_write_reg(stlink_t *sl, uint32_t reg, int idx) {
  struct stlink_libusb *const slu = sl->backend_data;
  unsigned char *const data = sl->q_buf;
  unsigned char *const cmd = sl->c_buf;
  ssize_t size;
  uint32_t rep_len = 2;
  int i = fill_command(sl, SG_DXFER_FROM_DEV, rep_len);

  cmd[i++] = STLINK_DEBUG_COMMAND;
  cmd[i++] = STLINK_DEBUG_WRITEREG;
  cmd[i++] = idx;
  write_uint32(&cmd[i], reg);
  size = send_recv(slu, 1, cmd, slu->cmd_len, data, rep_len);
  if (size == -1) {
    printf("[!] send_recv STLINK_DEBUG_WRITEREG\n");
    return (int) size;
  }
  sl->q_len = (int) size;

  return 0;
}

stlink_t *stlink_open_usb(bool reset, char serial[16]) {
  stlink_t *sl = NULL;
  struct stlink_libusb *slu = NULL;
  int ret = -1;
  int config;

  sl = calloc(1, sizeof(stlink_t));
  slu = calloc(1, sizeof(struct stlink_libusb));
  if (sl == NULL) goto on_malloc_error;
  if (slu == NULL) goto on_malloc_error;

  sl->backend_data = slu;

  sl->core_stat = STLINK_CORE_STAT_UNKNOWN;
  if (libusb_init(&(slu->libusb_ctx))) {
    printf("failed to init libusb context, wrong version of libraries?\n");
    goto on_error;
  }

  libusb_device **list;
  /** @todo We should use ssize_t and use it as a counter if > 0. As per libusb
   * API: ssize_t libusb_get_device_list (libusb_context *ctx, libusb_device
   * ***list) */
  int cnt = (int) libusb_get_device_list(slu->libusb_ctx, &list);
  struct libusb_device_descriptor desc;
  int devBus = 0;
  int devAddr = 0;

  /* @TODO: Reading a environment variable in a usb open function is not very
    nice, this
    should be refactored and moved into the CLI tools, and instead of giving
    USB_BUS:USB_ADDR a real stlink
    serial string should be passed to this function. Probably people are using
    this but this is very odd because
    as programmer can change to multiple busses and it is better to detect them
    based on serial.  */
  char *device = getenv("STLINK_DEVICE");
  if (device) {
    char *c = strchr(device, ':');
    if (c == NULL) {
      printf("STLINK_DEVICE must be <USB_BUS>:<USB_ADDR> format\n");
      goto on_error;
    }
    devBus = atoi(device);
    *c++ = 0;
    devAddr = atoi(c);
    printf("bus %03d dev %03d\n", devBus, devAddr);
  }

  while (cnt--) {
    libusb_get_device_descriptor(list[cnt], &desc);
    if (desc.idVendor != STLINK_USB_VID_ST) continue;

    if (devBus && devAddr) {
      if ((libusb_get_bus_number(list[cnt]) != devBus) ||
          (libusb_get_device_address(list[cnt]) != devAddr)) {
        continue;
      }
    }

    if ((desc.idProduct == STLINK_USB_PID_STLINK_32L) ||
        (desc.idProduct == STLINK_USB_PID_STLINK_NUCLEO)) {
      struct libusb_device_handle *handle;

      ret = libusb_open(list[cnt], &handle);
      if (ret) continue;

      sl->serial_size = libusb_get_string_descriptor_ascii(
          handle, desc.iSerialNumber, (unsigned char *) sl->serial,
          sizeof(sl->serial));
      libusb_close(handle);

      if ((serial == NULL) || (*serial == 0)) break;

      if (sl->serial_size < 0) continue;

      if (memcmp(serial, &sl->serial, sl->serial_size) == 0) break;

      continue;
    }

    if (desc.idProduct == STLINK_USB_PID_STLINK) {
      slu->protocoll = 1;
      break;
    }
  }

  if (cnt < 0) {
    printf("Couldn't find %s ST-Link/V2 devices\n",
           (devBus && devAddr) ? "matched" : "any");
    goto on_error;
  } else {
    ret = libusb_open(list[cnt], &slu->usb_handle);
    if (ret != 0) {
      printf("Error %d (%s) opening ST-Link/V2 device %03d:%03d\n", ret,
             strerror(errno), libusb_get_bus_number(list[cnt]),
             libusb_get_device_address(list[cnt]));
      goto on_error;
    }
  }

  libusb_free_device_list(list, 1);

  if (libusb_kernel_driver_active(slu->usb_handle, 0) == 1) {
    ret = libusb_detach_kernel_driver(slu->usb_handle, 0);
    if (ret < 0) {
      printf("libusb_detach_kernel_driver(() error %s\n", strerror(-ret));
      goto on_libusb_error;
    }
  }

  if (libusb_get_configuration(slu->usb_handle, &config)) {
    /* this may fail for a previous configured device */
    printf("libusb_get_configuration()\n");
    goto on_libusb_error;
  }

  if (config != 1) {
    printf("setting new configuration (%d -> 1)\n", config);
    if (libusb_set_configuration(slu->usb_handle, 1)) {
      /* this may fail for a previous configured device */
      printf("libusb_set_configuration() failed\n");
      goto on_libusb_error;
    }
  }

  if (libusb_claim_interface(slu->usb_handle, 0)) {
    printf(
        "Stlink usb device found, but unable to claim (probably already in "
        "use?)\n");
    goto on_libusb_error;
  }

  // TODO - could use the scanning techniq from stm8 code here...
  slu->ep_rep = 1 /* ep rep */ | LIBUSB_ENDPOINT_IN;
  if (desc.idProduct == STLINK_USB_PID_STLINK_NUCLEO) {
    slu->ep_req = 1 /* ep req */ | LIBUSB_ENDPOINT_OUT;
  } else {
    slu->ep_req = 2 /* ep req */ | LIBUSB_ENDPOINT_OUT;
  }

  slu->sg_transfer_idx = 0;
  // TODO - never used at the moment, always CMD_SIZE
  slu->cmd_len = (slu->protocoll == 1) ? STLINK_SG_SIZE : STLINK_CMD_SIZE;

  if (stlink_current_mode(sl) == STLINK_DEV_DFU_MODE) {
    printf("-- exit_dfu_mode\n");
    stlink_exit_dfu_mode(sl);
  }

  if (stlink_current_mode(sl) != STLINK_DEV_DEBUG_MODE) {
    stlink_enter_swd_mode(sl);
  }

  sl->version.stlink_v = 2;

  if (reset) {
    if (sl->version.stlink_v > 1) stlink_jtag_reset(sl, 2);
    stlink_reset(sl);
    usleep(10000);
  }

  ret = stlink_load_device_params(sl);

  // Set the stlink clock speed (default is 1800kHz)
  stlink_set_swdclk(sl, STLINK_SWDCLK_1P8MHZ_DIVISOR);

on_libusb_error:
  if (ret == -1) {
    stlink_close(sl);
    return NULL;
  }

  return sl;

on_error:
  if (slu->libusb_ctx) libusb_exit(slu->libusb_ctx);

on_malloc_error:
  if (sl != NULL) free(sl);
  if (slu != NULL) free(slu);

  return NULL;
}
