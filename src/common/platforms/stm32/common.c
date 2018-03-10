#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "stlink.h"
#include "usb.h"

#ifndef _WIN32
#define O_BINARY 0  //! @todo get rid of this OH MY (@xor-gate)
#endif

/* todo: stm32l15xxx flash memory, pm0062 manual */

/* stm32f FPEC flash controller interface, pm0063 manual */
// TODO - all of this needs to be abstracted out....
// STM32F05x is identical, based on RM0091 (DM00031936, Doc ID 018940 Rev 2,
// August 2012)
#define FLASH_REGS_ADDR 0x40022000
#define FLASH_REGS_SIZE 0x28

#define FLASH_ACR (FLASH_REGS_ADDR + 0x00)
#define FLASH_KEYR (FLASH_REGS_ADDR + 0x04)
#define FLASH_SR (FLASH_REGS_ADDR + 0x0c)
#define FLASH_CR (FLASH_REGS_ADDR + 0x10)
#define FLASH_AR (FLASH_REGS_ADDR + 0x14)
#define FLASH_OBR (FLASH_REGS_ADDR + 0x1c)
#define FLASH_WRPR (FLASH_REGS_ADDR + 0x20)

// For STM32F05x, the RDPTR_KEY may be wrong, but as it is not used anywhere...
#define FLASH_RDPTR_KEY 0x00a5
#define FLASH_KEY1 0x45670123
#define FLASH_KEY2 0xcdef89ab

#define FLASH_SR_BSY 0
#define FLASH_SR_EOP 5

#define FLASH_CR_PG 0
#define FLASH_CR_PER 1
#define FLASH_CR_MER 2
#define FLASH_CR_STRT 6
#define FLASH_CR_LOCK 7

// 32L = 32F1 same CoreID as 32F4!
#define STM32L_FLASH_REGS_ADDR ((uint32_t) 0x40023c00)
#define STM32L_FLASH_ACR (STM32L_FLASH_REGS_ADDR + 0x00)
#define STM32L_FLASH_PECR (STM32L_FLASH_REGS_ADDR + 0x04)
#define STM32L_FLASH_PDKEYR (STM32L_FLASH_REGS_ADDR + 0x08)
#define STM32L_FLASH_PEKEYR (STM32L_FLASH_REGS_ADDR + 0x0c)
#define STM32L_FLASH_PRGKEYR (STM32L_FLASH_REGS_ADDR + 0x10)
#define STM32L_FLASH_OPTKEYR (STM32L_FLASH_REGS_ADDR + 0x14)
#define STM32L_FLASH_SR (STM32L_FLASH_REGS_ADDR + 0x18)
#define STM32L_FLASH_OBR (STM32L_FLASH_REGS_ADDR + 0x1c)
#define STM32L_FLASH_WRPR (STM32L_FLASH_REGS_ADDR + 0x20)
#define FLASH_L1_FPRG 10
#define FLASH_L1_PROG 3

// 32L4 register base is at FLASH_REGS_ADDR (0x40022000)
#define STM32L4_FLASH_KEYR (FLASH_REGS_ADDR + 0x08)
#define STM32L4_FLASH_SR (FLASH_REGS_ADDR + 0x10)
#define STM32L4_FLASH_CR (FLASH_REGS_ADDR + 0x14)
#define STM32L4_FLASH_OPTR (FLASH_REGS_ADDR + 0x20)

#define STM32L4_FLASH_SR_BSY 16
#define STM32L4_FLASH_SR_ERRMASK 0x3f8 /* SR [9:3] */

#define STM32L4_FLASH_CR_LOCK 31 /* Lock control register */
#define STM32L4_FLASH_CR_PG 0    /* Program */
#define STM32L4_FLASH_CR_PER 1   /* Page erase */
#define STM32L4_FLASH_CR_MER1 2  /* Bank 1 erase */
#define STM32L4_FLASH_CR_MER2 15 /* Bank 2 erase */
#define STM32L4_FLASH_CR_STRT 16 /* Start command */
#define STM32L4_FLASH_CR_BKER 11 /* Bank select for page erase */
#define STM32L4_FLASH_CR_PNB 3   /* Page number (8 bits) */
// Bits requesting flash operations (useful when we want to clear them)
#define STM32L4_FLASH_CR_OPBITS                                   \
  ((1lu << STM32L4_FLASH_CR_PG) | (1lu << STM32L4_FLASH_CR_PER) | \
   (1lu << STM32L4_FLASH_CR_MER1) | (1lu << STM32L4_FLASH_CR_MER1))
