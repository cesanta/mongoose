/*
 * Copyright (c) 2014-2016 Cesanta Software Limited
 * All rights reserved
 */

#ifndef CS_MONGOOSE_EXAMPLES_CC3200_BM222_H_
#define CS_MONGOOSE_EXAMPLES_CC3200_BM222_H_

#include <inttypes.h>
#include <stdbool.h>

#define BM222_NUM_SAMPLES 64
struct bm222_ctx {
  uint8_t addr;
  struct bm222_sample {
    double ts;
    int8_t x;
    int8_t y;
    int8_t z;
  } data[BM222_NUM_SAMPLES];
  int last_index;
};

struct bm222_ctx *bm222_init(uint8_t addr);
bool bm222_fifo_init(struct bm222_ctx *ctx);
bool bm222_get_data(struct bm222_ctx *ctx);

#endif /* CS_MONGOOSE_EXAMPLES_CC3200_BM222_H_ */
