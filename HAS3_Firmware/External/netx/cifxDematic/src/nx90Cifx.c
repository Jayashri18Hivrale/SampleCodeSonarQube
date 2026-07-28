#include "nx90cifx.h"

#include "OS_Dependent.h"
#include <string.h>

#define N90_IDMP_BOARD_NUMBER 0

static int32_t InitializeToolkit_Netx90(const char * name);
static int32_t GetCifxDriverAndRelatedInfo(CIFXHANDLE * hDriver, DRIVER_INFORMATION * tDriverInfo);
static int32_t ReadBoardInfo( const CIFXHANDLE  hDriver, BOARD_INFORMATION* const ptBoardInfo, const unsigned long ulBoard);

static N90Cifx_t CommFirmwareComm = { { 0 } };

N90Cifx_t * N90Cifx_Init(const char * name)
{
     N90Cifx_t * me = &CommFirmwareComm;
     if(CIFX_NO_ERROR != (me->cifxStatus = InitializeToolkit_Netx90(name))) return 0;
     if(CIFX_NO_ERROR != (me->cifxStatus = GetCifxDriverAndRelatedInfo(&(me->cifxHandle), &(me->cifxDriverInfo)))) return 0;
     // Assert only one board? iDPM
     if(CIFX_NO_ERROR != (me->cifxStatus = ReadBoardInfo(me->cifxHandle, &me->n90_iDPM_Info, N90_IDMP_BOARD_NUMBER))) return 0;
     // Assert board name (me->n90_iDPM_Info.abBoardName == name) ?
     // Assert other relevant info?
     for(uint32_t i = 0; i < me->n90_iDPM_Info.ulChannelCnt; i++)
     {
       if(CIFX_NO_ERROR != (me->cifxStatus = xChannelOpen(me->cifxHandle, me->n90_iDPM_Info.abBoardName, i, &(me->n90Channels[i])))) return 0;
       do{
         if(CIFX_NO_ERROR != (me->cifxStatus = xChannelInfo(me->n90Channels[i], sizeof(me->n90ChannelsInfo[i]), &(me->n90ChannelsInfo[i])))) return 0;
       }
       while(!me->n90ChannelsInfo[i].ulDeviceCOSFlags & HIL_COMM_COS_READY || me->n90ChannelsInfo[i].ulDeviceCOSFlags == CIFX_DPM_NO_MEMORY_ASSIGNED);

       if(CIFX_NO_ERROR != (me->cifxStatus = xChannelHostState(me->n90Channels[i], CIFX_HOST_STATE_READY, NULL, 100))) return 0;
       // Assert Host running?
     }
     return me;
}

static int32_t InitializeToolkit_Netx90(const char * name)
{
  int32_t lRet = CIFX_NO_ERROR;
  static PDEVICEINSTANCE ptDevInstance;

  lRet = cifXTKitInit();

  if(CIFX_NO_ERROR == lRet)
  {
    ptDevInstance = (PDEVICEINSTANCE)OS_Memalloc(sizeof(*ptDevInstance));
    OS_Memset(ptDevInstance, 0, sizeof(*ptDevInstance));

    /** Set trace level of toolkit */
    g_ulTraceLevel =  TRACE_LEVEL_ERROR   |
                      TRACE_LEVEL_WARNING |
                      TRACE_LEVEL_INFO    |
                      TRACE_LEVEL_DEBUG;

    /** Insert the basic device information into the DeviceInstance structure
          for the toolkit.
          NOTE: The irq number are for information use only, so we skip them here.
          Interrupt is currently not supported and ignored, so we don't need to set it */
    ptDevInstance->ulPhysicalAddress = 0xB0000000U;   //Addr_NX90_idpm_slave_mirror_ext_peri;
    ptDevInstance->ulIrqNumber        = 3;
    ptDevInstance->fIrqEnabled       = 0;
    ptDevInstance->fPCICard          = 0;
    ptDevInstance->fModuleLoad       = 0;

    ptDevInstance->eDeviceType       = eCIFX_DEVICE_DONT_TOUCH;
    ptDevInstance->pfnNotify         = NULL;
    ptDevInstance->pvOSDependent     = ptDevInstance;
    ptDevInstance->ulDPMSize         = 0x8000;        //32K
    ptDevInstance->pbDPM             = (uint8_t*) ptDevInstance->ulPhysicalAddress;
    OS_Strncpy(ptDevInstance->szName, name, sizeof(ptDevInstance->szName));

    /** give netX time to get access to SPM functionality */
    OS_Sleep(1000);

    /** Add the device to the toolkits handled device list */
    lRet = cifXTKitAddDevice(ptDevInstance);

    /** If it succeeded do device tests */
    if(CIFX_NO_ERROR != lRet)
    {
      /** Uninitialize Toolkit, this will remove all handled boards from the toolkit and
            deallocate the device instance */
      cifXTKitDeinit();
    }
  }
  return lRet;
}

static int32_t GetCifxDriverAndRelatedInfo(CIFXHANDLE * cifxDriver, DRIVER_INFORMATION* tDriverInfo)
{
    int32_t    lRet = CIFX_NO_ERROR;
    if (CIFX_NO_ERROR == (lRet = xDriverOpen(cifxDriver)))
    {
        lRet = xDriverGetInformation(*cifxDriver, sizeof(*tDriverInfo), tDriverInfo);
    }
    return lRet;
}

static int32_t ReadBoardInfo( const CIFXHANDLE  hDriver, BOARD_INFORMATION* const ptBoardInfo, const unsigned long ulBoard)
{
  CIFXHANDLE                       hSys         = 0;
  SYSTEM_CHANNEL_SYSTEM_INFO_BLOCK tSysInfo     = ptBoardInfo->tSystemInfo;
  long                             lRet         = 0;      /** Return value for common error codes  */

  if (CIFX_NO_ERROR != (lRet = xDriverEnumBoards(hDriver, ulBoard, sizeof(BOARD_INFORMATION), ptBoardInfo)))
  {
    xDriverClose(hDriver);
    return lRet;
  }

  /* Get Sys-Channel information */
  if(CIFX_NO_ERROR == (lRet = xSysdeviceOpen(hDriver, ptBoardInfo->abBoardName, &hSys)))
  {
      lRet = xSysdeviceInfo(hSys, CIFX_INFO_CMD_SYSTEM_INFO_BLOCK, sizeof(tSysInfo), &tSysInfo);
      xSysdeviceClose(hSys);
  }
  return lRet;
}
