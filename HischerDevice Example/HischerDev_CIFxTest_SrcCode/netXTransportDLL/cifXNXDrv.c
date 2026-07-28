/**************************************************************************************

   Copyright (c) Hilscher GmbH. All Rights Reserved.

 **************************************************************************************

   Filename:
    $Id: cifXNXDrv.c 149 2013-06-10 07:25:40Z sebastiand $
   Last Modification:
    $Author: sebastiand $
    $Date: 2013-06-10 09:25:40 +0200 (Mo, 10. Jun 2013) $
    $Revision: 149 $

   Targets:
     win32 : yes

   Description:
    Implementation of the cifX netXTransport configuration interface

   Changes:

     Version   Date        Author   Description
     ----------------------------------------------------------------------------------
      1        11.06.13    SD       initial version

**************************************************************************************/

/****************************************************************************/
/*! \file cifXNXDrv.c
    Implementation of NXDRV API                                             */
/****************************************************************************/

/*****************************************************************************/
/*****************************************************************************/
/*   EXAMPLE IMPLEMENTATION OF THE CONFIGURATION INTERFACE                   */
/*****************************************************************************/
/*****************************************************************************/

#include <windows.h>
#include <io.h>
#include <stdio.h>
#include "netXTransport.h"
#include "HilTransportLayer.h"
#include "rcX_User.h"
#include "cifXUser.h"
#include "cifXErrors.h"
#include "netxapi.h"
#include "cifXAPI.h"
#include "ConnectorAPI.h"
#include "OS_Dependent.h"
#include "OS_Includes.h"
#include "TL_Marshaller.h"
#include "TL_rcXPacket.h"

#define NXCON_DLL_PLUGIN_DIR         "Plugins"    /*!< Directory containing the connector libraries        */
#define NXCON_DLL_SEARCH_STRING      "*.dll"      /*!< Search string for connector file names              */

/* extern variables */
extern HMODULE            g_hModule;
extern CIFX_DRIVER_INFO_T g_ptCifXDrvInfo;     /*!< Global cifX driver information */

/* list of available device        */
TAILQ_HEAD(NXDRV_DEVICE_LIST_T, NXDRV_DEVICE_Ttag);
/* list of netXtransport connector */
TAILQ_HEAD(NETX_CONNECTOR_LIST_T, NETX_CONNECTOR_DLL_INFO_Ttag);

/*****************************************************************************/
/*! NXDRV structure definition                                               */
/*****************************************************************************/
typedef struct NXDRV_INFO_Ttag
{
  TL_INIT_T                     tDataLayerInit[2]; /*!< List of supported translation layers                      */
  struct NETX_CONNECTOR_LIST_T  tConnectorList;    /*!< List of registered connectors                             */
  struct NXDRV_DEVICE_LIST_T    tDeviceList;       /*!< List of detected devices                                  */
  DWORD                         dwThreadID;        /*!< ID of the cyclic thread (netXTransport cyclic thread)     */
  HANDLE                        hThread;           /*!< Handle of the cyclic thread (netXTransport cyclic thread) */
  char*                         szLastInterface;   /*!< Last notified interface                                   */
  uint32_t                      ulInterfaceCount;  /*!< Interface counter                                         */
  int                           fInitialized;

} NXDRV_INFO_T;

typedef struct NXDRV_DEVICE_Ttag
{
  TAILQ_ENTRY(NXDRV_DEVICE_Ttag) tList;         /*!< List entry                       */
  BOARD_INFORMATION              tBoardInfo;    /*!< cifX board information structure */
  NXDRV_DEVICE_INFORMATION       tDevInfo;      /*!< NXDRV device information         */

} NXDRV_DEVICE_T, *PNXDRV_DEVICE_T              /*!< Pointer to NXDRV_DEVICE_T */;

/*! Union holding the type of the connector */
typedef union NETX_CONNECTOR_TYPE_Utag
{
  struct NETX_CONNECTOR_TYPE_BF_Ttag
  {
    unsigned int  fNXDRV_TYPE_DPM       : 1;     /*!< Flag, to signal connector type DPM      */
    unsigned int  fNXDRV_TYPE_USB       : 1;     /*!< Flag, to signal connector type USB      */
    unsigned int  fNXDRV_TYPE_SERIAL    : 1;     /*!< Flag, to signal connector type SERIAL   */
    unsigned int  fNXDRV_TYPE_ETHERNET  : 1;     /*!< Flag, to signal connector type ETHERNET */
    unsigned int  ulReserved            : 28;    /*!< Reserved for future                     */
  } tBfType;
  unsigned long   ulType;                        /*!< Type of connector                       */

} NETX_CONNECTOR_TYPE_U;

/*! Global connector DLL information structure */
typedef struct NETX_CONNECTOR_DLL_INFO_Ttag
{
  TAILQ_ENTRY(NETX_CONNECTOR_DLL_INFO_Ttag) tList;                                        /*!< List entry                                           */
  HMODULE                         hDll;                                                   /*!< DLL handle                                           */
  unsigned long                   ulError;                                                /*!< DLL load error                                       */
  char                            szFileName[NXCON_FILE_NAME_LENGTH];                     /*!< File name of the DLL                                 */
  char                            szFullFileName[NXCON_FILE_NAME_LENGTH];                 /*!< Full file name of the DLL                            */
  char                            szConIdentifier[NXCON_MAX_LENGTH_CONNECTOR_IDENTIFIER]; /*!< Identifier of connection, realized by this connector */
  char                            szConnectorUUID[NXCON_UUID_STRING_SIZE];                /*!< UUID as string                                       */
  char                            szDescription[NXCON_DESCRIPTION_LENGTH];                /*!< Connector description                                */
  NETX_CONNECTOR_TYPE_U           tType;                                                  /*!< Type of connector                                    */
  NETX_CONNECTOR_FUNCTION_TABLE   tFncTable;                                              /*!< Function table                                       */

  NETX_CONNECTOR_T                tNetXTransportConnectorInfo;

} NETX_CONNECTOR_DLL_INFO_T, *PNETX_CONNECTOR_DLL_INFO_T;

/* function declaration */
static int32_t      TLLayerInit        ( void* pvParam);
static void         TLLayerDeInit      ( void* pvParam);
static int32_t      GetPluginPath      ( uint32_t ulSize, char* szPluginPath );
static int32_t      SearchConnectorDlls( char *szPluginPath);
static int32_t      Connector_Load     ( NETX_CONNECTOR_DLL_INFO_T* ptConnectorInfo);
static DWORD WINAPI CyclicThread       ( LPVOID lpParameter);

/* global variables */
static NXDRV_INFO_T s_tNXDrv_Info      = {0}; /*!< global NXDRV API information structure */
static int          s_fRunCyclicThread = 0;

/***************************************************************************
*
*
*   netX API Global Functions
*
*
*
*
*
******************************************************************************/

