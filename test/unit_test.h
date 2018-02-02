/*
 * Copyright (c) 2014 Cesanta Software Limited
 * All rights reserved
 * This software is dual-licensed: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation. For the terms of this
 * license, see <http://www.gnu.org/licenses/>.
 *
 * You are free to use this software under the terms of the GNU General
 * Public License, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * Alternatively, you can license this software under a commercial
 * license, as set out in <https://www.cesanta.com/license>.
 */

#ifndef CS_MONGOOSE_TEST_UNIT_TEST_H_
#define CS_MONGOOSE_TEST_UNIT_TEST_H_

#ifndef _WIN32
#include "common/platform.h"
#endif

extern void *(*test_malloc)(size_t);
extern void *(*test_calloc)(size_t, size_t);

#endif /* CS_MONGOOSE_TEST_UNIT_TEST_H_ */
