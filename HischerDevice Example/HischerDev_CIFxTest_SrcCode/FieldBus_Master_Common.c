#include "FieldBus_Master_Common.h"

#include <stdlib.h>

#include "cifx_Helper_Functions.h"
#include "cifXErrors.h"
#include "netXTransport.h"
#include "TL_rcXPacket.h"
#include "TL_Marshaller.h"
#include "TCP_Connector.h"


#define RCX_DEFAULT_DESTINATION                0x20
// RCX Commands for querying slave information
#define RCX_GET_SLAVE_HANDLE_REQ               0x2F08
#define RCX_GET_SLAVE_HANDLE_CNF               0x2F09
#define RCX_GET_SLAVE_CONN_INFO_REQ            0x2F0A
#define RCX_GET_SLAVE_CONN_INFO_CNF            0x2F0B

/* NETXTRANSPORT Functions*/
static int32_t TLLayerInit(void* pvParam);
static void TLLayerDeInit(void* pvParam);

int32_t StartTcpTransportLayer()
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

void StopTcpTransportLayer()
{
    netXTransportStop();
    TCP_Connector_Deinit();
}

int32_t GetCifxDriverAndRelatedInfo(CIFXHANDLE* cifxDriver, DRIVER_INFORMATION* tDriverInfo)
{
    int32_t    lRet = CIFX_NO_ERROR;

    /* open the driver */
    printf("\nGetting Driver Information ...");
    if (CIFX_NO_ERROR == (lRet = xDriverOpen(cifxDriver)))
    {
        if (CIFX_NO_ERROR != (lRet = xDriverGetInformation(*cifxDriver, sizeof(tDriverInfo), tDriverInfo)))
        {
            printf("\n    Error while requesting the driver information (lRet=0x%X)!\n", lRet);
            ShowError(lRet);
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

int32_t GetConnectedNetHostsBoardInformation(CIFXHANDLE cifxDriver, BOARD_INFORMATION** BoardsInfo, uint32_t nNetHosts)
{
    int32_t lRet = CIFX_NO_ERROR;

    printf("\nGetting NetHosts Information...");
    if (!nNetHosts)
    {
        printf("FAIL\n");
        printf("     No NetHostConnected to PC");
        return CIFX_FUNCTION_FAILED;
    }
    for (int i = 0; i < nNetHosts; i++)
    {
        BOARD_INFORMATION* BoardInfo = (BOARD_INFORMATION*)malloc(sizeof(BOARD_INFORMATION));
        if (CIFX_NO_ERROR == (lRet = xDriverEnumBoards(cifxDriver, i, sizeof(*BoardInfo), BoardInfo)))
        {
            BoardsInfo[i] = BoardInfo;
        }
        else
        {
            free(BoardInfo);
            printf("FAIL\n");
            printf("\n    Error while requesting NetHost %d Board information(lRet = 0x % X)!\n", i + 1, lRet);
            ShowError(lRet);
            return lRet;
        }
    }
    printf("OK\n");
    return lRet;
}

int32_t FieldBusGetActiveSlaves(FIELDBUS_SLAVES_INFO_T* pSlaveInfo) {
    CIFXHANDLE hChannel = NULL;
    CIFX_PACKET tSendPacket = { 0 };
    CIFX_PACKET tRecvPacket = { 0 };
    int32_t lRet = CIFX_NO_ERROR;

    // Open the system device using the provided board name
    if (CIFX_NO_ERROR == (lRet = xChannelOpen(pSlaveInfo->connectionHandler->cifxDriver, 
                               pSlaveInfo->connectionHandler->szBoardName,0, &hChannel))) {
        int ulState;
        // Step 1: Send RCX_GET_SLAVE_HANDLE_REQ to retrieve slave handles based on the state
        memset(&tSendPacket.tHeader, 0, sizeof(tSendPacket.tHeader));
        tSendPacket.tHeader.ulDest = RCX_DEFAULT_DESTINATION;
        tSendPacket.tHeader.ulCmd = RCX_GET_SLAVE_HANDLE_REQ;  // Command to get slave handles
        tSendPacket.tHeader.ulLen = sizeof(uint32_t);  // Data length
        *((uint32_t *)tSendPacket.abData) = pSlaveInfo->ulSlaveState;              // Slave state (configured, active, or faulted)
        lRet = xChannelPutPacket(hChannel, &tSendPacket, 50);
        if (lRet != CIFX_NO_ERROR) {
            printf("Failed to send handle request. Error: 0x%X\n", lRet);
            return lRet;
        }

        // Step 2: Receive the response RCX_GET_SLAVE_HANDLE_CNF
        lRet = xChannelGetPacket(hChannel, sizeof(tRecvPacket), &tRecvPacket, 50);
        if (lRet != CIFX_NO_ERROR || tRecvPacket.tHeader.ulCmd != RCX_GET_SLAVE_HANDLE_CNF) {
            printf("Failed to get slave handles. Error: 0x%X\n", lRet);
            return lRet;
        }
        // Step 3: Process the list of handles
        int numSlaves = (tRecvPacket.tHeader.ulLen - 4) / sizeof(uint32_t);   // Number of connected slaves

        printf("Number of connected slaves: %d\n", numSlaves);

        // Allocate memory for the array of slave handles
        pSlaveInfo->pSlaveHandles = (uint32_t*)malloc(numSlaves * sizeof(uint32_t));
        
        // Copy the slave handles from the received packet
        memcpy(pSlaveInfo->pSlaveHandles, tRecvPacket.abData + 4, tRecvPacket.tHeader.ulLen);
        // Set the number of slaves (if pSlaveInfo->pNumSlaves is not NULL)
         pSlaveInfo->pNumSlaves = numSlaves;
        
        // Close the system device
        lRet = xChannelClose(hChannel);
    }
    else {
        printf("Failed to open system device. Error: 0x%X\n", lRet);
    }
    return lRet;
}

int32_t GetConnectionHandlers(FIELDBUS_CONNECTION_HANDLERS** connHandlers, BOARD_INFORMATION** BoardsInfo, uint32_t nNetHosts)
{
    int32_t lRet = CIFX_NO_ERROR;

    printf("\nGetting NetHosts Information...");
    if (!nNetHosts)
    {
        printf("FAIL\n");
        printf("     No NetHostConnected to PC");
        return CIFX_FUNCTION_FAILED;
    }
    for (int i = 0; i < nNetHosts; i++)
    {
        CHANNEL_INFORMATION Chan0Info;
        CIFXHANDLE hChannel = NULL;
        if (CIFX_NO_ERROR != (lRet = xChannelOpen(connHandlers[i]->cifxDriver, BoardsInfo[i]->abBoardName, 0, &hChannel))) {
            printf("Failed to open channel driver device. Error: 0x%X\n", lRet);
        }
        if (CIFX_NO_ERROR == (lRet = xChannelInfo(hChannel, sizeof(Chan0Info), &Chan0Info)))
        {
            memcpy(connHandlers[i]->fwName, &Chan0Info.abFWName, 25);
            memcpy(connHandlers[i]->szBoardName, BoardsInfo[i]->abBoardName, 25);
        }
        else
        {
            printf("FAIL\n");
            printf("\n    Error while requesting NetHost %d Channel 0 information(lRet = 0x % X)!\n", i + 1, lRet);
            ShowError(lRet);
            return lRet;
        }
        xChannelClose(connHandlers[i]->cifxDriver);
    }
    printf("OK\n");
    return lRet;
}


// Private Functions

/*****************************************************************************/
/*! Initializes the cifX Marshaller Translation-Layer and the rcX-Packet
*   Translation-Layer.
*   \param pvParam    User param (currently not used)
*   \return NXT_NO_ERROR on success                                          */
/*****************************************************************************/
int32_t TLLayerInit(void* pvParam)
{
    int32_t lRet = NXT_NO_ERROR;

    if (NXT_NO_ERROR == (lRet = cifX_Marshaller_Init(pvParam)))
    {
        lRet = rcXPacket_Init(pvParam);
    }
    return lRet;
}

/*****************************************************************************/
/*! De-initializes translation layer
*   \param pvParam    User param (currently not used)                        */
/*****************************************************************************/
void TLLayerDeInit(void* pvParam)
{
    cifX_Marshaller_DeInit(pvParam);
    rcXPacket_DeInit(pvParam);
}