/**************************************************************************************

   Copyright (c) Hilscher GmbH. All Rights Reserved.

 **************************************************************************************

   Filename:
    $Id: cifxapi.c 169 2013-06-17 12:00:59Z sebastiand $
   Last Modification:
    $Author: sebastiand $
    $Date: 2013-06-17 14:00:59 +0200 (Mo, 17. Jun 2013) $
    $Revision: 169 $

   Targets:
     OS-Independent : yes

   Description:
    Implementation of the cifX Driver API

   Changes:

     Version   Date        Author   Description
     ----------------------------------------------------------------------------------
      1        23.02.13    SD       initial version

**************************************************************************************/

/****************************************************************************/
/*! \file cifXAPI.c
    Implementation of cifX Driver API                                       */
/****************************************************************************/

#include "netXTransport.h"
#include "HilTransportLayer.h"
#include "rcX_User.h"
#include "cifXUser.h"
#include "cifXErrors.h"
#include "netxapi.h"
#include "cifXAPI.h"
#include "OS_Dependent.h"
#include "OS_Includes.h"
#include <sys/queue.h>

/*****************************************************************************/
/*! cifX API Macro definitions                                               */
/*****************************************************************************/
#define CHECK_SYS_HANDLE(handle) if (CheckHandle( handle, 1)!= CIFX_NO_ERROR) return CheckHandle( handle, 1);     /*!< Verifies given handle (CheckHandle()) */
#define CHECK_CHANNEL_HANDLE(handle) if (CheckHandle( handle, 0)!= CIFX_NO_ERROR) return CheckHandle( handle, 0); /*!< Verifies given handle (CheckHandle()) */

#define ATTACH_DEVICE( handle) if (netXTransportAttachDevice( handle)!= 1) return CIFX_INVALID_HANDLE; /*!< - */
#define DETACH_DEVICE( handle) (netXTransportDetachDevice( handle))                                    /*!< - */

/*****************************************************************************/
/*! cifX API Global variables                                                */
/*****************************************************************************/
CIFX_DRIVER_INFO_T*       g_ptCifXDrvInfo;                                                       /*!< Global cifX driver information          */
static uint16_t           s_ausSupportedTranslationLayers[] = { HIL_TRANSPORT_TYPE_RCX_PACKET,
                                                                HIL_TRANSPORT_TYPE_MARSHALLER,
                                                                HIL_TRANSPORT_TYPE_KEEP_ALIVE};  /*!< Supported data types                    */
void*                     s_pvDriverLock                    = NULL;                              /*!< Sync lock for global driver information */

/*****************************************************************************/
/*! Errorcode to Errordescription lookup table (english only)                */
/*****************************************************************************/
static struct CIFX_ERROR_TO_DESCRtag
{
  int32_t  lError;
  char* szErrorDescr;

} s_atErrorToDescrTable[] =
{
  /*******************************************************************************
  * cifX Device Driver Errors (Global)
  *******************************************************************************/
  {CIFX_INVALID_POINTER              ,"Invalid pointer (e.g. NULL) passed to driver"  },
  {CIFX_INVALID_BOARD                ,"No board with the given name / index available"},
  {CIFX_INVALID_CHANNEL              ,"No channel with the given index available"     },
  {CIFX_INVALID_HANDLE               ,"Invalid handle passed to driver"               },
  {CIFX_INVALID_PARAMETER            ,"Invalid parameter"                             },
  {CIFX_INVALID_COMMAND              ,"Invalid command"                               },
  {CIFX_INVALID_BUFFERSIZE           ,"Invalid buffer size"                           },
  {CIFX_INVALID_ACCESS_SIZE          ,"Invalid access size"                           },
  {CIFX_FUNCTION_FAILED              ,"Function failed"                               },
  {CIFX_FILE_OPEN_FAILED             ,"File could not be opened"                      },
  {CIFX_FILE_SIZE_ZERO               ,"File size is zero"                             },
  {CIFX_FILE_LOAD_INSUFF_MEM         ,"Insufficient memory to load file"              },
  {CIFX_FILE_READ_ERROR              ,"Error reading from file"                       },
  {CIFX_FILE_TYPE_INVALID            ,"Invalid file type"                             },
  {CIFX_FILE_NAME_INVALID            ,"Invalid file name"                             },
  {CIFX_FUNCTION_NOT_AVAILABLE       ,"Driver function not available"                 },
  {CIFX_BUFFER_TOO_SHORT             ,"Given buffer is too short"                     },
  {CIFX_MEMORY_MAPPING_FAILED        ,"Failed to map the memory"                      },
  {CIFX_NO_MORE_ENTRIES              ,"No more entries available"                     },
  {CIFX_CALLBACK_MODE_UNKNOWN        ,"Unkown callback handling mode"                 },
  {CIFX_CALLBACK_CREATE_EVENT_FAILED ,"Failed to create callback events"              },
  {CIFX_CALLBACK_CREATE_RECV_BUFFER  ,"Failed to create callback receive buffer"      },
  /*******************************************************************************
  * Generic Driver Errors
  *******************************************************************************/
  {CIFX_DRV_NOT_INITIALIZED          ,"Driver not initialized"                           },
  {CIFX_DRV_INIT_STATE_ERROR         ,"Driver init state error"                          },
  {CIFX_DRV_READ_STATE_ERROR         ,"Driver read state error"                          },
  {CIFX_DRV_CMD_ACTIVE               ,"Command is active on device"                      },
  {CIFX_DRV_DOWNLOAD_FAILED          ,"General error during download"                    },
  {CIFX_DRV_WRONG_DRIVER_VERSION     ,"Wrong driver version"                             },
  {CIFX_DRV_DRIVER_NOT_LOADED        ,"CIFx driver is not running"                       },
  {CIFX_DRV_INIT_ERROR               ,"Failed to initialize the device"                  },
  {CIFX_DRV_CHANNEL_NOT_INITIALIZED  ,"Channel not initialized (xChannelOpen not called)"},
  {CIFX_DRV_IO_CONTROL_FAILED        ,"IOControl call failed"                            },
  {CIFX_DRV_NOT_OPENED               ,"Driver was not opened"                            },
  {CIFX_DRV_DOWNLOAD_STORAGE_UNKNOWN ,"Unknown download storage type (RAM/FLASH based) found"},
  {CIFX_DRV_DOWNLOAD_FW_WRONG_CHANNEL,"Channel number for a firmware download not supported" },
  {CIFX_DRV_DOWNLOAD_MODULE_NO_BASEOS,"Modules are not allowed without a Base OS firmware"   },
  /*******************************************************************************
  * Generic Device Errors
  *******************************************************************************/
  {CIFX_DEV_DPM_ACCESS_ERROR         ,"Dual port memory not accessable (board not found)"},
  {CIFX_DEV_NOT_READY                ,"Device not ready (ready flag failed)"             },
  {CIFX_DEV_NOT_RUNNING              ,"Device not running (running flag failed)"         },
  {CIFX_DEV_WATCHDOG_FAILED          ,"Watchdog test failed"                             },
  {CIFX_DEV_SYSERR                   ,"Error in handshake flags"                         },
  {CIFX_DEV_MAILBOX_FULL             ,"Send mailbox is full"                             },
  {CIFX_DEV_PUT_TIMEOUT              ,"Send packet timeout"                              },
  {CIFX_DEV_GET_TIMEOUT              ,"Receive packet timeout"                           },
  {CIFX_DEV_GET_NO_PACKET            ,"No packet available"                              },
  {CIFX_DEV_RESET_TIMEOUT            ,"Reset command timeout"                            },
  {CIFX_DEV_NO_COM_FLAG              ,"COM-flag not set"                                 },
  {CIFX_DEV_EXCHANGE_FAILED          ,"I/O data exchange failed"                         },
  {CIFX_DEV_EXCHANGE_TIMEOUT         ,"I/O data exchange timeout"                        },
  {CIFX_DEV_COM_MODE_UNKNOWN         ,"Unknown I/O exchange mode "                       },
  {CIFX_DEV_FUNCTION_FAILED          ,"Device function failed "                          },
  {CIFX_DEV_DPMSIZE_MISMATCH         ,"DPM size differs from configuration"              },
  {CIFX_DEV_STATE_MODE_UNKNOWN       ,"Unknown state mode"                               },
  {CIFX_DEV_HW_PORT_IS_USED          ,"Output port already in use"                       },
  {CIFX_DEV_CONFIG_LOCK_TIMEOUT      ,"Configuration locking timeout"                    },
  {CIFX_DEV_CONFIG_UNLOCK_TIMEOUT    ,"Configuration unlocking timeout"                  },
  {CIFX_DEV_HOST_STATE_SET_TIMEOUT   ,"Set HOST state timeout"                           },
  {CIFX_DEV_HOST_STATE_CLEAR_TIMEOUT ,"Clear HOST state timeout"                         },
  {CIFX_DEV_INITIALIZATION_TIMEOUT   ,"Timeout during channel initialization"            },
  {CIFX_DEV_BUS_STATE_ON_TIMEOUT     ,"Set Bus ON timeout"                               },
  {CIFX_DEV_BUS_STATE_OFF_TIMEOUT    ," Set Bus OFF timeout"                             },
  {CIFX_DEV_MODULE_ALREADY_RUNNING   ,"Module already running"                           },
  {CIFX_DEV_MODULE_ALREADY_EXISTS    ,"Module already exists"                            },
  {CIFX_DEV_DMA_INSUFF_BUFFER_COUNT  ,"Number of configured DMA buffers insufficient"    },
  {CIFX_DEV_DMA_BUFFER_TOO_SMALL     ,"DMA buffers size too small (min size 256Byte)"    },
  {CIFX_DEV_DMA_BUFFER_TOO_BIG       ,"DMA buffers size too big (max size 63,75KByte)"   },
  {CIFX_DEV_DMA_BUFFER_NOT_ALIGNED   ,"DMA buffer alignment failed (must be 256Byte)"    },
  {CIFX_DEV_DMA_HANSHAKEMODE_NOT_SUPPORTED ,"I/O data uncontrolled handshake mode not supported"    },
  {CIFX_DEV_DMA_IO_AREA_NOT_SUPPORTED,"I/O area in DMA mode not supported (only area 0 possible)"   },
  {CIFX_DEV_DMA_STATE_ON_TIMEOUT     ,"Set DMA ON timeout"                               },
  {CIFX_DEV_DMA_STATE_OFF_TIMEOUT    ,"Set DMA OFF timeout"                              },

  /*******************************************************************************/
};

/*****************************************************************************/
/*! Initializes cifXAPI TL                                                   */
/*****************************************************************************/
void cifXAPIInit( NXTHANDLE hNetXTransport)
{
  /* store handle to netXTransport instance */
  if (NULL != (g_ptCifXDrvInfo = OS_Memalloc( sizeof(CIFX_DRIVER_INFO_T))))
  {
    OS_Memset( g_ptCifXDrvInfo, 0, sizeof(CIFX_DRIVER_INFO_T));
    g_ptCifXDrvInfo->hNetXTransport = hNetXTransport;

    s_pvDriverLock = OS_CreateLock();
  }
}

/*****************************************************************************/
/*! De-initializes cifXAPI TL                                                */
/*****************************************************************************/
void cifXAPIDeInit( NXTHANDLE hNetXTransport)
{
  UNREFERENCED_PARAMETER( hNetXTransport);

  if (g_ptCifXDrvInfo != NULL)
  {
    g_ptCifXDrvInfo->hNetXTransport = NULL;

    OS_Memfree( g_ptCifXDrvInfo);
    g_ptCifXDrvInfo = NULL;
  }

  if (s_pvDriverLock != NULL)
  {
    OS_DeleteLock( s_pvDriverLock);
    s_pvDriverLock = NULL;
  }
}

/*****************************************************************************/
/*! Checks if the given channel handle is valid
*   \param hChannel      Channel handle
*   \param fSysDevice    set to 1 if it is a system channel
*   \return CIFX_NO_ERROR on success                                         */
/*****************************************************************************/
static int32_t CheckHandle(CIFXHANDLE hChannel, int fSysDevice)
{
  int32_t                       lRet   = CIFX_INVALID_HANDLE;
  CIFX_TRANSPORT_DEVICE_INST_T* ptTemp = NULL;

  if (g_ptCifXDrvInfo->pvDevListLock == NULL)
    return CIFX_DRV_NOT_OPENED;

  OS_EnterLock( g_ptCifXDrvInfo->pvDevListLock);

  if (fSysDevice == 1)
  {
    TAILQ_FOREACH( ptTemp, &g_ptCifXDrvInfo->tSysDeviceList, tList)
    {
      if (ptTemp == hChannel)
      {
        lRet = CIFX_NO_ERROR;
        break;
      }
    }
  } else
  {
    TAILQ_FOREACH( ptTemp, &g_ptCifXDrvInfo->tChannelDeviceList, tList)
    {
      if (ptTemp == hChannel)
      {
        lRet = CIFX_NO_ERROR;
        break;
      }
    }
  }

  OS_LeaveLock( g_ptCifXDrvInfo->pvDevListLock);

  return lRet;
}