// Page is fully specified by BKER and PNB
#define STM32L4_FLASH_CR_PAGEMASK (0x1fflu << STM32L4_FLASH_CR_PNB)

#define STM32L4_FLASH_OPTR_DUALBANK 21

// STM32L0x flash register base and offsets
// same as 32L1 above
// RM0090 - DM00031020.pdf
#define STM32L0_FLASH_REGS_ADDR ((uint32_t) 0x40022000)
#define FLASH_ACR_OFF ((uint32_t) 0x00)
#define FLASH_PECR_OFF ((uint32_t) 0x04)
#define FLASH_PDKEYR_OFF ((uint32_t) 0x08)
#define FLASH_PEKEYR_OFF ((uint32_t) 0x0c)
#define FLASH_PRGKEYR_OFF ((uint32_t) 0x10)
#define FLASH_OPTKEYR_OFF ((uint32_t) 0x14)
#define FLASH_SR_OFF ((uint32_t) 0x18)
#define FLASH_OBR_OFF ((uint32_t) 0x1c)
#define FLASH_WRPR_OFF ((uint32_t) 0x20)

// STM32F4
#define FLASH_F4_REGS_ADDR ((uint32_t) 0x40023c00)
#define FLASH_F4_KEYR (FLASH_F4_REGS_ADDR + 0x04)
#define FLASH_F4_OPT_KEYR (FLASH_F4_REGS_ADDR + 0x08)
#define FLASH_F4_SR (FLASH_F4_REGS_ADDR + 0x0c)
#define FLASH_F4_CR (FLASH_F4_REGS_ADDR + 0x10)
#define FLASH_F4_OPT_CR (FLASH_F4_REGS_ADDR + 0x14)
#define FLASH_F4_CR_STRT 16
#define FLASH_F4_CR_LOCK 31
#define FLASH_F4_CR_SER 1
#define FLASH_F4_CR_SNB 3
#define FLASH_F4_CR_SNB_MASK 0xf8
#define FLASH_F4_SR_BSY 16

#define L1_WRITE_BLOCK_SIZE 0x80
#define L0_WRITE_BLOCK_SIZE 0x40

#define STLINK_REG_CM3_CPUID 0xE000ED00
#define STLINK_REG_CM3_FP_CTRL 0xE0002000
#define STLINK_REG_CM3_FP_COMP0 0xE0002008

/* Cortex™-M3 Technical Reference Manual */
/* Debug Halting Control and Status Register */
#define STLINK_REG_DHCSR 0xe000edf0
#define STLINK_REG_DHCSR_DBGKEY 0xa05f0000
#define STLINK_REG_DCRSR 0xe000edf4
#define STLINK_REG_DCRDR 0xe000edf8

struct stlink_chipid_params {
  uint32_t chip_id;
  char *description;
  uint32_t flash_size_reg;
  uint32_t flash_pagesize;
  uint32_t sram_size;
  uint32_t bootrom_base;
  uint32_t bootrom_size;
};

void write_uint32(unsigned char *buf, uint32_t ui) {
  if (!is_bigendian()) {  // le -> le (don't swap)
    buf[0] = ((unsigned char *) &ui)[0];
    buf[1] = ((unsigned char *) &ui)[1];
    buf[2] = ((unsigned char *) &ui)[2];
    buf[3] = ((unsigned char *) &ui)[3];
  } else {
    buf[0] = ((unsigned char *) &ui)[3];
    buf[1] = ((unsigned char *) &ui)[2];
    buf[2] = ((unsigned char *) &ui)[1];
    buf[3] = ((unsigned char *) &ui)[0];
  }
}

