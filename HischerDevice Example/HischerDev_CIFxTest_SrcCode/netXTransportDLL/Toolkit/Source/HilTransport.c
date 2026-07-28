/**************************************************************************************

   Copyright (c) Hilscher GmbH. All Rights Reserved.

 **************************************************************************************

   Filename:
    $Id: HilTransport.c 177 2013-06-21 05:05:30Z sebastiand $
   Last Modification:
    $Author: sebastiand $
    $Date: 2013-06-21 07:05:30 +0200 (Fr, 21. Jun 2013) $
    $Revision: 177 $

   Targets:
     O/S Independent : yes

   Description:
    Implementation of the Hilscher Transport protocol

   Changes:

     Version   Date        Author   Description
     ----------------------------------------------------------------------------------
      1         23.02.13    SD/MT       initial version

**************************************************************************************/

/****************************************************************************/
/*! \file HilTransport.c
    Implementation of the Hilscher Transport protocol                       */
/****************************************************************************/

#include "netXTransport.h"
#include "HilTransportLayer.h"
#include "OS_Dependent.h"
#include "cifXErrors.h"
#include "OS_Includes.h"

/*****************************************************************************/
/*! Global management information                                            */
/*****************************************************************************/
typedef struct HIL_TRANSPORT_INSTANCE_Ttag
{
  void* hNetXTransportHandle;  /*!< reference to the netXTransport instance (e.g. required for trace messages) */

} HIL_TRANSPORT_INSTANCE_T, *PHIL_TRANSPORT_INSTANCE_T /*!< Pointer to HIL_TRANSPORT_INSTANCE_T */;

/*****************************************************************************/
/*! State definitions of the recevice state machine                          */
/*****************************************************************************/
typedef enum
{
  eWAIT_FOR_COOKIE  = 0,        /*!< State machine is waiting for cookie                            */
  eWAIT_FOR_HEADER,             /*!< State machine got the cookie and is waiting to complete header */
  eWAIT_FOR_PACKET_COMPLETE,    /*!< State machine got the header and is waiting to complete packet */

} HIL_TRANSPORT_RX_STATE_E;

/*****************************************************************************/
/*! HilTransport transaction resource structure                              */
/*****************************************************************************/
typedef struct HIL_TRANSPORT_TRANSACTION_DATA_Ttag
{
  TAILQ_ENTRY(HIL_TRANSPORT_TRANSACTION_DATA_Ttag) tList;  /*!< List entry                             */
  union
  {
    PHIL_TRANSPORT_HEADER   ptSendPacket;   /*!< Send packet                                           */
    PHIL_TRANSPORT_PACKET_T ptRecvPacket;   /*!< Receive packet (response)                             */
  } uPacket;                                /*!< Union: HilTransport packet                            */
  void*   pvComplete;                       /*!< Pointer to event, signalling transaction is completed */
  int32_t lError;                           /*!< Error (valid if tranaction is complete)               */
  int     fPending;                         /*!< Indicates transaction is already pending              */
  int     fCancelled;

} HIL_TRANSPORT_TRANSACTION_DATA_T, *PHIL_TRANSPORT_TRANSACTION_DATA_T /*!< Pointer to HIL_TRANSPORT_INSTANCE_T */;

/*****************************************************************************/
/*! HilTransport Translation-Layer information structure                     */
/*****************************************************************************/
typedef struct HIL_TL_INFO_Ttag
{
  TAILQ_ENTRY(HIL_TL_INFO_Ttag)  tList;         /*!< List entry                             */
  void*                          hTLHandle;     /*!< Handle to Translation-Layer            */
  PNETX_TL_INTERFACE_T           ptTLInterface; /*!< Pointer to Translation-Layer interface */

} HIL_TL_INFO_T, *PHIL_TL_INFO_T /*!< Pointer to HIL_TL_INFO_T */;

TAILQ_HEAD(HIL_TRANSACTIONS_T, HIL_TRANSPORT_TRANSACTION_DATA_Ttag);
TAILQ_HEAD(HIL_TL_INFO_LIST_T, HIL_TL_INFO_Ttag);

/*****************************************************************************/
/*! HilTransport Keep-Alive information structure                            */
/*****************************************************************************/
typedef struct TRANSPORT_KEEPALIVE_Ttag
{
  KEEPALIVE_STATE_E eKeepAliveState;     /*!< State of Keep-Alive handling            */
  uint32_t          ulKeepAliveIdent;    /*!< Ident number of Keep-Alive handling     */
  uint32_t          ulKeepAliveTimeout;  /*!< Connection dependent Keep-Alive timeout */
  uint32_t          ulLastKeepAlive;     /*!< Timestamp of last transmission          */

} TRANSPORT_KEEPALIVE_T;

/*****************************************************************************/
/*! HilTransport instance information structure                              */
/*****************************************************************************/
typedef struct HIL_TRANSPORT_DATA_Ttag
{
  TAILQ_ENTRY(HIL_TRANSPORT_DATA_Ttag) tList;                      /*!< List entry                                            */
  struct HIL_TL_INFO_LIST_T            tTLInfoList;                /*!< List of supported Translation-Layers                  */
  PNETX_CONNECTOR_T                    ptConnector;                /*!< Pointer to netXTransport connector                    */
  PHIL_TL_INFO_T                       ptDefaultTranslationLayer;  /*!< Default Translation-Layer (deliver incoming requests) */
  uint8_t                              bLastSequenceNr;            /*!< Last sequence number                                  */
  uint16_t                             usLastTransaction;          /*!< Last transaction number                               */
  char                                 szInterfaceName[16];        /*!< Interface name                                        */
  void*                                pvConnectorData;            /*!< Private connector data                                */
  void*                                pvConnectorInterface;       /*!<                                                       */
  TRANSPORT_SERVER_INFO_T              tServerInfo;                /*!< Server information                                    */
  HIL_TRANSPORT_RX_STATE_E             eRxState;                   /*!< State of the receive state machine                    */
  HIL_TRANSPORT_HEADER                 tRxCurrentHeader;           /*!< Current receive header                                */
  uint32_t                             ulRXCurrentOffset;          /*!< Current send header                                   */
  PHIL_TRANSPORT_PACKET_T              ptCurrentRxPacket;          /*!< Current receive packet                                */
  uint32_t                             ulDataOffset;               /*!< Current data offset                                   */
  void*                                hTXTransactionsLock;        /*!< Pointer to send transaction lock                      */
  struct HIL_TRANSACTIONS_T            tTXTransactions;            /*!< List of active send transactions                      */
  void*                                hRXTransactionsLock;        /*!< Pointer to receive transaction lock                   */
  struct HIL_TRANSACTIONS_T            tRXTransactions;            /*!< List of active receive transactions                   */
  TRANSPORT_KEEPALIVE_T                tKeepAlive;                 /*!< State of Keep-Alive handling                          */

} HIL_TRANSPORT_DATA_T, *PHIL_TRANSPORT_DATA_T /*!< Pointer to HIL_TRANSPORT_DATA_T */;

