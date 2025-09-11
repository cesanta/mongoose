/*
 * Copyright 2017-2020 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __MFLASH_FILE__
#define __MFLASH_FILE__

#include "fsl_common.h"

#include "mflash_drv.h"

#define MFLASH_MAX_PATH_LEN 56

/*
 * Template for file record defines file path and size to be pre-allocated for that file.
 * The actual size of the file shall not exceed the size defined in the template.
 */
typedef struct
{
    char *path;
    uint32_t max_size;
} mflash_file_t;

/*! @brief Initialization status of mflash subsystem */
bool mflash_is_initialized(void);

/*! @brief Initializes mflash filesystem and driver. Creates new filesystem unless already in place. */
status_t mflash_init(const mflash_file_t *dir_template, bool init_drv);

/*! @brief Saves data to file with given path. */
status_t mflash_file_save(char *path, uint8_t *data, uint32_t size);

/*! @brief Returns pointer for direct memory mapped access to file data. */
status_t mflash_file_mmap(char *path, uint8_t **pdata, uint32_t *psize);

#endif
