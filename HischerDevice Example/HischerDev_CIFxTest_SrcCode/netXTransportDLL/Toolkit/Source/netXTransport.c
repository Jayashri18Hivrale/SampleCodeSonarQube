/**************************************************************************************

   Copyright (c) Hilscher GmbH. All Rights Reserved.

 **************************************************************************************

   Filename:
    $Id: netXTransport.c 194 2013-06-27 10:23:42Z sebastiand $
   Last Modification:
    $Author: sebastiand $
    $Date: 2013-06-27 12:23:42 +0200 (Do, 27. Jun 2013) $
    $Revision: 194 $

   Targets:
     O/S Independent : yes

   Description:
    netX Transport Basic initialization

   Changes:

     Version   Date        Author   Description
     ----------------------------------------------------------------------------------
      1        23.02.13    SD/MT       initial version

**************************************************************************************/

/****************************************************************************/
/*! \file netXTransport.c
    netX Transport Basic initialization                                     */
/****************************************************************************/

#include "netXTransport.h"
#include "HilTransportLayer.h"
#include "OS_Dependent.h"
#include "OS_Includes.h"
#include "TranslationLayer.h"
#include <sys/queue.h>

uint32_t g_ulTraceLevel = TRACE_LEVEL_ERROR;  /*!< Tracelevel used by the toolkit */

/*****************************************************************************/
/*! Global netXTransport instance information                                */
/*****************************************************************************/
static NETX_TRANSPORT_DATA_T s_tNetXTransportInfo;

/* static function declaration */
static void      netXTransportInterfaceNotification( const char* szInterfaceName, NETX_INTERFACE_NOTIFICATION_E eNotify, void* pvConnectorData, void* pvUser);
static void      DeviceNotifyCallback              ( const char* szInterfaceName, const char* szDeviceName, uint32_t ulDeviceIdentifier, NETX_INTERFACE_NOTIFICATION_E eNotify, void* hTransport, void* pvDevicePrivateData);
static void      netXTransportDeviceStateChange    ( PNETX_TRANSPORT_INTERFACE_T ptInterface, PNETX_TRANSPORT_GENERIC_DEVICE_T ptGenericDevice, char* szDeviceName, uint32_t ulDeviceIdentifier, NETX_INTERFACE_NOTIFICATION_E eNotify, NXTHANDLE hTransport, void* pvDevicePrivateData);
static NXTHANDLE netXTransportCreateDeviceHandle   ( NXTHANDLE hNetXTransportDevGroup, PNETX_TRANSPORT_GENERIC_DEVICE_T ptGenericDevice);
static void      netXTransportFreeDeviceHandle     ( NXTHANDLE hnetXTransportDeviceHandle);
static int       netXTransportRemoveInterface      ( PNETX_TRANSPORT_INTERFACE_T ptInterface);
static void      netXTransportCleanUpInterface     ( PNETX_TRANSPORT_INTERFACE_T ptInterface);
static void      netXTransportCleanUpConnector     ( void);
static void      netXTransportRemoveConnectorSec   ( PNETX_CONNECTOR_T ptConnector, int fLockRemove);

/*****************************************************************************/
/*! Initializes the netXTransport Toolkit.
*   \param ptTLInitTable    Initialization table of supported translation layers (currently provided data types: 0x100, 0x200)
*   \param ulInitTableSize  Size of table ptTLInitTable
*   \return NXT_NO_ERROR on success                                          */
/*****************************************************************************/
int32_t APIENTRY netXTransportInit( TL_INIT_T atTLInitTable[], uint32_t ulInitTableSize)
{
  int32_t    lRet          = NXT_NO_ERROR;
  uint32_t   ulTLCounter   = 0;
  PTL_INIT_T ptTLInitTable = atTLInitTable;

  if ((ptTLInitTable == NULL) || (ulInitTableSize == 0))
    return NXT_INVALID_POINTER;

  /* initialize global lists */
  TAILQ_INIT(&s_tNetXTransportInfo.tConnectors);                 /* list of available connector (e.g. TCP, RS232/USB, ...)                                                 */
  TAILQ_INIT(&s_tNetXTransportInfo.tnetXTransportInterface);     /* list of all interfaces with responsible endpoint (server or single device)) (e.g. TCP0, TCP1, COM0,...)*/
  TAILQ_INIT(&s_tNetXTransportInfo.tnetXTransportDevGroupList);  /* list of all Translation-Layer types                                                                    */

  s_tNetXTransportInfo.pvnetXTransportLock = OS_CreateLock();

  /* Initialize user trace for debug messages */
  USER_TraceInitialize( &s_tNetXTransportInfo);

  s_tNetXTransportInfo.ptTLInitTable   = ptTLInitTable;
  s_tNetXTransportInfo.ulInitTableSize = ulInitTableSize;

  /* initialize all given translation layers */
  /* NULL defines last item in list          */
  for (ulTLCounter=0;ulTLCounter<(ulInitTableSize/sizeof(TL_INIT_T));ulTLCounter++)
  {
    if ((ptTLInitTable != NULL) && (ptTLInitTable->pfnTLInit != NULL))
    {
      lRet = ptTLInitTable->pfnTLInit( ptTLInitTable->pvData);
    }
    ptTLInitTable++;
    ulTLCounter++;
  }
  if (lRet == NXT_NO_ERROR)
  {
    s_tNetXTransportInfo.eState = eNETXTRANSPORT_INITIALIZED;
  }
  return lRet;
}

/*****************************************************************************/
/*! De-initializes netXTransport Toolkit.                                    */
/*****************************************************************************/
void APIENTRY netXTransportDeinit( void)
{
  PNETX_TRANSPORT_INTERFACE_T ptInterface = NULL;

  /* check if there are still remaining active connectors */
  netXTransportStop();
  netXTransportCleanUpConnector();

  /* remove interface resources */
  while (NULL != (ptInterface = TAILQ_FIRST(&s_tNetXTransportInfo.tnetXTransportInterface)))
  {
    netXTransportCleanUpInterface( ptInterface);
  }

  /* remove translation layer */
  if (NULL != s_tNetXTransportInfo.ptTLInitTable)
  {
    uint32_t   ulTLCounter   = 0;
    PTL_INIT_T ptTLInitTable = s_tNetXTransportInfo.ptTLInitTable;

    /* NULL defines last item in list */
    for (ulTLCounter=0;ulTLCounter<(s_tNetXTransportInfo.ulInitTableSize/sizeof(TL_INIT_T));ulTLCounter++)
    {
      if ((ptTLInitTable != NULL) && (ptTLInitTable->pfnTLDeInit != NULL))
      {
        ptTLInitTable->pfnTLDeInit( ptTLInitTable->pvData);
      }
      ptTLInitTable++;
      ulTLCounter++;
    }
  }
  USER_TraceDeInitialize( &s_tNetXTransportInfo);

  if(NULL != s_tNetXTransportInfo.pvnetXTransportLock)
  {
    OS_DeleteLock( s_tNetXTransportInfo.pvnetXTransportLock);
    s_tNetXTransportInfo.pvnetXTransportLock = NULL;
  }

  OS_Memset(&s_tNetXTransportInfo, 0, sizeof(s_tNetXTransportInfo));
}

/*****************************************************************************/
/*! Starts netXTransport Toolkit (device discovering process). Starts all registered connector
    (see netXTransportAddConnector()).
*   \return NXT_NO_ERROR on success                                          */
/*****************************************************************************/
int32_t APIENTRY netXTransportStart( PFN_DEVICEBROWSE_CALLBACK pfnCallback, void* pvUser)
{
  int32_t           lRet        = NXT_NO_ERROR;
  PNETX_CONNECTOR_T ptConnector = NULL;

  if (s_tNetXTransportInfo.eState == eNETXTRANSPORT_STARTED)
    return NXT_NO_ERROR;

  OS_EnterLock( s_tNetXTransportInfo.pvnetXTransportLock);

  /* prepare HiltTransport-Layer */
  HilTransportPrepare( &s_tNetXTransportInfo);

  if (pfnCallback != NULL)
  {
    /* register device discover status callback */
    s_tNetXTransportInfo.pfnDeviceDiscoverCB       = pfnCallback;
    s_tNetXTransportInfo.pvDeviceDiscoverUserParam = pvUser;
  }

  /* initialize all registered connector */
  TAILQ_FOREACH(ptConnector, &s_tNetXTransportInfo.tConnectors, tList)
  {
    if(NXT_NO_ERROR != (lRet = ptConnector->tFunctions.pfnConOpen(netXTransportInterfaceNotification, ptConnector)))
    {
      if(g_ulTraceLevel & TRACE_LEVEL_ERROR)
      {
        USER_Trace( &s_tNetXTransportInfo, TRACE_LEVEL_ERROR, "Failed to start connector 0x%p (ConnectorOpen() returned: 0x%0.8X)", ptConnector, lRet);
      }
      break;
    } else
    {
      if(g_ulTraceLevel & TRACE_LEVEL_ERROR)
      {
        USER_Trace( &s_tNetXTransportInfo, TRACE_LEVEL_INFO, "Successfully started connector (0x%p)", ptConnector);
      }
    }
  }
  if(NXT_NO_ERROR == lRet)
  {
    s_tNetXTransportInfo.eState = eNETXTRANSPORT_STARTED;
  }
  OS_LeaveLock( s_tNetXTransportInfo.pvnetXTransportLock);

  if(NXT_NO_ERROR != lRet)
  {
    netXTransportStop();
    netXTransportCleanUpConnector();
  }
  return lRet;
}

