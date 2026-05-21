#include "mongoose.h"
#include "main.h"

typedef enum {RESOURCE_IN_MEMORY,RESOURCE_IN_EXTERNAL_STORAGE} resource_location_t;
typedef struct {resource_location_t l;unsigned long sz;union{struct {unsigned long o;const char *f;}fs;struct {const char *m;}mem;}val;} resource_hnd_t;

#include "resources/firmware/COMPONENT_4343W_FS/4343WA1_clm_blob.c"
#include "resources/firmware/COMPONENT_4343W_FS/4343WA1_bin.c"
#include "resources/nvram/wifi_nvram_image.h"

#ifndef WIFI_NAME
#define WIFI_NAME ""
#endif

#ifndef WIFI_PASS
#define WIFI_PASS ""
#endif

#ifndef WIFI_AP_NAME
#define WIFI_AP_NAME "Mongoose"
#endif

#ifndef WIFI_AP_PASS
#define WIFI_AP_PASS "mongoose"
#endif

#ifndef ENABLE_WIFI_AP
#define ENABLE_WIFI_AP 0
#endif

// - 58.4 Table 473
// - 58.5.8 AHB bw >= 3x SDMMC bus bw; Table 492; SD HS => AHB >= ~20MHz
// - SDIO 2.0 section 3.1
static inline void sdmmc_set_ds(void) { // NO FLOW CONTROL ??? REALLY ??? *******************************
  MG_SET_BITS(SDMMC1->CLKCR, (1 << 17) | 0x3ff, (1 << 14) | ((200000000 / (2 * 25000000)) & 0x3ff));  // 4-bit bus, 25MHz
}
static inline void sdmmc_set_hs(void) { // mbed enables flow control... AIROC does not
  MG_SET_BITS(SDMMC1->CLKCR, 0x3ff, (1 << 17) | (1 << 14) | ((200000000 / (2 * 50000000)) & 0x3ff));  // hw flow control, 4-bit bus, 50MHz
}
static inline void sdmmc_init(void) {
  RCC->AHB3RSTR |= MG_BIT(16);  // Reset SDIO block
  RCC->AHB3RSTR &= ~MG_BIT(16);
  RCC->D1CCIPR &= ~MG_BIT(16); // 9.7.18, use PLL1Q
  RCC->AHB3ENR |= MG_BIT(16);  // Enable SDIO clock
  // SDIO pins initialized by Cube; Portenta H7 doesn't pull-up the SDIO bus
  // 1-bit bus, rising edge, clock always on, no flow control, 400KHz
  SDMMC1->CLKCR = (200000000 / (2 * 400000)) & 0x3ff;
  SDMMC1->POWER |= 3 << 0; // SDMMC_POWER_PWRCTRL
  SDMMC1->DCTRL = MG_BIT(10); // Read Wait control by stopping SDMMCCLK
  // wait 74 clocks (~200us) before accessing the "card"
}

static inline bool sdmmc_transact(uint8_t cmd, uint32_t arg, uint32_t *resp) {
  unsigned int wresp = (resp != NULL) ? 1 : 0; // 58.10.4: 2-bit; cmds used
  uint32_t cmdr = MG_BIT(12) | (wresp << 8) | ((cmd & 0x3f) << 0); // CPSMEN
  SDMMC1->ARG = arg;
  MG_SET_BITS(SDMMC1->CMD, 0x00011f3f /* CMDSUSPEND CPSMEN WAITPEND WAITINT WAITRESP CMD */, cmdr);
  if (wresp == 0) {
    while ((SDMMC1->STA & SDMMC_STA_CMDSENT) == 0) (void)0;
    SDMMC1->ICR = 0x002000C5; // BUSYD0END CMDSENT CMDREND CTIMEOUT CCRCFAIL
    return true;
  }
  //while ((SDMMC1->STA & 0x00200045 /*BUSYD0END CMDREND CTIMEOUT CCRCFAIL*/) == 0 || SDMMC1->STA & SDMMC_STA_CPSMACT) spin(1);
  while (SDMMC1->STA & SDMMC_STA_CPSMACT) (void)0;
  bool res = (SDMMC1->STA & 0x00200005 /*BUSYD0END CTIMEOUT CCRCFAIL*/) == 0;
  SDMMC1->ICR = 0x002000C5; // BUSYD0END CMDSENT CMDREND CTIMEOUT CCRCFAIL
  *resp = SDMMC1->RESP1;
  if (!res || SDMMC1->RESPCMD != cmd) return false;
  return true;
}

static inline unsigned int _log2(uint16_t x) {
  unsigned int i = 0;
  while (x != 0) x = ((uint16_t)x) >> 1, ++i;
  return i - 1;
}