void write_uint16(unsigned char *buf, uint16_t ui) {
  if (!is_bigendian()) {  // le -> le (don't swap)
    buf[0] = ((unsigned char *) &ui)[0];
    buf[1] = ((unsigned char *) &ui)[1];
  } else {
    buf[0] = ((unsigned char *) &ui)[1];
    buf[1] = ((unsigned char *) &ui)[0];
  }
}

uint32_t read_uint32(const unsigned char *c, const int pt) {
  uint32_t ui;
  char *p = (char *) &ui;

  if (!is_bigendian()) {  // le -> le (don't swap)
    p[0] = c[pt + 0];
    p[1] = c[pt + 1];
    p[2] = c[pt + 2];
    p[3] = c[pt + 3];
  } else {
    p[0] = c[pt + 3];
    p[1] = c[pt + 2];
    p[2] = c[pt + 1];
    p[3] = c[pt + 0];
  }
  return ui;
}

static uint32_t __attribute__((unused)) read_flash_rdp(stlink_t *sl) {
  uint32_t rdp;
  stlink_read_debug32(sl, FLASH_WRPR, &rdp);
  return rdp & 0xff;
}

static inline uint32_t read_flash_cr(stlink_t *sl) {
  uint32_t res;
  stlink_read_debug32(sl, FLASH_F4_CR, &res);
  return res;
}

static inline unsigned int is_flash_locked(stlink_t *sl) {
  /* return non zero for true */
  uint32_t cr_lock_shift = FLASH_F4_CR_LOCK, cr = read_flash_cr(sl);
  return cr & (1 << cr_lock_shift);
}

static void unlock_flash(stlink_t *sl) {
  uint32_t key_reg = FLASH_F4_KEYR;
  stlink_write_debug32(sl, key_reg, FLASH_KEY1);
  stlink_write_debug32(sl, key_reg, FLASH_KEY2);
}

static int unlock_flash_if(stlink_t *sl) {
  /* unlock flash if already locked */

  if (is_flash_locked(sl)) {
    unlock_flash(sl);
    if (is_flash_locked(sl)) {
      printf("Failed to unlock flash!\n");
      return -1;
    }
  }
  return 0;
}

static void lock_flash(stlink_t *sl) {
  uint32_t n;
  uint32_t cr_reg = FLASH_F4_CR;
  uint32_t cr_lock_shift = FLASH_F4_CR_LOCK;

  n = read_flash_cr(sl) | (1 << cr_lock_shift);
  stlink_write_debug32(sl, cr_reg, n);
}

static void set_flash_cr_pg(stlink_t *sl) {
  uint32_t cr_reg, x;

  x = read_flash_cr(sl);

  cr_reg = FLASH_F4_CR;
  x |= 1 << FLASH_CR_PG;

  stlink_write_debug32(sl, cr_reg, x);
}

static void __attribute__((unused)) clear_flash_cr_pg(stlink_t *sl) {
  uint32_t cr_reg, n;

  cr_reg = FLASH_F4_CR;

  n = read_flash_cr(sl) & ~(1 << FLASH_CR_PG);
  stlink_write_debug32(sl, cr_reg, n);
}

static void set_flash_cr_per(stlink_t *sl) {
  const uint32_t n = 1 << FLASH_CR_PER;
  stlink_write_debug32(sl, FLASH_CR, n);
}

static void __attribute__((unused)) clear_flash_cr_per(stlink_t *sl) {
  const uint32_t n = read_flash_cr(sl) & ~(1 << FLASH_CR_PER);
  stlink_write_debug32(sl, FLASH_CR, n);
}

static void set_flash_cr_mer(stlink_t *sl, bool v) {
  uint32_t val, cr_reg, cr_mer, cr_pg;

  cr_reg = FLASH_F4_CR;
  cr_mer = 1 << FLASH_CR_MER;
  cr_pg = 1 << FLASH_CR_PG;

  stlink_read_debug32(sl, cr_reg, &val);
  if (val & cr_pg) {
    /* STM32F030 will drop MER bit if PG was set */
    val &= ~cr_pg;
    stlink_write_debug32(sl, cr_reg, val);
  }

  if (v)
    val |= cr_mer;
  else
    val &= ~cr_mer;
  stlink_write_debug32(sl, cr_reg, val);
}