void netXTransportInitcifXAPI( void)
{
  int32_t   lRet                   = NXT_NO_ERROR;
  char      szPluginPath[MAX_PATH] = {0};

  OS_Memset(&s_tNXDrv_Info, 0, sizeof(s_tNXDrv_Info));

  TAILQ_INIT( &s_tNXDrv_Info.tDeviceList);
  TAILQ_INIT( &s_tNXDrv_Info.tConnectorList);

  /* setup netXTransport initialization structure */
  s_tNXDrv_Info.tDataLayerInit[0].pfnTLInit   = TLLayerInit;   /* function pointer the Translation-Layer initialization    */
  s_tNXDrv_Info.tDataLayerInit[0].pfnTLDeInit = TLLayerDeInit; /* function pointer the Translation-Layer de-initialization */
  s_tNXDrv_Info.tDataLayerInit[0].pvData      = NULL;

  if (NXT_NO_ERROR != (lRet = netXTransportInit( s_tNXDrv_Info.tDataLayerInit, sizeof(s_tNXDrv_Info.tDataLayerInit))))
  {
    lRet = NXAPI_DRIVER_NOT_INITIALIZED;
  } else
  {
    /* Query plugin directory containing the connector libraries, */
    /* search for connector libraries and load them               */
    if ( NXAPI_NO_ERROR == (lRet= GetPluginPath( MAX_PATH, szPluginPath)))
    {
      lRet= SearchConnectorDlls( szPluginPath);
    }
  }
  /* start netXTransport cyclic thread */
  s_tNXDrv_Info.hThread = CreateThread( NULL, 0, CyclicThread, NULL, 0, &s_tNXDrv_Info.dwThreadID);

  s_tNXDrv_Info.fInitialized = 1;
}

void netXTransportDeInitcifXAPI( void)
{
  if (s_tNXDrv_Info.fInitialized == 1)
  {
    s_fRunCyclicThread = 0;

    if (s_tNXDrv_Info.hThread != NULL)
    {
      if (WAIT_TIMEOUT == WaitForSingleObject( s_tNXDrv_Info.hThread, 1000))
        TerminateThread( s_tNXDrv_Info.hThread, 0xFFFFFFFF);
    }
    netXTransportDeinit();

    if (s_tNXDrv_Info.szLastInterface != NULL)
      OS_Memfree( s_tNXDrv_Info.szLastInterface);
  }
}

/*! **************************************************************************
* Read netX API information
*   \param ulInfoSize    Size of the user buffer
*   \param ptDrvInfo     Pointer to the user information buffer
*   \return NAXPI_NO_ERROR on success                                         */
/******************************************************************************/
int32_t APIENTRY nxDrvGetInformation ( uint32_t ulInfoSize, PNXDRV_HW_INFORMATION ptDrvInfo)
{
  NETX_CONNECTOR_DLL_INFO_T* ptNetXTransportCon;

  if (NULL == ptDrvInfo) { return NXAPI_INVALID_PARAMETER; }
  if ( ulInfoSize < sizeof(*ptDrvInfo))
    return NXAPI_BUFFER_TOO_SHORT;

  OS_Strvsprintf( ptDrvInfo->szVersion, sizeof(ptDrvInfo->szVersion)/sizeof(ptDrvInfo->szVersion[0]),
                "%d.%d.%d.%d",
                CIFX_DRV_VERSION_MAJOR,
                CIFX_DRV_VERSION_MINOR,
                CIFX_DRV_VERSION_BUILD,
                CIFX_DRV_VERSION_REV);

  OS_Strncpy( ptDrvInfo->szDriverName, sizeof(ptDrvInfo->szDriverName)/sizeof(ptDrvInfo->szDriverName[0]), CIFX_DRIVER_NAME, OS_Strlen(CIFX_DRIVER_NAME));

  ptDrvInfo->ulDriverType = 0;
  TAILQ_FOREACH(ptNetXTransportCon, &s_tNXDrv_Info.tConnectorList, tList);
  {
	  if (NULL == ptNetXTransportCon) { return NXAPI_INVALID_PARAMETER; }
    ptDrvInfo->ulDriverType = ptDrvInfo->ulDriverType | ptNetXTransportCon->tType.ulType;
  }
  ptDrvInfo->ulDeviceClass        = RCX_HW_DEV_CLASS_UNDEFINED;
  ptDrvInfo->ulDriverRequirements = NXDRV_REQ_STARTUP_SW;

  return NXAPI_NO_ERROR;
}

/*! **************************************************************************
* Initialize API interface
*   \return NXAPI_NO_ERROR on success                                         */
/******************************************************************************/
int32_t APIENTRY nxDrvInit ( void)
{
  int32_t lRet = NXT_NO_ERROR;

  return lRet;
}

/*! ***************************************************************************
* Deinitialize API interface
*   \return NXAPI_NO_ERROR on success                                         */
/******************************************************************************/
int32_t APIENTRY nxDrvExit ( void)
{
  int32_t lRet = NXT_NO_ERROR;

  return lRet;
}

/*! **************************************************************************
* Find available devices
*   \param  ulCmd         Command for the FindDevice function
*   \param  ulInfoSize    Size of the information structure
*   \param  ptDeviceInfo  Information of the device
*   \param  pulSearchIdx  Search index
*   \return NAXPI_NO_ERROR on success                                         */
/******************************************************************************/
int32_t APIENTRY nxDrvFindDevice(uint32_t                  ulCmd,
                                 uint32_t                  ulInfoSize,
                                 NXDRV_DEVICE_INFORMATION* ptDeviceInfo,
                                 uint32_t*                 pulSearchIdx)
{
  int32_t         lRet        = NXAPI_NO_ENTRIES;
  NXDRV_DEVICE_T* ptDevice    = NULL;
  uint32_t        ulDeviceIdx = 0;

  /* Check incomming parameters */
  if( (NULL == ptDeviceInfo) ||
      (NULL == pulSearchIdx))
    return NXAPI_INVALID_PARAMETER;

  if(ulInfoSize < (uint32_t)sizeof(NXDRV_DEVICE_INFORMATION))
    return NXAPI_BUFFER_TOO_SHORT;

  switch (ulCmd)
  {
    case NXDRV_FIND_FIRST:
      *pulSearchIdx = 0;
      break;

    case NXDRV_FIND_NEXT:
      break;

    default:
      return NXAPI_UNKOWN_COMMAND;
  }
  /* In earlier versions, the device list was created during nxDrvInit(). */
  /* But this takes too long, depending on how many interfaces must be searched. */
  /* New application using the nxDrvBrowseDevices() functions which supports a progress call back.*/
  /* For older application we now to call nxDrvBrowseDevices() before looking into the device list.*/
  /* Otherwise the device list ist always empty!!! */
  if( NXDRV_FIND_FIRST == ulCmd )
    lRet = nxDrvBrowseDevices( NULL, NULL);

  /* Search for device in list */
  TAILQ_FOREACH( ptDevice, &s_tNXDrv_Info.tDeviceList, tList)
  {
    if (*pulSearchIdx == ulDeviceIdx)
    {
      OS_Memcpy( ptDeviceInfo, &ptDevice->tDevInfo, sizeof(ptDevice->tDevInfo));
      *pulSearchIdx = (*pulSearchIdx) + 1;
      lRet = NXAPI_NO_ERROR;
      break;
    }
    ulDeviceIdx++;
  }
  return lRet;
}

