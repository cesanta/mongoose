idf_component_register(SRCS "main.c" "../../../../mongoose.c"
                       INCLUDE_DIRS "." "../../../.."
                       REQUIRES esp_wifi esp_netif esp_event esp_timer esp_coex
                                ieee802154 nvs_flash lwip app_update esp_driver_gpio)
# WIFI_NAME and WIFI_PASS environment variables override the defaults in
# main.c. They are read when CMake configures: run "idf.py reconfigure" after
# changing them
foreach(var WIFI_NAME WIFI_PASS)
  if(NOT "$ENV{${var}}" STREQUAL "")
    target_compile_definitions(${COMPONENT_LIB} PRIVATE ${var}="$ENV{${var}}")
  endif()
endforeach()
