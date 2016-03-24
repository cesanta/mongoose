/*
 * Copyright (c) 2014-2016 Cesanta Software Limited
 * All rights reserved
 */

#include "bm222.h"

#include "mongoose.h"

#include "i2c_if.h"

#define BM222_REG_FIFO_STATUS 0x0e

#define BM222_REG_PMU_BW 0x10
#define BM222_PMU_BW_7_81_HZ 0x8
#define BM222_PMU_BW_31_25_HZ 0xA
#define BM222_PMU_BW_62_5_HZ 0xB
#define BM222_PMU_BW_125_HZ 0xC

#define BM222_REG_BGW_SOFTRESET 0x14
#define BM222_DO_SOFT_RESET 0xB6

#define BM222_REG_FIFO_CONFIG_1 0x3e
#define BM222_FIFO_MODE_FIFO 0x40
#define BM222_FIFO_DATA_ALL 0

#define BM222_REG_FIFO_DATA 0x3f

struct bm222_ctx *bm222_init(uint8_t addr) {
  struct bm222_ctx *ctx = (struct bm222_ctx *) calloc(1, sizeof(*ctx));
  if (ctx == NULL) return NULL;
  ctx->addr = addr;
  {
    unsigned char val[2] = {BM222_REG_BGW_SOFTRESET, BM222_DO_SOFT_RESET};
    if (I2C_IF_Write(addr, val, 2, 1) != 0) goto out_err;
    osi_Sleep(2 /* ms */); /* t_w,up1 = 1.8 ms */
  }
  if (!bm222_fifo_init(ctx)) return false;
  {
    unsigned char val[2] = {BM222_REG_PMU_BW, BM222_PMU_BW_125_HZ};
    if (I2C_IF_Write(addr, val, 2, 1) != 0) goto out_err;
  }
  return ctx;
out_err:
  free(ctx);
  return NULL;
}

bool bm222_fifo_init(struct bm222_ctx *ctx) {
  unsigned char val[2] = {BM222_REG_FIFO_CONFIG_1,
                          BM222_FIFO_MODE_FIFO | BM222_FIFO_DATA_ALL};
  return (I2C_IF_Write(ctx->addr, val, 2, 1) == 0);
}

bool bm222_get_data(struct bm222_ctx *ctx) {
  unsigned char reg = BM222_REG_FIFO_STATUS;
  unsigned char val;
  if (I2C_IF_ReadFrom(ctx->addr, &reg, 1, &val, 1) < 0) return false;
  unsigned char len = (val & 0x7f);
  unsigned char overflow = (val & 0x80 ? 1 : 0);
  LOG(LL_DEBUG, ("FIFO len: %d (ovf? %d)", len, overflow));
  reg = BM222_REG_FIFO_DATA;
  int8_t fifo[32 * 6], *v = fifo;
  if (I2C_IF_ReadFrom(ctx->addr, &reg, 1, (unsigned char *) fifo, len * 6) <
      0) {
    return false;
  }
  double now = mg_time();
  /* Of potentially multiple samples, pick one with maximum difference. */
  int32_t max_d = 0;
  bool sampled = false;
  struct bm222_sample *ls = ctx->data + ctx->last_index, *s = NULL;
  if (ls->ts == 0) {
    /* The very first sample. */
    ls->ts = now;
    ls->x = v[1];
    ls->y = v[3];
    ls->z = v[5];
    v += 6;
    len--;
    sampled = true;
    s = ls;
  }
  for (; len > 0; v += 6, len--) {
    int32_t dx = ((int32_t) v[1]) - ((int32_t) ls->x);
    int32_t dy = ((int32_t) v[3]) - ((int32_t) ls->y);
    int32_t dz = ((int32_t) v[5]) - ((int32_t) ls->z);
    int32_t d = dx * dx + dy * dy + dz * dz;
    if ((d > 2 && d > max_d) || (!sampled && now - ls->ts > 1.0)) {
      if (!sampled) {
        ctx->last_index = (ctx->last_index + 1) % BM222_NUM_SAMPLES;
        s = ctx->data + ctx->last_index;
        sampled = true;
      }
      s->ts = now;
      s->x = v[1];
      s->y = v[3];
      s->z = v[5];
      if (d > max_d) max_d = d;
      LOG(LL_VERBOSE_DEBUG, ("dx %d dy %d dz %d d %d", dx, dy, dz, d));
    }
  }
  return (overflow ? bm222_fifo_init(ctx) : true); /* Clear the ovf flag. */
}