/* browse devices (nxDrvBrowseDevices()) callback wrapper structure */
typedef struct BROWSE_DEV_CB_Ttag
{
  PFN_NXAPI_BROWSE_CALLBACK pfnCallback;  /*!< User callback  */
  void*                     pvUser;       /*!< User parameter */

} BROWSE_DEV_CB_T;

/*! **************************************************************************
* Enumeration callback, called during device scan.
*   \param szInterfaceName  Name of the currently scanned interface
*   \param szDeviceName     Name of the detected device
*   \param ulDevIdx         Device index
*   \param usDataType       Data type (Marshaller (0x200), rcX Packet (0x100))
*   \param pvDevTLPrivData  TL device data
*   \param pvUser           User parameter
*   \return CIFX_NO_ERROR on success, CIFX_NO_MORE_ENTRIES when ulBoard is unknown */
/******************************************************************************/
void DeviceDiscoverCB( char* szInterfaceName, char* szDeviceName, uint32_t ulDevIdx, uint16_t usDataType, void* pvDevTLPrivData, void* pvUser)
{
  uint32_t           lRet;
  BROWSE_DEV_CB_T*   ptBrowswCB = (BROWSE_DEV_CB_T*)pvUser;
  BOARD_INFORMATION* ptBoardInfo = NULL;
  BOARD_INFORMATION  tBoardInfo;

  OS_Memset(&tBoardInfo, 0, sizeof(tBoardInfo));

  if (szInterfaceName != NULL)
  {
    if (s_tNXDrv_Info.szLastInterface == NULL)
    {
      s_tNXDrv_Info.szLastInterface = OS_Memrealloc( s_tNXDrv_Info.szLastInterface, (lRet = OS_Strlen( szInterfaceName)+1));
      OS_Strncpy( s_tNXDrv_Info.szLastInterface, OS_Strlen( szInterfaceName)+1, szInterfaceName, OS_Strlen( szInterfaceName));
      s_tNXDrv_Info.ulInterfaceCount++;
    } else
    {
      if (OS_Strcmp( s_tNXDrv_Info.szLastInterface, szInterfaceName) != 0)
      {
        s_tNXDrv_Info.ulInterfaceCount++;
        s_tNXDrv_Info.szLastInterface = OS_Memrealloc( s_tNXDrv_Info.szLastInterface, OS_Strlen( szInterfaceName)+1);
        OS_Strncpy( s_tNXDrv_Info.szLastInterface, OS_Strlen( szInterfaceName)+1, szInterfaceName, OS_Strlen( szInterfaceName));
      }
    }
  }
  /* get board information from TLDevData */
  /* it is a rcX Packet TL info structure */
  if (usDataType == 0x100)
  {
    PRCX_PACKET_TL_INFO_T ptTLDevInfo = (PRCX_PACKET_TL_INFO_T)pvDevTLPrivData;

    OS_Strncpy( tBoardInfo.abBoardName, sizeof(tBoardInfo.abBoardName), szDeviceName, OS_Strlen(szDeviceName));
    OS_Memcpy( &tBoardInfo.tSystemInfo, &ptTLDevInfo->tDevice.tSystemInfo, sizeof(ptTLDevInfo->tDevice.tSystemInfo));
    tBoardInfo.ulChannelCnt = ptTLDevInfo->tDevice.ulChannelCount;
    ptBoardInfo = &tBoardInfo;

  /* it is a Marshaller TL info structure */
  } else if (usDataType == 0x200)
  {
    MARSHALLER_ENDPOINT_DATA_T* ptTLDevInfo = (MARSHALLER_ENDPOINT_DATA_T*)pvDevTLPrivData;
    ptBoardInfo = &ptTLDevInfo->tBoardInfo.tBoardInfo;
  }
  /* if we got a valid board information structure, call the users callback */
  if (ptBoardInfo != NULL)
  {
    NXDRV_DEVICE_T* pvNxDrvDev = OS_Memalloc(sizeof(NXDRV_DEVICE_T));

    pvNxDrvDev->tDevInfo.hDevice = pvNxDrvDev;
    OS_Strncpy( pvNxDrvDev->tDevInfo.szDeviceName, sizeof(pvNxDrvDev->tDevInfo.szDeviceName), szDeviceName, OS_Strlen( szDeviceName));
    OS_Memcpy( &pvNxDrvDev->tDevInfo.tSystemInfoBlock, &ptBoardInfo->tSystemInfo, sizeof(ptBoardInfo->tSystemInfo));

    if ((ptBrowswCB != NULL) && (ptBrowswCB->pfnCallback != NULL))
    {
      ptBrowswCB->pfnCallback( ulDevIdx, &pvNxDrvDev->tBoardInfo, s_tNXDrv_Info.ulInterfaceCount, s_tNXDrv_Info.ulInterfaceCount, ptBrowswCB->pvUser, FALSE, NXAPI_NO_ERROR);
    }
    TAILQ_INSERT_TAIL( &s_tNXDrv_Info.tDeviceList, pvNxDrvDev, tList);
  }
}

/*! **************************************************************************
* Enumerate over all available Boards
*   \param pfnCallback  Progress callback
*   \param pvUser       User pointer for progress callback
*   \return CIFX_NO_ERROR on success, CIFX_NO_MORE_ENTRIES when ulBoard is unknown */
/******************************************************************************/
int32_t APIENTRY nxDrvBrowseDevices ( PFN_NXAPI_BROWSE_CALLBACK pfnCallback,
                                      void*                     pvUser)
{
  int32_t         lRet      = CIFX_NO_ERROR;
  BROWSE_DEV_CB_T tBrowswCB = {0};

  tBrowswCB.pfnCallback = pfnCallback;
  tBrowswCB.pvUser      = pvUser;

  /* stop netXTransport interface to re-enumerate all devices */
  netXTransportStop();

  /* start netXTransport Toolkit (starts enumeration process) */
  if (NXT_NO_ERROR == netXTransportStart( DeviceDiscoverCB, &tBrowswCB))
  {
    lRet = NXAPI_NO_ERROR;
  }

  return lRet;
}

