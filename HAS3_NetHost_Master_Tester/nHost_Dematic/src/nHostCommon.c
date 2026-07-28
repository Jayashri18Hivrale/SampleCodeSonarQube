#include "nHost_common.h"

#include "cifXErrors.h"
#include "nHostCommonApi.h"

#include <stdlib.h>

int32_t nHCommon_GetActiveSlaveHandles(CIFXHANDLE channel, uint32_t** handlersList, uint32_t* slavesCount)
{
    nHCommon_GetSlaveHandles(channel, handlersList, slavesCount, RCX_LIST_ACTV_SLAVES);
}

int32_t nHCommon_GetSlaveHandles(CIFXHANDLE channel, uint32_t** handlersList, uint32_t* slavesCount, uint32_t slaveStatus)
{
    CIFX_PACKET tSendPacket = { 0 };
    CIFX_PACKET tRecvPacket = { 0 };

    tSendPacket.tHeader.ulDest = RCX_DEFAULT_DESTINATION;
    tSendPacket.tHeader.ulCmd = RCX_GET_SLAVE_HANDLE_REQ;      // Command to get slave handles
    tSendPacket.tHeader.ulLen = sizeof(uint32_t);              // Data length
    *((uint32_t*)tSendPacket.abData) = slaveStatus;   // Slave state (configured, active, or faulted)
    int32_t lRet = xChannelPutPacket(channel, &tSendPacket, 50);
    if (lRet != CIFX_NO_ERROR) {
        printf("Failed to send handle request. Error: 0x%X\n", lRet);
        return lRet;
    }

    // Step 2: Receive the response RCX_GET_SLAVE_HANDLE_CNF
    lRet = xChannelGetPacket(channel, sizeof(tRecvPacket), &tRecvPacket, 50);
    if (lRet != CIFX_NO_ERROR || tRecvPacket.tHeader.ulCmd != RCX_GET_SLAVE_HANDLE_CNF) {
        printf("Failed to get slave handles. Error: 0x%X\n", lRet);
        return lRet;
    }
    // Step 3: Process the list of handles
    *slavesCount = (tRecvPacket.tHeader.ulLen - 4) / sizeof(uint32_t);   // Number of connected slaves

    // Allocate memory for the array of slave handles
    *handlersList = (uint32_t*)malloc(*slavesCount * sizeof(uint32_t));

    // Copy the slave handles from the received packet
    memcpy(*handlersList, tRecvPacket.abData + 4, tRecvPacket.tHeader.ulLen);

    return lRet;
}

