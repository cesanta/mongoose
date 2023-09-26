#include "device.h"

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