/*****************************************************************************/
/*! Stops netXTransport Toolkit.                                             */
/*****************************************************************************/
void APIENTRY netXTransportStop(void)
{
  PNETX_CONNECTOR_T ptConnector;

  if (s_tNetXTransportInfo.eState == eNETXTRANSPORT_UNINITIALIZED)
    return;

  OS_EnterLock( s_tNetXTransportInfo.pvnetXTransportLock);

  if (s_tNetXTransportInfo.eState == eNETXTRANSPORT_STARTED)
  {
    /* de-initialize all registerd connector */
    TAILQ_FOREACH(ptConnector, &s_tNetXTransportInfo.tConnectors, tList)
    {
      /* fLockRemove=0 -> we already locked the list so do not lock remove */
      if (NULL != ptConnector->tFunctions.pfnConClose)
      {
        ptConnector->tFunctions.pfnConClose();
      }
    }
    s_tNetXTransportInfo.eState = eNETXTRANSPORT_STOPPED;
  }
  OS_LeaveLock( s_tNetXTransportInfo.pvnetXTransportLock);
}

/*****************************************************************************/
/*! Adds connector instance to netXTransport Toolkit.
*   \param ptConnector  Pointer to connector, which should be added
*   \return NXT_NO_ERROR on success                                          */
/*****************************************************************************/
int32_t APIENTRY netXTransportAddConnector(PNETX_CONNECTOR_T ptConnector)
{
  int32_t           lRet   = NXT_NO_ERROR;
  NETX_CONNECTOR_T* ptTemp = NULL;

  ptConnector->tFunctions.pfnConGetIdentifier(ptConnector->szConIdentifier, &ptConnector->tConnectorUUID);

  OS_EnterLock( s_tNetXTransportInfo.pvnetXTransportLock);

  /* check for dublicate connector registration */
  TAILQ_FOREACH(ptTemp, &s_tNetXTransportInfo.tConnectors, tList)
  {
    if(0 == OS_Strcmp(ptConnector->szConIdentifier, ptTemp->szConIdentifier))
    {
      /* Duplicate identifier */
      lRet = NXT_DUPLICATE_CONN_IDENTIFIER;

      if(g_ulTraceLevel & TRACE_LEVEL_ERROR)
      {
        USER_Trace( &s_tNetXTransportInfo, TRACE_LEVEL_ERROR, "Connector already registered 0x%p (lRet=0x%0.8X)", ptConnector, lRet);
      }
      break;
    } else if(0 == OS_Memcmp(&ptConnector->tConnectorUUID, &ptTemp->tConnectorUUID, sizeof(ptConnector->tConnectorUUID)))
    {
      /* Duplicate UUID */
      lRet = NXT_DUPLICATE_CONN_UUID;

      if(g_ulTraceLevel & TRACE_LEVEL_ERROR)
      {
        USER_Trace( &s_tNetXTransportInfo, TRACE_LEVEL_ERROR, "Connector already registered 0x%p (lRet=0x%0.8X)", ptConnector, lRet);
      }
      break;
    }
  }

  OS_LeaveLock( s_tNetXTransportInfo.pvnetXTransportLock);

  /* no error? -> register connetor */
  if(NXT_NO_ERROR == lRet)
  {
    TAILQ_INSERT_TAIL(&s_tNetXTransportInfo.tConnectors, ptConnector, tList);

    if(g_ulTraceLevel & TRACE_LEVEL_ERROR)
    {
      USER_Trace( &s_tNetXTransportInfo, TRACE_LEVEL_ERROR, "Connector succesfully added 0x%p", ptConnector);
    }
  }

  return lRet;
}

/*****************************************************************************/
/*! Removes previously registered connector (see netXTransportAddConnector())
*   \param ptConnector  Pointer to connector, which should be removed
*   \param fLockRemove set != 0 if remove process should be locked           */
/*****************************************************************************/
static void netXTransportRemoveConnectorSec(PNETX_CONNECTOR_T ptConnector, int fLockRemove)
{
  if (s_tNetXTransportInfo.eState == eNETXTRANSPORT_UNINITIALIZED)
    return;

   if (s_tNetXTransportInfo.eState == eNETXTRANSPORT_STOPPED)
  {
    if(g_ulTraceLevel & TRACE_LEVEL_INFO)
    {
      USER_Trace( &s_tNetXTransportInfo, TRACE_LEVEL_INFO, "Stopped conncetor (0x%p)", ptConnector);
    }

    if (fLockRemove) OS_EnterLock( s_tNetXTransportInfo.pvnetXTransportLock);

    /* remove connector from global list */
    TAILQ_REMOVE(&s_tNetXTransportInfo.tConnectors, ptConnector, tList);

    if (fLockRemove) OS_LeaveLock( s_tNetXTransportInfo.pvnetXTransportLock);
   }
}


/*****************************************************************************/
/*! Removes previously registered connector (see netXTransportAddConnector())
*   \param ptConnector  Pointer to connector, which should be removed        */
/*****************************************************************************/
void APIENTRY netXTransportRemoveConnector(PNETX_CONNECTOR_T ptConnector)
{
  if (s_tNetXTransportInfo.eState == eNETXTRANSPORT_UNINITIALIZED)
    return;

   netXTransportRemoveConnectorSec( ptConnector, 1);
}

/*****************************************************************************/
/*! Removes all previously registered connector (see netXTransportAddConnector())*/
/*****************************************************************************/
static void netXTransportCleanUpConnector(void)
{
  PNETX_CONNECTOR_T ptConnector;

  if (s_tNetXTransportInfo.eState == eNETXTRANSPORT_UNINITIALIZED)
    return;

  OS_EnterLock( s_tNetXTransportInfo.pvnetXTransportLock);

  /* de-initialize all registerd connector */
  TAILQ_FOREACH(ptConnector, &s_tNetXTransportInfo.tConnectors, tList)
  {
    /* fLockRemove=0 -> we already locked the list so do not lock remove */
    netXTransportRemoveConnectorSec(ptConnector, 0);
  }
  s_tNetXTransportInfo.eState = eNETXTRANSPORT_STOPPED;

  OS_LeaveLock( s_tNetXTransportInfo.pvnetXTransportLock);
}

/*****************************************************************************/
/*! Adds translation layer.
*   \param ptTLInterface           Pointer to translation layer, which should be registerd
*   \param phNetXTransportDevGroup Returned handle to the created netXTransport device group
*   \return NXT_NO_ERROR on success                                          */
/*****************************************************************************/
int32_t APIENTRY netXTransportAddTranslationLayer(PNETX_TL_INTERFACE_T ptTLInterface, NXTHANDLE* phNetXTransportDevGroupHandle)
{
  NETX_TRANSPORT_DEV_GROUP_T* ptTemp = NULL;
  int32_t                     lRet   = NXT_NO_ERROR;

  OS_EnterLock( s_tNetXTransportInfo.pvnetXTransportLock);

  /* check if translation layer is already registered */
  TAILQ_FOREACH(ptTemp, &s_tNetXTransportInfo.tnetXTransportDevGroupList, tList)
  {
    if(ptTLInterface->usDataType == ptTemp->ptTLInterface->usDataType)
    {
      /* Duplicate data type  */
      lRet = NXT_DUPLICATE_TL_DATATYPE;

      if(g_ulTraceLevel & TRACE_LEVEL_ERROR)
      {
        USER_Trace( &s_tNetXTransportInfo, TRACE_LEVEL_ERROR, "Translation-Layer already registered 0x%p,Type=0x%X (lRet=0x%0.8X)",
                    ptTLInterface,
                    ptTLInterface->usDataType,
                    lRet);
      }
      break;
    }
  }
  OS_LeaveLock( s_tNetXTransportInfo.pvnetXTransportLock);

  if(NXT_NO_ERROR == lRet)
  {
    /* create new instance related to the current Translation-Layer */
    PNETX_TRANSPORT_DEV_GROUP_T ptnetXTransportDevGroup = (PNETX_TRANSPORT_DEV_GROUP_T)OS_Memalloc(sizeof(NETX_TRANSPORT_DEV_GROUP_T));

    TAILQ_INSERT_TAIL(&s_tNetXTransportInfo.tnetXTransportDevGroupList, ptnetXTransportDevGroup, tList);

    /* initialize device list (contains all devices, which use the current Translation-Layer for communication) */
    TAILQ_INIT(&ptnetXTransportDevGroup->tGenericDeviceList);
    ptnetXTransportDevGroup->ptTLInterface = ptTLInterface;

    /* store pointer to netXTransport instance */
    ptTLInterface->hNetXTransport = &s_tNetXTransportInfo;

    *phNetXTransportDevGroupHandle = (void*)ptnetXTransportDevGroup;

    if(g_ulTraceLevel & TRACE_LEVEL_INFO)
    {
      USER_Trace( &s_tNetXTransportInfo, TRACE_LEVEL_INFO, "Successfully added Translation-Layer (0x%p,Type=0x%X)",
                    ptTLInterface,
                    ptTLInterface->usDataType);
    }
  }
  return lRet;
}

