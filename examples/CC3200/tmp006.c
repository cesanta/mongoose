/*
 * Copyright (c) 2014-2016 Cesanta Software Limited
 * All rights reserved
 */

#include "tmp006.h"

#include "mongoose.h"

#include "i2c_if.h"

#define TMP006_REG_SENSOR_VOLTAGE 0x00
#define TMP006_REG_DIE_TEMP 0x01
#define TMP006_REG_CONFIG 0x02

bool tmp006_init(uint8_t addr, enum tmp006_conversion_rate conv_rate,
                 bool drdy_en) {
  unsigned char val[3] = {TMP006_REG_CONFIG, 0x80, 0};
  /* Reset first */
  if (I2C_IF_Write(addr, val, 3, 1) != 0) return false;
  val[1] = 0x70 | (conv_rate << 1) | drdy_en;
  return (I2C_IF_Write(addr, val, 3, 1) == 0);
}

double tmp006_read_sensor_voltage(uint8_t addr) {
  unsigned char reg = TMP006_REG_SENSOR_VOLTAGE;
  unsigned char val[2] = {0, 0};
  int status = I2C_IF_ReadFrom(addr, &reg, 1, val, 2);
  if (status < 0) return -1000;
  int voltage = (val[0] << 8) | val[1];
  if (val[0] & 0x80) voltage = -((1 << 16) - voltage);
  return voltage * 0.00015625;
}

double tmp006_read_die_temp(uint8_t addr) {
  unsigned char reg = TMP006_REG_DIE_TEMP;
  unsigned char val[2] = {0, 0};
  int status = I2C_IF_ReadFrom(addr, &reg, 1, val, 2);
  if (status < 0) return -1000;
  int temp = (val[0] << 6) | (val[1] >> 2);
  if (val[0] & 0x80) temp = -((1 << 14) - temp);
  return temp / 32.0;
}
