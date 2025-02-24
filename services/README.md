# Microsite: Services

## Overview

This folder contains systemd services and scripts for monitoring and managing
the Microsite MCU on a Raspberry Pi 4B. The services handle UART logging and
automatic USB power cycling for the ESP32-S3.

## Installation and Configuration

Services are installed and managed using `install_services.sh`, located in the
`scripts/` folder:

```sh
cd scripts/
./install_services.sh
```

## Dependencies

- `uhubctl` is required for USB power cycling:
  ```sh
  sudo apt install uhubctl
  ```
