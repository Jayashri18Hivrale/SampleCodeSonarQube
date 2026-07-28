/**************************************************************************************

   Copyright (c) Hilscher GmbH. All Rights Reserved.

 **************************************************************************************

   Filename:
    $Id: netXTransport.h 182 2013-06-26 09:07:01Z sebastiand $
   Last Modification:
    $Author: sebastiand $
    $Date: 2013-06-26 11:07:01 +0200 (Mi, 26. Jun 2013) $
    $Revision: 182 $

   Targets:
     O/S Independent : yes

   Description:
    netX Transport definitions

   Changes:

     Version   Date        Author   Description
     ----------------------------------------------------------------------------------
      1        23.02.13    SD       initial version

**************************************************************************************/

/****************************************************************************/
/*! \file netXTransport.h
    netX Transport definitions                                              */
/****************************************************************************/

#ifndef __NETXTRANSPORT__H
#define __NETXTRANSPORT__H

#include "ConnectorAPI.h"
#include "TranslationLayer.h"
#include "netXTransport_Errors.h"
#include <sys/queue.h>

#ifdef __cplusplus
extern "C"
{
#endif

/*****************************************************************************/
/*! netXTransport version information                                        */
/*****************************************************************************/
#define NXT_VERSION_MAJOR 1  /*!< Version major    */
#define NXT_VERSION_MINOR 0  /*!< Version minor    */
#define NXT_VERSION_BUILD 0  /*!< Version revision */
#define NXT_VERSION_REV   0  /*!< Version build    */

/*****************************************************************************/
/*!  \addtogroup NETX_TRANSPORT_DEF netXTransport global definitions
*    \{                                                                      */
/*****************************************************************************/
#define NXTHANDLE void*      /*!< netXTransport handle definiton    */

#define NXT_MAX_PATH 256     /*!< netXTransport maximum string size */

/*****************************************************************************/
/*! Trace level definitions                                                  */
/*****************************************************************************/
#define TRACE_LEVEL_DEBUG   0x00000001  /*!< enables debug trace   */
#define TRACE_LEVEL_INFO    0x00000002  /*!< enables info trace    */
#define TRACE_LEVEL_WARNING 0x00000004  /*!< enables warning trace */
#define TRACE_LEVEL_ERROR   0x00000008  /*!< enables error trace   */

/*****************************************************************************/
/*! Generic timeout definitions                                              */
/*****************************************************************************/
#define   TRANSPORT_TO_TRANSFER               1000      /*!< Transfer packet timeout            */
#define   TRANSPORT_TO_QUERY_SERVER           1000      /*!< Query server information timeout   */
#define   TRANSPORT_TO_ESTABLISH_CONNECTION   2000      /*!< Establish connection timeout       */
#define   TRANSPORT_THREAD_TIMEOUT            1000      /*!< Start transport thread timeout     */

/*****************************************************************************/
/*! \}                                                                       */
/*****************************************************************************/

TAILQ_HEAD ( CONNECTOR_LIST,        NETX_CONNECTOR_Ttag);                 /*!< List of registered connector                       */
TAILQ_HEAD ( GEN_DEVICE_LIST,       NETX_TRANSPORT_GENERIC_DEVICE_Ttag);  /*!< List of active devices                             */
TAILQ_HEAD ( PDEVICE_LIST,          NETX_TRANSPORT_DEVICE_REF_Ttag);      /*!< List contains reference to netXTransport devices   */
TAILQ_HEAD ( DEV_GROUP_LIST,        NETX_TRANSPORT_DEV_GROUP_Ttag);       /*!< List of registered netXTransport groups            */
TAILQ_HEAD ( NETXTRANSPORT_IF_LIST, NETX_TRANSPORT_INTERFACE_Ttag);       /*!< List of active interfaces                          */

/*****************************************************************************/
/*! netXTransport Function Pointer definition                                */
/*****************************************************************************/
typedef int32_t (*PFN_TRANSLATIONLAYER_INIT)  ( void* pvUserParam);  /*!< Translation-Layer initialization       */
typedef void    (*PFN_TRANSLATIONLAYER_DEINIT)( void* pvUserParam);  /*!< Translation-Layer de-initialization    */
typedef void    (*PFN_CYCLIC_USER_JOB_CB)     ( char* szInterfaceName, KEEPALIVE_STATE_E eState, void* pvUserParam);  /*!< netXTransport cyclic function          */
typedef void    (*PFN_DEVICEBROWSE_CALLBACK)  ( char* szInterfaceName, char* szDeviceName, uint32_t ulDevIdx, uint16_t usDataType, void* pvDevTLPrivData, void* pvUser);

/*****************************************************************************/
/*!  \addtogroup NETX_TRANSPORT_STRUCTURES netXTransport Structure Definitions
*    \{                                                                      */
/*****************************************************************************/
/*****************************************************************************/
/*! Translation layer init structure                                         */
/*****************************************************************************/
typedef struct TL_INIT_Ttag
{
  PFN_TRANSLATIONLAYER_INIT   pfnTLInit;   /*!< Pointer to initialization function    */
  PFN_TRANSLATIONLAYER_DEINIT pfnTLDeInit; /*!< Pointer to de-initialization function */
  void*                       pvData;      /*!< Pointer to user defineded data        */

} TL_INIT_T, *PTL_INIT_T;                  /*!< Pointer to TL_INIT_T                  */

/*****************************************************************************/
/*! netXTransport state definitions                                          */
/*****************************************************************************/
typedef enum NXT_NETXTRANSPORT_STATE_Etag
{
  eNETXTRANSPORT_UNINITIALIZED = 0,          /*!< netXtransport de-initialized */
  eNETXTRANSPORT_INITIALIZED,                /*!< netXtransport initialized    */
  eNETXTRANSPORT_STOPPED,                    /*!< netXtransport stopped        */
  eNETXTRANSPORT_STARTED,                    /*!< netXtransport started        */

} NXT_NETXTRANSPORT_STATE_E;

/*****************************************************************************/
/*! Device state definitions                                                 */
/*****************************************************************************/
typedef enum NXT_DEVICE_STATE_Etag
{
  eDEVICE_STOPPED = 0,                /*!< Device is down and not in an operational state  */
  eDEVICE_STARTED,                    /*!< Device is fully up and in an operational state  */

} NXT_DEVICE_STATE_E;

/*****************************************************************************/
/*! interface state definitions                                              */
/*****************************************************************************/
typedef enum NXT_INTERFACE_STATE_Etag
{
  eINTERFACE_STOPPED = 0,             /*!< Interface is down and not in an operational state  */
  eINTERFACE_STARTED,                 /*!< Interface is fully up and in an operational state  */

} NXT_INTERFACE_STATE_E;

/*****************************************************************************/
/*! netXTransport device definition                                          */
/*****************************************************************************/
typedef struct NETX_TRANSPORT_GENERIC_DEVICE_Ttag
{
  TAILQ_ENTRY(NETX_TRANSPORT_GENERIC_DEVICE_Ttag) tList; /*!< List entry                                   */

  char                szDeviceName[16];            /*!< Device name                                        */
  uint32_t            ulDeviceIdentifier;          /*!< Unique device identifier                           */
  void*               hTransport;                  /*!< Handle to the transport layer                      */
  NXT_DEVICE_STATE_E  eState;                      /*!< Current state of the device                        */
  uint32_t            ulRefCounter;                /*!< Reference counter                                  */
  void*               pvTLDeviceData;              /*!< Translation layer device specific information      */
  void*               hNetXTransportDeviceHandle;  /*!< netXtransport device handle                        */
  void*               pvDeviceLock;                /*!< Lock for sync purposes                             */
  void*               pvEvent;                     /*!< Device specific event                              */
  void*               pvInterface;                 /*!< Pointer to the interface the device is related to  */

} NETX_TRANSPORT_GENERIC_DEVICE_T, *PNETX_TRANSPORT_GENERIC_DEVICE_T; /*!< Pointer to NETX_TRANSPORT_DEVICE_T      */

/*****************************************************************************/
/*! netXTransport device reference structure                                 */
/*****************************************************************************/
typedef struct NETX_TRANSPORT_DEVICE_REF_Ttag
{
  TAILQ_ENTRY(NETX_TRANSPORT_DEVICE_REF_Ttag) tList; /*!< List entry                                 */
  void* hNetXTransportDeviceHandle;                  /*!< pointer to the netXTransport device handle */

}NETX_TRANSPORT_DEVICE_REF_T, *PNETX_TRANSPORT_DEVICE_REF_T; /*!< Pointer to NETX_TRANSPORT_DEVICE_REF_T */

/*****************************************************************************/
/*! netXTransport interface definition                                       */
/*****************************************************************************/
typedef struct NETX_TRANSPORT_INTERFACE_Ttag
{
  TAILQ_ENTRY(NETX_TRANSPORT_INTERFACE_Ttag) tList;     /*!< List entry                                      */

  char                  szInterfaceName[NXT_MAX_PATH];  /*!< Interface name                                  */
  NXTHANDLE             hTransport;                     /*!< Handle to corresponding transport layer         */
  void*                 pvConnectorData;                /*!< Pointer to the related connector private data   */
  void*                 pvUser;                         /*!< Pointer to the related connector private data   */
  void*                 pvConnector;                    /*!< Pointer to the related connector private data   */
  NXT_INTERFACE_STATE_E eInterfaceState;                /*!< Current state of the interface                  */
  struct PDEVICE_LIST   tDeviceRefList;                 /*!< Referneces list of registered devices           */
  uint32_t              ulDeviceCounter;                /*!< Number of registered devices                    */
  uint32_t              ulActiveTransfers;              /*!< Number of active transfers                      */
  uint32_t              ulActiveConnections;            /*!< Number of active connections                    */
  void*                 pvInterfaceLock;                /*!< Lock for sync purposes                          */

} NETX_TRANSPORT_INTERFACE_T, *PNETX_TRANSPORT_INTERFACE_T; /*!< Pointer to NETX_TRANSPORT_INTERFACE_T */

/*****************************************************************************/
/*! netXTransport device group definition                                    */
/*****************************************************************************/
typedef struct NETX_TRANSPORT_DEV_GROUP_Ttag
{
  TAILQ_ENTRY(NETX_TRANSPORT_DEV_GROUP_Ttag) tList; /*!< List entry */

  PNETX_TL_INTERFACE_T   ptTLInterface;       /*!< Pointer to translation layer interface               */
  struct GEN_DEVICE_LIST tGenericDeviceList;  /*!< List of the registered netXTransport generic devices */

} NETX_TRANSPORT_DEV_GROUP_T, *PNETX_TRANSPORT_DEV_GROUP_T; /*!< Pointer to NETX_TRANSPORT_DEV_GROUP_T */

/*****************************************************************************/
/*! netXTransport management structure                                       */
/*****************************************************************************/
typedef struct NETX_TRANSPORT_DATA_Ttag
{
  struct CONNECTOR_LIST        tConnectors;                  /*!< List of registered connector                       */
  struct NETXTRANSPORT_IF_LIST tnetXTransportInterface;      /*!< List of recognized connector interfaces            */
  struct DEV_GROUP_LIST        tnetXTransportDevGroupList;   /*!< List of device ordered by transport types          */
  void*                        pvLogFile;                    /*!< Pointer to log file info structure                 */
  PTL_INIT_T                   ptTLInitTable;                /*!< Pointer to translation layer initialization table  */
  uint32_t                     ulInitTableSize;              /*!< size of table ptTLInitTable                        */
  NXT_NETXTRANSPORT_STATE_E    eState;                       /*!< state of the netXTransport toolkit                 */
  void*                        pvnetXTransportLock;          /*!< Lock for sync purposes                             */
  PFN_DEVICEBROWSE_CALLBACK    pfnDeviceDiscoverCB;
  void*                        pvDeviceDiscoverUserParam;

} NETX_TRANSPORT_DATA_T, *PNETX_TRANSPORT_DATA_T; /*!< Pointer to NETX_TRANSPORT_DATA_T */
/*****************************************************************************/
/*! \}                                                                       */
/*****************************************************************************/

/*****************************************************************************/
/*!  \addtogroup NETX_TRANSPORT_API_IF netXTransport API Functions
*    \{                                                                      */
/*****************************************************************************/
/* API Function declaration */
int32_t APIENTRY netXTransportInit                   ( TL_INIT_T atTLInitTable[], uint32_t ulTableSize);
void    APIENTRY netXTransportDeinit                 ( void);
int32_t APIENTRY netXTransportStart                  ( PFN_DEVICEBROWSE_CALLBACK, void* pvUser);
void    APIENTRY netXTransportStop                   ( void);
int32_t APIENTRY netXTransportAddConnector           ( PNETX_CONNECTOR_T ptConnector);
void    APIENTRY netXTransportRemoveConnector        ( PNETX_CONNECTOR_T ptConnector);
int32_t APIENTRY netXTransportAddTranslationLayer    ( PNETX_TL_INTERFACE_T ptTLInterface, NXTHANDLE* phNetXTransportDeviceGroup);
void    APIENTRY netXTransportRemoveTranslationLayer ( NXTHANDLE hNetXTransportDeviceGroup);
void    APIENTRY netXTransportCyclicFunction         ( void);

void netXTransportGetConnectorTimeout( PNETX_CONNECTOR_T ptConnector, uint32_t* pulTimeout);

/*****************************************************************************/
/*! \}                                                                       */
/*****************************************************************************/

/*****************************************************************************/
/*!  \addtogroup NETX_TRANSPORT_TK_FUNC netXTransport internal Toolkit Functions
*    \{                                                                      */
/*****************************************************************************/
/* netXTransport Toolkit internal function interface */
uint16_t*                netXTransportGetSupportedTranslationLayer ( void);
PNETX_TL_INTERFACE_T     netXTransportGetTranslationLayer          ( uint16_t usDataType);
/* Translation-Layer related functions */
NXTHANDLE                netXTransportGetHandleToDeviceGroup       ( uint16_t  usDataTypes);
int32_t                  netXTransportGetDeviceCount               ( NXTHANDLE hNetXTransportDevGroup);
/* Translation-Layer related functions */
int32_t                  netXTransportGetTLFctTable                ( NXTHANDLE hNetXTransportDevGroup,     FUNCTION_POINTER_TABLE_T* pfnDriverFunc);
int32_t                  netXTransportGetTLFctTableByDevice        ( NXTHANDLE hNetXTransportDeviceHandle, FUNCTION_POINTER_TABLE_T* pfnDriverFunc);
NXTHANDLE                netXTransportGetTLInfo                    ( NXTHANDLE hNetXTransportDeviceHandle);
void*                    netXTransportGetTLDeviceData              ( NXTHANDLE hNetXTransportDeviceHandle);
/* Device related functions */
int32_t                  netXTransportGetDeviceHandle              ( char* szDeviceName, NXTHANDLE* hNetXTransportDeviceHandle);
NXTHANDLE                netXTransportGetDeviceHandleByNumber      ( NXTHANDLE hNetXTransportDevGroupHandle, uint32_t ulDeviceNo);
void                     netXTransportGetDeviceName                ( NXTHANDLE hNetxTransportDeviceHandle, char* szDeviceName, uint32_t ulStrlen);
int                      netXTransportAttachDevice                 ( NXTHANDLE hNetXTransportDeviceHandle);
void                     netXTransportDetachDevice                 ( NXTHANDLE hNetXTransportDeviceHandle);

/* Connection related functions */
/* Interface */
int     netXTransportCheckInterfaceState  ( NXTHANDLE hNetXTransportDevGroup);
/* Device    */
void    netXTransportActivateConnection   ( NXTHANDLE hNetXTransportDeviceHandle);
void    netXTransportDeActivateConnection ( NXTHANDLE hNetXTransportDeviceHandle);
int32_t netXTransportScheduleReconnect    ( NXTHANDLE hNetXTransportDeviceHandle, uint32_t ulTimeout);
int32_t netXTransportDisconnectDevice     ( NXTHANDLE hNetXTransportDeviceHandle);
int32_t netXTransportReconnectDevice      ( NXTHANDLE hNetXTransportDeviceHandle);
void    netXTransportRegisterReconnect    ( NXTHANDLE hNetXTransportDeviceHandle);
void    netXTransportDeRegisterReconnect  ( NXTHANDLE hNetXTransportDeviceHandle);
/*****************************************************************************/
/*! \}                                                                       */
/*****************************************************************************/

/*****************************************************************************/
/*!  \addtogroup NETX_TRANSPORT_USER_FUNC netXTransport User Functions
*    \{                                                                      */
/*****************************************************************************/
/*****************************************************************************/
/*! netXTransport User implementet functions                                 */
/*****************************************************************************/
void USER_TraceInitialize    ( NETX_TRANSPORT_DATA_T* ptnetXTransportInst);                                              /*!< Init trace                                       */
void USER_TraceDeInitialize  ( NETX_TRANSPORT_DATA_T* ptnetXTransportInst);                                              /*!< De-init trace                                    */
void USER_Trace              ( NETX_TRANSPORT_DATA_T* ptnetXTransportInst, uint32_t ulTraceLevel, char* szFormat, ...);  /*!< Trace function (writes debug info into Log-File) */
void USER_GetConnectorTimeout( PNETX_CONNECTOR_T ptConnector, uint32_t* pulTimeout);                                     /*!< Retrieves connector specific timeout             */

/*****************************************************************************/
/*! \}                                                                       */
/*****************************************************************************/

extern uint32_t g_ulTraceLevel;

#ifdef __cplusplus
}
#endif

#endif /* __NETXTRANSPORT__H */
