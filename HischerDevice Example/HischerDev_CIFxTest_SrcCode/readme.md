# Hilscher Example Device Test Program

## Introduction

This document provides instructions for setting up and using the Hilscher Example Test Program. This program is designed to support the development and testing of profibus/profinet devices by simulating a PROFIBUS Master and a PROFINET Controller using their corresponding NetHosts devices. The program is able to detect multiple two NetHost devices on the network, allowing the user to select which device to interact with and specify whether to test a PROFIBUS or PROFINET device.

## Table of Contents

1. [Overview](#overview)
2. [Hardware Setup](#hardware-setup)
3. [Software Setup](#software-setup)
4. [Running the Test Program](#running-the-test-program)
5. [Troubleshooting](#troubleshooting)

## Overview

### Purpose
The Hilscher Example Device Test Program is used to validate communication, protocol compliance, and overall functionality of Netx90 profibus and profinet devices. The program can simulate both a PROFIBUS DP Master and a PROFINET IO Controller, allowing developers to test/monitor cyclic data for the Hilscher Example firmware for PB (PROFIBUS) and PN (PROFINET) devices in a controlled environment.

### Supported Devices
- **NetHost**: NHST-T100-DP/DPM (PROFIBUS), NHST-T100-PNS (PROFINET)
- **Device Under Test (DUT)**: Hilscher Device Example PB (PROFIBUS) device, Hilscher Device Example PN (PROFINET) device

## Hardware Setup

### Required Equipment
- **ProfiBus NetHost**: NHST-T100-DP/DPM for PROFIBUS.
- **ProfiNet NetHost**: NHST-T100-EN/PNM for PROFINET.
- **Windows PC**: With SYCON.net installed for configuration, Visual Studio 2019 for test program development and an available Ethernet connection.
- **Cabling**: Ethernet cables for PC connection, PROFIBUS cables and terminators for PROFIBUS, Ethernet cables for PROFINET.
- **SD memory card**

### Wiring and Connections
- **PROFIBUS Setup**:
  - Connect the NHST-T100-DP/DPM to the PROFIBUS network.
  - Ensure correct termination to prevent signal reflection issues.
- **PROFINET Setup**:
  - Connect the NHST-T100-PNS to the PROFINET network using Ethernet cables.
  - Ensure all devices are on the same network subnet.
- **Masters Setup**
    - Connect the two NetHost to the windows ethernet network. Recommended Connection:
      1. PC to NHST-T100-DP/DPM (X2 Eth)
      2. NHST-T100-DP/DPM (X2 Eth) to X2 Eth.


## Software Setup

### Required Software
- **Test Program**: Hilscher Example Device Test Program (visual studio 2019).
- **SYCON.net**
- **NetHost Solutions DVD**: `Supplements & Examples
\netHOST Device Recovery\Recovery via Memory Card`

### Installation
- Set the NetHosts to its factory state using the SD card ( section 8.2 of document `netHOST NHST-T100
LAN controlled master devices for Fieldbus and Real-
Time Ethernet Systems`).
- Use SYCON.net to download the configuration to each NetHost. Sycon projects for each NetHost on `\has-iii\HischerDevice Example\HischerDev_CIFxTest_SrcCode\SyconConf` of this repo.
- Ensure the configuration is working properly by connecting and diagnosing each NetHost.
- Once verified, disconnect and close Sycon.net software

## Running the Test Program

### Initial Setup
- Launch the Test Program on your PC.
- The program will automatically detect the properly connected NetHost devices.
- A list of detected netHost, including their IP addresses and types (PROFIBUS or PROFINET), will be displayed.

### Device and Protocol Selection
- Select the NetHost device (Protocol) you want to interact with by entering the corresponding number from the list.

### Test Execution
- Once a netHost (protocol) are selected, the program will initiate communication with it and its connected HAS III device.
- Monitor real-time communication, logs and status indicators provided by the program.

## Troubleshooting

- **No Communication**: Verify the SYCON.net configuration and ensure the correct termination of the network for PROFIBUS or proper IP setup for PROFINET.
- **Unexpected Errors**: Check the configuration settings in SYCON.net and ensure all cables are correctly connected.