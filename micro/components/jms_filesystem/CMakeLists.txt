idf_component_register(
    SRCS "filesystem.c"
    INCLUDE_DIRS "include"
    PRIV_INCLUDE_DIRS "."
    REQUIRES jms_error nvs_flash joltwallet__littlefs)