/*! **************************************************************************
* Opens the Driver
*   \param phDriver Returned handle to the driver
*   \return CIFX_NO_ERROR on success
******************************************************************************/
int32_t APIENTRY xDriverOpen(CIFXHANDLE* phDriver)
{
  int32_t        lRet                   = CIFX_NO_ERROR;
  uint32_t       ulTLCounter            = 0;
  uint32_t       ulTLInfoCnt            = 0;
  CIFXHANDLE     hNetXTransportDevGroup = NULL;
  PDRV_TL_INFO_T ptDrvInfo              = g_ptCifXDrvInfo->atTLInfo;

  if (phDriver == NULL)
    return lRet;

  if ( s_pvDriverLock == NULL)
    return CIFX_DRV_NOT_INITIALIZED;

  OS_EnterLock( s_pvDriverLock);

  if( g_ptCifXDrvInfo->fInitialized == 0)
  {
    /* start netXTransport Toolkit (starts enumeration process) */
    if (NXT_NO_ERROR != netXTransportStart( NULL, NULL))
      lRet = CIFX_DRV_NOT_INITIALIZED;

    if (lRet == CIFX_NO_ERROR)
    {
      lRet = CIFX_INVALID_POINTER;

      /* 0x0000 marks end of list */
      while (s_ausSupportedTranslationLayers[ulTLCounter] != 0x0000)
      {
        if (NULL != (hNetXTransportDevGroup = netXTransportGetHandleToDeviceGroup( s_ausSupportedTranslationLayers[ulTLCounter])))
        {
          if (ptDrvInfo == NULL)
          {
            ptDrvInfo = OS_Memalloc(sizeof(DRV_TL_INFO_T));
			if (NULL == ptDrvInfo) { return CIFX_INVALID_HANDLE; }

            OS_Memset( ptDrvInfo, 0, sizeof(DRV_TL_INFO_T));

            g_ptCifXDrvInfo->atTLInfo     = ptDrvInfo;
            g_ptCifXDrvInfo->ulTLInfoSize = sizeof(DRV_TL_INFO_T);

          } else
          {
            g_ptCifXDrvInfo->atTLInfo      = OS_Memrealloc( g_ptCifXDrvInfo->atTLInfo, g_ptCifXDrvInfo->ulTLInfoSize + sizeof(DRV_TL_INFO_T));
            ptDrvInfo                   = &g_ptCifXDrvInfo->atTLInfo[++ulTLInfoCnt];
            g_ptCifXDrvInfo->ulTLInfoSize += sizeof(DRV_TL_INFO_T);
          }
		  if (NULL == ptDrvInfo) { return CIFX_INVALID_HANDLE; }
          ptDrvInfo->hNetXTransportDevGroup  = hNetXTransportDevGroup;
          ptDrvInfo->usDataType              = s_ausSupportedTranslationLayers[ulTLCounter];

          /* map function pointer table */
          if (NXT_NO_ERROR == netXTransportGetTLFctTable( hNetXTransportDevGroup, &ptDrvInfo->tTLFctTable))
          {
            lRet = CIFX_FUNCTION_FAILED;

            /* check if version */
            if (ptDrvInfo->tTLFctTable.ulVersionMajor < CIFX_DRV_VERSION_MAJOR)
            {
              lRet = CIFX_FUNCTION_FAILED;
            } else if (ptDrvInfo->tTLFctTable.ulVersionMinor < CIFX_DRV_VERSION_MINOR)
            {
              lRet = CIFX_FUNCTION_NOT_AVAILABLE;
            } else if (ptDrvInfo->tTLFctTable.ulVersionRevision < CIFX_DRV_VERSION_REV)
            {
              lRet = CIFX_FUNCTION_NOT_AVAILABLE;
            } else if (ptDrvInfo->tTLFctTable.ulVersionBuild >= CIFX_DRV_VERSION_BUILD)
            {
              ptDrvInfo->ptCifXFctTable = (CIFX_DRV_FUNCTION_TABLE_T*)ptDrvInfo->tTLFctTable.pvTranslationLayerFunctionTable;
              lRet = CIFX_NO_ERROR;
            }
          }
        }
        ulTLCounter++;
      }
      if (lRet == CIFX_NO_ERROR)
      {
        g_ptCifXDrvInfo->pvDevListLock = OS_CreateLock();
        TAILQ_INIT(&g_ptCifXDrvInfo->tSysDeviceList);
        TAILQ_INIT(&g_ptCifXDrvInfo->tChannelDeviceList);

        g_ptCifXDrvInfo->fInitialized = 1;
      }
    }
  }
  if (lRet == CIFX_NO_ERROR)
  {
    *phDriver = (void*)g_ptCifXDrvInfo;
    g_ptCifXDrvInfo->ulRefCount++;
  }
  OS_LeaveLock( s_pvDriverLock);

  return lRet;
}

/*! **************************************************************************
* Closes a previously opened driver
*   \param hDriver Driver handle
*   \return CIFX_NO_ERROR on success
******************************************************************************/
int32_t APIENTRY xDriverClose(CIFXHANDLE hDriver)
{
  int32_t lRet = NXT_NO_ERROR;

  if ((g_ptCifXDrvInfo == NULL) || (g_ptCifXDrvInfo->fInitialized == 0))
  {
    return CIFX_DRV_NOT_INITIALIZED;
  } else if (g_ptCifXDrvInfo != (CIFX_DRIVER_INFO_T*)hDriver)
  {
    return CIFX_INVALID_HANDLE;
  }

  OS_EnterLock( s_pvDriverLock);
  {
    if (g_ptCifXDrvInfo->fInitialized == 0)
    {
      lRet = CIFX_DRV_NOT_INITIALIZED;
    } else
    {
      if (--g_ptCifXDrvInfo->ulRefCount == 0)
      {
        OS_DeleteLock( g_ptCifXDrvInfo->pvDevListLock);
        g_ptCifXDrvInfo->pvDevListLock = NULL;

        OS_Memfree( g_ptCifXDrvInfo->atTLInfo);
        g_ptCifXDrvInfo->atTLInfo     = NULL;
        g_ptCifXDrvInfo->ulTLInfoSize = 0;

        g_ptCifXDrvInfo->fInitialized = 0;
      }
    }
  }
  OS_LeaveLock( s_pvDriverLock);

  return lRet;
}

/*! **************************************************************************
* Restart a device
*   \param hDriver      Driver handle, if NULL the driver will be temporarily opened by the API
*   \param szBoardName  Board name
*   \param pvData
*   \return CIFX_NO_ERROR on success
******************************************************************************/
int32_t APIENTRY xDriverRestartDevice(CIFXHANDLE hDriver, char* szBoardName, void* pvData)
{
  int32_t lRet         = CIFX_NO_ERROR;
  int     fCloseDriver = 0;

  UNREFERENCED_PARAMETER( pvData);

  if(NULL == hDriver)
  {
    lRet = xDriverOpen(&hDriver);
    fCloseDriver = 1;
  }

  if(CIFX_NO_ERROR == lRet)
  {
    CIFXHANDLE  hSysdevice = NULL;

    /* Open Sysdevice -> connect device, verify device is available */
    if(CIFX_NO_ERROR == (lRet = xSysdeviceOpen( hDriver, szBoardName, &hSysdevice)))
    {
      /* Reset device. Use interface specific reset timeout */
      lRet = xSysdeviceReset( hSysdevice, 10000);
      xSysdeviceClose( hSysdevice);
    }
  }
  if(fCloseDriver)
    xDriverClose( hDriver);

  return lRet;
}

/*! **************************************************************************
* Query information about the running device driver
*   \param hDriver      Driver handle, if NULL the driver will be temporarily opened by the API
*   \param ulSize       Buffer size
*   \param pvDriverInfo Returned Driver information
*   \return CIFX_NO_ERROR on success
******************************************************************************/
int32_t APIENTRY xDriverGetInformation( CIFXHANDLE  hDriver,
                                        uint32_t    ulSize,
                                        void*       pvDriverInfo)
{
  int32_t             lRet               = CIFX_NO_ERROR;
  DRIVER_INFORMATION* ptDriverInfo       = (DRIVER_INFORMATION*)pvDriverInfo;
  uint32_t            ulSupportedTLCount = 0;

  if ((g_ptCifXDrvInfo == NULL) || (g_ptCifXDrvInfo->fInitialized == 0))
  {
    return CIFX_DRV_NOT_INITIALIZED;
  } else if (g_ptCifXDrvInfo != (CIFX_DRIVER_INFO_T*)hDriver)
  {
    return CIFX_INVALID_HANDLE;
  }
  if(ulSize >= sizeof(ptDriverInfo))
  {
    OS_Strvsprintf( ptDriverInfo->abDriverVersion, 32,
                  "%s, V%d.%d.%d.%d",
                  "cifXAPI-netXTransport",
                  CIFX_DRV_VERSION_MAJOR,
                  CIFX_DRV_VERSION_MINOR,
                  CIFX_DRV_VERSION_REV,
                  CIFX_DRV_VERSION_BUILD);

    ptDriverInfo->ulBoardCnt = 0;
    for (ulSupportedTLCount = 0; ulSupportedTLCount<(g_ptCifXDrvInfo->ulTLInfoSize/sizeof(DRV_TL_INFO_T));ulSupportedTLCount++)
    {
      PDRV_TL_INFO_T ptDrvInfo = &g_ptCifXDrvInfo->atTLInfo[ulSupportedTLCount];

      if (ptDrvInfo->hNetXTransportDevGroup)
      {
        ptDriverInfo->ulBoardCnt += netXTransportGetDeviceCount(ptDrvInfo->hNetXTransportDevGroup);
      } else
      {
        break;
      }
    }
  } else
  {
    lRet = CIFX_INVALID_BUFFERSIZE;
  }
  return lRet;
}

/*! **************************************************************************
* Enumerate over all available Boards
*   \param hDriver      Driver handle, if NULL the driver will be temporarily opened by the API
*   \param ulBoard      Should be counted from 0 upwards on enumeration
*   \param ulSize       Size of the user buffer
*   \param pvBoardInfo  Returned board information
*   \return CIFX_NO_ERROR on success, CIFX_NO_MORE_ENTRIES when ulBoard is unknown
******************************************************************************/
int32_t APIENTRY xDriverEnumBoards( CIFXHANDLE  hDriver,
                                    uint32_t    ulBoard,
                                    uint32_t    ulSize,
                                    void*       pvBoardInfo)
{
  int32_t                    lRet                    = CIFX_INVALID_BOARD;
  uint32_t                   ulBoardCount            = 0;
  uint32_t                   ulSupportedTLCount      = 0;
  PDRV_TL_INFO_T             ptDrvInfo               = NULL;

  if ((g_ptCifXDrvInfo == NULL) || (g_ptCifXDrvInfo->fInitialized == 0))
  {
    return CIFX_DRV_NOT_INITIALIZED;
  } else if (g_ptCifXDrvInfo != (CIFX_DRIVER_INFO_T*)hDriver)
  {
    return CIFX_INVALID_HANDLE;
  }

  /* iterate over all registered translation layers */
  for (ulSupportedTLCount = 0; ulSupportedTLCount<(g_ptCifXDrvInfo->ulTLInfoSize/sizeof(DRV_TL_INFO_T));ulSupportedTLCount++)
  {
    ptDrvInfo = &g_ptCifXDrvInfo->atTLInfo[ulSupportedTLCount];

    if (ptDrvInfo->hNetXTransportDevGroup)
    {
      /* check if the requested board is part of the current instance */
      if (ulBoard < (ulBoardCount + netXTransportGetDeviceCount( ptDrvInfo->hNetXTransportDevGroup)))
      {
        lRet                    = CIFX_NO_ERROR;
        ulBoard                -= ulBoardCount;
        break;
      }
      ulBoardCount += netXTransportGetDeviceCount( ptDrvInfo->hNetXTransportDevGroup);
    } else
    {
      break;
    }
  }
  if (CIFX_NO_ERROR == lRet)
  {
    if (0 == netXTransportCheckInterfaceState( ptDrvInfo->hNetXTransportDevGroup))
      return CIFX_INVALID_HANDLE;

    /* check if the function is supported */
    /* the version is already checked in xDriverOpen() */
    if (ptDrvInfo->ptCifXFctTable && ptDrvInfo->ptCifXFctTable->pfnxDriverEnumBoards)
    {
      BOARD_INFORMATION tBoardInfo;
      if (CIFX_NO_ERROR == (lRet = ptDrvInfo->ptCifXFctTable->pfnxDriverEnumBoards( ptDrvInfo->hNetXTransportDevGroup, ulBoard, sizeof(BOARD_INFORMATION), &tBoardInfo)))
      {
        if (ulSize>sizeof(tBoardInfo))
          ulSize = sizeof(tBoardInfo);

        OS_Memcpy( pvBoardInfo, (void*)&tBoardInfo, ulSize);
        lRet = CIFX_NO_ERROR;
      }
    }
  }
  return lRet;
}

/*! **************************************************************************
* Enumerate over all available Channels on a board
*   \param hDriver        Driver handle, if NULL the driver will be temporarily opened by the API
*   \param ulBoard        The board number during board enumeration
*   \param ulChannel      Should be counted from 0 upwards on enumeration
*   \param ulSize         Size of the user buffer
*   \param pvChannelInfo  Returned channel information
*   \return CIFX_NO_ERROR on success, CIFX_NO_MORE_ENTRIES when ulChannel is unknown
******************************************************************************/
int32_t APIENTRY xDriverEnumChannels( CIFXHANDLE  hDriver,
                                      uint32_t    ulBoard,
                                      uint32_t    ulChannel,
                                      uint32_t    ulSize,
                                      void*       pvChannelInfo)
{
  int32_t                    lRet                    = CIFX_NO_ERROR;
  NXTHANDLE                  ptnetXTransportInstance = NULL;
  uint32_t                   ulSupportedTLCount      = 0;
  uint32_t                   ulBoardCount            = 0;
  PDRV_TL_INFO_T             ptDrvInfo               = NULL;

  if ((g_ptCifXDrvInfo == NULL) || (g_ptCifXDrvInfo->fInitialized == 0))
  {
    return CIFX_DRV_NOT_INITIALIZED;
  } else if (g_ptCifXDrvInfo != (CIFX_DRIVER_INFO_T*)hDriver)
  {
    return CIFX_INVALID_HANDLE;
  }

  for (ulSupportedTLCount = 0; ulSupportedTLCount < (g_ptCifXDrvInfo->ulTLInfoSize / sizeof(DRV_TL_INFO_T)); ulSupportedTLCount++)
  {
	ptDrvInfo = &g_ptCifXDrvInfo->atTLInfo[ulSupportedTLCount];
	if (NULL == ptDrvInfo) { return CIFX_INVALID_HANDLE; }
    if (ptDrvInfo->hNetXTransportDevGroup)
    {
      /* enumerate all Marshaller device */
      if (ulBoard < (ulBoardCount + netXTransportGetDeviceCount( ptDrvInfo->hNetXTransportDevGroup)))
      {
        lRet                    = CIFX_NO_ERROR;
        ptnetXTransportInstance = ptDrvInfo->hNetXTransportDevGroup;
        ulBoard                -= ulBoardCount;
        break;
      }
      ulBoardCount += netXTransportGetDeviceCount( ptDrvInfo->hNetXTransportDevGroup);
    } else
    {
      break;
    }
  }
  if (CIFX_NO_ERROR == lRet)
  {
    if (NULL == ptDrvInfo) { return CIFX_INVALID_HANDLE; }
    if (0 == netXTransportCheckInterfaceState( ptDrvInfo->hNetXTransportDevGroup))
    return CIFX_INVALID_HANDLE;

    if (ptDrvInfo->ptCifXFctTable && ptDrvInfo->ptCifXFctTable->pfnxDriverEnumChannels)
    {
      CHANNEL_INFORMATION tChannelInfo;
      if (CIFX_NO_ERROR == (lRet = ptDrvInfo->ptCifXFctTable->pfnxDriverEnumChannels( ptnetXTransportInstance, ulBoard, ulChannel, sizeof(CHANNEL_INFORMATION), (void*)&tChannelInfo)))
      {
        if (ulSize>sizeof(tChannelInfo))
          ulSize = sizeof(tChannelInfo);

        OS_Memcpy( pvChannelInfo, (void*)&tChannelInfo, ulSize);
        lRet = CIFX_NO_ERROR;
      }
    }
  }
  return lRet;
}