/*! **************************************************************************
* Download files
*   \param  hDevice       Handle to a specific device
*   \param  ulChannel     Number of specific channel, where the file should be downloaded
*   \param  ulCmd         Command for the download function (How to handle download)
*   \param  ulFileSize    Size of file
*   \param  pszFileName   Name of file which should be downloaded
*   \param  pabFileData   Pointer reference to file data
*   \param  pvUser        User pointer
*   \param  pfnCallback   Callback function pointer, for status information
*   \return NAXPI_NO_ERROR on success                                         */
/******************************************************************************/
int32_t APIENTRY nxDrvDownload ( CIFXHANDLE      hDevice,      /* This is the board         */
                                 uint32_t        ulChannel,
                                 uint32_t        ulCmd,        /* Firmware/Config/Warmstart */
                                 uint32_t        ulFileSize,
                                 char*           pszFileName,
                                 uint8_t*        pabFileData,
                                 void*           pvUser,
                                 PFN_NXAPI_PROGRESS_CALLBACK pfnCallback)
{
  int32_t         lRet     = NXAPI_NO_ERROR;
  NXDRV_DEVICE_T* ptDevice = NULL;
  int             fFound   = 0;

  /* Check input values */
  if( (NULL == hDevice)                         ||
      (CIFX_MAX_NUMBER_OF_CHANNELS < ulChannel) ||                /* check numerb of channel    */
      (0 == ulFileSize)                         ||                /* check size of file         */
      (NULL == pszFileName)                     ||                /* check pointer of filename  */
      (NULL == pabFileData)                        )              /* check pointer of data      */
    return NXAPI_INVALID_PARAMETER;

  /* Find device handle */
  TAILQ_FOREACH( ptDevice, &s_tNXDrv_Info.tDeviceList, tList)
  {
    if (ptDevice->tDevInfo.hDevice == hDevice)
    {
      fFound = 1;
      break;
    }
  }
  if (fFound == 0)
  {
    return NXAPI_DRIVER_NO_DEVICE_FOUND;
  } else
  {
    CIFXHANDLE hDriver        = 0;                        /* Driver handle        */
    int32_t    lRet           = NXAPI_NO_ERROR;           /* return errorcode     */
    uint32_t   ulDownloadMode = 0;                        /* new download command */
    CIFXHANDLE hChannel       = NULL;                     /* handle of channel    */

    lRet = xDriverOpen( &hDriver);
    if( CIFX_NO_ERROR != lRet)
    {
      lRet = NXAPI_DRIVER_NOT_INITIALIZED;
    }else
    {
      /* Check type of download */
      switch(ulCmd)
      {
        case NXAPI_CMD_FIRMWARE:
        {
          /* Download firmware */
          ulDownloadMode = DOWNLOAD_MODE_FIRMWARE;
        }
        break;

        case NXAPI_CMD_CONFIGURATION:
        {
          /* Download configuration */
          ulDownloadMode = DOWNLOAD_MODE_CONFIG;
        }
        break;

        case NXAPI_CMD_WARMSTART:
        case NXAPI_CMD_BOOTLOADER:
        default:
        {
          /* Set unknown command */
          lRet = NXAPI_UNKOWN_COMMAND;
        }
        break;
      }

      if(NXAPI_NO_ERROR == lRet)
      {
        /* Create a file name if full path is passed in pszFileName */
        char szFileName[NXT_MAX_PATH] = {0};
        char szFileExt[NXT_MAX_PATH]  = {0};

        _splitpath_s( pszFileName, NULL, 0, NULL, 0, szFileName, NXT_MAX_PATH, szFileExt, NXT_MAX_PATH);

        OS_Strcat( szFileName, NXT_MAX_PATH, szFileExt);

        /* Check channel */
        if(CIFX_SYSTEM_DEVICE != ulChannel)
        {
          /* xChannelDownload */
          if(CIFX_NO_ERROR == (lRet = xChannelOpen( hDriver, ptDevice->tDevInfo.szDeviceName, ulChannel, &hChannel)))
          {
            /* Download on xChannel */
            lRet =  xChannelDownload( hChannel,            /* Channel                  */
                                      ulDownloadMode,      /* Download Mode            */
                                      szFileName,          /* File name                */
                                      pabFileData,         /* File data                */
                                      ulFileSize,          /* Size                     */
              (PFN_PROGRESS_CALLBACK) pfnCallback,         /* Driver download callback */
                                      NULL,                /* Receive packet callback  */
                                      pvUser);             /* pv User                  */
            /* Close channel */
            xChannelClose(hChannel);
          }
        } else
        {
          /*  xSysdeviceDownload */
          CIFXHANDLE hSysDevice = NULL;
          if(CIFX_NO_ERROR == (lRet = xSysdeviceOpen( hDriver, ptDevice->tDevInfo.szDeviceName, &hSysDevice)))
          {
            /*Download on xSysdevice */
            lRet = xSysdeviceDownload(hSysDevice,
                                      ulChannel,
                                      ulDownloadMode,
                                      szFileName,
                                      pabFileData,
                                      ulFileSize,
                                      (PFN_PROGRESS_CALLBACK) pfnCallback,
                                      NULL,
                                      pvUser);
            /* Close sysdevice */
            xSysdeviceClose( hSysDevice);
          }
        }
      }

      /* Close the driver */
      xDriverClose( hDriver);
    }
  }
  return lRet;
}

/*! **************************************************************************
* Start a device
*   \param  hDevice       Handle of a specific device
*   \param  ulChannel     Number of channel
*   \return NXAPI_NO_ERROR on success                                         */
/******************************************************************************/
int32_t APIENTRY nxDrvStart (CIFXHANDLE hDevice, uint32_t ulChannel)
{
  NXDRV_DEVICE_T* ptDevice = NULL;
  int             fFound   = 0;

  UNREFERENCED_PARAMETER( ulChannel);

  /* Check device handle (identifer) */
  if(NULL == hDevice)
  {
    return NXAPI_INVALID_PARAMETER;
  } else
  {
    /* search for the requested device */
    TAILQ_FOREACH( ptDevice, &s_tNXDrv_Info.tDeviceList, tList)
    {
      if (ptDevice->tDevInfo.hDevice == hDevice)
      {
        fFound = 1;
        break;
      }
    }
  }
  /* if the device exists, restart device */
  if (fFound == 1)
  {
    return xDriverRestartDevice( NULL, ptDevice->tDevInfo.szDeviceName, NULL);
  } else
  {
    return NXAPI_DRIVER_NO_DEVICE_FOUND;
  }
}

/*! **************************************************************************
* Read the configuration information
*   \param  hDevice           Handle of a specific channnel
*   \param  ulCmd             Command for the config function
*   \param  lChannel          Number of channel
*   \param  ulCFGSearchIndex  Search index
*   \param  ulBufferSize      Size of buffer
*   \param  ptCfgData         Pointer for config data
*   \return NXAPI_NO_ERROR on success                                         */
/******************************************************************************/
int32_t APIENTRY nxDrvGetConfigInfo ( CIFXHANDLE                  hDevice,
                                      uint32_t                    ulCmd,
                                      int32_t                     lChannel,
                                      uint32_t                    ulCFGSearchIndex,
                                      uint32_t                    ulBufferSize,
                                      NXDRV_DRIVER_CFG_DATA_INFO* ptCfgData)
{
  UNREFERENCED_PARAMETER( hDevice);
  UNREFERENCED_PARAMETER( ulCmd);
  UNREFERENCED_PARAMETER( lChannel);
  UNREFERENCED_PARAMETER( ulCFGSearchIndex);
  UNREFERENCED_PARAMETER( ulBufferSize);
  UNREFERENCED_PARAMETER( ptCfgData);

  return NXAPI_UNKOWN_COMMAND;
}

