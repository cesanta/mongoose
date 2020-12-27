#include <string.h>
#include "esp_event.h"
#include "esp_log.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"
#include "freertos/task.h"
#include "nvs_flash.h"

#include "lwip/err.h"
#include "lwip/sys.h"

#include "mongoose.h"

static EventGroupHandle_t s_wifi_event_group;

/* The event group allows multiple bits for each event, but we only care about
 * two events:
 * - we are connected to the AP with an IP
 * - we failed to connect after the maximum amount of retries */
#define WIFI_CONNECTED_BIT BIT0
#define WIFI_FAIL_BIT BIT1

static int s_retry_num = 0;

static void event_handler(void *arg, esp_event_base_t event_base,
                          int32_t event_id, void *event_data) {
  if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START) {
    esp_wifi_connect();
  } else if (event_base == WIFI_EVENT &&
             event_id == WIFI_EVENT_STA_DISCONNECTED) {
    if (s_retry_num < 3) {
      esp_wifi_connect();
      s_retry_num++;
      LOG(LL_DEBUG, ("retry to connect to the AP"));
    } else {
      xEventGroupSetBits(s_wifi_event_group, WIFI_FAIL_BIT);
    }
    LOG(LL_ERROR, ("connect to the AP fail"));
  } else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {
    ip_event_got_ip_t *event = (ip_event_got_ip_t *) event_data;
    LOG(LL_INFO, ("got ip:%s", ip4addr_ntoa(&event->ip_info.ip)));
    s_retry_num = 0;
    xEventGroupSetBits(s_wifi_event_group, WIFI_CONNECTED_BIT);
  }
}

void wifi_init(const char *ssid, const char *pass) {
  s_wifi_event_group = xEventGroupCreate();
  tcpip_adapter_init();
  ESP_ERROR_CHECK(esp_event_loop_create_default());
  wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
  ESP_ERROR_CHECK(esp_wifi_init(&cfg));
  ESP_ERROR_CHECK(esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID,
                                             &event_handler, NULL));
  ESP_ERROR_CHECK(esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP,
                                             &event_handler, NULL));

  wifi_config_t c = {};
  snprintf((char *) c.sta.ssid, sizeof(c.sta.ssid), "%s", ssid);
  snprintf((char *) c.sta.password, sizeof(c.sta.password), "%s", pass);
  if (pass != NULL && pass[0] != '\0') {
    c.sta.threshold.authmode = WIFI_AUTH_WPA2_PSK;
  }
  ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
  ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_STA, &c));
  ESP_ERROR_CHECK(esp_wifi_start());
  EventBits_t bits = xEventGroupWaitBits(s_wifi_event_group,
                                         WIFI_CONNECTED_BIT | WIFI_FAIL_BIT,
                                         pdFALSE, pdFALSE, portMAX_DELAY);

  if (bits & WIFI_CONNECTED_BIT) {
    LOG(LL_INFO, ("connected to ap SSID:%s password:%s", ssid, pass));
  } else if (bits & WIFI_FAIL_BIT) {
    LOG(LL_ERROR, ("Failed to connect to SSID:%s, password:%s", ssid, pass));
  } else {
    LOG(LL_ERROR, ("UNEXPECTED EVENT"));
  }

  ESP_ERROR_CHECK(esp_event_handler_unregister(IP_EVENT, IP_EVENT_STA_GOT_IP,
                                               &event_handler));
  ESP_ERROR_CHECK(esp_event_handler_unregister(WIFI_EVENT, ESP_EVENT_ANY_ID,
                                               &event_handler));
  vEventGroupDelete(s_wifi_event_group);
}
