/*
 * Copyright (c) 2014-2018 Cesanta Software Limited
 * All rights reserved
 *
 * Licensed under the Apache License, Version 2.0 (the ""License"");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an ""AS IS"" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "stub_flasher.h"

#include <stdint.h>
#include <string.h>

#include "rom_functions.h"

#if defined(ESP8266)
#include "eagle_soc.h"
#include "ets_sys.h"
#include "miniz.c"
#elif defined(ESP32)
#include "rom/efuse.h"
#include "rom/miniz.h"
#include "rom/spi_flash.h"
#include "soc/uart_reg.h"
#include "led.h"
#endif

#include "slip.h"
#include "uart.h"

/* Param: baud rate. */
uint32_t params[2] __attribute__((section(".params")));

#define FLASH_BLOCK_SIZE 65536
#define FLASH_SECTOR_SIZE 4096
#define FLASH_PAGE_SIZE 256

/* These consts should be in sync with flasher_client.go */
#define NUM_BUFS 4
#define BUF_SIZE 4096
#define FLASH_WRITE_SIZE BUF_SIZE

#define UART_RX_INTS (UART_RXFIFO_FULL_INT_ENA | UART_RXFIFO_TOUT_INT_ENA)

extern uint32_t _bss_start, _bss_end;

#ifdef ESP8266
#define REG_SPI_BASE(i) (0x60000200 - i * 0x100)

#define SPI_CMD_REG(i) (REG_SPI_BASE(i) + 0x0)
#define SPI_FLASH_WREN (BIT(30))
#define SPI_FLASH_RDID (BIT(28))
#define SPI_FLASH_SE (BIT(24))
#define SPI_FLASH_BE (BIT(23))

#define SPI_ADDR_REG(i) (REG_SPI_BASE(i) + 0x4)

#define SPI_USER_REG(i) (REG_SPI_BASE(i) + 0x1C)

#define SPI_W0_REG(i) (REG_SPI_BASE(i) + 0x40)
#endif

enum read_state {
  READ_WAIT_START = 0,
  READ_FLAGS,
  READ_DATA,
  READ_UNESCAPE,
  READ_ERROR,
};

struct data_buf {
  uint32_t len;
  uint8_t data[BUF_SIZE];
  uint8_t flags;
};

#define FLAG_COMPRESSED 1

struct uart_buf {
  enum read_state state;
  struct data_buf bufs[NUM_BUFS];
  uint32_t bri, bwi;
  uint32_t ps;
};

static inline uint32_t ccount(void) {
  uint32_t r;
  __asm volatile("rsr.ccount %0" : "=a"(r));
  return r;
}

struct write_progress {
  uint32_t num_written;
  uint32_t buf_level;
  uint8_t digest[16];
};

struct write_result {
  uint32_t wait_time;
  uint32_t decomp_time;
  uint32_t write_time;
  uint32_t erase_time;
  uint32_t total_time;
  uint8_t digest[16];
};

static struct uart_buf ub;
static uint32_t inflate_buf[TINFL_LZ_DICT_SIZE / sizeof(uint32_t)];

static void next_write_buf(void) {
  ub.bwi++;
  if (ub.bwi == NUM_BUFS) ub.bwi = 0;
  ub.bufs[ub.bwi].len = 0;
  ub.bufs[ub.bwi].flags = 0;
}

static void add_byte(uint8_t byte) {
  struct data_buf *buf = &ub.bufs[ub.bwi];
  if (buf->len < BUF_SIZE) {
    buf->data[buf->len++] = byte;
    ub.ps++;
  }
}

