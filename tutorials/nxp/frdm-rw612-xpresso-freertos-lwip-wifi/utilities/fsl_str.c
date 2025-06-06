/*
 * Copyright 2017, 2020, 2022-2023 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include <math.h>
#include <stdarg.h>
#include <stdlib.h>
#include <errno.h> /* MISRA C-2012 Rule 22.9 */
#include "fsl_str.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*! @brief The overflow value.*/
#ifndef HUGE_VAL
#define HUGE_VAL (99.e99)
#endif /* HUGE_VAL */

#ifndef MAX_FIELD_WIDTH
#define MAX_FIELD_WIDTH 99U
#endif

/*! @brief Keil: suppress ellipsis warning in va_arg usage below. */
#if defined(__CC_ARM)
#pragma diag_suppress 1256
#endif /* __CC_ARM */

#if (defined(PRINTF_ADVANCED_ENABLE) && (PRINTF_ADVANCED_ENABLE > 0U))
#define STR_FORMAT_PRINTF_UVAL_TYPE unsigned long long int
#define STR_FORMAT_PRINTF_IVAL_TYPE long long int
#else
#define STR_FORMAT_PRINTF_UVAL_TYPE unsigned int
#define STR_FORMAT_PRINTF_IVAL_TYPE int
#endif
/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*!
 * @brief Scanline function which ignores white spaces.
 *
 * @param[in]   s The address of the string pointer to update.
 * @return      String without white spaces.
 */
static uint32_t ScanIgnoreWhiteSpace(const char **s);

/*!
 * @brief Converts a radix number to a string and return its length.
 *
 * @param[in] numstr    Converted string of the number.
 * @param[in] nump      Pointer to the number.
 * @param[in] neg       Polarity of the number.
 * @param[in] radix     The radix to be converted to.
 * @param[in] use_caps  Used to identify %x/X output format.

 * @return Length of the converted string.
 */
static int32_t ConvertRadixNumToString(char *numstr, void *nump, unsigned int neg, unsigned int radix, bool use_caps);

#if (defined(PRINTF_FLOAT_ENABLE) && (PRINTF_FLOAT_ENABLE > 0U))
/*!
 * @brief Converts a floating radix number to a string and return its length.
 *
 * @param[in] numstr            Converted string of the number.
 * @param[in] nump              Pointer to the number.
 * @param[in] radix             The radix to be converted to.
 * @param[in] precision_width   Specify the precision width.

 * @return Length of the converted string.
 */
static int32_t ConvertFloatRadixNumToString(char *numstr, void *nump, int32_t radix, uint32_t precision_width);

#endif /* PRINTF_FLOAT_ENABLE */

/*************Code for process formatted data*******************************/
#if (defined(PRINTF_ADVANCED_ENABLE) && (PRINTF_ADVANCED_ENABLE > 0U))
static uint8_t PrintGetSignChar(long long int ival, uint32_t flags_used, char *schar)
{
    uint8_t len = 1U;
    if (ival < 0)
    {
        *schar = '-';
    }
    else
    {
        if (0U != (flags_used & (uint32_t)kPRINTF_Plus))
        {
            *schar = '+';
        }
        else if (0U != (flags_used & (uint32_t)kPRINTF_Space))
        {
            *schar = ' ';
        }
        else
        {
            *schar = '\0';
            len    = 0U;
        }
    }
    return len;
}
#endif /* PRINTF_ADVANCED_ENABLE */

static uint32_t PrintGetWidth(const char **p, va_list *ap)
{
    uint32_t field_width = 0;
    uint8_t done         = 0U;
    char c;

    while (0U == done)
    {
        c = *(++(*p));
        if ((c >= '0') && (c <= '9'))
        {
            (field_width) = ((field_width)*10U) + ((uint32_t)c - (uint32_t)'0');
        }
#if (defined(PRINTF_ADVANCED_ENABLE) && (PRINTF_ADVANCED_ENABLE > 0U))
        else if (c == '*')
        {
            (field_width) = (uint32_t)va_arg(*ap, uint32_t);
        }
#endif /* PRINTF_ADVANCED_ENABLE */
        else
        {
            /* We've gone one char too far. */
            --(*p);
            done = 1U;
        }
    }
    return field_width;
}

static uint32_t PrintGetPrecision(const char **s, va_list *ap, bool *valid_precision_width)
{
    const char *p            = *s;
    uint32_t precision_width = 6U;
    uint8_t done             = 0U;

#if (defined(PRINTF_ADVANCED_ENABLE) && (PRINTF_ADVANCED_ENABLE > 0U))
    if (NULL != valid_precision_width)
    {
        *valid_precision_width = false;
    }
#endif /* PRINTF_ADVANCED_ENABLE */
    if (*++p == '.')
    {
        /* Must get precision field width, if present. */
        precision_width = 0U;
        done            = 0U;
        while (0U == done)
        {
            char c = *++p;
            if ((c >= '0') && (c <= '9'))
            {
                precision_width = (precision_width * 10U) + ((uint32_t)c - (uint32_t)'0');
#if (defined(PRINTF_ADVANCED_ENABLE) && (PRINTF_ADVANCED_ENABLE > 0U))
                if (NULL != valid_precision_width)
                {
                    *valid_precision_width = true;
                }
#endif /* PRINTF_ADVANCED_ENABLE */
            }
#if (defined(PRINTF_ADVANCED_ENABLE) && (PRINTF_ADVANCED_ENABLE > 0U))
            else if (c == '*')
            {
                precision_width = (uint32_t)va_arg(*ap, uint32_t);
                if (NULL != valid_precision_width)
                {
                    *valid_precision_width = true;
                }
            }
#endif /* PRINTF_ADVANCED_ENABLE */
            else
            {
                /* We've gone one char too far. */
                --p;
                done = 1U;
            }
        }
    }
    else
    {
        /* We've gone one char too far. */
        --p;
    }
    *s = p;
    return precision_width;
}

static uint32_t PrintIsobpu(const char c)
{
    uint32_t ret = 0U;
    if ((c == 'o') || (c == 'b') || (c == 'p') || (c == 'u'))
    {
        ret = 1U;
    }
    return ret;
}

static uint32_t PrintIsdi(const char c)
{
    uint32_t ret = 0U;
    if ((c == 'd') || (c == 'i'))
    {
        ret = 1U;
    }
    return ret;
}