/*! **************************************************************************
* Read a device configuration value
*   \param  hDevice       Handle of specific device
*   \param  ulCmd         Command for the get configuration function
*   \param  lChannel      Number of channel
*   \param  ptCfgData     Reference of config information structure
*   \param  ulBufferSize  Size of data buffer
*   \param  pvData        Reference of data buffer
*   \return NXAPI_NO_ERROR on success                                         */
/******************************************************************************/
int32_t APIENTRY nxDrvGetConfig ( CIFXHANDLE hDevice, uint32_t ulCmd, int32_t lChannel, NXDRV_DRIVER_CFG_DATA_INFO* ptCfgData, uint32_t ulBufferSize, void* pvData)
{
  UNREFERENCED_PARAMETER( hDevice);
  UNREFERENCED_PARAMETER( ulCmd);
  UNREFERENCED_PARAMETER( lChannel);
  UNREFERENCED_PARAMETER( ptCfgData);
  UNREFERENCED_PARAMETER( ulBufferSize);
  UNREFERENCED_PARAMETER( pvData);

  return NXAPI_UNKOWN_COMMAND;
}

/*! **************************************************************************
* Write a device configuration value
*   \param  hDevice       Reference of data buffer
*   \param  ulCmd         Command for the Set configuration function
*   \param  lChannel      Number of channel
*   \param  ptCfgData     Reference of config information structure
*   \param  ulBufferSize  Size of data buffer
*   \param  pvData        Reference of data buffer
*   \return NXAPI_NO_ERROR on success                                         */
/******************************************************************************/
int32_t APIENTRY nxDrvSetConfig (CIFXHANDLE hDevice, uint32_t ulCmd, int32_t lChannel, NXDRV_DRIVER_CFG_DATA_INFO* ptCfgData,  uint32_t ulBufferSize, void* pvData)
{
  UNREFERENCED_PARAMETER( hDevice);
  UNREFERENCED_PARAMETER( ulCmd);
  UNREFERENCED_PARAMETER( lChannel);
  UNREFERENCED_PARAMETER( ptCfgData);
  UNREFERENCED_PARAMETER( ulBufferSize);
  UNREFERENCED_PARAMETER( pvData);

  return NXAPI_UNKOWN_COMMAND;
}

/*! **************************************************************************
* Delete the device configuration
*   \param  hDevice       Reference of data buffer
*   \param  ulCmd         Command for the delete configuration function
*   \param  lChannel      Number of channel
*   \param  ptCfgData     Reference of config information structure
*   \return NXAPI_NO_ERROR on success                                         */
/******************************************************************************/
int32_t APIENTRY nxDrvDeleteConfig (CIFXHANDLE hDevice, uint32_t ulCmd, int32_t lChannel, NXDRV_DRIVER_CFG_DATA_INFO* ptCfgData)
{
  UNREFERENCED_PARAMETER( hDevice);
  UNREFERENCED_PARAMETER( ulCmd);
  UNREFERENCED_PARAMETER( lChannel);
  UNREFERENCED_PARAMETER( ptCfgData);

  return NXAPI_UNKOWN_COMMAND;
}

/*! **************************************************************************
* Enumerate Connectors
*   \param  ulConnectorIdx  Connector index
*   \param  ulSize          Size of the user buffer
*   \param  pvConnectorInfo Returned connector information
*   \return NXAPI_NO_ERROR on success                                         */
/******************************************************************************/
int32_t APIENTRY nxConEnumerate ( uint32_t ulConnectorIdx,
                                  uint32_t ulSize,
                                  void*    pvConnectorInfo)
{
  int32_t  lRet       = NXAPI_BUFFER_TOO_SHORT;

  if(sizeof(NXCON_CONNECTOR_INFO_T) >= ulSize)
  {
    PNXCON_CONNECTOR_INFO_T    ptConnector        = (PNXCON_CONNECTOR_INFO_T) pvConnectorInfo;
    uint32_t                   ulSearchIdx        = 0;
    NETX_CONNECTOR_DLL_INFO_T* ptNetXTransportCon = NULL;

    lRet = NXAPI_NO_ENTRIES;

    TAILQ_FOREACH( ptNetXTransportCon, &s_tNXDrv_Info.tConnectorList, tList)
    {
      if (ulConnectorIdx == ulSearchIdx)
      {
        /* Copy UUID, filename, full file name and identifier */
        strcpy_s( ptConnector->szConnectorUUID, sizeof(ptConnector->szConnectorUUID)/sizeof(ptConnector->szConnectorUUID[0]), ptNetXTransportCon->szConnectorUUID);
        strcpy_s( ptConnector->szFileName, 256, ptNetXTransportCon->szFileName);
        strcpy_s( ptConnector->szFullFileName, 256, ptNetXTransportCon->szFullFileName);
        strcpy_s( ptConnector->szIdentifier, 6, ptNetXTransportCon->szConIdentifier);
        strcpy_s( ptConnector->szDescription, 64, ptNetXTransportCon->szDescription);
        ptConnector->ulConnectorType = ptNetXTransportCon->tType.ulType;

        lRet = NXAPI_NO_ERROR;
        break;
      }
      ulSearchIdx++;
    }
    if (lRet != NXAPI_NO_ERROR)
      lRet = 1;
  }
  return lRet;
}

/*! **************************************************************************
* Get connector specific confguration
*   \param  szUUID          String of connector UUID
*   \param  pulConfigSize   size of supplied buffer, returns actual size of config string
*   \param  pcConfig        Returned configuration string
*   \return NXAPI_NO_ERROR on success                                         */
/******************************************************************************/
int32_t APIENTRY nxConGetConfig ( char*     szUUID,
                                  uint32_t* pulConfigSize,
                                  char*     pcConfig)
{
  int32_t                    lRet               = NXAPI_NO_ERROR;
  NETX_CONNECTOR_DLL_INFO_T* ptNetXTransportCon = NULL;

  /* Check parameter */
  if ((NULL == pulConfigSize) || (NULL == szUUID))
    return NXAPI_INVALID_POINTER;

  TAILQ_FOREACH( ptNetXTransportCon, &s_tNXDrv_Info.tConnectorList, tList)
  {
    if (0 == strcmp( szUUID, ptNetXTransportCon->szConnectorUUID))
    {
      /* Set new configuration */
      if (NULL == pcConfig)
      {
        lRet = ptNetXTransportCon->tNetXTransportConnectorInfo.tFunctions.pfnConGetConfig( eCMD_CONFIG_GETLEN, (void*)pulConfigSize);

      } else
      {
        uint32_t ulSize = 0;
        lRet = ptNetXTransportCon->tNetXTransportConnectorInfo.tFunctions.pfnConGetConfig( eCMD_CONFIG_GETLEN, (void*)&ulSize);
        if (ulSize>*pulConfigSize)
        {
          lRet = NXT_BUFFER_TOO_SHORT;
        } else
        {
          lRet = ptNetXTransportCon->tNetXTransportConnectorInfo.tFunctions.pfnConGetConfig( eCMD_CONFIG_GETSTRING, pcConfig);
        }
        *pulConfigSize = ulSize;
      }
      break;
    }
  }
  return lRet;
}

