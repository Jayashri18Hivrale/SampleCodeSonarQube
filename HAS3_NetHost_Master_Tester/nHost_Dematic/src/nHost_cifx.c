#include "nHost_cifx.h"

#include "cifXErrors.h"
#include "netXTransport.h"
#include "TCP_Connector.h"

#include <stdlib.h>
#include <string.h>

static int32_t TLLayerInit(void* pvParam);
static void TLLayerDeInit(void* pvParam);
static int32_t StartTcpTransportLayer();
static int32_t GetCifxDriverAndRelatedInfo(CIFXHANDLE* cifxDriver, DRIVER_INFORMATION* tDriverInfo);
static int32_t ReadBoardInfo(const CIFXHANDLE  hDriver, BOARD_INFORMATION* const ptBoardInfo, const unsigned long ulBoard);
static void StopTcpTransportLayer();
static void closeOpenedChannels(hnHostCifx me);

static nHostCifx_t nHost = { { 0 } };

hnHostCifx NHsCifx_Init()
{
	hnHostCifx me = &nHost;
    if (CIFX_NO_ERROR != (me->cifxStatus = StartTcpTransportLayer())) return NULL;
    if (CIFX_NO_ERROR != (me->cifxStatus = GetCifxDriverAndRelatedInfo(&(me->cifxHandle), &(me->cifxDriverInfo)))) return NULL;
    if (me->cifxDriverInfo.ulBoardCnt == 0) return me;
    me->nHostConnectedCount = me->cifxDriverInfo.ulBoardCnt;
    me->nHostConnected = malloc(me->nHostConnectedCount * sizeof(nHost_Device));
    if (NULL == me->nHostConnected) return NULL;
    for (int i = 0; i < me->nHostConnectedCount; i++)
    {
        if (CIFX_NO_ERROR != (me->cifxStatus = ReadBoardInfo(me->cifxHandle, &me->nHostConnected[i].netHostInfo, i))) return 0;
        for (int j = 0; j < NHOST_TOTAL_CHANNELS; j++)
        {
            char* boardName = me->nHostConnected[i].netHostInfo.abBoardName;
            CIFXHANDLE* phChannel = &(me->nHostConnected[i].nHostChannels[j]);
            CHANNEL_INFORMATION* pChannelInfo = &(me->nHostConnected[i].nHostChannelsInfo[j]);
            if (CIFX_NO_ERROR != (me->cifxStatus = xChannelOpen(me->cifxHandle, boardName, j, phChannel))) return me;
            if (CIFX_NO_ERROR != (me->cifxStatus = xChannelInfo(*phChannel, sizeof(CHANNEL_INFORMATION), pChannelInfo))) return me;
        }
    }
    return me;
}

void NHsCifx_DeInit()
{
    hnHostCifx me = &nHost;
    closeOpenedChannels(me);
    StopTcpTransportLayer();
    if (me->nHostConnected)
    {
        free(me->nHostConnected);
    }
    memset(me, 0, sizeof(*me));
}

void NHsCifx_showError(int32_t lError)
{
    if (lError != CIFX_NO_ERROR)
    {
        /* Read driver error description */
        char szError[1024] = { 0 };
        xDriverGetErrorDescription(lError, szError, sizeof(szError));
        printf("Error: 0x%X, <%s>\r\n", lError, szError);
    }
}

void NHsCifx_printChannelInfo(CHANNEL_INFORMATION* tChannelInfo)
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

void NHsCifx_printBoardInfo(BOARD_INFORMATION* tBoardInfo)
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

static int32_t ReadBoardInfo(const CIFXHANDLE  hDriver, BOARD_INFORMATION* const ptBoardInfo, const unsigned long ulBoard)
{
    CIFXHANDLE                       hSys = 0;
    SYSTEM_CHANNEL_SYSTEM_INFO_BLOCK tSysInfo = ptBoardInfo->tSystemInfo;
    long                             lRet = 0;      /** Return value for common error codes  */

    if (CIFX_NO_ERROR != (lRet = xDriverEnumBoards(hDriver, ulBoard, sizeof(BOARD_INFORMATION), ptBoardInfo)))
    {
        xDriverClose(hDriver);
        return lRet;
    }

    /* Get Sys-Channel information */
    if (CIFX_NO_ERROR == (lRet = xSysdeviceOpen(hDriver, ptBoardInfo->abBoardName, &hSys)))
    {
        lRet = xSysdeviceInfo(hSys, CIFX_INFO_CMD_SYSTEM_INFO_BLOCK, sizeof(tSysInfo), &tSysInfo);
        xSysdeviceClose(hSys);
    }
    return lRet;
}

