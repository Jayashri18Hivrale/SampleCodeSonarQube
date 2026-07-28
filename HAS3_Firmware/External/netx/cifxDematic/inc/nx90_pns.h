/**
 * @file NETX90_PNS.h
 * @brief Interface for initializing and configuring the PROFINET stack on netX90.
 *
 * This module provides functions to initialize, configure, and manage
 * communication tasks for the PROFINET stack on the netX90 platform.
 * It also supports registration of callbacks to handle specific PROFINET
 * indications, allowing for flexible and selective event handling.
 */

#ifndef NETX90_PNS_H_
#define NETX90_PNS_H_

#include "nx90Cifx.h"
#include "nx90_pns_config.h"

/**
 * @typedef cyclicDataReceived
 * @brief Type definition for callback functions that handle cyclic data.
 *
 * @param[in] submodule   Identifier of the submodule providing the data.
 * @param[in] data        Pointer to the data received in cyclic communication.
 * @param[in] dataLength  Length of the received data.
 * @param[in] userData    Pointer to user-defined data provided during configuration.
 */
typedef void (*cyclicDataReceived)(uint32_t submodule, uint8_t * data, uint32_t dataLength, void * userData);
typedef void (*pFuDebugCB)(uint8_t * data, uint32_t dataLength);

/**
 * @typedef indicationCallback
 * @brief Type definition for callback functions that handle PROFINET indications.
 *
 * Each callback function processes an indication command and returns a `CIFX_PACKET *`
 * that contains the response to be sent back. The `ulCmd` field in the returned packet
 * must be properly set with the original command (with the response flag, if applicable).
 *
 * If the callback does not set `ulCmd` correctly, it is treated as improper handling
 * and the system will send a default response indicating "No Application Registered."
 *
 * @param[in] indicationCommandData Pointer to the command data received for this indication.
 *                                  The data should be cast to the specific type expected by each indication.
 * @param[in] userData              Pointer to user-defined data, as registered with the callback.
 *
 * @return Pointer to a `CIFX_PACKET` containing the properly prepared response.
 *         If no response is to be sent, the callback may return `NULL`.
 */
typedef CIFX_PACKET * (*indicationCallback)(void * indicationCommandData, void * userData);

/**
 * @struct nx90PNS_config_t
 * @brief Configuration structure for the netX90 PROFINET stack.
 *
 * This structure holds the cyclic data callbacks and user data for input and output data handling.
 */
typedef struct
{
  cyclicDataReceived pfCyclicInputdata;     /**< Callback for handling cyclic input data. */
  void * pCyclicInputUserData;              /**< User data for the input data callback. */
  cyclicDataReceived pfCyclicOutputdata;    /**< Callback for handling cyclic output data. */
  void * pCyclicOutputUserData;             /**< User data for the output data callback. */
  pktProcessType SendSucessCB;
  pktProcessType RecSucessCB;
  pFuDebugCB debugCB;
} nx90PNS_config_t;

/**
 * @brief Initializes the netX90 PROFINET stack.
 *
 * This function initializes the PROFINET stack by binding it to an existing,
 * initialized `N90Cifx_t` instance. It sets up the CIFX handles and ensures
 * that the stack is ready for further communication tasks.
 *
 * @param[in] hInitializedN90Cifx Pointer to an initialized `N90Cifx_t` structure,
 *                                which provides CIFX handles and necessary setup data.
 * @param[in] conf                Pointer to an `nx90PNS_config_t` structure containing
 *                                cyclic data handling callbacks and user data.
 */
void nx90PNS_Init(N90Cifx_t * hInitializedN90Cifx, nx90PNS_config_t * conf);

/**
 * @brief Registers a callback function for handling a specific PROFINET indication.
 *
 * This function allows the application to register a callback to handle a particular
 * indication, such as `PNS_IF_SAVE_STATION_NAME_IND`. The callback is identified by
 * `indicationId` and receives indication-specific data as well as user data.
 *
 * If an indication with the same ID is already registered, this function overwrites it
 * with the new callback and user data.
 *
 * @param indicationId  The indication ID to register (from `PNS_IF_CMD_E`).
 * @param callback      Pointer to the callback function for handling the indication.
 *                      This callback should set the command and response in the returned
 *                      `CIFX_PACKET *` as described in the `indicationCallback` typedef.
 * @param userData      Pointer to user-defined data passed to the callback.
 *
 * @return 0 on successful registration, -1 if registration fails (e.g., indication list is full).
 */
int32_t nx90PNS_RegisterIndicationCallback(PNS_IF_CMD_E indicationId, indicationCallback callback, void * userData);

/**
 * @brief Executes periodic communication tasks for the PROFINET stack.
 *
 * This function should be called periodically to handle ongoing communication
 * tasks and to process incoming and outgoing packets for the PROFINET stack.
 *
 * @return Status code of the communication tasks, indicating success or specific errors.
 *         Refer to the PROFINET IO-Device API documentation for a list of possible status codes.
 */
int32_t nx90PNS_CommStackTasks(void);

/**
 * @brief Executes periodic cyclic tasks for the PROFINET stack.
 *
 * This function processes the cyclic communication tasks in the PROFINET stack,
 * including handling input and output data exchanges with connected devices.
 *
 * @return Status code indicating success or specific errors encountered during cyclic tasks.
 */
int32_t nx90PNS_CyclicTasks(void);

/**
 * @brief Configures the PROFINET stack with provided configuration data.
 *
 * This function applies the specified configuration settings to the PROFINET stack.
 * The configuration includes essential parameters like vendor ID, device ID,
 * and module configuration, which are encapsulated in the `PNS_CONFIGURATION_DATA_T` structure.
 *
 * @param[in] confData Pointer to a `PNS_CONFIGURATION_DATA_T` structure that contains
 *                     configuration parameters for the PROFINET stack. This structure
 *                     defines the device settings, such as station name and network parameters.
 *
 * @return Status code of the configuration process, indicating success or specific errors.
 *         Possible errors include invalid configuration data or network-related issues.
 *
 * @see PNS_CONFIGURATION_DATA_T for detailed configuration structure definition.
 */
int32_t nx90PNS_ConfigureStack(PNS_CONFIGURATION_DATA_T * confData);

/**
 * @brief Retrieves the configured station name of the PROFINET stack.
 *
 * This function provides the current station name of the PROFINET device,
 * which can be used to identify the device on the network.
 *
 * @param[out] name Pointer to a character buffer where the station name will be copied.
 *                  The buffer should be large enough to hold the station name string.
 */
void nex90PNS_GetStationName(char * name);

#endif /* NETX90_PNS_H_ */
