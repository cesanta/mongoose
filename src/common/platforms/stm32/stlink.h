/*
 * File:   stlink.h
 *
 * This should contain all the common top level stlink interfaces, regardless
 * of how the backend does the work....
 */
#ifndef STLINK_H
#define STLINK_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

#define STLINK_ARRAY_SIZE(x) (sizeof(x) / sizeof(x[0]))

// Max data transfer size.
// 6kB = max mem32_read block, 8kB sram
//#define Q_BUF_LEN	96
#define Q_BUF_LEN (1024 * 100)

// STLINK_DEBUG_RESETSYS, etc:
#define STLINK_CORE_RUNNING 0x80
#define STLINK_CORE_HALTED 0x81
#define STLINK_CORE_STAT_UNKNOWN -1

#define STLINK_GET_VERSION 0xf1
#define STLINK_GET_CURRENT_MODE 0xf5
#define STLINK_GET_TARGET_VOLTAGE 0xF7

#define STLINK_DEBUG_COMMAND 0xF2
#define STLINK_DFU_COMMAND 0xF3
#define STLINK_DFU_EXIT 0x07

// STLINK_GET_CURRENT_MODE
#define STLINK_DEV_DFU_MODE 0x00
#define STLINK_DEV_MASS_MODE 0x01
#define STLINK_DEV_DEBUG_MODE 0x02
#define STLINK_DEV_UNKNOWN_MODE -1

// TODO - possible poor names...
#define STLINK_SWD_ENTER 0x30
#define STLINK_SWD_READCOREID 0x32  // TBD
#define STLINK_JTAG_WRITEDEBUG_32BIT 0x35
#define STLINK_JTAG_READDEBUG_32BIT 0x36
#define STLINK_JTAG_DRIVE_NRST 0x3c

#define STLINK_DEBUG_APIV2_SWD_SET_FREQ 0x43

/* cortex core ids */
// TODO clean this up...
#define STM32VL_CORE_ID 0x1ba01477
#define STM32F7_CORE_ID 0x5ba02477

// Constant STM32 memory map figures
#define STM32_FLASH_BASE 0x08000000
#define STM32_SRAM_BASE 0x20000000

// Baud rate divisors for SWDCLK
#define STLINK_SWDCLK_4MHZ_DIVISOR 0
#define STLINK_SWDCLK_1P8MHZ_DIVISOR 1
#define STLINK_SWDCLK_1P2MHZ_DIVISOR 2
#define STLINK_SWDCLK_950KHZ_DIVISOR 3
#define STLINK_SWDCLK_480KHZ_DIVISOR 7
#define STLINK_SWDCLK_240KHZ_DIVISOR 15
#define STLINK_SWDCLK_125KHZ_DIVISOR 31
#define STLINK_SWDCLK_100KHZ_DIVISOR 40
#define STLINK_SWDCLK_50KHZ_DIVISOR 79
#define STLINK_SWDCLK_25KHZ_DIVISOR 158
#define STLINK_SWDCLK_15KHZ_DIVISOR 265
#define STLINK_SWDCLK_5KHZ_DIVISOR 798

#define STLINK_CHIPID_STM32_F7 0x449

/* Enough space to hold both a V2 command or a V1 command packaged as generic
 * scsi*/
#define C_BUF_LEN 32

struct stlink_reg {
  uint32_t r[16];
  uint32_t s[32];
  uint32_t xpsr;
  uint32_t main_sp;
  uint32_t process_sp;
  uint32_t rw;
  uint32_t rw2;
  uint8_t control;
  uint8_t faultmask;
  uint8_t basepri;
  uint8_t primask;
  uint32_t fpscr;
};

typedef uint32_t stm32_addr_t;

typedef struct flash_loader {
  stm32_addr_t loader_addr; /* loader sram adddr */
  stm32_addr_t buf_addr;    /* buffer sram address */
} flash_loader_t;

