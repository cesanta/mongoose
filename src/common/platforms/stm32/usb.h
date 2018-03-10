/*
 * File:   stlink/usb.h
 * Author: karl
 *
 * Created on October 1, 2011, 11:29 PM
 */

#ifndef STLINK_USB_H
#define STLINK_USB_H

#include <stdbool.h>
#include <libusb.h>

#include "stlink.h"

#ifdef __cplusplus
extern "C" {
#endif

#define STLINK_USB_VID_ST 0x0483
#define STLINK_USB_PID_STLINK 0x3744
#define STLINK_USB_PID_STLINK_32L 0x3748
#define STLINK_USB_PID_STLINK_NUCLEO 0x374b

#define STLINK_SG_SIZE 31
#define STLINK_CMD_SIZE 16

struct stlink_libusb {
  libusb_context *libusb_ctx;
  libusb_device_handle *usb_handle;
  unsigned int ep_req;
  unsigned int ep_rep;
  int protocoll;
  unsigned int sg_transfer_idx;
  unsigned int cmd_len;
};

/**
 * Open a stlink
 * @param verbose Verbosity loglevel
 * @param reset   Reset stlink programmer
 * @param serial  Serial number to search for, when NULL the first stlink found
 * is opened (binary format)
 * @retval NULL   Error while opening the stlink
 * @retval !NULL  Stlink found and ready to use
 */
stlink_t *stlink_open_usb(bool reset, char serial[16]);

void _stlink_usb_close(stlink_t *sl);
int _stlink_usb_version(stlink_t *sl);
int32_t _stlink_usb_target_voltage(stlink_t *sl);
int _stlink_usb_read_debug32(stlink_t *sl, uint32_t addr, uint32_t *data);
int _stlink_usb_write_debug32(stlink_t *sl, uint32_t addr, uint32_t data);
int _stlink_usb_write_mem32(stlink_t *sl, uint32_t addr, uint16_t len);
int _stlink_usb_write_mem8(stlink_t *sl, uint32_t addr, uint16_t len);
int _stlink_usb_current_mode(stlink_t *sl);
int _stlink_usb_core_id(stlink_t *sl);
int _stlink_usb_status(stlink_t *sl);
int _stlink_usb_force_debug(stlink_t *sl);
int _stlink_usb_enter_swd_mode(stlink_t *sl);
int _stlink_usb_exit_dfu_mode(stlink_t *sl);
int _stlink_usb_reset(stlink_t *sl);
int _stlink_usb_jtag_reset(stlink_t *sl, int value);
int _stlink_usb_step(stlink_t *sl);
int _stlink_usb_run(stlink_t *sl);
int _stlink_usb_set_swdclk(stlink_t *sl, uint16_t clk_divisor);
int _stlink_usb_exit_debug_mode(stlink_t *sl);
int _stlink_usb_read_mem32(stlink_t *sl, uint32_t addr, uint16_t len);
int _stlink_usb_read_reg(stlink_t *sl, int r_idx, struct stlink_reg *regp);
int _stlink_usb_write_reg(stlink_t *sl, uint32_t reg, int idx);

#ifdef __cplusplus
}
#endif

#endif /* STLINK_USB_H */
