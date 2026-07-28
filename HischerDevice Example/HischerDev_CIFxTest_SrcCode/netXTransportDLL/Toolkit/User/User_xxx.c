/**************************************************************************************

   Copyright (c) Hilscher GmbH. All Rights Reserved.

 **************************************************************************************

   Filename:
    $Id: User_xxx.c 3194 2011-12-14 16:36:28Z Robert $
   Last Modification:
    $Author: Robert $
    $Date: 2011-12-14 17:36:28 +0100 (Mi, 14 Dez 2011) $
    $Revision: 3194 $

   Targets:
     xxx

   Description:
     USER implemented functions called by the netXTransport Toolkit.

   Changes:

     Version   Date        Author   Description
     ----------------------------------------------------------------------------------
      1        23.02.13    SD       initial version

**************************************************************************************/

#include "netXTransport.h"
#include "HilTransportLayer.h"
#include "ConnectorAPI.h"
#include "OS_Dependent.h"

/*****************************************************************************/
/*! Print a trace message from netXtransport toolkit
*     \param ptDevInstance  Device instance the trace is coming from
*     \param ulTraceLevel   see TRACE_LVL_XXX defines
*     \param szFormat       printf style format string
*     \param ...            printf arguments                                 */
/*****************************************************************************/
void USER_Trace(NETX_TRANSPORT_DATA_T* ptnetXTransportInst, uint32_t ulTraceLevel, char* szFormat, ...)
{
  UNREFERENCED_PARAMETER( ptnetXTransportInst);
  UNREFERENCED_PARAMETER( ulTraceLevel);
  UNREFERENCED_PARAMETER( szFormat);

}

/*****************************************************************************/
/*! Initialize trace
*     \param ptnetXTransportInst Pointer to netXTransport instance           */
/*****************************************************************************/
void USER_TraceInitialize(NETX_TRANSPORT_DATA_T* ptnetXTransportInst)
{
  UNREFERENCED_PARAMETER( ptnetXTransportInst);
}

/*****************************************************************************/
/*! De-initialize trace
*     \param ptnetXTransportInst Pointer to netXTransport instance           */
/*****************************************************************************/
void USER_TraceDeInitialize(NETX_TRANSPORT_DATA_T* ptnetXTransportInst)
{
  UNREFERENCED_PARAMETER( ptnetXTransportInst);
}

/*****************************************************************************/
/*! Function retrieves connector specific timeout
*     \param ptConnector  Pointer to connector
*     \param ptConnector  Pointer to returned timeout
/*****************************************************************************/
void USER_GetConnectorTimeout( PNETX_CONNECTOR_T ptConnector, uint32_t* pulTimeout)
{
  UNREFERENCED_PARAMETER( ptConnector);
  UNREFERENCED_PARAMETER( pulTimeout);
}
