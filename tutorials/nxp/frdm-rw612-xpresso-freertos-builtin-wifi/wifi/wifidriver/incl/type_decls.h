/** @file type_decls.h
 *
 *  @brief This file declares the generic data structures and APIs.
 *
 *  Copyright 2022 NXP
 *
 *  SPDX-License-Identifier: BSD-3-Clause
 *
 */

/******************************************************
Change log:
    20/01/2022: initial version
******************************************************/

#ifndef _TYPE_DECL_H_
#define _TYPE_DECL_H_


/** Re-define generic data types for firmware downloader */
/** Signed char (1-byte) */
typedef char t_s8;
/** Unsigned char (1-byte) */
typedef unsigned char t_u8;
/** Signed short (2-bytes) */
typedef short t_s16;
/** Unsigned short (2-bytes) */
typedef unsigned short t_u16;
/** Signed long (4-bytes) */
typedef int t_s32;
/** Unsigned long (4-bytes) */
typedef unsigned int t_u32;
/** Signed long long 8-bytes) */
typedef long long t_s64;
/** Unsigned long long 8-bytes) */
typedef unsigned long long t_u64;
/** Void pointer (4-bytes) */
typedef void t_void;
/** Size type */
typedef t_u32 t_size;
/** Boolean type */
typedef t_u8 t_bool;

#ifdef MLAN_64BIT
/** Pointer type (64-bit) */
typedef t_u64 t_ptr;
/** Signed value (64-bit) */
typedef t_s64 t_sval;
#else
/** Pointer type (32-bit) */
typedef t_u32 t_ptr;
/** Signed value (32-bit) */
typedef t_s32 t_sval;
#endif

/** MLAN TRUE */
#define MTRUE (1)
/** MLAN FALSE */
#define MFALSE (0)

#endif /* !_TYPE_DECL_H_ */
