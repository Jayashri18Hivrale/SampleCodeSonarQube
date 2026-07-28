# Separating Startup and CMSIS

The provided Hilscher template and examples tightly couple `CMSIS` and `netx_drv` components with the `<FirmwareApp>`. To achieve a fully modularized and unit-testable project, we’ll break this dependency, creating independent modules for `CMSIS` and `startup` configurations.

## Steps to Modularize Startup Code
### 1. Relocate Startup Files:

Move the following files from `CMSIS/Device/Hilscher/netx` to the `Target/<firmwareApp>/Startup` folder:
- startup_netx90_app.S
- system_netx.c
- system_netx.h

### 2. Update the Main Waf File (`<FirmwareApp>`):

In the main Waf build file for FirmwareApp, add the relocated source files and their configuration options to the build system.

Example configuration:

```python
sources = [
    "Source/handler.c",
    "Source/netx90_app_header.c",
    "Source/main.c",
    "firmware/startup/startup_netx90_app.S",
    "firmware/startup/system_netx.c",
]

includes = [
    "Include",
]

defines = [
    "__NETX90",
    "_NETX_",
    "__STARTUP_CLEAR_BSS",
    "__START = main",
    "__STACK_SIZE = 0x00002000",
    "__HEAP_SIZE = 0x00008000",
    # Optional defines (uncomment if needed):
    # "__GENERATE_DUMMY",
    # "SYSTEM_TEMPERATURE_FLOATING_POINT_SUPPORT",
    # "NVIC_SOFT_VECTOR",
]
```
### 3. Create a Startup SDK Component:

Create a new SDK component to manage startup dependencies and include paths, ensuring all startup includes are available across different parts of the application.

Example SDK definition:

```python
def build(bld):
    defines = [
        # Add any additional defines specific to the startup SDK here
    ]

    includes = [
        "firmware/startup",  # Path to the startup headers
    ]
    
    bld.sdkcomponent(
        name = "startup_sdk",
        export_includes = includes,
    )
```

### 4. Modify the `CMSIS` Waf File:

Update the CMSIS Waf file to define CMSIS solely as an SDK (not a library).
Ensure the CMSIS SDK depends on the startup_sdk to access all necessary startup configurations.

Example CMSIS SDK definition:

```python
bld.sdkcomponent(
    name = "CMSIS",
    uses = "startup_sdk",                # Links CMSIS with system_netx.h
    export_includes = includes,
)
```

### 5. Link SDK Components Where Required:

In any modules that depend on startup configurations or CMSIS, include startup_sdk and CMSIS as required dependencies. This setup ensures that configurations, includes, and defines are correctly propagated throughout the build. At least the `<FirmwareApp>` and the `netx_drv` needs to  use `system_netx.h` for which it will require including `startup_sdk` component.


# Flashing Netx90 using the provided stack webserver.

Netx90 processor offers a webserver with basic tasks to perform flashing task. 

The file to be flashed here a specific .zip file generated after each build. This file is generated on the `build` folder and it is named `fwupdate.zip`. In order to access the server, the programming computer needs to be connected to the same ethernet network of the device and the device i.p. address must be known. In order to access to the server just enter this ip address followed by `/netx`, example: `192.168.0.1/netx`.

In there it is straight forward to select the file and update the firmware.