static int32_t GetCifxDriverAndRelatedInfo(CIFXHANDLE* cifxDriver, DRIVER_INFORMATION* tDriverInfo)
{
    int32_t    lRet = CIFX_NO_ERROR;

    /* open the driver */
    printf("\nGetting Driver Information ...");
    if (CIFX_NO_ERROR == (lRet = xDriverOpen(cifxDriver)))
    {
        if (CIFX_NO_ERROR != (lRet = xDriverGetInformation(*cifxDriver, sizeof(tDriverInfo), tDriverInfo)))
        {
            printf("\n    Error while requesting the driver information (lRet=0x%X)!\n", lRet);
            NHsCifx_showError(lRet);
            return;
        }
        else
        {
            printf("OK\n");
            printf("      Driver Version: %s\n", tDriverInfo->abDriverVersion);
        }
    }
    return lRet;
}

static int32_t StartTcpTransportLayer()
{
    int32_t  lRet = CIFX_NO_ERROR;
    /* setup netXTransport initialization structure */
    TL_INIT_T                tDataLayerInit;
    tDataLayerInit.pfnTLInit = TLLayerInit;   /* function pointer the Translation-Layer initialization    */
    tDataLayerInit.pfnTLDeInit = TLLayerDeInit; /* function pointer the Translation-Layer de-initialization */
    tDataLayerInit.pvData = NULL;          /* Private data (currently  not used)                       */

    printf("\nInitializing the netXTransport Toolkit...");
    if (CIFX_NO_ERROR == (lRet = netXTransportInit(&tDataLayerInit, sizeof(tDataLayerInit))))
    {
        printf("OK\n");
    }
    else
    {
        printf("\nError during toolkit initialization. lRet=0x%X\n", lRet);
    }
    /*****************************************************************************/
    /* 2. Add TCP connector                                                      */
    /*****************************************************************************/
    if (lRet == CIFX_NO_ERROR)
    {
        printf("\nAddding connector TCP...");
        if (CIFX_NO_ERROR == (lRet = TCP_Connector_Init()))
        {
            printf("OK\n");
        }
        else
        {
            printf("\nError during TCP-Connector initialization. lRet=0x%X\n", lRet);
        }
    }

    /*****************************************************************************/
    /* 3. Start the netXTransport Toolkit                                        */
    /*****************************************************************************/
    if (lRet == CIFX_NO_ERROR)
    {
        printf("\nStarting netXTransport Toolkit...");
        /* start netXTransport (starts device discovering and registration) */
        if (CIFX_NO_ERROR != (lRet = netXTransportStart(NULL, NULL)))
        {
            printf("\nError while trying to start. lRet=0x%X\n", lRet);
        }
        else
        {
            printf("OK\n");
        }
    }
    return lRet;
}

static int32_t TLLayerInit(void* pvParam)
{
    int32_t lRet = NXT_NO_ERROR;

    if (NXT_NO_ERROR == (lRet = cifX_Marshaller_Init(pvParam)))
    {
        lRet = rcXPacket_Init(pvParam);
    }
    return lRet;
}

static void TLLayerDeInit(void* pvParam)
{
    cifX_Marshaller_DeInit(pvParam);
    rcXPacket_DeInit(pvParam);
}

static void StopTcpTransportLayer()
{
    netXTransportStop();
    TCP_Connector_Deinit();
}

static void closeOpenedChannels(hnHostCifx me)
{
    if (me->nHostConnected)
    {
        for (int i = 0; i < me->nHostConnectedCount; i++)
        {
            for (int j = 0; j < NHOST_TOTAL_CHANNELS; j++)
            {
                CIFXHANDLE* phChannel = &(me->nHostConnected[i].nHostChannels[j]);
                xChannelClose(*phChannel);
            }
        }
    }
}