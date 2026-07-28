/**
 * @file nx90_SystemPackets.h
 * @brief System packet assembly and management for the netX90 platform.
 *
 * This module provides functions to assemble, send, and handle various
 * system-level packets on the netX90 platform. It includes packet structures
 * for system initialization, configuration, MAC address setup, firmware identification,
 * and other control tasks.
 *
 * @note This demo software is intended for evaluation in a laboratory environment only.
 * It is provided without any warranty. Please refer to README_DISCLAIMER.txt for full
 * liability and usage information.
 */

#ifndef NX90_SYSTEMPACKETS_H_
#define NX90_SYSTEMPACKETS_H_

#include <stdint.h>
#include <stdbool.h>

#include "dPacketCommunication.h"
#include "cifXUser.h"
#include "cifXErrors.h"

/**
 * @brief Initializes the communication channel on the netX90.
 *
 * Performs an initialization of the specified channel in an extended configuration
 * scenario. This may involve shutting down and restarting the stack to allow for
 * configuration from a defined state.
 *
 * @param hPkt Pointer to the `Pkt_Comm` structure managing the communication channel.
 * @return Status code of the operation (CIFX_NO_ERROR on success).
 */
uint32_t nx90_SysPkt_ChannelInit(Pkt_Comm * hPkt);

/**
 * @brief Deletes the stored configuration on the netX90.
 *
 * Deletes the stored configuration (if applicable), including any remanent data.
 *
 * @param hPkt Pointer to the `Pkt_Comm` structure managing the communication channel.
 * @return Status code of the operation (CIFX_NO_ERROR on success).
 */
uint32_t nx90_SysPkt_DeleteConfig(Pkt_Comm * hPkt);

/**
 * @brief Assembles a packet to register an application on the netX90.
 *
 * @param ptPkt Pointer to the `CIFX_PACKET` to use for assembling.
 */
void nx90_SysPkt_AssembleRegisterAppReq(CIFX_PACKET* ptPkt);

/**
 * @brief Assembles a packet to set the MAC address on the netX90.
 *
 * @param ptPkt Pointer to the `CIFX_PACKET` to use for assembling.
 * @param abMacAddr Pointer to the MAC address to set.
 */
void nx90_SysPkt_AssembleSetMacAddressReq(CIFX_PACKET* ptPkt, uint8_t* abMacAddr);

/**
 * @brief Assembles a channel initialization request packet.
 *
 * Initializes the channel by setting up a request packet with relevant parameters.
 *
 * @param ptPkt Pointer to the `CIFX_PACKET` to use for assembling.
 */
void nx90_SysPkt_AssembleChannelInitReq(CIFX_PACKET* ptPkt);

/**
 * @brief Assembles a packet to start or stop communication on the netX90.
 *
 * @param ptPkt Pointer to the `CIFX_PACKET` to use for assembling.
 * @param fStart Boolean flag indicating the desired communication state:
 *               - `true`: Start communication (BUS_ON).
 *               - `false`: Stop communication (BUS_OFF).
 */
void nx90_SysPkt_AssembleStartStopCommReq(CIFX_PACKET* ptPkt, bool fStart);

/**
 * @brief Assembles a packet to identify firmware on a specified channel.
 *
 * This packet is used to request firmware identification on a given channel.
 *
 * @param ptPkt Pointer to the `CIFX_PACKET` to use for assembling.
 * @param ulChannelId Channel ID, such as:
 *                    - SYSTEM_CHANNEL (0xFFFFFFFF)
 *                    - COMM_CHANNEL_0 (0x00000000)
 *                    - COMM_CHANNEL_1 (0x00000001)
 *                    - COMM_CHANNEL_2 (0x00000002)
 */
void nx90_SysPkt_AssembleFirmwareIdentifyReq(CIFX_PACKET* ptPkt, uint32_t ulChannelId);

/**
 * @brief Handles a firmware identification confirmation packet.
 *
 * @param ptPkt Pointer to the `CIFX_PACKET` containing the confirmation data.
 */
void nx90_SysPkt_HandleFirmwareIdentifyCnf(CIFX_PACKET* ptPkt);

/**
 * @brief Assembles a request packet for hardware information on the netX90.
 *
 * @param ptPkt Pointer to the `CIFX_PACKET` to use for assembling.
 */
void nx90_SysPkt_AssembleHardwareInfoReq(CIFX_PACKET* ptPkt);

/**
 * @brief Handles a hardware information confirmation packet.
 *
 * Processes the confirmation packet containing hardware details.
 *
 * @param ptHardwareInfoCnf Pointer to the `CIFX_PACKET` with hardware information.
 */
void nx90_SysPkt_HandleHardwareInfoCnf(CIFX_PACKET* ptHardwareInfoCnf);

/**
 * @brief Handles a link status change indication packet.
 *
 * Processes the link status change for communication channels, including speed,
 * duplex, and link status on specified ports.
 *
 * @param hPkt Pointer to the `Pkt_Comm` structure managing the communication channel.
 * @param ptPacket Pointer to the `CIFX_PACKET` with link status data.
 * @return Status code of the operation (CIFX_NO_ERROR on success).
 */
int32_t nx90_SysPkt_HandleLinkStatusChangeInd(Pkt_Comm * hPkt, CIFX_PACKET* ptPacket);

/**
 * @brief Assembles a firmware reset request packet.
 *
 * Triggers a firmware update, restart, or cold start on the netX90.
 *
 * @param ptPkt Pointer to the `CIFX_PACKET` to use for assembling.
 * @param bResetMode Reset mode, e.g.:
 *                   - 0: Coldstart
 *                   - 1: Warmstart (unused)
 *                   - 2: Bootstart
 *                   - 3: Updatestart
 * @param bResetParam Arguments for reset, defining the variant to install in update mode.
 * @param bDeleteRemanentData Flag indicating whether to delete the remanent data area.
 */
void nx90_SysPkt_AssembleFirmwareResetReq(CIFX_PACKET* ptPkt, uint8_t bResetMode, uint8_t bResetParam, uint8_t bDeleteRemanentData);

#endif /** NX90_SYSTEMPACKETS_H_ */