/*! **************************************************************************
* Get/Return a memory pointer to the boards dual-port memory
*   \param hDriver       Driver handle, if NULL the driver will be temporarily opened by the API
*   \param ulBoardNumber The board number
*   \param ulCmd         Function command
*   \param pvMemoryInfo  Memory information structure
*   \return CIFX_NO_ERROR on success
******************************************************************************/
int32_t APIENTRY xDriverMemoryPointer(CIFXHANDLE  hDriver,
                                      uint32_t    ulBoardNumber,
                                      uint32_t    ulCmd,
                                      void*       pvMemoryInfo)
{
  int32_t lRet = CIFX_FUNCTION_NOT_AVAILABLE;

  UNREFERENCED_PARAMETER(hDriver);
  UNREFERENCED_PARAMETER(ulBoardNumber);
  UNREFERENCED_PARAMETER(ulCmd);
  UNREFERENCED_PARAMETER(pvMemoryInfo);

  return lRet;
}

/*! **************************************************************************
* Gets the error description in english from error number
*   \param lError         Error number returned by driver function
*   \param szBuffer       Return buffer for error description
*   \param ulBufferLen    Length of the return buffer
*   \return CIFX_NO_ERROR on success
******************************************************************************/
int32_t APIENTRY xDriverGetErrorDescription( int32_t  lError,
                                             char*    szBuffer,
                                             uint32_t ulBufferLen)
{
  int32_t lRet = CIFX_FUNCTION_FAILED;
  int     iIdx = 0;

  if (szBuffer == NULL)
    return CIFX_INVALID_PARAMETER;

  for(iIdx = 0; iIdx < (int)(sizeof(s_atErrorToDescrTable) / sizeof(s_atErrorToDescrTable[0])); ++iIdx)
  {
    if(s_atErrorToDescrTable[iIdx].lError == lError)
    {
      OS_Strncpy(szBuffer, ulBufferLen, s_atErrorToDescrTable[iIdx].szErrorDescr, ulBufferLen);
      lRet = CIFX_NO_ERROR;
      break;
    }
  }

  return lRet;
}

/***************************************************************************
*
*
*   SYSDEVICE FUNCTIONS....................
*
*
*
*
*
******************************************************************************/

/*! **************************************************************************
* Opens the System device on the given board
*   \param hDriver      Driver handle
*   \param szBoard      Name of the board to open
*   \param phSysdevice  Returned handle to the System device area
*   \return CIFX_NO_ERROR on success
******************************************************************************/
int32_t APIENTRY xSysdeviceOpen(CIFXHANDLE  hDriver,
                                char*       szBoard,
                                CIFXHANDLE* phSysdevice)
{
  int32_t                       lRet                  = CIFX_NO_ERROR;
  CIFX_TRANSPORT_DEVICE_INST_T* ptnetXTransportDevice = NULL;
  CIFX_DRV_FUNCTION_TABLE_T*    ptCifXFctTable        = NULL;

  if ((g_ptCifXDrvInfo == NULL) || (g_ptCifXDrvInfo->fInitialized == 0))
  {
    return CIFX_DRV_NOT_INITIALIZED;
  } else if (g_ptCifXDrvInfo != (CIFX_DRIVER_INFO_T*)hDriver)
  {
    return CIFX_INVALID_HANDLE;
  } else if (NULL == (ptnetXTransportDevice = OS_Memalloc(sizeof(CIFX_TRANSPORT_DEVICE_INST_T))))
  {
    return CIFX_FUNCTION_FAILED;
  }

  /* first search for the requested device */
  if (NXT_NO_ERROR != netXTransportGetDeviceHandle( szBoard, &ptnetXTransportDevice->hNetXTransportDeviceHandle))
  {
    lRet = CIFX_INVALID_BOARD;
    /* if the device exists, get the function pointer table of the translations-layer, the device is 'talking' (rcXPacket (0x100) or Marshaller (0x200)) */
  } else if (NXT_NO_ERROR != netXTransportGetTLFctTableByDevice( ptnetXTransportDevice->hNetXTransportDeviceHandle, &ptnetXTransportDevice->tTLFctTable))
  {
    lRet = CIFX_INVALID_BOARD;
  } else if (NULL == ptnetXTransportDevice->tTLFctTable.pvTranslationLayerFunctionTable)
  {
    lRet = CIFX_INVALID_BOARD;
  } else
  {
    ptnetXTransportDevice->ptCifXFctTable = ptnetXTransportDevice->tTLFctTable.pvTranslationLayerFunctionTable;
    ptCifXFctTable                        = ptnetXTransportDevice->tTLFctTable.pvTranslationLayerFunctionTable;
    lRet                                   = CIFX_NO_ERROR;
  }

  if (lRet == CIFX_NO_ERROR)
  {
    lRet = CIFX_FUNCTION_NOT_AVAILABLE;

    /* Before transmitting any data check if the connection is already available (if so, netXTransportAttachDevice() */
    /* will lock the connection resources until we call netXTransportDetachDevice() or an error occurs).             */
    if (0 == netXTransportAttachDevice( ptnetXTransportDevice->hNetXTransportDeviceHandle))
      return CIFX_INVALID_HANDLE;

    if (ptCifXFctTable->pfnxSysdeviceOpen)
    {
      if (CIFX_NO_ERROR == (lRet = ptCifXFctTable->pfnxSysdeviceOpen( (CIFXHANDLE)ptnetXTransportDevice, szBoard, &ptnetXTransportDevice->hSysDevice)))
      {
        *phSysdevice = (CIFXHANDLE)ptnetXTransportDevice;

        /* notify netXTransport Layer to keep this connection active until we close it (if supported Keep-Alive,...) */
        netXTransportActivateConnection( ptnetXTransportDevice->hNetXTransportDeviceHandle);

        /* store handle in list, to be able to verify user input */
        if (g_ptCifXDrvInfo->pvDevListLock)
        {
          OS_EnterLock(g_ptCifXDrvInfo->pvDevListLock);

          TAILQ_INSERT_TAIL(&g_ptCifXDrvInfo->tSysDeviceList, ptnetXTransportDevice, tList);

          OS_LeaveLock(g_ptCifXDrvInfo->pvDevListLock);
        }
      }
    }
    /* Notify netXTransport-Layer that all connection related resources are now free */
    netXTransportDetachDevice( ptnetXTransportDevice->hNetXTransportDeviceHandle);
  }

  if (lRet != CIFX_NO_ERROR)
  {
    if (NULL != ptnetXTransportDevice)
    {
      OS_Memfree(ptnetXTransportDevice);
    }
  }
  return lRet;
}

/*! **************************************************************************
* Closed an open System device
*   \param hSysdevice  Handle to the System device to close
*   \return CIFX_NO_ERROR on success
******************************************************************************/
int32_t APIENTRY xSysdeviceClose(CIFXHANDLE hSysdevice)
{
  int32_t                       lRet               = CIFX_FUNCTION_NOT_AVAILABLE;
  CIFX_TRANSPORT_DEVICE_INST_T* ptcifXTransportDev = (CIFX_TRANSPORT_DEVICE_INST_T*)hSysdevice;

  CHECK_SYS_HANDLE( hSysdevice);

  if (NULL != ptcifXTransportDev)
  {
    lRet = CIFX_INVALID_CHANNEL;

    if (1 == netXTransportAttachDevice( ptcifXTransportDev->hNetXTransportDeviceHandle))
    {
      CIFX_DRV_FUNCTION_TABLE_T* ptCifXFctTable = (CIFX_DRV_FUNCTION_TABLE_T*)ptcifXTransportDev->ptCifXFctTable;

      if (ptCifXFctTable->pfnxSysdeviceClose)
      {
        lRet = ptCifXFctTable->pfnxSysdeviceClose( ptcifXTransportDev->hSysDevice);
      }
      netXTransportDetachDevice( ptcifXTransportDev->hNetXTransportDeviceHandle);

      if (lRet == CIFX_NO_ERROR)
      {
        OS_EnterLock(g_ptCifXDrvInfo->pvDevListLock);

        TAILQ_REMOVE(&g_ptCifXDrvInfo->tSysDeviceList, ptcifXTransportDev, tList);

        OS_LeaveLock(g_ptCifXDrvInfo->pvDevListLock);

        netXTransportDeActivateConnection( ptcifXTransportDev->hNetXTransportDeviceHandle);

        ptcifXTransportDev->hNetXTransportDeviceHandle = NULL;
        ptcifXTransportDev->hSysDevice                 = NULL;
        OS_Memfree(ptcifXTransportDev);
      }
    }
  }

  return lRet;
}

/*! **************************************************************************
* Gets the information of a system device
*   \param hSysdevice   Handle to the system device
*   \param ulCmd        Command
*   \param ulSize       Size of the passed structure
*   \param pvInfo       Pointer to the structure for returned data
*   \return CIFX_NO_ERROR on success
******************************************************************************/
int32_t APIENTRY xSysdeviceInfo(CIFXHANDLE hSysdevice,
                                uint32_t   ulCmd,
                                uint32_t   ulSize,
                                void*      pvInfo)
{
  int32_t                       lRet               = CIFX_FUNCTION_NOT_AVAILABLE;
  CIFX_TRANSPORT_DEVICE_INST_T* ptcifXTransportDev = (CIFX_TRANSPORT_DEVICE_INST_T*)hSysdevice;
  CIFX_DRV_FUNCTION_TABLE_T*    ptCifXFctTable     = (CIFX_DRV_FUNCTION_TABLE_T*)ptcifXTransportDev->ptCifXFctTable;

  CHECK_SYS_HANDLE( hSysdevice);
  ATTACH_DEVICE( ptcifXTransportDev->hNetXTransportDeviceHandle);

  if (ptCifXFctTable->pfnxSysdeviceInfo)
  {
    lRet = ptCifXFctTable->pfnxSysdeviceInfo( ptcifXTransportDev->hSysDevice,
                                                ulCmd,
                                                ulSize,
                                                pvInfo);
  }
  DETACH_DEVICE( ptcifXTransportDev->hNetXTransportDeviceHandle);

  return lRet;
}

/*! **************************************************************************
* Gets the Mailbox state of an open system device
*   \param hSysdevice      Handle to the System device
*   \param pulRecvPktCount Number of packets in receive mailbox
*   \param pulSendPktCount Number of packets the application is able to send at once
*   \return CIFX_NO_ERROR on success
******************************************************************************/
int32_t APIENTRY xSysdeviceGetMBXState(CIFXHANDLE hSysdevice,
                                       uint32_t*  pulRecvPktCount,
                                       uint32_t*  pulSendPktCount)
{
  int32_t                       lRet               = CIFX_FUNCTION_NOT_AVAILABLE;
  CIFX_TRANSPORT_DEVICE_INST_T* ptcifXTransportDev = (CIFX_TRANSPORT_DEVICE_INST_T*)hSysdevice;
  CIFX_DRV_FUNCTION_TABLE_T*    ptCifXFctTable     = (CIFX_DRV_FUNCTION_TABLE_T*)ptcifXTransportDev->ptCifXFctTable;

  CHECK_SYS_HANDLE( hSysdevice);
  ATTACH_DEVICE( ptcifXTransportDev->hNetXTransportDeviceHandle);

  if (ptCifXFctTable->pfnxSysdeviceGetMBXState)
  {
    lRet = ptCifXFctTable->pfnxSysdeviceGetMBXState( ptcifXTransportDev->hSysDevice, pulRecvPktCount, pulSendPktCount);
  }
  DETACH_DEVICE( ptcifXTransportDev->hNetXTransportDeviceHandle);

  return lRet;
}

/*! **************************************************************************
* Inserts a packet into the System Mailbox
*   \param hSysdevice      Handle to the System device
*   \param ptSendPkt       Packet to send to device
*   \param ulTimeout       maximum time to wait for packet to be accepted by device (in ms)
*   \return CIFX_NO_ERROR on success
******************************************************************************/
int32_t APIENTRY xSysdevicePutPacket(CIFXHANDLE    hSysdevice,
                                     CIFX_PACKET*  ptSendPkt,
                                     uint32_t      ulTimeout)
{
  int32_t                       lRet               = CIFX_FUNCTION_NOT_AVAILABLE;
  CIFX_TRANSPORT_DEVICE_INST_T* ptcifXTransportDev = (CIFX_TRANSPORT_DEVICE_INST_T*)hSysdevice;
  CIFX_DRV_FUNCTION_TABLE_T*    ptCifXFctTable     = (CIFX_DRV_FUNCTION_TABLE_T*)ptcifXTransportDev->ptCifXFctTable;

  CHECK_SYS_HANDLE( hSysdevice);
  ATTACH_DEVICE( ptcifXTransportDev->hNetXTransportDeviceHandle);

  if (ptCifXFctTable->pfnxSysdevicePutPacket)
  {
    lRet = ptCifXFctTable->pfnxSysdevicePutPacket( ptcifXTransportDev->hSysDevice, ptSendPkt, ulTimeout);
  }
  DETACH_DEVICE( ptcifXTransportDev->hNetXTransportDeviceHandle);

  return lRet;
}

