idf_component_register(SRCS "main.c"
                            "wifi.c"
                            "uart.c"
                            "cli.c"
                            "net.c"
                            "packed_fs.c"
                            "mongoose.c")
component_compile_options(-DUART_API_IMPLEMENTED=1)
