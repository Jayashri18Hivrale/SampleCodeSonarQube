#ifndef NETHOST_COMMON_H
#define NETHOST_COMMON_H

#include "cifXUser.h"
#include <stdint.h>

typedef struct
{
	CIFXHANDLE hCyclicChannel;      // Handle to the CIFX driver
	uint32_t* configuredSlavesHandlers;
	uint32_t configuredSlavesCount;
}NHost_FieldBus_t;

int32_t nHCommon_GetSlaveHandles(CIFXHANDLE channel, uint32_t** handlersList, uint32_t* slavesCount, uint32_t slaveStatus);

int32_t nHCommon_GetActiveSlaveHandles(CIFXHANDLE channel, uint32_t** handlersList, uint32_t* slavesCount);

#endif