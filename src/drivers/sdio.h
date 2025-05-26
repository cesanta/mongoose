#pragma once

#if MG_ENABLE_TCPIP && \
    (defined(MG_ENABLE_DRIVER_CYW_SDIO) && MG_ENABLE_DRIVER_CYW_SDIO)

// Specific chip/card driver --> SDIO driver --> HAL --> SDIO hw controller

// API with HAL for hardware controller
// - Provide a function to init the controller (external)
// - Provide these functions:
struct mg_tcpip_sdio {
  void *sdio;                    // Opaque SDIO bus descriptor
  void (*cfg)(void *, uint8_t);  // select operating parameters
  // SDIO transaction: send cmd with a possible 1-byte read or write
  bool (*txn)(void *, uint8_t cmd, uint32_t arg, uint32_t *r);
  // SDIO extended transaction: write or read len bytes, using blksz blocks
  bool (*xfr)(void *, bool write, uint32_t arg, uint16_t blksz, uint32_t *,
              uint32_t len, uint32_t *r);
};

// API with driver (e.g.: cyw.c)
// Once the hardware controller has been initialized:
// - Init card: selects the card, sets F0 block size, sets bus width and speed
bool mg_sdio_init(struct mg_tcpip_sdio *sdio);
// - Enable other possible functions (F1 to F7)
bool mg_sdio_enable_f(struct mg_tcpip_sdio *sdio, unsigned int f);
// - Wait for them to be ready
bool mg_sdio_waitready_f(struct mg_tcpip_sdio *sdio, unsigned int f);
// - Set their transfer block length
bool mg_sdio_set_blksz(struct mg_tcpip_sdio *sdio, unsigned int f,
                       uint16_t blksz);
// - Transfer data to/from a function (abstracts from transaction type)
// - Requesting a read transfer > blocksize means block transfer will be used.
// - Drivers must have room to accomodate a whole block transfer, see sdio.c
// - Transfers of > 1 byte --> (uint32_t *) data. 1-byte --> (uint8_t *) data
bool mg_sdio_transfer(struct mg_tcpip_sdio *sdio, bool write, unsigned int f,
                      uint32_t addr, void *data, uint32_t len);

#endif
