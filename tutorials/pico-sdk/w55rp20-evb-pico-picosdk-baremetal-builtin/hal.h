// Copyright (c) 2026 Cesanta Software Limited
// All rights reserved

#include "pico/stdlib.h"
#include "hardware/pio.h"

extern void mg_delayms(unsigned int ms);

#define SPIPIO pio0
#define SPISM 0
#define MOSIPIN 23
#define MISOPIN 22
#define CLKPIN 21
#define CSPIN 20
#define RSTPIN 25

// Try it on a bare Pico !
// #define MOSIPIN 3
// #define MISOPIN 4
// #define CLKPIN 2
// #define CSPIN 5

#define piospi_wrap_target 0
#define piospi_wrap 3
#define piospi_pio_version 0

static const uint16_t piospi_program_instructions[] = {
            //     .wrap_target
    0xe027, //  0: set    x, 7            side 0     
    0x6101, //  1: out    pins, 1         side 0 [1] 
    0x5001, //  2: in     pins, 1         side 1     
    0x1041, //  3: jmp    x--, 1          side 1     
            //     .wrap
};

static const struct pio_program piospi_program = {
    .instructions = piospi_program_instructions,
    .length = 4,
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

static inline pio_sm_config piospi_init(PIO pio, uint sm, uint addr, uint mosi, uint miso, uint clk) {
    pio_gpio_init(pio, mosi);
    pio_gpio_init(pio, miso);
    pio_gpio_init(pio, clk);
    pio_sm_set_pins_with_mask(pio, sm, 0, (1 << mosi) | (1 << clk));
    pio_sm_config c = piospi_program_get_default_config(addr);
    sm_config_set_out_pins(&c, mosi, 1);
    sm_config_set_in_pins(&c, miso);
    sm_config_set_set_pins(&c, mosi, 1);
    sm_config_set_sideset_pins(&c, clk);
    pio_sm_set_pindirs_with_mask(pio, sm, (1 << mosi) | (1 << clk), (1 << mosi) | (1 << clk));
    sm_config_set_out_shift(&c, false, true, 8); // pull bytes, MSB first, auto
    sm_config_set_in_shift(&c, false, true, 8);  // push bytes, MSB first, auto
#if PICO_RP2040
    sm_config_set_clkdiv(&c, 18);                // Run at 133/1 = <33MHz (4x data rate)
#else
    sm_config_set_clkdiv(&c, 20);                // Run at 150/2 = <19MHz (4x data rate)
#endif
    return c;
}


static pio_sm_config s_piospi_sm_config;
static uint s_piospi_sm_addr;

uint8_t hwspecific_spi_txn(void *arg, uint8_t data) {
  (void) arg;
  pio_sm_put(SPIPIO, SPISM, data << 24);
  return pio_sm_get_blocking(SPIPIO, SPISM);
}

void hwspecific_spi_init(void) {
  gpio_init(RSTPIN);
  gpio_set_dir(RSTPIN, GPIO_OUT);
  gpio_put(RSTPIN, 0);
  gpio_init(CSPIN);
  gpio_set_dir(CSPIN, GPIO_OUT);
  gpio_put(CSPIN, 1);
  s_piospi_sm_addr = pio_add_program(SPIPIO, &piospi_program);
  s_piospi_sm_config = piospi_init(SPIPIO, SPISM, s_piospi_sm_addr, MOSIPIN, MISOPIN, CLKPIN);
  mg_delayms(2);
  gpio_put(RSTPIN, 1);
}

void hwspecific_spi_begin(void *arg) {
  pio_sm_init(SPIPIO, SPISM, s_piospi_sm_addr, &s_piospi_sm_config);
  pio_sm_set_enabled(SPIPIO, SPISM, true);
  gpio_put(CSPIN, 0);
  (void) arg;
}

void hwspecific_spi_end(void *arg) {
  gpio_put(CSPIN, 1);
  pio_sm_set_enabled(SPIPIO, SPISM, false);
  (void) arg;
}


// Construct MAC address from the unique board ID
#include "pico/unique_id.h"
static inline void hal_ethernet_genmac(unsigned char *mac) {
  pico_unique_board_id_t board_id;
  pico_get_unique_board_id(&board_id);
  mac[0] = 2;
  memcpy(&mac[1], &board_id.id[3], 5);
}
