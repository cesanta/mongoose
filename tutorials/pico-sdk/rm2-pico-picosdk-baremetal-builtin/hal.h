// Copyright (c) 2025 Cesanta Software Limited
// All rights reserved

#include "pico/stdlib.h"
#include "hardware/pio.h"

extern void mg_delayms(unsigned int ms);

#define SPIPIO pio0
#define SPISM 0
#define DATAPIN 2
#define CLKPIN 3
#define CSPIN 1
#define PWRPIN 0

#define piospi_wrap_target 0
#define piospi_wrap 6
#define piospi_pio_version 0

#define piospi_offset_rx_ 7u

static const uint16_t piospi_program_instructions[] = {
            //     .wrap_target
    0x80a0, //  0: pull   block           side 0     
    0xa027, //  1: mov    x, osr          side 0     
    0x6068, //  2: out    null, 8         side 0     
    0x6001, //  3: out    pins, 1         side 0     
    0x1043, //  4: jmp    x--, 3          side 1     
    0xe000, //  5: set    pins, 0         side 0     
    0xc020, //  6: irq    wait 0          side 0     
            //     .wrap
    0xe080, //  7: set    pindirs, 0      side 0     
    0x6020, //  8: out    x, 32           side 0     
    0x4001, //  9: in     pins, 1         side 0     
    0x1049, // 10: jmp    x--, 9          side 1     
    0xe000, // 11: set    pins, 0         side 0     
    0xe081, // 12: set    pindirs, 1      side 0     
    0xc020, // 13: irq    wait 0          side 0     
    0x0000, // 14: jmp    0               side 0     
};

static const struct pio_program piospi_program = {
    .instructions = piospi_program_instructions,
    .length = 15,
    .origin = -1,
    .pio_version = 0,
#if PICO_PIO_VERSION > 0
    .used_gpio_ranges = 0x0
#endif
};

static inline pio_sm_config piospi_program_get_default_config(uint offset) {
    pio_sm_config c = pio_get_default_sm_config();
    sm_config_set_wrap(&c, offset + piospi_wrap_target, offset + piospi_wrap);
    sm_config_set_sideset(&c, 1, false, false);
    return c;
}

static inline pio_sm_config piospi_init(PIO pio, uint sm, uint addr, uint data, uint clk) {
    pio_gpio_init(pio, data);
    pio_gpio_init(pio, clk);
    pio_sm_set_pins_with_mask(pio, sm, 0, (1 << data) | (1 << clk));
    pio_sm_config c = piospi_program_get_default_config(addr);
    sm_config_set_out_pins(&c, data, 1);
    sm_config_set_in_pins(&c, data);
    sm_config_set_set_pins(&c, data, 1);
    sm_config_set_sideset_pins(&c, clk);
    pio_sm_set_consecutive_pindirs(pio, sm, data, 1, true);
    pio_sm_set_consecutive_pindirs(pio, sm, clk, 1, true);
    pio_sm_set_pindirs_with_mask(pio, sm, (1 << data) | (1 << clk), (1 << data) | (1 << clk));
    sm_config_set_in_shift(&c, false, true, 8);  // push bytes, MSB first, auto
#if PICO_RP2040
    sm_config_set_clkdiv(&c, 18);                // Run at 133/1 = <50MHz (2x data rate)
#else
    sm_config_set_clkdiv(&c, 20);                // Run at 150/1 = <50MHz (2x data rate)
#endif
    return c;
}
static inline void piospi_tx(PIO pio, uint sm, pio_sm_config *c, uint addr, uint data) {
    sm_config_set_fifo_join(c, PIO_FIFO_JOIN_TX);
    sm_config_set_out_shift(c, false, true, 8); // pull bytes, MSB first, auto
    pio_sm_init(pio, sm, addr, c);
}
static inline void piospi_rx(PIO pio, uint sm, pio_sm_config *c, uint addr, uint data) {
    sm_config_set_fifo_join(c, PIO_FIFO_JOIN_NONE);
    sm_config_set_out_shift(c, false, true, 32); // pull words, MSB first, auto
    pio_sm_init(pio, sm, addr + piospi_offset_rx_, c);
}
static inline void piospi_done(PIO pio, uint sm) {
    while(!pio_interrupt_get(pio, 0));
    pio_sm_set_enabled(pio, sm, false);
    pio_interrupt_clear(pio, 0);
}

static pio_sm_config s_piospi_sm_config;
static uint s_piospi_sm_addr;

void piospi_write(uint8_t *data, size_t len) {
  size_t initial_len = len;
  piospi_tx(SPIPIO, SPISM, &s_piospi_sm_config, s_piospi_sm_addr, DATAPIN);
  __compiler_memory_barrier();
  pio_sm_put(SPIPIO, SPISM, (len * 8) - 1); // bits to transmit
  pio_sm_set_enabled(SPIPIO, SPISM, true);
  while (len--) pio_sm_put_blocking(SPIPIO, SPISM, (*data++) << 24);
  piospi_done(SPIPIO, SPISM);
}

// Read data block from SPI interface
void piospi_read(uint8_t *data, size_t len) {
  piospi_rx(SPIPIO, SPISM, &s_piospi_sm_config, s_piospi_sm_addr, DATAPIN);
  __compiler_memory_barrier();
  pio_sm_put(SPIPIO, SPISM, (len * 8) - 1); // bits to receive
  pio_sm_set_enabled(SPIPIO, SPISM, true);
  while (len--) *data++ = pio_sm_get_blocking(SPIPIO, SPISM);
  piospi_done(SPIPIO, SPISM);
}

void hwspecific_spi_init(void) {
  gpio_init(PWRPIN);
  gpio_set_dir(PWRPIN, GPIO_OUT);
  gpio_put(PWRPIN, 0);
  gpio_init(CSPIN);
  gpio_set_dir(CSPIN, GPIO_OUT);
  gpio_put(CSPIN, 0);
  // init SPI pins so they are idle and stable during power up
  gpio_init(DATAPIN);
  gpio_set_dir(DATAPIN, GPIO_OUT);
  gpio_put(DATAPIN, 0);
  gpio_init(CLKPIN);
  gpio_set_dir(CLKPIN, GPIO_OUT);
  gpio_put(CLKPIN, 0);
  mg_delayms(100);
  gpio_put(CSPIN, 1);
  gpio_put(PWRPIN, 1);
  s_piospi_sm_addr = pio_add_program(SPIPIO, &piospi_program);
  s_piospi_sm_config = piospi_init(SPIPIO, SPISM, s_piospi_sm_addr, DATAPIN, CLKPIN);
  mg_delayms(50);
}

void hwspecific_spi_begin(void *arg) {
  gpio_put(CSPIN, 0);
  (void) arg;
}

// either write or read, not both
void hwspecific_spi_txn(void *arg, uint8_t *txdata, uint8_t *rxdata, size_t len) {
  if (txdata != NULL) piospi_write(txdata, len);
  if (rxdata != NULL) piospi_read(rxdata, len);
  (void) arg;
}

void hwspecific_spi_end(void *arg) {
  gpio_put(CSPIN, 1);
  (void) arg;
}