void uart_isr(void *arg) {
  uint32_t int_st = READ_PERI_REG(UART_INT_ST_REG(0));
  uint8_t fifo_len, i;
  while ((fifo_len = READ_PERI_REG(UART_STATUS_REG(0))) > 0) {
    for (i = 0; i < fifo_len; i++) {
      uint8_t byte = READ_PERI_REG(UART_FIFO_REG(0));
      switch (ub.state) {
        case READ_WAIT_START: {
          if (byte == 0xc0) {
            ub.state = READ_FLAGS;
            ub.ps = 0;
          }
          break;
        }
        case READ_FLAGS:
        case READ_DATA: {
          struct data_buf *buf = &ub.bufs[ub.bwi];
          if (byte == 0xdb) {
            ub.state = READ_UNESCAPE;
          } else if (byte == 0xc0) {
            next_write_buf();
            if (ub.ps == 0) {
              /* Empty packet, sender aborted. */
              ub.state = READ_ERROR;
              SET_PERI_REG_MASK(UART_INT_ENA_REG(0), 0);
              goto out;
            } else {
              ub.state = READ_WAIT_START;
            }
          } else {
            if (ub.state == READ_FLAGS) {
              buf->flags = byte;
              ub.state = READ_DATA;
            } else {
              add_byte(byte);
            }
          }
          break;
        }
        case READ_UNESCAPE: {
          if (byte == 0xdc) {
            byte = 0xc0;
          } else if (byte == 0xdd) {
            byte = 0xdb;
          } else {
            ub.state = READ_ERROR;
            SET_PERI_REG_MASK(UART_INT_ENA_REG(0), 0);
            goto out;
          }
          add_byte(byte);
          ub.state = READ_DATA;
          break;
        }
        case READ_ERROR: {
          goto out;
        }
      }
    }
  }
out:
  WRITE_PERI_REG(UART_INT_CLR_REG(0), int_st);
  (void) arg;
}

size_t tinfl_decompress_mem_to_mem(void *pOut_buf, size_t out_buf_len,
                                   const void *pSrc_buf, size_t src_buf_len,
                                   int flags);
#define TINFL_DECOMPRESS_MEM_TO_MEM_FAILED ((size_t)(-1))
#define TINFL_FLAG_PARSE_ZLIB_HEADER 1

#if defined(ESP8266)
int esp_rom_spiflash_erase_start(uint32_t addr, uint32_t cmd) {
  SPI_write_enable(flashchip);
  WRITE_PERI_REG(SPI_ADDR_REG(0), addr);
  WRITE_PERI_REG(SPI_CMD_REG(0), cmd);
  while (READ_PERI_REG(SPI_CMD_REG(0)) & cmd) {
  }
  return 0;
}
#elif defined(ESP32)
extern esp_rom_spiflash_chip_t g_rom_spiflash_chip;

esp_rom_spiflash_result_t esp_rom_spiflash_erase_start(uint32_t addr,
                                                       uint32_t cmd) {
  esp_rom_spiflash_chip_t *spi = &g_rom_spiflash_chip;
  esp_rom_spiflash_wait_idle(spi);

  REG_CLR_BIT(PERIPHS_SPI_FLASH_USRREG, SPI_USR_DUMMY);
  REG_SET_FIELD(PERIPHS_SPI_FLASH_USRREG1, SPI_USR_ADDR_BITLEN,
                ESP_ROM_SPIFLASH_W_SIO_ADDR_BITSLEN);

  WRITE_PERI_REG(PERIPHS_SPI_FLASH_CMD, SPI_FLASH_WREN);
  while (READ_PERI_REG(PERIPHS_SPI_FLASH_CMD) != 0) {
  }

  WRITE_PERI_REG(PERIPHS_SPI_FLASH_ADDR, addr);
  WRITE_PERI_REG(PERIPHS_SPI_FLASH_CMD, cmd);

  return ESP_ROM_SPIFLASH_RESULT_OK;
}
#endif

