idf_component_register(SRCS "main.c" "../../../../mongoose.c"
                            "../../../mqtt/mqtt-client/mongoose_mqtt.c"
                       INCLUDE_DIRS "." "../../../.."
                       REQUIRES ieee802154 esp_timer esp_hw_support
                                esp_driver_gpio app_update)
