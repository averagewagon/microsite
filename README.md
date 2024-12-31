# Microsite - [averagewagon.dev](https://averagewagon.dev)

**Microsite** is a small static website hosted on an ESP32 microcontroller, live
at **[averagewagon.dev](https://averagewagon.dev)**. This project experiments
with running a functional website on resource-limited hardware.

## Prerequisites

- ESP-IDF v5.1+:

  - Clone:
    `git clone -b release/v5.1 --recursive https://github.com/espressif/esp-idf.git`
  - Install: `cd esp-idf && ./install.sh && . $HOME/esp/esp-idf/export.sh`

- Toolchain:

  - Verify: `xtensa-esp32-elf-gcc --version`

- Python 3.6+:

  - Install: `sudo apt install python3 python3-pip`
  - ESP-IDF reqs: `python3 -m pip install -r $IDF_PATH/requirements.txt`

- Git: `sudo apt install git`

- Shellcheck: `sudo apt install shellcheck`

- mklittlefs: [mklittlefs](https://github.com/earlephilhower/mklittlefs)

- Build Tools:
  - Install: `sudo apt install build-essential cmake ninja`
