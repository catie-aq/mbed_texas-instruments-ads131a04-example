# Texas Instruments ADS131A04 Example
Mbed OS example usage of ADS131A04

## Requirements
### Hardware requirements
The following boards are required:
- *List Texas Instruments ADS131A04 Example hardware requirements here*

### Software requirements
Texas Instruments ADS131A04 Example makes use of the following libraries (automatically
imported by `mbed deploy` or `mbed import`):
- *List Texas Instruments ADS131A04 Example software requirements here*

## Usage
To clone **and** deploy the project in one command, use `mbed import` and skip to the
target and toolchain definition:
```shell
mbed import https://gitlab.com/catie_6tron/texas-instruments-ads131a04-example.git texas-instruments-ads131a04-example
```

Alternatively:

- Clone to "texas-instruments-ads131a04-example" and enter it:
  ```shell
  git clone https://gitlab.com/catie_6tron/texas-instruments-ads131a04-example.git texas-instruments-ads131a04-example
  cd texas-instruments-ads131a04-example
  ```

- Deploy software requirements with:
  ```shell
  mbed deploy
  ```

- Set Mbed project root path:
  ```shell
  mbed config root .
  ```

Define your target (eg. `ZEST_CORE_STM32L4A6RG`) and toolchain:
```shell
mbed target ZEST_CORE_STM32L4A6RG
mbed toolchain GCC_ARM
```

Compile the project:
```shell
mbed compile
```

Program the target device (eg. `STM32L4A6RG` for the Zest_Core_STM32L4A6RG) with a J-Link
debug probe and [`sixtron_flash`](https://gitlab.com/catie_6tron/6tron-flash) tool:
```shell
sixtron_flash STM32L4A6RG BUILD/ZEST_CORE_STM32L4A6RG/GCC_ARM/texas-instruments-ads131a04-example.elf
```

Debug on the target device with a debug probe, eg. Segger J-Link and
[Ozone](https://www.segger.com/products/development-tools/ozone-j-link-debugger)
software.
