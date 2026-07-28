#ifndef FIELDBUS_MASTER_COMMON_H
#define FIELDBUS_MASTER_COMMON_H

#include <stdint.h>
#include "cifXUser.h"

// Parameters for querying different slave states
#define RCX_LIST_CONF_SLAVES                   0x00000001  // Configured slaves
#define RCX_LIST_ACTV_SLAVES                   0x00000002  // Active slaves
#define RCX_LIST_FAULTED_SLAVES                0x00000003  // Faulted slaves

// Struct to hold all parameters for FieldBusGetActiveSlaves
typedef struct FIELDBUS_CONNECTION_HANDLERS {
    CIFXHANDLE cifxDriver;            // Handle to the CIFX driver
    const char szBoardName[25];    // Board name (e.g., "cifX0")
    const char fwName[25];         // firmware description
}FIELDBUS_CONNECTION_HANDLERS;

typedef struct FIELDBUS_SLAVES_INFO_T {
    FIELDBUS_CONNECTION_HANDLERS* connectionHandler;
    uint32_t                        ulSlaveState;      // Slave state to query (configured, active, or faulted)
    uint32_t*                       pSlaveHandles;     // Pointer to store slave handles (dynamically allocated)
    int pNumSlaves;            // Pointer to store the number of connected slaves
} FIELDBUS_SLAVES_INFO_T;

// Function  
int32_t StartTcpTransportLayer();

void StopTcpTransportLayer();
// Function
int32_t GetCifxDriverAndRelatedInfo(CIFXHANDLE* cifxDriver, DRIVER_INFORMATION* tDriverInfo);

// Function
int32_t GetConnectedNetHostsBoardInformation(CIFXHANDLE cifxDriver, BOARD_INFORMATION** BoardsInfo, uint32_t nNetHosts);

// Function
int32_t GetConnectionHandlers(FIELDBUS_CONNECTION_HANDLERS** connHandlers, BOARD_INFORMATION** BoardsInfo, uint32_t nNetHosts);

// Function to retrieve connected slaves based on state using struct for inputs
int32_t FieldBusGetActiveSlaves(FIELDBUS_SLAVES_INFO_T* pSlaveInfo);

#endif // FIELDBUS_MASTER_COMMON_H