static void PrintOutputdifFobpu(uint32_t flags_used,
                                uint32_t field_width,
                                uint32_t vlen,
                                char schar,
                                char *vstrp,
                                printfCb cb,
                                char *buf,
                                int32_t *count)
{
#if (defined(PRINTF_ADVANCED_ENABLE) && (PRINTF_ADVANCED_ENABLE > 0U))
    /* Do the ZERO pad. */
    if (0U != (flags_used & (uint32_t)kPRINTF_Zero))
    {
        if ('\0' != schar)
        {
            cb(buf, count, schar, 1);
            schar = '\0';
        }
        cb(buf, count, '0', (int)field_width - (int)vlen);
        vlen = field_width;
    }
    else
    {
        if (0U == (flags_used & (uint32_t)kPRINTF_Minus))
        {
            cb(buf, count, ' ', (int)field_width - (int)vlen);
            if ('\0' != schar)
            {
                cb(buf, count, schar, 1);
                schar = '\0';
            }
        }
    }
    /* The string was built in reverse order, now display in correct order. */
    if ('\0' != schar)
    {
        cb(buf, count, schar, 1);
    }
#else
    cb(buf, count, ' ', (int)field_width - (int)vlen);
#endif /* PRINTF_ADVANCED_ENABLE */
    while ('\0' != (*vstrp))
    {
        cb(buf, count, *vstrp--, 1);
    }
#if (defined(PRINTF_ADVANCED_ENABLE) && (PRINTF_ADVANCED_ENABLE > 0U))
    if (0U != (flags_used & (uint32_t)kPRINTF_Minus))
    {
        cb(buf, count, ' ', (int)field_width - (int)vlen);
    }
#endif /* PRINTF_ADVANCED_ENABLE */
}

static void PrintOutputxX(uint32_t flags_used,
                          uint32_t field_width,
                          uint32_t vlen,
                          bool use_caps,
                          char *vstrp,
                          printfCb cb,
                          char *buf,
                          int32_t *count)
{
#if (defined(PRINTF_ADVANCED_ENABLE) && (PRINTF_ADVANCED_ENABLE > 0U))
    uint8_t dschar = 0;
    if (0U != (flags_used & (uint32_t)kPRINTF_Zero))
    {
        if (0U != (flags_used & (uint32_t)kPRINTF_Pound))
        {
            cb(buf, count, '0', 1);
            cb(buf, count, (use_caps ? 'X' : 'x'), 1);
            dschar = 1U;
        }
        cb(buf, count, '0', (int)field_width - (int)vlen);
        vlen = field_width;
    }
    else
    {
        if (0U == (flags_used & (uint32_t)kPRINTF_Minus))
        {
            if (0U != (flags_used & (uint32_t)kPRINTF_Pound))
            {
                vlen += 2U;
            }
            cb(buf, count, ' ', (int)field_width - (int)vlen);
            if (0U != (flags_used & (uint32_t)kPRINTF_Pound))
            {
                cb(buf, count, '0', 1);
                cb(buf, count, (use_caps ? 'X' : 'x'), 1);
                dschar = 1U;
            }
        }
    }

    if ((0U != (flags_used & (uint32_t)kPRINTF_Pound)) && (0U == dschar))
    {
        cb(buf, count, '0', 1);
        cb(buf, count, (use_caps ? 'X' : 'x'), 1);
        vlen += 2U;
    }
#else
    cb(buf, count, ' ', (int)field_width - (int)vlen);
#endif /* PRINTF_ADVANCED_ENABLE */
    while ('\0' != (*vstrp))
    {
        cb(buf, count, *vstrp--, 1);
    }
#if (defined(PRINTF_ADVANCED_ENABLE) && (PRINTF_ADVANCED_ENABLE > 0U))
    if (0U != (flags_used & (uint32_t)kPRINTF_Minus))
    {
        cb(buf, count, ' ', (int)field_width - (int)vlen);
    }
#endif /* PRINTF_ADVANCED_ENABLE */
}

static uint32_t PrintIsfF(const char c)
{
    uint32_t ret = 0U;
    if ((c == 'f') || (c == 'F'))
    {
        ret = 1U;
    }
    return ret;
}

static uint32_t PrintIsxX(const char c)
{
    uint32_t ret = 0U;
    if ((c == 'x') || (c == 'X'))
    {
        ret = 1U;
    }
    return ret;
}

#if (defined(PRINTF_ADVANCED_ENABLE) && (PRINTF_ADVANCED_ENABLE > 0U))
static uint32_t PrintCheckFlags(const char **s)
{
    const char *p = *s;
    /* First check for specification modifier flags. */
    uint32_t flags_used = 0U;
    bool done           = false;
    while (false == done)
    {
        switch (*++p)
        {
            case '-':
                flags_used |= (uint32_t)kPRINTF_Minus;
                break;
            case '+':
                flags_used |= (uint32_t)kPRINTF_Plus;
                break;
            case ' ':
                flags_used |= (uint32_t)kPRINTF_Space;
                break;
            case '0':
                flags_used |= (uint32_t)kPRINTF_Zero;
                break;
            case '#':
                flags_used |= (uint32_t)kPRINTF_Pound;
                break;
            default:
                /* We've gone one char too far. */
                --p;
                done = true;
                break;
        }
    }
    *s = p;
    return flags_used;
}
#endif /* PRINTF_ADVANCED_ENABLE */

#if (defined(PRINTF_ADVANCED_ENABLE) && (PRINTF_ADVANCED_ENABLE > 0U))
/*
 * Check for the length modifier.
 */
static uint32_t PrintGetLengthFlag(const char **s)
{
    const char *p = *s;
    /* First check for specification modifier flags. */
    uint32_t flags_used = 0U;

    switch (/* c = */ *++p)
    {
        case 'h':
            if (*++p != 'h')
            {
                flags_used |= (uint32_t)kPRINTF_LengthShortInt;
                --p;
            }
            else
            {
                flags_used |= (uint32_t)kPRINTF_LengthChar;
            }
            break;
        case 'l':
            if (*++p != 'l')
            {
                flags_used |= (uint32_t)kPRINTF_LengthLongInt;
                --p;
            }
            else
            {
                flags_used |= (uint32_t)kPRINTF_LengthLongLongInt;
            }
            break;
        case 'z':
            if (sizeof(size_t) == sizeof(uint32_t))
            {
                flags_used |= (uint32_t)kPRINTF_LengthLongInt;
            }
            else if (sizeof(size_t) == (2U * sizeof(uint32_t)))
            {
                flags_used |= (uint32_t)kPRINTF_LengthLongLongInt;
            }
            else if (sizeof(size_t) == sizeof(uint16_t))
            {
                flags_used |= (uint32_t)kPRINTF_LengthShortInt;
            }
            else
            {
                /* MISRA C-2012 Rule 15.7 */
            }
            break;
        default:
            /* we've gone one char too far */
            --p;
            break;
    }
    *s = p;
    return flags_used;
}
#else
static void PrintFilterLengthFlag(const char **s)
{
    const char *p = *s;
    char strCh;

    do
    {
        strCh = *++p;
    } while ((strCh == 'h') || (strCh == 'l'));

    *s = --p;
}
#endif /* PRINTF_ADVANCED_ENABLE */