static void __attribute__((unused)) clear_flash_cr_mer(stlink_t *sl) {
  uint32_t val, cr_reg, cr_mer;

  cr_reg = FLASH_F4_CR;
  cr_mer = 1 << FLASH_CR_MER;

  stlink_read_debug32(sl, cr_reg, &val);
  val &= ~cr_mer;
  stlink_write_debug32(sl, cr_reg, val);
}

static void set_flash_cr_strt(stlink_t *sl) {
  uint32_t val, cr_reg, cr_strt;

  cr_reg = FLASH_F4_CR;
  cr_strt = 1 << FLASH_F4_CR_STRT;

  stlink_read_debug32(sl, cr_reg, &val);
  val |= cr_strt;
  stlink_write_debug32(sl, cr_reg, val);
}

static inline uint32_t read_flash_sr(stlink_t *sl) {
  uint32_t res, sr_reg;
  sr_reg = FLASH_F4_SR;
  stlink_read_debug32(sl, sr_reg, &res);
  return res;
}

static inline unsigned int is_flash_busy(stlink_t *sl) {
  uint32_t sr_busy_shift;
  sr_busy_shift = FLASH_F4_SR_BSY;
  return read_flash_sr(sl) & (1 << sr_busy_shift);
}

static void wait_flash_busy(stlink_t *sl) {
  /* todo: add some delays here */
  while (is_flash_busy(sl))
    ;
}

static void wait_flash_busy_progress(stlink_t *sl) {
  while (is_flash_busy(sl)) {
    usleep(10000);
  }
}

static inline unsigned int is_flash_eop(stlink_t *sl) {
  return read_flash_sr(sl) & (1 << FLASH_SR_EOP);
}

static void __attribute__((unused)) clear_flash_sr_eop(stlink_t *sl) {
  const uint32_t n = read_flash_sr(sl) & ~(1 << FLASH_SR_EOP);
  stlink_write_debug32(sl, FLASH_SR, n);
}

static void __attribute__((unused)) wait_flash_eop(stlink_t *sl) {
  /* todo: add some delays here */
  while (is_flash_eop(sl) == 0)
    ;
}

static inline void write_flash_ar(stlink_t *sl, uint32_t n) {
  stlink_write_debug32(sl, FLASH_AR, n);
}

static inline void write_flash_cr_psiz(stlink_t *sl, uint32_t n) {
  uint32_t x = read_flash_cr(sl);
  x &= ~(0x03 << 8);
  x |= (n << 8);
  stlink_write_debug32(sl, FLASH_F4_CR, x);
}

static inline void write_flash_cr_snb(stlink_t *sl, uint32_t n) {
  uint32_t x = read_flash_cr(sl);
  x &= ~FLASH_F4_CR_SNB_MASK;
  x |= (n << FLASH_F4_CR_SNB);
  x |= (1 << FLASH_F4_CR_SER);
  stlink_write_debug32(sl, FLASH_F4_CR, x);
}

static inline void write_flash_cr_bker_pnb(stlink_t *sl, uint32_t n) {
  stlink_write_debug32(sl, STM32L4_FLASH_SR,
                       0xFFFFFFFF & ~(1 << STM32L4_FLASH_SR_BSY));
  uint32_t x = read_flash_cr(sl);
  x &= ~STM32L4_FLASH_CR_OPBITS;
  x &= ~STM32L4_FLASH_CR_PAGEMASK;
  x &= ~(1 << STM32L4_FLASH_CR_MER1);
  x &= ~(1 << STM32L4_FLASH_CR_MER2);
  x |= (n << STM32L4_FLASH_CR_PNB);
  x |= (1lu << STM32L4_FLASH_CR_PER);
  stlink_write_debug32(sl, STM32L4_FLASH_CR, x);
}

void stlink_close(stlink_t *sl) {
  if (!sl) return;
  _stlink_usb_close(sl);
  free(sl);
}