/*! **************************************************************************
* Get connector specific confguration (dialog)
*   \param  szUUID          String of connector UUID
*   \param  pulConfigSize   size of supplied buffer, returns actual size of config string
*   \param  pcConfig        Returned configuration string
*   \return NXAPI_NO_ERROR on success                                         */
/******************************************************************************/
int32_t APIENTRY nxConGetDialogConfig ( char*     szUUID,
                                        uint32_t* pulConfigSize,
                                        char*     pcConfig)
{
  int32_t                    lRet               = NXAPI_NO_ERROR;
  NETX_CONNECTOR_DLL_INFO_T* ptNetXTransportCon = NULL;

  /* Check parameter */
  if ((NULL == pulConfigSize) || (NULL == szUUID))
    return NXAPI_INVALID_POINTER;

  TAILQ_FOREACH( ptNetXTransportCon, &s_tNXDrv_Info.tConnectorList, tList)
  {
    if (0 == strcmp( szUUID, ptNetXTransportCon->szConnectorUUID))
    {
      /* Set new configuration */
      if (NULL == pcConfig)
      {
        lRet = ptNetXTransportCon->tNetXTransportConnectorInfo.tFunctions.pfnConGetConfig( eCMD_DIALOG_GETLEN, (void*)pulConfigSize);

      } else
      {
        uint32_t ulSize = 0;
        lRet = ptNetXTransportCon->tNetXTransportConnectorInfo.tFunctions.pfnConGetConfig( eCMD_DIALOG_GETLEN, (void*)&ulSize);
        if (ulSize>*pulConfigSize)
        {
          lRet = NXT_BUFFER_TOO_SHORT;
        } else
        {
          lRet = ptNetXTransportCon->tNetXTransportConnectorInfo.tFunctions.pfnConGetConfig( eCMD_DIALOG_GETSTRING, pcConfig);
        }
        *pulConfigSize = ulSize;
      }
      break;
    }
  }
  return lRet;
}

/*! **************************************************************************
* Set connector specific configuration
*   \param  szUUID       UUID of the connector
*   \param  pcConfig     String of configuration
*   \return NXAPI_NO_ERROR on success                                         */
/******************************************************************************/
int32_t APIENTRY nxConSetConfig ( char* szUUID, char* pcConfig)
{
  int32_t                    lRet               = NXAPI_NO_ENTRIES;
  NETX_CONNECTOR_DLL_INFO_T* ptNetXTransportCon = NULL;

  if( (NULL == szUUID)  ||
      (NULL == pcConfig) )
    return CIFX_INVALID_POINTER;

  TAILQ_FOREACH( ptNetXTransportCon, &s_tNXDrv_Info.tConnectorList, tList)
  {
    if(0 == strcmp(szUUID, ptNetXTransportCon->szConnectorUUID))
    {
      /* Set new configuration */
      lRet = ptNetXTransportCon->tNetXTransportConnectorInfo.tFunctions.pfnConSetConfig( eCMD_CONFIG_SETSTRING, pcConfig);
      break;
    }
  }
  return lRet;
}

/*! **************************************************************************
* Get corresponding name information
*   \param  pszSourceName    Name of a device / interface
*   \param  ulCmd            NXCON_GET_FULL_NAME or NXCON_GET_SHORT_NAME
*   \param  ulCorrespondSize Size of buffer referenced by szCorrespondName
*   \param  szCorrespondName Corresponding name of the device, depends on given command
*   \return NAXPI_NO_ERROR on success                                         */
/******************************************************************************/
int32_t APIENTRY nxConGetCorrespondName ( char* pszSourceName, uint32_t ulCmd, uint32_t ulCorrespondSize, char* szCorrespondName)
{
  int32_t                      lRet               = NXAPI_NO_ERROR;
  INTERFACE_NAME_TRANSLATION_T tTranslate         = {0};
  int                          fFound             = 0;
  uint32_t                     ulOffs             = 0;
  NETX_CONNECTOR_DLL_INFO_T*   ptNetXTransportCon = NULL;
  char                         szInterfaceName[16];

  if((NULL == pszSourceName)   ||
    (NULL == szCorrespondName) )
    return NXAPI_INVALID_POINTER;

  ulOffs = OS_Strcspn( pszSourceName, "_");

  tTranslate.szSource          = pszSourceName;
  tTranslate.szDestination     = szCorrespondName;
  tTranslate.ulDestinationSize = ulCorrespondSize;
  /* get interface name */
  OS_Strncpy( szInterfaceName, 16, pszSourceName, ulOffs);

  switch(ulCmd)
  {
    case NXCON_GET_FULL_NAME: /* Get the device */
    {
      TAILQ_FOREACH( ptNetXTransportCon, &s_tNXDrv_Info.tConnectorList, tList)
      {
        if (OS_Strcmp( ptNetXTransportCon->szConIdentifier, szInterfaceName) == 0)
        {
          ptNetXTransportCon->tNetXTransportConnectorInfo.tFunctions.pfnConGetInformation( eLONG_INTERFACE_NAME, sizeof(tTranslate), &tTranslate);
          fFound = 1;
          break;
        }
      }
    }
    break;
    case NXCON_GET_SHORT_NAME: /* Get short device name from human readable name */
    {
      TAILQ_FOREACH( ptNetXTransportCon, &s_tNXDrv_Info.tConnectorList, tList)
      {
        if (OS_Strcmp( ptNetXTransportCon->szConIdentifier, szInterfaceName) == 0)
        {
          ptNetXTransportCon->tNetXTransportConnectorInfo.tFunctions.pfnConGetInformation( eSHORT_INTERFACE_NAME, sizeof(tTranslate), &tTranslate);
          fFound = 1;
          break;
        }
      }
    }
    break;

    default:
    {
      lRet = NXAPI_DRIVER_INVALID_COMMAND;
    }
    break;
  }

  /* Check if the device connection was not found */
  if(!fFound)
  {
    lRet = NXAPI_NO_ENTRY_FOUND;
  }
  return lRet;
}

/*! **************************************************************************
* Create the configuration dialog of the connector
*   \param  szUUID      UUID of the connector
*   \param  pvParentWnd Parent window
*   \param  pvDialogWnd Window handle of created dialog
*   \return NAXPI_NO_ERROR on success                                         */
/******************************************************************************/
int32_t APIENTRY nxConCreateConfigDialog( char* szUUID, void* pvParentWnd, void** pvDialogWnd)
{
  int32_t                    lRet               = NXT_NO_ERROR;
  uint32_t                   ulSize             = 0;
  NETX_CONNECTOR_DLL_INFO_T* ptNetXTransportCon = NULL;

  *pvDialogWnd = NULL;

  if (0 == (lRet = nxConGetConfig( szUUID, &ulSize, NULL)))
  {
    char* szConfig = OS_Memalloc( (ulSize+1));
    ulSize = ulSize+1;
    if (szConfig != NULL)
    {
      if (0 == (lRet = nxConGetConfig( szUUID, &ulSize, szConfig)))
      {
        TAILQ_FOREACH( ptNetXTransportCon, &s_tNXDrv_Info.tConnectorList, tList)
        {
          if(0 == strcmp(szUUID, ptNetXTransportCon->szConnectorUUID))
          {
            /* Create dialog */
            *pvDialogWnd = ptNetXTransportCon->tNetXTransportConnectorInfo.tFunctions.pfnConCreateDialog( pvParentWnd, szConfig);
            break;
          }
        }
      }
      if (*pvDialogWnd == NULL)
        OS_Memfree( szConfig);
    }
  }
  if (*pvDialogWnd != NULL)
  {
    lRet = NXAPI_NO_ERROR;
  }
  return lRet;
}

