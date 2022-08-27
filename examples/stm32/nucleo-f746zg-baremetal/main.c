// Copyright (c) 2022 Cesanta Software Limited
// All rights reserved

#include "mcu.h"
#include "mongoose.h"

#define LED1 PIN('B', 0)   // On-board LED pin (green)
#define LED2 PIN('B', 7)   // On-board LED pin (blue)
#define LED3 PIN('B', 14)  // On-board LED pin (red)
#define BTN1 PIN('C', 13)  // On-board user button

static uint64_t s_ticks, s_exti;  // Counters, increased by IRQ handlers

static void blink_cb(void *arg) {  // Blink periodically
  // MG_INFO(("ticks: %u", (unsigned) s_ticks));
  gpio_toggle(LED2);
  (void) arg;
}

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

int main(void) {
  static struct uart *uart = UART3;  // Use UART3 - its attached to debug
  clock_init();                      // Set clock to 216MHz
  systick_init(FREQ / 1000);         // Increment s_ticks every ms
  gpio_output(LED1);                 // Setup green LED
  gpio_output(LED2);                 // Setup blue LED
  gpio_input(BTN1);                  // Set button to input
  irq_exti_attach(BTN1);             // Attach BTN1 to exti
  uart_init(uart, 115200);           // It is wired to the debug port

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
  mg_timer_add(&mgr, 500, MG_TIMER_REPEAT, blink_cb, &mgr);

  // Initialise Mongoose network stack
  // Specify MAC address, and use 0 for IP, mask, GW - i.e. use DHCP
  // For static configuration, specify IP/mask/GW in network byte order
  struct mip_ipcfg c = {.mac = {0, 0, 1, 2, 3, 4}, .ip = 0, .mask = 0, .gw = 0};
  mip_init(&mgr, &c, &mip_driver_stm32, NULL);
  MG_INFO(("Init done, starting main loop"));

  extern void device_dashboard_fn(struct mg_connection *, int, void *, void *);
  mg_http_listen(&mgr, "http://0.0.0.0", device_dashboard_fn, &mgr);
  for (;;) mg_mgr_poll(&mgr, 0);  // Infinite event loop

  return 0;
}
