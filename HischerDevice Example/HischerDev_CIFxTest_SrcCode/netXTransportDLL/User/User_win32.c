/**************************************************************************************

   Copyright (c) Hilscher GmbH. All Rights Reserved.

 **************************************************************************************

   Filename:
    $Id: User_win32.c 3194 2011-12-14 16:36:28Z Robert $
   Last Modification:
    $Author: Robert $
    $Date: 2011-12-14 17:36:28 +0100 (Mi, 14 Dez 2011) $
    $Revision: 3194 $

   Targets:
     win32        : yes

   Description:
     USER implemented functions called by the netXTransport Toolkit.

   Changes:

     Version   Date        Author   Description
     ----------------------------------------------------------------------------------
      1        23.02.13    SD       initial version

**************************************************************************************/

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include "netXTransport.h"
#include "HilTransportLayer.h"
#include "ConnectorAPI.h"
#include "OS_Dependent.h"

typedef struct NXT_LOG_FILE_Ttag
{
  void* hnetXTransport;
  FILE* hFile;
} NXT_LOG_FILE_T;

/*****************************************************************************/
/*! Print a trace message from netXtransport toolkit
*     \param ptDevInstance  Device instance the trace is coming from
*     \param ulTraceLevel   see TRACE_LVL_XXX defines
*     \param szFormat       printf style format string
*     \param ...            printf arguments                                 */
/*****************************************************************************/
void USER_Trace(NETX_TRANSPORT_DATA_T* ptnetXTransportInst, uint32_t ulTraceLevel, char* szFormat, ...)
{
  va_list         vaformat;
  char            szVarstring[NXT_MAX_PATH];
  char            szBuffer[NXT_MAX_PATH];
  SYSTEMTIME      SystemTime;
  NXT_LOG_FILE_T* ptLogFile;

  ptLogFile = (NXT_LOG_FILE_T*)ptnetXTransportInst->pvLogFile;

  UNREFERENCED_PARAMETER(ulTraceLevel);

  GetLocalTime( &SystemTime);
  sprintf_s( szBuffer, NXT_MAX_PATH,"%.2d.%.2d.%.4d %.2d:%.2d:%.2d.%.3d: ",
    SystemTime.wDay,
    SystemTime.wMonth,
    SystemTime.wYear,
    SystemTime.wHour,
    SystemTime.wMinute,
    SystemTime.wSecond,
    SystemTime.wMilliseconds);

  va_start( vaformat, szFormat);

  vsnprintf_s( szVarstring, NXT_MAX_PATH, NXT_MAX_PATH-strlen(szBuffer),szFormat, vaformat);
  va_end( vaformat);

  strcat_s(szBuffer, NXT_MAX_PATH, szVarstring);

  /* check if log file exists */
  if (ptLogFile && ptLogFile->hFile)
  {
    fprintf(ptLogFile->hFile, "%s\n",szBuffer);
  } else
  {
    printf("%s\n",szBuffer);
  }
}

/*****************************************************************************/
/*! Initialize trace
*     \param ptnetXTransportInst Pointer to netXTransport instance           */
/*****************************************************************************/
void USER_TraceInitialize(NETX_TRANSPORT_DATA_T* ptnetXTransportInst)
{
  NXT_LOG_FILE_T* ptLogFile;

  ptLogFile = OS_Memalloc(sizeof(NXT_LOG_FILE_T));
  OS_Memset(ptLogFile, 0, sizeof(NXT_LOG_FILE_T));

  ptnetXTransportInst->pvLogFile = (void*)ptLogFile;

  /* check if log file exists */
  if (ptLogFile)
  {
    int iRet;
    if (0 != ( iRet = fopen_s( &ptLogFile->hFile, "netXTransport.log","w+")))
    {
      printf("Error while creating log file. Print Debug-Messages to console!\n");
    }
  }
  /* log file header */
  USER_Trace(ptnetXTransportInst, 0xFF, "----- netXTransport Log started ---------------------\n");
  USER_Trace(ptnetXTransportInst, 0xFF, "netXTransport Toolkit-Version: %d.%d.%d.%d\n", NXT_VERSION_MAJOR, NXT_VERSION_MINOR, NXT_VERSION_REV, NXT_VERSION_BUILD);
}

/*****************************************************************************/
/*! De-initialize trace
*     \param ptnetXTransportInst Pointer to netXTransport instance           */
/*****************************************************************************/
void USER_TraceDeInitialize(NETX_TRANSPORT_DATA_T* ptnetXTransportInst)
{
  void* pvLogFile = ptnetXTransportInst->pvLogFile;

  if (pvLogFile)
  {
    NXT_LOG_FILE_T* ptLogFile = pvLogFile;
    if (ptLogFile->hFile)
    {
      fclose(ptLogFile->hFile);
    }
    OS_Memfree(ptLogFile);
    ptnetXTransportInst->pvLogFile = NULL;
  }
}

/*****************************************************************************/
/*! Function retrieves connector specific timeout
*     \param ptConnector  Pointer to connector
*     \param ptConnector  Pointer to returned timeout
/*****************************************************************************/
void USER_GetConnectorTimeout( PNETX_CONNECTOR_T ptConnector, uint32_t* pulTimeout)
{
  if (pulTimeout == NULL)
    return;

  UNREFERENCED_PARAMETER( ptConnector);

  /* currently set to default timeout */
  *pulTimeout = 100;
}