/*****************************************************************************/
/*! Removes previously registered translation layer (see netXTransportAddTranslationLayer())
*   \param hNetXTransportDevGroup Handle of the netXTransport device group   */
/*****************************************************************************/
void APIENTRY netXTransportRemoveTranslationLayer(NXTHANDLE hNetXTransportDevGroup)
{
  PNETX_TRANSPORT_DEV_GROUP_T      ptNetXTransportDevGroup = (PNETX_TRANSPORT_DEV_GROUP_T)hNetXTransportDevGroup;
  PNETX_TRANSPORT_GENERIC_DEVICE_T ptGenericDevice         = {0};

  if (s_tNetXTransportInfo.eState == eNETXTRANSPORT_UNINITIALIZED)
    return;

  if (NULL != ptNetXTransportDevGroup)
  {
    /* clean up connected devices */
    while(NULL != (ptGenericDevice = TAILQ_FIRST( &ptNetXTransportDevGroup->tGenericDeviceList)))
    {
      TAILQ_REMOVE(&ptNetXTransportDevGroup->tGenericDeviceList, ptGenericDevice, tList);

      if(g_ulTraceLevel & TRACE_LEVEL_INFO)
      {
        USER_Trace( &s_tNetXTransportInfo, TRACE_LEVEL_INFO, "Remove device %s (0x%p)",
                ptGenericDevice->szDeviceName,
                ptGenericDevice);
      }

      netXTransportFreeDeviceHandle( ptGenericDevice->hNetXTransportDeviceHandle);
      OS_Memfree(ptGenericDevice);
      ptGenericDevice = NULL;
    }

    OS_EnterLock( s_tNetXTransportInfo.pvnetXTransportLock);
    TAILQ_REMOVE(&s_tNetXTransportInfo.tnetXTransportDevGroupList, ptNetXTransportDevGroup, tList);
    OS_LeaveLock( s_tNetXTransportInfo.pvnetXTransportLock);

    if(g_ulTraceLevel & TRACE_LEVEL_INFO)
    {
      USER_Trace( &s_tNetXTransportInfo, TRACE_LEVEL_INFO, "Successfully removed Translation-Layer (0x%p,Type=0x%X)",
                ptNetXTransportDevGroup->ptTLInterface,
                ptNetXTransportDevGroup->ptTLInterface->usDataType);
    }

    OS_Memfree(ptNetXTransportDevGroup);
  }
}

/*****************************************************************************/
/*! Returns an array of the currently registered translation layers (0x0000 marks the last item in the list).
*   \return Array of supported data types (e.g. [0x100 0x200])               */
/*****************************************************************************/
uint16_t* netXTransportGetSupportedTranslationLayer(void)
{
  PNETX_TRANSPORT_DEV_GROUP_T ptTemp                       = NULL;
  uint16_t*                   ausSupportedTranslationLayer = NULL;
  int                         iCount                       = 0;

  TAILQ_FOREACH(ptTemp, &s_tNetXTransportInfo.tnetXTransportDevGroupList, tList)
  {
    iCount++;
    ausSupportedTranslationLayer = (uint16_t*)OS_Memrealloc( ausSupportedTranslationLayer, iCount*sizeof(uint16_t));
    if (ausSupportedTranslationLayer)
    {
      ausSupportedTranslationLayer[iCount-1] = ptTemp->ptTLInterface->usDataType;
    }
  }
  /* add last item in list (0) */
  if ((ausSupportedTranslationLayer) && (NULL != (ausSupportedTranslationLayer = (uint16_t*)OS_Memrealloc( ausSupportedTranslationLayer, ((iCount+1)*sizeof(uint16_t))))))
  {
    ausSupportedTranslationLayer[iCount] = 0x0000;
  }
  return ausSupportedTranslationLayer;
}

/*****************************************************************************/
/*! Returns pointer to translation layer of the requested type.
*   \param usTLType  Type of the requested translation layer
*   \return Pointer to appropriate translation layer                         */
/*****************************************************************************/
PNETX_TL_INTERFACE_T netXTransportGetTranslationLayer( uint16_t usTLType)
{
  PNETX_TRANSPORT_DEV_GROUP_T ptTemp = NULL;
  PNETX_TL_INTERFACE_T        ptRet  = NULL;

  TAILQ_FOREACH(ptTemp, &s_tNetXTransportInfo.tnetXTransportDevGroupList, tList)
  {
    if(ptTemp->ptTLInterface->usDataType == usTLType)
    {
      ptRet = ptTemp->ptTLInterface;
      break;
    }
  }
  return ptRet;
}

/*****************************************************************************/
/*! Returns handle to translation layer of specific type.
*   \param usTLType  Type of the requested translation layer (e.g. 0x100, 0x200)
*   \return Pointer to handle of the requested translation layer             */
/*****************************************************************************/
NXTHANDLE netXTransportGetHandleToDeviceGroup( uint16_t usTLType)
{
  PNETX_TRANSPORT_DEV_GROUP_T ptTemp = NULL;
  PNETX_TRANSPORT_DEV_GROUP_T ptRet  = NULL;

  OS_EnterLock( s_tNetXTransportInfo.pvnetXTransportLock);

  /* check if translation layer of the requested type is registered */
  TAILQ_FOREACH(ptTemp, &s_tNetXTransportInfo.tnetXTransportDevGroupList, tList)
  {
    if(usTLType == ptTemp->ptTLInterface->usDataType)
    {
      ptRet = ptTemp;
      break;
    }
  }
  OS_LeaveLock( s_tNetXTransportInfo.pvnetXTransportLock);

  return ptRet;
}

/*****************************************************************************/
/*! netXTransport device handle                                              */
/*****************************************************************************/
typedef struct NETXTRANSPORT_DEVICE_HANDLE_Ttag
{
  PNETX_TRANSPORT_DEV_GROUP_T      ptNetXTransportDevGroup; /*!< Pointer to netXTransport device group (Translation-Layer), the device is related to */
  PNETX_TRANSPORT_GENERIC_DEVICE_T ptGenericDevice;         /*!< Pointer to netXTransport generic device                                             */

} NETXTRANSPORT_DEVICE_HANDLE_T;

/*****************************************************************************/
/*! Creates a netXTransport device handle for specific device.
*   \param hNetXTransportDevGroup  Handle of the netXTransport device group, the device should be added
*   \param ptGenericDevice         Pointer to the netXTransport device
*   \return NULL on failure                                                  */
/*****************************************************************************/
static NXTHANDLE netXTransportCreateDeviceHandle( NXTHANDLE hNetXTransportDevGroup, PNETX_TRANSPORT_GENERIC_DEVICE_T ptGenericDevice)
{
  PNETX_TRANSPORT_DEV_GROUP_T    ptNetXTransportDevGroup     = (PNETX_TRANSPORT_DEV_GROUP_T)hNetXTransportDevGroup;
  NETXTRANSPORT_DEVICE_HANDLE_T* ptNetXTransportDeviceHandle = NULL;

  if (NULL == ptNetXTransportDevGroup)
    return NULL;

  if (NULL != (ptNetXTransportDeviceHandle = (NETXTRANSPORT_DEVICE_HANDLE_T*)OS_Memalloc(sizeof(NETXTRANSPORT_DEVICE_HANDLE_T))))
  {
    ptNetXTransportDeviceHandle->ptNetXTransportDevGroup = ptNetXTransportDevGroup;
    ptNetXTransportDeviceHandle->ptGenericDevice         = ptGenericDevice;
    ptGenericDevice->hNetXTransportDeviceHandle          = ptNetXTransportDeviceHandle;
  }
  return (NXTHANDLE)ptNetXTransportDeviceHandle;
}

/*****************************************************************************/
/*! Frees netXTransport device handle of previously requested one (netXTransportCreateDeviceHandle()).
*   \param hnetXTransportHandle  Handle which should be freed                */
/*****************************************************************************/
static void netXTransportFreeDeviceHandle( NXTHANDLE hnetXTransportHandle)
{
  if (hnetXTransportHandle != NULL)
    OS_Memfree( hnetXTransportHandle);
}

/*****************************************************************************/
/*! Returns the number of registered devices of the given instance.
*   \param hNetXTransportDevGroup  Handle to the netXTransport device group
*   \return Number of registered devices                                     */
/*****************************************************************************/
int32_t netXTransportGetDeviceCount(NXTHANDLE hNetXTransportDevGroup)
{
  PNETX_TRANSPORT_DEV_GROUP_T ptTemp        = (PNETX_TRANSPORT_DEV_GROUP_T)hNetXTransportDevGroup;
  uint32_t                    ulDeviceCount = 0;

  if (NULL != hNetXTransportDevGroup)
  {
    PNETX_TRANSPORT_GENERIC_DEVICE_T ptGenericDevice = NULL;

    TAILQ_FOREACH(ptGenericDevice, &ptTemp->tGenericDeviceList, tList)
    {
      ulDeviceCount++;
    }
  }
  return ulDeviceCount;
}

