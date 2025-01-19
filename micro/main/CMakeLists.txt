idf_component_register(SRCS "main.c"
                            "jms/mime/mime.c"
                            "jms/mime/mime_hash.c"
                    INCLUDE_DIRS "."
                    PRIV_REQUIRES esp_https_server esp_wifi nvs_flash esp_eth
                    EMBED_TXTFILES "certs/fullchain.pem"
                                   "certs/privkey.pem")
