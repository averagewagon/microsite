cmake_minimum_required(VERSION 3.20.0)

# Load WiFi credentials from a separate file if available
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/creds/wifi_credentials.cmake")
    include("${CMAKE_CURRENT_SOURCE_DIR}/creds/wifi_credentials.cmake")
else()
    message(FATAL_ERROR "WiFi credentials not found! Create creds/wifi_credentials.cmake.")
endif()

# Pass WiFi credentials as compiler definitions
add_definitions(-DWIFI_SSID=\"${WIFI_SSID}\" -DWIFI_PSK=\"${WIFI_PSK}\")

# Set board
set(BOARD esp32s3_devkitc/esp32s3/procpu)

# Merge configuration files
set(CONF_FILE ${CMAKE_CURRENT_SOURCE_DIR}/prj.conf)

# Find and include Zephyr
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(microsite)

# Define source files
target_sources(app PRIVATE src/main.c)

# -------------------- Embed TLS Certificates --------------------

# Set the generated include directory inside the build directory
set(GEN_DIR ${CMAKE_BINARY_DIR}/include/generated/)

# Define certificate and key file paths
set(CERT_FILE ${CMAKE_CURRENT_SOURCE_DIR}/creds/fullchain.pem)
set(KEY_FILE ${CMAKE_CURRENT_SOURCE_DIR}/creds/privkey.pem)

# Ensure the generated directory exists
file(MAKE_DIRECTORY ${GEN_DIR})

# Convert the certificate files into header files
generate_inc_file_for_target(app ${CERT_FILE} ${GEN_DIR}/fullchain.pem.inc)
generate_inc_file_for_target(app ${KEY_FILE} ${GEN_DIR}/privkey.pem.inc)

# Include the generated directory
target_include_directories(app PRIVATE ${GEN_DIR})