/*****************************************************************************/
/*! Returns netXTransport device handle for specific device.
*   \param szDeviceName                Name of the requested device
*   \param hNetxTransportDeviceHandle  Returned handle to requested device
*   \return NXT_NO_ERROR on success                                          */
/*****************************************************************************/
int32_t netXTransportGetDeviceHandle( char* szDeviceName, NXTHANDLE* hNetxTransportDeviceHandle)
{
  NETX_TRANSPORT_INTERFACE_T* ptTemp = NULL;
  int32_t                     lRet   = NXT_RESOURCE_NOT_FOUND;

  TAILQ_FOREACH( ptTemp, &s_tNetXTransportInfo.tnetXTransportInterface, tList)
  {
    NETX_TRANSPORT_DEVICE_REF_T* ptDeviceHandleRef = NULL;

    TAILQ_FOREACH( ptDeviceHandleRef, &ptTemp->tDeviceRefList, tList)
    {
      NETXTRANSPORT_DEVICE_HANDLE_T* ptNetXTransportDeviceHandle = (NETXTRANSPORT_DEVICE_HANDLE_T*)ptDeviceHandleRef->hNetXTransportDeviceHandle;
      if(0 == OS_Strcmp( ptNetXTransportDeviceHandle->ptGenericDevice->szDeviceName, szDeviceName))
      {
        *hNetxTransportDeviceHandle = (NXTHANDLE)ptNetXTransportDeviceHandle;
        lRet                        = NXT_NO_ERROR;
        break;
      }
    }
  }
  return lRet;
}

/*****************************************************************************/
/*! Returns a handle to a device, identified using the device index.
*   \param hNetXTransportDevGroup  Handle to the netXTransport devic group
*   \param ulDeviceNo              Number of the requested device
*   \return Handle to the netXTrasnport device                               */
/*****************************************************************************/
NXTHANDLE netXTransportGetDeviceHandleByNumber( NXTHANDLE hNetXTransportDevGroup, uint32_t ulDeviceNo)
{
  PNETX_TRANSPORT_DEV_GROUP_T      ptTemp          = (PNETX_TRANSPORT_DEV_GROUP_T)hNetXTransportDevGroup;
  PNETX_TRANSPORT_GENERIC_DEVICE_T ptGenericDevice = NULL;
  NXTHANDLE                        ptRet           = NULL;
  uint32_t                         ulDeviceCount   = 0;

  TAILQ_FOREACH(ptGenericDevice, &ptTemp->tGenericDeviceList, tList)
  {
    if (ulDeviceCount >= ulDeviceNo)
    {
      ptRet = ptGenericDevice->hNetXTransportDeviceHandle;
      break;
    }
    ulDeviceCount++;
  }
  return ptRet;
}

/*****************************************************************************/
/*! Returns a pointer to a netXTransport device, identified via device handle.
*   \param hNetxTransportDeviceHandle  Handle to the netXTransport device
*   \param szDeviceName                Pointer to buffer for device name     */
/*****************************************************************************/
void netXTransportGetDeviceName( NXTHANDLE hNetxTransportDeviceHandle, char* szDeviceName, uint32_t ulStrlen)
{
  if ((hNetxTransportDeviceHandle != NULL) && (szDeviceName != NULL))
  {
    NETXTRANSPORT_DEVICE_HANDLE_T* ptNetXTransportDeviceHandle = (NETXTRANSPORT_DEVICE_HANDLE_T*)hNetxTransportDeviceHandle;
    OS_Strncpy( szDeviceName, ulStrlen, ptNetXTransportDeviceHandle->ptGenericDevice->szDeviceName, OS_Strlen(ptNetXTransportDeviceHandle->ptGenericDevice->szDeviceName));
  }
}

/*****************************************************************************/
/*! Increments the interface ulActiveTransfers counter.
*   \param ptInterface Pointer to interface                                  */
/*****************************************************************************/
static void netXTransportAttachInterface( PNETX_TRANSPORT_INTERFACE_T ptInterface)
{
  OS_EnterLock( ptInterface->pvInterfaceLock);

  ptInterface->ulActiveTransfers++;

  OS_LeaveLock( ptInterface->pvInterfaceLock);
}

/*****************************************************************************/
/*! Decrements the interface ulActiveTransfers counter.
*   \param ptInterface Pointer to interface                                  */
/*****************************************************************************/
static void netXTransportDetachInterface( PNETX_TRANSPORT_INTERFACE_T ptInterface)
{
  OS_EnterLock( ptInterface->pvInterfaceLock);

  ptInterface->ulActiveTransfers--;

  OS_LeaveLock( ptInterface->pvInterfaceLock);
}

/*****************************************************************************/
/*! Checks if the device is accessible. If the device is accessible the device
    reference counter becomes incremented (see netXTransportDetachDevice()).
*   \param hNetxTransportDeviceHandle handle to the netXTransport device
*   \return 1 if the device is accessible                                    */
/*****************************************************************************/
int netXTransportAttachDevice( NXTHANDLE hNetxTransportDeviceHandle)
{
  NETXTRANSPORT_DEVICE_HANDLE_T* ptNetXTransportDevice = (NETXTRANSPORT_DEVICE_HANDLE_T*)hNetxTransportDeviceHandle;
  int                            fRet                  = 0;

  if ((ptNetXTransportDevice == NULL) || (ptNetXTransportDevice->ptGenericDevice == NULL))
    return fRet;

  OS_EnterLock( ptNetXTransportDevice->ptGenericDevice->pvDeviceLock);

  if (ptNetXTransportDevice->ptGenericDevice->eState == eDEVICE_STARTED)
  {
    fRet = 1;

    /* we have to lock the device, but count the reference on the interface */
    netXTransportAttachInterface((PNETX_TRANSPORT_INTERFACE_T)ptNetXTransportDevice->ptGenericDevice->pvInterface);
  }

  OS_LeaveLock( ptNetXTransportDevice->ptGenericDevice->pvDeviceLock);

  /* if the device is not available, try to reconnect */
  if (fRet == 0)
  {
    /* we have to sleep for while, the hardware may not be quick enough to be responsible */
    OS_Sleep(500);
    netXTransportReconnectDevice( hNetxTransportDeviceHandle);
  }

  return fRet;
}

/*****************************************************************************/
/*! Decrement reference counter of the device (see netXTransportAttachDevice())
*   \param  hNetxTransportDeviceHandle  Handle to the netXTransport device   */
/*****************************************************************************/
void netXTransportDetachDevice( NXTHANDLE hNetxTransportDeviceHandle)
{
  NETXTRANSPORT_DEVICE_HANDLE_T* ptNetXTransportDevice = (NETXTRANSPORT_DEVICE_HANDLE_T*)hNetxTransportDeviceHandle;

  if ((ptNetXTransportDevice == NULL) || (ptNetXTransportDevice->ptGenericDevice == NULL))
    return;

  /* we have to lock the device, but count the reference on the interface */
  OS_EnterLock( ptNetXTransportDevice->ptGenericDevice->pvDeviceLock);

  /* store the reference */
  netXTransportDetachInterface( (PNETX_TRANSPORT_INTERFACE_T)ptNetXTransportDevice->ptGenericDevice->pvInterface);

  OS_LeaveLock( ptNetXTransportDevice->ptGenericDevice->pvDeviceLock);
}

/*****************************************************************************/
/*! Returns pointer the private data of the corresponding translation layer.
*   \param hNetxTransportDeviceHandle  Handle to the netXTransport device
*   \return NULL on failure                                                  */
/*****************************************************************************/
void* netXTransportGetTLInfo(NXTHANDLE hNetxTransportDeviceHandle)
{
  NXTHANDLE hRet = NULL;

  if (hNetxTransportDeviceHandle != NULL)
  {
    NETXTRANSPORT_DEVICE_HANDLE_T* ptNetXTransportDeviceHandle = (NETXTRANSPORT_DEVICE_HANDLE_T*)hNetxTransportDeviceHandle;
    hRet = HilTransportGetTLHandle(ptNetXTransportDeviceHandle->ptGenericDevice->hTransport);
  }
  return (void*)hRet;
}

/*****************************************************************************/
/*! Returns pointer to device specific private translation layer data.
*   \param hNetxTransportDeviceHandle  Handle to the netXTransport device
*   \return NULL on failure                                                  */
/*****************************************************************************/
void* netXTransportGetTLDeviceData(NXTHANDLE hNetxTransportDeviceHandle)
{
  NXTHANDLE pvRet = NULL;

  if (hNetxTransportDeviceHandle != NULL)
  {
    NETXTRANSPORT_DEVICE_HANDLE_T* ptNetXTransportDeviceHandle = (NETXTRANSPORT_DEVICE_HANDLE_T*)hNetxTransportDeviceHandle;

    pvRet = ptNetXTransportDeviceHandle->ptGenericDevice->pvTLDeviceData;
  }
  return pvRet;
}