/* local functions */
static void     HilTransportRxData         ( uint8_t*              pabRxBuffer, uint32_t                                 ulReadLen, void* pvUser);
static int32_t  HilTransportQueryServerData( PHIL_TRANSPORT_DATA_T ptTransport, PHIL_TRANSPORT_ADMIN_QUERYSERVER_DATA_T* pptServerInfo);
static uint16_t CalcCrc16                  ( uint8_t*              pbData,      uint32_t                                 ulDataLen);
static uint16_t CalcCrcNetIC               ( uint8_t*              pbData,      uint32_t                                 ulDataLen);
static int32_t  cifXErrorFromTransportState( uint8_t               bState);

static HIL_TRANSPORT_INSTANCE_T s_tHilTransportInst = {0};

/*****************************************************************************/
/*! Global preparation for Transport-Layer. Needs to be called once before running any
*   other HilTransportXX() function!
*   \param hnetXTransport  Handle to the global netXTransport Instance
*   \return NXT_NO_ERROR on success                                          */
/*****************************************************************************/
int32_t HilTransportPrepare( void* hnetXTransport)
{
  s_tHilTransportInst.hNetXTransportHandle = hnetXTransport; /* the handle is required to be able to use User_Trace() */

  return NXT_NO_ERROR;
}

/*****************************************************************************/
/*! Initializes Transport Layer for specific connector.
*   \param ptConnector      Pointer to the connector instance
*   \param szInterfaceName  Name of the interface which should be initialized
*   \param pvConnectorData  Private connector data
*   \param ausSupportedTL   Array of available translation layers
*   \param phTransport      Handle to initialized instance of the transport layer
*   \param pfnDevNotify     Called a device is detected and its translation layer is successfully initialized
*   \return NXT_NO_ERROR on success                                          */
/*****************************************************************************/
int32_t HilTransportInit( PNETX_CONNECTOR_T ptConnector, const char* szInterfaceName, void* pvConnectorData, uint16_t* ausSupportedTL, NXTHANDLE* phTransport, PFN_NETXCON_DEVICE_NOTIFY_CALLBACK pfnDevNotify)
{
  PHIL_TRANSPORT_DATA_T ptTransport = OS_Memalloc(sizeof(HIL_TRANSPORT_DATA_T));
  int32_t               lRet        = NXT_OUT_OF_MEMORY;

  if(NULL != ptTransport)
  {
    OS_Memset(ptTransport, 0, sizeof(*ptTransport));

    TAILQ_INIT(&ptTransport->tRXTransactions);
    ptTransport->hRXTransactionsLock = OS_CreateLock();

    TAILQ_INIT(&ptTransport->tTXTransactions);
    ptTransport->hTXTransactionsLock = OS_CreateLock();

    TAILQ_INIT(&ptTransport->tTLInfoList);

    ptTransport->ptConnector      = ptConnector;
    ptTransport->pvConnectorData  = pvConnectorData;
    OS_Strncpy(ptTransport->szInterfaceName, 16, szInterfaceName, 16);

    /* Try to open the interface */
    if(NULL != (ptTransport->pvConnectorInterface = ptConnector->tFunctions.pfnConCreateInterface(szInterfaceName)))
    {
      /* Try to start interface */
      if(NXT_NO_ERROR == (lRet = ptConnector->tFunctions.pfnConIntfStart(ptTransport->pvConnectorInterface, HilTransportRxData, ptTransport)))
      {
        int32_t         lIdx = 0;
        uint16_t const* ausTL;
        uint32_t        ulTLCount;

        if (NXT_NO_ERROR == (lRet = HilTransportQueryServerData( ptTransport, &ptTransport->tServerInfo.ptQueryServerData)))
        {
          ausTL     = ptTransport->tServerInfo.ptQueryServerData->ausDataTypes;
          ulTLCount = ptTransport->tServerInfo.ptQueryServerData->ulDatatypeCnt;

          ptTransport->tServerInfo.fIgnoreSeqNr        = 0;
          ptTransport->tServerInfo.ulBufferSize        = ptTransport->tServerInfo.ptQueryServerData->ulBufferSize;
          ptTransport->tServerInfo.ulFeatures          = ptTransport->tServerInfo.ptQueryServerData->ulFeatures;
          ptTransport->tServerInfo.ulParallelServices  = ptTransport->tServerInfo.ptQueryServerData->ulParallelServices;

          /* validate feature flags */
          if ((ptTransport->tServerInfo.ulFeatures & HIL_TRANSPORT_FEATURES_KEEPALIVE))
          {
            ptTransport->tKeepAlive.eKeepAliveState = eKEEP_ALIVE_INITIALIZATION;
          }

        } else
        {
          /* No Query server features, so we need to probe every available translation layer */
          ptTransport->tServerInfo.ulBufferSize       = 6000;
          ptTransport->tServerInfo.ulParallelServices = 1;
          ptTransport->tServerInfo.ulFeatures         = 0;
          ptTransport->tServerInfo.fIgnoreSeqNr       = 1;

          ausTL     = ausSupportedTL;
          ulTLCount = 0;
          if (NULL != ausSupportedTL)
          {
            while(ausSupportedTL[ulTLCount] != 0x0000) /* 0x0000 marks end of array */
            {
              ulTLCount++;
            }
          }
        }
        ptTransport->ptDefaultTranslationLayer = NULL;

        for(lIdx = ulTLCount - 1; lIdx >= 0; lIdx--)
        {
          PNETX_TL_INTERFACE_T ptTLInterface = netXTransportGetTranslationLayer(ausTL[lIdx]);
          NXTHANDLE            hTLHandle;

          if(ptTLInterface != NULL)
          {
            uint32_t ulDeviceCount = 0;

            /* check if server supports current translation layer */
            if(NXT_NO_ERROR == (lRet = ptTLInterface->pfnProbe(ptTransport, &hTLHandle, &ulDeviceCount, &ptTransport->tServerInfo)))
            {
              PHIL_TL_INFO_T ptLayerInstance = OS_Memalloc(sizeof(*ptLayerInstance));

              if(NULL == ptLayerInstance)
              {
                /* Out of memory */
                if(g_ulTraceLevel & TRACE_LEVEL_ERROR)
                {
                  USER_Trace( s_tHilTransportInst.hNetXTransportHandle, TRACE_LEVEL_ERROR, "Error allocating memory for Receive-Packet (Packet-ID:%d)!\n", ptTransport->tRxCurrentHeader.bSequenceNr);
                }
                ptTLInterface->pfnRelease( hTLHandle);
              } else
              {
                void*    pvDeviceData       = NULL;
                uint32_t ulDevice           = 0;
                uint32_t ulDeviceIdentifier = 0;
                char     szDeviceName[NXT_MAX_PATH];

                OS_Memset(ptLayerInstance, 0, sizeof(*ptLayerInstance));
                ptLayerInstance->ptTLInterface  = ptTLInterface;
                ptLayerInstance->hTLHandle      = hTLHandle;
                TAILQ_INSERT_TAIL(&ptTransport->tTLInfoList, ptLayerInstance, tList);

                /* TODO: do not set default layer here */
                /* TODO: if we set it in the driver open method it is possible to switch between different data types */
                ptTransport->ptDefaultTranslationLayer = ptLayerInstance;

                /* scan for all available boards end initialize the data typical resources needed for every device */
                while((ulDevice < ulDeviceCount) && (NXT_NO_ERROR == ptTLInterface->pfnScan( hTLHandle, szDeviceName, &ulDeviceIdentifier, &pvDeviceData, ulDevice)))
                {
                  /* notify a new device, to the global management structure in the netXTransport Layer */
                  pfnDevNotify( szInterfaceName, szDeviceName, ulDeviceIdentifier, eNXT_DEVICE_ATTACHED, (void*)ptTransport, pvDeviceData);
                  ulDevice++;
                  pvDeviceData = NULL;
                }
                break;
              }
            }
          }
        }

        if(NULL == ptTransport->ptDefaultTranslationLayer)
        {
          /* we failed to establish a connection to a server */
          ptConnector->tFunctions.pfnConIntfStop( ptTransport->pvConnectorInterface);

          /* No translation layer to communicate, so this must be an unsupported device */
          lRet = NXT_UNSUPPORTED_DEVICE;
        } else
        {
#ifdef NXT_DISABLE_KEEPALIVE
          if(ptTransport->tKeepAlive.eKeepAliveState != eKEEP_ALIVE_UNSUPPORTED)
          {
            USER_Trace( s_tHilTransportInst.hNetXTransportHandle,
                        TRACE_LEVEL_DEBUG,
                        "Disabling Keep-Alive for interface %s\n",
                        szInterfaceName);

            ptTransport->tKeepAlive.eKeepAliveState = eKEEP_ALIVE_UNSUPPORTED;
          }
#endif
          *phTransport = ptTransport;
        }
      }
    }
    /* in case of an error free ptTransport */
    if (lRet != NXT_NO_ERROR && ptTransport != NULL)
      OS_Memfree(ptTransport);
  }
  return lRet;
}

