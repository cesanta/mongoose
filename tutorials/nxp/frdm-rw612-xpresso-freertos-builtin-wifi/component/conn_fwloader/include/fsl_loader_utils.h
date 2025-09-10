/*
 * Copyright 2024 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __FSL_LOADER_UTILS_H__
#define __FSL_LOADER_UTILS_H__

#include <stdint.h>
#include "fsl_common.h"
#include "fsl_loader.h"
#include "fusemap.h"
#include "fsl_ocotp.h"
#include "fsl_os_abstraction.h"

//! @addtogroup sbloader
//! @{
/*! @brief Status group numbers. */
#define kStatusGroup_SBLoader                  (101U)
#define NBOOT_SB3_BLOCK_HASH384_SIZE_IN_BYTES  (48u)
#define NBOOT_ROOT_OF_TRUST_HASH_SIZE_IN_BYTES (48u)
#define NBOOT_EC_COORDINATE_384_SIZE_IN_BYTES  (48u)
#define NBOOT_EC_COORDINATE_MAX_SIZE           NBOOT_EC_COORDINATE_384_SIZE_IN_BYTES
#define NBOOT_ROOT_CERT_COUNT                  (4u)
#define NBOOT_SB3_CHUNK_SIZE_IN_BYTES          (256u)
#define NBOOT_KEYINFO_WORDLEN                  (23u)
#define NXPCLHASH_WA_SIZE_MAX                  (128u + 64u)
#define NBOOT_CONTEXT_BYTELEN                  (192u + NXPCLHASH_WA_SIZE_MAX)
#define NBOOT_CONTEXT_WORDLEN                  (NBOOT_CONTEXT_BYTELEN / sizeof(uint32_t))
#define NXPCLCSS_HASH_RTF_OUTPUT_SIZE          ((size_t)32U)

#define SECURE_TERM_PART_LOCK        (0x6ac3c36au)
#define SECURE_TERM_PART_OPEN        (0xc36ac36au)
#define SECURE_TERM_SECURE_PART_OPEN (0xc36a6ac3u)
#define SECURE_TERM_FA_PART_OPEN     (0xc3c36a6au)
#define SECURE_OEM_FA_PART_OPEN      (0xc3c3a6a6u)

#define LOADER_RAW_BINARY_FORMAT      (0x72617762U)
#ifdef CONFIG_FW_VDLLV2
#define LOADER_VDLL_RAW_BINARY_FORMAT (0x76646c6cU)
#endif

/*! @brief partition table constants. */
#define WIFI_IMAGE_SIZE_MAX (0xa0000U)
#define BLE_IMAGE_SIZE_MAX  (0x50000U)
#define Z154_IMAGE_SIZE_MAX (0x50000U)
#ifndef WIFI_IMAGE_A_OFFSET
#define WIFI_IMAGE_A_OFFSET (0x08400000U)
#endif
#define WIFI_IMAGE_B_OFFSET (WIFI_IMAGE_A_OFFSET + WIFI_IMAGE_SIZE_MAX) // 0x4a0000
#define BLE_IMAGE_A_OFFSET  (WIFI_IMAGE_B_OFFSET + WIFI_IMAGE_SIZE_MAX) // 0x540000
#define BLE_IMAGE_B_OFFSET  (BLE_IMAGE_A_OFFSET + BLE_IMAGE_SIZE_MAX)   // 0x590000
#define Z154_IMAGE_A_OFFSET (BLE_IMAGE_B_OFFSET + BLE_IMAGE_SIZE_MAX)   // 0x5e0000
#define Z154_IMAGE_B_OFFSET (Z154_IMAGE_A_OFFSET + Z154_IMAGE_SIZE_MAX) // 0x630000

/** Type for nboot status codes */
typedef uint32_t fsl_nboot_status_t;

/**
 * \defgroup nbootStatusValues  This type defines status return values used by NBOOT functions that are not easily
 * disturbed by Fault Attacks
 * @{
 */