/*****************************************************************************/
/*! Retrieves the function pointer table of corresponding translation layer.
*   \param hNetxTransportDeviceHandle  Handle to a netXTransport device
*   \param pfnDriverFunc               Ref to pointer for function table
*   \return NXT_NO_ERROR on success                                          */
/*****************************************************************************/
int32_t netXTransportGetTLFctTableByDevice( NXTHANDLE hNetxTransportDeviceHandle, FUNCTION_POINTER_TABLE_T* pfnDriverFunc)
{
  int32_t lRet = NXT_INVALID_HANDLE;

  if (hNetxTransportDeviceHandle == NULL)
  {
    return lRet;
  } else
  {
    PNETX_TRANSPORT_DEV_GROUP_T ptNetXTransportDevGroup = ((NETXTRANSPORT_DEVICE_HANDLE_T*)hNetxTransportDeviceHandle)->ptNetXTransportDevGroup;
    lRet = ptNetXTransportDevGroup->ptTLInterface->pfnMapFunctionTable( pfnDriverFunc);
  }
  return lRet;
}

/*****************************************************************************/
/*! Retrieves a function pointer table of translation layer.
*   \param hNetXTransportDevGroup  Handle to the requested device group
*   \param pfnDriverFunc           Ref to pointer for function table
*   \return NXT_NO_ERROR on success                                          */
/*****************************************************************************/
int32_t netXTransportGetTLFctTable( NXTHANDLE hNetXTransportDevGroup, FUNCTION_POINTER_TABLE_T* pfnDriverFunc)
{
  int32_t lRet = NXT_INVALID_HANDLE;

  if (hNetXTransportDevGroup == NULL)
  {
    return lRet;
  } else
  {
    PNETX_TRANSPORT_DEV_GROUP_T ptNetXTransportDevGroup = (PNETX_TRANSPORT_DEV_GROUP_T)hNetXTransportDevGroup;
    lRet = ptNetXTransportDevGroup->ptTLInterface->pfnMapFunctionTable(pfnDriverFunc);
  }

  return lRet;
}

/*****************************************************************************/
/*! Search for the interface given by the name szInterfaceName
*   \param szInterfaceName  Name of the interface to search for
*   \return NULL on error                                                    */
/*****************************************************************************/
PNETX_TRANSPORT_INTERFACE_T netXTransportSearchInterface(const char* szInterfaceName)
{
  PNETX_TRANSPORT_INTERFACE_T ptTemp = NULL;
  PNETX_TRANSPORT_INTERFACE_T ptRet  = NULL;

  TAILQ_FOREACH(ptTemp, &s_tNetXTransportInfo.tnetXTransportInterface, tList)
  {
    if(0 == OS_Strcmp(ptTemp->szInterfaceName, szInterfaceName))
    {
      ptRet = ptTemp;
      break;
    }
  }
  return ptRet;
}

/*****************************************************************************/
/*! Search for the interface given by the name szInterfaceName. If the Interface does
*   not exist it will be created.
*   \param szInterfaceName  Name of the interface to search for
*   \return NULL on error                                                    */
/*****************************************************************************/
PNETX_TRANSPORT_INTERFACE_T netXTransportGetInterface(const char* szInterfaceName)
{
  PNETX_TRANSPORT_INTERFACE_T ptRet = NULL;

  if (NULL == (ptRet = netXTransportSearchInterface( szInterfaceName)))
  {
    NETX_TRANSPORT_INTERFACE_T* ptIF = (NETX_TRANSPORT_INTERFACE_T*)OS_Memalloc(sizeof(NETX_TRANSPORT_INTERFACE_T));

    OS_Memset( ptIF, 0, sizeof(NETX_TRANSPORT_INTERFACE_T));

    ptIF->pvInterfaceLock = OS_CreateLock();

    /* create new interface */
    TAILQ_INSERT_TAIL(&s_tNetXTransportInfo.tnetXTransportInterface, ptIF, tList);
    OS_Strncpy(ptIF->szInterfaceName, 16, szInterfaceName, 16);

    TAILQ_INIT(&ptIF->tDeviceRefList);

    ptRet = ptIF;
  }
  return ptRet;
}

/*****************************************************************************/
/*! Removes every device, which is connected to this interface.
*   \param ptInterface  Pointer to the interface                             */
/*****************************************************************************/
static void netXTransportCleanUpInterface(PNETX_TRANSPORT_INTERFACE_T ptInterface)
{
  if (ptInterface)
  {
    NETX_TRANSPORT_DEVICE_REF_T* ptDeviceRef;

    while(NULL != (ptDeviceRef = TAILQ_FIRST(&ptInterface->tDeviceRefList)))
    {
      NETXTRANSPORT_DEVICE_HANDLE_T* ptNetXTransportDeviceHandle = (NETXTRANSPORT_DEVICE_HANDLE_T*)ptDeviceRef->hNetXTransportDeviceHandle;

      ptNetXTransportDeviceHandle->ptGenericDevice = NULL;

      TAILQ_REMOVE(&ptInterface->tDeviceRefList, ptDeviceRef, tList);

      OS_Memfree(ptDeviceRef);
    }
    netXTransportRemoveInterface( ptInterface);
  }
}

/*****************************************************************************/
/*! Checks the current interface state of the netXTransport instance.
*   \param hNetXTransportDevGroup  Handle to the netXTransport device group
*   \return 1 if interface is active (see eINTERFACE_STARTED)                */
/*****************************************************************************/
int netXTransportCheckInterfaceState(NXTHANDLE hNetXTransportDevGroup)
{
  PNETX_TRANSPORT_DEV_GROUP_T ptTemp = (PNETX_TRANSPORT_DEV_GROUP_T)hNetXTransportDevGroup;
  int                         fRet   = 0;

  if (NULL != hNetXTransportDevGroup)
  {
    PNETX_TRANSPORT_GENERIC_DEVICE_T ptGenericDevice = NULL;

    /* get the first device, to be able to verifiy the interface state */
    if (NULL != (ptGenericDevice = TAILQ_FIRST(&ptTemp->tGenericDeviceList)))
    {
      PNETX_TRANSPORT_INTERFACE_T ptInterface = (PNETX_TRANSPORT_INTERFACE_T)ptGenericDevice->pvInterface;

      fRet = 1;
      if (ptInterface->eInterfaceState != eINTERFACE_STARTED)
      {
        netXTransportReconnectDevice( ptGenericDevice->hNetXTransportDeviceHandle);

        if (ptInterface->eInterfaceState != eINTERFACE_STARTED) fRet = 0;
      }
    }
  }
  return fRet;
}

/*****************************************************************************/
/*! Deletes the interface. If there are still active device connected to
    the function will fail.
*   \param ptInterface  Pointer to the interface, to be deleted
*   \return 1 on success                                                     */
/*****************************************************************************/
static int netXTransportRemoveInterface(PNETX_TRANSPORT_INTERFACE_T ptInterface)
{
  int iRet = 1;

  if (NULL != ptInterface)
  {
    NETX_TRANSPORT_DEVICE_REF_T* ptDeviceRef;

    if (NULL != (ptDeviceRef = TAILQ_FIRST(&ptInterface->tDeviceRefList)))
    {
      /* fail remove, there are still device references -> first call netXTransportCleanUpInterface() */
      iRet = 0;
    } else
    {
      TAILQ_REMOVE(&s_tNetXTransportInfo.tnetXTransportInterface, ptInterface, tList);

      OS_DeleteLock( ptInterface->pvInterfaceLock);

      OS_Memfree(ptInterface);
    }
  }
  return iRet;
}

/*****************************************************************************/
/*! Registers for reconnect event of a device.
*   \param hNetXTransportDeviceHandle  Pointer to the requested device       */
/*****************************************************************************/
void netXTransportRegisterReconnect( NXTHANDLE hNetXTransportDeviceHandle)
{
  NETXTRANSPORT_DEVICE_HANDLE_T* ptNetXTransportDeviceHandle = (NETXTRANSPORT_DEVICE_HANDLE_T*)hNetXTransportDeviceHandle;

  OS_EnterLock( ptNetXTransportDeviceHandle->ptGenericDevice->pvDeviceLock);

  if (ptNetXTransportDeviceHandle->ptGenericDevice->pvEvent == NULL)
  {
    ptNetXTransportDeviceHandle->ptGenericDevice->pvEvent = OS_CreateEvent();
  }

  OS_LeaveLock( ptNetXTransportDeviceHandle->ptGenericDevice->pvDeviceLock);
}

