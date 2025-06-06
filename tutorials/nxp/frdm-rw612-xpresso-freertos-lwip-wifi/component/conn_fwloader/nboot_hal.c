/*
 * Copyright 2020 - 2021,2024 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include <limits.h>
#include "fusemap.h"
#include "fsl_loader_utils.h"
#include "life_cycle.h"
#include "fsl_ocotp.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define NBOOT_HAL_PRINTF(...)

/*******************************************************************************
 * Prototype
 ******************************************************************************/
/*!
 * @brief Get the root security parameters.
 *
 * Read the root security configurations from OTP.
 *
 * @param context   NBOOT context.
 * @param parms     Root security parameters.
 * @retval kStatus_NBOOT_Fail Operate successfully.
 * @retval kStatus_NBOOT_InvalidArgument Invalid values of the parms
 * @retval kStatus_NBOOT_Fail Failed to operate.
 */
static fsl_nboot_status_t nboot_hal_get_root_auth_parms(fsl_nboot_context_t *context, fsl_nboot_rot_auth_parms_t *parms);

/*!
 * @brief Get the ROTK revoke settings.
 *
 * Read the OTP_ROTK_REVOKE.
 *
 * @param rotkRevoke ROTK revoke state.
 * @param rotkCnt ROTK count.
 * @retval kStatus_NBOOT_Fail Operate successfully.
 * @retval kStatus_NBOOT_Fail Failed to operate.
 */
static inline fsl_nboot_status_t nboot_hal_get_rotk_revoke(fsl_nboot_root_key_revocation_t *rotkRevoke, uint32_t rotkCnt);

/*!
 * @brief Get the ROTKH.
 *
 * Read the IFR_ROTKH.
 *
 * @param rotkh ROTKH value.
 * @param rotkhSize ROTKH size in bytes.
 * @retval kStatus_NBOOT_Fail Operate successfully.
 * @retval kStatus_NBOOT_Fail Failed to operate.
 */
static fsl_nboot_status_t nboot_hal_get_rotkh(uint32_t *rotkh, uint32_t rotkhSize);

/*!
 * @brief Get the type of the root keys.
 *
 * Get the root key type
 *
 * @param rootKeyType Type of the root keys.
 * @retval kStatus_NBOOT_Fail Operate successfully.
 * @retval kStatus_NBOOT_Fail Failed to operate.
 */
static inline fsl_nboot_status_t nboot_hal_get_root_key_type(fsl_nboot_root_key_type_and_length_t *rootKeyType);

/*!
 * @brief Get the Part Common Key(PCK).
 *
 * Read the IFR_PCK_BLOB.
 *
 * @param pckBlob PCK blob.
 * @param pckBlobSize PCK blob size in byte.
 * @retval kStatus_NBOOT_Fail Operate successfully.
 * @retval kStatus_NBOOT_Fail Failed to operate.
 */
static inline fsl_nboot_status_t nboot_hal_get_pck_blob(uint8_t *pckBlob, uint32_t pckBlobSize);

/*******************************************************************************
 * Codes
 ******************************************************************************/
static inline uint32_t set_antipole(uint32_t value)
{
    return (value & 0xFFFFu) | (~(value & 0xFFFFu) << 16);
}

