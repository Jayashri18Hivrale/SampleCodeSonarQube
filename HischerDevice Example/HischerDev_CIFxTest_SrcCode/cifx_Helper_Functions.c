#include "cifx_Helper_Functions.h"

#include <stdint.h>
#include "cifXUser.h"
#include "cifXErrors.h"

void ShowError(int32_t lError)
{
    if (lError != CIFX_NO_ERROR)
    {
        /* Read driver error description */
        char szError[1024] = { 0 };
        xDriverGetErrorDescription(lError, szError, sizeof(szError));
        printf("Error: 0x%X, <%s>\r\n", lError, szError);
    }
}

void printChannelInfo(CHANNEL_INFORMATION* tChannelInfo)
{
    if (tChannelInfo == NULL)
    {
        printf("  Channel Null pointer");
        return;
    }
    /* Show information */
    printf("   Channel Error            : 0x%08X\r\n", tChannelInfo->ulChannelError);
    printf("   Board Name               : %s\r\n", tChannelInfo->abBoardName);
    printf("   Alias Name               : %s\r\n", tChannelInfo->abBoardAlias);
    printf("   Device Nr.               : %lu\r\n", (long unsigned int)tChannelInfo->ulDeviceNumber);
    printf("   Serial Nr.               : %lu\r\n", (long unsigned int)tChannelInfo->ulSerialNumber);
    printf("   MBX Size                 : %lu\r\n", (long unsigned int)tChannelInfo->ulMailboxSize);
    printf("   Firmware Name            : %s\r\n", tChannelInfo->abFWName);
    printf("   Firmware Version         : %d.%d.%d Build %d\r\n", tChannelInfo->usFWMajor, tChannelInfo->usFWMinor, tChannelInfo->usFWRevision, tChannelInfo->usFWBuild);
    printf("   Open Counter             : %lu\r\n", (long unsigned int)tChannelInfo->ulOpenCnt);
    printf("   Put Packet Counter       : %lu\r\n", (long unsigned int)tChannelInfo->ulPutPacketCnt);
    printf("   Get Packet Counter       : %lu\r\n", (long unsigned int)tChannelInfo->ulGetPacketCnt);
    printf("   Number of IO Input Areas : %lu\r\n", (long unsigned int)tChannelInfo->ulIOInAreaCnt);
    printf("   Number of IO Output Areas: %lu\r\n", (long unsigned int)tChannelInfo->ulIOOutAreaCnt);
    printf("   Size of handshake cells  : %lu\r\n", (long unsigned int)tChannelInfo->ulHskSize);
    printf("   Actual netX Flags        : 0x%08X\r\n", tChannelInfo->ulNetxFlags);
    printf("   Actual host Flags        : 0x%08X\r\n", tChannelInfo->ulHostFlags);
}

void printBoardInfo(BOARD_INFORMATION* tBoardInfo)
{
    if (tBoardInfo == NULL)
    {
        printf("  Board Null pointer");
        return;
    }
    printf(" Name         : %s\r\n", tBoardInfo->abBoardName);
    printf(" Alias        : %s\r\n", tBoardInfo->abBoardAlias);
    printf(" DevNr        : %lu\r\n", (long unsigned int)tBoardInfo->tSystemInfo.ulDeviceNumber);
    printf(" SN           : %lu\r\n", (long unsigned int)tBoardInfo->tSystemInfo.ulSerialNumber);
    printf(" Channel Count: %lu\r\n", (long unsigned int)tBoardInfo->ulChannelCnt);
    printf("\r\n");
}
