/*
 * Copyright (c) 2014-2017 Cesanta Software Limited
 * All rights reserved
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