static uint8_t PrintGetRadixFromobpu(const char c)
{
    uint8_t radix;

    if (c == 'o')
    {
        radix = 8U;
    }
    else if (c == 'b')
    {
        radix = 2U;
    }
    else if (c == 'p')
    {
        radix = 16U;
    }
    else
    {
        radix = 10U;
    }
    return radix;
}

static uint32_t ScanIsWhiteSpace(const char c)
{
    uint32_t ret = 0U;
    if ((c == ' ') || (c == '\t') || (c == '\n') || (c == '\r') || (c == '\v') || (c == '\f'))
    {
        ret = 1U;
    }
    return ret;
}

static uint32_t ScanIgnoreWhiteSpace(const char **s)
{
    uint32_t count = 0U;
    char c;

    c = **s;
    while (1U == ScanIsWhiteSpace(c))
    {
        count++;
        (*s)++;
        c = **s;
    }
    return count;
}

static int32_t ConvertRadixNumToString(char *numstr, void *nump, unsigned int neg, unsigned int radix, bool use_caps)
{
#if (defined(PRINTF_ADVANCED_ENABLE) && (PRINTF_ADVANCED_ENABLE > 0U))
    long long int a;
    long long int b;
    long long int c;

    unsigned long long int ua;
    unsigned long long int ub;
    unsigned long long int uc;
    unsigned long long int uc_param;
#else
    int a;
    int b;
    int c;

    unsigned int ua;
    unsigned int ub;
    unsigned int uc;
    unsigned int uc_param;
#endif /* PRINTF_ADVANCED_ENABLE */

    int32_t nlen;
    char *nstrp;

    nlen     = 0;
    nstrp    = numstr;
    *nstrp++ = '\0';

#if !(defined(PRINTF_ADVANCED_ENABLE) && (PRINTF_ADVANCED_ENABLE > 0u))
    neg = 0U;
#endif

#if (defined(PRINTF_ADVANCED_ENABLE) && (PRINTF_ADVANCED_ENABLE > 0U))
    a        = 0;
    b        = 0;
    c        = 0;
    ua       = 0ULL;
    ub       = 0ULL;
    uc       = 0ULL;
    uc_param = 0ULL;
#else
    a        = 0;
    b        = 0;
    c        = 0;
    ua       = 0U;
    ub       = 0U;
    uc       = 0U;
    uc_param = 0U;
#endif /* PRINTF_ADVANCED_ENABLE */

    (void)a;
    (void)b;
    (void)c;
    (void)ua;
    (void)ub;
    (void)uc;
    (void)uc_param;
    (void)neg;
    /*
     * Fix MISRA issue: CID 15972928 (#15 of 15): MISRA C-2012 Control Flow Expressions (MISRA C-2012 Rule 14.3)
     * misra_c_2012_rule_14_3_violation: Execution cannot reach this statement: a = *((int *)nump);
     */
#if (defined(PRINTF_ADVANCED_ENABLE) && (PRINTF_ADVANCED_ENABLE > 0U))
    if (0U != neg)
    {
#if (defined(PRINTF_ADVANCED_ENABLE) && (PRINTF_ADVANCED_ENABLE > 0U))
        a = *(long long int *)nump;
#else
        a = *(int *)nump;
#endif /* PRINTF_ADVANCED_ENABLE */
        if (a == 0)
        {
            *nstrp = '0';
            ++nlen;
            return nlen;
        }
        while (a != 0)
        {
#if (defined(PRINTF_ADVANCED_ENABLE) && (PRINTF_ADVANCED_ENABLE > 0U))
            b = (long long int)a / (long long int)radix;
            c = (long long int)a - ((long long int)b * (long long int)radix);
            if (c < 0)
            {
                uc       = (unsigned long long int)c;
                uc_param = ~uc;
                c        = (long long int)uc_param + 1 + (long long int)'0';
            }
#else
            b = (int)a / (int)radix;
            c = (int)a - ((int)b * (int)radix);
            if (c < 0)
            {
                uc       = (unsigned int)c;
                uc_param = ~uc;
                c        = (int)uc_param + 1 + (int)'0';
            }
#endif /* PRINTF_ADVANCED_ENABLE */
            else
            {
                c = c + (int)'0';
            }
            a        = b;
            *nstrp++ = (char)c;
            ++nlen;
        }
    }
    else
#endif /* PRINTF_ADVANCED_ENABLE */
    {
#if (defined(PRINTF_ADVANCED_ENABLE) && (PRINTF_ADVANCED_ENABLE > 0U))
        ua = *(unsigned long long int *)nump;
#else
        ua = *(unsigned int *)nump;
#endif /* PRINTF_ADVANCED_ENABLE */
        if (ua == 0U)
        {
            *nstrp = '0';
            ++nlen;
            return nlen;
        }
        while (ua != 0U)
        {
#if (defined(PRINTF_ADVANCED_ENABLE) && (PRINTF_ADVANCED_ENABLE > 0U))
            ub = (unsigned long long int)ua / (unsigned long long int)radix;
            uc = (unsigned long long int)ua - ((unsigned long long int)ub * (unsigned long long int)radix);
#else
            ub = ua / (unsigned int)radix;
            uc = ua - (ub * (unsigned int)radix);
#endif /* PRINTF_ADVANCED_ENABLE */

            if (uc < 10U)
            {
                uc = uc + (unsigned int)'0';
            }
            else
            {
                uc = uc - 10U + (unsigned int)(use_caps ? 'A' : 'a');
            }
            ua       = ub;
            *nstrp++ = (char)uc;
            ++nlen;
        }
    }
    return nlen;
}

