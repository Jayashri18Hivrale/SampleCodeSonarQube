/**************************************************************************************

   Copyright (c) Hilscher GmbH. All Rights Reserved.

 **************************************************************************************

   Filename:
    $Id: netXTransport.cpp 1136 2010-04-08 13:41:37Z stephans $
   Last Modification:
    $Author: stephans $
    $Date: 2010-04-08 15:41:37 +0200 (Do, 08. Apr 2010) $
    $Revision: 1136 $

   Targets:
     Win32/ANSI   : yes
     Win32/Unicode: yes (define _UNICODE)
     WinCE        : no

   Description:
    Main module and entrypoint of the netXTransport DLL

   Changes:

     Version   Date        Author   Description
     ----------------------------------------------------------------------------------
      1        xx

**************************************************************************************/

#include <windows.h>
#include "netXTransport.h"
/**************************************************************************************
* \file netXTransport.c
* Main module and entrypoint of the netXTransport DLL
/**************************************************************************************/

HANDLE g_hModule = NULL;

extern void netXTransportDeInitcifXAPI( void);
extern void netXTransportInitcifXAPI  ( void);


BOOL APIENTRY DllMain(HANDLE hModule, DWORD dwReason, LPVOID lpReserved)
{
  UNREFERENCED_PARAMETER( lpReserved);
  switch(dwReason)
  {
  case DLL_PROCESS_ATTACH:
    /* Store our module handle */
    g_hModule = (HMODULE)hModule;

    /* NETXTRANSPORT_CIFX_API_INIT enables automatic netXTransport Toolkit initialization */
    /* configured for cifX API interface (windows specific feature).                      */
#ifdef NETXTRANSPORT_CIFX_API_INIT
    netXTransportInitcifXAPI();
#endif
    break;

  case DLL_PROCESS_DETACH:
#ifdef NETXTRANSPORT_CIFX_API_INIT
    netXTransportStop();
    netXTransportDeInitcifXAPI();
#endif
  break;

  case DLL_THREAD_ATTACH:
  case DLL_THREAD_DETACH:
    break;

  }

  return TRUE;
}
