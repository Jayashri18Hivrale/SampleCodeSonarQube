#ifndef NHOST_CIFX_H
#define NHOST_CIFX_H

#define NHOST_TOTAL_CHANNELS 1 // For now only access to cyclic data

#include "cifXUser.h"

typedef struct
{
	BOARD_INFORMATION netHostInfo;
	CIFXHANDLE nHostChannels[NHOST_TOTAL_CHANNELS];
	CHANNEL_INFORMATION nHostChannelsInfo[NHOST_TOTAL_CHANNELS];
}nHost_Device;

typedef struct
{
	CIFXHANDLE cifxHandle;                  /**< Handle for CIFX toolkit operations. */
	DRIVER_INFORMATION cifxDriverInfo;
	nHost_Device* nHostConnected;
	uint32_t nHostConnectedCount;
	int32_t cifxStatus;
}nHostCifx_t;

typedef nHostCifx_t* hnHostCifx;

hnHostCifx NHsCifx_Init();

void NHsCifx_DeInit();

void NHsCifx_showError(int32_t lError);

void NHsCifx_printChannelInfo(CHANNEL_INFORMATION* tChannelInfo);

void NHsCifx_printBoardInfo(BOARD_INFORMATION* tBoardInfo);

#endif
