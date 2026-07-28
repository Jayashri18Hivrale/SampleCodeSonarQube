# Introduction
This example is based on the Hilscher-provided ProfiBus and ProfiNet firmware stacks (LFW).
Some modifications have been made in order to leave a project example that can be used to create a firware that works with both protocols.

Use case A is used and according matching Hardware Configuration files (HWC `*.hwc`), Flash Device Label file (FDL `*.fdl`), and Maintenance Firmware (MTF) file are available in the example.
Those must be be programmed into the internal flash memory.

## Common Functions Supported by the Firmware:
- **netX Diagnostic and Remote Access**:  
  Interface for Hilscher diagnosis and configuration tools via Serial Line (UART) on netX90 / FTDI USB bridge on NXHX90.
  
- **DPM Channel 0**:  
  Real-Time Ethernet (RTE) Protocol-Specific and FW Generic Services.
  
- **DPM Channel 1**:  
  Network Services included in profinet firmware variant, providing:
    - **Socket Interface API**:  
      Allows socket communication via the netX integrated TCP/IP stack using the same MAC address.
    - **Ethernet Interface API**:  
      Allows sending/receiving raw Ethernet frames using a dedicated MAC address (disabled by default, can be enabled via Firmware TagListEditor).
    - **Web Interface API**:  
      Allows forwarding specific URL requests (HTTP GET/POST) to the user application for building custom web page content.

- **Integrated Webserver**:  
  - ** Basic Web Server**:  
    Included in profinet firmware variant, providing the following:
      - `URL\netx`: Provides a graphical interface to other functions (diagnostics, firmware update, reset).
      - `URL\netx\diag`: Displays basic information about the netX device (uptime, MAC address, etc.).
      - `URL\netx\firmware`: Allows firmware upload to netX flash.
      - `URL\netx\reset`: Triggers a netX reset (to install new firmware).
      - `URL\`: Forwards URL requests to the user application via DPM Channel 1 (Web Interface API).

The file `netx90_app_iflash_dummy.nai` is a dummy file to be flashed on the app iflash when using `netx90_app_usecase_X_sdram.elf` for debugging, instead of `netx90_app_usecase_X_iflash.nai`.

# Protocol Specific Part
The protocol-specific part is covered by the component `cifXApplicationdemoXX`, where `XX` is a placeholder for different protocols, PROFINET slave (PN), ProfiBus slave (PB).

- Protocol-specific startup begins in the function `Protocol_StartConfiguration()`.
- All responses and indications are handled in `Protocol_PacketHandler()`.


# Loading Firmware in Netx90 Dev Board.

- Open Flasher in NetX Dev Studio
- Connect JTAG 
- Unprotect flash areas
- Erase Mode 
- Erase all internal flash
  - Internal Flash 01 (COM)
  - Internal Flash 02 (AP)
- Make sure FDL is not detected
- Write mode
- Load files
  - FDL (same for all)
  - HWC (Select depending on dev board and protocol)
  - LWF (Select depending on protocol)
  - MFW (Same for all)
  - build/debug/Target (Select depending on protocol).
- Write
- Manual reset of development board