#if (defined(PRINTF_FLOAT_ENABLE) && (PRINTF_FLOAT_ENABLE > 0U))
static int32_t ConvertFloatRadixNumToString(char *numstr, void *nump, int32_t radix, uint32_t precision_width)
{
    int32_t a;
    int32_t b;
    int32_t c;
    int32_t i;
    uint32_t uc;
    double fa;
    double dc;
    double fb;
    double r;
    double fractpart;
    double intpart;

    int32_t nlen;
    char *nstrp;
    nlen     = 0;
    nstrp    = numstr;
    *nstrp++ = '\0';
    r        = *(double *)nump;
    if (0.0 == r)
    {
        *nstrp = '0';
        ++nlen;
        return nlen;
    }
    fractpart = modf((double)r, (double *)&intpart);
    /* Process fractional part. */
    for (i = 0; i < (int32_t)precision_width; i++)
    {
        fractpart *= (double)radix;
    }
    if (r >= (double)0.0)
    {
        fa = fractpart + (double)0.5;
        if (fa >= pow((double)10, (double)precision_width))
        {
            intpart++;
        }
    }
    else
    {
        fa = fractpart - (double)0.5;
        if (fa <= -pow((double)10, (double)precision_width))
        {
            intpart--;
        }
    }
    for (i = 0; i < (int32_t)precision_width; i++)
    {
        fb = fa / (double)radix;
        dc = (fa - (double)(long long int)fb * (double)radix);
        c  = (int32_t)dc;
        if (c < 0)
        {
            uc = (uint32_t)c;
            uc = ~uc;
            c  = (int32_t)uc;
            c += (int32_t)1;
            c += (int32_t)'0';
        }
        else
        {
            c = c + '0';
        }
        fa       = fb;
        *nstrp++ = (char)c;
        ++nlen;
    }
    *nstrp++ = (char)'.';
    ++nlen;
    a = (int32_t)intpart;
    if (a == 0)
    {
        *nstrp++ = '0';
        ++nlen;
    }
    else
    {
        while (a != 0)
        {
            b = (int32_t)a / (int32_t)radix;
            c = (int32_t)a - ((int32_t)b * (int32_t)radix);
            if (c < 0)
            {
                uc = (uint32_t)c;
                uc = ~uc;
                c  = (int32_t)uc;
                c += (int32_t)1;
                c += (int32_t)'0';
            }
            else
            {
                c = c + '0';
            }
            a        = b;
            *nstrp++ = (char)c;
            ++nlen;
        }
    }
    return nlen;
}
#endif /* PRINTF_FLOAT_ENABLE */

#if (defined(PRINTF_ADVANCED_ENABLE) && (PRINTF_ADVANCED_ENABLE > 0U))
static void StrFormatExaminedi(uint32_t *flags_used, long long int *ival, va_list *ap)
#else
static void StrFormatExaminedi(int *ival, va_list *ap)
#endif
{
#if (defined(PRINTF_ADVANCED_ENABLE) && (PRINTF_ADVANCED_ENABLE > 0U))
    if (0U != (*flags_used & (uint32_t)kPRINTF_LengthLongLongInt))
    {
        *ival = (long long int)va_arg(*ap, long long int);
    }
    else if (0U != (*flags_used & (uint32_t)kPRINTF_LengthLongInt))
    {
        *ival = (long long int)va_arg(*ap, long int);
    }
    else
#endif /* PRINTF_ADVANCED_ENABLE */
    {
        *ival = (STR_FORMAT_PRINTF_IVAL_TYPE)va_arg(*ap, int);
    }
}

#if (defined(PRINTF_ADVANCED_ENABLE) && (PRINTF_ADVANCED_ENABLE > 0U))
static void StrFormatExaminexX(uint32_t *flags_used, unsigned long long int *uval, va_list *ap)
#else
static void StrFormatExaminexX(unsigned int *uval, va_list *ap)
#endif
{
#if (defined(PRINTF_ADVANCED_ENABLE) && (PRINTF_ADVANCED_ENABLE > 0U))
    if (0U != (*flags_used & (unsigned int)kPRINTF_LengthLongLongInt))
    {
        *uval = (unsigned long long int)va_arg(*ap, unsigned long long int);
    }
    else if (0U != (*flags_used & (unsigned int)kPRINTF_LengthLongInt))
    {
        *uval = (unsigned long long int)va_arg(*ap, unsigned long int);
    }
    else
#endif /* PRINTF_ADVANCED_ENABLE */
    {
        *uval = (STR_FORMAT_PRINTF_UVAL_TYPE)va_arg(*ap, unsigned int);
    }
}

#if (defined(PRINTF_ADVANCED_ENABLE) && (PRINTF_ADVANCED_ENABLE > 0U))
static void StrFormatExamineobpu(uint32_t *flags_used, unsigned long long int *uval, va_list *ap)
#else
static void StrFormatExamineobpu(unsigned int *uval, va_list *ap)
#endif
{
#if (defined(PRINTF_ADVANCED_ENABLE) && (PRINTF_ADVANCED_ENABLE > 0U))
    if (0U != (*flags_used & (unsigned int)kPRINTF_LengthLongLongInt))
    {
        *uval = (unsigned long long int)va_arg(*ap, unsigned long long int);
    }
    else if (0U != (*flags_used & (unsigned int)kPRINTF_LengthLongInt))
    {
        *uval = (unsigned long long int)va_arg(*ap, unsigned long int);
    }
    else
#endif /* PRINTF_ADVANCED_ENABLE */
    {
        *uval = (STR_FORMAT_PRINTF_UVAL_TYPE)va_arg(*ap, unsigned int);
    }
}
#if (defined(PRINTF_ADVANCED_ENABLE) && (PRINTF_ADVANCED_ENABLE > 0U))
static int32_t ConvertPrecisionWidthToLength(bool valid_precision_width, uint32_t precision_width, char *sval)
#else
static int32_t ConvertPrecisionWidthToLength(char *sval)
#endif
{
    int32_t vlen = 0;
#if (defined(PRINTF_ADVANCED_ENABLE) && (PRINTF_ADVANCED_ENABLE > 0U))
    if (valid_precision_width)
    {
        vlen = (int)precision_width;
    }
    else
    {
        vlen = (int)strlen(sval);
    }
#else
    vlen = (int32_t)strlen(sval);
#endif /* PRINTF_ADVANCED_ENABLE */

    return vlen;
}

/*!
 * brief This function outputs its parameters according to a formatted string.
 *
 * note I/O is performed by calling given function pointer using following
 * (*func_ptr)(c);
 *
 * param[in] fmt   Format string for printf.
 * param[in] ap    Arguments to printf.
 * param[in] buf  pointer to the buffer
 * param cb print callback function pointer
 *
 * return Number of characters to be print
 */