/*! **************************************************************************
* Close the configuration dialog of the connector
*   \param  szUUID       UUID of the connector
*   \param  fSaveChanges Store new configuration
*   \return NAXPI_NO_ERROR on success                                        */
/*****************************************************************************/
int32_t APIENTRY nxConCloseConfigDialog( char* szUUID, int32_t fSaveChanges)
{
  int32_t                    lRet               = NXAPI_NO_ERROR;
  uint32_t                   ulSize             = 0;
  NETX_CONNECTOR_DLL_INFO_T* ptNetXTransportCon = NULL;

  if (fSaveChanges)
  {
    if (0 == (lRet = nxConGetConfig ( szUUID, &ulSize, NULL)))
    {
      char* szConfig = (char*)malloc( ulSize+1);
      ulSize = ulSize+1;
      if (szConfig != NULL)
      {
        if (0 == (lRet = nxConGetDialogConfig( szUUID, &ulSize, szConfig)))
        {
          lRet = nxConSetConfig( szUUID, szConfig);
        }
      }
    }
  }
  TAILQ_FOREACH( ptNetXTransportCon, &s_tNXDrv_Info.tConnectorList, tList)
  {
    if(0 == strcmp(szUUID, ptNetXTransportCon->szConnectorUUID))
    {
      ptNetXTransportCon->tNetXTransportConnectorInfo.tFunctions.pfnConEndDialog();
      break;
    }
  }
  return lRet;
}

/*****************************************************************************/
/*! Get plugin directory containing the connector libraries
*   \param  ulSize       Size of buffer for plugin path string
*   \param  szPluginPath String buffer to return plugin path
*   \return NXAPI_NO_ERROR on success                                        */
/*****************************************************************************/
static int32_t GetPluginPath( uint32_t ulSize, char* szPluginPath )
{
  int32_t lRet                       = NXAPI_NO_ERROR;
  TCHAR   szModulePath[NXT_MAX_PATH] = {0};

  /* Read actual path by getting the directory of this dll */
  if ( 0 == GetModuleFileName( g_hModule, szModulePath, sizeof(szModulePath)))
  {
    lRet = NXAPI_NO_WORKING_DIRECTORY;

  } else
  {
    /* We have a valid full file name, extract the path from it */
    char szDrive[_MAX_DRIVE]   = {0};
    char szDirectory[_MAX_DIR] = {0};
    char szPath[_MAX_DIR] = {0};

    _splitpath_s( szPath, szDrive, _MAX_DRIVE, szDirectory, _MAX_DIR, NULL, 0, NULL, 0);

    memset( szPluginPath, 0, ulSize);
    strcat_s( szPluginPath, ulSize, szDrive);
    strcat_s( szPluginPath, ulSize, szDirectory);
    strcat_s( szPluginPath, ulSize, NXCON_DLL_PLUGIN_DIR);
    strcat_s( szPluginPath, ulSize, "\\");
  }

  return lRet;
}

/*****************************************************************************/
/*! Search the available driver DLLs
*   \param  szPluginPath    Plugin path to search for connector libraries
*   \param  pvConnectorList List to return found connector libraries
*   \return  NXAPI_NO_ERROR on success                                       */
/*****************************************************************************/
static int32_t SearchConnectorDlls( char *szPluginPath)
{
  int32_t               lRet               = NXAPI_NO_ERROR;
  char                  szSearchFile[1024] = {0};
  intptr_t              hFile              = 0;
  struct   _finddata_t tFile;
  int                   fConnectorFound    = 0;

  /* Search interface DLLs and load driver */
  if ( 0 == strlen(szPluginPath))
    return NXAPI_NO_WORKING_DIRECTORY;

  strcpy_s(szSearchFile, 1024, szPluginPath);
  strcat_s(szSearchFile, 1024, NXCON_DLL_SEARCH_STRING);

  /* Create search string with path information */
  if( (hFile = _findfirst( szSearchFile, &tFile)) != -1L )
  {
    /* DLL found */
    do
    {
      if ( !(tFile.attrib & _A_SUBDIR) )
      {
        /* Connector DLL found                                 */
        /* Check if this is a connector DLL and load interface */
        NETX_CONNECTOR_DLL_INFO_T* ptConnectorInfo = (NETX_CONNECTOR_DLL_INFO_T*)malloc(sizeof(NETX_CONNECTOR_DLL_INFO_T));
        
		if (ptConnectorInfo == NULL) { return NXAPI_NO_ENTRY_FOUND; }
		memset( ptConnectorInfo, 0, sizeof(*ptConnectorInfo));

        /* Store information into info structure */
        strcat_s( ptConnectorInfo->szFileName, NXCON_FILE_NAME_LENGTH, tFile.name);
        strcat_s( ptConnectorInfo->szFullFileName, NXCON_FILE_NAME_LENGTH, szPluginPath);
        strcat_s( ptConnectorInfo->szFullFileName, NXCON_FILE_NAME_LENGTH, tFile.name);

        /* Try to load the connector */
        if (NXAPI_NO_ERROR == Connector_Load( ptConnectorInfo))
        {
          /* We have a new connector, save to connector list */
          TAILQ_INSERT_TAIL(&s_tNXDrv_Info.tConnectorList, ptConnectorInfo, tList);
          fConnectorFound = 1;
        } else
        {
          free(ptConnectorInfo);
          ptConnectorInfo = NULL;
        }
      }
    } while( _findnext( hFile, &tFile) == 0 );

    _findclose( hFile);
  }
  if ((lRet == NXAPI_NO_ERROR) && (fConnectorFound == 0))
    lRet = NXAPI_NO_ENTRY_FOUND;

  return lRet;
}

/*****************************************************************************/
/*! Convert UUID from UUID struct to string
*   \param  tUUID           UUID structur
*   \param  szConnectorUUID Reference of UUID string
*   \return   NXAPI_NO_ERROR on success                                      */
/*****************************************************************************/
void HandelUUIDtoString( UUID tUUID, char* szConnectorUUID, uint32_t ulSize)
{
  char szUUIDString[256];

  sprintf_s( szUUIDString, 256, "%X-%X-%X-%X%X-%X%X%X%X%X%X", tUUID.Data1,tUUID.Data2,
                                                        tUUID.Data3,tUUID.Data4[0],
                                                        tUUID.Data4[1],tUUID.Data4[2],
                                                        tUUID.Data4[3],tUUID.Data4[4],
                                                        tUUID.Data4[5],tUUID.Data4[6],
                                                        tUUID.Data4[7]);

  strncpy_s( szConnectorUUID, ulSize, szUUIDString, ulSize);
}