/*****************************************************************************/
/*! De-initializes Transport Layer.
*   \param hTransport  Handle to transport layer instance                    */
/*****************************************************************************/
void HilTransportDeinit( NXTHANDLE hTransport)
{
  PHIL_TRANSPORT_DATA_T ptTransport = (PHIL_TRANSPORT_DATA_T)hTransport;
  PHIL_TL_INFO_T        ptLayerInstance;

  /* Stop the interface */
  ptTransport->ptConnector->tFunctions.pfnConIntfStop(ptTransport->pvConnectorInterface);

  HilTransportStopPendingTransactions( hTransport);

  /* Deinit and delete all translation layer */
  while(NULL != (ptLayerInstance = TAILQ_FIRST(&ptTransport->tTLInfoList)))
  {
    /* Call deinit on translation layer */
    PNETX_TL_INTERFACE_T ptTLInterface = ptLayerInstance->ptTLInterface;

    ptTLInterface->pfnRelease( ptLayerInstance->hTLHandle);

    TAILQ_REMOVE(&ptTransport->tTLInfoList, ptLayerInstance, tList);
    OS_Memfree(ptLayerInstance);
  }
  if(NULL != ptTransport->hRXTransactionsLock)
  {
    OS_DeleteLock(ptTransport->hRXTransactionsLock);
    ptTransport->hRXTransactionsLock = NULL;
  }

  if(NULL != ptTransport->hTXTransactionsLock)
  {
    OS_DeleteLock(ptTransport->hTXTransactionsLock);
    ptTransport->hTXTransactionsLock = NULL;
  }

  if(NULL != ptTransport->tServerInfo.ptQueryServerData)
  {
    OS_Memfree(ptTransport->tServerInfo.ptQueryServerData);
    ptTransport->tServerInfo.ptQueryServerData = NULL;
  }

  OS_Memfree(ptTransport);
}

/*****************************************************************************/
/*! HiltTransport initialization for specific connector, in case of a reconnect.
    (Called in case of netXTransport-Server based endpoint)
*   \param hTransport          Handle to transport layer instance
*   \param ulDeviceIdentifier  Unique identifier of the device which should be reconnected
*   \param pfnDevNotify        Called everytime a device is detected and its translation layer is successfully initialized
*   \return NXT_NO_ERROR on success                                          */
/*****************************************************************************/
int32_t HilTransportSoftReconnect( NXTHANDLE hTransport, uint32_t ulDeviceIdentifier, PFN_NETXCON_DEVICE_NOTIFY_CALLBACK pfnDevNotify)
{
  PHIL_TRANSPORT_DATA_T ptTransport = (PHIL_TRANSPORT_DATA_T)hTransport;
  int32_t               lRet        = NXT_OUT_OF_MEMORY;

  if(NULL != ptTransport)
  {
    PHIL_TL_INFO_T ptLayerInstance = ptTransport->ptDefaultTranslationLayer;

    if (ptLayerInstance != NULL)
    {
      PNETX_TL_INTERFACE_T ptTLInterface = ptLayerInstance->ptTLInterface;

      if(ptTLInterface != NULL)
      {
        uint32_t  ulDeviceCount = 0;
        NXTHANDLE hTmpTLHandle  = NULL;

        /* check if server supports current translation layer */
        if(NXT_NO_ERROR == (lRet = ptTLInterface->pfnProbe( ptTransport, &hTmpTLHandle, &ulDeviceCount, &ptTransport->tServerInfo)))
        {
          void*    pvDeviceData  = NULL;
          uint32_t ulDevice      = 0;
          uint32_t ulDeviceIdent = 0;
          char     szDeviceName[NXT_MAX_PATH];

          /* scan for all available boards end initialize the data typical resources needed for every device */
          while((ulDevice < ulDeviceCount) && (NXT_NO_ERROR == ptTLInterface->pfnScan( ptLayerInstance->hTLHandle, szDeviceName, &ulDeviceIdent, &pvDeviceData, ulDevice)))
          {
            if (ulDeviceIdentifier == ulDeviceIdent)
            {
              /* notify a new device, to the global management structure in the netXTransport Layer */
              pfnDevNotify( ptTransport->szInterfaceName, szDeviceName, ulDeviceIdentifier, eNXT_DEVICE_ATTACHED, (void*)ptTransport, pvDeviceData);
              break;
            }
            ulDevice++;
            pvDeviceData = NULL;
          }
        }
      }
    }
  }
  return lRet;
}

/*****************************************************************************/
/*! Restores the connection states (open active connections and update internal resources).
*   \param hnetXTransportDevHandle  Hanlde to netXTransport device
*   \param hTransport               Handle to the Transport Layer            */
/*****************************************************************************/
void HilTransportRestoreConnectionStates ( NXTHANDLE hnetXTransportDevHandle, NXTHANDLE hTransport)
{
  PHIL_TRANSPORT_DATA_T ptTransport     = hTransport;
  PHIL_TL_INFO_T        ptLayerInstance = ptTransport->ptDefaultTranslationLayer;

  ptLayerInstance->ptTLInterface->pfnRestoreCon( hnetXTransportDevHandle);
}

