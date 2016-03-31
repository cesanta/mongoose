/*
 * Copyright (c) 2014-2016 Cesanta Software Limited
 * All rights reserved
 */

#ifndef CS_MONGOOSE_EXAMPLES_CC3200_DATA_H_
#define CS_MONGOOSE_EXAMPLES_CC3200_DATA_H_

#include "mongoose.h"

void data_collect();
void data_init_sensors(int tmp006_addr, int bm222_addr);
void data_conn_handler(struct mg_connection *nc, int ev, void *p);

#endif /* CS_MONGOOSE_EXAMPLES_CC3200_DATA_H_ */
