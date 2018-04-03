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

#include "soc/gpio_reg.h"

void led_setup(int io) {
  if (io < 32) {
    WRITE_PERI_REG(GPIO_ENABLE_W1TS_REG, 1 << io);
  } else {
    WRITE_PERI_REG(GPIO_ENABLE1_W1TS_REG, 1 << (io - 32));
  }
}

void led_on(int io) {
  if (io < 32) {
    WRITE_PERI_REG(GPIO_OUT_W1TS_REG, 1 << io);
  } else {
    WRITE_PERI_REG(GPIO_OUT1_W1TS_REG, 1 << (io - 32));
  }
}

void led_off(int io) {
  if (io < 32) {
    WRITE_PERI_REG(GPIO_OUT_W1TC_REG, 1 << io);
  } else {
    WRITE_PERI_REG(GPIO_OUT1_W1TC_REG, 1 << (io - 32));
  }
}

void led_toggle(int io) {
  if (READ_PERI_REG(GPIO_OUT_REG & (1 << io))) {
    WRITE_PERI_REG(GPIO_OUT_W1TC_REG, 1 << io);
  } else {
    WRITE_PERI_REG(GPIO_OUT_W1TS_REG, 1 << io);
  }
}
