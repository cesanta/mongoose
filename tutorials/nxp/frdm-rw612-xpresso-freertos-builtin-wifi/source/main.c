
#include <stdio.h>

#include "mongoose.h"

#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "timers.h"

#include "fsl_debug_console.h"

#include "fsl_power.h"



//#define WIFI_SSID "YOUR_WIFI_NETWORK_NAME"  // SET THIS!
//#define WIFI_PASS "YOUR_WIFI_PASSWORD"      // SET THIS!
#define WIFI_SSID "LinternaVerde"
#define WIFI_PASS "StanleyJordan69"


// mif user states
enum {AP, SCANNING, STOPPING_AP, CONNECTING, READY};
static unsigned int state;


static void mif_fn(struct mg_tcpip_if *ifp, int ev, void *ev_data) {
  // TODO(): should we include this inside ifp ? add an fn_data ?
  if (ev == MG_TCPIP_EV_ST_CHG) {
    MG_INFO(("State change: %u", *(uint8_t *) ev_data));
  }
  switch(state) {
    case AP: // we are in AP mode, wait for a user connection to trigger a scan or a connection to a network
      if (ev == MG_TCPIP_EV_ST_CHG && *(uint8_t *) ev_data == MG_TCPIP_STATE_READY) {
        MG_INFO(("Access Point READY !"));

        // simulate user request to scan for networks
        bool res = mg_wifi_scan();
        MG_INFO(("Starting scan: %s", res ? "OK":"FAIL"));
        if (res) state = SCANNING;
      }
      break;
    case SCANNING:
      if (ev == MG_TCPIP_EV_WIFI_SCAN_RESULT) {
        struct mg_wifi_scan_bss_data *bss = (struct mg_wifi_scan_bss_data *) ev_data;
        MG_INFO(("BSS: %.*s (%u) (%M) %d dBm %u", bss->SSID.len, bss->SSID.buf, bss->channel, mg_print_mac, bss->BSSID, (int) bss->RSSI, bss->security));
      } else if (ev == MG_TCPIP_EV_WIFI_SCAN_END) {
        // struct mg_tcpip_driver_nxp_wifi_data *d = (struct mg_tcpip_driver_nxp_wifi_data *) ifp->driver_data;
        MG_INFO(("Wi-Fi scan finished"));

        // simulate user selection of a network (1/2: stop AP)
        bool res = mg_wifi_ap_stop();
        MG_INFO(("Manually stopping AP: %s", res ? "OK":"FAIL"));
        if (res) state = STOPPING_AP;
        // else we have a hw/fw problem
      }
      break;
    case STOPPING_AP:
      if (ev == MG_TCPIP_EV_ST_CHG && *(uint8_t *) ev_data == MG_TCPIP_STATE_DOWN) {
        struct mg_wifi_data *wifi = &((struct mg_tcpip_driver_nxp_wifi_data *) ifp->driver_data)->wifi;
        wifi->apmode = false;

        // simulate user selection of a network (2/2: actual connect)
        bool res = mg_wifi_connect(wifi);
        MG_INFO(("Manually connecting: %s", res ? "OK":"FAIL"));
        if (res) {
          state = CONNECTING;
        } // else manually start AP as below
      }
      break;
    case CONNECTING:
      if (ev == MG_TCPIP_EV_ST_CHG && *(uint8_t *) ev_data == MG_TCPIP_STATE_READY) {
        MG_INFO(("READY!"));
        state = READY;

        // simulate user code disconnection and go back to AP mode (1/2: disconnect)
        bool res = mg_wifi_disconnect();
        MG_INFO(("Manually disconnecting: %s", res ? "OK":"FAIL"));
      } else if (ev == MG_TCPIP_EV_WIFI_CONNECT_ERR) {
        MG_ERROR(("Wi-Fi connect failed"));
        // manually start AP as below
      }
      break;
    case READY:
      // go back to AP mode after a disconnection (simulation 2/2), you could retry
      if (ev == MG_TCPIP_EV_ST_CHG && *(uint8_t *) ev_data == MG_TCPIP_STATE_DOWN) {
        struct mg_wifi_data *wifi = &((struct mg_tcpip_driver_nxp_wifi_data *) ifp->driver_data)->wifi;
        bool res = mg_wifi_ap_start(wifi);
        MG_INFO(("Disconnected"));
        MG_INFO(("Manually starting AP: %s", res ? "OK":"FAIL"));
        if (res) {
          state = AP;
          wifi->apmode = true;
        }
      }
      break;
  }
}


static void mongoose(void *arg) {
  struct mg_mgr mgr;        // Initialise Mongoose event manager
  mg_mgr_init(&mgr);        // and attach it to the interface
  mg_log_set(MG_LL_DEBUG);

  extern const struct mg_tcpip_driver netif_driver;

  // Initialise WiFi creds
  struct mg_tcpip_driver_nxp_wifi_data driver_data = {
    .wifi.ssid = WIFI_SSID,
    .wifi.pass = WIFI_PASS,
    .wifi.apssid = "mongoose",
    .wifi.appass = "mongoose",
    .wifi.apip = MG_IPV4(192, 168, 169, 1),
    .wifi.apmask = MG_IPV4(255, 255, 255, 0),
    .wifi.security = 0,
    .wifi.apsecurity = 0,
    .wifi.apchannel = 10,
    .wifi.apmode = true
  };

  state = driver_data.wifi.apmode ? AP : CONNECTING;

  // Initialise Mongoose network stack
  // Either set use_dhcp or enter a static config.
  // For static configuration, specify IP/mask/GW in network byte order
  struct mg_tcpip_if mif = {
      .ip = 0,
      .driver = (struct mg_tcpip_driver *)&mg_tcpip_driver_nxp_wifi,
      .driver_data = (struct mg_tcpip_driver_nxp_wifi_data*)&driver_data,
      .fn = mif_fn,
//      .recv_queue.size = 8192
  };

  mg_tcpip_init(&mgr, &mif);

    MG_INFO(("Starting event loop"));

    for (;;) {
      mg_mgr_poll(&mgr, 10);
    }
}


int main(void)
{
    BOARD_InitBootPins();
    BOARD_BootClockLPR();
    CLOCK_EnableClock(kCLOCK_Otp);
    CLOCK_EnableClock(kCLOCK_Els);
    CLOCK_EnableClock(kCLOCK_ElsApb);
    RESET_PeripheralReset(kOTP_RST_SHIFT_RSTn);
    RESET_PeripheralReset(kELS_APB_RST_SHIFT_RSTn);
    BOARD_InitDebugConsole();
    RESET_PeripheralReset(kGDMA_RST_SHIFT_RSTn);
    POWER_ConfigCauInSleep(false);
    BOARD_InitSleepPinConfig();

    xTaskCreate(mongoose, "mongoose", 2048, NULL, configMAX_PRIORITIES - 4, NULL);

    vTaskStartScheduler();  // This blocks

    return 0;
}
