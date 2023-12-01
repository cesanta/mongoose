// Copyright (c) 2023 Cesanta Software Limited

#include "hal.h"
#include "mongoose.h"
#include "net.h"

#define BTN_PIN PIN('B', 3)    // On-board user button
#define LED1_PIN PIN('A', 15)  // On-board red LED
#define LED2_PIN PIN('B', 4)   // On-board blue LED
#define LED_PIN LED2_PIN

#define BLINK_PERIOD_MS 1000  // LED_PIN blinking period in millis

static volatile uint64_t s_ticks;  // Milliseconds since boot
__attribute__((interrupt())) void SysTick_Handler(void) {
  s_ticks++;
  SysTick->SR = 0;
}

uint64_t mg_millis(void) {  // Let Mongoose use our uptime function
  return s_ticks;           // Return number of milliseconds since boot
}

void mg_random(void *buf, size_t len) {  // Use on-board RNG
  for (size_t n = 0; n < len; n += sizeof(uint32_t)) {
    uint32_t r = rng_read();
    memcpy((char *) buf + n, &r, n + sizeof(r) > len ? len - n : sizeof(r));
  }
}

// This flash space resides at after the 0-wait 320k area
static char *s_flash_space = (char *) (0x8000000 + 320 * 1024);

bool web_load_settings(void *buf, size_t len) {
  if (*(uint32_t *) s_flash_space != SETTINGS_MAGIC) return false;
  memcpy(buf, s_flash_space, len);
  return true;
}

bool web_save_settings(void *buf, size_t len) {
  return mg_flash_write(s_flash_space, buf, len);
}

static void timer_fn(void *arg) {
  gpio_toggle(LED_PIN);                                  // Blink LED_PIN
  struct mg_tcpip_if *ifp = arg;                         // And show
  const char *names[] = {"down", "up", "req", "ready"};  // network stats
  return;
  MG_INFO(("Ethernet: %s, IP: %M, rx:%u, tx:%u, dr:%u, er:%u",
           names[ifp->state], mg_print_ip4, &ifp->ip, ifp->nrecv, ifp->nsent,
           ifp->ndrop, ifp->nerr));
}

void SysTick_Init(void) {
  SysTick->CMP = SystemCoreClock / 1000 - 1;
  SysTick->CNT = 0;
  SysTick->SR = 0;
  SysTick->CTLR = BIT(0) | BIT(1) | BIT(2) | BIT(3);
  NVIC_EnableIRQ(SysTicK_IRQn);
}

static void mg_putchar(char ch, void *param) {
  uart_write_byte(param, ch);
}

// https://mongoose.ws/documentation/#2-minute-integration-guide
static void fn(struct mg_connection *c, int ev, void *ev_data, void *fn_data) {
  if (ev == MG_EV_HTTP_MSG) {
    struct mg_http_message *hm = (struct mg_http_message *) ev_data;
    if (mg_http_match_uri(hm, "/api/ram")) {
      // This endpoint allows to read RAM:
      // curl IP:8000/api/ram -d '{"addr":"0x20", "len": 32}'
      char *s = mg_json_get_str(hm->body, "$.addr");
      void *buf = (void *) (uintptr_t) (s ? strtoul(s, NULL, 0) : 0);
      size_t len = mg_json_get_long(hm->body, "$.len", 4);
      mg_hexdump(buf, len);
      mg_http_reply(c, 200, "", "{%m:%m}\n", MG_ESC("data"), mg_print_hex, len,
                    buf);
      free(s);
    } else {
      // For all other URIs, serve static content
      mg_http_reply(c, 200, "", "hi tick %llu\n", s_ticks);
    }
  }
  (void) fn_data;
}

int main(void) {
  SystemCoreClockUpdate();
  SysTick_Init();

  gpio_output(LED_PIN);           // Setup LED
  uart_init(UART_DEBUG, 115200);  // Initialise debug printf

  struct mg_mgr mgr;        // Initialise
  mg_mgr_init(&mgr);        // Mongoose event manager
  mg_log_set(MG_LL_DEBUG);  // Set log level
  mg_log_set_fn(mg_putchar, UART_DEBUG);

  MG_INFO(("Starting, CPU freq %g MHz", (double) SystemCoreClock / 1000000));
  // MG_INFO(("RCC_RSTSCKR=%#lx", RCC->RSTSCKR));
  extern char _end[], _heap_end[];
  MG_INFO(("Heap size: %lu bytes", _heap_end - _end));

  // Print chip RAM/Flash configuration, and set to 64/256
  const char *sizes[] = {"128/192", "96/224", "64/256", "32/288"};
  uint32_t mode = (FLASH->OBR >> 8) & 3U;
  MG_INFO(("RAM/FLASH configuration: %s", sizes[mode]));
  if (mode != 3) set_ram_size(3), mg_device_reset();

  // Initialise Mongoose network stack
  ethernet_init();  // Initialise ethernet pins
  struct mg_tcpip_driver_stm32_data driver_data = {.mdc_cr = 1};
  struct mg_tcpip_if mif = {.mac = GENERATE_LOCALLY_ADMINISTERED_MAC(),
                            // Uncomment below for static configuration:
                            // .ip = mg_htonl(MG_U32(192, 168, 0, 223)),
                            // .mask = mg_htonl(MG_U32(255, 255, 255, 0)),
                            // .gw = mg_htonl(MG_U32(192, 168, 0, 1)),
                            .driver = &mg_tcpip_driver_stm32,
                            .driver_data = &driver_data};
  mg_tcpip_init(&mgr, &mif);
  mg_timer_add(&mgr, BLINK_PERIOD_MS, MG_TIMER_REPEAT, timer_fn, &mif);

  MG_INFO(("MAC: %M. Waiting for IP...", mg_print_mac, mif.mac));
  while (mif.state != MG_TCPIP_STATE_READY) {
    mg_mgr_poll(&mgr, 0);
  }

  MG_INFO(("Initialising application..."));
  mg_http_listen(&mgr, "http://0.0.0.0:8000", fn, NULL);
  web_init(&mgr);

  MG_INFO(("Starting event loop"));
  for (;;) {
    mg_mgr_poll(&mgr, 0);
  }

  return 0;
}

// Newlib syscall for malloc
void *_sbrk(ptrdiff_t incr) {
  extern char _end[], _heap_end[];
  static char *curbrk = _end;
  if ((curbrk + incr < _end) || (curbrk + incr > _heap_end)) return NULL - 1;
  //MG_INFO(("%p %ld", curbrk, incr));
  curbrk += incr;
  return curbrk - incr;
}
