cmake_minimum_required(VERSION 3.20.0)

# Load WiFi credentials from a separate file if available
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/creds/wifi_credentials.cmake")
    include("${CMAKE_CURRENT_SOURCE_DIR}/creds/wifi_credentials.cmake")
else()
    message(FATAL_ERROR "WiFi credentials not found! Create creds/wifi_credentials.cmake.")
endif()

# Pass credentials as compiler definitions
add_definitions(-DWIFI_SSID=\"${WIFI_SSID}\" -DWIFI_PSK=\"${WIFI_PSK}\")

set(BOARD esp32s3_devkitc/esp32s3/procpu)

# Merge both configuration files
set(CONF_FILE
  ${CMAKE_CURRENT_SOURCE_DIR}/prj.conf
)

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(microsite)

target_sources(app PRIVATE src/main.c)