/*! **************************************************************************
* Retrieves a packet from the System Mailbox
*   \param hSysdevice      Handle to the System device
*   \param ulSize          Size of the buffer to retrieve the packet
*   \param ptRecvPkt       Pointer to buffer for received packet
*   \param ulTimeout       maximum time to wait for packet to be delivered by device (in ms)
*   \return CIFX_NO_ERROR on success
******************************************************************************/
int32_t APIENTRY xSysdeviceGetPacket(CIFXHANDLE    hSysdevice,
                                     uint32_t      ulSize,
                                     CIFX_PACKET*  ptRecvPkt,
                                     uint32_t      ulTimeout)
{
  int32_t                       lRet               = CIFX_FUNCTION_NOT_AVAILABLE;
  CIFX_TRANSPORT_DEVICE_INST_T* ptcifXTransportDev = (CIFX_TRANSPORT_DEVICE_INST_T*)hSysdevice;
  CIFX_DRV_FUNCTION_TABLE_T*    ptCifXFctTable     = (CIFX_DRV_FUNCTION_TABLE_T*)ptcifXTransportDev->ptCifXFctTable;

  CHECK_SYS_HANDLE( hSysdevice);
  ATTACH_DEVICE( ptcifXTransportDev->hNetXTransportDeviceHandle);

  if (ptCifXFctTable->pfnxSysdeviceGetPacket)
  {
    lRet = ptCifXFctTable->pfnxSysdeviceGetPacket( ptcifXTransportDev->hSysDevice,  ulSize,  ptRecvPkt, ulTimeout);
  }
  DETACH_DEVICE( ptcifXTransportDev->hNetXTransportDeviceHandle);

  return lRet;
}

/*! **************************************************************************
* Reset the whole device (coldstart)
*   \param hSysdevice   Sysdevice handle acquired by xSysdeviceOpen
*   \param ulTimeout    Time in ms to wait for the reset
*   \return CIFX_NO_ERROR on success
******************************************************************************/
int32_t APIENTRY xSysdeviceReset(CIFXHANDLE hSysdevice,
                                  uint32_t  ulTimeout)
{
  int32_t                       lRet               = CIFX_FUNCTION_NOT_AVAILABLE;
  CIFX_TRANSPORT_DEVICE_INST_T* ptcifXTransportDev = (CIFX_TRANSPORT_DEVICE_INST_T*)hSysdevice;
  CIFX_DRV_FUNCTION_TABLE_T*    ptCifXFctTable     = (CIFX_DRV_FUNCTION_TABLE_T*)ptcifXTransportDev->ptCifXFctTable;

  CHECK_SYS_HANDLE( hSysdevice);
  ATTACH_DEVICE( ptcifXTransportDev->hNetXTransportDeviceHandle);

  if (ptCifXFctTable->pfnxSysdeviceReset)
  {
    netXTransportRegisterReconnect( ptcifXTransportDev->hNetXTransportDeviceHandle);
    lRet = ptCifXFctTable->pfnxSysdeviceReset( ptcifXTransportDev->hSysDevice, ulTimeout);
  }
  DETACH_DEVICE( ptcifXTransportDev->hNetXTransportDeviceHandle);

  if ((ptCifXFctTable->pfnxSysdeviceReset) && (lRet == CIFX_NO_ERROR))
  {
    if (NXT_NO_ERROR != netXTransportScheduleReconnect( ptcifXTransportDev->hNetXTransportDeviceHandle, ulTimeout))
    {
      lRet = CIFX_DEV_RESET_TIMEOUT;
    }
  }
  netXTransportDeRegisterReconnect( ptcifXTransportDev->hNetXTransportDeviceHandle);

  return lRet;
}

/*! **************************************************************************
* Download a file (Firmware, Configuratio, etc) to the device
*   \param hSysdevice         Handle to the system device
*   \param ulChannel          Channel number to load the file to
*   \param ulMode             Download mode (DOWNLOAD_MODE_FIRMWARE, etc)
*   \param szFileName         Name of the file
*   \param pabFileData        Pointer to the file data
*   \param ulFileSize         Length of the file data
*   \param pfnCallback        Callback for progress indication (NULL for no callback)
*   \param pfnRecvPktCallback Callback pointer for unhandled receive packets
*   \param pvUser             User parameter on callback.
*   \return CIFX_NO_ERROR on success
******************************************************************************/
int32_t APIENTRY xSysdeviceDownload(CIFXHANDLE          hSysdevice,
                                  uint32_t              ulChannel,
                                  uint32_t              ulMode,
                                  char*                 szFileName,
                                  uint8_t*              pabFileData,
                                  uint32_t              ulFileSize,
                                  PFN_PROGRESS_CALLBACK pfnCallback,
                                  PFN_RECV_PKT_CALLBACK pfnRecvPktCallback,
                                  void*                 pvUser)
{
  int32_t                       lRet               = CIFX_FUNCTION_NOT_AVAILABLE;
  CIFX_TRANSPORT_DEVICE_INST_T* ptcifXTransportDev = (CIFX_TRANSPORT_DEVICE_INST_T*)hSysdevice;
  CIFX_DRV_FUNCTION_TABLE_T*    ptCifXFctTable     = (CIFX_DRV_FUNCTION_TABLE_T*)ptcifXTransportDev->ptCifXFctTable;

  CHECK_SYS_HANDLE( hSysdevice);
  ATTACH_DEVICE( ptcifXTransportDev->hNetXTransportDeviceHandle);

  if (ptCifXFctTable->pfnxSysdeviceDownload)
  {
    lRet = ptCifXFctTable->pfnxSysdeviceDownload( ptcifXTransportDev->hSysDevice,
                                                      ulChannel,
                                                      ulMode,
                                                      szFileName,
                                                      pabFileData,
                                                      ulFileSize,
                                                      ptcifXTransportDev,
                                                      pfnCallback,
                                                      pfnRecvPktCallback,
                                                      pvUser);
  }
  DETACH_DEVICE( ptcifXTransportDev->hNetXTransportDeviceHandle);

  return lRet;
}

/*****************************************************************************/
/*! Starts directory enumeration on the given channel
*   \param hSysdevice         Handle to the channel
*   \param ulChannel          Channel number to get directory from
*   \param ptDirectoryInfo    Pointer to enumeration result.
*                             (Will be initialized inside function)
*   \param pfnRecvPktCallback Callback pointer for unhandled receive packets
*   \param pvUser             User parameter on callback.
*   \return CIFX_NO_ERROR on success                                         */
/*****************************************************************************/
int32_t APIENTRY xSysdeviceFindFirstFile(  CIFXHANDLE         hSysdevice,
                                        uint32_t              ulChannel,
                                        CIFX_DIRECTORYENTRY*  ptDirectoryInfo,
                                        PFN_RECV_PKT_CALLBACK pfnRecvPktCallback,
                                        void*                 pvUser)
{
  int32_t                       lRet               = CIFX_FUNCTION_NOT_AVAILABLE;
  CIFX_TRANSPORT_DEVICE_INST_T* ptcifXTransportDev = (CIFX_TRANSPORT_DEVICE_INST_T*)hSysdevice;
  CIFX_DRV_FUNCTION_TABLE_T*    ptCifXFctTable     = (CIFX_DRV_FUNCTION_TABLE_T*)ptcifXTransportDev->ptCifXFctTable;

  CHECK_SYS_HANDLE( hSysdevice);
  ATTACH_DEVICE( ptcifXTransportDev->hNetXTransportDeviceHandle);

  if (ptCifXFctTable->pfnxSysdeviceFindFirstFile)
  {
    lRet = ptCifXFctTable->pfnxSysdeviceFindFirstFile( ptcifXTransportDev->hSysDevice,
                                                      ulChannel,
                                                      ptDirectoryInfo,
                                                      pfnRecvPktCallback,
                                                      pvUser);
  }
  DETACH_DEVICE( ptcifXTransportDev->hNetXTransportDeviceHandle);

  return lRet;
}

/*****************************************************************************/
/*! Enumerate next entry in directoy on the given channel
*   \param hSysdevice         Handle to the system device
*   \param ulChannel          Channel number to get directory from
*   \param ptDirectoryInfo    Pointer to enumeration result.
*   \param pfnRecvPktCallback Callback pointer for unhandled receive packets
*   \param pvUser             User parameter on callback.
*   \return CIFX_NO_ERROR on success                                         */
/*****************************************************************************/
int32_t APIENTRY xSysdeviceFindNextFile( CIFXHANDLE             hSysdevice,
                                          uint32_t              ulChannel,
                                          CIFX_DIRECTORYENTRY*  ptDirectoryInfo,
                                          PFN_RECV_PKT_CALLBACK pfnRecvPktCallback,
                                          void*                 pvUser)
{
  int32_t                       lRet               = CIFX_FUNCTION_NOT_AVAILABLE;
  CIFX_TRANSPORT_DEVICE_INST_T* ptcifXTransportDev = (CIFX_TRANSPORT_DEVICE_INST_T*)hSysdevice;
  CIFX_DRV_FUNCTION_TABLE_T*    ptCifXFctTable     = (CIFX_DRV_FUNCTION_TABLE_T*)ptcifXTransportDev->ptCifXFctTable;

  CHECK_SYS_HANDLE( hSysdevice);
  ATTACH_DEVICE( ptcifXTransportDev->hNetXTransportDeviceHandle);

  if (ptCifXFctTable->pfnxSysdeviceFindNextFile)
  {
    lRet = ptCifXFctTable->pfnxSysdeviceFindNextFile( ptcifXTransportDev->hSysDevice,
                                                          ulChannel,
                                                          ptDirectoryInfo,
                                                          pfnRecvPktCallback,
                                                          pvUser);
  }
  DETACH_DEVICE( ptcifXTransportDev->hNetXTransportDeviceHandle);

  return lRet;
}

/*****************************************************************************/
/*! Uploads a file via system channel
*   \param hSysdevice         Handle to the System device
*   \param ulChannel          Channel number to get directory from
*   \param ulMode             Transfer Mode
*   \param szFileName         Filename to upload
*   \param pabFileData        Pointer to buffer receiving upload
*   \param pulFileSize        [in]Length of buffer, [out] Bytes copied to buffer
*   \param pfnCallback        Callback pointer for progress
*   \param pfnRecvPktCallback Callback pointer for unhandled receive packets
*   \param pvUser             User parameter on callback.
*   \return CIFX_NO_ERROR on success                                         */
/*****************************************************************************/
int32_t APIENTRY xSysdeviceUpload( CIFXHANDLE         hSysdevice,
                                uint32_t              ulChannel,
                                uint32_t              ulMode,
                                char*                 szFileName,
                                uint8_t*              pabFileData,
                                uint32_t*             pulFileSize,
                                PFN_PROGRESS_CALLBACK pfnCallback,
                                PFN_RECV_PKT_CALLBACK pfnRecvPktCallback,
                                void*                 pvUser)
{
  int32_t                       lRet               = CIFX_NO_ERROR;
  CIFX_TRANSPORT_DEVICE_INST_T* ptcifXTransportDev = (CIFX_TRANSPORT_DEVICE_INST_T*)hSysdevice;
  CIFX_DRV_FUNCTION_TABLE_T*    ptCifXFctTable     = (CIFX_DRV_FUNCTION_TABLE_T*)ptcifXTransportDev->ptCifXFctTable;

  CHECK_SYS_HANDLE( hSysdevice);
  ATTACH_DEVICE( ptcifXTransportDev->hNetXTransportDeviceHandle);

  if (ptCifXFctTable->pfnxSysdeviceUpload)
  {
    lRet = ptCifXFctTable->pfnxSysdeviceUpload( ptcifXTransportDev->hSysDevice,
                                                          ulChannel,
                                                          ulMode,
                                                          szFileName,
                                                          pabFileData,
                                                          pulFileSize,
                                                          ptcifXTransportDev,
                                                          pfnCallback,
                                                          pfnRecvPktCallback,
                                                          pvUser);
  }
  DETACH_DEVICE( ptcifXTransportDev->hNetXTransportDeviceHandle);

  return lRet;
}

/*****************************************************************************/
/*! Boot start reset to via system channel
*   \param hSysdevice Handle to system device
*   \param ulTimeout  Timeout to wait for card to finish reset
*   \return CIFX_NO_ERROR on success                                         */
/*****************************************************************************/
int32_t APIENTRY xSysdeviceBootstart(CIFXHANDLE hSysdevice, uint32_t ulTimeout)
{
  int32_t                       lRet               = CIFX_FUNCTION_NOT_AVAILABLE;
  CIFX_TRANSPORT_DEVICE_INST_T* ptcifXTransportDev = (CIFX_TRANSPORT_DEVICE_INST_T*)hSysdevice;
  CIFX_DRV_FUNCTION_TABLE_T*    ptCifXFctTable     = (CIFX_DRV_FUNCTION_TABLE_T*)ptcifXTransportDev->ptCifXFctTable;

  UNREFERENCED_PARAMETER(hSysdevice);
  UNREFERENCED_PARAMETER(ulTimeout);
  UNREFERENCED_PARAMETER(ptCifXFctTable);

  return lRet;
}

/*****************************************************************************/
/*! Get/Return a memory pointer to an extended board memory if available
*   \param hSysdevice   Handle to system device
*   \param ulCmd        Command for get/free
*   \param ptExtMemInfo Pointer to a user buffer to return the information
*   \return CIFX_NO_ERROR on success                                         */
/*****************************************************************************/
int32_t APIENTRY xSysdeviceExtendedMemory(CIFXHANDLE hSysdevice, uint32_t ulCmd, CIFX_EXTENDED_MEMORY_INFORMATION* ptExtMemInfo)
{
  int32_t                       lRet               = CIFX_FUNCTION_NOT_AVAILABLE;
  CIFX_TRANSPORT_DEVICE_INST_T* ptcifXTransportDev = (CIFX_TRANSPORT_DEVICE_INST_T*)hSysdevice;
  CIFX_DRV_FUNCTION_TABLE_T*    ptCifXFctTable     = (CIFX_DRV_FUNCTION_TABLE_T*)ptcifXTransportDev->ptCifXFctTable;

  UNREFERENCED_PARAMETER(hSysdevice);
  UNREFERENCED_PARAMETER(ulCmd);
  UNREFERENCED_PARAMETER(ptExtMemInfo);
  UNREFERENCED_PARAMETER(ptCifXFctTable);

  return lRet;
}