/*****************************************************************************/
/*! De-queues remaining transactions of the transport layer instance pointed by hTransport.
*   \param hTransport  Handle of transport layer                             */
/*****************************************************************************/
void HilTransportStopPendingTransactions(NXTHANDLE hTransport)
{
  PHIL_TRANSPORT_DATA_T             ptTransport = (PHIL_TRANSPORT_DATA_T)hTransport;
  PHIL_TRANSPORT_TRANSACTION_DATA_T ptTransaction;

  OS_EnterLock(ptTransport->hTXTransactionsLock);
  /* clear all pending transactions */
  TAILQ_FOREACH(ptTransaction, &ptTransport->tTXTransactions, tList)
  {
    if (ptTransaction->fPending)
    {
      TAILQ_REMOVE(&ptTransport->tTXTransactions, ptTransaction, tList);
      ptTransaction->fPending = 0;
      ptTransaction->lError   = CIFX_TRANSPORT_CONNECT;
      OS_SetEvent(ptTransaction->pvComplete);
    }
  }
  OS_LeaveLock(ptTransport->hTXTransactionsLock);

  OS_EnterLock(ptTransport->hRXTransactionsLock);
  TAILQ_FOREACH(ptTransaction, &ptTransport->tRXTransactions, tList)
  {
    if (ptTransaction->fPending)
    {
      TAILQ_REMOVE(&ptTransport->tRXTransactions, ptTransaction, tList);
      ptTransaction->fPending   = 0;
      ptTransaction->lError     = CIFX_TRANSPORT_CONNECT;
      ptTransaction->fCancelled = 1;
      OS_SetEvent(ptTransaction->pvComplete);
    }
  }
  OS_LeaveLock(ptTransport->hRXTransactionsLock);
}

/*****************************************************************************/
/*! Returns handle of the currently used default translation layer.
*   \param hTransport  Handle to transport layer instance
*   \return NULL on failure                                                  */
/*****************************************************************************/
NXTHANDLE HilTransportGetTLHandle( NXTHANDLE hTransport)
{
  PHIL_TRANSPORT_DATA_T ptTransport = (PHIL_TRANSPORT_DATA_T)hTransport;

  if ((ptTransport != NULL) && (ptTransport->ptDefaultTranslationLayer != NULL))
    return ptTransport->ptDefaultTranslationLayer->hTLHandle;
  else
    return NULL;
}

/*****************************************************************************/
/*! Retrieves data type of the currently used default translation layer.
*   \param hTransport  Handle to transport layer instance
*   \return Data Type (e.g. 0x100, 0x200, ...)                               */
/*****************************************************************************/
uint16_t HilTransportGetDefaultTLType( NXTHANDLE hTransport)
{
  PHIL_TRANSPORT_DATA_T ptTransport = (PHIL_TRANSPORT_DATA_T)hTransport;

  return ptTransport->ptDefaultTranslationLayer->ptTLInterface->usDataType;
}

/*****************************************************************************/
/*! Allocates and initializes a HilTransportPacket.
*   \param hTransport  Handle to transport layer instance
*   \param ulDataSize   Size of data section
*   \param usDataType   Type of packet (according to the used translation layer)
*   \return Pointer to the allocated packet                                  */
/*****************************************************************************/
PHIL_TRANSPORT_PACKET_T HilTransportAllocatePacket(NXTHANDLE hTransport, uint32_t ulDataSize, uint16_t usDataType)
{
  PHIL_TRANSPORT_PACKET_T ptPacket = OS_Memalloc(sizeof(*ptPacket) + sizeof(*ptPacket->ptHeader) + ulDataSize);

  if(NULL != ptPacket)
  {
    OS_Memset(ptPacket, 0, sizeof(*ptPacket));
    ptPacket->hTransport = hTransport;

    ptPacket->ptHeader   = (PHIL_TRANSPORT_HEADER)(ptPacket + 1);
    OS_Memset(ptPacket->ptHeader, 0, sizeof(*ptPacket->ptHeader));

    ptPacket->pbData               = (uint8_t*)(ptPacket->ptHeader + 1);
    ptPacket->ulMaxSize            = ulDataSize;
    ptPacket->ptHeader->ulCookie   = HIL_TRANSPORT_COOKIE;
    ptPacket->ptHeader->usDataType = usDataType;
    ptPacket->ptHeader->usChecksum = 0; /* we don't have a checksum yet */
  }
  return ptPacket;
}

/*****************************************************************************/
/*! Frees a previous allocated HilTransportPacket (see HilTransportAllocatePacket())
*   \param ptPacket  Pointer to packet                                       */
/*****************************************************************************/
void HilTransportFreePacket(PHIL_TRANSPORT_PACKET_T ptPacket)
{
  OS_Memfree(ptPacket);
}

