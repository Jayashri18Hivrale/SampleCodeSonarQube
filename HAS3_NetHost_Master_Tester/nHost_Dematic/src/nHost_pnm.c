#include "nHost_pnm.h"

#include "nHostCommonApi.h"

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

NHost_Profinet_Array pnm_data = { { 0 } };
hHNost_Profinet me = &pnm_data;
hnHostCifx nHostCifxHandle = NULL;

static int32_t FillNHostPN(NHost_Profinet_t * nHost, nHost_Device * device);
static bool isProfinet(nHost_Device* device);

hHNost_Profinet NHsPNM_Init(hnHostCifx cifxHandle)
{
    nHostCifxHandle = cifxHandle;

    // Allocate memory for the Profinet array initially as NULL
    NHost_Profinet_Array* me = (NHost_Profinet_Array*)malloc(sizeof(NHost_Profinet_Array));
    if (me == NULL)
        return NULL;

    me->PN_NetHostsCount = 0;
    me->PN_NetHosts = NULL;

    uint32_t profinetCount = 0;

    // First, count the number of Profinet netHosts to allocate memory
    for (int i = 0; i < nHostCifxHandle->nHostConnectedCount; i++)
    {
        nHost_Device* pDev = &(nHostCifxHandle->nHostConnected[i]);
        if (isProfinet(pDev)) // Check if genericNethost is Profinet
        {
            profinetCount++;
        }
    }

    if (profinetCount == 0)
    {
        free(me);
        return NULL; // No Profinet devices found
    }

    // Allocate memory for the Profinet PN_NetHosts array
    me->PN_NetHosts = (NHost_Profinet_t*)malloc(sizeof(NHost_Profinet_t) * profinetCount);
    if (me->PN_NetHosts == NULL)
    {
        free(me);
        return NULL; // Memory allocation failed
    }

    // Fill the PN_NetHosts array with Profinet genericNethost information
    uint32_t nHostPnIndex = 0;
    for (int i = 0; i < nHostCifxHandle->nHostConnectedCount; i++)
    {
        nHost_Device* pDev = &(nHostCifxHandle->nHostConnected[i]);
        if (isProfinet(pDev)) // Check if genericNethost is Profinet
        {
            NHost_Profinet_t* pnHostPn = &(me->PN_NetHosts[nHostPnIndex]);
            FillNHostPN(pnHostPn, pDev);
            nHostPnIndex++;
        }
    }

    me->PN_NetHostsCount = profinetCount;

    // Return the initialized Profinet array
    return me;
}

int32_t NHsPNM_SetCyclicData(NHost_PNSlave_t* slave, uint8_t* outData, uint32_t nBytes)
{
	// use CONTAINER_OF to get the handle
}

int32_t NHsPNM_GetCyclicData(NHost_PNSlave_t* slave, uint8_t* inData, uint32_t nBytes) 
{
	// use CONTAINER_OF to get the handle
}

void NHsPNM_DumpSlavesInfo(NHost_Profinet_t* profinetNethost) {
    if (profinetNethost == NULL || profinetNethost->pnSlaves == NULL) {
        printf("No slaves to dump.\n");
        return;
    }

    printf("Dumping Profinet Slave Information:\n");
    printf("Total Slaves: %u\n", profinetNethost->pnSlavesCount);

    for (uint32_t i = 0; i < profinetNethost->pnSlavesCount; i++) {
        NHost_PNSlave_t* slave = &profinetNethost->pnSlaves[i];

        printf("\nSlave %u\n", i);
        printf("  Device ID: 0x%X\n", slave->isActive ? slave->activeInfo.usDeviceID : slave->inactiveInfo.usDeviceID);
        printf("  Vendor ID: 0x%X\n", slave->isActive ? slave->activeInfo.usVendorID : slave->inactiveInfo.usVendorID);

        if (slave->isActive) {
            printf("  Status: ACTIVE\n");
            printf("  IP Address: %u.%u.%u.%u\n",
                (slave->activeInfo.ulIPAddress >> 24) & 0xFF,
                (slave->activeInfo.ulIPAddress >> 16) & 0xFF,
                (slave->activeInfo.ulIPAddress >> 8) & 0xFF,
                (slave->activeInfo.ulIPAddress) & 0xFF);
            printf("  MAC Address: %02X:%02X:%02X:%02X:%02X:%02X\n",
                slave->activeInfo.abMac[0], slave->activeInfo.abMac[1],
                slave->activeInfo.abMac[2], slave->activeInfo.abMac[3],
                slave->activeInfo.abMac[4], slave->activeInfo.abMac[5]);
            printf("  Name: %.*s\n", slave->activeInfo.ulLenName, slave->activeInfo.abName);
            printf("  Type: %.*s\n", slave->activeInfo.ulLenType, slave->activeInfo.abType);
            printf("  Diagnostic Flags: 0x%X\n", slave->activeInfo.ulDiagFlags);
        }
        else {
            printf("  Status: INACTIVE\n");
            printf("  Name: %.*s\n", slave->inactiveInfo.ulLenName, slave->inactiveInfo.abName);
            printf("  Diagnostic Flags: 0x%X\n", slave->inactiveInfo.ulDiagFlags);
        }
    }
    printf("Press any key to return to menu...");
    _getch();
    system("cls");
}