#define kStatus_NBOOT_Success                ((fsl_nboot_status_t)0x5A5A5A5Au) /*!< Operation completed successfully. */
#define kStatus_NBOOT_Fail                   ((fsl_nboot_status_t)0x5A5AA5A5u) /*!< Operation failed. */
#define kStatus_NBOOT_InvalidArgument        ((fsl_nboot_status_t)0x5A5AA5F0u) /*!< Invalid argument passed to the function. */
#define kStatus_NBOOT_RequestTimeout         ((fsl_nboot_status_t)0x5A5AA5E1u) /*!< Operation timed out. */
#define kStatus_NBOOT_KeyNotLoaded           ((fsl_nboot_status_t)0x5A5AA5E2u) /*!< The requested key is not loaded. */
#define kStatus_NBOOT_AuthFail               ((fsl_nboot_status_t)0x5A5AA5E4u) /*!< Authentication failed. */
#define kStatus_NBOOT_OperationNotAvaialable ((fsl_nboot_status_t)0x5A5AA5E5u) /*!< Operation not available on this HW. */
#define kStatus_NBOOT_KeyNotAvailable        ((fsl_nboot_status_t)0x5A5AA5E6u) /*!< Key is not avaialble. */
#define kStatus_NBOOT_IvCounterOverflow      ((fsl_nboot_status_t)0x5A5AA5E7u) /*!< Overflow of IV counter (PRINCE/IPED). */
#define kStatus_NBOOT_SelftestFail           ((fsl_nboot_status_t)0x5A5AA5E8u) /*!< FIPS self-test failure. */
#define kStatus_NBOOT_InvalidDataFormat      ((fsl_nboot_status_t)0x5A5AA5E9u) /*!< Invalid data format for example antipole */
#define kStatus_NBOOT_IskCertUserDataTooBig \
    ((fsl_nboot_status_t)0x5A5AA5EAu) /*!< Size of User data in ISK certificate is greater than 96 bytes */
#define kStatus_NBOOT_IskCertSignatureOffsetTooSmall \
    ((fsl_nboot_status_t)0x5A5AA5EBu) /*!< Signature offset in ISK certificate is smaller than expected */
#define kStatus_NBOOT_MemcpyFail ((fsl_nboot_status_t)0x5A5A845A) /*!< Unexpected error detected during nboot_memcpy() */

/*! @brief sb3.1 maigc number. */
#define TAG_SB_V3 (0x33766273U) // "sbv3"

/*!
 * @brief NBOOT type for the root key revocation
 *
 * This type defines the NBOOT root key revocation
 *
 */
#define kNBOOT_RootKey_Enabled (0xAAu)
#define kNBOOT_RootKey_Revoked (0xBBu)

/*! @brief The size of the root of trust key table hash. */
#define NBOOT_ROOT_ROTKH_SIZE_IN_WORD (12U)
#define NBOOT_ROOT_ROTKH_SIZE_IN_BYTE (NBOOT_ROOT_ROTKH_SIZE_IN_WORD * 4U)

/*! @brief The size of PKC Blob. */
#define NBOOT_PCK_BLOB_SIZE_IN_WORD (12U)
#define NBOOT_PCK_BLOB_SIZE_IN_BYTE (NBOOT_PCK_BLOB_SIZE_IN_WORD * 4U)

/*!
 * @brief NBOOT type specifying the elliptic curve to be used
 *
 * This type defines the elliptic curve type and length
 *
 */
#define kNBOOT_RootKey_Ecdsa_P256 (0x0000FE01u)
#define kNBOOT_RootKey_Ecdsa_P384 (0x0000FD02u)

/*!
 * @brief NBOOT type for the root key usage
 *
 * This type defines the NBOOT root key usage
 *
 */
#define kNBOOT_RootKeyUsage_DebugCA_ImageCA_FwCA_ImageKey_FwKey (0x0u)
#define kNBOOT_RootKeyUsage_Unused                              (0x7u)

//! @brief SB loader status codes.
enum _sbloader_status
{
    kStatusRomLdrSectionOverrun  = MAKE_STATUS(kStatusGroup_SBLoader, 0),
    kStatusRomLdrSignature       = MAKE_STATUS(kStatusGroup_SBLoader, 1),
    kStatusRomLdrSectionLength   = MAKE_STATUS(kStatusGroup_SBLoader, 2),
    kStatusRomLdrUnencryptedOnly = MAKE_STATUS(kStatusGroup_SBLoader, 3),
    kStatusRomLdrEOFReached      = MAKE_STATUS(kStatusGroup_SBLoader, 4),
    kStatusRomLdrChecksum        = MAKE_STATUS(kStatusGroup_SBLoader, 5),
    kStatusRomLdrCrc32Error      = MAKE_STATUS(kStatusGroup_SBLoader, 6),
    kStatusRomLdrUnknownCommand  = MAKE_STATUS(kStatusGroup_SBLoader, 7),
    kStatusRomLdrIdNotFound      = MAKE_STATUS(kStatusGroup_SBLoader, 8),
    kStatusRomLdrDataUnderrun    = MAKE_STATUS(kStatusGroup_SBLoader, 9),
    kStatusRomLdrJumpReturned    = MAKE_STATUS(kStatusGroup_SBLoader, 10),
    kStatusRomLdrCallFailed      = MAKE_STATUS(kStatusGroup_SBLoader, 11),
    kStatusRomLdrKeyNotFound     = MAKE_STATUS(kStatusGroup_SBLoader, 12),
    kStatusRomLdrSecureOnly      = MAKE_STATUS(kStatusGroup_SBLoader, 13),
    kStatusRomLdrResetReturned   = MAKE_STATUS(kStatusGroup_SBLoader, 14),

