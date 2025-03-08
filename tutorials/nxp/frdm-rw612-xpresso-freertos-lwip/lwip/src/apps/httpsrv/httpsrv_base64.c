/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "httpsrv_base64.h"

#include <string.h>

static void base64_encodeblock(unsigned char in[3], unsigned char out[4], int len);
static void base64_decodeblock(char in[4], char out[3]);
/*
** base64 decoding alphabet
*/
static const char decode_base64[] = "n###odefghijklm#######0123456789:;<=>?@ABCDEFGHI######JKLMNOPQRSTUVWXYZ[\\]^_`abc";

#define DECODE_BASE64_IDX(ch) (decode_base64[ch - 43] - '0')

/*
** Translation Table as described in RFC1113
*/
static const char cb64[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

/*
** base64_encodeblock
**
** encode 3 8-bit binary bytes as 4 '6-bit' characters
*/
static void base64_encodeblock(unsigned char in[3], unsigned char out[4], int len)
{
    out[0] = (unsigned char)cb64[(int)(in[0] >> 2)];
    out[1] = (unsigned char)cb64[(int)(((in[0] & 0x03) << 4) | ((in[1] & 0xf0) >> 4))];
    out[2] = (unsigned char)(len > 1 ? cb64[(int)(((in[1] & 0x0f) << 2) | ((in[2] & 0xc0) >> 6))] : '=');
    out[3] = (unsigned char)(len > 2 ? cb64[(int)(in[2] & 0x3f)] : '=');
}

char *base64_encode(char *source, char *destination)
{
    unsigned char in[3], out[4];
    int i, n, p, len;
    i = 0;
    n = 0;
    p = 0;

    *destination = '\0';

    while (source[n] != '\0')
    {
        len = 0;
        for (i = 0; i < 3; i++)
        {
            in[i] = 0;
            if (source[n] != '\0')
            {
                in[i] = source[n++];
                len++;
            }
        }

        if (len)
        {
            base64_encodeblock(in, out, len);
            for (i = 0; i < 4; i++)
            {
                destination[p++] = out[i];
            }
        }
    }

    destination[p] = '\0';
    return destination;
}

char *base64_encode_binary(char *source, char *destination, uint32_t length)
{
    unsigned char in[3], out[4];
    int i, n, p, len;
    i = 0;
    n = 0;
    p = 0;

    *destination = '\0';

    while (n < length)
    {
        len = 0;
        for (i = 0; i < 3; i++)
        {
            in[i] = 0;
            if (n != length)
            {
                in[i] = source[n++];
                len++;
            }
        }

        if (len > 0)
        {
            base64_encodeblock(in, out, len);
            for (i = 0; i < 4; i++)
            {
                destination[p++] = out[i];
            }
        }
    }

    destination[p] = '\0';
    return destination;
}

/** Decode base64 single block (4 char string).
 * \param in < 4 char input buffer
 * \param out < 3 char output buffer
 */
static void base64_decodeblock(char in[4], char out[3])
{
    out[0] = (unsigned char)(in[0] << 2 | in[1] >> 4);
    out[1] = (unsigned char)(in[1] << 4 | in[2] >> 2);
    out[2] = (unsigned char)(((in[2] << 6) & 0xc0) | in[3]);
}

char *base64_decode(char *dst, char *src, uint32_t dst_size)
{
    char in[4];
    int i, len;
    uint32_t free_size = dst_size;

    while (*src && free_size)
    {
        for (i = 0, len = 0; i < 4; i++)
        {
            if (*src)
            {
                if (!(*src < 43 || *src > 122) && *src != '=')
                {
                    len++;
                    in[i] = (unsigned char)DECODE_BASE64_IDX(*src);
                }
                else
                {
                    in[i] = 0;
                }
                src++;
            }
            else
            {
                in[i] = 0;
            }
        }

        if ((len > 1) && (free_size > (len - 1)))
        {
            base64_decodeblock(in, dst);
            dst += len - 1;
            *dst = '\0';
            free_size -= (len - 1);
        }
        else
        {
            break;
        }
    }
    return dst;
}

/*
 * Check if string is base64 encoded
 */
bool isbase64(const char *string)
{
    uint32_t i;
    uint32_t length;

    length = strlen(string);
    for (i = 0; i < length; i++)
    {
        if (memchr(cb64, (int)string[i], 64) == NULL)
        {
            if ((string[i] == '=') && ((string[i + 1] != '\0') && ((string[i + 1] == '=') && (string[i + 2] != '\0'))))
            {
                break;
            }
        }
    }
    return ((string[i] == '\0') ? true : false);
}
