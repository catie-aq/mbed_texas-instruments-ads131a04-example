# Texas Instruments ADS131A04 Example

Demo project to show ADS131A04 capacity by using 4 independant delta-segma differentials input channels.

## Requirements

### Hardware requirements

The following boards are required:

- *One Zest Core STM32L4A6RG*
- *One Zest Sensor 4-20mA*

### Software requirements

Texas Instruments ADS131A04 Example makes use of the following libraries (automatically
imported by `mbed deploy` or `mbed import`):

- *[texas-instruments-ads131a04](https://gitlab.com/catie_6tron/texas-instruments-ads131a04) library*

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

Enable the custom target (by default `ZEST_CORE_STM32L4A6RG`):

```shell
cp zest-core-stm32l4a6rg/custom_targets.json .
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

Additionnaly, you can generate CMAKE files for IDE like Clion:

```shell
mbed export -i cmake_gcc_arm
```

Debug on the target device with a debug probe, eg. Segger J-Link and
[Ozone](https://www.segger.com/products/development-tools/ozone-j-link-debugger)
software.
