/* simple wrapper around the stlink_flash_write function */

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#include "stlink.h"
#include "usb.h"

#define FLASH_ADDRESS 0x8000000

int stm32_flash(const char *device_name, void *data, int len) {
  stlink_t *sl = NULL;
  int err = -1;
  uint8_t serial[16];

  int j = (int) strlen(device_name);
  int length = j / 2;  // the length of the destination-array
  if (j % 2 != 0) return -1;

  for (size_t k = 0; j >= 0 && k < sizeof(serial); ++k, j -= 2) {
    char buffer[3] = {0};
    memcpy(buffer, device_name + j, 2);
    serial[length - k] = (uint8_t) strtol(buffer, NULL, 16);
  }

  sl = stlink_open_usb(1, (char *) serial);

  if (sl == NULL) return -1;

  if (stlink_current_mode(sl) == STLINK_DEV_DFU_MODE) {
    if (stlink_exit_dfu_mode(sl)) {
      printf("Failed to exit DFU mode\n");
      goto on_error;
    }
  }

  if (stlink_current_mode(sl) != STLINK_DEV_DEBUG_MODE) {
    if (stlink_enter_swd_mode(sl)) {
      printf("Failed to enter SWD mode\n");
      goto on_error;
    }
  }

  if (stlink_jtag_reset(sl, 2)) {
    printf("Failed to reset JTAG\n");
    goto on_error;
  }

  if (stlink_reset(sl)) {
    printf("Failed to reset device\n");
    goto on_error;
  }

  // Core must be halted to use RAM based flashloaders
  if (stlink_force_debug(sl)) {
    printf("Failed to halt the core\n");
    goto on_error;
  }

  if (stlink_status(sl)) {
    printf("Failed to get Core's status\n");
    goto on_error;
  }

  size_t size = 0;

  if ((FLASH_ADDRESS > sl->flash_base + sl->flash_size)) {
    printf("Unknown memory region\n");
    goto on_error;
  }

  err = stlink_write_flash(sl, FLASH_ADDRESS, (uint8_t *) data, len, 0);
  stlink_fwrite_finalize(sl, FLASH_ADDRESS);

  if (err == -1) {
    printf("stlink_fwrite_flash() == -1\n");
    goto on_error;
  }

  stlink_jtag_reset(sl, 2);
  stlink_reset(sl);

  err = 0;

on_error:
  stlink_exit_debug_mode(sl);
  stlink_close(sl);

  return err;
}