int stlink_exit_debug_mode(stlink_t *sl) {
  int ret;

  ret = stlink_write_debug32(sl, STLINK_REG_DHCSR, STLINK_REG_DHCSR_DBGKEY);
  if (ret == -1) return ret;

  return _stlink_usb_exit_debug_mode(sl);
}

int stlink_enter_swd_mode(stlink_t *sl) {
  return _stlink_usb_enter_swd_mode(sl);
}

// Force the core into the debug mode -> halted state.
int stlink_force_debug(stlink_t *sl) {
  return _stlink_usb_force_debug(sl);
}

int stlink_exit_dfu_mode(stlink_t *sl) {
  return _stlink_usb_exit_dfu_mode(sl);
}

int stlink_core_id(stlink_t *sl) {
  int ret;

  ret = _stlink_usb_core_id(sl);
  if (ret == -1) {
    printf("Failed to read core_id\n");
    return ret;
  }
  return ret;
}

int stlink_chip_id(stlink_t *sl, uint32_t *chip_id) {
  int ret;

  ret = stlink_read_debug32(sl, 0xE0042000, chip_id);
  if (ret == -1) return ret;

  if (*chip_id == 0)
    ret = stlink_read_debug32(
        sl, 0x40015800,
        chip_id);  // Try Corex M0 DBGMCU_IDCODE register address

  return ret;
}

/**
 * reads and decodes the flash parameters, as dynamically as possible
 * @param sl
 * @return 0 for success, or -1 for unsupported core type.
 */
static const struct stlink_chipid_params F7_params = {
    // RM0385 and DS10916 document was used to find these paramaters
    .chip_id = STLINK_CHIPID_STM32_F7,
    .description = "F7 device",
    .flash_size_reg = 0x1ff0f442,  // section 41.2
    .flash_pagesize = 0x800,       // No flash pages
    .sram_size = 0x50000,          // "SRAM" byte size in hex from DS Fig 18
    .bootrom_base =
        0x00100000,         // "System memory" starting address from DS Fig 18
    .bootrom_size = 0xEDC0  // "System memory" byte size in hex from DS Fig 18
};

int stlink_load_device_params(stlink_t *sl) {
  const struct stlink_chipid_params *params = &F7_params;
  stlink_core_id(sl);
  uint32_t chip_id;
  uint32_t flash_size;

  stlink_chip_id(sl, &chip_id);
  sl->chip_id = chip_id & 0xfff;
  /* Fix chip_id for F4 rev A errata , Read CPU ID, as CoreID is the same for
   * F2/F4*/
  if (sl->chip_id == 0x411) {
    uint32_t cpuid;
    stlink_read_debug32(sl, 0xE000ED00, &cpuid);
    if ((cpuid & 0xfff0) == 0xc240) sl->chip_id = 0x413;
  }

  if (sl->chip_id != STLINK_CHIPID_STM32_F7) {
    printf("unsupported chip id! %d\n", chip_id);
    return -1;
  }

  // These are fixed...
  sl->flash_base = STM32_FLASH_BASE;
  sl->sram_base = STM32_SRAM_BASE;
  stlink_read_debug32(sl, (params->flash_size_reg) & ~3, &flash_size);
  if (params->flash_size_reg & 2) flash_size = flash_size >> 16;
  flash_size = flash_size & 0xffff;

  sl->flash_size = flash_size * 1024;
  sl->flash_pgsz = params->flash_pagesize;
  sl->sram_size = params->sram_size;
  sl->sys_base = params->bootrom_base;
  sl->sys_size = params->bootrom_size;

  return 0;
}

int stlink_reset(stlink_t *sl) {
  return _stlink_usb_reset(sl);
}

int stlink_jtag_reset(stlink_t *sl, int value) {
  return _stlink_usb_jtag_reset(sl, value);
}

int stlink_run(stlink_t *sl) {
  return _stlink_usb_run(sl);
}

int stlink_set_swdclk(stlink_t *sl, uint16_t divisor) {
  return _stlink_usb_set_swdclk(sl, divisor);
}

int stlink_status(stlink_t *sl) {
  int ret;

  ret = _stlink_usb_status(sl);
  stlink_core_stat(sl);

  return ret;
}

