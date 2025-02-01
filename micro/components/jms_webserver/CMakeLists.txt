idf_component_register(
    SRCS "webserver.c"
    INCLUDE_DIRS "include"
    PRIV_INCLUDE_DIRS "."
    REQUIRES jms_error esp_https_server)
