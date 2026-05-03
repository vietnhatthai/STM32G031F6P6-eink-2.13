# STM32G031F6P6-eink-2.13

This project implements a **Smart Baggage Tag** using the STM32G031F6P6 microcontroller and a 2.13-inch e-ink display. It leverages the Adafruit GFX library for high-quality text and graphics rendering.

## Features

- **Low Power Display**: Utilizes a 2.13" e-ink display for static information.
- **Rich Graphics**: Integrated with `Adafruit_GFX` for flexible font and shape rendering.
- **Baggage Tag UI**: Specifically designed to display flight numbers, destinations, and priority status.

## Hardware Requirements

- **MCU**: STM32G031F6P6
- **Display**: 2.13" E-Ink Display (SPI interface)

## Software Architecture

The project is structured as follows:
- `Core/`: Main application logic and STM32 HAL configuration.
- `Components/eink/`: Low-level driver for the e-ink panel.
- `Components/eink_gfx/`: Adaptation layer connecting the e-ink driver to the Adafruit GFX library.
- `Components/Adafruit_GFX/`: The core Adafruit GFX library for graphics primitives.

## Build and Flash

### Prerequisites
- `arm-none-eabi-gcc` toolchain
- `CMake`
- `Ninja` build system

### Build Instructions
The project uses CMake. To build the project:
```bash
mkdir build
cd build
cmake ..
ninja
```

## Resource Usage

Based on the latest build:
- **RAM**: ~3.3 KB (40.53% used)
- **FLASH**: ~16.3 KB (49.83% used)

## License
Refer to the LICENSE files in the respective component directories for detailed licensing information.