/*****************************************************************************/
/*! HilTransport receive function. Parses incoming HilTransport data and forwards
    the data if the HilTransportPacket is complete.
*   \param pabRxBuffer  Pointer to receive buffer
*   \param ulReadLen    Size of buffer pabRxBuffer
*   \param pvUser       User data                                            */
/*****************************************************************************/
static void HilTransportRxData(uint8_t* pabRxBuffer, uint32_t ulReadLen, void* pvUser)
{
  PHIL_TRANSPORT_DATA_T ptTransport = (PHIL_TRANSPORT_DATA_T)pvUser;

  /* Parse incoming HilTransport Data and pass them to appropriate translation layer, */
  /* ACKS must be passed to HilTransportSendData to indicate send complete            */
  while(ulReadLen > 0)
  {
    switch(ptTransport->eRxState)
    {
    case eWAIT_FOR_COOKIE:
      ptTransport->tRxCurrentHeader.ulCookie <<= 8;
      ptTransport->tRxCurrentHeader.ulCookie |= *pabRxBuffer;

      ptTransport->ulRXCurrentOffset++;

      if(ptTransport->ulRXCurrentOffset >= sizeof(ptTransport->tRxCurrentHeader.ulCookie))
      {
        /* Cookie complete, so check it */
        if(HIL_TRANSPORT_COOKIE == ptTransport->tRxCurrentHeader.ulCookie)
        {
          /* Header complete */
          ptTransport->eRxState = eWAIT_FOR_HEADER;
        } else
        {
          /* Header incomplete, wait for next byte to check cookie again */
          ptTransport->ulRXCurrentOffset = 3;
        }
      }
      pabRxBuffer++;
      ulReadLen--;
      break;

    case eWAIT_FOR_HEADER:
      {
        uint32_t ulRemaining = sizeof(ptTransport->tRxCurrentHeader) - ptTransport->ulRXCurrentOffset;
        uint32_t ulCopySize  = ulRemaining;
        uint8_t* pbHeader    = (uint8_t*)&ptTransport->tRxCurrentHeader;

        if(ulCopySize > ulReadLen)
          ulCopySize = ulReadLen;

        OS_Memcpy(pbHeader + ptTransport->ulRXCurrentOffset, pabRxBuffer, ulCopySize);

        ptTransport->ulRXCurrentOffset += ulCopySize;
        pabRxBuffer                    += ulCopySize;
        ulReadLen                      -= ulCopySize;

        if(ptTransport->ulRXCurrentOffset >= sizeof(ptTransport->tRxCurrentHeader))
        {
          /* Header complete, so check it */
          if(ptTransport->tRxCurrentHeader.ulLength > 0)
          {
            ptTransport->eRxState = eWAIT_FOR_PACKET_COMPLETE;

            /* Allocate packet buffer */
            ptTransport->ptCurrentRxPacket = HilTransportAllocatePacket(ptTransport, ptTransport->tRxCurrentHeader.ulLength, 0);

            if(NULL == ptTransport->ptCurrentRxPacket)
            {
              /* Out of memory */
              if(g_ulTraceLevel & TRACE_LEVEL_ERROR)
              {
                USER_Trace( s_tHilTransportInst.hNetXTransportHandle, TRACE_LEVEL_ERROR, "Error allocating memory for Receive-Packet (Packet-ID:%d)!\n", ptTransport->tRxCurrentHeader.bSequenceNr);
              }
            } else
            {
              OS_Memcpy(ptTransport->ptCurrentRxPacket->ptHeader, &ptTransport->tRxCurrentHeader, sizeof(ptTransport->tRxCurrentHeader));
            }
          } else
          {
            switch(ptTransport->tRxCurrentHeader.usDataType)
            {
            case HIL_TRANSPORT_TYPE_ACKNOWLEDGE:
              {
                PHIL_TRANSPORT_TRANSACTION_DATA_T ptTransaction;

                /* Handle acknowledge */
                OS_EnterLock(ptTransport->hTXTransactionsLock);

                TAILQ_FOREACH(ptTransaction, &ptTransport->tTXTransactions, tList)
                {
                  if(ptTransaction->uPacket.ptSendPacket->bSequenceNr == ptTransport->tRxCurrentHeader.bSequenceNr)
                  {
                    TAILQ_REMOVE(&ptTransport->tTXTransactions, ptTransaction, tList);
                    ptTransaction->fPending = 0;
                    ptTransaction->lError   = cifXErrorFromTransportState(ptTransport->tRxCurrentHeader.bState);
                    OS_SetEvent(ptTransaction->pvComplete);
                    break;
                  }
                }

                OS_LeaveLock(ptTransport->hTXTransactionsLock);
              }
              break;

            case HIL_TRANSPORT_TYPE_KEEP_ALIVE:
              break;

            default:
              /* Invalid packet received */
              break;
            }

            /* Reset RX State machine for next packet */
            ptTransport->tRxCurrentHeader.ulCookie = 0;
            ptTransport->ulRXCurrentOffset         = 0;
            ptTransport->ulDataOffset              = 0;
            ptTransport->eRxState                  = eWAIT_FOR_COOKIE;

          }
        }
      }
      break;

    case eWAIT_FOR_PACKET_COMPLETE:
      {
        /* Store packet data */
        PHIL_TRANSPORT_PACKET_T ptRxPacket      = ptTransport->ptCurrentRxPacket;
        uint32_t                ulRemaining     = ptRxPacket->ptHeader->ulLength - ptTransport->ulDataOffset;
        uint32_t                ulCopySize      = ulRemaining;
        int                     fDispatchPacket = 1;

        if(ulCopySize > ulReadLen)
          ulCopySize = ulReadLen;

        OS_Memcpy(ptRxPacket->pbData + ptTransport->ulDataOffset, pabRxBuffer, ulCopySize);

        ulReadLen                 -= ulCopySize;
        pabRxBuffer               += ulCopySize;
        ptTransport->ulDataOffset += ulCopySize;

        if(ptTransport->ulDataOffset >= ptTransport->tRxCurrentHeader.ulLength)
        {
          /* Packet is finished, validate Checksum */
          if(ptTransport->tRxCurrentHeader.usChecksum != 0)
          {
            /* First we check against the correct CRC16 checksum. If it does not match it might
               be an old netIC Firmware which uses a wrong checksum calculation */
            if( (ptTransport->tRxCurrentHeader.usChecksum != CalcCrc16(ptRxPacket->pbData,
                                                                       ptRxPacket->ptHeader->ulLength)) &&
                (ptTransport->tRxCurrentHeader.usChecksum != CalcCrcNetIC(ptRxPacket->pbData,
                                                                         ptRxPacket->ptHeader->ulLength)) )
            {
              /* Send negative acknowledge */
              HilTransportSendAcknowledge(ptTransport, &ptTransport->tRxCurrentHeader, HIL_TSTATE_CHECKSUM_ERROR);
              fDispatchPacket = 0;
            }
          }

          if(fDispatchPacket)
          {
            PHIL_TRANSPORT_TRANSACTION_DATA_T ptTransaction;

            /* Check if this belongs to a transaction */
            OS_EnterLock(ptTransport->hRXTransactionsLock);

            TAILQ_FOREACH(ptTransaction, &ptTransport->tRXTransactions, tList)
            {
              if( (ptTransaction->uPacket.ptSendPacket->bChannel   == ptTransport->ptCurrentRxPacket->ptHeader->bChannel) &&
                  (ptTransaction->uPacket.ptSendPacket->bDevice    == ptTransport->ptCurrentRxPacket->ptHeader->bDevice) &&
                  (ptTransaction->uPacket.ptSendPacket->usDataType == ptTransport->ptCurrentRxPacket->ptHeader->usDataType) )
              {
                if (  ptTransport->tServerInfo.fIgnoreSeqNr ||
                      (ptTransaction->uPacket.ptSendPacket->bSequenceNr == ptTransport->ptCurrentRxPacket->ptHeader->bSequenceNr) )
                {
                  TAILQ_REMOVE(&ptTransport->tRXTransactions, ptTransaction, tList);

                  ptTransaction->fPending             = 0;
                  ptTransaction->uPacket.ptRecvPacket = ptTransport->ptCurrentRxPacket;

                  OS_SetEvent(ptTransaction->pvComplete);
                  fDispatchPacket = 0;
                  break;
                }
              }
            }

            OS_LeaveLock(ptTransport->hRXTransactionsLock);

            if(fDispatchPacket)
            {
              int fFreePacket = 1;

              /* Dispatch packet to default transport */
              if(ptTransport->ptDefaultTranslationLayer != NULL)
              {
                PHIL_TL_INFO_T ptDefaultTranslationLayer = ptTransport->ptDefaultTranslationLayer;

                if(ptTransport->tRxCurrentHeader.usDataType == ptDefaultTranslationLayer->ptTLInterface->usDataType)
                {
                  ptDefaultTranslationLayer->ptTLInterface->pfnReceive(ptDefaultTranslationLayer->hTLHandle,
                                                          ptTransport->ptCurrentRxPacket);
                  fFreePacket = 0;
                }
              }

              if(fFreePacket)
                HilTransportFreePacket(ptTransport->ptCurrentRxPacket);

              ptTransport->ptCurrentRxPacket = NULL;
            } else
            {
              HilTransportSendAcknowledge(ptTransport, &ptTransport->tRxCurrentHeader, 0);
            }
          } else
          {
            HilTransportFreePacket(ptTransport->ptCurrentRxPacket);
            ptTransport->ptCurrentRxPacket = NULL;
          }

          /* Reset RX State machine for next packet */
          ptTransport->tRxCurrentHeader.ulCookie = 0;
          ptTransport->ulRXCurrentOffset         = 0;
          ptTransport->ulDataOffset              = 0;
          ptTransport->eRxState                  = eWAIT_FOR_COOKIE;
        }
      }
      break;

    default:
      if(g_ulTraceLevel & TRACE_LEVEL_ERROR)
      {
        USER_Trace( s_tHilTransportInst.hNetXTransportHandle, TRACE_LEVEL_ERROR, "Error in HilTransport receive state machine! Current state (0xd) not known!", ptTransport->eRxState);
      }

      pabRxBuffer += ulReadLen;
      ulReadLen   = 0;
      break;
    }
  }

  /* store last receive time stamp */
  ptTransport->tKeepAlive.ulLastKeepAlive = OS_GetMilliSecCounter();
}