    kStatusRomLdrRollbackBlocked        = MAKE_STATUS(kStatusGroup_SBLoader, 15),
    kStatusRomLdrInvalidSectionMacCount = MAKE_STATUS(kStatusGroup_SBLoader, 16),
    kStatusRomLdrUnexpectedCommand      = MAKE_STATUS(kStatusGroup_SBLoader, 17),
    kStatusRomLdrBadSBKEK               = MAKE_STATUS(kStatusGroup_SBLoader, 18),
    kStatusRomLdrPendingJumpCommand     = MAKE_STATUS(kStatusGroup_SBLoader, 19),
};

/*!
 * @brief Boolean type for the NBOOT functions
 *
 * This type defines boolean values used by NBOOT functions that are not easily disturbed by Fault Attacks
 *
 */
typedef enum _fsl_nboot_bool
{
    kNBOOT_TRUE                = 0x3C5AC33Cu, /*!< Value for TRUE.  */
    kNBOOT_TRUE256             = 0x3C5AC35Au, /*!< Value for TRUE when P256 was used to sign the image.  */
    kNBOOT_TRUE384             = 0x3C5AC3A5u, /*!< Value for TRUE when P384 was used to sign the image.  */
    kNBOOT_FALSE               = 0x5AA55AA5u, /*!< Value for FALSE. */
    kNBOOT_OperationAllowed    = 0x3c5a33ccU,
    kNBOOT_OperationDisallowed = 0x5aa5cc33U,
} fsl_nboot_bool_t;

/** Type for nboot protected status codes */
typedef uint64_t fsl_nboot_status_protected_t;

/*!
 * @brief NBOOT type for a timestamp
 *
 * This type defines the NBOOT timestamp
 *
 */
typedef uint32_t fsl_nboot_timestamp_t[2];
typedef uint32_t fsl_nboot_root_key_revocation_t;
typedef uint32_t fsl_nboot_root_key_usage_t;
typedef uint32_t fsl_nboot_root_key_type_and_length_t;
typedef uint32_t fsl_nboot_soc_lifecycle_t;

/*!
 * @brief NBOOT type for the root of trust parameters
 *
 * This type defines the NBOOT root of trust parameters
 *
 */
#define kNBOOT_SocRkh_Size_Words_P384 (12u)
#define kNBOOT_SocRkh_Size_Words_P256 (8u)
typedef struct _fsl_nboot_rot_auth_parms
{
    /* trusted information originated from CFPA */
    fsl_nboot_root_key_revocation_t soc_rootKeyRevocation[NBOOT_ROOT_CERT_COUNT]; /*!< Provided by caller based on NVM
                                                                                 information in CFPA: ROTKH_REVOKE */
    uint32_t soc_imageKeyRevocation; /*!< Provided by caller based on NVM information in CFPA: IMAGE_KEY_REVOKE */

    /* trusted information originated from CMPA */
    uint32_t soc_rkh[kNBOOT_SocRkh_Size_Words_P384]; /*!< Provided by caller based on NVM information in CMPA: ROTKH
                                                        (hash of hashes) */
    /*!< In case of kNBOOT_RootKey_Ecdsa_P384, sock_rkh[0..11] are used */
    /*!< In case of kNBOOT_RootKey_Ecdsa_P256, sock_rkh[0..7] are used */

    uint32_t soc_numberOfRootKeys; /* unsigned int, between minimum = 1 and maximum = 4; */
    fsl_nboot_root_key_usage_t soc_rootKeyUsage[NBOOT_ROOT_CERT_COUNT]; /* CMPA */
    fsl_nboot_root_key_type_and_length_t
        soc_rootKeyTypeAndLength; /* static selection between ECDSA P-256 or ECDSA P-384 based root keys */

    /* trusted information originated from OTP fuses */
    fsl_nboot_soc_lifecycle_t soc_lifecycle;
} fsl_nboot_rot_auth_parms_t;

/*!
 * @brief NBOOT SB3.1 header type
 *
 * This type defines the header used in the SB3.1 manifest
 *
 */
