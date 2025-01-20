idf_component_register(
    SRCS "mime.c" "mime_hash.c"
    INCLUDE_DIRS "include"
    PRIV_INCLUDE_DIRS "."
    REQUIRES jms_error)