/*****************************************************************************/
/*! De-registers reconnect event of a device.
*   \param hNetxTransportDeviceHandle  Handle to a netXTransport device      */
/*****************************************************************************/
void netXTransportDeRegisterReconnect( NXTHANDLE hNetxTransportDeviceHandle)
{
  NETXTRANSPORT_DEVICE_HANDLE_T* ptNetXTransportDeviceHandle = (NETXTRANSPORT_DEVICE_HANDLE_T*)hNetxTransportDeviceHandle;

  OS_EnterLock( ptNetXTransportDeviceHandle->ptGenericDevice->pvDeviceLock);

  if (ptNetXTransportDeviceHandle->ptGenericDevice->pvEvent != NULL)
  {
    OS_DeleteEvent(ptNetXTransportDeviceHandle->ptGenericDevice->pvEvent);
  }
  ptNetXTransportDeviceHandle->ptGenericDevice->pvEvent = NULL;

  OS_LeaveLock( ptNetXTransportDeviceHandle->ptGenericDevice->pvDeviceLock);
}

/*****************************************************************************/
/*! Monitors a reconnect for the given device. In case of the reconnect is not processed
    automatically, the reconnect will be scheduled manually via netXTransportDisconnectDevice()
    and netXTransportReconnectDevice().
*   \param hNetxTransportDeviceHandle  Handle to netXTransport device
*   \param ulTimeout                   Timeout for reconnect (max. time to wait before starting
                                       manual reconnect)
*   \return NXT_NO_ERROR on success                                         */
/*****************************************************************************/
int32_t netXTransportScheduleReconnect( NXTHANDLE hNetxTransportDeviceHandle, uint32_t ulTimeout)
{
  NETXTRANSPORT_DEVICE_HANDLE_T* ptNetXTransportDeviceHandle = (NETXTRANSPORT_DEVICE_HANDLE_T*)hNetxTransportDeviceHandle;
  uint32_t                       lRet;

  if(g_ulTraceLevel & TRACE_LEVEL_INFO)
  {
    USER_Trace( &s_tNetXTransportInfo, TRACE_LEVEL_INFO, "Try to reconnect with device: %s", ptNetXTransportDeviceHandle->ptGenericDevice->szDeviceName);
  }

  /* first wait if reconnect is automatically done by the connector (depends on the interface) */
  if (OS_EVENT_TIMEOUT == (lRet = OS_WaitEvent( ptNetXTransportDeviceHandle->ptGenericDevice->pvEvent, ulTimeout)))
  {
    if(g_ulTraceLevel & TRACE_LEVEL_INFO)
    {
      USER_Trace( &s_tNetXTransportInfo, TRACE_LEVEL_INFO, "Trying manual reconnect for device: %s", ptNetXTransportDeviceHandle->ptGenericDevice->szDeviceName);
    }

    /* we have to reconnect manually */
    netXTransportDisconnectDevice( hNetxTransportDeviceHandle);

    /* reconnect */
    netXTransportReconnectDevice( hNetxTransportDeviceHandle);

    lRet = NXT_FUNCTION_FAILED;
    /* check if device is already returned */
    if ( ptNetXTransportDeviceHandle->ptGenericDevice->eState == eDEVICE_STARTED)
    {
      lRet = NXT_NO_ERROR;
    }
  }
  return lRet;
}

/*****************************************************************************/
/*! Called when scheduling manual connect. In case of a server based connection (identified if
    there are more than one endpoints), no physical connect is done.
*   \param hNetxTransportDeviceHandle  Handle to the netXTransport device
*   \return NXT_NO_ERROR on success                                          */
/*****************************************************************************/
int32_t netXTransportReconnectDevice( NXTHANDLE hNetxTransportDeviceHandle)
{
  NETXTRANSPORT_DEVICE_HANDLE_T* ptNetXTransportDeviceHandle = (NETXTRANSPORT_DEVICE_HANDLE_T*)hNetxTransportDeviceHandle;
  PNETX_TRANSPORT_INTERFACE_T    ptInterface                 = NULL;

  if (ptNetXTransportDeviceHandle == NULL)
    return NXT_INVALID_HANDLE;

  ptInterface = (PNETX_TRANSPORT_INTERFACE_T)ptNetXTransportDeviceHandle->ptGenericDevice->pvInterface;

  /* NOTE: if the interface serves more than one device, we are talking to a server, so there is no need for a physical reconnect */
  /* NOTE: to not interrupt the connection to other devices. In this case only update the device information.                     */
  /* NOTE: In case of a timeout the server will also close the connection (delete Transport-Layer -> hTransport = NULL), then we  */
  /* NOTE: have to reopen the connection.                                                                                         */
  if ((ptInterface->ulDeviceCounter) > 1 && (ptInterface->hTransport != NULL))
  {
    HilTransportSoftReconnect( ptInterface->hTransport, ptNetXTransportDeviceHandle->ptGenericDevice->ulDeviceIdentifier, DeviceNotifyCallback);
  } else
  {
    /* schedule hardware interface reconnect (open interface) */
    netXTransportInterfaceNotification( ptInterface->szInterfaceName, eNXT_DEVICE_ATTACHED, ptInterface->pvConnectorData, ptInterface->pvUser);
  }
  return NXT_NO_ERROR;
}

/*****************************************************************************/
/*! Called when scheduling manual disconnect. In case of a server based connection (identified if
    there are more than one endpoints), no physical disconnect is done.
*   \param hNetxTransportDeviceHandle  Handle to the netXTransport device
*   \return NXT_NO_ERROR on success                                          */
/*****************************************************************************/
int32_t netXTransportDisconnectDevice( NXTHANDLE hNetxTransportDeviceHandle)
{
  NETXTRANSPORT_DEVICE_HANDLE_T* ptNetXTransportDeviceHandle = (NETXTRANSPORT_DEVICE_HANDLE_T*)hNetxTransportDeviceHandle;
  PNETX_TRANSPORT_INTERFACE_T    ptInterface                 = NULL;

  if (ptNetXTransportDeviceHandle == NULL)
    return NXT_INVALID_HANDLE;

  ptInterface = (PNETX_TRANSPORT_INTERFACE_T)ptNetXTransportDeviceHandle->ptGenericDevice->pvInterface;

  /* NOTE: if the interface serves more than one device, we are talking to a server, so there is no need for a physical reconnect */
  /* NOTE: In  this case only update the device information, of the device which should be reconnected                            */
  if (ptInterface->ulDeviceCounter > 1)
  {
    netXTransportDeviceStateChange( ptInterface,
                                    ptNetXTransportDeviceHandle->ptGenericDevice,
                                    NULL,
                                    ptNetXTransportDeviceHandle->ptGenericDevice->ulDeviceIdentifier,
                                    eNXT_DEVICE_DETACHED,
                                    ptNetXTransportDeviceHandle->ptGenericDevice->hTransport,
                                    ptNetXTransportDeviceHandle->ptGenericDevice->pvTLDeviceData);
  } else
  {
    /* schedule hardware interface disconnect (closeinterface) */
    netXTransportInterfaceNotification( ptInterface->szInterfaceName, eNXT_DEVICE_DETACHED, ptInterface->pvConnectorData, ptInterface->pvConnector);
  }
  return NXT_NO_ERROR;
}

