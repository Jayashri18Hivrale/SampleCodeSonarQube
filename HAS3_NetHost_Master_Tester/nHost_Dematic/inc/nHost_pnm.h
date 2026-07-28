#ifndef NHOST_PNM_H
#define NHOST_PNM_H

#include "nHost_cifx.h"
#include "nHost_common.h"
#include "nHostProfinetApi.h"

#include <stdbool.h>

typedef struct
{
    PNM_AP_ACTIVE_SLAVE_CONNECT_INFO_T activeInfo;   
    PNM_AP_INACTIVE_SLAVE_CONNECT_INFO_T inactiveInfo;
    PNM_AP_SUBMODULE_INFO_T** submodulesInfo; // just added
    uint32_t nSubmodules; // just added
    bool isActive; 
}NHost_PNSlave_t;


typedef struct 
{
    NHost_FieldBus_t common;
    NHost_PNSlave_t* pnSlaves;
    uint32_t pnSlavesCount;
}NHost_Profinet_t;


typedef struct
{
	NHost_Profinet_t * PN_NetHosts;
	uint32_t PN_NetHostsCount;
}NHost_Profinet_Array;

typedef NHost_Profinet_Array * hHNost_Profinet;

hHNost_Profinet NHsPNM_Init(hnHostCifx cifxHandle);

void NHsPNM_DumpSlavesInfo(NHost_Profinet_t* profinetNethost);

int32_t NHsPNM_SetCyclicData(NHost_PNSlave_t* slave, uint8_t* outData, uint32_t nBytes);

int32_t NHsPNM_GetCyclicData(NHost_PNSlave_t* slave, uint8_t* inData, uint32_t nBytes);

#endif