/***************************************************************************
*
*
*   CHANNEL FUNCTIONS....................
*
*
*
*
*
******************************************************************************/
/*! **************************************************************************
* Opens a channel by name (Name can be obtained when enumerating Channels)
*   \param hDriver    Driver handle
*   \param szBoard    Name of the board
*   \param ulChannel  Channel number
*   \param phChannel  Returned handle to the channel (Needed for all channel specific operations)
*   \return CIFX_NO_ERROR on success
******************************************************************************/
int32_t APIENTRY xChannelOpen( CIFXHANDLE  hDriver,
                               char*       szBoard,
                               uint32_t    ulChannel,
                               CIFXHANDLE* phChannel)
{
  int32_t                       lRet                  = CIFX_INVALID_BOARD;
  CIFX_TRANSPORT_DEVICE_INST_T* ptnetXTransportDevice = NULL;
  CIFX_DRV_FUNCTION_TABLE_T*    ptCifXFctTable        = NULL;

  if ((g_ptCifXDrvInfo == NULL) || (g_ptCifXDrvInfo->fInitialized == 0))
  {
    return CIFX_DRV_NOT_INITIALIZED;
  } else if (g_ptCifXDrvInfo != (CIFX_DRIVER_INFO_T*)hDriver)
  {
    return CIFX_INVALID_HANDLE;
  } else if (NULL == (ptnetXTransportDevice = OS_Memalloc(sizeof(CIFX_TRANSPORT_DEVICE_INST_T))))
  {
    return CIFX_FUNCTION_FAILED;
  }

  /* first search for the requested device */
  if (NXT_NO_ERROR != netXTransportGetDeviceHandle( szBoard, &ptnetXTransportDevice->hNetXTransportDeviceHandle))
  {
    lRet = CIFX_INVALID_BOARD;

  /* if the device exists, get the function pointer table of the translations-layer, the device is 'talking' (rcXPacket (0x100) or Marshaller (0x200)) */
  }else if (NXT_NO_ERROR != netXTransportGetTLFctTableByDevice( ptnetXTransportDevice->hNetXTransportDeviceHandle, &ptnetXTransportDevice->tTLFctTable))
  {
    lRet = CIFX_INVALID_BOARD;
  } else if (NULL == ptnetXTransportDevice->tTLFctTable.pvTranslationLayerFunctionTable)
  {
    lRet = CIFX_INVALID_BOARD;
  } else
  {
    ptnetXTransportDevice->ptCifXFctTable = ptnetXTransportDevice->tTLFctTable.pvTranslationLayerFunctionTable;
    ptCifXFctTable                        = ptnetXTransportDevice->tTLFctTable.pvTranslationLayerFunctionTable;
    lRet = CIFX_NO_ERROR;
  }

  if (lRet == CIFX_NO_ERROR)
  {
    lRet = CIFX_FUNCTION_NOT_AVAILABLE;

    /* Before transmitting any data check if the connection is already available (if so, netXTransportAttachDevice() */
    /* will lock the connection resources until we call netXTransportDetachDevice() or an error occurs).             */
    if (0 == netXTransportAttachDevice( ptnetXTransportDevice->hNetXTransportDeviceHandle))
          return CIFX_INVALID_HANDLE;

    /* there is no need to check the version, since the this is already checked in xDriverOpen() */
    if ((ptCifXFctTable) && (ptCifXFctTable->pfnxChannelOpen))
    {
      if (CIFX_NO_ERROR == (lRet = ptCifXFctTable->pfnxChannelOpen( (CIFXHANDLE)ptnetXTransportDevice,
                                                  szBoard,
                                                  ulChannel,
                                                  &ptnetXTransportDevice->hChannelHandle)))
      {
        *phChannel = (CIFXHANDLE)ptnetXTransportDevice;

        /* notify netXTransport Layer to keep this connection active until we close it (if supported Keep-Alive,...) */
        netXTransportActivateConnection( ptnetXTransportDevice->hNetXTransportDeviceHandle);

        /* store handle in list, to be able to verify user input */
        if (g_ptCifXDrvInfo->pvDevListLock)
        {
          OS_EnterLock(g_ptCifXDrvInfo->pvDevListLock);

          TAILQ_INSERT_TAIL(&g_ptCifXDrvInfo->tChannelDeviceList, ptnetXTransportDevice, tList);

          OS_LeaveLock(g_ptCifXDrvInfo->pvDevListLock);
        }
      }
    }
    /* Notify netXTransport-Layer that all connection related resources are now free */
    netXTransportDetachDevice( ptnetXTransportDevice->hNetXTransportDeviceHandle);
  }

  if (lRet != CIFX_NO_ERROR)
  {
    if (NULL != ptnetXTransportDevice)
    {
      OS_Memfree(ptnetXTransportDevice);
    }
  }

  return lRet;
}

/*! **************************************************************************
* Closes an open channel
*   \param hChannel Channel handle acquired by xChannelOpen
*   \return CIFX_NO_ERROR on success
******************************************************************************/
int32_t APIENTRY xChannelClose(CIFXHANDLE hChannel)
{
  int32_t                       lRet               = CIFX_INVALID_PARAMETER;
  CIFX_TRANSPORT_DEVICE_INST_T* ptcifXTransportDev = (CIFX_TRANSPORT_DEVICE_INST_T*)hChannel;

  CHECK_CHANNEL_HANDLE( hChannel);

  if (NULL != ptcifXTransportDev)
  {
    lRet = CIFX_INVALID_CHANNEL;

    if (1 == netXTransportAttachDevice( ptcifXTransportDev->hNetXTransportDeviceHandle))
    {
      CIFX_DRV_FUNCTION_TABLE_T* ptCifXFctTable = (CIFX_DRV_FUNCTION_TABLE_T*)ptcifXTransportDev->ptCifXFctTable;

      if (ptCifXFctTable->pfnxChannelClose)
      {
        lRet = ptCifXFctTable->pfnxChannelClose( ptcifXTransportDev->hChannelHandle);
      }
      netXTransportDetachDevice( ptcifXTransportDev->hNetXTransportDeviceHandle);

      if (lRet == CIFX_NO_ERROR)
      {
        OS_EnterLock(g_ptCifXDrvInfo->pvDevListLock);

        TAILQ_REMOVE(&g_ptCifXDrvInfo->tChannelDeviceList, ptcifXTransportDev, tList);

        OS_LeaveLock(g_ptCifXDrvInfo->pvDevListLock);

        /* notifiy netXTransport Layer that connection is closed */
        netXTransportDeActivateConnection( ptcifXTransportDev->hNetXTransportDeviceHandle);

        ptcifXTransportDev->hNetXTransportDeviceHandle = NULL;
        ptcifXTransportDev->hChannelHandle             = NULL;
        OS_Memfree(ptcifXTransportDev);
      }
    }
  }

  return lRet;
}

/*! **************************************************************************
* Reset the channel
*   \param hChannel     Channel handle acquired by xChannelOpen
*   \param ulResetMode  Cold/Warmstart selection
*   \param ulTimeout    Time in ms to wait for the reset
*   \return CIFX_NO_ERROR on success
******************************************************************************/
int32_t APIENTRY xChannelReset( CIFXHANDLE hChannel,
                                uint32_t   ulResetMode,
                                uint32_t   ulTimeout)
{
  int32_t                       lRet               = CIFX_FUNCTION_NOT_AVAILABLE;
  CIFX_TRANSPORT_DEVICE_INST_T* ptcifXTransportDev = (CIFX_TRANSPORT_DEVICE_INST_T*)hChannel;
  CIFX_DRV_FUNCTION_TABLE_T*    ptCifXFctTable     = (CIFX_DRV_FUNCTION_TABLE_T*)ptcifXTransportDev->ptCifXFctTable;

  CHECK_CHANNEL_HANDLE( hChannel);
  ATTACH_DEVICE( ptcifXTransportDev->hNetXTransportDeviceHandle);

  if (ptCifXFctTable->pfnxChannelReset)
  {
    lRet = ptCifXFctTable->pfnxChannelReset( ptcifXTransportDev->hChannelHandle,
                                              ulResetMode,
                                              ulTimeout);
  }
  DETACH_DEVICE( ptcifXTransportDev->hNetXTransportDeviceHandle);

  return lRet;
}

/*! **************************************************************************
* Returns Channel specific information (same information as from Channel enumeration)
*   \param hChannel       Channel handle acquired by xChannelOpen
*   \param ulSize         Size of the user buffer
*   \param pvChannelInfo  Returned channel specific information
*   \return CIFX_NO_ERROR on success
******************************************************************************/
int32_t APIENTRY xChannelInfo( CIFXHANDLE hChannel,
                               uint32_t   ulSize,
                               void*      pvChannelInfo)
{
  int32_t                       lRet               = CIFX_FUNCTION_NOT_AVAILABLE;
  CIFX_TRANSPORT_DEVICE_INST_T* ptcifXTransportDev = (CIFX_TRANSPORT_DEVICE_INST_T*)hChannel;
  CIFX_DRV_FUNCTION_TABLE_T*    ptCifXFctTable     = (CIFX_DRV_FUNCTION_TABLE_T*)ptcifXTransportDev->ptCifXFctTable;

  CHECK_CHANNEL_HANDLE( hChannel);
  ATTACH_DEVICE( ptcifXTransportDev->hNetXTransportDeviceHandle);

  if (ptCifXFctTable->pfnxChannelInfo)
  {
    lRet = ptCifXFctTable->pfnxChannelInfo( ptcifXTransportDev->hChannelHandle,
                                                  ulSize,
                                                  pvChannelInfo);
  }
  DETACH_DEVICE( ptcifXTransportDev->hNetXTransportDeviceHandle);

  return lRet;
}

/*! **************************************************************************
* Returns the Mailbox state from a specific channel
*   \param hChannel         Channel handle acquired by xChannelOpen
*   \param pulRecvPktCount  Number of Messages waiting in receive mailbox
*   \param pulSendMbxState  State of the Send Mailbox
*   \return CIFX_NO_ERROR on success
******************************************************************************/
int32_t APIENTRY xChannelGetMBXState( CIFXHANDLE hChannel,
                                      uint32_t* pulRecvPktCount,
                                      uint32_t* pulSendMbxState)
{
  int32_t                       lRet               = CIFX_FUNCTION_NOT_AVAILABLE;
  CIFX_TRANSPORT_DEVICE_INST_T* ptcifXTransportDev = (CIFX_TRANSPORT_DEVICE_INST_T*)hChannel;
  CIFX_DRV_FUNCTION_TABLE_T*    ptCifXFctTable     = (CIFX_DRV_FUNCTION_TABLE_T*)ptcifXTransportDev->ptCifXFctTable;

  CHECK_CHANNEL_HANDLE( hChannel);
  ATTACH_DEVICE( ptcifXTransportDev->hNetXTransportDeviceHandle);

  if (ptCifXFctTable->pfnxChannelGetMBXState)
  {
    lRet = ptCifXFctTable->pfnxChannelGetMBXState( ptcifXTransportDev->hChannelHandle,
                                                   pulRecvPktCount,
                                                   pulSendMbxState);
  }
  DETACH_DEVICE( ptcifXTransportDev->hNetXTransportDeviceHandle);

  return lRet;
}

/*! **************************************************************************
* Inserts a packet into a channel mailbox
*   \param hChannel   Channel handle acquired by xChannelOpen
*   \param ptSendPkt  Packet to send to channel
*   \param ulTimeout  Time in ms to wait for card to accept the packet
*   \return CIFX_NO_ERROR on success
******************************************************************************/
int32_t APIENTRY xChannelPutPacket( CIFXHANDLE   hChannel,
                                    CIFX_PACKET* ptSendPkt,
                                    uint32_t     ulTimeout)
{
  int32_t                       lRet               = CIFX_FUNCTION_NOT_AVAILABLE;
  CIFX_TRANSPORT_DEVICE_INST_T* ptcifXTransportDev = (CIFX_TRANSPORT_DEVICE_INST_T*)hChannel;
  CIFX_DRV_FUNCTION_TABLE_T*    ptCifXFctTable     = (CIFX_DRV_FUNCTION_TABLE_T*)ptcifXTransportDev->ptCifXFctTable;

  CHECK_CHANNEL_HANDLE( hChannel);
  ATTACH_DEVICE( ptcifXTransportDev->hNetXTransportDeviceHandle);

  if (ptCifXFctTable->pfnxChannelPutPacket)
  {
    lRet = ptCifXFctTable->pfnxChannelPutPacket( ptcifXTransportDev->hChannelHandle,
                                                  ptSendPkt,
                                                  ulTimeout);
  }
  DETACH_DEVICE( ptcifXTransportDev->hNetXTransportDeviceHandle);

  return lRet;
}

/*! **************************************************************************
* Gets a packet from a channel mailbox
*   \param hChannel   Channel handle acquired by xChannelOpen
*   \param ulSize     Size of returned packet
*   \param ptRecvPkt  Returned packet
*   \param ulTimeout  Time in ms to wait for available packets
*   \return CIFX_NO_ERROR on success
******************************************************************************/
int32_t APIENTRY xChannelGetPacket( CIFXHANDLE   hChannel,
                                    uint32_t     ulSize,
                                    CIFX_PACKET* ptRecvPkt,
                                    uint32_t     ulTimeout)
{
  int32_t                       lRet               = CIFX_FUNCTION_NOT_AVAILABLE;
  CIFX_TRANSPORT_DEVICE_INST_T* ptcifXTransportDev = (CIFX_TRANSPORT_DEVICE_INST_T*)hChannel;
  CIFX_DRV_FUNCTION_TABLE_T*    ptCifXFctTable     = (CIFX_DRV_FUNCTION_TABLE_T*)ptcifXTransportDev->ptCifXFctTable;

  CHECK_CHANNEL_HANDLE( hChannel);
  ATTACH_DEVICE( ptcifXTransportDev->hNetXTransportDeviceHandle);

  if (ptCifXFctTable->pfnxChannelGetPacket)
  {
    lRet = ptCifXFctTable->pfnxChannelGetPacket( ptcifXTransportDev->hChannelHandle,
                                                  ulSize,
                                                  ptRecvPkt,
                                                  ulTimeout);
  }
  DETACH_DEVICE( ptcifXTransportDev->hNetXTransportDeviceHandle);

  return lRet;
}

