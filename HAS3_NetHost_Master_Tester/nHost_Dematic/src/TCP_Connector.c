/**************************************************************************************

   Copyright (c) Hilscher GmbH. All Rights Reserved.

 **************************************************************************************

   Filename:
    $Id:  $
   Last Modification:
    $Author:  $
    $Date: 2011-12-14 17:36:28 +0100 (Mi, 14 Dez 2011) $
    $Revision: 3194 $

   Targets:
     win32    : yes

   Description:
    netXTransport Connector Example (TCP)

   Changes:

     Version   Date        Author   Description
     ----------------------------------------------------------------------------------
      1        23.02.13    SD       initial version

**************************************************************************************/

#include "TCP_Connector.h"

#include <stdlib.h>
#include <stdio.h>
#include <winsock2.h>

#include "netXTransport.h"
#include "netXTransport_Errors.h"

static HANDLE           g_hConnectorDll = NULL;
static NETX_CONNECTOR_T s_tTCPConnector;

int32_t TCP_Connector_Init(void)
{
  int32_t lRet = NXT_NO_ERROR;

  memset(&s_tTCPConnector, 0, sizeof(s_tTCPConnector));

  /* try to load the tcp - connector */
  if ( NULL == (g_hConnectorDll = LoadLibrary( L".\\Plugins\\TCPConnector.dll")))
  {
    printf("Error loading Connector-Library! (Error = 0x%X)\n", GetLastError());
    /* Unable to load connector DLL */
    lRet = -1;

  } else
  {
    /* Read the functions from the DLL */
    if (((s_tTCPConnector.tFunctions.pfnConGetIdentifier      = ((PFN_NETXCON_GETIDENTIFIER)       GetProcAddress( g_hConnectorDll, "netXConGetIdentifier")))   == NULL) ||
       ((s_tTCPConnector.tFunctions.pfnConOpen               = ((PFN_NETXCON_OPEN)                GetProcAddress( g_hConnectorDll, "netXConOpen")))            == NULL) ||
       ((s_tTCPConnector.tFunctions.pfnConClose              = ((PFN_NETXCON_CLOSE)               GetProcAddress( g_hConnectorDll, "netXConClose")))           == NULL) ||
       ((s_tTCPConnector.tFunctions.pfnConSetConfig          = ((PFN_NETXCON_SETCONFIG)           GetProcAddress( g_hConnectorDll, "netXConSetConfig")))       == NULL) ||
       ((s_tTCPConnector.tFunctions.pfnConCreateInterface    = ((PFN_NETXCON_CREATEINTERFACE)     GetProcAddress( g_hConnectorDll, "netXConCreateInterface"))) == NULL) ||
       ((s_tTCPConnector.tFunctions.pfnConIntfStart          = ((PFN_NETXCON_INTF_START)          GetProcAddress( g_hConnectorDll, "netXConStartInterface")))  == NULL) ||
       ((s_tTCPConnector.tFunctions.pfnConIntfStop           = ((PFN_NETXCON_INTF_STOP)           GetProcAddress( g_hConnectorDll, "netXConStopInterface")))   == NULL) ||
       ((s_tTCPConnector.tFunctions.pfnConIntfSend           = ((PFN_NETXCON_INTF_SEND)           GetProcAddress( g_hConnectorDll, "netXConSendInterface")))   == NULL) ||
       ((s_tTCPConnector.tFunctions.pfnConIntfGetInformation = ((PFN_NETXCON_INTF_GETINFORMATION) GetProcAddress( g_hConnectorDll, "netXConGetConfig")))       == NULL) )
    {
	  printf("Error loading TCP function from TCPConnector.DLL\n");
      lRet = -1;
    } else
    {
      lRet = netXTransportAddConnector(&s_tTCPConnector);
    }
  }
  return lRet;
}

void TCP_Connector_Deinit(void)
{
  if (g_hConnectorDll != NULL)
  {
    FreeLibrary( g_hConnectorDll);

    /* we are free to remove the registerd connector since the toolkit will do this */
    netXTransportRemoveConnector( &s_tTCPConnector);
  }
}