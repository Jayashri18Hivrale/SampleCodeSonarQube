
# CMSIS for NetX90 Firmware Development

## Overview

This document provides a guide on how to use the **CMSIS** (Cortex Microcontroller Software Interface Standard) register definitions for the **NetX90** processor in your firmware development. The CMSIS layer abstracts access to the processor’s peripherals and provides an efficient way to control hardware resources.

The NetX90 includes an ARM Cortex-M4 core, and CMSIS facilitates access to the registers and system-level configuration for this core, making it easier to develop firmware.

## Folder Structure

The relevant folder structure for CMSIS and NetX90 in this project is as follows:

```
Components/
    CMSIS/
        Device/
            Hilscher/
                netx/
                    Include/                # Header files for CMSIS and peripheral registers
                    regdef/                 # Register definitions
                    netx90_app.h            # Application-specific CMSIS configurations for NetX90
```

### Key Files

1. **Include/**: Contains header files for the CMSIS API, including core and peripheral register definitions for the NetX90.
2. **regdef/**: Register definition files for specific peripherals on the NetX90.
3. **netx90_app.h**: Defines application-specific constants and function declarations related to the NetX90. This file contains configurations and setups specific to your firmware application.

---

## Usage

### 1. Using the CMSIS for NetX90

To utilize the CMSIS register definitions and API in your module or application, you need to include the **CMSIS_sdk** component in the **waf** build script for the respective module or application.

In the build script (waf file), include the following:

```python
def build(bld): 
    bld.sdkcomponent(
        name = "Your_Module_Name",
        use  = ["CMSIS_sdk"],  # Ensure the CMSIS_sdk is included
    )
```

Once the **CMSIS_sdk** component is included, you can access the register definitions and NetX90-specific peripheral functionalities by including the appropriate header files in your source code. For example, to access NetX90 peripheral definitions, include the following:

```c
#include "netx90_app.h"
```

This allows you to use the CMSIS-defined structures and macros to work with the NetX90 peripherals in your firmware.


### 2. Register Definitions and Access

The CMSIS standard provides a unified way to access the NetX90’s registers through a set of pre-defined structures and macros. Register definitions are available in the `regdef/` directory and are used to access the peripherals on the NetX90.

Example of accessing a peripheral register (e.g., GPIO):

```c
#include "netx90_app.h"

// Set a specific bit in a GPIO register
GPIOx->BSRR = (1U << PIN_NUMBER);
```

This allows you to manipulate the registers directly through the CMSIS-provided macros, ensuring that you maintain compatibility with the ARM standard and NetX90 hardware.

### 3. Peripheral Initialization

Each peripheral on the NetX90 (e.g., UART, SPI, etc.) can be accessed and configured through CMSIS register definitions. Before using a peripheral, ensure that:
- The peripheral clock is enabled.
- The peripheral is properly configured (e.g., baud rate for UART, frequency for timers).

Example for UART initialization:

```c
#include "netx90_app.h"

// Enable clock for UART and configure settings
UARTx->CR1 |= (UART_CR1_TE | UART_CR1_RE);  // Enable transmit and receive
UARTx->BRR = UART_BAUD_RATE_CONFIG;         // Set baud rate
```