int do_flash_write(uint32_t addr, uint32_t len, uint32_t erase) {
  int ret = 0;
  uint32_t num_erased = 0;
  struct MD5Context ctx;
  MD5Init(&ctx);

  if (addr % FLASH_SECTOR_SIZE != 0) return 0x32;
  if (len % FLASH_SECTOR_SIZE != 0) return 0x33;
  if (esp_rom_spiflash_unlock() != 0) return 0x34;

  memset(&ub, 0, sizeof(ub));
  memset(&inflate_buf, 0, sizeof(inflate_buf));
  ets_isr_attach(ETS_UART0_INUM, uart_isr, &ub);
  uint32_t saved_conf1 = READ_PERI_REG(UART_CONF1_REG(0));
  /* Reduce frequency of UART interrupts */
  WRITE_PERI_REG(UART_CONF1_REG(0), UART_RX_TOUT_EN |
                                        (20 << UART_RX_TOUT_THRHD_S) |
                                        (100 << UART_RXFIFO_FULL_THRHD_S));
  SET_PERI_REG_MASK(UART_INT_ENA_REG(0), UART_RX_INTS);
  ets_isr_unmask(1 << ETS_UART0_INUM);

  struct write_result wr;
  memset(&wr, 0, sizeof(wr));

  struct write_progress wp = {.num_written = 0, .buf_level = 0};
  wp.buf_level = (uint32_t) &addr;
  SLIP_send(&wp, sizeof(wp));
  wr.total_time = ccount();
  while (wp.num_written < len) {
    /* Prepare the space ahead. */
    uint32_t start_count = ccount();
    while (erase && num_erased < wp.num_written + FLASH_WRITE_SIZE) {
      const uint32_t num_left = (len - num_erased);
      if (num_left >= FLASH_BLOCK_SIZE && addr % FLASH_BLOCK_SIZE == 0) {
        if (esp_rom_spiflash_erase_start(addr, SPI_FLASH_BE) != 0) {
          ret = 0x35;
          goto out;
        }
        num_erased += FLASH_BLOCK_SIZE;
      } else {
        /* len % FLASH_SECTOR_SIZE == 0 is enforced, no further checks needed */
        if (esp_rom_spiflash_erase_start(addr, SPI_FLASH_SE) != 0) {
          ret = 0x36;
          goto out;
        }
        num_erased += FLASH_SECTOR_SIZE;
      }
    }
    wr.erase_time += ccount() - start_count;
    start_count = ccount();
    /* Wait for data to arrive. */
    wp.buf_level = 0;
    for (int i = 0; i < NUM_BUFS; i++) wp.buf_level += ub.bufs[i].len;
    volatile uint32_t *bwi = &ub.bwi;
    while (*bwi == ub.bri && ub.state != READ_ERROR) {
    }
    struct data_buf *buf = &ub.bufs[ub.bri];
    if (ub.state == READ_ERROR) {
      ret = 0x37;
      goto out;
    }
    wr.wait_time += ccount() - start_count;
    start_count = ccount();
    uint32_t *data = (uint32_t *) buf->data;
    uint32_t write_len = buf->len;
    if (buf->flags & FLAG_COMPRESSED) {
      data = inflate_buf;
      write_len = tinfl_decompress_mem_to_mem(
          &inflate_buf[0], sizeof(inflate_buf), buf->data, write_len,
          TINFL_FLAG_PARSE_ZLIB_HEADER);
      if (write_len == TINFL_DECOMPRESS_MEM_TO_MEM_FAILED) {
        ret = 0x40;
        goto out;
      }
    }
    wr.decomp_time += ccount() - start_count;
    MD5Update(&ctx, (uint8_t *) data, write_len);
    start_count = ccount();
    wr.erase_time += ccount() - start_count;
    start_count = ccount();
    if (esp_rom_spiflash_write(addr, data, write_len) != 0) {
      ret = 0x38;
      goto out;
    }
    wr.write_time += ccount() - start_count;
    buf->len = 0;
    ub.bri++;
    if (ub.bri == NUM_BUFS) ub.bri = 0;
    addr += write_len;
    wp.num_written += write_len;
    struct MD5Context ctx2;
    memcpy(&ctx2, &ctx, sizeof(ctx));
    MD5Final(wp.digest, &ctx2);
    SLIP_send(&wp, sizeof(wp));
  }

  MD5Final(wr.digest, &ctx);

  wr.total_time = ccount() - wr.total_time;
  SLIP_send(&wr, sizeof(wr));

out:
  WRITE_PERI_REG(UART_CONF1_REG(0), saved_conf1);
  ets_isr_mask(1 << ETS_UART0_INUM);
  return ret;
}

