idf_component_register(SRCS "main.c"
                            "wifi.c"
                            "../../../mongoose.c"
                       INCLUDE_DIRS "../../..")
component_compile_options(-DMG_ENABLE_DIRECTORY_LISTING=1 -DMG_ENABLE_LINES)
