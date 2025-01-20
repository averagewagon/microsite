idf_component_register(
    SRCS "mime.c" "mime_gperf_output.c"
    INCLUDE_DIRS "include"
    PRIV_INCLUDE_DIRS "."
    REQUIRES jms_error)
