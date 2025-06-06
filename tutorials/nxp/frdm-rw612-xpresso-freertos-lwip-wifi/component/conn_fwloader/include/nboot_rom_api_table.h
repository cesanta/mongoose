/*
 *     Copyright 2020-2021 NXP
 *     All rights reserved.
 *
 *     SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef __NBOOT_ROM_API_TABLE_H__
#define __NBOOT_ROM_API_TABLE_H__

typedef int romapi_status_t;

typedef struct
{
    romapi_status_t (*nboot_rsvd0)(void);
    fsl_nboot_status_t (*nboot_context_init)(fsl_nboot_context_t *context);
    fsl_nboot_status_t (*nboot_context_deinit)(fsl_nboot_context_t *context);
    fsl_nboot_status_protected_t (*nboot_sb3_load_manifest)(fsl_nboot_context_t *context,
                                                        uint32_t *manifest,
                                                        fsl_nboot_sb3_load_manifest_parms_t *parms);
    fsl_nboot_status_protected_t (*nboot_sb3_load_block)(fsl_nboot_context_t *context, uint32_t *block);
    fsl_nboot_status_protected_t (*nboot_rsvd1)(void);
    fsl_nboot_status_protected_t (*nboot_rsvd2)(void);
} nboot_interface_v0_t;

typedef struct
{
    romapi_status_t (*romapi_rng_generate_random)(uint8_t *output, size_t outputByteLen);
    fsl_nboot_status_t (*nboot_context_init)(fsl_nboot_context_t *context);
    fsl_nboot_status_t (*nboot_context_deinit)(fsl_nboot_context_t *context);
    fsl_nboot_status_protected_t (*nboot_sb3_load_manifest)(fsl_nboot_context_t *context,
                                                        uint32_t *manifest,
                                                        fsl_nboot_sb3_load_manifest_parms_t *parms);
    fsl_nboot_status_protected_t (*nboot_sb3_load_block)(fsl_nboot_context_t *context, uint32_t *block);
    fsl_nboot_status_protected_t (*nboot_img_authenticate_ecdsa)(fsl_nboot_context_t *context,
                                                             uint8_t imageStartAddress[],
                                                             fsl_nboot_bool_t *isSignatureVerified,
                                                             fsl_nboot_img_auth_ecdsa_parms_t *parms);
} nboot_interface_v1_t;

#endif /* _NBOOT_ROM_API_TABLE_H_ */
