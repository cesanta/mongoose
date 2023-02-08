// Copyright (c) 2023 Cesanta Software Limited
// All rights reserved

#include "mcu.h"
#include "mongoose.h"

#define LED1 PIN('B', 0)      // On-board LED pin (green)
#define LED2 PIN('E', 1)      // On-board LED pin (yellow)
#define LED3 PIN('B', 14)     // On-board LED pin (red)
#define BLINK_PERIOD_MS 1000  // LED blinking period in millis

static uint64_t s_ticks;
void SysTick_Handler(void) { s_ticks++; }
uint64_t mg_millis(void) { return s_ticks; }

static void timer_cb(void *arg) {
  gpio_toggle(LED2);                      // Blink LED
  MG_INFO(("ticks: %lld", mg_millis()));  // Log something
  (void) arg;
}

int main(void) {
  clock_init();
  systick_init(CPU_FREQUENCY / 1000);  // Increment s_ticks every ms
  gpio_output(LED2);                   // Setup LED
  uart_init(UART_DEBUG, 115200);       // Initialise debug printf

  char rev = chiprev();
  MG_INFO(("Chip revision: %c, max cpu clock: %u MHz", rev, (rev == 'V') ? 480 : 400));


  // Initialise Ethernet. Enable MAC GPIO pins, see UM2407 for MB1364 boards
  // section 6.6.7:
  // https://www.st.com/resource/en/user_manual/um2407-stm32h7-nucleo144-boards-mb1364-stmicroelectronics.pdf
  uint16_t pins[] = {PIN('A', 1),  PIN('A', 2),  PIN('A', 7),
                     PIN('B', 13), PIN('C', 1),  PIN('C', 4),
                     PIN('C', 5),  PIN('G', 11), PIN('G', 13)};
  for (size_t i = 0; i < sizeof(pins) / sizeof(pins[0]); i++) {
    gpio_init(pins[i], GPIO_MODE_AF, GPIO_OTYPE_PUSH_PULL, GPIO_SPEED_INSANE,
              GPIO_PULL_NONE, 11);
  }
  nvic_enable_irq(61);                          // Setup Ethernet IRQ handler
  RCC->APB4ENR |= BIT(1);                       // Enable SYSCFG
  SETBITS(SYSCFG->PMCR, 7 << 21, 4 << 21);      // Use RMII (12.3.1)
  RCC->AHB1ENR |= BIT(15) | BIT(16) | BIT(17);  // Enable Ethernet clocks
  RCC->AHB1RSTR |= BIT(15);                     // ETHMAC force reset
  RCC->AHB1RSTR &= ~BIT(15);                    // ETHMAC release reset

  MG_INFO(("Initialising Mongoose..."));
  struct mg_mgr mgr;        // Initialise Mongoose event manager
  mg_mgr_init(&mgr);        // and attach it to the MIP interface
  mg_log_set(MG_LL_DEBUG);  // Set log level
  mg_timer_add(&mgr, BLINK_PERIOD_MS, MG_TIMER_REPEAT, timer_cb, NULL);

  // Initialise Mongoose network stack
  // Specify MAC address, and IP/mask/GW in network byte order for static
  // IP configuration. If IP/mask/GW are unset, DHCP is going to be used
  struct mg_tcpip_driver_stm32h_data driver_data = {.mdc_cr = 4};
  struct mg_tcpip_if mif = {
      .mac = {2, 0, 1, 2, 3, 5},
      .driver = &mg_tcpip_driver_stm32h,
      .driver_data = &driver_data,
  };
  mg_tcpip_init(&mgr, &mif);

  MG_INFO(("Waiting until network is up..."));
  while (mif.state != MIP_STATE_READY) {
    mg_mgr_poll(&mgr, 0);
  }

  MG_INFO(("Initialising application..."));
  extern void device_dashboard_fn(struct mg_connection *, int, void *, void *);
  mg_http_listen(&mgr, "http://0.0.0.0", device_dashboard_fn, &mgr);

  MG_INFO(("Starting event loop"));
  for (;;) mg_mgr_poll(&mgr, 0);  // Infinite event loop

  return 0;
}