int StrFormatPrintf(const char *fmt, va_list ap, char *buf, printfCb cb)
{
    /* va_list ap; */
    const char *p;
    char c;

    char vstr[33];
    char *vstrp  = NULL;
    int32_t vlen = 0;

    int32_t count = 0;

    uint32_t field_width;
    uint32_t precision_width;
    char *sval;
    int32_t cval;
    bool use_caps;
    unsigned int radix = 0;

#if (defined(PRINTF_ADVANCED_ENABLE) && (PRINTF_ADVANCED_ENABLE > 0U))
    uint32_t flags_used;
    char schar;
    long long int ival;
    unsigned long long int uval = 0;
    bool valid_precision_width;
#else
    int ival;
    unsigned int uval = 0;
#endif /* PRINTF_ADVANCED_ENABLE */

#if (defined(PRINTF_FLOAT_ENABLE) && (PRINTF_FLOAT_ENABLE > 0U))
    double fval;
#endif /* PRINTF_FLOAT_ENABLE */

    /* Start parsing apart the format string and display appropriate formats and data. */
    p = fmt;
    while (true)
    {
        if ('\0' == *p)
        {
            break;
        }
        c = *p;
        /*
         * All formats begin with a '%' marker.  Special chars like
         * '\n' or '\t' are normally converted to the appropriate
         * character by the __compiler__.  Thus, no need for this
         * routine to account for the '\' character.
         */
        if (c != '%')
        {
            cb(buf, &count, c, 1);
            p++;
            /* By using 'continue', the next iteration of the loop is used, skipping the code that follows. */
            continue;
        }

        use_caps = true;

#if (defined(PRINTF_ADVANCED_ENABLE) && (PRINTF_ADVANCED_ENABLE > 0U))
        /* First check for specification modifier flags. */
        flags_used = PrintCheckFlags(&p);
#endif /* PRINTF_ADVANCED_ENABLE */

        /* Next check for minimum field width. */
        field_width = PrintGetWidth(&p, &ap);

        /* Next check for the width and precision field separator. */
#if (defined(PRINTF_ADVANCED_ENABLE) && (PRINTF_ADVANCED_ENABLE > 0U))
        precision_width = PrintGetPrecision(&p, &ap, &valid_precision_width);
#else
        precision_width = PrintGetPrecision(&p, &ap, NULL);
        (void)precision_width;
#endif

#if (defined(PRINTF_ADVANCED_ENABLE) && (PRINTF_ADVANCED_ENABLE > 0U))
        /* Check for the length modifier. */
        flags_used |= PrintGetLengthFlag(&p);
#else
        /* Filter length modifier. */
        PrintFilterLengthFlag(&p);
#endif

        /* Now we're ready to examine the format. */
        c = *++p;
        {
            if (1U == PrintIsdi(c))
            {
#if (defined(PRINTF_ADVANCED_ENABLE) && (PRINTF_ADVANCED_ENABLE > 0U))
                StrFormatExaminedi(&flags_used, &ival, &ap);
#else
                StrFormatExaminedi(&ival, &ap);
#endif

                vlen  = ConvertRadixNumToString((char *)vstr, (void *)&ival, 1, 10, use_caps);
                vstrp = &vstr[vlen];
#if (defined(PRINTF_ADVANCED_ENABLE) && (PRINTF_ADVANCED_ENABLE > 0U))
                vlen += (int)PrintGetSignChar(ival, flags_used, &schar);
                PrintOutputdifFobpu(flags_used, field_width, (unsigned int)vlen, schar, vstrp, cb, buf, &count);
#else
                PrintOutputdifFobpu(0U, field_width, (unsigned int)vlen, '\0', vstrp, cb, buf, &count);
#endif
            }
            else if (1U == PrintIsfF(c))
            {
#if (defined(PRINTF_FLOAT_ENABLE) && (PRINTF_FLOAT_ENABLE > 0U))
                fval  = (double)va_arg(ap, double);
                vlen  = ConvertFloatRadixNumToString(vstr, &fval, 10, precision_width);
                vstrp = &vstr[vlen];

#if (defined(PRINTF_ADVANCED_ENABLE) && (PRINTF_ADVANCED_ENABLE > 0U))
                vlen += (int32_t)PrintGetSignChar(((fval < 0.0) ? ((long long int)-1) : ((long long int)fval)),
                                                  flags_used, &schar);
                PrintOutputdifFobpu(flags_used, field_width, (unsigned int)vlen, schar, vstrp, cb, buf, &count);
#else
                PrintOutputdifFobpu(0, field_width, (unsigned int)vlen, '\0', vstrp, cb, buf, &count);
#endif

#else
                (void)va_arg(ap, double);
#endif /* PRINTF_FLOAT_ENABLE */
            }
            else if (1U == PrintIsxX(c))
            {
                if (c == 'x')
                {
                    use_caps = false;
                }
#if (defined(PRINTF_ADVANCED_ENABLE) && (PRINTF_ADVANCED_ENABLE > 0U))
                StrFormatExaminexX(&flags_used, &uval, &ap);
#else
                StrFormatExaminexX(&uval, &ap);
#endif

                vlen  = ConvertRadixNumToString((char *)vstr, (void *)&uval, 0, 16, use_caps);
                vstrp = &vstr[vlen];
#if (defined(PRINTF_ADVANCED_ENABLE) && (PRINTF_ADVANCED_ENABLE > 0U))
                PrintOutputxX(flags_used, field_width, (unsigned int)vlen, use_caps, vstrp, cb, buf, &count);
#else
                PrintOutputxX(0U, field_width, (uint32_t)vlen, use_caps, vstrp, cb, buf, &count);
#endif
            }
            else if (1U == PrintIsobpu(c))
            {
                if ('p' == c)
                {
                    /*
                     * Fix MISRA issue: CID 17205581 (#15 of 15): MISRA C-2012 Pointer Type Conversions (MISRA C-2012
                     * Rule 11.6) 1.misra_c_2012_rule_11_6_violation: The expression va_arg (ap, void *) of type void *
                     * is cast to type uint32_t.
                     *
                     * Orignal code: uval = (STR_FORMAT_PRINTF_UVAL_TYPE)(uint32_t)va_arg(ap, void *);
                     */
                    void *pval;
                    pval = (void *)va_arg(ap, void *);
                    (void)memcpy((void *)&uval, (void *)&pval, sizeof(void *));
                }
                else
                {
#if (defined(PRINTF_ADVANCED_ENABLE) && (PRINTF_ADVANCED_ENABLE > 0U))
                    StrFormatExamineobpu(&flags_used, &uval, &ap);
#else
                    StrFormatExamineobpu(&uval, &ap);
#endif
                }

                radix = PrintGetRadixFromobpu(c);

                vlen  = ConvertRadixNumToString((char *)vstr, (void *)&uval, 0, radix, use_caps);
                vstrp = &vstr[vlen];
#if (defined(PRINTF_ADVANCED_ENABLE) && (PRINTF_ADVANCED_ENABLE > 0U))
                PrintOutputdifFobpu(flags_used, field_width, (unsigned int)vlen, '\0', vstrp, cb, buf, &count);
#else
                PrintOutputdifFobpu(0U, field_width, (uint32_t)vlen, '\0', vstrp, cb, buf, &count);
#endif
            }
            else if (c == 'c')
            {
                cval = (int32_t)va_arg(ap, int);
                cb(buf, &count, cval, 1);
            }
            else if (c == 's')
            {
                sval = (char *)va_arg(ap, char *);
                if (NULL != sval)
                {
#if (defined(PRINTF_ADVANCED_ENABLE) && (PRINTF_ADVANCED_ENABLE > 0U))
                    vlen = ConvertPrecisionWidthToLength(valid_precision_width, precision_width, sval);
#else
                    vlen = ConvertPrecisionWidthToLength(sval);
#endif
#if (defined(PRINTF_ADVANCED_ENABLE) && (PRINTF_ADVANCED_ENABLE > 0U))
                    if (0U == (flags_used & (unsigned int)kPRINTF_Minus))
#endif /* PRINTF_ADVANCED_ENABLE */
                    {
                        cb(buf, &count, ' ', (int)field_width - (int)vlen);
                    }

#if (defined(PRINTF_ADVANCED_ENABLE) && (PRINTF_ADVANCED_ENABLE > 0U))
                    if (valid_precision_width)
                    {
                        while (('\0' != *sval) && (vlen > 0))
                        {
                            cb(buf, &count, *sval++, 1);
                            vlen--;
                        }
                        /* In case that vlen sval is shorter than vlen */
                        vlen = (int)precision_width - vlen;
                    }
                    else
                    {
#endif /* PRINTF_ADVANCED_ENABLE */
                        while ('\0' != (*sval))
                        {
                            cb(buf, &count, *sval++, 1);
                        }
#if (defined(PRINTF_ADVANCED_ENABLE) && (PRINTF_ADVANCED_ENABLE > 0U))
                    }
#endif /* PRINTF_ADVANCED_ENABLE */

#if (defined(PRINTF_ADVANCED_ENABLE) && (PRINTF_ADVANCED_ENABLE > 0U))
                    if (0U != (flags_used & (unsigned int)kPRINTF_Minus))
                    {
                        cb(buf, &count, ' ', (int)field_width - vlen);
                    }
#endif /* PRINTF_ADVANCED_ENABLE */
                }
            }
            else
            {
                cb(buf, &count, c, 1);
            }
        }
        p++;
    }

    return (int)count;
}

