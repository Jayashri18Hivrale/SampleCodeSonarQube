/**************************************************************************************

   Copyright (c) Hilscher GmbH. All Rights Reserved.

 **************************************************************************************

   Filename:
    $Id: TL_rcXPacket.c 195 2013-06-27 10:31:26Z sebastiand $
   Last Modification:
    $Author: sebastiand $
    $Date: 2013-06-27 12:31:26 +0200 (Do, 27. Jun 2013) $
    $Revision: 195 $

   Targets:
     OS-Independent : yes

   Description:
    Implementation of the rcX Packet transfer (datatype = 0x100)

   Changes:

     Version   Date        Author   Description
     ----------------------------------------------------------------------------------
      1         23.02.13    SD      initial version

**************************************************************************************/

/****************************************************************************/
/*! \file TL_rcXPacket.c
    Implementation of the rcX Packet Translation-Layer (datatype = 0x100)   */
/****************************************************************************/

#include <OS_Includes.h>
#include "OS_Dependent.h"
#include "netXTransport.h"
#include "HilTransportLayer.h"
#include "rcX_User.h"
#include "rcX_Public.h"
#include "cifXErrors.h"
#include "cifXAPI.h"
#include "TL_rcXPacket.h"

/*****************************************************************************/
/*! cifX API rcXPacket version information                                   */
/*****************************************************************************/
#define RCXAPI_VERSION_MAJOR 1  /*!< Version major    */
#define RCXAPI_VERSION_MINOR 0  /*!< Version minor    */
#define RCXAPI_VERSION_BUILD 0  /*!< Version build    */
#define RCXAPI_VERSION_REV   0  /*!< Version revision */

/*****************************************************************************/
/*! cifX API rcXPacket definitions                                          */
/*****************************************************************************/
#define RCXPACKET_SYSTEMCHANNEL_INTERNAL   0 /*!< Channel type: 'sysdevice'    */

/*****************************************************************************/
/*! cifX API Marshaller global variables                                     */
/*****************************************************************************/
static NETX_TL_INTERFACE_T s_tRCXPacketLayer;  /*!< TL rcXPacket information */

static int32_t ReadHWIdentify      ( PRCX_PACKET_TL_INFO_T ptTLInfo, PHIL_TRANSPORT_PACKET_T ptSendPacket);
static int32_t ReadChannelFirmware ( PRCX_PACKET_TL_INFO_T ptTLInfo, PHIL_TRANSPORT_PACKET_T ptSendPacket, uint32_t ulAreaIdx, RCX_FW_IDENTIFICATION_T* ptFirmwareInfo);
static void    CreateChannelInfo   ( PRCX_PACKET_TL_INFO_T ptTLInfo, PHIL_TRANSPORT_PACKET_T ptSendPacket, uint32_t ulAreaIdx);
static int32_t ReadSysInfoBlock    ( PRCX_PACKET_TL_INFO_T ptTLInfo, PHIL_TRANSPORT_PACKET_T ptSendPacket, NETX_SYSTEM_INFO_BLOCK* ptSysInfoBlock);
static int32_t GenerateChannelInfo ( PRCX_PACKET_TL_INFO_T ptTLInfo, uint32_t ulChannel, CHANNEL_INFORMATION* ptChannelInfo);

int32_t GenerateChannelInfoByDeviceHandle( CIFXHANDLE hChannel, uint32_t ulChannel, CHANNEL_INFORMATION* ptChannelInfo);
int32_t ReadSysInformation( CIFXHANDLE hSysDevice, SYSTEM_CHANNEL_SYSTEM_INFORMATION* ptSystemInfo);
static int32_t ReadSysStatusBlock          ( CIFXHANDLE hSysdevice, NETX_SYSTEM_STATUS_BLOCK* ptSysStatusBlock);
static int32_t ReadChannelCommonStatusBlock( CIFXHANDLE hChannel,   uint32_t ulCmd,       uint32_t ulOffset, uint32_t ulDataLen, void* pvData);
static int32_t ReadChannelControlBlock     ( CIFXHANDLE hChannel,   uint32_t ulCmd,       uint32_t ulOffset, uint32_t ulDataLen, void* pvData);
static int32_t ReadChannelInfoBlock        ( NXTHANDLE hTransport,  uint32_t ulChannelId, uint32_t ulSize, void* pvData);
static int32_t rcXReset                    ( CIFXHANDLE hDevice,    uint32_t ulResetMode, uint32_t ulTimeout);

static int32_t rcXPacketSendPacket    ( NXTHANDLE hTransport, CIFX_PACKET* ptSendPacket, uint32_t ulTimeout);
static int32_t rcXPacketTransferPacket( NXTHANDLE hTransport, CIFX_PACKET* ptSendPacket, CIFX_PACKET* ptRecvPacket, uint32_t ulTimeout);

/*! **************************************************************************
* Opens the Driver
*   \param phDriver Returned handle to the driver
*   \return CIFX_NO_ERROR on success
******************************************************************************/
int32_t APIENTRY rcXDriverOpen( CIFXHANDLE* hhDriver)
{
  int32_t lRet = CIFX_NO_ERROR;

  UNREFERENCED_PARAMETER(hhDriver);

  /* temporarily */
  /* return Ok as the driver is already opened */
  return lRet;
}

/*! **************************************************************************
* Closes a previously opened driver
*   \param hDriver Driver handle
*   \return CIFX_NO_ERROR on success
******************************************************************************/
int32_t APIENTRY rcXDriverClose(CIFXHANDLE hDriver)
{
  int32_t lRet = CIFX_NO_ERROR;

  UNREFERENCED_PARAMETER(hDriver);

  return lRet;
}

/*! **************************************************************************
* Restart a device
*   \param hDriver      Driver handle, if NULL the driver will be temporarily opened by the API
*   \param szBoardName  Board name
*   \param pvData
*   \return CIFX_NO_ERROR on success
******************************************************************************/
int32_t APIENTRY rcXDriverRestartDevice(CIFXHANDLE hDriver, char* szBoardName, void* pvData)
{
  int32_t lRet = CIFX_NO_ERROR;

  UNREFERENCED_PARAMETER(hDriver);
  UNREFERENCED_PARAMETER(szBoardName);
  UNREFERENCED_PARAMETER(pvData);

  return lRet;
}

