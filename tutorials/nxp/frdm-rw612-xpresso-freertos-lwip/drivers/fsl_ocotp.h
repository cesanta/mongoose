/*
 * Copyright 2021-2023 NXP
 *  
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __FSL_OCOTP_H_
#define __FSL_OCOTP_H_

#include "fsl_common.h"
/*!
 * @addtogroup ocotp
 * @{
 */

/*! @file */

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*! @name Driver version */
/*@{*/
/*! @brief OCOTP driver version 2.2.1. */
#define FSL_OCOTP_DRIVER_VERSION (MAKE_VERSION(2, 2, 1))
/*@}*/

/*! @brief OCOTP unique ID length. */
#define FSL_OCOTP_UID_LENGTH 16U

/*! @brief OTP Status Group */
enum
{
    kStatusGroup_OtpGroup = 0x210,
};

/*! @brief OTP Error Status definitions */
enum
{
    kStatus_OTP_InvalidAddress = MAKE_STATUS(kStatusGroup_OtpGroup, 1), /*!< Invalid OTP address */
    kStatus_OTP_Timeout        = MAKE_STATUS(kStatusGroup_OtpGroup, 7), /*!< OTP operation time out */
};

/*******************************************************************************
 * API
 ******************************************************************************/
#if defined(__cplusplus)
extern "C" {
#endif

/*!
 * @brief Initialize OTP controller
 *
 * This function enables OTP Controller clock.
 *
 * @return kStatus_Success
 */
status_t OCOTP_OtpInit(void);

/*!
 * @brief De-Initialize OTP controller
 *
 * This functin disables OTP Controller Clock.
 * @return kStatus_Success
 */
status_t OCOTP_OtpDeinit(void);

/*!
 * @brief Read Fuse value from OTP Fuse Block
 *
 * This function read fuse data from OTP Fuse block to specified data buffer.
 *
 * @param addr Fuse address
 * @param data Buffer to hold the data read from OTP Fuse block
 * @return kStatus_Success - Data read from OTP Fuse block successfully
 *         kStatus_OTP_Timeout - OTP read timeout
 *         kStatus_InvalidArgument - data pointer is invalid
 */
status_t OCOTP_OtpFuseRead(uint32_t addr, uint32_t *data);

/*!
 * @brief Read Fuse line with specific tag value from SoC OTP
 *
 * This function read Fuse line with specific tag value from SoC OTP to specified data buffer.
 *
 * @param data Buffer to hold the data read from SoC OTP
 * @param tag  Tag value to match
 * @return kStatus_Success - Data read from SoC OTP successfully
 *         kStatus_Fail    - Data read from SoC OTP failed, or cannot find the tag
 *         kStatus_InvalidArgument - data pointer is invalid
 */
status_t OCOTP_ReadSocOtp(uint64_t *data, uint32_t tag);

/*!
 * @brief Read unique ID from OTP Fuse Block
 *
 * This function read unique ID from OTP Fuse block to specified data buffer.
 *
 * @param uid The buffer to store unique ID, buffer byte length is FSL_OCOTP_UID_LENGTH.
 * @param idLen[in/out] The unique ID byte length. Set the length to read, return the length read out.
 * @return kStatus_Success - Data read from OTP Fuse block successfully
 *         kStatus_OTP_Timeout - OTP read timeout
 *         kStatus_InvalidArgument - data pointer is invalid
 */
status_t OCOTP_ReadUniqueID(uint8_t *uid, uint32_t *idLen);

/*!
 * @brief Read Static Voltage Compansation from SOC OTP
 *
 * This function read SVC from OTP Fuse block to specified data buffer.
 *
 * @param svc The buffer to store SVC.
 * @return kStatus_Success - Data read from SOC OTP successfully
 *         kStatus_Fail    - SOC OTP read failure
 */
status_t OCOTP_ReadSVC(uint64_t *svc);

/*!
 * @brief Read package type from SOC OTP
 *
 * @param pack The buffer to store package type.
 * @return kStatus_Success - Data read from SOC OTP successfully
 *         kStatus_Fail    - SOC OTP read failure
 */
status_t OCOTP_ReadPackage(uint32_t *pack);

#if defined(__cplusplus)
}
#endif

/*! @}*/

#endif /* __FSL_OCOTP_H_ */
