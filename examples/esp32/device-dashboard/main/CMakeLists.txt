idf_component_register(SRCS "main.c"
                            "wifi.c"
                            "net.c"
                            "packed_fs.c"
                            "mongoose.c")
component_compile_options(-DMG_ENABLE_LINES)
component_compile_options(-DMG_ENABLE_PACKED_FS)
