/*
 * Copyright (c) 2015-2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2017, 2022-2023 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_common.h"
#include "fsl_assert.h"
#include "fsl_debug_console.h"

/* User can implement its own asser handler (dump logs, registers, etc) by reimplementing the function fsl_assert_hook() */
__attribute__ ((weak)) int fsl_assert_hook(const char *failedExpr, const char *file, int line)
{
    (void)failedExpr;
    (void)file;
    (void)line;

    return 0;
}

#ifndef NDEBUG
#if (defined(__CC_ARM)) || (defined(__ARMCC_VERSION)) || (defined(__ICCARM__))
void __aeabi_assert(const char *failedExpr, const char *file, int line)
{
#if SDK_DEBUGCONSOLE == DEBUGCONSOLE_DISABLE
    PRINTF("ASSERT ERROR \" %s \": file \"%s\" Line \"%d\" \n", failedExpr, file, line);
#else
    (void)PRINTF("ASSERT ERROR \" %s \": file \"%s\" Line \"%d\" \n", failedExpr, file, line);
#endif

    (void)fsl_assert_hook(failedExpr, file, line);

    for (;;)
    {
        __BKPT(0);
    }
}
#elif (defined(__GNUC__))
#if defined(__REDLIB__)
void __assertion_failed(char *failedExpr)
{
    const char *file = NULL;
    int line = -1;

    (void)PRINTF("ASSERT ERROR \" %s \n", failedExpr);

    (void)fsl_assert_hook(failedExpr, file, line);

    for (;;)
    {
        __BKPT(0);
    }
}
#else
void __assert_func(const char *file, int line, const char *func, const char *failedExpr)
{
    (void)PRINTF("ASSERT ERROR \" %s \": file \"%s\" Line \"%d\" function name \"%s\" \n", failedExpr, file, line,
                 func);

    (void)fsl_assert_hook(failedExpr, file, line);

    for (;;)
    {
        __BKPT(0);
    }
}
#endif /* defined(__REDLIB__) */
#else  /* (defined(__CC_ARM) || (defined(__ICCARM__)) || (defined(__ARMCC_VERSION)) */

#if (defined(__DSC__) && defined(__CW__))

void __msl_assertion_failed(char const *failedExpr, char const *file, char const *func, int line)
{
    PRINTF("\r\nASSERT ERROR\r\n");
    PRINTF("  File      : %s\r\n", file);
    PRINTF("  Function  : %s\r\n", func); /*compiler not support func name yet*/
    PRINTF("  Line      : %u\r\n", (uint32_t)line);
    PRINTF("  failedExpr: %s\r\n", failedExpr);
    asm(DEBUGHLT);
}

#endif /* (defined(__DSC__) && defined (__CW__)) */

#endif /* (defined(__CC_ARM) || (defined(__ICCARM__)) || (defined(__ARMCC_VERSION)) */
#endif /* NDEBUG */