typedef struct stlink_version_ {
  uint32_t stlink_v;
  uint32_t jtag_v;
  uint32_t swim_v;
  uint32_t st_vid;
  uint32_t stlink_pid;
} stlink_version_t;

typedef struct _stlink stlink_t;

struct _stlink {
  void *backend_data;

  // Room for the command header
  unsigned char c_buf[C_BUF_LEN];
  // Data transferred from or to device
  unsigned char q_buf[Q_BUF_LEN];
  int q_len;

  // transport layer verboseness: 0 for no debug info, 10 for lots
  uint32_t core_id;
  uint32_t chip_id;
  int core_stat;

  char serial[16];
  int serial_size;

  stm32_addr_t flash_base;
  size_t flash_size;
  size_t flash_pgsz;

  /* sram settings */
  stm32_addr_t sram_base;
  size_t sram_size;

  // bootloader
  stm32_addr_t sys_base;
  size_t sys_size;

  struct stlink_version_ version;
};

int stlink_enter_swd_mode(stlink_t *sl);
int stlink_enter_jtag_mode(stlink_t *sl);
int stlink_exit_debug_mode(stlink_t *sl);
int stlink_exit_dfu_mode(stlink_t *sl);
void stlink_close(stlink_t *sl);
int stlink_core_id(stlink_t *sl);
int stlink_reset(stlink_t *sl);
int stlink_jtag_reset(stlink_t *sl, int value);
int stlink_run(stlink_t *sl);
int stlink_status(stlink_t *sl);
int stlink_version(stlink_t *sl);
int stlink_read_debug32(stlink_t *sl, uint32_t addr, uint32_t *data);
int stlink_read_mem32(stlink_t *sl, uint32_t addr, uint16_t len);
int stlink_write_debug32(stlink_t *sl, uint32_t addr, uint32_t data);
int stlink_write_mem32(stlink_t *sl, uint32_t addr, uint16_t len);
int stlink_write_mem8(stlink_t *sl, uint32_t addr, uint16_t len);
int stlink_read_all_regs(stlink_t *sl, struct stlink_reg *regp);
int stlink_read_reg(stlink_t *sl, int r_idx, struct stlink_reg *regp);
int stlink_write_reg(stlink_t *sl, uint32_t reg, int idx);
int stlink_step(stlink_t *sl);
int stlink_current_mode(stlink_t *sl);
int stlink_force_debug(stlink_t *sl);
int stlink_target_voltage(stlink_t *sl);
int stlink_set_swdclk(stlink_t *sl, uint16_t divisor);
int stlink_write_flash(stlink_t *sl, stm32_addr_t address, uint8_t *data,
                       uint32_t length, uint8_t eraseonly);
int stlink_fwrite_flash(stlink_t *sl, const char *path, stm32_addr_t addr);
int stlink_verify_write_flash(stlink_t *sl, stm32_addr_t address, uint8_t *data,
                              uint32_t length);
int stlink_chip_id(stlink_t *sl, uint32_t *chip_id);
int stlink_erase_flash_page(stlink_t *sl, stm32_addr_t flashaddr);
uint32_t stlink_calculate_pagesize(stlink_t *sl, uint32_t flashaddr);
uint16_t read_uint16(const unsigned char *c, const int pt);
void stlink_core_stat(stlink_t *sl);
unsigned int is_bigendian(void);
uint32_t read_uint32(const unsigned char *c, const int pt);
void write_uint32(unsigned char *buf, uint32_t ui);
void write_uint16(unsigned char *buf, uint16_t ui);
bool stlink_is_core_halted(stlink_t *sl);
int write_buffer_to_sram(stlink_t *sl, flash_loader_t *fl, const uint8_t *buf,
                         size_t size);
int write_loader_to_sram(stlink_t *sl, stm32_addr_t *addr, size_t *size);
int stlink_load_device_params(stlink_t *sl);
void stlink_fwrite_finalize(stlink_t *sl, stm32_addr_t addr);

#include "usb.h"
#include "flash_loader.h"

#ifdef __cplusplus
}
#endif

#endif /* STLINK_H */