static inline bool sdmmc_transfer(bool write, uint32_t arg, uint16_t blksz, uint32_t *data, uint32_t length, uint32_t *resp) {
  uint32_t cmdr = MG_BIT(12) | (1 << 8) | (53 << 0); // CPSMEN WRESP CMD=53
  bool bmode = (blksz != 0);
  uint8_t dblksz = bmode ? (uint8_t)_log2(blksz) : 0;
  uint32_t dctrlr = (dblksz << 4) | (bmode ? (0 << 2) : (1 << 2)) | (write ? 0 : MG_BIT(1)); // DBLOCKSIZE=f(blksz) DTMODE=f(bmode) DTDIR=!write !DTEN
  SDMMC1->DCTRL = SDMMC_DCTRL_RWMOD;
  SDMMC1->IDMACTRL = SDMMC_IDMA_IDMAEN; // single buffer
  SDMMC1->IDMABASE0 = (uint32_t)data;
  SDMMC1->DTIMER = (uint32_t) 50000000; // 58.10.7, set for 1 sec @50MHz (2 secs @25MHz)
  SDMMC1->DLEN = length;  // 58.10.8 call with a multiple of blksz (we do)
  MG_SET_BITS(SDMMC1->DCTRL, 0xff /* DBLOCKSIZE DTMODE DTDIR DTEN */, dctrlr);
  SDMMC1->CMD = SDMMC_CMD_CMDTRANS;  // CPSM will perform a data transfer
  SDMMC1->ARG = arg;
  SDMMC1->CMD |= cmdr; // other flags cleared above
  while ((SDMMC1->STA & 0x0020003f) == 0 && SDMMC1->STA & SDMMC_STA_DPSMACT)
    (void) 0; // incorrect "card" usage will stall here
  bool res = (SDMMC1->STA & 0x0020003f /* BUSYD0END RXOVERR TXUNDERR DTIMEOUT CTIMEOUT DCRCFAIL CCRCFAIL */) == 0;
  SDMMC1->CMD = 0;
  SDMMC1->DLEN = 0;
  SDMMC1->IDMACTRL = 0;
  SDMMC1->ICR = (uint32_t) ~0; // clear all flags
  *resp = SDMMC1->RESP1;
  SDMMC1->DCTRL = SDMMC_DCTRL_RWMOD;
  if (!res || SDMMC1->RESPCMD != 53) return false;
  return true;
}


void hwspecific_sdio_init(void) {
  HAL_GPIO_WritePin(PWR_GPIO_Port, PWR_Pin, 0);
  mg_delayms(100);
  sdmmc_init();
  mg_delayms(1);
  HAL_GPIO_WritePin(PWR_GPIO_Port, PWR_Pin, 1);
  mg_delayms(50);
}

void hwspecific_sdio_config(void *sdio, uint8_t cfg) {
  (void) sdio;
  if (cfg == 0) return sdmmc_set_ds();
  if (cfg == 1) return sdmmc_set_hs();
}
bool hwspecific_sdio_txn(void *sdio, uint8_t cmd, uint32_t arg, uint32_t *resp) {
  (void) sdio;
  return sdmmc_transact(cmd, arg, resp);
}
bool hwspecific_sdio_xfr(void *sdio, bool write, uint32_t arg, uint16_t blksz, uint32_t *data, uint32_t len, uint32_t *resp){
  (void) sdio;
  return sdmmc_transfer(write, arg, blksz, data, len, resp);
}


static const struct mg_tcpip_sdio sdio = {NULL, hwspecific_sdio_config, hwspecific_sdio_txn, hwspecific_sdio_xfr};

static const struct mg_tcpip_driver_cyw_firmware fw = {
  (const uint8_t *)wifi_firmware_image_data, (size_t)sizeof(wifi_firmware_image_data),
  (const uint8_t *)wifi_nvram_image, (size_t)sizeof(wifi_nvram_image),
  (const uint8_t *)wifi_firmware_clm_blob_image_data, (size_t)sizeof(wifi_firmware_clm_blob_image_data)};


void wifi_setconfig(void *data) {
  struct mg_tcpip_driver_cyw_data *d = (struct mg_tcpip_driver_cyw_data *) data;
  d->bus = (struct mg_tcpip_sdio *) &sdio;
  d->fw = (struct mg_tcpip_driver_cyw_firmware *)&fw;
  d->wifi.ssid = WIFI_NAME;
  d->wifi.pass = WIFI_PASS;
  d->wifi.apssid = WIFI_AP_NAME;
  d->wifi.appass = WIFI_AP_PASS;
  d->wifi.apip = MG_IPV4(192, 168, 111, 1);
  d->wifi.apmask = MG_IPV4(255, 255, 255, 0);
  d->wifi.security = 0;
  d->wifi.apsecurity = 0;
  d->wifi.apchannel = 10;
  d->wifi.apmode = ENABLE_WIFI_AP;
  d->hs = false;
}
