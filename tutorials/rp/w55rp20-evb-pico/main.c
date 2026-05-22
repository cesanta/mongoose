// SPDX-FileCopyrightText: 2026 Cesanta Software Limited
// SPDX-License-Identifier: GPL-2.0-only or commercial

#include "pico/stdlib.h"
#include "pico/unique_id.h"
#include "hardware/pio.h"

#include "mongoose.h"

#define SPIPIO pio0
#define SPISM 0
#define MOSIPIN 23
#define MISOPIN 22
#define CLKPIN 21
#define CSPIN 20
#define RSTPIN 25

#define USER_LED 19

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
    0x6020, //  7: out    x, 32           side 0
    0x5001, //  8: in     pins, 1         side 1
    0x0048, //  9: jmp    x--, 8          side 0
    0xc020, // 10: irq    wait 0          side 0
    0x0000, // 11: jmp    0               side 0
};

static const struct pio_program piospi_program = {
    .instructions = piospi_program_instructions,
    .length = 12,
    .origin = -1,
    .pio_version = 0,
#if PICO_PIO_VERSION > 0
    .used_gpio_ranges = 0x0
#endif
};

static pio_sm_config piospi_program_get_default_config(uint offset) {
    pio_sm_config c = pio_get_default_sm_config();
    sm_config_set_wrap(&c, offset + piospi_wrap_target, offset + piospi_wrap);
    sm_config_set_sideset(&c, 1, false, false);
    return c;
}

static pio_sm_config piospi_init(PIO pio, uint sm, uint addr, uint mosi, uint miso, uint clk) {
    pio_gpio_init(pio, mosi);
    pio_gpio_init(pio, miso);
    pio_gpio_init(pio, clk);
    pio_sm_set_pins_with_mask(pio, sm, 0, (1 << mosi) | (1 << clk));
    pio_sm_config c = piospi_program_get_default_config(addr);
    sm_config_set_out_pins(&c, mosi, 1);
    sm_config_set_in_pins(&c, miso);
    sm_config_set_set_pins(&c, mosi, 1);
    sm_config_set_sideset_pins(&c, clk);
    pio_sm_set_consecutive_pindirs(pio, sm, mosi, 1, true);
    pio_sm_set_consecutive_pindirs(pio, sm, clk, 1, true);
    pio_sm_set_pindirs_with_mask(pio, sm, (1 << mosi) | (1 << clk), (1 << mosi) | (1 << clk));
    sm_config_set_out_shift(&c, false, true, 8); // pull bytes, MSB first, auto
    sm_config_set_in_shift(&c, false, true, 8);  // push bytes, MSB first, auto
#if PICO_RP2040
    sm_config_set_clkdiv(&c, 10);                // Run at  <33.3MHz (2x data rate)
#else
    sm_config_set_clkdiv(&c, 3);                // Run at <33.3MHz (2x data rate)
#endif
    return c;
}

static void piospi_tx(PIO pio, uint sm, pio_sm_config *c, uint addr) {
    sm_config_set_fifo_join(c, PIO_FIFO_JOIN_TX);
    sm_config_set_out_shift(c, false, true, 8); // pull bytes, MSB first, auto
    pio_sm_init(pio, sm, addr, c);
}

static void piospi_rx(PIO pio, uint sm, pio_sm_config *c, uint addr) {
    sm_config_set_fifo_join(c, PIO_FIFO_JOIN_NONE);
    sm_config_set_out_shift(c, false, true, 32); // pull words, MSB first, auto
    pio_sm_init(pio, sm, addr + piospi_offset_rx_, c);
}

static void piospi_done(PIO pio, uint sm) {
    while(!pio_interrupt_get(pio, 0));
    pio_sm_set_enabled(pio, sm, false);
    pio_interrupt_clear(pio, 0);
}

// Construct MAC address from the unique board ID
void genmac(unsigned char *mac) {
  pico_unique_board_id_t board_id;
  pico_get_unique_board_id(&board_id);
  mac[0] = 2;
  memcpy(&mac[1], &board_id.id[3], 5);
}

static pio_sm_config s_piospi_sm_config;
static uint s_piospi_sm_addr;

void ethernet_init(void) {
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

// Hardware specific SPI support
static void piospi_write(uint8_t *data, size_t len) {
  size_t initial_len = len;
  piospi_tx(SPIPIO, SPISM, &s_piospi_sm_config, s_piospi_sm_addr);
  __compiler_memory_barrier();
  pio_sm_put(SPIPIO, SPISM, (len * 8) - 1); // bits to transmit
  pio_sm_set_enabled(SPIPIO, SPISM, true);
  while (len--) pio_sm_put_blocking(SPIPIO, SPISM, (*data++) << 24);
  piospi_done(SPIPIO, SPISM);
}

// Read data block from SPI interface
static void piospi_read(uint8_t *data, size_t len) {
  piospi_rx(SPIPIO, SPISM, &s_piospi_sm_config, s_piospi_sm_addr);
  __compiler_memory_barrier();
  pio_sm_put(SPIPIO, SPISM, (len * 8) - 1); // bits to receive
  pio_sm_set_enabled(SPIPIO, SPISM, true);
  while (len--) *data++ = pio_sm_get_blocking(SPIPIO, SPISM);
  piospi_done(SPIPIO, SPISM);
}

void spi_begin(void *spi) {
  (void) spi;
  gpio_put(CSPIN, 0);
}

void spi_end(void *spi) {
  (void) spi;
  gpio_put(CSPIN, 1);
}

// either write or read, not both
void spi_txn(void *arg, uint8_t *txdata, uint8_t *rxdata, size_t len) {
  if (txdata != NULL) piospi_write(txdata, len);
  if (rxdata != NULL) piospi_read(rxdata, len);
  (void) arg;
}


static void blink_task(void) {
  static uint64_t timer;
  if (mg_timer_expired(&timer, 500, mg_now())) {
    gpio_put(USER_LED, !gpio_get(USER_LED));
  }
}

static void http_ev_handler(struct mg_connection *c, int ev, void *ev_data) {
  if (ev == MG_EV_HTTP_MSG) {  // New HTTP request received
    struct mg_http_message *hm = (struct mg_http_message *) ev_data;
    uint64_t tick = to_ms_since_boot(get_absolute_time());
    if (mg_match(hm->uri, mg_str("/api/tick"), NULL)) {
      mg_http_reply(c, 200, "", "{%m:%llu}\n", MG_ESC("tick"), tick);
    } else {
      mg_http_reply(c, 200, "", "Hi from Mongoose, tick %llu\n", tick);
    }
  }
}

// In an RTOS environment, give this task 8 KB of stack space.
static void run_mongoose(void) {
  struct mg_mgr mgr;

  mg_mgr_init(&mgr);
  mg_http_listen(&mgr, "http://0.0.0.0", http_ev_handler, NULL);

  for (;;) {
    mg_mgr_poll(&mgr, 1);
    blink_task();
  }
}

int main(void) {
  stdio_init_all();
  gpio_init(USER_LED);                // Initialise GPIO
  gpio_set_dir(USER_LED, GPIO_OUT);   // Set as output
  gpio_put(USER_LED, 1);
  ethernet_init();
  MG_INFO(("Hardware initialised, starting firmware..."));

  run_mongoose();
  return 0;
}
