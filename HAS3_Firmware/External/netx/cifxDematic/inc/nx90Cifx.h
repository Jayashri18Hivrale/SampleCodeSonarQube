/**
 * @file NETX90_CIFX.h
 * @brief Interface for initializing and managing communication on netX90
 * between the firmware application and communication stack.
 *
 * This module defines the structure and initialization function to set up
 * communication channels on netX90, gathering relevant device information
 * and providing a singleton instance for further module access.
 */

#ifndef NETX90_CIFX_H_
#define NETX90_CIFX_H_

#include <stdint.h>
#include "cifXToolkit.h"
#include "dPacketCommunication.h"

/** @def N90_NAME_MAX_LEN
 *  @brief Maximum length for the name identifier of the netX90 module.
 */
#define N90_NAME_MAX_LEN 12

/** @def N90_CHANNEL_COUNT
 *  @brief Defines the number of communication channels in the netX90 module.
 */
#define N90_CHANNEL_COUNT 3

/**
 * @struct N90Cifx_t
 * @brief Structure representing the netX90 communication interface.
 *
 * Contains essential fields for configuring, initializing, and managing
 * communication across three channels on the netX90, including handles for
 * each channel, driver and board information, and status tracking.
 */
typedef struct
{
  uint8_t name[N90_NAME_MAX_LEN];         /**< Module name for identification. */
  CIFXHANDLE cifxHandle;                  /**< Handle for CIFX toolkit operations. */
  DRIVER_INFORMATION cifxDriverInfo;      /**< Information about the CIFX driver. */
  BOARD_INFORMATION n90_iDPM_Info;        /**< Dual-Port Memory configuration information. */
  CIFXHANDLE n90Channels[N90_CHANNEL_COUNT]; /**< Array of handles for each of the communication channels. */
  CHANNEL_INFORMATION n90ChannelsInfo[N90_CHANNEL_COUNT]; /**< Channel-specific information for each channel. */
  int32_t cifxStatus;                     /**< Status of CIFX toolkit operations, error codes if any. */
} N90Cifx_t;

/**
 * @brief Initializes the netX90 CIFX module.
 *
 * This function initializes the netX90 communication stack, sets up the
 * dual-port memory interface, and configures three communication channels.
 *
 * @param[in] name A string identifier for the instance, assigned to the toolkit’s device instance.
 *                 This name helps identify the specific instance of the device and is added to the toolkit’s
 *                 handled device list for future reference.
 *
 * @return A pointer to an `N90Cifx_t` structure containing initialized module information,
 *         or `NULL` if initialization fails.
 *
 * @details The initialization process involves setting up CIFX driver information,
 * configuring the dual-port memory interface, and initializing three communication channels
 * with their specific configurations and status information.
 *
 * Example:
 * @code
 * N90Cifx_t* myNetX90Module = N90Cifx_Init("netX90CommModule");
 * if (myNetX90Module) {
 *     // Ready for further interaction with the communication stack
 * }
 * @endcode
 */
N90Cifx_t * N90Cifx_Init(const char * name);

#endif /* NETX90_CIFX_H_ */


