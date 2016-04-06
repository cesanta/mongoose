/*
 * Copyright (c) 2014-2016 Cesanta Software Limited
 * All rights reserved
 */

#ifndef CS_MONGOOSE_EXAMPLES_CC3200_MG_TASK_H_
#define CS_MONGOOSE_EXAMPLES_CC3200_MG_TASK_H_

#include <stdbool.h>
#include <stdlib.h>

#include <oslib/osi.h>
#include "mongoose.h"

typedef void (*mg_init_cb)(struct mg_mgr *mgr);
bool mg_start_task(int priority, int stack_size, mg_init_cb mg_init);

void mg_run_in_task(void (*cb)(struct mg_mgr *mgr, void *arg), void *cb_arg);

#endif /* CS_MONGOOSE_EXAMPLES_CC3200_MG_TASK_H_ */
