/*
 * Copyright (c) 2014 Cesanta Software Limited
 * All rights reserved
 */

#if ESP_UMM_ENABLE

/*
 * This is a no-op dummy function that is merely needed because we have to
 * reference any function from the file `esp_umm_malloc.c`, so that linker
 * won't garbage-collect the whole compilation unit.
 */
void esp_umm_init(void);

/*
 * Callback that gets called by umm_malloc in case of Out-of-memory error.
 *
 * `size` is the size requested by user, and `block_cnt` is a number of heap
 * blocks that umm_malloc failed to allocate
 */
void esp_umm_oom_cb(size_t size, size_t blocks_cnt);

#endif /* CS_COMMON_PLATFORMS_ESP8266_ESP_UMM_MALLOC_H_ */
