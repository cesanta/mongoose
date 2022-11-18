// Copyright (c) 2022 Cesanta Software Limited
// All rights reserved

#include "mcu.h"
#include "mongoose.h"

#define LED1 PIN('N', 1)  // On-board LED pin
#define LED2 PIN('N', 0)  // On-board LED pin
#define LED3 PIN('F', 4)  // On-board LED pin
#define LED4 PIN('F', 0)  // On-board LED pin
#define BTN1 PIN('J', 0)  // On-board user button
#define BTN2 PIN('J', 1)  // On-board user button

static uint64_t s_ticks, s_exti;  // Counters, increased by IRQ handlers

static void blink_cb(void *arg) {  // Blink periodically
  gpio_toggle(LED1);
  (void) arg;
}

uint64_t mg_millis(void) {  // Declare our own uptime function
  return s_ticks;           // Return number of milliseconds since boot
}

void HardFault_Handler(void) {  // Escalated fault handler
  gpio_write(LED1, 1);
  gpio_write(LED2, 0);
  for (;;)
    spin(2999999), gpio_toggle(LED1),
        gpio_toggle(LED2);  // Alternate LEDs forever
}

void SysTick_Handler(void) {  // SyStick IRQ handler, triggered every 1ms
  s_ticks++;
}

void GPIOJ_Handler(void) {  // External interrupt handler
  s_exti++;
  struct gpio *gpio = gpio_bank(BTN1);
  if (gpio->GPIOMIS & BIT(PINNO(BTN1))) gpio->GPIOICR = BIT(PINNO(BTN1));
  gpio_write(LED2,
             !gpio_read(BTN1));  // No debounce. Turn LED if button is pressed
}

int main(void) {
  static struct uart *uart = UART0;  // Use UART0 (attached to ICDI)
  clock_init();                      // Set clock to 120MHz
  systick_init(FREQ / 1000);         // Increment s_ticks every ms
  gpio_output(LED1);                 // Setup user LEDs
  gpio_output(LED2);
  gpio_input(BTN1);       // Set button to input
  gpio_irq_attach(BTN1);  // generate interrupts
  uart_init(uart, 115200);

  nvic_enable_irq(40);  // Setup Ethernet IRQ handler

  MG_INFO(("Init hw"));
  // See datasheet: https://www.ti.com/lit/pdf/spms433
  // Assign LED3 and LED4 to the EPHY, "activity" and "link", respectively.
  // (20.4.2.4)
  gpio_init(LED3, GPIO_MODE_AF, GPIO_OTYPE_PUSH_PULL, GPIO_SPEED_HIGH,
            GPIO_PULL_NONE, 5);  // EN0LED1
  gpio_init(LED4, GPIO_MODE_AF, GPIO_OTYPE_PUSH_PULL, GPIO_SPEED_HIGH,
            GPIO_PULL_NONE, 5);  // EN0LED0
  // Initialize Ethernet clocks, see datasheet section 5
  // Turn Flash Prefetch off (silicon errata ETH#02)
  volatile uint32_t *IMC = (uint32_t *) 0x400FD000;
  uint32_t val = IMC[0xFC8 / sizeof(*IMC)];
  val &= ~BIT(17);
  val |= BIT(16);
  IMC[0xFC8 / sizeof(*IMC)] = val;
  SYSCTL->RCGCEMAC |= BIT(0);  // Enable EMAC clock
  SYSCTL->SREMAC |= BIT(0);    // Reset EMAC
  SYSCTL->SREMAC &= ~BIT(0);
  SYSCTL->RCGCEPHY |= BIT(0);  // Enable EPHY clock
  SYSCTL->SREPHY |= BIT(0);    // Reset EPHY
  SYSCTL->SREPHY &= ~BIT(0);
  while (!(SYSCTL->PREMAC & BIT(0)) || !(SYSCTL->PREPHY & BIT(0)))
    spin(1);  // Wait for reset to complete

  struct mg_mgr mgr;        // Initialize Mongoose event manager
  mg_mgr_init(&mgr);        // and attach it to the MIP interface
  mg_log_set(MG_LL_DEBUG);  // Set log level
  mg_timer_add(&mgr, 500, MG_TIMER_REPEAT, blink_cb, &mgr);

  // Initialize Mongoose network stack
  // Specify MAC address, either set use_dhcp or enter a static config.
  // For static configuration, specify IP/mask/GW in network byte order
  struct mip_driver_tm4c driver_data = {.mdc_cr = 1};  // See driver_tm4c.h
  struct mip_if mif = {
      .mac = {2, 0, 1, 2, 3, 5},
      .use_dhcp = true,
      .driver = &mip_driver_tm4c,
      .driver_data = &driver_data,
  };
  mip_init(&mgr, &mif);
  val = IMC[0xFC8 / sizeof(*IMC)];  // Turn Flash Prefetch on again
  val &= ~BIT(16);
  val |= BIT(17);
  IMC[0xFC8 / sizeof(*IMC)] = val;
  MG_INFO(("Init done, starting main loop"));

  extern void device_dashboard_fn(struct mg_connection *, int, void *, void *);
  mg_http_listen(&mgr, "http://0.0.0.0", device_dashboard_fn, &mgr);
  for (;;) mg_mgr_poll(&mgr, 0);  // Infinite event loop

  return 0;
}
