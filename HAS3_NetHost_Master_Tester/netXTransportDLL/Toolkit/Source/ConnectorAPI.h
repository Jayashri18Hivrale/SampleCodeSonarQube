/**************************************************************************************

   Copyright (c) Hilscher GmbH. All Rights Reserved.

 **************************************************************************************

   Filename:
    $Id: ConnectorAPI.h 152 2013-06-14 10:49:29Z sebastiand $
   Last Modification:
    $Author: sebastiand $
    $Date: 2013-06-14 12:49:29 +0200 (Fr, 14. Jun 2013) $
    $Revision: 152 $

   Targets:
     O/S Independent : yes

   Description:
    Declaration of the physical interface (connectors)

   Changes:

     Version   Date        Author   Description
     ----------------------------------------------------------------------------------
      1         23.02.13    MT       initial version

**************************************************************************************/

/****************************************************************************/
/*! \file ConnectorAPI.h
    General declarations of the physical interface (connectors)             */
/****************************************************************************/

#ifndef __CONNECTORAPI__H
#define __CONNECTORAPI__H

#include <OS_Includes.h>

#include <stdint.h>
#include <sys/queue.h>

#ifdef __cplusplus
extern "C"
{
#endif

/*****************************************************************************/
/*! Global definitions                                                       */
/*****************************************************************************/
#define NXCON_MAX_LENGTH_CONNECTOR_IDENTIFIER    6    /*!< Maximum length of the connector identifier          */
#define NXCON_DESCRIPTION_LENGTH                64    /*!< Maximum length of a connector description           */

#define NXCON_DEFAULT_INTERFACE_NAME_LEN        32    /*!< Maximum length of a interface name                  */
#define NXCON_DEFAULT_HUMANREADABLE_NAME_LEN    64    /*!< Maximum length of the interface human readable name */

#define NXCON_MAX_INTFACE_CNT                   99    /*!< Maximum allowed count of interfaces per connector   */

#define NETX_CONNECTOR_TYPE_DPM       0x00000001      /*!< DPM connector                                       */
#define NETX_CONNECTOR_TYPE_USB       0x00000002      /*!< USB connector                                       */
#define NETX_CONNECTOR_TYPE_SERIAL    0x00000004      /*!< UART/Serial connector                               */
#define NETX_CONNECTOR_TYPE_ETHERNET  0x00000008      /*!< Ethernet connector                                  */
#define NETX_CONNECTOR_TYPE_CUSTOM    0x80000000      /*!< Custom connector (unspecified)                      */

/*****************************************************************************/
/*! UU-Identifier of the connector                                           */
/*****************************************************************************/
typedef struct NETX_TRANSPORT_UUID_Ttag
{
  uint32_t  ulData1;     /*!< - */
  uint16_t  usData2;     /*!< - */
  uint16_t  usData3;     /*!< - */
  uint8_t   abData4[8];  /*!< - */
} NETX_TRANSPORT_UUID_T;

/*****************************************************************************/
/*! Signals for interface state notification                                 */
/*****************************************************************************/
typedef enum NETX_INTERFACE_NOTIFICATION_Etag
{
  eNXT_DEVICE_ATTACHED = 0,       /*!< New Interface attached (e.g. plugged usb)   */
  eNXT_DEVICE_DETACHED,           /*!< Interface detached (e.g. unplugged usb)     */
  eNXT_DEVICE_DELETED,            /*!< Interface deleted (e.g. Connector reconfig) */
} NETX_INTERFACE_NOTIFICATION_E;

/*****************************************************************************/
/*! Translation structure for translating short --> full device name         */
/*****************************************************************************/
typedef struct INTERFACE_NAME_TRANSLATION_Ttag
{
  const char*   szSource;           /*!< Source device name (short)         */
  char*         szDestination;      /*!< Returned long device name          */
  uint32_t      ulDestinationSize;  /*!< Length of the buffer szDestination */

} INTERFACE_NAME_TRANSLATION_T;

/*****************************************************************************/
/*! Commands used with netXConGetInformation() to query connector information */
/*****************************************************************************/
typedef enum NETX_CONNECTOR_INFO_Etag
{
  eSTATE,                             /*!< Get state of connector ( Enable / Disable )    */
  eUUID,                              /*!< Get UUID of the connector                      */
  eTYPE,                              /*!< Get type of the connector                      */
  eESTABLISH_TIMEOUT,                 /*!< Get timeout for intial device query            */
  eIDENTIFIER,                        /*!< Get connector identifier (e.g. COM, TCP)       */

  eSHORT_INTERFACE_NAME,              /*!< Interface name (e.g. TCP0)                     */
  eLONG_INTERFACE_NAME,               /*!< Human readable name (e.g. IP192.168.1.2:50111) */
  eDESCRIPTION,                       /*!< Get description of the connector               */

} NETX_CONNECTOR_INFO_E;

/*****************************************************************************/
/*! Commands used with netXConGetInformationInterface() to query interface information */
/*****************************************************************************/
typedef enum NETX_INTERFACE_INFO_Etag
{
  eINTERFACE_STATE,                   /*!< Get state of interface                         */
  eSEND_TIMEOUT,                      /*!< Get send timeout                               */
  eRESET_TIMEOUT,                     /*!< Get reset timeout                              */
  eKEEPALIVE_TIMEOUT,                 /*!< Get keep alive timeout                         */

} NETX_INTERFACE_INFO_E;

/*****************************************************************************/
/*! Commands used with netXConGetConfig() / netXConSetConfig() to query/set connector configuration */
/*****************************************************************************/
typedef enum NETX_CONNECTOR_CONFIG_CMD_Etag
{
  eCMD_CONFIG_GETLEN,                 /*!< Get length of global configuration string      */
  eCMD_CONFIG_GETSTRING,              /*!< Get global configuration                       */
  eCMD_CONFIG_SETSTRING,              /*!< Set global configuration                       */
  eCMD_DIALOG_GETLEN,                 /*!< Get length of configuration string from dialog */
  eCMD_DIALOG_GETSTRING,              /*!< Get configuration string from dialog           */
  eCMD_DIALOG_SETSTRING,              /*!< Set configuration of dialog                    */

} NETX_CONNECTOR_CONFIG_CMD_E;

/*****************************************************************************/
/*! List of interface communication states                                   */
/*****************************************************************************/
typedef enum NETX_INTERFACE_STATE_Etag
{
  eINTERFACE_STATE_NOT_SET,           /*!< Interface invalid, no information are valid   */
  eINTERFACE_STATE_NOT_INIT,          /*!< Interface object created, but not initialized */
  eINTERFACE_STATE_INITIALIZED,       /*!< Interface is initialized                      */
  eINTERFACE_STATE_AVAILABLE,         /*!< Interface is available, but not running       */
  eINTERFACE_STATE_NOT_AVAIL,         /*!< Interface not mor available                   */
  eINTERFACE_STATE_RUNNING,           /*!< Interface is ready to handle communication    */
  eINTERFACE_STATE_STOPPED,           /*!< Interface was stopped                         */
  eINTERFACE_STATE_DISABLED,          /*!< Interface is disabled by user                 */

} NETX_INTERFACE_STATE_E;

/*! not documented */
#define NETX_CONNECTOR_INFO_STATE_ENABLED   0x00000001

/*****************************************************************************/
/*! Union holding the state of a connector                                   */
/*****************************************************************************/
typedef struct NETX_CONNECTOR_INFO_STATE_Ttag
{
  uint32_t   ulState;            /*!< State of connector */

} NETX_CONNECTOR_INFO_STATE_T;

/*****************************************************************************/
/*  Connecter API Functions                                                  */
/*****************************************************************************/
/*! Connector callback to notify upper layers about interface attachment/detachment */
typedef void (*PFN_NETXCON_INTERFACE_NOTIFY_CALLBACK)     ( const char* szDeviceName, NETX_INTERFACE_NOTIFICATION_E eNotify, void* pvConnectorData, void* pvUser);

/*! Connector callback to pass received data to upper layers */
typedef void (*PFN_NETXCON_DEVICE_RECEIVE_CALLBACK)    ( uint8_t* pabRxBuffer, uint32_t ulReadLen, void* pvUser);

/*! Connector interface functions */
/*! Connector callback to pass received data to upper layers */
typedef int32_t  (APIENTRY *PFN_NETXCON_GETIDENTIFIER)       ( char* szIdentifier, NETX_TRANSPORT_UUID_T* ptUUID);
typedef int32_t  (APIENTRY *PFN_NETXCON_OPEN)                ( PFN_NETXCON_INTERFACE_NOTIFY_CALLBACK pfnDevNotifyCallback, void* pvUser);
typedef int32_t  (APIENTRY *PFN_NETXCON_CLOSE)               ( void);
typedef void*    (APIENTRY *PFN_NETXCON_CREATEINTERFACE)     ( const char* szDeviceName);
typedef int32_t  (APIENTRY *PFN_NETXCON_GETINFORMATION)      ( NETX_CONNECTOR_INFO_E eCmd, uint32_t ulSize, void* pvConnectorInfo);
typedef int32_t  (APIENTRY *PFN_NETXCON_GETCONFIG)           ( NETX_CONNECTOR_CONFIG_CMD_E eCmd, void* pvConfig);
typedef int32_t  (APIENTRY *PFN_NETXCON_SETCONFIG)           ( NETX_CONNECTOR_CONFIG_CMD_E eCmd, const char* szConfig);

typedef int32_t  (APIENTRY *PFN_NETXCON_INTF_START)          ( void* pvInterface, PFN_NETXCON_DEVICE_RECEIVE_CALLBACK pfnReceiveData, void* pvUser);
typedef int32_t  (APIENTRY *PFN_NETXCON_INTF_STOP)           ( void* pvInterface);
typedef int32_t  (APIENTRY *PFN_NETXCON_INTF_SEND)           ( void* pvInterface, uint8_t* pabData, uint32_t ulDataLen);
typedef int32_t  (APIENTRY *PFN_NETXCON_INTF_GETINFORMATION) ( void* pvInterface, NETX_INTERFACE_INFO_E eCmd, uint32_t ulSize, void* pvInterfaceInfo);

typedef void*    (APIENTRY *PFN_NETXCON_CREATEDIALOG)        ( void* hParentWnd, const char* szConfig);
typedef void     (APIENTRY *PFN_NETXCON_ENDDIALOG)           ( void);

/*****************************************************************************/
/*! Connector interface function table                                       */
/*****************************************************************************/
typedef struct NETX_CONNECTOR_FUNCTION_TABLEtag
{
  PFN_NETXCON_GETIDENTIFIER       pfnConGetIdentifier;          /*!< Get identifier of the connector (e.g. COM) */
  PFN_NETXCON_OPEN                pfnConOpen;                   /*!< Open connector                             */
  PFN_NETXCON_CLOSE               pfnConClose;                  /*!< Close connector                            */
  PFN_NETXCON_CREATEINTERFACE     pfnConCreateInterface;        /*!< Create a device interface                  */
  PFN_NETXCON_GETINFORMATION      pfnConGetInformation;         /*!< Request connector information              */
  PFN_NETXCON_GETCONFIG           pfnConGetConfig;              /*!< Get connector configuration                */
  PFN_NETXCON_SETCONFIG           pfnConSetConfig;              /*!< Set connector configuration                */

  PFN_NETXCON_INTF_START          pfnConIntfStart;              /*!< Start connector interface                  */
  PFN_NETXCON_INTF_STOP           pfnConIntfStop;               /*!< Stop connector interface                   */
  PFN_NETXCON_INTF_SEND           pfnConIntfSend;               /*!< Send data via the connector interface      */
  PFN_NETXCON_INTF_GETINFORMATION pfnConIntfGetInformation;     /*!< Request interface information              */

  PFN_NETXCON_CREATEDIALOG        pfnConCreateDialog;           /*!< Create configuration dialog of connector  */
  PFN_NETXCON_ENDDIALOG           pfnConEndDialog;              /*!< Remove configuration dialog of connector  */

} NETX_CONNECTOR_FUNCTION_TABLE, *PNETX_CONNECTOR_FUNCTION_TABLE /*!< Pointer to NETX_CONNECTOR_FUNCTION_TABLE */;

/*****************************************************************************/
/*! Generic connector information structure                                  */
/*****************************************************************************/
typedef struct NETX_CONNECTOR_Ttag
{
  TAILQ_ENTRY(NETX_CONNECTOR_Ttag) tList;                                                  /*!< List entry                                           */
  NETX_CONNECTOR_FUNCTION_TABLE    tFunctions;                                             /*!< Interface function table                             */
  char                             szConIdentifier[NXCON_MAX_LENGTH_CONNECTOR_IDENTIFIER]; /*!< Identifier of connection, realized by this connector */
  NETX_TRANSPORT_UUID_T            tConnectorUUID;                                         /*!< UU-Identifier of connector                           */

  uint32_t                         ulConnectorInfoSize;
  void*                            pvConnectorInfo;

} NETX_CONNECTOR_T, *PNETX_CONNECTOR_T /*!< Pointer to NETX_CONNECTOR_T */;

#ifdef __cplusplus
}
#endif

#endif /* __CONNECTORAPI__H */