/*****************************************************************************/
/*! HILTransport Keep-Alive function. Manages Keep-Alive handling (Keep-Alive initialization,
    Keep-Alive messages, notifies interrupted connections).
*   \param hTransport            Handle to transport layer instance
*    return KEEPALIVE_STATE_E                                                 */
/******************************************************************************/
KEEPALIVE_STATE_E HilTransportHandleKeepAlive(NXTHANDLE hTransport)
{
  PHIL_TRANSPORT_DATA_T   ptTransport     = (PHIL_TRANSPORT_DATA_T)hTransport;
  PHIL_TRANSPORT_PACKET_T ptSendPacket    = NULL;
  HIL_TRANSPORT_PACKET_T* ptRecvPacket    = NULL;
  uint32_t                lRet            = 0;
  KEEPALIVE_STATE_E       eKeepAliveState = eKEEP_ALIVE_UNSUPPORTED;

  if (ptTransport->tKeepAlive.eKeepAliveState == eKEEP_ALIVE_UNSUPPORTED)
    return ptTransport->tKeepAlive.eKeepAliveState;

  ptSendPacket = HilTransportAllocatePacket( hTransport, sizeof(uint32_t), HIL_TRANSPORT_TYPE_KEEP_ALIVE);

  ptSendPacket->ptHeader->ulLength   = sizeof(HIL_TRANSPORT_KEEPALIVE_DATA_T);
  OS_Memcpy ( ptSendPacket->pbData, &ptTransport->tKeepAlive.ulKeepAliveIdent, sizeof(HIL_TRANSPORT_KEEPALIVE_DATA_T));


  switch(ptTransport->tKeepAlive.eKeepAliveState)
  {
   case eKEEP_ALIVE_UNSUPPORTED:
      /* Nothing to do, as the device does not support keep alive */
    break;

    case eKEEP_ALIVE_INITIALIZATION:
    {
      ptTransport->tKeepAlive.ulKeepAliveIdent = 0;

      if (CIFX_NO_ERROR != (lRet = HilTransportTransferPacket( hTransport, ptSendPacket->ptHeader, &ptRecvPacket, 1000)))
      {
        if(g_ulTraceLevel & TRACE_LEVEL_INFO)
        {
          USER_Trace( s_tHilTransportInst.hNetXTransportHandle, TRACE_LEVEL_INFO, "No keep alive support for interface %s!", ptTransport->szInterfaceName);
        }
        /* We were unable to get a keep valid alive sequence number,
          so we assume the device does not support keep alive correctly */
        ptTransport->tKeepAlive.eKeepAliveState = eKEEP_ALIVE_UNSUPPORTED;

      } else
      {
        PHIL_TRANSPORT_KEEPALIVE_DATA_T ptKeepAliveData = (PHIL_TRANSPORT_KEEPALIVE_DATA_T)ptRecvPacket->pbData;

        if (ptRecvPacket->ptHeader->bState == HIL_TRANSPORT_STATE_OK)
        {
          /* Store COMId for verification */
          ptTransport->tKeepAlive.ulKeepAliveIdent   = ptKeepAliveData->ulComID;
          ptTransport->tKeepAlive.eKeepAliveState    = eKEEP_ALIVE_ACTIVE;
          ptTransport->tKeepAlive.ulKeepAliveTimeout = TRANSPORT_TO_TRANSFER;
          ptTransport->tKeepAlive.ulLastKeepAlive    = OS_GetMilliSecCounter();
        } else
        {
          if(g_ulTraceLevel & TRACE_LEVEL_INFO)
          {
            USER_Trace( s_tHilTransportInst.hNetXTransportHandle, TRACE_LEVEL_INFO, "No keep alive support for interface %s!", ptTransport->szInterfaceName);
          }
          /* error handling KeepAlive request */
          ptTransport->tKeepAlive.eKeepAliveState = eKEEP_ALIVE_UNSUPPORTED;
        }
      }
      if (NULL != ptRecvPacket)
      {
        HilTransportFreePacket( ptRecvPacket);
      }
    }
    break;

    case eKEEP_ALIVE_ACTIVE:
    {
      uint32_t ulTimeDiff = OS_GetMilliSecCounter() - ptTransport->tKeepAlive.ulLastKeepAlive;

      if(ulTimeDiff > ptTransport->tKeepAlive.ulKeepAliveTimeout)
      {
        HIL_TRANSPORT_PACKET_T* ptRecvPacket = NULL;

        if (CIFX_NO_ERROR != HilTransportTransferPacket( hTransport, ptSendPacket->ptHeader, &ptRecvPacket, 1000))
        {
          if(g_ulTraceLevel & TRACE_LEVEL_INFO)
          {
            USER_Trace( s_tHilTransportInst.hNetXTransportHandle, TRACE_LEVEL_INFO, "Stop interface %s due to failed keep alive request.", ptTransport->szInterfaceName);
          }

          /* Assume Device disconnect, as it does not answer */
          ptTransport->tKeepAlive.eKeepAliveState = eKEEP_ALIVE_TIMEOUT;
        } else
        {
          ptTransport->tKeepAlive.ulLastKeepAlive = OS_GetMilliSecCounter();
        }
        if (NULL != ptRecvPacket)
        {
          HilTransportFreePacket( ptRecvPacket);
        }
      }
    }
    break;

    case eKEEP_ALIVE_TIMEOUT:
    break;

    default:
    break;
  }
  if (ptSendPacket != NULL)
  {
    HilTransportFreePacket( ptSendPacket);
  }

  eKeepAliveState = ptTransport->tKeepAlive.eKeepAliveState;

  return eKeepAliveState;
}

