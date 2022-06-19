idf_component_register(SRCS "main.c"
                            "wifi.c"
                            "uart.c"
                            "cli.c"
                            "net.c"
                            "packed_fs.c"
                            "mongoose.c")
component_compile_options(-DMG_ENABLE_LINES=1)
component_compile_options(-DMG_ENABLE_PACKED_FS=1)
component_compile_options(-DUART_API_IMPLEMENTED=1)
