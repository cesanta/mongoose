/*
 * Copyright 2023 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _FSL_ASSERT_H_
#define _FSL_ASSERT_H_

/*!
 * @addtogroup assert
 * @{
 */

/*******************************************************************************
 * Definitions
 ******************************************************************************/


/*******************************************************************************
 * Prototypes
 ******************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

/*! @name Initialization*/
/* @{ */


/*!
 * @brief Assert hook that can be redifined
 *
 * @param failedExpr  Expression that caused the assert
 * @param file  File where the exception occured.
 * @param line  Line on the file where the exception occured.
 */
int fsl_assert_hook(const char *failedExpr, const char *file, int line);

/*! @} */

#if defined(__cplusplus)
}
#endif /* __cplusplus */

/*! @} */

#endif /* _FSL_DEBUGCONSOLE_H_ */

