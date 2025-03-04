#include "mongoose.h"
#include "net.h"

void ethernet_init(void);
struct mg_mgr mgr;
struct mg_tcpip_driver_imxrt_data data = {.mdc_cr = 24, .phy_addr = 0};
struct mg_tcpip_if mif = {
    // Construct MAC address from the unique chip ID
    .mac = {2, (uint8_t) (HW_OCOTP_CFG0 & 255),
            (uint8_t) ((HW_OCOTP_CFG0 >> 10) & 255),
            (uint8_t) (((HW_OCOTP_CFG0 >> 19) ^ (HW_OCOTP_CFG1 >> 19)) & 255),
            (uint8_t) ((HW_OCOTP_CFG1 >> 10) & 255),
            (uint8_t) (HW_OCOTP_CFG1 & 255)},
    // The default is DHCP. Uncomment 3 lines below for static IP config:
    // .ip = mg_htonl(MG_U32(192, 168, 0, 223)),
    // .mask = mg_htonl(MG_U32(255, 255, 255, 0)),
    // .gw = mg_htonl(MG_U32(192, 168, 0, 1)),
    .driver = &mg_tcpip_driver_imxrt,
    .driver_data = &data};

uint64_t mg_millis(void) {  // Let Mongoose use our uptime function
  return millis();
}

// Simple HTTP server that runs on port 8000
// Mongoose event handler function, gets called by the mg_mgr_poll()
// See https://mongoose.ws/documentation/#2-minute-integration-guide
static void simple_http_listener(struct mg_connection *c, int ev, void *ev_data) {
  if (ev == MG_EV_HTTP_MSG) {
    // The MG_EV_HTTP_MSG event means HTTP request. `hm` holds parsed request,
    // see https://mongoose.ws/documentation/#struct-mg_http_message
    struct mg_http_message *hm = (struct mg_http_message *) ev_data;

    // If the requested URI is "/api/hi", send a simple JSON response back
    if (mg_match(hm->uri, mg_str("/api/hi"), NULL)) {
      // Use mg_http_reply() API function to generate JSON response. It adds a
      // Content-Length header automatically. In the response, we show
      // the requested URI and HTTP body:
      mg_http_reply(c, 200, "", "{%m:%m,%m:%m}\n",  // See mg_snprintf doc
                    MG_ESC("uri"), mg_print_esc, hm->uri.len, hm->uri.buf,
                    MG_ESC("body"), mg_print_esc, hm->body.len, hm->body.buf);
    } else {
      // For all other URIs, serve some static content
      mg_http_reply(c, 200, "", "<html>millis: %lu</html>", millis());
    }
  }
}

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(115200);
  while (!Serial) delay(50);

  mg_mgr_init(&mgr);        // Initialise Mongoose event manager
  mg_log_set(MG_LL_DEBUG);  // Set log level and log function
  mg_log_set_fn([](char ch, void *) { Serial.print(ch); }, NULL);

  MG_INFO(("CPU %g MHz. Starting TCP/IP stack", (double) F_CPU / 1000000));
  ethernet_init();
  mg_tcpip_init(&mgr, &mif);  // Initialise built-in TCP/IP stack

  MG_INFO(("Waiting for IP..."));
  while (mif.state != MG_TCPIP_STATE_READY) mg_mgr_poll(&mgr, 1);

  // We start two HTTP listeners: one is a simple one on port 8000,
  // with event handler function defined above - simple_http_listener
  // See https://mongoose.ws/documentation/#2-minute-integration-guide
  // Another listener is for a more sophisticated Web device dashboard
  MG_INFO(("Starting web dashboard"));
  mg_http_listen(&mgr, "http://0.0.0.0:8000", simple_http_listener, NULL);
  web_init(&mgr);  // Sophisticated Web UI, see net.c :: fn()
}

void loop() {
  static uint64_t timer;
  if (mg_timer_expired(&timer, 500, mg_millis())) {        // Every 500ms
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));  // blink an LED
  }

  mg_mgr_poll(&mgr, 1);  // Process network events
}

extern "C" void ENET_IRQHandler(void);

#define CLRSET(reg, clear, set) ((reg) = ((reg) & ~(clear)) | (set))
#define RMII_PAD_INPUT_PULLDOWN 0x30E9
#define RMII_PAD_INPUT_PULLUP 0xB0E9
#define RMII_PAD_CLOCK 0x0031