int do_flash_read(uint32_t addr, uint32_t len, uint32_t block_size,
                  uint32_t max_in_flight) {
  uint8_t buf[FLASH_SECTOR_SIZE];
  uint8_t digest[16];
  struct MD5Context ctx;
  uint32_t num_sent = 0, num_acked = 0;
  if (block_size > sizeof(buf)) return 0x52;
  MD5Init(&ctx);
  while (num_acked < len) {
    while (num_sent < len && num_sent - num_acked < max_in_flight) {
      uint32_t n = len - num_sent;
      if (n > block_size) n = block_size;
      if (esp_rom_spiflash_read(addr, (uint32_t *) buf, n) != 0) return 0x53;
      send_packet(buf, n);
      MD5Update(&ctx, buf, n);
      addr += n;
      num_sent += n;
    }
    {
      if (SLIP_recv(&num_acked, sizeof(num_acked)) != 4) return 0x54;
      if (num_acked > num_sent) return 0x55;
    }
  }
  MD5Final(digest, &ctx);
  send_packet(digest, sizeof(digest));
  return 0;
}

int do_flash_digest(uint32_t addr, uint32_t len, uint32_t digest_block_size) {
  uint8_t buf[FLASH_SECTOR_SIZE];
  uint8_t digest[16];
  uint32_t read_block_size =
      digest_block_size ? digest_block_size : sizeof(buf);
  struct MD5Context ctx;
  if (digest_block_size > sizeof(buf)) return 0x62;
  MD5Init(&ctx);
  while (len > 0) {
    uint32_t n = len;
    struct MD5Context block_ctx;
    MD5Init(&block_ctx);
    if (n > read_block_size) n = read_block_size;
    if (esp_rom_spiflash_read(addr, (uint32_t *) buf, n) != 0) return 0x63;
    MD5Update(&ctx, buf, n);
    if (digest_block_size > 0) {
      MD5Update(&block_ctx, buf, n);
      MD5Final(digest, &block_ctx);
      send_packet(digest, sizeof(digest));
    }
    addr += n;
    len -= n;
  }
  MD5Final(digest, &ctx);
  send_packet(digest, sizeof(digest));
  return 0;
}

int do_flash_read_chip_id(void) {
  uint32_t chip_id = 0;
  WRITE_PERI_REG(SPI_CMD_REG(0), SPI_FLASH_RDID);
  while (READ_PERI_REG(SPI_CMD_REG(0)) & SPI_FLASH_RDID) {
  }
  chip_id = READ_PERI_REG(SPI_W0_REG(0)) & 0xFFFFFF;
  send_packet((uint8_t *) &chip_id, sizeof(chip_id));
  return 0;
}

uint8_t cmd_loop(void) {
  uint8_t cmd = 0x55;
  do {
    /* Reset FIFO to re-sync */
    SET_PERI_REG_MASK(UART_CONF0_REG(0), UART_RXFIFO_RST);
    CLEAR_PERI_REG_MASK(UART_CONF0_REG(0), UART_RXFIFO_RST);
    uint32_t args[4];
    uint32_t len = SLIP_recv(&cmd, 1);
    if (len != 1) {
      continue;
    }
    uint8_t resp = 0xff;
    switch (cmd) {
      case CMD_FLASH_WRITE: {
        len = SLIP_recv(args, sizeof(args));
        if (len == 12) {
          resp = do_flash_write(args[0] /* addr */, args[1] /* len */,
                                args[2] /* erase */);
        } else {
          resp = 0x41;
        }
        break;
      }
      case CMD_FLASH_READ: {
        len = SLIP_recv(args, sizeof(args));
        if (len == 16) {
          resp = do_flash_read(args[0] /* addr */, args[1], /* len */
                               args[2] /* block_size */,
                               args[3] /* max_in_flight */);
        } else {
          resp = 0x51;
        }
        break;
      }
      case CMD_FLASH_DIGEST: {
        len = SLIP_recv(args, sizeof(args));
        if (len == 12) {
          resp = do_flash_digest(args[0] /* addr */, args[1], /* len */
                                 args[2] /* digest_block_size */);
        } else {
          resp = 0x61;
        }
        break;
      }
      case CMD_FLASH_READ_CHIP_ID: {
        resp = do_flash_read_chip_id();
        break;
      }
      case CMD_FLASH_ERASE_CHIP: {
        resp = esp_rom_spiflash_erase_chip();
        break;
      }
      case CMD_BOOT_FW:
      case CMD_REBOOT: {
        resp = 0;
        SLIP_send(&resp, 1);
        return cmd;
      }
      case CMD_ECHO: {
        len = SLIP_recv(args, sizeof(args));
        SLIP_send(args, len);
        resp = 0;
        break;
      }
      case CMD_READ_REG: {
        len = SLIP_recv(args, sizeof(args));
        if (len == 4) {
          uint32_t value = READ_PERI_REG((uint32_t *) args[0]);
          SLIP_send(&value, sizeof(value));
          resp = 0;
        } else {
          resp = 0x91;
        }
        break;
      }
      case CMD_WRITE_REG: {
        len = SLIP_recv(args, sizeof(args));
        if (len == 8) {
          WRITE_PERI_REG((uint32_t *) args[0], args[1]);
          resp = 0;
        } else {
          resp = 0xa1;
        }
        break;
      }
    }
    SLIP_send(&resp, 1);
  } while (cmd != CMD_BOOT_FW && cmd != CMD_REBOOT);
  return cmd;
}