int stlink_target_voltage(stlink_t *sl) {
  return _stlink_usb_target_voltage(sl);
}

int stlink_read_debug32(stlink_t *sl, uint32_t addr, uint32_t *data) {
  return _stlink_usb_read_debug32(sl, addr, data);
}

int stlink_write_debug32(stlink_t *sl, uint32_t addr, uint32_t data) {
  return _stlink_usb_write_debug32(sl, addr, data);
}

int stlink_write_mem32(stlink_t *sl, uint32_t addr, uint16_t len) {
  if (len % 4 != 0) {
    fprintf(stderr,
            "Error: Data length doesn't have a 32 bit alignment: +%d byte.\n",
            len % 4);
    abort();
  }
  return _stlink_usb_write_mem32(sl, addr, len);
}

int stlink_read_mem32(stlink_t *sl, uint32_t addr, uint16_t len) {
  if (len % 4 != 0) {  // !!! never ever: fw gives just wrong values
    fprintf(stderr,
            "Error: Data length doesn't have a 32 bit alignment: +%d byte.\n",
            len % 4);
    abort();
  }
  return _stlink_usb_read_mem32(sl, addr, len);
}

int stlink_write_mem8(stlink_t *sl, uint32_t addr, uint16_t len) {
  if (len > 0x40) {  // !!! never ever: Writing more then 0x40 bytes gives
                     // unexpected behaviour
    fprintf(stderr, "Error: Data length > 64: +%d byte.\n", len);
    abort();
  }
  return _stlink_usb_write_mem8(sl, addr, len);
}

int stlink_write_reg(stlink_t *sl, uint32_t reg, int idx) {
  return _stlink_usb_write_reg(sl, reg, idx);
}

int stlink_read_reg(stlink_t *sl, int r_idx, struct stlink_reg *regp) {
  if (r_idx > 20 || r_idx < 0) {
    fprintf(stderr, "Error: register index must be in [0..20]\n");
    return -1;
  }

  return _stlink_usb_read_reg(sl, r_idx, regp);
}

bool stlink_is_core_halted(stlink_t *sl) {
  bool ret = false;

  stlink_status(sl);
  if (sl->q_buf[0] == STLINK_CORE_HALTED) ret = true;

  return ret;
}

int stlink_step(stlink_t *sl) {
  return _stlink_usb_step(sl);
}

int stlink_current_mode(stlink_t *sl) {
  return _stlink_usb_current_mode(sl);
}

// End of delegates....  Common code below here...

// Endianness
// http://www.ibm.com/developerworks/aix/library/au-endianc/index.html
// const int i = 1;
// #define is_bigendian() ( (*(char*)&i) == 0 )

unsigned int is_bigendian(void) {
  static volatile const unsigned int i = 1;
  return *(volatile const char *) &i == 0;
}

uint16_t read_uint16(const unsigned char *c, const int pt) {
  uint32_t ui;
  char *p = (char *) &ui;

  if (!is_bigendian()) {  // le -> le (don't swap)
    p[0] = c[pt + 0];
    p[1] = c[pt + 1];
  } else {
    p[0] = c[pt + 1];
    p[1] = c[pt + 0];
  }
  return ui;
}

// same as above with entrypoint.

void stlink_run_at(stlink_t *sl, stm32_addr_t addr) {
  stlink_write_reg(sl, addr, 15); /* pc register */

  stlink_run(sl);

  while (stlink_is_core_halted(sl)) usleep(3000000);
}

void stlink_core_stat(stlink_t *sl) {
  if (sl->q_len <= 0) return;

  switch (sl->q_buf[0]) {
    case STLINK_CORE_RUNNING:
      sl->core_stat = STLINK_CORE_RUNNING;
      return;
    case STLINK_CORE_HALTED:
      sl->core_stat = STLINK_CORE_HALTED;
      return;
    default:
      sl->core_stat = STLINK_CORE_STAT_UNKNOWN;
      fprintf(stderr, "  core status: unknown\n");
  }
}