// initialize the ethernet hardware
void ethernet_init(void) {
  CCM_CCGR1 |= CCM_CCGR1_ENET(CCM_CCGR_ON);
  // configure PLL6 for 50 MHz, pg 1173
  CCM_ANALOG_PLL_ENET_CLR =
      CCM_ANALOG_PLL_ENET_POWERDOWN | CCM_ANALOG_PLL_ENET_BYPASS | 0x0F;
  CCM_ANALOG_PLL_ENET_SET = CCM_ANALOG_PLL_ENET_ENABLE |
                            CCM_ANALOG_PLL_ENET_BYPASS
                            /*| CCM_ANALOG_PLL_ENET_ENET2_REF_EN*/
                            | CCM_ANALOG_PLL_ENET_ENET_25M_REF_EN
                            /*| CCM_ANALOG_PLL_ENET_ENET2_DIV_SELECT(1)*/
                            | CCM_ANALOG_PLL_ENET_DIV_SELECT(1);
  while (!(CCM_ANALOG_PLL_ENET & CCM_ANALOG_PLL_ENET_LOCK))
    ;  // wait for PLL lock
  CCM_ANALOG_PLL_ENET_CLR = CCM_ANALOG_PLL_ENET_BYPASS;
  // configure REFCLK to be driven as output by PLL6, pg 326

  CLRSET(IOMUXC_GPR_GPR1,
         IOMUXC_GPR_GPR1_ENET1_CLK_SEL | IOMUXC_GPR_GPR1_ENET_IPG_CLK_S_EN,
         IOMUXC_GPR_GPR1_ENET1_TX_CLK_DIR);

  // Configure pins
  IOMUXC_SW_MUX_CTL_PAD_GPIO_B0_14 = 5;  // Reset   B0_14 Alt5 GPIO7.15
  IOMUXC_SW_MUX_CTL_PAD_GPIO_B0_15 = 5;  // Power   B0_15 Alt5 GPIO7.14
  GPIO7_GDIR |= (1 << 14) | (1 << 15);
  GPIO7_DR_SET = (1 << 15);                                    // Power on
  GPIO7_DR_CLEAR = (1 << 14);                                  // Reset PHY chip
  IOMUXC_SW_PAD_CTL_PAD_GPIO_B1_04 = RMII_PAD_INPUT_PULLDOWN;  // PhyAdd[0] = 0
  IOMUXC_SW_PAD_CTL_PAD_GPIO_B1_06 = RMII_PAD_INPUT_PULLDOWN;  // PhyAdd[1] = 1
  IOMUXC_SW_PAD_CTL_PAD_GPIO_B1_05 = RMII_PAD_INPUT_PULLUP;    // Slave mode
  IOMUXC_SW_PAD_CTL_PAD_GPIO_B1_11 = RMII_PAD_INPUT_PULLDOWN;  // Auto MDIX
  IOMUXC_SW_PAD_CTL_PAD_GPIO_B1_07 = RMII_PAD_INPUT_PULLUP;
  IOMUXC_SW_PAD_CTL_PAD_GPIO_B1_08 = RMII_PAD_INPUT_PULLUP;
  IOMUXC_SW_PAD_CTL_PAD_GPIO_B1_09 = RMII_PAD_INPUT_PULLUP;
  IOMUXC_SW_PAD_CTL_PAD_GPIO_B1_10 = RMII_PAD_CLOCK;
  IOMUXC_SW_MUX_CTL_PAD_GPIO_B1_05 = 3;         // RXD1    B1_05 Alt3, pg 525
  IOMUXC_SW_MUX_CTL_PAD_GPIO_B1_04 = 3;         // RXD0    B1_04 Alt3, pg 524
  IOMUXC_SW_MUX_CTL_PAD_GPIO_B1_10 = 6 | 0x10;  // REFCLK  B1_10 Alt6, pg 530
  IOMUXC_SW_MUX_CTL_PAD_GPIO_B1_11 = 3;         // RXER    B1_11 Alt3, pg 531
  IOMUXC_SW_MUX_CTL_PAD_GPIO_B1_06 = 3;         // RXEN    B1_06 Alt3, pg 526
  IOMUXC_SW_MUX_CTL_PAD_GPIO_B1_09 = 3;         // TXEN    B1_09 Alt3, pg 529
  IOMUXC_SW_MUX_CTL_PAD_GPIO_B1_07 = 3;         // TXD0    B1_07 Alt3, pg 527
  IOMUXC_SW_MUX_CTL_PAD_GPIO_B1_08 = 3;         // TXD1    B1_08 Alt3, pg 528
  IOMUXC_SW_MUX_CTL_PAD_GPIO_B1_15 = 0;         // MDIO    B1_15 Alt0, pg 535
  IOMUXC_SW_MUX_CTL_PAD_GPIO_B1_14 = 0;         // MDC     B1_14 Alt0, pg 534
  IOMUXC_ENET_MDIO_SELECT_INPUT = 2;            // GPIO_B1_15_ALT0, pg 792
  IOMUXC_ENET0_RXDATA_SELECT_INPUT = 1;         // GPIO_B1_04_ALT3, pg 792
  IOMUXC_ENET1_RXDATA_SELECT_INPUT = 1;         // GPIO_B1_05_ALT3, pg 793
  IOMUXC_ENET_RXEN_SELECT_INPUT = 1;            // GPIO_B1_06_ALT3, pg 794
  IOMUXC_ENET_RXERR_SELECT_INPUT = 1;           // GPIO_B1_11_ALT3, pg 795
  IOMUXC_ENET_IPG_CLK_RMII_SELECT_INPUT = 1;    // GPIO_B1_10_ALT6, pg 791
  delay(1);
  GPIO7_DR_SET = (1 << 14);  // Start PHY chip
  // ENET_MSCR = ENET_MSCR_MII_SPEED(9);
  delay(1);

  // Setup IRQ handler
  attachInterruptVector(IRQ_ENET, ENET_IRQHandler);
  NVIC_ENABLE_IRQ(IRQ_ENET);
}
