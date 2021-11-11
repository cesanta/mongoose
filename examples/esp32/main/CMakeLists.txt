idf_component_register(SRCS "main.c"
                            "wifi.c"
                            "mongoose.c")
component_compile_options(-DMG_ENABLE_LINES)