#if (defined(SCANF_FLOAT_ENABLE) && (SCANF_FLOAT_ENABLE > 0U))
static uint8_t StrFormatScanIsFloat(char *c)
{
    uint8_t ret = 0U;
    if (('a' == (*c)) || ('A' == (*c)) || ('e' == (*c)) || ('E' == (*c)) || ('f' == (*c)) || ('F' == (*c)) ||
        ('g' == (*c)) || ('G' == (*c)))
    {
        ret = 1U;
    }
    return ret;
}
#endif

static uint8_t StrFormatScanIsFormatStarting(char *c)
{
    uint8_t ret = 1U;
    if ((*c != '%'))
    {
        ret = 0U;
    }
    else if (*(c + 1) == '%')
    {
        ret = 0U;
    }
    else
    {
        /*MISRA rule 15.7*/
    }

    return ret;
}

static uint8_t StrFormatScanGetBase(uint8_t base, const char *s)
{
    if (base == 0U)
    {
        if (s[0] == '0')
        {
            if ((s[1] == 'x') || (s[1] == 'X'))
            {
                base = 16;
            }
            else
            {
                base = 8;
            }
        }
        else
        {
            base = 10;
        }
    }
    return base;
}

static uint8_t StrFormatScanCheckSymbol(const char *p, int8_t *neg)
{
    uint8_t len;
    switch (*p)
    {
        case '-':
            *neg = -1;
            len  = 1;
            break;
        case '+':
            *neg = 1;
            len  = 1;
            break;
        default:
            *neg = 1;
            len  = 0;
            break;
    }
    return len;
}

static uint8_t StrFormatScanFillInteger(uint32_t flag, va_list *args_ptr, int32_t val)
{
#if (defined(SCANF_ADVANCED_ENABLE) && (SCANF_ADVANCED_ENABLE > 0U))
    if (0U != (flag & (uint32_t)kSCANF_Suppress))
    {
        return 0u;
    }

    switch (flag & (uint32_t)kSCANF_LengthMask)
    {
        case (uint32_t)kSCANF_LengthChar:
            if (0U != (flag & (uint32_t)kSCANF_TypeSinged))
            {
                *va_arg(*args_ptr, signed char *) = (signed char)val;
            }
            else
            {
                *va_arg(*args_ptr, unsigned char *) = (unsigned char)val;
            }
            break;
        case (uint32_t)kSCANF_LengthShortInt:
            if (0U != (flag & (uint32_t)kSCANF_TypeSinged))
            {
                *va_arg(*args_ptr, signed short *) = (signed short)val;
            }
            else
            {
                *va_arg(*args_ptr, unsigned short *) = (unsigned short)val;
            }
            break;
        case (uint32_t)kSCANF_LengthLongInt:
            if (0U != (flag & (uint32_t)kSCANF_TypeSinged))
            {
                *va_arg(*args_ptr, signed long int *) = (signed long int)val;
            }
            else
            {
                *va_arg(*args_ptr, unsigned long int *) = (unsigned long int)val;
            }
            break;
        case (uint32_t)kSCANF_LengthLongLongInt:
            if (0U != (flag & (uint32_t)kSCANF_TypeSinged))
            {
                *va_arg(*args_ptr, signed long long int *) = (signed long long int)val;
            }
            else
            {
                *va_arg(*args_ptr, unsigned long long int *) = (unsigned long long int)val;
            }
            break;
        default:
            /* The default type is the type int. */
            if (0U != (flag & (uint32_t)kSCANF_TypeSinged))
            {
                *va_arg(*args_ptr, signed int *) = (signed int)val;
            }
            else
            {
                *va_arg(*args_ptr, unsigned int *) = (unsigned int)val;
            }
            break;
    }
#else
    /* The default type is the type int. */
    if (0U != (flag & (uint32_t)kSCANF_TypeSinged))
    {
        *va_arg(*args_ptr, signed int *) = (signed int)val;
    }
    else
    {
        *va_arg(*args_ptr, unsigned int *) = (unsigned int)val;
    }
#endif /* SCANF_ADVANCED_ENABLE */

    return 1U;
}

#if (defined(SCANF_FLOAT_ENABLE) && (SCANF_FLOAT_ENABLE > 0U))
static uint8_t StrFormatScanFillFloat(uint32_t flag, va_list *args_ptr, double fnum)
{
#if (defined(SCANF_ADVANCED_ENABLE) && (SCANF_ADVANCED_ENABLE > 0U))
    if (0U != (flag & (uint32_t)kSCANF_Suppress))
    {
        return 0U;
    }
    else
#endif /* SCANF_ADVANCED_ENABLE */
    {
        if (0U != (flag & (uint32_t)kSCANF_LengthLongLongDouble))
        {
            *va_arg(*args_ptr, double *) = fnum;
        }
        else
        {
            *va_arg(*args_ptr, float *) = (float)fnum;
        }
        return 1U;
    }
}
#endif /* SCANF_FLOAT_ENABLE */

