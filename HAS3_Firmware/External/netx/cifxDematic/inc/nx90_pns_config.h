/**
 * @file nx90_pns_config.h
 * @brief Configuration interface for setting up the PROFINET stack on netX90.
 *
 * This header defines structures and functions required to initialize and
 * configure the PROFINET stack on the netX90 platform. It includes data structures
 * to specify configurations for APIs, submodules, and various network settings.
 *
 * Key features include:
 * - Initialization of the `PNS_CONFIGURATION_DATA_T` structure with proper
 *   data arrangements for APIs and submodules.
 * - Functions to set system parameters, device identification, and network settings.
 * - Macros for defining limits on the number of APIs and submodules.
 *
 * This module is used in conjunction with the `NETX90_PNS` module to fully configure
 * and manage the PROFINET communication stack on the netX90 platform.
 *
 * @note This file was created on Oct 28, 2024 by A0084817.
 */

#ifndef _NX90_PNS_CONFIG_H_
#define _NX90_PNS_CONFIG_H_

#include <stdint.h>
#include "PNSIF_API.h"
#include "nx90Cifx.h"

#define PNS_DAP_SUBMODULES 4
#define PNS_SUBMODULES PNS_DAP_SUBMODULES + PNS_INPUT_SUBMODULES + PNS_IN_OUT_SUBMODULES+PNS_OUTPUT_SUBMODULES

#define DEMATIC_VENDOR_ID 0x042B
#define HILSCHER_VENDOR_ID 0x011E

/**
 * @struct PNS_CONFIGURATION_DATA_T
 * @brief Configuration data for initializing the PROFINET stack.
 *
 * This structure holds configuration requests and arrays of API and submodule structures,
 * which define the PROFINET stack's setup and behavior.
 */
typedef struct {
    PNS_IF_SET_CONFIGURATION_REQUEST_DATA_T tSetConfigReq;      /**< Configuration request structure. */
    PNS_IF_API_STRUCT_T tAPIs[PNS_APIS];                        /**< Array of API structures. */
    PNS_IF_SUBMODULE_STRUCT_T tSubmodules[PNS_SUBMODULES];      /**< Array of submodule structures. */
} PNS_CONFIGURATION_DATA_T;

typedef struct {
    uint32_t totalSubmodules;
    uint32_t * Offsets;
    uint32_t * Lengths;
} PNS_Cyclic_Offsets_t;


/**
 * @brief Initializes the PROFINET configuration data structure.
 *
 * Configures the data structure for the specified number of APIs and submodules,
 * defaulting to initial values for four submodules within the first API.
 *
 * @param num_apis Number of APIs to initialize (up to MAX_APIS).
 * @param num_submodules Array defining the number of submodules for each API.
 * @return Pointer to the statically allocated configuration data or NULL on failure.
 */
PNS_CONFIGURATION_DATA_T * PNSC_InitData(const uint32_t num_apis, const uint32_t *num_submodules, uint32_t dapModuleId) ;

/**
 * @brief Configures the PROFINET stack with initialized settings.
 *
 * Applies the configuration settings to the PROFINET stack using an initialized `N90Cifx_t` structure.
 *
 * @param hInitializedN90 Pointer to the initialized `N90Cifx_t` structure.
 * @param confData Pointer to the `PNS_CONFIGURATION_DATA_T` configuration data.
 * @return int32_t Status code (0 for success, error otherwise).
 */
int32_t PNSC_ConfigureStack(N90Cifx_t * hInitializedN90, PNS_CONFIGURATION_DATA_T * confData);

/**
 * @brief Adds a cyclic input submodule to a specified API.
 *
 * Adds a cyclic input submodule, ensuring it doesn't exceed the API's configured limit.
 *
 * @param apiIndex The API index to which the input submodule is added.
 * @param provider_data_length Data length supplied by the provider (device to controller).
 * @return int32_t Status code (0 for success, error otherwise).
 */
int32_t PNSC_AddCyclicInputSubmodule(uint32_t apiIndex, uint32_t provider_data_length);

/**
 * @brief Adds a cyclic output submodule to a specified API.
 *
 * Adds a cyclic output submodule, ensuring it doesn't exceed the API's configured limit.
 *
 * @param apiIndex The API index to which the output submodule is added.
 * @param consumer_data_length Data length for the consumer (controller to device).
 * @return int32_t Status code (0 for success, error otherwise).
 */
int32_t PNSC_AddCyclicOutputSubmodule(uint32_t apiIndex, uint32_t consumer_data_length);

int32_t PNSC_AddCyclicInputOutputSubmodule(uint32_t apiIndex,uint32_t input_data_length, uint32_t output_data_length);

/**
 * @brief Prepares to add new submodules by incrementing module ID and slot for a specified API.
 *
 * Increments the module ID and slot number, resetting submodule and subslot to 1.
 *
 * @param apiIndex Index of the API to increment the module and slot numbers for.
 * @return int32_t Status code (0 for success, error otherwise).
 */
int32_t PNSC_AddModule(uint32_t apiIndex);

/**
 * @brief Sets system behavior parameters like system flags and watchdog timer.
 *
 * @param ulSystemFlags Flags defining system behavior.
 * @param ulWdgTime Watchdog timer duration in milliseconds.
 * @return int32_t Status code (0 for success, error otherwise).
 */
int32_t PNSC_SetSystemParameters(uint32_t ulSystemFlags, uint32_t ulWdgTime);

/**
 * @brief Configures device ID for identification within the product line.
 *
 * @param ulDeviceId Unique device ID for identification.
 * @return int32_t Status code (0 for success, error otherwise).
 */
int32_t PNSC_SetDeviceIdentification(uint32_t vendorId, uint32_t ulDeviceId);

/**
 * @brief Sets the name, type, and station type of the device.
 *
 * @param abNameOfStation Pointer to station name string.
 * @param abTypeOfStation Pointer to station type string.
 * @param abDeviceType Pointer to device type string.
 * @return int32_t Status code (0 for success, error otherwise).
 */
int32_t PNSC_SetNameAndType(const uint8_t *abNameOfStation, const uint8_t *abTypeOfStation);

/**
 * @brief Configures the device's network settings, including IP, Netmask, and Gateway.
 *
 * @param ulIpAddr IPv4 address.
 * @param ulNetMask Subnet mask.
 * @param ulGateway Gateway IP address.
 * @return int32_t Status code (0 for success, error otherwise).
 */
int32_t PNSC_SetNetworkConfig(uint32_t ulIpAddr, uint32_t ulNetMask, uint32_t ulGateway);

/**
 * @brief Configures firmware version details for external visibility.
 *
 * Sets the firmware version as viewed by external systems without affecting internal tracking.
 *
 * @param usSwRevision1 Major firmware revision.
 * @param usSwRevision2 Minor firmware revision.
 * @param usSwRevision3 Build version.
 * @param bSwRevisionPrefix Prefix character for firmware version.
 * @return int32_t Status code (0 for success, error otherwise).
 */
int32_t PNSC_SetSoftwareVersionInfo(uint16_t usSwRevision1, uint16_t usSwRevision2, uint16_t usSwRevision3, uint8_t bSwRevisionPrefix);

PNS_Cyclic_Offsets_t * PNSC_GetInputCyclicOffsets();

PNS_Cyclic_Offsets_t * PNSC_GetOutputCyclicOffsets();

#endif /* _NX90_PNS_CONFIG_H_ */

