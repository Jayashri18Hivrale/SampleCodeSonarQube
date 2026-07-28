/**
 * @file dPacketCommunication.h
 * @brief Interface for packet-based communication on netX90 channels.
 *
 * This module defines structures and functions to handle packet communication
 * through the CIFX toolkit. It provides interfaces for synchronous packet
 * sending and receiving, as well as mailbox polling and indication handling.
 */

#ifndef DPACKETCOMMUNICATION_H_
#define DPACKETCOMMUNICATION_H_

#include <stdint.h>
#include <stdbool.h>
#include "cifXUser.h"
#include "cifXErrors.h"

/**
 * @typedef indicationHandlerType
 * @brief Function pointer type for handling indication packets.
 *
 * @param ptPacket Pointer to the packet to be handled.
 * @param pvUserData Pointer to user data associated with the handler.
 * @return Boolean value indicating the success of handling.
 */
typedef bool (*indicationHandlerType)(CIFX_PACKET* ptPacket, void* pvUserData);

/**
 * @typedef pktProcessType
 * @brief Function pointer type for processing a packet.
 *
 * @param ptPacket Pointer to the packet to process.
 */
typedef void (*pktProcessType)(CIFX_PACKET* ptPacket);

/**
 * @struct Pkt_Comm
 * @brief Structure for managing packet communication on a channel.
 *
 * Holds data for managing a communication channel, including packet counts,
 * indication handlers, and queued unhandled packets.
 */
typedef struct
{
  CIFXHANDLE hChannel;                     /**< Channel handle for communication. */
  uint32_t s_ulNextPacketId;               /**< ID for the next packet to send. */
  uint32_t ulPacketSendCountSuccess;       /**< Count of successfully sent packets. */
  uint32_t ulPacketSendCountError;         /**< Count of packet send errors. */
  uint32_t ulPacketReceiveCount;           /**< Count of received packets. */
  indicationHandlerType fnIndHandler;      /**< Pointer to the indication handler function. */
  void * pvIndHandlerUserData;             /**< User data passed to the indication handler. */
  CIFX_PACKET unHandledPktQue[5];          /**< Queue for unhandled packets. */
  bool unHandledPktQueStatus[5];           /**< Status flags for unhandled packet queue. */
  pktProcessType setSucessHandler;         /**< Handler for successful packet set operations. */
  pktProcessType setFailHandler;           /**< Handler for failed packet set operations. */
  pktProcessType getSucessHandler;         /**< Handler for successful packet get operations. */
  pktProcessType getFailHandler;           /**< Handler for failed packet get operations. */
} Pkt_Comm;

/**
 * @struct Pkt_Comm_config
 * @brief Configuration structure for packet communication.
 *
 * Holds channel handle and function pointers for indication and process handlers.
 */
typedef struct
{
  CIFXHANDLE hChannel;                     /**< Channel handle for communication. */
  indicationHandlerType fnIndHandler;      /**< Pointer to the indication handler function. */
  void * pvIndHandlerUserData;             /**< User data passed to the indication handler. */
  pktProcessType setSucessHandler;         /**< Handler for successful packet set operations. */
  pktProcessType setFailHandler;           /**< Handler for failed packet set operations. */
  pktProcessType getSucessHandler;         /**< Handler for successful packet get operations. */
  pktProcessType getFailHandler;           /**< Handler for failed packet get operations. */
} Pkt_Comm_config;

/**
 * @brief Initializes packet communication on a given channel.
 *
 * Allocates memory and sets up the packet handling environment for the specified channel.
 *
 * @param me Pointer to a `Pkt_Comm` structure for communication management.
 * @param conf Pointer to a `Pkt_Comm_config` structure with initial configuration settings.
 */
void dPkt_Init(Pkt_Comm * me, Pkt_Comm_config * conf);

/**
 * @brief Sends a packet via the specified channel.
 *
 * Sends a packet through the provided `Pkt_Comm` instance and channel handle.
 *
 * @param me Pointer to the `Pkt_Comm` structure managing the channel.
 * @param ptPacket Pointer to the packet to send.
 * @return Status code of the operation (CIFX_NO_ERROR on success).
 */
uint32_t dPkt_SendPacket(Pkt_Comm * me, CIFX_PACKET* ptPacket);

/**
 * @brief Receives a packet from the channel mailbox.
 *
 * Retrieves an incoming packet from the mailbox of the specified channel.
 *
 * @param me Pointer to the `Pkt_Comm` structure managing the channel.
 * @param ptPacket Pointer to store the received packet if successful.
 * @return Status code of the operation (CIFX_NO_ERROR on success).
 */
uint32_t dPkt_ReceivePacket(Pkt_Comm * me, CIFX_PACKET* ptPacket);

/**
 * @brief Sends and receives a packet synchronously.
 *
 * Sends a request packet and waits for the corresponding confirmation packet.
 * Handles indication packets and unexpected confirmation packets while waiting.
 *
 * @param me Pointer to the `Pkt_Comm` structure managing the channel.
 * @param ptPacket Pointer to the request packet, also used to store the confirmation.
 * @return Status code of the send/receive operation (CIFX_NO_ERROR on success).
 */
uint32_t dPkt_SendReceivePacket(Pkt_Comm * me, CIFX_PACKET* ptPacket);

/**
 * @brief Polls the mailbox for packets and processes them using the indication handler.
 *
 * This function retrieves packets from the mailbox and passes them to the
 * registered indication handler. Unhandled packets are returned with an error status.
 *
 * @param me Pointer to the `Pkt_Comm` structure managing the channel.
 * @param ptPacket Pointer to the packet resource used for polling.
 * @return Status code of the operation (CIFX_NO_ERROR on success).
 */
uint32_t dPkt_CheckReceiveMailbox(Pkt_Comm * me, CIFX_PACKET* ptPacket);

#endif /* DPACKETCOMMUNICATION_H_ */

