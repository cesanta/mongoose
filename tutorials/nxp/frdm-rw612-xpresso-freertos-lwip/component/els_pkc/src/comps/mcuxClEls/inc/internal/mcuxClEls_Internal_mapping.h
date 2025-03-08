/*--------------------------------------------------------------------------*/
/* Copyright 2022 NXP                                                       */
/*                                                                          */
/* NXP Proprietary. This software is owned or controlled by NXP and may     */
/* only be used strictly in accordance with the applicable license terms.   */
/* By expressly accepting such terms or by downloading, installing,         */
/* activating and/or otherwise using the software, you are agreeing that    */
/* you have read, and that you agree to comply with and are bound by, such  */
/* license terms.  If you do not agree to be bound by the applicable        */
/* license terms, then you may not retain, install, activate or otherwise   */
/* use the software.                                                        */
/*--------------------------------------------------------------------------*/

/**
 * @file  mcuxClEls_Internal_mapping.h
 * @brief Header providing mapping for legacy definitions (with CSS)
 */

#ifndef MCUXCLELS_INTERNAL_MAPPING_H_
#define MCUXCLELS_INTERNAL_MAPPING_H_

#if !defined(ELS_KS_CNT)
#define ELS_KS_CNT  CSS_KS_CNT
#endif

#if !defined(ID_CFG_ELS_CMD_AUTH_CIPHER)
#define ID_CFG_ELS_CMD_AUTH_CIPHER  ID_CFG_CSS_CMD_AUTH_CIPHER
#endif
#if !defined(ID_CFG_ELS_CMD_CHAL_RESP_GEN)
#define ID_CFG_ELS_CMD_CHAL_RESP_GEN  ID_CFG_CSS_CMD_CHAL_RESP_GEN
#endif
#if !defined(ID_CFG_ELS_CMD_CIPHER)
#define ID_CFG_ELS_CMD_CIPHER  ID_CFG_CSS_CMD_CIPHER
#endif
#if !defined(ID_CFG_ELS_CMD_CKDF)
#define ID_CFG_ELS_CMD_CKDF  ID_CFG_CSS_CMD_CKDF
#endif
#if !defined(ID_CFG_ELS_CMD_CMAC)
#define ID_CFG_ELS_CMD_CMAC  ID_CFG_CSS_CMD_CMAC
#endif
#if !defined(ID_CFG_ELS_CMD_DRBG_TEST)
#define ID_CFG_ELS_CMD_DRBG_TEST  ID_CFG_CSS_CMD_DRBG_TEST
#endif
#if !defined(ID_CFG_ELS_CMD_DTRNG_CFG_LOAD)
#define ID_CFG_ELS_CMD_DTRNG_CFG_LOAD  ID_CFG_CSS_CMD_DTRNG_CFG_LOAD
#endif
#if !defined(ID_CFG_ELS_CMD_DTRNG_EVAL)
#define ID_CFG_ELS_CMD_DTRNG_EVAL  ID_CFG_CSS_CMD_DTRNG_EVAL
#endif
#if !defined(ID_CFG_ELS_CMD_DTRNG_PRVL_CFG_LOAD)
#define ID_CFG_ELS_CMD_DTRNG_PRVL_CFG_LOAD  ID_CFG_CSS_CMD_DTRNG_PRVL_CFG_LOAD
#endif
#if !defined(ID_CFG_ELS_CMD_ECKXH)
#define ID_CFG_ELS_CMD_ECKXH  ID_CFG_CSS_CMD_ECKXH
#endif
#if !defined(ID_CFG_ELS_CMD_ECSIGN)
#define ID_CFG_ELS_CMD_ECSIGN  ID_CFG_CSS_CMD_ECSIGN
#endif
#if !defined(ID_CFG_ELS_CMD_ECVFY)
#define ID_CFG_ELS_CMD_ECVFY  ID_CFG_CSS_CMD_ECVFY
#endif
#if !defined(ID_CFG_ELS_CMD_GDET_CFG_LOAD)
#define ID_CFG_ELS_CMD_GDET_CFG_LOAD  ID_CFG_CSS_CMD_GDET_CFG_LOAD
#endif
#if !defined(ID_CFG_ELS_CMD_GDET_TRIM)
#define ID_CFG_ELS_CMD_GDET_TRIM  ID_CFG_CSS_CMD_GDET_TRIM
#endif
#if !defined(ID_CFG_ELS_CMD_HASH)
#define ID_CFG_ELS_CMD_HASH  ID_CFG_CSS_CMD_HASH
#endif
#if !defined(ID_CFG_ELS_CMD_HKDF)
#define ID_CFG_ELS_CMD_HKDF  ID_CFG_CSS_CMD_HKDF
#endif
#if !defined(ID_CFG_ELS_CMD_HMAC)
#define ID_CFG_ELS_CMD_HMAC  ID_CFG_CSS_CMD_HMAC
#endif
#if !defined(ID_CFG_ELS_CMD_KDELETE)
#define ID_CFG_ELS_CMD_KDELETE  ID_CFG_CSS_CMD_KDELETE
#endif
#if !defined(ID_CFG_ELS_CMD_KEYGEN)
#define ID_CFG_ELS_CMD_KEYGEN  ID_CFG_CSS_CMD_KEYGEN
#endif
#if !defined(ID_CFG_ELS_CMD_KEYIN)
#define ID_CFG_ELS_CMD_KEYIN  ID_CFG_CSS_CMD_KEYIN
#endif
#if !defined(ID_CFG_ELS_CMD_KEYOUT)
#define ID_CFG_ELS_CMD_KEYOUT  ID_CFG_CSS_CMD_KEYOUT
#endif
#if !defined(ID_CFG_ELS_CMD_KEYPROV)
#define ID_CFG_ELS_CMD_KEYPROV  ID_CFG_CSS_CMD_KEYPROV
#endif
#if !defined(ID_CFG_ELS_CMD_TLS)
#define ID_CFG_ELS_CMD_TLS  ID_CFG_CSS_CMD_TLS
#endif

#if (!defined(ID_CFG_ELS_CMD_RND_REQ)) && defined(ID_CFG_CSS_CMD_RND_REQ)
#define ID_CFG_ELS_CMD_RND_REQ  ID_CFG_CSS_CMD_RND_REQ
#endif

#if (!defined(ID_CFG_ELS_CMD_DRBG_REQ)) && defined(ID_CFG_CSS_CMD_DRBG_REQ)
#define ID_CFG_ELS_CMD_DRBG_REQ  ID_CFG_CSS_CMD_DRBG_REQ
#endif


#endif /* MCUXCLELS_INTERNAL_MAPPING_H_ */
