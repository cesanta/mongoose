/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef __utf8_h__
#define __utf8_h__

#include <stdint.h>
#include <stdbool.h>

#define UTF8_TAIL_MIN 0x80
#define UTF8_TAIL_MAX 0xBF

#ifdef __cplusplus
extern "C" {
#endif

bool utf8_is_valid(uint8_t *input, uint32_t length, uint8_t **bad, uint32_t *missing);

static inline bool utf8_check_1(uint8_t *data);
static inline bool utf8_check_2(uint8_t *data);
static inline bool utf8_check_3(uint8_t *data);
static inline bool utf8_check_4(uint8_t *data);

/* Check 1 byte for validity. */
static inline bool utf8_check_1(uint8_t *data)
{
    if ((data[0] == 0x09) || (data[0] == 0x0A) || (data[0] == 0x0D) || (data[0] <= 0x7F))
    {
        return (true);
    }
    return (false);
}

/* Check 2 bytes for validity. */
static inline bool utf8_check_2(uint8_t *data)
{
    if (((data[0] >= 0xC2) && (data[0] <= 0xDF)) && ((data[1] >= 0x80) && (data[1] <= 0xBF)))
    {
        return (true);
    }
    return (false);
}

/* Check 3 bytes for validity. */
static inline bool utf8_check_3(uint8_t *data)
{
    if ((/* Excluding overlongs. */
         (data[0] == 0xE0) && ((data[1] >= 0xA0) && (data[1] <= 0xBF)) && ((data[2] >= 0x80) && (data[2] <= 0xBF))) ||
        (/* Straight 3-byte. */
         (((data[0] >= 0xE1) && (data[0] <= 0xEC)) || (data[0] == 0xEE) || (data[0] == 0xEF)) &&
         ((data[1] >= 0x80) && (data[1] <= 0xBF)) && ((data[2] >= 0x80) && (data[2] <= 0xBF))) ||
        (/* Excluding surrogates. */
         (data[0] == 0xED) && ((data[1] >= 0x80) && (data[1] <= 0x9F)) && ((data[2] >= 0x80) && (data[2] <= 0xBF))))
    {
        return (true);
    }
    return (false);
}

/* Check 4 bytes for validity. */
static inline bool utf8_check_4(uint8_t *data)
{
    if ((/* Planes 1-3. */
         (data[0] == 0xF0) && ((data[1] >= 0x90) && (data[1] <= 0xBF)) && ((data[2] >= 0x80) && (data[2] <= 0xBF)) &&
         ((data[3] >= 0x80) && (data[3] <= 0xBF))) ||
        (/* Planes 4-15. */
         ((data[0] >= 0xF1) && (data[0] <= 0xF3)) && ((data[1] >= 0x80) && (data[1] <= 0xBF)) &&
         ((data[2] >= 0x80) && (data[2] <= 0xBF)) && ((data[3] >= 0x80) && (data[3] <= 0xBF))) ||
        (/* Plane 16. */
         (data[0] == 0xF4) && ((data[1] >= 0x80) && (data[1] <= 0x8F)) && ((data[2] >= 0x80) && (data[2] <= 0xBF)) &&
         ((data[3] >= 0x80) && (data[3] <= 0xBF))))
    {
        return (true);
    }
    return (false);
}

#ifdef __cplusplus
}
#endif

#endif