void stub_main1(void) {
  uint32_t old_baud_rate = params[0], new_baud_rate = params[1];
  uint8_t last_cmd;

  /* This points at us right now, reset for next boot. */
  ets_set_user_start(0);

/* Selects SPI functions for flash pins. */
#if defined(ESP8266)
  SelectSpiFunction();
  spi_flash_attach();
  SET_PERI_REG_MASK(0x3FF00014, 1); /* Switch to 160 MHz */
#elif defined(ESP32)
  esp_rom_spiflash_attach(ets_efuse_get_spiconfig(), 0 /* legacy */);
#endif

  esp_rom_spiflash_config_param(
      0 /* deviceId */, 16 * 1024 * 1024 /* chip_size */, FLASH_BLOCK_SIZE,
      FLASH_SECTOR_SIZE, FLASH_PAGE_SIZE, 0xffff /* status_mask */);

  uint32_t old_div = 0;
  if (new_baud_rate > 0) {
    ets_delay_us(10000);
    old_div = set_baud_rate(0, old_baud_rate, new_baud_rate);
  }

  /* Give host time to get ready too. */
  ets_delay_us(50000);

#ifdef BAUD_TEST
  while (1) {
    WRITE_PERI_REG(UART_FIFO_REG(0), 0x55);
  }
#else
  SLIP_send(&old_div, 4);
#endif

  last_cmd = cmd_loop();

  ets_delay_us(10000);

  if (last_cmd == CMD_BOOT_FW) {
#if defined(ESP8266)
    /*
     * Find the return address in our own stack and change it.
     * "flash_finish" it gets to the same point, except it doesn't need to
     * patch up its RA: it returns from UartDwnLdProc, then from f_400011ac,
     * then jumps to 0x4000108a, then checks strapping bits again (which will
     * not have changed), and then proceeds to 0x400010a8.
     */
    volatile uint32_t *sp = &old_baud_rate;
    while (*sp != (uint32_t) 0x40001100) sp++;
    *sp = 0x400010a8;
    /*
     * The following dummy asm fragment acts as a barrier, to make sure function
     * epilogue, including return address loading, is added after our stack
     * patching.
     */
    __asm volatile("nop.n");
    return; /* To 0x400010a8 */
#elif defined(ESP32)
/* TODO(rojer) */
#endif
  } else {
    software_reset();
  }
  /* Not reached */
}

/* miniz requires at least 12K of stack */
uint32_t stack[3071];
uint32_t stack_end;

void stub_main(void) {
  memset(&_bss_start, 0, (&_bss_end - &_bss_start));
  __asm volatile("movi a1, stack_end\n");
  stub_main1();
  // Keep the stack vars alive.
  stack[0] = stack_end = 0xff;
}
