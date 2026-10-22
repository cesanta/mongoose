idf_component_register(SRCS "main.c"
                            "wifi.c"
                            "../../../../mongoose.c"
                       INCLUDE_DIRS "."
                                    "../../../.."
                       PRIV_REQUIRES mbedtls
                       REQUIRES esp_event
                                esp_netif
                                esp_partition
                                esp_wifi
                                esp_timer
                                espcoredump
                                nvs_flash
                                app_update)
separate_arguments(CFLAGS_EXTRA)
component_compile_options(${CFLAGS_EXTRA})
