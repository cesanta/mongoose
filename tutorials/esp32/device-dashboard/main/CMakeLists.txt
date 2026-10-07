idf_component_register(SRCS "main.c"
                            "wifi.c"
                            "net.c"
                            "packed_fs.c"
                            "mongoose.c")
component_compile_options(-DHTTP_URL="http://0.0.0.0:80")
component_compile_options(-DHTTPS_URL="https://0.0.0.0:443")