/*****************************************************************************/
/*! Interface notifiaction callback (called from HilTransport-Layer) in case of
    interface state changes.
*   \param szInterfaceName  Name of the interface
*   \param eNotify          Event which occured
*   \param pvConnectorData  Connector private data
*   \param pvUser           netXTransport connector specific information     */
/*****************************************************************************/
void netXTransportInterfaceNotification( const char* szInterfaceName, NETX_INTERFACE_NOTIFICATION_E eNotify, void* pvConnectorData, void* pvUser)
{
  int32_t           lRet        = 0;
  PNETX_CONNECTOR_T ptConnector = (PNETX_CONNECTOR_T)pvUser;

  switch(eNotify)
  {
    case eNXT_DEVICE_ATTACHED:
      {
        PNETX_TRANSPORT_INTERFACE_T ptInterface = NULL;

        if(g_ulTraceLevel & TRACE_LEVEL_INFO)
        {
          USER_Trace( &s_tNetXTransportInfo, TRACE_LEVEL_INFO, "Attach event notified for interface \"%s\" ", szInterfaceName);
        }
        {
          if (NULL != (ptInterface = netXTransportGetInterface( szInterfaceName)))
          {
            uint16_t* ausDataTypes = netXTransportGetSupportedTranslationLayer();

            OS_EnterLock( ptInterface->pvInterfaceLock);
            if (ptInterface->eInterfaceState == eINTERFACE_STOPPED)
            {
              ptInterface->pvConnectorData = pvConnectorData;
              ptInterface->pvUser          = pvUser;
              ptInterface->pvConnector     = ptConnector;

              /* initialize a conncection for every device found at this interface */
              if (NXT_NO_ERROR == (lRet = HilTransportInit( ptConnector, szInterfaceName, pvConnectorData, ausDataTypes, &ptInterface->hTransport, DeviceNotifyCallback)))
              {
                ptInterface->eInterfaceState = eINTERFACE_STARTED;
              }
            }
            OS_LeaveLock( ptInterface->pvInterfaceLock);

            OS_Memfree( ausDataTypes);
          }
        }
      }
      break;
    case eNXT_DEVICE_DETACHED:
      {
        PNETX_TRANSPORT_INTERFACE_T ptInterface = netXTransportSearchInterface( szInterfaceName);

        if(g_ulTraceLevel & TRACE_LEVEL_INFO)
        {
          USER_Trace( &s_tNetXTransportInfo, TRACE_LEVEL_INFO, "Detach-Event notified for interface \"%s\" ", szInterfaceName);
        }

        /* check if interface exists */
        if (NULL != ptInterface)
        {
          NETX_TRANSPORT_DEVICE_REF_T* ptTmpDevice;

          /* lock device before freeing the transport layer */
          OS_EnterLock( ptInterface->pvInterfaceLock);

          /* interface is already stopped, so skip notification */
          if (ptInterface->eInterfaceState == eINTERFACE_STARTED)
          {
            /* set every device conneted over the current interface in a not active state */
            TAILQ_FOREACH(ptTmpDevice, &ptInterface->tDeviceRefList, tList)
            {
              NETXTRANSPORT_DEVICE_HANDLE_T* ptNetXTransportDeviceHandle = (NETXTRANSPORT_DEVICE_HANDLE_T*)ptTmpDevice->hNetXTransportDeviceHandle;

              netXTransportDeviceStateChange( ptInterface,
                                               ptNetXTransportDeviceHandle->ptGenericDevice,
                                               NULL, /* device name is not required since we got the reference ptGenericDevice */
                                               0,
                                               eNotify,
                                               ptInterface->hTransport,
                                               ptNetXTransportDeviceHandle->ptGenericDevice->pvTLDeviceData);
            }
            /* mark interface as stopped */
            ptInterface->eInterfaceState = eINTERFACE_STOPPED;
          }
          OS_LeaveLock( ptInterface->pvInterfaceLock);

          /* check if there are any active connections */
          while(ptInterface->ulActiveTransfers>0)
          {
            OS_Sleep(1);
            /* reject all remaining transactions */
            OS_EnterLock( ptInterface->pvInterfaceLock);
            if (ptInterface->hTransport != NULL)
            {
              HilTransportStopPendingTransactions( ptInterface->hTransport);
            }
            OS_LeaveLock( ptInterface->pvInterfaceLock);
          }

          /* Deinit transport layer (stop pending requests and delete instance of transport layer) */
          OS_EnterLock( ptInterface->pvInterfaceLock);
          if (ptInterface->hTransport != NULL)
          {
            HilTransportDeinit(ptInterface->hTransport);
            ptInterface->hTransport = NULL;
          }
          OS_LeaveLock( ptInterface->pvInterfaceLock);
        }
      }
      break;
    case eNXT_DEVICE_DELETED:
      {
        PNETX_TRANSPORT_INTERFACE_T ptInterface = netXTransportSearchInterface( szInterfaceName);

        /* check if interface exists */
        if (NULL != ptInterface)
        {
          NETX_TRANSPORT_DEVICE_REF_T* ptTmpDevice;

          OS_EnterLock( ptInterface->pvInterfaceLock);

          /* interface is already stopped, so skip notification */
          if (ptInterface->eInterfaceState == eINTERFACE_STARTED)
          {
            /* for every conected device */
            TAILQ_FOREACH(ptTmpDevice, &ptInterface->tDeviceRefList, tList)
            {
              NETXTRANSPORT_DEVICE_HANDLE_T* ptNetXTransportDeviceHandle = (NETXTRANSPORT_DEVICE_HANDLE_T*)ptTmpDevice->hNetXTransportDeviceHandle;

              netXTransportDeviceStateChange( ptInterface,
                                              ptNetXTransportDeviceHandle->ptGenericDevice,
                                              NULL, /* device name is not required since we got the reference ptGenericDevice */
                                              0,
                                              eNotify,
                                              ptInterface->hTransport,
                                              ptNetXTransportDeviceHandle->ptGenericDevice->pvTLDeviceData);
            }
            /* mark interface as stopped */
            ptInterface->eInterfaceState = eINTERFACE_STOPPED;
          }
          OS_LeaveLock( ptInterface->pvInterfaceLock);

          /* check if there are any active connections */
          while(ptInterface->ulActiveTransfers>0)
          {
            OS_EnterLock( ptInterface->pvInterfaceLock);
            if (ptInterface->hTransport != NULL)
            {
              /* reject all remaining transactions */
              HilTransportStopPendingTransactions( ptInterface->hTransport);
            }
            OS_LeaveLock( ptInterface->pvInterfaceLock);
          }

          /* deinit hiltransport interface */
          OS_EnterLock( ptInterface->pvInterfaceLock);
          if (ptInterface->hTransport != NULL)
          {
            HilTransportDeinit( ptInterface->hTransport);
            ptInterface->hTransport = NULL;
          }
          OS_LeaveLock( ptInterface->pvInterfaceLock);
          /* we can not cleanup the interface yet, since the user might still use the API */
        }
      }
      break;
    default:
    /* Unknown notification */
    break;
  }
}

/*****************************************************************************/
/*! Device notification callback (called from HilTransport-Layer) in case of
    device state changes.
*   \param szInterfaceName      Name of the interface
*   \param szDeviceName         Device name
*   \param ulDeviceIdentifier   Unique device identifier
*   \param eNotify              Event which occured
*   \param hTransport           Handle to the corresponding transport layer
*   \param pvDevicePrivateData  Private data (device specific Translation-Layer info) */
/*****************************************************************************/
void DeviceNotifyCallback( const char* szInterfaceName, const char* szDeviceName, uint32_t ulDeviceIdentifier, NETX_INTERFACE_NOTIFICATION_E eNotify, void* hTransport, void* pvDevicePrivateData)
{
  PNETX_TRANSPORT_INTERFACE_T ptInterface = netXTransportSearchInterface( szInterfaceName);
  char                        szNewDeviceName[NXT_MAX_PATH];

  OS_Strvsprintf( szNewDeviceName, 16, "%s_%s", szInterfaceName, szDeviceName);

  netXTransportDeviceStateChange( ptInterface, NULL, szNewDeviceName, ulDeviceIdentifier, eNotify, hTransport, pvDevicePrivateData);
}