typedef struct _fsl_nboot_sb3_header
{
    uint32_t magic;         /*! offset 0x00: Fixed 4-byte string of 'sbv3' without the trailing NULL */
    uint32_t formatVersion; /*! offset 0x04: (major = 3, minor = 1); The format version determines the manifest (block0)
                               size. */
    uint32_t flags;         /*! offset 0x08: not defined yet, keep zero for future compatibility */
    uint32_t blockCount;    /*! offset 0x0C: Number of blocks not including the manifest (block0). */
    uint32_t blockSize; /*! offset 0x10: Size in bytes of data block (repeated blockCount times for SB3 data stream). */
    fsl_nboot_timestamp_t timeStamp;     /*! offset 0x14: 64-bit value used as key derivation data. */
    uint32_t firmwareVersion;        /*! offset 0x1c: Version number of the included firmware */
    uint32_t imageTotalLength;       /*! offset 0x20: Total manifest length in bytes, including signatures etc. */
    uint32_t imageType;              /*! offset 0x24: image type and flags */
    uint32_t certificateBlockOffset; /*! offset 0x28: Offset from start of header block to the certificate block. */
    uint8_t description[16];         /*! offset 0x32: This field provides description of the file. It is an arbitrary
                                                      string injected by the signing tool, which helps to identify the file. */
} fsl_nboot_sb3_header_t;

/*!
 * @brief  manifest loading parameters
 *
 * This type defines the NBOOT SB3.1 manifest loading parameters
 *
 */
typedef struct _fsl_nboot_sb3_load_manifest_parms
{
    fsl_nboot_rot_auth_parms_t soc_RoTNVM;   /*! trusted information originated from CFPA and NMPA */
    uint32_t soc_trustedFirmwareVersion; /*!< Provided by caller based on NVM information in CFPA: Secure_FW_Version */
    uint8_t pckBlob[48];                 /*! CSSv2 protected blob with Part Common Key (PCK) */
} fsl_nboot_sb3_load_manifest_parms_t;

/*! @brief Data structure holding input arguments to POR secure boot (authentication) algorithm. Shall be read from SoC
 * trusted NVM or SoC fuses. */
typedef struct _fsl_nboot_img_auth_ecdsa_parms
{
    /* trusted information originated from CFPA and NMPA */
    fsl_nboot_rot_auth_parms_t soc_RoTNVM;

    uint32_t soc_trustedFirmwareVersion; /*!< Provided by caller based on NVM information in CFPA: Secure_FW_Version */
} fsl_nboot_img_auth_ecdsa_parms_t;

/*! @brief The size of the blob with Key Blob. */
#define NBOOT_KEY_BLOB_SIZE_IN_BYTE_256 (32)
#define NBOOT_KEY_BLOB_SIZE_IN_BYTE_384 (48)
#define NBOOT_KEY_BLOB_SIZE_IN_BYTE_MAX (NBOOT_KEY_BLOB_SIZE_IN_BYTE_384)

#define NBOOT_SB3_MANIFEST_MAX_SIZE_IN_BYTES (808)
#define NBOOT_SB3_BLOCK_MAX_SIZE_IN_BYTES    (308)

// Provides forward reference to the loader context definition.
typedef struct _fsl_ldr_Context_v3 fsl_ldr_Context_v3_t;

//! sb3 section definitions

//! section type
typedef enum _fsl_sectionType
{
    kSectionNone       = 0, // end or invalid
    kSectionDataRange  = 1,
    kSectionDiffUpdate = 2,
    kSectionDDRConfig  = 3,
    kSectionRegister   = 4,
} fsl_section_type_t;

#define SB3_DATA_RANGE_HEADER_FLAGS_ERASE_MASK (0x1u) // bit 0
#define SB3_DATA_RANGE_HEADER_FLAGS_LOAD_MASK  (0x2u) // bit 1

#define SB3_DATA_RANGE_HEADER_TAG       (0x55aaaa55U)
#define SB3_DATA_ALIGNMENT_SIZE_IN_BYTE (16u)

//! section data range structure
typedef struct fsl_range_header
{
    uint32_t tag;
    uint32_t startAddress;
    uint32_t length;
    uint32_t cmd;
} fsl_sb3_data_range_header_t;

typedef struct fsl_range_header_expansion
{
    uint32_t memoryId;
    uint32_t pad0;
    uint32_t pad1;
    uint32_t pad2;
} fsl_sb3_data_range_expansion_t;

//! sb3 DATA section header format
typedef struct fsl_section_header
{
    uint32_t sectionUid;
    uint32_t sectionType;
    uint32_t length;
    uint32_t _pad;
} fsl_sb3_section_header_t;

// loader command enum

typedef enum _fsl_loader_command_sb3
{
    kSB3_CmdInvalid = 0,
    kSB3_CmdLoad    = 2,
    kSB3_CmdExecute = 3,
} fsl_sb3_cmd_t;

//! The all of the allowed command
#define SBLOADER_V3_CMD_SET_ALL ((1u << kSB3_CmdLoad) | (1u << kSB3_CmdExecute))

#define SB3_DATA_BUFFER_SIZE_IN_BYTE (MAX(128, NBOOT_KEY_BLOB_SIZE_IN_BYTE_MAX))

/*! @brief Data structure holding secure counter value used by nboot library */
typedef struct _fsl_nboot_secure_counter
{
    uint32_t sc;
    uint32_t scAp;
} fsl_nboot_secure_counter_t;