void stlink_fwrite_finalize(stlink_t *sl, stm32_addr_t addr) {
  unsigned int val;
  /* set stack*/
  stlink_read_debug32(sl, addr, &val);
  stlink_write_reg(sl, val, 13);
  /* Set PC to the reset routine*/
  stlink_read_debug32(sl, addr + 4, &val);
  stlink_write_reg(sl, val, 15);
  stlink_run(sl);
}

typedef bool (*save_block_fn)(void *arg, uint8_t *block, ssize_t len);

static int stlink_read(stlink_t *sl, stm32_addr_t addr, size_t size,
                       save_block_fn fn, void *fn_arg) {
  int error = -1;

  if (size < 1) size = sl->flash_size;

  if (size > sl->flash_size) size = sl->flash_size;

  size_t cmp_size = (sl->flash_pgsz > 0x1800) ? 0x1800 : sl->flash_pgsz;
  for (size_t off = 0; off < size; off += cmp_size) {
    size_t aligned_size;

    /* adjust last page size */
    if ((off + cmp_size) > size) cmp_size = size - off;

    aligned_size = cmp_size;
    if (aligned_size & (4 - 1)) aligned_size = (cmp_size + 4) & ~(4 - 1);

    stlink_read_mem32(sl, addr + (uint32_t) off, aligned_size);

    if (!fn(fn_arg, sl->q_buf, aligned_size)) {
      goto on_error;
    }
  }

  /* success */
  error = 0;

on_error:
  return error;
}

int write_buffer_to_sram(stlink_t *sl, flash_loader_t *fl, const uint8_t *buf,
                         size_t size) {
  /* write the buffer right after the loader */
  size_t chunk = size & ~0x3;
  size_t rem = size & 0x3;
  if (chunk) {
    memcpy(sl->q_buf, buf, chunk);
    stlink_write_mem32(sl, fl->buf_addr, chunk);
  }
  if (rem) {
    memcpy(sl->q_buf, buf + chunk, rem);
    stlink_write_mem8(sl, (fl->buf_addr) + (uint32_t) chunk, rem);
  }
  return 0;
}

uint32_t calculate_F7_sectornum(uint32_t flashaddr) {
  flashaddr &= ~STM32_FLASH_BASE;  // Page now holding the actual flash address
  if (flashaddr < 0x20000)
    return (flashaddr / 0x8000);
  else if (flashaddr < 0x40000)
    return (4);
  else
    return (flashaddr / 0x40000) + 4;
}

uint32_t stlink_calculate_pagesize(stlink_t *sl, uint32_t flashaddr) {
  if (sl->chip_id == STLINK_CHIPID_STM32_F7) {
    uint32_t sector = calculate_F7_sectornum(flashaddr);
    if (sector < 4)
      sl->flash_pgsz = 0x8000;
    else if (sector < 5)
      sl->flash_pgsz = 0x20000;
    else
      sl->flash_pgsz = 0x40000;
  }
  return (uint32_t) sl->flash_pgsz;
}

/**
 * Erase a page of flash, assumes sl is fully populated with things like
 * chip/core ids
 * @param sl stlink context
 * @param flashaddr an address in the flash page to erase
 * @return 0 on success -ve on failure
 */
int stlink_erase_flash_page(stlink_t *sl, stm32_addr_t flashaddr) {
  /* wait for ongoing op to finish */
  wait_flash_busy(sl);

  /* unlock if locked */
  unlock_flash_if(sl);

  /* select the page to erase */
  // calculate the actual page from the address
  uint32_t sector = calculate_F7_sectornum(flashaddr);

  write_flash_cr_snb(sl, sector);

  /* start erase operation */
  set_flash_cr_strt(sl);

  /* wait for completion */
  wait_flash_busy(sl);

  /* relock the flash */
  // todo: fails to program if this is in
  lock_flash(sl);

  return 0;
}

/**
 * Verify addr..addr+len is binary identical to base...base+len
 * @param sl stlink context
 * @param address stm device address
 * @param data host side buffer to check against
 * @param length how much
 * @return 0 for success, -ve for failure
 */