/*! **************************************************************************
* Query information about the running device driver
*   \param hDriver      Driver handle, if NULL the driver will be temporarily opened by the API
*   \param ulSize       Buffer size
*   \param pvDriverInfo Returned Driver information
*   \return CIFX_NO_ERROR on success
******************************************************************************/
int32_t APIENTRY rcXDriverGetInformation( CIFXHANDLE hDriver,
                                 uint32_t   ulSize,
                                 void*      pvDriverInfo)
{
  int32_t lRet = CIFX_NO_ERROR;

  UNREFERENCED_PARAMETER(hDriver);
  UNREFERENCED_PARAMETER(ulSize);
  UNREFERENCED_PARAMETER(pvDriverInfo);

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
int32_t APIENTRY rcXDriverEnumBoards( CIFXHANDLE hDriver,
                             uint32_t   ulBoard,
                             uint32_t   ulSize,
                             void*      pvBoardInfo)
{
  int32_t   lRet             = CIFX_INVALID_PARAMETER;
  NXTHANDLE hNetXrcXInstance = hDriver;
  NXTHANDLE hDeviceHandle    = NULL;

  if (NULL != (hDeviceHandle = netXTransportGetDeviceHandleByNumber( hNetXrcXInstance, ulBoard)))
  {
    PRCX_PACKET_TL_INFO_T ptTLInfo;

    if (1 == netXTransportAttachDevice( hDeviceHandle))
    {
      if (NULL != (ptTLInfo = (PRCX_PACKET_TL_INFO_T)netXTransportGetTLDeviceData( hDeviceHandle)))
      {
        BOARD_INFORMATION* ptBoardInfo = pvBoardInfo;

        OS_Memset( ptBoardInfo, 0, sizeof(BOARD_INFORMATION));
        ptBoardInfo->lBoardError          = 0;
        ptBoardInfo->ulBoardID            = 0;
        ptBoardInfo->ulSystemError        = 0;
        ptBoardInfo->ulPhysicalAddress    = 0;
        ptBoardInfo->bIrqEnabled          = 0;
        ptBoardInfo->ulIrqNumber          = 0;
        ptBoardInfo->ulDpmTotalSize       = ptTLInfo->tDevice.tSystemInfo.ulDpmTotalSize;

        netXTransportGetDeviceName( hDeviceHandle, ptBoardInfo->abBoardName, sizeof(ptBoardInfo->abBoardName));
        OS_Memcpy( &ptBoardInfo->tSystemInfo, &ptTLInfo->tDevice.tSystemInfo, sizeof(ptTLInfo->tDevice.tSystemInfo));
        ptBoardInfo->ulChannelCnt = ptTLInfo->tDevice.ulChannelCount;

        if (ulSize>sizeof(BOARD_INFORMATION))
        {
          ulSize = sizeof(BOARD_INFORMATION);
        }
        OS_Memcpy( pvBoardInfo, ptBoardInfo, ulSize);
        lRet = CIFX_NO_ERROR;
      }
      netXTransportDetachDevice( hDeviceHandle);
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
int32_t APIENTRY rcXDriverEnumChannels( CIFXHANDLE hDriver,
                               uint32_t   ulBoard,
                               uint32_t   ulChannel,
                               uint32_t   ulSize,
                               void*      pvChannelInfo)
{
  int32_t   lRet                   = CIFX_NO_ERROR;
  NXTHANDLE hnetXrcXPacketInstance = hDriver;
  NXTHANDLE hDeviceHandle          = NULL;

  if (NULL != (hDeviceHandle = netXTransportGetDeviceHandleByNumber( hnetXrcXPacketInstance, ulBoard)))
  {
    PRCX_PACKET_TL_INFO_T ptTLInfo;

    if (1 == netXTransportAttachDevice( hDeviceHandle))
    {
      if (NULL != (ptTLInfo = (PRCX_PACKET_TL_INFO_T)netXTransportGetTLDeviceData( hDeviceHandle)))
      {
        CHANNEL_INFORMATION tChannelInfo = {{0}};

        ulChannel++; /* skip system channel */
        if (CIFX_NO_ERROR == (lRet = GenerateChannelInfo( ptTLInfo, ulChannel, &tChannelInfo)))
        {
          if (ulSize>sizeof(tChannelInfo))
          {
            ulSize = sizeof(tChannelInfo);
          }
          OS_Memcpy( pvChannelInfo, &tChannelInfo, ulSize);
        }
      } else
      {
        lRet = CIFX_FUNCTION_FAILED;
      }
      netXTransportDetachDevice( hDeviceHandle);
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
int32_t APIENTRY rcXDriverMemoryPointer(CIFXHANDLE hDriver,
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
int32_t APIENTRY rcXDriverGetErrorDescription(int32_t   lError,
                                      char*    szBuffer,
                                      uint32_t ulBufferLen)
{
  int32_t lRet = CIFX_FUNCTION_NOT_AVAILABLE;

  UNREFERENCED_PARAMETER(lError);
  UNREFERENCED_PARAMETER(szBuffer);
  UNREFERENCED_PARAMETER(ulBufferLen);

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
int32_t APIENTRY rcXSysdeviceOpen( CIFXHANDLE hDriver,
                         char*       szBoard,
                         CIFXHANDLE* phSysdevice)
{
  CIFX_TRANSPORT_DEVICE_INST_T* ptnetXTransportDevice = (CIFX_TRANSPORT_DEVICE_INST_T*)hDriver;
  int32_t                       lRet                  = CIFX_INVALID_BOARD;
  PCIFX_CHANNEL_HANDLE_T        ptChannel             = OS_Memalloc(sizeof(*ptChannel));

  /* the board name is abstracted in the given handle hDriver */
  UNREFERENCED_PARAMETER(szBoard);

  if(NULL == ptChannel)
  {
    lRet = CIFX_FUNCTION_FAILED;
  } else
  {
    ptChannel->hSysDevice                 = (void*)netXTransportGetTLDeviceData(ptnetXTransportDevice->hNetXTransportDeviceHandle);
    ptChannel->hNetxTransportDeviceHandle = ptnetXTransportDevice->hNetXTransportDeviceHandle;
    ptChannel->ulChannelNr                = RCX_SYSTEM_CHANNEL;

    *phSysdevice = ptChannel;

    lRet = CIFX_NO_ERROR;
  }

  return lRet;
}

/*! **************************************************************************
* Closed an open System device
*   \param hSysdevice  Handle to the System device to close
*   \return CIFX_NO_ERROR on success
******************************************************************************/
int32_t APIENTRY rcXSysdeviceClose(CIFXHANDLE hSysdevice)
{
  PCIFX_CHANNEL_HANDLE_T ptChannel = (PCIFX_CHANNEL_HANDLE_T)hSysdevice;
  int32_t                lRet      = CIFX_NO_ERROR;

  if (ptChannel->hSysDevice != (void*)netXTransportGetTLDeviceData( ptChannel->hNetxTransportDeviceHandle))
  {
    lRet = CIFX_INVALID_HANDLE;
  } else
  {
    OS_Memfree(ptChannel);
    lRet = CIFX_NO_ERROR;
  }
  return lRet;
}

/*! **************************************************************************
* Gets the Mailbox state of an open system device
*   \param hSysdevice      Handle to the System device
*   \param pulRecvPktCount Number of packets in receive mailbox
*   \param pulSendPktCount Number of packets the application is able to send at once
*   \return CIFX_NO_ERROR on success
******************************************************************************/
int32_t APIENTRY rcXSysdeviceGetMBXState( CIFXHANDLE hSysdevice,
                                          uint32_t*  pulRecvPktCount,
                                          uint32_t*  pulSendPktCount)
{
  PCIFX_CHANNEL_HANDLE_T ptChannel    = (PCIFX_CHANNEL_HANDLE_T)hSysdevice;
  PRCX_PACKET_TL_INFO_T  ptPacketData = (PRCX_PACKET_TL_INFO_T)netXTransportGetTLDeviceData( ptChannel->hNetxTransportDeviceHandle);
  int32_t                lRet         = CIFX_INVALID_PARAMETER;

  if ((pulRecvPktCount != NULL) && (pulSendPktCount != NULL))
  {
    PHIL_TRANSPORT_PACKET_T ptPacket;

    lRet = CIFX_NO_ERROR;

    *pulRecvPktCount = 0;
    TAILQ_FOREACH( ptPacket, &ptPacketData->tDevice.aptChannels[0]->tRecvPackets, tList)
    {
      (*pulRecvPktCount)++;
    }
    *pulSendPktCount = 0;
  }

  return lRet;
}

/*! **************************************************************************
* Inserts a packet into the System Mailbox
*   \param hSysdevice      Handle to the System device
*   \param ptSendPkt       Packet to send to device
*   \param ulTimeout       maximum time to wait for packet to be accepted by device (in ms)
*   \return CIFX_NO_ERROR on success
******************************************************************************/
int32_t APIENTRY rcXSysdevicePutPacket(CIFXHANDLE   hSysdevice,
                                     CIFX_PACKET* ptSendPkt,
                                     uint32_t     ulTimeout)
{
  PCIFX_CHANNEL_HANDLE_T ptChannel = (PCIFX_CHANNEL_HANDLE_T)hSysdevice;

  return rcXPacketSendPacket(((PRCX_PACKET_TL_INFO_T)(netXTransportGetTLDeviceData( ptChannel->hNetxTransportDeviceHandle)))->hTransport, ptSendPkt, ulTimeout);
}

/*! **************************************************************************
* Retrieves a packet from the System Mailbox
*   \param hSysdevice      Handle to the System device
*   \param ulSize          Size of the buffer to retrieve the packet
*   \param ptRecvPkt       Pointer to buffer for received packet
*   \param ulTimeout       maximum time to wait for packet to be delivered by device (in ms)
*   \return CIFX_NO_ERROR on success
******************************************************************************/
int32_t APIENTRY rcXSysdeviceGetPacket(CIFXHANDLE   hSysdevice,
                              uint32_t     ulSize,
                              CIFX_PACKET* ptRecvPkt,
                              uint32_t     ulTimeout)
{
  PCIFX_CHANNEL_HANDLE_T ptChannel    = (PCIFX_CHANNEL_HANDLE_T)hSysdevice;
  PRCX_PACKET_TL_INFO_T  ptPacketData = (PRCX_PACKET_TL_INFO_T)netXTransportGetTLDeviceData( ptChannel->hNetxTransportDeviceHandle);
  PRCX_PACKET_CHANNEL_T  ptSyschannel = ptPacketData->tDevice.aptChannels[0];
  int32_t                lRet         = CIFX_NO_ERROR;

  if(OS_SEM_TIMEOUT == OS_WaitSemaphore(ptSyschannel->hRecvSemaphore, ulTimeout))
  {
    lRet = CIFX_DEV_GET_NO_PACKET;
  } else
  {
    PHIL_TRANSPORT_PACKET_T ptTransportPacket = TAILQ_FIRST(&ptSyschannel->tRecvPackets);
    uint32_t                ulCopyCount       = ptTransportPacket->ptHeader->ulLength;

    lRet = CIFX_NO_ERROR;

    TAILQ_REMOVE(&ptSyschannel->tRecvPackets, ptTransportPacket, tList);

    if(ulCopyCount > ulSize)
    {
      lRet = CIFX_BUFFER_TOO_SHORT;
      ulCopyCount = ulSize;
    }
    OS_Memcpy( ptRecvPkt, ptTransportPacket->pbData, ulCopyCount);

    HilTransportFreePacket( ptTransportPacket);
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
int32_t APIENTRY rcXSysdeviceInfo(CIFXHANDLE hSysdevice,
                                  uint32_t   ulCmd,
                                  uint32_t   ulSize,
                                  void*      pvInfo)
{
  PCIFX_CHANNEL_HANDLE_T ptChannel    = (PCIFX_CHANNEL_HANDLE_T)hSysdevice;
  PRCX_PACKET_TL_INFO_T  ptPacketData = (PRCX_PACKET_TL_INFO_T)netXTransportGetTLDeviceData( ptChannel->hNetxTransportDeviceHandle);

  int32_t lRet = CIFX_NO_ERROR;

  switch(ulCmd)
  {
    case CIFX_INFO_CMD_SYSTEM_INFORMATION:
      if (ulSize < sizeof(SYSTEM_CHANNEL_SYSTEM_INFORMATION))
      {
        lRet = CIFX_INVALID_BUFFERSIZE;
      } else
      {
        SYSTEM_CHANNEL_SYSTEM_INFORMATION* ptSystemInfo = (SYSTEM_CHANNEL_SYSTEM_INFORMATION*)pvInfo;
        lRet = ReadSysInformation( hSysdevice, ptSystemInfo);
        lRet = CIFX_NO_ERROR;
      }
      break;

    case CIFX_INFO_CMD_SYSTEM_INFO_BLOCK:
      if (ulSize < sizeof(ptPacketData->tDevice.tSystemInfo))
      {
        lRet = CIFX_INVALID_BUFFERSIZE;
      } else
      {
        ulSize = sizeof(ptPacketData->tDevice.tSystemInfo);

        OS_Memcpy( pvInfo, &ptPacketData->tDevice.tSystemInfo, ulSize);
      }
      break;

    case CIFX_INFO_CMD_SYSTEM_CHANNEL_BLOCK:
      if( ulSize < sizeof(SYSTEM_CHANNEL_CHANNEL_INFO_BLOCK))
      {
        lRet = CIFX_INVALID_BUFFERSIZE;
      } else
      {
        SYSTEM_CHANNEL_CHANNEL_INFO_BLOCK tSystemChannelInfoBlock = {{{0}}};
        uint32_t                          ulChannelId             = 0;

        for( ulChannelId = 0; (ulChannelId < CIFX_MAX_NUMBER_OF_CHANNEL_DEFINITION); ulChannelId++)
        {
          lRet = ReadChannelInfoBlock( ((PRCX_PACKET_TL_INFO_T)(netXTransportGetTLDeviceData( ptChannel->hNetxTransportDeviceHandle)))->hTransport, ulChannelId, sizeof(tSystemChannelInfoBlock.abInfoBlock[ulChannelId]), &tSystemChannelInfoBlock.abInfoBlock[ulChannelId]);
          if (lRet == RCX_E_UNKNOWN_COMMAND)
          {
            lRet = CIFX_NO_ERROR;
          }
        }
        OS_Memcpy(pvInfo, &tSystemChannelInfoBlock, sizeof(tSystemChannelInfoBlock));
        lRet = CIFX_NO_ERROR;
      }
      break;

    case CIFX_INFO_CMD_SYSTEM_CONTROL_BLOCK:
      if (ulSize < sizeof(NETX_SYSTEM_CONTROL_BLOCK))
      {
        lRet = CIFX_INVALID_BUFFERSIZE;
      } else
      {
        NETX_SYSTEM_CONTROL_BLOCK* ptSysContolBlock = (NETX_SYSTEM_CONTROL_BLOCK*)pvInfo;
        lRet = ReadChannelControlBlock( hSysdevice, CIFX_CMD_READ_DATA, 0, sizeof(NETX_SYSTEM_CONTROL_BLOCK), (void*)ptSysContolBlock);
        lRet = CIFX_NO_ERROR;
      }
      break;

    case CIFX_INFO_CMD_SYSTEM_STATUS_BLOCK:
      if (ulSize < sizeof(NETX_SYSTEM_STATUS_BLOCK))
      {
        lRet = CIFX_INVALID_BUFFERSIZE;
      } else
      {
        NETX_SYSTEM_STATUS_BLOCK* ptSysStatusBlock = (NETX_SYSTEM_STATUS_BLOCK*)pvInfo;
        lRet = ReadSysStatusBlock( hSysdevice, ptSysStatusBlock);
      }
      break;

    default:
      lRet = CIFX_INVALID_COMMAND;
      break;
  }
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
int32_t APIENTRY rcXSysdeviceFindFirstFile( CIFXHANDLE            hSysdevice,
                                            uint32_t              ulChannel,
                                            CIFX_DIRECTORYENTRY*  ptDirectoryInfo,
                                            PFN_RECV_PKT_CALLBACK pfnRecvPktCallback,
                                            void*                 pvUser)
{
  PCIFX_CHANNEL_HANDLE_T ptChannel    = (PCIFX_CHANNEL_HANDLE_T)hSysdevice;
  int32_t                lRet         = CIFX_NO_ERROR;
  CIFX_PACKET            tSendPkt     = {{0}};
  CIFX_PACKET            tRecvPacket  = {{0}};
  RCX_DIR_LIST_REQ_T*    ptDirListReq = (RCX_DIR_LIST_REQ_T*)&tSendPkt;

  /* NOTE: We ignore the RecvCallback functions pointer, cause we don't receive a async packet */
  UNREFERENCED_PARAMETER(pfnRecvPktCallback);
  UNREFERENCED_PARAMETER(pvUser);

  /* Check size of filename (ptDirectoryInfo->szfilename) */
  if( OS_Strlen(ptDirectoryInfo->szFilename) > 0)
  {
    uint16_t usDirNameLength = (uint16_t)(OS_Strlen(ptDirectoryInfo->szFilename) + 1);

    ptDirListReq->tData.usDirNameLength = usDirNameLength;
    OS_Strncpy((char*)((&ptDirListReq->tData) + 1),
                usDirNameLength,
                ptDirectoryInfo->szFilename,
                usDirNameLength);
  }

  /* create request packet */
  ptDirListReq->tHead.ulDest      = RCX_PACKET_DEST_SYSTEM;
  ptDirListReq->tHead.ulSrc       = 0;
  ptDirListReq->tHead.ulCmd       = RCX_DIR_LIST_REQ;
  ptDirListReq->tHead.ulLen       = (sizeof(ptDirListReq->tData) + ptDirListReq->tData.usDirNameLength) ;
  ptDirListReq->tData.ulChannelNo = (uint32_t) ulChannel;

  lRet = rcXPacketTransferPacket( ((PRCX_PACKET_TL_INFO_T)(netXTransportGetTLDeviceData( ptChannel->hNetxTransportDeviceHandle)))->hTransport, &tSendPkt, &tRecvPacket, TRANSPORT_TO_TRANSFER);
  if(CIFX_NO_ERROR == lRet)
  {
    RCX_DIR_LIST_CNF_T* ptDirListCnf = (RCX_DIR_LIST_CNF_T*)&tRecvPacket;

    if (~ptDirListCnf->tHead.ulCmd&RCX_MSK_PACKET_ANSWER)
    {
      lRet = CIFX_TRANSPORT_INVALID_RESPONSE;
    } else if( RCX_S_OK == (lRet = ptDirListCnf->tHead.ulSta) )
    {
      /* TODO: Store handle for directory list, which needs to be set by firmware */
      ptDirectoryInfo->hList = (void*)1;
      OS_Strncpy( ptDirectoryInfo->szFilename,
              sizeof(ptDirectoryInfo->szFilename),
              ptDirListCnf->tData.szName,
              sizeof(ptDirectoryInfo->szFilename));

      ptDirectoryInfo->bFiletype  = ptDirListCnf->tData.bFileType;
      ptDirectoryInfo->ulFilesize = ptDirListCnf->tData.ulFileSize;
    }
  }
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
int32_t APIENTRY rcXSysdeviceFindNextFile( CIFXHANDLE            hSysdevice,
                                           uint32_t              ulChannel,
                                           CIFX_DIRECTORYENTRY*  ptDirectoryInfo,
                                           PFN_RECV_PKT_CALLBACK pfnRecvPktCallback,
                                           void*                 pvUser)
{
  PCIFX_CHANNEL_HANDLE_T  ptChannel    = (PCIFX_CHANNEL_HANDLE_T)hSysdevice;
  int32_t                 lRet         = CIFX_NO_ERROR;
  CIFX_PACKET             tSendPkt     = {{0}};
  CIFX_PACKET             tRecvPacket  = {{0}};
  RCX_DIR_LIST_REQ_T*     ptDirListReq = (RCX_DIR_LIST_REQ_T*)&tSendPkt;
  uint16_t                usDirNameLen = (uint16_t)(OS_Strlen(ptDirectoryInfo->szFilename) + 1);

  UNREFERENCED_PARAMETER(pfnRecvPktCallback);
  UNREFERENCED_PARAMETER(pvUser);

  /* NOW, We have a rcX packet and on this time only one connection via one device */
  /* NOTE: We ignore the RecvCallback functions pointer, cause we don't receive a async packet */
  ptDirListReq->tData.usDirNameLength = usDirNameLen;
  OS_Strncpy( (char*)((&ptDirListReq->tData) + 1),
           usDirNameLen,
           ptDirectoryInfo->szFilename,
           usDirNameLen);

  ptDirListReq->tHead.ulDest      = RCX_PACKET_DEST_SYSTEM;
  ptDirListReq->tHead.ulSrc       = (uint32_t)PTR2LONG(hSysdevice);
  ptDirListReq->tHead.ulCmd       = RCX_DIR_LIST_REQ;
  ptDirListReq->tHead.ulLen       = (sizeof(ptDirListReq->tData) + usDirNameLen);
  ptDirListReq->tHead.ulExt       = RCX_PACKET_SEQ_MIDDLE;
  ptDirListReq->tData.ulChannelNo = ulChannel;

  lRet = rcXPacketTransferPacket(((PRCX_PACKET_TL_INFO_T)(netXTransportGetTLDeviceData( ptChannel->hNetxTransportDeviceHandle)))->hTransport, &tSendPkt, &tRecvPacket, TRANSPORT_TO_TRANSFER);

  if(CIFX_NO_ERROR == lRet)
  {
    RCX_DIR_LIST_CNF_T* ptDirListCnf = (RCX_DIR_LIST_CNF_T*)&tRecvPacket;

    if (~ptDirListCnf->tHead.ulCmd&RCX_MSK_PACKET_ANSWER)
    {
      lRet = CIFX_TRANSPORT_INVALID_RESPONSE;
    } else if( RCX_S_OK == (lRet = ptDirListCnf->tHead.ulSta) )
    {
      if(( ptDirListCnf->tHead.ulExt & RCX_PACKET_SEQ_MASK) == RCX_PACKET_SEQ_LAST)
      {
        /* this is the last packet */
        lRet = CIFX_NO_MORE_ENTRIES;

        /* invalidate handle */
        ptDirectoryInfo->hList = (void*)0;
      } else
      {
        OS_Strncpy(ptDirectoryInfo->szFilename,
                sizeof(ptDirListCnf->tData.szName),
                ptDirListCnf->tData.szName,
                sizeof(ptDirListCnf->tData.szName));

        ptDirectoryInfo->bFiletype  = ptDirListCnf->tData.bFileType;
        ptDirectoryInfo->ulFilesize = ptDirListCnf->tData.ulFileSize;
      }
    }
  }
  return lRet;
}

/*! **************************************************************************
* Download a file (Firmware, Configuratio, etc) to the device
*   \param hSysdevice           Handle to the system device
*   \param ulChannel            Channel number to load the file to
*   \param ulMode               Download mode (DOWNLOAD_MODE_FIRMWARE, etc)
*   \param szFileName           Name of the file
*   \param pabFileData          Pointer to the file data
*   \param ulFileSize           Length of the file data
*   \param ptcifXTransportInst  Pointer to cifX Translation-Layer instance
*   \param pfnCallback          Callback for progress indication (NULL for no callback)
*   \param pfnRecvPktCallback   Callback pointer for unhandled receive packets
*   \param pvUser               User parameter on callback.
*   \return CIFX_NO_ERROR on success
******************************************************************************/
int32_t APIENTRY rcXSysdeviceDownload(CIFXHANDLE                 hSysdevice,
                                      uint32_t                   ulChannel,
                                      uint32_t                   ulMode,
                                      char*                      szFileName,
                                      uint8_t*                   pabFileData,
                                      uint32_t                   ulFileSize,
                                      CIFX_TRANSPORT_DEVICE_INST_T* ptcifXTransportInst,
                                      PFN_PROGRESS_CALLBACK      pfnCallback,
                                      PFN_RECV_PKT_CALLBACK      pfnRecvPktCallback,
                                      void*                      pvUser)
{
  int32_t  lRet           = CIFX_NO_ERROR;
  uint32_t ulTransferType = 0;

  switch(ulMode)
  {
    case DOWNLOAD_MODE_FIRMWARE:
      if( CIFX_NO_ERROR != (lRet = DEV_GetFWTransferTypeFromFileName( szFileName, &ulTransferType)))
        return lRet;

      DEV_RemoveChannelFiles( hSysdevice,
                              ulChannel,
                              ptcifXTransportInst,
                              pfnRecvPktCallback,
                              pvUser,
                              NULL);

      break;

    case DOWNLOAD_MODE_CONFIG:
    case DOWNLOAD_MODE_FILE:
      ulTransferType = RCX_FILE_XFER_FILE;
      break;

    case DOWNLOAD_MODE_LICENSECODE:
      ulTransferType = RCX_FILE_XFER_LICENSE_CODE;
      break;

    default:
      return CIFX_INVALID_PARAMETER;
  }
  lRet = DEV_DownloadFile(hSysdevice,
                          ulChannel,
                          CIFX_MAX_PACKET_SIZE,
                          ulTransferType,
                          szFileName,
                          ulFileSize,
                          pabFileData,
                          ptcifXTransportInst,
                          pfnCallback,
                          pfnRecvPktCallback,
                          pvUser);

  return lRet;
}

/*****************************************************************************/
/*! xSysdeviceUpload upload files from target
*   \param hSysdevice           Handle, of a system channel
*   \param ulChannel            Channel number
*   \param ulMode               Download mode
*   \param szFileName           File name
*   \param pabFileData          File data buffer
*   \param pulFileSize          Size of data buffer, returns file size
*   \param ptcifXTransportInst  Pointer to cifX Translation-Layer instance
*   \param pfnCallback          Progress callback
*   \param pfnRecvPktCallback   Receive packet callback
*   \param pvUser               User callback
*   \return CIFX_NO_ERROR on success
******************************************************************************/
int32_t APIENTRY rcXSysdeviceUpload(CIFXHANDLE                    hSysdevice,
                                    uint32_t                      ulChannel,
                                    uint32_t                      ulMode,
                                    char*                         szFileName,
                                    uint8_t*                      pabFileData,
                                    uint32_t*                     pulFileSize,
                                    CIFX_TRANSPORT_DEVICE_INST_T* ptcifXTransportInst,
                                    PFN_PROGRESS_CALLBACK         pfnCallback,
                                    PFN_RECV_PKT_CALLBACK         pfnRecvPktCallback,
                                    void*                         pvUser)
{
  int32_t  lRet           = CIFX_NO_ERROR;
  uint32_t ulTransferType = 0;

  switch(ulMode)
  {
  case DOWNLOAD_MODE_FIRMWARE:
    if( CIFX_NO_ERROR != (lRet = DEV_GetFWTransferTypeFromFileName( szFileName, &ulTransferType)))
      return lRet;
    break;

  case DOWNLOAD_MODE_CONFIG:
  case DOWNLOAD_MODE_FILE:
    ulTransferType = RCX_FILE_XFER_FILE;
    break;

  default:
    return CIFX_INVALID_PARAMETER;
  }

  lRet =  DEV_UploadFile( hSysdevice,
                          ulChannel,
                          CIFX_MAX_PACKET_SIZE,
                          ulTransferType,
                          szFileName,
                          pulFileSize,
                          pabFileData,
                          ptcifXTransportInst,
                          pfnCallback,
                          pfnRecvPktCallback,
                          pvUser);
  return lRet;
}

/*! **************************************************************************
* Reset the whole device (coldstart)
*   \param hSysdevice   Sysdevice handle acquired by xSysdeviceOpen
*   \param ulTimeout    Time in ms to wait for the reset
*   \return CIFX_NO_ERROR on success
******************************************************************************/
int32_t APIENTRY rcXSysdeviceReset( CIFXHANDLE hSysdevice,
                                    uint32_t   ulTimeout)
{
  int32_t lRet = CIFX_NO_ERROR;

  lRet = rcXReset(hSysdevice, CIFX_SYSTEMSTART, ulTimeout);

  return lRet;
}

/***************************************************************************
*
*
*   CHANNEL FUNCTIONS...................
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
int32_t APIENTRY rcXChannelOpen(CIFXHANDLE  hDriver,
                                char*       szBoard,
                                uint32_t    ulChannel,
                                CIFXHANDLE* phChannel)
{
  int32_t                       lRet                       = CIFX_INVALID_BOARD;
  CIFX_TRANSPORT_DEVICE_INST_T* ptnetXTransportDevice      = (CIFX_TRANSPORT_DEVICE_INST_T*)hDriver;
  NXTHANDLE                     hNetxTransportDeviceHandle = ptnetXTransportDevice->hNetXTransportDeviceHandle;
  PCIFX_CHANNEL_HANDLE_T        ptChannel                  = OS_Memalloc(sizeof(*ptChannel));

  /* the board name is abstracted in the given handle hDriver */
  UNREFERENCED_PARAMETER(szBoard);

  if(NULL == ptChannel)
  {
    lRet = CIFX_FUNCTION_FAILED;
  } else
  {
    ptChannel->hChannel    = netXTransportGetTLDeviceData( hNetxTransportDeviceHandle);
    ptChannel->hNetxTransportDeviceHandle = hNetxTransportDeviceHandle;
    ptChannel->ulChannelNr = ulChannel++;   /* skip system channel */
    *phChannel             = ptChannel;

    lRet = CIFX_NO_ERROR;
  }
  return lRet;
}

/*! **************************************************************************
* Closes an open channel
*   \param hChannel Channel handle acquired by xChannelOpen
*   \return CIFX_NO_ERROR on success
******************************************************************************/
int32_t APIENTRY rcXChannelClose(CIFXHANDLE hChannel)
{
  PCIFX_CHANNEL_HANDLE_T ptChannel = (PCIFX_CHANNEL_HANDLE_T)hChannel;
  int32_t                lRet      = CIFX_NO_ERROR;

  if (ptChannel->hChannel != (void*)netXTransportGetTLDeviceData( ptChannel->hNetxTransportDeviceHandle))
  {
    lRet = CIFX_INVALID_HANDLE;
  } else
  {
    OS_Memfree(ptChannel);
    lRet = CIFX_NO_ERROR;
  }
  return lRet;
}

/*! **************************************************************************
* Returns the Mailbox state from a specific channel
*   \param hChannel         Channel handle acquired by xChannelOpen
*   \param pulRecvPktCount  Number of Messages waiting in receive mailbox
*   \param pulSendMbxState  State of the Send Mailbox
*   \return CIFX_NO_ERROR on success
******************************************************************************/
int32_t APIENTRY rcXChannelGetMBXState(CIFXHANDLE hChannel,
                                        uint32_t* pulRecvPktCount,
                                        uint32_t* pulSendMbxState)
{
  PCIFX_CHANNEL_HANDLE_T ptChannel    = (PCIFX_CHANNEL_HANDLE_T)hChannel;
  PRCX_PACKET_TL_INFO_T  ptPacketData = (PRCX_PACKET_TL_INFO_T)netXTransportGetTLDeviceData( ptChannel->hNetxTransportDeviceHandle);
  int32_t                lRet         = CIFX_INVALID_PARAMETER;

  if ((pulRecvPktCount != NULL) && (pulSendMbxState != NULL))
  {
    PHIL_TRANSPORT_PACKET_T ptPacket;

    lRet = CIFX_NO_ERROR;

    *pulRecvPktCount = 0;
    TAILQ_FOREACH( ptPacket, &ptPacketData->tDevice.aptChannels[ptChannel->ulChannelNr]->tRecvPackets, tList)
    {
      (*pulRecvPktCount)++;
    }
    *pulSendMbxState = 0;
  }

  return lRet;
}

/*! **************************************************************************
* Inserts a packet into a channel mailbox
*   \param hChannel   Channel handle acquired by xChannelOpen
*   \param ptSendPkt  Packet to send to channel
*   \param ulTimeout  Time in ms to wait for card to accept the packet
*   \return CIFX_NO_ERROR on success
******************************************************************************/
int32_t APIENTRY rcXChannelPutPacket(CIFXHANDLE    hChannel,
                                      CIFX_PACKET* ptSendPkt,
                                      uint32_t     ulTimeout)
{
  PCIFX_CHANNEL_HANDLE_T  ptChannel = (PCIFX_CHANNEL_HANDLE_T)hChannel;

  return rcXPacketSendPacket( ((PRCX_PACKET_TL_INFO_T)(netXTransportGetTLDeviceData( ptChannel->hNetxTransportDeviceHandle)))->hTransport, ptSendPkt, ulTimeout);
}

/*! **************************************************************************
* Gets a packet from a channel mailbox
*   \param hChannel   Channel handle acquired by xChannelOpen
*   \param ulSize     Size of returned packet
*   \param ptRecvPkt  Returned packet
*   \param ulTimeout  Time in ms to wait for available packets
*   \return CIFX_NO_ERROR on success
******************************************************************************/
int32_t APIENTRY rcXChannelGetPacket(CIFXHANDLE    hChannel,
                                      uint32_t     ulSize,
                                      CIFX_PACKET* ptRecvPkt,
                                      uint32_t     ulTimeout)
{
  PCIFX_CHANNEL_HANDLE_T ptChannel    = (PCIFX_CHANNEL_HANDLE_T)hChannel;
  PRCX_PACKET_TL_INFO_T  ptPacketData = (PRCX_PACKET_TL_INFO_T)netXTransportGetTLDeviceData( ptChannel->hNetxTransportDeviceHandle);/*ptChannel->ptDevice->pvTLDeviceData;*/
  PRCX_PACKET_CHANNEL_T  ptCurChannel = ptPacketData->tDevice.aptChannels[ptChannel->ulChannelNr];
  int32_t                lRet         = CIFX_NO_ERROR;

  if(OS_SEM_TIMEOUT == OS_WaitSemaphore(ptCurChannel->hRecvSemaphore, ulTimeout))
  {
    lRet = CIFX_DEV_GET_NO_PACKET;
  } else
  {
    PHIL_TRANSPORT_PACKET_T ptTransportPacket = TAILQ_FIRST(&ptCurChannel->tRecvPackets);
    uint32_t                ulCopyCount       = ptTransportPacket->ptHeader->ulLength;

    lRet = CIFX_NO_ERROR;

    TAILQ_REMOVE(&ptCurChannel->tRecvPackets, ptTransportPacket, tList);

    if(ulCopyCount > ulSize)
    {
      lRet = CIFX_BUFFER_TOO_SHORT;
      ulCopyCount = ulSize;
    }
    OS_Memcpy( ptRecvPkt, ptTransportPacket->pbData, ulCopyCount);

    HilTransportFreePacket( ptTransportPacket);
  }
  return lRet;
}

/*! **************************************************************************
* Read back a packet from a channel send mailbox
*   \param hChannel   Channel handle acquired by xChannelOpen
*   \param ulSize     Size of returned packet
*   \param ptRecvPkt  Returned packet
*   \return CIFX_NO_ERROR on success
******************************************************************************/
int32_t APIENTRY rcXChannelGetSendPacket( CIFXHANDLE   hChannel,
                                          uint32_t     ulSize,
                                          CIFX_PACKET* ptRecvPkt)
{
  int32_t lRet = CIFX_FUNCTION_NOT_AVAILABLE;

  UNREFERENCED_PARAMETER( hChannel);
  UNREFERENCED_PARAMETER( ulSize);
  UNREFERENCED_PARAMETER( ptRecvPkt);

  return lRet;
}

/*! **************************************************************************
* Returns Channel specific information (same information as from Channel enumeration)
*   \param hChannel       Channel handle acquired by xChannelOpen
*   \param ulSize         Size of the user buffer
*   \param pvChannelInfo  Returned channel specific information
*   \return CIFX_NO_ERROR on success
******************************************************************************/
int32_t APIENTRY rcXChannelInfo(CIFXHANDLE hChannel,
                                 uint32_t  ulSize,
                                 void*     pvChannelInfo)
{
  int32_t                lRet         = CIFX_NO_ERROR;
  PCIFX_CHANNEL_HANDLE_T ptChannel    = (PCIFX_CHANNEL_HANDLE_T)hChannel;
  CHANNEL_INFORMATION    tChannelInfo = {{0}};

  if (CIFX_NO_ERROR == GenerateChannelInfoByDeviceHandle( hChannel, ptChannel->ulChannelNr, &tChannelInfo))
  {
    if (ulSize>sizeof(CHANNEL_INFORMATION))
      ulSize = sizeof(CHANNEL_INFORMATION);

    OS_Memcpy( pvChannelInfo, &tChannelInfo, ulSize);
  }

  return lRet;
}

/*! **************************************************************************
* Trigger the channel watchdog
*   \param hChannel    Channel handle acquired by xChannelOpen
*   \param ulCmd       Watchdog command
*   \param pulTrigger  Actual trigger value from hardware
*   \return CIFX_NO_ERROR on success
******************************************************************************/
int32_t APIENTRY rcXChannelWatchdog(CIFXHANDLE hChannel,
                                    uint32_t   ulCmd,
                                    uint32_t*  pulTrigger)
{
  int32_t lRet = CIFX_FUNCTION_NOT_AVAILABLE;

  UNREFERENCED_PARAMETER( hChannel);
  UNREFERENCED_PARAMETER( ulCmd);
  UNREFERENCED_PARAMETER( pulTrigger);

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
int32_t APIENTRY rcXChannelHostState(CIFXHANDLE hChannel,
                                     uint32_t   ulCmd,
                                     uint32_t*  pulState,
                                     uint32_t   ulTimeout)
{
  int32_t lRet = CIFX_FUNCTION_NOT_AVAILABLE;

  UNREFERENCED_PARAMETER( hChannel);
  UNREFERENCED_PARAMETER( ulCmd);
  UNREFERENCED_PARAMETER( pulState);
  UNREFERENCED_PARAMETER( ulTimeout);

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
int32_t APIENTRY rcXChannelConfigLock(CIFXHANDLE hChannel,
                                      uint32_t   ulCmd,
                                      uint32_t*  pulState,
                                      uint32_t   ulTimeout)
{
  int32_t lRet = CIFX_FUNCTION_NOT_AVAILABLE;

  UNREFERENCED_PARAMETER( hChannel);
  UNREFERENCED_PARAMETER( ulCmd);
  UNREFERENCED_PARAMETER( pulState);
  UNREFERENCED_PARAMETER( ulTimeout);

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
int32_t APIENTRY rcXChannelBusState(CIFXHANDLE hChannel,
                                    uint32_t   ulCmd,
                                    uint32_t*  pulState,
                                    uint32_t   ulTimeout)
{
  int32_t                lRet         = CIFX_NO_ERROR;
  PCIFX_CHANNEL_HANDLE_T ptChannel    = (PCIFX_CHANNEL_HANDLE_T)hChannel;

  switch(ulCmd)
  {
    case CIFX_BUS_STATE_GETSTATE:
      break;

    case CIFX_BUS_STATE_ON:
    case CIFX_BUS_STATE_OFF:
    {
      CIFX_PACKET                tSendPacket           = {{0}};
      CIFX_PACKET                tRecvPacket           = {{0}};
      RCX_START_STOP_COMM_REQ_T* ptRcxStartStopCommReq = (RCX_START_STOP_COMM_REQ_T*)&tSendPacket;

      ptRcxStartStopCommReq->tHead.ulSrc    = 0;
      ptRcxStartStopCommReq->tHead.ulDest   = RCX_PKT_COMM_CHANNEL_TOKEN;
      ptRcxStartStopCommReq->tHead.ulCmd    = RCX_START_STOP_COMM_REQ;
      ptRcxStartStopCommReq->tHead.ulLen    = sizeof(ptRcxStartStopCommReq->tData);
      ptRcxStartStopCommReq->tData.ulParam  = (ulCmd == CIFX_BUS_STATE_ON)?1:2;

      lRet = rcXPacketTransferPacket(((PRCX_PACKET_TL_INFO_T)(netXTransportGetTLDeviceData( ptChannel->hNetxTransportDeviceHandle)))->hTransport, &tSendPacket, &tRecvPacket, ulTimeout);

      if(CIFX_NO_ERROR == lRet)
      {
        RCX_START_STOP_COMM_CNF_T* ptCnf = (RCX_START_STOP_COMM_CNF_T*)&tRecvPacket;

        if (~ptCnf->tHead.ulCmd&RCX_MSK_PACKET_ANSWER)
        {
          lRet = CIFX_TRANSPORT_INVALID_RESPONSE;
        } else if(CIFX_NO_ERROR == (lRet = ptCnf->tHead.ulSta))
        {
          *pulState = ulCmd;
        }
      }
    }
    break;

  default:
    lRet = CIFX_INVALID_COMMAND;
    break;
  }

  /* Read actual BUS state */
  if (CIFX_NO_ERROR == lRet)
  {
    NETX_COMMON_STATUS_BLOCK tCommonStatusBlock = {0};
    if (CIFX_NO_ERROR == ReadChannelCommonStatusBlock( hChannel, CIFX_CMD_READ_DATA, 0, sizeof(tCommonStatusBlock), (void*)&tCommonStatusBlock))
    {
      *pulState = (tCommonStatusBlock.ulCommunicationCOS&RCX_COMM_COS_BUS_ON)?CIFX_BUS_STATE_ON:CIFX_BUS_STATE_OFF;
    }
  }
  return lRet;
}

/*! **************************************************************************
* Set/Get DMA state (not supported)
*   \param hChannel   Channel handle acquired by xChannelOpen
*   \param ulCmd      Function command
*   \param pulState   Actual state returned by the get command
*   \return CIFX_NO_ERROR on success
******************************************************************************/
int32_t APIENTRY rcXChannelDMAState(CIFXHANDLE hChannel, uint32_t ulCmd, uint32_t* pulState)
{
  int32_t lRet = CIFX_FUNCTION_NOT_AVAILABLE;

  UNREFERENCED_PARAMETER(hChannel);
  UNREFERENCED_PARAMETER(ulCmd);
  UNREFERENCED_PARAMETER(pulState);

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
int32_t APIENTRY rcXChannelRegisterNotification(CIFXHANDLE  hChannel, uint32_t ulNotification, PFN_NOTIFY_CALLBACK  pfnCallback, void* pvUser)
{
  int32_t lRet = CIFX_FUNCTION_NOT_AVAILABLE;

  UNREFERENCED_PARAMETER(hChannel);
  UNREFERENCED_PARAMETER(ulNotification);
  UNREFERENCED_PARAMETER(pfnCallback);
  UNREFERENCED_PARAMETER(pvUser);

  return lRet;
}

/*****************************************************************************/
/*! Unregister notification callbacks (not available on netX transport)
*   \param hChannel         Channel handle
*   \param ulNotification   Notification command
*   \return CIFX_FUNCTION_NOT_AVAILABLE                                      */
/*****************************************************************************/
int32_t APIENTRY rcXChannelUnregisterNotification( CIFXHANDLE  hChannel, uint32_t ulNotification)
{
  int32_t lRet = CIFX_FUNCTION_NOT_AVAILABLE;

  UNREFERENCED_PARAMETER(hChannel);
  UNREFERENCED_PARAMETER(ulNotification);

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
int32_t APIENTRY rcXChannelSyncState(CIFXHANDLE  hChannel, uint32_t ulCmd, uint32_t ulTimeout, uint32_t* pulErrorCount)
{
  int32_t lRet = CIFX_FUNCTION_NOT_AVAILABLE;

  UNREFERENCED_PARAMETER(hChannel);
  UNREFERENCED_PARAMETER(ulCmd);
  UNREFERENCED_PARAMETER(ulTimeout);
  UNREFERENCED_PARAMETER(pulErrorCount);

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
int32_t APIENTRY rcXChannelIOInfo( CIFXHANDLE hChannel,
                                    uint32_t  ulCmd,
                                    uint32_t  ulAreaNumber,
                                    uint32_t  ulSize,
                                    void*     pvData)
{
  int32_t lRet = CIFX_FUNCTION_NOT_AVAILABLE;

  UNREFERENCED_PARAMETER( hChannel);
  UNREFERENCED_PARAMETER( ulCmd);
  UNREFERENCED_PARAMETER( ulAreaNumber);
  UNREFERENCED_PARAMETER( ulSize);
  UNREFERENCED_PARAMETER( pvData);

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
int32_t APIENTRY rcXChannelIORead(CIFXHANDLE hChannel,
                                  uint32_t   ulAreaNumber,
                                  uint32_t   ulOffset,
                                  uint32_t   ulDataLen,
                                  void*      pvData,
                                  uint32_t   ulTimeout)
{
  int32_t lRet = CIFX_FUNCTION_NOT_AVAILABLE;

  UNREFERENCED_PARAMETER( hChannel);
  UNREFERENCED_PARAMETER( ulAreaNumber);
  UNREFERENCED_PARAMETER( ulOffset);
  UNREFERENCED_PARAMETER( ulDataLen);
  UNREFERENCED_PARAMETER( pvData);
  UNREFERENCED_PARAMETER( ulTimeout);

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
int32_t APIENTRY rcXChannelIOWrite(CIFXHANDLE hChannel,
                                    uint32_t  ulAreaNumber,
                                    uint32_t  ulOffset,
                                    uint32_t  ulDataLen,
                                    void*     pvWriteData,
                                    uint32_t  ulTimeout)
{
  int32_t lRet = CIFX_FUNCTION_NOT_AVAILABLE;

  UNREFERENCED_PARAMETER( hChannel);
  UNREFERENCED_PARAMETER( ulAreaNumber);
  UNREFERENCED_PARAMETER( ulOffset);
  UNREFERENCED_PARAMETER( ulDataLen);
  UNREFERENCED_PARAMETER( pvWriteData);
  UNREFERENCED_PARAMETER( ulTimeout);

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
int32_t APIENTRY rcXChannelIOReadSendData(CIFXHANDLE hChannel,
                                          uint32_t   ulAreaNumber,
                                          uint32_t   ulOffset,
                                          uint32_t   ulDataLen,
                                          void*      pvData)
{
  int32_t lRet = CIFX_FUNCTION_NOT_AVAILABLE;

  UNREFERENCED_PARAMETER( hChannel);
  UNREFERENCED_PARAMETER( ulAreaNumber);
  UNREFERENCED_PARAMETER( ulOffset);
  UNREFERENCED_PARAMETER( ulDataLen);
  UNREFERENCED_PARAMETER( pvData);

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
int32_t APIENTRY rcXChannelControlBlock(CIFXHANDLE hChannel,
                                        uint32_t   ulCmd,
                                        uint32_t   ulOffset,
                                        uint32_t   ulDataLen,
                                        void*      pvData)
{
  int32_t lRet = CIFX_NO_ERROR;

  lRet = ReadChannelControlBlock( hChannel, ulCmd, ulOffset, ulDataLen, pvData);

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
int32_t APIENTRY rcXChannelCommonStatusBlock(CIFXHANDLE hChannel,
                                             uint32_t   ulCmd,
                                             uint32_t   ulOffset,
                                             uint32_t   ulDataLen,
                                             void*      pvData)
{
  int32_t lRet = CIFX_NO_ERROR;

  if (CIFX_CMD_READ_DATA != ulCmd)
  {
    lRet = CIFX_INVALID_COMMAND;
  } else
  {
    lRet = ReadChannelCommonStatusBlock( hChannel, ulCmd, ulOffset, ulDataLen, pvData);
  }
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
int32_t APIENTRY rcXChannelUserBlock(CIFXHANDLE hChannel,
                                      uint32_t  ulAreaNumber,
                                      uint32_t  ulCmd,
                                      uint32_t  ulOffset,
                                      uint32_t  ulDataLen,
                                      void*     pvData)
{
  int32_t lRet = CIFX_FUNCTION_NOT_AVAILABLE;

  UNREFERENCED_PARAMETER( hChannel);
  UNREFERENCED_PARAMETER( ulAreaNumber);
  UNREFERENCED_PARAMETER( ulCmd);
  UNREFERENCED_PARAMETER( ulOffset);
  UNREFERENCED_PARAMETER( ulDataLen);
  UNREFERENCED_PARAMETER( pvData);

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
int32_t APIENTRY rcXChannelExtendedStatusBlock(CIFXHANDLE hChannel,
                                               uint32_t   ulCmd,
                                               uint32_t   ulOffset,
                                               uint32_t   ulDataLen,
                                               void*      pvData)
{
  int32_t                lRet      = CIFX_NO_ERROR;
  PCIFX_CHANNEL_HANDLE_T ptChannel = (PCIFX_CHANNEL_HANDLE_T)hChannel;

  switch(ulCmd)
  {
    case CIFX_CMD_READ_DATA :
    {
      CIFX_PACKET                       tSendPacket       = {{0}};
      CIFX_PACKET                       tRecvPacket       = {{0}};
      RCX_DPM_GET_EXTENDED_STATE_REQ_T* ptRcxExtStatusReq = (RCX_DPM_GET_EXTENDED_STATE_REQ_T*)&tSendPacket;

      ptRcxExtStatusReq->tHead.ulSrc  = 0;
      ptRcxExtStatusReq->tHead.ulDest = RCX_PACKET_DEST_SYSTEM;
      ptRcxExtStatusReq->tHead.ulCmd  = RCX_DPM_GET_EXTENDED_STATE_REQ;
      ptRcxExtStatusReq->tHead.ulLen  = sizeof(ptRcxExtStatusReq->tData.ulOffset) +
                                        sizeof(ptRcxExtStatusReq->tData.ulDataLen) +
                                        sizeof(ptRcxExtStatusReq->tData.ulChannelIndex);

      ptRcxExtStatusReq->tData.ulOffset       = ulOffset;
      ptRcxExtStatusReq->tData.ulDataLen      = ulDataLen;
      ptRcxExtStatusReq->tData.ulChannelIndex = ptChannel->ulChannelNr;

      lRet = rcXPacketTransferPacket(((PRCX_PACKET_TL_INFO_T)(netXTransportGetTLDeviceData( ptChannel->hNetxTransportDeviceHandle)))->hTransport, &tSendPacket, &tRecvPacket, TRANSPORT_TO_TRANSFER);

      if(CIFX_NO_ERROR == lRet)
      {
        RCX_DPM_GET_EXTENDED_STATE_CNF_T* ptCnf = (RCX_DPM_GET_EXTENDED_STATE_CNF_T*)&tRecvPacket;

        if (~ptCnf->tHead.ulCmd&RCX_MSK_PACKET_ANSWER)
        {
          lRet = CIFX_TRANSPORT_INVALID_RESPONSE;
        } else if(CIFX_NO_ERROR == (lRet = ptCnf->tHead.ulSta))
        {
          if(ptCnf->tData.ulDataLen >= (ulOffset + ulDataLen))
          {
            OS_Memcpy(pvData, &ptCnf->tData.abData, ulDataLen);
          } else
          {
            lRet = CIFX_INVALID_ACCESS_SIZE;
          }
        }
      }
    }
    break;

  default:
    lRet = CIFX_INVALID_COMMAND;
    break;
  }

  return lRet;
}

/*! **************************************************************************
* Get a Pointer to the I/O Area
*   \param hChannel       Channel handle acquired by xChannelOpen
*   \param ulCmd          Command
*   \param pvMemoryInfo   Returned I/O Area pointer
*   \return CIFX_NO_ERROR on success
******************************************************************************/
int32_t APIENTRY rcXChannelPLCMemoryPtr(CIFXHANDLE hChannel, uint32_t ulCmd, void* pvMemoryInfo)
{
  int32_t lRet = CIFX_FUNCTION_NOT_AVAILABLE;

  UNREFERENCED_PARAMETER(hChannel);
  UNREFERENCED_PARAMETER(ulCmd);
  UNREFERENCED_PARAMETER(pvMemoryInfo);

  return lRet;
}

/*! **************************************************************************
* Checks if Input Data area has been updated and is ready for reading
*   \param hChannel       Channel handle acquired by xChannelOpen
*   \param ulAreaNumber   Area Number
*   \param pulReadState   State of the input area
*   \return CIFX_NO_ERROR on success
******************************************************************************/
int32_t APIENTRY rcXChannelPLCIsReadReady(CIFXHANDLE hChannel, uint32_t ulAreaNumber, uint32_t* pulReadState)
{
  int32_t lRet = CIFX_FUNCTION_NOT_AVAILABLE;

  UNREFERENCED_PARAMETER(hChannel);
  UNREFERENCED_PARAMETER(ulAreaNumber);
  UNREFERENCED_PARAMETER(pulReadState);

  return lRet;
}

/*! **************************************************************************
* Checks if Output Data area has been read by the firmware
*   \param hChannel       Channel handle acquired by xChannelOpen
*   \param ulAreaNumber   Area number
*   \param pulWriteState  State of the output area
*   \return CIFX_NO_ERROR on success
******************************************************************************/
int32_t APIENTRY rcXChannelPLCIsWriteReady(CIFXHANDLE hChannel, uint32_t ulAreaNumber, uint32_t* pulWriteState)
{
  int32_t lRet = CIFX_FUNCTION_NOT_AVAILABLE;

  UNREFERENCED_PARAMETER(hChannel);
  UNREFERENCED_PARAMETER(ulAreaNumber);
  UNREFERENCED_PARAMETER(pulWriteState);

  return lRet;
}

/*! **************************************************************************
* Requests Firmware to read the new send data
*   \param hChannel       Channel handle acquired by xChannelOpen
*   \param ulAreaNumber   Area number
*   \return CIFX_NO_ERROR on success
******************************************************************************/
int32_t APIENTRY rcXChannelPLCActivateWrite(CIFXHANDLE hChannel, uint32_t ulAreaNumber)
{
  int32_t lRet = CIFX_FUNCTION_NOT_AVAILABLE;

  UNREFERENCED_PARAMETER(hChannel);
  UNREFERENCED_PARAMETER(ulAreaNumber);

  return lRet;
}

/*! **************************************************************************
* Requests Firmware to update input data
*   \param hChannel       Channel handle acquired by xChannelOpen
*   \param ulAreaNumber   I/O Area number
*   \return CIFX_NO_ERROR on success
******************************************************************************/
int32_t APIENTRY rcXChannelPLCActivateRead(CIFXHANDLE hChannel, uint32_t ulAreaNumber)
{
  int32_t lRet = CIFX_FUNCTION_NOT_AVAILABLE;

  UNREFERENCED_PARAMETER(hChannel);
  UNREFERENCED_PARAMETER(ulAreaNumber);

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
int32_t APIENTRY rcXChannelFindFirstFile( CIFXHANDLE            hChannel,
                                          CIFX_DIRECTORYENTRY*  ptDirectoryInfo,
                                          PFN_RECV_PKT_CALLBACK pfnRecvPktCallback,
                                          void*                 pvUser)
{
  int32_t                lRet      = CIFX_NO_ERROR;
  PCIFX_CHANNEL_HANDLE_T ptChannel = (PCIFX_CHANNEL_HANDLE_T)hChannel;

  lRet = rcXSysdeviceFindFirstFile( hChannel, ptChannel->ulChannelNr, ptDirectoryInfo, pfnRecvPktCallback, pvUser);

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
int32_t APIENTRY rcXChannelFindNextFile( CIFXHANDLE              hChannel,
                                          CIFX_DIRECTORYENTRY*   ptDirectoryInfo,
                                          PFN_RECV_PKT_CALLBACK  pfnRecvPktCallback,
                                          void*                  pvUser)
{
  int32_t                lRet      = CIFX_NO_ERROR;
  PCIFX_CHANNEL_HANDLE_T ptChannel = (PCIFX_CHANNEL_HANDLE_T)hChannel;

  lRet = rcXSysdeviceFindNextFile( hChannel, ptChannel->ulChannelNr, ptDirectoryInfo, pfnRecvPktCallback, pvUser);

  return lRet;
}

/*! **************************************************************************
* Download a file (Firmware, Configuratio, etc) to the device
*   \param hChannel             Handle to the channel
*   \param ulMode               Download mode (DOWNLOAD_MODE_FIRMWARE, etc)
*   \param szFileName           Name of the file
*   \param pabFileData          Pointer to the file data
*   \param ulFileSize           Length of the file data
*   \param ptcifXTransportInst  Pointer to cifX Translation-Layer instance
*   \param pfnCallback          Callback for progress indication (NULL for no callback)
*   \param pfnRecvPktCallback   Callback pointer for unhandled receive packets
*   \param pvUser               User parameter on callback.
*   \return CIFX_NO_ERROR on success
******************************************************************************/
int32_t APIENTRY rcXChannelDownload(CIFXHANDLE                  hChannel,
                                     uint32_t                   ulMode,
                                     char*                      szFileName,
                                     uint8_t*                   pabFileData,
                                     uint32_t                   ulFileSize,
                                     CIFX_TRANSPORT_DEVICE_INST_T* ptcifXTransportInst,
                                     PFN_PROGRESS_CALLBACK      pfnCallback,
                                     PFN_RECV_PKT_CALLBACK      pfnRecvPktCallback,
                                     void*                      pvUser)
{
  int32_t                lRet      = CIFX_NO_ERROR;
  PCIFX_CHANNEL_HANDLE_T ptChannel = (PCIFX_CHANNEL_HANDLE_T)hChannel;

  lRet = rcXSysdeviceDownload( hChannel,
                              ptChannel->ulChannelNr,
                              ulMode,
                              szFileName,
                              pabFileData,
                              ulFileSize,
                              ptcifXTransportInst,
                              pfnCallback,
                              pfnRecvPktCallback,
                              pvUser);

  return lRet;
}

/*****************************************************************************/
/*! Uploads a file via Communication channel
*   \param hChannel             Handle to the Channel
*   \param ulMode               Transfer Mode
*   \param szFileName           Filename to upload
*   \param pabFileData          Pointer to buffer receiving upload
*   \param pulFileSize          [in]Length of buffer, [out] Bytes copied to buffer
*   \param ptcifXTransportInst  Pointer to cifX Translation-Layer instance
*   \param pfnCallback          Callback pointer for progress
*   \param pfnRecvPktCallback   Callback pointer for unhandled receive packets
*   \param pvUser               User parameter on callback.
*   \return CIFX_NO_ERROR on success                                         */
/*****************************************************************************/
int32_t APIENTRY rcXChannelUpload( CIFXHANDLE                  hChannel,
                                    uint32_t                   ulMode,
                                    char*                      szFileName,
                                    uint8_t*                   pabFileData,
                                    uint32_t*                  pulFileSize,
                                    CIFX_TRANSPORT_DEVICE_INST_T* ptcifXTransportInst,
                                    PFN_PROGRESS_CALLBACK      pfnCallback,
                                    PFN_RECV_PKT_CALLBACK      pfnRecvPktCallback,
                                    void*                      pvUser)
{
  int32_t                lRet      = CIFX_NO_ERROR;
  PCIFX_CHANNEL_HANDLE_T ptChannel = (PCIFX_CHANNEL_HANDLE_T)hChannel;

  lRet = rcXSysdeviceUpload(hChannel,
                            ptChannel->ulChannelNr,
                            ulMode,
                            szFileName,
                            pabFileData,
                            pulFileSize,
                            ptcifXTransportInst,
                            pfnCallback,
                            pfnRecvPktCallback,
                            pvUser);

  return lRet;
}

/*! **************************************************************************
* Reset the channel
*   \param hChannel     Channel handle acquired by xChannelOpen
*   \param ulResetMode  Cold/Warmstart selection
*   \param ulTimeout    Time in ms to wait for the reset
*   \return CIFX_NO_ERROR on success
******************************************************************************/
int32_t APIENTRY rcXChannelReset(CIFXHANDLE hChannel,
                                  uint32_t  ulResetMode,
                                  uint32_t  ulTimeout)
{
  int32_t                lRet      = CIFX_NO_ERROR;
  PCIFX_CHANNEL_HANDLE_T ptChannel = (PCIFX_CHANNEL_HANDLE_T)hChannel;

  if( RCXPACKET_SYSTEMCHANNEL_INTERNAL == ptChannel->ulChannelNr)
    return CIFX_INVALID_CHANNEL;

  lRet = rcXReset( hChannel, ulResetMode, ulTimeout);

  return lRet;
}

/***************************************************************************
*
*
*   AUXILIARY FUNCTIONS....................
*
*
*
*
*
******************************************************************************/

/*****************************************************************************/
/*! Send CIFX_PACKET
*   \param hTransport   handle of the transport layer
*   \param ptSendPacket pointer to allocated send packet
*   \param ulTimeout    timeout
*   \return NXT_NO_ERROR on success                                         */
/*****************************************************************************/
static int32_t rcXPacketSendPacket( NXTHANDLE hTransport, CIFX_PACKET* ptSendPacket, uint32_t ulTimeout)
{
  int32_t                 lRet              = CIFX_NO_ERROR;
  uint32_t                ulDataLen         = ptSendPacket->tHeader.ulLen + sizeof(ptSendPacket->tHeader);
  PHIL_TRANSPORT_PACKET_T ptTransportPacket = NULL;

  /* allocate netXTransport send packet */
  ptTransportPacket = HilTransportAllocatePacket( hTransport, ulDataLen, HIL_TRANSPORT_TYPE_RCX_PACKET);

  if(ptTransportPacket != NULL)
  {
    ptTransportPacket->ptHeader->ulLength = ulDataLen;
    OS_Memcpy(ptTransportPacket->pbData, ptSendPacket, ulDataLen);

    /* execute physical send packet */
    lRet = HilTransportSendPacket( hTransport, ptTransportPacket->ptHeader, ulTimeout);
  } else
  {
    lRet = CIFX_FUNCTION_FAILED;
  }
  HilTransportFreePacket( ptTransportPacket);
  return lRet;
}

/*****************************************************************************/
/*! Transfer CIFX_PACKET
*   \param hTransport   handle of the transport layer
*   \param ptSendPacket pointer to allocated send packet
*   \param ptRecvPacket pointer to receive packet
*   \param ulTimeout    timeout
*   \return NXT_NO_ERROR on success                                         */
/*****************************************************************************/
static int32_t rcXPacketTransferPacket(NXTHANDLE hTransport, CIFX_PACKET* ptSendPacket, CIFX_PACKET* ptRecvPacket, uint32_t ulTimeout)
{
  int32_t                 lRet                  = CIFX_NO_ERROR;
  uint32_t                ulDataLen             = 0;
  PHIL_TRANSPORT_PACKET_T ptTransportSendPacket = NULL;
  PHIL_TRANSPORT_PACKET_T ptTransportRecvPacket = NULL;

  ulDataLen = ptSendPacket->tHeader.ulLen + sizeof(ptSendPacket->tHeader);

  /* allocate netXTransport send packet */
  ptTransportSendPacket = HilTransportAllocatePacket( hTransport, ulDataLen, HIL_TRANSPORT_TYPE_RCX_PACKET);

  if (ptTransportSendPacket != NULL)
  {
    ptTransportSendPacket->ptHeader->ulLength = ulDataLen;
    OS_Memcpy(ptTransportSendPacket->pbData, ptSendPacket, ulDataLen);

    if (NXT_NO_ERROR == (lRet = HilTransportTransferPacket( hTransport,
                                        ptTransportSendPacket->ptHeader,
                                        &ptTransportRecvPacket,
                                        ulTimeout)))
    {
      OS_Memcpy( ptRecvPacket, ptTransportRecvPacket->pbData, ptTransportRecvPacket->ptHeader->ulLength);
    }
  }
  HilTransportFreePacket( ptTransportSendPacket);
  HilTransportFreePacket( ptTransportRecvPacket);
  return lRet;
}

/*****************************************************************************/
/*! Retrieves the hardware identfication of a device
*   \param ptTLInfo handle to the translation layer
*   \param ptSendPacket pointer to allocated send packet
*   \return NXT_NO_ERROR on success                                         */
/*****************************************************************************/
static int32_t ReadHWIdentify(PRCX_PACKET_TL_INFO_T ptTLInfo, PHIL_TRANSPORT_PACKET_T ptSendPacket)
{
  int32_t                   lRet;
  RCX_HW_IDENTIFY_REQ_T*    ptHwIdentReq = (RCX_HW_IDENTIFY_REQ_T*)ptSendPacket->pbData;
  PHIL_TRANSPORT_PACKET_T   ptRecvPacket = NULL;

  ptSendPacket->ptHeader->ulLength   = sizeof(*ptHwIdentReq);
  ptSendPacket->ptHeader->usChecksum = 0;

  OS_Memset(&ptHwIdentReq->tHead, 0, sizeof(ptHwIdentReq->tHead));

  ptHwIdentReq->tHead.ulSrc    = 0;
  ptHwIdentReq->tHead.ulDest   = RCX_PACKET_DEST_SYSTEM;
  ptHwIdentReq->tHead.ulCmd = RCX_HW_IDENTIFY_REQ;

  if(NXT_NO_ERROR == (lRet = HilTransportTransferPacket(ptTLInfo->hTransport,
                                                        ptSendPacket->ptHeader,
                                                        &ptRecvPacket,
                                                        CIFX_TO_CONT_PACKET)))
  {
    RCX_HW_IDENTIFY_CNF_T* ptCnf = (RCX_HW_IDENTIFY_CNF_T*)ptRecvPacket->pbData;

    ptTLInfo->tDevice.tDeviceInfo = ptCnf->tData;

    HilTransportFreePacket(ptRecvPacket);
  }
  return lRet;
}

/*****************************************************************************/
/*! Retrieves the channel frmware information
*   \param ptTLInfo handle to the translation layer
*   \param ptSendPacket pointer to allocated send packet
*   \param ulAreaIdx
*   \param ptFirmwareInfo
*   \return NXT_NO_ERROR on success                                         */
/*****************************************************************************/
static int32_t ReadChannelFirmware(PRCX_PACKET_TL_INFO_T ptTLInfo, PHIL_TRANSPORT_PACKET_T ptSendPacket, uint32_t ulAreaIdx, RCX_FW_IDENTIFICATION_T* ptFirmwareInfo)
{
  int32_t                       lRet                = CIFX_NO_ERROR;
  RCX_FIRMWARE_IDENTIFY_REQ_T*  ptFirmwareIdentReq  = (RCX_FIRMWARE_IDENTIFY_REQ_T*)ptSendPacket->pbData;
  PHIL_TRANSPORT_PACKET_T       ptRecvPacket        = NULL;

  ptSendPacket->ptHeader->ulLength   = sizeof(*ptFirmwareIdentReq);

  OS_Memset(&ptFirmwareIdentReq->tHead, 0, sizeof(ptFirmwareIdentReq->tHead));

  ptFirmwareIdentReq->tHead.ulDest      = RCX_PACKET_DEST_SYSTEM;
  ptFirmwareIdentReq->tHead.ulLen       = sizeof(ptFirmwareIdentReq->tData);
  ptFirmwareIdentReq->tHead.ulCmd       = RCX_FIRMWARE_IDENTIFY_REQ;
  ptFirmwareIdentReq->tData.ulChannelId = ulAreaIdx;

  if(NXT_NO_ERROR == (lRet = HilTransportTransferPacket(ptTLInfo->hTransport,
                                                        ptSendPacket->ptHeader,
                                                        &ptRecvPacket,
                                                        CIFX_TO_CONT_PACKET)))
  {
    RCX_FIRMWARE_IDENTIFY_CNF_T* ptCnf = (RCX_FIRMWARE_IDENTIFY_CNF_T*)ptRecvPacket->pbData;

    if(CIFX_NO_ERROR == (lRet = ptCnf->tHead.ulSta))
    {
      *ptFirmwareInfo = ptCnf->tData.tFirmwareIdentification;
    }

    HilTransportFreePacket(ptRecvPacket);
  }

  return lRet;
}

/*****************************************************************************/
/*! Retrieves the channel information
*   \param ptTLInfo handle to the translation layer
*   \param ptSendPacket pointer to allocated send packet
*   \param ulAreaIdx
*   \return NXT_NO_ERROR on success                                         */
/*****************************************************************************/
static void CreateChannelInfo(PRCX_PACKET_TL_INFO_T ptTLInfo, PHIL_TRANSPORT_PACKET_T ptSendPacket, uint32_t ulAreaIdx)
{
  int32_t                       lRet;
  RCX_DPM_GET_BLOCK_INFO_REQ_T* ptBlockInfoReq = (RCX_DPM_GET_BLOCK_INFO_REQ_T*)ptSendPacket->pbData;
  PHIL_TRANSPORT_PACKET_T       ptRecvPacket = NULL;

  ptSendPacket->ptHeader->ulLength   = sizeof(*ptBlockInfoReq);

  OS_Memset(&ptBlockInfoReq->tHead, 0, sizeof(ptBlockInfoReq->tHead));

  ptBlockInfoReq->tHead.ulCmd           = RCX_DPM_GET_BLOCK_INFO_REQ;
  ptBlockInfoReq->tHead.ulLen           = sizeof(ptBlockInfoReq->tData);
  ptBlockInfoReq->tData.ulAreaIndex     = ulAreaIdx;
  ptBlockInfoReq->tData.ulSubblockIndex = 0;

  lRet = HilTransportTransferPacket( ptTLInfo->hTransport,
                                     ptSendPacket->ptHeader,
                                     &ptRecvPacket,
                                     CIFX_TO_CONT_PACKET);
  {
    RCX_DPM_GET_BLOCK_INFO_CNF_T* ptCnf = (RCX_DPM_GET_BLOCK_INFO_CNF_T*)ptRecvPacket->pbData;

    if( (CIFX_NO_ERROR == lRet) ||
        /* Bootloader does not support this command. Proceed anyway because there has to be the system channel. */
        ((0 == ulAreaIdx) && (RCX_E_UNKNOWN_COMMAND == ptCnf->tHead.ulSta)) )
    {
      NETX_CHANNEL_INFO_BLOCK tChannelInfoBlock;
      PRCX_PACKET_CHANNEL_T   ptChannel         = OS_Memalloc(sizeof(*ptChannel));

      if(NULL == ptChannel)
      {
        lRet = NXT_OUT_OF_MEMORY;
      } else
      {
        OS_Memset(ptChannel, 0, sizeof(*ptChannel));
        TAILQ_INIT(&ptChannel->tRecvPackets);
        ptChannel->hRecvSemaphore = OS_CreateSemaphore(0);

        lRet = ReadChannelInfoBlock( ptTLInfo->hTransport, ulAreaIdx, sizeof(tChannelInfoBlock), &tChannelInfoBlock);
        /* Older bootloader may not support this command. Proceed anyway because there has to be the system channel. */
        if ((0 == ulAreaIdx) && (RCX_E_UNKNOWN_COMMAND == lRet))
        {
          /* Command not supported. Use default system channel */
          tChannelInfoBlock.tSystem.bChannelType             = RCX_CHANNEL_TYPE_SYSTEM;
          tChannelInfoBlock.tSystem.ulSizeOfChannel          = NETX_SYSTEM_CHANNEL_SIZE;
          tChannelInfoBlock.tSystem.bSizePositionOfHandshake = RCX_HANDSHAKE_POSITION_CHANNEL|RCX_HANDSHAKE_SIZE_8BIT;
          tChannelInfoBlock.tSystem.usSizeOfMailbox          = 256;
          tChannelInfoBlock.tSystem.usMailboxStartOffset     = 256;

          lRet = CIFX_NO_ERROR;
        }
        if(CIFX_NO_ERROR == lRet)
        {
          if ( (tChannelInfoBlock.tCom.bChannelType == RCX_CHANNEL_TYPE_COMMUNICATION) ||
               (tChannelInfoBlock.tCom.bChannelType == RCX_CHANNEL_TYPE_SYSTEM)   )
          {
            /* This is the system channel or a communication channel, so continue with firmware request */
            uint32_t ulChannelId = (tChannelInfoBlock.tCom.bChannelType == RCX_CHANNEL_TYPE_SYSTEM)?RCX_SYSTEM_CHANNEL:tChannelInfoBlock.tCom.bChannelId;

            /* Query Firmware name. If that works ok, we can continue enumerating blocks */
            if(CIFX_NO_ERROR == (lRet = ReadChannelFirmware(ptTLInfo, ptSendPacket, ulChannelId, &ptChannel->tFirmwareInfo)))
            {
              ptChannel->ulAreaNr       = ulAreaIdx;
              ptChannel->ulChannelId    = ulChannelId;

              ptTLInfo->tDevice.aptChannels[ptTLInfo->tDevice.ulChannelCount] = ptChannel;
              ptTLInfo->tDevice.ulChannelCount++;
            }
          } else
          {
            /* Return error as we are only interested in system and communication channel */
            lRet = CIFX_INVALID_CHANNEL;
          }
        }
      }

      if(CIFX_NO_ERROR != lRet)
        OS_Memfree(ptChannel);
    }

    HilTransportFreePacket(ptRecvPacket);
  }
}

/*****************************************************************************/
/*! Retrieves the system info block information
*   \param ptTLInfo handle to the translation layer
*   \param ptSendPacket pointer to allocated send packet
*   \param ptSysInfoBlock
*   \return NXT_NO_ERROR on success                                         */
/*****************************************************************************/
static int32_t ReadSysInfoBlock(PRCX_PACKET_TL_INFO_T ptTLInfo, PHIL_TRANSPORT_PACKET_T ptSendPacket, NETX_SYSTEM_INFO_BLOCK* ptSysInfoBlock)
{
  int32_t                        lRet                 = CIFX_NO_ERROR;
  RCX_READ_SYS_INFO_BLOCK_REQ_T* ptRcxSysInfoBlockReq = (RCX_READ_SYS_INFO_BLOCK_REQ_T*)ptSendPacket->pbData;
  PHIL_TRANSPORT_PACKET_T        ptRecvPacket         = NULL;

  ptSendPacket->ptHeader->ulLength   = sizeof(*ptRcxSysInfoBlockReq);

  OS_Memset(&ptRcxSysInfoBlockReq->tHead, 0, sizeof(ptRcxSysInfoBlockReq->tHead));

  ptRcxSysInfoBlockReq->tHead.ulDest          = RCX_PACKET_DEST_SYSTEM;
  ptRcxSysInfoBlockReq->tHead.ulCmd           = RCX_SYSTEM_INFORMATION_BLOCK_REQ;

  if(NXT_NO_ERROR == (lRet = HilTransportTransferPacket(ptTLInfo->hTransport,
                                                        ptSendPacket->ptHeader,
                                                        &ptRecvPacket,
                                                        CIFX_TO_CONT_PACKET)))
  {
    RCX_READ_SYS_INFO_BLOCK_CNF_T* ptCnf = (RCX_READ_SYS_INFO_BLOCK_CNF_T*)ptRecvPacket->pbData;

    if(CIFX_NO_ERROR == (lRet = ptCnf->tHead.ulSta))
    {
      *ptSysInfoBlock = ptCnf->tData.tSystemInfo;

      /* 2nd Stage Loader clear the cookie the show DPM is disabled. (it only supports 1
         connection at a time). */
      if( (ptSysInfoBlock->abCookie[0] == 0) &&
          (ptSysInfoBlock->abCookie[1] == 0) &&
          (ptSysInfoBlock->abCookie[2] == 0) &&
          (ptSysInfoBlock->abCookie[3] == 0) )
      {
        OS_Memcpy(ptSysInfoBlock->abCookie, CIFX_DPMSIGNATURE_BSL_STR, sizeof(ptSysInfoBlock->abCookie));
      }
    } else if (RCX_E_UNKNOWN_COMMAND == lRet)
    {
      lRet = CIFX_NO_ERROR;
      ptSysInfoBlock->ulDeviceNumber   = ptTLInfo->tDevice.tDeviceInfo.ulDeviceNumber;
      ptSysInfoBlock->ulSerialNumber   = ptTLInfo->tDevice.tDeviceInfo.ulSerialNumber;
      ptSysInfoBlock->ausHwOptions[0]  = ptTLInfo->tDevice.tDeviceInfo.ausHwOptions[0];
      ptSysInfoBlock->ausHwOptions[1]  = ptTLInfo->tDevice.tDeviceInfo.ausHwOptions[1];
      ptSysInfoBlock->ausHwOptions[2]  = ptTLInfo->tDevice.tDeviceInfo.ausHwOptions[2];
      ptSysInfoBlock->ausHwOptions[3]  = ptTLInfo->tDevice.tDeviceInfo.ausHwOptions[3];
      ptSysInfoBlock->usDeviceClass    = ptTLInfo->tDevice.tDeviceInfo.usDeviceClass;
      ptSysInfoBlock->bHwRevision      = ptTLInfo->tDevice.tDeviceInfo.bHwRevision;
      ptSysInfoBlock->bHwCompatibility = ptTLInfo->tDevice.tDeviceInfo.bHwCompatibility;
      ptSysInfoBlock->usManufacturer   = RCX_MANUFACTURER_HILSCHER_GMBH;

      OS_Memcpy((char*)ptSysInfoBlock->abCookie,CIFX_DPMSIGNATURE_BSL_STR, 4);
    }

    HilTransportFreePacket(ptRecvPacket);
  }

  return lRet;
}

/*****************************************************************************/
/*! Read System Status Block
*   \param  ptSysStatusBlock Reference to system status block
*   \return NXT_NO_ERROR on success                                         */
/*****************************************************************************/
static int32_t ReadSysStatusBlock( CIFXHANDLE  hSysdevice, NETX_SYSTEM_STATUS_BLOCK* ptSysStatusBlock)
{
  PCIFX_CHANNEL_HANDLE_T          ptChannel             = (PCIFX_CHANNEL_HANDLE_T)hSysdevice;
  PRCX_PACKET_TL_INFO_T           ptPacketData          = (PRCX_PACKET_TL_INFO_T)netXTransportGetTLDeviceData( ptChannel->hNetxTransportDeviceHandle);
  int32_t                         lRet                  = CIFX_NO_ERROR;
  PHIL_TRANSPORT_PACKET_T         ptTransportSendPacket = NULL;
  PHIL_TRANSPORT_PACKET_T         ptTransportRecvPacket = NULL;
  RCX_READ_SYS_STATUS_BLOCK_REQ_T tRcxSysStaBlockReq = {{0}};

  if (NULL == ptSysStatusBlock)
    return CIFX_INVALID_PARAMETER;

  tRcxSysStaBlockReq.tHead.ulSrc    = 0;
  tRcxSysStaBlockReq.tHead.ulDest   = RCX_PACKET_DEST_SYSTEM;
  tRcxSysStaBlockReq.tHead.ulCmd    = RCX_SYSTEM_STATUS_BLOCK_REQ;

  ptTransportSendPacket = HilTransportAllocatePacket( ((PRCX_PACKET_TL_INFO_T)(netXTransportGetTLDeviceData( ptChannel->hNetxTransportDeviceHandle)))->hTransport, sizeof(tRcxSysStaBlockReq.tHead), HIL_TRANSPORT_TYPE_RCX_PACKET);

  if (ptTransportSendPacket != 0)
  {
    ptTransportSendPacket->ptHeader->ulLength = sizeof(tRcxSysStaBlockReq.tHead);
    OS_Memcpy( ptTransportSendPacket->pbData, &tRcxSysStaBlockReq, sizeof(tRcxSysStaBlockReq.tHead));

    lRet = HilTransportTransferPacket(  ptPacketData->hTransport,
                                        ptTransportSendPacket->ptHeader,
                                        &ptTransportRecvPacket,
                                        100);
  } else
  {
    lRet = CIFX_FUNCTION_FAILED;
  }
  if(CIFX_NO_ERROR == lRet)
  {
    RCX_READ_SYS_STATUS_BLOCK_CNF_T* ptCnf = (RCX_READ_SYS_STATUS_BLOCK_CNF_T*)ptTransportRecvPacket->pbData;

    if (~ptCnf->tHead.ulCmd&RCX_MSK_PACKET_ANSWER)
    {
      lRet = CIFX_TRANSPORT_INVALID_RESPONSE;
    } else if(CIFX_NO_ERROR == (lRet = ptCnf->tHead.ulSta))
    {
      *ptSysStatusBlock  = ptCnf->tData.tSystemState;
    }
  }
  if (ptTransportSendPacket != NULL)
  {
    HilTransportFreePacket(ptTransportSendPacket);
  }

  return lRet;
}

static int32_t ReadChannelInfoBlock( NXTHANDLE hTransport, uint32_t ulChannelId, uint32_t ulSize, void* pvData)
{
  int32_t                         lRet                = CIFX_NO_ERROR;
  CIFX_PACKET                     tSendPacket         = {{0}};
  CIFX_PACKET                     tRecvPacket         = {{0}};
  RCX_READ_CHNL_INFO_BLOCK_REQ_T* ptRcxSysChnBlockReq = (RCX_READ_CHNL_INFO_BLOCK_REQ_T*)&tSendPacket;

  if( ulSize < (uint32_t)sizeof(RCX_READ_CHNL_INFO_BLOCK_CNF_DATA_T))
  {
    lRet = CIFX_INVALID_BUFFERSIZE;
  } else
  {
    ptRcxSysChnBlockReq->tHead.ulSrc       = 0;
    ptRcxSysChnBlockReq->tHead.ulDest      = RCX_PACKET_DEST_SYSTEM;
    ptRcxSysChnBlockReq->tHead.ulLen       = sizeof(ptRcxSysChnBlockReq->tData);
    ptRcxSysChnBlockReq->tHead.ulCmd       = RCX_CHANNEL_INFORMATION_BLOCK_REQ;
    ptRcxSysChnBlockReq->tData.ulChannelId = ulChannelId;

    lRet = rcXPacketTransferPacket( hTransport, &tSendPacket, &tRecvPacket, TRANSPORT_TO_TRANSFER);

    if(CIFX_NO_ERROR == lRet)
    {
      RCX_READ_CHNL_INFO_BLOCK_CNF_T* ptCnf = (RCX_READ_CHNL_INFO_BLOCK_CNF_T*)&tRecvPacket;

      if (~ptCnf->tHead.ulCmd&RCX_MSK_PACKET_ANSWER)
      {
        lRet = CIFX_TRANSPORT_INVALID_RESPONSE;
      } else if(CIFX_NO_ERROR == (lRet = ptCnf->tHead.ulSta))
      {
        uint32_t ulCopySize = ptCnf->tHead.ulLen;

        if(ulSize < ulCopySize)
        {
          ulCopySize = ulSize;
          lRet = CIFX_BUFFER_TOO_SHORT;
        }
        OS_Memcpy(pvData, &ptCnf->tData, ulCopySize);
      }
    }
  }
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
static int32_t ReadChannelCommonStatusBlock( CIFXHANDLE hChannel,
                                  uint32_t   ulCmd,
                                  uint32_t   ulOffset,
                                  uint32_t   ulDataLen,
                                  void*      pvData)
{
  int32_t                          lRet                  = CIFX_NO_ERROR;
  PCIFX_CHANNEL_HANDLE_T           ptChannel             = (PCIFX_CHANNEL_HANDLE_T)hChannel;
  CIFX_PACKET                      tSendPacket           = {{0}};
  CIFX_PACKET                      tRecvPacket           = {{0}};
  RCX_READ_COMMON_STS_BLOCK_REQ_T* ptRcxCommonStatusReq  = (RCX_READ_COMMON_STS_BLOCK_REQ_T*)&tSendPacket;

  UNREFERENCED_PARAMETER(ulCmd);

  ptRcxCommonStatusReq->tHead.ulSrc       = 0;
  ptRcxCommonStatusReq->tHead.ulDest      = RCX_PACKET_DEST_SYSTEM;
  ptRcxCommonStatusReq->tHead.ulCmd       = RCX_DPM_GET_COMMON_STATE_REQ;
  ptRcxCommonStatusReq->tHead.ulLen       = sizeof(ptRcxCommonStatusReq->tData.ulChannelId);
  ptRcxCommonStatusReq->tData.ulChannelId = ptChannel->ulChannelNr;

  lRet = rcXPacketTransferPacket(((PRCX_PACKET_TL_INFO_T)(netXTransportGetTLDeviceData( ptChannel->hNetxTransportDeviceHandle)))->hTransport, &tSendPacket, &tRecvPacket, TRANSPORT_TO_TRANSFER);

  if(CIFX_NO_ERROR == lRet)
  {
    RCX_READ_COMMON_STS_BLOCK_CNF_T* ptCnf = (RCX_READ_COMMON_STS_BLOCK_CNF_T*)&tRecvPacket;

    if (~ptCnf->tHead.ulCmd&RCX_MSK_PACKET_ANSWER)
    {
      lRet = CIFX_TRANSPORT_INVALID_RESPONSE;
    } else if(CIFX_NO_ERROR == (lRet = ptCnf->tHead.ulSta))
    {
      if(sizeof(ptCnf->tData.tCommonStatus) >= (ulOffset + ulDataLen))
      {
        /* Copy data to user buffer */
        OS_Memcpy(pvData, (void*)(&ptCnf->tData.tCommonStatus + ulOffset), ulDataLen);
      }else
      {
        lRet = CIFX_INVALID_ACCESS_SIZE;
      }
    }
  }
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
static int32_t ReadChannelControlBlock(CIFXHANDLE hChannel,
                                        uint32_t   ulCmd,
                                        uint32_t   ulOffset,
                                        uint32_t   ulDataLen,
                                        void*      pvData)
{
  int32_t                          lRet                  = CIFX_NO_ERROR;
  PCIFX_CHANNEL_HANDLE_T           ptChannel             = (PCIFX_CHANNEL_HANDLE_T)hChannel;
  CIFX_PACKET                      tSendPacket           = {{0}};
  CIFX_PACKET                      tRecvPacket           = {{0}};
  RCX_READ_COMM_CNTRL_BLOCK_REQ_T* ptRcxCommonControlReq = (RCX_READ_COMM_CNTRL_BLOCK_REQ_T*)&tSendPacket;

  UNREFERENCED_PARAMETER(ulCmd);

  ptRcxCommonControlReq->tHead.ulSrc       = 0;
  ptRcxCommonControlReq->tHead.ulDest      = RCX_PACKET_DEST_SYSTEM;
  ptRcxCommonControlReq->tHead.ulCmd       = RCX_CONTROL_BLOCK_REQ;
  ptRcxCommonControlReq->tHead.ulLen       = sizeof(ptRcxCommonControlReq->tData.ulChannelId);
  ptRcxCommonControlReq->tData.ulChannelId = ptChannel->ulChannelNr;

  lRet = rcXPacketTransferPacket(((PRCX_PACKET_TL_INFO_T)(netXTransportGetTLDeviceData( ptChannel->hNetxTransportDeviceHandle)))->hTransport, &tSendPacket, &tRecvPacket, TRANSPORT_TO_TRANSFER);

  if(CIFX_NO_ERROR == lRet)
  {
    RCX_READ_COMM_CNTRL_BLOCK_CNF_T* ptCnf = (RCX_READ_COMM_CNTRL_BLOCK_CNF_T*)&tRecvPacket;

    if (~ptCnf->tHead.ulCmd&RCX_MSK_PACKET_ANSWER)
    {
      lRet = CIFX_TRANSPORT_INVALID_RESPONSE;
    } else if(CIFX_NO_ERROR == (lRet = ptCnf->tHead.ulSta))
    {
      if(sizeof(ptCnf->tData.tControl) >= (ulOffset + ulDataLen))
      {
        /* Copy data to user buffer */
        OS_Memcpy(pvData, (void*)(&ptCnf->tData.tControl + ulOffset), ulDataLen);
      }else
      {
        lRet = CIFX_INVALID_ACCESS_SIZE;
      }
    }
  }
  return lRet;
}

/*! **************************************************************************
* Read Com Flags
*   \param ulAreaIndex Area index
*   \param pulNetxComFlag Reference to netx com flags
*   \param pulHostComFlag Reference to host com flags
*   \return CIFX_NO_ERROR on success
******************************************************************************/
int32_t ReadComFlags( CIFXHANDLE hChannel, uint32_t* pulNetxComFlag, uint32_t* pulHostComFlag)
{
  int32_t                          lRet                  = CIFX_NO_ERROR;
  PCIFX_CHANNEL_HANDLE_T           ptChannel             = (PCIFX_CHANNEL_HANDLE_T)hChannel;
  CIFX_PACKET                      tSendPacket           = {{0}};
  CIFX_PACKET                      tRecvPacket           = {{0}};
  RCX_DPM_GET_COMFLAG_INFO_REQ_T*  tRcxComFlagReq        = (RCX_DPM_GET_COMFLAG_INFO_REQ_T*)&tSendPacket;

  if ( (NULL == pulNetxComFlag) ||
       (NULL == pulHostComFlag)   )
    return CIFX_INVALID_PARAMETER;

  tRcxComFlagReq->tHead.ulSrc       = 0;
  tRcxComFlagReq->tHead.ulDest      = RCX_PACKET_DEST_SYSTEM;
  tRcxComFlagReq->tHead.ulCmd       = RCX_DPM_GET_COMFLAG_INFO_REQ;
  tRcxComFlagReq->tHead.ulLen       = sizeof(tRcxComFlagReq->tData.ulAreaIndex);
  tRcxComFlagReq->tData.ulAreaIndex = ptChannel->ulChannelNr;

  lRet = rcXPacketTransferPacket(((PRCX_PACKET_TL_INFO_T)(netXTransportGetTLDeviceData( ptChannel->hNetxTransportDeviceHandle)))->hTransport, &tSendPacket, &tRecvPacket, TRANSPORT_TO_TRANSFER);

  if(CIFX_NO_ERROR == lRet)
  {
    RCX_DPM_GET_COMFLAG_INFO_CNF_T* ptCnf = (RCX_DPM_GET_COMFLAG_INFO_CNF_T*)&tRecvPacket;

    if (~ptCnf->tHead.ulCmd&RCX_MSK_PACKET_ANSWER)
    {
      lRet = CIFX_TRANSPORT_INVALID_RESPONSE;
    } else if(CIFX_NO_ERROR == (lRet = ptCnf->tHead.ulSta))
    {
      *pulNetxComFlag = ptCnf->tData.ulNetxComFlag;
      *pulHostComFlag = ptCnf->tData.ulHostComFlag;
    }
  }
  return lRet;
}

/*! **************************************************************************
* Reset device
*   \param hSysdevice   Sysdevice handle acquired by xSysdeviceOpen
*   \param  ulResetMode CIFX_SYSTEMSTART or CIFX_CHANNELINIT
*   \param ulTimeout    Time in ms to wait for the reset
*   \return CIFX_NO_ERROR on success
******************************************************************************/
static int32_t rcXReset( CIFXHANDLE hDevice, uint32_t ulResetMode, uint32_t ulTimeout)
{
  int32_t                lRet        = CIFX_INVALID_COMMAND;
  PCIFX_CHANNEL_HANDLE_T ptChannel   = (PCIFX_CHANNEL_HANDLE_T)hDevice;
  CIFX_PACKET            tSendPacket = {{0}};
  CIFX_PACKET            tRecvPacket = {{0}};
  uint8_t                bChannel    = (uint8_t)ptChannel->ulChannelNr;

  switch(ulResetMode)
  {
    case CIFX_SYSTEMSTART:
    {
      RCX_FIRMWARE_RESET_REQ_T* ptReset = (RCX_FIRMWARE_RESET_REQ_T*)&tSendPacket;

      ptReset->tHead.ulSrc  = 0;
      ptReset->tHead.ulDest = RCX_PACKET_DEST_SYSTEM;
      ptReset->tHead.ulLen  = sizeof(RCX_FIRMWARE_RESET_REQ_DATA_T);
      ptReset->tHead.ulCmd  = RCX_FIRMWARE_RESET_REQ;

      ptReset->tData.ulTimeToReset = 500; /* Device shall wait 500ms to proceed with reset */
      ptReset->tData.ulResetMode   = 0;

      /* We need to send a systemstart to systemchannel */
      bChannel = RCXPACKET_SYSTEMCHANNEL_INTERNAL;

      lRet = CIFX_NO_ERROR;
    }
    break;

    case CIFX_CHANNELINIT:
      if(bChannel == RCXPACKET_SYSTEMCHANNEL_INTERNAL)
      {
        lRet = CIFX_INVALID_CHANNEL;
      } else
      {
        tSendPacket.tHeader.ulSrc  = 0;
        tSendPacket.tHeader.ulDest = RCX_PKT_COMM_CHANNEL_TOKEN;
        tSendPacket.tHeader.ulCmd  = RCX_CHANNEL_INIT_REQ;

        lRet = CIFX_NO_ERROR;
      }
    break;

    default:
      lRet = CIFX_INVALID_COMMAND;
    break;
  }

  if(CIFX_NO_ERROR == lRet)
  {
    lRet = rcXPacketTransferPacket(((PRCX_PACKET_TL_INFO_T)(netXTransportGetTLDeviceData( ptChannel->hNetxTransportDeviceHandle)))->hTransport, &tSendPacket, &tRecvPacket, ulTimeout);

    /* Send rcx packet -> reset, depends on given parameter */
    if(CIFX_NO_ERROR == lRet)
    {
      RCX_FIRMWARE_RESET_CNF_T* ptCnf = (RCX_FIRMWARE_RESET_CNF_T*)&tRecvPacket;

      lRet = ptCnf->tHead.ulSta;
      /* System start is done, so handle host side */
    }
  }
  return lRet;
}

/*! **************************************************************************
* Read system information
*   \param hSysdevice     Sysdevice handle acquired by xSysdeviceOpen
*   \param  ptSystemInfo  Pointer to system information structure
*   \return CIFX_NO_ERROR on success
******************************************************************************/
int32_t ReadSysInformation( CIFXHANDLE hSysDevice, SYSTEM_CHANNEL_SYSTEM_INFORMATION* ptSystemInfo)
{
  PCIFX_CHANNEL_HANDLE_T ptChannel = (PCIFX_CHANNEL_HANDLE_T)hSysDevice;
  PRCX_PACKET_TL_INFO_T  ptTLInfo  = (PRCX_PACKET_TL_INFO_T)(netXTransportGetTLDeviceData( ptChannel->hNetxTransportDeviceHandle));

  if (NULL == ptSystemInfo)
    return CIFX_INVALID_PARAMETER;

  ptSystemInfo->ulSystemError  = CIFX_NO_ERROR;
  ptSystemInfo->ulDpmTotalSize = ptTLInfo->tDevice.tSystemInfo.ulDpmTotalSize;
  ptSystemInfo->ulMBXSize      = CIFX_MAX_PACKET_SIZE;
  ptSystemInfo->ulDeviceNumber = ptTLInfo->tDevice.tDeviceInfo.ulDeviceNumber;
  ptSystemInfo->ulSerialNumber = ptTLInfo->tDevice.tDeviceInfo.ulSerialNumber;
  ptSystemInfo->ulOpenCnt      = 1;

  return CIFX_NO_ERROR;
}

/*! **************************************************************************
* Reads channel info
*   \param  hChannel       Handle
*   \param  ulChannel      Number of channel
*   \param  ptChannelInfo  Pointer to channel information structure
*   \return CIFX_NO_ERROR on success
******************************************************************************/
int32_t GenerateChannelInfoByDeviceHandle( CIFXHANDLE hChannel, uint32_t ulChannel, CHANNEL_INFORMATION* ptChannelInfo)
{
  int32_t                  lRet        = CIFX_NO_ERROR;
  NETX_COMMON_STATUS_BLOCK tStatus     = {0};
  NETX_CONTROL_BLOCK       tControl    = {0};
  PCIFX_CHANNEL_HANDLE_T   ptChannel   = (PCIFX_CHANNEL_HANDLE_T)hChannel;
  PRCX_PACKET_TL_INFO_T    ptTLInfo    = netXTransportGetTLDeviceData( ptChannel->hNetxTransportDeviceHandle);
  uint32_t                 ulNetxFlags = 0;
  uint32_t                 ulHostFlags = 0;

  /* get device name */
  netXTransportGetDeviceName( ptChannel->hNetxTransportDeviceHandle, ptChannelInfo->abBoardName, sizeof(ptChannelInfo->abBoardName));

  ptChannelInfo->ulDeviceNumber = ptTLInfo->tDevice.tSystemInfo.ulDeviceNumber;   /* Global board device number     */
  ptChannelInfo->ulSerialNumber = ptTLInfo->tDevice.tSystemInfo.ulSerialNumber;   /* Global board serial number     */
  if (ptTLInfo->tDevice.aptChannels[ulChannel] != NULL)
  {
    ptChannelInfo->usFWMajor      = ptTLInfo->tDevice.aptChannels[ulChannel]->tFirmwareInfo.tFwVersion.usMajor;     /* Major Version of Channel Firmware  */
    ptChannelInfo->usFWMinor      = ptTLInfo->tDevice.aptChannels[ulChannel]->tFirmwareInfo.tFwVersion.usMinor;     /* Minor Version of Channel Firmware  */
    ptChannelInfo->usFWBuild      = ptTLInfo->tDevice.aptChannels[ulChannel]->tFirmwareInfo.tFwVersion.usBuild;     /* Build number of Channel Firmware   */
    ptChannelInfo->usFWRevision   = ptTLInfo->tDevice.aptChannels[ulChannel]->tFirmwareInfo.tFwVersion.usRevision;  /* Revision of Channel Firmware       */
    ptChannelInfo->bFWNameLength  = ptTLInfo->tDevice.aptChannels[ulChannel]->tFirmwareInfo.tFwName.bNameLength;    /* Length  of FW Name                 */

    /* get firmware name */
    OS_Memcpy( ptChannelInfo->abFWName,
               ptTLInfo->tDevice.aptChannels[ulChannel]->tFirmwareInfo.tFwName.abName,
               sizeof(ptChannelInfo->abFWName));

    ptChannelInfo->usFWYear         = ptTLInfo->tDevice.aptChannels[ulChannel]->tFirmwareInfo.tFwDate.usYear;       /* Build Year of Firmware             */
    ptChannelInfo->bFWMonth         = ptTLInfo->tDevice.aptChannels[ulChannel]->tFirmwareInfo.tFwDate.bMonth;       /* Build Month of Firmware (1..12)    */
    ptChannelInfo->bFWDay           = ptTLInfo->tDevice.aptChannels[ulChannel]->tFirmwareInfo.tFwDate.bDay;         /* Build Day of Firmware (1..31)      */
    ptChannelInfo->ulChannelError   = 0;                                               /* Channel error                      */
    ptChannelInfo->ulOpenCnt        = 1;
    ptChannelInfo->ulMailboxSize    = CIFX_MAX_PACKET_SIZE;
  } else
  {
    lRet = CIFX_NO_MORE_ENTRIES;
  }
  /* Read communication COS flags */
  if (CIFX_NO_ERROR == ReadChannelCommonStatusBlock( hChannel, 0, 0, sizeof(tStatus), &tStatus))
  {
    ptChannelInfo->ulDeviceCOSFlags = tStatus.ulCommunicationCOS;
  }

  /* Read applicaton COS flags */
  if (CIFX_NO_ERROR == ReadChannelControlBlock( hChannel, 0, 0, sizeof(tControl), &tControl))
  {
    ptChannelInfo->ulHostCOSFlags = tControl.ulApplicationCOS;
  }

  /* Read actual netX and host communication flags */
  ReadComFlags( hChannel, &ulNetxFlags, &ulHostFlags);

  ptChannelInfo->ulNetxFlags = ulNetxFlags;
  ptChannelInfo->ulHostFlags = ulHostFlags;

  return lRet;
}

/*! **************************************************************************
* Generates channel information
*   \param  ptTLInfo       Pointer to TL device info
*   \param  ulChannel      Number of channel
*   \param  ptChannelInfo  Pointer to channel information structure
*   \return CIFX_NO_ERROR on success
******************************************************************************/
int32_t GenerateChannelInfo( PRCX_PACKET_TL_INFO_T ptTLInfo, uint32_t ulChannel, CHANNEL_INFORMATION* ptChannelInfo)
{
  int32_t lRet = CIFX_NO_ERROR;

  if (ulChannel >= ptTLInfo->tDevice.ulChannelCount)
    return CIFX_NO_MORE_ENTRIES;

  /* get device name */
  OS_Strncpy( ptChannelInfo->abBoardName,
              sizeof(ptChannelInfo->abBoardName),
              ptTLInfo->tDevice.szDevicename,
              OS_Strlen(ptTLInfo->tDevice.szDevicename));  /* Global board name              */

  ptChannelInfo->ulDeviceNumber = ptTLInfo->tDevice.tSystemInfo.ulDeviceNumber;   /* Global board device number     */
  ptChannelInfo->ulSerialNumber = ptTLInfo->tDevice.tSystemInfo.ulSerialNumber;   /* Global board serial number     */
  if (ptTLInfo->tDevice.aptChannels[ulChannel] != NULL)
  {
    ptChannelInfo->usFWMajor      = ptTLInfo->tDevice.aptChannels[ulChannel]->tFirmwareInfo.tFwVersion.usMajor;     /* Major Version of Channel Firmware  */
    ptChannelInfo->usFWMinor      = ptTLInfo->tDevice.aptChannels[ulChannel]->tFirmwareInfo.tFwVersion.usMinor;     /* Minor Version of Channel Firmware  */
    ptChannelInfo->usFWBuild      = ptTLInfo->tDevice.aptChannels[ulChannel]->tFirmwareInfo.tFwVersion.usBuild;     /* Build number of Channel Firmware   */
    ptChannelInfo->usFWRevision   = ptTLInfo->tDevice.aptChannels[ulChannel]->tFirmwareInfo.tFwVersion.usRevision;  /* Revision of Channel Firmware       */
    ptChannelInfo->bFWNameLength  = ptTLInfo->tDevice.aptChannels[ulChannel]->tFirmwareInfo.tFwName.bNameLength;    /* Length  of FW Name                 */

    /* get firmware name */
    OS_Memcpy( ptChannelInfo->abFWName,
               ptTLInfo->tDevice.aptChannels[ulChannel]->tFirmwareInfo.tFwName.abName,
               sizeof(ptChannelInfo->abFWName));

    ptChannelInfo->usFWYear         = ptTLInfo->tDevice.aptChannels[ulChannel]->tFirmwareInfo.tFwDate.usYear;       /* Build Year of Firmware             */
    ptChannelInfo->bFWMonth         = ptTLInfo->tDevice.aptChannels[ulChannel]->tFirmwareInfo.tFwDate.bMonth;       /* Build Month of Firmware (1..12)    */
    ptChannelInfo->bFWDay           = ptTLInfo->tDevice.aptChannels[ulChannel]->tFirmwareInfo.tFwDate.bDay;         /* Build Day of Firmware (1..31)      */
    ptChannelInfo->ulChannelError   = 0;                                               /* Channel error                      */
    ptChannelInfo->ulOpenCnt        = 1;
    ptChannelInfo->ulMailboxSize    = CIFX_MAX_PACKET_SIZE;
  } else
  {
    lRet = CIFX_NO_MORE_ENTRIES;
  }

  return lRet;
}

/*****************************************************************************/
/*! Translation-Layer initialization function. Checks the translation layer is available on remote end
*   \param hTransport       Handle to the Transport-Layer
*   \param phTLHandle       On success handle to the initialized translation layer
*   \param pulDeviceNumber  On success number of detected device
*   \param ptServerInfo     Pointer to server information
*   \return NXT_NO_ERROR on success                                         */
/*****************************************************************************/
int32_t APIENTRY rcXPacket_Probe(NXTHANDLE hTransport, NXTHANDLE* phTLHandle, uint32_t* pulDeviceNumber, TRANSPORT_SERVER_INFO_T* ptServerInfo)
{
  /* Use device number zero, cause only one device supported on interface at this time*/
  PRCX_PACKET_TL_INFO_T ptTLInfo       = OS_Memalloc(sizeof(RCX_PACKET_TL_INFO_T));
  int32_t               lRet           = NXT_OUT_OF_MEMORY;

  UNREFERENCED_PARAMETER( ptServerInfo);

  *phTLHandle = NULL;

  if(NULL != ptTLInfo)
  {
    PHIL_TRANSPORT_PACKET_T ptSendPacket = HilTransportAllocatePacket(hTransport, RCX_MAX_PACKET_SIZE, HIL_TRANSPORT_TYPE_RCX_PACKET);

    OS_Memset(ptTLInfo, 0, sizeof(*ptTLInfo));
    ptTLInfo->hTransport = hTransport;

    /* handle initialization for device ulDeviceNumber */
    if(NULL != ptSendPacket)
    {
      if(NXT_NO_ERROR != (lRet = ReadHWIdentify( ptTLInfo, ptSendPacket)))
      {
        /* TODO: Trace error */
      } else
      {
        uint32_t ulAreaIndex;

        for(ulAreaIndex = 0; (ulAreaIndex < NETX_MAX_SUPPORTED_CHANNELS); ulAreaIndex++)
        {
          CreateChannelInfo( ptTLInfo, ptSendPacket, ulAreaIndex);
        }

        /* Read additional informations. This command may not supported on older bootloader versions */
        ReadSysInfoBlock(ptTLInfo, ptSendPacket, &ptTLInfo->tDevice.tSystemInfo);

        *pulDeviceNumber = 1;
        *phTLHandle      = ptTLInfo;
        *pulDeviceNumber = 1;
      }
      HilTransportFreePacket(ptSendPacket);
    }
    if(NXT_NO_ERROR != lRet)
    {
      OS_Memfree(ptTLInfo);
    }

  }
  return lRet;
}

/*****************************************************************************/
/*! De-initializes rcX-Packet Translation-Layer
*   \param hTLHandle  Handle to Translation-Layer instance                   */
/*****************************************************************************/
void APIENTRY rcXPacket_Release( NXTHANDLE hTLHandle)
{
  PRCX_PACKET_TL_INFO_T ptTLInfo = (PRCX_PACKET_TL_INFO_T)hTLHandle;

  if (ptTLInfo != NULL)
  {
    uint32_t ulAreaIndex;

    for(ulAreaIndex = 0; (ulAreaIndex < ptTLInfo->tDevice.ulChannelCount); ulAreaIndex++)
    {
      if (ptTLInfo->tDevice.aptChannels[ulAreaIndex])
      {
        if (ptTLInfo->tDevice.aptChannels[ulAreaIndex]->hRecvSemaphore != NULL)
          OS_DeleteSemaphore(ptTLInfo->tDevice.aptChannels[ulAreaIndex]->hRecvSemaphore);

        if (ptTLInfo->tDevice.aptChannels[ulAreaIndex]->pvOpenCloseLock != NULL)
          OS_DeleteLock(ptTLInfo->tDevice.aptChannels[ulAreaIndex]->pvOpenCloseLock);

        OS_Memfree(ptTLInfo->tDevice.aptChannels[ulAreaIndex]);
      }
    }
    OS_Memfree( ptTLInfo);
  }
}

/*****************************************************************************/
/*! Translation-Layer restore function. Function is called in case of a reconnect to
    restore all active connections.
*   \param hNetXTransportDeviceHandle  Handle to the netXTransport device    */
/*****************************************************************************/
static void APIENTRY rcXPacket_RestoreConnectionStates( NXTHANDLE hNetXTransportDeviceHandle)
{
  UNREFERENCED_PARAMETER( hNetXTransportDeviceHandle);

  /* in case of the packet translation layer we can skip this step */
}

/*****************************************************************************/
/*! Scan for device (ulDeviceNumber specifies device to scan for) and initialize all
*   the required components.
*   \param hTLHandle            Handle to the translation layer
*   \param szDeviceName         Buffer to device name
*   \param pulDeviceIdentifier  Private data
*   \param ppvPrivateData
*   \param ulDeviceNumber
*   \return NXT_NO_ERROR on success                                         */
/*****************************************************************************/
int32_t APIENTRY rcXPacket_DeviceScan( NXTHANDLE hTLHandle, char* szDeviceName, uint32_t* pulDeviceIdentifier, NXTHANDLE* ppvPrivateData, uint32_t ulDeviceNumber)
{
  PRCX_PACKET_TL_INFO_T ptTLDevInfo = (PRCX_PACKET_TL_INFO_T)hTLHandle;

  if(g_ulTraceLevel & TRACE_LEVEL_INFO)
  {
    USER_Trace( s_tRCXPacketLayer.hNetXTransport, TRACE_LEVEL_INFO, "Scanning for device %d (rcX-Packet Translation-Layer -0x100).", ulDeviceNumber);
  }

  /* since this translation layer supports only one device per interface, we can set the device private data to the global translation layer data */
  *ppvPrivateData = ptTLDevInfo;

  OS_Strncpy( szDeviceName, 16, "cifX0", 16);

  if (NULL == ptTLDevInfo)
  {
    return NXT_INVALID_PARAMETER;
  } else
  {
    /* unique device identification -> to be able to identify a reconnected device       */
    /* (the identifier needs to be unique per interface, so we can use the serial number */
    *pulDeviceIdentifier = ptTLDevInfo->tDevice.tSystemInfo.ulSerialNumber;
    return NXT_NO_ERROR;
  }
}

/*****************************************************************************/
/*! Translation-Layer receive function. Queues incoming packets
*   \param hTLHandle handle to the translation layer
*   \param ptPacket Packet to process
*   \return NXT_NO_ERROR on success                                         */
/*****************************************************************************/
void APIENTRY rcXPacket_Receive(NXTHANDLE hTLHandle, PHIL_TRANSPORT_PACKET_T ptPacket)
{
  /* Use device number zero, cause only one device supported on interface at this time*/
  PRCX_PACKET_TL_INFO_T ptTLInfo  = (PRCX_PACKET_TL_INFO_T)hTLHandle;

  if( (ptPacket->ptHeader->bChannel < sizeof(ptTLInfo->tDevice.aptChannels) / sizeof(ptTLInfo->tDevice.aptChannels[0])) &&
       (ptTLInfo->tDevice.aptChannels[ptPacket->ptHeader->bChannel] != NULL) )
  {
    PRCX_PACKET_CHANNEL_T ptChannel = ptTLInfo->tDevice.aptChannels[ptPacket->ptHeader->bChannel];
    TAILQ_INSERT_TAIL(&ptChannel->tRecvPackets, ptPacket, tList);
    OS_PutSemaphore(ptChannel->hRecvSemaphore, 1);
  } else
  {
    HilTransportFreePacket(ptPacket);
  }
}

/*****************************************************************************/
/*! Function returns pointer to function table (Translation-Layer API interface)
*   \param pfnFunctionPointerTable structure to the function pointer table
*   \return NXT_NO_ERROR on success                                         */
/*****************************************************************************/
int32_t APIENTRY rcXPacket_MapFunctionPointerTable( FUNCTION_POINTER_TABLE_T* pfnFunctionPointerTable)
{
  uint32_t lRet = 0;

  if (pfnFunctionPointerTable)
  {
    CIFX_DRV_FUNCTION_TABLE_T* ptrcXConfig = (CIFX_DRV_FUNCTION_TABLE_T*)OS_Memalloc(sizeof(CIFX_DRV_FUNCTION_TABLE_T));

    OS_Memset(ptrcXConfig, 0, sizeof(CIFX_DRV_FUNCTION_TABLE_T));

    /* driver functions */
    ptrcXConfig->pfnxDriverOpen                 = rcXDriverOpen;
    ptrcXConfig->pfnxDriverClose                = rcXDriverClose;
    ptrcXConfig->pfnxDriverGetInformation       = rcXDriverGetInformation;
    ptrcXConfig->pfnxDriverGetErrorDescription  = rcXDriverGetErrorDescription;
    ptrcXConfig->pfnxDriverEnumBoards           = rcXDriverEnumBoards;
    ptrcXConfig->pfnxDriverEnumChannels         = rcXDriverEnumChannels;
    ptrcXConfig->pfnxDriverMemoryPointer        = rcXDriverMemoryPointer;

    /* systme device functions */
    ptrcXConfig->pfnxSysdeviceOpen              = rcXSysdeviceOpen;
    ptrcXConfig->pfnxSysdeviceClose             = rcXSysdeviceClose;
    ptrcXConfig->pfnxSysdeviceReset             = rcXSysdeviceReset;
    ptrcXConfig->pfnxSysdeviceGetMBXState       = rcXSysdeviceGetMBXState;
    ptrcXConfig->pfnxSysdevicePutPacket         = rcXSysdevicePutPacket;
    ptrcXConfig->pfnxSysdeviceGetPacket         = rcXSysdeviceGetPacket;
    ptrcXConfig->pfnxSysdeviceDownload          = rcXSysdeviceDownload;
    ptrcXConfig->pfnxSysdeviceInfo              = rcXSysdeviceInfo;
    ptrcXConfig->pfnxSysdeviceFindFirstFile     = rcXSysdeviceFindFirstFile;
    ptrcXConfig->pfnxSysdeviceFindNextFile      = rcXSysdeviceFindNextFile;

    /* channel functions */
    ptrcXConfig->pfnxChannelOpen                = rcXChannelOpen;
    ptrcXConfig->pfnxChannelClose               = rcXChannelClose;
    ptrcXConfig->pfnxChannelDownload            = rcXChannelDownload;
    ptrcXConfig->pfnxChannelGetMBXState         = rcXChannelGetMBXState;
    ptrcXConfig->pfnxChannelPutPacket           = rcXChannelPutPacket;
    ptrcXConfig->pfnxChannelGetPacket           = rcXChannelGetPacket;
    ptrcXConfig->pfnxChannelGetSendPacket       = rcXChannelGetSendPacket;
    ptrcXConfig->pfnxChannelConfigLock          = rcXChannelConfigLock;
    ptrcXConfig->pfnxChannelReset               = rcXChannelReset;
    ptrcXConfig->pfnxChannelInfo                = rcXChannelInfo;
    ptrcXConfig->pfnxChannelWatchdog            = rcXChannelWatchdog;
    ptrcXConfig->pfnxChannelHostState           = rcXChannelHostState;
    ptrcXConfig->pfnxChannelBusState            = rcXChannelBusState;
    ptrcXConfig->pfnxChannelIORead              = rcXChannelIORead;
    ptrcXConfig->pfnxChannelIOWrite             = rcXChannelIOWrite;
    ptrcXConfig->pfnxChannelIOReadSendData      = rcXChannelIOReadSendData;
    ptrcXConfig->pfnxChannelControlBlock        = rcXChannelControlBlock;
    ptrcXConfig->pfnxChannelCommonStatusBlock   = rcXChannelCommonStatusBlock;
    ptrcXConfig->pfnxChannelExtendedStatusBlock = rcXChannelExtendedStatusBlock;
    ptrcXConfig->pfnxChannelPLCMemoryPtr        = rcXChannelPLCMemoryPtr;
    ptrcXConfig->pfnxChannelPLCIsReadReady      = rcXChannelPLCIsReadReady;
    ptrcXConfig->pfnxChannelPLCIsWriteReady     = rcXChannelPLCIsWriteReady;
    ptrcXConfig->pfnxChannelPLCActivateWrite    = rcXChannelPLCActivateWrite;
    ptrcXConfig->pfnxChannelPLCActivateRead     = rcXChannelPLCActivateRead;
    ptrcXConfig->pfnxChannelFindFirstFile       = rcXChannelFindFirstFile;
    ptrcXConfig->pfnxChannelFindNextFile        = rcXChannelFindNextFile;

    pfnFunctionPointerTable->ulVersionMajor     = RCXAPI_VERSION_MAJOR;
    pfnFunctionPointerTable->ulVersionMinor     = RCXAPI_VERSION_MINOR;
    pfnFunctionPointerTable->ulVersionRevision  = RCXAPI_VERSION_REV;
    pfnFunctionPointerTable->ulVersionBuild     = RCXAPI_VERSION_BUILD;

    pfnFunctionPointerTable->pvTranslationLayerFunctionTable = (void*)ptrcXConfig;
  } else
  {
    lRet = NXT_INVALID_POINTER;
  }
  return lRet;
}

/*****************************************************************************/
/*! Function initializes the translation layer (rcXPacket-> data type = 0x100) and adds
*   the initialized translation layer to the netXTransport layer
*   \return NXT_NO_ERROR on success                                         */
/*****************************************************************************/
int32_t APIENTRY rcXPacket_Init( void* pvParam)
{
  int32_t lRet = NXT_NO_ERROR;

  UNREFERENCED_PARAMETER( pvParam);

  s_tRCXPacketLayer.usDataType          = HIL_TRANSPORT_TYPE_RCX_PACKET;     /* supported data type of the translation layer                */
  s_tRCXPacketLayer.pfnProbe            = rcXPacket_Probe;                   /* called to check if data is supported and for initialization */
  s_tRCXPacketLayer.pfnRelease          = rcXPacket_Release;
  s_tRCXPacketLayer.pfnScan             = rcXPacket_DeviceScan;              /* called to check if data is supported and for initialization */
  s_tRCXPacketLayer.pfnRestoreCon       = rcXPacket_RestoreConnectionStates;
  s_tRCXPacketLayer.pfnReceive          = rcXPacket_Receive;                 /* called in case of incoming packets with the packet type HIL_TRANSPORT_TYPE_RCX_PACKET */
  s_tRCXPacketLayer.pfnMapFunctionTable = rcXPacket_MapFunctionPointerTable; /* table of API functions                                      */

  /* register marshaller translation layer */
  if (NXT_NO_ERROR == (lRet = netXTransportAddTranslationLayer(&s_tRCXPacketLayer, &s_tRCXPacketLayer.hNetXTransportDevGroup)))
    cifXAPIInit( s_tRCXPacketLayer.hNetXTransportDevGroup);

  return lRet;
}

/*****************************************************************************/
/*! Function de-registers translation layer
*   \return NXT_NO_ERROR on success                                         */
/*****************************************************************************/
void APIENTRY rcXPacket_DeInit( void* pvParam)
{
  UNREFERENCED_PARAMETER( pvParam);

  /* de-register rcXPacket translation layer */
  netXTransportRemoveTranslationLayer(s_tRCXPacketLayer.hNetXTransportDevGroup);
}