/*!
 * @brief NBOOT context type
 *
 * This type defines the NBOOT context
 *
 */
typedef struct _fsl_nboot_context
{
    uint32_t totalBlocks; /*!< holds number of SB3 blocks. Initialized by nboot_sb3_load_header(). */
    uint32_t processData; /*!< flag, initialized by nboot_sb3_load_header().
                             SB3 related flag set by NBOOT in case the nboot_sb3_load_block()
                             provides plain data to output buffer (for processing by ROM SB3 loader */
    uint32_t timeout;     /*!< timeout value for css operation. In case it is 0, infinite wait is performed */
    uint32_t keyinfo[NBOOT_KEYINFO_WORDLEN]; /*!< data for NBOOT key management. */
    uint32_t context[NBOOT_CONTEXT_WORDLEN]; /*!< work area for NBOOT lib. */
    uint32_t uuid[4];                        /*!< holds UUID value from NMPA */
    uint32_t prngReadyFlag;     /*!< flag, used by nboot_rng_generate_lq_random() to determine whether CSS is ready to
                                   generate rnd number */
    uint32_t multipartMacBuffer[1024 / sizeof(uint32_t)];
    uint32_t oemShareValidFlag; /*!< flag, used during TP to determine whether valid oemShare was set by
                                   nboot_tp_isp_gen_oem_master_share() */
    uint32_t oemShare[4]; /*!< buffer to store OEM_SHARE computed by nxpCLTrustProv_nboot_isp_gen_oem_master_share() */
    fsl_nboot_secure_counter_t secureCounter; /*!< Secure counter used by nboot */
    uint32_t rtf[NXPCLCSS_HASH_RTF_OUTPUT_SIZE / sizeof(uint32_t)];
    uint32_t imageHash[48 / sizeof(uint32_t)];
    uint32_t authStatus;
    fsl_nboot_bool_t disableProvisioningFirmwareNXP; /*!< Flag to disable execution of NXP signed provisioning Firmwares */
} fsl_nboot_context_t;

//! @brief Structure of version property.
//!
//! @ingroup bl_core
typedef union fsl_StandardVersion
{
    struct
    {
        uint8_t bugfix; //!< bugfix version [7:0]
        uint8_t minor;  //!< minor version [15:8]
        uint8_t major;  //!< major version [23:16]
        char name;      //!< name [31:24]
    };
    uint32_t version;   //!< combined version numbers

#if defined(__cplusplus)
    StandardVersion() : version(0)
    {
    }
    StandardVersion(uint32_t version) : version(version)
    {
    }
#endif
} fsl_standard_version_t;

//!@brief Memory region information table
typedef struct fsl_mem_region
{
    uint32_t start;
    uint32_t end;
} fsl_mem_region_t;

//! @brief Memory Attribute Structure
typedef struct _fsl_mem_attribute
{
    uint32_t memId;
    uint32_t regionCount;
    fsl_mem_region_t *memRegions;
    void *context;
} fsl_mem_attribute_t;

typedef struct _fsl_arena_context
{
    uint32_t start;
    uint32_t end;
    uint32_t nextAddr;
} fsl_arena_context_t;

//!@brief Memory region interface structure
typedef struct fsl_api_memory_region_interface
{
    status_t (*init)(fsl_mem_attribute_t *attr);
#if defined(ROM_API_HAS_FEATURE_MEM_READ)
#if ROM_API_HAS_FEATURE_MEM_READ
    status_t (*read)(fsl_mem_attribute_t *attr, uint32_t addr, uint32_t leth, uint8_t *buf);
#endif
#endif

    status_t (*write)(fsl_mem_attribute_t *attr, uint32_t addr, uint32_t len, const uint8_t *buf);
    status_t (*fill)(fsl_mem_attribute_t *attr, uint32_t addr, uint32_t len, uint32_t pattern);
    status_t (*flush)(fsl_mem_attribute_t *attr);
    status_t (*erase)(fsl_mem_attribute_t *attr, uint32_t addr, uint32_t len);
    status_t (*config)(fsl_mem_attribute_t *attr, uint32_t *buf);
    status_t (*erase_all)(fsl_mem_attribute_t *attr);
    status_t (*alloc_ctx)(fsl_arena_context_t *ctx, fsl_mem_attribute_t *attr, void *miscParams);
} fsl_api_memory_region_interface_t;

/*******************************************************************************
 * Definitions
 ******************************************************************************/
//! @brief Contiguous RAM region count
#define RAM_REGION_COUNT (2U)

//! @brief Contiguous FLEXSPINOR meomry count
#define FLEXSPINOR_REGION_COUNT (1U)

//! @brief Memory Interface count
#define MEM_INTERFACE_COUNT (2U)