int stlink_verify_write_flash(stlink_t *sl, stm32_addr_t address, uint8_t *data,
                              unsigned length) {
  size_t off;
  size_t cmp_size = (sl->flash_pgsz > 0x1800) ? 0x1800 : sl->flash_pgsz;
  printf("Verifying flash...\n");
  for (off = 0; off < length; off += cmp_size) {
    size_t aligned_size;

    /* adjust last page size */
    if ((off + cmp_size) > length) cmp_size = length - off;

    aligned_size = cmp_size;
    if (aligned_size & (4 - 1)) aligned_size = (cmp_size + 4) & ~(4 - 1);

    stlink_read_mem32(sl, address + (uint32_t) off, aligned_size);

    if (memcmp(sl->q_buf, data + off, cmp_size)) {
      printf("Verification of flash failed at offset: %u\n",
             (unsigned int) off);
      return -1;
    }
  }
  return 0;
}

int stlink_write_flash(stlink_t *sl, stm32_addr_t addr, uint8_t *base,
                       uint32_t len, uint8_t eraseonly) {
  size_t off;
  flash_loader_t fl;
  printf("Writing %d (%#x) bytes to stm32 address: %u (%#x)\n", len, len, addr,
         addr);
  /* check addr range is inside the flash */
  stlink_calculate_pagesize(sl, addr);
  if (addr < sl->flash_base) {
    printf("addr too low %#x < %#x\n", addr, sl->flash_base);
    return -1;
  } else if ((addr + len) < addr) {
    printf("addr overruns\n");
    return -1;
  } else if ((addr + len) > (sl->flash_base + sl->flash_size)) {
    printf("addr too high\n");
    return -1;
  } else if (addr & 1) {
    printf("unaligned addr 0x%x\n", addr);
    return -1;
  } else if (len & 1) {
    printf("unaligned len 0x%x -- padding with zero\n", len);
    len += 1;
  } else if (addr & (sl->flash_pgsz - 1)) {
    printf("addr not a multiple of pagesize, not supported\n");
    return -1;
  }

  // Make sure we've loaded the context with the chip details
  stlink_core_id(sl);
  /* erase each page */
  int page_count = 0;
  for (off = 0; off < len;
       off += stlink_calculate_pagesize(sl, addr + (uint32_t) off)) {
    fprintf(stdout, "Erasing flash page at addr 0x%08X\n", (int) (addr + off));
    /* addr must be an addr inside the page */
    if (stlink_erase_flash_page(sl, addr + (uint32_t) off) == -1) {
      printf("Failed to erase_flash_page(%#zx) == -1\n", addr + off);
      return -1;
    }
    page_count++;
  }

  if (eraseonly) return 0;

  /* flash loader initialization */
  if (stlink_flash_loader_init(sl, &fl) == -1) {
    printf("stlink_flash_loader_init() == -1\n");
    return -1;
  }

  /* First unlock the cr */
  unlock_flash_if(sl);

  /* TODO: Check that Voltage range is 2.7 - 3.6 V */
  /* set parallelisim to 32 bit*/
  int voltage = stlink_target_voltage(sl);
  if (voltage == -1) {
    printf("Failed to read Target voltage\n");
    return voltage;
  } else if (voltage > 2700) {
    write_flash_cr_psiz(sl, 2);
  } else {
    printf(
        "Target voltage (%d mV) too low for 32-bit flash, using 8-bit "
        "flash writes\n",
        voltage);
    write_flash_cr_psiz(sl, 0);
  }

  /* set programming mode */
  set_flash_cr_pg(sl);

  for (off = 0; off < len;) {
    size_t size = len - off > 0x8000 ? 0x8000 : len - off;
    printf("Writing %d bytes at 0x%08X\n", (int) size,
           (int) (addr + (uint32_t) off));
    if (stlink_flash_loader_run(sl, &fl, addr + (uint32_t) off, base + off,
                                size) == -1) {
      printf("stlink_flash_loader_run(%#zx) failed! == -1\n", addr + off);
      return -1;
    }
    off += size;
  }

  /* Relock flash */
  lock_flash(sl);

  return stlink_verify_write_flash(sl, addr, base, len);
}
