// Copyright (c) 2004-2013 Sergey Lyubka
// Copyright (c) 2013-2022 Cesanta Software Limited
// All rights reserved
//
// This software is dual-licensed: you can redistribute it and/or modify
// it under the terms of the GNU General Public License version 2 as
// published by the Free Software Foundation. For the terms of this
// license, see http://www.gnu.org/licenses/
//
// You are free to use this software under the terms of the GNU General
// Public License, but WITHOUT ANY WARRANTY; without even the implied
// warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
// See the GNU General Public License for more details.
//
// Alternatively, you can license this software under a commercial
// license, as set out in https://www.mongoose.ws/licensing/
//
// SPDX-License-Identifier: GPL-2.0-only or commercial

#include "mongoose.h"

#ifdef MG_ENABLE_LINES
#line 1 "src/base64.c"
#endif


static int mg_base64_encode_single(int c) {
  if (c < 26) {
    return c + 'A';
  } else if (c < 52) {
    return c - 26 + 'a';
  } else if (c < 62) {
    return c - 52 + '0';
  } else {
    return c == 62 ? '+' : '/';
  }
}

static int mg_base64_decode_single(int c) {
  if (c >= 'A' && c <= 'Z') {
    return c - 'A';
  } else if (c >= 'a' && c <= 'z') {
    return c + 26 - 'a';
  } else if (c >= '0' && c <= '9') {
    return c + 52 - '0';
  } else if (c == '+') {
    return 62;
  } else if (c == '/') {
    return 63;
  } else if (c == '=') {
    return 64;
  } else {
    return -1;
  }
}

size_t mg_base64_update(unsigned char ch, char *to, size_t n) {
  unsigned long rem = (n & 3) % 3;
  if (rem == 0) {
    to[n] = (char) mg_base64_encode_single(ch >> 2);
    to[++n] = (char) ((ch & 3) << 4);
  } else if (rem == 1) {
    to[n] = (char) mg_base64_encode_single(to[n] | (ch >> 4));
    to[++n] = (char) ((ch & 15) << 2);
  } else {
    to[n] = (char) mg_base64_encode_single(to[n] | (ch >> 6));
    to[++n] = (char) mg_base64_encode_single(ch & 63);
    n++;
  }
  return n;
}

size_t mg_base64_final(char *to, size_t n) {
  size_t saved = n;
  // printf("---[%.*s]\n", n, to);
  if (n & 3) n = mg_base64_update(0, to, n);
  if ((saved & 3) == 2) n--;
  // printf("    %d[%.*s]\n", n, n, to);
  while (n & 3) to[n++] = '=';
  to[n] = '\0';
  return n;
}

size_t mg_base64_encode(const unsigned char *p, size_t n, char *to, size_t dl) {
  size_t i, len = 0;
  if (dl > 0) to[0] = '\0';
  if (dl < ((n / 3) + (n % 3 ? 1 : 0)) * 4 + 1) return 0;
  for (i = 0; i < n; i++) len = mg_base64_update(p[i], to, len);
  len = mg_base64_final(to, len);
  return len;
}

size_t mg_base64_decode(const char *src, size_t n, char *dst, size_t dl) {
  const char *end = src == NULL ? NULL : src + n;  // Cannot add to NULL
  size_t len = 0;
  if (dl > 0) dst[0] = '\0';
  if (dl < n / 4 * 3 + 1) return 0;
  while (src != NULL && src + 3 < end) {
    int a = mg_base64_decode_single(src[0]),
        b = mg_base64_decode_single(src[1]),
        c = mg_base64_decode_single(src[2]),
        d = mg_base64_decode_single(src[3]);
    if (a == 64 || a < 0 || b == 64 || b < 0 || c < 0 || d < 0) return 0;
    dst[len++] = (char) ((a << 2) | (b >> 4));
    if (src[2] != '=') {
      dst[len++] = (char) ((b << 4) | (c >> 2));
      if (src[3] != '=') dst[len++] = (char) ((c << 6) | d);
    }
    src += 4;
  }
  dst[len] = '\0';
  return len;
}

#ifdef MG_ENABLE_LINES
#line 1 "src/device_dummy.c"
#endif


#if MG_DEVICE == MG_DEVICE_NONE
void *mg_flash_start(void) {
  return NULL;
}
size_t mg_flash_size(void) {
  return 0;
}
size_t mg_flash_sector_size(void) {
  return 0;
}
size_t mg_flash_write_align(void) {
  return 0;
}
int mg_flash_bank(void) {
  return 0;
}
bool mg_flash_erase(void *location) {
  (void) location;
  return false;
}
bool mg_flash_swap_bank(void) {
  return true;
}
bool mg_flash_write(void *addr, const void *buf, size_t len) {
  (void) addr, (void) buf, (void) len;
  return false;
}
void mg_device_reset(void) {
}
#endif

#ifdef MG_ENABLE_LINES
#line 1 "src/device_flash.c"
#endif


#if MG_DEVICE == MG_DEVICE_STM32H7 || MG_DEVICE == MG_DEVICE_STM32H5
// Flash can be written only if it is erased. Erased flash is 0xff (all bits 1)
// Writes must be mg_flash_write_align() - aligned. Thus if we want to save an
// object, we pad it at the end for alignment.
//
// Objects in the flash sector are stored sequentially:
// | 32-bit size | 32-bit KEY | ..data.. | ..pad.. | 32-bit size | ......
//
// In order to get to the next object, read its size, then align up.

// Traverse the list of saved objects
size_t mg_flash_next(char *p, char *end, uint32_t *key, size_t *size) {
  size_t aligned_size = 0, align = mg_flash_write_align(), left = end - p;
  uint32_t *p32 = (uint32_t *) p, min_size = sizeof(uint32_t) * 2;
  if (p32[0] != 0xffffffff && left > MG_ROUND_UP(min_size, align)) {
    if (size) *size = (size_t) p32[0];
    if (key) *key = p32[1];
    aligned_size = MG_ROUND_UP(p32[0] + sizeof(uint32_t) * 2, align);
    if (left < aligned_size) aligned_size = 0;  // Out of bounds, fail
  }
  return aligned_size;
}

// Return the last sector of Bank 2
static char *flash_last_sector(void) {
  size_t ss = mg_flash_sector_size(), size = mg_flash_size();
  char *base = (char *) mg_flash_start(), *last = base + size - ss;
  if (mg_flash_bank() == 2) last -= size / 2;
  return last;
}

// Find a saved object with a given key
bool mg_flash_load(void *sector, uint32_t key, void *buf, size_t len) {
  char *base = (char *) mg_flash_start(), *s = (char *) sector, *res = NULL;
  size_t ss = mg_flash_sector_size(), ofs = 0, n, sz;
  bool ok = false;
  if (s == NULL) s = flash_last_sector();
  if (s < base || s >= base + mg_flash_size()) {
    MG_ERROR(("%p is outsize of flash", sector));
  } else if (((s - base) % ss) != 0) {
    MG_ERROR(("%p is not a sector boundary", sector));
  } else {
    uint32_t k, scanned = 0;
    while ((n = mg_flash_next(s + ofs, s + ss, &k, &sz)) > 0) {
      // MG_DEBUG((" > obj %lu, ofs %lu, key %x/%x", scanned, ofs, k, key));
      // mg_hexdump(s + ofs, n);
      if (k == key && sz == len) {
        res = s + ofs + sizeof(uint32_t) * 2;
        memcpy(buf, res, len);  // Copy object
        ok = true;              // Keep scanning for the newer versions of it
      }
      ofs += n, scanned++;
    }
    MG_DEBUG(("Scanned %u objects, key %x is @ %p", scanned, key, res));
  }
  return ok;
}

static bool mg_flash_writev(char *location, struct mg_str *strings, size_t n) {
  size_t align = mg_flash_write_align(), i, j, k = 0, nwritten = 0;
  char buf[align];
  bool ok = true;
  for (i = 0; ok && i < n; i++) {
    for (j = 0; ok && j < strings[i].len; j++) {
      buf[k++] = strings[i].ptr[j];
      if (k >= sizeof(buf)) {
        ok = mg_flash_write(location + nwritten, buf, sizeof(buf));
        k = 0, nwritten += sizeof(buf);
      }
    }
  }
  if (k > 0) {
    while (k < sizeof(buf)) buf[k++] = 0xff;
    ok = mg_flash_write(location + nwritten, buf, sizeof(buf));
  }
  return ok;
}

// For all saved objects in the sector, delete old versions of objects
static void mg_flash_sector_cleanup(char *sector) {
  // Buffer all saved objects into an IO buffer (backed by RAM)
  // erase sector, and re-save them.
  struct mg_iobuf io = {0, 0, 0, 2048};
  size_t ss = mg_flash_sector_size();
  size_t n, size, size2, ofs = 0, hs = sizeof(uint32_t) * 2;
  uint32_t key;
  // Traverse all objects
  MG_DEBUG(("Cleaning up sector %p", sector));
  while ((n = mg_flash_next(sector + ofs, sector + ss, &key, &size)) > 0) {
    // Delete an old copy of this object in the cache
    for (size_t o = 0; o < io.len; o += size2 + hs) { 
      uint32_t k = *(uint32_t *) (io.buf + o + sizeof(uint32_t));
      size2 = *(uint32_t *) (io.buf + o);
      if (k == key) {
        mg_iobuf_del(&io, o, size2 + hs);
        break;
      }
    }
    // And add the new copy
    mg_iobuf_add(&io, io.len, sector + ofs, size + hs);  
    ofs += n;
  }
  // All objects are cached in RAM now
  if (mg_flash_erase(sector)) {  // Erase sector. If successful,
    for (ofs = 0; ofs < io.len; ofs += size + hs) {  // Traverse cached objects
      size = *(uint32_t *) (io.buf + ofs);
      key = *(uint32_t *) (io.buf + ofs + sizeof(uint32_t));
      mg_flash_save(sector, key, io.buf + ofs + hs, size);  // Save to flash
    }
  }
  mg_iobuf_free(&io);
}

// Save an object with a given key - append to the end of an object list
bool mg_flash_save(void *sector, uint32_t key, const void *buf, size_t len) {
  char *base = (char *) mg_flash_start(), *s = (char *) sector;
  size_t ss = mg_flash_sector_size(), ofs = 0, n;
  bool ok = false;
  if (s == NULL) s = flash_last_sector();
  if (s < base || s >= base + mg_flash_size()) {
    MG_ERROR(("%p is outsize of flash", sector));
  } else if (((s - base) % ss) != 0) {
    MG_ERROR(("%p is not a sector boundary", sector));
  } else {
    size_t needed = sizeof(uint32_t) * 2 + len;
    size_t needed_aligned = MG_ROUND_UP(needed, mg_flash_write_align());
    while ((n = mg_flash_next(s + ofs, s + ss, NULL, NULL)) > 0) ofs += n;

    // If there is not enough space left, cleanup sector and re-eval ofs
    if (ofs + needed_aligned > ss) {
      mg_flash_sector_cleanup(s);
      ofs = 0;
      while ((n = mg_flash_next(s + ofs, s + ss, NULL, NULL)) > 0) ofs += n;
    }

    if (ofs + needed_aligned <= ss) {
      // Enough space to save this object
      uint32_t hdr[2] = {(uint32_t) len, key};
      struct mg_str data[] = {mg_str_n((char *) hdr, sizeof(hdr)),
                              mg_str_n(buf, len)};
      ok = mg_flash_writev(s + ofs, data, 2);
      MG_DEBUG(("Saving %lu bytes @ %p, key %x: %d", len, s + ofs, key, ok));
      MG_DEBUG(("Sector space left: %lu bytes", ss - ofs - needed_aligned));
    } else {
      MG_ERROR(("Sector is full"));
    }
  }
  return ok;
}
#else
bool mg_flash_save(void *sector, uint32_t key, const void *buf, size_t len) {
  (void) sector, (void) key, (void) buf, (void) len;
  return false;
}
bool mg_flash_load(void *sector, uint32_t key, void *buf, size_t len) {
  (void) sector, (void) key, (void) buf, (void) len;
  return false;
}
#endif

#ifdef MG_ENABLE_LINES
#line 1 "src/device_stm32h5.c"
#endif



#if MG_DEVICE == MG_DEVICE_STM32H5

#define FLASH_BASE 0x40022000          // Base address of the flash controller
#define FLASH_KEYR (FLASH_BASE + 0x4)  // See RM0481 7.11
#define FLASH_OPTKEYR (FLASH_BASE + 0xc)
#define FLASH_OPTCR (FLASH_BASE + 0x1c)
#define FLASH_NSSR (FLASH_BASE + 0x20)
#define FLASH_NSCR (FLASH_BASE + 0x28)
#define FLASH_NSCCR (FLASH_BASE + 0x30)
#define FLASH_OPTSR_CUR (FLASH_BASE + 0x50)
#define FLASH_OPTSR_PRG (FLASH_BASE + 0x54)

void *mg_flash_start(void) {
  return (void *) 0x08000000;
}
size_t mg_flash_size(void) {
  return 2 * 1024 * 1024;  // 2Mb
}
size_t mg_flash_sector_size(void) {
  return 8 * 1024;  // 8k
}
size_t mg_flash_write_align(void) {
  return 16;  // 128 bit
}
int mg_flash_bank(void) {
  return MG_REG(FLASH_OPTCR) & MG_BIT(31) ? 2 : 1;
}

static void flash_unlock(void) {
  static bool unlocked = false;
  if (unlocked == false) {
    MG_REG(FLASH_KEYR) = 0x45670123;
    MG_REG(FLASH_KEYR) = 0Xcdef89ab;
    MG_REG(FLASH_OPTKEYR) = 0x08192a3b;
    MG_REG(FLASH_OPTKEYR) = 0x4c5d6e7f;
    unlocked = true;
  }
}

static int flash_page_start(volatile uint32_t *dst) {
  char *base = (char *) mg_flash_start(), *end = base + mg_flash_size();
  volatile char *p = (char *) dst;
  return p >= base && p < end && ((p - base) % mg_flash_sector_size()) == 0;
}

static bool flash_is_err(void) {
  return MG_REG(FLASH_NSSR) & ((MG_BIT(8) - 1) << 17);  // RM0481 7.11.9
}

static void flash_wait(void) {
  while ((MG_REG(FLASH_NSSR) & MG_BIT(0)) &&
         (MG_REG(FLASH_NSSR) & MG_BIT(16)) == 0) {
    (void) 0;
  }
}

static void flash_clear_err(void) {
  flash_wait();                                    // Wait until ready
  MG_REG(FLASH_NSCCR) = ((MG_BIT(9) - 1) << 16U);  // Clear all errors
}

static bool flash_bank_is_swapped(void) {
  return MG_REG(FLASH_OPTCR) & MG_BIT(31);  // RM0481 7.11.8
}

bool mg_flash_erase(void *location) {
  bool ok = false;
  if (flash_page_start(location) == false) {
    MG_ERROR(("%p is not on a sector boundary"));
  } else {
    uintptr_t diff = (char *) location - (char *) mg_flash_start();
    uint32_t sector = diff / mg_flash_sector_size();
    flash_unlock();
    flash_clear_err();
    MG_REG(FLASH_NSCR) = 0;
    if ((sector < 128 && flash_bank_is_swapped()) ||
        (sector > 127 && !flash_bank_is_swapped())) {
      MG_REG(FLASH_NSCR) |= MG_BIT(31);  // Set FLASH_CR_BKSEL
    }
    if (sector > 127) sector -= 128;
    MG_REG(FLASH_NSCR) |= MG_BIT(2) | (sector << 6);  // Erase | sector_num
    MG_REG(FLASH_NSCR) |= MG_BIT(5);                  // Start erasing
    flash_wait();
    ok = !flash_is_err();
    MG_DEBUG(("Erase sector %lu @ %p: %s. CR %#lx SR %#lx", sector, location,
              ok ? "ok" : "fail", MG_REG(FLASH_NSCR), MG_REG(FLASH_NSSR)));
    // mg_hexdump(location, 32);
  }
  return ok;
}

bool mg_flash_swap_bank(void) {
  uint32_t desired = flash_bank_is_swapped() ? 0 : MG_BIT(31);
  flash_unlock();
  flash_clear_err();
  // printf("OPTSR_PRG 1 %#lx\n", FLASH->OPTSR_PRG);
  MG_SET_BITS(MG_REG(FLASH_OPTSR_PRG), MG_BIT(31), desired);
  // printf("OPTSR_PRG 2 %#lx\n", FLASH->OPTSR_PRG);
  MG_REG(FLASH_OPTCR) |= MG_BIT(1);  // OPTSTART
  while ((MG_REG(FLASH_OPTSR_CUR) & MG_BIT(31)) != desired) (void) 0;
  return true;
}

bool mg_flash_write(void *addr, const void *buf, size_t len) {
  if ((len % mg_flash_write_align()) != 0) {
    MG_ERROR(("%lu is not aligned to %lu", len, mg_flash_write_align()));
    return false;
  }
  uint32_t *dst = (uint32_t *) addr;
  uint32_t *src = (uint32_t *) buf;
  uint32_t *end = (uint32_t *) ((char *) buf + len);
  bool ok = true;
  flash_unlock();
  flash_clear_err();
  MG_ARM_DISABLE_IRQ();
  // MG_DEBUG(("Starting flash write %lu bytes @ %p", len, addr));
  while (ok && src < end) {
    if (flash_page_start(dst) && mg_flash_erase(dst) == false) break;
    MG_REG(FLASH_NSCR) = MG_BIT(1);  // Set programming flag
    *(volatile uint32_t *) dst++ = *src++;
    flash_wait();
    if (flash_is_err()) ok = false;
  }
  MG_DEBUG(("Flash write %lu bytes @ %p: %s. CR %#lx SR %#lx", len, dst,
            flash_is_err() ? "fail" : "ok", MG_REG(FLASH_NSCR),
            MG_REG(FLASH_NSSR)));
  if (flash_is_err()) ok = false;
  // mg_hexdump(addr, len > 32 ? 32 : len);
  //  MG_REG(FLASH_NSCR) &= ~MG_BIT(1);  // Set programming flag
  MG_REG(FLASH_NSCR) = 0;  // Clear flags
  MG_ARM_ENABLE_IRQ();
  return ok;
}

void mg_device_reset(void) {
  // SCB->AIRCR = ((0x5fa << SCB_AIRCR_VECTKEY_Pos)|SCB_AIRCR_SYSRESETREQ_Msk);
  *(volatile unsigned long *) 0xe000ed0c = 0x5fa0004;
}
#endif

#ifdef MG_ENABLE_LINES
#line 1 "src/device_stm32h7.c"
#endif



#if MG_DEVICE == MG_DEVICE_STM32H7

#define FLASH_BASE1 0x52002000  // Base address for bank1
#define FLASH_BASE2 0x52002100  // Base address for bank2
#define FLASH_KEYR 0x04         // See RM0433 4.9.2
#define FLASH_OPTKEYR 0x08
#define FLASH_OPTCR 0x18
#define FLASH_SR 0x10
#define FLASH_CR 0x0c
#define FLASH_CCR 0x14
#define FLASH_OPTSR_CUR 0x1c
#define FLASH_OPTSR_PRG 0x20

void *mg_flash_start(void) {
  return (void *) 0x08000000;
}
size_t mg_flash_size(void) {
  return 2 * 1024 * 1024;  // 2Mb
}
size_t mg_flash_sector_size(void) {
  return 128 * 1024;  // 128k
}
size_t mg_flash_write_align(void) {
  return 32;  // 256 bit
}
int mg_flash_bank(void) {
  return MG_REG(FLASH_BASE1 + FLASH_OPTCR) & MG_BIT(31) ? 2 : 1;
}

static void flash_unlock(void) {
  static bool unlocked = false;
  if (unlocked == false) {
    MG_REG(FLASH_BASE1 + FLASH_KEYR) = 0x45670123;
    MG_REG(FLASH_BASE1 + FLASH_KEYR) = 0xcdef89ab;
    MG_REG(FLASH_BASE2 + FLASH_KEYR) = 0x45670123;
    MG_REG(FLASH_BASE2 + FLASH_KEYR) = 0xcdef89ab;
    MG_REG(FLASH_BASE1 + FLASH_OPTKEYR) = 0x08192a3b;  // opt reg is "shared"
    MG_REG(FLASH_BASE1 + FLASH_OPTKEYR) = 0x4c5d6e7f;  // thus unlock once
    unlocked = true;
  }
}

static bool flash_page_start(volatile uint32_t *dst) {
  char *base = (char *) mg_flash_start(), *end = base + mg_flash_size();
  volatile char *p = (char *) dst;
  return p >= base && p < end && ((p - base) % mg_flash_sector_size()) == 0;
}

static bool flash_is_err(uint32_t bank) {
  return MG_REG(bank + FLASH_SR) & ((MG_BIT(11) - 1) << 17);  // RM0433 4.9.5
}

static void flash_wait(uint32_t bank) {
  while (MG_REG(bank + FLASH_SR) & (MG_BIT(0) | MG_BIT(2))) (void) 0;
}

static void flash_clear_err(uint32_t bank) {
  flash_wait(bank);                                      // Wait until ready
  MG_REG(bank + FLASH_CCR) = ((MG_BIT(11) - 1) << 16U);  // Clear all errors
}

static bool flash_bank_is_swapped(uint32_t bank) {
  return MG_REG(bank + FLASH_OPTCR) & MG_BIT(31);  // RM0433 4.9.7
}

// Figure out flash bank based on the address
static uint32_t flash_bank(void *addr) {
  size_t ofs = (char *) addr - (char *) mg_flash_start();
  return ofs < mg_flash_size() / 2 ? FLASH_BASE1 : FLASH_BASE2;
}

bool mg_flash_erase(void *addr) {
  bool ok = false;
  if (flash_page_start(addr) == false) {
    MG_ERROR(("%p is not on a sector boundary", addr));
  } else {
    uintptr_t diff = (char *) addr - (char *) mg_flash_start();
    uint32_t sector = diff / mg_flash_sector_size();
    uint32_t bank = flash_bank(addr);

    flash_unlock();
    if (sector > 7) sector -= 8;
    // MG_INFO(("Erasing @ %p, sector %lu, bank %#x", addr, sector, bank));

    flash_clear_err(bank);
    MG_REG(bank + FLASH_CR) |= (sector & 7U) << 8U;  // Sector to erase
    MG_REG(bank + FLASH_CR) |= MG_BIT(2);            // Sector erase bit
    MG_REG(bank + FLASH_CR) |= MG_BIT(7);            // Start erasing
    ok = !flash_is_err(bank);
    MG_DEBUG(("Erase sector %lu @ %p %s. CR %#lx SR %#lx", sector, addr,
              ok ? "ok" : "fail", MG_REG(bank + FLASH_CR),
              MG_REG(bank + FLASH_SR)));
    // mg_hexdump(addr, 32);
  }
  return ok;
}

bool mg_flash_swap_bank() {
  uint32_t bank = FLASH_BASE1;
  uint32_t desired = flash_bank_is_swapped(bank) ? 0 : MG_BIT(31);
  flash_unlock();
  flash_clear_err(bank);
  // printf("OPTSR_PRG 1 %#lx\n", FLASH->OPTSR_PRG);
  MG_SET_BITS(MG_REG(bank + FLASH_OPTSR_PRG), MG_BIT(31), desired);
  // printf("OPTSR_PRG 2 %#lx\n", FLASH->OPTSR_PRG);
  MG_REG(bank + FLASH_OPTCR) |= MG_BIT(1);  // OPTSTART
  while ((MG_REG(bank + FLASH_OPTSR_CUR) & MG_BIT(31)) != desired) (void) 0;
  return true;
}

bool mg_flash_write(void *addr, const void *buf, size_t len) {
  if ((len % mg_flash_write_align()) != 0) {
    MG_ERROR(("%lu is not aligned to %lu", len, mg_flash_write_align()));
    return false;
  }
  uint32_t bank = flash_bank(addr);
  uint32_t *dst = (uint32_t *) addr;
  uint32_t *src = (uint32_t *) buf;
  uint32_t *end = (uint32_t *) ((char *) buf + len);
  bool ok = true;
  flash_unlock();
  flash_clear_err(bank);
  MG_ARM_DISABLE_IRQ();
  MG_REG(bank + FLASH_CR) = MG_BIT(1);  // Set programming flag
  // MG_INFO(("Writing flash @ %p, %lu bytes", addr, len));
  while (ok && src < end) {
    if (flash_page_start(dst) && mg_flash_erase(dst) == false) break;
    *(volatile uint32_t *) dst++ = *src++;
    flash_wait(bank);
    if (flash_is_err(bank)) ok = false;
  }
  MG_DEBUG(("Flash write %lu bytes @ %p: %s. CR %#lx SR %#lx", len, dst,
            ok ? "ok" : "fail", MG_REG(bank + FLASH_CR),
            MG_REG(bank + FLASH_SR)));
  // mg_hexdump(addr, len > 32 ? 32 : len);
  MG_REG(bank + FLASH_CR) &= ~MG_BIT(1);  // Clear programming flag
  MG_ARM_ENABLE_IRQ();
  return ok;
}

void mg_device_reset(void) {
  // SCB->AIRCR = ((0x5fa << SCB_AIRCR_VECTKEY_Pos)|SCB_AIRCR_SYSRESETREQ_Msk);
  *(volatile unsigned long *) 0xe000ed0c = 0x5fa0004;
}
#endif

#ifdef MG_ENABLE_LINES
#line 1 "src/dns.c"
#endif








struct dns_data {
  struct dns_data *next;
  struct mg_connection *c;
  uint64_t expire;
  uint16_t txnid;
};

static void mg_sendnsreq(struct mg_connection *, struct mg_str *, int,
                         struct mg_dns *, bool);

static void mg_dns_free(struct dns_data **head, struct dns_data *d) {
  LIST_DELETE(struct dns_data, head, d);
  free(d);
}

void mg_resolve_cancel(struct mg_connection *c) {
  struct dns_data *tmp, *d;
  struct dns_data **head = (struct dns_data **) &c->mgr->active_dns_requests;
  for (d = *head; d != NULL; d = tmp) {
    tmp = d->next;
    if (d->c == c) mg_dns_free(head, d);
  }
}

static size_t mg_dns_parse_name_depth(const uint8_t *s, size_t len, size_t ofs,
                                      char *to, size_t tolen, size_t j,
                                      int depth) {
  size_t i = 0;
  if (tolen > 0 && depth == 0) to[0] = '\0';
  if (depth > 5) return 0;
  // MG_INFO(("ofs %lx %x %x", (unsigned long) ofs, s[ofs], s[ofs + 1]));
  while (ofs + i + 1 < len) {
    size_t n = s[ofs + i];
    if (n == 0) {
      i++;
      break;
    }
    if (n & 0xc0) {
      size_t ptr = (((n & 0x3f) << 8) | s[ofs + i + 1]);  // 12 is hdr len
      // MG_INFO(("PTR %lx", (unsigned long) ptr));
      if (ptr + 1 < len && (s[ptr] & 0xc0) == 0 &&
          mg_dns_parse_name_depth(s, len, ptr, to, tolen, j, depth + 1) == 0)
        return 0;
      i += 2;
      break;
    }
    if (ofs + i + n + 1 >= len) return 0;
    if (j > 0) {
      if (j < tolen) to[j] = '.';
      j++;
    }
    if (j + n < tolen) memcpy(&to[j], &s[ofs + i + 1], n);
    j += n;
    i += n + 1;
    if (j < tolen) to[j] = '\0';  // Zero-terminate this chunk
    // MG_INFO(("--> [%s]", to));
  }
  if (tolen > 0) to[tolen - 1] = '\0';  // Make sure make sure it is nul-term
  return i;
}

static size_t mg_dns_parse_name(const uint8_t *s, size_t n, size_t ofs,
                                char *dst, size_t dstlen) {
  return mg_dns_parse_name_depth(s, n, ofs, dst, dstlen, 0, 0);
}

size_t mg_dns_parse_rr(const uint8_t *buf, size_t len, size_t ofs,
                       bool is_question, struct mg_dns_rr *rr) {
  const uint8_t *s = buf + ofs, *e = &buf[len];

  memset(rr, 0, sizeof(*rr));
  if (len < sizeof(struct mg_dns_header)) return 0;  // Too small
  if (len > 512) return 0;  //  Too large, we don't expect that
  if (s >= e) return 0;     //  Overflow

  if ((rr->nlen = (uint16_t) mg_dns_parse_name(buf, len, ofs, NULL, 0)) == 0)
    return 0;
  s += rr->nlen + 4;
  if (s > e) return 0;
  rr->atype = (uint16_t) (((uint16_t) s[-4] << 8) | s[-3]);
  rr->aclass = (uint16_t) (((uint16_t) s[-2] << 8) | s[-1]);
  if (is_question) return (size_t) (rr->nlen + 4);

  s += 6;
  if (s > e) return 0;
  rr->alen = (uint16_t) (((uint16_t) s[-2] << 8) | s[-1]);
  if (s + rr->alen > e) return 0;
  return (size_t) (rr->nlen + rr->alen + 10);
}

bool mg_dns_parse(const uint8_t *buf, size_t len, struct mg_dns_message *dm) {
  const struct mg_dns_header *h = (struct mg_dns_header *) buf;
  struct mg_dns_rr rr;
  size_t i, n, ofs = sizeof(*h);
  memset(dm, 0, sizeof(*dm));

  if (len < sizeof(*h)) return 0;                // Too small, headers dont fit
  if (mg_ntohs(h->num_questions) > 1) return 0;  // Sanity
  if (mg_ntohs(h->num_answers) > 15) return 0;   // Sanity
  dm->txnid = mg_ntohs(h->txnid);

  for (i = 0; i < mg_ntohs(h->num_questions); i++) {
    if ((n = mg_dns_parse_rr(buf, len, ofs, true, &rr)) == 0) return false;
    // MG_INFO(("Q %lu %lu %hu/%hu", ofs, n, rr.atype, rr.aclass));
    ofs += n;
  }
  for (i = 0; i < mg_ntohs(h->num_answers); i++) {
    if ((n = mg_dns_parse_rr(buf, len, ofs, false, &rr)) == 0) return false;
    // MG_INFO(("A -- %lu %lu %hu/%hu %s", ofs, n, rr.atype, rr.aclass,
    // dm->name));
    mg_dns_parse_name(buf, len, ofs, dm->name, sizeof(dm->name));
    ofs += n;

    if (rr.alen == 4 && rr.atype == 1 && rr.aclass == 1) {
      dm->addr.is_ip6 = false;
      memcpy(&dm->addr.ip, &buf[ofs - 4], 4);
      dm->resolved = true;
      break;  // Return success
    } else if (rr.alen == 16 && rr.atype == 28 && rr.aclass == 1) {
      dm->addr.is_ip6 = true;
      memcpy(&dm->addr.ip, &buf[ofs - 16], 16);
      dm->resolved = true;
      break;  // Return success
    }
  }
  return true;
}

static void dns_cb(struct mg_connection *c, int ev, void *ev_data,
                   void *fn_data) {
  struct dns_data *d, *tmp;
  struct dns_data **head = (struct dns_data **) &c->mgr->active_dns_requests;
  if (ev == MG_EV_POLL) {
    uint64_t now = *(uint64_t *) ev_data;
    for (d = *head; d != NULL; d = tmp) {
      tmp = d->next;
      // MG_DEBUG ("%lu %lu dns poll", d->expire, now));
      if (now > d->expire) mg_error(d->c, "DNS timeout");
    }
  } else if (ev == MG_EV_READ) {
    struct mg_dns_message dm;
    int resolved = 0;
    if (mg_dns_parse(c->recv.buf, c->recv.len, &dm) == false) {
      MG_ERROR(("Unexpected DNS response:"));
      mg_hexdump(c->recv.buf, c->recv.len);
    } else {
      // MG_VERBOSE(("%s %d", dm.name, dm.resolved));
      for (d = *head; d != NULL; d = tmp) {
        tmp = d->next;
        // MG_INFO(("d %p %hu %hu", d, d->txnid, dm.txnid));
        if (dm.txnid != d->txnid) continue;
        if (d->c->is_resolving) {
          if (dm.resolved) {
            dm.addr.port = d->c->rem.port;  // Save port
            d->c->rem = dm.addr;            // Copy resolved address
            MG_DEBUG(
                ("%lu %s is %M", d->c->id, dm.name, mg_print_ip, &d->c->rem));
            mg_connect_resolved(d->c);
#if MG_ENABLE_IPV6
          } else if (dm.addr.is_ip6 == false && dm.name[0] != '\0' &&
                     c->mgr->use_dns6 == false) {
            struct mg_str x = mg_str(dm.name);
            mg_sendnsreq(d->c, &x, c->mgr->dnstimeout, &c->mgr->dns6, true);
#endif
          } else {
            mg_error(d->c, "%s DNS lookup failed", dm.name);
          }
        } else {
          MG_ERROR(("%lu already resolved", d->c->id));
        }
        mg_dns_free(head, d);
        resolved = 1;
      }
    }
    if (!resolved) MG_ERROR(("stray DNS reply"));
    c->recv.len = 0;
  } else if (ev == MG_EV_CLOSE) {
    for (d = *head; d != NULL; d = tmp) {
      tmp = d->next;
      mg_error(d->c, "DNS error");
      mg_dns_free(head, d);
    }
  }
  (void) fn_data;
}

static bool mg_dns_send(struct mg_connection *c, const struct mg_str *name,
                        uint16_t txnid, bool ipv6) {
  struct {
    struct mg_dns_header header;
    uint8_t data[256];
  } pkt;
  size_t i, n;
  memset(&pkt, 0, sizeof(pkt));
  pkt.header.txnid = mg_htons(txnid);
  pkt.header.flags = mg_htons(0x100);
  pkt.header.num_questions = mg_htons(1);
  for (i = n = 0; i < sizeof(pkt.data) - 5; i++) {
    if (name->ptr[i] == '.' || i >= name->len) {
      pkt.data[n] = (uint8_t) (i - n);
      memcpy(&pkt.data[n + 1], name->ptr + n, i - n);
      n = i + 1;
    }
    if (i >= name->len) break;
  }
  memcpy(&pkt.data[n], "\x00\x00\x01\x00\x01", 5);  // A query
  n += 5;
  if (ipv6) pkt.data[n - 3] = 0x1c;  // AAAA query
  // memcpy(&pkt.data[n], "\xc0\x0c\x00\x1c\x00\x01", 6);  // AAAA query
  // n += 6;
  return mg_send(c, &pkt, sizeof(pkt.header) + n);
}

static void mg_sendnsreq(struct mg_connection *c, struct mg_str *name, int ms,
                         struct mg_dns *dnsc, bool ipv6) {
  struct dns_data *d = NULL;
  if (dnsc->url == NULL) {
    mg_error(c, "DNS server URL is NULL. Call mg_mgr_init()");
  } else if (dnsc->c == NULL) {
    dnsc->c = mg_connect(c->mgr, dnsc->url, NULL, NULL);
    if (dnsc->c != NULL) {
      dnsc->c->pfn = dns_cb;
      // dnsc->c->is_hexdumping = 1;
    }
  }
  if (dnsc->c == NULL) {
    mg_error(c, "resolver");
  } else if ((d = (struct dns_data *) calloc(1, sizeof(*d))) == NULL) {
    mg_error(c, "resolve OOM");
  } else {
    struct dns_data *reqs = (struct dns_data *) c->mgr->active_dns_requests;
    d->txnid = reqs ? (uint16_t) (reqs->txnid + 1) : 1;
    d->next = (struct dns_data *) c->mgr->active_dns_requests;
    c->mgr->active_dns_requests = d;
    d->expire = mg_millis() + (uint64_t) ms;
    d->c = c;
    c->is_resolving = 1;
    MG_VERBOSE(("%lu resolving %.*s @ %s, txnid %hu", c->id, (int) name->len,
                name->ptr, dnsc->url, d->txnid));
    if (!mg_dns_send(dnsc->c, name, d->txnid, ipv6)) {
      mg_error(dnsc->c, "DNS send");
    }
  }
}

void mg_resolve(struct mg_connection *c, const char *url) {
  struct mg_str host = mg_url_host(url);
  c->rem.port = mg_htons(mg_url_port(url));
  if (mg_aton(host, &c->rem)) {
    // host is an IP address, do not fire name resolution
    mg_connect_resolved(c);
  } else {
    // host is not an IP, send DNS resolution request
    struct mg_dns *dns = c->mgr->use_dns6 ? &c->mgr->dns6 : &c->mgr->dns4;
    mg_sendnsreq(c, &host, c->mgr->dnstimeout, dns, c->mgr->use_dns6);
  }
}

#ifdef MG_ENABLE_LINES
#line 1 "src/event.c"
#endif





void mg_call(struct mg_connection *c, int ev, void *ev_data) {
  // Run user-defined handler first, in order to give it an ability
  // to intercept processing (e.g. clean input buffer) before the
  // protocol handler kicks in
  if (c->fn != NULL) c->fn(c, ev, ev_data, c->fn_data);
  if (c->pfn != NULL) c->pfn(c, ev, ev_data, c->pfn_data);
}

void mg_error(struct mg_connection *c, const char *fmt, ...) {
  char buf[64];
  va_list ap;
  va_start(ap, fmt);
  mg_vsnprintf(buf, sizeof(buf), fmt, &ap);
  va_end(ap);
  MG_ERROR(("%lu %ld %s", c->id, c->fd, buf));
  c->is_closing = 1;             // Set is_closing before sending MG_EV_CALL
  mg_call(c, MG_EV_ERROR, buf);  // Let user handler to override it
}

#ifdef MG_ENABLE_LINES
#line 1 "src/fmt.c"
#endif




static bool is_digit(int c) {
  return c >= '0' && c <= '9';
}

static int addexp(char *buf, int e, int sign) {
  int n = 0;
  buf[n++] = 'e';
  buf[n++] = (char) sign;
  if (e > 400) return 0;
  if (e < 10) buf[n++] = '0';
  if (e >= 100) buf[n++] = (char) (e / 100 + '0'), e -= 100 * (e / 100);
  if (e >= 10) buf[n++] = (char) (e / 10 + '0'), e -= 10 * (e / 10);
  buf[n++] = (char) (e + '0');
  return n;
}

static int xisinf(double x) {
  union {
    double f;
    uint64_t u;
  } ieee754 = {x};
  return ((unsigned) (ieee754.u >> 32) & 0x7fffffff) == 0x7ff00000 &&
         ((unsigned) ieee754.u == 0);
}

static int xisnan(double x) {
  union {
    double f;
    uint64_t u;
  } ieee754 = {x};
  return ((unsigned) (ieee754.u >> 32) & 0x7fffffff) +
             ((unsigned) ieee754.u != 0) >
         0x7ff00000;
}

static size_t mg_dtoa(char *dst, size_t dstlen, double d, int width, bool tz) {
  char buf[40];
  int i, s = 0, n = 0, e = 0;
  double t, mul, saved;
  if (d == 0.0) return mg_snprintf(dst, dstlen, "%s", "0");
  if (xisinf(d)) return mg_snprintf(dst, dstlen, "%s", d > 0 ? "inf" : "-inf");
  if (xisnan(d)) return mg_snprintf(dst, dstlen, "%s", "nan");
  if (d < 0.0) d = -d, buf[s++] = '-';

  // Round
  saved = d;
  mul = 1.0;
  while (d >= 10.0 && d / mul >= 10.0) mul *= 10.0;
  while (d <= 1.0 && d / mul <= 1.0) mul /= 10.0;
  for (i = 0, t = mul * 5; i < width; i++) t /= 10.0;
  d += t;
  // Calculate exponent, and 'mul' for scientific representation
  mul = 1.0;
  while (d >= 10.0 && d / mul >= 10.0) mul *= 10.0, e++;
  while (d < 1.0 && d / mul < 1.0) mul /= 10.0, e--;
  // printf(" --> %g %d %g %g\n", saved, e, t, mul);

  if (e >= width && width > 1) {
    n = (int) mg_dtoa(buf, sizeof(buf), saved / mul, width, tz);
    // printf(" --> %.*g %d [%.*s]\n", 10, d / t, e, n, buf);
    n += addexp(buf + s + n, e, '+');
    return mg_snprintf(dst, dstlen, "%.*s", n, buf);
  } else if (e <= -width && width > 1) {
    n = (int) mg_dtoa(buf, sizeof(buf), saved / mul, width, tz);
    // printf(" --> %.*g %d [%.*s]\n", 10, d / mul, e, n, buf);
    n += addexp(buf + s + n, -e, '-');
    return mg_snprintf(dst, dstlen, "%.*s", n, buf);
  } else {
    for (i = 0, t = mul; t >= 1.0 && s + n < (int) sizeof(buf); i++) {
      int ch = (int) (d / t);
      if (n > 0 || ch > 0) buf[s + n++] = (char) (ch + '0');
      d -= ch * t;
      t /= 10.0;
    }
    // printf(" --> [%g] -> %g %g (%d) [%.*s]\n", saved, d, t, n, s + n, buf);
    if (n == 0) buf[s++] = '0';
    while (t >= 1.0 && n + s < (int) sizeof(buf)) buf[n++] = '0', t /= 10.0;
    if (s + n < (int) sizeof(buf)) buf[n + s++] = '.';
    // printf(" 1--> [%g] -> [%.*s]\n", saved, s + n, buf);
    for (i = 0, t = 0.1; s + n < (int) sizeof(buf) && n < width; i++) {
      int ch = (int) (d / t);
      buf[s + n++] = (char) (ch + '0');
      d -= ch * t;
      t /= 10.0;
    }
  }
  while (tz && n > 0 && buf[s + n - 1] == '0') n--;  // Trim trailing zeroes
  if (n > 0 && buf[s + n - 1] == '.') n--;           // Trim trailing dot
  n += s;
  if (n >= (int) sizeof(buf)) n = (int) sizeof(buf) - 1;
  buf[n] = '\0';
  return mg_snprintf(dst, dstlen, "%s", buf);
}

static size_t mg_lld(char *buf, int64_t val, bool is_signed, bool is_hex) {
  const char *letters = "0123456789abcdef";
  uint64_t v = (uint64_t) val;
  size_t s = 0, n, i;
  if (is_signed && val < 0) buf[s++] = '-', v = (uint64_t) (-val);
  // This loop prints a number in reverse order. I guess this is because we
  // write numbers from right to left: least significant digit comes last.
  // Maybe because we use Arabic numbers, and Arabs write RTL?
  if (is_hex) {
    for (n = 0; v; v >>= 4) buf[s + n++] = letters[v & 15];
  } else {
    for (n = 0; v; v /= 10) buf[s + n++] = letters[v % 10];
  }
  // Reverse a string
  for (i = 0; i < n / 2; i++) {
    char t = buf[s + i];
    buf[s + i] = buf[s + n - i - 1], buf[s + n - i - 1] = t;
  }
  if (val == 0) buf[n++] = '0';  // Handle special case
  return n + s;
}

static size_t scpy(void (*out)(char, void *), void *ptr, char *buf,
                          size_t len) {
  size_t i = 0;
  while (i < len && buf[i] != '\0') out(buf[i++], ptr);
  return i;
}

size_t mg_xprintf(void (*out)(char, void *), void *ptr, const char *fmt, ...) {
  size_t len = 0;
  va_list ap;
  va_start(ap, fmt);
  len = mg_vxprintf(out, ptr, fmt, &ap);
  va_end(ap);
  return len;
}

size_t mg_vxprintf(void (*out)(char, void *), void *param, const char *fmt,
                   va_list *ap) {
  size_t i = 0, n = 0;
  while (fmt[i] != '\0') {
    if (fmt[i] == '%') {
      size_t j, k, x = 0, is_long = 0, w = 0 /* width */, pr = ~0U /* prec */;
      char pad = ' ', minus = 0, c = fmt[++i];
      if (c == '#') x++, c = fmt[++i];
      if (c == '-') minus++, c = fmt[++i];
      if (c == '0') pad = '0', c = fmt[++i];
      while (is_digit(c)) w *= 10, w += (size_t) (c - '0'), c = fmt[++i];
      if (c == '.') {
        c = fmt[++i];
        if (c == '*') {
          pr = (size_t) va_arg(*ap, int);
          c = fmt[++i];
        } else {
          pr = 0;
          while (is_digit(c)) pr *= 10, pr += (size_t) (c - '0'), c = fmt[++i];
        }
      }
      while (c == 'h') c = fmt[++i];  // Treat h and hh as int
      if (c == 'l') {
        is_long++, c = fmt[++i];
        if (c == 'l') is_long++, c = fmt[++i];
      }
      if (c == 'p') x = 1, is_long = 1;
      if (c == 'd' || c == 'u' || c == 'x' || c == 'X' || c == 'p' ||
          c == 'g' || c == 'f') {
        bool s = (c == 'd'), h = (c == 'x' || c == 'X' || c == 'p');
        char tmp[40];
        size_t xl = x ? 2 : 0;
        if (c == 'g' || c == 'f') {
          double v = va_arg(*ap, double);
          if (pr == ~0U) pr = 6;
          k = mg_dtoa(tmp, sizeof(tmp), v, (int) pr, c == 'g');
        } else if (is_long == 2) {
          int64_t v = va_arg(*ap, int64_t);
          k = mg_lld(tmp, v, s, h);
        } else if (is_long == 1) {
          long v = va_arg(*ap, long);
          k = mg_lld(tmp, s ? (int64_t) v : (int64_t) (unsigned long) v, s, h);
        } else {
          int v = va_arg(*ap, int);
          k = mg_lld(tmp, s ? (int64_t) v : (int64_t) (unsigned) v, s, h);
        }
        for (j = 0; j < xl && w > 0; j++) w--;
        for (j = 0; pad == ' ' && !minus && k < w && j + k < w; j++)
          n += scpy(out, param, &pad, 1);
        n += scpy(out, param, (char *) "0x", xl);
        for (j = 0; pad == '0' && k < w && j + k < w; j++)
          n += scpy(out, param, &pad, 1);
        n += scpy(out, param, tmp, k);
        for (j = 0; pad == ' ' && minus && k < w && j + k < w; j++)
          n += scpy(out, param, &pad, 1);
      } else if (c == 'm' || c == 'M') {
        mg_pm_t f = va_arg(*ap, mg_pm_t);
        if (c == 'm') out('"', param);
        n += f(out, param, ap);
        if (c == 'm') n += 2, out('"', param);
      } else if (c == 'c') {
        int ch = va_arg(*ap, int);
        out((char) ch, param);
        n++;
      } else if (c == 's') {
        char *p = va_arg(*ap, char *);
        if (pr == ~0U) pr = p == NULL ? 0 : strlen(p);
        for (j = 0; !minus && pr < w && j + pr < w; j++)
          n += scpy(out, param, &pad, 1);
        n += scpy(out, param, p, pr);
        for (j = 0; minus && pr < w && j + pr < w; j++)
          n += scpy(out, param, &pad, 1);
      } else if (c == '%') {
        out('%', param);
        n++;
      } else {
        out('%', param);
        out(c, param);
        n += 2;
      }
      i++;
    } else {
      out(fmt[i], param), n++, i++;
    }
  }
  return n;
}

#ifdef MG_ENABLE_LINES
#line 1 "src/fs.c"
#endif



struct mg_fd *mg_fs_open(struct mg_fs *fs, const char *path, int flags) {
  struct mg_fd *fd = (struct mg_fd *) calloc(1, sizeof(*fd));
  if (fd != NULL) {
    fd->fd = fs->op(path, flags);
    fd->fs = fs;
    if (fd->fd == NULL) {
      free(fd);
      fd = NULL;
    }
  }
  return fd;
}

void mg_fs_close(struct mg_fd *fd) {
  if (fd != NULL) {
    fd->fs->cl(fd->fd);
    free(fd);
  }
}

char *mg_file_read(struct mg_fs *fs, const char *path, size_t *sizep) {
  struct mg_fd *fd;
  char *data = NULL;
  size_t size = 0;
  fs->st(path, &size, NULL);
  if ((fd = mg_fs_open(fs, path, MG_FS_READ)) != NULL) {
    data = (char *) calloc(1, size + 1);
    if (data != NULL) {
      if (fs->rd(fd->fd, data, size) != size) {
        free(data);
        data = NULL;
      } else {
        data[size] = '\0';
        if (sizep != NULL) *sizep = size;
      }
    }
    mg_fs_close(fd);
  }
  return data;
}

bool mg_file_write(struct mg_fs *fs, const char *path, const void *buf,
                   size_t len) {
  bool result = false;
  struct mg_fd *fd;
  char tmp[MG_PATH_MAX];
  mg_snprintf(tmp, sizeof(tmp), "%s..%d", path, rand());
  if ((fd = mg_fs_open(fs, tmp, MG_FS_WRITE)) != NULL) {
    result = fs->wr(fd->fd, buf, len) == len;
    mg_fs_close(fd);
    if (result) {
      fs->rm(path);
      fs->mv(tmp, path);
    } else {
      fs->rm(tmp);
    }
  }
  return result;
}

bool mg_file_printf(struct mg_fs *fs, const char *path, const char *fmt, ...) {
  va_list ap;
  char *data;
  bool result = false;
  va_start(ap, fmt);
  data = mg_vmprintf(fmt, &ap);
  va_end(ap);
  result = mg_file_write(fs, path, data, strlen(data));
  free(data);
  return result;
}

#ifdef MG_ENABLE_LINES
#line 1 "src/fs_fat.c"
#endif



#if MG_ENABLE_FATFS
#include <ff.h>

static int mg_days_from_epoch(int y, int m, int d) {
  y -= m <= 2;
  int era = y / 400;
  int yoe = y - era * 400;
  int doy = (153 * (m + (m > 2 ? -3 : 9)) + 2) / 5 + d - 1;
  int doe = yoe * 365 + yoe / 4 - yoe / 100 + doy;
  return era * 146097 + doe - 719468;
}

static time_t mg_timegm(const struct tm *t) {
  int year = t->tm_year + 1900;
  int month = t->tm_mon;  // 0-11
  if (month > 11) {
    year += month / 12;
    month %= 12;
  } else if (month < 0) {
    int years_diff = (11 - month) / 12;
    year -= years_diff;
    month += 12 * years_diff;
  }
  int x = mg_days_from_epoch(year, month + 1, t->tm_mday);
  return 60 * (60 * (24L * x + t->tm_hour) + t->tm_min) + t->tm_sec;
}

static time_t ff_time_to_epoch(uint16_t fdate, uint16_t ftime) {
  struct tm tm;
  memset(&tm, 0, sizeof(struct tm));
  tm.tm_sec = (ftime << 1) & 0x3e;
  tm.tm_min = ((ftime >> 5) & 0x3f);
  tm.tm_hour = ((ftime >> 11) & 0x1f);
  tm.tm_mday = (fdate & 0x1f);
  tm.tm_mon = ((fdate >> 5) & 0x0f) - 1;
  tm.tm_year = ((fdate >> 9) & 0x7f) + 80;
  return mg_timegm(&tm);
}

static int ff_stat(const char *path, size_t *size, time_t *mtime) {
  FILINFO fi;
  if (path[0] == '\0') {
    if (size) *size = 0;
    if (mtime) *mtime = 0;
    return MG_FS_DIR;
  } else if (f_stat(path, &fi) == 0) {
    if (size) *size = (size_t) fi.fsize;
    if (mtime) *mtime = ff_time_to_epoch(fi.fdate, fi.ftime);
    return MG_FS_READ | MG_FS_WRITE | ((fi.fattrib & AM_DIR) ? MG_FS_DIR : 0);
  } else {
    return 0;
  }
}

static void ff_list(const char *dir, void (*fn)(const char *, void *),
                    void *userdata) {
  DIR d;
  FILINFO fi;
  if (f_opendir(&d, dir) == FR_OK) {
    while (f_readdir(&d, &fi) == FR_OK && fi.fname[0] != '\0') {
      if (!strcmp(fi.fname, ".") || !strcmp(fi.fname, "..")) continue;
      fn(fi.fname, userdata);
    }
    f_closedir(&d);
  }
}

static void *ff_open(const char *path, int flags) {
  FIL f;
  unsigned char mode = FA_READ;
  if (flags & MG_FS_WRITE) mode |= FA_WRITE | FA_OPEN_ALWAYS | FA_OPEN_APPEND;
  if (f_open(&f, path, mode) == 0) {
    FIL *fp;
    if ((fp = calloc(1, sizeof(*fp))) != NULL) {
      memcpy(fp, &f, sizeof(*fp));
      return fp;
    }
  }
  return NULL;
}

static void ff_close(void *fp) {
  if (fp != NULL) {
    f_close((FIL *) fp);
    free(fp);
  }
}

static size_t ff_read(void *fp, void *buf, size_t len) {
  UINT n = 0, misalign = ((size_t) buf) & 3;
  if (misalign) {
    char aligned[4];
    f_read((FIL *) fp, aligned, len > misalign ? misalign : len, &n);
    memcpy(buf, aligned, n);
  } else {
    f_read((FIL *) fp, buf, len, &n);
  }
  return n;
}

static size_t ff_write(void *fp, const void *buf, size_t len) {
  UINT n = 0;
  return f_write((FIL *) fp, (char *) buf, len, &n) == FR_OK ? n : 0;
}

static size_t ff_seek(void *fp, size_t offset) {
  f_lseek((FIL *) fp, offset);
  return offset;
}

static bool ff_rename(const char *from, const char *to) {
  return f_rename(from, to) == FR_OK;
}

static bool ff_remove(const char *path) {
  return f_unlink(path) == FR_OK;
}

static bool ff_mkdir(const char *path) {
  return f_mkdir(path) == FR_OK;
}

struct mg_fs mg_fs_fat = {ff_stat,  ff_list, ff_open,   ff_close,  ff_read,
                          ff_write, ff_seek, ff_rename, ff_remove, ff_mkdir};
#endif

#ifdef MG_ENABLE_LINES
#line 1 "src/fs_packed.c"
#endif




struct packed_file {
  const char *data;
  size_t size;
  size_t pos;
};

#if MG_ENABLE_PACKED_FS
#else
const char *mg_unpack(const char *path, size_t *size, time_t *mtime) {
  *size = 0, *mtime = 0;
  (void) path;
  return NULL;
}
const char *mg_unlist(size_t no) {
  (void) no;
  return NULL;
}
#endif

struct mg_str mg_unpacked(const char *path) {
  size_t len = 0;
  const char *buf = mg_unpack(path, &len, NULL);
  return mg_str_n(buf, len);
}

static int is_dir_prefix(const char *prefix, size_t n, const char *path) {
  // MG_INFO(("[%.*s] [%s] %c", (int) n, prefix, path, path[n]));
  return n < strlen(path) && strncmp(prefix, path, n) == 0 &&
         (n == 0 || path[n] == '/' || path[n - 1] == '/');
}

static int packed_stat(const char *path, size_t *size, time_t *mtime) {
  const char *p;
  size_t i, n = strlen(path);
  if (mg_unpack(path, size, mtime)) return MG_FS_READ;  // Regular file
  // Scan all files. If `path` is a dir prefix for any of them, it's a dir
  for (i = 0; (p = mg_unlist(i)) != NULL; i++) {
    if (is_dir_prefix(path, n, p)) return MG_FS_DIR;
  }
  return 0;
}

static void packed_list(const char *dir, void (*fn)(const char *, void *),
                        void *userdata) {
  char buf[MG_PATH_MAX], tmp[sizeof(buf)];
  const char *path, *begin, *end;
  size_t i, n = strlen(dir);
  tmp[0] = '\0';  // Previously listed entry
  for (i = 0; (path = mg_unlist(i)) != NULL; i++) {
    if (!is_dir_prefix(dir, n, path)) continue;
    begin = &path[n + 1];
    end = strchr(begin, '/');
    if (end == NULL) end = begin + strlen(begin);
    mg_snprintf(buf, sizeof(buf), "%.*s", (int) (end - begin), begin);
    buf[sizeof(buf) - 1] = '\0';
    // If this entry has been already listed, skip
    // NOTE: we're assuming that file list is sorted alphabetically
    if (strcmp(buf, tmp) == 0) continue;
    fn(buf, userdata);  // Not yet listed, call user function
    strcpy(tmp, buf);   // And save this entry as listed
  }
}

static void *packed_open(const char *path, int flags) {
  size_t size = 0;
  const char *data = mg_unpack(path, &size, NULL);
  struct packed_file *fp = NULL;
  if (data == NULL) return NULL;
  if (flags & MG_FS_WRITE) return NULL;
  if ((fp = (struct packed_file *) calloc(1, sizeof(*fp))) != NULL) {
    fp->size = size;
    fp->data = data;
  }
  return (void *) fp;
}

static void packed_close(void *fp) {
  if (fp != NULL) free(fp);
}

static size_t packed_read(void *fd, void *buf, size_t len) {
  struct packed_file *fp = (struct packed_file *) fd;
  if (fp->pos + len > fp->size) len = fp->size - fp->pos;
  memcpy(buf, &fp->data[fp->pos], len);
  fp->pos += len;
  return len;
}

static size_t packed_write(void *fd, const void *buf, size_t len) {
  (void) fd, (void) buf, (void) len;
  return 0;
}

static size_t packed_seek(void *fd, size_t offset) {
  struct packed_file *fp = (struct packed_file *) fd;
  fp->pos = offset;
  if (fp->pos > fp->size) fp->pos = fp->size;
  return fp->pos;
}

static bool packed_rename(const char *from, const char *to) {
  (void) from, (void) to;
  return false;
}

static bool packed_remove(const char *path) {
  (void) path;
  return false;
}

static bool packed_mkdir(const char *path) {
  (void) path;
  return false;
}

struct mg_fs mg_fs_packed = {
    packed_stat,  packed_list, packed_open,   packed_close,  packed_read,
    packed_write, packed_seek, packed_rename, packed_remove, packed_mkdir};

#ifdef MG_ENABLE_LINES
#line 1 "src/fs_posix.c"
#endif


#if MG_ENABLE_FILE

#ifndef MG_STAT_STRUCT
#define MG_STAT_STRUCT stat
#endif

#ifndef MG_STAT_FUNC
#define MG_STAT_FUNC stat
#endif

static int p_stat(const char *path, size_t *size, time_t *mtime) {
#if !defined(S_ISDIR)
  MG_ERROR(("stat() API is not supported. %p %p %p", path, size, mtime));
  return 0;
#else
#if MG_ARCH == MG_ARCH_WIN32
  struct _stati64 st;
  wchar_t tmp[MG_PATH_MAX];
  MultiByteToWideChar(CP_UTF8, 0, path, -1, tmp, sizeof(tmp) / sizeof(tmp[0]));
  if (_wstati64(tmp, &st) != 0) return 0;
  // If path is a symlink, windows reports 0 in st.st_size.
  // Get a real file size by opening it and jumping to the end
  if (st.st_size == 0 && (st.st_mode & _S_IFREG)) {
    FILE *fp = _wfopen(tmp, L"rb");
    if (fp != NULL) {
      fseek(fp, 0, SEEK_END);
      if (ftell(fp) > 0) st.st_size = ftell(fp);  // Use _ftelli64 on win10+
      fclose(fp);
    }
  }
#else
  struct MG_STAT_STRUCT st;
  if (MG_STAT_FUNC(path, &st) != 0) return 0;
#endif
  if (size) *size = (size_t) st.st_size;
  if (mtime) *mtime = st.st_mtime;
  return MG_FS_READ | MG_FS_WRITE | (S_ISDIR(st.st_mode) ? MG_FS_DIR : 0);
#endif
}

#if MG_ARCH == MG_ARCH_WIN32
struct dirent {
  char d_name[MAX_PATH];
};

typedef struct win32_dir {
  HANDLE handle;
  WIN32_FIND_DATAW info;
  struct dirent result;
} DIR;

int gettimeofday(struct timeval *tv, void *tz) {
  FILETIME ft;
  unsigned __int64 tmpres = 0;

  if (tv != NULL) {
    GetSystemTimeAsFileTime(&ft);
    tmpres |= ft.dwHighDateTime;
    tmpres <<= 32;
    tmpres |= ft.dwLowDateTime;
    tmpres /= 10;  // convert into microseconds
    tmpres -= (int64_t) 11644473600000000;
    tv->tv_sec = (long) (tmpres / 1000000UL);
    tv->tv_usec = (long) (tmpres % 1000000UL);
  }
  (void) tz;
  return 0;
}

static int to_wchar(const char *path, wchar_t *wbuf, size_t wbuf_len) {
  int ret;
  char buf[MAX_PATH * 2], buf2[MAX_PATH * 2], *p;
  strncpy(buf, path, sizeof(buf));
  buf[sizeof(buf) - 1] = '\0';
  // Trim trailing slashes. Leave backslash for paths like "X:\"
  p = buf + strlen(buf) - 1;
  while (p > buf && p[-1] != ':' && (p[0] == '\\' || p[0] == '/')) *p-- = '\0';
  memset(wbuf, 0, wbuf_len * sizeof(wchar_t));
  ret = MultiByteToWideChar(CP_UTF8, 0, buf, -1, wbuf, (int) wbuf_len);
  // Convert back to Unicode. If doubly-converted string does not match the
  // original, something is fishy, reject.
  WideCharToMultiByte(CP_UTF8, 0, wbuf, (int) wbuf_len, buf2, sizeof(buf2),
                      NULL, NULL);
  if (strcmp(buf, buf2) != 0) {
    wbuf[0] = L'\0';
    ret = 0;
  }
  return ret;
}

DIR *opendir(const char *name) {
  DIR *d = NULL;
  wchar_t wpath[MAX_PATH];
  DWORD attrs;

  if (name == NULL) {
    SetLastError(ERROR_BAD_ARGUMENTS);
  } else if ((d = (DIR *) calloc(1, sizeof(*d))) == NULL) {
    SetLastError(ERROR_NOT_ENOUGH_MEMORY);
  } else {
    to_wchar(name, wpath, sizeof(wpath) / sizeof(wpath[0]));
    attrs = GetFileAttributesW(wpath);
    if (attrs != 0Xffffffff && (attrs & FILE_ATTRIBUTE_DIRECTORY)) {
      (void) wcscat(wpath, L"\\*");
      d->handle = FindFirstFileW(wpath, &d->info);
      d->result.d_name[0] = '\0';
    } else {
      free(d);
      d = NULL;
    }
  }
  return d;
}

int closedir(DIR *d) {
  int result = 0;
  if (d != NULL) {
    if (d->handle != INVALID_HANDLE_VALUE)
      result = FindClose(d->handle) ? 0 : -1;
    free(d);
  } else {
    result = -1;
    SetLastError(ERROR_BAD_ARGUMENTS);
  }
  return result;
}

struct dirent *readdir(DIR *d) {
  struct dirent *result = NULL;
  if (d != NULL) {
    memset(&d->result, 0, sizeof(d->result));
    if (d->handle != INVALID_HANDLE_VALUE) {
      result = &d->result;
      WideCharToMultiByte(CP_UTF8, 0, d->info.cFileName, -1, result->d_name,
                          sizeof(result->d_name), NULL, NULL);
      if (!FindNextFileW(d->handle, &d->info)) {
        FindClose(d->handle);
        d->handle = INVALID_HANDLE_VALUE;
      }
    } else {
      SetLastError(ERROR_FILE_NOT_FOUND);
    }
  } else {
    SetLastError(ERROR_BAD_ARGUMENTS);
  }
  return result;
}
#endif

static void p_list(const char *dir, void (*fn)(const char *, void *),
                   void *userdata) {
#if MG_ENABLE_DIRLIST
  struct dirent *dp;
  DIR *dirp;
  if ((dirp = (opendir(dir))) == NULL) return;
  while ((dp = readdir(dirp)) != NULL) {
    if (!strcmp(dp->d_name, ".") || !strcmp(dp->d_name, "..")) continue;
    fn(dp->d_name, userdata);
  }
  closedir(dirp);
#else
  (void) dir, (void) fn, (void) userdata;
#endif
}

static void *p_open(const char *path, int flags) {
#if MG_ARCH == MG_ARCH_WIN32
  const char *mode = flags == MG_FS_READ ? "rb" : "a+b";
  wchar_t b1[MG_PATH_MAX], b2[10];
  MultiByteToWideChar(CP_UTF8, 0, path, -1, b1, sizeof(b1) / sizeof(b1[0]));
  MultiByteToWideChar(CP_UTF8, 0, mode, -1, b2, sizeof(b2) / sizeof(b2[0]));
  return (void *) _wfopen(b1, b2);
#else
  const char *mode = flags == MG_FS_READ ? "rbe" : "a+be";  // e for CLOEXEC
  return (void *) fopen(path, mode);
#endif
}

static void p_close(void *fp) {
  fclose((FILE *) fp);
}

static size_t p_read(void *fp, void *buf, size_t len) {
  return fread(buf, 1, len, (FILE *) fp);
}

static size_t p_write(void *fp, const void *buf, size_t len) {
  return fwrite(buf, 1, len, (FILE *) fp);
}

static size_t p_seek(void *fp, size_t offset) {
#if (defined(_FILE_OFFSET_BITS) && _FILE_OFFSET_BITS == 64) ||  \
    (defined(_POSIX_C_SOURCE) && _POSIX_C_SOURCE >= 200112L) || \
    (defined(_XOPEN_SOURCE) && _XOPEN_SOURCE >= 600)
  if (fseeko((FILE *) fp, (off_t) offset, SEEK_SET) != 0) (void) 0;
#else
  if (fseek((FILE *) fp, (long) offset, SEEK_SET) != 0) (void) 0;
#endif
  return (size_t) ftell((FILE *) fp);
}

static bool p_rename(const char *from, const char *to) {
  return rename(from, to) == 0;
}

static bool p_remove(const char *path) {
  return remove(path) == 0;
}

static bool p_mkdir(const char *path) {
  return mkdir(path, 0775) == 0;
}

#else

static int p_stat(const char *path, size_t *size, time_t *mtime) {
  (void) path, (void) size, (void) mtime;
  return 0;
}
static void p_list(const char *path, void (*fn)(const char *, void *),
                   void *userdata) {
  (void) path, (void) fn, (void) userdata;
}
static void *p_open(const char *path, int flags) {
  (void) path, (void) flags;
  return NULL;
}
static void p_close(void *fp) {
  (void) fp;
}
static size_t p_read(void *fd, void *buf, size_t len) {
  (void) fd, (void) buf, (void) len;
  return 0;
}
static size_t p_write(void *fd, const void *buf, size_t len) {
  (void) fd, (void) buf, (void) len;
  return 0;
}
static size_t p_seek(void *fd, size_t offset) {
  (void) fd, (void) offset;
  return (size_t) ~0;
}
static bool p_rename(const char *from, const char *to) {
  (void) from, (void) to;
  return false;
}
static bool p_remove(const char *path) {
  (void) path;
  return false;
}
static bool p_mkdir(const char *path) {
  (void) path;
  return false;
}
#endif

struct mg_fs mg_fs_posix = {p_stat,  p_list, p_open,   p_close,  p_read,
                            p_write, p_seek, p_rename, p_remove, p_mkdir};

#ifdef MG_ENABLE_LINES
#line 1 "src/http.c"
#endif













bool mg_to_size_t(struct mg_str str, size_t *val);
bool mg_to_size_t(struct mg_str str, size_t *val) {
  size_t i = 0, max = (size_t) -1, max2 = max / 10, result = 0, ndigits = 0;
  while (i < str.len && (str.ptr[i] == ' ' || str.ptr[i] == '\t')) i++;
  if (i < str.len && str.ptr[i] == '-') return false;
  while (i < str.len && str.ptr[i] >= '0' && str.ptr[i] <= '9') {
    size_t digit = (size_t) (str.ptr[i] - '0');
    if (result > max2) return false;  // Overflow
    result *= 10;
    if (result > max - digit) return false;  // Overflow
    result += digit;
    i++, ndigits++;
  }
  while (i < str.len && (str.ptr[i] == ' ' || str.ptr[i] == '\t')) i++;
  if (ndigits == 0) return false;  // #2322: Content-Length = 1 * DIGIT
  if (i != str.len) return false;  // Ditto
  *val = (size_t) result;
  return true;
}

// Chunk deletion marker is the MSB in the "processed" counter
#define MG_DMARK ((size_t) 1 << (sizeof(size_t) * 8 - 1))

// Multipart POST example:
// --xyz
// Content-Disposition: form-data; name="val"
//
// abcdef
// --xyz
// Content-Disposition: form-data; name="foo"; filename="a.txt"
// Content-Type: text/plain
//
// hello world
//
// --xyz--
size_t mg_http_next_multipart(struct mg_str body, size_t ofs,
                              struct mg_http_part *part) {
  struct mg_str cd = mg_str_n("Content-Disposition", 19);
  const char *s = body.ptr;
  size_t b = ofs, h1, h2, b1, b2, max = body.len;

  // Init part params
  if (part != NULL) part->name = part->filename = part->body = mg_str_n(0, 0);

  // Skip boundary
  while (b + 2 < max && s[b] != '\r' && s[b + 1] != '\n') b++;
  if (b <= ofs || b + 2 >= max) return 0;
  // MG_INFO(("B: %zu %zu [%.*s]", ofs, b - ofs, (int) (b - ofs), s));

  // Skip headers
  h1 = h2 = b + 2;
  for (;;) {
    while (h2 + 2 < max && s[h2] != '\r' && s[h2 + 1] != '\n') h2++;
    if (h2 == h1) break;
    if (h2 + 2 >= max) return 0;
    // MG_INFO(("Header: [%.*s]", (int) (h2 - h1), &s[h1]));
    if (part != NULL && h1 + cd.len + 2 < h2 && s[h1 + cd.len] == ':' &&
        mg_ncasecmp(&s[h1], cd.ptr, cd.len) == 0) {
      struct mg_str v = mg_str_n(&s[h1 + cd.len + 2], h2 - (h1 + cd.len + 2));
      part->name = mg_http_get_header_var(v, mg_str_n("name", 4));
      part->filename = mg_http_get_header_var(v, mg_str_n("filename", 8));
    }
    h1 = h2 = h2 + 2;
  }
  b1 = b2 = h2 + 2;
  while (b2 + 2 + (b - ofs) + 2 < max && !(s[b2] == '\r' && s[b2 + 1] == '\n' &&
                                           memcmp(&s[b2 + 2], s, b - ofs) == 0))
    b2++;

  if (b2 + 2 >= max) return 0;
  if (part != NULL) part->body = mg_str_n(&s[b1], b2 - b1);
  // MG_INFO(("Body: [%.*s]", (int) (b2 - b1), &s[b1]));
  return b2 + 2;
}

void mg_http_bauth(struct mg_connection *c, const char *user,
                   const char *pass) {
  struct mg_str u = mg_str(user), p = mg_str(pass);
  size_t need = c->send.len + 36 + (u.len + p.len) * 2;
  if (c->send.size < need) mg_iobuf_resize(&c->send, need);
  if (c->send.size >= need) {
    size_t i, n = 0;
    char *buf = (char *) &c->send.buf[c->send.len];
    memcpy(buf, "Authorization: Basic ", 21);  // DON'T use mg_send!
    for (i = 0; i < u.len; i++) {
      n = mg_base64_update(((unsigned char *) u.ptr)[i], buf + 21, n);
    }
    if (p.len > 0) {
      n = mg_base64_update(':', buf + 21, n);
      for (i = 0; i < p.len; i++) {
        n = mg_base64_update(((unsigned char *) p.ptr)[i], buf + 21, n);
      }
    }
    n = mg_base64_final(buf + 21, n);
    c->send.len += 21 + (size_t) n + 2;
    memcpy(&c->send.buf[c->send.len - 2], "\r\n", 2);
  } else {
    MG_ERROR(("%lu oom %d->%d ", c->id, (int) c->send.size, (int) need));
  }
}

struct mg_str mg_http_var(struct mg_str buf, struct mg_str name) {
  struct mg_str k, v, result = mg_str_n(NULL, 0);
  while (mg_split(&buf, &k, &v, '&')) {
    if (name.len == k.len && mg_ncasecmp(name.ptr, k.ptr, k.len) == 0) {
      result = v;
      break;
    }
  }
  return result;
}

int mg_http_get_var(const struct mg_str *buf, const char *name, char *dst,
                    size_t dst_len) {
  int len;
  if (dst == NULL || dst_len == 0) {
    len = -2;  // Bad destination
  } else if (buf->ptr == NULL || name == NULL || buf->len == 0) {
    len = -1;  // Bad source
    dst[0] = '\0';
  } else {
    struct mg_str v = mg_http_var(*buf, mg_str(name));
    if (v.ptr == NULL) {
      len = -4;  // Name does not exist
    } else {
      len = mg_url_decode(v.ptr, v.len, dst, dst_len, 1);
      if (len < 0) len = -3;  // Failed to decode
    }
  }
  return len;
}

static bool isx(int c) {
  return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') ||
         (c >= 'A' && c <= 'F');
}

int mg_url_decode(const char *src, size_t src_len, char *dst, size_t dst_len,
                  int is_form_url_encoded) {
  size_t i, j;
  for (i = j = 0; i < src_len && j + 1 < dst_len; i++, j++) {
    if (src[i] == '%') {
      // Use `i + 2 < src_len`, not `i < src_len - 2`, note small src_len
      if (i + 2 < src_len && isx(src[i + 1]) && isx(src[i + 2])) {
        mg_unhex(src + i + 1, 2, (uint8_t *) &dst[j]);
        i += 2;
      } else {
        return -1;
      }
    } else if (is_form_url_encoded && src[i] == '+') {
      dst[j] = ' ';
    } else {
      dst[j] = src[i];
    }
  }
  if (j < dst_len) dst[j] = '\0';  // Null-terminate the destination
  return i >= src_len && j < dst_len ? (int) j : -1;
}

static bool isok(uint8_t c) {
  return c == '\n' || c == '\r' || c >= ' ';
}

int mg_http_get_request_len(const unsigned char *buf, size_t buf_len) {
  size_t i;
  for (i = 0; i < buf_len; i++) {
    if (!isok(buf[i])) return -1;
    if ((i > 0 && buf[i] == '\n' && buf[i - 1] == '\n') ||
        (i > 3 && buf[i] == '\n' && buf[i - 1] == '\r' && buf[i - 2] == '\n'))
      return (int) i + 1;
  }
  return 0;
}
struct mg_str *mg_http_get_header(struct mg_http_message *h, const char *name) {
  size_t i, n = strlen(name), max = sizeof(h->headers) / sizeof(h->headers[0]);
  for (i = 0; i < max && h->headers[i].name.len > 0; i++) {
    struct mg_str *k = &h->headers[i].name, *v = &h->headers[i].value;
    if (n == k->len && mg_ncasecmp(k->ptr, name, n) == 0) return v;
  }
  return NULL;
}

// Get character length. Used to parse method, URI, headers
static size_t clen(const char *s) {
  uint8_t c = *(uint8_t *) s;
  if (c > ' ' && c < '~') return 1;  // Usual ascii printed char
  if ((c & 0xe0) == 0xc0) return 2;  // 2-byte UTF8
  if ((c & 0xf0) == 0xe0) return 3;  // 3-byte UTF8
  if ((c & 0xf8) == 0xf0) return 4;  // 4-byte UTF8
  return 0;
}

// Skip until the newline. Return advanced `s`, or NULL on error
static const char *skiptorn(const char *s, const char *end, struct mg_str *v) {
  v->ptr = s;
  while (s < end && s[0] != '\n' && s[0] != '\r') s++, v->len++;  // To newline
  if (s >= end || (s[0] == '\r' && s[1] != '\n')) return NULL;    // Stray \r
  if (s < end && s[0] == '\r') s++;                               // Skip \r
  if (s >= end || *s++ != '\n') return NULL;                      // Skip \n
  return s;
}

static bool mg_http_parse_headers(const char *s, const char *end,
                                  struct mg_http_header *h, size_t max_hdrs) {
  size_t i, n;
  for (i = 0; i < max_hdrs; i++) {
    struct mg_str k = {NULL, 0}, v = {NULL, 0};
    if (s >= end) return false;
    if (s[0] == '\n' || (s[0] == '\r' && s[1] == '\n')) break;
    k.ptr = s;
    while (s < end && s[0] != ':' && (n = clen(s)) > 0) s += n, k.len += n;
    if (k.len == 0) return false;               // Empty name
    if (s >= end || *s++ != ':') return false;  // Invalid, not followed by :
    while (s < end && s[0] == ' ') s++;         // Skip spaces
    if ((s = skiptorn(s, end, &v)) == NULL) return false;
    while (v.len > 0 && v.ptr[v.len - 1] == ' ') v.len--;  // Trim spaces
    // MG_INFO(("--HH [%.*s] [%.*s]", (int) k.len, k.ptr, (int) v.len, v.ptr));
    h[i].name = k, h[i].value = v;  // Success. Assign values
  }
  return true;
}

int mg_http_parse(const char *s, size_t len, struct mg_http_message *hm) {
  int is_response, req_len = mg_http_get_request_len((unsigned char *) s, len);
  const char *end = s == NULL ? NULL : s + req_len, *qs;  // Cannot add to NULL
  struct mg_str *cl;
  size_t n;

  memset(hm, 0, sizeof(*hm));
  if (req_len <= 0) return req_len;

  hm->message.ptr = hm->head.ptr = s;
  hm->body.ptr = end;
  hm->head.len = (size_t) req_len;
  hm->message.len = hm->body.len = (size_t) -1;  // Set body length to infinite

  // Parse request line
  hm->method.ptr = s;
  while (s < end && (n = clen(s)) > 0) s += n, hm->method.len += n;
  while (s < end && s[0] == ' ') s++;  // Skip spaces
  hm->uri.ptr = s;
  while (s < end && (n = clen(s)) > 0) s += n, hm->uri.len += n;
  while (s < end && s[0] == ' ') s++;  // Skip spaces
  if ((s = skiptorn(s, end, &hm->proto)) == NULL) return false;

  // If URI contains '?' character, setup query string
  if ((qs = (const char *) memchr(hm->uri.ptr, '?', hm->uri.len)) != NULL) {
    hm->query.ptr = qs + 1;
    hm->query.len = (size_t) (&hm->uri.ptr[hm->uri.len] - (qs + 1));
    hm->uri.len = (size_t) (qs - hm->uri.ptr);
  }

  // Sanity check. Allow protocol/reason to be empty
  // Do this check after hm->method.len and hm->uri.len are finalised
  if (hm->method.len == 0 || hm->uri.len == 0) return -1;

  if (!mg_http_parse_headers(s, end, hm->headers,
                             sizeof(hm->headers) / sizeof(hm->headers[0])))
    return -1;  // error when parsing
  if ((cl = mg_http_get_header(hm, "Content-Length")) != NULL) {
    if (mg_to_size_t(*cl, &hm->body.len) == false) return -1;
    hm->message.len = (size_t) req_len + hm->body.len;
  }

  // mg_http_parse() is used to parse both HTTP requests and HTTP
  // responses. If HTTP response does not have Content-Length set, then
  // body is read until socket is closed, i.e. body.len is infinite (~0).
  //
  // For HTTP requests though, according to
  // http://tools.ietf.org/html/rfc7231#section-8.1.3,
  // only POST and PUT methods have defined body semantics.
  // Therefore, if Content-Length is not specified and methods are
  // not one of PUT or POST, set body length to 0.
  //
  // So, if it is HTTP request, and Content-Length is not set,
  // and method is not (PUT or POST) then reset body length to zero.
  is_response = mg_ncasecmp(hm->method.ptr, "HTTP/", 5) == 0;
  if (hm->body.len == (size_t) ~0 && !is_response &&
      mg_vcasecmp(&hm->method, "PUT") != 0 &&
      mg_vcasecmp(&hm->method, "POST") != 0) {
    hm->body.len = 0;
    hm->message.len = (size_t) req_len;
  }

  // The 204 (No content) responses also have 0 body length
  if (hm->body.len == (size_t) ~0 && is_response &&
      mg_vcasecmp(&hm->uri, "204") == 0) {
    hm->body.len = 0;
    hm->message.len = (size_t) req_len;
  }
  if (hm->message.len < (size_t) req_len) return -1;  // Overflow protection

  return req_len;
}

static void mg_http_vprintf_chunk(struct mg_connection *c, const char *fmt,
                                  va_list *ap) {
  size_t len = c->send.len;
  mg_send(c, "        \r\n", 10);
  mg_vxprintf(mg_pfn_iobuf, &c->send, fmt, ap);
  if (c->send.len >= len + 10) {
    mg_snprintf((char *) c->send.buf + len, 9, "%08lx", c->send.len - len - 10);
    c->send.buf[len + 8] = '\r';
    if (c->send.len == len + 10) c->is_resp = 0;  // Last chunk, reset marker
  }
  mg_send(c, "\r\n", 2);
}

void mg_http_printf_chunk(struct mg_connection *c, const char *fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  mg_http_vprintf_chunk(c, fmt, &ap);
  va_end(ap);
}

void mg_http_write_chunk(struct mg_connection *c, const char *buf, size_t len) {
  mg_printf(c, "%lx\r\n", (unsigned long) len);
  mg_send(c, buf, len);
  mg_send(c, "\r\n", 2);
  if (len == 0) c->is_resp = 0;
}

// clang-format off
static const char *mg_http_status_code_str(int status_code) {
  switch (status_code) {
    case 100: return "Continue";
    case 101: return "Switching Protocols";
    case 102: return "Processing";
    case 200: return "OK";
    case 201: return "Created";
    case 202: return "Accepted";
    case 203: return "Non-authoritative Information";
    case 204: return "No Content";
    case 205: return "Reset Content";
    case 206: return "Partial Content";
    case 207: return "Multi-Status";
    case 208: return "Already Reported";
    case 226: return "IM Used";
    case 300: return "Multiple Choices";
    case 301: return "Moved Permanently";
    case 302: return "Found";
    case 303: return "See Other";
    case 304: return "Not Modified";
    case 305: return "Use Proxy";
    case 307: return "Temporary Redirect";
    case 308: return "Permanent Redirect";
    case 400: return "Bad Request";
    case 401: return "Unauthorized";
    case 402: return "Payment Required";
    case 403: return "Forbidden";
    case 404: return "Not Found";
    case 405: return "Method Not Allowed";
    case 406: return "Not Acceptable";
    case 407: return "Proxy Authentication Required";
    case 408: return "Request Timeout";
    case 409: return "Conflict";
    case 410: return "Gone";
    case 411: return "Length Required";
    case 412: return "Precondition Failed";
    case 413: return "Payload Too Large";
    case 414: return "Request-URI Too Long";
    case 415: return "Unsupported Media Type";
    case 416: return "Requested Range Not Satisfiable";
    case 417: return "Expectation Failed";
    case 418: return "I'm a teapot";
    case 421: return "Misdirected Request";
    case 422: return "Unprocessable Entity";
    case 423: return "Locked";
    case 424: return "Failed Dependency";
    case 426: return "Upgrade Required";
    case 428: return "Precondition Required";
    case 429: return "Too Many Requests";
    case 431: return "Request Header Fields Too Large";
    case 444: return "Connection Closed Without Response";
    case 451: return "Unavailable For Legal Reasons";
    case 499: return "Client Closed Request";
    case 500: return "Internal Server Error";
    case 501: return "Not Implemented";
    case 502: return "Bad Gateway";
    case 503: return "Service Unavailable";
    case 504: return "Gateway Timeout";
    case 505: return "HTTP Version Not Supported";
    case 506: return "Variant Also Negotiates";
    case 507: return "Insufficient Storage";
    case 508: return "Loop Detected";
    case 510: return "Not Extended";
    case 511: return "Network Authentication Required";
    case 599: return "Network Connect Timeout Error";
    default: return "";
  }
}
// clang-format on

void mg_http_reply(struct mg_connection *c, int code, const char *headers,
                   const char *fmt, ...) {
  va_list ap;
  size_t len;
  mg_printf(c, "HTTP/1.1 %d %s\r\n%sContent-Length:            \r\n\r\n", code,
            mg_http_status_code_str(code), headers == NULL ? "" : headers);
  len = c->send.len;
  va_start(ap, fmt);
  mg_vxprintf(mg_pfn_iobuf, &c->send, fmt, &ap);
  va_end(ap);
  if (c->send.len > 16) {
    size_t n = mg_snprintf((char *) &c->send.buf[len - 15], 11, "%-10lu",
                           (unsigned long) (c->send.len - len));
    c->send.buf[len - 15 + n] = ' ';  // Change ending 0 to space
  }
  c->is_resp = 0;
}

static void http_cb(struct mg_connection *, int, void *, void *);
static void restore_http_cb(struct mg_connection *c) {
  mg_fs_close((struct mg_fd *) c->pfn_data);
  c->pfn_data = NULL;
  c->pfn = http_cb;
  c->is_resp = 0;
}

char *mg_http_etag(char *buf, size_t len, size_t size, time_t mtime);
char *mg_http_etag(char *buf, size_t len, size_t size, time_t mtime) {
  mg_snprintf(buf, len, "\"%lld.%lld\"", (int64_t) mtime, (int64_t) size);
  return buf;
}

static void static_cb(struct mg_connection *c, int ev, void *ev_data,
                      void *fn_data) {
  if (ev == MG_EV_WRITE || ev == MG_EV_POLL) {
    struct mg_fd *fd = (struct mg_fd *) fn_data;
    // Read to send IO buffer directly, avoid extra on-stack buffer
    size_t n, max = MG_IO_SIZE, space;
    size_t *cl = (size_t *) &c->data[(sizeof(c->data) - sizeof(size_t)) /
                                     sizeof(size_t) * sizeof(size_t)];
    if (c->send.size < max) mg_iobuf_resize(&c->send, max);
    if (c->send.len >= c->send.size) return;  // Rate limit
    if ((space = c->send.size - c->send.len) > *cl) space = *cl;
    n = fd->fs->rd(fd->fd, c->send.buf + c->send.len, space);
    c->send.len += n;
    *cl -= n;
    if (n == 0) restore_http_cb(c);
  } else if (ev == MG_EV_CLOSE) {
    restore_http_cb(c);
  }
  (void) ev_data;
}

// Known mime types. Keep it outside guess_content_type() function, since
// some environments don't like it defined there.
// clang-format off
static struct mg_str s_known_types[] = {
    MG_C_STR("html"), MG_C_STR("text/html; charset=utf-8"),
    MG_C_STR("htm"), MG_C_STR("text/html; charset=utf-8"),
    MG_C_STR("css"), MG_C_STR("text/css; charset=utf-8"),
    MG_C_STR("js"), MG_C_STR("text/javascript; charset=utf-8"),
    MG_C_STR("gif"), MG_C_STR("image/gif"),
    MG_C_STR("png"), MG_C_STR("image/png"),
    MG_C_STR("jpg"), MG_C_STR("image/jpeg"),
    MG_C_STR("jpeg"), MG_C_STR("image/jpeg"),
    MG_C_STR("woff"), MG_C_STR("font/woff"),
    MG_C_STR("ttf"), MG_C_STR("font/ttf"),
    MG_C_STR("svg"), MG_C_STR("image/svg+xml"),
    MG_C_STR("txt"), MG_C_STR("text/plain; charset=utf-8"),
    MG_C_STR("avi"), MG_C_STR("video/x-msvideo"),
    MG_C_STR("csv"), MG_C_STR("text/csv"),
    MG_C_STR("doc"), MG_C_STR("application/msword"),
    MG_C_STR("exe"), MG_C_STR("application/octet-stream"),
    MG_C_STR("gz"), MG_C_STR("application/gzip"),
    MG_C_STR("ico"), MG_C_STR("image/x-icon"),
    MG_C_STR("json"), MG_C_STR("application/json"),
    MG_C_STR("mov"), MG_C_STR("video/quicktime"),
    MG_C_STR("mp3"), MG_C_STR("audio/mpeg"),
    MG_C_STR("mp4"), MG_C_STR("video/mp4"),
    MG_C_STR("mpeg"), MG_C_STR("video/mpeg"),
    MG_C_STR("pdf"), MG_C_STR("application/pdf"),
    MG_C_STR("shtml"), MG_C_STR("text/html; charset=utf-8"),
    MG_C_STR("tgz"), MG_C_STR("application/tar-gz"),
    MG_C_STR("wav"), MG_C_STR("audio/wav"),
    MG_C_STR("webp"), MG_C_STR("image/webp"),
    MG_C_STR("zip"), MG_C_STR("application/zip"),
    MG_C_STR("3gp"), MG_C_STR("video/3gpp"),
    {0, 0},
};
// clang-format on

static struct mg_str guess_content_type(struct mg_str path, const char *extra) {
  struct mg_str k, v, s = mg_str(extra);
  size_t i = 0;

  // Shrink path to its extension only
  while (i < path.len && path.ptr[path.len - i - 1] != '.') i++;
  path.ptr += path.len - i;
  path.len = i;

  // Process user-provided mime type overrides, if any
  while (mg_commalist(&s, &k, &v)) {
    if (mg_strcmp(path, k) == 0) return v;
  }

  // Process built-in mime types
  for (i = 0; s_known_types[i].ptr != NULL; i += 2) {
    if (mg_strcmp(path, s_known_types[i]) == 0) return s_known_types[i + 1];
  }

  return mg_str("text/plain; charset=utf-8");
}

static int getrange(struct mg_str *s, size_t *a, size_t *b) {
  size_t i, numparsed = 0;
  for (i = 0; i + 6 < s->len; i++) {
    struct mg_str k, v = mg_str_n(s->ptr + i + 6, s->len - i - 6);
    if (memcmp(&s->ptr[i], "bytes=", 6) != 0) continue;
    if (mg_split(&v, &k, NULL, '-')) {
      if (mg_to_size_t(k, a)) numparsed++;
      if (v.len > 0 && mg_to_size_t(v, b)) numparsed++;
    } else {
      if (mg_to_size_t(v, a)) numparsed++;
    }
    break;
  }
  return (int) numparsed;
}

void mg_http_serve_file(struct mg_connection *c, struct mg_http_message *hm,
                        const char *path,
                        const struct mg_http_serve_opts *opts) {
  char etag[64], tmp[MG_PATH_MAX];
  struct mg_fs *fs = opts->fs == NULL ? &mg_fs_posix : opts->fs;
  struct mg_fd *fd = NULL;
  size_t size = 0;
  time_t mtime = 0;
  struct mg_str *inm = NULL;
  struct mg_str mime = guess_content_type(mg_str(path), opts->mime_types);
  bool gzip = false;

  if (path != NULL) {
    // If a browser sends us "Accept-Encoding: gzip", try to open .gz first
    struct mg_str *ae = mg_http_get_header(hm, "Accept-Encoding");
    if (ae != NULL && mg_strstr(*ae, mg_str("gzip")) != NULL) {
      mg_snprintf(tmp, sizeof(tmp), "%s.gz", path);
      fd = mg_fs_open(fs, tmp, MG_FS_READ);
      if (fd != NULL) gzip = true, path = tmp;
    }
    // No luck opening .gz? Open what we've told to open
    if (fd == NULL) fd = mg_fs_open(fs, path, MG_FS_READ);
  }

  // Failed to open, and page404 is configured? Open it, then
  if (fd == NULL && opts->page404 != NULL) {
    fd = mg_fs_open(fs, opts->page404, MG_FS_READ);
    mime = guess_content_type(mg_str(path), opts->mime_types);
    path = opts->page404;
  }

  if (fd == NULL || fs->st(path, &size, &mtime) == 0) {
    mg_http_reply(c, 404, opts->extra_headers, "Not found\n");
    mg_fs_close(fd);
    // NOTE: mg_http_etag() call should go first!
  } else if (mg_http_etag(etag, sizeof(etag), size, mtime) != NULL &&
             (inm = mg_http_get_header(hm, "If-None-Match")) != NULL &&
             mg_vcasecmp(inm, etag) == 0) {
    mg_fs_close(fd);
    mg_http_reply(c, 304, opts->extra_headers, "");
  } else {
    int n, status = 200;
    char range[100];
    size_t r1 = 0, r2 = 0, cl = size;

    // Handle Range header
    struct mg_str *rh = mg_http_get_header(hm, "Range");
    range[0] = '\0';
    if (rh != NULL && (n = getrange(rh, &r1, &r2)) > 0) {
      // If range is specified like "400-", set second limit to content len
      if (n == 1) r2 = cl - 1;
      if (r1 > r2 || r2 >= cl) {
        status = 416;
        cl = 0;
        mg_snprintf(range, sizeof(range), "Content-Range: bytes */%lld\r\n",
                    (int64_t) size);
      } else {
        status = 206;
        cl = r2 - r1 + 1;
        mg_snprintf(range, sizeof(range),
                    "Content-Range: bytes %llu-%llu/%llu\r\n", (uint64_t) r1,
                    (uint64_t) (r1 + cl - 1), (uint64_t) size);
        fs->sk(fd->fd, r1);
      }
    }
    mg_printf(c,
              "HTTP/1.1 %d %s\r\n"
              "Content-Type: %.*s\r\n"
              "Etag: %s\r\n"
              "Content-Length: %llu\r\n"
              "%s%s%s\r\n",
              status, mg_http_status_code_str(status), (int) mime.len, mime.ptr,
              etag, (uint64_t) cl, gzip ? "Content-Encoding: gzip\r\n" : "",
              range, opts->extra_headers ? opts->extra_headers : "");
    if (mg_vcasecmp(&hm->method, "HEAD") == 0) {
      c->is_draining = 1;
      c->is_resp = 0;
      mg_fs_close(fd);
    } else {
      // Track to-be-sent content length at the end of c->data, aligned
      size_t *clp = (size_t *) &c->data[(sizeof(c->data) - sizeof(size_t)) /
                                        sizeof(size_t) * sizeof(size_t)];
      c->pfn = static_cb;
      c->pfn_data = fd;
      *clp = cl;
    }
  }
}

struct printdirentrydata {
  struct mg_connection *c;
  struct mg_http_message *hm;
  const struct mg_http_serve_opts *opts;
  const char *dir;
};

#if MG_ENABLE_DIRLIST
static void printdirentry(const char *name, void *userdata) {
  struct printdirentrydata *d = (struct printdirentrydata *) userdata;
  struct mg_fs *fs = d->opts->fs == NULL ? &mg_fs_posix : d->opts->fs;
  size_t size = 0;
  time_t t = 0;
  char path[MG_PATH_MAX], sz[40], mod[40];
  int flags, n = 0;

  // MG_DEBUG(("[%s] [%s]", d->dir, name));
  if (mg_snprintf(path, sizeof(path), "%s%c%s", d->dir, '/', name) >
      sizeof(path)) {
    MG_ERROR(("%s truncated", name));
  } else if ((flags = fs->st(path, &size, &t)) == 0) {
    MG_ERROR(("%lu stat(%s): %d", d->c->id, path, errno));
  } else {
    const char *slash = flags & MG_FS_DIR ? "/" : "";
    if (flags & MG_FS_DIR) {
      mg_snprintf(sz, sizeof(sz), "%s", "[DIR]");
    } else {
      mg_snprintf(sz, sizeof(sz), "%lld", (uint64_t) size);
    }
#if defined(MG_HTTP_DIRLIST_TIME_FMT)
    {
      char time_str[40];
      struct tm *time_info = localtime(&t);
      strftime(time_str, sizeof time_str, "%Y/%m/%d %H:%M:%S", time_info);
      mg_snprintf(mod, sizeof(mod), "%s", time_str);
    }
#else
    mg_snprintf(mod, sizeof(mod), "%lu", (unsigned long) t);
#endif
    n = (int) mg_url_encode(name, strlen(name), path, sizeof(path));
    mg_printf(d->c,
              "  <tr><td><a href=\"%.*s%s\">%s%s</a></td>"
              "<td name=%lu>%s</td><td name=%lld>%s</td></tr>\n",
              n, path, slash, name, slash, (unsigned long) t, mod,
              flags & MG_FS_DIR ? (int64_t) -1 : (int64_t) size, sz);
  }
}

static void listdir(struct mg_connection *c, struct mg_http_message *hm,
                    const struct mg_http_serve_opts *opts, char *dir) {
  const char *sort_js_code =
      "<script>function srt(tb, sc, so, d) {"
      "var tr = Array.prototype.slice.call(tb.rows, 0),"
      "tr = tr.sort(function (a, b) { var c1 = a.cells[sc], c2 = b.cells[sc],"
      "n1 = c1.getAttribute('name'), n2 = c2.getAttribute('name'), "
      "t1 = a.cells[2].getAttribute('name'), "
      "t2 = b.cells[2].getAttribute('name'); "
      "return so * (t1 < 0 && t2 >= 0 ? -1 : t2 < 0 && t1 >= 0 ? 1 : "
      "n1 ? parseInt(n2) - parseInt(n1) : "
      "c1.textContent.trim().localeCompare(c2.textContent.trim())); });";
  const char *sort_js_code2 =
      "for (var i = 0; i < tr.length; i++) tb.appendChild(tr[i]); "
      "if (!d) window.location.hash = ('sc=' + sc + '&so=' + so); "
      "};"
      "window.onload = function() {"
      "var tb = document.getElementById('tb');"
      "var m = /sc=([012]).so=(1|-1)/.exec(window.location.hash) || [0, 2, 1];"
      "var sc = m[1], so = m[2]; document.onclick = function(ev) { "
      "var c = ev.target.rel; if (c) {if (c == sc) so *= -1; srt(tb, c, so); "
      "sc = c; ev.preventDefault();}};"
      "srt(tb, sc, so, true);"
      "}"
      "</script>";
  struct mg_fs *fs = opts->fs == NULL ? &mg_fs_posix : opts->fs;
  struct printdirentrydata d = {c, hm, opts, dir};
  char tmp[10], buf[MG_PATH_MAX];
  size_t off, n;
  int len = mg_url_decode(hm->uri.ptr, hm->uri.len, buf, sizeof(buf), 0);
  struct mg_str uri = len > 0 ? mg_str_n(buf, (size_t) len) : hm->uri;

  mg_printf(c,
            "HTTP/1.1 200 OK\r\n"
            "Content-Type: text/html; charset=utf-8\r\n"
            "%s"
            "Content-Length:         \r\n\r\n",
            opts->extra_headers == NULL ? "" : opts->extra_headers);
  off = c->send.len;  // Start of body
  mg_printf(c,
            "<!DOCTYPE html><html><head><title>Index of %.*s</title>%s%s"
            "<style>th,td {text-align: left; padding-right: 1em; "
            "font-family: monospace; }</style></head>"
            "<body><h1>Index of %.*s</h1><table cellpadding=\"0\"><thead>"
            "<tr><th><a href=\"#\" rel=\"0\">Name</a></th><th>"
            "<a href=\"#\" rel=\"1\">Modified</a></th>"
            "<th><a href=\"#\" rel=\"2\">Size</a></th></tr>"
            "<tr><td colspan=\"3\"><hr></td></tr>"
            "</thead>"
            "<tbody id=\"tb\">\n",
            (int) uri.len, uri.ptr, sort_js_code, sort_js_code2, (int) uri.len,
            uri.ptr);
  mg_printf(c, "%s",
            "  <tr><td><a href=\"..\">..</a></td>"
            "<td name=-1></td><td name=-1>[DIR]</td></tr>\n");

  fs->ls(dir, printdirentry, &d);
  mg_printf(c,
            "</tbody><tfoot><tr><td colspan=\"3\"><hr></td></tr></tfoot>"
            "</table><address>Mongoose v.%s</address></body></html>\n",
            MG_VERSION);
  n = mg_snprintf(tmp, sizeof(tmp), "%lu", (unsigned long) (c->send.len - off));
  if (n > sizeof(tmp)) n = 0;
  memcpy(c->send.buf + off - 12, tmp, n);  // Set content length
  c->is_resp = 0;                          // Mark response end
}
#endif

// Resolve requested file into `path` and return its fs->st() result
static int uri_to_path2(struct mg_connection *c, struct mg_http_message *hm,
                        struct mg_fs *fs, struct mg_str url, struct mg_str dir,
                        char *path, size_t path_size) {
  int flags, tmp;
  // Append URI to the root_dir, and sanitize it
  size_t n = mg_snprintf(path, path_size, "%.*s", (int) dir.len, dir.ptr);
  if (n + 2 >= path_size) {
    mg_http_reply(c, 400, "", "Exceeded path size");
    return -1;
  }
  path[path_size - 1] = '\0';
  // Terminate root dir with slash
  if (n > 0 && path[n - 1] != '/') path[n++] = '/', path[n] = '\0';
  if (url.len < hm->uri.len) {
    mg_url_decode(hm->uri.ptr + url.len, hm->uri.len - url.len, path + n,
                  path_size - n, 0);
  }
  path[path_size - 1] = '\0';  // Double-check
  if (!mg_path_is_sane(path)) {
    mg_http_reply(c, 400, "", "Invalid path");
    return -1;
  }
  n = strlen(path);
  while (n > 1 && path[n - 1] == '/') path[--n] = 0;  // Trim trailing slashes
  flags = mg_vcmp(&hm->uri, "/") == 0 ? MG_FS_DIR : fs->st(path, NULL, NULL);
  MG_VERBOSE(("%lu %.*s -> %s %d", c->id, (int) hm->uri.len, hm->uri.ptr, path,
              flags));
  if (flags == 0) {
    // Do nothing - let's caller decide
  } else if ((flags & MG_FS_DIR) && hm->uri.len > 0 &&
             hm->uri.ptr[hm->uri.len - 1] != '/') {
    mg_printf(c,
              "HTTP/1.1 301 Moved\r\n"
              "Location: %.*s/\r\n"
              "Content-Length: 0\r\n"
              "\r\n",
              (int) hm->uri.len, hm->uri.ptr);
    c->is_resp = 0;
    flags = -1;
  } else if (flags & MG_FS_DIR) {
    if (((mg_snprintf(path + n, path_size - n, "/" MG_HTTP_INDEX) > 0 &&
          (tmp = fs->st(path, NULL, NULL)) != 0) ||
         (mg_snprintf(path + n, path_size - n, "/index.shtml") > 0 &&
          (tmp = fs->st(path, NULL, NULL)) != 0))) {
      flags = tmp;
    } else if ((mg_snprintf(path + n, path_size - n, "/" MG_HTTP_INDEX ".gz") >
                    0 &&
                (tmp = fs->st(path, NULL, NULL)) !=
                    0)) {  // check for gzipped index
      flags = tmp;
      path[n + 1 + strlen(MG_HTTP_INDEX)] =
          '\0';  // Remove appended .gz in index file name
    } else {
      path[n] = '\0';  // Remove appended index file name
    }
  }
  return flags;
}

static int uri_to_path(struct mg_connection *c, struct mg_http_message *hm,
                       const struct mg_http_serve_opts *opts, char *path,
                       size_t path_size) {
  struct mg_fs *fs = opts->fs == NULL ? &mg_fs_posix : opts->fs;
  struct mg_str k, v, s = mg_str(opts->root_dir), u = {0, 0}, p = {0, 0};
  while (mg_commalist(&s, &k, &v)) {
    if (v.len == 0) v = k, k = mg_str("/"), u = k, p = v;
    if (hm->uri.len < k.len) continue;
    if (mg_strcmp(k, mg_str_n(hm->uri.ptr, k.len)) != 0) continue;
    u = k, p = v;
  }
  return uri_to_path2(c, hm, fs, u, p, path, path_size);
}

void mg_http_serve_dir(struct mg_connection *c, struct mg_http_message *hm,
                       const struct mg_http_serve_opts *opts) {
  char path[MG_PATH_MAX];
  const char *sp = opts->ssi_pattern;
  int flags = uri_to_path(c, hm, opts, path, sizeof(path));
  if (flags < 0) {
    // Do nothing: the response has already been sent by uri_to_path()
  } else if (flags & MG_FS_DIR) {
#if MG_ENABLE_DIRLIST
    listdir(c, hm, opts, path);
#else
    mg_http_reply(c, 403, "", "Forbidden\n");
#endif
  } else if (flags && sp != NULL &&
             mg_globmatch(sp, strlen(sp), path, strlen(path))) {
    mg_http_serve_ssi(c, opts->root_dir, path);
  } else {
    mg_http_serve_file(c, hm, path, opts);
  }
}

static bool mg_is_url_safe(int c) {
  return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'z') ||
         (c >= 'A' && c <= 'Z') || c == '.' || c == '_' || c == '-' || c == '~';
}

size_t mg_url_encode(const char *s, size_t sl, char *buf, size_t len) {
  size_t i, n = 0;
  for (i = 0; i < sl; i++) {
    int c = *(unsigned char *) &s[i];
    if (n + 4 >= len) return 0;
    if (mg_is_url_safe(c)) {
      buf[n++] = s[i];
    } else {
      buf[n++] = '%';
      mg_hex(&s[i], 1, &buf[n]);
      n += 2;
    }
  }
  if (len > 0 && n < len - 1) buf[n] = '\0';  // Null-terminate the destination
  if (len > 0) buf[len - 1] = '\0';           // Always.
  return n;
}

void mg_http_creds(struct mg_http_message *hm, char *user, size_t userlen,
                   char *pass, size_t passlen) {
  struct mg_str *v = mg_http_get_header(hm, "Authorization");
  user[0] = pass[0] = '\0';
  if (v != NULL && v->len > 6 && memcmp(v->ptr, "Basic ", 6) == 0) {
    char buf[256];
    size_t n = mg_base64_decode(v->ptr + 6, v->len - 6, buf, sizeof(buf));
    const char *p = (const char *) memchr(buf, ':', n > 0 ? n : 0);
    if (p != NULL) {
      mg_snprintf(user, userlen, "%.*s", p - buf, buf);
      mg_snprintf(pass, passlen, "%.*s", n - (size_t) (p - buf) - 1, p + 1);
    }
  } else if (v != NULL && v->len > 7 && memcmp(v->ptr, "Bearer ", 7) == 0) {
    mg_snprintf(pass, passlen, "%.*s", (int) v->len - 7, v->ptr + 7);
  } else if ((v = mg_http_get_header(hm, "Cookie")) != NULL) {
    struct mg_str t = mg_http_get_header_var(*v, mg_str_n("access_token", 12));
    if (t.len > 0) mg_snprintf(pass, passlen, "%.*s", (int) t.len, t.ptr);
  } else {
    mg_http_get_var(&hm->query, "access_token", pass, passlen);
  }
}

static struct mg_str stripquotes(struct mg_str s) {
  return s.len > 1 && s.ptr[0] == '"' && s.ptr[s.len - 1] == '"'
             ? mg_str_n(s.ptr + 1, s.len - 2)
             : s;
}

struct mg_str mg_http_get_header_var(struct mg_str s, struct mg_str v) {
  size_t i;
  for (i = 0; v.len > 0 && i + v.len + 2 < s.len; i++) {
    if (s.ptr[i + v.len] == '=' && memcmp(&s.ptr[i], v.ptr, v.len) == 0) {
      const char *p = &s.ptr[i + v.len + 1], *b = p, *x = &s.ptr[s.len];
      int q = p < x && *p == '"' ? 1 : 0;
      while (p < x &&
             (q ? p == b || *p != '"' : *p != ';' && *p != ' ' && *p != ','))
        p++;
      // MG_INFO(("[%.*s] [%.*s] [%.*s]", (int) s.len, s.ptr, (int) v.len,
      // v.ptr, (int) (p - b), b));
      return stripquotes(mg_str_n(b, (size_t) (p - b + q)));
    }
  }
  return mg_str_n(NULL, 0);
}

bool mg_http_match_uri(const struct mg_http_message *hm, const char *glob) {
  return mg_match(hm->uri, mg_str(glob), NULL);
}

long mg_http_upload(struct mg_connection *c, struct mg_http_message *hm,
                    struct mg_fs *fs, const char *path, size_t max_size) {
  char buf[20] = "0";
  long res = 0, offset;
  mg_http_get_var(&hm->query, "offset", buf, sizeof(buf));
  offset = strtol(buf, NULL, 0);
  if (hm->body.len == 0) {
    mg_http_reply(c, 200, "", "%ld", res);  // Nothing to write
  } else {
    struct mg_fd *fd;
    size_t current_size = 0;
    MG_DEBUG(("%s -> %d bytes @ %ld", path, (int) hm->body.len, offset));
    if (offset == 0) fs->rm(path);  // If offset if 0, truncate file
    fs->st(path, &current_size, NULL);
    if (offset < 0) {
      mg_http_reply(c, 400, "", "offset required");
      res = -1;
    } else if (offset > 0 && current_size != (size_t) offset) {
      mg_http_reply(c, 400, "", "%s: offset mismatch", path);
      res = -2;
    } else if ((size_t) offset + hm->body.len > max_size) {
      mg_http_reply(c, 400, "", "%s: over max size of %lu", path,
                    (unsigned long) max_size);
      res = -3;
    } else if ((fd = mg_fs_open(fs, path, MG_FS_WRITE)) == NULL) {
      mg_http_reply(c, 400, "", "open(%s): %d", path, errno);
      res = -4;
    } else {
      res = offset + (long) fs->wr(fd->fd, hm->body.ptr, hm->body.len);
      mg_fs_close(fd);
      mg_http_reply(c, 200, "", "%ld", res);
    }
  }
  return res;
}

int mg_http_status(const struct mg_http_message *hm) {
  return atoi(hm->uri.ptr);
}

static bool is_hex_digit(int c) {
  return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') ||
         (c >= 'A' && c <= 'F');
}

static int skip_chunk(const char *buf, int len, int *pl, int *dl) {
  int i = 0, n = 0;
  if (len < 3) return 0;
  while (i < len && is_hex_digit(buf[i])) i++;
  if (len < i + 1 || buf[i] != '\r' || buf[i + 1] != '\n') return -1;  // Error
  n = (int) mg_unhexn(buf, (size_t) i);  // Decode hex length
  if (n < 0) return -1;                  // Error
  if (len < i + n + 4) return 0;         // Chunk not yet fully buffered
  if (buf[i + n + 2] != '\r' || buf[i + n + 3] != '\n') return -1;  // Error
  *pl = i + 2, *dl = n;
  return i + 2 + n + 2;
}

static bool is_chunked(struct mg_http_message *hm) {
  const char *needle = "chunked";
  struct mg_str *te = mg_http_get_header(hm, "Transfer-Encoding");
  return te != NULL && mg_vcasecmp(te, needle) == 0;
}

static void http_cb(struct mg_connection *c, int ev, void *evd, void *fnd) {
  if (ev == MG_EV_READ || ev == MG_EV_CLOSE) {
    struct mg_http_message hm;
    size_t ofs = 0;  // Parsing offset

    while (c->is_resp == 0 && ofs < c->recv.len) {
      const char *buf = (char *) c->recv.buf + ofs;
      int n = mg_http_parse(buf, c->recv.len - ofs, &hm);
      if (n < 0) {
        mg_error(c, "HTTP parse");
        return;
      }
      if (n == 0) break;        // Request is not buffered yet
      if (ev == MG_EV_CLOSE) {  // If client did not set Content-Length
        hm.message.len = c->recv.len - ofs;  // and closes now, deliver MSG
        hm.body.len = hm.message.len - (size_t) (hm.body.ptr - hm.message.ptr);
      }

      if (is_chunked(&hm)) {
        // For chunked data, strip off prefixes and suffixes from chunks
        // and relocate them right after the headers, then report a message
        char *s = (char *) c->recv.buf + ofs + n;
        int o = 0, pl, dl, cl, len = (int) (c->recv.len - ofs - (size_t) n);

        // Find zero-length chunk (the end of the body)
        while ((cl = skip_chunk(s + o, len - o, &pl, &dl)) > 0 && dl) o += cl;
        if (cl == 0) break;  // No zero-len chunk, buffer more data
        if (cl < 0) {
          mg_error(c, "Invalid chunk");
          break;
        }

        // Zero chunk found. Second pass: strip + relocate
        o = 0, hm.body.len = 0, hm.message.len = (size_t) n;
        while ((cl = skip_chunk(s + o, len - o, &pl, &dl)) > 0) {
          memmove(s + hm.body.len, s + o + pl, (size_t) dl);
          o += cl, hm.body.len += (size_t) dl, hm.message.len += (size_t) dl;
          if (dl == 0) break;
        }
        ofs += (size_t) (n + o);
      } else { // Normal, non-chunked data
        size_t len = c->recv.len - ofs - (size_t) n;
        if (hm.body.len > len) break;  // Buffer more data
        ofs += (size_t) n + hm.body.len;
      }

      if (c->is_accepted) c->is_resp = 1;  // Start generating response
      mg_call(c, MG_EV_HTTP_MSG, &hm);     // User handler can clear is_resp
    }
    if (ofs > 0) mg_iobuf_del(&c->recv, 0, ofs);  // Delete processed data
  }
  (void) evd, (void) fnd;
}

static void mg_hfn(struct mg_connection *c, int ev, void *ev_data, void *fnd) {
  if (ev == MG_EV_HTTP_MSG) {
    struct mg_http_message *hm = (struct mg_http_message *) ev_data;
    if (mg_http_match_uri(hm, "/quit")) {
      mg_http_reply(c, 200, "", "ok\n");
      c->is_draining = 1;
      c->data[0] = 'X';
    } else if (mg_http_match_uri(hm, "/debug")) {
      int level = (int) mg_json_get_long(hm->body, "$.level", MG_LL_DEBUG);
      mg_log_set(level);
      mg_http_reply(c, 200, "", "Debug level set to %d\n", level);
    } else {
      mg_http_reply(c, 200, "", "hi\n");
    }
  } else if (ev == MG_EV_CLOSE) {
    if (c->data[0] == 'X') *(bool *) fnd = true;
  }
}

void mg_hello(const char *url) {
  struct mg_mgr mgr;
  bool done = false;
  mg_mgr_init(&mgr);
  if (mg_http_listen(&mgr, url, mg_hfn, &done) == NULL) done = true;
  while (done == false) mg_mgr_poll(&mgr, 100);
  mg_mgr_free(&mgr);
}

struct mg_connection *mg_http_connect(struct mg_mgr *mgr, const char *url,
                                      mg_event_handler_t fn, void *fn_data) {
  struct mg_connection *c = mg_connect(mgr, url, fn, fn_data);
  if (c != NULL) c->pfn = http_cb;
  return c;
}

struct mg_connection *mg_http_listen(struct mg_mgr *mgr, const char *url,
                                     mg_event_handler_t fn, void *fn_data) {
  struct mg_connection *c = mg_listen(mgr, url, fn, fn_data);
  if (c != NULL) c->pfn = http_cb;
  return c;
}

#ifdef MG_ENABLE_LINES
#line 1 "src/iobuf.c"
#endif





static size_t roundup(size_t size, size_t align) {
  return align == 0 ? size : (size + align - 1) / align * align;
}

int mg_iobuf_resize(struct mg_iobuf *io, size_t new_size) {
  int ok = 1;
  new_size = roundup(new_size, io->align);
  if (new_size == 0) {
    mg_bzero(io->buf, io->size);
    free(io->buf);
    io->buf = NULL;
    io->len = io->size = 0;
  } else if (new_size != io->size) {
    // NOTE(lsm): do not use realloc here. Use calloc/free only, to ease the
    // porting to some obscure platforms like FreeRTOS
    void *p = calloc(1, new_size);
    if (p != NULL) {
      size_t len = new_size < io->len ? new_size : io->len;
      if (len > 0 && io->buf != NULL) memmove(p, io->buf, len);
      mg_bzero(io->buf, io->size);
      free(io->buf);
      io->buf = (unsigned char *) p;
      io->size = new_size;
    } else {
      ok = 0;
      MG_ERROR(("%lld->%lld", (uint64_t) io->size, (uint64_t) new_size));
    }
  }
  return ok;
}

int mg_iobuf_init(struct mg_iobuf *io, size_t size, size_t align) {
  io->buf = NULL;
  io->align = align;
  io->size = io->len = 0;
  return mg_iobuf_resize(io, size);
}

size_t mg_iobuf_add(struct mg_iobuf *io, size_t ofs, const void *buf,
                    size_t len) {
  size_t new_size = roundup(io->len + len, io->align);
  mg_iobuf_resize(io, new_size);      // Attempt to resize
  if (new_size != io->size) len = 0;  // Resize failure, append nothing
  if (ofs < io->len) memmove(io->buf + ofs + len, io->buf + ofs, io->len - ofs);
  if (buf != NULL) memmove(io->buf + ofs, buf, len);
  if (ofs > io->len) io->len += ofs - io->len;
  io->len += len;
  return len;
}

size_t mg_iobuf_del(struct mg_iobuf *io, size_t ofs, size_t len) {
  if (ofs > io->len) ofs = io->len;
  if (ofs + len > io->len) len = io->len - ofs;
  if (io->buf) memmove(io->buf + ofs, io->buf + ofs + len, io->len - ofs - len);
  if (io->buf) mg_bzero(io->buf + io->len - len, len);
  io->len -= len;
  return len;
}

void mg_iobuf_free(struct mg_iobuf *io) {
  mg_iobuf_resize(io, 0);
}

#ifdef MG_ENABLE_LINES
#line 1 "src/json.c"
#endif




static const char *escapeseq(int esc) {
  return esc ? "\b\f\n\r\t\\\"" : "bfnrt\\\"";
}

static char json_esc(int c, int esc) {
  const char *p, *esc1 = escapeseq(esc), *esc2 = escapeseq(!esc);
  for (p = esc1; *p != '\0'; p++) {
    if (*p == c) return esc2[p - esc1];
  }
  return 0;
}

static int mg_pass_string(const char *s, int len) {
  int i;
  for (i = 0; i < len; i++) {
    if (s[i] == '\\' && i + 1 < len && json_esc(s[i + 1], 1)) {
      i++;
    } else if (s[i] == '\0') {
      return MG_JSON_INVALID;
    } else if (s[i] == '"') {
      return i;
    }
  }
  return MG_JSON_INVALID;
}

static double mg_atod(const char *p, int len, int *numlen) {
  double d = 0.0;
  int i = 0, sign = 1;

  // Sign
  if (i < len && *p == '-') {
    sign = -1, i++;
  } else if (i < len && *p == '+') {
    i++;
  }

  // Decimal
  for (; i < len && p[i] >= '0' && p[i] <= '9'; i++) {
    d *= 10.0;
    d += p[i] - '0';
  }
  d *= sign;

  // Fractional
  if (i < len && p[i] == '.') {
    double frac = 0.0, base = 0.1;
    i++;
    for (; i < len && p[i] >= '0' && p[i] <= '9'; i++) {
      frac += base * (p[i] - '0');
      base /= 10.0;
    }
    d += frac * sign;
  }

  // Exponential
  if (i < len && (p[i] == 'e' || p[i] == 'E')) {
    int j, exp = 0, minus = 0;
    i++;
    if (i < len && p[i] == '-') minus = 1, i++;
    if (i < len && p[i] == '+') i++;
    while (i < len && p[i] >= '0' && p[i] <= '9' && exp < 308)
      exp = exp * 10 + (p[i++] - '0');
    if (minus) exp = -exp;
    for (j = 0; j < exp; j++) d *= 10.0;
    for (j = 0; j < -exp; j++) d /= 10.0;
  }

  if (numlen != NULL) *numlen = i;
  return d;
}

// Iterate over object or array elements
size_t mg_json_next(struct mg_str obj, size_t ofs, struct mg_str *key,
                    struct mg_str *val) {
  if (ofs >= obj.len) {
    ofs = 0;  // Out of boundaries, stop scanning
  } else if (obj.len < 2 || (*obj.ptr != '{' && *obj.ptr != '[')) {
    ofs = 0;  // Not an array or object, stop
  } else {
    struct mg_str sub = mg_str_n(obj.ptr + ofs, obj.len - ofs);
    if (ofs == 0) ofs++, sub.ptr++, sub.len--;
    if (*obj.ptr == '[') {  // Iterate over an array
      int n = 0, o = mg_json_get(sub, "$", &n);
      if (n < 0 || o < 0 || (size_t) (o + n) > sub.len) {
        ofs = 0;  // Error parsing key, stop scanning
      } else {
        if (key) *key = mg_str_n(NULL, 0);
        if (val) *val = mg_str_n(sub.ptr + o, (size_t) n);
        ofs = (size_t) (&sub.ptr[o + n] - obj.ptr);
      }
    } else {  // Iterate over an object
      int n = 0, o = mg_json_get(sub, "$", &n);
      if (n < 0 || o < 0 || (size_t) (o + n) > sub.len) {
        ofs = 0;  // Error parsing key, stop scanning
      } else {
        if (key) *key = mg_str_n(sub.ptr + o, (size_t) n);
        sub.ptr += o + n, sub.len -= (size_t) (o + n);
        while (sub.len > 0 && *sub.ptr != ':') sub.len--, sub.ptr++;
        if (sub.len > 0 && *sub.ptr == ':') sub.len--, sub.ptr++;
        n = 0, o = mg_json_get(sub, "$", &n);
        if (n < 0 || o < 0 || (size_t) (o + n) > sub.len) {
          ofs = 0;  // Error parsing value, stop scanning
        } else {
          if (val) *val = mg_str_n(sub.ptr + o, (size_t) n);
          ofs = (size_t) (&sub.ptr[o + n] - obj.ptr);
        }
      }
    }
    //MG_INFO(("SUB ofs %u %.*s", ofs, sub.len, sub.ptr));
    while (ofs && ofs < obj.len &&
           (obj.ptr[ofs] == ' ' || obj.ptr[ofs] == '\t' ||
            obj.ptr[ofs] == '\n' || obj.ptr[ofs] == '\r')) {
      ofs++;
    }
    if (ofs && ofs < obj.len && obj.ptr[ofs] == ',') ofs++;
    if (ofs > obj.len) ofs = 0;
  }
  return ofs;
}

int mg_json_get(struct mg_str json, const char *path, int *toklen) {
  const char *s = json.ptr;
  int len = (int) json.len;
  enum { S_VALUE, S_KEY, S_COLON, S_COMMA_OR_EOO } expecting = S_VALUE;
  unsigned char nesting[MG_JSON_MAX_DEPTH];
  int i = 0;             // Current offset in `s`
  int j = 0;             // Offset in `s` we're looking for (return value)
  int depth = 0;         // Current depth (nesting level)
  int ed = 0;            // Expected depth
  int pos = 1;           // Current position in `path`
  int ci = -1, ei = -1;  // Current and expected index in array

  if (toklen) *toklen = 0;
  if (path[0] != '$') return MG_JSON_INVALID;

#define MG_CHECKRET(x)                                  \
  do {                                                  \
    if (depth == ed && path[pos] == '\0' && ci == ei) { \
      if (toklen) *toklen = i - j + 1;                  \
      return j;                                         \
    }                                                   \
  } while (0)

// In the ascii table, the distance between `[` and `]` is 2.
// Ditto for `{` and `}`. Hence +2 in the code below.
#define MG_EOO(x)                                            \
  do {                                                       \
    if (depth == ed && ci != ei) return MG_JSON_NOT_FOUND;   \
    if (c != nesting[depth - 1] + 2) return MG_JSON_INVALID; \
    depth--;                                                 \
    MG_CHECKRET(x);                                          \
  } while (0)

  for (i = 0; i < len; i++) {
    unsigned char c = ((unsigned char *) s)[i];
    if (c == ' ' || c == '\t' || c == '\n' || c == '\r') continue;
    switch (expecting) {
      case S_VALUE:
        // p("V %s [%.*s] %d %d %d %d\n", path, pos, path, depth, ed, ci, ei);
        if (depth == ed) j = i;
        if (c == '{') {
          if (depth >= (int) sizeof(nesting)) return MG_JSON_TOO_DEEP;
          if (depth == ed && path[pos] == '.' && ci == ei) {
            // If we start the object, reset array indices
            ed++, pos++, ci = ei = -1;
          }
          nesting[depth++] = c;
          expecting = S_KEY;
          break;
        } else if (c == '[') {
          if (depth >= (int) sizeof(nesting)) return MG_JSON_TOO_DEEP;
          if (depth == ed && path[pos] == '[' && ei == ci) {
            ed++, pos++, ci = 0;
            for (ei = 0; path[pos] != ']' && path[pos] != '\0'; pos++) {
              ei *= 10;
              ei += path[pos] - '0';
            }
            if (path[pos] != 0) pos++;
          }
          nesting[depth++] = c;
          break;
        } else if (c == ']' && depth > 0) {  // Empty array
          MG_EOO(']');
        } else if (c == 't' && i + 3 < len && memcmp(&s[i], "true", 4) == 0) {
          i += 3;
        } else if (c == 'n' && i + 3 < len && memcmp(&s[i], "null", 4) == 0) {
          i += 3;
        } else if (c == 'f' && i + 4 < len && memcmp(&s[i], "false", 5) == 0) {
          i += 4;
        } else if (c == '-' || ((c >= '0' && c <= '9'))) {
          int numlen = 0;
          mg_atod(&s[i], len - i, &numlen);
          i += numlen - 1;
        } else if (c == '"') {
          int n = mg_pass_string(&s[i + 1], len - i - 1);
          if (n < 0) return n;
          i += n + 1;
        } else {
          return MG_JSON_INVALID;
        }
        MG_CHECKRET('V');
        if (depth == ed && ei >= 0) ci++;
        expecting = S_COMMA_OR_EOO;
        break;

      case S_KEY:
        if (c == '"') {
          int n = mg_pass_string(&s[i + 1], len - i - 1);
          if (n < 0) return n;
          if (i + 1 + n >= len) return MG_JSON_NOT_FOUND;
          if (depth < ed) return MG_JSON_NOT_FOUND;
          if (depth == ed && path[pos - 1] != '.') return MG_JSON_NOT_FOUND;
          // printf("K %s [%.*s] [%.*s] %d %d %d %d %d\n", path, pos, path, n,
          //        &s[i + 1], n, depth, ed, ci, ei);
          //  NOTE(cpq): in the check sequence below is important.
          //  strncmp() must go first: it fails fast if the remaining length
          //  of the path is smaller than `n`.
          if (depth == ed && path[pos - 1] == '.' &&
              strncmp(&s[i + 1], &path[pos], (size_t) n) == 0 &&
              (path[pos + n] == '\0' || path[pos + n] == '.' ||
               path[pos + n] == '[')) {
            pos += n;
          }
          i += n + 1;
          expecting = S_COLON;
        } else if (c == '}') {  // Empty object
          MG_EOO('}');
          expecting = S_COMMA_OR_EOO;
          if (depth == ed && ei >= 0) ci++;
        } else {
          return MG_JSON_INVALID;
        }
        break;

      case S_COLON:
        if (c == ':') {
          expecting = S_VALUE;
        } else {
          return MG_JSON_INVALID;
        }
        break;

      case S_COMMA_OR_EOO:
        if (depth <= 0) {
          return MG_JSON_INVALID;
        } else if (c == ',') {
          expecting = (nesting[depth - 1] == '{') ? S_KEY : S_VALUE;
        } else if (c == ']' || c == '}') {
          if (depth == ed && c == '}' && path[pos - 1] == '.')
            return MG_JSON_NOT_FOUND;
          if (depth == ed && c == ']' && path[pos - 1] == ',')
            return MG_JSON_NOT_FOUND;
          MG_EOO('O');
          if (depth == ed && ei >= 0) ci++;
        } else {
          return MG_JSON_INVALID;
        }
        break;
    }
  }
  return MG_JSON_NOT_FOUND;
}

bool mg_json_get_num(struct mg_str json, const char *path, double *v) {
  int n, toklen, found = 0;
  if ((n = mg_json_get(json, path, &toklen)) >= 0 &&
      (json.ptr[n] == '-' || (json.ptr[n] >= '0' && json.ptr[n] <= '9'))) {
    if (v != NULL) *v = mg_atod(json.ptr + n, toklen, NULL);
    found = 1;
  }
  return found;
}

bool mg_json_get_bool(struct mg_str json, const char *path, bool *v) {
  int found = 0, off = mg_json_get(json, path, NULL);
  if (off >= 0 && (json.ptr[off] == 't' || json.ptr[off] == 'f')) {
    if (v != NULL) *v = json.ptr[off] == 't';
    found = 1;
  }
  return found;
}

bool mg_json_unescape(struct mg_str s, char *to, size_t n) {
  size_t i, j;
  for (i = 0, j = 0; i < s.len && j < n; i++, j++) {
    if (s.ptr[i] == '\\' && i + 5 < s.len && s.ptr[i + 1] == 'u') {
      //  \uXXXX escape. We could process a simple one-byte chars
      // \u00xx from the ASCII range. More complex chars would require
      // dragging in a UTF8 library, which is too much for us
      if (s.ptr[i + 2] != '0' || s.ptr[i + 3] != '0') return false;  // Give up
      ((unsigned char *) to)[j] = (unsigned char) mg_unhexn(s.ptr + i + 4, 2);

      i += 5;
    } else if (s.ptr[i] == '\\' && i + 1 < s.len) {
      char c = json_esc(s.ptr[i + 1], 0);
      if (c == 0) return false;
      to[j] = c;
      i++;
    } else {
      to[j] = s.ptr[i];
    }
  }
  if (j >= n) return false;
  if (n > 0) to[j] = '\0';
  return true;
}

char *mg_json_get_str(struct mg_str json, const char *path) {
  char *result = NULL;
  int len = 0, off = mg_json_get(json, path, &len);
  if (off >= 0 && len > 1 && json.ptr[off] == '"') {
    if ((result = (char *) calloc(1, (size_t) len)) != NULL &&
        !mg_json_unescape(mg_str_n(json.ptr + off + 1, (size_t) (len - 2)),
                          result, (size_t) len)) {
      free(result);
      result = NULL;
    }
  }
  return result;
}

char *mg_json_get_b64(struct mg_str json, const char *path, int *slen) {
  char *result = NULL;
  int len = 0, off = mg_json_get(json, path, &len);
  if (off >= 0 && json.ptr[off] == '"' && len > 1 &&
      (result = (char *) calloc(1, (size_t) len)) != NULL) {
    size_t k = mg_base64_decode(json.ptr + off + 1, (size_t) (len - 2), result,
                                (size_t) len);
    if (slen != NULL) *slen = (int) k;
  }
  return result;
}

char *mg_json_get_hex(struct mg_str json, const char *path, int *slen) {
  char *result = NULL;
  int len = 0, off = mg_json_get(json, path, &len);
  if (off >= 0 && json.ptr[off] == '"' && len > 1 &&
      (result = (char *) calloc(1, (size_t) len / 2)) != NULL) {
    mg_unhex(json.ptr + off + 1, (size_t) (len - 2), (uint8_t *) result);
    result[len / 2 - 1] = '\0';
    if (slen != NULL) *slen = len / 2 - 1;
  }
  return result;
}

long mg_json_get_long(struct mg_str json, const char *path, long dflt) {
  double dv;
  long result = dflt;
  if (mg_json_get_num(json, path, &dv)) result = (long) dv;
  return result;
}

#ifdef MG_ENABLE_LINES
#line 1 "src/log.c"
#endif





static int s_level = MG_LL_INFO;
static mg_pfn_t s_log_func = mg_pfn_stdout;
static void *s_log_func_param = NULL;

void mg_log_set_fn(mg_pfn_t fn, void *param) {
  s_log_func = fn;
  s_log_func_param = param;
}

static void logc(unsigned char c) {
  s_log_func((char) c, s_log_func_param);
}

static void logs(const char *buf, size_t len) {
  size_t i;
  for (i = 0; i < len; i++) logc(((unsigned char *) buf)[i]);
}

void mg_log_set(int log_level) {
  MG_DEBUG(("Setting log level to %d", log_level));
  s_level = log_level;
}

#if MG_ENABLE_CUSTOM_LOG
// Let user define their own mg_log_prefix() and mg_log()
#else
bool mg_log_prefix(int level, const char *file, int line, const char *fname) {
  if (level <= s_level) {
    const char *p = strrchr(file, '/');
    char buf[41];
    size_t n;
    if (p == NULL) p = strrchr(file, '\\');
    n = mg_snprintf(buf, sizeof(buf), "%-6llx %d %s:%d:%s", mg_millis(), level,
                    p == NULL ? file : p + 1, line, fname);
    if (n > sizeof(buf) - 2) n = sizeof(buf) - 2;
    while (n < sizeof(buf)) buf[n++] = ' ';
    logs(buf, n - 1);
    return true;
  } else {
    return false;
  }
}

void mg_log(const char *fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  mg_vxprintf(s_log_func, s_log_func_param, fmt, &ap);
  va_end(ap);
  logs("\r\n", 2);
}
#endif

static unsigned char nibble(unsigned c) {
  return (unsigned char) (c < 10 ? c + '0' : c + 'W');
}

#define ISPRINT(x) ((x) >= ' ' && (x) <= '~')
void mg_hexdump(const void *buf, size_t len) {
  const unsigned char *p = (const unsigned char *) buf;
  unsigned char ascii[16], alen = 0;
  size_t i;
  for (i = 0; i < len; i++) {
    if ((i % 16) == 0) {
      // Print buffered ascii chars
      if (i > 0) logs("  ", 2), logs((char *) ascii, 16), logc('\n'), alen = 0;
      // Print hex address, then \t
      logc(nibble((i >> 12) & 15)), logc(nibble((i >> 8) & 15)),
          logc(nibble((i >> 4) & 15)), logc('0'), logs("   ", 3);
    }
    logc(nibble(p[i] >> 4)), logc(nibble(p[i] & 15));  // Two nibbles, e.g. c5
    logc(' ');                                         // Space after hex number
    ascii[alen++] = ISPRINT(p[i]) ? p[i] : '.';        // Add to the ascii buf
  }
  while (alen < 16) logs("   ", 3), ascii[alen++] = ' ';
  logs("  ", 2), logs((char *) ascii, 16), logc('\n');
}

#ifdef MG_ENABLE_LINES
#line 1 "src/md5.c"
#endif



//  This code implements the MD5 message-digest algorithm.
//  The algorithm is due to Ron Rivest.  This code was
//  written by Colin Plumb in 1993, no copyright is claimed.
//  This code is in the public domain; do with it what you wish.
//
//  Equivalent code is available from RSA Data Security, Inc.
//  This code has been tested against that, and is equivalent,
//  except that you don't need to include two pages of legalese
//  with every copy.
//
//  To compute the message digest of a chunk of bytes, declare an
//  MD5Context structure, pass it to MD5Init, call MD5Update as
//  needed on buffers full of bytes, and then call MD5Final, which
//  will fill a supplied 16-byte array with the digest.

#if defined(MG_ENABLE_MD5) && MG_ENABLE_MD5

static void mg_byte_reverse(unsigned char *buf, unsigned longs) {
  if (MG_BIG_ENDIAN) {
    do {
      uint32_t t = (uint32_t) ((unsigned) buf[3] << 8 | buf[2]) << 16 |
                   ((unsigned) buf[1] << 8 | buf[0]);
      *(uint32_t *) buf = t;
      buf += 4;
    } while (--longs);
  } else {
    (void) buf, (void) longs;  // Little endian. Do nothing
  }
}

#define F1(x, y, z) (z ^ (x & (y ^ z)))
#define F2(x, y, z) F1(z, x, y)
#define F3(x, y, z) (x ^ y ^ z)
#define F4(x, y, z) (y ^ (x | ~z))

#define MD5STEP(f, w, x, y, z, data, s) \
  (w += f(x, y, z) + data, w = w << s | w >> (32 - s), w += x)

/*
 * Start MD5 accumulation.  Set bit count to 0 and buffer to mysterious
 * initialization constants.
 */
void mg_md5_init(mg_md5_ctx *ctx) {
  ctx->buf[0] = 0x67452301;
  ctx->buf[1] = 0xefcdab89;
  ctx->buf[2] = 0x98badcfe;
  ctx->buf[3] = 0x10325476;

  ctx->bits[0] = 0;
  ctx->bits[1] = 0;
}

static void mg_md5_transform(uint32_t buf[4], uint32_t const in[16]) {
  uint32_t a, b, c, d;

  a = buf[0];
  b = buf[1];
  c = buf[2];
  d = buf[3];

  MD5STEP(F1, a, b, c, d, in[0] + 0xd76aa478, 7);
  MD5STEP(F1, d, a, b, c, in[1] + 0xe8c7b756, 12);
  MD5STEP(F1, c, d, a, b, in[2] + 0x242070db, 17);
  MD5STEP(F1, b, c, d, a, in[3] + 0xc1bdceee, 22);
  MD5STEP(F1, a, b, c, d, in[4] + 0xf57c0faf, 7);
  MD5STEP(F1, d, a, b, c, in[5] + 0x4787c62a, 12);
  MD5STEP(F1, c, d, a, b, in[6] + 0xa8304613, 17);
  MD5STEP(F1, b, c, d, a, in[7] + 0xfd469501, 22);
  MD5STEP(F1, a, b, c, d, in[8] + 0x698098d8, 7);
  MD5STEP(F1, d, a, b, c, in[9] + 0x8b44f7af, 12);
  MD5STEP(F1, c, d, a, b, in[10] + 0xffff5bb1, 17);
  MD5STEP(F1, b, c, d, a, in[11] + 0x895cd7be, 22);
  MD5STEP(F1, a, b, c, d, in[12] + 0x6b901122, 7);
  MD5STEP(F1, d, a, b, c, in[13] + 0xfd987193, 12);
  MD5STEP(F1, c, d, a, b, in[14] + 0xa679438e, 17);
  MD5STEP(F1, b, c, d, a, in[15] + 0x49b40821, 22);

  MD5STEP(F2, a, b, c, d, in[1] + 0xf61e2562, 5);
  MD5STEP(F2, d, a, b, c, in[6] + 0xc040b340, 9);
  MD5STEP(F2, c, d, a, b, in[11] + 0x265e5a51, 14);
  MD5STEP(F2, b, c, d, a, in[0] + 0xe9b6c7aa, 20);
  MD5STEP(F2, a, b, c, d, in[5] + 0xd62f105d, 5);
  MD5STEP(F2, d, a, b, c, in[10] + 0x02441453, 9);
  MD5STEP(F2, c, d, a, b, in[15] + 0xd8a1e681, 14);
  MD5STEP(F2, b, c, d, a, in[4] + 0xe7d3fbc8, 20);
  MD5STEP(F2, a, b, c, d, in[9] + 0x21e1cde6, 5);
  MD5STEP(F2, d, a, b, c, in[14] + 0xc33707d6, 9);
  MD5STEP(F2, c, d, a, b, in[3] + 0xf4d50d87, 14);
  MD5STEP(F2, b, c, d, a, in[8] + 0x455a14ed, 20);
  MD5STEP(F2, a, b, c, d, in[13] + 0xa9e3e905, 5);
  MD5STEP(F2, d, a, b, c, in[2] + 0xfcefa3f8, 9);
  MD5STEP(F2, c, d, a, b, in[7] + 0x676f02d9, 14);
  MD5STEP(F2, b, c, d, a, in[12] + 0x8d2a4c8a, 20);

  MD5STEP(F3, a, b, c, d, in[5] + 0xfffa3942, 4);
  MD5STEP(F3, d, a, b, c, in[8] + 0x8771f681, 11);
  MD5STEP(F3, c, d, a, b, in[11] + 0x6d9d6122, 16);
  MD5STEP(F3, b, c, d, a, in[14] + 0xfde5380c, 23);
  MD5STEP(F3, a, b, c, d, in[1] + 0xa4beea44, 4);
  MD5STEP(F3, d, a, b, c, in[4] + 0x4bdecfa9, 11);
  MD5STEP(F3, c, d, a, b, in[7] + 0xf6bb4b60, 16);
  MD5STEP(F3, b, c, d, a, in[10] + 0xbebfbc70, 23);
  MD5STEP(F3, a, b, c, d, in[13] + 0x289b7ec6, 4);
  MD5STEP(F3, d, a, b, c, in[0] + 0xeaa127fa, 11);
  MD5STEP(F3, c, d, a, b, in[3] + 0xd4ef3085, 16);
  MD5STEP(F3, b, c, d, a, in[6] + 0x04881d05, 23);
  MD5STEP(F3, a, b, c, d, in[9] + 0xd9d4d039, 4);
  MD5STEP(F3, d, a, b, c, in[12] + 0xe6db99e5, 11);
  MD5STEP(F3, c, d, a, b, in[15] + 0x1fa27cf8, 16);
  MD5STEP(F3, b, c, d, a, in[2] + 0xc4ac5665, 23);

  MD5STEP(F4, a, b, c, d, in[0] + 0xf4292244, 6);
  MD5STEP(F4, d, a, b, c, in[7] + 0x432aff97, 10);
  MD5STEP(F4, c, d, a, b, in[14] + 0xab9423a7, 15);
  MD5STEP(F4, b, c, d, a, in[5] + 0xfc93a039, 21);
  MD5STEP(F4, a, b, c, d, in[12] + 0x655b59c3, 6);
  MD5STEP(F4, d, a, b, c, in[3] + 0x8f0ccc92, 10);
  MD5STEP(F4, c, d, a, b, in[10] + 0xffeff47d, 15);
  MD5STEP(F4, b, c, d, a, in[1] + 0x85845dd1, 21);
  MD5STEP(F4, a, b, c, d, in[8] + 0x6fa87e4f, 6);
  MD5STEP(F4, d, a, b, c, in[15] + 0xfe2ce6e0, 10);
  MD5STEP(F4, c, d, a, b, in[6] + 0xa3014314, 15);
  MD5STEP(F4, b, c, d, a, in[13] + 0x4e0811a1, 21);
  MD5STEP(F4, a, b, c, d, in[4] + 0xf7537e82, 6);
  MD5STEP(F4, d, a, b, c, in[11] + 0xbd3af235, 10);
  MD5STEP(F4, c, d, a, b, in[2] + 0x2ad7d2bb, 15);
  MD5STEP(F4, b, c, d, a, in[9] + 0xeb86d391, 21);

  buf[0] += a;
  buf[1] += b;
  buf[2] += c;
  buf[3] += d;
}

void mg_md5_update(mg_md5_ctx *ctx, const unsigned char *buf, size_t len) {
  uint32_t t;

  t = ctx->bits[0];
  if ((ctx->bits[0] = t + ((uint32_t) len << 3)) < t) ctx->bits[1]++;
  ctx->bits[1] += (uint32_t) len >> 29;

  t = (t >> 3) & 0x3f;

  if (t) {
    unsigned char *p = (unsigned char *) ctx->in + t;

    t = 64 - t;
    if (len < t) {
      memcpy(p, buf, len);
      return;
    }
    memcpy(p, buf, t);
    mg_byte_reverse(ctx->in, 16);
    mg_md5_transform(ctx->buf, (uint32_t *) ctx->in);
    buf += t;
    len -= t;
  }

  while (len >= 64) {
    memcpy(ctx->in, buf, 64);
    mg_byte_reverse(ctx->in, 16);
    mg_md5_transform(ctx->buf, (uint32_t *) ctx->in);
    buf += 64;
    len -= 64;
  }

  memcpy(ctx->in, buf, len);
}

void mg_md5_final(mg_md5_ctx *ctx, unsigned char digest[16]) {
  unsigned count;
  unsigned char *p;
  uint32_t *a;

  count = (ctx->bits[0] >> 3) & 0x3F;

  p = ctx->in + count;
  *p++ = 0x80;
  count = 64 - 1 - count;
  if (count < 8) {
    memset(p, 0, count);
    mg_byte_reverse(ctx->in, 16);
    mg_md5_transform(ctx->buf, (uint32_t *) ctx->in);
    memset(ctx->in, 0, 56);
  } else {
    memset(p, 0, count - 8);
  }
  mg_byte_reverse(ctx->in, 14);

  a = (uint32_t *) ctx->in;
  a[14] = ctx->bits[0];
  a[15] = ctx->bits[1];

  mg_md5_transform(ctx->buf, (uint32_t *) ctx->in);
  mg_byte_reverse((unsigned char *) ctx->buf, 4);
  memcpy(digest, ctx->buf, 16);
  memset((char *) ctx, 0, sizeof(*ctx));
}
#endif

#ifdef MG_ENABLE_LINES
#line 1 "src/mqtt.c"
#endif








#define MQTT_CLEAN_SESSION 0x02
#define MQTT_HAS_WILL 0x04
#define MQTT_WILL_RETAIN 0x20
#define MQTT_HAS_PASSWORD 0x40
#define MQTT_HAS_USER_NAME 0x80

struct mg_mqtt_pmap {
  uint8_t id;
  uint8_t type;
};

static const struct mg_mqtt_pmap s_prop_map[] = {
    {MQTT_PROP_PAYLOAD_FORMAT_INDICATOR, MQTT_PROP_TYPE_BYTE},
    {MQTT_PROP_MESSAGE_EXPIRY_INTERVAL, MQTT_PROP_TYPE_INT},
    {MQTT_PROP_CONTENT_TYPE, MQTT_PROP_TYPE_STRING},
    {MQTT_PROP_RESPONSE_TOPIC, MQTT_PROP_TYPE_STRING},
    {MQTT_PROP_CORRELATION_DATA, MQTT_PROP_TYPE_BINARY_DATA},
    {MQTT_PROP_SUBSCRIPTION_IDENTIFIER, MQTT_PROP_TYPE_VARIABLE_INT},
    {MQTT_PROP_SESSION_EXPIRY_INTERVAL, MQTT_PROP_TYPE_INT},
    {MQTT_PROP_ASSIGNED_CLIENT_IDENTIFIER, MQTT_PROP_TYPE_STRING},
    {MQTT_PROP_SERVER_KEEP_ALIVE, MQTT_PROP_TYPE_SHORT},
    {MQTT_PROP_AUTHENTICATION_METHOD, MQTT_PROP_TYPE_STRING},
    {MQTT_PROP_AUTHENTICATION_DATA, MQTT_PROP_TYPE_BINARY_DATA},
    {MQTT_PROP_REQUEST_PROBLEM_INFORMATION, MQTT_PROP_TYPE_BYTE},
    {MQTT_PROP_WILL_DELAY_INTERVAL, MQTT_PROP_TYPE_INT},
    {MQTT_PROP_REQUEST_RESPONSE_INFORMATION, MQTT_PROP_TYPE_BYTE},
    {MQTT_PROP_RESPONSE_INFORMATION, MQTT_PROP_TYPE_STRING},
    {MQTT_PROP_SERVER_REFERENCE, MQTT_PROP_TYPE_STRING},
    {MQTT_PROP_REASON_STRING, MQTT_PROP_TYPE_STRING},
    {MQTT_PROP_RECEIVE_MAXIMUM, MQTT_PROP_TYPE_SHORT},
    {MQTT_PROP_TOPIC_ALIAS_MAXIMUM, MQTT_PROP_TYPE_SHORT},
    {MQTT_PROP_TOPIC_ALIAS, MQTT_PROP_TYPE_SHORT},
    {MQTT_PROP_MAXIMUM_QOS, MQTT_PROP_TYPE_BYTE},
    {MQTT_PROP_RETAIN_AVAILABLE, MQTT_PROP_TYPE_BYTE},
    {MQTT_PROP_USER_PROPERTY, MQTT_PROP_TYPE_STRING_PAIR},
    {MQTT_PROP_MAXIMUM_PACKET_SIZE, MQTT_PROP_TYPE_INT},
    {MQTT_PROP_WILDCARD_SUBSCRIPTION_AVAILABLE, MQTT_PROP_TYPE_BYTE},
    {MQTT_PROP_SUBSCRIPTION_IDENTIFIER_AVAILABLE, MQTT_PROP_TYPE_BYTE},
    {MQTT_PROP_SHARED_SUBSCRIPTION_AVAILABLE, MQTT_PROP_TYPE_BYTE}};

void mg_mqtt_send_header(struct mg_connection *c, uint8_t cmd, uint8_t flags,
                         uint32_t len) {
  uint8_t buf[1 + sizeof(len)], *vlen = &buf[1];
  buf[0] = (uint8_t) ((cmd << 4) | flags);
  do {
    *vlen = len % 0x80;
    len /= 0x80;
    if (len > 0) *vlen |= 0x80;
    vlen++;
  } while (len > 0 && vlen < &buf[sizeof(buf)]);
  mg_send(c, buf, (size_t) (vlen - buf));
}

static void mg_send_u16(struct mg_connection *c, uint16_t value) {
  mg_send(c, &value, sizeof(value));
}

static void mg_send_u32(struct mg_connection *c, uint32_t value) {
  mg_send(c, &value, sizeof(value));
}

static uint8_t varint_size(size_t length) {
  uint8_t bytes_needed = 0;
  do {
    bytes_needed++;
    length /= 0x80;
  } while (length > 0);
  return bytes_needed;
}

static size_t encode_varint(uint8_t *buf, size_t value) {
  size_t len = 0;

  do {
    uint8_t byte = (uint8_t) (value % 128);
    value /= 128;
    if (value > 0) byte |= 0x80;
    buf[len++] = byte;
  } while (value > 0);

  return len;
}

static size_t decode_varint(const uint8_t *buf, size_t len, size_t *value) {
  size_t multiplier = 1, offset;
  *value = 0;

  for (offset = 0; offset < 4 && offset < len; offset++) {
    uint8_t encoded_byte = buf[offset];
    *value += (encoded_byte & 0x7f) * multiplier;
    multiplier *= 128;

    if ((encoded_byte & 0x80) == 0) return offset + 1;
  }

  return 0;
}

static int mqtt_prop_type_by_id(uint8_t prop_id) {
  size_t i, num_properties = sizeof(s_prop_map) / sizeof(s_prop_map[0]);
  for (i = 0; i < num_properties; ++i) {
    if (s_prop_map[i].id == prop_id) return s_prop_map[i].type;
  }
  return -1;  // Property ID not found
}

// Returns the size of the properties section, without the
// size of the content's length
static size_t get_properties_length(struct mg_mqtt_prop *props, size_t count) {
  size_t i, size = 0;
  for (i = 0; i < count; i++) {
    size++;  // identifier
    switch (mqtt_prop_type_by_id(props[i].id)) {
      case MQTT_PROP_TYPE_STRING_PAIR:
        size += (uint32_t) (props[i].val.len + props[i].key.len +
                            2 * sizeof(uint16_t));
        break;
      case MQTT_PROP_TYPE_STRING:
        size += (uint32_t) (props[i].val.len + sizeof(uint16_t));
        break;
      case MQTT_PROP_TYPE_BINARY_DATA:
        size += (uint32_t) (props[i].val.len + sizeof(uint16_t));
        break;
      case MQTT_PROP_TYPE_VARIABLE_INT:
        size += varint_size((uint32_t) props[i].iv);
        break;
      case MQTT_PROP_TYPE_INT:
        size += (uint32_t) sizeof(uint32_t);
        break;
      case MQTT_PROP_TYPE_SHORT:
        size += (uint32_t) sizeof(uint16_t);
        break;
      case MQTT_PROP_TYPE_BYTE:
        size += (uint32_t) sizeof(uint8_t);
        break;
      default:
        return size;  // cannot parse further down
    }
  }

  return size;
}

// returns the entire size of the properties section, including the
// size of the variable length of the content
static size_t get_props_size(struct mg_mqtt_prop *props, size_t count) {
  size_t size = get_properties_length(props, count);
  size += varint_size(size);
  return size;
}

static void mg_send_mqtt_properties(struct mg_connection *c,
                                    struct mg_mqtt_prop *props, size_t nprops) {
  size_t total_size = get_properties_length(props, nprops);
  uint8_t buf_v[4] = {0, 0, 0, 0};
  uint8_t buf[4] = {0, 0, 0, 0};
  size_t i, len = encode_varint(buf, total_size);

  mg_send(c, buf, (size_t) len);
  for (i = 0; i < nprops; i++) {
    mg_send(c, &props[i].id, sizeof(props[i].id));
    switch (mqtt_prop_type_by_id(props[i].id)) {
      case MQTT_PROP_TYPE_STRING_PAIR:
        mg_send_u16(c, mg_htons((uint16_t) props[i].key.len));
        mg_send(c, props[i].key.ptr, props[i].key.len);
        mg_send_u16(c, mg_htons((uint16_t) props[i].val.len));
        mg_send(c, props[i].val.ptr, props[i].val.len);
        break;
      case MQTT_PROP_TYPE_BYTE:
        mg_send(c, &props[i].iv, sizeof(uint8_t));
        break;
      case MQTT_PROP_TYPE_SHORT:
        mg_send_u16(c, mg_htons((uint16_t) props[i].iv));
        break;
      case MQTT_PROP_TYPE_INT:
        mg_send_u32(c, mg_htonl((uint32_t) props[i].iv));
        break;
      case MQTT_PROP_TYPE_STRING:
        mg_send_u16(c, mg_htons((uint16_t) props[i].val.len));
        mg_send(c, props[i].val.ptr, props[i].val.len);
        break;
      case MQTT_PROP_TYPE_BINARY_DATA:
        mg_send_u16(c, mg_htons((uint16_t) props[i].val.len));
        mg_send(c, props[i].val.ptr, props[i].val.len);
        break;
      case MQTT_PROP_TYPE_VARIABLE_INT:
        len = encode_varint(buf_v, props[i].iv);
        mg_send(c, buf_v, (size_t) len);
        break;
    }
  }
}

size_t mg_mqtt_next_prop(struct mg_mqtt_message *msg, struct mg_mqtt_prop *prop,
                         size_t ofs) {
  uint8_t *i = (uint8_t *) msg->dgram.ptr + msg->props_start + ofs;
  uint8_t *end = (uint8_t *) msg->dgram.ptr + msg->dgram.len;
  size_t new_pos = ofs, len;
  prop->id = i[0];

  if (ofs >= msg->dgram.len || ofs >= msg->props_start + msg->props_size)
    return 0;
  i++, new_pos++;

  switch (mqtt_prop_type_by_id(prop->id)) {
    case MQTT_PROP_TYPE_STRING_PAIR:
      prop->key.len = (uint16_t) ((((uint16_t) i[0]) << 8) | i[1]);
      prop->key.ptr = (char *) i + 2;
      i += 2 + prop->key.len;
      prop->val.len = (uint16_t) ((((uint16_t) i[0]) << 8) | i[1]);
      prop->val.ptr = (char *) i + 2;
      new_pos += 2 * sizeof(uint16_t) + prop->val.len + prop->key.len;
      break;
    case MQTT_PROP_TYPE_BYTE:
      prop->iv = (uint8_t) i[0];
      new_pos++;
      break;
    case MQTT_PROP_TYPE_SHORT:
      prop->iv = (uint16_t) ((((uint16_t) i[0]) << 8) | i[1]);
      new_pos += sizeof(uint16_t);
      break;
    case MQTT_PROP_TYPE_INT:
      prop->iv = ((uint32_t) i[0] << 24) | ((uint32_t) i[1] << 16) |
                 ((uint32_t) i[2] << 8) | i[3];
      new_pos += sizeof(uint32_t);
      break;
    case MQTT_PROP_TYPE_STRING:
      prop->val.len = (uint16_t) ((((uint16_t) i[0]) << 8) | i[1]);
      prop->val.ptr = (char *) i + 2;
      new_pos += 2 + prop->val.len;
      break;
    case MQTT_PROP_TYPE_BINARY_DATA:
      prop->val.len = (uint16_t) ((((uint16_t) i[0]) << 8) | i[1]);
      prop->val.ptr = (char *) i + 2;
      new_pos += 2 + prop->val.len;
      break;
    case MQTT_PROP_TYPE_VARIABLE_INT:
      len = decode_varint(i, (size_t) (end - i), (size_t *) &prop->iv);
      new_pos = (!len) ? 0 : new_pos + len;
      break;
    default:
      new_pos = 0;
  }

  return new_pos;
}

void mg_mqtt_login(struct mg_connection *c, const struct mg_mqtt_opts *opts) {
  char rnd[10], client_id[21];
  struct mg_str cid = opts->client_id;
  size_t total_len = 7 + 1 + 2 + 2;
  uint8_t hdr[8] = {0, 4, 'M', 'Q', 'T', 'T', opts->version, 0};

  if (cid.len == 0) {
    mg_random(rnd, sizeof(rnd));
    mg_hex(rnd, sizeof(rnd), client_id);
    client_id[sizeof(client_id) - 1] = '\0';
    cid = mg_str(client_id);
  }

  if (hdr[6] == 0) hdr[6] = 4;  // If version is not set, use 4 (3.1.1)
  c->is_mqtt5 = hdr[6] == 5;    // Set version 5 flag
  hdr[7] = (uint8_t) ((opts->qos & 3) << 3);  // Connection flags
  if (opts->user.len > 0) {
    total_len += 2 + (uint32_t) opts->user.len;
    hdr[7] |= MQTT_HAS_USER_NAME;
  }
  if (opts->pass.len > 0) {
    total_len += 2 + (uint32_t) opts->pass.len;
    hdr[7] |= MQTT_HAS_PASSWORD;
  }
  if (opts->topic.len > 0 && opts->message.len > 0) {
    total_len += 4 + (uint32_t) opts->topic.len + (uint32_t) opts->message.len;
    hdr[7] |= MQTT_HAS_WILL;
  }
  if (opts->clean || cid.len == 0) hdr[7] |= MQTT_CLEAN_SESSION;
  if (opts->retain) hdr[7] |= MQTT_WILL_RETAIN;
  total_len += (uint32_t) cid.len;
  if (c->is_mqtt5) {
    total_len += get_props_size(opts->props, opts->num_props);
    if (hdr[7] & MQTT_HAS_WILL)
      total_len += get_props_size(opts->will_props, opts->num_will_props);
  }

  mg_mqtt_send_header(c, MQTT_CMD_CONNECT, 0, (uint32_t) total_len);
  mg_send(c, hdr, sizeof(hdr));
  // keepalive == 0 means "do not disconnect us!"
  mg_send_u16(c, mg_htons((uint16_t) opts->keepalive));

  if (c->is_mqtt5) mg_send_mqtt_properties(c, opts->props, opts->num_props);

  mg_send_u16(c, mg_htons((uint16_t) cid.len));
  mg_send(c, cid.ptr, cid.len);

  if (hdr[7] & MQTT_HAS_WILL) {
    if (c->is_mqtt5)
      mg_send_mqtt_properties(c, opts->will_props, opts->num_will_props);

    mg_send_u16(c, mg_htons((uint16_t) opts->topic.len));
    mg_send(c, opts->topic.ptr, opts->topic.len);
    mg_send_u16(c, mg_htons((uint16_t) opts->message.len));
    mg_send(c, opts->message.ptr, opts->message.len);
  }
  if (opts->user.len > 0) {
    mg_send_u16(c, mg_htons((uint16_t) opts->user.len));
    mg_send(c, opts->user.ptr, opts->user.len);
  }
  if (opts->pass.len > 0) {
    mg_send_u16(c, mg_htons((uint16_t) opts->pass.len));
    mg_send(c, opts->pass.ptr, opts->pass.len);
  }
}

void mg_mqtt_pub(struct mg_connection *c, const struct mg_mqtt_opts *opts) {
  uint8_t flags = (uint8_t) (((opts->qos & 3) << 1) | (opts->retain ? 1 : 0));
  size_t len = 2 + opts->topic.len + opts->message.len;
  MG_DEBUG(("%lu [%.*s] -> [%.*s]", c->id, (int) opts->topic.len,
            (char *) opts->topic.ptr, (int) opts->message.len,
            (char *) opts->message.ptr));
  if (opts->qos > 0) len += 2;
  if (c->is_mqtt5) len += get_props_size(opts->props, opts->num_props);

  mg_mqtt_send_header(c, MQTT_CMD_PUBLISH, flags, (uint32_t) len);
  mg_send_u16(c, mg_htons((uint16_t) opts->topic.len));
  mg_send(c, opts->topic.ptr, opts->topic.len);
  if (opts->qos > 0) {
    if (++c->mgr->mqtt_id == 0) ++c->mgr->mqtt_id;
    mg_send_u16(c, mg_htons(c->mgr->mqtt_id));
  }

  if (c->is_mqtt5) mg_send_mqtt_properties(c, opts->props, opts->num_props);

  mg_send(c, opts->message.ptr, opts->message.len);
}

void mg_mqtt_sub(struct mg_connection *c, const struct mg_mqtt_opts *opts) {
  uint8_t qos_ = opts->qos & 3;
  size_t plen = c->is_mqtt5 ? get_props_size(opts->props, opts->num_props) : 0;
  size_t len = 2 + opts->topic.len + 2 + 1 + plen;

  mg_mqtt_send_header(c, MQTT_CMD_SUBSCRIBE, 2, (uint32_t) len);
  if (++c->mgr->mqtt_id == 0) ++c->mgr->mqtt_id;
  mg_send_u16(c, mg_htons(c->mgr->mqtt_id));
  if (c->is_mqtt5) mg_send_mqtt_properties(c, opts->props, opts->num_props);

  mg_send_u16(c, mg_htons((uint16_t) opts->topic.len));
  mg_send(c, opts->topic.ptr, opts->topic.len);
  mg_send(c, &qos_, sizeof(qos_));
}

int mg_mqtt_parse(const uint8_t *buf, size_t len, uint8_t version,
                  struct mg_mqtt_message *m) {
  uint8_t lc = 0, *p, *end;
  uint32_t n = 0, len_len = 0;

  memset(m, 0, sizeof(*m));
  m->dgram.ptr = (char *) buf;
  if (len < 2) return MQTT_INCOMPLETE;
  m->cmd = (uint8_t) (buf[0] >> 4);
  m->qos = (buf[0] >> 1) & 3;

  n = len_len = 0;
  p = (uint8_t *) buf + 1;
  while ((size_t) (p - buf) < len) {
    lc = *((uint8_t *) p++);
    n += (uint32_t) ((lc & 0x7f) << 7 * len_len);
    len_len++;
    if (!(lc & 0x80)) break;
    if (len_len >= 4) return MQTT_MALFORMED;
  }
  end = p + n;
  if ((lc & 0x80) || (end > buf + len)) return MQTT_INCOMPLETE;
  m->dgram.len = (size_t) (end - buf);

  switch (m->cmd) {
    case MQTT_CMD_CONNACK:
      if (end - p < 2) return MQTT_MALFORMED;
      m->ack = p[1];
      break;
    case MQTT_CMD_PUBACK:
    case MQTT_CMD_PUBREC:
    case MQTT_CMD_PUBREL:
    case MQTT_CMD_PUBCOMP:
    case MQTT_CMD_SUBSCRIBE:
    case MQTT_CMD_SUBACK:
    case MQTT_CMD_UNSUBSCRIBE:
    case MQTT_CMD_UNSUBACK:
      if (p + 2 > end) return MQTT_MALFORMED;
      m->id = (uint16_t) ((((uint16_t) p[0]) << 8) | p[1]);
      p += 2;
      break;
    case MQTT_CMD_PUBLISH: {
      if (p + 2 > end) return MQTT_MALFORMED;
      m->topic.len = (uint16_t) ((((uint16_t) p[0]) << 8) | p[1]);
      m->topic.ptr = (char *) p + 2;
      p += 2 + m->topic.len;
      if (p > end) return MQTT_MALFORMED;
      if (m->qos > 0) {
        if (p + 2 > end) return MQTT_MALFORMED;
        m->id = (uint16_t) ((((uint16_t) p[0]) << 8) | p[1]);
        p += 2;
      }
      if (p > end) return MQTT_MALFORMED;
      if (version == 5 && p + 2 < end) {
        len_len = (uint32_t) decode_varint(p, (size_t) (end - p), &m->props_size);
        if (!len_len) return MQTT_MALFORMED;
        m->props_start = (size_t) (p + len_len - buf);
        p += len_len + m->props_size;
      }
      if (p > end) return MQTT_MALFORMED;
      m->data.ptr = (char *) p;
      m->data.len = (size_t) (end - p);
      break;
    }
    default:
      break;
  }
  return MQTT_OK;
}

static void mqtt_cb(struct mg_connection *c, int ev, void *ev_data,
                    void *fn_data) {
  if (ev == MG_EV_READ) {
    for (;;) {
      uint8_t version = c->is_mqtt5 ? 5 : 4;
      struct mg_mqtt_message mm;
      int rc = mg_mqtt_parse(c->recv.buf, c->recv.len, version, &mm);
      if (rc == MQTT_MALFORMED) {
        MG_ERROR(("%lu MQTT malformed message", c->id));
        c->is_closing = 1;
        break;
      } else if (rc == MQTT_OK) {
        MG_VERBOSE(("%lu MQTT CMD %d len %d [%.*s]", c->id, mm.cmd,
                    (int) mm.dgram.len, (int) mm.data.len, mm.data.ptr));
        switch (mm.cmd) {
          case MQTT_CMD_CONNACK:
            mg_call(c, MG_EV_MQTT_OPEN, &mm.ack);
            if (mm.ack == 0) {
              MG_DEBUG(("%lu Connected", c->id));
            } else {
              MG_ERROR(("%lu MQTT auth failed, code %d", c->id, mm.ack));
              c->is_closing = 1;
            }
            break;
          case MQTT_CMD_PUBLISH: {
            MG_DEBUG(("%lu [%.*s] -> [%.*s]", c->id, (int) mm.topic.len,
                      mm.topic.ptr, (int) mm.data.len, mm.data.ptr));
            if (mm.qos > 0) {
              uint16_t id = mg_ntohs(mm.id);
              uint32_t remaining_len = sizeof(id);
              if (c->is_mqtt5) remaining_len += 2;  // 3.4.2

              mg_mqtt_send_header(
                  c,
                  (uint8_t) (mm.qos == 2 ? MQTT_CMD_PUBREC : MQTT_CMD_PUBACK),
                  0, remaining_len);
              mg_send(c, &id, sizeof(id));

              if (c->is_mqtt5) {
                uint16_t zero = 0;
                mg_send(c, &zero, sizeof(zero));
              }
            }
            mg_call(c, MG_EV_MQTT_MSG, &mm);  // let the app handle qos stuff
            break;
          }
          case MQTT_CMD_PUBREC: {  // MQTT5: 3.5.2-1 TODO(): variable header rc
            uint16_t id = mg_ntohs(mm.id);
            uint32_t remaining_len = sizeof(id);  // MQTT5 3.6.2-1
            mg_mqtt_send_header(c, MQTT_CMD_PUBREL, 2, remaining_len);
            mg_send(c, &id, sizeof(id));  // MQTT5 3.6.1-1, flags = 2
            break;
          }
          case MQTT_CMD_PUBREL: {  // MQTT5: 3.6.2-1 TODO(): variable header rc
            uint16_t id = mg_ntohs(mm.id);
            uint32_t remaining_len = sizeof(id);  // MQTT5 3.7.2-1
            mg_mqtt_send_header(c, MQTT_CMD_PUBCOMP, 0, remaining_len);
            mg_send(c, &id, sizeof(id));
            break;
          }
        }
        mg_call(c, MG_EV_MQTT_CMD, &mm);
        mg_iobuf_del(&c->recv, 0, mm.dgram.len);
      } else {
        break;
      }
    }
  }
  (void) ev_data;
  (void) fn_data;
}

void mg_mqtt_ping(struct mg_connection *nc) {
  mg_mqtt_send_header(nc, MQTT_CMD_PINGREQ, 0, 0);
}

void mg_mqtt_pong(struct mg_connection *nc) {
  mg_mqtt_send_header(nc, MQTT_CMD_PINGRESP, 0, 0);
}

void mg_mqtt_disconnect(struct mg_connection *c,
                        const struct mg_mqtt_opts *opts) {
  size_t len = 0;
  if (c->is_mqtt5) len = 1 + get_props_size(opts->props, opts->num_props);
  mg_mqtt_send_header(c, MQTT_CMD_DISCONNECT, 0, (uint32_t) len);

  if (c->is_mqtt5) {
    uint8_t zero = 0;
    mg_send(c, &zero, sizeof(zero));  // reason code
    mg_send_mqtt_properties(c, opts->props, opts->num_props);
  }
}

struct mg_connection *mg_mqtt_connect(struct mg_mgr *mgr, const char *url,
                                      const struct mg_mqtt_opts *opts,
                                      mg_event_handler_t fn, void *fn_data) {
  struct mg_connection *c = mg_connect(mgr, url, fn, fn_data);
  if (c != NULL) {
    struct mg_mqtt_opts empty;
    memset(&empty, 0, sizeof(empty));
    mg_mqtt_login(c, opts == NULL ? &empty : opts);
    c->pfn = mqtt_cb;
  }
  return c;
}

struct mg_connection *mg_mqtt_listen(struct mg_mgr *mgr, const char *url,
                                     mg_event_handler_t fn, void *fn_data) {
  struct mg_connection *c = mg_listen(mgr, url, fn, fn_data);
  if (c != NULL) c->pfn = mqtt_cb, c->pfn_data = mgr;
  return c;
}

#ifdef MG_ENABLE_LINES
#line 1 "src/net.c"
#endif








size_t mg_vprintf(struct mg_connection *c, const char *fmt, va_list *ap) {
  size_t old = c->send.len;
  mg_vxprintf(mg_pfn_iobuf, &c->send, fmt, ap);
  return c->send.len - old;
}

size_t mg_printf(struct mg_connection *c, const char *fmt, ...) {
  size_t len = 0;
  va_list ap;
  va_start(ap, fmt);
  len = mg_vprintf(c, fmt, &ap);
  va_end(ap);
  return len;
}

static bool mg_atonl(struct mg_str str, struct mg_addr *addr) {
  uint32_t localhost = mg_htonl(0x7f000001);
  if (mg_vcasecmp(&str, "localhost") != 0) return false;
  memcpy(addr->ip, &localhost, sizeof(uint32_t));
  addr->is_ip6 = false;
  return true;
}

static bool mg_atone(struct mg_str str, struct mg_addr *addr) {
  if (str.len > 0) return false;
  memset(addr->ip, 0, sizeof(addr->ip));
  addr->is_ip6 = false;
  return true;
}

static bool mg_aton4(struct mg_str str, struct mg_addr *addr) {
  uint8_t data[4] = {0, 0, 0, 0};
  size_t i, num_dots = 0;
  for (i = 0; i < str.len; i++) {
    if (str.ptr[i] >= '0' && str.ptr[i] <= '9') {
      int octet = data[num_dots] * 10 + (str.ptr[i] - '0');
      if (octet > 255) return false;
      data[num_dots] = (uint8_t) octet;
    } else if (str.ptr[i] == '.') {
      if (num_dots >= 3 || i == 0 || str.ptr[i - 1] == '.') return false;
      num_dots++;
    } else {
      return false;
    }
  }
  if (num_dots != 3 || str.ptr[i - 1] == '.') return false;
  memcpy(&addr->ip, data, sizeof(data));
  addr->is_ip6 = false;
  return true;
}

static bool mg_v4mapped(struct mg_str str, struct mg_addr *addr) {
  int i;
  uint32_t ipv4;
  if (str.len < 14) return false;
  if (str.ptr[0] != ':' || str.ptr[1] != ':' || str.ptr[6] != ':') return false;
  for (i = 2; i < 6; i++) {
    if (str.ptr[i] != 'f' && str.ptr[i] != 'F') return false;
  }
  // struct mg_str s = mg_str_n(&str.ptr[7], str.len - 7);
  if (!mg_aton4(mg_str_n(&str.ptr[7], str.len - 7), addr)) return false;
  memcpy(&ipv4, addr->ip, sizeof(ipv4));
  memset(addr->ip, 0, sizeof(addr->ip));
  addr->ip[10] = addr->ip[11] = 255;
  memcpy(&addr->ip[12], &ipv4, 4);
  addr->is_ip6 = true;
  return true;
}

static bool mg_aton6(struct mg_str str, struct mg_addr *addr) {
  size_t i, j = 0, n = 0, dc = 42;
  addr->scope_id = 0;
  if (str.len > 2 && str.ptr[0] == '[') str.ptr++, str.len -= 2;
  if (mg_v4mapped(str, addr)) return true;
  for (i = 0; i < str.len; i++) {
    if ((str.ptr[i] >= '0' && str.ptr[i] <= '9') ||
        (str.ptr[i] >= 'a' && str.ptr[i] <= 'f') ||
        (str.ptr[i] >= 'A' && str.ptr[i] <= 'F')) {
      unsigned long val;
      if (i > j + 3) return false;
      // MG_DEBUG(("%lu %lu [%.*s]", i, j, (int) (i - j + 1), &str.ptr[j]));
      val = mg_unhexn(&str.ptr[j], i - j + 1);
      addr->ip[n] = (uint8_t) ((val >> 8) & 255);
      addr->ip[n + 1] = (uint8_t) (val & 255);
    } else if (str.ptr[i] == ':') {
      j = i + 1;
      if (i > 0 && str.ptr[i - 1] == ':') {
        dc = n;  // Double colon
        if (i > 1 && str.ptr[i - 2] == ':') return false;
      } else if (i > 0) {
        n += 2;
      }
      if (n > 14) return false;
      addr->ip[n] = addr->ip[n + 1] = 0;  // For trailing ::
    } else if (str.ptr[i] == '%') {       // Scope ID
      for (i = i + 1; i < str.len; i++) {
        if (str.ptr[i] < '0' || str.ptr[i] > '9') return false;
        addr->scope_id *= 10, addr->scope_id += (uint8_t) (str.ptr[i] - '0');
      }
    } else {
      return false;
    }
  }
  if (n < 14 && dc == 42) return false;
  if (n < 14) {
    memmove(&addr->ip[dc + (14 - n)], &addr->ip[dc], n - dc + 2);
    memset(&addr->ip[dc], 0, 14 - n);
  }

  addr->is_ip6 = true;
  return true;
}

bool mg_aton(struct mg_str str, struct mg_addr *addr) {
  // MG_INFO(("[%.*s]", (int) str.len, str.ptr));
  return mg_atone(str, addr) || mg_atonl(str, addr) || mg_aton4(str, addr) ||
         mg_aton6(str, addr);
}

struct mg_connection *mg_alloc_conn(struct mg_mgr *mgr) {
  struct mg_connection *c =
      (struct mg_connection *) calloc(1, sizeof(*c) + mgr->extraconnsize);
  if (c != NULL) {
    c->mgr = mgr;
    c->send.align = c->recv.align = MG_IO_SIZE;
    c->id = ++mgr->nextid;
  }
  return c;
}

void mg_close_conn(struct mg_connection *c) {
  mg_resolve_cancel(c);  // Close any pending DNS query
  LIST_DELETE(struct mg_connection, &c->mgr->conns, c);
  if (c == c->mgr->dns4.c) c->mgr->dns4.c = NULL;
  if (c == c->mgr->dns6.c) c->mgr->dns6.c = NULL;
  // Order of operations is important. `MG_EV_CLOSE` event must be fired
  // before we deallocate received data, see #1331
  mg_call(c, MG_EV_CLOSE, NULL);
  MG_DEBUG(("%lu %ld closed", c->id, c->fd));

  mg_tls_free(c);
  mg_iobuf_free(&c->recv);
  mg_iobuf_free(&c->send);
  mg_bzero((unsigned char *) c, sizeof(*c));
  free(c);
}

struct mg_connection *mg_connect(struct mg_mgr *mgr, const char *url,
                                 mg_event_handler_t fn, void *fn_data) {
  struct mg_connection *c = NULL;
  if (url == NULL || url[0] == '\0') {
    MG_ERROR(("null url"));
  } else if ((c = mg_alloc_conn(mgr)) == NULL) {
    MG_ERROR(("OOM"));
  } else {
    LIST_ADD_HEAD(struct mg_connection, &mgr->conns, c);
    c->is_udp = (strncmp(url, "udp:", 4) == 0);
    c->fd = (void *) (size_t) MG_INVALID_SOCKET;
    c->fn = fn;
    c->is_client = true;
    c->fn_data = fn_data;
    MG_DEBUG(("%lu %ld %s", c->id, c->fd, url));
    mg_call(c, MG_EV_OPEN, (void *) url);
    mg_resolve(c, url);
  }
  return c;
}

struct mg_connection *mg_listen(struct mg_mgr *mgr, const char *url,
                                mg_event_handler_t fn, void *fn_data) {
  struct mg_connection *c = NULL;
  if ((c = mg_alloc_conn(mgr)) == NULL) {
    MG_ERROR(("OOM %s", url));
  } else if (!mg_open_listener(c, url)) {
    MG_ERROR(("Failed: %s, errno %d", url, errno));
    free(c);
    c = NULL;
  } else {
    c->is_listening = 1;
    c->is_udp = strncmp(url, "udp:", 4) == 0;
    LIST_ADD_HEAD(struct mg_connection, &mgr->conns, c);
    c->fn = fn;
    c->fn_data = fn_data;
    mg_call(c, MG_EV_OPEN, NULL);
    if (mg_url_is_ssl(url)) c->is_tls = 1;  // Accepted connection must
    MG_DEBUG(("%lu %ld %s", c->id, c->fd, url));
  }
  return c;
}

struct mg_connection *mg_wrapfd(struct mg_mgr *mgr, int fd,
                                mg_event_handler_t fn, void *fn_data) {
  struct mg_connection *c = mg_alloc_conn(mgr);
  if (c != NULL) {
    c->fd = (void *) (size_t) fd;
    c->fn = fn;
    c->fn_data = fn_data;
    MG_EPOLL_ADD(c);
    mg_call(c, MG_EV_OPEN, NULL);
    LIST_ADD_HEAD(struct mg_connection, &mgr->conns, c);
  }
  return c;
}

struct mg_timer *mg_timer_add(struct mg_mgr *mgr, uint64_t milliseconds,
                              unsigned flags, void (*fn)(void *), void *arg) {
  struct mg_timer *t = (struct mg_timer *) calloc(1, sizeof(*t));
  if (t != NULL) {
    mg_timer_init(&mgr->timers, t, milliseconds, flags, fn, arg);
    t->id = mgr->timerid++;
  }
  return t;
}

void mg_mgr_free(struct mg_mgr *mgr) {
  struct mg_connection *c;
  struct mg_timer *tmp, *t = mgr->timers;
  while (t != NULL) tmp = t->next, free(t), t = tmp;
  mgr->timers = NULL;  // Important. Next call to poll won't touch timers
  for (c = mgr->conns; c != NULL; c = c->next) c->is_closing = 1;
  mg_mgr_poll(mgr, 0);
#if MG_ENABLE_FREERTOS_TCP
  FreeRTOS_DeleteSocketSet(mgr->ss);
#endif
  MG_DEBUG(("All connections closed"));
#if MG_ENABLE_EPOLL
  if (mgr->epoll_fd >= 0) close(mgr->epoll_fd), mgr->epoll_fd = -1;
#endif
  mg_tls_ctx_free(mgr);
}

void mg_mgr_init(struct mg_mgr *mgr) {
  memset(mgr, 0, sizeof(*mgr));
#if MG_ENABLE_EPOLL
  if ((mgr->epoll_fd = epoll_create1(EPOLL_CLOEXEC)) < 0)
    MG_ERROR(("epoll_create1 errno %d", errno));
#else
  mgr->epoll_fd = -1;
#endif
#if MG_ARCH == MG_ARCH_WIN32 && MG_ENABLE_WINSOCK
  // clang-format off
  { WSADATA data; WSAStartup(MAKEWORD(2, 2), &data); }
  // clang-format on
#elif MG_ENABLE_FREERTOS_TCP
  mgr->ss = FreeRTOS_CreateSocketSet();
#elif defined(__unix) || defined(__unix__) || defined(__APPLE__)
  // Ignore SIGPIPE signal, so if client cancels the request, it
  // won't kill the whole process.
  signal(SIGPIPE, SIG_IGN);
#endif
  mgr->dnstimeout = 3000;
  mgr->dns4.url = "udp://8.8.8.8:53";
  mgr->dns6.url = "udp://[2001:4860:4860::8888]:53";
  mg_tls_ctx_init(mgr);
}

#ifdef MG_ENABLE_LINES
#line 1 "src/net_builtin.c"
#endif


#if defined(MG_ENABLE_TCPIP) && MG_ENABLE_TCPIP
#define MG_EPHEMERAL_PORT_BASE 32768
#define PDIFF(a, b) ((size_t) (((char *) (b)) - ((char *) (a))))

#ifndef MIP_TCP_KEEPALIVE_MS
#define MIP_TCP_KEEPALIVE_MS 45000  // TCP keep-alive period, ms
#endif

#define MIP_TCP_ACK_MS 150    // Timeout for ACKing
#define MIP_TCP_ARP_MS 100    // Timeout for ARP response
#define MIP_TCP_SYN_MS 15000  // Timeout for connection establishment
#define MIP_TCP_FIN_MS 1000   // Timeout for closing connection

struct connstate {
  uint32_t seq, ack;           // TCP seq/ack counters
  uint64_t timer;              // TCP keep-alive / ACK timer
  uint8_t mac[6];              // Peer MAC address
  uint8_t ttype;               // Timer type. 0: ack, 1: keep-alive
#define MIP_TTYPE_KEEPALIVE 0  // Connection is idle for long, send keepalive
#define MIP_TTYPE_ACK 1        // Peer sent us data, we have to ack it soon
#define MIP_TTYPE_ARP 2        // ARP resolve sent, waiting for response
#define MIP_TTYPE_SYN 3        // SYN sent, waiting for response
#define MIP_TTYPE_FIN 4  // FIN sent, waiting until terminating the connection
  uint8_t tmiss;         // Number of keep-alive misses
  struct mg_iobuf raw;   // For TLS only. Incoming raw data
};

#pragma pack(push, 1)

struct lcp {
  uint8_t addr, ctrl, proto[2], code, id, len[2];
};

struct eth {
  uint8_t dst[6];  // Destination MAC address
  uint8_t src[6];  // Source MAC address
  uint16_t type;   // Ethernet type
};

struct ip {
  uint8_t ver;    // Version
  uint8_t tos;    // Unused
  uint16_t len;   // Length
  uint16_t id;    // Unused
  uint16_t frag;  // Fragmentation
#define IP_FRAG_OFFSET_MSK 0xFF1F
#define IP_MORE_FRAGS_MSK 0x20
  uint8_t ttl;    // Time to live
  uint8_t proto;  // Upper level protocol
  uint16_t csum;  // Checksum
  uint32_t src;   // Source IP
  uint32_t dst;   // Destination IP
};

struct ip6 {
  uint8_t ver;      // Version
  uint8_t opts[3];  // Options
  uint16_t len;     // Length
  uint8_t proto;    // Upper level protocol
  uint8_t ttl;      // Time to live
  uint8_t src[16];  // Source IP
  uint8_t dst[16];  // Destination IP
};

struct icmp {
  uint8_t type;
  uint8_t code;
  uint16_t csum;
};

struct arp {
  uint16_t fmt;    // Format of hardware address
  uint16_t pro;    // Format of protocol address
  uint8_t hlen;    // Length of hardware address
  uint8_t plen;    // Length of protocol address
  uint16_t op;     // Operation
  uint8_t sha[6];  // Sender hardware address
  uint32_t spa;    // Sender protocol address
  uint8_t tha[6];  // Target hardware address
  uint32_t tpa;    // Target protocol address
};

struct tcp {
  uint16_t sport;  // Source port
  uint16_t dport;  // Destination port
  uint32_t seq;    // Sequence number
  uint32_t ack;    // Acknowledgement number
  uint8_t off;     // Data offset
  uint8_t flags;   // TCP flags
#define TH_FIN 0x01
#define TH_SYN 0x02
#define TH_RST 0x04
#define TH_PUSH 0x08
#define TH_ACK 0x10
#define TH_URG 0x20
#define TH_ECE 0x40
#define TH_CWR 0x80
  uint16_t win;   // Window
  uint16_t csum;  // Checksum
  uint16_t urp;   // Urgent pointer
};

struct udp {
  uint16_t sport;  // Source port
  uint16_t dport;  // Destination port
  uint16_t len;    // UDP length
  uint16_t csum;   // UDP checksum
};

struct dhcp {
  uint8_t op, htype, hlen, hops;
  uint32_t xid;
  uint16_t secs, flags;
  uint32_t ciaddr, yiaddr, siaddr, giaddr;
  uint8_t hwaddr[208];
  uint32_t magic;
  uint8_t options[32];
};

#pragma pack(pop)

struct pkt {
  struct mg_str raw;  // Raw packet data
  struct mg_str pay;  // Payload data
  struct eth *eth;
  struct llc *llc;
  struct arp *arp;
  struct ip *ip;
  struct ip6 *ip6;
  struct icmp *icmp;
  struct tcp *tcp;
  struct udp *udp;
  struct dhcp *dhcp;
};

static void send_syn(struct mg_connection *c);

static void mkpay(struct pkt *pkt, void *p) {
  pkt->pay =
      mg_str_n((char *) p, (size_t) (&pkt->raw.ptr[pkt->raw.len] - (char *) p));
}

static uint32_t csumup(uint32_t sum, const void *buf, size_t len) {
  const uint8_t *p = (const uint8_t *) buf;
  for (size_t i = 0; i < len; i++) sum += i & 1 ? p[i] : (uint32_t) (p[i] << 8);
  return sum;
}

static uint16_t csumfin(uint32_t sum) {
  while (sum >> 16) sum = (sum & 0xffff) + (sum >> 16);
  return mg_htons(~sum & 0xffff);
}

static uint16_t ipcsum(const void *buf, size_t len) {
  uint32_t sum = csumup(0, buf, len);
  return csumfin(sum);
}

static void settmout(struct mg_connection *c, uint8_t type) {
  struct mg_tcpip_if *ifp = (struct mg_tcpip_if *) c->mgr->priv;
  struct connstate *s = (struct connstate *) (c + 1);
  unsigned n = type == MIP_TTYPE_ACK   ? MIP_TCP_ACK_MS
               : type == MIP_TTYPE_ARP ? MIP_TCP_ARP_MS
               : type == MIP_TTYPE_SYN ? MIP_TCP_SYN_MS
               : type == MIP_TTYPE_FIN ? MIP_TCP_FIN_MS
                                       : MIP_TCP_KEEPALIVE_MS;
  s->timer = ifp->now + n;
  s->ttype = type;
  MG_VERBOSE(("%lu %d -> %llx", c->id, type, s->timer));
}

static size_t ether_output(struct mg_tcpip_if *ifp, size_t len) {
  // size_t min = 64;  // Pad short frames to 64 bytes (minimum Ethernet size)
  // if (len < min) memset(ifp->tx.ptr + len, 0, min - len), len = min;
  // mg_hexdump(ifp->tx.ptr, len);
  size_t n = ifp->driver->tx(ifp->tx.ptr, len, ifp);
  if (n == len) ifp->nsent++;
  return n;
}

static void arp_ask(struct mg_tcpip_if *ifp, uint32_t ip) {
  struct eth *eth = (struct eth *) ifp->tx.ptr;
  struct arp *arp = (struct arp *) (eth + 1);
  memset(eth->dst, 255, sizeof(eth->dst));
  memcpy(eth->src, ifp->mac, sizeof(eth->src));
  eth->type = mg_htons(0x806);
  memset(arp, 0, sizeof(*arp));
  arp->fmt = mg_htons(1), arp->pro = mg_htons(0x800), arp->hlen = 6,
  arp->plen = 4;
  arp->op = mg_htons(1), arp->tpa = ip, arp->spa = ifp->ip;
  memcpy(arp->sha, ifp->mac, sizeof(arp->sha));
  ether_output(ifp, PDIFF(eth, arp + 1));
}

static void onstatechange(struct mg_tcpip_if *ifp) {
  if (ifp->state == MG_TCPIP_STATE_READY) {
    MG_INFO(("READY, IP: %M", mg_print_ip4, &ifp->ip));
    MG_INFO(("       GW: %M", mg_print_ip4, &ifp->gw));
    MG_INFO(("      MAC: %M", mg_print_mac, &ifp->mac));
    arp_ask(ifp, ifp->gw);
  } else if (ifp->state == MG_TCPIP_STATE_UP) {
    MG_ERROR(("Link up"));
    srand((unsigned int) mg_millis());
  } else if (ifp->state == MG_TCPIP_STATE_DOWN) {
    MG_ERROR(("Link down"));
  }
}

static struct ip *tx_ip(struct mg_tcpip_if *ifp, uint8_t *mac_dst,
                        uint8_t proto, uint32_t ip_src, uint32_t ip_dst,
                        size_t plen) {
  struct eth *eth = (struct eth *) ifp->tx.ptr;
  struct ip *ip = (struct ip *) (eth + 1);
  memcpy(eth->dst, mac_dst, sizeof(eth->dst));
  memcpy(eth->src, ifp->mac, sizeof(eth->src));  // Use our MAC
  eth->type = mg_htons(0x800);
  memset(ip, 0, sizeof(*ip));
  ip->ver = 0x45;   // Version 4, header length 5 words
  ip->frag = 0x40;  // Don't fragment
  ip->len = mg_htons((uint16_t) (sizeof(*ip) + plen));
  ip->ttl = 64;
  ip->proto = proto;
  ip->src = ip_src;
  ip->dst = ip_dst;
  ip->csum = ipcsum(ip, sizeof(*ip));
  return ip;
}

static void tx_udp(struct mg_tcpip_if *ifp, uint8_t *mac_dst, uint32_t ip_src,
                   uint16_t sport, uint32_t ip_dst, uint16_t dport,
                   const void *buf, size_t len) {
  struct ip *ip =
      tx_ip(ifp, mac_dst, 17, ip_src, ip_dst, len + sizeof(struct udp));
  struct udp *udp = (struct udp *) (ip + 1);
  // MG_DEBUG(("UDP XX LEN %d %d", (int) len, (int) ifp->tx.len));
  udp->sport = sport;
  udp->dport = dport;
  udp->len = mg_htons((uint16_t) (sizeof(*udp) + len));
  udp->csum = 0;
  uint32_t cs = csumup(0, udp, sizeof(*udp));
  cs = csumup(cs, buf, len);
  cs = csumup(cs, &ip->src, sizeof(ip->src));
  cs = csumup(cs, &ip->dst, sizeof(ip->dst));
  cs += (uint32_t) (ip->proto + sizeof(*udp) + len);
  udp->csum = csumfin(cs);
  memmove(udp + 1, buf, len);
  // MG_DEBUG(("UDP LEN %d %d", (int) len, (int) ifp->frame_len));
  ether_output(ifp, sizeof(struct eth) + sizeof(*ip) + sizeof(*udp) + len);
}

static void tx_dhcp(struct mg_tcpip_if *ifp, uint8_t *mac_dst, uint32_t ip_src,
                    uint32_t ip_dst, uint8_t *opts, size_t optslen,
                    bool ciaddr) {
  // https://datatracker.ietf.org/doc/html/rfc2132#section-9.6
  struct dhcp dhcp = {1, 1, 6, 0, 0, 0, 0, 0, 0, 0, 0, {0}, 0, {0}};
  dhcp.magic = mg_htonl(0x63825363);
  memcpy(&dhcp.hwaddr, ifp->mac, sizeof(ifp->mac));
  memcpy(&dhcp.xid, ifp->mac + 2, sizeof(dhcp.xid));
  memcpy(&dhcp.options, opts, optslen);
  if (ciaddr) dhcp.ciaddr = ip_src;
  tx_udp(ifp, mac_dst, ip_src, mg_htons(68), ip_dst, mg_htons(67), &dhcp,
         sizeof(dhcp));
}

static const uint8_t broadcast[] = {255, 255, 255, 255, 255, 255};

// RFC-2131 #4.3.6, #4.4.1
static void tx_dhcp_request_sel(struct mg_tcpip_if *ifp, uint32_t ip_req,
                                uint32_t ip_srv) {
  uint8_t opts[] = {
      53, 1, 3,                 // Type: DHCP request
      55, 2, 1,   3,            // GW and mask
      12, 3, 'm', 'i', 'p',     // Host name: "mip"
      54, 4, 0,   0,   0,   0,  // DHCP server ID
      50, 4, 0,   0,   0,   0,  // Requested IP
      255                       // End of options
  };
  memcpy(opts + 14, &ip_srv, sizeof(ip_srv));
  memcpy(opts + 20, &ip_req, sizeof(ip_req));
  tx_dhcp(ifp, (uint8_t *) broadcast, 0, 0xffffffff, opts, sizeof(opts), false);
  MG_DEBUG(("DHCP req sent"));
}

// RFC-2131 #4.3.6, #4.4.5 (renewing: unicast, rebinding: bcast)
static void tx_dhcp_request_re(struct mg_tcpip_if *ifp, uint8_t *mac_dst,
                               uint32_t ip_src, uint32_t ip_dst) {
  uint8_t opts[] = {
      53, 1, 3,  // Type: DHCP request
      255        // End of options
  };
  tx_dhcp(ifp, mac_dst, ip_src, ip_dst, opts, sizeof(opts), true);
  MG_DEBUG(("DHCP req sent"));
}

static void tx_dhcp_discover(struct mg_tcpip_if *ifp) {
  uint8_t opts[] = {
      53, 1, 1,     // Type: DHCP discover
      55, 2, 1, 3,  // Parameters: ip, mask
      255           // End of options
  };
  tx_dhcp(ifp, (uint8_t *) broadcast, 0, 0xffffffff, opts, sizeof(opts), false);
  MG_DEBUG(("DHCP discover sent. Our MAC: %M", mg_print_mac, ifp->mac));
}

static struct mg_connection *getpeer(struct mg_mgr *mgr, struct pkt *pkt,
                                     bool lsn) {
  struct mg_connection *c = NULL;
  for (c = mgr->conns; c != NULL; c = c->next) {
    if (c->is_arplooking && pkt->arp &&
        memcmp(&pkt->arp->spa, c->rem.ip, sizeof(pkt->arp->spa)) == 0)
      break;
    if (c->is_udp && pkt->udp && c->loc.port == pkt->udp->dport) break;
    if (!c->is_udp && pkt->tcp && c->loc.port == pkt->tcp->dport &&
        lsn == c->is_listening && (lsn || c->rem.port == pkt->tcp->sport))
      break;
  }
  return c;
}

static void rx_arp(struct mg_tcpip_if *ifp, struct pkt *pkt) {
  if (pkt->arp->op == mg_htons(1) && pkt->arp->tpa == ifp->ip) {
    // ARP request. Make a response, then send
    // MG_DEBUG(("ARP op %d %M: %M", mg_ntohs(pkt->arp->op), mg_print_ip4,
    //          &pkt->arp->spa, mg_print_ip4, &pkt->arp->tpa));
    struct eth *eth = (struct eth *) ifp->tx.ptr;
    struct arp *arp = (struct arp *) (eth + 1);
    memcpy(eth->dst, pkt->eth->src, sizeof(eth->dst));
    memcpy(eth->src, ifp->mac, sizeof(eth->src));
    eth->type = mg_htons(0x806);
    *arp = *pkt->arp;
    arp->op = mg_htons(2);
    memcpy(arp->tha, pkt->arp->sha, sizeof(pkt->arp->tha));
    memcpy(arp->sha, ifp->mac, sizeof(pkt->arp->sha));
    arp->tpa = pkt->arp->spa;
    arp->spa = ifp->ip;
    MG_DEBUG(("ARP: tell %M we're %M", mg_print_ip4, &arp->tpa, mg_print_mac,
              &ifp->mac));
    ether_output(ifp, PDIFF(eth, arp + 1));
  } else if (pkt->arp->op == mg_htons(2)) {
    if (memcmp(pkt->arp->tha, ifp->mac, sizeof(pkt->arp->tha)) != 0) return;
    if (pkt->arp->spa == ifp->gw) {
      // Got response for the GW ARP request. Set ifp->gwmac
      memcpy(ifp->gwmac, pkt->arp->sha, sizeof(ifp->gwmac));
    } else {
      struct mg_connection *c = getpeer(ifp->mgr, pkt, false);
      if (c != NULL && c->is_arplooking) {
        struct connstate *s = (struct connstate *) (c + 1);
        memcpy(s->mac, pkt->arp->sha, sizeof(s->mac));
        MG_DEBUG(("%lu ARP resolved %M -> %M", c->id, mg_print_ip4, c->rem.ip,
                  mg_print_mac, s->mac));
        c->is_arplooking = 0;
        send_syn(c);
        settmout(c, MIP_TTYPE_SYN);
      }
    }
  }
}

static void rx_icmp(struct mg_tcpip_if *ifp, struct pkt *pkt) {
  // MG_DEBUG(("ICMP %d", (int) len));
  if (pkt->icmp->type == 8 && pkt->ip != NULL && pkt->ip->dst == ifp->ip) {
    size_t hlen = sizeof(struct eth) + sizeof(struct ip) + sizeof(struct icmp);
    size_t space = ifp->tx.len - hlen, plen = pkt->pay.len;
    if (plen > space) plen = space;
    struct ip *ip = tx_ip(ifp, pkt->eth->src, 1, ifp->ip, pkt->ip->src,
                          sizeof(struct icmp) + plen);
    struct icmp *icmp = (struct icmp *) (ip + 1);
    memset(icmp, 0, sizeof(*icmp));        // Set csum to 0
    memcpy(icmp + 1, pkt->pay.ptr, plen);  // Copy RX payload to TX
    icmp->csum = ipcsum(icmp, sizeof(*icmp) + plen);
    ether_output(ifp, hlen + plen);
  }
}

static void rx_dhcp_client(struct mg_tcpip_if *ifp, struct pkt *pkt) {
  uint32_t ip = 0, gw = 0, mask = 0, lease = 0;
  uint8_t msgtype = 0, state = ifp->state;
  // perform size check first, then access fields
  uint8_t *p = pkt->dhcp->options,
          *end = (uint8_t *) &pkt->raw.ptr[pkt->raw.len];
  if (end < (uint8_t *) (pkt->dhcp + 1)) return;
  if (memcmp(&pkt->dhcp->xid, ifp->mac + 2, sizeof(pkt->dhcp->xid))) return;
  while (p + 1 < end && p[0] != 255) {  // Parse options RFC-1533 #9
    if (p[0] == 1 && p[1] == sizeof(ifp->mask) && p + 6 < end) {  // Mask
      memcpy(&mask, p + 2, sizeof(mask));
    } else if (p[0] == 3 && p[1] == sizeof(ifp->gw) && p + 6 < end) {  // GW
      memcpy(&gw, p + 2, sizeof(gw));
      ip = pkt->dhcp->yiaddr;
    } else if (p[0] == 51 && p[1] == 4 && p + 6 < end) {  // Lease
      memcpy(&lease, p + 2, sizeof(lease));
      lease = mg_ntohl(lease);
    } else if (p[0] == 53 && p[1] == 1 && p + 6 < end) {  // Msg Type
      msgtype = p[2];
    }
    p += p[1] + 2;
  }
  // Process message type, RFC-1533 (9.4); RFC-2131 (3.1, 4)
  if (msgtype == 6 && ifp->ip == ip) {  // DHCPNACK, release IP
    ifp->state = MG_TCPIP_STATE_UP, ifp->ip = 0;
  } else if (msgtype == 2 && ifp->state == MG_TCPIP_STATE_UP && ip && gw &&
             lease) {                                 // DHCPOFFER
    tx_dhcp_request_sel(ifp, ip, pkt->dhcp->siaddr);  // select IP, (4.4.1)
    ifp->state = MG_TCPIP_STATE_REQ;                  // REQUESTING state
  } else if (msgtype == 5) {                          // DHCPACK
    if (ifp->state == MG_TCPIP_STATE_REQ && ip && gw && lease) {  // got an IP
      ifp->lease_expire = ifp->now + lease * 1000;
      MG_INFO(("Lease: %u sec (%lld)", lease, ifp->lease_expire / 1000));
      // assume DHCP server = router until ARP resolves
      memcpy(ifp->gwmac, pkt->eth->src, sizeof(ifp->gwmac));
      ifp->ip = ip, ifp->gw = gw, ifp->mask = mask;
      ifp->state = MG_TCPIP_STATE_READY;  // BOUND state
      uint64_t rand;
      mg_random(&rand, sizeof(rand));
      srand((unsigned int) (rand + mg_millis()));
    } else if (ifp->state == MG_TCPIP_STATE_READY && ifp->ip == ip) {  // renew
      ifp->lease_expire = ifp->now + lease * 1000;
      MG_INFO(("Lease: %u sec (%lld)", lease, ifp->lease_expire / 1000));
    }  // TODO(): accept provided T1/T2 and store server IP for renewal (4.4)
  }
  if (ifp->state != state) onstatechange(ifp);
}

// Simple DHCP server that assigns a next IP address: ifp->ip + 1
static void rx_dhcp_server(struct mg_tcpip_if *ifp, struct pkt *pkt) {
  uint8_t op = 0, *p = pkt->dhcp->options,
          *end = (uint8_t *) &pkt->raw.ptr[pkt->raw.len];
  if (end < (uint8_t *) (pkt->dhcp + 1)) return;
  // struct dhcp *req = pkt->dhcp;
  struct dhcp res = {2, 1, 6, 0, 0, 0, 0, 0, 0, 0, 0, {0}, 0, {0}};
  res.yiaddr = ifp->ip;
  ((uint8_t *) (&res.yiaddr))[3]++;                // Offer our IP + 1
  while (p + 1 < end && p[0] != 255) {             // Parse options
    if (p[0] == 53 && p[1] == 1 && p + 2 < end) {  // Message type
      op = p[2];
    }
    p += p[1] + 2;
  }
  if (op == 1 || op == 3) {         // DHCP Discover or DHCP Request
    uint8_t msg = op == 1 ? 2 : 5;  // Message type: DHCP OFFER or DHCP ACK
    uint8_t opts[] = {
        53, 1, msg,                 // Message type
        1,  4, 0,   0,   0,   0,    // Subnet mask
        54, 4, 0,   0,   0,   0,    // Server ID
        12, 3, 'm', 'i', 'p',       // Host name: "mip"
        51, 4, 255, 255, 255, 255,  // Lease time
        255                         // End of options
    };
    memcpy(&res.hwaddr, pkt->dhcp->hwaddr, 6);
    memcpy(opts + 5, &ifp->mask, sizeof(ifp->mask));
    memcpy(opts + 11, &ifp->ip, sizeof(ifp->ip));
    memcpy(&res.options, opts, sizeof(opts));
    res.magic = pkt->dhcp->magic;
    res.xid = pkt->dhcp->xid;
    // memcpy(ifp->gwmac, pkt->eth->src, sizeof(ifp->gwmac));
    tx_udp(ifp, pkt->eth->src, ifp->ip, mg_htons(67),
           op == 1 ? ~0U : res.yiaddr, mg_htons(68), &res, sizeof(res));
  }
}

static void rx_udp(struct mg_tcpip_if *ifp, struct pkt *pkt) {
  struct mg_connection *c = getpeer(ifp->mgr, pkt, true);
  if (c == NULL) {
    // No UDP listener on this port. Should send ICMP, but keep silent.
  } else {
    c->rem.port = pkt->udp->sport;
    memcpy(c->rem.ip, &pkt->ip->src, sizeof(uint32_t));
    struct connstate *s = (struct connstate *) (c + 1);
    memcpy(s->mac, pkt->eth->src, sizeof(s->mac));
    if (c->recv.len >= MG_MAX_RECV_SIZE) {
      mg_error(c, "max_recv_buf_size reached");
    } else if (c->recv.size - c->recv.len < pkt->pay.len &&
               !mg_iobuf_resize(&c->recv, c->recv.len + pkt->pay.len)) {
      mg_error(c, "oom");
    } else {
      memcpy(&c->recv.buf[c->recv.len], pkt->pay.ptr, pkt->pay.len);
      c->recv.len += pkt->pay.len;
      mg_call(c, MG_EV_READ, &pkt->pay.len);
    }
  }
}

static size_t tx_tcp(struct mg_tcpip_if *ifp, uint8_t *dst_mac, uint32_t dst_ip,
                     uint8_t flags, uint16_t sport, uint16_t dport,
                     uint32_t seq, uint32_t ack, const void *buf, size_t len) {
  struct ip *ip =
      tx_ip(ifp, dst_mac, 6, ifp->ip, dst_ip, sizeof(struct tcp) + len);
  struct tcp *tcp = (struct tcp *) (ip + 1);
  memset(tcp, 0, sizeof(*tcp));
  if (buf != NULL && len) memmove(tcp + 1, buf, len);
  tcp->sport = sport;
  tcp->dport = dport;
  tcp->seq = seq;
  tcp->ack = ack;
  tcp->flags = flags;
  tcp->win = mg_htons(8192);
  tcp->off = (uint8_t) (sizeof(*tcp) / 4 << 4);
  uint32_t cs = 0;
  uint16_t n = (uint16_t) (sizeof(*tcp) + len);
  uint8_t pseudo[] = {0, ip->proto, (uint8_t) (n >> 8), (uint8_t) (n & 255)};
  cs = csumup(cs, tcp, n);
  cs = csumup(cs, &ip->src, sizeof(ip->src));
  cs = csumup(cs, &ip->dst, sizeof(ip->dst));
  cs = csumup(cs, pseudo, sizeof(pseudo));
  tcp->csum = csumfin(cs);
  MG_VERBOSE(("TCP %M:%hu -> %M:%hu fl %x len %u", mg_print_ip4, &ip->src,
              mg_ntohs(tcp->sport), mg_print_ip4, &ip->dst,
              mg_ntohs(tcp->dport), tcp->flags, (int) len));
  // mg_hexdump(ifp->tx.ptr, PDIFF(ifp->tx.ptr, tcp + 1) + len);
  return ether_output(ifp, PDIFF(ifp->tx.ptr, tcp + 1) + len);
}

static size_t tx_tcp_pkt(struct mg_tcpip_if *ifp, struct pkt *pkt,
                         uint8_t flags, uint32_t seq, const void *buf,
                         size_t len) {
  uint32_t delta = (pkt->tcp->flags & (TH_SYN | TH_FIN)) ? 1 : 0;
  return tx_tcp(ifp, pkt->eth->src, pkt->ip->src, flags, pkt->tcp->dport,
                pkt->tcp->sport, seq, mg_htonl(mg_ntohl(pkt->tcp->seq) + delta),
                buf, len);
}

static struct mg_connection *accept_conn(struct mg_connection *lsn,
                                         struct pkt *pkt) {
  struct mg_connection *c = mg_alloc_conn(lsn->mgr);
  if (c == NULL) {
    MG_ERROR(("OOM"));
    return NULL;
  }
  struct connstate *s = (struct connstate *) (c + 1);
  s->seq = mg_ntohl(pkt->tcp->ack), s->ack = mg_ntohl(pkt->tcp->seq);
  memcpy(s->mac, pkt->eth->src, sizeof(s->mac));
  settmout(c, MIP_TTYPE_KEEPALIVE);
  memcpy(c->rem.ip, &pkt->ip->src, sizeof(uint32_t));
  c->rem.port = pkt->tcp->sport;
  MG_DEBUG(("%lu accepted %M", c->id, mg_print_ip_port, &c->rem));
  LIST_ADD_HEAD(struct mg_connection, &lsn->mgr->conns, c);
  c->is_accepted = 1;
  c->is_hexdumping = lsn->is_hexdumping;
  c->pfn = lsn->pfn;
  c->loc = lsn->loc;
  c->pfn_data = lsn->pfn_data;
  c->fn = lsn->fn;
  c->fn_data = lsn->fn_data;
  mg_call(c, MG_EV_OPEN, NULL);
  mg_call(c, MG_EV_ACCEPT, NULL);
  return c;
}

static size_t trim_len(struct mg_connection *c, size_t len) {
  struct mg_tcpip_if *ifp = (struct mg_tcpip_if *) c->mgr->priv;
  size_t eth_h_len = 14, ip_max_h_len = 24, tcp_max_h_len = 60, udp_h_len = 8;
  size_t max_headers_len = eth_h_len + ip_max_h_len +
                          (c->is_udp ? udp_h_len : tcp_max_h_len);
  size_t min_mtu = c->is_udp ? 68 /* RFC-791 */ : max_headers_len - eth_h_len;

  // If the frame exceeds the available buffer, trim the length
  if (len + max_headers_len > ifp->tx.len) {
    len = ifp->tx.len - max_headers_len;
  }
  // Ensure the MTU isn't lower than the minimum allowed value
  if (ifp->mtu < min_mtu) {
    MG_ERROR(("MTU is lower than minimum possible value. Setting it to %d.",
              min_mtu));
    ifp->mtu = (uint16_t) min_mtu;
  }
  // If the total packet size exceeds the MTU, trim the length
  if (len + max_headers_len - eth_h_len > ifp->mtu) {
    len = ifp->mtu - max_headers_len + eth_h_len;
    if (c->is_udp) {
      MG_ERROR(("UDP datagram exceeds MTU. Truncating it."));
    }
  }

  return len;
}

long mg_io_send(struct mg_connection *c, const void *buf, size_t len) {
  struct mg_tcpip_if *ifp = (struct mg_tcpip_if *) c->mgr->priv;
  struct connstate *s = (struct connstate *) (c + 1);
  uint32_t rem_ip;
  memcpy(&rem_ip, c->rem.ip, sizeof(uint32_t));
  len = trim_len(c, len);
  if (c->is_udp) {
    tx_udp(ifp, s->mac, ifp->ip, c->loc.port, rem_ip, c->rem.port, buf, len);
  } else {
    if (tx_tcp(ifp, s->mac, rem_ip, TH_PUSH | TH_ACK, c->loc.port, c->rem.port,
               mg_htonl(s->seq), mg_htonl(s->ack), buf, len) > 0) {
      s->seq += (uint32_t) len;
      if (s->ttype == MIP_TTYPE_ACK) settmout(c, MIP_TTYPE_KEEPALIVE);
    } else {
      return MG_IO_ERR;
    }
  }
  return (long) len;
}

long mg_io_recv(struct mg_connection *c, void *buf, size_t len) {
  struct connstate *s = (struct connstate *) (c + 1);
  if (s->raw.len == 0) return MG_IO_WAIT;
  if (len > s->raw.len) len = s->raw.len;
  memcpy(buf, s->raw.buf, len);
  mg_iobuf_del(&s->raw, 0, len);
  return (long) len;
}

static void read_conn(struct mg_connection *c, struct pkt *pkt) {
  struct connstate *s = (struct connstate *) (c + 1);
  struct mg_iobuf *io = c->is_tls ? &s->raw : &c->recv;
  uint32_t seq = mg_ntohl(pkt->tcp->seq);
  s->raw.align = c->recv.align;
  uint32_t rem_ip;
  memcpy(&rem_ip, c->rem.ip, sizeof(uint32_t));
  if (pkt->tcp->flags & TH_FIN) {
    // If we initiated the closure, we reply with ACK upon receiving FIN
    // If we didn't initiate it, we reply with FIN as part of the normal TCP
    // closure process
    uint8_t flags = TH_ACK;
    s->ack = (uint32_t) (mg_htonl(pkt->tcp->seq) + pkt->pay.len + 1);
    if (c->is_draining && s->ttype == MIP_TTYPE_FIN) {
      if (s->seq == mg_htonl(pkt->tcp->ack)) {  // Simultaneous closure ?
        s->seq++;                               // Yes. Increment our SEQ
      } else {                                  // Otherwise,
        s->seq = mg_htonl(pkt->tcp->ack);       // Set to peer's ACK
      }
    } else {
      flags |= TH_FIN;
      c->is_draining = 1;
      settmout(c, MIP_TTYPE_FIN);
    }
    tx_tcp((struct mg_tcpip_if *) c->mgr->priv, s->mac, rem_ip, flags,
           c->loc.port, c->rem.port, mg_htonl(s->seq), mg_htonl(s->ack), "", 0);
  } else if (pkt->pay.len == 0) {
    // TODO(cpq): handle this peer's ACK
  } else if (seq != s->ack) {
    uint32_t ack = (uint32_t) (mg_htonl(pkt->tcp->seq) + pkt->pay.len);
    if (s->ack == ack) {
      MG_VERBOSE(("ignoring duplicate pkt"));
    } else {
      MG_VERBOSE(("SEQ != ACK: %x %x %x", seq, s->ack, ack));
      tx_tcp((struct mg_tcpip_if *) c->mgr->priv, s->mac, rem_ip, TH_ACK,
             c->loc.port, c->rem.port, mg_htonl(s->seq), mg_htonl(s->ack), "",
             0);
    }
  } else if (io->size - io->len < pkt->pay.len &&
             !mg_iobuf_resize(io, io->len + pkt->pay.len)) {
    mg_error(c, "oom");
  } else {
    // Copy TCP payload into the IO buffer. If the connection is plain text,
    // we copy to c->recv. If the connection is TLS, this data is encrypted,
    // therefore we copy that encrypted data to the s->raw iobuffer instead,
    // and then call mg_tls_recv() to decrypt it. NOTE: mg_tls_recv() will
    // call back mg_io_recv() which grabs raw data from s->raw
    memcpy(&io->buf[io->len], pkt->pay.ptr, pkt->pay.len);
    io->len += pkt->pay.len;

    MG_VERBOSE(("%lu SEQ %x -> %x", c->id, mg_htonl(pkt->tcp->seq), s->ack));
    // Advance ACK counter
    s->ack = (uint32_t) (mg_htonl(pkt->tcp->seq) + pkt->pay.len);
#if 0
    // Send ACK immediately
    uint32_t rem_ip;
    memcpy(&rem_ip, c->rem.ip, sizeof(uint32_t));
    MG_DEBUG(("  imm ACK", c->id, mg_htonl(pkt->tcp->seq), s->ack));
    tx_tcp((struct mg_tcpip_if *) c->mgr->priv, s->mac, rem_ip, TH_ACK, c->loc.port,
           c->rem.port, mg_htonl(s->seq), mg_htonl(s->ack), "", 0);
#else
    // if not already running, setup a timer to send an ACK later
    if (s->ttype != MIP_TTYPE_ACK) settmout(c, MIP_TTYPE_ACK);
#endif

    if (c->is_tls) {
      // TLS connection. Make room for decrypted data in c->recv
      io = &c->recv;
      if (io->size - io->len < pkt->pay.len &&
          !mg_iobuf_resize(io, io->len + pkt->pay.len)) {
        mg_error(c, "oom");
      } else {
        // Decrypt data directly into c->recv
        long n = mg_tls_recv(c, &io->buf[io->len], io->size - io->len);
        if (n == MG_IO_ERR) {
          mg_error(c, "TLS recv error");
        } else if (n > 0) {
          // Decrypted successfully - trigger MG_EV_READ
          io->len += (size_t) n;
          mg_call(c, MG_EV_READ, &n);
        }
      }
    } else {
      // Plain text connection, data is already in c->recv, trigger
      // MG_EV_READ
      mg_call(c, MG_EV_READ, &pkt->pay.len);
    }
  }
}

static void rx_tcp(struct mg_tcpip_if *ifp, struct pkt *pkt) {
  struct mg_connection *c = getpeer(ifp->mgr, pkt, false);
  struct connstate *s = c == NULL ? NULL : (struct connstate *) (c + 1);
#if 0
  MG_INFO(("%lu %hhu %d", c ? c->id : 0, pkt->tcp->flags, (int) pkt->pay.len));
#endif
  if (c != NULL && c->is_connecting && pkt->tcp->flags == (TH_SYN | TH_ACK)) {
    s->seq = mg_ntohl(pkt->tcp->ack), s->ack = mg_ntohl(pkt->tcp->seq) + 1;
    tx_tcp_pkt(ifp, pkt, TH_ACK, pkt->tcp->ack, NULL, 0);
    c->is_connecting = 0;  // Client connected
    settmout(c, MIP_TTYPE_KEEPALIVE);
    mg_call(c, MG_EV_CONNECT, NULL);  // Let user know
  } else if (c != NULL && c->is_connecting && pkt->tcp->flags != TH_ACK) {
    // mg_hexdump(pkt->raw.ptr, pkt->raw.len);
    tx_tcp_pkt(ifp, pkt, TH_RST | TH_ACK, pkt->tcp->ack, NULL, 0);
  } else if (c != NULL && pkt->tcp->flags & TH_RST) {
    mg_error(c, "peer RST");  // RFC-1122 4.2.2.13
  } else if (c != NULL) {
#if 0
    MG_DEBUG(("%lu %d %M:%hu -> %M:%hu", c->id, (int) pkt->raw.len,
              mg_print_ip4, &pkt->ip->src, mg_ntohs(pkt->tcp->sport),
              mg_print_ip4, &pkt->ip->dst, mg_ntohs(pkt->tcp->dport)));
    mg_hexdump(pkt->pay.ptr, pkt->pay.len);
#endif
    s->tmiss = 0;                         // Reset missed keep-alive counter
    if (s->ttype == MIP_TTYPE_KEEPALIVE)  // Advance keep-alive timer
      settmout(c,
               MIP_TTYPE_KEEPALIVE);  // unless a former ACK timeout is pending
    read_conn(c, pkt);  // Override timer with ACK timeout if needed
  } else if ((c = getpeer(ifp->mgr, pkt, true)) == NULL) {
    tx_tcp_pkt(ifp, pkt, TH_RST | TH_ACK, pkt->tcp->ack, NULL, 0);
  } else if (pkt->tcp->flags & TH_RST) {
    if (c->is_accepted) mg_error(c, "peer RST");  // RFC-1122 4.2.2.13
    // ignore RST if not connected
  } else if (pkt->tcp->flags & TH_SYN) {
    // Use peer's source port as ISN, in order to recognise the handshake
    uint32_t isn = mg_htonl((uint32_t) mg_ntohs(pkt->tcp->sport));
    tx_tcp_pkt(ifp, pkt, TH_SYN | TH_ACK, isn, NULL, 0);
  } else if (pkt->tcp->flags & TH_FIN) {
    tx_tcp_pkt(ifp, pkt, TH_FIN | TH_ACK, pkt->tcp->ack, NULL, 0);
  } else if (mg_htonl(pkt->tcp->ack) == mg_htons(pkt->tcp->sport) + 1U) {
    accept_conn(c, pkt);
  } else if (!c->is_accepted) {  // no peer
    tx_tcp_pkt(ifp, pkt, TH_RST | TH_ACK, pkt->tcp->ack, NULL, 0);
  } else {
    // MG_VERBOSE(("dropped silently.."));
  }
}

static void rx_ip(struct mg_tcpip_if *ifp, struct pkt *pkt) {
  if (pkt->ip->frag & IP_MORE_FRAGS_MSK ||
        pkt->ip->frag & IP_FRAG_OFFSET_MSK) {
    if (pkt->ip->proto == 17) pkt->udp = (struct udp *) (pkt->ip + 1);
    if (pkt->ip->proto == 6) pkt->tcp = (struct tcp *) (pkt->ip + 1);
    struct mg_connection *c = getpeer(ifp->mgr, pkt, false);
    if (c)  mg_error(c, "Received fragmented packet");
  } else if (pkt->ip->proto == 1) {
    pkt->icmp = (struct icmp *) (pkt->ip + 1);
    if (pkt->pay.len < sizeof(*pkt->icmp)) return;
    mkpay(pkt, pkt->icmp + 1);
    rx_icmp(ifp, pkt);
  } else if (pkt->ip->proto == 17) {
    pkt->udp = (struct udp *) (pkt->ip + 1);
    if (pkt->pay.len < sizeof(*pkt->udp)) return;
    mkpay(pkt, pkt->udp + 1);
    MG_VERBOSE(("UDP %M:%hu -> %M:%hu len %u", mg_print_ip4, &pkt->ip->src,
                mg_ntohs(pkt->udp->sport), mg_print_ip4, &pkt->ip->dst,
                mg_ntohs(pkt->udp->dport), (int) pkt->pay.len));
    if (ifp->enable_dhcp_client && pkt->udp->dport == mg_htons(68)) {
      pkt->dhcp = (struct dhcp *) (pkt->udp + 1);
      mkpay(pkt, pkt->dhcp + 1);
      rx_dhcp_client(ifp, pkt);
    } else if (ifp->enable_dhcp_server && pkt->udp->dport == mg_htons(67)) {
      pkt->dhcp = (struct dhcp *) (pkt->udp + 1);
      mkpay(pkt, pkt->dhcp + 1);
      rx_dhcp_server(ifp, pkt);
    } else {
      rx_udp(ifp, pkt);
    }
  } else if (pkt->ip->proto == 6) {
    pkt->tcp = (struct tcp *) (pkt->ip + 1);
    if (pkt->pay.len < sizeof(*pkt->tcp)) return;
    mkpay(pkt, pkt->tcp + 1);
    uint16_t iplen = mg_ntohs(pkt->ip->len);
    uint16_t off = (uint16_t) (sizeof(*pkt->ip) + ((pkt->tcp->off >> 4) * 4U));
    if (iplen >= off) pkt->pay.len = (size_t) (iplen - off);
    MG_VERBOSE(("TCP %M:%hu -> %M:%hu len %u", mg_print_ip4, &pkt->ip->src,
                mg_ntohs(pkt->tcp->sport), mg_print_ip4, &pkt->ip->dst,
                mg_ntohs(pkt->tcp->dport), (int) pkt->pay.len));
    rx_tcp(ifp, pkt);
  }
}

static void rx_ip6(struct mg_tcpip_if *ifp, struct pkt *pkt) {
  // MG_DEBUG(("IP %d", (int) len));
  if (pkt->ip6->proto == 1 || pkt->ip6->proto == 58) {
    pkt->icmp = (struct icmp *) (pkt->ip6 + 1);
    if (pkt->pay.len < sizeof(*pkt->icmp)) return;
    mkpay(pkt, pkt->icmp + 1);
    rx_icmp(ifp, pkt);
  } else if (pkt->ip6->proto == 17) {
    pkt->udp = (struct udp *) (pkt->ip6 + 1);
    if (pkt->pay.len < sizeof(*pkt->udp)) return;
    // MG_DEBUG(("  UDP %u %u -> %u", len, mg_htons(udp->sport),
    // mg_htons(udp->dport)));
    mkpay(pkt, pkt->udp + 1);
  }
}

static void mg_tcpip_rx(struct mg_tcpip_if *ifp, void *buf, size_t len) {
  struct pkt pkt;
  memset(&pkt, 0, sizeof(pkt));
  pkt.raw.ptr = (char *) buf;
  pkt.raw.len = len;
  pkt.eth = (struct eth *) buf;
  // mg_hexdump(buf, len > 16 ? 16: len);
  if (pkt.raw.len < sizeof(*pkt.eth)) return;  // Truncated - runt?
  if (ifp->enable_mac_check &&
      memcmp(pkt.eth->dst, ifp->mac, sizeof(pkt.eth->dst)) != 0 &&
      memcmp(pkt.eth->dst, broadcast, sizeof(pkt.eth->dst)) != 0)
    return;
  if (ifp->enable_crc32_check && len > 4) {
    len -= 4;  // TODO(scaprile): check on bigendian
    uint32_t crc = mg_crc32(0, (const char *) buf, len);
    if (memcmp((void *) ((size_t) buf + len), &crc, sizeof(crc))) return;
  }
  if (pkt.eth->type == mg_htons(0x806)) {
    pkt.arp = (struct arp *) (pkt.eth + 1);
    if (sizeof(*pkt.eth) + sizeof(*pkt.arp) > pkt.raw.len) return;  // Truncated
    rx_arp(ifp, &pkt);
  } else if (pkt.eth->type == mg_htons(0x86dd)) {
    pkt.ip6 = (struct ip6 *) (pkt.eth + 1);
    if (pkt.raw.len < sizeof(*pkt.eth) + sizeof(*pkt.ip6)) return;  // Truncated
    if ((pkt.ip6->ver >> 4) != 0x6) return;                         // Not IP
    mkpay(&pkt, pkt.ip6 + 1);
    rx_ip6(ifp, &pkt);
  } else if (pkt.eth->type == mg_htons(0x800)) {
    pkt.ip = (struct ip *) (pkt.eth + 1);
    if (pkt.raw.len < sizeof(*pkt.eth) + sizeof(*pkt.ip)) return;  // Truncated
    // Truncate frame to what IP header tells us
    if ((size_t) mg_ntohs(pkt.ip->len) + sizeof(struct eth) < pkt.raw.len) {
      pkt.raw.len = (size_t) mg_ntohs(pkt.ip->len) + sizeof(struct eth);
    }
    if (pkt.raw.len < sizeof(*pkt.eth) + sizeof(*pkt.ip)) return;  // Truncated
    if ((pkt.ip->ver >> 4) != 4) return;                           // Not IP
    mkpay(&pkt, pkt.ip + 1);
    rx_ip(ifp, &pkt);
  } else {
    MG_DEBUG(("Unknown eth type %x", mg_htons(pkt.eth->type)));
    mg_hexdump(buf, len >= 32 ? 32 : len);
  }
}

static void mg_tcpip_poll(struct mg_tcpip_if *ifp, uint64_t uptime_ms) {
  if (ifp == NULL || ifp->driver == NULL) return;
  bool expired_1000ms = mg_timer_expired(&ifp->timer_1000ms, 1000, uptime_ms);
  ifp->now = uptime_ms;

  // Handle physical interface up/down status
  if (expired_1000ms && ifp->driver->up) {
    bool up = ifp->driver->up(ifp);
    bool current = ifp->state != MG_TCPIP_STATE_DOWN;
    if (up != current) {
      ifp->state = up == false               ? MG_TCPIP_STATE_DOWN
                   : ifp->enable_dhcp_client ? MG_TCPIP_STATE_UP
                                             : MG_TCPIP_STATE_READY;
      if (!up && ifp->enable_dhcp_client) ifp->ip = 0;
      onstatechange(ifp);
    }
  }
  if (ifp->state == MG_TCPIP_STATE_DOWN) return;

  // DHCP RFC-2131 (4.4)
  if (ifp->state == MG_TCPIP_STATE_UP && expired_1000ms) {
    tx_dhcp_discover(ifp);  // INIT (4.4.1)
  } else if (expired_1000ms && ifp->state == MG_TCPIP_STATE_READY &&
             ifp->lease_expire > 0) {  // BOUND / RENEWING / REBINDING
    if (ifp->now >= ifp->lease_expire) {
      ifp->state = MG_TCPIP_STATE_UP, ifp->ip = 0;  // expired, release IP
      onstatechange(ifp);
    } else if (ifp->now + 30UL * 60UL * 1000UL > ifp->lease_expire &&
               ((ifp->now / 1000) % 60) == 0) {
      // hack: 30 min before deadline, try to rebind (4.3.6) every min
      tx_dhcp_request_re(ifp, (uint8_t *) broadcast, ifp->ip, 0xffffffff);
    }  // TODO(): Handle T1 (RENEWING) and T2 (REBINDING) (4.4.5)
  }

  // Read data from the network
  if (ifp->driver->rx != NULL) {  // Polling driver. We must call it
    size_t len =
        ifp->driver->rx(ifp->recv_queue.buf, ifp->recv_queue.size, ifp);
    if (len > 0) {
      ifp->nrecv++;
      mg_tcpip_rx(ifp, ifp->recv_queue.buf, len);
    }
  } else {  // Interrupt-based driver. Fills recv queue itself
    char *buf;
    size_t len = mg_queue_next(&ifp->recv_queue, &buf);
    if (len > 0) {
      mg_tcpip_rx(ifp, buf, len);
      mg_queue_del(&ifp->recv_queue, len);
    }
  }

  // Process timeouts
  for (struct mg_connection *c = ifp->mgr->conns; c != NULL; c = c->next) {
    if (c->is_udp || c->is_listening || c->is_resolving) continue;
    struct connstate *s = (struct connstate *) (c + 1);
    uint32_t rem_ip;
    memcpy(&rem_ip, c->rem.ip, sizeof(uint32_t));
    if (uptime_ms > s->timer) {
      if (s->ttype == MIP_TTYPE_ACK) {
        MG_VERBOSE(("%lu ack %x %x", c->id, s->seq, s->ack));
        tx_tcp(ifp, s->mac, rem_ip, TH_ACK, c->loc.port, c->rem.port,
               mg_htonl(s->seq), mg_htonl(s->ack), "", 0);
      } else if (s->ttype == MIP_TTYPE_ARP) {
        mg_error(c, "ARP timeout");
      } else if (s->ttype == MIP_TTYPE_SYN) {
        mg_error(c, "Connection timeout");
      } else if (s->ttype == MIP_TTYPE_FIN) {
        c->is_closing = 1;
        continue;
      } else {
        if (s->tmiss++ > 2) {
          mg_error(c, "keepalive");
        } else {
          MG_VERBOSE(("%lu keepalive", c->id));
          tx_tcp(ifp, s->mac, rem_ip, TH_ACK, c->loc.port, c->rem.port,
                 mg_htonl(s->seq - 1), mg_htonl(s->ack), "", 0);
        }
      }

      settmout(c, MIP_TTYPE_KEEPALIVE);
    }
  }
}

// This function executes in interrupt context, thus it should copy data
// somewhere fast. Note that newlib's malloc is not thread safe, thus use
// our lock-free queue with preallocated buffer to copy data and return asap
void mg_tcpip_qwrite(void *buf, size_t len, struct mg_tcpip_if *ifp) {
  char *p;
  if (mg_queue_book(&ifp->recv_queue, &p, len) >= len) {
    memcpy(p, buf, len);
    mg_queue_add(&ifp->recv_queue, len);
    ifp->nrecv++;
  } else {
    ifp->ndrop++;
  }
}

void mg_tcpip_init(struct mg_mgr *mgr, struct mg_tcpip_if *ifp) {
  // If MAC address is not set, make a random one
  if (ifp->mac[0] == 0 && ifp->mac[1] == 0 && ifp->mac[2] == 0 &&
      ifp->mac[3] == 0 && ifp->mac[4] == 0 && ifp->mac[5] == 0) {
    ifp->mac[0] = 0x02;  // Locally administered, unicast
    mg_random(&ifp->mac[1], sizeof(ifp->mac) - 1);
    MG_INFO(("MAC not set. Generated random: %M", mg_print_mac, ifp->mac));
  }

  if (ifp->driver->init && !ifp->driver->init(ifp)) {
    MG_ERROR(("driver init failed"));
  } else {
    size_t framesize = 1540;
    ifp->tx.ptr = (char *) calloc(1, framesize), ifp->tx.len = framesize;
    if (ifp->recv_queue.size == 0)
      ifp->recv_queue.size = ifp->driver->rx ? framesize : 8192;
    ifp->recv_queue.buf = (char *) calloc(1, ifp->recv_queue.size);
    ifp->timer_1000ms = mg_millis();
    mgr->priv = ifp;
    ifp->mgr = mgr;
    ifp->mtu = MG_TCPIP_MTU_DEFAULT;
    mgr->extraconnsize = sizeof(struct connstate);
    if (ifp->ip == 0) ifp->enable_dhcp_client = true;
    memset(ifp->gwmac, 255, sizeof(ifp->gwmac));  // Set to broadcast
    mg_random(&ifp->eport, sizeof(ifp->eport));   // Random from 0 to 65535
    ifp->eport |= MG_EPHEMERAL_PORT_BASE;         // Random from
                                           // MG_EPHEMERAL_PORT_BASE to 65535
    if (ifp->tx.ptr == NULL || ifp->recv_queue.buf == NULL) MG_ERROR(("OOM"));
  }
}

void mg_tcpip_free(struct mg_tcpip_if *ifp) {
  free(ifp->recv_queue.buf);
  free((char *) ifp->tx.ptr);
}

static void send_syn(struct mg_connection *c) {
  struct connstate *s = (struct connstate *) (c + 1);
  uint32_t isn = mg_htonl((uint32_t) mg_ntohs(c->loc.port));
  struct mg_tcpip_if *ifp = (struct mg_tcpip_if *) c->mgr->priv;
  uint32_t rem_ip;
  memcpy(&rem_ip, c->rem.ip, sizeof(uint32_t));
  tx_tcp(ifp, s->mac, rem_ip, TH_SYN, c->loc.port, c->rem.port, isn, 0, NULL,
         0);
}

void mg_connect_resolved(struct mg_connection *c) {
  struct mg_tcpip_if *ifp = (struct mg_tcpip_if *) c->mgr->priv;
  uint32_t rem_ip;
  memcpy(&rem_ip, c->rem.ip, sizeof(uint32_t));
  c->is_resolving = 0;
  if (ifp->eport < MG_EPHEMERAL_PORT_BASE) ifp->eport = MG_EPHEMERAL_PORT_BASE;
  memcpy(c->loc.ip, &ifp->ip, sizeof(uint32_t));
  c->loc.port = mg_htons(ifp->eport++);
  MG_DEBUG(("%lu %M -> %M", c->id, mg_print_ip_port, &c->loc, mg_print_ip_port,
            &c->rem));
  mg_call(c, MG_EV_RESOLVE, NULL);
  if (c->is_udp && (rem_ip == 0xffffffff || rem_ip == (ifp->ip | ~ifp->mask))) {
    struct connstate *s = (struct connstate *) (c + 1);
    memset(s->mac, 0xFF, sizeof(s->mac));  // global or local broadcast
  } else if (((rem_ip & ifp->mask) == (ifp->ip & ifp->mask))) {
    // If we're in the same LAN, fire an ARP lookup.
    MG_DEBUG(("%lu ARP lookup...", c->id));
    arp_ask(ifp, rem_ip);
    settmout(c, MIP_TTYPE_ARP);
    c->is_arplooking = 1;
    c->is_connecting = 1;
  } else if ((*((uint8_t *) &rem_ip) & 0xE0) == 0xE0) {
    struct connstate *s = (struct connstate *) (c + 1);  // 224 to 239, E0 to EF
    uint8_t mcastp[3] = {0x01, 0x00, 0x5E};              // multicast group
    memcpy(s->mac, mcastp, 3);
    memcpy(s->mac + 3, ((uint8_t *) &rem_ip) + 1, 3);  // 23 LSb
    s->mac[3] &= 0x7F;
  } else {
    struct connstate *s = (struct connstate *) (c + 1);
    memcpy(s->mac, ifp->gwmac, sizeof(ifp->gwmac));
    if (c->is_udp) {
      mg_call(c, MG_EV_CONNECT, NULL);
    } else {
      send_syn(c);
      settmout(c, MIP_TTYPE_SYN);
      c->is_connecting = 1;
    }
  }
}

bool mg_open_listener(struct mg_connection *c, const char *url) {
  c->loc.port = mg_htons(mg_url_port(url));
  return true;
}

static void write_conn(struct mg_connection *c) {
  long len = c->is_tls ? mg_tls_send(c, c->send.buf, c->send.len)
                       : mg_io_send(c, c->send.buf, c->send.len);
  if (len > 0) {
    mg_iobuf_del(&c->send, 0, (size_t) len);
    mg_call(c, MG_EV_WRITE, &len);
  }
}

static void init_closure(struct mg_connection *c) {
  struct connstate *s = (struct connstate *) (c + 1);
  if (c->is_udp == false && c->is_listening == false &&
      c->is_connecting == false) {  // For TCP conns,
    struct mg_tcpip_if *ifp =
        (struct mg_tcpip_if *) c->mgr->priv;  // send TCP FIN
    uint32_t rem_ip;
    memcpy(&rem_ip, c->rem.ip, sizeof(uint32_t));
    tx_tcp(ifp, s->mac, rem_ip, TH_FIN | TH_ACK, c->loc.port, c->rem.port,
           mg_htonl(s->seq), mg_htonl(s->ack), NULL, 0);
    settmout(c, MIP_TTYPE_FIN);
  }
}

static void close_conn(struct mg_connection *c) {
  struct connstate *s = (struct connstate *) (c + 1);
  mg_iobuf_free(&s->raw);  // For TLS connections, release raw data
  mg_close_conn(c);
}

static bool can_write(struct mg_connection *c) {
  return c->is_connecting == 0 && c->is_resolving == 0 && c->send.len > 0 &&
         c->is_tls_hs == 0 && c->is_arplooking == 0;
}

void mg_mgr_poll(struct mg_mgr *mgr, int ms) {
  struct mg_connection *c, *tmp;
  uint64_t now = mg_millis();
  mg_tcpip_poll((struct mg_tcpip_if *) mgr->priv, now);
  mg_timer_poll(&mgr->timers, now);
  for (c = mgr->conns; c != NULL; c = tmp) {
    tmp = c->next;
    struct connstate *s = (struct connstate *) (c + 1);
    mg_call(c, MG_EV_POLL, &now);
    MG_VERBOSE(("%lu .. %c%c%c%c%c", c->id, c->is_tls ? 'T' : 't',
                c->is_connecting ? 'C' : 'c', c->is_tls_hs ? 'H' : 'h',
                c->is_resolving ? 'R' : 'r', c->is_closing ? 'C' : 'c'));
    if (c->is_tls_hs) mg_tls_handshake(c);
    if (can_write(c)) write_conn(c);
    if (c->is_draining && c->send.len == 0 && s->ttype != MIP_TTYPE_FIN)
      init_closure(c);
    if (c->is_closing) close_conn(c);
  }
  (void) ms;
}

bool mg_send(struct mg_connection *c, const void *buf, size_t len) {
  struct mg_tcpip_if *ifp = (struct mg_tcpip_if *) c->mgr->priv;
  bool res = false;
  uint32_t rem_ip;
  memcpy(&rem_ip, c->rem.ip, sizeof(uint32_t));
  if (ifp->ip == 0 || ifp->state != MG_TCPIP_STATE_READY) {
    mg_error(c, "net down");
  } else if (c->is_udp) {
    struct connstate *s = (struct connstate *) (c + 1);
    len = trim_len(c, len);   // Trimming length if necessary
    tx_udp(ifp, s->mac, ifp->ip, c->loc.port, rem_ip, c->rem.port, buf, len);
    res = true;
  } else {
    res = mg_iobuf_add(&c->send, c->send.len, buf, len);
  }
  return res;
}
#endif  // MG_ENABLE_TCPIP

#ifdef MG_ENABLE_LINES
#line 1 "src/ota_dummy.c"
#endif



#if MG_OTA == MG_OTA_NONE
bool mg_ota_begin(size_t new_firmware_size) {
  (void) new_firmware_size;
  return true;
}
bool mg_ota_write(const void *buf, size_t len) {
  (void) buf, (void) len;
  return true;
}
bool mg_ota_end(void) {
  return true;
}
bool mg_ota_commit(void) {
  return true;
}
bool mg_ota_rollback(void) {
  return true;
}
int mg_ota_status(int fw) {
  (void) fw;
  return 0;
}
uint32_t mg_ota_crc32(int fw) {
  (void) fw;
  return 0;
}
uint32_t mg_ota_timestamp(int fw) {
  (void) fw;
  return 0;
}
size_t mg_ota_size(int fw) {
  (void) fw;
  return 0;
}
#endif

#ifdef MG_ENABLE_LINES
#line 1 "src/ota_flash.c"
#endif





// This OTA implementation uses the internal flash API outlined in device.h
// It splits flash into 2 equal partitions, and stores OTA status in the
// last sector of the partition.

#if MG_OTA == MG_OTA_FLASH

#define MG_OTADATA_KEY 0xb07afed0

static char *s_addr;      // Current address to write to
static size_t s_size;     // Firmware size to flash. In-progress indicator
static uint32_t s_crc32;  // Firmware checksum

struct mg_otadata {
  uint32_t crc32, size, timestamp, status;
};

bool mg_ota_begin(size_t new_firmware_size) {
  bool ok = false;
  if (s_size) {
    MG_ERROR(("OTA already in progress. Call mg_ota_end()"));
  } else {
    size_t half = mg_flash_size() / 2, max = half - mg_flash_sector_size();
    s_crc32 = 0;
    s_addr = (char *) mg_flash_start() + half;
    MG_DEBUG(("Firmware %lu bytes, max %lu", s_size, max));
    if (new_firmware_size < max) {
      ok = true;
      s_size = new_firmware_size;
      MG_INFO(("Starting OTA, firmware size %lu", s_size));
    } else {
      MG_ERROR(("Firmware %lu is too big to fit %lu", new_firmware_size, max));
    }
  }
  return ok;
}

bool mg_ota_write(const void *buf, size_t len) {
  bool ok = false;
  if (s_size == 0) {
    MG_ERROR(("OTA is not started, call mg_ota_begin()"));
  } else {
    size_t align = mg_flash_write_align();
    size_t len_aligned_down = MG_ROUND_DOWN(len, align);
    if (len_aligned_down) ok = mg_flash_write(s_addr, buf, len_aligned_down);
    if (len_aligned_down < len) {
      size_t left = len - len_aligned_down;
      char tmp[align];
      memset(tmp, 0xff, sizeof(tmp));
      memcpy(tmp, (char *) buf + len_aligned_down, left);
      ok = mg_flash_write(s_addr + len_aligned_down, tmp, sizeof(tmp));
    }
    s_crc32 = mg_crc32(s_crc32, (char *) buf, len);  // Update CRC
    MG_DEBUG(("%#x %p %lu -> %d", s_addr - len, buf, len, ok));
    s_addr += len;
  }
  return ok;
}

bool mg_ota_end(void) {
  char *base = (char *) mg_flash_start() + mg_flash_size() / 2;
  bool ok = false;
  if (s_size) {
    size_t size = s_addr - base;
    uint32_t crc32 = mg_crc32(0, base, s_size);
    if (size == s_size && crc32 == s_crc32) {
      uint32_t now = (uint32_t) (mg_now() / 1000);
      struct mg_otadata od = {crc32, size, now, MG_OTA_FIRST_BOOT};
      uint32_t key = MG_OTADATA_KEY + (mg_flash_bank() == 2 ? 1 : 2);
      ok = mg_flash_save(NULL, key, &od, sizeof(od));
    }
    MG_DEBUG(("CRC: %x/%x, size: %lu/%lu, status: %s", s_crc32, crc32, s_size,
              size, ok ? "ok" : "fail"));
    s_size = 0;
    if (ok) ok = mg_flash_swap_bank();
  }
  MG_INFO(("Finishing OTA: %s", ok ? "ok" : "fail"));
  return ok;
}

static struct mg_otadata mg_otadata(int fw) {
  struct mg_otadata od = {};
  int bank = mg_flash_bank();
  uint32_t key = MG_OTADATA_KEY + 1;
  if ((fw == MG_FIRMWARE_CURRENT && bank == 2)) key++;
  if ((fw == MG_FIRMWARE_PREVIOUS && bank == 1)) key++;
  mg_flash_load(NULL, key, &od, sizeof(od));
  // MG_DEBUG(("Loaded OTA data. fw %d, bank %d, key %p", fw, bank, key));
  // mg_hexdump(&od, sizeof(od));
  return od;
}

int mg_ota_status(int fw) {
  struct mg_otadata od = mg_otadata(fw);
  return od.status;
}
uint32_t mg_ota_crc32(int fw) {
  struct mg_otadata od = mg_otadata(fw);
  return od.crc32;
}
uint32_t mg_ota_timestamp(int fw) {
  struct mg_otadata od = mg_otadata(fw);
  return od.timestamp;
}
size_t mg_ota_size(int fw) {
  struct mg_otadata od = mg_otadata(fw);
  return od.size;
}

bool mg_ota_commit(void) {
  struct mg_otadata od = mg_otadata(MG_FIRMWARE_CURRENT);
  od.status = MG_OTA_COMMITTED;
  uint32_t key = MG_OTADATA_KEY + mg_flash_bank();
  return mg_flash_save(NULL, key, &od, sizeof(od));
}

bool mg_ota_rollback(void) {
  MG_DEBUG(("Rolling firmware back"));
  return mg_flash_swap_bank();
}
#endif

#ifdef MG_ENABLE_LINES
#line 1 "src/printf.c"
#endif




size_t mg_queue_vprintf(struct mg_queue *q, const char *fmt, va_list *ap) {
  size_t len = mg_snprintf(NULL, 0, fmt, ap);
  char *buf;
  if (len == 0 || mg_queue_book(q, &buf, len + 1) < len + 1) {
    len = 0;  // Nah. Not enough space
  } else {
    len = mg_vsnprintf((char *) buf, len + 1, fmt, ap);
    mg_queue_add(q, len);
  }
  return len;
}

size_t mg_queue_printf(struct mg_queue *q, const char *fmt, ...) {
  va_list ap;
  size_t len;
  va_start(ap, fmt);
  len = mg_queue_vprintf(q, fmt, &ap);
  va_end(ap);
  return len;
}

static void mg_pfn_iobuf_private(char ch, void *param, bool expand) {
  struct mg_iobuf *io = (struct mg_iobuf *) param;
  if (expand && io->len + 2 > io->size) mg_iobuf_resize(io, io->len + 2);
  if (io->len + 2 <= io->size) {
    io->buf[io->len++] = (uint8_t) ch;
    io->buf[io->len] = 0;
  } else if (io->len < io->size) {
    io->buf[io->len++] = 0;  // Guarantee to 0-terminate
  }
}

static void mg_putchar_iobuf_static(char ch, void *param) {
  mg_pfn_iobuf_private(ch, param, false);
}

void mg_pfn_iobuf(char ch, void *param) {
  mg_pfn_iobuf_private(ch, param, true);
}

size_t mg_vsnprintf(char *buf, size_t len, const char *fmt, va_list *ap) {
  struct mg_iobuf io = {(uint8_t *) buf, len, 0, 0};
  size_t n = mg_vxprintf(mg_putchar_iobuf_static, &io, fmt, ap);
  if (n < len) buf[n] = '\0';
  return n;
}

size_t mg_snprintf(char *buf, size_t len, const char *fmt, ...) {
  va_list ap;
  size_t n;
  va_start(ap, fmt);
  n = mg_vsnprintf(buf, len, fmt, &ap);
  va_end(ap);
  return n;
}

char *mg_vmprintf(const char *fmt, va_list *ap) {
  struct mg_iobuf io = {0, 0, 0, 256};
  mg_vxprintf(mg_pfn_iobuf, &io, fmt, ap);
  return (char *) io.buf;
}

char *mg_mprintf(const char *fmt, ...) {
  char *s;
  va_list ap;
  va_start(ap, fmt);
  s = mg_vmprintf(fmt, &ap);
  va_end(ap);
  return s;
}

void mg_pfn_stdout(char c, void *param) {
  putchar(c);
  (void) param;
}

static size_t print_ip4(void (*out)(char, void *), void *arg, uint8_t *p) {
  return mg_xprintf(out, arg, "%d.%d.%d.%d", p[0], p[1], p[2], p[3]);
}

static size_t print_ip6(void (*out)(char, void *), void *arg, uint16_t *p) {
  return mg_xprintf(out, arg, "[%x:%x:%x:%x:%x:%x:%x:%x]", mg_ntohs(p[0]),
                    mg_ntohs(p[1]), mg_ntohs(p[2]), mg_ntohs(p[3]),
                    mg_ntohs(p[4]), mg_ntohs(p[5]), mg_ntohs(p[6]),
                    mg_ntohs(p[7]));
}

size_t mg_print_ip4(void (*out)(char, void *), void *arg, va_list *ap) {
  uint8_t *p = va_arg(*ap, uint8_t *);
  return print_ip4(out, arg, p);
}

size_t mg_print_ip6(void (*out)(char, void *), void *arg, va_list *ap) {
  uint16_t *p = va_arg(*ap, uint16_t *);
  return print_ip6(out, arg, p);
}

size_t mg_print_ip(void (*out)(char, void *), void *arg, va_list *ap) {
  struct mg_addr *addr = va_arg(*ap, struct mg_addr *);
  if (addr->is_ip6) return print_ip6(out, arg, (uint16_t *) addr->ip);
  return print_ip4(out, arg, (uint8_t *) &addr->ip);
}

size_t mg_print_ip_port(void (*out)(char, void *), void *arg, va_list *ap) {
  struct mg_addr *a = va_arg(*ap, struct mg_addr *);
  return mg_xprintf(out, arg, "%M:%hu", mg_print_ip, a, mg_ntohs(a->port));
}

size_t mg_print_mac(void (*out)(char, void *), void *arg, va_list *ap) {
  uint8_t *p = va_arg(*ap, uint8_t *);
  return mg_xprintf(out, arg, "%02x:%02x:%02x:%02x:%02x:%02x", p[0], p[1], p[2],
                    p[3], p[4], p[5]);
}

static char mg_esc(int c, bool esc) {
  const char *p, *esc1 = "\b\f\n\r\t\\\"", *esc2 = "bfnrt\\\"";
  for (p = esc ? esc1 : esc2; *p != '\0'; p++) {
    if (*p == c) return esc ? esc2[p - esc1] : esc1[p - esc2];
  }
  return 0;
}

static char mg_escape(int c) {
  return mg_esc(c, true);
}

static size_t qcpy(void (*out)(char, void *), void *ptr, char *buf,
                   size_t len) {
  size_t i = 0, extra = 0;
  for (i = 0; i < len && buf[i] != '\0'; i++) {
    char c = mg_escape(buf[i]);
    if (c) {
      out('\\', ptr), out(c, ptr), extra++;
    } else {
      out(buf[i], ptr);
    }
  }
  return i + extra;
}

static size_t bcpy(void (*out)(char, void *), void *arg, uint8_t *buf,
                   size_t len) {
  size_t i, j, n = 0;
  const char *t =
      "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
  for (i = 0; i < len; i += 3) {
    uint8_t c1 = buf[i], c2 = i + 1 < len ? buf[i + 1] : 0,
            c3 = i + 2 < len ? buf[i + 2] : 0;
    char tmp[4] = {t[c1 >> 2], t[(c1 & 3) << 4 | (c2 >> 4)], '=', '='};
    if (i + 1 < len) tmp[2] = t[(c2 & 15) << 2 | (c3 >> 6)];
    if (i + 2 < len) tmp[3] = t[c3 & 63];
    for (j = 0; j < sizeof(tmp) && tmp[j] != '\0'; j++) out(tmp[j], arg);
    n += j;
  }
  return n;
}

size_t mg_print_hex(void (*out)(char, void *), void *arg, va_list *ap) {
  size_t bl = (size_t) va_arg(*ap, int);
  uint8_t *p = va_arg(*ap, uint8_t *);
  const char *hex = "0123456789abcdef";
  size_t j;
  for (j = 0; j < bl; j++) {
    out(hex[(p[j] >> 4) & 0x0F], arg);
    out(hex[p[j] & 0x0F], arg);
  }
  return 2 * bl;
}
size_t mg_print_base64(void (*out)(char, void *), void *arg, va_list *ap) {
  size_t len = (size_t) va_arg(*ap, int);
  uint8_t *buf = va_arg(*ap, uint8_t *);
  return bcpy(out, arg, buf, len);
}

size_t mg_print_esc(void (*out)(char, void *), void *arg, va_list *ap) {
  size_t len = (size_t) va_arg(*ap, int);
  char *p = va_arg(*ap, char *);
  if (len == 0) len = p == NULL ? 0 : strlen(p);
  return qcpy(out, arg, p, len);
}

#ifdef MG_ENABLE_LINES
#line 1 "src/queue.c"
#endif



#if defined(__GNUC__) || defined(__clang__)
#define MG_MEMORY_BARRIER() __sync_synchronize()
#elif defined(_MSC_VER) && _MSC_VER >= 1700
#define MG_MEMORY_BARRIER() MemoryBarrier()
#elif !defined(MG_MEMORY_BARRIER)
#define MG_MEMORY_BARRIER()
#endif

// Every message in a queue is prepended by a 32-bit message length (ML).
// If ML is 0, then it is the end, and reader must wrap to the beginning.
//
//  Queue when q->tail <= q->head:
//  |----- free -----| ML | message1 | ML | message2 |  ----- free ------|
//  ^                ^                               ^                   ^
// buf              tail                            head                len
//
//  Queue when q->tail > q->head:
//  | ML | message2 |----- free ------| ML | message1 | 0 |---- free ----|
//  ^               ^                 ^                                  ^
// buf             head              tail                               len

void mg_queue_init(struct mg_queue *q, char *buf, size_t size) {
  q->size = size;
  q->buf = buf;
  q->head = q->tail = 0;
}

static size_t mg_queue_read_len(struct mg_queue *q) {
  uint32_t n = 0;
  MG_MEMORY_BARRIER();
  memcpy(&n, q->buf + q->tail, sizeof(n));
  assert(q->tail + n + sizeof(n) <= q->size);
  return n;
}

static void mg_queue_write_len(struct mg_queue *q, size_t len) {
  uint32_t n = (uint32_t) len;
  memcpy(q->buf + q->head, &n, sizeof(n));
  MG_MEMORY_BARRIER();
}

size_t mg_queue_book(struct mg_queue *q, char **buf, size_t len) {
  size_t space = 0, hs = sizeof(uint32_t) * 2;  // *2 is for the 0 marker
  if (q->head >= q->tail && q->head + len + hs <= q->size) {
    space = q->size - q->head - hs;  // There is enough space
  } else if (q->head >= q->tail && q->tail > hs) {
    mg_queue_write_len(q, 0);  // Not enough space ahead
    q->head = 0;               // Wrap head to the beginning
  }
  if (q->head + hs + len < q->tail) space = q->tail - q->head - hs;
  if (buf != NULL) *buf = q->buf + q->head + sizeof(uint32_t);
  return space;
}

size_t mg_queue_next(struct mg_queue *q, char **buf) {
  size_t len = 0;
  if (q->tail != q->head) {
    len = mg_queue_read_len(q);
    if (len == 0) {  // Zero (head wrapped) ?
      q->tail = 0;   // Reset tail to the start
      if (q->head > q->tail) len = mg_queue_read_len(q);  // Read again
    }
  }
  if (buf != NULL) *buf = q->buf + q->tail + sizeof(uint32_t);
  assert(q->tail + len <= q->size);
  return len;
}

void mg_queue_add(struct mg_queue *q, size_t len) {
  assert(len > 0);
  mg_queue_write_len(q, len);
  assert(q->head + sizeof(uint32_t) * 2 + len <= q->size);
  q->head += len + sizeof(uint32_t);
}

void mg_queue_del(struct mg_queue *q, size_t len) {
  q->tail += len + sizeof(uint32_t);
  assert(q->tail + sizeof(uint32_t) <= q->size);
}

#ifdef MG_ENABLE_LINES
#line 1 "src/rpc.c"
#endif



void mg_rpc_add(struct mg_rpc **head, struct mg_str method,
                void (*fn)(struct mg_rpc_req *), void *fn_data) {
  struct mg_rpc *rpc = (struct mg_rpc *) calloc(1, sizeof(*rpc));
  if (rpc != NULL) {
    rpc->method = mg_strdup(method), rpc->fn = fn, rpc->fn_data = fn_data;
    rpc->next = *head, *head = rpc;
  }
}

void mg_rpc_del(struct mg_rpc **head, void (*fn)(struct mg_rpc_req *)) {
  struct mg_rpc *r;
  while ((r = *head) != NULL) {
    if (r->fn == fn || fn == NULL) {
      *head = r->next;
      free((void *) r->method.ptr);
      free(r);
    } else {
      head = &(*head)->next;
    }
  }
}

static void mg_rpc_call(struct mg_rpc_req *r, struct mg_str method) {
  struct mg_rpc *h = r->head == NULL ? NULL : *r->head;
  while (h != NULL && !mg_match(method, h->method, NULL)) h = h->next;
  if (h != NULL) {
    r->rpc = h;
    h->fn(r);
  } else {
    mg_rpc_err(r, -32601, "\"%.*s not found\"", (int) method.len, method.ptr);
  }
}

void mg_rpc_process(struct mg_rpc_req *r) {
  int len, off = mg_json_get(r->frame, "$.method", &len);
  if (off > 0 && r->frame.ptr[off] == '"') {
    struct mg_str method = mg_str_n(&r->frame.ptr[off + 1], (size_t) len - 2);
    mg_rpc_call(r, method);
  } else if ((off = mg_json_get(r->frame, "$.result", &len)) > 0 ||
             (off = mg_json_get(r->frame, "$.error", &len)) > 0) {
    mg_rpc_call(r, mg_str(""));  // JSON response! call "" method handler
  } else {
    mg_rpc_err(r, -32700, "%m", mg_print_esc, (int) r->frame.len,
               r->frame.ptr);  // Invalid
  }
}

void mg_rpc_vok(struct mg_rpc_req *r, const char *fmt, va_list *ap) {
  int len, off = mg_json_get(r->frame, "$.id", &len);
  if (off > 0) {
    mg_xprintf(r->pfn, r->pfn_data, "{%m:%.*s,%m:", mg_print_esc, 0, "id", len,
               &r->frame.ptr[off], mg_print_esc, 0, "result");
    mg_vxprintf(r->pfn, r->pfn_data, fmt == NULL ? "null" : fmt, ap);
    mg_xprintf(r->pfn, r->pfn_data, "}");
  }
}

void mg_rpc_ok(struct mg_rpc_req *r, const char *fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  mg_rpc_vok(r, fmt, &ap);
  va_end(ap);
}

void mg_rpc_verr(struct mg_rpc_req *r, int code, const char *fmt, va_list *ap) {
  int len, off = mg_json_get(r->frame, "$.id", &len);
  mg_xprintf(r->pfn, r->pfn_data, "{");
  if (off > 0) {
    mg_xprintf(r->pfn, r->pfn_data, "%m:%.*s,", mg_print_esc, 0, "id", len,
               &r->frame.ptr[off]);
  }
  mg_xprintf(r->pfn, r->pfn_data, "%m:{%m:%d,%m:", mg_print_esc, 0, "error",
             mg_print_esc, 0, "code", code, mg_print_esc, 0, "message");
  mg_vxprintf(r->pfn, r->pfn_data, fmt == NULL ? "null" : fmt, ap);
  mg_xprintf(r->pfn, r->pfn_data, "}}");
}

void mg_rpc_err(struct mg_rpc_req *r, int code, const char *fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  mg_rpc_verr(r, code, fmt, &ap);
  va_end(ap);
}

static size_t print_methods(mg_pfn_t pfn, void *pfn_data, va_list *ap) {
  struct mg_rpc *h, **head = (struct mg_rpc **) va_arg(*ap, void **);
  size_t len = 0;
  for (h = *head; h != NULL; h = h->next) {
    if (h->method.len == 0) continue;  // Ignore response handler
    len += mg_xprintf(pfn, pfn_data, "%s%m", h == *head ? "" : ",",
                      mg_print_esc, (int) h->method.len, h->method.ptr);
  }
  return len;
}

void mg_rpc_list(struct mg_rpc_req *r) {
  mg_rpc_ok(r, "[%M]", print_methods, r->head);
}

#ifdef MG_ENABLE_LINES
#line 1 "src/sha1.c"
#endif
/* Copyright(c) By Steve Reid <steve@edmweb.com> */
/* 100% Public Domain */



union char64long16 {
  unsigned char c[64];
  uint32_t l[16];
};

#define rol(value, bits) (((value) << (bits)) | ((value) >> (32 - (bits))))

static uint32_t blk0(union char64long16 *block, int i) {
  if (MG_BIG_ENDIAN) {
  } else {
    block->l[i] = (rol(block->l[i], 24) & 0xFF00FF00) |
                  (rol(block->l[i], 8) & 0x00FF00FF);
  }
  return block->l[i];
}

/* Avoid redefine warning (ARM /usr/include/sys/ucontext.h define R0~R4) */
#undef blk
#undef R0
#undef R1
#undef R2
#undef R3
#undef R4

#define blk(i)                                                               \
  (block->l[i & 15] = rol(block->l[(i + 13) & 15] ^ block->l[(i + 8) & 15] ^ \
                              block->l[(i + 2) & 15] ^ block->l[i & 15],     \
                          1))
#define R0(v, w, x, y, z, i)                                          \
  z += ((w & (x ^ y)) ^ y) + blk0(block, i) + 0x5A827999 + rol(v, 5); \
  w = rol(w, 30);
#define R1(v, w, x, y, z, i)                                  \
  z += ((w & (x ^ y)) ^ y) + blk(i) + 0x5A827999 + rol(v, 5); \
  w = rol(w, 30);
#define R2(v, w, x, y, z, i)                          \
  z += (w ^ x ^ y) + blk(i) + 0x6ED9EBA1 + rol(v, 5); \
  w = rol(w, 30);
#define R3(v, w, x, y, z, i)                                        \
  z += (((w | x) & y) | (w & x)) + blk(i) + 0x8F1BBCDC + rol(v, 5); \
  w = rol(w, 30);
#define R4(v, w, x, y, z, i)                          \
  z += (w ^ x ^ y) + blk(i) + 0xCA62C1D6 + rol(v, 5); \
  w = rol(w, 30);

static void mg_sha1_transform(uint32_t state[5],
                              const unsigned char *buffer) {
  uint32_t a, b, c, d, e;
  union char64long16 block[1];

  memcpy(block, buffer, 64);
  a = state[0];
  b = state[1];
  c = state[2];
  d = state[3];
  e = state[4];
  R0(a, b, c, d, e, 0);
  R0(e, a, b, c, d, 1);
  R0(d, e, a, b, c, 2);
  R0(c, d, e, a, b, 3);
  R0(b, c, d, e, a, 4);
  R0(a, b, c, d, e, 5);
  R0(e, a, b, c, d, 6);
  R0(d, e, a, b, c, 7);
  R0(c, d, e, a, b, 8);
  R0(b, c, d, e, a, 9);
  R0(a, b, c, d, e, 10);
  R0(e, a, b, c, d, 11);
  R0(d, e, a, b, c, 12);
  R0(c, d, e, a, b, 13);
  R0(b, c, d, e, a, 14);
  R0(a, b, c, d, e, 15);
  R1(e, a, b, c, d, 16);
  R1(d, e, a, b, c, 17);
  R1(c, d, e, a, b, 18);
  R1(b, c, d, e, a, 19);
  R2(a, b, c, d, e, 20);
  R2(e, a, b, c, d, 21);
  R2(d, e, a, b, c, 22);
  R2(c, d, e, a, b, 23);
  R2(b, c, d, e, a, 24);
  R2(a, b, c, d, e, 25);
  R2(e, a, b, c, d, 26);
  R2(d, e, a, b, c, 27);
  R2(c, d, e, a, b, 28);
  R2(b, c, d, e, a, 29);
  R2(a, b, c, d, e, 30);
  R2(e, a, b, c, d, 31);
  R2(d, e, a, b, c, 32);
  R2(c, d, e, a, b, 33);
  R2(b, c, d, e, a, 34);
  R2(a, b, c, d, e, 35);
  R2(e, a, b, c, d, 36);
  R2(d, e, a, b, c, 37);
  R2(c, d, e, a, b, 38);
  R2(b, c, d, e, a, 39);
  R3(a, b, c, d, e, 40);
  R3(e, a, b, c, d, 41);
  R3(d, e, a, b, c, 42);
  R3(c, d, e, a, b, 43);
  R3(b, c, d, e, a, 44);
  R3(a, b, c, d, e, 45);
  R3(e, a, b, c, d, 46);
  R3(d, e, a, b, c, 47);
  R3(c, d, e, a, b, 48);
  R3(b, c, d, e, a, 49);
  R3(a, b, c, d, e, 50);
  R3(e, a, b, c, d, 51);
  R3(d, e, a, b, c, 52);
  R3(c, d, e, a, b, 53);
  R3(b, c, d, e, a, 54);
  R3(a, b, c, d, e, 55);
  R3(e, a, b, c, d, 56);
  R3(d, e, a, b, c, 57);
  R3(c, d, e, a, b, 58);
  R3(b, c, d, e, a, 59);
  R4(a, b, c, d, e, 60);
  R4(e, a, b, c, d, 61);
  R4(d, e, a, b, c, 62);
  R4(c, d, e, a, b, 63);
  R4(b, c, d, e, a, 64);
  R4(a, b, c, d, e, 65);
  R4(e, a, b, c, d, 66);
  R4(d, e, a, b, c, 67);
  R4(c, d, e, a, b, 68);
  R4(b, c, d, e, a, 69);
  R4(a, b, c, d, e, 70);
  R4(e, a, b, c, d, 71);
  R4(d, e, a, b, c, 72);
  R4(c, d, e, a, b, 73);
  R4(b, c, d, e, a, 74);
  R4(a, b, c, d, e, 75);
  R4(e, a, b, c, d, 76);
  R4(d, e, a, b, c, 77);
  R4(c, d, e, a, b, 78);
  R4(b, c, d, e, a, 79);
  state[0] += a;
  state[1] += b;
  state[2] += c;
  state[3] += d;
  state[4] += e;
  /* Erase working structures. The order of operations is important,
   * used to ensure that compiler doesn't optimize those out. */
  memset(block, 0, sizeof(block));
  a = b = c = d = e = 0;
  (void) a;
  (void) b;
  (void) c;
  (void) d;
  (void) e;
}

void mg_sha1_init(mg_sha1_ctx *context) {
  context->state[0] = 0x67452301;
  context->state[1] = 0xEFCDAB89;
  context->state[2] = 0x98BADCFE;
  context->state[3] = 0x10325476;
  context->state[4] = 0xC3D2E1F0;
  context->count[0] = context->count[1] = 0;
}

void mg_sha1_update(mg_sha1_ctx *context, const unsigned char *data,
                    size_t len) {
  size_t i, j;

  j = context->count[0];
  if ((context->count[0] += (uint32_t) len << 3) < j) context->count[1]++;
  context->count[1] += (uint32_t) (len >> 29);
  j = (j >> 3) & 63;
  if ((j + len) > 63) {
    memcpy(&context->buffer[j], data, (i = 64 - j));
    mg_sha1_transform(context->state, context->buffer);
    for (; i + 63 < len; i += 64) {
      mg_sha1_transform(context->state, &data[i]);
    }
    j = 0;
  } else
    i = 0;
  memcpy(&context->buffer[j], &data[i], len - i);
}

void mg_sha1_final(unsigned char digest[20], mg_sha1_ctx *context) {
  unsigned i;
  unsigned char finalcount[8], c;

  for (i = 0; i < 8; i++) {
    finalcount[i] = (unsigned char) ((context->count[(i >= 4 ? 0 : 1)] >>
                                      ((3 - (i & 3)) * 8)) &
                                     255);
  }
  c = 0200;
  mg_sha1_update(context, &c, 1);
  while ((context->count[0] & 504) != 448) {
    c = 0000;
    mg_sha1_update(context, &c, 1);
  }
  mg_sha1_update(context, finalcount, 8);
  for (i = 0; i < 20; i++) {
    digest[i] =
        (unsigned char) ((context->state[i >> 2] >> ((3 - (i & 3)) * 8)) & 255);
  }
  memset(context, '\0', sizeof(*context));
  memset(&finalcount, '\0', sizeof(finalcount));
}

#ifdef MG_ENABLE_LINES
#line 1 "src/sntp.c"
#endif






#define SNTP_TIME_OFFSET 2208988800U  // (1970 - 1900) in seconds
#define SNTP_MAX_FRAC 4294967295.0    // 2 ** 32 - 1

static int64_t gettimestamp(const uint32_t *data) {
  uint32_t sec = mg_ntohl(data[0]), frac = mg_ntohl(data[1]);
  if (sec) sec -= SNTP_TIME_OFFSET;
  return ((int64_t) sec) * 1000 + (int64_t) (frac / SNTP_MAX_FRAC * 1000.0);
}

int64_t mg_sntp_parse(const unsigned char *buf, size_t len) {
  int64_t res = -1;
  int mode = len > 0 ? buf[0] & 7 : 0;
  int version = len > 0 ? (buf[0] >> 3) & 7 : 0;
  if (len < 48) {
    MG_ERROR(("%s", "corrupt packet"));
  } else if (mode != 4 && mode != 5) {
    MG_ERROR(("%s", "not a server reply"));
  } else if (buf[1] == 0) {
    MG_ERROR(("%s", "server sent a kiss of death"));
  } else if (version == 4 || version == 3) {
    // int64_t ref = gettimestamp((uint32_t *) &buf[16]);
    int64_t t0 = gettimestamp((uint32_t *) &buf[24]);
    int64_t t1 = gettimestamp((uint32_t *) &buf[32]);
    int64_t t2 = gettimestamp((uint32_t *) &buf[40]);
    int64_t t3 = (int64_t) mg_millis();
    int64_t delta = (t3 - t0) - (t2 - t1);
    MG_VERBOSE(("%lld %lld %lld %lld delta:%lld", t0, t1, t2, t3, delta));
    res = t2 + delta / 2;
  } else {
    MG_ERROR(("unexpected version: %d", version));
  }
  return res;
}

static void sntp_cb(struct mg_connection *c, int ev, void *evd, void *fnd) {
  if (ev == MG_EV_READ) {
    int64_t milliseconds = mg_sntp_parse(c->recv.buf, c->recv.len);
    if (milliseconds > 0) {
      MG_INFO(("%lu got time: %lld ms from epoch", c->id, milliseconds));
      mg_call(c, MG_EV_SNTP_TIME, (uint64_t *) &milliseconds);
      MG_VERBOSE(("%u.%u", (unsigned) (milliseconds / 1000),
                  (unsigned) (milliseconds % 1000)));
    }
    mg_iobuf_del(&c->recv, 0, c->recv.len);  // Free receive buffer
  } else if (ev == MG_EV_CONNECT) {
    mg_sntp_request(c);
  } else if (ev == MG_EV_CLOSE) {
  }
  (void) fnd;
  (void) evd;
}

void mg_sntp_request(struct mg_connection *c) {
  if (c->is_resolving) {
    MG_ERROR(("%lu wait until resolved", c->id));
  } else {
    int64_t now = (int64_t) mg_millis();  // Use int64_t, for vc98
    uint8_t buf[48] = {0};
    uint32_t *t = (uint32_t *) &buf[40];
    double frac = ((double) (now % 1000)) / 1000.0 * SNTP_MAX_FRAC;
    buf[0] = (0 << 6) | (4 << 3) | 3;
    t[0] = mg_htonl((uint32_t) (now / 1000) + SNTP_TIME_OFFSET);
    t[1] = mg_htonl((uint32_t) frac);
    mg_send(c, buf, sizeof(buf));
  }
}

struct mg_connection *mg_sntp_connect(struct mg_mgr *mgr, const char *url,
                                      mg_event_handler_t fn, void *fnd) {
  struct mg_connection *c = NULL;
  if (url == NULL) url = "udp://time.google.com:123";
  if ((c = mg_connect(mgr, url, fn, fnd)) != NULL) c->pfn = sntp_cb;
  return c;
}

#ifdef MG_ENABLE_LINES
#line 1 "src/sock.c"
#endif











#if MG_ENABLE_SOCKET

#ifndef closesocket
#define closesocket(x) close(x)
#endif

#define FD(c_) ((MG_SOCKET_TYPE) (size_t) (c_)->fd)
#define S2PTR(s_) ((void *) (size_t) (s_))

#ifndef MSG_NONBLOCKING
#define MSG_NONBLOCKING 0
#endif

#ifndef AF_INET6
#define AF_INET6 10
#endif

#ifndef MG_SOCK_ERR
#define MG_SOCK_ERR(errcode) ((errcode) < 0 ? errno : 0)
#endif

#ifndef MG_SOCK_INTR
#define MG_SOCK_INTR(fd) (fd == MG_INVALID_SOCKET && MG_SOCK_ERR(-1) == EINTR)
#endif

#ifndef MG_SOCK_PENDING
#define MG_SOCK_PENDING(errcode) \
  (((errcode) < 0) && (errno == EINPROGRESS || errno == EWOULDBLOCK))
#endif

#ifndef MG_SOCK_RESET
#define MG_SOCK_RESET(errcode) \
  (((errcode) < 0) && (errno == EPIPE || errno == ECONNRESET))
#endif

union usa {
  struct sockaddr sa;
  struct sockaddr_in sin;
#if MG_ENABLE_IPV6
  struct sockaddr_in6 sin6;
#endif
};

static socklen_t tousa(struct mg_addr *a, union usa *usa) {
  socklen_t len = sizeof(usa->sin);
  memset(usa, 0, sizeof(*usa));
  usa->sin.sin_family = AF_INET;
  usa->sin.sin_port = a->port;
  memcpy(&usa->sin.sin_addr, a->ip, sizeof(uint32_t));
#if MG_ENABLE_IPV6
  if (a->is_ip6) {
    usa->sin.sin_family = AF_INET6;
    usa->sin6.sin6_port = a->port;
    usa->sin6.sin6_scope_id = a->scope_id;
    memcpy(&usa->sin6.sin6_addr, a->ip, sizeof(a->ip));
    len = sizeof(usa->sin6);
  }
#endif
  return len;
}

static void tomgaddr(union usa *usa, struct mg_addr *a, bool is_ip6) {
  a->is_ip6 = is_ip6;
  a->port = usa->sin.sin_port;
  memcpy(&a->ip, &usa->sin.sin_addr, sizeof(uint32_t));
#if MG_ENABLE_IPV6
  if (is_ip6) {
    memcpy(a->ip, &usa->sin6.sin6_addr, sizeof(a->ip));
    a->port = usa->sin6.sin6_port;
    a->scope_id = (uint8_t) usa->sin6.sin6_scope_id;
  }
#endif
}

static void setlocaddr(MG_SOCKET_TYPE fd, struct mg_addr *addr) {
  union usa usa;
  socklen_t n = sizeof(usa);
  if (getsockname(fd, &usa.sa, &n) == 0) {
    tomgaddr(&usa, addr, n != sizeof(usa.sin));
  }
}

static void iolog(struct mg_connection *c, char *buf, long n, bool r) {
  if (n == MG_IO_WAIT) {
    // Do nothing
  } else if (n <= 0) {
    c->is_closing = 1;  // Termination. Don't call mg_error(): #1529
  } else if (n > 0) {
    if (c->is_hexdumping) {
      union usa usa;
      socklen_t slen = sizeof(usa.sin);
      if (getsockname(FD(c), &usa.sa, &slen) < 0) (void) 0;  // Ignore result
      MG_INFO(("\n-- %lu %M %s %M %ld", c->id, mg_print_ip_port, &c->loc,
               r ? "<-" : "->", mg_print_ip_port, &c->rem, n));

      mg_hexdump(buf, (size_t) n);
    }
    if (r) {
      c->recv.len += (size_t) n;
      mg_call(c, MG_EV_READ, &n);
    } else {
      mg_iobuf_del(&c->send, 0, (size_t) n);
      // if (c->send.len == 0) mg_iobuf_resize(&c->send, 0);
      if (c->send.len == 0) {
        MG_EPOLL_MOD(c, 0);
      }
      mg_call(c, MG_EV_WRITE, &n);
    }
  }
}

long mg_io_send(struct mg_connection *c, const void *buf, size_t len) {
  long n;
  if (c->is_udp) {
    union usa usa;
    socklen_t slen = tousa(&c->rem, &usa);
    n = sendto(FD(c), (char *) buf, len, 0, &usa.sa, slen);
    if (n > 0) setlocaddr(FD(c), &c->loc);
  } else {
    n = send(FD(c), (char *) buf, len, MSG_NONBLOCKING);
  }
  if (MG_SOCK_PENDING(n)) return MG_IO_WAIT;
  if (MG_SOCK_RESET(n)) return MG_IO_RESET;
  if (n <= 0) return MG_IO_ERR;
  return n;
}

bool mg_send(struct mg_connection *c, const void *buf, size_t len) {
  if (c->is_udp) {
    long n = mg_io_send(c, buf, len);
    MG_DEBUG(("%lu %ld %d:%d %ld err %d", c->id, c->fd, (int) c->send.len,
              (int) c->recv.len, n, MG_SOCK_ERR(n)));
    iolog(c, (char *) buf, n, false);
    return n > 0;
  } else {
    return mg_iobuf_add(&c->send, c->send.len, buf, len);
  }
}

static void mg_set_non_blocking_mode(MG_SOCKET_TYPE fd) {
#if defined(MG_CUSTOM_NONBLOCK)
  MG_CUSTOM_NONBLOCK(fd);
#elif MG_ARCH == MG_ARCH_WIN32 && MG_ENABLE_WINSOCK
  unsigned long on = 1;
  ioctlsocket(fd, FIONBIO, &on);
#elif MG_ENABLE_RL
  unsigned long on = 1;
  ioctlsocket(fd, FIONBIO, &on);
#elif MG_ENABLE_FREERTOS_TCP
  const BaseType_t off = 0;
  if (setsockopt(fd, 0, FREERTOS_SO_RCVTIMEO, &off, sizeof(off)) != 0) (void) 0;
  if (setsockopt(fd, 0, FREERTOS_SO_SNDTIMEO, &off, sizeof(off)) != 0) (void) 0;
#elif MG_ENABLE_LWIP
  lwip_fcntl(fd, F_SETFL, O_NONBLOCK);
#elif MG_ARCH == MG_ARCH_AZURERTOS
  fcntl(fd, F_SETFL, O_NONBLOCK);
#elif MG_ARCH == MG_ARCH_TIRTOS
  int val = 0;
  setsockopt(fd, SOL_SOCKET, SO_BLOCKING, &val, sizeof(val));
  // SPRU524J section 3.3.3 page 63, SO_SNDLOWAT
  int sz = sizeof(val);
  getsockopt(fd, SOL_SOCKET, SO_SNDBUF, &val, &sz);
  val /= 2;  // set send low-water mark at half send buffer size
  setsockopt(fd, SOL_SOCKET, SO_SNDLOWAT, &val, sizeof(val));
#else
  fcntl(fd, F_SETFL, fcntl(fd, F_GETFL, 0) | O_NONBLOCK);  // Non-blocking mode
  fcntl(fd, F_SETFD, FD_CLOEXEC);                          // Set close-on-exec
#endif
}

bool mg_open_listener(struct mg_connection *c, const char *url) {
  MG_SOCKET_TYPE fd = MG_INVALID_SOCKET;
  bool success = false;
  c->loc.port = mg_htons(mg_url_port(url));
  if (!mg_aton(mg_url_host(url), &c->loc)) {
    MG_ERROR(("invalid listening URL: %s", url));
  } else {
    union usa usa;
    socklen_t slen = tousa(&c->loc, &usa);
    int rc, on = 1, af = c->loc.is_ip6 ? AF_INET6 : AF_INET;
    int type = strncmp(url, "udp:", 4) == 0 ? SOCK_DGRAM : SOCK_STREAM;
    int proto = type == SOCK_DGRAM ? IPPROTO_UDP : IPPROTO_TCP;
    (void) on;

    if ((fd = socket(af, type, proto)) == MG_INVALID_SOCKET) {
      MG_ERROR(("socket: %d", MG_SOCK_ERR(-1)));
#if defined(SO_EXCLUSIVEADDRUSE)
    } else if ((rc = setsockopt(fd, SOL_SOCKET, SO_EXCLUSIVEADDRUSE,
                                (char *) &on, sizeof(on))) != 0) {
      // "Using SO_REUSEADDR and SO_EXCLUSIVEADDRUSE"
      MG_ERROR(("setsockopt(SO_EXCLUSIVEADDRUSE): %d %d", on, MG_SOCK_ERR(rc)));
#elif defined(SO_REUSEADDR) && (!defined(LWIP_SOCKET) || SO_REUSE)
    } else if ((rc = setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, (char *) &on,
                                sizeof(on))) != 0) {
      // 1. SO_REUSEADDR semantics on UNIX and Windows is different.  On
      // Windows, SO_REUSEADDR allows to bind a socket to a port without error
      // even if the port is already open by another program. This is not the
      // behavior SO_REUSEADDR was designed for, and leads to hard-to-track
      // failure scenarios.
      //
      // 2. For LWIP, SO_REUSEADDR should be explicitly enabled by defining
      // SO_REUSE = 1 in lwipopts.h, otherwise the code below will compile but
      // won't work! (setsockopt will return EINVAL)
      MG_ERROR(("setsockopt(SO_REUSEADDR): %d", MG_SOCK_ERR(rc)));
#endif
#if MG_IPV6_V6ONLY
      // Bind only to the V6 address, not V4 address on this port
    } else if (c->loc.is_ip6 &&
               (rc = setsockopt(fd, IPPROTO_IPV6, IPV6_V6ONLY, (char *) &on,
                                sizeof(on))) != 0) {
      // See #2089. Allow to bind v4 and v6 sockets on the same port
      MG_ERROR(("setsockopt(IPV6_V6ONLY): %d", MG_SOCK_ERR(rc)));
#endif
    } else if ((rc = bind(fd, &usa.sa, slen)) != 0) {
      MG_ERROR(("bind: %d", MG_SOCK_ERR(rc)));
    } else if ((type == SOCK_STREAM &&
                (rc = listen(fd, MG_SOCK_LISTEN_BACKLOG_SIZE)) != 0)) {
      // NOTE(lsm): FreeRTOS uses backlog value as a connection limit
      // In case port was set to 0, get the real port number
      MG_ERROR(("listen: %d", MG_SOCK_ERR(rc)));
    } else {
      setlocaddr(fd, &c->loc);
      mg_set_non_blocking_mode(fd);
      c->fd = S2PTR(fd);
      MG_EPOLL_ADD(c);
      success = true;
    }
  }
  if (success == false && fd != MG_INVALID_SOCKET) closesocket(fd);
  return success;
}

long mg_io_recv(struct mg_connection *c, void *buf, size_t len) {
  long n = 0;
  if (c->is_udp) {
    union usa usa;
    socklen_t slen = tousa(&c->rem, &usa);
    n = recvfrom(FD(c), (char *) buf, len, 0, &usa.sa, &slen);
    if (n > 0) tomgaddr(&usa, &c->rem, slen != sizeof(usa.sin));
  } else {
    n = recv(FD(c), (char *) buf, len, MSG_NONBLOCKING);
  }
  if (MG_SOCK_PENDING(n)) return MG_IO_WAIT;
  if (MG_SOCK_RESET(n)) return MG_IO_RESET;
  if (n <= 0) return MG_IO_ERR;
  return n;
}

// NOTE(lsm): do only one iteration of reads, cause some systems
// (e.g. FreeRTOS stack) return 0 instead of -1/EWOULDBLOCK when no data
static void read_conn(struct mg_connection *c) {
  long n = -1;
  if (c->recv.len >= MG_MAX_RECV_SIZE) {
    mg_error(c, "max_recv_buf_size reached");
  } else if (c->recv.size <= c->recv.len &&
             !mg_iobuf_resize(&c->recv, c->recv.size + MG_IO_SIZE)) {
    mg_error(c, "oom");
  } else {
    char *buf = (char *) &c->recv.buf[c->recv.len];
    size_t len = c->recv.size - c->recv.len;
    n = c->is_tls ? mg_tls_recv(c, buf, len) : mg_io_recv(c, buf, len);
    MG_DEBUG(("%lu %ld snd %ld/%ld rcv %ld/%ld n=%ld err=%d", c->id, c->fd,
              (long) c->send.len, (long) c->send.size, (long) c->recv.len,
              (long) c->recv.size, n, MG_SOCK_ERR(n)));
    iolog(c, buf, n, true);
  }
}

static void write_conn(struct mg_connection *c) {
  char *buf = (char *) c->send.buf;
  size_t len = c->send.len;
  long n = c->is_tls ? mg_tls_send(c, buf, len) : mg_io_send(c, buf, len);
  MG_DEBUG(("%lu %ld snd %ld/%ld rcv %ld/%ld n=%ld err=%d", c->id, c->fd,
            (long) c->send.len, (long) c->send.size, (long) c->recv.len,
            (long) c->recv.size, n, MG_SOCK_ERR(n)));
  iolog(c, buf, n, false);
}

static void close_conn(struct mg_connection *c) {
  if (FD(c) != MG_INVALID_SOCKET) {
#if MG_ENABLE_EPOLL
    epoll_ctl(c->mgr->epoll_fd, EPOLL_CTL_DEL, FD(c), NULL);
#endif
    closesocket(FD(c));
#if MG_ENABLE_FREERTOS_TCP
    FreeRTOS_FD_CLR(c->fd, c->mgr->ss, eSELECT_ALL);
#endif
  }
  mg_close_conn(c);
}

static void connect_conn(struct mg_connection *c) {
  union usa usa;
  socklen_t n = sizeof(usa);
  // Use getpeername() to test whether we have connected
  if (getpeername(FD(c), &usa.sa, &n) == 0) {
    c->is_connecting = 0;
    mg_call(c, MG_EV_CONNECT, NULL);
    MG_EPOLL_MOD(c, 0);
    if (c->is_tls_hs) mg_tls_handshake(c);
  } else {
    mg_error(c, "socket error");
  }
}

static void setsockopts(struct mg_connection *c) {
#if MG_ENABLE_FREERTOS_TCP || MG_ARCH == MG_ARCH_AZURERTOS || \
    MG_ARCH == MG_ARCH_TIRTOS
  (void) c;
#else
  int on = 1;
#if !defined(SOL_TCP)
#define SOL_TCP IPPROTO_TCP
#endif
  if (setsockopt(FD(c), SOL_TCP, TCP_NODELAY, (char *) &on, sizeof(on)) != 0)
    (void) 0;
  if (setsockopt(FD(c), SOL_SOCKET, SO_KEEPALIVE, (char *) &on, sizeof(on)) !=
      0)
    (void) 0;
#endif
}

void mg_connect_resolved(struct mg_connection *c) {
  int type = c->is_udp ? SOCK_DGRAM : SOCK_STREAM;
  int rc, af = c->rem.is_ip6 ? AF_INET6 : AF_INET;  // c->rem has resolved IP
  c->fd = S2PTR(socket(af, type, 0));               // Create outbound socket
  c->is_resolving = 0;                              // Clear resolving flag
  if (FD(c) == MG_INVALID_SOCKET) {
    mg_error(c, "socket(): %d", MG_SOCK_ERR(-1));
  } else if (c->is_udp) {
    MG_EPOLL_ADD(c);
#if MG_ARCH == MG_ARCH_TIRTOS
    union usa usa;  // TI-RTOS NDK requires binding to receive on UDP sockets
    socklen_t slen = tousa(&c->loc, &usa);
    if ((rc = bind(c->fd, &usa.sa, slen)) != 0)
      MG_ERROR(("bind: %d", MG_SOCK_ERR(rc)));
#endif
    mg_call(c, MG_EV_RESOLVE, NULL);
    mg_call(c, MG_EV_CONNECT, NULL);
  } else {
    union usa usa;
    socklen_t slen = tousa(&c->rem, &usa);
    mg_set_non_blocking_mode(FD(c));
    setsockopts(c);
    MG_EPOLL_ADD(c);
    mg_call(c, MG_EV_RESOLVE, NULL);
    rc = connect(FD(c), &usa.sa, slen);  // Attempt to connect
    if (rc == 0) {                       // Success
      mg_call(c, MG_EV_CONNECT, NULL);   // Send MG_EV_CONNECT to the user
    } else if (MG_SOCK_PENDING(rc)) {    // Need to wait for TCP handshake
      MG_DEBUG(("%lu %ld -> %M pend", c->id, c->fd, mg_print_ip_port, &c->rem));
      c->is_connecting = 1;
    } else {
      mg_error(c, "connect: %d", MG_SOCK_ERR(rc));
    }
  }
}

static MG_SOCKET_TYPE raccept(MG_SOCKET_TYPE sock, union usa *usa,
                              socklen_t *len) {
  MG_SOCKET_TYPE fd = MG_INVALID_SOCKET;
  do {
    memset(usa, 0, sizeof(*usa));
    fd = accept(sock, &usa->sa, len);
  } while (MG_SOCK_INTR(fd));
  return fd;
}

static void accept_conn(struct mg_mgr *mgr, struct mg_connection *lsn) {
  struct mg_connection *c = NULL;
  union usa usa;
  socklen_t sa_len = sizeof(usa);
  MG_SOCKET_TYPE fd = raccept(FD(lsn), &usa, &sa_len);
  if (fd == MG_INVALID_SOCKET) {
#if MG_ARCH == MG_ARCH_AZURERTOS || defined(__ECOS)
    // AzureRTOS, in non-block socket mode can mark listening socket readable
    // even it is not. See comment for 'select' func implementation in
    // nx_bsd.c That's not an error, just should try later
    if (errno != EAGAIN)
#endif
      MG_ERROR(("%lu accept failed, errno %d", lsn->id, MG_SOCK_ERR(-1)));
#if (MG_ARCH != MG_ARCH_WIN32) && !MG_ENABLE_FREERTOS_TCP && \
    (MG_ARCH != MG_ARCH_TIRTOS) && !MG_ENABLE_POLL && !MG_ENABLE_EPOLL
  } else if ((long) fd >= FD_SETSIZE) {
    MG_ERROR(("%ld > %ld", (long) fd, (long) FD_SETSIZE));
    closesocket(fd);
#endif
  } else if ((c = mg_alloc_conn(mgr)) == NULL) {
    MG_ERROR(("%lu OOM", lsn->id));
    closesocket(fd);
  } else {
    tomgaddr(&usa, &c->rem, sa_len != sizeof(usa.sin));
    LIST_ADD_HEAD(struct mg_connection, &mgr->conns, c);
    c->fd = S2PTR(fd);
    MG_EPOLL_ADD(c);
    mg_set_non_blocking_mode(FD(c));
    setsockopts(c);
    c->is_accepted = 1;
    c->is_hexdumping = lsn->is_hexdumping;
    c->loc = lsn->loc;
    c->pfn = lsn->pfn;
    c->pfn_data = lsn->pfn_data;
    c->fn = lsn->fn;
    c->fn_data = lsn->fn_data;
    MG_DEBUG(("%lu %ld accepted %M -> %M", c->id, c->fd, mg_print_ip_port,
              &c->rem, mg_print_ip_port, &c->loc));
    mg_call(c, MG_EV_OPEN, NULL);
    mg_call(c, MG_EV_ACCEPT, NULL);
  }
}

static bool can_read(const struct mg_connection *c) {
  return c->is_full == false;
}

static bool can_write(const struct mg_connection *c) {
  return c->is_connecting || (c->send.len > 0 && c->is_tls_hs == 0);
}

static bool skip_iotest(const struct mg_connection *c) {
  return (c->is_closing || c->is_resolving || FD(c) == MG_INVALID_SOCKET) ||
         (can_read(c) == false && can_write(c) == false);
}

static void mg_iotest(struct mg_mgr *mgr, int ms) {
#if MG_ENABLE_FREERTOS_TCP
  struct mg_connection *c;
  for (c = mgr->conns; c != NULL; c = c->next) {
    c->is_readable = c->is_writable = 0;
    if (skip_iotest(c)) continue;
    if (can_read(c))
      FreeRTOS_FD_SET(c->fd, mgr->ss, eSELECT_READ | eSELECT_EXCEPT);
    if (can_write(c)) FreeRTOS_FD_SET(c->fd, mgr->ss, eSELECT_WRITE);
  }
  FreeRTOS_select(mgr->ss, pdMS_TO_TICKS(ms));
  for (c = mgr->conns; c != NULL; c = c->next) {
    EventBits_t bits = FreeRTOS_FD_ISSET(c->fd, mgr->ss);
    c->is_readable = bits & (eSELECT_READ | eSELECT_EXCEPT) ? 1U : 0;
    c->is_writable = bits & eSELECT_WRITE ? 1U : 0;
    if (c->fd != MG_INVALID_SOCKET)
      FreeRTOS_FD_CLR(c->fd, mgr->ss,
                      eSELECT_READ | eSELECT_EXCEPT | eSELECT_WRITE);
  }
#elif MG_ENABLE_EPOLL
  size_t max = 1;
  for (struct mg_connection *c = mgr->conns; c != NULL; c = c->next) {
    c->is_readable = c->is_writable = 0;
    if (mg_tls_pending(c) > 0) ms = 1, c->is_readable = 1;
    if (can_write(c)) MG_EPOLL_MOD(c, 1);
    max++;
  }
  struct epoll_event *evs = (struct epoll_event *) alloca(max * sizeof(evs[0]));
  int n = epoll_wait(mgr->epoll_fd, evs, (int) max, ms);
  for (int i = 0; i < n; i++) {
    struct mg_connection *c = (struct mg_connection *) evs[i].data.ptr;
    if (evs[i].events & EPOLLERR) {
      mg_error(c, "socket error");
    } else if (c->is_readable == 0) {
      bool rd = evs[i].events & (EPOLLIN | EPOLLHUP);
      bool wr = evs[i].events & EPOLLOUT;
      c->is_readable = can_read(c) && rd ? 1U : 0;
      c->is_writable = can_write(c) && wr ? 1U : 0;
    }
  }
  (void) skip_iotest;
#elif MG_ENABLE_POLL
  nfds_t n = 0;
  for (struct mg_connection *c = mgr->conns; c != NULL; c = c->next) n++;
  struct pollfd *fds = (struct pollfd *) alloca(n * sizeof(fds[0]));
  memset(fds, 0, n * sizeof(fds[0]));
  n = 0;
  for (struct mg_connection *c = mgr->conns; c != NULL; c = c->next) {
    c->is_readable = c->is_writable = 0;
    if (skip_iotest(c)) {
      // Socket not valid, ignore
    } else if (mg_tls_pending(c) > 0) {
      ms = 1;  // Don't wait if TLS is ready
    } else {
      fds[n].fd = FD(c);
      if (can_read(c)) fds[n].events |= POLLIN;
      if (can_write(c)) fds[n].events |= POLLOUT;
      n++;
    }
  }

  // MG_INFO(("poll n=%d ms=%d", (int) n, ms));
  if (poll(fds, n, ms) < 0) {
#if MG_ARCH == MG_ARCH_WIN32
    if (n == 0) Sleep(ms);  // On Windows, poll fails if no sockets
#endif
    memset(fds, 0, n * sizeof(fds[0]));
  }
  n = 0;
  for (struct mg_connection *c = mgr->conns; c != NULL; c = c->next) {
    if (skip_iotest(c)) {
      // Socket not valid, ignore
    } else if (mg_tls_pending(c) > 0) {
      c->is_readable = 1;
    } else {
      if (fds[n].revents & POLLERR) {
        mg_error(c, "socket error");
      } else {
        c->is_readable =
            (unsigned) (fds[n].revents & (POLLIN | POLLHUP) ? 1 : 0);
        c->is_writable = (unsigned) (fds[n].revents & POLLOUT ? 1 : 0);
      }
      n++;
    }
  }
#else
  struct timeval tv = {ms / 1000, (ms % 1000) * 1000}, tv_zero = {0, 0}, *tvp;
  struct mg_connection *c;
  fd_set rset, wset, eset;
  MG_SOCKET_TYPE maxfd = 0;
  int rc;

  FD_ZERO(&rset);
  FD_ZERO(&wset);
  FD_ZERO(&eset);
  tvp = ms < 0 ? NULL : &tv;
  for (c = mgr->conns; c != NULL; c = c->next) {
    c->is_readable = c->is_writable = 0;
    if (skip_iotest(c)) continue;
    FD_SET(FD(c), &eset);
    if (can_read(c)) FD_SET(FD(c), &rset);
    if (can_write(c)) FD_SET(FD(c), &wset);
    if (mg_tls_pending(c) > 0) tvp = &tv_zero;
    if (FD(c) > maxfd) maxfd = FD(c);
  }

  if ((rc = select((int) maxfd + 1, &rset, &wset, &eset, tvp)) < 0) {
#if MG_ARCH == MG_ARCH_WIN32
    if (maxfd == 0) Sleep(ms);  // On Windows, select fails if no sockets
#else
    MG_ERROR(("select: %d %d", rc, MG_SOCK_ERR(rc)));
#endif
    FD_ZERO(&rset);
    FD_ZERO(&wset);
    FD_ZERO(&eset);
  }

  for (c = mgr->conns; c != NULL; c = c->next) {
    if (FD(c) != MG_INVALID_SOCKET && FD_ISSET(FD(c), &eset)) {
      mg_error(c, "socket error");
    } else {
      c->is_readable = FD(c) != MG_INVALID_SOCKET && FD_ISSET(FD(c), &rset);
      c->is_writable = FD(c) != MG_INVALID_SOCKET && FD_ISSET(FD(c), &wset);
      if (mg_tls_pending(c) > 0) c->is_readable = 1;
    }
  }
#endif
}

void mg_mgr_poll(struct mg_mgr *mgr, int ms) {
  struct mg_connection *c, *tmp;
  uint64_t now;

  mg_iotest(mgr, ms);
  now = mg_millis();
  mg_timer_poll(&mgr->timers, now);

  for (c = mgr->conns; c != NULL; c = tmp) {
    bool is_resp = c->is_resp;
    tmp = c->next;
    mg_call(c, MG_EV_POLL, &now);
    if (is_resp && !c->is_resp) {
      long n = 0;
      mg_call(c, MG_EV_READ, &n);
    }
    MG_VERBOSE(("%lu %c%c %c%c%c%c%c", c->id, c->is_readable ? 'r' : '-',
                c->is_writable ? 'w' : '-', c->is_tls ? 'T' : 't',
                c->is_connecting ? 'C' : 'c', c->is_tls_hs ? 'H' : 'h',
                c->is_resolving ? 'R' : 'r', c->is_closing ? 'C' : 'c'));
    if (c->is_resolving || c->is_closing) {
      // Do nothing
    } else if (c->is_listening && c->is_udp == 0) {
      if (c->is_readable) accept_conn(mgr, c);
    } else if (c->is_connecting) {
      if (c->is_readable || c->is_writable) connect_conn(c);
    } else if (c->is_tls_hs) {
      if ((c->is_readable || c->is_writable)) mg_tls_handshake(c);
    } else {
      if (c->is_readable) read_conn(c);
      if (c->is_writable) write_conn(c);
    }

    if (c->is_draining && c->send.len == 0) c->is_closing = 1;
    if (c->is_closing) close_conn(c);
  }
}
#endif

#ifdef MG_ENABLE_LINES
#line 1 "src/ssi.c"
#endif




#ifndef MG_MAX_SSI_DEPTH
#define MG_MAX_SSI_DEPTH 5
#endif

#ifndef MG_SSI_BUFSIZ
#define MG_SSI_BUFSIZ 1024
#endif

#if MG_ENABLE_SSI
static char *mg_ssi(const char *path, const char *root, int depth) {
  struct mg_iobuf b = {NULL, 0, 0, MG_IO_SIZE};
  FILE *fp = fopen(path, "rb");
  if (fp != NULL) {
    char buf[MG_SSI_BUFSIZ], arg[sizeof(buf)];
    int ch, intag = 0;
    size_t len = 0;
    buf[0] = arg[0] = '\0';
    while ((ch = fgetc(fp)) != EOF) {
      if (intag && ch == '>' && buf[len - 1] == '-' && buf[len - 2] == '-') {
        buf[len++] = (char) (ch & 0xff);
        buf[len] = '\0';
        if (sscanf(buf, "<!--#include file=\"%[^\"]", arg)) {
          char tmp[MG_PATH_MAX + MG_SSI_BUFSIZ + 10],
              *p = (char *) path + strlen(path), *data;
          while (p > path && p[-1] != MG_DIRSEP && p[-1] != '/') p--;
          mg_snprintf(tmp, sizeof(tmp), "%.*s%s", (int) (p - path), path, arg);
          if (depth < MG_MAX_SSI_DEPTH &&
              (data = mg_ssi(tmp, root, depth + 1)) != NULL) {
            mg_iobuf_add(&b, b.len, data, strlen(data));
            free(data);
          } else {
            MG_ERROR(("%s: file=%s error or too deep", path, arg));
          }
        } else if (sscanf(buf, "<!--#include virtual=\"%[^\"]", arg)) {
          char tmp[MG_PATH_MAX + MG_SSI_BUFSIZ + 10], *data;
          mg_snprintf(tmp, sizeof(tmp), "%s%s", root, arg);
          if (depth < MG_MAX_SSI_DEPTH &&
              (data = mg_ssi(tmp, root, depth + 1)) != NULL) {
            mg_iobuf_add(&b, b.len, data, strlen(data));
            free(data);
          } else {
            MG_ERROR(("%s: virtual=%s error or too deep", path, arg));
          }
        } else {
          // Unknown SSI tag
          MG_ERROR(("Unknown SSI tag: %.*s", (int) len, buf));
          mg_iobuf_add(&b, b.len, buf, len);
        }
        intag = 0;
        len = 0;
      } else if (ch == '<') {
        intag = 1;
        if (len > 0) mg_iobuf_add(&b, b.len, buf, len);
        len = 0;
        buf[len++] = (char) (ch & 0xff);
      } else if (intag) {
        if (len == 5 && strncmp(buf, "<!--#", 5) != 0) {
          intag = 0;
        } else if (len >= sizeof(buf) - 2) {
          MG_ERROR(("%s: SSI tag is too large", path));
          len = 0;
        }
        buf[len++] = (char) (ch & 0xff);
      } else {
        buf[len++] = (char) (ch & 0xff);
        if (len >= sizeof(buf)) {
          mg_iobuf_add(&b, b.len, buf, len);
          len = 0;
        }
      }
    }
    if (len > 0) mg_iobuf_add(&b, b.len, buf, len);
    if (b.len > 0) mg_iobuf_add(&b, b.len, "", 1);  // nul-terminate
    fclose(fp);
  }
  (void) depth;
  (void) root;
  return (char *) b.buf;
}

void mg_http_serve_ssi(struct mg_connection *c, const char *root,
                       const char *fullpath) {
  const char *headers = "Content-Type: text/html; charset=utf-8\r\n";
  char *data = mg_ssi(fullpath, root, 0);
  mg_http_reply(c, 200, headers, "%s", data == NULL ? "" : data);
  free(data);
}
#else
void mg_http_serve_ssi(struct mg_connection *c, const char *root,
                       const char *fullpath) {
  mg_http_reply(c, 501, NULL, "SSI not enabled");
  (void) root, (void) fullpath;
}
#endif

#ifdef MG_ENABLE_LINES
#line 1 "src/str.c"
#endif


struct mg_str mg_str_s(const char *s) {
  struct mg_str str = {s, s == NULL ? 0 : strlen(s)};
  return str;
}

struct mg_str mg_str_n(const char *s, size_t n) {
  struct mg_str str = {s, n};
  return str;
}

int mg_lower(const char *s) {
  int c = *s;
  if (c >= 'A' && c <= 'Z') c += 'a' - 'A';
  return c;
}

int mg_ncasecmp(const char *s1, const char *s2, size_t len) {
  int diff = 0;
  if (len > 0) do {
      diff = mg_lower(s1++) - mg_lower(s2++);
    } while (diff == 0 && s1[-1] != '\0' && --len > 0);
  return diff;
}

int mg_casecmp(const char *s1, const char *s2) {
  return mg_ncasecmp(s1, s2, (size_t) ~0);
}

int mg_vcmp(const struct mg_str *s1, const char *s2) {
  size_t n2 = strlen(s2), n1 = s1->len;
  int r = strncmp(s1->ptr, s2, (n1 < n2) ? n1 : n2);
  if (r == 0) return (int) (n1 - n2);
  return r;
}

int mg_vcasecmp(const struct mg_str *str1, const char *str2) {
  size_t n2 = strlen(str2), n1 = str1->len;
  int r = mg_ncasecmp(str1->ptr, str2, (n1 < n2) ? n1 : n2);
  if (r == 0) return (int) (n1 - n2);
  return r;
}

struct mg_str mg_strdup(const struct mg_str s) {
  struct mg_str r = {NULL, 0};
  if (s.len > 0 && s.ptr != NULL) {
    char *sc = (char *) calloc(1, s.len + 1);
    if (sc != NULL) {
      memcpy(sc, s.ptr, s.len);
      sc[s.len] = '\0';
      r.ptr = sc;
      r.len = s.len;
    }
  }
  return r;
}

int mg_strcmp(const struct mg_str str1, const struct mg_str str2) {
  size_t i = 0;
  while (i < str1.len && i < str2.len) {
    int c1 = str1.ptr[i];
    int c2 = str2.ptr[i];
    if (c1 < c2) return -1;
    if (c1 > c2) return 1;
    i++;
  }
  if (i < str1.len) return 1;
  if (i < str2.len) return -1;
  return 0;
}

const char *mg_strstr(const struct mg_str haystack,
                      const struct mg_str needle) {
  size_t i;
  if (needle.len > haystack.len) return NULL;
  if (needle.len == 0) return haystack.ptr;
  for (i = 0; i <= haystack.len - needle.len; i++) {
    if (memcmp(haystack.ptr + i, needle.ptr, needle.len) == 0) {
      return haystack.ptr + i;
    }
  }
  return NULL;
}

static bool is_space(int c) {
  return c == ' ' || c == '\r' || c == '\n' || c == '\t';
}

struct mg_str mg_strstrip(struct mg_str s) {
  while (s.len > 0 && is_space((int) *s.ptr)) s.ptr++, s.len--;
  while (s.len > 0 && is_space((int) *(s.ptr + s.len - 1))) s.len--;
  return s;
}

bool mg_match(struct mg_str s, struct mg_str p, struct mg_str *caps) {
  size_t i = 0, j = 0, ni = 0, nj = 0;
  if (caps) caps->ptr = NULL, caps->len = 0;
  while (i < p.len || j < s.len) {
    if (i < p.len && j < s.len && (p.ptr[i] == '?' || s.ptr[j] == p.ptr[i])) {
      if (caps == NULL) {
      } else if (p.ptr[i] == '?') {
        caps->ptr = &s.ptr[j], caps->len = 1;     // Finalize `?` cap
        caps++, caps->ptr = NULL, caps->len = 0;  // Init next cap
      } else if (caps->ptr != NULL && caps->len == 0) {
        caps->len = (size_t) (&s.ptr[j] - caps->ptr);  // Finalize current cap
        caps++, caps->len = 0, caps->ptr = NULL;       // Init next cap
      }
      i++, j++;
    } else if (i < p.len && (p.ptr[i] == '*' || p.ptr[i] == '#')) {
      if (caps && !caps->ptr) caps->len = 0, caps->ptr = &s.ptr[j];  // Init cap
      ni = i++, nj = j + 1;
    } else if (nj > 0 && nj <= s.len && (p.ptr[ni] == '#' || s.ptr[j] != '/')) {
      i = ni, j = nj;
      if (caps && caps->ptr == NULL && caps->len == 0) {
        caps--, caps->len = 0;  // Restart previous cap
      }
    } else {
      return false;
    }
  }
  if (caps && caps->ptr && caps->len == 0) {
    caps->len = (size_t) (&s.ptr[j] - caps->ptr);
  }
  return true;
}

bool mg_globmatch(const char *s1, size_t n1, const char *s2, size_t n2) {
  return mg_match(mg_str_n(s2, n2), mg_str_n(s1, n1), NULL);
}

static size_t mg_nce(const char *s, size_t n, size_t ofs, size_t *koff,
                     size_t *klen, size_t *voff, size_t *vlen, char delim) {
  size_t kvlen, kl;
  for (kvlen = 0; ofs + kvlen < n && s[ofs + kvlen] != delim;) kvlen++;
  for (kl = 0; kl < kvlen && s[ofs + kl] != '=';) kl++;
  if (koff != NULL) *koff = ofs;
  if (klen != NULL) *klen = kl;
  if (voff != NULL) *voff = kl < kvlen ? ofs + kl + 1 : 0;
  if (vlen != NULL) *vlen = kl < kvlen ? kvlen - kl - 1 : 0;
  ofs += kvlen + 1;
  return ofs > n ? n : ofs;
}

bool mg_split(struct mg_str *s, struct mg_str *k, struct mg_str *v, char sep) {
  size_t koff = 0, klen = 0, voff = 0, vlen = 0, off = 0;
  if (s->ptr == NULL || s->len == 0) return 0;
  off = mg_nce(s->ptr, s->len, 0, &koff, &klen, &voff, &vlen, sep);
  if (k != NULL) *k = mg_str_n(s->ptr + koff, klen);
  if (v != NULL) *v = mg_str_n(s->ptr + voff, vlen);
  *s = mg_str_n(s->ptr + off, s->len - off);
  return off > 0;
}

bool mg_commalist(struct mg_str *s, struct mg_str *k, struct mg_str *v) {
  return mg_split(s, k, v, ',');
}

char *mg_hex(const void *buf, size_t len, char *to) {
  const unsigned char *p = (const unsigned char *) buf;
  const char *hex = "0123456789abcdef";
  size_t i = 0;
  for (; len--; p++) {
    to[i++] = hex[p[0] >> 4];
    to[i++] = hex[p[0] & 0x0f];
  }
  to[i] = '\0';
  return to;
}

static unsigned char mg_unhex_nimble(unsigned char c) {
  return (c >= '0' && c <= '9')   ? (unsigned char) (c - '0')
         : (c >= 'A' && c <= 'F') ? (unsigned char) (c - '7')
                                  : (unsigned char) (c - 'W');
}

unsigned long mg_unhexn(const char *s, size_t len) {
  unsigned long i = 0, v = 0;
  for (i = 0; i < len; i++) v <<= 4, v |= mg_unhex_nimble(((uint8_t *) s)[i]);
  return v;
}

void mg_unhex(const char *buf, size_t len, unsigned char *to) {
  size_t i;
  for (i = 0; i < len; i += 2) {
    to[i >> 1] = (unsigned char) mg_unhexn(&buf[i], 2);
  }
}

bool mg_path_is_sane(const char *path) {
  const char *s = path;
  for (; s[0] != '\0'; s++) {
    if (s == path || s[0] == '/' || s[0] == '\\') {  // Subdir?
      if (s[1] == '.' && s[2] == '.') return false;  // Starts with ..
    }
  }
  return true;
}

#ifdef MG_ENABLE_LINES
#line 1 "src/timer.c"
#endif



#define MG_TIMER_CALLED 4

void mg_timer_init(struct mg_timer **head, struct mg_timer *t, uint64_t ms,
                   unsigned flags, void (*fn)(void *), void *arg) {
  t->id = 0, t->period_ms = ms, t->expire = 0;
  t->flags = flags, t->fn = fn, t->arg = arg, t->next = *head;
  *head = t;
}

void mg_timer_free(struct mg_timer **head, struct mg_timer *t) {
  while (*head && *head != t) head = &(*head)->next;
  if (*head) *head = t->next;
}

// t: expiration time, prd: period, now: current time. Return true if expired
bool mg_timer_expired(uint64_t *t, uint64_t prd, uint64_t now) {
  if (now + prd < *t) *t = 0;                    // Time wrapped? Reset timer
  if (*t == 0) *t = now + prd;                   // Firt poll? Set expiration
  if (*t > now) return false;                    // Not expired yet, return
  *t = (now - *t) > prd ? now + prd : *t + prd;  // Next expiration time
  return true;                                   // Expired, return true
}

void mg_timer_poll(struct mg_timer **head, uint64_t now_ms) {
  struct mg_timer *t, *tmp;
  for (t = *head; t != NULL; t = tmp) {
    bool once = t->expire == 0 && (t->flags & MG_TIMER_RUN_NOW) &&
                !(t->flags & MG_TIMER_CALLED);  // Handle MG_TIMER_NOW only once
    bool expired = mg_timer_expired(&t->expire, t->period_ms, now_ms);
    tmp = t->next;
    if (!once && !expired) continue;
    if ((t->flags & MG_TIMER_REPEAT) || !(t->flags & MG_TIMER_CALLED)) {
      t->fn(t->arg);
    }
    t->flags |= MG_TIMER_CALLED;
  }
}

#ifdef MG_ENABLE_LINES
#line 1 "src/tls_builtin.c"
#endif


#if MG_TLS == MG_TLS_BUILTIN
struct tls_data {
  struct mg_iobuf send;
  struct mg_iobuf recv;
};

#define MG_LOAD_BE16(p) ((uint16_t) ((MG_U8P(p)[0] << 8U) | MG_U8P(p)[1]))
#define TLS_HDR_SIZE 5  // 1 byte type, 2 bytes version, 2 bytes len

static inline bool mg_is_big_endian(void) {
  int v = 1;
  return *(unsigned char *) &v == 1;
}
static inline uint16_t mg_swap16(uint16_t v) {
  return (uint16_t) ((v << 8U) | (v >> 8U));
}
static inline uint32_t mg_swap32(uint32_t v) {
  return (v >> 24) | (v >> 8 & 0xff00) | (v << 8 & 0xff0000) | (v << 24);
}
static inline uint64_t mg_swap64(uint64_t v) {
  return (((uint64_t) mg_swap32((uint32_t) v)) << 32) |
         mg_swap32((uint32_t) (v >> 32));
}
static inline uint16_t mg_be16(uint16_t v) {
  return mg_is_big_endian() ? mg_swap16(v) : v;
}
static inline uint32_t mg_be32(uint32_t v) {
  return mg_is_big_endian() ? mg_swap32(v) : v;
}

static inline void add8(struct mg_iobuf *io, uint8_t data) {
  mg_iobuf_add(io, io->len, &data, sizeof(data));
}
static inline void add16(struct mg_iobuf *io, uint16_t data) {
  data = mg_htons(data);
  mg_iobuf_add(io, io->len, &data, sizeof(data));
}
static inline void add32(struct mg_iobuf *io, uint32_t data) {
  data = mg_htonl(data);
  mg_iobuf_add(io, io->len, &data, sizeof(data));
}

void mg_tls_init(struct mg_connection *c, struct mg_str hostname) {
  struct tls_data *tls = (struct tls_data *) calloc(1, sizeof(struct tls_data));
  if (tls != NULL) {
    tls->send.align = tls->recv.align = MG_IO_SIZE;
    c->tls = tls;
    c->is_tls = c->is_tls_hs = 1;
  } else {
    mg_error(c, "tls oom");
  }
  (void) hostname;
}
void mg_tls_free(struct mg_connection *c) {
  struct tls_data *tls = c->tls;
  if (tls != NULL) {
    mg_iobuf_free(&tls->send);
    mg_iobuf_free(&tls->recv);
  }
  free(c->tls);
  c->tls = NULL;
}
long mg_tls_send(struct mg_connection *c, const void *buf, size_t len) {
  (void) c, (void) buf, (void) len;
  // MG_INFO(("BBBBBBBB"));
  return -1;
}
long mg_tls_recv(struct mg_connection *c, void *buf, size_t len) {
  (void) c, (void) buf, (void) len;
  char tmp[8192];
  long n = mg_io_recv(c, tmp, sizeof(tmp));
  if (n > 0) mg_hexdump(tmp, (size_t) n);
  MG_INFO(("AAAAAAAA"));
  return -1;
  // struct mg_tls *tls = (struct mg_tls *) c->tls;
  // long n = mbedtls_ssl_read(&tls->ssl, (unsigned char *) buf, len);
  // if (n == MBEDTLS_ERR_SSL_WANT_READ || n == MBEDTLS_ERR_SSL_WANT_WRITE)
  //   return MG_IO_WAIT;
  // if (n <= 0) return MG_IO_ERR;
  // return n;
}
size_t mg_tls_pending(struct mg_connection *c) {
  (void) c;
  return 0;
}
void mg_tls_handshake(struct mg_connection *c) {
  struct tls_data *tls = c->tls;
  struct mg_iobuf *rio = &tls->recv;
  struct mg_iobuf *wio = &tls->send;
  // Pull data from TCP
  for (;;) {
    mg_iobuf_resize(rio, rio->len + 1);
    long n = mg_io_recv(c, &rio->buf[rio->len], rio->size - rio->len);
    if (n > 0) {
      rio->len += (size_t) n;
    } else if (n == MG_IO_WAIT) {
      break;
    } else {
      mg_error(c, "IO err");
      return;
    }
  }
  // Look if we've pulled everything
  if (rio->len < TLS_HDR_SIZE) return;
  uint8_t record_type = rio->buf[0];
  uint16_t record_len = MG_LOAD_BE16(rio->buf + 3);
  uint16_t record_version = MG_LOAD_BE16(rio->buf + 1);
  if (record_type != 22) {
    mg_error(c, "no 22");
    return;
  }
  if (rio->len < (size_t) TLS_HDR_SIZE + record_len) return;
  // Got full hello
  // struct tls_hello *hello = (struct tls_hello *) (hdr + 1);
  MG_INFO(("CT=%d V=%hx L=%hu", record_type, record_version, record_len));
  mg_hexdump(rio->buf, rio->len);

  // Send response. Server Hello
  size_t ofs = wio->len;
  add8(wio, 22), add16(wio, 0x303), add16(wio, 0);  // Layer: type, ver, len
  add8(wio, 2), add8(wio, 0), add16(wio, 0), add16(wio, 0x304);  // Hello
  mg_iobuf_add(wio, wio->len, NULL, 32);                         // 32 random
  mg_random(wio->buf + wio->len - 32, 32);                       // bytes
  add8(wio, 0);                                                  // Session ID
  add16(wio, 0x1301);  // Cipher: TLS_AES_128_GCM_SHA256
  add8(wio, 0);        // Compression method: 0
  add16(wio, 46);      // Extensions length
  add16(wio, 43), add16(wio, 2), add16(wio, 0x304);  // extension: TLS 1.3
  add16(wio, 51), add16(wio, 36), add16(wio, 29), add16(wio, 32);  // keyshare
  mg_iobuf_add(wio, wio->len, NULL, 32);                           // 32 random
  mg_random(wio->buf + wio->len - 32, 32);                         // bytes
  *(uint16_t *) &wio->buf[ofs + 3] = mg_be16((uint16_t) (wio->len - ofs - 5));
  *(uint16_t *) &wio->buf[ofs + 7] = mg_be16((uint16_t) (wio->len - ofs - 9));

  // Change cipher. Cipher's payload is an encypted app data
  // ofs = wio->len;
  add8(wio, 20), add16(wio, 0x303);  // Layer: type, version
  add16(wio, 1), add8(wio, 1);

  ofs = wio->len;                                   // Application data
  add8(wio, 23), add16(wio, 0x303), add16(wio, 5);  // Layer: type, version
  // mg_iobuf_add(wio, wio->len, "\x01\x02\x03\x04\x05", 5);
  add8(wio, 22);                                       // handshake message
  add8(wio, 8);                                        // encrypted extensions
  add8(wio, 0), add16(wio, 2), add16(wio, 0);          // empty 2 bytes
  add8(wio, 11);                                       // certificate message
  add8(wio, 0), add16(wio, 4), add32(wio, 0x1020304);  // len
  *(uint16_t *) &wio->buf[ofs + 3] = mg_be16((uint16_t)(wio->len - ofs - 5));

  mg_io_send(c, wio->buf, wio->len);
  wio->len = 0;

  rio->len = 0;
  c->is_tls_hs = 0;
  mg_error(c, "doh");
}
void mg_tls_ctx_free(struct mg_mgr *mgr) {
  mgr->tls_ctx = NULL;
}
void mg_tls_ctx_init(struct mg_mgr *mgr, const struct mg_tls_opts *opts) {
  (void) opts, (void) mgr;
}
#endif

#ifdef MG_ENABLE_LINES
#line 1 "src/tls_dummy.c"
#endif


#if MG_TLS == MG_TLS_NONE
void mg_tls_init(struct mg_connection *c, const struct mg_tls_opts *opts) {
  (void) opts;
  mg_error(c, "TLS is not enabled");
}
void mg_tls_handshake(struct mg_connection *c) {
  (void) c;
}
void mg_tls_free(struct mg_connection *c) {
  (void) c;
}
long mg_tls_recv(struct mg_connection *c, void *buf, size_t len) {
  return c == NULL || buf == NULL || len == 0 ? 0 : -1;
}
long mg_tls_send(struct mg_connection *c, const void *buf, size_t len) {
  return c == NULL || buf == NULL || len == 0 ? 0 : -1;
}
size_t mg_tls_pending(struct mg_connection *c) {
  (void) c;
  return 0;
}
void mg_tls_ctx_init(struct mg_mgr *mgr) {
  (void) mgr;
}
void mg_tls_ctx_free(struct mg_mgr *mgr) {
  (void) mgr;
}
#endif

#ifdef MG_ENABLE_LINES
#line 1 "src/tls_mbed.c"
#endif



#if MG_TLS == MG_TLS_MBED

#if defined(MBEDTLS_VERSION_NUMBER) && MBEDTLS_VERSION_NUMBER >= 0x03000000
#define MG_MBEDTLS_RNG_GET , mg_mbed_rng, NULL
#else
#define MG_MBEDTLS_RNG_GET
#endif

static int mg_mbed_rng(void *ctx, unsigned char *buf, size_t len) {
  mg_random(buf, len);
  (void) ctx;
  return 0;
}

static bool mg_load_cert(struct mg_str str, mbedtls_x509_crt *p) {
  int rc;
  if (str.ptr == NULL || str.ptr[0] == '\0' || str.ptr[0] == '*') return true;
  if (str.ptr[0] == '-') str.len++;  // PEM, include trailing NUL
  if ((rc = mbedtls_x509_crt_parse(p, (uint8_t *) str.ptr, str.len)) != 0) {
    MG_ERROR(("cert err %#x", -rc));
    return false;
  }
  return true;
}

static bool mg_load_key(struct mg_str str, mbedtls_pk_context *p) {
  int rc;
  if (str.ptr == NULL || str.ptr[0] == '\0' || str.ptr[0] == '*') return true;
  if (str.ptr[0] == '-') str.len++;  // PEM, include trailing NUL
  if ((rc = mbedtls_pk_parse_key(p, (uint8_t *) str.ptr, str.len, NULL,
                                 0 MG_MBEDTLS_RNG_GET)) != 0) {
    MG_ERROR(("key err %#x", -rc));
    return false;
  }
  return true;
}

void mg_tls_free(struct mg_connection *c) {
  struct mg_tls *tls = (struct mg_tls *) c->tls;
  if (tls != NULL) {
    mbedtls_ssl_free(&tls->ssl);
    mbedtls_pk_free(&tls->pk);
    mbedtls_x509_crt_free(&tls->ca);
    mbedtls_x509_crt_free(&tls->cert);
    mbedtls_ssl_config_free(&tls->conf);
#ifdef MBEDTLS_SSL_SESSION_TICKETS
    mbedtls_ssl_ticket_free(&tls->ticket);
#endif
    free(tls);
    c->tls = NULL;
  }
}

static int mg_net_send(void *ctx, const unsigned char *buf, size_t len) {
  long n = mg_io_send((struct mg_connection *) ctx, buf, len);
  MG_VERBOSE(("%lu n=%ld e=%d", ((struct mg_connection *) ctx)->id, n, errno));
  if (n == MG_IO_WAIT) return MBEDTLS_ERR_SSL_WANT_WRITE;
  if (n == MG_IO_RESET) return MBEDTLS_ERR_NET_CONN_RESET;
  if (n == MG_IO_ERR) return MBEDTLS_ERR_NET_SEND_FAILED;
  return (int) n;
}

static int mg_net_recv(void *ctx, unsigned char *buf, size_t len) {
  long n = mg_io_recv((struct mg_connection *) ctx, buf, len);
  MG_VERBOSE(("%lu n=%ld", ((struct mg_connection *) ctx)->id, n));
  if (n == MG_IO_WAIT) return MBEDTLS_ERR_SSL_WANT_WRITE;
  if (n == MG_IO_RESET) return MBEDTLS_ERR_NET_CONN_RESET;
  if (n == MG_IO_ERR) return MBEDTLS_ERR_NET_RECV_FAILED;
  return (int) n;
}

void mg_tls_handshake(struct mg_connection *c) {
  struct mg_tls *tls = (struct mg_tls *) c->tls;
  int rc = mbedtls_ssl_handshake(&tls->ssl);
  if (rc == 0) {  // Success
    MG_DEBUG(("%lu success", c->id));
    c->is_tls_hs = 0;
    mg_call(c, MG_EV_TLS_HS, NULL);
  } else if (rc == MBEDTLS_ERR_SSL_WANT_READ ||
             rc == MBEDTLS_ERR_SSL_WANT_WRITE) {  // Still pending
    MG_VERBOSE(("%lu pending, %d%d %d (-%#x)", c->id, c->is_connecting,
                c->is_tls_hs, rc, -rc));
  } else {
    mg_error(c, "TLS handshake: -%#x", -rc);  // Error
  }
}

static void debug_cb(void *c, int lev, const char *s, int n, const char *s2) {
  n = (int) strlen(s2) - 1;
  MG_INFO(("%lu %d %.*s", ((struct mg_connection *) c)->id, lev, n, s2));
  (void) s;
}

void mg_tls_init(struct mg_connection *c, const struct mg_tls_opts *opts) {
  struct mg_tls *tls = (struct mg_tls *) calloc(1, sizeof(*tls));
  int rc = 0;
  c->tls = tls;
  if (c->tls == NULL) {
    mg_error(c, "TLS OOM");
    goto fail;
  }
  if (c->is_listening) goto fail;
  MG_DEBUG(("%lu Setting TLS", c->id));
  mbedtls_ssl_init(&tls->ssl);
  mbedtls_ssl_config_init(&tls->conf);
  mbedtls_x509_crt_init(&tls->ca);
  mbedtls_x509_crt_init(&tls->cert);
  mbedtls_pk_init(&tls->pk);
  mbedtls_ssl_conf_dbg(&tls->conf, debug_cb, c);
#if defined(MG_MBEDTLS_DEBUG_LEVEL)
  mbedtls_debug_set_threshold(MG_MBEDTLS_DEBUG_LEVEL);
#endif
  if ((rc = mbedtls_ssl_config_defaults(
           &tls->conf,
           c->is_client ? MBEDTLS_SSL_IS_CLIENT : MBEDTLS_SSL_IS_SERVER,
           MBEDTLS_SSL_TRANSPORT_STREAM, MBEDTLS_SSL_PRESET_DEFAULT)) != 0) {
    mg_error(c, "tls defaults %#x", -rc);
    goto fail;
  }
  mbedtls_ssl_conf_rng(&tls->conf, mg_mbed_rng, c);

  if (opts->ca.len == 0 || mg_vcmp(&opts->ca, "*") == 0) {
    mbedtls_ssl_conf_authmode(&tls->conf, MBEDTLS_SSL_VERIFY_NONE);
  } else {
    if (mg_load_cert(opts->ca, &tls->ca) == false) goto fail;
    mbedtls_ssl_conf_ca_chain(&tls->conf, &tls->ca, NULL);
    if (c->is_client && opts->name.ptr != NULL && opts->name.ptr[0] != '\0') {
      char *host = mg_mprintf("%.*s", opts->name.len, opts->name.ptr);
      mbedtls_ssl_set_hostname(&tls->ssl, host);
      MG_DEBUG(("%lu hostname verification: %s", c->id, host));
      free(host);
    }
    mbedtls_ssl_conf_authmode(&tls->conf, MBEDTLS_SSL_VERIFY_REQUIRED);
  }
  if (!mg_load_cert(opts->cert, &tls->cert)) goto fail;
  if (!mg_load_key(opts->key, &tls->pk)) goto fail;
  if (tls->cert.version &&
      (rc = mbedtls_ssl_conf_own_cert(&tls->conf, &tls->cert, &tls->pk)) != 0) {
    mg_error(c, "own cert %#x", -rc);
    goto fail;
  }

#ifdef MBEDTLS_SSL_SESSION_TICKETS
  mbedtls_ssl_conf_session_tickets_cb(
      &tls->conf, mbedtls_ssl_ticket_write, mbedtls_ssl_ticket_parse,
      &((struct mg_tls_ctx *) c->mgr->tls_ctx)->tickets);
#endif

  if ((rc = mbedtls_ssl_setup(&tls->ssl, &tls->conf)) != 0) {
    mg_error(c, "setup err %#x", -rc);
    goto fail;
  }
  c->is_tls = 1;
  c->is_tls_hs = 1;
  mbedtls_ssl_set_bio(&tls->ssl, c, mg_net_send, mg_net_recv, 0);
  if (c->is_client && c->is_resolving == 0 && c->is_connecting == 0) {
    mg_tls_handshake(c);
  }
  return;
fail:
  mg_tls_free(c);
}

size_t mg_tls_pending(struct mg_connection *c) {
  struct mg_tls *tls = (struct mg_tls *) c->tls;
  return tls == NULL ? 0 : mbedtls_ssl_get_bytes_avail(&tls->ssl);
}

long mg_tls_recv(struct mg_connection *c, void *buf, size_t len) {
  struct mg_tls *tls = (struct mg_tls *) c->tls;
  long n = mbedtls_ssl_read(&tls->ssl, (unsigned char *) buf, len);
  if (n == MBEDTLS_ERR_SSL_WANT_READ || n == MBEDTLS_ERR_SSL_WANT_WRITE)
    return MG_IO_WAIT;
  if (n <= 0) return MG_IO_ERR;
  return n;
}

long mg_tls_send(struct mg_connection *c, const void *buf, size_t len) {
  struct mg_tls *tls = (struct mg_tls *) c->tls;
  long n = mbedtls_ssl_write(&tls->ssl, (unsigned char *) buf, len);
  if (n == MBEDTLS_ERR_SSL_WANT_READ || n == MBEDTLS_ERR_SSL_WANT_WRITE)
    return MG_IO_WAIT;
  if (n <= 0) return MG_IO_ERR;
  return n;
}

void mg_tls_ctx_init(struct mg_mgr *mgr) {
  struct mg_tls_ctx *ctx = (struct mg_tls_ctx *) calloc(1, sizeof(*ctx));
  if (ctx == NULL) {
    MG_ERROR(("TLS context init OOM"));
  } else {
#ifdef MBEDTLS_SSL_SESSION_TICKETS
    int rc;
    mbedtls_ssl_ticket_init(&ctx->tickets);
    if ((rc = mbedtls_ssl_ticket_setup(&ctx->tickets, mg_mbed_rng, NULL,
                                       MBEDTLS_CIPHER_AES_128_GCM, 86400)) !=
        0) {
      MG_ERROR((" mbedtls_ssl_ticket_setup %#x", -rc));
    }
#endif
    mgr->tls_ctx = ctx;
  }
}

void mg_tls_ctx_free(struct mg_mgr *mgr) {
  struct mg_tls_ctx *ctx = (struct mg_tls_ctx *) mgr->tls_ctx;
  if (ctx != NULL) {
#ifdef MBEDTLS_SSL_SESSION_TICKETS
    mbedtls_ssl_ticket_free(&ctx->tickets);
#endif
    free(ctx);
    mgr->tls_ctx = NULL;
  }
}
#endif

#ifdef MG_ENABLE_LINES
#line 1 "src/tls_openssl.c"
#endif



#if MG_TLS == MG_TLS_OPENSSL
static int mg_tls_err(struct mg_tls *tls, int res) {
  int err = SSL_get_error(tls->ssl, res);
  // We've just fetched the last error from the queue.
  // Now we need to clear the error queue. If we do not, then the following
  // can happen (actually reported):
  //  - A new connection is accept()-ed with cert error (e.g. self-signed cert)
  //  - Since all accept()-ed connections share listener's context,
  //  - *ALL* SSL accepted connection report read error on the next poll cycle.
  //    Thus a single errored connection can close all the rest, unrelated ones.
  // Clearing the error keeps the shared SSL_CTX in an OK state.

  if (err != 0) ERR_print_errors_fp(stderr);
  ERR_clear_error();
  if (err == SSL_ERROR_WANT_READ) return 0;
  if (err == SSL_ERROR_WANT_WRITE) return 0;
  return err;
}

static STACK_OF(X509_INFO) * load_ca_certs(struct mg_str ca) {
  BIO *bio = BIO_new_mem_buf(ca.ptr, (int) ca.len);
  STACK_OF(X509_INFO) *certs =
      bio ? PEM_X509_INFO_read_bio(bio, NULL, NULL, NULL) : NULL;
  if (bio) BIO_free(bio);
  return certs;
}

static bool add_ca_certs(SSL_CTX *ctx, STACK_OF(X509_INFO) * certs) {
  X509_STORE *cert_store = SSL_CTX_get_cert_store(ctx);
  for (int i = 0; i < sk_X509_INFO_num(certs); i++) {
    X509_INFO *cert_info = sk_X509_INFO_value(certs, i);
    if (cert_info->x509 && !X509_STORE_add_cert(cert_store, cert_info->x509))
      return false;
  }
  return true;
}

static EVP_PKEY *load_key(struct mg_str s) {
  BIO *bio = BIO_new_mem_buf(s.ptr, (int) (long) s.len);
  EVP_PKEY *key = bio ? PEM_read_bio_PrivateKey(bio, NULL, 0, NULL) : NULL;
  if (bio) BIO_free(bio);
  return key;
}

static X509 *load_cert(struct mg_str s) {
  BIO *bio = BIO_new_mem_buf(s.ptr, (int) (long) s.len);
  X509 *cert = bio == NULL ? NULL
               : s.ptr[0] == '-'
                   ? PEM_read_bio_X509(bio, NULL, NULL, NULL)  // PEM
                   : d2i_X509_bio(bio, NULL);                  // DER
  if (bio) BIO_free(bio);
  return cert;
}

void mg_tls_init(struct mg_connection *c, const struct mg_tls_opts *opts) {
  struct mg_tls *tls = (struct mg_tls *) calloc(1, sizeof(*tls));
  const char *id = "mongoose";
  static unsigned char s_initialised = 0;
  int rc;

  if (tls == NULL) {
    mg_error(c, "TLS OOM");
    goto fail;
  }

  if (!s_initialised) {
    SSL_library_init();
    s_initialised++;
  }
  MG_DEBUG(("%lu Setting TLS", c->id));
  tls->ctx = c->is_client ? SSL_CTX_new(SSLv23_client_method())
                          : SSL_CTX_new(SSLv23_server_method());
  if ((tls->ssl = SSL_new(tls->ctx)) == NULL) {
    mg_error(c, "SSL_new");
    goto fail;
  }
  SSL_set_session_id_context(tls->ssl, (const uint8_t *) id,
                             (unsigned) strlen(id));
  // Disable deprecated protocols
  SSL_set_options(tls->ssl, SSL_OP_NO_SSLv2);
  SSL_set_options(tls->ssl, SSL_OP_NO_SSLv3);
  SSL_set_options(tls->ssl, SSL_OP_NO_TLSv1);
  SSL_set_options(tls->ssl, SSL_OP_NO_TLSv1_1);
#ifdef MG_ENABLE_OPENSSL_NO_COMPRESSION
  SSL_set_options(tls->ssl, SSL_OP_NO_COMPRESSION);
#endif
#ifdef MG_ENABLE_OPENSSL_CIPHER_SERVER_PREFERENCE
  SSL_set_options(tls->ssl, SSL_OP_CIPHER_SERVER_PREFERENCE);
#endif

  if (opts->ca.ptr != NULL && opts->ca.ptr[0] != '\0') {
    SSL_set_verify(tls->ssl, SSL_VERIFY_PEER | SSL_VERIFY_FAIL_IF_NO_PEER_CERT,
                   NULL);
    STACK_OF(X509_INFO) *certs = load_ca_certs(opts->ca);
    rc = add_ca_certs(tls->ctx, certs);
    sk_X509_INFO_pop_free(certs, X509_INFO_free);
    if (!rc) {
      mg_error(c, "CA err");
      goto fail;
    }
  }
  if (opts->cert.ptr != NULL && opts->cert.ptr[0] != '\0') {
    X509 *cert = load_cert(opts->cert);
    rc = cert == NULL ? 0 : SSL_use_certificate(tls->ssl, cert);
    X509_free(cert);
    if (cert == NULL || rc != 1) {
      mg_error(c, "CERT err %d", mg_tls_err(tls, rc));
      goto fail;
    }
  }
  if (opts->key.ptr != NULL && opts->key.ptr[0] != '\0') {
    EVP_PKEY *key = load_key(opts->key);
    rc = key == NULL ? 0 : SSL_use_PrivateKey(tls->ssl, key);
    EVP_PKEY_free(key);
    if (key == NULL || rc != 1) {
      mg_error(c, "KEY err %d", mg_tls_err(tls, rc));
      goto fail;
    }
  }

  SSL_set_mode(tls->ssl, SSL_MODE_ACCEPT_MOVING_WRITE_BUFFER);
#if OPENSSL_VERSION_NUMBER > 0x10002000L
  SSL_set_ecdh_auto(tls->ssl, 1);
#endif
#if OPENSSL_VERSION_NUMBER >= 0x10100000L
  if (opts->name.len > 0) {
    char *s = mg_mprintf("%.*s", (int) opts->name.len, opts->name.ptr);
    SSL_set1_host(tls->ssl, s);
    SSL_set_tlsext_host_name(tls->ssl, s);
    free(s);
  }
#endif
  c->tls = tls;
  c->is_tls = 1;
  c->is_tls_hs = 1;
  if (c->is_client && c->is_resolving == 0 && c->is_connecting == 0) {
    mg_tls_handshake(c);
  }
  MG_DEBUG(("%lu SSL %s OK", c->id, c->is_accepted ? "accept" : "client"));
  return;
fail:
  free(tls);
}

void mg_tls_handshake(struct mg_connection *c) {
  struct mg_tls *tls = (struct mg_tls *) c->tls;
  int rc;
  SSL_set_fd(tls->ssl, (int) (size_t) c->fd);
  rc = c->is_client ? SSL_connect(tls->ssl) : SSL_accept(tls->ssl);
  if (rc == 1) {
    MG_DEBUG(("%lu success", c->id));
    c->is_tls_hs = 0;
    mg_call(c, MG_EV_TLS_HS, NULL);
  } else {
    int code = mg_tls_err(tls, rc);
    if (code != 0) mg_error(c, "tls hs: rc %d, err %d", rc, code);
  }
}

void mg_tls_free(struct mg_connection *c) {
  struct mg_tls *tls = (struct mg_tls *) c->tls;
  if (tls == NULL) return;
  SSL_free(tls->ssl);
  SSL_CTX_free(tls->ctx);
  free(tls);
  c->tls = NULL;
}

size_t mg_tls_pending(struct mg_connection *c) {
  struct mg_tls *tls = (struct mg_tls *) c->tls;
  return tls == NULL ? 0 : (size_t) SSL_pending(tls->ssl);
}

long mg_tls_recv(struct mg_connection *c, void *buf, size_t len) {
  struct mg_tls *tls = (struct mg_tls *) c->tls;
  int n = SSL_read(tls->ssl, buf, (int) len);
  if (n < 0 && mg_tls_err(tls, n) == 0) return MG_IO_WAIT;
  if (n <= 0) return MG_IO_ERR;
  return n;
}

long mg_tls_send(struct mg_connection *c, const void *buf, size_t len) {
  struct mg_tls *tls = (struct mg_tls *) c->tls;
  int n = SSL_write(tls->ssl, buf, (int) len);
  if (n < 0 && mg_tls_err(tls, n) == 0) return MG_IO_WAIT;
  if (n <= 0) return MG_IO_ERR;
  return n;
}

void mg_tls_ctx_init(struct mg_mgr *mgr) {
  (void) mgr;
}

void mg_tls_ctx_free(struct mg_mgr *mgr) {
  (void) mgr;
}
#endif

#ifdef MG_ENABLE_LINES
#line 1 "src/url.c"
#endif


struct url {
  size_t key, user, pass, host, port, uri, end;
};

int mg_url_is_ssl(const char *url) {
  return strncmp(url, "wss:", 4) == 0 || strncmp(url, "https:", 6) == 0 ||
         strncmp(url, "mqtts:", 6) == 0 || strncmp(url, "ssl:", 4) == 0 ||
         strncmp(url, "tls:", 4) == 0 || strncmp(url, "tcps:", 5) == 0;
}

static struct url urlparse(const char *url) {
  size_t i;
  struct url u;
  memset(&u, 0, sizeof(u));
  for (i = 0; url[i] != '\0'; i++) {
    if (url[i] == '/' && i > 0 && u.host == 0 && url[i - 1] == '/') {
      u.host = i + 1;
      u.port = 0;
    } else if (url[i] == ']') {
      u.port = 0;  // IPv6 URLs, like http://[::1]/bar
    } else if (url[i] == ':' && u.port == 0 && u.uri == 0) {
      u.port = i + 1;
    } else if (url[i] == '@' && u.user == 0 && u.pass == 0 && u.uri == 0) {
      u.user = u.host;
      u.pass = u.port;
      u.host = i + 1;
      u.port = 0;
    } else if (url[i] == '/' && u.host && u.uri == 0) {
      u.uri = i;
    }
  }
  u.end = i;
#if 0
  printf("[%s] %d %d %d %d %d\n", url, u.user, u.pass, u.host, u.port, u.uri);
#endif
  return u;
}

struct mg_str mg_url_host(const char *url) {
  struct url u = urlparse(url);
  size_t n = u.port  ? u.port - u.host - 1
             : u.uri ? u.uri - u.host
                     : u.end - u.host;
  struct mg_str s = mg_str_n(url + u.host, n);
  return s;
}

const char *mg_url_uri(const char *url) {
  struct url u = urlparse(url);
  return u.uri ? url + u.uri : "/";
}

unsigned short mg_url_port(const char *url) {
  struct url u = urlparse(url);
  unsigned short port = 0;
  if (strncmp(url, "http:", 5) == 0 || strncmp(url, "ws:", 3) == 0) port = 80;
  if (strncmp(url, "wss:", 4) == 0 || strncmp(url, "https:", 6) == 0)
    port = 443;
  if (strncmp(url, "mqtt:", 5) == 0) port = 1883;
  if (strncmp(url, "mqtts:", 6) == 0) port = 8883;
  if (u.port) port = (unsigned short) atoi(url + u.port);
  return port;
}

struct mg_str mg_url_user(const char *url) {
  struct url u = urlparse(url);
  struct mg_str s = mg_str("");
  if (u.user && (u.pass || u.host)) {
    size_t n = u.pass ? u.pass - u.user - 1 : u.host - u.user - 1;
    s = mg_str_n(url + u.user, n);
  }
  return s;
}

struct mg_str mg_url_pass(const char *url) {
  struct url u = urlparse(url);
  struct mg_str s = mg_str_n("", 0UL);
  if (u.pass && u.host) {
    size_t n = u.host - u.pass - 1;
    s = mg_str_n(url + u.pass, n);
  }
  return s;
}

#ifdef MG_ENABLE_LINES
#line 1 "src/util.c"
#endif


// Not using memset for zeroing memory, cause it can be dropped by compiler
// See https://github.com/cesanta/mongoose/pull/1265
void mg_bzero(volatile unsigned char *buf, size_t len) {
  if (buf != NULL) {
    while (len--) *buf++ = 0;
  }
}

#if MG_ENABLE_CUSTOM_RANDOM
#else
void mg_random(void *buf, size_t len) {
  bool done = false;
  unsigned char *p = (unsigned char *) buf;
#if MG_ARCH == MG_ARCH_ESP32
  while (len--) *p++ = (unsigned char) (esp_random() & 255);
  done = true;
#elif MG_ARCH == MG_ARCH_WIN32
#elif MG_ARCH == MG_ARCH_UNIX
  FILE *fp = fopen("/dev/urandom", "rb");
  if (fp != NULL) {
    if (fread(buf, 1, len, fp) == len) done = true;
    fclose(fp);
  }
#endif
  // If everything above did not work, fallback to a pseudo random generator
  while (!done && len--) *p++ = (unsigned char) (rand() & 255);
}
#endif

char *mg_random_str(char *buf, size_t len) {
  size_t i;
  mg_random(buf, len);
  for (i = 0; i < len; i++) {
    uint8_t c = ((uint8_t *) buf)[i] % 62U;
    buf[i] = i == len - 1 ? (char) '\0'            // 0-terminate last byte
             : c < 26     ? (char) ('a' + c)       // lowercase
             : c < 52     ? (char) ('A' + c - 26)  // uppercase
                          : (char) ('0' + c - 52);     // numeric
  }
  return buf;
}

uint32_t mg_ntohl(uint32_t net) {
  uint8_t data[4] = {0, 0, 0, 0};
  memcpy(&data, &net, sizeof(data));
  return (((uint32_t) data[3]) << 0) | (((uint32_t) data[2]) << 8) |
         (((uint32_t) data[1]) << 16) | (((uint32_t) data[0]) << 24);
}

uint16_t mg_ntohs(uint16_t net) {
  uint8_t data[2] = {0, 0};
  memcpy(&data, &net, sizeof(data));
  return (uint16_t) ((uint16_t) data[1] | (((uint16_t) data[0]) << 8));
}

uint32_t mg_crc32(uint32_t crc, const char *buf, size_t len) {
  static const uint32_t crclut[16] = {
      // table for polynomial 0xEDB88320 (reflected)
      0x00000000, 0x1DB71064, 0x3B6E20C8, 0x26D930AC, 0x76DC4190, 0x6B6B51F4,
      0x4DB26158, 0x5005713C, 0xEDB88320, 0xF00F9344, 0xD6D6A3E8, 0xCB61B38C,
      0x9B64C2B0, 0x86D3D2D4, 0xA00AE278, 0xBDBDF21C};
  crc = ~crc;
  while (len--) {
    uint8_t byte = *(uint8_t *) buf++;
    crc = crclut[(crc ^ byte) & 0x0F] ^ (crc >> 4);
    crc = crclut[(crc ^ (byte >> 4)) & 0x0F] ^ (crc >> 4);
  }
  return ~crc;
}

static int isbyte(int n) {
  return n >= 0 && n <= 255;
}

static int parse_net(const char *spec, uint32_t *net, uint32_t *mask) {
  int n, a, b, c, d, slash = 32, len = 0;
  if ((sscanf(spec, "%d.%d.%d.%d/%d%n", &a, &b, &c, &d, &slash, &n) == 5 ||
       sscanf(spec, "%d.%d.%d.%d%n", &a, &b, &c, &d, &n) == 4) &&
      isbyte(a) && isbyte(b) && isbyte(c) && isbyte(d) && slash >= 0 &&
      slash < 33) {
    len = n;
    *net = ((uint32_t) a << 24) | ((uint32_t) b << 16) | ((uint32_t) c << 8) |
           (uint32_t) d;
    *mask = slash ? (uint32_t) (0xffffffffU << (32 - slash)) : (uint32_t) 0;
  }
  return len;
}

int mg_check_ip_acl(struct mg_str acl, struct mg_addr *remote_ip) {
  struct mg_str k, v;
  int allowed = acl.len == 0 ? '+' : '-';  // If any ACL is set, deny by default
  uint32_t remote_ip4;
  if (remote_ip->is_ip6) {
    return -1;  // TODO(): handle IPv6 ACL and addresses
  } else {      // IPv4
    memcpy((void *) &remote_ip4, remote_ip->ip, sizeof(remote_ip4));
    while (mg_commalist(&acl, &k, &v)) {
      uint32_t net, mask;
      if (k.ptr[0] != '+' && k.ptr[0] != '-') return -1;
      if (parse_net(&k.ptr[1], &net, &mask) == 0) return -2;
      if ((mg_ntohl(remote_ip4) & mask) == net) allowed = k.ptr[0];
    }
  }
  return allowed == '+';
}

#if MG_ENABLE_CUSTOM_MILLIS
#else
uint64_t mg_millis(void) {
#if MG_ARCH == MG_ARCH_WIN32
  return GetTickCount();
#elif MG_ARCH == MG_ARCH_RP2040
  return time_us_64() / 1000;
#elif MG_ARCH == MG_ARCH_ESP8266 || MG_ARCH == MG_ARCH_ESP32 || \
    MG_ARCH == MG_ARCH_FREERTOS
  return xTaskGetTickCount() * portTICK_PERIOD_MS;
#elif MG_ARCH == MG_ARCH_AZURERTOS
  return tx_time_get() * (1000 /* MS per SEC */ / TX_TIMER_TICKS_PER_SECOND);
#elif MG_ARCH == MG_ARCH_TIRTOS
  return (uint64_t) Clock_getTicks();
#elif MG_ARCH == MG_ARCH_ZEPHYR
  return (uint64_t) k_uptime_get();
#elif MG_ARCH == MG_ARCH_CMSIS_RTOS1
  return (uint64_t) rt_time_get();
#elif MG_ARCH == MG_ARCH_CMSIS_RTOS2
  return (uint64_t) ((osKernelGetTickCount() * 1000) / osKernelGetTickFreq());
#elif MG_ARCH == MG_ARCH_RTTHREAD
  return (uint64_t) ((rt_tick_get() * 1000) / RT_TICK_PER_SECOND);
#elif MG_ARCH == MG_ARCH_UNIX && defined(__APPLE__)
  // Apple CLOCK_MONOTONIC_RAW is equivalent to CLOCK_BOOTTIME on linux
  // Apple CLOCK_UPTIME_RAW is equivalent to CLOCK_MONOTONIC_RAW on linux
  return clock_gettime_nsec_np(CLOCK_UPTIME_RAW) / 1000000;
#elif MG_ARCH == MG_ARCH_UNIX
  struct timespec ts = {0, 0};
  // See #1615 - prefer monotonic clock
#if defined(CLOCK_MONOTONIC_RAW)
  // Raw hardware-based time that is not subject to NTP adjustment
  clock_gettime(CLOCK_MONOTONIC_RAW, &ts);
#elif defined(CLOCK_MONOTONIC)
  // Affected by the incremental adjustments performed by adjtime and NTP
  clock_gettime(CLOCK_MONOTONIC, &ts);
#else
  // Affected by discontinuous jumps in the system time and by the incremental
  // adjustments performed by adjtime and NTP
  clock_gettime(CLOCK_REALTIME, &ts);
#endif
  return ((uint64_t) ts.tv_sec * 1000 + (uint64_t) ts.tv_nsec / 1000000);
#elif defined(ARDUINO)
  return (uint64_t) millis();
#else
  return (uint64_t) (time(NULL) * 1000);
#endif
}
#endif

#ifdef MG_ENABLE_LINES
#line 1 "src/ws.c"
#endif











struct ws_msg {
  uint8_t flags;
  size_t header_len;
  size_t data_len;
};

size_t mg_ws_vprintf(struct mg_connection *c, int op, const char *fmt,
                     va_list *ap) {
  size_t len = c->send.len;
  size_t n = mg_vxprintf(mg_pfn_iobuf, &c->send, fmt, ap);
  mg_ws_wrap(c, c->send.len - len, op);
  return n;
}

size_t mg_ws_printf(struct mg_connection *c, int op, const char *fmt, ...) {
  size_t len = 0;
  va_list ap;
  va_start(ap, fmt);
  len = mg_ws_vprintf(c, op, fmt, &ap);
  va_end(ap);
  return len;
}

static void ws_handshake(struct mg_connection *c, const struct mg_str *wskey,
                         const struct mg_str *wsproto, const char *fmt,
                         va_list *ap) {
  const char *magic = "258EAFA5-E914-47DA-95CA-C5AB0DC85B11";
  unsigned char sha[20], b64_sha[30];

  mg_sha1_ctx sha_ctx;
  mg_sha1_init(&sha_ctx);
  mg_sha1_update(&sha_ctx, (unsigned char *) wskey->ptr, wskey->len);
  mg_sha1_update(&sha_ctx, (unsigned char *) magic, 36);
  mg_sha1_final(sha, &sha_ctx);
  mg_base64_encode(sha, sizeof(sha), (char *) b64_sha, sizeof(b64_sha));
  mg_xprintf(mg_pfn_iobuf, &c->send,
             "HTTP/1.1 101 Switching Protocols\r\n"
             "Upgrade: websocket\r\n"
             "Connection: Upgrade\r\n"
             "Sec-WebSocket-Accept: %s\r\n",
             b64_sha);
  if (fmt != NULL) mg_vxprintf(mg_pfn_iobuf, &c->send, fmt, ap);
  if (wsproto != NULL) {
    mg_printf(c, "Sec-WebSocket-Protocol: %.*s\r\n", (int) wsproto->len,
              wsproto->ptr);
  }
  mg_send(c, "\r\n", 2);
}

static uint32_t be32(const uint8_t *p) {
  return (((uint32_t) p[3]) << 0) | (((uint32_t) p[2]) << 8) |
         (((uint32_t) p[1]) << 16) | (((uint32_t) p[0]) << 24);
}

static size_t ws_process(uint8_t *buf, size_t len, struct ws_msg *msg) {
  size_t i, n = 0, mask_len = 0;
  memset(msg, 0, sizeof(*msg));
  if (len >= 2) {
    n = buf[1] & 0x7f;                // Frame length
    mask_len = buf[1] & 128 ? 4 : 0;  // last bit is a mask bit
    msg->flags = buf[0];
    if (n < 126 && len >= mask_len) {
      msg->data_len = n;
      msg->header_len = 2 + mask_len;
    } else if (n == 126 && len >= 4 + mask_len) {
      msg->header_len = 4 + mask_len;
      msg->data_len = (((size_t) buf[2]) << 8) | buf[3];
    } else if (len >= 10 + mask_len) {
      msg->header_len = 10 + mask_len;
      msg->data_len =
          (size_t) (((uint64_t) be32(buf + 2) << 32) + be32(buf + 6));
    }
  }
  // Sanity check, and integer overflow protection for the boundary check below
  // data_len should not be larger than 1 Gb
  if (msg->data_len > 1024 * 1024 * 1024) return 0;
  if (msg->header_len + msg->data_len > len) return 0;
  if (mask_len > 0) {
    uint8_t *p = buf + msg->header_len, *m = p - mask_len;
    for (i = 0; i < msg->data_len; i++) p[i] ^= m[i & 3];
  }
  return msg->header_len + msg->data_len;
}

static size_t mkhdr(size_t len, int op, bool is_client, uint8_t *buf) {
  size_t n = 0;
  buf[0] = (uint8_t) (op | 128);
  if (len < 126) {
    buf[1] = (unsigned char) len;
    n = 2;
  } else if (len < 65536) {
    uint16_t tmp = mg_htons((uint16_t) len);
    buf[1] = 126;
    memcpy(&buf[2], &tmp, sizeof(tmp));
    n = 4;
  } else {
    uint32_t tmp;
    buf[1] = 127;
    tmp = mg_htonl((uint32_t) (((uint64_t) len) >> 32));
    memcpy(&buf[2], &tmp, sizeof(tmp));
    tmp = mg_htonl((uint32_t) (len & 0xffffffffU));
    memcpy(&buf[6], &tmp, sizeof(tmp));
    n = 10;
  }
  if (is_client) {
    buf[1] |= 1 << 7;  // Set masking flag
    mg_random(&buf[n], 4);
    n += 4;
  }
  return n;
}

static void mg_ws_mask(struct mg_connection *c, size_t len) {
  if (c->is_client && c->send.buf != NULL) {
    size_t i;
    uint8_t *p = c->send.buf + c->send.len - len, *mask = p - 4;
    for (i = 0; i < len; i++) p[i] ^= mask[i & 3];
  }
}

size_t mg_ws_send(struct mg_connection *c, const void *buf, size_t len,
                  int op) {
  uint8_t header[14];
  size_t header_len = mkhdr(len, op, c->is_client, header);
  mg_send(c, header, header_len);
  MG_VERBOSE(("WS out: %d [%.*s]", (int) len, (int) len, buf));
  mg_send(c, buf, len);
  mg_ws_mask(c, len);
  return header_len + len;
}

static bool mg_ws_client_handshake(struct mg_connection *c) {
  int n = mg_http_get_request_len(c->recv.buf, c->recv.len);
  if (n < 0) {
    mg_error(c, "not http");  // Some just, not an HTTP request
  } else if (n > 0) {
    if (n < 15 || memcmp(c->recv.buf + 9, "101", 3) != 0) {
      mg_error(c, "ws handshake error");
    } else {
      struct mg_http_message hm;
      if (mg_http_parse((char *) c->recv.buf, c->recv.len, &hm)) {
        c->is_websocket = 1;
        mg_call(c, MG_EV_WS_OPEN, &hm);
      } else {
        mg_error(c, "ws handshake error");
      }
    }
    mg_iobuf_del(&c->recv, 0, (size_t) n);
  } else {
    return true;  // Request is not yet received, quit event handler
  }
  return false;  // Continue event handler
}

static void mg_ws_cb(struct mg_connection *c, int ev, void *ev_data,
                     void *fn_data) {
  struct ws_msg msg;
  size_t ofs = (size_t) c->pfn_data;

  // assert(ofs < c->recv.len);
  if (ev == MG_EV_READ) {
    if (c->is_client && !c->is_websocket && mg_ws_client_handshake(c)) return;

    while (ws_process(c->recv.buf + ofs, c->recv.len - ofs, &msg) > 0) {
      char *s = (char *) c->recv.buf + ofs + msg.header_len;
      struct mg_ws_message m = {{s, msg.data_len}, msg.flags};
      size_t len = msg.header_len + msg.data_len;
      uint8_t final = msg.flags & 128, op = msg.flags & 15;
      // MG_VERBOSE ("fin %d op %d len %d [%.*s]", final, op,
      //                       (int) m.data.len, (int) m.data.len, m.data.ptr));
      switch (op) {
        case WEBSOCKET_OP_CONTINUE:
          mg_call(c, MG_EV_WS_CTL, &m);
          break;
        case WEBSOCKET_OP_PING:
          MG_DEBUG(("%s", "WS PONG"));
          mg_ws_send(c, s, msg.data_len, WEBSOCKET_OP_PONG);
          mg_call(c, MG_EV_WS_CTL, &m);
          break;
        case WEBSOCKET_OP_PONG:
          mg_call(c, MG_EV_WS_CTL, &m);
          break;
        case WEBSOCKET_OP_TEXT:
        case WEBSOCKET_OP_BINARY:
          if (final) mg_call(c, MG_EV_WS_MSG, &m);
          break;
        case WEBSOCKET_OP_CLOSE:
          MG_DEBUG(("%lu WS CLOSE", c->id));
          mg_call(c, MG_EV_WS_CTL, &m);
          // Echo the payload of the received CLOSE message back to the sender
          mg_ws_send(c, m.data.ptr, m.data.len, WEBSOCKET_OP_CLOSE);
          c->is_draining = 1;
          break;
        default:
          // Per RFC6455, close conn when an unknown op is recvd
          mg_error(c, "unknown WS op %d", op);
          break;
      }

      // Handle fragmented frames: strip header, keep in c->recv
      if (final == 0 || op == 0) {
        if (op) ofs++, len--, msg.header_len--;       // First frame
        mg_iobuf_del(&c->recv, ofs, msg.header_len);  // Strip header
        len -= msg.header_len;
        ofs += len;
        c->pfn_data = (void *) ofs;
        // MG_INFO(("FRAG %d [%.*s]", (int) ofs, (int) ofs, c->recv.buf));
      }
      // Remove non-fragmented frame
      if (final && op) mg_iobuf_del(&c->recv, ofs, len);
      // Last chunk of the fragmented frame
      if (final && !op) {
        m.flags = c->recv.buf[0];
        m.data = mg_str_n((char *) &c->recv.buf[1], (size_t) (ofs - 1));
        mg_call(c, MG_EV_WS_MSG, &m);
        mg_iobuf_del(&c->recv, 0, ofs);
        ofs = 0;
        c->pfn_data = NULL;
      }
    }
  }
  (void) fn_data;
  (void) ev_data;
}

struct mg_connection *mg_ws_connect(struct mg_mgr *mgr, const char *url,
                                    mg_event_handler_t fn, void *fn_data,
                                    const char *fmt, ...) {
  struct mg_connection *c = mg_connect(mgr, url, fn, fn_data);
  if (c != NULL) {
    char nonce[16], key[30];
    struct mg_str host = mg_url_host(url);
    mg_random(nonce, sizeof(nonce));
    mg_base64_encode((unsigned char *) nonce, sizeof(nonce), key, sizeof(key));
    mg_xprintf(mg_pfn_iobuf, &c->send,
               "GET %s HTTP/1.1\r\n"
               "Upgrade: websocket\r\n"
               "Host: %.*s\r\n"
               "Connection: Upgrade\r\n"
               "Sec-WebSocket-Version: 13\r\n"
               "Sec-WebSocket-Key: %s\r\n",
               mg_url_uri(url), (int) host.len, host.ptr, key);
    if (fmt != NULL) {
      va_list ap;
      va_start(ap, fmt);
      mg_vxprintf(mg_pfn_iobuf, &c->send, fmt, &ap);
      va_end(ap);
    }
    mg_xprintf(mg_pfn_iobuf, &c->send, "\r\n");
    c->pfn = mg_ws_cb;
    c->pfn_data = NULL;
  }
  return c;
}

void mg_ws_upgrade(struct mg_connection *c, struct mg_http_message *hm,
                   const char *fmt, ...) {
  struct mg_str *wskey = mg_http_get_header(hm, "Sec-WebSocket-Key");
  c->pfn = mg_ws_cb;
  c->pfn_data = NULL;
  if (wskey == NULL) {
    mg_http_reply(c, 426, "", "WS upgrade expected\n");
    c->is_draining = 1;
  } else {
    struct mg_str *wsproto = mg_http_get_header(hm, "Sec-WebSocket-Protocol");
    va_list ap;
    va_start(ap, fmt);
    ws_handshake(c, wskey, wsproto, fmt, &ap);
    va_end(ap);
    c->is_websocket = 1;
    c->is_resp = 0;
    mg_call(c, MG_EV_WS_OPEN, hm);
  }
}

size_t mg_ws_wrap(struct mg_connection *c, size_t len, int op) {
  uint8_t header[14], *p;
  size_t header_len = mkhdr(len, op, c->is_client, header);

  // NOTE: order of operations is important!
  mg_iobuf_add(&c->send, c->send.len, NULL, header_len);
  p = &c->send.buf[c->send.len - len];         // p points to data
  memmove(p, p - header_len, len);             // Shift data
  memcpy(p - header_len, header, header_len);  // Prepend header
  mg_ws_mask(c, len);                          // Mask data

  return c->send.len;
}

#ifdef MG_ENABLE_LINES
#line 1 "src/drivers/rt1020.c"
#endif


#if MG_ENABLE_TCPIP && defined(MG_ENABLE_DRIVER_RT1020) && \
    MG_ENABLE_DRIVER_RT1020
struct rt1020_enet {
  volatile uint32_t RESERVED0, EIR, EIMR, RESERVED1, RDAR, TDAR, RESERVED2[3],
      ECR, RESERVED3[6], MMFR, MSCR, RESERVED4[7], MIBC, RESERVED5[7], RCR,
      RESERVED6[15], TCR, RESERVED7[7], PALR, PAUR, OPD, TXIC0, TXIC1, TXIC2,
      RESERVED8, RXIC0, RXIC1, RXIC2, RESERVED9[3], IAUR, IALR, GAUR, GALR,
      RESERVED10[7], TFWR, RESERVED11[14], RDSR, TDSR, MRBR[2], RSFL, RSEM,
      RAEM, RAFL, TSEM, TAEM, TAFL, TIPG, FTRL, RESERVED12[3], TACC, RACC,
      RESERVED13[15], RMON_T_PACKETS, RMON_T_BC_PKT, RMON_T_MC_PKT,
      RMON_T_CRC_ALIGN, RMON_T_UNDERSIZE, RMON_T_OVERSIZE, RMON_T_FRAG,
      RMON_T_JAB, RMON_T_COL, RMON_T_P64, RMON_T_P65TO127, RMON_T_P128TO255,
      RMON_T_P256TO511, RMON_T_P512TO1023, RMON_T_P1024TO2048, RMON_T_GTE2048,
      RMON_T_OCTETS, IEEE_T_DROP, IEEE_T_FRAME_OK, IEEE_T_1COL, IEEE_T_MCOL,
      IEEE_T_DEF, IEEE_T_LCOL, IEEE_T_EXCOL, IEEE_T_MACERR, IEEE_T_CSERR,
      IEEE_T_SQE, IEEE_T_FDXFC, IEEE_T_OCTETS_OK, RESERVED14[3], RMON_R_PACKETS,
      RMON_R_BC_PKT, RMON_R_MC_PKT, RMON_R_CRC_ALIGN, RMON_R_UNDERSIZE,
      RMON_R_OVERSIZE, RMON_R_FRAG, RMON_R_JAB, RESERVED15, RMON_R_P64,
      RMON_R_P65TO127, RMON_R_P128TO255, RMON_R_P256TO511, RMON_R_P512TO1023,
      RMON_R_P1024TO2047, RMON_R_GTE2048, RMON_R_OCTETS, IEEE_R_DROP,
      IEEE_R_FRAME_OK, IEEE_R_CRC, IEEE_R_ALIGN, IEEE_R_MACERR, IEEE_R_FDXFC,
      IEEE_R_OCTETS_OK, RESERVED16[71], ATCR, ATVR, ATOFF, ATPER, ATCOR, ATINC,
      ATSTMP, RESERVED17[122], TGSR, TCSR0, TCCR0, TCSR1, TCCR1, TCSR2, TCCR2,
      TCSR3;
};

#undef ENET
#define ENET ((struct rt1020_enet *) (uintptr_t) 0x402D8000u)

#undef BIT
#define BIT(x) ((uint32_t) 1 << (x))

// Max frame size, every buffer must be 64-bit aligned (1536 = 0x600)
#define ETH_PKT_SIZE 1536
#define ETH_DESC_CNT 4  // Descriptors count

typedef struct {
  uint16_t length;   // Data length
  uint16_t control;  // Control and status
  uint32_t *buffer;  // Data ptr
} enet_bd_t;

// TODO(): handle these in a portable compiler-independent CMSIS-friendly way
// Descriptors: in non-cached area (TODO(scaprile)), 64-bit aligned
enet_bd_t s_rxdesc[ETH_DESC_CNT] __attribute__((aligned((64U))));
enet_bd_t s_txdesc[ETH_DESC_CNT] __attribute__((aligned((64U))));
// Buffers: 64-bit aligned
uint8_t s_rxbuf[ETH_DESC_CNT][ETH_PKT_SIZE] __attribute__((aligned((64U))));
uint8_t s_txbuf[ETH_DESC_CNT][ETH_PKT_SIZE] __attribute__((aligned((64U))));

static struct mg_tcpip_if *s_ifp;  // MIP interface

enum { PHY_ADDR = 2, PHY_BCR = 0, PHY_BSR = 1, PHY_PC1R = 30, PHY_PC2R = 31 };

static uint32_t eth_read_phy(uint8_t addr, uint8_t reg) {
  ENET->EIR |= BIT(23);  // MII interrupt clear
  ENET->MMFR = (1 << 30) | (2 << 28) | (addr << 23) | (reg << 18) | (2 << 16);
  while ((ENET->EIR & BIT(23)) == 0) (void) 0;
  return ENET->MMFR & 0xffff;
}

static void eth_write_phy(uint8_t addr, uint8_t reg, uint32_t val) {
  ENET->EIR |= BIT(23);  // MII interrupt clear
  ENET->MMFR =
      (1 << 30) | (1 << 28) | (addr << 23) | (reg << 18) | (2 << 16) | val;
  while ((ENET->EIR & BIT(23)) == 0) (void) 0;
}

//  MDC clock is generated from IPS Bus clock (ipg_clk); as per 802.3,
//  it must not exceed 2.5MHz
// The PHY receives the PLL6-generated 50MHz clock
static bool mg_tcpip_driver_rt1020_init(struct mg_tcpip_if *ifp) {
  struct mg_tcpip_driver_rt1020_data *d =
      (struct mg_tcpip_driver_rt1020_data *) ifp->driver_data;
  s_ifp = ifp;

  // Init RX descriptors
  for (int i = 0; i < ETH_DESC_CNT; i++) {
    s_rxdesc[i].control = BIT(15);                 // Own (E)
    s_rxdesc[i].buffer = (uint32_t *) s_rxbuf[i];  // Point to data buffer
  }
  s_rxdesc[ETH_DESC_CNT - 1].control |= BIT(13);  // Wrap last descriptor

  // Init TX descriptors
  for (int i = 0; i < ETH_DESC_CNT; i++) {
    s_txdesc[i].control = BIT(10);  // Own (TC)
    s_txdesc[i].buffer = (uint32_t *) s_txbuf[i];
  }
  s_txdesc[ETH_DESC_CNT - 1].control |= BIT(13);  // Wrap last descriptor

  ENET->ECR = BIT(0);                     // Software reset, disable
  while ((ENET->ECR & BIT(0))) (void) 0;  // Wait until done

  // Set MDC clock divider. If user told us the value, use it.
  // TODO(): Otherwise, guess (currently assuming max freq)
  int cr = (d == NULL || d->mdc_cr < 0) ? 24 : d->mdc_cr;
  ENET->MSCR = (1 << 8) | ((cr & 0x3f) << 1);  // HOLDTIME 2 clks

  eth_write_phy(PHY_ADDR, PHY_BCR, BIT(15));  // Reset PHY
  eth_write_phy(PHY_ADDR, PHY_BCR, BIT(12));  // Set autonegotiation
  // PHY: Enable 50 MHz external ref clock at XI (preserve defaults)
  eth_write_phy(PHY_ADDR, PHY_PC2R, BIT(15) | BIT(8) | BIT(7));
  // Select RMII mode, 100M, keep CRC, set max rx length, disable loop
  ENET->RCR = (1518 << 16) | BIT(8) | BIT(2);
  // ENET->RCR |= BIT(3);     // Receive all
  ENET->TCR = BIT(2);  // Full-duplex
  ENET->RDSR = (uint32_t) (uintptr_t) s_rxdesc;
  ENET->TDSR = (uint32_t) (uintptr_t) s_txdesc;
  ENET->MRBR[0] = ETH_PKT_SIZE;  // Same size for RX/TX buffers
  // MAC address filtering (bytes in reversed order)
  ENET->PAUR = ((uint32_t) ifp->mac[4] << 24U) | (uint32_t) ifp->mac[5] << 16U;
  ENET->PALR = (uint32_t) (ifp->mac[0] << 24U) |
               ((uint32_t) ifp->mac[1] << 16U) |
               ((uint32_t) ifp->mac[2] << 8U) | ifp->mac[3];
  ENET->ECR = BIT(8) | BIT(1);  // Little-endian CPU, Enable
  ENET->EIMR = BIT(25);         // Set interrupt mask
  ENET->RDAR = BIT(24);         // Receive Descriptors have changed
  return true;
}

// Transmit frame
static uint32_t s_txno;

static size_t mg_tcpip_driver_rt1020_tx(const void *buf, size_t len,
                                        struct mg_tcpip_if *ifp) {
  if (len > sizeof(s_txbuf[ETH_DESC_CNT])) {
    MG_ERROR(("Frame too big, %ld", (long) len));
    len = 0;  // fail
  } else if ((s_txdesc[s_txno].control & BIT(15))) {
    ifp->nerr++;
    MG_ERROR(("No descriptors available"));
    len = 0;  // fail
  } else {
    memcpy(s_txbuf[s_txno], buf, len);         // Copy data
    s_txdesc[s_txno].length = (uint16_t) len;  // Set data len
    // Table 37-34, R, L, TC (Ready, last, transmit CRC after frame
    s_txdesc[s_txno].control |= (uint16_t) (BIT(15) | BIT(11) | BIT(10));
    ENET->TDAR = BIT(24);  // Descriptor ring updated
    if (++s_txno >= ETH_DESC_CNT) s_txno = 0;
  }
  (void) ifp;
  return len;
}

static bool mg_tcpip_driver_rt1020_up(struct mg_tcpip_if *ifp) {
  uint32_t bsr = eth_read_phy(PHY_ADDR, PHY_BSR);
  bool up = bsr & BIT(2) ? 1 : 0;
  if ((ifp->state == MG_TCPIP_STATE_DOWN) && up) {  // link state just went up
    uint32_t pc1r = eth_read_phy(PHY_ADDR, PHY_PC1R);
    uint32_t tcr = ENET->TCR |= BIT(2);        // Full-duplex
    uint32_t rcr = ENET->RCR &= ~BIT(9);       // 100M
    if ((pc1r & 3) == 1) rcr |= BIT(9);        // 10M
    if ((pc1r & BIT(2)) == 0) tcr &= ~BIT(2);  // Half-duplex
    ENET->TCR = tcr;  // IRQ handler does not fiddle with these registers
    ENET->RCR = rcr;
    MG_DEBUG(("Link is %uM %s-duplex", rcr & BIT(9) ? 10 : 100,
              tcr & BIT(2) ? "full" : "half"));
  }
  return up;
}

void ENET_IRQHandler(void);
static uint32_t s_rxno;
void ENET_IRQHandler(void) {
  ENET->EIR = BIT(25);  // Ack IRQ
  // Frame received, loop
  for (uint32_t i = 0; i < 10; i++) {  // read as they arrive but not forever
    if (s_rxdesc[s_rxno].control & BIT(15)) break;  // exit when done
    // skip partial/errored frames (Table 37-32)
    if ((s_rxdesc[s_rxno].control & BIT(11)) &&
        !(s_rxdesc[s_rxno].control &
          (BIT(5) | BIT(4) | BIT(2) | BIT(1) | BIT(0)))) {
      uint32_t len = (s_rxdesc[s_rxno].length);
      mg_tcpip_qwrite(s_rxbuf[s_rxno], len > 4 ? len - 4 : len, s_ifp);
    }
    s_rxdesc[s_rxno].control |= BIT(15);
    if (++s_rxno >= ETH_DESC_CNT) s_rxno = 0;
  }
  ENET->RDAR = BIT(24);  // Receive Descriptors have changed
  // If b24 == 0, descriptors were exhausted and probably frames were dropped
}

struct mg_tcpip_driver mg_tcpip_driver_rt1020 = {
    mg_tcpip_driver_rt1020_init, mg_tcpip_driver_rt1020_tx, NULL,
    mg_tcpip_driver_rt1020_up};

#endif

#ifdef MG_ENABLE_LINES
#line 1 "src/drivers/same54.c"
#endif


#if MG_ENABLE_TCPIP && defined(MG_ENABLE_DRIVER_SAME54) && \
    MG_ENABLE_DRIVER_SAME54

#include <sam.h>

#undef BIT
#define BIT(x) ((uint32_t) 1 << (x))
#define ETH_PKT_SIZE 1536  // Max frame size
#define ETH_DESC_CNT 4     // Descriptors count
#define ETH_DS 2           // Descriptor size (words)

static uint8_t s_rxbuf[ETH_DESC_CNT][ETH_PKT_SIZE];
static uint8_t s_txbuf[ETH_DESC_CNT][ETH_PKT_SIZE];
static uint32_t s_rxdesc[ETH_DESC_CNT][ETH_DS];  // RX descriptors
static uint32_t s_txdesc[ETH_DESC_CNT][ETH_DS];  // TX descriptors
static uint8_t s_txno;                           // Current TX descriptor
static uint8_t s_rxno;                           // Current RX descriptor

static struct mg_tcpip_if *s_ifp;  // MIP interface
enum { PHY_ADDR = 0, PHY_BCR = 0, PHY_BSR = 1 };

#define PHY_BCR_DUPLEX_MODE_Msk BIT(8)
#define PHY_BCR_SPEED_Msk BIT(13)
#define PHY_BSR_LINK_STATUS_Msk BIT(2)

static uint16_t eth_read_phy(uint8_t addr, uint8_t reg) {
  GMAC_REGS->GMAC_MAN = GMAC_MAN_CLTTO_Msk |
                        GMAC_MAN_OP(2) |  // Setting the read operation
                        GMAC_MAN_WTN(2) | GMAC_MAN_PHYA(addr) |  // PHY address
                        GMAC_MAN_REGA(reg);  // Setting the register
  while (!(GMAC_REGS->GMAC_NSR & GMAC_NSR_IDLE_Msk)) (void) 0;
  return GMAC_REGS->GMAC_MAN & GMAC_MAN_DATA_Msk;  // Getting the read value
}

#if 0
static void eth_write_phy(uint8_t addr, uint8_t reg, uint16_t val) {
  GMAC_REGS->GMAC_MAN = GMAC_MAN_CLTTO_Msk | GMAC_MAN_OP(1) |   // Setting the write operation
                        GMAC_MAN_WTN(2) | GMAC_MAN_PHYA(addr) | // PHY address
                        GMAC_MAN_REGA(reg) | GMAC_MAN_DATA(val);  // Setting the register
  while (!(GMAC_REGS->GMAC_NSR & GMAC_NSR_IDLE_Msk)); // Waiting until the write op is complete
}
#endif

int get_clock_rate(struct mg_tcpip_driver_same54_data *d) {
  if (d && d->mdc_cr >= 0 && d->mdc_cr <= 5) {
    return d->mdc_cr;
  } else {
    // get MCLK from GCLK_GENERATOR 0
    uint32_t div = 512;
    uint32_t mclk;
    if (!(GCLK_REGS->GCLK_GENCTRL[0] & GCLK_GENCTRL_DIVSEL_Msk)) {
      div = ((GCLK_REGS->GCLK_GENCTRL[0] & 0x00FF0000) >> 16);
      if (div == 0) div = 1;
    }
    switch (GCLK_REGS->GCLK_GENCTRL[0] & GCLK_GENCTRL_SRC_Msk) {
      case GCLK_GENCTRL_SRC_XOSC0_Val:
        mclk = 32000000UL; /* 32MHz */
        break;
      case GCLK_GENCTRL_SRC_XOSC1_Val:
        mclk = 32000000UL; /* 32MHz */
        break;
      case GCLK_GENCTRL_SRC_OSCULP32K_Val:
        mclk = 32000UL;
        break;
      case GCLK_GENCTRL_SRC_XOSC32K_Val:
        mclk = 32000UL;
        break;
      case GCLK_GENCTRL_SRC_DFLL_Val:
        mclk = 48000000UL; /* 48MHz */
        break;
      case GCLK_GENCTRL_SRC_DPLL0_Val:
        mclk = 200000000UL; /* 200MHz */
        break;
      case GCLK_GENCTRL_SRC_DPLL1_Val:
        mclk = 200000000UL; /* 200MHz */
        break;
      default:
        mclk = 200000000UL; /* 200MHz */
    }

    mclk /= div;
    uint8_t crs[] = {0, 1, 2, 3, 4, 5};          // GMAC->NCFGR::CLK values
    uint8_t dividers[] = {8, 16, 32, 48, 64, 128};  // Respective CLK dividers
    for (int i = 0; i < 6; i++) {
      if (mclk / dividers[i] <= 2375000UL /* 2.5MHz - 5% */) {
        return crs[i];
      }
    }

    return 5;
  }
}

static bool mg_tcpip_driver_same54_init(struct mg_tcpip_if *ifp) {
  struct mg_tcpip_driver_same54_data *d =
      (struct mg_tcpip_driver_same54_data *) ifp->driver_data;
  s_ifp = ifp;

  MCLK_REGS->MCLK_APBCMASK |= MCLK_APBCMASK_GMAC_Msk;
  MCLK_REGS->MCLK_AHBMASK |= MCLK_AHBMASK_GMAC_Msk;
  GMAC_REGS->GMAC_NCFGR = GMAC_NCFGR_CLK(get_clock_rate(d));  // Set MDC divider
  GMAC_REGS->GMAC_NCR = 0;                            // Disable RX & TX
  GMAC_REGS->GMAC_NCR |= GMAC_NCR_MPE_Msk;            // Enable MDC & MDIO

  for (int i = 0; i < ETH_DESC_CNT; i++) {   // Init TX descriptors
    s_txdesc[i][0] = (uint32_t) s_txbuf[i];  // Point to data buffer
    s_txdesc[i][1] = BIT(31);                // OWN bit
  }
  s_txdesc[ETH_DESC_CNT - 1][1] |= BIT(30);  // Last tx descriptor - wrap

  GMAC_REGS->GMAC_DCFGR = GMAC_DCFGR_DRBS(0x18);  // DMA recv buf 1536
  for (int i = 0; i < ETH_DESC_CNT; i++) {        // Init RX descriptors
    s_rxdesc[i][0] = (uint32_t) s_rxbuf[i];       // Address of the data buffer
    s_rxdesc[i][1] = 0;                           // Clear status
  }
  s_rxdesc[ETH_DESC_CNT - 1][0] |= BIT(1);  // Last rx descriptor - wrap

  GMAC_REGS->GMAC_TBQB = (uint32_t) s_txdesc;  // about the descriptor addresses
  GMAC_REGS->GMAC_RBQB = (uint32_t) s_rxdesc;  // Let the controller know

  GMAC_REGS->SA[0].GMAC_SAB =
      MG_U32(ifp->mac[3], ifp->mac[2], ifp->mac[1], ifp->mac[0]);
  GMAC_REGS->SA[0].GMAC_SAT = MG_U32(0, 0, ifp->mac[5], ifp->mac[4]);

  GMAC_REGS->GMAC_UR &= ~GMAC_UR_MII_Msk;  // Disable MII, use RMII
  GMAC_REGS->GMAC_NCFGR |= GMAC_NCFGR_MAXFS_Msk | GMAC_NCFGR_MTIHEN_Msk |
                           GMAC_NCFGR_EFRHD_Msk | GMAC_NCFGR_CAF_Msk;
  GMAC_REGS->GMAC_TSR = GMAC_TSR_HRESP_Msk | GMAC_TSR_UND_Msk |
                        GMAC_TSR_TXCOMP_Msk | GMAC_TSR_TFC_Msk |
                        GMAC_TSR_TXGO_Msk | GMAC_TSR_RLE_Msk |
                        GMAC_TSR_COL_Msk | GMAC_TSR_UBR_Msk;
  GMAC_REGS->GMAC_RSR = GMAC_RSR_HNO_Msk | GMAC_RSR_RXOVR_Msk |
                        GMAC_RSR_REC_Msk | GMAC_RSR_BNA_Msk;
  GMAC_REGS->GMAC_IDR = ~0U;  // Disable interrupts, then enable required
  GMAC_REGS->GMAC_IER = GMAC_IER_HRESP_Msk | GMAC_IER_ROVR_Msk |
                        GMAC_IER_TCOMP_Msk | GMAC_IER_TFC_Msk |
                        GMAC_IER_RLEX_Msk | GMAC_IER_TUR_Msk |
                        GMAC_IER_RXUBR_Msk | GMAC_IER_RCOMP_Msk;
  GMAC_REGS->GMAC_NCR |= GMAC_NCR_TXEN_Msk | GMAC_NCR_RXEN_Msk;
  NVIC_EnableIRQ(GMAC_IRQn);

  return true;
}

static size_t mg_tcpip_driver_same54_tx(const void *buf, size_t len,
                                        struct mg_tcpip_if *ifp) {
  if (len > sizeof(s_txbuf[s_txno])) {
    MG_ERROR(("Frame too big, %ld", (long) len));
    len = 0;  // Frame is too big
  } else if ((s_txdesc[s_txno][1] & BIT(31)) == 0) {
    ifp->nerr++;
    MG_ERROR(("No free descriptors"));
    len = 0;  // All descriptors are busy, fail
  } else {
    uint32_t status = len | BIT(15);  // Frame length, last chunk
    if (s_txno == ETH_DESC_CNT - 1) status |= BIT(30);  // wrap
    memcpy(s_txbuf[s_txno], buf, len);                  // Copy data
    s_txdesc[s_txno][1] = status;
    if (++s_txno >= ETH_DESC_CNT) s_txno = 0;
  }
  __DSB();  // Ensure descriptors have been written
  GMAC_REGS->GMAC_NCR |= GMAC_NCR_TSTART_Msk;  // Enable transmission
  return len;
}

static bool mg_tcpip_driver_same54_up(struct mg_tcpip_if *ifp) {
  uint16_t bsr = eth_read_phy(PHY_ADDR, PHY_BSR);
  bool up = bsr & PHY_BSR_LINK_STATUS_Msk ? 1 : 0;

  // If PHY is ready, update NCFGR accordingly
  if (ifp->state == MG_TCPIP_STATE_DOWN && up) {
    uint16_t bcr = eth_read_phy(PHY_ADDR, PHY_BCR);
    bool fd = bcr & PHY_BCR_DUPLEX_MODE_Msk ? 1 : 0;
    bool spd = bcr & PHY_BCR_SPEED_Msk ? 1 : 0;
    GMAC_REGS->GMAC_NCFGR |= GMAC_NCFGR_SPD(spd) | GMAC_NCFGR_FD(fd);
  }

  return up;
}

void GMAC_Handler(void);
void GMAC_Handler(void) {
  uint32_t isr = GMAC_REGS->GMAC_ISR;
  uint32_t rsr = GMAC_REGS->GMAC_RSR;
  uint32_t tsr = GMAC_REGS->GMAC_TSR;
  if (isr & GMAC_ISR_RCOMP_Msk) {
    if (rsr & GMAC_ISR_RCOMP_Msk) {
      for (uint8_t i = 0; i < ETH_DESC_CNT; i++) {
        if ((s_rxdesc[s_rxno][0] & BIT(0)) == 0) break;
        size_t len = s_rxdesc[s_rxno][1] & (BIT(13) - 1);
        mg_tcpip_qwrite(s_rxbuf[s_rxno], len, s_ifp);
        s_rxdesc[s_rxno][0] &= ~BIT(0);  // Disown
        if (++s_rxno >= ETH_DESC_CNT) s_rxno = 0;
      }
    }
  }

  if ((tsr & (GMAC_TSR_HRESP_Msk | GMAC_TSR_UND_Msk | GMAC_TSR_TXCOMP_Msk |
              GMAC_TSR_TFC_Msk | GMAC_TSR_TXGO_Msk | GMAC_TSR_RLE_Msk |
              GMAC_TSR_COL_Msk | GMAC_TSR_UBR_Msk)) != 0) {
    // MG_INFO((" --> %#x %#x", s_txdesc[s_txno][1], tsr));
    if (!(s_txdesc[s_txno][1] & BIT(31))) s_txdesc[s_txno][1] |= BIT(31);
  }

  GMAC_REGS->GMAC_RSR = rsr;
  GMAC_REGS->GMAC_TSR = tsr;
}

struct mg_tcpip_driver mg_tcpip_driver_same54 = {
    mg_tcpip_driver_same54_init, mg_tcpip_driver_same54_tx, NULL,
    mg_tcpip_driver_same54_up};
#endif

#ifdef MG_ENABLE_LINES
#line 1 "src/drivers/stm32.c"
#endif


#if MG_ENABLE_TCPIP && defined(MG_ENABLE_DRIVER_STM32) && MG_ENABLE_DRIVER_STM32
struct stm32_eth {
  volatile uint32_t MACCR, MACFFR, MACHTHR, MACHTLR, MACMIIAR, MACMIIDR, MACFCR,
      MACVLANTR, RESERVED0[2], MACRWUFFR, MACPMTCSR, RESERVED1, MACDBGR, MACSR,
      MACIMR, MACA0HR, MACA0LR, MACA1HR, MACA1LR, MACA2HR, MACA2LR, MACA3HR,
      MACA3LR, RESERVED2[40], MMCCR, MMCRIR, MMCTIR, MMCRIMR, MMCTIMR,
      RESERVED3[14], MMCTGFSCCR, MMCTGFMSCCR, RESERVED4[5], MMCTGFCR,
      RESERVED5[10], MMCRFCECR, MMCRFAECR, RESERVED6[10], MMCRGUFCR,
      RESERVED7[334], PTPTSCR, PTPSSIR, PTPTSHR, PTPTSLR, PTPTSHUR, PTPTSLUR,
      PTPTSAR, PTPTTHR, PTPTTLR, RESERVED8, PTPTSSR, PTPPPSCR, RESERVED9[564],
      DMABMR, DMATPDR, DMARPDR, DMARDLAR, DMATDLAR, DMASR, DMAOMR, DMAIER,
      DMAMFBOCR, DMARSWTR, RESERVED10[8], DMACHTDR, DMACHRDR, DMACHTBAR,
      DMACHRBAR;
};
#undef ETH
#define ETH ((struct stm32_eth *) (uintptr_t) 0x40028000)

#undef DSB
#if defined(__CC_ARM)
#define DSB() __dsb(0xF)
#elif defined(__ARMCC_VERSION)
#define DSB() __builtin_arm_dsb(0xF)
#elif defined(__GNUC__) && defined(__arm__) && defined(__thumb__)
#define DSB() asm("DSB 0xF")
#elif defined(__ICCARM__)
#define DSB() __iar_builtin_DSB()
#else
#define DSB()
#endif

#undef BIT
#define BIT(x) ((uint32_t) 1 << (x))
#define ETH_PKT_SIZE 1540  // Max frame size
#define ETH_DESC_CNT 4     // Descriptors count
#define ETH_DS 4           // Descriptor size (words)

static uint32_t s_rxdesc[ETH_DESC_CNT][ETH_DS];      // RX descriptors
static uint32_t s_txdesc[ETH_DESC_CNT][ETH_DS];      // TX descriptors
static uint8_t s_rxbuf[ETH_DESC_CNT][ETH_PKT_SIZE];  // RX ethernet buffers
static uint8_t s_txbuf[ETH_DESC_CNT][ETH_PKT_SIZE];  // TX ethernet buffers
static uint8_t s_txno;                               // Current TX descriptor
static uint8_t s_rxno;                               // Current RX descriptor

static struct mg_tcpip_if *s_ifp;  // MIP interface
enum { PHY_ADDR = 0, PHY_BCR = 0, PHY_BSR = 1, PHY_CSCR = 31 };

static uint32_t eth_read_phy(uint8_t addr, uint8_t reg) {
  ETH->MACMIIAR &= (7 << 2);
  ETH->MACMIIAR |= ((uint32_t) addr << 11) | ((uint32_t) reg << 6);
  ETH->MACMIIAR |= BIT(0);
  while (ETH->MACMIIAR & BIT(0)) (void) 0;
  return ETH->MACMIIDR;
}

static void eth_write_phy(uint8_t addr, uint8_t reg, uint32_t val) {
  ETH->MACMIIDR = val;
  ETH->MACMIIAR &= (7 << 2);
  ETH->MACMIIAR |= ((uint32_t) addr << 11) | ((uint32_t) reg << 6) | BIT(1);
  ETH->MACMIIAR |= BIT(0);
  while (ETH->MACMIIAR & BIT(0)) (void) 0;
}

static uint32_t get_hclk(void) {
  struct rcc {
    volatile uint32_t CR, PLLCFGR, CFGR;
  } *rcc = (struct rcc *) 0x40023800;
  uint32_t clk = 0, hsi = 16000000 /* 16 MHz */, hse = 8000000 /* 8MHz */;

  if (rcc->CFGR & (1 << 2)) {
    clk = hse;
  } else if (rcc->CFGR & (1 << 3)) {
    uint32_t vco, m, n, p;
    m = (rcc->PLLCFGR & (0x3f << 0)) >> 0;
    n = (rcc->PLLCFGR & (0x1ff << 6)) >> 6;
    p = (((rcc->PLLCFGR & (3 << 16)) >> 16) + 1) * 2;
    clk = (rcc->PLLCFGR & (1 << 22)) ? hse : hsi;
    vco = (uint32_t) ((uint64_t) clk * n / m);
    clk = vco / p;
  } else {
    clk = hsi;
  }
  uint32_t hpre = (rcc->CFGR & (15 << 4)) >> 4;
  if (hpre < 8) return clk;

  uint8_t ahbptab[8] = {1, 2, 3, 4, 6, 7, 8, 9};  // log2(div)
  return ((uint32_t) clk) >> ahbptab[hpre - 8];
}

//  Guess CR from HCLK. MDC clock is generated from HCLK (AHB); as per 802.3,
//  it must not exceed 2.5MHz As the AHB clock can be (and usually is) derived
//  from the HSI (internal RC), and it can go above specs, the datasheets
//  specify a range of frequencies and activate one of a series of dividers to
//  keep the MDC clock safely below 2.5MHz. We guess a divider setting based on
//  HCLK with a +5% drift. If the user uses a different clock from our
//  defaults, needs to set the macros on top Valid for STM32F74xxx/75xxx
//  (38.8.1) and STM32F42xxx/43xxx (33.8.1) (both 4.5% worst case drift)
static int guess_mdc_cr(void) {
  uint8_t crs[] = {2, 3, 0, 1, 4, 5};          // ETH->MACMIIAR::CR values
  uint8_t div[] = {16, 26, 42, 62, 102, 124};  // Respective HCLK dividers
  uint32_t hclk = get_hclk();                  // Guess system HCLK
  int result = -1;                             // Invalid CR value
  if (hclk < 25000000) {
    MG_ERROR(("HCLK too low"));
  } else {
    for (int i = 0; i < 6; i++) {
      if (hclk / div[i] <= 2375000UL /* 2.5MHz - 5% */) {
        result = crs[i];
        break;
      }
    }
    if (result < 0) MG_ERROR(("HCLK too high"));
  }
  MG_DEBUG(("HCLK: %u, CR: %d", hclk, result));
  return result;
}

static bool mg_tcpip_driver_stm32_init(struct mg_tcpip_if *ifp) {
  struct mg_tcpip_driver_stm32_data *d =
      (struct mg_tcpip_driver_stm32_data *) ifp->driver_data;
  s_ifp = ifp;

  // Init RX descriptors
  for (int i = 0; i < ETH_DESC_CNT; i++) {
    s_rxdesc[i][0] = BIT(31);                            // Own
    s_rxdesc[i][1] = sizeof(s_rxbuf[i]) | BIT(14);       // 2nd address chained
    s_rxdesc[i][2] = (uint32_t) (uintptr_t) s_rxbuf[i];  // Point to data buffer
    s_rxdesc[i][3] =
        (uint32_t) (uintptr_t) s_rxdesc[(i + 1) % ETH_DESC_CNT];  // Chain
  }

  // Init TX descriptors
  for (int i = 0; i < ETH_DESC_CNT; i++) {
    s_txdesc[i][2] = (uint32_t) (uintptr_t) s_txbuf[i];  // Buf pointer
    s_txdesc[i][3] =
        (uint32_t) (uintptr_t) s_txdesc[(i + 1) % ETH_DESC_CNT];  // Chain
  }

  ETH->DMABMR |= BIT(0);                         // Software reset
  while ((ETH->DMABMR & BIT(0)) != 0) (void) 0;  // Wait until done

  // Set MDC clock divider. If user told us the value, use it. Otherwise, guess
  int cr = (d == NULL || d->mdc_cr < 0) ? guess_mdc_cr() : d->mdc_cr;
  ETH->MACMIIAR = ((uint32_t) cr & 7) << 2;

  // NOTE(cpq): we do not use extended descriptor bit 7, and do not use
  // hardware checksum. Therefore, descriptor size is 4, not 8
  // ETH->DMABMR = BIT(13) | BIT(16) | BIT(22) | BIT(23) | BIT(25);
  ETH->MACIMR = BIT(3) | BIT(9);  // Mask timestamp & PMT IT
  ETH->MACFCR = BIT(7);           // Disable zero quarta pause
  // ETH->MACFFR = BIT(31);                            // Receive all
  eth_write_phy(PHY_ADDR, PHY_BCR, BIT(15));           // Reset PHY
  eth_write_phy(PHY_ADDR, PHY_BCR, BIT(12));           // Set autonegotiation
  ETH->DMARDLAR = (uint32_t) (uintptr_t) s_rxdesc;     // RX descriptors
  ETH->DMATDLAR = (uint32_t) (uintptr_t) s_txdesc;     // RX descriptors
  ETH->DMAIER = BIT(6) | BIT(16);                      // RIE, NISE
  ETH->MACCR = BIT(2) | BIT(3) | BIT(11) | BIT(14);    // RE, TE, Duplex, Fast
  ETH->DMAOMR = BIT(1) | BIT(13) | BIT(21) | BIT(25);  // SR, ST, TSF, RSF

  // MAC address filtering
  ETH->MACA0HR = ((uint32_t) ifp->mac[5] << 8U) | ifp->mac[4];
  ETH->MACA0LR = (uint32_t) (ifp->mac[3] << 24) |
                 ((uint32_t) ifp->mac[2] << 16) |
                 ((uint32_t) ifp->mac[1] << 8) | ifp->mac[0];
  return true;
}

static size_t mg_tcpip_driver_stm32_tx(const void *buf, size_t len,
                                       struct mg_tcpip_if *ifp) {
  if (len > sizeof(s_txbuf[s_txno])) {
    MG_ERROR(("Frame too big, %ld", (long) len));
    len = 0;  // Frame is too big
  } else if ((s_txdesc[s_txno][0] & BIT(31))) {
    ifp->nerr++;
    MG_ERROR(("No free descriptors"));
    // printf("D0 %lx SR %lx\n", (long) s_txdesc[0][0], (long) ETH->DMASR);
    len = 0;  // All descriptors are busy, fail
  } else {
    memcpy(s_txbuf[s_txno], buf, len);     // Copy data
    s_txdesc[s_txno][1] = (uint32_t) len;  // Set data len
    s_txdesc[s_txno][0] = BIT(20) | BIT(28) | BIT(29);  // Chain,FS,LS
    s_txdesc[s_txno][0] |= BIT(31);  // Set OWN bit - let DMA take over
    if (++s_txno >= ETH_DESC_CNT) s_txno = 0;
  }
  DSB();                         // ensure descriptors have been written
  ETH->DMASR = BIT(2) | BIT(5);  // Clear any prior TBUS/TUS
  ETH->DMATPDR = 0;              // and resume
  return len;
}

static bool mg_tcpip_driver_stm32_up(struct mg_tcpip_if *ifp) {
  uint32_t bsr = eth_read_phy(PHY_ADDR, PHY_BSR);
  bool up = bsr & BIT(2) ? 1 : 0;
  if ((ifp->state == MG_TCPIP_STATE_DOWN) && up) {  // link state just went up
    uint32_t scsr = eth_read_phy(PHY_ADDR, PHY_CSCR);
    uint32_t maccr = ETH->MACCR | BIT(14) | BIT(11);  // 100M, Full-duplex
    if ((scsr & BIT(3)) == 0) maccr &= ~BIT(14);      // 10M
    if ((scsr & BIT(4)) == 0) maccr &= ~BIT(11);      // Half-duplex
    ETH->MACCR = maccr;  // IRQ handler does not fiddle with this register
    MG_DEBUG(("Link is %uM %s-duplex", maccr & BIT(14) ? 100 : 10,
              maccr & BIT(11) ? "full" : "half"));
  }
  return up;
}

void ETH_IRQHandler(void);
void ETH_IRQHandler(void) {
  if (ETH->DMASR & BIT(6)) {             // Frame received, loop
    ETH->DMASR = BIT(16) | BIT(6);       // Clear flag
    for (uint32_t i = 0; i < 10; i++) {  // read as they arrive but not forever
      if (s_rxdesc[s_rxno][0] & BIT(31)) break;  // exit when done
      if (((s_rxdesc[s_rxno][0] & (BIT(8) | BIT(9))) == (BIT(8) | BIT(9))) &&
          !(s_rxdesc[s_rxno][0] & BIT(15))) {  // skip partial/errored frames
        uint32_t len = ((s_rxdesc[s_rxno][0] >> 16) & (BIT(14) - 1));
        //  printf("%lx %lu %lx %.8lx\n", s_rxno, len, s_rxdesc[s_rxno][0],
        //  ETH->DMASR);
        mg_tcpip_qwrite(s_rxbuf[s_rxno], len > 4 ? len - 4 : len, s_ifp);
      }
      s_rxdesc[s_rxno][0] = BIT(31);
      if (++s_rxno >= ETH_DESC_CNT) s_rxno = 0;
    }
  }
  ETH->DMASR = BIT(7);  // Clear possible RBUS while processing
  ETH->DMARPDR = 0;     // and resume RX
}

struct mg_tcpip_driver mg_tcpip_driver_stm32 = {mg_tcpip_driver_stm32_init,
                                                mg_tcpip_driver_stm32_tx, NULL,
                                                mg_tcpip_driver_stm32_up};
#endif

#ifdef MG_ENABLE_LINES
#line 1 "src/drivers/stm32h.c"
#endif


#if MG_ENABLE_TCPIP && defined(MG_ENABLE_DRIVER_STM32H) && \
    MG_ENABLE_DRIVER_STM32H
struct stm32h_eth {
  volatile uint32_t MACCR, MACECR, MACPFR, MACWTR, MACHT0R, MACHT1R,
      RESERVED1[14], MACVTR, RESERVED2, MACVHTR, RESERVED3, MACVIR, MACIVIR,
      RESERVED4[2], MACTFCR, RESERVED5[7], MACRFCR, RESERVED6[7], MACISR,
      MACIER, MACRXTXSR, RESERVED7, MACPCSR, MACRWKPFR, RESERVED8[2], MACLCSR,
      MACLTCR, MACLETR, MAC1USTCR, RESERVED9[12], MACVR, MACDR, RESERVED10,
      MACHWF0R, MACHWF1R, MACHWF2R, RESERVED11[54], MACMDIOAR, MACMDIODR,
      RESERVED12[2], MACARPAR, RESERVED13[59], MACA0HR, MACA0LR, MACA1HR,
      MACA1LR, MACA2HR, MACA2LR, MACA3HR, MACA3LR, RESERVED14[248], MMCCR,
      MMCRIR, MMCTIR, MMCRIMR, MMCTIMR, RESERVED15[14], MMCTSCGPR, MMCTMCGPR,
      RESERVED16[5], MMCTPCGR, RESERVED17[10], MMCRCRCEPR, MMCRAEPR,
      RESERVED18[10], MMCRUPGR, RESERVED19[9], MMCTLPIMSTR, MMCTLPITCR,
      MMCRLPIMSTR, MMCRLPITCR, RESERVED20[65], MACL3L4C0R, MACL4A0R,
      RESERVED21[2], MACL3A0R0R, MACL3A1R0R, MACL3A2R0R, MACL3A3R0R,
      RESERVED22[4], MACL3L4C1R, MACL4A1R, RESERVED23[2], MACL3A0R1R,
      MACL3A1R1R, MACL3A2R1R, MACL3A3R1R, RESERVED24[108], MACTSCR, MACSSIR,
      MACSTSR, MACSTNR, MACSTSUR, MACSTNUR, MACTSAR, RESERVED25, MACTSSR,
      RESERVED26[3], MACTTSSNR, MACTTSSSR, RESERVED27[2], MACACR, RESERVED28,
      MACATSNR, MACATSSR, MACTSIACR, MACTSEACR, MACTSICNR, MACTSECNR,
      RESERVED29[4], MACPPSCR, RESERVED30[3], MACPPSTTSR, MACPPSTTNR, MACPPSIR,
      MACPPSWR, RESERVED31[12], MACPOCR, MACSPI0R, MACSPI1R, MACSPI2R, MACLMIR,
      RESERVED32[11], MTLOMR, RESERVED33[7], MTLISR, RESERVED34[55], MTLTQOMR,
      MTLTQUR, MTLTQDR, RESERVED35[8], MTLQICSR, MTLRQOMR, MTLRQMPOCR, MTLRQDR,
      RESERVED36[177], DMAMR, DMASBMR, DMAISR, DMADSR, RESERVED37[60], DMACCR,
      DMACTCR, DMACRCR, RESERVED38[2], DMACTDLAR, RESERVED39, DMACRDLAR,
      DMACTDTPR, RESERVED40, DMACRDTPR, DMACTDRLR, DMACRDRLR, DMACIER,
      DMACRIWTR, DMACSFCSR, RESERVED41, DMACCATDR, RESERVED42, DMACCARDR,
      RESERVED43, DMACCATBR, RESERVED44, DMACCARBR, DMACSR, RESERVED45[2],
      DMACMFCR;
};
#undef ETH
#define ETH \
  ((struct stm32h_eth *) (uintptr_t) (0x40000000UL + 0x00020000UL + 0x8000UL))

#undef BIT
#define BIT(x) ((uint32_t) 1 << (x))
#define ETH_PKT_SIZE 1540  // Max frame size
#define ETH_DESC_CNT 4     // Descriptors count
#define ETH_DS 4           // Descriptor size (words)

static volatile uint32_t s_rxdesc[ETH_DESC_CNT][ETH_DS];  // RX descriptors
static volatile uint32_t s_txdesc[ETH_DESC_CNT][ETH_DS];  // TX descriptors
static uint8_t s_rxbuf[ETH_DESC_CNT][ETH_PKT_SIZE];       // RX ethernet buffers
static uint8_t s_txbuf[ETH_DESC_CNT][ETH_PKT_SIZE];       // TX ethernet buffers
static struct mg_tcpip_if *s_ifp;                         // MIP interface
enum {
  PHY_ADDR = 0,
  PHY_BCR = 0,
  PHY_BSR = 1,
  PHY_CSCR = 31
};  // PHY constants

static uint32_t eth_read_phy(uint8_t addr, uint8_t reg) {
  ETH->MACMDIOAR &= (0xF << 8);
  ETH->MACMDIOAR |= ((uint32_t) addr << 21) | ((uint32_t) reg << 16) | 3 << 2;
  ETH->MACMDIOAR |= BIT(0);
  while (ETH->MACMDIOAR & BIT(0)) (void) 0;
  return ETH->MACMDIODR;
}

static void eth_write_phy(uint8_t addr, uint8_t reg, uint32_t val) {
  ETH->MACMDIODR = val;
  ETH->MACMDIOAR &= (0xF << 8);
  ETH->MACMDIOAR |= ((uint32_t) addr << 21) | ((uint32_t) reg << 16) | 1 << 2;
  ETH->MACMDIOAR |= BIT(0);
  while (ETH->MACMDIOAR & BIT(0)) (void) 0;
}

static uint32_t get_hclk(void) {
  struct rcc {
    volatile uint32_t CR, HSICFGR, CRRCR, CSICFGR, CFGR, RESERVED1, D1CFGR,
        D2CFGR, D3CFGR, RESERVED2, PLLCKSELR, PLLCFGR, PLL1DIVR, PLL1FRACR,
        PLL2DIVR, PLL2FRACR, PLL3DIVR, PLL3FRACR, RESERVED3, D1CCIPR, D2CCIP1R,
        D2CCIP2R, D3CCIPR, RESERVED4, CIER, CIFR, CICR, RESERVED5, BDCR, CSR,
        RESERVED6, AHB3RSTR, AHB1RSTR, AHB2RSTR, AHB4RSTR, APB3RSTR, APB1LRSTR,
        APB1HRSTR, APB2RSTR, APB4RSTR, GCR, RESERVED8, D3AMR, RESERVED11[9],
        RSR, AHB3ENR, AHB1ENR, AHB2ENR, AHB4ENR, APB3ENR, APB1LENR, APB1HENR,
        APB2ENR, APB4ENR, RESERVED12, AHB3LPENR, AHB1LPENR, AHB2LPENR,
        AHB4LPENR, APB3LPENR, APB1LLPENR, APB1HLPENR, APB2LPENR, APB4LPENR,
        RESERVED13[4];
  } *rcc = ((struct rcc *) (0x40000000 + 0x18020000 + 0x4400));
  uint32_t clk = 0, hsi = 64000000 /* 64 MHz */, hse = 8000000 /* 8MHz */,
           csi = 4000000 /* 4MHz */;
  unsigned int sel = (rcc->CFGR & (7 << 3)) >> 3;

  if (sel == 1) {
    clk = csi;
  } else if (sel == 2) {
    clk = hse;
  } else if (sel == 3) {
    uint32_t vco, m, n, p;
    unsigned int src = (rcc->PLLCKSELR & (3 << 0)) >> 0;
    m = ((rcc->PLLCKSELR & (0x3F << 4)) >> 4);
    n = ((rcc->PLL1DIVR & (0x1FF << 0)) >> 0) + 1 +
        ((rcc->PLLCFGR & BIT(0)) ? 1 : 0);  // round-up in fractional mode
    p = ((rcc->PLL1DIVR & (0x7F << 9)) >> 9) + 1;
    if (src == 1) {
      clk = csi;
    } else if (src == 2) {
      clk = hse;
    } else {
      clk = hsi;
      clk >>= ((rcc->CR & 3) >> 3);
    }
    vco = (uint32_t) ((uint64_t) clk * n / m);
    clk = vco / p;
  } else {
    clk = hsi;
    clk >>= ((rcc->CR & 3) >> 3);
  }
  const uint8_t cptab[12] = {1, 2, 3, 4, 6, 7, 8, 9};  // log2(div)
  uint32_t d1cpre = (rcc->D1CFGR & (0x0F << 8)) >> 8;
  if (d1cpre >= 8) clk >>= cptab[d1cpre - 8];
  MG_DEBUG(("D1 CLK: %u", clk));
  uint32_t hpre = (rcc->D1CFGR & (0x0F << 0)) >> 0;
  if (hpre < 8) return clk;
  return ((uint32_t) clk) >> cptab[hpre - 8];
}

//  Guess CR from AHB1 clock. MDC clock is generated from the ETH peripheral
//  clock (AHB1); as per 802.3, it must not exceed 2. As the AHB clock can
//  be derived from HSI or CSI (internal RC) clocks, and those can go above
//  specs, the datasheets specify a range of frequencies and activate one of a
//  series of dividers to keep the MDC clock safely below 2.5MHz. We guess a
//  divider setting based on HCLK with some drift. If the user uses a different
//  clock from our defaults, needs to set the macros on top. Valid for
//  STM32H74xxx/75xxx (58.11.4)(4.5% worst case drift)(CSI clock has a 7.5 %
//  worst case drift @ max temp)
static int guess_mdc_cr(void) {
  const uint8_t crs[] = {2, 3, 0, 1, 4, 5};  // ETH->MACMDIOAR::CR values
  const uint8_t div[] = {16, 26, 42, 62, 102, 124};  // Respective HCLK dividers
  uint32_t hclk = get_hclk();                        // Guess system HCLK
  int result = -1;                                   // Invalid CR value
  for (int i = 0; i < 6; i++) {
    if (hclk / div[i] <= 2375000UL /* 2.5MHz - 5% */) {
      result = crs[i];
      break;
    }
  }
  if (result < 0) MG_ERROR(("HCLK too high"));
  MG_DEBUG(("HCLK: %u, CR: %d", hclk, result));
  return result;
}

static bool mg_tcpip_driver_stm32h_init(struct mg_tcpip_if *ifp) {
  struct mg_tcpip_driver_stm32h_data *d =
      (struct mg_tcpip_driver_stm32h_data *) ifp->driver_data;
  s_ifp = ifp;

  // Init RX descriptors
  for (int i = 0; i < ETH_DESC_CNT; i++) {
    s_rxdesc[i][0] = (uint32_t) (uintptr_t) s_rxbuf[i];  // Point to data buffer
    s_rxdesc[i][3] = BIT(31) | BIT(30) | BIT(24);        // OWN, IOC, BUF1V
  }

  // Init TX descriptors
  for (int i = 0; i < ETH_DESC_CNT; i++) {
    s_txdesc[i][0] = (uint32_t) (uintptr_t) s_txbuf[i];  // Buf pointer
  }

  ETH->DMAMR |= BIT(0);                         // Software reset
  while ((ETH->DMAMR & BIT(0)) != 0) (void) 0;  // Wait until done

  // Set MDC clock divider. If user told us the value, use it. Otherwise, guess
  int cr = (d == NULL || d->mdc_cr < 0) ? guess_mdc_cr() : d->mdc_cr;
  ETH->MACMDIOAR = ((uint32_t) cr & 0xF) << 8;

  // NOTE(scaprile): We do not use timing facilities so the DMA engine does not
  // re-write buffer address
  ETH->DMAMR = 0 << 16;     // use interrupt mode 0 (58.8.1) (reset value)
  ETH->DMASBMR |= BIT(12);  // AAL NOTE(scaprile): is this actually needed
  ETH->MACIER = 0;        // Do not enable additional irq sources (reset value)
  ETH->MACTFCR = BIT(7);  // Disable zero-quanta pause
  // ETH->MACPFR = BIT(31);  // Receive all
  eth_write_phy(PHY_ADDR, PHY_BCR, BIT(15));  // Reset PHY
  eth_write_phy(PHY_ADDR, PHY_BCR, BIT(12));  // Set autonegotiation
  ETH->DMACRDLAR =
      (uint32_t) (uintptr_t) s_rxdesc;  // RX descriptors start address
  ETH->DMACRDRLR = ETH_DESC_CNT - 1;    // ring length
  ETH->DMACRDTPR =
      (uint32_t) (uintptr_t) &s_rxdesc[ETH_DESC_CNT -
                                       1];  // last valid descriptor address
  ETH->DMACTDLAR =
      (uint32_t) (uintptr_t) s_txdesc;  // TX descriptors start address
  ETH->DMACTDRLR = ETH_DESC_CNT - 1;    // ring length
  ETH->DMACTDTPR =
      (uint32_t) (uintptr_t) s_txdesc;  // first available descriptor address
  ETH->DMACCR = 0;  // DSL = 0 (contiguous descriptor table) (reset value)
  ETH->DMACIER = BIT(6) | BIT(15);  // RIE, NIE
  ETH->MACCR = BIT(0) | BIT(1) | BIT(13) | BIT(14) |
               BIT(15);     // RE, TE, Duplex, Fast, Reserved
  ETH->MTLTQOMR |= BIT(1);  // TSF
  ETH->MTLRQOMR |= BIT(5);  // RSF
  ETH->DMACTCR |= BIT(0);   // ST
  ETH->DMACRCR |= BIT(0);   // SR

  // MAC address filtering
  ETH->MACA0HR = ((uint32_t) ifp->mac[5] << 8U) | ifp->mac[4];
  ETH->MACA0LR = (uint32_t) (ifp->mac[3] << 24) |
                 ((uint32_t) ifp->mac[2] << 16) |
                 ((uint32_t) ifp->mac[1] << 8) | ifp->mac[0];
  return true;
}

static uint32_t s_txno;
static size_t mg_tcpip_driver_stm32h_tx(const void *buf, size_t len,
                                        struct mg_tcpip_if *ifp) {
  if (len > sizeof(s_txbuf[s_txno])) {
    MG_ERROR(("Frame too big, %ld", (long) len));
    len = 0;  // Frame is too big
  } else if ((s_txdesc[s_txno][3] & BIT(31))) {
    MG_ERROR(("No free descriptors: %u %08X %08X %08X", s_txno,
              s_txdesc[s_txno][3], ETH->DMACSR, ETH->DMACTCR));
    for (int i = 0; i < ETH_DESC_CNT; i++) MG_ERROR(("%08X", s_txdesc[i][3]));
    len = 0;  // All descriptors are busy, fail
  } else {
    memcpy(s_txbuf[s_txno], buf, len);        // Copy data
    s_txdesc[s_txno][2] = (uint32_t) len;     // Set data len
    s_txdesc[s_txno][3] = BIT(28) | BIT(29);  // FD, LD
    s_txdesc[s_txno][3] |= BIT(31);           // Set OWN bit - let DMA take over
    if (++s_txno >= ETH_DESC_CNT) s_txno = 0;
  }
  ETH->DMACSR |= BIT(2) | BIT(1);  // Clear any prior TBU, TPS
  ETH->DMACTDTPR = (uint32_t) (uintptr_t) &s_txdesc[s_txno];  // and resume
  return len;
  (void) ifp;
}

static bool mg_tcpip_driver_stm32h_up(struct mg_tcpip_if *ifp) {
  uint32_t bsr = eth_read_phy(PHY_ADDR, PHY_BSR);
  bool up = bsr & BIT(2) ? 1 : 0;
  if ((ifp->state == MG_TCPIP_STATE_DOWN) && up) {  // link state just went up
    uint32_t scsr = eth_read_phy(PHY_ADDR, PHY_CSCR);
    uint32_t maccr = ETH->MACCR | BIT(14) | BIT(13);  // 100M, Full-duplex
    if ((scsr & BIT(3)) == 0) maccr &= ~BIT(14);      // 10M
    if ((scsr & BIT(4)) == 0) maccr &= ~BIT(13);      // Half-duplex
    ETH->MACCR = maccr;  // IRQ handler does not fiddle with this register
    MG_DEBUG(("Link is %uM %s-duplex", maccr & BIT(14) ? 100 : 10,
              maccr & BIT(13) ? "full" : "half"));
  }
  return up;
}

void ETH_IRQHandler(void);
static uint32_t s_rxno;
void ETH_IRQHandler(void) {
  if (ETH->DMACSR & BIT(6)) {            // Frame received, loop
    ETH->DMACSR = BIT(15) | BIT(6);      // Clear flag
    for (uint32_t i = 0; i < 10; i++) {  // read as they arrive but not forever
      if (s_rxdesc[s_rxno][3] & BIT(31)) break;  // exit when done
      if (((s_rxdesc[s_rxno][3] & (BIT(28) | BIT(29))) ==
           (BIT(28) | BIT(29))) &&
          !(s_rxdesc[s_rxno][3] & BIT(15))) {  // skip partial/errored frames
        uint32_t len = s_rxdesc[s_rxno][3] & (BIT(15) - 1);
        // MG_DEBUG(("%lx %lu %lx %08lx", s_rxno, len, s_rxdesc[s_rxno][3],
        // ETH->DMACSR));
        mg_tcpip_qwrite(s_rxbuf[s_rxno], len > 4 ? len - 4 : len, s_ifp);
      }
      s_rxdesc[s_rxno][3] = BIT(31) | BIT(30) | BIT(24);  // OWN, IOC, BUF1V
      if (++s_rxno >= ETH_DESC_CNT) s_rxno = 0;
    }
  }
  ETH->DMACSR = BIT(7) | BIT(8);  // Clear possible RBU RPS while processing
  ETH->DMACRDTPR =
      (uint32_t) (uintptr_t) &s_rxdesc[ETH_DESC_CNT - 1];  // and resume RX
}

struct mg_tcpip_driver mg_tcpip_driver_stm32h = {
    mg_tcpip_driver_stm32h_init, mg_tcpip_driver_stm32h_tx, NULL,
    mg_tcpip_driver_stm32h_up};
#endif

#ifdef MG_ENABLE_LINES
#line 1 "src/drivers/tm4c.c"
#endif


#if MG_ENABLE_TCPIP && defined(MG_ENABLE_DRIVER_TM4C) && MG_ENABLE_DRIVER_TM4C
struct tm4c_emac {
  volatile uint32_t EMACCFG, EMACFRAMEFLTR, EMACHASHTBLH, EMACHASHTBLL,
      EMACMIIADDR, EMACMIIDATA, EMACFLOWCTL, EMACVLANTG, RESERVED0, EMACSTATUS,
      EMACRWUFF, EMACPMTCTLSTAT, RESERVED1[2], EMACRIS, EMACIM, EMACADDR0H,
      EMACADDR0L, EMACADDR1H, EMACADDR1L, EMACADDR2H, EMACADDR2L, EMACADDR3H,
      EMACADDR3L, RESERVED2[31], EMACWDOGTO, RESERVED3[8], EMACMMCCTRL,
      EMACMMCRXRIS, EMACMMCTXRIS, EMACMMCRXIM, EMACMMCTXIM, RESERVED4,
      EMACTXCNTGB, RESERVED5[12], EMACTXCNTSCOL, EMACTXCNTMCOL, RESERVED6[4],
      EMACTXOCTCNTG, RESERVED7[6], EMACRXCNTGB, RESERVED8[4], EMACRXCNTCRCERR,
      EMACRXCNTALGNERR, RESERVED9[10], EMACRXCNTGUNI, RESERVED10[239],
      EMACVLNINCREP, EMACVLANHASH, RESERVED11[93], EMACTIMSTCTRL, EMACSUBSECINC,
      EMACTIMSEC, EMACTIMNANO, EMACTIMSECU, EMACTIMNANOU, EMACTIMADD,
      EMACTARGSEC, EMACTARGNANO, EMACHWORDSEC, EMACTIMSTAT, EMACPPSCTRL,
      RESERVED12[12], EMACPPS0INTVL, EMACPPS0WIDTH, RESERVED13[294],
      EMACDMABUSMOD, EMACTXPOLLD, EMACRXPOLLD, EMACRXDLADDR, EMACTXDLADDR,
      EMACDMARIS, EMACDMAOPMODE, EMACDMAIM, EMACMFBOC, EMACRXINTWDT,
      RESERVED14[8], EMACHOSTXDESC, EMACHOSRXDESC, EMACHOSTXBA, EMACHOSRXBA,
      RESERVED15[218], EMACPP, EMACPC, EMACCC, RESERVED16, EMACEPHYRIS,
      EMACEPHYIM, EMACEPHYIMSC;
};
#undef EMAC
#define EMAC ((struct tm4c_emac *) (uintptr_t) 0x400EC000)

#undef BIT
#define BIT(x) ((uint32_t) 1 << (x))
#define ETH_PKT_SIZE 1540  // Max frame size
#define ETH_DESC_CNT 4     // Descriptors count
#define ETH_DS 4           // Descriptor size (words)

static uint32_t s_rxdesc[ETH_DESC_CNT][ETH_DS];      // RX descriptors
static uint32_t s_txdesc[ETH_DESC_CNT][ETH_DS];      // TX descriptors
static uint8_t s_rxbuf[ETH_DESC_CNT][ETH_PKT_SIZE];  // RX ethernet buffers
static uint8_t s_txbuf[ETH_DESC_CNT][ETH_PKT_SIZE];  // TX ethernet buffers
static struct mg_tcpip_if *s_ifp;                    // MIP interface
enum {
  EPHY_ADDR = 0,
  EPHYBMCR = 0,
  EPHYBMSR = 1,
  EPHYSTS = 16
};  // PHY constants

static inline void tm4cspin(volatile uint32_t count) {
  while (count--) (void) 0;
}

static uint32_t emac_read_phy(uint8_t addr, uint8_t reg) {
  EMAC->EMACMIIADDR &= (0xf << 2);
  EMAC->EMACMIIADDR |= ((uint32_t) addr << 11) | ((uint32_t) reg << 6);
  EMAC->EMACMIIADDR |= BIT(0);
  while (EMAC->EMACMIIADDR & BIT(0)) tm4cspin(1);
  return EMAC->EMACMIIDATA;
}

static void emac_write_phy(uint8_t addr, uint8_t reg, uint32_t val) {
  EMAC->EMACMIIDATA = val;
  EMAC->EMACMIIADDR &= (0xf << 2);
  EMAC->EMACMIIADDR |= ((uint32_t) addr << 11) | ((uint32_t) reg << 6) | BIT(1);
  EMAC->EMACMIIADDR |= BIT(0);
  while (EMAC->EMACMIIADDR & BIT(0)) tm4cspin(1);
}

static uint32_t get_sysclk(void) {
  struct sysctl {
    volatile uint32_t DONTCARE0[44], RSCLKCFG, DONTCARE1[43], PLLFREQ0,
        PLLFREQ1;
  } *sysctl = (struct sysctl *) 0x400FE000;
  uint32_t clk = 0, piosc = 16000000 /* 16 MHz */, mosc = 25000000 /* 25MHz */;
  if (sysctl->RSCLKCFG & (1 << 28)) {  // USEPLL
    uint32_t fin, vco, mdiv, n, q, psysdiv;
    uint32_t pllsrc = (sysctl->RSCLKCFG & (0xf << 24)) >> 24;
    if (pllsrc == 0) {
      clk = piosc;
    } else if (pllsrc == 3) {
      clk = mosc;
    } else {
      MG_ERROR(("Unsupported clock source"));
    }
    q = (sysctl->PLLFREQ1 & (0x1f << 8)) >> 8;
    n = (sysctl->PLLFREQ1 & (0x1f << 0)) >> 0;
    fin = clk / ((q + 1) * (n + 1));
    mdiv = (sysctl->PLLFREQ0 & (0x3ff << 0)) >>
           0;  // mint + (mfrac / 1024); MFRAC not supported
    psysdiv = (sysctl->RSCLKCFG & (0x3f << 0)) >> 0;
    vco = (uint32_t) ((uint64_t) fin * mdiv);
    return vco / (psysdiv + 1);
  }
  uint32_t oscsrc = (sysctl->RSCLKCFG & (0xf << 20)) >> 20;
  if (oscsrc == 0) {
    clk = piosc;
  } else if (oscsrc == 3) {
    clk = mosc;
  } else {
    MG_ERROR(("Unsupported clock source"));
  }
  uint32_t osysdiv = (sysctl->RSCLKCFG & (0xf << 16)) >> 16;
  return clk / (osysdiv + 1);
}

//  Guess CR from SYSCLK. MDC clock is generated from SYSCLK (AHB); as per
//  802.3, it must not exceed 2.5MHz (also 20.4.2.6) As the AHB clock can be
//  derived from the PIOSC (internal RC), and it can go above  specs, the
//  datasheets specify a range of frequencies and activate one of a series of
//  dividers to keep the MDC clock safely below 2.5MHz. We guess a divider
//  setting based on SYSCLK with a +5% drift. If the user uses a different clock
//  from our defaults, needs to set the macros on top Valid for TM4C129x (20.7)
//  (4.5% worst case drift)
// The PHY receives the main oscillator (MOSC) (20.3.1)
static int guess_mdc_cr(void) {
  uint8_t crs[] = {2, 3, 0, 1};      // EMAC->MACMIIAR::CR values
  uint8_t div[] = {16, 26, 42, 62};  // Respective HCLK dividers
  uint32_t sysclk = get_sysclk();    // Guess system SYSCLK
  int result = -1;                   // Invalid CR value
  if (sysclk < 25000000) {
    MG_ERROR(("SYSCLK too low"));
  } else {
    for (int i = 0; i < 4; i++) {
      if (sysclk / div[i] <= 2375000UL /* 2.5MHz - 5% */) {
        result = crs[i];
        break;
      }
    }
    if (result < 0) MG_ERROR(("SYSCLK too high"));
  }
  MG_DEBUG(("SYSCLK: %u, CR: %d", sysclk, result));
  return result;
}

static bool mg_tcpip_driver_tm4c_init(struct mg_tcpip_if *ifp) {
  struct mg_tcpip_driver_tm4c_data *d =
      (struct mg_tcpip_driver_tm4c_data *) ifp->driver_data;
  s_ifp = ifp;

  // Init RX descriptors
  for (int i = 0; i < ETH_DESC_CNT; i++) {
    s_rxdesc[i][0] = BIT(31);                            // Own
    s_rxdesc[i][1] = sizeof(s_rxbuf[i]) | BIT(14);       // 2nd address chained
    s_rxdesc[i][2] = (uint32_t) (uintptr_t) s_rxbuf[i];  // Point to data buffer
    s_rxdesc[i][3] =
        (uint32_t) (uintptr_t) s_rxdesc[(i + 1) % ETH_DESC_CNT];  // Chain
    // MG_DEBUG(("%d %p", i, s_rxdesc[i]));
  }

  // Init TX descriptors
  for (int i = 0; i < ETH_DESC_CNT; i++) {
    s_txdesc[i][2] = (uint32_t) (uintptr_t) s_txbuf[i];  // Buf pointer
    s_txdesc[i][3] =
        (uint32_t) (uintptr_t) s_txdesc[(i + 1) % ETH_DESC_CNT];  // Chain
  }

  EMAC->EMACDMABUSMOD |= BIT(0);                            // Software reset
  while ((EMAC->EMACDMABUSMOD & BIT(0)) != 0) tm4cspin(1);  // Wait until done

  // Set MDC clock divider. If user told us the value, use it. Otherwise, guess
  int cr = (d == NULL || d->mdc_cr < 0) ? guess_mdc_cr() : d->mdc_cr;
  EMAC->EMACMIIADDR = ((uint32_t) cr & 0xf) << 2;

  // NOTE(cpq): we do not use extended descriptor bit 7, and do not use
  // hardware checksum. Therefore, descriptor size is 4, not 8
  // EMAC->EMACDMABUSMOD = BIT(13) | BIT(16) | BIT(22) | BIT(23) | BIT(25);
  EMAC->EMACIM = BIT(3) | BIT(9);  // Mask timestamp & PMT IT
  EMAC->EMACFLOWCTL = BIT(7);      // Disable zero-quanta pause
  // EMAC->EMACFRAMEFLTR = BIT(31);   // Receive all
  // EMAC->EMACPC defaults to internal PHY (EPHY) in MMI mode
  emac_write_phy(EPHY_ADDR, EPHYBMCR, BIT(15));  // Reset internal PHY (EPHY)
  emac_write_phy(EPHY_ADDR, EPHYBMCR, BIT(12));  // Set autonegotiation
  EMAC->EMACRXDLADDR = (uint32_t) (uintptr_t) s_rxdesc;  // RX descriptors
  EMAC->EMACTXDLADDR = (uint32_t) (uintptr_t) s_txdesc;  // TX descriptors
  EMAC->EMACDMAIM = BIT(6) | BIT(16);                    // RIE, NIE
  EMAC->EMACCFG = BIT(2) | BIT(3) | BIT(11) | BIT(14);   // RE, TE, Duplex, Fast
  EMAC->EMACDMAOPMODE =
      BIT(1) | BIT(13) | BIT(21) | BIT(25);  // SR, ST, TSF, RSF
  EMAC->EMACADDR0H = ((uint32_t) ifp->mac[5] << 8U) | ifp->mac[4];
  EMAC->EMACADDR0L = (uint32_t) (ifp->mac[3] << 24) |
                     ((uint32_t) ifp->mac[2] << 16) |
                     ((uint32_t) ifp->mac[1] << 8) | ifp->mac[0];
  // NOTE(scaprile) There are 3 additional slots for filtering, disabled by
  // default. This also applies to the STM32 driver (at least for F7)
  return true;
}

static uint32_t s_txno;
static size_t mg_tcpip_driver_tm4c_tx(const void *buf, size_t len,
                                      struct mg_tcpip_if *ifp) {
  if (len > sizeof(s_txbuf[s_txno])) {
    MG_ERROR(("Frame too big, %ld", (long) len));
    len = 0;  // fail
  } else if ((s_txdesc[s_txno][0] & BIT(31))) {
    MG_ERROR(("No descriptors available"));
    // printf("D0 %lx SR %lx\n", (long) s_txdesc[0][0], (long)
    // EMAC->EMACDMARIS);
    len = 0;  // fail
  } else {
    memcpy(s_txbuf[s_txno], buf, len);     // Copy data
    s_txdesc[s_txno][1] = (uint32_t) len;  // Set data len
    s_txdesc[s_txno][0] =
        BIT(20) | BIT(28) | BIT(29) | BIT(30);  // Chain,FS,LS,IC
    s_txdesc[s_txno][0] |= BIT(31);  // Set OWN bit - let DMA take over
    if (++s_txno >= ETH_DESC_CNT) s_txno = 0;
  }
  EMAC->EMACDMARIS = BIT(2) | BIT(5);  // Clear any prior TU/UNF
  EMAC->EMACTXPOLLD = 0;               // and resume
  return len;
  (void) ifp;
}

static bool mg_tcpip_driver_tm4c_up(struct mg_tcpip_if *ifp) {
  uint32_t bmsr = emac_read_phy(EPHY_ADDR, EPHYBMSR);
  bool up = (bmsr & BIT(2)) ? 1 : 0;
  if ((ifp->state == MG_TCPIP_STATE_DOWN) && up) {  // link state just went up
    uint32_t sts = emac_read_phy(EPHY_ADDR, EPHYSTS);
    uint32_t emaccfg = EMAC->EMACCFG | BIT(14) | BIT(11);  // 100M, Full-duplex
    if (sts & BIT(1)) emaccfg &= ~BIT(14);                 // 10M
    if ((sts & BIT(2)) == 0) emaccfg &= ~BIT(11);          // Half-duplex
    EMAC->EMACCFG = emaccfg;  // IRQ handler does not fiddle with this register
    MG_DEBUG(("Link is %uM %s-duplex", emaccfg & BIT(14) ? 100 : 10,
              emaccfg & BIT(11) ? "full" : "half"));
  }
  return up;
}

void EMAC0_IRQHandler(void);
static uint32_t s_rxno;
void EMAC0_IRQHandler(void) {
  if (EMAC->EMACDMARIS & BIT(6)) {        // Frame received, loop
    EMAC->EMACDMARIS = BIT(16) | BIT(6);  // Clear flag
    for (uint32_t i = 0; i < 10; i++) {   // read as they arrive but not forever
      if (s_rxdesc[s_rxno][0] & BIT(31)) break;  // exit when done
      if (((s_rxdesc[s_rxno][0] & (BIT(8) | BIT(9))) == (BIT(8) | BIT(9))) &&
          !(s_rxdesc[s_rxno][0] & BIT(15))) {  // skip partial/errored frames
        uint32_t len = ((s_rxdesc[s_rxno][0] >> 16) & (BIT(14) - 1));
        //  printf("%lx %lu %lx %.8lx\n", s_rxno, len, s_rxdesc[s_rxno][0],
        //  EMAC->EMACDMARIS);
        mg_tcpip_qwrite(s_rxbuf[s_rxno], len > 4 ? len - 4 : len, s_ifp);
      }
      s_rxdesc[s_rxno][0] = BIT(31);
      if (++s_rxno >= ETH_DESC_CNT) s_rxno = 0;
    }
  }
  EMAC->EMACDMARIS = BIT(7);  // Clear possible RU while processing
  EMAC->EMACRXPOLLD = 0;      // and resume RX
}

struct mg_tcpip_driver mg_tcpip_driver_tm4c = {mg_tcpip_driver_tm4c_init,
                                               mg_tcpip_driver_tm4c_tx, NULL,
                                               mg_tcpip_driver_tm4c_up};
#endif

#ifdef MG_ENABLE_LINES
#line 1 "src/drivers/w5500.c"
#endif


#if MG_ENABLE_TCPIP

enum { W5500_CR = 0, W5500_S0 = 1, W5500_TX0 = 2, W5500_RX0 = 3 };

static void w5500_txn(struct mg_tcpip_spi *s, uint8_t block, uint16_t addr, bool wr,
                      void *buf, size_t len) {
  uint8_t *p = (uint8_t *) buf;
  uint8_t cmd[] = {(uint8_t) (addr >> 8), (uint8_t) (addr & 255),
                   (uint8_t) ((block << 3) | (wr ? 4 : 0))};
  s->begin(s->spi);
  for (size_t i = 0; i < sizeof(cmd); i++) s->txn(s->spi, cmd[i]);
  for (size_t i = 0; i < len; i++) {
    uint8_t r = s->txn(s->spi, p[i]);
    if (!wr) p[i] = r;
  }
  s->end(s->spi);
}

// clang-format off
static  void w5500_wn(struct mg_tcpip_spi *s, uint8_t block, uint16_t addr, void *buf, size_t len) { w5500_txn(s, block, addr, true, buf, len); }
static  void w5500_w1(struct mg_tcpip_spi *s, uint8_t block, uint16_t addr, uint8_t val) { w5500_wn(s, block, addr, &val, 1); }
static  void w5500_w2(struct mg_tcpip_spi *s, uint8_t block, uint16_t addr, uint16_t val) { uint8_t buf[2] = {(uint8_t) (val >> 8), (uint8_t) (val & 255)}; w5500_wn(s, block, addr, buf, sizeof(buf)); }
static  void w5500_rn(struct mg_tcpip_spi *s, uint8_t block, uint16_t addr, void *buf, size_t len) { w5500_txn(s, block, addr, false, buf, len); }
static  uint8_t w5500_r1(struct mg_tcpip_spi *s, uint8_t block, uint16_t addr) { uint8_t r = 0; w5500_rn(s, block, addr, &r, 1); return r; }
static  uint16_t w5500_r2(struct mg_tcpip_spi *s, uint8_t block, uint16_t addr) { uint8_t buf[2] = {0, 0}; w5500_rn(s, block, addr, buf, sizeof(buf)); return (uint16_t) ((buf[0] << 8) | buf[1]); }
// clang-format on

static size_t w5500_rx(void *buf, size_t buflen, struct mg_tcpip_if *ifp) {
  struct mg_tcpip_spi *s = (struct mg_tcpip_spi *) ifp->driver_data;
  uint16_t r = 0, n = 0, len = (uint16_t) buflen, n2;     // Read recv len
  while ((n2 = w5500_r2(s, W5500_S0, 0x26)) > n) n = n2;  // Until it is stable
  // printf("RSR: %d\n", (int) n);
  if (n > 0) {
    uint16_t ptr = w5500_r2(s, W5500_S0, 0x28);  // Get read pointer
    n = w5500_r2(s, W5500_RX0, ptr);             // Read frame length
    if (n <= len + 2 && n > 1) {
      r = (uint16_t) (n - 2);
      w5500_rn(s, W5500_RX0, (uint16_t) (ptr + 2), buf, r);
    }
    w5500_w2(s, W5500_S0, 0x28, (uint16_t) (ptr + n));  // Advance read pointer
    w5500_w1(s, W5500_S0, 1, 0x40);                     // Sock0 CR -> RECV
    // printf("  RX_RD: tot=%u n=%u r=%u\n", n2, n, r);
  }
  return r;
}

static size_t w5500_tx(const void *buf, size_t buflen, struct mg_tcpip_if *ifp) {
  struct mg_tcpip_spi *s = (struct mg_tcpip_spi *) ifp->driver_data;
  uint16_t n = 0, len = (uint16_t) buflen;
  while (n < len) n = w5500_r2(s, W5500_S0, 0x20);      // Wait for space
  uint16_t ptr = w5500_r2(s, W5500_S0, 0x24);           // Get write pointer
  w5500_wn(s, W5500_TX0, ptr, (void *) buf, len);       // Write data
  w5500_w2(s, W5500_S0, 0x24, (uint16_t) (ptr + len));  // Advance write pointer
  w5500_w1(s, W5500_S0, 1, 0x20);                       // Sock0 CR -> SEND
  for (int i = 0; i < 40; i++) {
    uint8_t ir = w5500_r1(s, W5500_S0, 2);  // Read S0 IR
    if (ir == 0) continue;
    // printf("IR %d, len=%d, free=%d, ptr %d\n", ir, (int) len, (int) n, ptr);
    w5500_w1(s, W5500_S0, 2, ir);  // Write S0 IR: clear it!
    if (ir & 8) len = 0;           // Timeout. Report error
    if (ir & (16 | 8)) break;      // Stop on SEND_OK or timeout
  }
  return len;
}

static bool w5500_init(struct mg_tcpip_if *ifp) {
  struct mg_tcpip_spi *s = (struct mg_tcpip_spi *) ifp->driver_data;
  s->end(s->spi);
  w5500_w1(s, W5500_CR, 0, 0x80);     // Reset chip: CR -> 0x80
  w5500_w1(s, W5500_CR, 0x2e, 0);     // CR PHYCFGR -> reset
  w5500_w1(s, W5500_CR, 0x2e, 0xf8);  // CR PHYCFGR -> set
  // w5500_wn(s, W5500_CR, 9, s->mac, 6);      // Set source MAC
  w5500_w1(s, W5500_S0, 0x1e, 16);          // Sock0 RX buf size
  w5500_w1(s, W5500_S0, 0x1f, 16);          // Sock0 TX buf size
  w5500_w1(s, W5500_S0, 0, 4);              // Sock0 MR -> MACRAW
  w5500_w1(s, W5500_S0, 1, 1);              // Sock0 CR -> OPEN
  return w5500_r1(s, W5500_S0, 3) == 0x42;  // Sock0 SR == MACRAW
}

static bool w5500_up(struct mg_tcpip_if *ifp) {
  struct mg_tcpip_spi *spi = (struct mg_tcpip_spi *) ifp->driver_data;
  uint8_t phycfgr = w5500_r1(spi, W5500_CR, 0x2e);
  return phycfgr & 1;  // Bit 0 of PHYCFGR is LNK (0 - down, 1 - up)
}

struct mg_tcpip_driver mg_tcpip_driver_w5500 = {w5500_init, w5500_tx, w5500_rx, w5500_up};
#endif