//!@brief FlexSPI LUT Sequence structure
typedef struct _fsl_lut_sequence
{
    uint8_t seqNum; //!< Sequence Number, valid number: 1-16
    uint8_t seqId;  //!< Sequence Index, valid number: 0-15
    uint16_t reserved;
} fsl_flexspi_lut_seq_t;

//!@brief FlexSPI Pad Configuration Override
typedef struct
{
    uint8_t pu_pd_override_en;
    uint8_t pu_pd_value;
    uint8_t sr_config_override_en;
    uint8_t sr_config_value;
} fsl_flexspi_pad_config_override_t;

typedef struct
{
    uint8_t time_100ps;  // Data valid time, in terms of 100ps
    uint8_t delay_cells; // Data valid time, in terms of delay cells
} fsl_flexspi_dll_time_t;

//!@brief FlexSPI Memory Configuration Block
typedef struct _fsl_FlexSPIConfig
{
    uint32_t tag;               //!< [0x000-0x003] Tag, fixed value 0x42464346UL
    uint32_t version;           //!< [0x004-0x007] Version,[31:24] -'V', [23:16] - Major, [15:8] - Minor, [7:0] - bugfix
    uint32_t reserved0;         //!< [0x008-0x00b] Reserved for future use
    uint8_t readSampleClkSrc;   //!< [0x00c-0x00c] Read Sample Clock Source, valid value: 0/1/3
    uint8_t csHoldTime;         //!< [0x00d-0x00d] CS hold time, default value: 3
    uint8_t csSetupTime;        //!< [0x00e-0x00e] CS setup time, default value: 3
    uint8_t columnAddressWidth; //!< [0x00f-0x00f] Column Address with, for HyperBus protocol, it is fixed to 3, For
    //! Serial NAND, need to refer to datasheet
    uint8_t deviceModeCfgEnable; //!< [0x010-0x010] Device Mode Configure enable flag, 1 - Enable, 0 - Disable
    uint8_t deviceModeType; //!< [0x011-0x011] Specify the configuration command type:Quad Enable, DPI/QPI/OPI switch,
    //! Generic configuration, etc.
    uint16_t waitTimeCfgCommands; //!< [0x012-0x013] Wait time for all configuration commands, unit: 100us, Used for
    //! DPI/QPI/OPI switch or reset command
    fsl_flexspi_lut_seq_t deviceModeSeq; //!< [0x014-0x017] Device mode sequence info, [7:0] - LUT sequence id, [15:8] - LUt
    //! sequence number, [31:16] Reserved
    uint32_t deviceModeArg;    //!< [0x018-0x01b] Argument/Parameter for device configuration
    uint8_t configCmdEnable;   //!< [0x01c-0x01c] Configure command Enable Flag, 1 - Enable, 0 - Disable
    uint8_t configModeType[3]; //!< [0x01d-0x01f] Configure Mode Type, similar as deviceModeTpe
    fsl_flexspi_lut_seq_t
        configCmdSeqs[3]; //!< [0x020-0x02b] Sequence info for Device Configuration command, similar as deviceModeSeq
    uint32_t reserved1;   //!< [0x02c-0x02f] Reserved for future use
    uint32_t configCmdArgs[3];     //!< [0x030-0x03b] Arguments/Parameters for device Configuration commands
    uint32_t reserved2;            //!< [0x03c-0x03f] Reserved for future use
    uint32_t controllerMiscOption; //!< [0x040-0x043] Controller Misc Options, see Misc feature bit definitions for more
    //! details
    uint8_t deviceType;    //!< [0x044-0x044] Device Type:  See Flash Type Definition for more details
    uint8_t sflashPadType; //!< [0x045-0x045] Serial Flash Pad Type: 1 - Single, 2 - Dual, 4 - Quad, 8 - Octal
    uint8_t serialClkFreq; //!< [0x046-0x046] Serial Flash Frequencey, device specific definitions, See System Boot
    //! Chapter for more details
    uint8_t lutCustomSeqEnable; //!< [0x047-0x047] LUT customization Enable, it is required if the program/erase cannot
    //! be done using 1 LUT sequence, currently, only applicable to HyperFLASH
    uint32_t reserved3[2]; //!< [0x048-0x04f] Reserved for future use
    uint32_t sflashA1Size; //!< [0x050-0x053] Size of Flash connected to A1
    uint32_t sflashA2Size; //!< [0x054-0x057] Size of Flash connected to A2 (unused/not applicable on RW610)
    uint32_t sflashB1Size; //!< [0x058-0x05b] Size of Flash connected to B1
    uint32_t sflashB2Size; //!< [0x05c-0x05f] Size of Flash connected to B2 (unused/not applicable on RW610)
    fsl_flexspi_pad_config_override_t csPadSettingOverride;   //!< [0x060-0x063] CS pad setting override value
    fsl_flexspi_pad_config_override_t sclkPadSettingOverride; //!< [0x064-0x067] SCK pad setting override value
    fsl_flexspi_pad_config_override_t dataPadSettingOverride; //!< [0x068-0x06b] data pad setting override value
    fsl_flexspi_pad_config_override_t dqsPadSettingOverride;  //!< [0x06c-0x06f] DQS pad setting override value
    uint32_t timeoutInMs;                                 //!< [0x070-0x073] Timeout threshold for read status command
    uint32_t commandInterval;                             //!< [0x074-0x077] CS deselect interval between two commands
    fsl_flexspi_dll_time_t dataValidTime[2]; //!< [0x078-0x07b] CLK edge to data valid time for PORT A and PORT B
    uint16_t busyOffset;                 //!< [0x07c-0x07d] Busy offset, valid value: 0-31
    uint16_t busyBitPolarity; //!< [0x07e-0x07f] Busy flag polarity, 0 - busy flag is 1 when flash device is busy, 1 -
    //! busy flag is 0 when flash device is busy
    uint32_t lookupTable[64];           //!< [0x080-0x17f] Lookup table holds Flash command sequences
    fsl_flexspi_lut_seq_t lutCustomSeq[12]; //!< [0x180-0x1af] Customizable LUT Sequences
    uint32_t dll0CrVal;                 //!> [0x1b0-0x1b3] Customizable DLL0CR setting
    uint32_t dll1CrVal;                 //!> [0x1b4-0x1b7] Customizable DLL1CR setting
    uint32_t reserved4[2];              //!< [0x1b8-0x1bf] Reserved for future use
} fsl_flexspi_mem_config_t;

