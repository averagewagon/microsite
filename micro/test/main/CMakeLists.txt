idf_component_register(SRCS "main.c"
                    PRIV_REQUIRES
                        unity
                        nvs_flash
                        jms_error
                        jms_mime
                        jms_filesystem
                    INCLUDE_DIRS ".")