static fsl_nboot_status_t nboot_hal_get_root_auth_parms(fsl_nboot_context_t *context, fsl_nboot_rot_auth_parms_t *parms)
{
    if ((NULL == context) || (NULL == parms))
    {
        return kStatus_NBOOT_InvalidArgument;
    }

    fsl_nboot_status_t status = kStatus_NBOOT_Fail;
    do
    {
        status =
            nboot_hal_get_rotk_revoke(&parms->soc_rootKeyRevocation[0],
                                      sizeof(parms->soc_rootKeyRevocation) / sizeof(parms->soc_rootKeyRevocation[0]));
        if (status != kStatus_NBOOT_Success)
        {
            break;
        }

        status = nboot_hal_get_rotkh(&parms->soc_rkh[0], sizeof(parms->soc_rkh));
        if (status != kStatus_NBOOT_Success)
        {
            break;
        }

        parms->soc_numberOfRootKeys = 0u;
        parms->soc_rootKeyUsage[1]  = kNBOOT_RootKeyUsage_Unused;
        parms->soc_rootKeyUsage[2]  = kNBOOT_RootKeyUsage_Unused;
        parms->soc_rootKeyUsage[3]  = kNBOOT_RootKeyUsage_Unused;
        for (size_t i = 0; i < (sizeof(parms->soc_rootKeyUsage) / sizeof(parms->soc_rootKeyUsage[0])); i++)
        {
            if ((parms->soc_rootKeyUsage[i] & OTP_ROTK0_USAGE_FUSE_MASK) != kNBOOT_RootKeyUsage_Unused)
            {
                parms->soc_numberOfRootKeys++;
            }
        }

        status = nboot_hal_get_root_key_type(&parms->soc_rootKeyTypeAndLength);
        if (status != kStatus_NBOOT_Success)
        {
            break;
        }

        parms->soc_lifecycle = (fsl_nboot_soc_lifecycle_t)set_antipole(get_lifecycle_state());
        status               = kStatus_NBOOT_Success;
    } while (false);
	
	if (status != kStatus_NBOOT_Success)
	{
	    (void)memset(parms, 0, sizeof(*parms));
	}

    return status;
}

static inline fsl_nboot_status_t nboot_hal_get_rotk_revoke(fsl_nboot_root_key_revocation_t *rotkRevoke, uint32_t rotkCnt)
{
    /* No need to check the input arguments for this inline functions. */
    assert(rotkRevoke);
    assert(rotkCnt == NBOOT_ROOT_CERT_COUNT);

    /* Set all root key to 'revoked' state */
    for (uint32_t i = 0u; i < rotkCnt; i++)
    {
        rotkRevoke[i] = kNBOOT_RootKey_Enabled;
    }

    return kStatus_NBOOT_Success;
}

static fsl_nboot_status_t nboot_hal_get_rotkh(uint32_t *rotkh, uint32_t rotkhSize)
{
    /* No need to check the input arguments for this inline functions. */
    assert(rotkh);
    assert(rotkhSize == (uint32_t)NBOOT_ROOT_ROTKH_SIZE_IN_BYTE);

    fsl_nboot_status_t status = kStatus_NBOOT_Success;

    /* root key hash fixed in Flash memory */
#ifdef USE_ENG_CERTIFICATE
    rotkh[0]  = 0xd0cfb419U;
    rotkh[1]  = 0x4037ee3cU;
    rotkh[2]  = 0xde74393eU;
    rotkh[3]  = 0x0156d0a3U;
    rotkh[4]  = 0x373b8677U;
    rotkh[5]  = 0x6b6aee3dU;
    rotkh[6]  = 0x619b459eU;
    rotkh[7]  = 0xfa33f31dU;
    rotkh[8]  = 0x00000000U;
    rotkh[9]  = 0x00000000U;
    rotkh[10] = 0x00000000U;
    rotkh[11] = 0x00000000U;
#else
    if ((get_chip_revision() == 0U))
    {
        rotkh[0]  = 0x60DFBEE6U;
        rotkh[1]  = 0x8799305FU;
        rotkh[2]  = 0xBA9E4AE6U;
        rotkh[3]  = 0x1908394FU;
        rotkh[4]  = 0x7AC4F934U;
        rotkh[5]  = 0xEF76BF41U;
        rotkh[6]  = 0x2E27796EU;
        rotkh[7]  = 0x94DB19A0U;
        rotkh[8]  = 0x00000000U;
        rotkh[9]  = 0x00000000U;
        rotkh[10] = 0x00000000U;
        rotkh[11] = 0x00000000U;
    }
    else if ((get_chip_revision() == 1U))
    {
        rotkh[0]  = 0x9C758C58U;
        rotkh[1]  = 0x0A5CCEAAU;
        rotkh[2]  = 0x850DAD41U;
        rotkh[3]  = 0x1371EEBAU;
        rotkh[4]  = 0xB7874851U;
        rotkh[5]  = 0x53C5BA44U;
        rotkh[6]  = 0xF236F964U;
        rotkh[7]  = 0x3320ECDFU;
        rotkh[8]  = 0x00000000U;
        rotkh[9]  = 0x00000000U;
        rotkh[10] = 0x00000000U;
        rotkh[11] = 0x00000000U;
    }
    else if ((get_chip_revision() == 2U))
    {
        rotkh[0]  = 0xE7C7E9BBU;
        rotkh[1]  = 0x12C8C535U;
        rotkh[2]  = 0x37E61148U;
        rotkh[3]  = 0x2BE7F18CU;
        rotkh[4]  = 0x8F0E3094U;
        rotkh[5]  = 0xB2BA7F32U;
        rotkh[6]  = 0xEC9B4ECBU;
        rotkh[7]  = 0xAD9FC941U;
        rotkh[8]  = 0x00000000U;
        rotkh[9]  = 0x00000000U;
        rotkh[10] = 0x00000000U;
        rotkh[11] = 0x00000000U;
    }
	else
	{
	    ; /* none to do */
	}
#endif
    return status;
}

