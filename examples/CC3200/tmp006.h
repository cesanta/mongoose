/*
 * Copyright (c) 2014-2016 Cesanta Software Limited
 * All rights reserved
 */

#ifndef CS_MONGOOSE_EXAMPLES_CC3200_TMP006_H_
#define CS_MONGOOSE_EXAMPLES_CC3200_TMP006_H_

#include <inttypes.h>
#include <stdbool.h>

enum tmp006_conversion_rate {
  TMP006_CONV_4 = 0,
  TMP006_CONV_2,
  TMP006_CONV_1,
  TMP006_CONV_1_2,
  TMP006_CONV_1_4,
};

bool tmp006_init(uint8_t addr, enum tmp006_conversion_rate conv_rate,
                 bool drdy_en);
double tmp006_read_sensor_voltage(uint8_t addr);
double tmp006_read_die_temp(uint8_t addr);

#define TMP006_INVALID_READING (-1000.0)

#endif /* CS_MONGOOSE_EXAMPLES_CC3200_TMP006_H_ */