/*! **************************************************************************
* Read back a packet from a channel send mailbox
*   \param hChannel   Channel handle acquired by xChannelOpen
*   \param ulSize     Size of returned packet
*   \param ptRecvPkt  Returned packet
*   \return CIFX_NO_ERROR on success
******************************************************************************/
int32_t APIENTRY xChannelGetSendPacket( CIFXHANDLE   hChannel,
                                        uint32_t     ulSize,
                                        CIFX_PACKET* ptRecvPkt)
{
  int32_t                       lRet               = CIFX_FUNCTION_NOT_AVAILABLE;
  CIFX_TRANSPORT_DEVICE_INST_T* ptcifXTransportDev = (CIFX_TRANSPORT_DEVICE_INST_T*)hChannel;
  CIFX_DRV_FUNCTION_TABLE_T*    ptCifXFctTable     = (CIFX_DRV_FUNCTION_TABLE_T*)ptcifXTransportDev->ptCifXFctTable;

  CHECK_CHANNEL_HANDLE( hChannel);
  ATTACH_DEVICE( ptcifXTransportDev->hNetXTransportDeviceHandle);

  if (ptCifXFctTable->pfnxChannelGetSendPacket)
  {
    lRet = ptCifXFctTable->pfnxChannelGetSendPacket( ptcifXTransportDev->hChannelHandle,
                                                     ulSize,
                                                     ptRecvPkt);
  }
  DETACH_DEVICE( ptcifXTransportDev->hNetXTransportDeviceHandle);

  return lRet;
}

/*! **************************************************************************
* Trigger the channel watchdog
*   \param hChannel    Channel handle acquired by xChannelOpen
*   \param ulCmd       Watchdog command
*   \param pulTrigger  Actual trigger value from hardware
*   \return CIFX_NO_ERROR on success
******************************************************************************/
int32_t APIENTRY xChannelWatchdog(CIFXHANDLE hChannel,
                                  uint32_t   ulCmd,
                                  uint32_t*  pulTrigger)
{
  int32_t                       lRet               = CIFX_FUNCTION_NOT_AVAILABLE;
  CIFX_TRANSPORT_DEVICE_INST_T* ptcifXTransportDev = (CIFX_TRANSPORT_DEVICE_INST_T*)hChannel;
  CIFX_DRV_FUNCTION_TABLE_T*    ptCifXFctTable     = (CIFX_DRV_FUNCTION_TABLE_T*)ptcifXTransportDev->ptCifXFctTable;

  CHECK_CHANNEL_HANDLE( hChannel);
  ATTACH_DEVICE( ptcifXTransportDev->hNetXTransportDeviceHandle);

  if (ptCifXFctTable->pfnxChannelWatchdog)
  {
    lRet = ptCifXFctTable->pfnxChannelWatchdog( ptcifXTransportDev->hChannelHandle,
                                                 ulCmd,
                                                 pulTrigger);
  }
  DETACH_DEVICE( ptcifXTransportDev->hNetXTransportDeviceHandle);

  return lRet;
}

/*! **************************************************************************
* Set/Get Host state
*   \param hChannel   Channel handle acquired by xChannelOpen
*   \param ulCmd      Function command
*   \param pulState   Actual state returned by the get command
*   \param ulTimeout  Time in ms to wait for the defined state
*   \return CIFX_NO_ERROR on success
******************************************************************************/
int32_t APIENTRY xChannelHostState( CIFXHANDLE hChannel,
                                    uint32_t   ulCmd,
                                    uint32_t*  pulState,
                                    uint32_t   ulTimeout)
{
  int32_t                       lRet               = CIFX_FUNCTION_NOT_AVAILABLE;
  CIFX_TRANSPORT_DEVICE_INST_T* ptcifXTransportDev = (CIFX_TRANSPORT_DEVICE_INST_T*)hChannel;
  CIFX_DRV_FUNCTION_TABLE_T*    ptCifXFctTable     = (CIFX_DRV_FUNCTION_TABLE_T*)ptcifXTransportDev->ptCifXFctTable;

  CHECK_CHANNEL_HANDLE( hChannel);
  ATTACH_DEVICE( ptcifXTransportDev->hNetXTransportDeviceHandle);

  if (ptCifXFctTable->pfnxChannelHostState)
  {
    lRet = ptCifXFctTable->pfnxChannelHostState( ptcifXTransportDev->hChannelHandle,
                                                  ulCmd,
                                                  pulState,
                                                  ulTimeout);
  }
  DETACH_DEVICE( ptcifXTransportDev->hNetXTransportDeviceHandle);

  return lRet;
}

/*! **************************************************************************
* Set/Get Configuration Lock state
*   \param hChannel   Channel handle acquired by xChannelOpen
*   \param ulCmd      Function command
*   \param pulState   Actual state returned by the get command
*   \param ulTimeout  Time in ms to wait for the defined state
*   \return CIFX_NO_ERROR on success
******************************************************************************/
int32_t APIENTRY xChannelConfigLock( CIFXHANDLE hChannel,
                                     uint32_t   ulCmd,
                                     uint32_t*  pulState,
                                     uint32_t   ulTimeout)
{
  int32_t                       lRet               = CIFX_FUNCTION_NOT_AVAILABLE;
  CIFX_TRANSPORT_DEVICE_INST_T* ptcifXTransportDev = (CIFX_TRANSPORT_DEVICE_INST_T*)hChannel;
  CIFX_DRV_FUNCTION_TABLE_T*    ptCifXFctTable     = (CIFX_DRV_FUNCTION_TABLE_T*)ptcifXTransportDev->ptCifXFctTable;

  CHECK_CHANNEL_HANDLE( hChannel);
  ATTACH_DEVICE( ptcifXTransportDev->hNetXTransportDeviceHandle);

  if (ptCifXFctTable->pfnxChannelConfigLock)
  {
    lRet = ptCifXFctTable->pfnxChannelConfigLock( ptcifXTransportDev->hChannelHandle,
                                                    ulCmd,
                                                    pulState,
                                                    ulTimeout);
  }
  DETACH_DEVICE( ptcifXTransportDev->hNetXTransportDeviceHandle);

  return lRet;
}

/*! **************************************************************************
* Set/Get Bus state
*   \param hChannel   Channel handle acquired by xChannelOpen
*   \param ulCmd      Function command
*   \param pulState   Actual state returned by the get command
*   \param ulTimeout  Time in ms to wait for the defined state
*   \return CIFX_NO_ERROR on success
******************************************************************************/
int32_t APIENTRY xChannelBusState( CIFXHANDLE hChannel,
                                   uint32_t   ulCmd,
                                   uint32_t*  pulState,
                                   uint32_t   ulTimeout)
{
  int32_t                       lRet               = CIFX_FUNCTION_NOT_AVAILABLE;
  CIFX_TRANSPORT_DEVICE_INST_T* ptcifXTransportDev = (CIFX_TRANSPORT_DEVICE_INST_T*)hChannel;
  CIFX_DRV_FUNCTION_TABLE_T*    ptCifXFctTable     = (CIFX_DRV_FUNCTION_TABLE_T*)ptcifXTransportDev->ptCifXFctTable;

  CHECK_CHANNEL_HANDLE( hChannel);
  ATTACH_DEVICE( ptcifXTransportDev->hNetXTransportDeviceHandle);

  if (ptCifXFctTable->pfnxChannelBusState)
  {
    lRet = ptCifXFctTable->pfnxChannelBusState( ptcifXTransportDev->hChannelHandle,
                                                  ulCmd,
                                                  pulState,
                                                  ulTimeout);
  }
  DETACH_DEVICE( ptcifXTransportDev->hNetXTransportDeviceHandle);

  return lRet;
}

/*! **************************************************************************
* Set/Get DMA state (not supported)
*   \param hChannel   Channel handle acquired by xChannelOpen
*   \param ulCmd      Function command
*   \param pulState   Actual state returned by the get command
*   \return CIFX_NO_ERROR on success
******************************************************************************/
int32_t APIENTRY xChannelDMAState( CIFXHANDLE hChannel, uint32_t ulCmd, uint32_t* pulState)
{
  int32_t                       lRet               = CIFX_FUNCTION_NOT_AVAILABLE;
  CIFX_TRANSPORT_DEVICE_INST_T* ptcifXTransportDev = (CIFX_TRANSPORT_DEVICE_INST_T*)hChannel;
  CIFX_DRV_FUNCTION_TABLE_T*    ptCifXFctTable     = (CIFX_DRV_FUNCTION_TABLE_T*)ptcifXTransportDev->ptCifXFctTable;

  UNREFERENCED_PARAMETER(hChannel);
  UNREFERENCED_PARAMETER(ulCmd);
  UNREFERENCED_PARAMETER(pulState);
  UNREFERENCED_PARAMETER(ptCifXFctTable);

  /*if (ptCifXFctTable->pfnxChannelDMAState)
  {
    lRet = ptCifXFctTable->pfnxChannelDMAState( ptcifXTransportDev->hChannelHandle, ulCmd, pulState);
  } */

  return lRet;
}

/*****************************************************************************/
/*! Register notification callbacks (not available on netX transport)
*   \param hChannel         Channel handle
*   \param ulNotification   Notification command
*   \param pfnCallback      Callback function
*   \param pvUser           User parameter for Callback function
*   \return CIFX_FUNCTION_NOT_AVAILABLE                                      */
/*****************************************************************************/
int32_t APIENTRY xChannelRegisterNotification( CIFXHANDLE  hChannel, uint32_t ulNotification, PFN_NOTIFY_CALLBACK  pfnCallback, void* pvUser)
{
  int32_t                       lRet               = CIFX_FUNCTION_NOT_AVAILABLE;
  CIFX_TRANSPORT_DEVICE_INST_T* ptcifXTransportDev = (CIFX_TRANSPORT_DEVICE_INST_T*)hChannel;
  CIFX_DRV_FUNCTION_TABLE_T*    ptCifXFctTable     = (CIFX_DRV_FUNCTION_TABLE_T*)ptcifXTransportDev->ptCifXFctTable;

  UNREFERENCED_PARAMETER(hChannel);
  UNREFERENCED_PARAMETER(ulNotification);
  UNREFERENCED_PARAMETER(pfnCallback);
  UNREFERENCED_PARAMETER(pvUser);
  UNREFERENCED_PARAMETER(ptCifXFctTable);

  /*if (ptCifXFctTable->pfnxChannelRegisterNotification)
  {
    lRet = ptCifXFctTable->pfnxChannelRegisterNotification( ptcifXTransportDev->hChannelHandle, ulNotification, pfnCallback, pvUser);
  } */

  return lRet;
}

/*****************************************************************************/
/*! Unregister notification callbacks (not available on netX transport)
*   \param hChannel         Channel handle
*   \param ulNotification   Notification command
*   \return CIFX_FUNCTION_NOT_AVAILABLE                                      */
/*****************************************************************************/
int32_t APIENTRY xChannelUnregisterNotification( CIFXHANDLE  hChannel, uint32_t ulNotification)
{
  int32_t                       lRet               = CIFX_FUNCTION_NOT_AVAILABLE;
  CIFX_TRANSPORT_DEVICE_INST_T* ptcifXTransportDev = (CIFX_TRANSPORT_DEVICE_INST_T*)hChannel;
  CIFX_DRV_FUNCTION_TABLE_T*    ptCifXFctTable     = (CIFX_DRV_FUNCTION_TABLE_T*)ptcifXTransportDev->ptCifXFctTable;

  UNREFERENCED_PARAMETER(hChannel);
  UNREFERENCED_PARAMETER(ulNotification);
  UNREFERENCED_PARAMETER(ptCifXFctTable);

 /* if (ptCifXFctTable->pfnxChannelUnregisterNotification)
  {
    lRet = ptCifXFctTable->pfnxChannelUnregisterNotification( ptcifXTransportDev->hChannelHandle, ulNotification);
  }*/

  return lRet;
}

/*****************************************************************************/
/*! Wait / Set synch more (not available on netX transport)
*   \param hChannel         Channel handle
*   \param ulCmd            Synch command
*   \param ulTimeout        Timeout in ms
*   \param pulErrorCount    returned error counter
*   \return CIFX_FUNCTION_NOT_AVAILABLE                                      */
/*****************************************************************************/
int32_t APIENTRY xChannelSyncState( CIFXHANDLE hChannel, uint32_t ulCmd, uint32_t ulTimeout, uint32_t* pulErrorCount)
{
  int32_t                       lRet               = CIFX_FUNCTION_NOT_AVAILABLE;
  CIFX_TRANSPORT_DEVICE_INST_T* ptcifXTransportDev = (CIFX_TRANSPORT_DEVICE_INST_T*)hChannel;
  CIFX_DRV_FUNCTION_TABLE_T*    ptCifXFctTable     = (CIFX_DRV_FUNCTION_TABLE_T*)ptcifXTransportDev->ptCifXFctTable;

  UNREFERENCED_PARAMETER(hChannel);
  UNREFERENCED_PARAMETER(ulCmd);
  UNREFERENCED_PARAMETER(ulTimeout);
  UNREFERENCED_PARAMETER(pulErrorCount);
  UNREFERENCED_PARAMETER(ptCifXFctTable);

  /*if (ptCifXFctTable->pfnxChannelSyncState)
  {
    lRet = ptCifXFctTable->pfnxChannelSyncState( ptcifXTransportDev->hChannelHandle, ulCmd, ulTimeout, pulErrorCount);
  } */

  return lRet;
}

