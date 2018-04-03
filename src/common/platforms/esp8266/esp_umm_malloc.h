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
