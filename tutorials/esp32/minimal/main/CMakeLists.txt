idf_component_register(SRCS "main.c"
                            "wifi.c"
                            "../../../../mongoose.c"
                       INCLUDE_DIRS "."
                                    "../../../.."
                       REQUIRES esp_event
                                esp_netif
                                esp_partition
                                esp_wifi
                                esp_timer
                                espcoredump
                                nvs_flash
                                app_update)
component_compile_options(-DHTTP_URL="http://0.0.0.0:80")