#if (defined(SCANF_ADVANCED_ENABLE) && (SCANF_ADVANCED_ENABLE > 0U))
static uint8_t strFormatScanfHandleh(uint8_t exitPending, char **c, uint32_t *flag)
{
    if (0U != ((*flag) & (uint32_t)kSCANF_LengthMask))
    {
        /* Match failure. */
        exitPending = 1U;
    }
    else
    {
        if ((*c)[1] == 'h')
        {
            (*flag) |= (uint32_t)kSCANF_LengthChar;
            *c = *c + 1U;
        }
        else
        {
            (*flag) |= (uint32_t)kSCANF_LengthShortInt;
        }
    }

    return exitPending;
}

static uint8_t strFormatScanfHandlel(uint8_t exitPending, char **c, uint32_t *flag)
{
    if (0U != ((*flag) & (uint32_t)kSCANF_LengthMask))
    {
        /* Match failure. */
        exitPending = 1U;
    }
    else
    {
        if ((*c)[1] == 'l')
        {
            (*flag) |= (uint32_t)kSCANF_LengthLongLongInt;
            *c = *c + 1U;
        }
        else
        {
            (*flag) |= (uint32_t)kSCANF_LengthLongInt;
        }
    }

    return exitPending;
}
#endif

static uint8_t StrFormatScanfStringHandling(char **str, uint32_t *flag, uint32_t *field_width, uint8_t *base)
{
    uint8_t exitPending = 0U;
    char *c             = *str;

    /* Loop to get full conversion specification. */
    while (('\0' != (*c)) && (0U == (*flag & (uint32_t)kSCANF_DestMask)))
    {
#if (defined(SCANF_ADVANCED_ENABLE) && (SCANF_ADVANCED_ENABLE > 0U))
        if ('*' == (*c))
        {
            if (0U != ((*flag) & (uint32_t)kSCANF_Suppress))
            {
                /* Match failure. */
                exitPending = 1U;
            }
            else
            {
                (*flag) |= (uint32_t)kSCANF_Suppress;
            }
        }
        else if ('h' == (*c))
        {
            exitPending = strFormatScanfHandleh(exitPending, &c, flag);
        }
        else if ('l' == (*c))
        {
            exitPending = strFormatScanfHandlel(exitPending, &c, flag);
        }
        else
#endif /* SCANF_ADVANCED_ENABLE */
#if (defined(SCANF_FLOAT_ENABLE) && (SCANF_FLOAT_ENABLE > 0U))
            if ('L' == (*c))
        {
            if (0U != ((*flag) & (uint32_t)kSCANF_LengthMask))
            {
                /* Match failure. */
                exitPending = 1U;
            }
            else
            {
                (*flag) |= (uint32_t)kSCANF_LengthLongLongDouble;
            }
        }
        else
#endif /* SCANF_FLOAT_ENABLE */
            if (((*c) >= '0') && ((*c) <= '9'))
            {
                {
                    char *p;
                    errno          = 0;
                    (*field_width) = strtoul(c, &p, 10);
                    if (0 != errno)
                    {
                        *field_width = 0U;
                    }
                    c = p - 1;
                }
            }
            else if ('d' == (*c))
            {
                (*base) = 10U;
                (*flag) |= (uint32_t)kSCANF_TypeSinged;
                (*flag) |= (uint32_t)kSCANF_DestInt;
            }
            else if ('u' == (*c))
            {
                (*base) = 10U;
                (*flag) |= (uint32_t)kSCANF_DestInt;
            }
            else if ('o' == (*c))
            {
                (*base) = 8U;
                (*flag) |= (uint32_t)kSCANF_DestInt;
            }
            else if (('x' == (*c)))
            {
                (*base) = 16U;
                (*flag) |= (uint32_t)kSCANF_DestInt;
            }
            else if ('X' == (*c))
            {
                (*base) = 16U;
                (*flag) |= (uint32_t)kSCANF_DestInt;
            }
            else if ('i' == (*c))
            {
                (*base) = 0U;
                (*flag) |= (uint32_t)kSCANF_DestInt;
            }
#if (defined(SCANF_FLOAT_ENABLE) && (SCANF_FLOAT_ENABLE > 0U))
            else if (1U == StrFormatScanIsFloat(c))
            {
                (*flag) |= (uint32_t)kSCANF_DestFloat;
            }
#endif /* SCANF_FLOAT_ENABLE */
            else if ('c' == (*c))
            {
                (*flag) |= (uint32_t)kSCANF_DestChar;
                if (MAX_FIELD_WIDTH == (*field_width))
                {
                    (*field_width) = 1;
                }
            }
            else if ('s' == (*c))
            {
                (*flag) |= (uint32_t)kSCANF_DestString;
            }
            else
            {
                exitPending = 1U;
            }

        if (1U == exitPending)
        {
            break;
        }
        else
        {
            c++;
        }
    }
    *str = c;
    return exitPending;
}

static void StrFormatScanfHandleChar(
    const char **Cp, uint32_t *field_width, char **buf, uint32_t flag, uint32_t *n_decode, uint32_t *nassigned)
{
#if (defined(SCANF_ADVANCED_ENABLE) && (SCANF_ADVANCED_ENABLE > 0U))
    uint8_t added = 0;
#endif
    while ((0U != ((*field_width)--))
#if (defined(SCANF_ADVANCED_ENABLE) && (SCANF_ADVANCED_ENABLE > 0U))
           && ('\0' != (**Cp))
#endif
    )
    {
#if (defined(SCANF_ADVANCED_ENABLE) && (SCANF_ADVANCED_ENABLE > 0U))
        if (0U != (flag & (uint32_t)kSCANF_Suppress))
        {
            (*Cp) = (*Cp) + 1U;
        }
        else
#endif
        {
            **buf  = **Cp;
            (*Cp)  = (*Cp) + 1U;
            (*buf) = (*buf) + 1U;

#if (defined(SCANF_ADVANCED_ENABLE) && (SCANF_ADVANCED_ENABLE > 0U))
            added = 1u;
#endif
        }
        *n_decode = *n_decode + 1U;
    }

#if (defined(SCANF_ADVANCED_ENABLE) && (SCANF_ADVANCED_ENABLE > 0U))
    if (1u == added)
#endif
    {
        *nassigned = *nassigned + 1U;
    }
}

