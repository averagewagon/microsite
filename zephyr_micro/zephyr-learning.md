# Zephyr Project - Initial Setup Notes

## Configuration Management

- Two types of configuration files:
  - Board-specific (`esp32s3-config/prj.conf`): Hardware-specific settings
  - Application-specific (`prj.conf`): Software feature settings

## Configuration Workflow

1. Use menuconfig to explore options:
   ```bash
   west build -t menuconfig
   ```
2. Press 'd' to save minimal config showing only changes
3. Review changes in `.config` file
4. Add relevant settings to appropriate `prj.conf`:
   - Hardware settings -> board config
   - Application features -> app config

## Project Structure

```
zephyr_micro/
├── esp32s3-config/     # Board-specific settings
│   └── prj.conf        # Hardware configs
├── CMakeLists.txt      # Main application build
├── prj.conf            # Application configs
└── src/
    └── main.c
```

## Key Configs Found

Board-specific:

```
CONFIG_ESP_SPIRAM=y
CONFIG_SPIRAM_MODE_OCT=y
CONFIG_ESP_SPIRAM_SIZE=8388608
CONFIG_SPIRAM_SPEED_80M=y
```

Application features:

```
CONFIG_HEAP_MEM_POOL_SIZE=4096
CONFIG_KERNEL_MEM_POOL=y
CONFIG_STATS=y
CONFIG_MEMORY_STATS=y
CONFIG_SERIAL=y
CONFIG_GPIO=y
CONFIG_CONSOLE=y
CONFIG_UART_CONSOLE=y
```

## CMake Configuration

Important: Board and config settings must be set before `find_package(Zephyr)`

```cmake
set(BOARD esp32s3_devkitm/esp32s3/procpu)
set(CONF_FILE "${CMAKE_CURRENT_SOURCE_DIR}/esp32s3-config/prj.conf;${CMAKE_CURRENT_SOURCE_DIR}/prj.conf")
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
```

## Basic Build Commands

```bash
# Clean build
west build -t pristine

# Build with specific board
west build -b esp32s3_devkitm

# Flash to device
west flash
```
