// Copyright (c) 2022 Cesanta Software Limited
// All rights reserved

#include "mcu.h"
#include "mongoose.h"

#define LED1 PIN('B', 0)      // On-board LED pin (green)
#define LED2 PIN('B', 7)      // On-board LED pin (blue)
#define LED3 PIN('B', 14)     // On-board LED pin (red)
#define BTN1 PIN('C', 13)     // On-board user button
#define BLINK_PERIOD_MS 1000  // LED blinking period in millis

static uint64_t s_ticks, s_exti;  // Counters, increased by IRQ handlers

uint64_t mg_millis(void) {  // Declare our own uptime function
  return s_ticks;           // Return number of milliseconds since boot
}

void HardFault_Handler(void) {                // Escalated fault handler
  gpio_output(LED3);                          // Setup red LED
  for (;;) spin(2999999), gpio_toggle(LED3);  // Blink LED infinitely
}

void SysTick_Handler(void) {  // SyStick IRQ handler, triggered every 1ms
  s_ticks++;
}

void EXTI15_10_IRQHandler(void) {  // External interrupt handler
  s_exti++;
  if (EXTI->PR & BIT(PINNO(BTN1))) EXTI->PR = BIT(PINNO(BTN1));
  gpio_write(LED1, gpio_read(BTN1));  // No debounce. Turn LED if button pressed
}

static void timer_cb(void *arg) {
  gpio_toggle(LED2);  // Blink LED
  bool up = ((struct mip_if *) arg)->state == MIP_STATE_READY;
  MG_INFO(("Ethernet: %s", up ? "up" : "down"));  // Show network status
}

int main(void) {
  clock_init();                        // Set clock to 180MHz
  systick_init(SYS_FREQUENCY / 1000);  // Increment s_ticks every ms
  gpio_output(LED1);                   // Setup green LED
  gpio_output(LED2);                   // Setup blue LED
  gpio_input(BTN1);                    // Set button to input
  irq_exti_attach(BTN1);               // Attach BTN1 to exti
  uart_init(UART1, 115200);            // Initialise debug printf

  MG_INFO(("Starting, CPU freq %g MHz", (double) SYS_FREQUENCY / 1000000));

  // Initialise Ethernet. Enable MAC GPIO pins, see
  // https://www.farnell.com/datasheets/2014265.pdf section 6.10
  uint16_t pins[] = {PIN('A', 1),  PIN('A', 2),  PIN('A', 7),
                     PIN('B', 13), PIN('C', 1),  PIN('C', 4),
                     PIN('C', 5),  PIN('G', 11), PIN('G', 13)};
  for (size_t i = 0; i < sizeof(pins) / sizeof(pins[0]); i++) {
    gpio_init(pins[i], GPIO_MODE_AF, GPIO_OTYPE_PUSH_PULL, GPIO_SPEED_INSANE,
              GPIO_PULL_NONE, 11);
  }
  nvic_enable_irq(61);                          // Setup Ethernet IRQ handler
  RCC->APB2ENR |= BIT(14);                      // Enable SYSCFG
  SYSCFG->PMC |= BIT(23);                       // Use RMII. Goes first!
  RCC->AHB1ENR |= BIT(25) | BIT(26) | BIT(27);  // Enable Ethernet clocks
  RCC->AHB1RSTR |= BIT(25);                     // ETHMAC force reset
  RCC->AHB1RSTR &= ~BIT(25);                    // ETHMAC release reset

  struct mg_mgr mgr;        // Initialise Mongoose event manager
  mg_mgr_init(&mgr);        // and attach it to the MIP interface
  mg_log_set(MG_LL_DEBUG);  // Set log level

  // Initialise Mongoose network stack
  // Specify MAC address, and IP/mask/GW in network byte order for static
  // IP configuration. If IP/mask/GW are unset, DHCP is going to be used
  struct mip_driver_stm32_data driver_data = {.mdc_cr = 4};  // driver_stm32.h
  struct mip_if mif = {.mac = {2, 0, 1, 2, 3, 5},
                       .driver = &mip_driver_stm32,
                       .driver_data = &driver_data};
  mip_init(&mgr, &mif);
  mg_timer_add(&mgr, BLINK_PERIOD_MS, MG_TIMER_REPEAT, timer_cb, &mif);

  MG_INFO(("Waiting until network is up..."));
  while (mif.state != MIP_STATE_READY) {
    mg_mgr_poll(&mgr, 0);
  }

  MG_INFO(("Initialising application..."));
  extern void device_dashboard_fn(struct mg_connection *, int, void *, void *);
  mg_http_listen(&mgr, "http://0.0.0.0", device_dashboard_fn, &mgr);

  MG_INFO(("Starting event loop"));
  for (;;) {
    mg_mgr_poll(&mgr, 0);
  }

  return 0;
}
