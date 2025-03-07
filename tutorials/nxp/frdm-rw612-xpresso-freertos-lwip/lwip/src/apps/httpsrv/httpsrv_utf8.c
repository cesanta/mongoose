/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*
 *   This file contains various UTF-8 functions.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "httpsrv_utf8.h"

static inline bool utf8_check_boundary(uint8_t *position, uint32_t n, uint8_t *max);

/*
 * Check if input sequence is valid UTF-8 sequence.
 */
bool utf8_is_valid(uint8_t *input, uint32_t length, uint8_t **bad, uint32_t *missing)
{
    uint8_t *position;
    uint8_t *max;
    uint32_t n;

    position = input;
    max      = input + length;
    *missing = 0;

    while (position < max)
    {
        n = 0;

        if (utf8_check_1(position))
        {
            position++;
            continue;
        }
        n++;
        if (utf8_check_boundary(position, n, max))
        {
            *missing = 4 - n;
            goto BOUNDARY;
        }

        if (utf8_check_2(position))
        {
            position += n + 1;
            continue;
        }
        n++;
        if (utf8_check_boundary(position, n, max))
        {
            *missing = 4 - n;
            goto BOUNDARY;
        }
        if (utf8_check_3(position))
        {
            position += n + 1;
            continue;
        }
        n++;
        if (utf8_check_boundary(position, n, max))
        {
            *missing = 4 - n;
            goto BOUNDARY;
        }
        if (utf8_check_4(position))
        {
            position += n + 1;
            continue;
        }

    BOUNDARY:
        *bad = position;
        return (false);
    }

    *bad = NULL;
    return (true);
}

/* Check if next step would break array boundary. */
static inline bool utf8_check_boundary(uint8_t *position, uint32_t n, uint8_t *max)
{
    if ((position + n) >= max)
    {
        return (true);
    }
    return (false);
}
