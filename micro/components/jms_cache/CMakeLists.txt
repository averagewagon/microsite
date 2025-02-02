idf_component_register(
    SRCS "cache.c"
    INCLUDE_DIRS "include"
    PRIV_INCLUDE_DIRS "."
    REQUIRES jms_error jms_filesystem)
