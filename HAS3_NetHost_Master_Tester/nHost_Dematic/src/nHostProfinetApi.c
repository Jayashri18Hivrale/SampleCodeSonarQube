#include "nHostProfinetApi.h"

#include "cifXErrors.h"
#include "nHostCommonApi.h"


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int32_t NHsPNM_GetSlaveConnecInfo(CIFXHANDLE channel, uint32_t slaveHandle,
    PNM_AP_INACTIVE_SLAVE_CONNECT_INFO_T* inactiveInfo,
    PNM_AP_ACTIVE_SLAVE_CONNECT_INFO_T* activeInfo)
{
    CIFX_PACKET tSendPacket = { 0 };
    CIFX_PACKET tRecvPacket = { 0 };

    // Step 1: Prepare the request packet
    tSendPacket.tHeader.ulDest = RCX_DEFAULT_DESTINATION;
    tSendPacket.tHeader.ulCmd = RCX_GET_SLAVE_CONN_INFO_REQ;
    tSendPacket.tHeader.ulLen = sizeof(uint32_t);  // Slave handle is a 32-bit value
    *((uint32_t*)tSendPacket.abData) = slaveHandle;

    // Send the request
    int32_t lRet = xChannelPutPacket(channel, &tSendPacket, 50);
    if (lRet != CIFX_NO_ERROR) {
        printf("Failed to send connection info request. Error: 0x%X\n", lRet);
        return lRet;
    }

    // Step 2: Receive the response
    lRet = xChannelGetPacket(channel, sizeof(tRecvPacket), &tRecvPacket, 50);
    if (lRet != CIFX_NO_ERROR || tRecvPacket.tHeader.ulCmd != RCX_GET_SLAVE_CONN_INFO_CNF) {
        printf("Failed to receive connection info. Error: 0x%X\n", lRet);
        return lRet;
    }

    // Parse the response
    PNM_AP_SLAVE_CONNECT_INFO_T* response = (PNM_AP_SLAVE_CONNECT_INFO_T*)tRecvPacket.abData;

    // Verify the handle matches
    if (response->ulHandle != slaveHandle) {
        printf("Mismatched slave handle in response: expected 0x%X, got 0x%X\n", slaveHandle, response->ulHandle);
        return -1;
    }

    // Check the structure ID and copy the appropriate information
    if (response->ulStructID == APIOC_ACTIVE_DEVICE_CONNECT_INFO_STRUCT_ID) { // Active slave
        if (activeInfo) {
            memcpy(activeInfo, &response->tConnectInfo.tActiveInfo, sizeof(PNM_AP_ACTIVE_SLAVE_CONNECT_INFO_T));
        }
        if (inactiveInfo) {
            memset(inactiveInfo, 0, sizeof(PNM_AP_INACTIVE_SLAVE_CONNECT_INFO_T)); // Clear inactive info
        }
    }
    else if (response->ulStructID == APIOC_INACTIVE_DEVICE_CONNECT_INFO_STRUCT_ID) { // Inactive slave
        if (inactiveInfo) {
            memcpy(inactiveInfo, &response->tConnectInfo.tInactiveInfo, sizeof(PNM_AP_INACTIVE_SLAVE_CONNECT_INFO_T));
        }
        if (activeInfo) {
            memset(activeInfo, 0, sizeof(PNM_AP_ACTIVE_SLAVE_CONNECT_INFO_T)); // Clear active info
        }
    }
    else {
        printf("Invalid structure ID in response: 0x%X\n", response->ulStructID);
        return -1;
    }

    return lRet;
}


int32_t NHsPNM_GetNumConfiguredObjects(CIFXHANDLE channel, PNM_AP_NUM_CONFIGURED_OBJECTS_T* confObjects) {
    CIFX_PACKET tSendPacket = { 0 };
    CIFX_PACKET tRecvPacket = { 0 };

    // Step 1: Prepare the request packet
    tSendPacket.tHeader.ulDest = RCX_DEFAULT_DESTINATION;
    tSendPacket.tHeader.ulCmd = PNM_AP_CMD_CFG_GET_NUM_CONFIGURED_OBJECTS_REQ;
    tSendPacket.tHeader.ulLen = 0; // No additional parameters required

    // Step 2: Send the request
    int32_t lRet = xChannelPutPacket(channel, &tSendPacket, 50);
    if (lRet != CIFX_NO_ERROR) {
        printf("Failed to send configured objects request. Error: 0x%X\n", lRet);
        return lRet;
    }

    // Step 3: Receive the confirmation
    lRet = xChannelGetPacket(channel, sizeof(tRecvPacket), &tRecvPacket, 50);
    if (lRet != CIFX_NO_ERROR || tRecvPacket.tHeader.ulCmd != PNM_AP_CMD_CFG_GET_NUM_CONFIGURED_OBJECTS_CNF || tRecvPacket.tHeader.ulState != CIFX_NO_ERROR) {
        printf("Failed to receive configured objects response. Command Error: 0x%X\n", lRet);
        printf("Failed to receive configured objects response. Command State: 0x%X\n", tRecvPacket.tHeader.ulState);
        return lRet;
    }



    // Step 4: Copy
    memcpy(confObjects, tRecvPacket.abData, sizeof(PNM_AP_NUM_CONFIGURED_OBJECTS_T));
    

    printf("Number of configured submodules: %u\n", confObjects->usNumSubmodule);
    return CIFX_NO_ERROR;
}


int32_t NHsPNM_GetSubmoduleInfo(CIFXHANDLE channel, uint16_t submoduleHandle, PNM_AP_SUBMODULE_INFO_T* submoduleInfo) {
    CIFX_PACKET tSendPacket = { 0 };
    CIFX_PACKET tRecvPacket = { 0 };

    // Step 1: Prepare the request packet
    tSendPacket.tHeader.ulDest = RCX_DEFAULT_DESTINATION;
    tSendPacket.tHeader.ulCmd = PNM_AP_CMD_CFG_GET_SUBMODULE_REQ;
    tSendPacket.tHeader.ulLen = sizeof(uint32_t) + sizeof(uint16_t);

    // Populate request data
    uint32_t* requestData = (uint32_t*)tSendPacket.abData;
    requestData[0] = 1; // Structure version (1)
    requestData[1] = submoduleHandle;

    // Step 2: Send the request
    int32_t lRet = xChannelPutPacket(channel, &tSendPacket, 50);
    if (lRet != CIFX_NO_ERROR) {
        printf("Failed to send submodule request. Error: 0x%X\n", lRet);
        return lRet;
    }

    // Step 3: Receive the confirmation
    lRet = xChannelGetPacket(channel, sizeof(tRecvPacket), &tRecvPacket, 50);
    if (lRet != CIFX_NO_ERROR || tRecvPacket.tHeader.ulCmd != PNM_AP_CMD_CFG_GET_SUBMODULE_CNF || tRecvPacket.tHeader.ulState != CIFX_NO_ERROR) {
        printf("Failed to receive submodule info response. Command Error: 0x%X\n", lRet);
        printf("Failed to receive submodule info response. Command State: 0x%X\n", tRecvPacket.tHeader.ulState);
        return lRet;
    }

    // Step 4: Copy the submodule information
    memcpy(submoduleInfo, tRecvPacket.abData, sizeof(PNM_AP_SUBMODULE_INFO_T));

    return CIFX_NO_ERROR;
}