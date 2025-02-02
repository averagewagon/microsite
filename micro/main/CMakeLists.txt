idf_component_register(SRCS "main.c"
                    INCLUDE_DIRS "."
                    PRIV_REQUIRES
                        esp_https_server
                        esp_wifi
                        nvs_flash
                        esp_eth
                        esp_mm
                        esp_psram
                        jms_error
                        jms_mime
                        jms_filesystem
                        jms_webserver
                        jms_cache
                    EMBED_TXTFILES "certs/fullchain.pem"
                                   "certs/privkey.pem")
