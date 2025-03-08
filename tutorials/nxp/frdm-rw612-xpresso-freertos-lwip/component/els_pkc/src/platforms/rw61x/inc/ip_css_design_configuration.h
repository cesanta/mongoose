/*--------------------------------------------------------------------------*/
/* Copyright 2016-2022 NXP                                                  */
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

#ifndef IP_CSS_DESIGN_CONFIGURATION_H_
#define IP_CSS_DESIGN_CONFIGURATION_H_

// CSS COMMAND LIST
// **** --> processing include file 'config_css_commands.txt'
// 
// 
// List of commands available in CSS
// 
#define ID_CFG_CSS_CMD_CIPHER 0
#define ID_CFG_CSS_CMD_AUTH_CIPHER 1
#define ID_CFG_CSS_CMD_RFU_0 2
#define ID_CFG_CSS_CMD_RFU_1 3
#define ID_CFG_CSS_CMD_ECSIGN 4
#define ID_CFG_CSS_CMD_ECVFY 5
#define ID_CFG_CSS_CMD_ECKXH 6
#define ID_CFG_CSS_CMD_RFU_2 7
#define ID_CFG_CSS_CMD_KEYGEN 8
#define ID_CFG_CSS_CMD_KEYIN 9
#define ID_CFG_CSS_CMD_KEYOUT 10
#define ID_CFG_CSS_CMD_KDELETE 11
#define ID_CFG_CSS_CMD_KEYPROV 12
#define ID_CFG_CSS_CMD_RFU_4 13
#define ID_CFG_CSS_CMD_RFU_5 14
#define ID_CFG_CSS_CMD_RFU_6 15
#define ID_CFG_CSS_CMD_CKDF 16
#define ID_CFG_CSS_CMD_HKDF 17
#define ID_CFG_CSS_CMD_TLS 18
#define ID_CFG_CSS_CMD_RFU_7 19
#define ID_CFG_CSS_CMD_HASH 20
#define ID_CFG_CSS_CMD_HMAC 21
#define ID_CFG_CSS_CMD_CMAC 22
#define ID_CFG_CSS_CMD_RFU_9 23
#define ID_CFG_CSS_CMD_DRBG_REQ 24
#define ID_CFG_CSS_CMD_DRBG_TEST 25
#define ID_CFG_CSS_CMD_RFU_11 26
#define ID_CFG_CSS_CMD_RFU_12 27
#define ID_CFG_CSS_CMD_DTRNG_CFG_LOAD 28
#define ID_CFG_CSS_CMD_DTRNG_EVAL 29
#define ID_CFG_CSS_CMD_GDET_CFG_LOAD 30
#define ID_CFG_CSS_CMD_GDET_TRIM 31
// Command Configurations
// ID_CFG_CSS_CMD_CIPHER
#define ID_CFG_CSS_CMD_CIPHER_DCRYPT 1
#define ID_CFG_CSS_CMD_CIPHER_CPHMDE0 2
#define ID_CFG_CSS_CMD_CIPHER_CPHMDE1 3
#define ID_CFG_CSS_CMD_CIPHER_CPHSOE 4
#define ID_CFG_CSS_CMD_CIPHER_CPHSIE 5
#define ID_CFG_CSS_CMD_CIPHER_EXTKEY 13
// ID_CFG_CSS_CMD_AUTH_CIPHER
#define ID_CFG_CSS_CMD_AUTH_CIPHER_DCRYPT 1
#define ID_CFG_CSS_CMD_AUTH_CIPHER_ACPMOD0 2
#define ID_CFG_CSS_CMD_AUTH_CIPHER_ACPMOD1 3
#define ID_CFG_CSS_CMD_AUTH_CIPHER_CPHSOE 4
#define ID_CFG_CSS_CMD_AUTH_CIPHER_CPHSIE 5
#define ID_CFG_CSS_CMD_AUTH_CIPHER_MSGENDW0 6
#define ID_CFG_CSS_CMD_AUTH_CIPHER_MSGENDW1 7
#define ID_CFG_CSS_CMD_AUTH_CIPHER_MSGENDW2 8
#define ID_CFG_CSS_CMD_AUTH_CIPHER_MSGENDW3 9
#define ID_CFG_CSS_CMD_AUTH_CIPHER_LASTINIT 10
#define ID_CFG_CSS_CMD_AUTH_CIPHER_EXTKEY 13
// ID_CFG_CSS_CMD_ECSIGN
#define ID_CFG_CSS_CMD_ECSIGN_CFG_ECHASHCHL 0
#define ID_CFG_CSS_CMD_ECSIGN_CFG_SIGNRTF 1
#define ID_CFG_CSS_CMD_ECSIGN_CFG_REVF 4
// ID_CFG_CSS_CMD_ECVFY
#define ID_CFG_CSS_CMD_ECVFY_CFG_ECHASHCHL 0
#define ID_CFG_CSS_CMD_ECVFY_CFG_REVF 4
// ID_CFG_CSS_CMD_ECKXH
#define ID_CFG_CSS_CMD_ECKXH_REVF 4
// ID_CFG_CSS_CMD_KEYGEN
#define ID_CFG_CSS_CMD_KEYGEN_KGSIGN 0
#define ID_CFG_CSS_CMD_KEYGEN_KGTYPEDH 1
#define ID_CFG_CSS_CMD_KEYGEN_KGSRC 2
#define ID_CFG_CSS_CMD_KEYGEN_SKIP_PBK 3
#define ID_CFG_CSS_CMD_KEYGEN_REVF 4
#define ID_CFG_CSS_CMD_KEYGEN_KGSIGN_RND 5
// ID_CFG_CSS_CMD_KEYIN
#define ID_CFG_CSS_CMD_KEYIN_KFMT0 6
#define ID_CFG_CSS_CMD_KEYIN_KFMT1 7
// ID_CFG_CSS_CMD_KEYPROV
#define ID_CFG_CSS_CMD_KEYPROV_ICEN 0
// ID_CFG_CSS_CMD_CKDF
#define ID_CFG_CSS_CMD_CKDF_CKDF_ALGO0 12
#define ID_CFG_CSS_CMD_CKDF_CKDF_ALGO1 13
// ID_CFG_CSS_CMD_HKDF
#define ID_CFG_CSS_CMD_HKDF_RTFDRVDAT 0
#define ID_CFG_CSS_CMD_HKDF_HKDF_ALGO 1
#define ID_CFG_CSS_CMD_HKDF_SINGLE_STEP 2
// ID_CFG_CSS_CMD_TLS
#define ID_CFG_CSS_CMD_TLS_FINALIZE 10
// ID_CFG_CSS_CMD_HASH
#define ID_CFG_CSS_CMD_HASH_HASHINI 2
#define ID_CFG_CSS_CMD_HASH_HASHLD 3
// ID_CFG_CSS_CMD_HASH_HASHMD0
#define ID_CFG_CSS_CMD_HASH_HASHMD0 4
// ID_CFG_CSS_CMD_HASH_HASHMD1
#define ID_CFG_CSS_CMD_HASH_HASHMD1 5
#define ID_CFG_CSS_CMD_HASH_HASHOE 6
#define ID_CFG_CSS_CMD_HASH_RTFUPD 7
#define ID_CFG_CSS_CMD_HASH_RTFOE 8
// ID_CFG_CSS_CMD_HMAC
#define ID_CFG_CSS_CMD_HMAC_EXTKEY 13
// ID_CFG_CSS_CMD_CMAC
#define ID_CFG_CSS_CMD_CMAC_INIT 0
#define ID_CFG_CSS_CMD_CMAC_FINALIZE 1
#define ID_CFG_CSS_CMD_CMAC_SOE 2
#define ID_CFG_CSS_CMD_CMAC_SIE 3
#define ID_CFG_CSS_CMD_CMAC_EXTKEY 13
// ID_CFG_CSS_CMD_DRBG_TEST
#define ID_CFG_CSS_CMD_DRBG_TEST_MODE0 0
#define ID_CFG_CSS_CMD_DRBG_TEST_MODE1 1

#endif /* IP_CSS_DESIGN_CONFIGURATION_H_ */