typedef struct fsl_soc_memory_map_struct
{
    struct
    {
        uint32_t start;
        uint32_t end;
    } ramRegions[RAM_REGION_COUNT];
    struct
    {
        uint32_t start;
        uint32_t end;
    } flexspiNorRegions[FLEXSPINOR_REGION_COUNT];
} fsl_soc_mem_regions_t;

//!@brief Memory entry data structure
typedef struct fsl_memory_map_entry
{
    fsl_mem_attribute_t *memoryAttribute;
    const fsl_api_memory_region_interface_t *memoryInterface;
} fsl_api_memory_map_entry_t;

//!@brief API initialization data structure
typedef struct fsl_kb_api_parameter_struct
{
    uint32_t allocStart;
    uint32_t allocSize;
} fsl_kp_api_init_param_t;

//!@brief Memory context structure
typedef struct fsl_memory_context_struct
{
    status_t (*flush)(fsl_mem_attribute_t *attr);
    fsl_mem_attribute_t *attr;
} fsl_mem_context_t;

/*
 *  Serial NOR configuration block
 */
typedef struct _fsl_flexspi_nor_config
{
    fsl_flexspi_mem_config_t memConfig; //!< Common memory configuration info via FlexSPI
    uint32_t pageSize;              //!< Page size of Serial NOR
    uint32_t sectorSize;            //!< Sector size of Serial NOR
    uint8_t ipcmdSerialClkFreq;     //!< Clock frequency for IP command
    uint8_t isUniformBlockSize;     //!< Sector/Block size is the same
    uint8_t isDataOrderSwapped;     //!< Data order (D0, D1, D2, D3) is swapped (D1,D0, D3, D2)
    uint8_t reserved0[1];           //!< Reserved for future use
    uint8_t serialNorType;          //!< Serial NOR Flash type: 0/1/2/3
    uint8_t needExitNoCmdMode;      //!< Need to exit NoCmd mode before other IP command
    uint8_t halfClkForNonReadCmd;   //!< Half the Serial Clock for non-read command: true/false
    uint8_t needRestoreNoCmdMode;   //!< Need to Restore NoCmd mode after IP commmand execution
    uint32_t blockSize;             //!< Block size
    uint32_t flashStateCtx;         //!< Flash State Context
    uint32_t reserve2[10];          //!< Reserved for future use
} fsl_flexspi_nor_config_t;

//!@brief The API context structure
typedef struct fsl_api_core_context
{
    fsl_soc_mem_regions_t memRegions;
    fsl_arena_context_t arenaCtx;
    fsl_flexspi_nor_config_t flexspinorCfg;
    fsl_mem_context_t memCtx;
    fsl_ldr_Context_v3_t *sbloaderCtx;
    fsl_nboot_context_t *nbootCtx;
    uint8_t *sharedBuf;
    fsl_api_memory_map_entry_t memEntries[MEM_INTERFACE_COUNT];
} fsl_api_core_context_t;

