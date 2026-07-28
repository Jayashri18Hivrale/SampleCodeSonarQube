
# netX Peripherals Driver

## Introduction
This is the driver library for the netX microcontroller application side. This page provides an overview of the driver, its structure, and usage. The target IDE for this driver is **netX Studio**, uses the **CMSIS** component.

## Features and Requirements
The driver layer provides a simple API for applications to interact with available devices. Two types of drivers are provided:
- Drivers developed based on CMSIS recommendations.
- Drivers developed for verification purposes, providing a hardware-near abstraction layer.

The driver has the following features:
- No dynamic memory allocation.
- Consistent API look and feel.
- Performant and maintainable code.
- Drivers return elements of a state enumeration.
- Follows the CMSIS standard with minimal changes.
- A driver pack containing the flasher and SVD support will be available.
- Follows object-oriented C principles.

### Key Requirements
- Supports operating systems and reentrant functions.
- Supports concurrent access with locking mechanisms.
- Contains debug tracing functionality.
- Supports IRQ, DMAC, and polling programming models.
- Configured via a configuration object that cannot be modified externally.
- Drivers context objects/handles cannot be modified externally.
- Blocking calls implement timeouts and provide sleep/rescheduling options.
- Provides callbacks and proper DeInit functionality.

## Supported Devices
- **DIO** - Digital Input and Output devices (MMIO, HIF, PIO, GPIO, BOD)
- **MLED** - Multiple Light Emitting Diode device
- **Timer** - Timer devices (GPIO PWM, Blink, ARM Timer, System Tick, Systime)
- **ADC** - Analog Digital Converter peripheral (Temperature sensor)
- **UART** - Universal Asynchronous Receiver Transmitter
- **SPI** - Serial Peripheral Interface
- **I2C** - Inter-Integrated Circuit
- **DMAC** - Direct Memory Access Controller
- **TRACE** - Fast tracing module

Legacy driver support:
- **BiSS** - Bidirectional Serial Synchron Interface
- **CAN** - Controller Area Network
- **XPIC Ethernet MAC** - Driver for xPIC Ethernet MAC

## Driver Structure
The driver structure is organized into layers, with an example device driver "DEV" discussed to demonstrate the structure.

### Operation Modes
Devices handle communication and data streams. Data and control flows are separated, and data is transported using DMA or flush routines, while control is managed through API functions.

### Layers
The driver operates on a layered model with applications at the top and hardware peripherals at the bottom.

## OS Abstraction Layer
The OS abstraction layer is implemented through a locking mechanism to prevent multiple callers from using the same context.

## Configuration of the API

### Create required **netx_drv_configuration_sdk** component 

The driver API is configured via the `netx_drv_user_conf.h` file, derived from the template `netx_drv_user_conf_template.h`. This file manages inclusion/exclusion of components and OS-specific parts. This configuration file should be placed in a folder of the application project and must be passed to the API as the **netx_drv_configuration_sdk**. The following Waf build script should be used to define the required SDK component:

```python
def build(bld):
    
    includes = ["."]  # Ensure the configuration header file is in the current directory
    
    bld.sdkcomponent(
        name            = "netx_drv_configuration_sdk",  # Configuration SDK for the drivers
        export_includes = includes,                      # Export the include folder
    )
```

### Ensure `SystemCoreClock` is defined

The `SystemCoreClock` variable is essential for the correct operation of the driver API. It should be defined in a application project source file and must reflect the processor's clock speed in Hz. This variable it is mostlikely defined in system_init function at the very early stages of the microcontroller initialization process/code.

## Usage of the API
To utilize the specific device definitions and API in your module or application, you need to include the **netx_drv_sdk** component in the **waf** build script for the respective module or application.

In the build script (waf file), include the following:

```python
def build(bld): 
    bld.sdkcomponent(
        name = "Your_Module_Name",
        use  = ["netx_drv_sdk"], 
    )
```
Once the **netx_drv_sdk** component is included, you can access the device definitions and functions functionalities by including the appropriate header files in your source code. The API consists of core functions such as `Init`, `DeInit`, `Transmit`, `Receive`, and `GetState`. Here is an example of SPI device usage:

```c
#include "netx_drv_spi.h"

typedef DRV_SPI_HANDLE_T;
typedef DRV_STATUS_E;

DRV_STATUS_E DRV_SPI_Init(DRV_SPI_HANDLE_T * const ptHandle);
DRV_STATUS_E DRV_SPI_TransmitReceive(DRV_SPI_HANDLE_T * const ptHandle, uint8_t* pcData, size_t size);
```

### Error Handling
Example error handling in driver interaction:

```c
DRV_STATUS_E eReturn;
if(DRV_OK != (eReturn = DRV_SPI_Init(&tSPI))) {
  // Error handling code here
}
```

## Conventions
Naming conventions for files, devices, enumerations, and structures follow a strict format, ensuring consistency across the codebase.

## Memory Model
Drivers allocate context handles on the stack and do not allocate memory internally. The API is protected with an OS-free mutex implementation for locking, which can be replaced by OS-specific functions.

## Compiling the Driver
The driver should be compiled using CMSIS header files. Waf scripts are included for compilation.

## Changelog
### V0.1.3.0
- Added ADC descriptions and SPI fixes.
- Updated wscript and documentation.