
#include <stdio.h>

#include "mongoose.h"
#include "net.h"

#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "timers.h"

#include "wpl.h"

#include "fsl_debug_console.h"

#include "fsl_power.h"



//#define WIFI_SSID "YOUR_WIFI_NETWORK_NAME"  // SET THIS!
//#define WIFI_PASS "YOUR_WIFI_PASSWORD"      // SET THIS!
#define WIFI_SSID "LinternaVerde"
#define WIFI_PASS "StanleyJordan69"

#define WPL_NWKLBL "wifi"	// WPL ids interfaces with text labels...

static bool set_sta(char *ssid, char *password, enum _wpl_security sec);
static bool clr_sta(void);
static bool set_ap(char *ssid, char *password, int channel);
static bool clr_ap(void);


static volatile bool isup_ = true;
static volatile bool isup = false;
// THIS GETS CALLED ON A DIFFERENT CONTEXT, not Mongoose's
static void linkstate_cb(bool linkstate) {
	isup = linkstate;
}

static void mongoose(void *arg) {
	struct mg_mgr mgr;        // Initialise Mongoose event manager
	mg_mgr_init(&mgr);        // and attach it to the interface

    MG_INFO(("Initializing Wi-Fi chip"));
    if(WPL_Init() != WPLRET_SUCCESS) return;
    if(WPL_Start(linkstate_cb) != WPLRET_SUCCESS) return;

    if (strlen(WIFI_SSID) == 0 || strlen(WIFI_PASS) == 0) {
    	set_ap("mongoose", "mongoose", 1);
    } else {
        MG_INFO(("Connecting to Wi-Fi network..."));
    	set_sta(WIFI_SSID, WIFI_PASS, WPL_SECURITY_WILDCARD);
    }

    MG_INFO(("Initialising application..."));
    web_init(&mgr);

    MG_INFO(("Starting event loop"));

    for (;;) {
      mg_mgr_poll(&mgr, 100);
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


static bool set_sta(char *ssid, char *password, enum _wpl_security sec) {
    if (WPL_AddNetworkWithSecurity(ssid, password, WPL_NWKLBL, sec) != WPLRET_SUCCESS)
       	return false;
    if (WPL_Join(WPL_NWKLBL) != WPLRET_SUCCESS)
    	return false;
    char ip[16];
    WPL_GetIP(ip, 1);
    MG_INFO(("Connected to %s, IP: %s", ssid, ip));
    return true;
}

static bool clr_sta(void) {
    vTaskDelay(1000 / portTICK_PERIOD_MS); // Let a possible reply reach the browser
    if (WPL_Leave() != WPLRET_SUCCESS)
    	return false;
    if (WPL_RemoveNetwork(WPL_NWKLBL) != WPLRET_SUCCESS)
    	return false;
    return true;
}

static bool set_ap(char *ssid, char *password, int channel) {
    if (WPL_Start_AP(ssid, password, channel) != WPLRET_SUCCESS)
    	return false;
    char ip[16];
    WPL_GetIP(ip, 0);
    MG_INFO(("Started AP: %s, IP: %s", ssid, ip));
    return true;
}

static bool clr_ap(void) {
    vTaskDelay(1000 / portTICK_PERIOD_MS); // Let a possible reply reach the browser
    if (WPL_Stop_AP() != WPLRET_SUCCESS) return false;
    return true;
}
