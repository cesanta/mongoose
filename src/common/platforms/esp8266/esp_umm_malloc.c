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

#include <string.h>
#include <stdio.h>
#include "common/umm_malloc/umm_malloc.h"
#include "esp_umm_malloc.h"

#if ESP_UMM_ENABLE

/*
 * ESP-specific glue for the `umm_malloc`.
 *
 * In SDK (https://github.com/cesanta/esp-open-sdk), there is an archive
 * `sdk/lib/libmain.a` which contains some files, including `mem_manager.o`.
 *
 * The `mem_manager.o` contains all the heap-related functions: `pvPortMalloc`,
 * etc. We have weaken all symbols from `mem_manager.o` by
 * `xtensa-lx106-elf-objcopy` (see exact commands in Dockerfile:
 * `docker/esp8266/Dockerfile-esp8266-build-oss`), and provide our own
 * implementations in this file.
 *
 * ------------------------------------
 *
 * NOTE that not all public functions from `mem_manager.o` need to be replaced:
 * some of them are used only internally:
 *
 * - system_show_malloc()
 * - pvShowMalloc()
 * - prvInsertBlockIntoUsedList()
 * - prvRemoveBlockFromUsedList()
 * - check_memleak_debug_enable()
 * - vPortInitialiseBlocks()
 *
 * So when we replace all the rest (`pvPortMalloc`, etc), we can check with
 * `objdump` that resulting binary (for SJ, it's `fw.out`) doesn't contain
 * any of the "internal" functions.
 *
 * ------------------------------------
 *
 * NOTE that to make linker actually consider implementations in this file,
 * you should explicitly reference some function from it. This is what
 * `esp_umm_init()` is for: it is a dummy no-op function that must be called
 * from somewhere outside.
 *
 * If you don't do this, linker will merely garbage-collect this file, and
 * will use heap implementation from SDK.
 */

void *pvPortMalloc(size_t size, const char *file, unsigned line) {
  (void) file;
  (void) line;

  return umm_malloc(size);
}

void *pvPortCalloc(size_t num, size_t size, const char *file, unsigned line) {
  (void) file;
  (void) line;

  return umm_calloc(num, size);
}

void *pvPortZalloc(size_t size, const char *file, unsigned line) {
  void *ret;

  (void) file;
  (void) line;

  ret = umm_malloc(size);
  if (ret != NULL) memset(ret, 0, size);
  return ret;
}

void *pvPortRealloc(void *ptr, size_t size, const char *file, unsigned line) {
  (void) file;
  (void) line;

  return umm_realloc(ptr, size);
}

void vPortFree(void *ptr, const char *file, unsigned line) {
  (void) file;
  (void) line;

  umm_free(ptr);
}

size_t xPortGetFreeHeapSize(void) {
  return umm_free_heap_size();
}

size_t xPortWantedSizeAlign(void) {
  return 4;
}

void esp_umm_init(void) {
  /* Nothing to do, see header for details */
}

void esp_umm_oom_cb(size_t size, size_t blocks_cnt) {
  fprintf(stderr, "E:M %u (%u blocks)\n", (unsigned int) size,
          (unsigned int) blocks_cnt);
}

#endif /* ESP_UMM_ENABLE */