/*****************************************************************************/
/*! Handles HilTransport Acknowlegde messages.
*    \param  hTransport  Handle of the transport layer instance
*    \param  ptHeader    Pointer to the HilTransport Ack-Packet header
*    \param  bState      Acknowledge state                                  */
/****************************************************************************/
void HilTransportSendAcknowledge(NXTHANDLE hTransport, PHIL_TRANSPORT_HEADER ptHeader, uint8_t bState)
{
  HIL_TRANSPORT_HEADER  tACKPacket;
  PHIL_TRANSPORT_DATA_T ptTransport = (PHIL_TRANSPORT_DATA_T)hTransport;

  OS_Memset(&tACKPacket, 0, sizeof(tACKPacket));

  tACKPacket.ulCookie      = HIL_TRANSPORT_COOKIE;
  tACKPacket.usDataType    = HIL_TRANSPORT_TYPE_ACKNOWLEDGE;
  tACKPacket.bChannel      = ptHeader->bChannel;
  tACKPacket.bDevice       = ptHeader->bDevice;
  tACKPacket.bSequenceNr   = ptHeader->bSequenceNr;
  tACKPacket.usTransaction = ptHeader->usTransaction;
  tACKPacket.bState        = bState;

  /* Send acknowledge */
  ptTransport->ptConnector->tFunctions.pfnConIntfSend( ptTransport->pvConnectorInterface,
                                                    (uint8_t*)&tACKPacket,
                                                    sizeof(tACKPacket));
}

/*****************************************************************************/
/*! Sends HilTransportPacket via the corresponding connector.
*   \param hTransport  Handle of the transport layer instance
*   \param ptPacket    Pointer to packet, to be send
*   \param ulTimeout   Timeout (to wait for acknowlegde)
*   \return NXT_NO_ERROR on success                                          */
/*****************************************************************************/
int32_t HilTransportSendPacket( NXTHANDLE hTransport, PHIL_TRANSPORT_HEADER ptPacket, uint32_t ulTimeout)
{
  int32_t                          lRet        = NXT_OUT_OF_MEMORY;
  PHIL_TRANSPORT_DATA_T            ptTransport = (PHIL_TRANSPORT_DATA_T)hTransport;
  HIL_TRANSPORT_TRANSACTION_DATA_T tTransaction;

  OS_Memset(&tTransaction, 0, sizeof(tTransaction));
  tTransaction.pvComplete = OS_CreateEvent();

  if (ptTransport->tServerInfo.fIgnoreSeqNr)
  {
    /* Old netIC Firmware might be uses a wrong checksum calculation,
       so we don't calculate the checksum */
    ptPacket->usChecksum = 0x0000;
  } else
  {
    /* Calculate packet checksum */
    ptPacket->usChecksum = CalcCrc16((uint8_t*)(ptPacket + 1), ptPacket->ulLength);
  }

  /* We need to set a unique sequence number to make sure we can assign the ACK correctly */
  ptPacket->bSequenceNr = ptTransport->bLastSequenceNr++;
  if (ptTransport->usLastTransaction++ == 0x7FFF)
    ptTransport->usLastTransaction = 1;

  ptPacket->usTransaction = ptTransport->usLastTransaction;

  if(NULL != tTransaction.pvComplete)
  {
    NETX_CONNECTOR_FUNCTION_TABLE* ptConnFuncs = &ptTransport->ptConnector->tFunctions;
    uint32_t                       ulSendSize  = sizeof(*ptPacket) + ptPacket->ulLength;

    tTransaction.uPacket.ptSendPacket = ptPacket;
    tTransaction.fPending             = 1;
    tTransaction.lError               = NXT_NO_ERROR;

    OS_EnterLock(ptTransport->hTXTransactionsLock);

    TAILQ_INSERT_TAIL(&ptTransport->tTXTransactions, &tTransaction, tList);

    if(NXT_NO_ERROR != (lRet = ptConnFuncs->pfnConIntfSend(ptTransport->pvConnectorInterface,
                                                        (uint8_t*)ptPacket,
                                                        ulSendSize)))
    {
      if(g_ulTraceLevel & TRACE_LEVEL_INFO)
      {
        USER_Trace( s_tHilTransportInst.hNetXTransportHandle, TRACE_LEVEL_INFO, "Connector-Send() failed for packet with ID: %d (Error = 0x%X)!", ptPacket->bSequenceNr, lRet);
      }

      TAILQ_REMOVE(&ptTransport->tTXTransactions, &tTransaction, tList);
    }

    OS_LeaveLock(ptTransport->hTXTransactionsLock);

    if(NXT_NO_ERROR == lRet)
    {
      uint32_t ulConTimeout = 0;

      netXTransportGetConnectorTimeout( ptTransport->ptConnector, &ulConTimeout);

      /* Wait for ACK */
      if(OS_EVENT_SIGNALLED == OS_WaitEvent(tTransaction.pvComplete, ulTimeout+ulConTimeout))
      {
        /* We got the event, so this transaction is queued out and the error code has been set */
        lRet = tTransaction.lError;

      } else
      {
        /* Timeout, so return error and dequeue transaction */
        OS_EnterLock(ptTransport->hTXTransactionsLock);

        /* Only remove it from list, if it's still pending */
        if(tTransaction.fPending)
          TAILQ_REMOVE(&ptTransport->tTXTransactions, &tTransaction, tList);

        OS_LeaveLock(ptTransport->hTXTransactionsLock);

        lRet = NXT_SEND_TIMEOUT;
      }
    }

    OS_DeleteEvent(tTransaction.pvComplete);
  }

  return lRet;
}

/*****************************************************************************/
/*! Transfers HilTransportPacket (waits for response).
*   \param hTransport     Handle of the transport layer instance
*   \param ptSendPacket   Pointer to packet, to be send
*   \param pptRecvPacket  Reference to pointer for receive packet
*   \param ulTimeout      Timeout (to wait for response)
*   \return NXT_NO_ERROR on success                                          */
/*****************************************************************************/
int32_t HilTransportTransferPacket(NXTHANDLE hTransport, HIL_TRANSPORT_HEADER* ptSendPacket, PHIL_TRANSPORT_PACKET_T* pptRecvPacket, uint32_t ulTimeout)
{
  int32_t                          lRet        = NXT_OUT_OF_MEMORY;
  PHIL_TRANSPORT_DATA_T            ptTransport = (PHIL_TRANSPORT_DATA_T)hTransport;
  HIL_TRANSPORT_TRANSACTION_DATA_T tTransaction;

  OS_Memset(&tTransaction, 0, sizeof(tTransaction));

  tTransaction.pvComplete = OS_CreateEvent();

  if(NULL != tTransaction.pvComplete)
  {
    uint32_t ulConTimeout = 0;

    tTransaction.fPending             = 1;
    tTransaction.lError               = 0;
    tTransaction.uPacket.ptSendPacket = ptSendPacket;

    OS_EnterLock(ptTransport->hRXTransactionsLock);
    TAILQ_INSERT_TAIL(&ptTransport->tRXTransactions, &tTransaction, tList);
    OS_LeaveLock(ptTransport->hRXTransactionsLock);

    netXTransportGetConnectorTimeout( ptTransport->ptConnector, &ulConTimeout);

    if(NXT_NO_ERROR == (lRet = HilTransportSendPacket(hTransport, ptSendPacket, ulTimeout)))
    {
      if(OS_EVENT_TIMEOUT == OS_WaitEvent(tTransaction.pvComplete, ulTimeout + ulConTimeout))
      {
        lRet = NXT_RECV_TIMEOUT;
      } else
      {
        if (tTransaction.fCancelled == 0)
        {
          *pptRecvPacket = tTransaction.uPacket.ptRecvPacket;
        } else
        {
          lRet = NXT_TRANSACTION_CANCELLED;
        }
      }
    } else
    {
      if(g_ulTraceLevel & TRACE_LEVEL_INFO)
      {
        USER_Trace( s_tHilTransportInst.hNetXTransportHandle, TRACE_LEVEL_INFO, "HilTransportSendPacket() failed (lRet = 0x%X) for packet with ID: %d!", lRet, ptSendPacket->bSequenceNr);
      }
    }

    OS_EnterLock(ptTransport->hRXTransactionsLock);
    if(tTransaction.fPending)
      TAILQ_REMOVE(&ptTransport->tRXTransactions, &tTransaction, tList);
    OS_LeaveLock(ptTransport->hRXTransactionsLock);

    OS_DeleteEvent(tTransaction.pvComplete);
  }

  return lRet;
}

