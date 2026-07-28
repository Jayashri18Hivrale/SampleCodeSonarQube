/**************************************************************************************

   Copyright (c) Hilscher GmbH. All Rights Reserved.

 **************************************************************************************

   Filename:
    $Id: HilTransportLayer.h 149 2013-06-10 07:25:40Z sebastiand $
   Last Modification:
    $Author: sebastiand $
    $Date: 2013-06-10 09:25:40 +0200 (Mo, 10. Jun 2013) $
    $Revision: 149 $

   Targets:
     OS-Independent : yes

   Description:
     Declaration of Transport Layer Interface

   Changes:

     Version   Date        Author   Description
     ----------------------------------------------------------------------------------
     1         23.02.13    SD/MT       initial version

**************************************************************************************/

/****************************************************************************/
/*! \file HilTransportLayer.h
    Declaration of Transport Layer Interface                                */
/****************************************************************************/

#ifndef __TRANSPORTLAYER__H
#define __TRANSPORTLAYER__H

#include <sys/queue.h>
#include "HilTransport.h"
#include "netXTransport.h"

/*!< - */
#define NXTHANDLE void*

#ifdef __cplusplus
extern "C"
{
#endif

/*****************************************************************************/
/*! HilTransport Keep-Alive State Definition                                 */
/*****************************************************************************/
typedef enum
{
  eKEEP_ALIVE_UNSUPPORTED,     /*!< No keep-alive support by device */
  eKEEP_ALIVE_INITIALIZATION,  /*!< Initialization phase            */
  eKEEP_ALIVE_ACTIVE,          /*!< Keep alive active               */
  eKEEP_ALIVE_TIMEOUT,         /*!< Keep alive times out            */

} KEEPALIVE_STATE_E;

/*****************************************************************************/
/*! HilTransportPacket structure definition                                  */
/*****************************************************************************/
typedef struct HIL_TRANSPORT_PACKET_Ttag
{
  TAILQ_ENTRY(HIL_TRANSPORT_PACKET_Ttag) tList;       /*!< List entry                                             */
  void*                                  hTransport;  /*!< Handle to the transport layer the packet is related to */
  PHIL_TRANSPORT_HEADER                  ptHeader;    /*!< Pointer to the HiltTransport-Header                    */
  uint8_t*                               pbData;      /*!< Pointer to the data block                              */
  uint32_t                               ulMaxSize;   /*!< Size of the data block                                 */

} HIL_TRANSPORT_PACKET_T, *PHIL_TRANSPORT_PACKET_T /*!< Pointer to HIL_TRANSPORT_PACKET_T */;

/* function declaration */
typedef void (*PFN_NETXCON_DEVICE_NOTIFY_CALLBACK)( const char* szInterfaceName, const char* szDeviceName, uint32_t ulDeviceIdentifier, NETX_INTERFACE_NOTIFICATION_E eNotify, void* hTransport, void* pvDevicePrivateData);  /*!< Device notification callback */

int32_t   HilTransportPrepare ( void*             hnetXTransport);
int32_t   HilTransportInit    ( PNETX_CONNECTOR_T ptConnector, const char* szDeviceName, void* pvConnectorData, uint16_t* ausSupprotedTL, NXTHANDLE* phTransport, PFN_NETXCON_DEVICE_NOTIFY_CALLBACK pfnNotCB);
void      HilTransportDeinit  ( NXTHANDLE         hTransport);

int32_t   HilTransportSoftReconnect           ( NXTHANDLE hTransport, uint32_t ulDeviceIdentifier, PFN_NETXCON_DEVICE_NOTIFY_CALLBACK pfnDevNotify);
void      HilTransportStopPendingTransactions ( NXTHANDLE hTransport);
void      HilTransportRestoreConnectionStates ( NXTHANDLE hnetXTransportDevHandle, NXTHANDLE hTransport);

NXTHANDLE HilTransportGetTLHandle     ( NXTHANDLE hTransport);
uint16_t  HilTransportGetDefaultTLType( NXTHANDLE hTransport);

PHIL_TRANSPORT_PACKET_T HilTransportAllocatePacket  ( NXTHANDLE hTransport, uint32_t ulDataSize, uint16_t usDataType);
void                    HilTransportFreePacket      ( PHIL_TRANSPORT_PACKET_T ptPacket);
void                    HilTransportSendAcknowledge ( NXTHANDLE hTransport, PHIL_TRANSPORT_HEADER ptHeader, uint8_t  bState);
int32_t                 HilTransportSendPacket      ( NXTHANDLE hTransport, HIL_TRANSPORT_HEADER* ptPacket, uint32_t ulTimeout);
int32_t                 HilTransportTransferPacket  ( NXTHANDLE hTransport, HIL_TRANSPORT_HEADER* ptPacket, PHIL_TRANSPORT_PACKET_T* pptRecvPacket, uint32_t ulTimeout);
KEEPALIVE_STATE_E       HilTransportHandleKeepAlive ( NXTHANDLE hTransport);

#ifdef __cplusplus
}
#endif

#endif /* __TRANSPORTLAYER__H */
