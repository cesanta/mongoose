// Copyright (c) 2022 Cesanta Software Limited
// All rights reserved

#include "mcu.h"
#include "mongoose.h"

#define LED1 PIN('N', 1)  // On-board LED pin
#define LED2 PIN('N', 0)  // On-board LED pin
#define LED3 PIN('F', 4)  // On-board LED pin
#define LED4 PIN('F', 0)  // On-board LED pin

// HTTP server event handler function
static void fn(struct mg_connection *c, int ev, void *ev_data, void *fn_data) {
  if (ev == MG_EV_HTTP_MSG) {
    struct mg_http_message *hm = (struct mg_http_message *) ev_data;
    if (mg_http_match_uri(hm, "/api/debug")) {
      int level = mg_json_get_long(hm->body, "$.level", MG_LL_DEBUG);
      mg_log_set(level);
      mg_http_reply(c, 200, "", "Debug level set to %d\n", level);
    } else {
      mg_http_reply(c, 200, "", "%s\n", "hi");
    }
  }
  (void) fn_data;
}

static void ethernet_init(void) {
  // See datasheet: https://www.ti.com/lit/pdf/spms433
  // Assign LED3 and LED4 to the EPHY, "activity" and "link", respectively.
  // (20.4.2.4)
  gpio_init(LED3, GPIO_MODE_AF, GPIO_OTYPE_PUSH_PULL, GPIO_SPEED_HIGH,
            GPIO_PULL_NONE, 5);  // EN0LED1
  gpio_init(LED4, GPIO_MODE_AF, GPIO_OTYPE_PUSH_PULL, GPIO_SPEED_HIGH,
            GPIO_PULL_NONE, 5);  // EN0LED0
  nvic_enable_irq(40);  // Setup Ethernet IRQ handler
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
}

static void server(void *args) {
  struct mg_mgr mgr;        // Initialise Mongoose event manager
  mg_mgr_init(&mgr);        // and attach it to the MIP interface
  mg_log_set(MG_LL_DEBUG);  // Set log level

  // Initialise Mongoose network stack
  // Specify MAC address, either set use_dhcp or enter a static config.
  // For static configuration, specify IP/mask/GW in network byte order
  MG_INFO(("Initializing Ethernet driver"));
  ethernet_init();
  struct mip_driver_tm4c driver_data = {.mdc_cr = 1};  // See driver_tm4c.h
  struct mip_if mif = {
      .mac = {2, 0, 1, 2, 3, 5},
      .use_dhcp = true,
      .driver = &mip_driver_tm4c,
      .driver_data = &driver_data,
  };
  mip_init(&mgr, &mif);
  volatile uint32_t *IMC = (uint32_t *) 0x400FD000;
  uint32_t val = IMC[0xFC8 / sizeof(*IMC)];  // Turn Flash Prefetch on again
  val &= ~BIT(16);
  val |= BIT(17);
  IMC[0xFC8 / sizeof(*IMC)] = val;

  MG_INFO(("Starting Mongoose v%s", MG_VERSION));    // Tell the world
  mg_http_listen(&mgr, "http://0.0.0.0", fn, &mgr);  // Web listener
  while (args == NULL) mg_mgr_poll(&mgr, 1000);      // Infinite event loop
  mg_mgr_free(&mgr);                                 // Unreachable
}

static void blinker(void *args) {
  gpio_init(LED1, GPIO_MODE_OUTPUT, GPIO_OTYPE_PUSH_PULL, GPIO_SPEED_HIGH,
            GPIO_PULL_NONE, 0);
  for (;;) {
    gpio_toggle(LED1);
    vTaskDelay(pdMS_TO_TICKS(750));
    (void)args;//MG_INFO(("blink %s,  RAM: %u", (char *) args, xPortGetFreeHeapSize()));
  }
}

int main(void) {
  static struct uart *uart = UART0;  // Use UART0 (attached to ICDI)
  clock_init();                      // Set clock to 120MHz
  systick_init(FREQ / 1000);  // Tick every 1 ms
  uart_init(uart, 115200);   // Initialise UART
  xTaskCreate(blinker, "blinker", 128, ":)", configMAX_PRIORITIES - 1, NULL);
  xTaskCreate(server, "server", 2048, 0, configMAX_PRIORITIES - 1, NULL);
  vTaskStartScheduler();  // This blocks
  return 0;
}
