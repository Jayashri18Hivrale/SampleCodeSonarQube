/**
 * @file nx90_socket.h
 * @brief Socket handling API for netX90 processors.
 *
 * This header provides APIs for creating and managing UDP sockets on
 * the netX90 platform, supporting data transmission and reception.
 *
 * @date November 12, 2024
 * @author A0084817
 */
#ifndef NX90_SOCKET_H_
#define NX90_SOCKET_H_

#include "nx90Cifx.h"
#include "SOCK_API.h"

/** @brief Maximum number of sockets supported. */
#define NX90_MAX_SOCKETS 4

/** @brief Socket handle type for managing UDP sockets. */
typedef uint32_t* nx90_hSocket;

/**
 * @brief Data structure for managing UDP socket data.
 */
typedef struct
{
  uint32_t ip;        /**< IP address in network byte order. */
  uint16_t port;      /**< Port number. */
  uint32_t length;    /**< Length of the data buffer. */
  uint8_t* data;      /**< Pointer to the data buffer. */
} nx90Socket_data_t;

/**
 * @brief Callback type for received data events.
 *
 * This callback is invoked whenever data is received on the socket.
 *
 * @param socket Handle of the socket receiving the data.
 * @param data Pointer to the structure containing received data.
 */
typedef void (*socketData)(nx90_hSocket socket, nx90Socket_data_t * data);

/**
 * @brief Initializes the socket handling module.
 *
 * This function sets up the necessary environment for socket handling,
 * including registering the application with the system and initializing
 * the packet handler.
 *
 * @param initializedChannelHandler A valid CIFX channel handler.
 * @return 0 on success, or a negative error code on failure.
 */
int32_t nx90Socket_Init(CIFXHANDLE initializedChannelHandler);

/**
 * @brief Creates a generic UDP socket.
 *
 * This function creates a UDP socket. It is mainly used for client
 * purposes, since port is automatically assigned.
 *
 * @return Handle to the created socket, or NULL on failure.
 */
nx90_hSocket nx90Socket_CreateUdpSocket();

/**
 * @brief Creates a UDP server socket.
 *
 * This function creates a UDP socket bound to the specified port. Incoming
 * data is handled via the provided callback function.
 *
 * @param port Port number to bind the socket to.
 * @param length Maximum length of data to be received.
 * @param receivedCallback Callback function for received data events.
 * @return Handle to the created socket, or NULL on failure.
 */
nx90_hSocket nx90Socket_CreateUDPServerSocket(uint16_t port, uint16_t maxDataLength, socketData receivedCallback);

/**
 * @brief Sends data over the specified socket.
 *
 * This function transmits data to the specified destination using the provided socket.
 *
 * @param socket Handle to the socket used for transmission.
 * @param sentData Pointer to the data structure containing transmission details.
 * @return 0 on success, or a negative error code on failure.
 */
int32_t nx90Socket_SendData(nx90_hSocket  socket, nx90Socket_data_t * sentData);

/**
 * @brief Processes socket-related tasks.
 *
 * This function should be called periodically in the main loop to handle socket communication
 * tasks, such as processing received packets and triggering callbacks.
 *
 * @return 0 on success, or a negative error code on failure.
 */
int32_t nx90Socket_Tasks();

#endif /* NX90_SOCKET_H_ */
