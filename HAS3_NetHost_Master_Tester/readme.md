# Hilscher Example Device Test Program

## Introduction

This document provides instructions for setting up and using the **HAS III - Test Program**.  
The program is designed to support the development and testing of **HAS III** devices for both **PROFIBUS** and **PROFINET**.  
The setup involves a **PROFIBUS Master** and a **PROFINET Controller**, each using their corresponding **NetHost** devices.  
The same test program can detect both **NetHost** devices on the network, allowing the user to select which device to interact with and specify whether to test a **PROFIBUS** or **PROFINET** HAS III device.

A general diagram of the test bench is shown below:

![Test Bench](./doc/images/Hilscher%20Device%20Example%20Test-Bench%20topology.drawio.png)

## Table of Contents

1. [Overview](#overview)  
2. [Hardware Setup](#hardware-setup)  
3. [Software Setup](#software-setup)  
4. [Running the Test Program](#running-the-test-program)  
5. [Troubleshooting](#troubleshooting)  

## Overview

### Purpose

The **HAS III Test Program** is used to validate fieldbus functionality of HAS III devices during the design and development phases of the project.  
A PC program can control both a **PROFIBUS DP Master** and a **PROFINET IO Controller**, allowing developers to test and monitor cyclic data for the HAS III firmware for **PB** (PROFIBUS) and **PN** (PROFINET) devices in a controlled environment.

### Supported Devices

- **NetHost**: NHST-T100-DP/DPM (PROFIBUS), NHST-T100-PNS (PROFINET)  
- **Device Under Test (DUT)**: HAS III (PROFIBUS) device, HAS III PN (PROFINET) device  

## Hardware Setup

### Required Equipment

- **PROFIBUS NetHost**: NHST-T100-DP/DPM for PROFIBUS  
- **PROFINET NetHost**: NHST-T100-EN/PNM for PROFINET  
- **Windows PC** with the following software installed:  
  - [SYCON](https://kionnam.sharepoint.com/:u:/r/sites/scs_CTGlobalCTOControlsProductDevelopment/Shared%20Documents/HAS/TG30/Test%20Bench%20Software/SYCON.net%20V1.0500.240823.49689%20Setup.zip?csf=1&web=1&e=ZxhaIY) for configuration  
  - [Visual Studio 2019](https://kionnam.sharepoint.com/:u:/r/sites/scs_CTGlobalCTOControlsProductDevelopment/Shared%20Documents/HAS/TG30/Test%20Bench%20Software/vs_community__4d2f5cc871a14ab0883094cfc000d434.exe?csf=1&web=1&e=oIZngp) for test program development  
  - [NetxStudio](https://kionnam.sharepoint.com/:u:/r/sites/scs_CTGlobalCTOControlsProductDevelopment/Shared%20Documents/HAS/TG30/Test%20Bench%20Software/netX%20Studio%20CDT%20V1.1020.1.6768%20x86%20Setup.msi?csf=1&web=1&e=nYmdJM) for device firmware development  
  - [netHost Test](https://kionnam.sharepoint.com/:u:/r/sites/scs_CTGlobalCTOControlsProductDevelopment/Shared%20Documents/HAS/TG30/Test%20Bench%20Software/netHOST%20Test.zip?csf=1&web=1&e=QGMGFK) to test connections in a known environment  
- **SD memory card**: For the initial setup of NetHosts  

### Connection

As shown in the diagram in the introduction.

### Setup

- **PROFIBUS Setup**:  
  - Ensure proper termination to prevent signal reflection issues.  

- **PROFINET Setup**:  
  - Ensure all devices are on the same network subnet.  
    - Example Setup:  
      - PC: 10.200.7.20  
      - PN NetHost: 10.200.7.50  
      - PB NetHost: 10.200.7.51  

#### NetHosts Initial Configuration

1. Set the NetHosts to their factory state using the SD card, refer to section 8.2 of the document [netHOST NHST-T100 LAN controlled master devices for Fieldbus and Real-Time Ethernet Systems](https://kionnam.sharepoint.com/:b:/r/sites/scs_CTGlobalCTOControlsProductDevelopment/Shared%20Documents/HAS/TG30/CommFirmwareDevelopment/Hilscher%20Docs/netHOST_NHST-T100_UM_05_EN.pdf?csf=1&web=1&e=SbyYnT).  
2. Verify that the PC and the NetHosts are on the same network range.  
3. Use the **nHost Test Program** to ensure each device is connected and recognized. Run the test program with administrator privileges.  
4. Use **SYCON** to download the configuration to each NetHost. The SYCON projects for each NetHost can be found in `\has-iii\HAS3_NetHost_Master_Tester\SyconConf`.  
5. Confirm that the configuration is correct by connecting to and diagnosing each NetHost.  
6. After verification, disconnect and close SYCON.  

## Running the Test Program

### Initial Setup

1. Open the solution in **Visual Studio** and compile it. Ensure that the build configuration is set to **x64**.  
2. On the first run, two files will be missing from the `Debug/x64` folder:  
   - `netXTransport.lib`  
   - `netXTransport.dll`  
   These files are located in `\has-iii\HAS3_NetHost_Master_Tester\netXTransportDLL`. Copy them to the `Debug/x64` folder and recompile.  
3. Recompile the project and verify that there are no compilation errors.  

### Running the Program

1. Launch the test program. It will automatically detect any properly connected NetHost devices.  
2. Once launched, the program **automatically displays a main menu** showing available commands.  

### Menu Overview
1. **Run Cyclic Test** (`Press '1'`)  
   Executes the cyclic testing routine on the currently selected Profinet NetHost and displays progress or status information.
2. **Show Slave Information** (`Press '2'`)  
   Displays data about any connected slaves (e.g., device addresses, statuses).
3. **Other Tests** (`Press '3'`)  
   Reserved for additional tests (currently not implemented).
4. **Quit** (`Press 'Q'`)  
   Exits the program and performs any necessary cleanup operations.

### Usage Notes
- **Navigation**  
  - After choosing an option, the system may clear the screen and show relevant output or logs.  
  - When the command completes, the menu will be displayed again, allowing you to run further tests or exit.
- **Real-Time Updates**  
  - The program regularly checks for user input. As soon as you press a valid key, it immediately processes your choice.
- **Exiting**  
  - Press `Q` to **quit the program** at any time. This ensures all connections are properly closed before terminating.

By following these steps, you can easily select specific operations to perform on the NetHost or retrieve information about connected devices without having to focus on the internal testing details.


## Tests Descriptions


### 1. Cyclic Speed Test

#### Objective
Confirm that cyclic data is transferred between the Profinet master (*netHost*) and the slave device at the required minimum cyclic rate (2 ms). The primary goal is to verify that the slave can handle such fast cyclic updates.

#### Restrictions
Because the Profinet master (*netHost*) cannot synchronize with the test application at cyclic speeds below 2 ms, it is impossible to observe each cyclic transaction in real time. To overcome this limitation, the test uses timestamp deltas to indirectly verify the cyclic rate.

#### Test Principle

1. **Timestamp Injection**  
   The test application (PC) writes timestamps to the *netHost*'s Dual-Ported Memory (DPM) at its own pace, taking about 1.5 ms per write. Because of this write time, the minimum achievable cyclic time becomes 2 ms.

2. **Cyclic Forwarding**  
   The *netHost* forwards these timestamps to the slave at the configured cyclic rate (2 - 4 -8 -16 ms).

3. **Change Detection**  
   The slave device saves the timestamps in its internal RAM **only when they change**, effectively counting how many unique timestamps it has received over the test period.

4. **Cyclic Time Measurement**  
   After the transfer ends, the number of unique timestamps recorded on the slave is retrieved by the test application. The effective cycle time is then calculated by dividing the total duration of the test by the number of unique timestamps.