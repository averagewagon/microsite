idf_component_register(SRCS "main.c"
                    INCLUDE_DIRS "."
                    PRIV_REQUIRES esp_https_server esp_wifi nvs_flash esp_eth jms_error jms_mime
                    EMBED_TXTFILES "certs/fullchain.pem"
                                   "certs/privkey.pem")
