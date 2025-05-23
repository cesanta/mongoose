#pragma once

#if MG_ENABLE_TCPIP && \
    (defined(MG_ENABLE_DRIVER_CYW_SDIO) && MG_ENABLE_DRIVER_CYW_SDIO)

struct mg_tcpip_sdio {
  void *sdio;                    // Opaque SDIO bus descriptor
  void (*cfg)(void *, uint8_t);  // select operating parameters
  // SDIO transaction: send cmd with a possible 1-byte read or write
  bool (*txn)(void *, uint8_t cmd, uint32_t arg, uint32_t *r);
  // SDIO extended transaction: write or read len bytes, using blksz blocks
  bool (*xfr)(void *, bool write, uint32_t arg, uint16_t blksz, uint32_t *,
              uint32_t len, uint32_t *r);
};

bool mg_sdio_transfer(struct mg_tcpip_sdio *sdio, bool write, unsigned int f,
                      uint32_t addr, void *data, uint32_t len);
bool mg_sdio_set_blksz(struct mg_tcpip_sdio *sdio, unsigned int f,
                       uint16_t blksz);
bool mg_sdio_enable_f(struct mg_tcpip_sdio *sdio, unsigned int f);
bool mg_sdio_waitready_f(struct mg_tcpip_sdio *sdio, unsigned int f);
bool mg_sdio_init(struct mg_tcpip_sdio *sdio);

#endif