/*****************************************************************************/
/*! Registers connector
*   \param  ptConnectorInfo  Pointer to connector information to register
*   \return  NXAPI_NO_ERROR on success                                       */
/*****************************************************************************/
int32_t Connector_Load( NETX_CONNECTOR_DLL_INFO_T* ptConnectorInfo)
{
  int32_t                        lRet = NXT_NO_ERROR;
  NETX_CONNECTOR_FUNCTION_TABLE* ptConFunc = &ptConnectorInfo->tNetXTransportConnectorInfo.tFunctions;

  /* try to load the connector */
  if ( NULL == (ptConnectorInfo->hDll= LoadLibrary( ptConnectorInfo->szFullFileName)))
  {
    /* Unable to load connector DLL */
    lRet = -1;

  } else
  {
    /* Read the functions from the DLL */
    if ( ((ptConFunc->pfnConGetIdentifier      = ((PFN_NETXCON_GETIDENTIFIER)        GetProcAddress( ptConnectorInfo->hDll, "netXConGetIdentifier")))             == NULL) ||
         ((ptConFunc->pfnConOpen               = ((PFN_NETXCON_OPEN)                 GetProcAddress( ptConnectorInfo->hDll, "netXConOpen")))                      == NULL) ||
         ((ptConFunc->pfnConClose              = ((PFN_NETXCON_CLOSE)                GetProcAddress( ptConnectorInfo->hDll, "netXConClose")))                     == NULL) ||
         ((ptConFunc->pfnConCreateInterface    = ((PFN_NETXCON_CREATEINTERFACE)      GetProcAddress( ptConnectorInfo->hDll, "netXConCreateInterface")))           == NULL) ||
         ((ptConFunc->pfnConGetInformation     = ((PFN_NETXCON_GETINFORMATION)       GetProcAddress( ptConnectorInfo->hDll, "netXConGetInformation")))            == NULL) ||
         ((ptConFunc->pfnConGetConfig          = ((PFN_NETXCON_GETCONFIG)            GetProcAddress( ptConnectorInfo->hDll, "netXConGetConfig")))                 == NULL) ||
         ((ptConFunc->pfnConSetConfig          = ((PFN_NETXCON_SETCONFIG)            GetProcAddress( ptConnectorInfo->hDll, "netXConSetConfig")))                 == NULL) ||
         ((ptConFunc->pfnConIntfStart          = ((PFN_NETXCON_INTF_START)           GetProcAddress( ptConnectorInfo->hDll, "netXConStartInterface")))            == NULL) ||
         ((ptConFunc->pfnConIntfStop           = ((PFN_NETXCON_INTF_STOP)            GetProcAddress( ptConnectorInfo->hDll, "netXConStopInterface")))             == NULL) ||
         ((ptConFunc->pfnConIntfSend           = ((PFN_NETXCON_INTF_SEND)            GetProcAddress( ptConnectorInfo->hDll, "netXConSendInterface")))             == NULL) ||
         ((ptConFunc->pfnConIntfGetInformation = ((PFN_NETXCON_INTF_GETINFORMATION)  GetProcAddress( ptConnectorInfo->hDll, "netXConGetInformationInterface")))   == NULL) ||
         ((ptConFunc->pfnConCreateDialog       = ((PFN_NETXCON_CREATEDIALOG)         GetProcAddress( ptConnectorInfo->hDll, "netXConCreateDialog")))              == NULL) ||
         ((ptConFunc->pfnConEndDialog          = ((PFN_NETXCON_ENDDIALOG)            GetProcAddress( ptConnectorInfo->hDll, "netXConEndDialog")))                 == NULL)  )
    {
      lRet = -1;
    } else
    {
      /* setup remaining conector information */
      char  abIdentifier[NXCON_MAX_LENGTH_CONNECTOR_IDENTIFIER] = {0};
      UUID                  tUUID = {0};

      /* Check supported identifier of connector dll */
      if ( (0 != (lRet = ptConFunc->pfnConGetInformation( eIDENTIFIER, sizeof(abIdentifier), abIdentifier))) ||
           (0 != (lRet = ptConFunc->pfnConGetInformation( eUUID, sizeof(tUUID), &tUUID))) )
      {
        ptConnectorInfo->ulError = (uint32_t)CIFX_CONNECTOR_IDENTIFIER_EMPTY;
      } else
      {
        /* Check length of identifier */
        if (strlen( abIdentifier) == 0)
        {
          ptConnectorInfo->ulError = (uint32_t)CIFX_CONNECTOR_IDENTIFIER_EMPTY;
        } else
        {
          if( CIFX_NO_ERROR == ptConnectorInfo->ulError)
          {
            /* Save connector identifier and UUID */
            strncpy_s( ptConnectorInfo->szConIdentifier, sizeof(ptConnectorInfo->szConIdentifier), abIdentifier, strlen(abIdentifier));
            HandelUUIDtoString(tUUID, ptConnectorInfo->szConnectorUUID, sizeof(ptConnectorInfo->szConnectorUUID));
            /* Query and store description and connector type */
            ptConFunc->pfnConGetInformation( eTYPE, sizeof(ptConnectorInfo->tType), &ptConnectorInfo->tType);
            if (0 != (lRet = ptConFunc->pfnConGetInformation( eDESCRIPTION, sizeof(ptConnectorInfo->szDescription), ptConnectorInfo->szDescription)))
            {
              strcpy_s( ptConnectorInfo->szDescription, sizeof(ptConnectorInfo->szDescription), ptConnectorInfo->szFileName);
            }
            lRet = netXTransportAddConnector( &ptConnectorInfo->tNetXTransportConnectorInfo);
          }
        }
      }
    }
  }
  return lRet;
}

/*****************************************************************************/
/*! Initializes the cifX Marshaller Translation-Layer and the rcX-Packet
*   Translation-Layer.
*   \param pvParam    User param (currently not used)
*   \return NXT_NO_ERROR on success                                          */
/*****************************************************************************/
static int32_t TLLayerInit( void* pvParam)
{
  int32_t lRet = NXT_NO_ERROR;

  if (NXT_NO_ERROR == (lRet = cifX_Marshaller_Init( pvParam)))
  {
    lRet = rcXPacket_Init( pvParam);
  }
  return lRet;
}

/*****************************************************************************/
/*! De-initializes translation layer
*   \param pvParam    User param (currently not used)                        */
/*****************************************************************************/
static void TLLayerDeInit( void* pvParam)
{
  cifX_Marshaller_DeInit( pvParam);
  rcXPacket_DeInit( pvParam);
}

/*****************************************************************************/
/*! Starts the cyclic thread of the netXTransport Toolkit
*   (required for periodic jobs).
*   \param lpParameter  User param (currently not used)
*   \return 0 on success                                                     */
/*****************************************************************************/
static DWORD WINAPI CyclicThread( LPVOID lpParameter)
{
  UNREFERENCED_PARAMETER( lpParameter);

  s_fRunCyclicThread = 1;

  while( s_fRunCyclicThread == 1)
  {
    OS_Sleep(100);

    netXTransportCyclicFunction();
  }
  return 0;
}
