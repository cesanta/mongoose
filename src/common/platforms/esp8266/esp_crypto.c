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

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include "mongoose.h"

#ifdef RTOS_SDK
#include "esp_libc.h"
#else
#include "osapi.h"
#endif

extern int sha1_vector(size_t num_msgs, const uint8_t *msgs[],
                       const size_t *msg_lens, uint8_t *digest);

extern int md5_vector(size_t num_msgs, const uint8_t *msgs[],
                      const size_t *msg_lens, uint8_t *digest);

/* For digest auth. */
void mg_hash_md5_v(size_t num_msgs, const uint8_t *msgs[],
                   const size_t *msg_lens, uint8_t *digest) {
  (void) md5_vector(num_msgs, msgs, msg_lens, digest);
}

/* For WebSocket handshake. */
void mg_hash_sha1_v(size_t num_msgs, const uint8_t *msgs[],
                    const size_t *msg_lens, uint8_t *digest) {
  (void) sha1_vector(num_msgs, msgs, msg_lens, digest);
}