static void StrFormatScanfHandleString(
    const char **Sp, uint32_t *field_width, char **buf, uint32_t flag, uint32_t *n_decode, uint32_t *nassigned)
{
#if (defined(SCANF_ADVANCED_ENABLE) && (SCANF_ADVANCED_ENABLE > 0U))
    uint8_t added = 0;
#endif
    while ((0U != ((*field_width)--)) && (**Sp != '\0') && (0U == ScanIsWhiteSpace(**Sp)))
    {
#if (defined(SCANF_ADVANCED_ENABLE) && (SCANF_ADVANCED_ENABLE > 0U))
        if (0U != (flag & (uint32_t)kSCANF_Suppress))
        {
            (*Sp) = (*Sp) + 1U;
        }
        else
#endif
        {
            **buf  = **Sp;
            (*buf) = (*buf) + 1U;
            (*Sp)  = (*Sp) + 1U;
#if (defined(SCANF_ADVANCED_ENABLE) && (SCANF_ADVANCED_ENABLE > 0U))
            added = 1u;
#endif
        }
        *n_decode = *n_decode + 1U;
    }

#if (defined(SCANF_ADVANCED_ENABLE) && (SCANF_ADVANCED_ENABLE > 0U))
    if (1u == added)
#endif
    {
        /* Add NULL to end of string. */
        **buf      = '\0';
        *nassigned = *nassigned + 1U;
    }
}

/*!
 * brief Converts an input line of ASCII characters based upon a provided
 * string format.
 *
 * param[in] line_ptr The input line of ASCII data.
 * param[in] format   Format first points to the format string.
 * param[in] args_ptr The list of parameters.
 *
 * return Number of input items converted and assigned.
 * retval IO_EOF When line_ptr is empty string "".
 */
int StrFormatScanf(const char *line_ptr, char *format, va_list args_ptr)
{
    uint8_t base;
    int8_t neg;
    /* Identifier for the format string. */
    char *c = format;
    char *buf;
    /* Flag telling the conversion specification. */
    uint32_t flag = 0;
    /* Filed width for the matching input streams. */
    uint32_t field_width;
    /* How many arguments are assigned except the suppress. */
    uint32_t nassigned = 0;
    /* How many characters are read from the input streams. */
    uint32_t n_decode = 0;

    int32_t val;

    uint8_t added = 0U;

    uint8_t exitPending = 0;

    const char *s;
#if (defined(SCANF_FLOAT_ENABLE) && (SCANF_FLOAT_ENABLE > 0U))
    char *s_temp; /* MISRA C-2012 Rule 11.3 */
#endif

    /* Identifier for the input string. */
    const char *p = line_ptr;

#if (defined(SCANF_FLOAT_ENABLE) && (SCANF_FLOAT_ENABLE > 0U))
    double fnum = 0.0;
#endif /* SCANF_FLOAT_ENABLE */
    /* Return EOF error before any conversion. */
    if (*p == '\0')
    {
        return -1;
    }

    /* Decode directives. */
    while (('\0' != (*c)) && ('\0' != (*p)))
    {
        /* Ignore all white-spaces in the format strings. */
        if (0U != ScanIgnoreWhiteSpace((const char **)((void *)&c)))
        {
            n_decode += ScanIgnoreWhiteSpace(&p);
        }
        else if (0U == StrFormatScanIsFormatStarting(c))
        {
            /* Ordinary characters. */
            c++;
            if (*p == *c)
            {
                n_decode++;
                p++;
                c++;
            }
            else
            {
                /* Match failure. Misalignment with C99, the unmatched characters need to be pushed back to stream.
                 * However, it is deserted now. */
                break;
            }
        }
        else
        {
            /* convernsion specification */
            c++;
            /* Reset. */
            flag        = 0;
            field_width = MAX_FIELD_WIDTH;
            base        = 0;

            exitPending = StrFormatScanfStringHandling(&c, &flag, &field_width, &base);

            if (1U == exitPending)
            {
                /* Format strings are exhausted. */
                break;
            }

            /* Matching strings in input streams and assign to argument. */
            if ((flag & (uint32_t)kSCANF_DestMask) == (uint32_t)kSCANF_DestChar)
            {
                s   = (const char *)p;
                buf = va_arg(args_ptr, char *);
                StrFormatScanfHandleChar(&p, &field_width, &buf, flag, &n_decode, &nassigned);
            }
            else if ((flag & (uint32_t)kSCANF_DestMask) == (uint32_t)kSCANF_DestString)
            {
                n_decode += ScanIgnoreWhiteSpace(&p);
                buf = va_arg(args_ptr, char *);
                StrFormatScanfHandleString(&p, &field_width, &buf, flag, &n_decode, &nassigned);
            }
            else if ((flag & (uint32_t)kSCANF_DestMask) == (uint32_t)kSCANF_DestInt)
            {
                n_decode += ScanIgnoreWhiteSpace(&p);
                s    = p;
                val  = 0;
                base = StrFormatScanGetBase(base, s);

                added = StrFormatScanCheckSymbol(p, &neg);
                n_decode += added;
                p += added;
                field_width -= added;

                s = p;
                if (strlen(p) > field_width)
                {
                    char temp[12];
                    char *tempEnd;
                    (void)memcpy(temp, p, sizeof(temp) - 1U);
                    temp[sizeof(temp) - 1U] = '\0';
                    errno                   = 0;
                    val                     = (int32_t)strtoul(temp, &tempEnd, (int)base);
                    if (0 != errno)
                    {
                        break;
                    }
                    p = p + (tempEnd - temp);
                }
                else
                {
                    char *tempEnd;
                    errno = 0;
                    val   = (int32_t)strtoul(p, &tempEnd, (int)base);
                    if (0 != errno)
                    {
                        break;
                    }
                    p = tempEnd;
                }
                n_decode += (uintptr_t)p - (uintptr_t)s;

                val *= neg;

                nassigned += StrFormatScanFillInteger(flag, &args_ptr, val);
            }
#if (defined(SCANF_FLOAT_ENABLE) && (SCANF_FLOAT_ENABLE > 0U))
            else if ((flag & (uint32_t)kSCANF_DestMask) == (uint32_t)kSCANF_DestFloat)
            {
                n_decode += ScanIgnoreWhiteSpace(&p);
                errno = 0;

                fnum = strtod(p, (char **)&s_temp);
                s    = s_temp; /* MISRA C-2012 Rule 11.3 */

                /* MISRA C-2012 Rule 22.9 */
                if (0 != errno)
                {
                    break;
                }

                if ((fnum < HUGE_VAL) && (fnum > -HUGE_VAL))
                {
                    n_decode = (uint32_t)n_decode + (uint32_t)s - (uint32_t)p;
                    p        = s;
                    nassigned += StrFormatScanFillFloat(flag, &args_ptr, fnum);
                }
            }
#endif /* SCANF_FLOAT_ENABLE */
            else
            {
                break;
            }
        }
    }
    return (int)nassigned;
}