/*! **************************************************************************
* Get area information about IO Block
*   \param hChannel     Channel handle acquired by xChannelOpen
*   \param ulCmd        CIFX_IO_INPUT_AREA/CIFX_IO_OUTPUT_AREA
*   \param ulAreaNumber Area number to get data from
*   \param ulSize       Size of passed pointer
*   \param pvData       Pointer to CHANNEL_IO_INFORMATION structure
*   \return CIFX_NO_ERROR on success
******************************************************************************/
int32_t APIENTRY xChannelIOInfo( CIFXHANDLE hChannel,
                                  uint32_t  ulCmd,
                                  uint32_t  ulAreaNumber,
                                  uint32_t  ulSize,
                                  void*     pvData)
{
  int32_t                       lRet               = CIFX_FUNCTION_NOT_AVAILABLE;
  CIFX_TRANSPORT_DEVICE_INST_T* ptcifXTransportDev = (CIFX_TRANSPORT_DEVICE_INST_T*)hChannel;
  CIFX_DRV_FUNCTION_TABLE_T*    ptCifXFctTable     = (CIFX_DRV_FUNCTION_TABLE_T*)ptcifXTransportDev->ptCifXFctTable;

  CHECK_CHANNEL_HANDLE( hChannel);
  ATTACH_DEVICE( ptcifXTransportDev->hNetXTransportDeviceHandle);

  if (ptCifXFctTable->pfnxChannelIOInfo)
  {
    lRet = ptCifXFctTable->pfnxChannelIOInfo( ptcifXTransportDev->hChannelHandle,
                                                ulCmd,
                                                ulAreaNumber,
                                                ulSize,
                                                pvData);
  }
  DETACH_DEVICE( ptcifXTransportDev->hNetXTransportDeviceHandle);

  return lRet;
}

/*! **************************************************************************
* Reads the Input data from the channel
*   \param hChannel     Channel handle acquired by xChannelOpen
*   \param ulAreaNumber Area number
*   \param ulOffset     Data offset in Input area
*   \param ulDataLen    Length of data to read
*   \param pvData       Buffer to place returned data
*   \param ulTimeout    Time to wait for Send confirmation [ms]
*   \return CIFX_NO_ERROR on success
******************************************************************************/
int32_t APIENTRY xChannelIORead( CIFXHANDLE hChannel,
                                 uint32_t   ulAreaNumber,
                                 uint32_t   ulOffset,
                                 uint32_t   ulDataLen,
                                 void*      pvData,
                                 uint32_t   ulTimeout)
{
  int32_t                       lRet               = CIFX_FUNCTION_NOT_AVAILABLE;
  CIFX_TRANSPORT_DEVICE_INST_T* ptcifXTransportDev = (CIFX_TRANSPORT_DEVICE_INST_T*)hChannel;
  CIFX_DRV_FUNCTION_TABLE_T*    ptCifXFctTable     = (CIFX_DRV_FUNCTION_TABLE_T*)ptcifXTransportDev->ptCifXFctTable;

  CHECK_CHANNEL_HANDLE( hChannel);
  ATTACH_DEVICE( ptcifXTransportDev->hNetXTransportDeviceHandle);

  if (ptCifXFctTable->pfnxChannelIORead)
  {
    lRet = ptCifXFctTable->pfnxChannelIORead( ptcifXTransportDev->hChannelHandle,
                                                ulAreaNumber,
                                                ulOffset,
                                                ulDataLen,
                                                pvData,
                                                ulTimeout);
  }
  DETACH_DEVICE( ptcifXTransportDev->hNetXTransportDeviceHandle);

  return lRet;
}

/*! **************************************************************************
* Writes the Output data to the channel
*   \param hChannel     Channel handle acquired by xChannelOpen
*   \param ulAreaNumber Area number
*   \param ulOffset     Data offset in Output area
*   \param ulDataLen    Length of data to send
*   \param pvWriteData  Buffer containing send data
*   \param ulTimeout    Time to wait for Send confirmation [ms]
*   \return CIFX_NO_ERROR on success
******************************************************************************/
int32_t APIENTRY xChannelIOWrite( CIFXHANDLE hChannel,
                                  uint32_t   ulAreaNumber,
                                  uint32_t   ulOffset,
                                  uint32_t   ulDataLen,
                                  void*      pvWriteData,
                                  uint32_t   ulTimeout)
{
  int32_t                       lRet               = CIFX_FUNCTION_NOT_AVAILABLE;
  CIFX_TRANSPORT_DEVICE_INST_T* ptcifXTransportDev = (CIFX_TRANSPORT_DEVICE_INST_T*)hChannel;
  CIFX_DRV_FUNCTION_TABLE_T*    ptCifXFctTable     = (CIFX_DRV_FUNCTION_TABLE_T*)ptcifXTransportDev->ptCifXFctTable;

  CHECK_CHANNEL_HANDLE( hChannel);
  ATTACH_DEVICE( ptcifXTransportDev->hNetXTransportDeviceHandle);

  if (ptCifXFctTable->pfnxChannelIOWrite)
  {
    lRet = ptCifXFctTable->pfnxChannelIOWrite( ptcifXTransportDev->hChannelHandle,
                                                ulAreaNumber,
                                                ulOffset,
                                                ulDataLen,
                                                pvWriteData,
                                                ulTimeout);
  }
  DETACH_DEVICE( ptcifXTransportDev->hNetXTransportDeviceHandle);

  return lRet;
}

/*! **************************************************************************
* Read back Send Data Area from channel
*   \param hChannel     Channel handle acquired by xChannelOpen
*   \param ulAreaNumber Area number
*   \param ulOffset     Data start offset
*   \param ulDataLen    Data length to read
*   \param pvData       Data buffer
*   \return CIFX_NO_ERROR on success
******************************************************************************/
int32_t APIENTRY xChannelIOReadSendData( CIFXHANDLE hChannel,
                                         uint32_t   ulAreaNumber,
                                         uint32_t   ulOffset,
                                         uint32_t   ulDataLen,
                                         void*      pvData)
{
  int32_t                       lRet               = CIFX_FUNCTION_NOT_AVAILABLE;
  CIFX_TRANSPORT_DEVICE_INST_T* ptcifXTransportDev = (CIFX_TRANSPORT_DEVICE_INST_T*)hChannel;
  CIFX_DRV_FUNCTION_TABLE_T*    ptCifXFctTable     = (CIFX_DRV_FUNCTION_TABLE_T*)ptcifXTransportDev->ptCifXFctTable;

  CHECK_CHANNEL_HANDLE( hChannel);
  ATTACH_DEVICE( ptcifXTransportDev->hNetXTransportDeviceHandle);

  if (ptCifXFctTable->pfnxChannelIOReadSendData)
  {
    lRet = ptCifXFctTable->pfnxChannelIOReadSendData( ptcifXTransportDev->hChannelHandle,
                                                        ulAreaNumber,
                                                        ulOffset,
                                                        ulDataLen,
                                                        pvData);
  }
  DETACH_DEVICE( ptcifXTransportDev->hNetXTransportDeviceHandle);

  return lRet;
}

/*! **************************************************************************
* Acces the channels control block
*   \param hChannel     Channel handle acquired by xChannelOpen
*   \param ulCmd        Command
*   \param ulOffset     Data start offset
*   \param ulDataLen    Data length
*   \param pvData       Data buffer
*   \return CIFX_NO_ERROR on success
******************************************************************************/
int32_t APIENTRY xChannelControlBlock( CIFXHANDLE hChannel,
                                        uint32_t  ulCmd,
                                        uint32_t  ulOffset,
                                        uint32_t  ulDataLen,
                                        void*     pvData)
{
  int32_t                       lRet               = CIFX_FUNCTION_NOT_AVAILABLE;
  CIFX_TRANSPORT_DEVICE_INST_T* ptcifXTransportDev = (CIFX_TRANSPORT_DEVICE_INST_T*)hChannel;
  CIFX_DRV_FUNCTION_TABLE_T*    ptCifXFctTable     = (CIFX_DRV_FUNCTION_TABLE_T*)ptcifXTransportDev->ptCifXFctTable;

  CHECK_CHANNEL_HANDLE( hChannel);
  ATTACH_DEVICE( ptcifXTransportDev->hNetXTransportDeviceHandle);

  if (ptCifXFctTable->pfnxChannelControlBlock)
  {
    lRet = ptCifXFctTable->pfnxChannelControlBlock( ptcifXTransportDev->hChannelHandle,
                                                      ulCmd,
                                                      ulOffset,
                                                      ulDataLen,
                                                      pvData);
  }
  DETACH_DEVICE( ptcifXTransportDev->hNetXTransportDeviceHandle);

  return lRet;
}

/*! **************************************************************************
* Acces the channels common status block
*   \param hChannel     Channel handle acquired by xChannelOpen
*   \param ulCmd        Command
*   \param ulOffset     Data start offset
*   \param ulDataLen    Data length
*   \param pvData       Data buffer
*   \return CIFX_NO_ERROR on success
******************************************************************************/
int32_t APIENTRY xChannelCommonStatusBlock( CIFXHANDLE hChannel,
                                            uint32_t   ulCmd,
                                            uint32_t   ulOffset,
                                            uint32_t   ulDataLen,
                                            void*      pvData)
{
  int32_t                       lRet               = CIFX_FUNCTION_NOT_AVAILABLE;
  CIFX_TRANSPORT_DEVICE_INST_T* ptcifXTransportDev = (CIFX_TRANSPORT_DEVICE_INST_T*)hChannel;
  CIFX_DRV_FUNCTION_TABLE_T*    ptCifXFctTable     = (CIFX_DRV_FUNCTION_TABLE_T*)ptcifXTransportDev->ptCifXFctTable;

  CHECK_CHANNEL_HANDLE( hChannel);
  ATTACH_DEVICE( ptcifXTransportDev->hNetXTransportDeviceHandle);

  if (ptCifXFctTable->pfnxChannelCommonStatusBlock)
  {
    lRet = ptCifXFctTable->pfnxChannelCommonStatusBlock( ptcifXTransportDev->hChannelHandle,
                                                          ulCmd,
                                                          ulOffset,
                                                          ulDataLen,
                                                          pvData);
  }
  DETACH_DEVICE( ptcifXTransportDev->hNetXTransportDeviceHandle);

  return lRet;
}

/*! **************************************************************************
* Acces the channels common status block
*   \param hChannel     Channel handle acquired by xChannelOpen
*   \param ulAreaNumber I/O Area number
*   \param ulCmd        Command
*   \param ulOffset     Data start offset
*   \param ulDataLen    Data length
*   \param pvData       Data buffer
*   \return CIFX_NO_ERROR on success
******************************************************************************/
int32_t APIENTRY xChannelUserBlock( CIFXHANDLE hChannel,
                                    uint32_t   ulAreaNumber,
                                    uint32_t   ulCmd,
                                    uint32_t   ulOffset,
                                    uint32_t   ulDataLen,
                                    void*      pvData)
{
  int32_t                       lRet               = CIFX_FUNCTION_NOT_AVAILABLE;
  CIFX_TRANSPORT_DEVICE_INST_T* ptcifXTransportDev = (CIFX_TRANSPORT_DEVICE_INST_T*)hChannel;
  CIFX_DRV_FUNCTION_TABLE_T*    ptCifXFctTable     = (CIFX_DRV_FUNCTION_TABLE_T*)ptcifXTransportDev->ptCifXFctTable;

  CHECK_CHANNEL_HANDLE( hChannel);
  ATTACH_DEVICE( ptcifXTransportDev->hNetXTransportDeviceHandle);

  if (ptCifXFctTable->pfnxChannelUserBlock)
  {
    lRet = ptCifXFctTable->pfnxChannelUserBlock( ptcifXTransportDev->hChannelHandle,
                                                  ulAreaNumber,
                                                  ulCmd,
                                                  ulOffset,
                                                  ulDataLen,
                                                  pvData);
  }
  DETACH_DEVICE( ptcifXTransportDev->hNetXTransportDeviceHandle);

  return lRet;
}

/*! **************************************************************************
* Acces the channels extended status block
*   \param hChannel     Channel handle acquired by xChannelOpen
*   \param ulCmd        Command
*   \param ulOffset     Data start offset
*   \param ulDataLen    Data length
*   \param pvData       Data buffer
*   \return CIFX_NO_ERROR on success
******************************************************************************/
int32_t APIENTRY xChannelExtendedStatusBlock( CIFXHANDLE hChannel,
                                              uint32_t   ulCmd,
                                              uint32_t   ulOffset,
                                              uint32_t   ulDataLen,
                                              void*      pvData)
{
  int32_t                       lRet               = CIFX_FUNCTION_NOT_AVAILABLE;
  CIFX_TRANSPORT_DEVICE_INST_T* ptcifXTransportDev = (CIFX_TRANSPORT_DEVICE_INST_T*)hChannel;
  CIFX_DRV_FUNCTION_TABLE_T*    ptCifXFctTable     = (CIFX_DRV_FUNCTION_TABLE_T*)ptcifXTransportDev->ptCifXFctTable;

  CHECK_CHANNEL_HANDLE( hChannel);
  ATTACH_DEVICE( ptcifXTransportDev->hNetXTransportDeviceHandle);

  if (ptCifXFctTable->pfnxChannelExtendedStatusBlock)
  {
    lRet = ptCifXFctTable->pfnxChannelExtendedStatusBlock( ptcifXTransportDev->hChannelHandle,
                                                            ulCmd,
                                                            ulOffset,
                                                            ulDataLen,
                                                            pvData);
  }
  DETACH_DEVICE( ptcifXTransportDev->hNetXTransportDeviceHandle);

  return lRet;
}

/*! **************************************************************************
* Get a Pointer to the I/O Area
*   \param hChannel       Channel handle acquired by xChannelOpen
*   \param ulCmd          Command
*   \param pvMemoryInfo   Returned I/O Area pointer
*   \return CIFX_NO_ERROR on success
******************************************************************************/
int32_t APIENTRY xChannelPLCMemoryPtr( CIFXHANDLE hChannel, uint32_t ulCmd, void* pvMemoryInfo)
{
  int32_t                       lRet               = CIFX_FUNCTION_NOT_AVAILABLE;
  CIFX_TRANSPORT_DEVICE_INST_T* ptcifXTransportDev = (CIFX_TRANSPORT_DEVICE_INST_T*)hChannel;
  CIFX_DRV_FUNCTION_TABLE_T*    ptCifXFctTable     = (CIFX_DRV_FUNCTION_TABLE_T*)ptcifXTransportDev->ptCifXFctTable;

  CHECK_CHANNEL_HANDLE( hChannel);
  ATTACH_DEVICE( ptcifXTransportDev->hNetXTransportDeviceHandle);

  if (ptCifXFctTable->pfnxChannelPLCMemoryPtr)
  {
    lRet = ptCifXFctTable->pfnxChannelPLCMemoryPtr( ptcifXTransportDev->hChannelHandle, ulCmd, pvMemoryInfo);
  }
  DETACH_DEVICE( ptcifXTransportDev->hNetXTransportDeviceHandle);

  return lRet;
}