static int32_t FillNHostPN(NHost_Profinet_t* profinetNethost, nHost_Device* genericNethost)
{
    NHost_FieldBus_t* fieldBusNethost = &profinetNethost->common;
    fieldBusNethost->hCyclicChannel = genericNethost->nHostChannels[0];

    // Get slave handles
    int32_t lRet = nHCommon_GetSlaveHandles(fieldBusNethost->hCyclicChannel,
        &fieldBusNethost->configuredSlavesHandlers,
        &fieldBusNethost->configuredSlavesCount,
        RCX_LIST_CONF_SLAVES);

    if (lRet == 0) {
        profinetNethost->pnSlavesCount = fieldBusNethost->configuredSlavesCount;

        if (fieldBusNethost->configuredSlavesHandlers > 0) {
            profinetNethost->pnSlaves = malloc(profinetNethost->pnSlavesCount * sizeof(NHost_PNSlave_t));
            if (profinetNethost->pnSlaves == NULL)
                return -1;

            for (int i = 0; i < profinetNethost->pnSlavesCount; i++) {
                uint32_t handler = fieldBusNethost->configuredSlavesHandlers[i];
                NHost_PNSlave_t* currentSlave = &profinetNethost->pnSlaves[i];

                // Clear memory
                memset(currentSlave, 0, sizeof(NHost_PNSlave_t));

                // Step 1: Get connection info
                lRet = NHsPNM_GetSlaveConnecInfo(fieldBusNethost->hCyclicChannel, handler,
                    &currentSlave->inactiveInfo, &currentSlave->activeInfo);
                if (lRet != 0)
                    continue;

                // Determine if the slave is active
                currentSlave->isActive = (currentSlave->activeInfo.ulLenName > 0);

                // Step 2: Retrieve configured object information
                PNM_AP_NUM_CONFIGURED_OBJECTS_T confObjects = { 0 };
                lRet = NHsPNM_GetNumConfiguredObjects(fieldBusNethost->hCyclicChannel, &confObjects);
                if (lRet != 0)
                    continue;

                currentSlave->nSubmodules = confObjects.usNumSubmodule;

                // Step 3: Allocate memory for submodule information
                currentSlave->submodulesInfo = (PNM_AP_SUBMODULE_INFO_T**)malloc(currentSlave->nSubmodules * sizeof(PNM_AP_SUBMODULE_INFO_T*));
                if (currentSlave->submodulesInfo == NULL)
                    return -1;

                // Initialize submodule pointers to NULL
                for (uint32_t j = 0; j < currentSlave->nSubmodules; j++) {
                    currentSlave->submodulesInfo[j] = NULL;
                }

                // Step 4: Retrieve submodule details
                for (uint32_t j = 0; j < currentSlave->nSubmodules; j++) {
                    currentSlave->submodulesInfo[j] = (PNM_AP_SUBMODULE_INFO_T*)malloc(sizeof(PNM_AP_SUBMODULE_INFO_T));
                    if (currentSlave->submodulesInfo[j] == NULL)
                        return -1;

                    lRet = NHsPNM_GetSubmoduleInfo(fieldBusNethost->hCyclicChannel, j, currentSlave->submodulesInfo[j]);
                    if (lRet != 0) {
                        free(currentSlave->submodulesInfo[j]);
                        currentSlave->submodulesInfo[j] = NULL;
                    }
                }
            }
        }
    }

    return lRet;
}

static bool isProfinet(nHost_Device* device)
{
    const char* search = "PNM";
    return strstr((const char*)device->nHostChannelsInfo[0].abFWName, search) != NULL;
}
