#include "mongoose.h"
#include "main.h"

#define WIFISPI SPI1

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

#define BIT(x) (1UL << (x))
#define CLRSET(reg, clear, set) ((reg) = ((reg) & ~(clear)) | (set))

static inline void spin(volatile uint32_t n) {
  while (n--)
    (void)0;
}

static inline void hal_spi_init(SPI_TypeDef *spi, uint16_t div) {
  (void)spi;
  uint32_t br = 0; // Baud rate prescaler: div = 2,4,8...256 -> MBR = 0..7
  while (div > 2)
    div >>= 1, br++;
  if (br > 7)
    br = 7;


  CLRSET(RCC->CCIPR1, 3 << 20, 1); // SYSCLK source
  RCC->APB2ENR |= BIT(12); // SPI1EN
  spin(100);
  spi->CR1 = 0;                  // disable SPI
  spi->CR1 = SPI_CR1_SSI_Msk; // SSI
  spi->CFG1 = (7 << SPI_CFG1_DSIZE_Pos) | (br << SPI_CFG1_MBR_Pos); // DSIZE = 8-bit, MBR
  spi->CFG2 = SPI_CFG2_SSM_Msk | SPI_CFG2_MASTER_Msk;
  spi->CR1 |= SPI_CR1_SPE_Msk; // Enable SPI
  spi->CR1 |= SPI_CR1_CSTART_Msk; // Start transfer
}

static inline uint8_t hal_spi_xfer_byte(SPI_TypeDef *spi, uint8_t byte) {
  (void)spi;
  while ((spi->SR & SPI_SR_TXP_Msk) == 0)
    spin(1); // TXP
  *(volatile uint8_t *) &spi->TXDR = byte;
  while (((spi->SR & SPI_SR_RXP_Msk) == 0) && ((spi->SR & SPI_SR_RXWNE_Msk) == 0) && ((spi->SR & SPI_SR_RXPLVL_Msk) == 0))
    spin(1);        // RXP
  uint8_t rx = *(volatile uint8_t *) &spi->RXDR;
  return rx;
}

static inline void hal_spi_tx_rx(SPI_TypeDef *spi, uint8_t *tx, uint8_t *rx, size_t len) {
  for (size_t i = 0; i < len; i++) {
    uint8_t rx_ = hal_spi_xfer_byte(spi, tx ? tx[i] : 0);
    if (rx) rx[i] = rx_;
  }
}

static inline bool hal_spi_ready(SPI_TypeDef *spi) {
  (void) spi;
  return HAL_GPIO_ReadPin(SPI_RDY_GPIO_Port, SPI_RDY_Pin);
}

static inline void hal_spi_select(SPI_TypeDef *spi, bool s) {
  (void)spi;
  HAL_GPIO_WritePin(SPI_CS_GPIO_Port, SPI_CS_Pin, s);
}

static inline void hal_spi_done(SPI_TypeDef *spi) {
  while((spi->SR & SPI_SR_TXC_Msk) == 0) spin(1);
  spi->IFCR = 0xffff; // clear all flags
}

static bool hwspecific_is_ready(void) {
  return hal_spi_ready(WIFISPI);
}

static void hwspecific_spi_begin(void *arg) {
  hal_spi_select(WIFISPI, true);
  (void) arg;
}

static void hwspecific_spi_txn(void *arg, uint8_t *txdata, uint8_t *rxdata, size_t len) {
  while (len--) {
    uint8_t rx = hal_spi_xfer_byte(WIFISPI, txdata != NULL ? *txdata++ : 0);
    if (rxdata != NULL) *rxdata++ = rx;
  }
  hal_spi_done(WIFISPI);
  (void) arg;
}

static void hwspecific_spi_end(void *arg) {
  hal_spi_select(WIFISPI, false);
  (void) arg;
}

void hwspecific_spi_init(void) {
  HAL_GPIO_WritePin(SPI_CHIP_EN_GPIO_Port, SPI_CHIP_EN_Pin, 0);
  hal_spi_init(WIFISPI, 128); // runs at 1.25MHz (SYS_CLK/128)
  mg_delayms(10);
  HAL_GPIO_WritePin(SPI_CHIP_EN_GPIO_Port, SPI_CHIP_EN_Pin, 1);
}

static const struct mg_tcpip_spi spi = {NULL, hwspecific_spi_begin, hwspecific_spi_end, hwspecific_spi_txn};

void wifi_setconfig(void *data) {
  struct mg_tcpip_driver_st67w6_data *d = (struct mg_tcpip_driver_st67w6_data *) data;
  d->wifi.ssid = WIFI_NAME;
  d->wifi.pass = WIFI_PASS;
  d->wifi.apssid = WIFI_AP_NAME;
  d->wifi.appass = WIFI_AP_PASS;
  d->wifi.apip = MG_IPV4(192, 168, 169, 1);
  d->wifi.apmask = MG_IPV4(255, 255, 255, 0);
  d->wifi.security = 0;
  d->wifi.apsecurity = 0;
  d->wifi.apchannel = 10;
  d->wifi.apmode = ENABLE_WIFI_AP;
  d->spi = (struct mg_tcpip_spi *) &spi;
  d->is_ready = hwspecific_is_ready;
}