//!@brief IAP API Interface structure
typedef struct fsl_iap_api_interface_struct
{
    fsl_standard_version_t version; //!< IAP API version number.
    status_t (*api_init)(fsl_api_core_context_t *coreCtx, const fsl_kp_api_init_param_t *param);
    status_t (*api_deinit)(fsl_api_core_context_t *coreCtx);
    status_t (*mem_init)(fsl_api_core_context_t *ctx);
    status_t (*mem_read)(fsl_api_core_context_t *ctx, uint32_t addr, uint32_t len, uint8_t *buf, uint32_t memoryId);
    status_t (*mem_write)(fsl_api_core_context_t *ctx, uint32_t addr, uint32_t len, const uint8_t *buf, uint32_t memoryId);
    status_t (*mem_fill)(fsl_api_core_context_t *ctx, uint32_t addr, uint32_t len, uint32_t pattern, uint32_t memoryId);
    status_t (*mem_flush)(fsl_api_core_context_t *ctx);
    status_t (*mem_erase)(fsl_api_core_context_t *ctx, uint32_t addr, uint32_t len, uint32_t memoryId);
    status_t (*mem_config)(fsl_api_core_context_t *ctx, uint32_t *buf, uint32_t memoryId);
    status_t (*mem_erase_all)(fsl_api_core_context_t *ctx, uint32_t memoryId);
    status_t (*fsl_sbloader_init)(fsl_api_core_context_t *ctx);
    status_t (*fsl_sbloader_pump)(fsl_api_core_context_t *ctx, uint8_t *data, uint32_t length);
    status_t (*fsl_sbloader_finalize)(fsl_api_core_context_t *ctx);
} fsl_iap_api_interface_t;

//! Function pointer definition for all loader action functions.
// typedef status_t (*fsl_pLdrFnc_v3_t)(ldr_Context_v3_t *);
typedef status_t (*fsl_pLdrFnc_v3_t)(fsl_api_core_context_t *n);

//! Loader context definition.
struct _fsl_ldr_Context_v3
{
    fsl_pLdrFnc_v3_t Action;        //!< pointer to loader action function
    uint32_t block_size;        //!< size of each block in bytes
    uint32_t block_data_size;   //!< data size in bytes (NBOOT_SB3_CHUNK_SIZE_IN_BYTES)
    uint32_t block_data_total;  //!< data max size in bytes (block_size * data_size
    uint32_t block_buffer_size; //!< block0 and block size
    uint32_t block_buffer_position;
    uint8_t block_buffer[MAX(NBOOT_SB3_MANIFEST_MAX_SIZE_IN_BYTES,
                             NBOOT_SB3_BLOCK_MAX_SIZE_IN_BYTES)]; //! will be used for both block0 and blockx
    uint32_t processedBlocks;

    uint8_t data_block_offset; //! data block offset in a block.
    bool in_data_block;        //!< in progress of handling a data block within a block
    uint8_t *data_block;
    uint32_t data_block_position;

    bool in_data_section; //!< in progress of handling a data section within a data block
    uint32_t data_section_handled;
    fsl_sb3_section_header_t data_section_header;

    bool in_data_range; //!< in progress of handling a data range within a data section
    uint32_t data_range_handled;
    uint32_t data_range_gap;
    fsl_sb3_data_range_header_t data_range_header;
    bool has_data_range_expansion;
    fsl_sb3_data_range_expansion_t data_range_expansion;

    uint32_t commandSet; //!< support command set during sb file handling

    uint32_t data_position;
    uint8_t data_buffer[SB3_DATA_BUFFER_SIZE_IN_BYTE]; //!< temporary data buffer

    uint32_t fuse_cmd_position;
    uint8_t fuse_cmd_buffer[32 * 4]; //!< used for fuse command
};

#if defined(__cplusplus)
extern "C" {
#endif // __cplusplus

#if defined(__cplusplus)
}
#endif // __cplusplus

#define BIT0 (1UL << 0U)

void power_on_device_impl(LOAD_Target_Type loadTarget);
void power_off_device_impl(LOAD_Target_Type loadTarget);
void reset_device(LOAD_Target_Type loadTarget);
status_t load_service(LOAD_Target_Type loadTarget, uint32_t sourceAddr);
status_t sb3_fw_download_impl(LOAD_Target_Type loadTarget, uint32_t flag, uint32_t sourceAddr);
status_t sb3_fw_reset_impl(LOAD_Target_Type loadTarget, uint32_t flag, uint32_t sourceAddr);
fsl_nboot_status_t nboot_hal_get_sb3_manifest_params(fsl_nboot_context_t *context, fsl_nboot_sb3_load_manifest_parms_t *parms);
status_t loader_process_sb_file(uint32_t readOffset);
status_t fsl_sbloader_init(fsl_api_core_context_t *ctx);
status_t fsl_sbloader_finalize(fsl_api_core_context_t *ctx);
uint8_t get_chip_revision(void);

//! @}

#endif /* __FSL_LOADER_UTILS_H__ */