/*****************************************************************************/
/*! Queries the server information.
*   \param ptTransport   Pointer to transport layer information structure
*   \param ptServerInfo  Pointer to server information structure
*   \return NXT_NO_ERROR on success                                          */
/*****************************************************************************/
static int32_t HilTransportQueryServerData( PHIL_TRANSPORT_DATA_T ptTransport, PHIL_TRANSPORT_ADMIN_QUERYSERVER_DATA_T* pptServerInfo)
{
  int32_t                 lRet         = -1;
  PHIL_TRANSPORT_PACKET_T ptRecvPacket = NULL;
  HIL_TRANSPORT_HEADER    tQuery       = {0};

  tQuery.ulCookie      = HIL_TRANSPORT_COOKIE;
  tQuery.usDataType    = HIL_TRANSPORT_TYPE_QUERYSERVER;
  tQuery.usTransaction = 1;

  *pptServerInfo = NULL;

  if(NXT_NO_ERROR == (lRet = HilTransportTransferPacket( ptTransport, &tQuery, &ptRecvPacket, 1000)))
  {
    lRet = -1;
    if (ptRecvPacket->ptHeader->ulLength >= sizeof(HIL_TRANSPORT_ADMIN_QUERYSERVER_DATA_T))
    {
      if(NULL != (*pptServerInfo = OS_Memalloc(ptRecvPacket->ptHeader->ulLength)))
      {
        OS_Memcpy( *pptServerInfo, ptRecvPacket->pbData, ptRecvPacket->ptHeader->ulLength);
        lRet = NXT_NO_ERROR;
      }
    }
  }

  if(NULL != ptRecvPacket)
    HilTransportFreePacket(ptRecvPacket);

  return lRet;
}

/*****************************************************************************/
/*! Calculate CRC16 checksum for HIL_TRANSPORT_HEADER
 *   \param  pbData     Data pointer
 *   \param  ulDataLen  Length of data
 *   \return CRC16 Checksum                                                 */
/****************************************************************************/
static uint16_t CalcCrc16(uint8_t* pbData, uint32_t ulDataLen)
{
  /* CRC16-CCIT Polynom */
  uint16_t usCrcValue = 0xFFFF;
  uint32_t ulIdx;

  for(ulIdx = 0; ulIdx < ulDataLen; ulIdx++)
  {
    usCrcValue  = (usCrcValue >> 8) | (usCrcValue << 8);
    usCrcValue ^= pbData[ulIdx];
    usCrcValue ^= (usCrcValue & 0xFF) >> 4;
    usCrcValue ^= (usCrcValue << 8) << 4;
    usCrcValue ^= ((usCrcValue & 0xFF) << 4) << 1;
  }
  return ~usCrcValue;
}

/****************************************************************************/
/*! This is a checksum used on first release netIC Firmware version. It is NO Crc checksum,
 *  but we need it to be "old" compatible.
 *   \param  pbData     Data pointer
 *   \param  ulDataLen  Length of data
 *   \return netIC Checksum                                                 */
/****************************************************************************/
static uint16_t CalcCrcNetIC(uint8_t* pbData, uint32_t ulDataLen)
{
  uint16_t usCrcValue = 0xFFFF;
  uint32_t ulIdx;

  #define lo8(_x_) (uint8_t)(_x_ & 0xFF)
  #define hi8(_x_) (uint8_t)((_x_ >> 8) & 0xFF)

  for(ulIdx = 0; ulIdx < ulDataLen; ulIdx++)
  {
    uint8_t data = pbData[ulIdx];

    data ^= lo8(usCrcValue);
    data ^= data << 4;

    usCrcValue = ((((uint16_t)data << 8) | hi8(usCrcValue)) ^ (uint8_t)(data >> 4) ^ ((uint16_t)data << 3));
  }

  return usCrcValue;
}

/****************************************************************************/
/*! Create a CIFX error from a HIL transport state
 *   \param  bState  State from the HIL transport header
 *   \return corresponding CIFX error code for bState                       */
/****************************************************************************/
static int32_t cifXErrorFromTransportState(uint8_t bState)
{
  int32_t lRet;

  switch(bState)
  {
  case HIL_TRANSPORT_STATE_OK:
    lRet = CIFX_NO_ERROR;
    break;

  case HIL_TSTATE_CHECKSUM_ERROR:
    lRet = CIFX_TRANSPORT_CHECKSUM_ERROR;
    break;

  case HIL_TSTATE_LENGTH_INCOMPLETE:
    lRet = CIFX_TRANSPORT_LENGTH_INCOMPLETE;
    break;

  case HIL_TSTATE_DATA_TYPE_UNKNOWN:
    lRet = CIFX_TRANSPORT_DATA_TYPE_UNKOWN;
    break;

  case HIL_TSTATE_DEVICE_UNKNOWN:
    lRet = CIFX_TRANSPORT_DEVICE_UNKNOWN;
    break;

  case HIL_TSTATE_CHANNEL_UNKNOWN:
    lRet = CIFX_TRANSPORT_CHANNEL_UNKNOWN;
    break;

  case HIL_TSTATE_SEQUENCE_ERROR:
    lRet = CIFX_TRANSPORT_SEQUENCE;
    break;

  case HIL_TSTATE_BUFFEROVERFLOW_ERROR:
    lRet = CIFX_TRANSPORT_BUFFEROVERFLOW;
    break;

  case HIL_TSTATE_KEEP_ALIVE_ERROR:
    lRet = CIFX_TRANSPORT_KEEPALIVE;
    break;

  case HIL_TSTATE_RESOURCE_ERROR:
    lRet = CIFX_TRANSPORT_RESOURCE;
    break;

  default:
    lRet = CIFX_TRANSPORT_ERROR_UNKNOWN;
    break;
  }

  return lRet;
}