/*! **************************************************************************
* Checks if Input Data area has been updated and is ready for reading
*   \param hChannel       Channel handle acquired by xChannelOpen
*   \param ulAreaNumber   Area Number
*   \param pulReadState   State of the input area
*   \return CIFX_NO_ERROR on success
******************************************************************************/
int32_t APIENTRY xChannelPLCIsReadReady(CIFXHANDLE hChannel, uint32_t ulAreaNumber, uint32_t* pulReadState)
{
  int32_t                       lRet               = CIFX_FUNCTION_NOT_AVAILABLE;
  CIFX_TRANSPORT_DEVICE_INST_T* ptcifXTransportDev = (CIFX_TRANSPORT_DEVICE_INST_T*)hChannel;
  CIFX_DRV_FUNCTION_TABLE_T*    ptCifXFctTable     = (CIFX_DRV_FUNCTION_TABLE_T*)ptcifXTransportDev->ptCifXFctTable;

  CHECK_CHANNEL_HANDLE( hChannel);
  ATTACH_DEVICE( ptcifXTransportDev->hNetXTransportDeviceHandle);

  if (ptCifXFctTable->pfnxChannelPLCIsReadReady)
  {
    lRet = ptCifXFctTable->pfnxChannelPLCIsReadReady( ptcifXTransportDev->hChannelHandle, ulAreaNumber, pulReadState);
  }
  DETACH_DEVICE( ptcifXTransportDev->hNetXTransportDeviceHandle);

  return lRet;
}

/*! **************************************************************************
* Checks if Output Data area has been read by the firmware
*   \param hChannel       Channel handle acquired by xChannelOpen
*   \param ulAreaNumber   Area number
*   \param pulWriteState  State of the output area
*   \return CIFX_NO_ERROR on success
******************************************************************************/
int32_t APIENTRY xChannelPLCIsWriteReady(CIFXHANDLE hChannel, uint32_t ulAreaNumber, uint32_t* pulWriteState)
{
  int32_t                       lRet               = CIFX_FUNCTION_NOT_AVAILABLE;
  CIFX_TRANSPORT_DEVICE_INST_T* ptcifXTransportDev = (CIFX_TRANSPORT_DEVICE_INST_T*)hChannel;
  CIFX_DRV_FUNCTION_TABLE_T*    ptCifXFctTable     = (CIFX_DRV_FUNCTION_TABLE_T*)ptcifXTransportDev->ptCifXFctTable;

  CHECK_CHANNEL_HANDLE( hChannel);
  ATTACH_DEVICE( ptcifXTransportDev->hNetXTransportDeviceHandle);

  if (ptCifXFctTable->pfnxChannelPLCIsWriteReady)
  {
    lRet = ptCifXFctTable->pfnxChannelPLCIsWriteReady( ptcifXTransportDev->hChannelHandle, ulAreaNumber, pulWriteState);
  }
  DETACH_DEVICE( ptcifXTransportDev->hNetXTransportDeviceHandle);

  return lRet;
}

/*! **************************************************************************
* Requests Firmware to read the new send data
*   \param hChannel       Channel handle acquired by xChannelOpen
*   \param ulAreaNumber   Area number
*   \return CIFX_NO_ERROR on success
******************************************************************************/
int32_t APIENTRY xChannelPLCActivateWrite(CIFXHANDLE hChannel, uint32_t ulAreaNumber)
{
  int32_t                       lRet               = CIFX_FUNCTION_NOT_AVAILABLE;
  CIFX_TRANSPORT_DEVICE_INST_T* ptcifXTransportDev = (CIFX_TRANSPORT_DEVICE_INST_T*)hChannel;
  CIFX_DRV_FUNCTION_TABLE_T*    ptCifXFctTable     = (CIFX_DRV_FUNCTION_TABLE_T*)ptcifXTransportDev->ptCifXFctTable;

  CHECK_CHANNEL_HANDLE( hChannel);
  ATTACH_DEVICE( ptcifXTransportDev->hNetXTransportDeviceHandle);

  if (ptCifXFctTable->pfnxChannelPLCActivateWrite)
  {
    lRet = ptCifXFctTable->pfnxChannelPLCActivateWrite( ptcifXTransportDev->hChannelHandle, ulAreaNumber);
  }
  DETACH_DEVICE( ptcifXTransportDev->hNetXTransportDeviceHandle);

  return lRet;
}

/*! **************************************************************************
* Requests Firmware to update input data
*   \param hChannel       Channel handle acquired by xChannelOpen
*   \param ulAreaNumber   I/O Area number
*   \return CIFX_NO_ERROR on success
******************************************************************************/
int32_t APIENTRY xChannelPLCActivateRead(CIFXHANDLE hChannel, uint32_t ulAreaNumber)
{
  int32_t                       lRet               = CIFX_FUNCTION_NOT_AVAILABLE;
  CIFX_TRANSPORT_DEVICE_INST_T* ptcifXTransportDev = (CIFX_TRANSPORT_DEVICE_INST_T*)hChannel;
  CIFX_DRV_FUNCTION_TABLE_T*    ptCifXFctTable     = (CIFX_DRV_FUNCTION_TABLE_T*)ptcifXTransportDev->ptCifXFctTable;

  CHECK_CHANNEL_HANDLE( hChannel);
  ATTACH_DEVICE( ptcifXTransportDev->hNetXTransportDeviceHandle);

  if (ptCifXFctTable->pfnxChannelPLCActivateRead)
  {
    lRet = ptCifXFctTable->pfnxChannelPLCActivateRead( ptcifXTransportDev->hChannelHandle, ulAreaNumber);
  }
  DETACH_DEVICE( ptcifXTransportDev->hNetXTransportDeviceHandle);

  return lRet;
}

/*! **************************************************************************
* Download a file (Firmware, Configuration, etc) to the device
*   \param hChannel           Handle to the channel
*   \param ulMode             Download mode (DOWNLOAD_MODE_FIRMWARE, etc)
*   \param szFileName         Name of the file
*   \param pabFileData        Pointer to the file data
*   \param ulFileSize         Length of the file data
*   \param pfnCallback        Callback for progress indication (NULL for no callback)
*   \param pfnRecvPktCallback Callback pointer for unhandled receive packets
*   \param pvUser             User parameter on callback.
*   \return CIFX_NO_ERROR on success
******************************************************************************/
int32_t APIENTRY xChannelDownload(CIFXHANDLE         hChannel,
                               uint32_t              ulMode,
                               char*                 szFileName,
                               uint8_t*              pabFileData,
                               uint32_t              ulFileSize,
                               PFN_PROGRESS_CALLBACK pfnCallback,
                               PFN_RECV_PKT_CALLBACK pfnRecvPktCallback,
                               void*                 pvUser)
{
  int32_t                       lRet               = CIFX_FUNCTION_NOT_AVAILABLE;
  CIFX_TRANSPORT_DEVICE_INST_T* ptcifXTransportDev = (CIFX_TRANSPORT_DEVICE_INST_T*)hChannel;
  CIFX_DRV_FUNCTION_TABLE_T*    ptCifXFctTable     = (CIFX_DRV_FUNCTION_TABLE_T*)ptcifXTransportDev->ptCifXFctTable;

  CHECK_CHANNEL_HANDLE( hChannel);
  ATTACH_DEVICE( ptcifXTransportDev->hNetXTransportDeviceHandle);

  if (ptCifXFctTable->pfnxChannelDownload)
  {
    lRet = ptCifXFctTable->pfnxChannelDownload( ptcifXTransportDev->hChannelHandle,
                                                  ulMode,
                                                  szFileName,
                                                  pabFileData,
                                                  ulFileSize,
                                                  ptcifXTransportDev,
                                                  pfnCallback,
                                                  pfnRecvPktCallback,
                                                  pvUser);
  }
  DETACH_DEVICE( ptcifXTransportDev->hNetXTransportDeviceHandle);

  return lRet;
}

/*****************************************************************************/
/*! Starts directory enumeration on the given channel
*   \param hChannel           Handle to the channel
*   \param ptDirectoryInfo    Pointer to enumeration result.
*                             (Will be initialized inside function)
*   \param pfnRecvPktCallback Callback pointer for unhandled receive packets
*   \param pvUser             User parameter on callback.
*   \return CIFX_NO_ERROR on success                                         */
/*****************************************************************************/
int32_t APIENTRY xChannelFindFirstFile( CIFXHANDLE            hChannel,
                                        CIFX_DIRECTORYENTRY*  ptDirectoryInfo,
                                        PFN_RECV_PKT_CALLBACK pfnRecvPktCallback,
                                        void*                 pvUser)
{
  int32_t                       lRet               = CIFX_FUNCTION_NOT_AVAILABLE;
  CIFX_TRANSPORT_DEVICE_INST_T* ptcifXTransportDev = (CIFX_TRANSPORT_DEVICE_INST_T*)hChannel;
  CIFX_DRV_FUNCTION_TABLE_T*    ptCifXFctTable     = (CIFX_DRV_FUNCTION_TABLE_T*)ptcifXTransportDev->ptCifXFctTable;

  CHECK_CHANNEL_HANDLE( hChannel);
  ATTACH_DEVICE( ptcifXTransportDev->hNetXTransportDeviceHandle);

  if (ptCifXFctTable->pfnxChannelFindFirstFile)
  {
    lRet = ptCifXFctTable->pfnxChannelFindFirstFile( ptcifXTransportDev->hChannelHandle,
                                                      ptDirectoryInfo,
                                                      pfnRecvPktCallback,
                                                      pvUser);
  }
  DETACH_DEVICE( ptcifXTransportDev->hNetXTransportDeviceHandle);

  return lRet;
}

/*****************************************************************************/
/*! Enumerate next entry in directoy on the given channel
*   \param hChannel           Handle to the channel
*   \param ptDirectoryInfo    Pointer to enumeration result.
*   \param pfnRecvPktCallback Callback pointer for unhandled receive packets
*   \param pvUser             User parameter on callback.
*   \return CIFX_NO_ERROR on success                                         */
/*****************************************************************************/
int32_t APIENTRY xChannelFindNextFile( CIFXHANDLE         hChannel,
                                        CIFX_DIRECTORYENTRY*  ptDirectoryInfo,
                                        PFN_RECV_PKT_CALLBACK pfnRecvPktCallback,
                                        void*                 pvUser)
{
  int32_t                       lRet               = CIFX_FUNCTION_NOT_AVAILABLE;
  CIFX_TRANSPORT_DEVICE_INST_T* ptcifXTransportDev = (CIFX_TRANSPORT_DEVICE_INST_T*)hChannel;
  CIFX_DRV_FUNCTION_TABLE_T*    ptCifXFctTable     = (CIFX_DRV_FUNCTION_TABLE_T*)ptcifXTransportDev->ptCifXFctTable;

  CHECK_CHANNEL_HANDLE( hChannel);
  ATTACH_DEVICE( ptcifXTransportDev->hNetXTransportDeviceHandle);

  if (ptCifXFctTable->pfnxChannelFindNextFile)
  {
    lRet = ptCifXFctTable->pfnxChannelFindNextFile( ptcifXTransportDev->hChannelHandle,
                                                      ptDirectoryInfo,
                                                      pfnRecvPktCallback,
                                                      pvUser);
  }
  DETACH_DEVICE( ptcifXTransportDev->hNetXTransportDeviceHandle);

  return lRet;
}

/*****************************************************************************/
/*! Uploads a file via Communication channel
*   \param hChannel           Handle to the Channel
*   \param ulMode             Transfer Mode
*   \param szFileName         Filename to upload
*   \param pabFileData        Pointer to buffer receiving upload
*   \param pulFileSize        [in]Length of buffer, [out] Bytes copied to buffer
*   \param pfnCallback        Callback pointer for progress
*   \param pfnRecvPktCallback Callback pointer for unhandled receive packets
*   \param pvUser             User parameter on callback.
*   \return CIFX_NO_ERROR on success                                         */
/*****************************************************************************/
int32_t APIENTRY xChannelUpload( CIFXHANDLE         hChannel,
                              uint32_t              ulMode,
                              char*                 szFileName,
                              uint8_t*              pabFileData,
                              uint32_t*             pulFileSize,
                              PFN_PROGRESS_CALLBACK pfnCallback,
                              PFN_RECV_PKT_CALLBACK pfnRecvPktCallback,
                              void*                 pvUser)
{
  int32_t                       lRet               = CIFX_FUNCTION_NOT_AVAILABLE;
  CIFX_TRANSPORT_DEVICE_INST_T* ptcifXTransportDev = (CIFX_TRANSPORT_DEVICE_INST_T*)hChannel;
  CIFX_DRV_FUNCTION_TABLE_T*    ptCifXFctTable     = (CIFX_DRV_FUNCTION_TABLE_T*)ptcifXTransportDev->ptCifXFctTable;

  CHECK_CHANNEL_HANDLE( hChannel);
  ATTACH_DEVICE( ptcifXTransportDev->hNetXTransportDeviceHandle);

  if (ptCifXFctTable->pfnxChannelUpload)
  {
    lRet = ptCifXFctTable->pfnxChannelUpload( ptcifXTransportDev->hChannelHandle,
                                                    ulMode,
                                                    szFileName,
                                                    pabFileData,
                                                    pulFileSize,
                                                    ptcifXTransportDev,
                                                    pfnCallback,
                                                    pfnRecvPktCallback,
                                                    pvUser);
  }
  DETACH_DEVICE( ptcifXTransportDev->hNetXTransportDeviceHandle);

  return lRet;
}