/*****************************************************************************/
/*! Called in case of a device state change. Depending on the notified state,
    a device will be created, detached or deleted.
*   \param ptInterface         Pointer to the netXTransport interface
*   \param ptGenericDevice     Pointer to the netXTransport generic device
*   \param szDeviceName        Name of the device (optional only needed, if we got no reference to ptDevice)
*   \param eNotify             Type of occured event
*   \param ulDeviceIdentifier  Unique device identifier
*   \param hTransport          Handle to corresponding translation layer
*   \param pvDevicePrivateData User defined data                             */
/*****************************************************************************/
void netXTransportDeviceStateChange( PNETX_TRANSPORT_INTERFACE_T ptInterface, PNETX_TRANSPORT_GENERIC_DEVICE_T ptGenericDevice, char* szDeviceName, uint32_t ulDeviceIdentifier, NETX_INTERFACE_NOTIFICATION_E eNotify, NXTHANDLE hTransport, void* pvDevicePrivateData)
{
  NETX_TRANSPORT_DEVICE_REF_T* ptDeviceRef = NULL;

  /* check if we have to search for the device */
  if (ptGenericDevice == NULL)
  {
    /* check if device is already known */
    TAILQ_FOREACH( ptDeviceRef, &ptInterface->tDeviceRefList, tList)
    {
      /* check if we know the device already, so it is reconnected */
      if (0 == OS_Strcmp( ((NETXTRANSPORT_DEVICE_HANDLE_T*)ptDeviceRef->hNetXTransportDeviceHandle)->ptGenericDevice->szDeviceName, szDeviceName))
      {
        if ((((NETXTRANSPORT_DEVICE_HANDLE_T*)ptDeviceRef->hNetXTransportDeviceHandle)->ptGenericDevice)->ulDeviceIdentifier == ulDeviceIdentifier)
        {
          ptGenericDevice = ((NETXTRANSPORT_DEVICE_HANDLE_T*)ptDeviceRef->hNetXTransportDeviceHandle)->ptGenericDevice;
        }
      }
    }
  }

  switch(eNotify)
  {
  case eNXT_DEVICE_ATTACHED:

    /* re-activate device or create a new one */
    if(NULL == ptGenericDevice)
    {
      /* Newly attached device, so allocate it here */
      ptGenericDevice = OS_Memalloc(sizeof(*ptGenericDevice));

      if(NULL == ptGenericDevice)
      {
        /* Out of memory */
        if(g_ulTraceLevel & TRACE_LEVEL_ERROR)
        {
          USER_Trace( &s_tNetXTransportInfo, TRACE_LEVEL_ERROR, "Error adding device %s. Not enough memory available!\n", szDeviceName);
        }
        break;
      } else
      {
        uint16_t usTLType = 0;
        OS_Memset(ptGenericDevice, 0, sizeof(*ptGenericDevice));

        ptGenericDevice->hTransport         = hTransport;          /* store handle to the translation layer                          */
        ptGenericDevice->pvTLDeviceData     = pvDevicePrivateData; /* store device data delivered from translation layer scan method */
        ptGenericDevice->pvInterface        = ptInterface;
        ptGenericDevice->ulDeviceIdentifier = ulDeviceIdentifier;
        ptGenericDevice->pvDeviceLock       = OS_CreateLock();
        ptGenericDevice->pvEvent            = NULL;

        OS_Strncpy( ptGenericDevice->szDeviceName, 16, szDeviceName, 16);

        /* identify the type of of the device */
        if (HIL_TRANSPORT_TYPE_KEEP_ALIVE != (usTLType = HilTransportGetDefaultTLType(hTransport)))
        {
          PNETX_TRANSPORT_DEV_GROUP_T ptNetXTransportDevGroup;

          /* insert device into appropriate device group */
          TAILQ_FOREACH(ptNetXTransportDevGroup, &s_tNetXTransportInfo.tnetXTransportDevGroupList, tList)
          {
            if (ptNetXTransportDevGroup->ptTLInterface->usDataType == usTLType)
            {
              NETX_TRANSPORT_DEVICE_REF_T* ptDeviceRef = (NETX_TRANSPORT_DEVICE_REF_T*)OS_Memalloc(sizeof(NETX_TRANSPORT_DEVICE_REF_T));

              /* insert device to data translation layer list */
              TAILQ_INSERT_TAIL( &ptNetXTransportDevGroup->tGenericDeviceList, ptGenericDevice, tList);

              /* create device handle */
              ptDeviceRef->hNetXTransportDeviceHandle = netXTransportCreateDeviceHandle( ptNetXTransportDevGroup, ptGenericDevice);

              /* insert device reference into the list of the current interface */
              TAILQ_INSERT_TAIL(&ptInterface->tDeviceRefList, ptDeviceRef, tList);
              ptInterface->ulDeviceCounter++;

              if(g_ulTraceLevel & TRACE_LEVEL_INFO)
              {
                USER_Trace( &s_tNetXTransportInfo, TRACE_LEVEL_INFO, "Successfully added device: %s (Datatype=0x%X)", ptGenericDevice->szDeviceName, usTLType);
              }

              if (s_tNetXTransportInfo.pfnDeviceDiscoverCB != NULL)
              {
                void* pvUser = s_tNetXTransportInfo.pvDeviceDiscoverUserParam;

                s_tNetXTransportInfo.pfnDeviceDiscoverCB( ptInterface->szInterfaceName, ptGenericDevice->szDeviceName, ptInterface->ulDeviceCounter, ptNetXTransportDevGroup->ptTLInterface->usDataType, ptGenericDevice->pvTLDeviceData, pvUser);
              }

              break;
            }
          }
        } else
        {
          if(g_ulTraceLevel & TRACE_LEVEL_ERROR)
          {
            USER_Trace( &s_tNetXTransportInfo, TRACE_LEVEL_ERROR, "Error adding device! Unsupported data type detected (Datatype=0x%X)", usTLType);
          }
          /* this should never happen, since we already passed successfully HilTransportInit() */
          OS_Memfree(ptGenericDevice);
        }
      }
    } else
    {
      /* this device is re-connected, so just update its current data */
      ptGenericDevice->hTransport     = hTransport;          /* handle to the transport layer */
      ptGenericDevice->pvTLDeviceData = pvDevicePrivateData; /* private Data of the TL layer  */

      /* try to restore the connection states (open active connections and update internal resources) */
      HilTransportRestoreConnectionStates( ptGenericDevice->hNetXTransportDeviceHandle, hTransport);

      if(g_ulTraceLevel & TRACE_LEVEL_INFO)
      {
        USER_Trace( &s_tNetXTransportInfo, TRACE_LEVEL_INFO, "Device re-activated: %s", ptGenericDevice->szDeviceName);
      }
    }
    /* update state of the device */
    ptGenericDevice->eState = eDEVICE_STARTED;

    OS_EnterLock( ptGenericDevice->pvDeviceLock);
    if (ptGenericDevice->pvEvent)
    {
      OS_SetEvent( ptGenericDevice->pvEvent);
    }
    OS_LeaveLock( ptGenericDevice->pvDeviceLock);

    break;
  case eNXT_DEVICE_DETACHED: /* Device removed temporarily */
    /* Locally disable the device */
    if(NULL != ptGenericDevice)
    {
      ptGenericDevice->eState = eDEVICE_STOPPED;

      if(g_ulTraceLevel & TRACE_LEVEL_INFO)
      {
        USER_Trace( &s_tNetXTransportInfo, TRACE_LEVEL_INFO, "Device detached: %s", ptGenericDevice->szDeviceName);
      }
    }
    break;

  case eNXT_DEVICE_DELETED: /* Device removed completely */
    if(NULL != ptGenericDevice)
    {
      if(g_ulTraceLevel & TRACE_LEVEL_INFO)
      {
        USER_Trace( &s_tNetXTransportInfo, TRACE_LEVEL_INFO, "Device deleted: %s", ptGenericDevice->szDeviceName);
      }

      ptGenericDevice->eState = eDEVICE_STOPPED;

      /* NOTE: we can not delete the devices, at this point since the user will user the API */
    }
    break;

  default:
    /* Unknown notification */
    break;
  }
}

/*****************************************************************************/
/*! Increment connection counter for corresponding interface.
*   \param hNetxTransportDeviceHandle  Handle to the netXTransport device    */
/*****************************************************************************/
void netXTransportActivateConnection( NXTHANDLE hNetxTransportDeviceHandle)
{
  NETXTRANSPORT_DEVICE_HANDLE_T* ptNetXTransportDeviceHandle = (NETXTRANSPORT_DEVICE_HANDLE_T*)hNetxTransportDeviceHandle;
  PNETX_TRANSPORT_INTERFACE_T    ptInterface                 = (PNETX_TRANSPORT_INTERFACE_T)ptNetXTransportDeviceHandle->ptGenericDevice->pvInterface;

  OS_EnterLock( ptInterface->pvInterfaceLock);

  ptInterface->ulActiveConnections++;

  OS_LeaveLock( ptInterface->pvInterfaceLock);
}

/*****************************************************************************/
/*! Decrement connection counter for corresponding interface.
*   \param hNetxTransportDeviceHandle  Handle to the netXTransport device    */
/*****************************************************************************/
void netXTransportDeActivateConnection( NXTHANDLE hNetxTransportDeviceHandle)
{
  NETXTRANSPORT_DEVICE_HANDLE_T* ptNetXTransportDeviceHandle = (NETXTRANSPORT_DEVICE_HANDLE_T*)hNetxTransportDeviceHandle;
  PNETX_TRANSPORT_INTERFACE_T    ptInterface                 = (PNETX_TRANSPORT_INTERFACE_T)ptNetXTransportDeviceHandle->ptGenericDevice->pvInterface;

  OS_EnterLock( ptInterface->pvInterfaceLock);

  ptInterface->ulActiveConnections--;

  OS_LeaveLock( ptInterface->pvInterfaceLock);
}

/*****************************************************************************/
/*! Handles netXTransport cyclic jobs (KeepAlive,...)                        */
/*****************************************************************************/
void APIENTRY netXTransportCyclicFunction( void)
{
  PNETX_TRANSPORT_INTERFACE_T ptInterface = NULL;

  TAILQ_FOREACH( ptInterface, &s_tNetXTransportInfo.tnetXTransportInterface, tList)
  {
    int fRunKeepAlive = 0;

    /* first check if the interface and the connections are active */
    OS_EnterLock ( ptInterface->pvInterfaceLock);

    if (ptInterface->ulActiveConnections > 0)
    {
      if (eINTERFACE_STARTED == ptInterface->eInterfaceState)
      {
        fRunKeepAlive = 1;
      }
    }
    OS_LeaveLock ( ptInterface->pvInterfaceLock);

    if (fRunKeepAlive == 1)
    {
      KEEPALIVE_STATE_E eKeepAliveState;
      /* NOTE: netXTransportHandleKeepAlive() runs in an asynchronous thread, so we have to synchronize.  */
      /* netXTransportAttachInterface() prevents the Transport-Layer to be removed while, we are sending. */
      netXTransportAttachInterface( ptInterface);
      eKeepAliveState = HilTransportHandleKeepAlive( ptInterface->hTransport);
      netXTransportDetachInterface( ptInterface);

      if (eKeepAliveState == eKEEP_ALIVE_TIMEOUT)
      {
        /* Notify translation layers of a disconnect */
        netXTransportInterfaceNotification( ptInterface->szInterfaceName, eNXT_DEVICE_DETACHED, ptInterface->pvConnectorData, ptInterface->pvConnector);
      }
    }
  }
}

/*****************************************************************************/
/*! Retreives the connector specific timeout
*   \param ptConnector  Pointer to connector
*   \param pulTimeout   Pointer to returned timeout                          */
/*****************************************************************************/
void netXTransportGetConnectorTimeout( PNETX_CONNECTOR_T ptConnector, uint32_t* pulTimeout)
{
  if ((ptConnector == NULL) || (pulTimeout == NULL))
    return;

  USER_GetConnectorTimeout( ptConnector, pulTimeout);

  return;
}

