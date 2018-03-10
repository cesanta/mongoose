#include "stlink.h"

#include <stdio.h>
#include <string.h>
#include <unistd.h>

static const uint8_t loader_code_stm32f7[] = {
    0x08, 0x4b, 0x72, 0xb1, 0x04, 0x68, 0x0c, 0x60,
    0xbf, 0xf3, 0x4f, 0x8f,  // DSB Memory barrier for in order flash write
    0xdc, 0x89, 0x14, 0xf0, 0x01, 0x0f, 0xfb, 0xd1,
    0x00, 0xf1, 0x04, 0x00, 0x01, 0xf1, 0x04, 0x01,
    0xa2, 0xf1, 0x01, 0x02, 0xef, 0xe7, 0x00, 0xbe,  //     bkpt	#0x00
    0x00, 0x3c, 0x02, 0x40,
};

#define WAIT_ROUNDS 10000

int stlink_flash_loader_init(stlink_t *sl, flash_loader_t *fl) {
  size_t size;

  /* allocate the loader in sram */
  if (stlink_flash_loader_write_to_sram(sl, &fl->loader_addr, &size) == -1) {
    printf("Failed to write flash loader to sram!\n");
    return -1;
  }

  /* allocate a one page buffer in sram right after loader */
  fl->buf_addr = fl->loader_addr + (uint32_t) size;

  return 0;
}

int stlink_flash_loader_write_to_sram(stlink_t *sl, stm32_addr_t *addr,
                                      size_t *size) {
  memcpy(sl->q_buf, loader_code_stm32f7, sizeof(loader_code_stm32f7));
  stlink_write_mem32(sl, sl->sram_base, sizeof(loader_code_stm32f7));

  *addr = sl->sram_base;
  *size = sizeof(loader_code_stm32f7);

  /* success */
  return 0;
}

int stlink_flash_loader_run(stlink_t *sl, flash_loader_t *fl,
                            stm32_addr_t target, const uint8_t *buf,
                            size_t size) {
  struct stlink_reg rr;
  int i = 0;
  size_t count = 0;

  // FIXME This can never return -1
  if (write_buffer_to_sram(sl, fl, buf, size) == -1) {
    // IMPOSSIBLE!
    printf("write_buffer_to_sram() == -1\n");
    return -1;
  }

  count = size / sizeof(uint32_t);
  if (size % sizeof(uint32_t)) ++count;

  /* setup core */
  stlink_write_reg(sl, fl->buf_addr, 0);     /* source */
  stlink_write_reg(sl, target, 1);           /* target */
  stlink_write_reg(sl, (uint32_t) count, 2); /* count */
  stlink_write_reg(
      sl, 0,
      3); /* flash bank 0 (input), only used on F0, but armless fopr others */
  stlink_write_reg(sl, fl->loader_addr, 15); /* pc register */

  /* run loader */
  stlink_run(sl);

  /* wait until done (reaches breakpoint) */
  for (i = 0; i < WAIT_ROUNDS; i++) {
    usleep(10);
    if (stlink_is_core_halted(sl)) break;
  }

  if (i >= WAIT_ROUNDS) {
    printf("flash loader run error\n");
    return -1;
  }

  /* check written byte count */
  stlink_read_reg(sl, 2, &rr);
  if (rr.r[2] != 0) {
    printf("write error, count == %u\n", rr.r[2]);
    return -1;
  }

  return 0;
}