static inline fsl_nboot_status_t nboot_hal_get_root_key_type(fsl_nboot_root_key_type_and_length_t *rootKeyType)
{
    /* No need to check the input arguments for this inline functions. */
    assert(rootKeyType);

    *rootKeyType = kNBOOT_RootKey_Ecdsa_P256;

    return kStatus_NBOOT_Success;
}

static inline fsl_nboot_status_t nboot_hal_get_pck_blob(uint8_t *pckBlob, uint32_t pckBlobSize)
{
    /* No need to check the input arguments for this inline functions. */
    assert(pckBlob);
    assert(pckBlobSize == (uint32_t)NBOOT_PCK_BLOB_SIZE_IN_BYTE);

    fsl_nboot_status_t status = kStatus_NBOOT_Fail;
    status_t otpStatus    = kStatus_Fail;

    do
    {
        uint32_t fuseIdxStart = OTP_NXP_WIFI_SK_MK_31_0_FUSE_IDX;
        for (int i = 0; i < (NBOOT_PCK_BLOB_SIZE_IN_BYTE / 4); i++)
        {
            otpStatus = OCOTP_OtpFuseRead(fuseIdxStart, (uint32_t *)(&pckBlob[4 * i]));
            if (otpStatus != kStatus_Success)
            {
                return kStatus_NBOOT_Fail;
            }
            ++fuseIdxStart;
        }
        status = kStatus_NBOOT_Success;
    } while (false);

    return status;
}

fsl_nboot_status_t nboot_hal_get_sb3_manifest_params(fsl_nboot_context_t *context, fsl_nboot_sb3_load_manifest_parms_t *parms)
{
    if ((NULL == context) || (NULL == parms))
    {
        return kStatus_NBOOT_InvalidArgument;
    }

    fsl_nboot_status_t status = kStatus_NBOOT_Fail;
    do
    {
        status = nboot_hal_get_root_auth_parms(context, &parms->soc_RoTNVM);
        if (status != kStatus_NBOOT_Success)
        {
            break;
        }

        status = nboot_hal_get_pck_blob(&parms->pckBlob[0], sizeof(parms->pckBlob));
        if (status != kStatus_NBOOT_Success)
        {
            break;
        }

        status = kStatus_NBOOT_Success;
    } while (false);

    if (status != kStatus_NBOOT_Success)
    {
        (void)memset(parms, 0, sizeof(*parms));
    }

    return status;
}
