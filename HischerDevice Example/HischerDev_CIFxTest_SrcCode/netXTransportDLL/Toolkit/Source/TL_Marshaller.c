/**************************************************************************************

   Copyright (c) Hilscher GmbH. All Rights Reserved.

 **************************************************************************************

   Filename:
    $Id: TL_Marshaller.c 169 2013-06-17 12:00:59Z sebastiand $
   Last Modification:
    $Author: sebastiand $
    $Date: 2013-06-17 14:00:59 +0200 (Mo, 17. Jun 2013) $
    $Revision: 169 $

   Targets:
     OS-Independent : yes

   Description:
    Implementation of the Marshaller Packet transfer (datatype = 0x200)

   Changes:

     Version   Date        Author   Description
     ----------------------------------------------------------------------------------
      1         23.02.13    SD       initial version

**************************************************************************************/

/****************************************************************************/
/*! \file TL_Marshaller.c
    Implementation of the cifX Marshaller Translation-Layer (datatype = 0x200)  */
/****************************************************************************/

#include <OS_Includes.h>
#include <OS_Dependent.h>
#include "netXTransport.h"
#include "HilTransportLayer.h"
#include "rcX_User.h"
#include "rcX_Public.h"
#include "cifXErrors.h"
#include "cifXAPI.h"
#include "cifXUser.h"
#include "MarshallerFrame.h"
#include "TL_Marshaller.h"


/*****************************************************************************/
/*! cifX API Marshaller version information                                  */
/*****************************************************************************/
#define CIFXAPI_VERSION_MAJOR 1  /*!< Version major    */
#define CIFXAPI_VERSION_MINOR 0  /*!< Version minor    */
#define CIFXAPI_VERSION_BUILD 0  /*!< Version build    */
#define CIFXAPI_VERSION_REV   0  /*!< Version revision */


/*****************************************************************************/
/*! cifX API Marshaller definitions                                          */
/*****************************************************************************/
#define CHANNEL_TYPE_XSYSDEVICE 0  /*!< Channel type: 'sysdevice'    */
#define CHANNEL_TYPE_CHANNEL    1  /*!< Channel type: 'channel'      */

/*****************************************************************************/
/*! cifX Marshaller Macro definitions                                        */
/*****************************************************************************/
#define CHECK_REMOTE_HANDLE(handle) if ((((PMARSH_CHANNEL_INFO_T)(handle))->lHandleState)!= CIFX_NO_ERROR) return ((PMARSH_CHANNEL_INFO_T)(handle))->lHandleState; /*!< Verifies given handle */

/*****************************************************************************/
/*! cifX API Marshaller global variables                                     */
/*****************************************************************************/
uint32_t                     g_ulVersion = 0;          /*!< Version information            */
static   NETX_TL_INTERFACE_T s_tcifXMarshallerLayer;   /*!< TL cifX Marshaller information */

TAILQ_HEAD ( HANDLE_LIST, MARSH_CHANNEL_INFO_Ttag);     /*!< List of active handles */
static struct HANDLE_LIST s_tChannelHandleList;         /*!< List of active handles */

/*****************************************************************************/
/*! generic functions                                                        */
/*****************************************************************************/
static int32_t TransferPacket      ( NXTHANDLE hTransport, MARSHALLER_DATA_FRAME_HEADER_T* ptSendPacket , MARSHALLER_DATA_FRAME_HEADER_T** pptRecvPacket, uint32_t ulTimeout);
static void    GenerateHeader      ( PMARSHALLER_DATA_FRAME_HEADER_T ptHeader, uint32_t ulHandle, uint32_t ulMethodId, int fRequest, uint32_t ulDataSize, int fSequenceSupport, uint16_t usSeqNr);
static int32_t VerifyTransferResult( PMARSHALLER_DATA_FRAME_HEADER_T ptReq, PMARSHALLER_DATA_FRAME_HEADER_T ptCnf);

/* marshaller specific functions */
static int32_t QueryMarshallerVersion( NXTHANDLE hTransport);
static int32_t CreateObject          ( NXTHANDLE hTransport, uint32_t ulDestHandle, uint32_t ulObjectType, uint32_t* pulObjectHandle);

/* driver specific functions */
static int32_t EnumChannel( MARSHALLER_TL_T* ptTLInfo, uint32_t ulBoard, uint32_t ulChannel, uint32_t ulSize, void* pvChannelInfo);
static int32_t EnumDevice ( MARSHALLER_TL_T* ptTLInfo, uint32_t ulBoard, uint32_t ulSize, void* pvBoardInfo);

static int32_t OpenChannel ( CIFXHANDLE hDriver, char* szDevice, uint32_t ulChannelNr, CIFXHANDLE* phChannel);
static int32_t CloseChannel( PMARSH_CHANNEL_INFO_T ptChannel, int fSendRequest /* = true */, int fForceRemove /* = false */);

static int32_t MethodeInfo( PMARSH_CHANNEL_INFO_T ptDevice, uint32_t ulCmd, uint32_t ulSize, void* pvInfo, uint32_t ulType);
static int32_t GetMbxState( PMARSH_CHANNEL_INFO_T ptChannel, uint32_t* pulRecvCnt, uint32_t* pulSendCnt);
static int32_t PutPacket  ( PMARSH_CHANNEL_INFO_T ptChannel, CIFX_PACKET* ptPacket, uint32_t ulTimeout);
static int32_t GetPacket  ( PMARSH_CHANNEL_INFO_T ptChannel, CIFX_PACKET* ptPacket, uint32_t ulPacketLen, uint32_t ulTimeout);
static int32_t Reset      ( PMARSH_CHANNEL_INFO_T ptDevice, uint32_t ulResetMode, uint32_t ulType, uint32_t ulTimeout);
static int32_t ChannelReadWriteBlock( PMARSH_CHANNEL_INFO_T ptChannel, uint32_t ulCmd, uint32_t ulOffset, uint32_t ulDataLen, void* pvData, uint32_t ulMethodID);

/*! **************************************************************************
* Opens the Driver
*   \param phDriver Returned handle to the driver
*   \return CIFX_NO_ERROR on success
******************************************************************************/
static int32_t MsDriverOpen( MARSHALLER_TL_T* ptInfo)
{
  int32_t                        lRet          = CIFX_NO_ERROR;
  uint8_t*                       pbRecvData    = NULL;
  MARSHALLER_DATA_FRAME_HEADER_T tDriverOpen   = {{{0}}};

  /* not yet initialized */
  if (ptInfo->tDriverInfo.ulDriverHandle == 0)
    return CIFX_INVALID_HANDLE;

  GenerateHeader(&tDriverOpen,
                 ptInfo->tDriverInfo.ulDriverHandle,
                 MARSHALLER_DRV_METHODID_OPEN,
                 1,
                 0,
                 ptInfo->tDriverInfo.fSequenceNrSupport,
                 ptInfo->tDriverInfo.usSequenceNr);

  lRet = TransferPacket( ptInfo->hTransport, &tDriverOpen, (PMARSHALLER_DATA_FRAME_HEADER_T*)&pbRecvData, TRANSPORT_TO_TRANSFER);
  if(CIFX_NO_ERROR == lRet)
  {
    MARSHALLER_DATA_FRAME_HEADER_T* ptCnf = (MARSHALLER_DATA_FRAME_HEADER_T*)pbRecvData;

    lRet = VerifyTransferResult(&tDriverOpen, ptCnf);

    OS_Memfree(pbRecvData);
  }
  return lRet;
}

/*! **************************************************************************
* Opens the Driver
*   \param phDriver Returned handle to the driver
*   \return CIFX_NO_ERROR on success
******************************************************************************/
int32_t APIENTRY DriverOpen( CIFXHANDLE* hhDriver)
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
int32_t APIENTRY DriverClose(CIFXHANDLE hDriver)
{
  int32_t                        lRet          = CIFX_NO_ERROR;
  uint8_t*                       pbRecvData    = NULL;
  MARSHALLER_DATA_FRAME_HEADER_T tCloseDriver  = {{{0}}};
  MARSHALLER_TL_T*               ptInfo        = (MARSHALLER_TL_T*)hDriver;

  GenerateHeader( &tCloseDriver,
                  ptInfo->tDriverInfo.ulDriverHandle,
                  MARSHALLER_DRV_METHODID_CLOSE,
                  1,
                  0,
                  ptInfo->tDriverInfo.fSequenceNrSupport,
                  ptInfo->tDriverInfo.usSequenceNr);

  lRet = TransferPacket( ptInfo->hTransport, &tCloseDriver, (PMARSHALLER_DATA_FRAME_HEADER_T*)&pbRecvData, TRANSPORT_TO_TRANSFER);

  if(CIFX_NO_ERROR == lRet)
  {
    MARSHALLER_DATA_FRAME_HEADER_T* ptCnf = (MARSHALLER_DATA_FRAME_HEADER_T*)pbRecvData;

    if ( CIFX_NO_ERROR == (lRet = VerifyTransferResult(&tCloseDriver, ptCnf)) )
    {
      ptInfo->tDriverInfo.ulDriverHandle = 0;
    }

    OS_Memfree(pbRecvData);
  }
  return lRet;
}

/*! **************************************************************************
* Restart a device
*   \param hDriver      Driver handle, if NULL the driver will be temporarily opened by the API
*   \param szBoardName  Board name
*   \param pvData
*   \return CIFX_NO_ERROR on success
******************************************************************************/
int32_t APIENTRY DriverRestartDevice(CIFXHANDLE  hDriver, char* szBoardName, void* pvData)
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
int32_t APIENTRY DriverGetInformation( CIFXHANDLE hDriver,
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
int32_t APIENTRY DriverEnumBoards( CIFXHANDLE  hDriver,
                          uint32_t    ulBoard,
                          uint32_t    ulSize,
                          void*       pvBoardInfo)
{
  int32_t   lRet                    = CIFX_INVALID_PARAMETER;
  NXTHANDLE hnetXMarshallerInstance = hDriver;
  NXTHANDLE hDeviceHandle           = NULL;

  if (NULL != (hDeviceHandle = netXTransportGetDeviceHandleByNumber( hnetXMarshallerInstance, ulBoard)))
  {
    MARSHALLER_ENDPOINT_DATA_T* ptTLEndpointInfo;

    if (1 == netXTransportAttachDevice( hDeviceHandle))
    {
      if (NULL != (ptTLEndpointInfo = (MARSHALLER_ENDPOINT_DATA_T*)netXTransportGetTLDeviceData( hDeviceHandle)))
      {
        BOARD_INFORMATION* ptBoardInfo = pvBoardInfo;

        OS_Memcpy( ptBoardInfo, &ptTLEndpointInfo->tBoardInfo.tBoardInfo, ulSize);
        netXTransportGetDeviceName( hDeviceHandle, ptBoardInfo->abBoardName, sizeof(ptBoardInfo->abBoardName));

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
int32_t APIENTRY DriverEnumChannels( CIFXHANDLE hDriver,
                            uint32_t   ulBoard,
                            uint32_t   ulChannel,
                            uint32_t   ulSize,
                            void*      pvChannelInfo)
{
  int32_t   lRet                    = CIFX_NO_ERROR;
  NXTHANDLE hnetXMarshallerInstance = hDriver;
  NXTHANDLE hDeviceHandle           = NULL;

  if (NULL != (hDeviceHandle = netXTransportGetDeviceHandleByNumber( hnetXMarshallerInstance, ulBoard)))
  {
    MARSHALLER_TL_T* ptTLInfo;

    if (1 == netXTransportAttachDevice( hDeviceHandle))
    {
      if (NULL != (ptTLInfo = (MARSHALLER_TL_T*)netXTransportGetTLInfo( hDeviceHandle)))
      {
        CHANNEL_INFORMATION tChannelInfo = {{0}};

        lRet = CIFX_NO_ERROR;
        if (CIFX_NO_ERROR == (lRet = EnumChannel( ptTLInfo, ulBoard, ulChannel, sizeof(tChannelInfo), &tChannelInfo)))
        {
          if (ulSize>sizeof(tChannelInfo))
          {
            ulSize = sizeof(tChannelInfo);
          }
          OS_Memcpy( pvChannelInfo, &tChannelInfo, ulSize);
        }
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
int32_t APIENTRY DriverMemoryPointer(CIFXHANDLE  hDriver,
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
int32_t APIENTRY DriverGetErrorDescription(int32_t  lError,
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
int32_t APIENTRY SysdeviceOpen( CIFXHANDLE  hDriver,
                       char*       szBoard,
                       CIFXHANDLE* phSysdevice)
{
  int32_t lRet = CIFX_INVALID_PARAMETER;

  if (NULL != hDriver)
  {
    lRet = OpenChannel( hDriver, szBoard, CIFX_SYSTEM_DEVICE, phSysdevice);
  }

  return lRet;
}

/*! **************************************************************************
* Closed an open System device
*   \param hSysdevice  Handle to the System device to close
*   \return CIFX_NO_ERROR on success
******************************************************************************/
int32_t APIENTRY SysdeviceClose(CIFXHANDLE hSysdevice)
{
  int32_t               lRet      = CIFX_NO_ERROR;
  MARSH_CHANNEL_INFO_T* ptChannel = (MARSH_CHANNEL_INFO_T*)hSysdevice;

  CHECK_REMOTE_HANDLE( ptChannel);

  lRet = CloseChannel(ptChannel,1,1);

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
int32_t APIENTRY SysdeviceInfo( CIFXHANDLE hSysdevice,
                                uint32_t   ulCmd,
                                uint32_t   ulSize,
                                void*      pvInfo)
{
  PMARSH_CHANNEL_INFO_T ptSysDevice = (PMARSH_CHANNEL_INFO_T)hSysdevice;

  CHECK_REMOTE_HANDLE( ptSysDevice);

  return MethodeInfo( ptSysDevice, ulCmd, ulSize, pvInfo, CHANNEL_TYPE_XSYSDEVICE);
}

/*! **************************************************************************
* Gets the Mailbox state of an open system device
*   \param hSysdevice      Handle to the System device
*   \param pulRecvPktCount Number of packets in receive mailbox
*   \param pulSendPktCount Number of packets the application is able to send at once
*   \return CIFX_NO_ERROR on success
******************************************************************************/
int32_t APIENTRY SysdeviceGetMBXState( CIFXHANDLE hSysdevice,
                                       uint32_t*  pulRecvPktCount,
                                       uint32_t*  pulSendPktCount)
{
  PMARSH_CHANNEL_INFO_T ptSysDevice = (PMARSH_CHANNEL_INFO_T)hSysdevice;

  CHECK_REMOTE_HANDLE( ptSysDevice);

  return GetMbxState( ptSysDevice, pulRecvPktCount, pulSendPktCount);
}

/*! **************************************************************************
* Inserts a packet into the System Mailbox
*   \param hSysdevice      Handle to the System device
*   \param ptSendPkt       Packet to send to device
*   \param ulTimeout       maximum time to wait for packet to be accepted by device (in ms)
*   \return CIFX_NO_ERROR on success
******************************************************************************/
int32_t APIENTRY SysdevicePutPacket( CIFXHANDLE   hSysdevice,
                            CIFX_PACKET* ptSendPkt,
                            uint32_t     ulTimeout)
{
  PMARSH_CHANNEL_INFO_T ptSysDevice = (PMARSH_CHANNEL_INFO_T)hSysdevice;

  CHECK_REMOTE_HANDLE( ptSysDevice);

  return PutPacket( ptSysDevice, ptSendPkt, ulTimeout);
}

/*! **************************************************************************
* Retrieves a packet from the System Mailbox
*   \param hSysdevice      Handle to the System device
*   \param ulSize          Size of the buffer to retrieve the packet
*   \param ptRecvPkt       Pointer to buffer for received packet
*   \param ulTimeout       maximum time to wait for packet to be delivered by device (in ms)
*   \return CIFX_NO_ERROR on success
******************************************************************************/
int32_t APIENTRY SysdeviceGetPacket( CIFXHANDLE   hSysdevice,
                            uint32_t     ulSize,
                            CIFX_PACKET* ptRecvPkt,
                            uint32_t     ulTimeout)
{
  PMARSH_CHANNEL_INFO_T ptSysDevice = (PMARSH_CHANNEL_INFO_T)hSysdevice;

  CHECK_REMOTE_HANDLE( ptSysDevice);

  return GetPacket( ptSysDevice, ptRecvPkt, ulSize, ulTimeout);
}

/*! **************************************************************************
* Reset the whole device (coldstart)
*   \param hSysdevice   Sysdevice handle acquired by xSysdeviceOpen
*   \param ulTimeout    Time in ms to wait for the reset
*   \return CIFX_NO_ERROR on success
******************************************************************************/
int32_t APIENTRY SysdeviceReset( CIFXHANDLE hSysdevice,
                                 uint32_t   ulTimeout)
{
  /* Get Marshaller channel handling structure, if handle is valid */
  PMARSH_CHANNEL_INFO_T ptSysDevice = (PMARSH_CHANNEL_INFO_T)hSysdevice;

  CHECK_REMOTE_HANDLE( ptSysDevice);

  return Reset( ptSysDevice, 0, CHANNEL_TYPE_XSYSDEVICE, ulTimeout);
}

/*! **************************************************************************
* Download a file (Firmware, Configuratio, etc) to the device
*   \param hSysdevice           Handle to the system device
*   \param ulChannel            Channel number to load the file to
*   \param ulMode               Download mode (DOWNLOAD_MODE_FIRMWARE, etc)
*   \param szFileName           Name of the file
*   \param pabFileData          Pointer to the file data
*   \param ulFileSize           Length of the file data
*   \param ptcifXTransportInst  Pointer to netXTransport Device Instance
*   \param pfnCallback          Callback for progress indication (NULL for no callback)
*   \param pfnRecvPktCallback   Callback pointer for unhandled receive packets
*   \param pvUser               User parameter on callback.
*   \return CIFX_NO_ERROR on success
******************************************************************************/
int32_t APIENTRY SysdeviceDownload( CIFXHANDLE                    hSysdevice,
                                    uint32_t                      ulChannel,
                                    uint32_t                      ulMode,
                                    char*                         szFileName,
                                    uint8_t*                      pabFileData,
                                    uint32_t                      ulFileSize,
                                    CIFX_TRANSPORT_DEVICE_INST_T* ptcifXTransportInst,
                                    PFN_PROGRESS_CALLBACK         pfnCallback,
                                    PFN_RECV_PKT_CALLBACK         pfnRecvPktCallback,
                                    void*                         pvUser)
{
  int32_t               lRet        = CIFX_NO_ERROR;
  PMARSH_CHANNEL_INFO_T ptSysDevice = (PMARSH_CHANNEL_INFO_T)hSysdevice;

  CHECK_REMOTE_HANDLE( ptSysDevice);

  if(ptSysDevice->fOpen > 0)
  {
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
int32_t APIENTRY SysdeviceFindFirstFile( CIFXHANDLE            hSysdevice,
                                         uint32_t              ulChannel,
                                         CIFX_DIRECTORYENTRY*  ptDirectoryInfo,
                                         PFN_RECV_PKT_CALLBACK pfnRecvPktCallback,
                                         void*                 pvUser)
{
  int32_t                     lRet        = CIFX_INVALID_HANDLE;
  uint8_t*                    pbRecvData  = NULL;
  PMARSH_CHANNEL_INFO_T       ptSysDevice = (PMARSH_CHANNEL_INFO_T)hSysdevice;
  MARSHALLER_ENDPOINT_DATA_T* ptDLData    = netXTransportGetTLDeviceData( ptSysDevice->ptnetXTransportDev->hNetXTransportDeviceHandle);

  CHECK_REMOTE_HANDLE( ptSysDevice);

  if (ptSysDevice->ulChannelNr == CIFX_SYSTEM_DEVICE)
  {
    SYSDEV_FIND_FIRSTFILE_REQ_T tFindFirst = {{{{0}}}};

    GenerateHeader( &tFindFirst.tHeader,
                    ptSysDevice->dwRemoteHandle,
                    MARSHALLER_SYSDEV_METHODID_FINDFIRSTFILE,
                    1,
                    sizeof(SYSDEV_FIND_FIRSTFILE_REQ_DATA_T),
                    ptDLData->tDriverInfo.fSequenceNrSupport,
                    ptDLData->tDriverInfo.usSequenceNr);

    /* NOTE: This implementation works only on 32bit machines */
    tFindFirst.tData.hList              = PTR2LONG(ptDirectoryInfo->hList);
    tFindFirst.tData.ulFilesize         = ptDirectoryInfo->ulFilesize;
    tFindFirst.tData.bFiletype          = ptDirectoryInfo->bFiletype;
    tFindFirst.tData.ulChannel          = ulChannel;
    tFindFirst.tData.ulRecvPktCallback  = PTR2LONG(pfnRecvPktCallback);
    tFindFirst.tData.ulUser             = PTR2LONG(pvUser);
    OS_Memcpy( &tFindFirst.tData.szFilename, &ptDirectoryInfo->szFilename, sizeof(ptDirectoryInfo->szFilename));

    lRet = TransferPacket( ptDLData->hTransport, (PMARSHALLER_DATA_FRAME_HEADER_T)&tFindFirst, (PMARSHALLER_DATA_FRAME_HEADER_T*)&pbRecvData, TRANSPORT_TO_TRANSFER);
    if(CIFX_NO_ERROR == lRet)
    {
      PSYSDEV_FIND_FIRSTFILE_CNF_T ptCnf = (PSYSDEV_FIND_FIRSTFILE_CNF_T)pbRecvData;

      if ( CIFX_NO_ERROR == (lRet = VerifyTransferResult(&tFindFirst.tHeader, &ptCnf->tHeader)) )
      {
        /* Note: this implementation works only on 32 bit machines */
        ptDirectoryInfo->hList        = (CIFXHANDLE)LONG2PTR(ptCnf->tData.hList);
        ptDirectoryInfo->ulFilesize   = ptCnf->tData.ulFilesize;
        ptDirectoryInfo->bFiletype    = ptCnf->tData.bFiletype;
        OS_Memset( &ptDirectoryInfo->szFilename, 0, sizeof(ptDirectoryInfo->szFilename));
        OS_Memcpy( &ptDirectoryInfo->szFilename, &ptCnf->tData.szFilename, sizeof(ptDirectoryInfo->szFilename));
      }
      OS_Memfree(pbRecvData);
    }
  } else
  {
    CHANNEL_FIND_FIRSTFILE_REQ_T  tFindFirst = {{{{0}}}};

    GenerateHeader( &tFindFirst.tHeader,
                    ptSysDevice->dwRemoteHandle,
                    MARSHALLER_CHANNEL_METHODID_FINDFIRSTFILE,
                    1,
                    sizeof(CHANNEL_FIND_FIRSTFILE_REQ_DATA_T),
                    ptDLData->tDriverInfo.fSequenceNrSupport,
                    ptDLData->tDriverInfo.usSequenceNr);

    /* NOTE: This implementation works only on 32bit machines */
    tFindFirst.tData.hList              = PTR2LONG(ptDirectoryInfo->hList);
    tFindFirst.tData.ulFilesize         = ptDirectoryInfo->ulFilesize;
    tFindFirst.tData.bFiletype          = ptDirectoryInfo->bFiletype;
    tFindFirst.tData.ulRecvPktCallback  = PTR2LONG(pfnRecvPktCallback);
    tFindFirst.tData.ulUser             = PTR2LONG(pvUser);
    OS_Memcpy( &tFindFirst.tData.szFilename, &ptDirectoryInfo->szFilename, sizeof(ptDirectoryInfo->szFilename));

    lRet = TransferPacket( ptDLData->hTransport, (PMARSHALLER_DATA_FRAME_HEADER_T)&tFindFirst, (PMARSHALLER_DATA_FRAME_HEADER_T*)&pbRecvData, TRANSPORT_TO_TRANSFER);

    if(CIFX_NO_ERROR == lRet)
    {
      PCHANNEL_FIND_FIRSTFILE_CNF_T ptCnf = (PCHANNEL_FIND_FIRSTFILE_CNF_T)pbRecvData;

      if ( CIFX_NO_ERROR == (lRet = VerifyTransferResult(&tFindFirst.tHeader, &ptCnf->tHeader)) )
      {
        /* Note: this implementation works only on 32 bit machines */
        ptDirectoryInfo->hList        = (CIFXHANDLE) LONG2PTR(ptCnf->tData.hList);
        ptDirectoryInfo->ulFilesize   = ptCnf->tData.ulFilesize;
        ptDirectoryInfo->bFiletype    = ptCnf->tData.bFiletype;
        OS_Memset( &ptDirectoryInfo->szFilename, 0, sizeof(ptDirectoryInfo->szFilename));
        OS_Memcpy( &ptDirectoryInfo->szFilename, &ptCnf->tData.szFilename, sizeof(ptDirectoryInfo->szFilename));
      }
      OS_Memfree(pbRecvData);
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
int32_t APIENTRY SysdeviceFindNextFile( CIFXHANDLE            hSysdevice,
                                        uint32_t              ulChannel,
                                        CIFX_DIRECTORYENTRY*  ptDirectoryInfo,
                                        PFN_RECV_PKT_CALLBACK pfnRecvPktCallback,
                                        void*                 pvUser)
{
  int32_t                     lRet          = CIFX_INVALID_HANDLE;
  uint8_t*                    pbRecvData    = NULL;
  PMARSH_CHANNEL_INFO_T       ptSysDevice   = (PMARSH_CHANNEL_INFO_T)hSysdevice;
  MARSHALLER_ENDPOINT_DATA_T* ptDLData      = netXTransportGetTLDeviceData( ptSysDevice->ptnetXTransportDev->hNetXTransportDeviceHandle);

  CHECK_REMOTE_HANDLE( ptSysDevice);

  if (ptSysDevice->ulChannelNr == CIFX_SYSTEM_DEVICE)
  {
    SYSDEV_FIND_NEXTFILE_REQ_T  tFindNext     = {{{{0}}}};

    GenerateHeader( &tFindNext.tHeader,
                    ptSysDevice->dwRemoteHandle,
                    MARSHALLER_SYSDEV_METHODID_FINDNEXTFILE,
                    1,
                    sizeof(SYSDEV_FIND_NEXTFILE_REQ_DATA_T),
                    ptDLData->tDriverInfo.fSequenceNrSupport,
                    ptDLData->tDriverInfo.usSequenceNr);

    /* NOTE: This implementation works only on 32bit machines */
    tFindNext.tData.hList              = PTR2LONG(ptDirectoryInfo->hList);
    tFindNext.tData.ulFilesize         = ptDirectoryInfo->ulFilesize;
    tFindNext.tData.bFiletype          = ptDirectoryInfo->bFiletype;
    tFindNext.tData.ulChannel          = ulChannel;
    tFindNext.tData.ulRecvPktCallback  = PTR2LONG(pfnRecvPktCallback);
    tFindNext.tData.ulUser             = PTR2LONG(pvUser);
    OS_Memcpy( &tFindNext.tData.szFilename, &ptDirectoryInfo->szFilename, sizeof(ptDirectoryInfo->szFilename));

    lRet = TransferPacket( ptDLData->hTransport, (PMARSHALLER_DATA_FRAME_HEADER_T)&tFindNext, (PMARSHALLER_DATA_FRAME_HEADER_T*)&pbRecvData, TRANSPORT_TO_TRANSFER);
    if(CIFX_NO_ERROR == lRet)
    {
      PSYSDEV_FIND_NEXTFILE_CNF_T ptCnf = (PSYSDEV_FIND_NEXTFILE_CNF_T)pbRecvData;

      if ( CIFX_NO_ERROR == (lRet = VerifyTransferResult(&tFindNext.tHeader, &ptCnf->tHeader)) )
      {
        /* Note: this implementation works only on 32 bit machines */
        ptDirectoryInfo->hList        = (CIFXHANDLE)LONG2PTR(ptCnf->tData.hList);
        ptDirectoryInfo->ulFilesize   = ptCnf->tData.ulFilesize;
        ptDirectoryInfo->bFiletype    = ptCnf->tData.bFiletype ;
        OS_Memset( &ptDirectoryInfo->szFilename, 0, sizeof(ptDirectoryInfo->szFilename));
        OS_Memcpy( &ptDirectoryInfo->szFilename, &ptCnf->tData.szFilename, sizeof(ptDirectoryInfo->szFilename));
      }
      OS_Memfree(pbRecvData);
    }
  } else
  {
    CHANNEL_FIND_NEXTFILE_REQ_T tFindNext = {{{{0}}}};

    GenerateHeader( &tFindNext.tHeader,
                    ptSysDevice->dwRemoteHandle,
                    MARSHALLER_CHANNEL_METHODID_FINDNEXTFILE,
                    1,
                    sizeof(CHANNEL_FIND_NEXTFILE_REQ_DATA_T),
                    ptDLData->tDriverInfo.fSequenceNrSupport,
                    ptDLData->tDriverInfo.usSequenceNr);

    /* NOTE: This implementation works only on 32bit machines */
    tFindNext.tData.hList              = PTR2LONG(ptDirectoryInfo->hList);
    tFindNext.tData.ulFilesize         = ptDirectoryInfo->ulFilesize;
    tFindNext.tData.bFiletype          = ptDirectoryInfo->bFiletype;
    tFindNext.tData.ulRecvPktCallback  = PTR2LONG(pfnRecvPktCallback);
    tFindNext.tData.ulUser             = PTR2LONG(pvUser);
    OS_Memcpy( &tFindNext.tData.szFilename, &ptDirectoryInfo->szFilename, sizeof(ptDirectoryInfo->szFilename));

    lRet = TransferPacket( ptDLData->hTransport, (PMARSHALLER_DATA_FRAME_HEADER_T)&tFindNext, (PMARSHALLER_DATA_FRAME_HEADER_T*)&pbRecvData, TRANSPORT_TO_TRANSFER);

    if(CIFX_NO_ERROR == lRet)
    {
      PCHANNEL_FIND_NEXTFILE_CNF_T ptCnf = (PCHANNEL_FIND_NEXTFILE_CNF_T)pbRecvData;

      if ( CIFX_NO_ERROR == (lRet = VerifyTransferResult(&tFindNext.tHeader, &ptCnf->tHeader)) )
      {
        /* Note: this implementation works only on 32 bit machines */
        ptDirectoryInfo->hList        = (CIFXHANDLE)LONG2PTR(ptCnf->tData.hList);
        ptDirectoryInfo->ulFilesize   = ptCnf->tData.ulFilesize;
        ptDirectoryInfo->bFiletype    = ptCnf->tData.bFiletype ;
        OS_Memset( &ptDirectoryInfo->szFilename, 0, sizeof(ptDirectoryInfo->szFilename));
        OS_Memcpy( &ptDirectoryInfo->szFilename, &ptCnf->tData.szFilename, sizeof(ptDirectoryInfo->szFilename));
      }
      OS_Memfree(pbRecvData);
    }
  }
  return lRet;
}

/*****************************************************************************/
/*! Uploads a file via system channel
*   \param hSysdevice           Handle to the System device
*   \param ulChannel            Channel number to get directory from
*   \param ulMode               Transfer Mode
*   \param szFileName           Filename to upload
*   \param pabFileData          Pointer to buffer receiving upload
*   \param pulFileSize          [in]Length of buffer, [out] Bytes copied to buffer
*   \param ptcifXTransportInst  pointer to netXTransport Device Instance
*   \param pfnCallback          Callback pointer for progress
*   \param pfnRecvPktCallback   Callback pointer for unhandled receive packets
*   \param pvUser               User parameter on callback.
*   \return CIFX_NO_ERROR on success                                         */
/*****************************************************************************/
int32_t SysdeviceUpload( CIFXHANDLE                     hSysdevice,
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
  int32_t               lRet        = CIFX_NO_ERROR;
  PMARSH_CHANNEL_INFO_T ptSysDevice = (PMARSH_CHANNEL_INFO_T)hSysdevice;

  CHECK_REMOTE_HANDLE( ptSysDevice);

  if(ptSysDevice->fOpen > 0)
  {
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
  }else
  {
    lRet = CIFX_INVALID_HANDLE;
  }
  return lRet;
}

/*****************************************************************************/
/*! Boot start reset to via system channel
*   \param hSysdevice Handle to system device
*   \param ulTimeout  Timeout to wait for card to finish reset
*   \return CIFX_NO_ERROR on success                                         */
/*****************************************************************************/
int32_t SysdeviceBootstart(CIFXHANDLE hSysdevice, uint32_t ulTimeout)
{
  int32_t lRet = CIFX_FUNCTION_NOT_AVAILABLE;

  UNREFERENCED_PARAMETER(hSysdevice);
  UNREFERENCED_PARAMETER(ulTimeout);

  return lRet;
}

/*****************************************************************************/
/*! Get/Return a memory pointer to an extended board memory if available
*   \param hSysdevice   Handle to system device
*   \param ulCmd        Command for get/free
*   \param ptExtMemInfo Pointer to a user buffer to return the information
*   \return CIFX_NO_ERROR on success                                         */
/*****************************************************************************/
int32_t SysdeviceExtendedMemory(CIFXHANDLE hSysdevice, uint32_t ulCmd, CIFX_EXTENDED_MEMORY_INFORMATION* ptExtMemInfo)
{
  int32_t lRet = CIFX_FUNCTION_NOT_AVAILABLE;

  UNREFERENCED_PARAMETER(hSysdevice);
  UNREFERENCED_PARAMETER(ulCmd);
  UNREFERENCED_PARAMETER(ptExtMemInfo);

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
int32_t APIENTRY ChannelOpen( CIFXHANDLE  hDriver,
                              char*       szBoard,
                              uint32_t    ulChannel,
                              CIFXHANDLE* phChannel)
{
  int32_t lRet = CIFX_INVALID_BOARD;

  if (NULL != hDriver)
  {
    lRet = OpenChannel( hDriver, szBoard, ulChannel, phChannel);
  }

  return lRet;
}

/*! **************************************************************************
* Closes an open channel
*   \param hChannel Channel handle acquired by xChannelOpen
*   \return CIFX_NO_ERROR on success
******************************************************************************/
int32_t APIENTRY ChannelClose(CIFXHANDLE hChannel)
{
  int32_t               lRet      = CIFX_NO_ERROR;
  MARSH_CHANNEL_INFO_T* ptChannel = (MARSH_CHANNEL_INFO_T* )hChannel;

  CHECK_REMOTE_HANDLE( ptChannel);

  /* Interlocked Decrement */
  lRet = CloseChannel(ptChannel,1,1);

  return lRet;
}

/*! **************************************************************************
* Reset the channel
*   \param hChannel     Channel handle acquired by xChannelOpen
*   \param ulResetMode  Cold/Warmstart selection
*   \param ulTimeout    Time in ms to wait for the reset
*   \return CIFX_NO_ERROR on success
******************************************************************************/
int32_t APIENTRY ChannelReset( CIFXHANDLE hChannel,
                               uint32_t   ulResetMode,
                               uint32_t   ulTimeout)
{
  /* Get Marshaller channel handling structure, if handle is valid */
  PMARSH_CHANNEL_INFO_T ptChannel = (PMARSH_CHANNEL_INFO_T)hChannel;

  CHECK_REMOTE_HANDLE( ptChannel);

  return Reset( ptChannel, ulResetMode, CHANNEL_TYPE_CHANNEL, ulTimeout);
}

/*! **************************************************************************
* Returns Channel specific information (same information as from Channel enumeration)
*   \param hChannel       Channel handle acquired by xChannelOpen
*   \param ulSize         Size of the user buffer
*   \param pvChannelInfo  Returned channel specific information
*   \return CIFX_NO_ERROR on success
******************************************************************************/
int32_t APIENTRY ChannelInfo( CIFXHANDLE hChannel,
                              uint32_t   ulSize,
                              void*      pvChannelInfo)
{
  int32_t lRet = CIFX_NO_ERROR;

  CHECK_REMOTE_HANDLE( hChannel);

  if(ulSize < sizeof(CHANNEL_INFORMATION))
  {
    lRet = CIFX_INVALID_BUFFERSIZE;
  } else
  {
    /* Get Marshaller channel handling structure, if handle is valid */
    PMARSH_CHANNEL_INFO_T ptChannel = (PMARSH_CHANNEL_INFO_T)hChannel;

    /* Use MethodInfo, by signaling a channel */
    lRet = MethodeInfo( ptChannel, 0, ulSize, pvChannelInfo, CHANNEL_TYPE_CHANNEL);
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
int32_t APIENTRY ChannelGetMBXState( CIFXHANDLE hChannel,
                                     uint32_t*  pulRecvPktCount,
                                     uint32_t*  pulSendMbxState)
{
  PMARSH_CHANNEL_INFO_T ptChannel = (PMARSH_CHANNEL_INFO_T)hChannel;

  CHECK_REMOTE_HANDLE( ptChannel);

  return GetMbxState( ptChannel, pulRecvPktCount, pulSendMbxState);
}

/*! **************************************************************************
* Inserts a packet into a channel mailbox
*   \param hChannel   Channel handle acquired by xChannelOpen
*   \param ptSendPkt  Packet to send to channel
*   \param ulTimeout  Time in ms to wait for card to accept the packet
*   \return CIFX_NO_ERROR on success
******************************************************************************/
int32_t APIENTRY ChannelPutPacket( CIFXHANDLE   hChannel,
                                   CIFX_PACKET* ptSendPkt,
                                   uint32_t     ulTimeout)
{
  PMARSH_CHANNEL_INFO_T ptChannel = (PMARSH_CHANNEL_INFO_T)hChannel;

  CHECK_REMOTE_HANDLE( ptChannel);

  return PutPacket(ptChannel, ptSendPkt, ulTimeout);
}

/*! **************************************************************************
* Gets a packet from a channel mailbox
*   \param hChannel   Channel handle acquired by xChannelOpen
*   \param ulSize     Size of returned packet
*   \param ptRecvPkt  Returned packet
*   \param ulTimeout  Time in ms to wait for available packets
*   \return CIFX_NO_ERROR on success
******************************************************************************/
int32_t APIENTRY ChannelGetPacket( CIFXHANDLE   hChannel,
                                   uint32_t     ulSize,
                                   CIFX_PACKET* ptRecvPkt,
                                   uint32_t     ulTimeout)
{
  PMARSH_CHANNEL_INFO_T ptChannel = (PMARSH_CHANNEL_INFO_T)hChannel;

  CHECK_REMOTE_HANDLE( ptChannel);

  return GetPacket( ptChannel, ptRecvPkt, ulSize, ulTimeout);
}

/*! **************************************************************************
* Read back a packet from a channel send mailbox
*   \param hChannel   Channel handle acquired by xChannelOpen
*   \param ulSize     Size of returned packet
*   \param ptRecvPkt  Returned packet
*   \return CIFX_NO_ERROR on success
******************************************************************************/
int32_t APIENTRY ChannelGetSendPacket( CIFXHANDLE   hChannel,
                                       uint32_t     ulSize,
                                       CIFX_PACKET* ptRecvPkt)
{
  int32_t                      lRet          = CIFX_INVALID_HANDLE;
  uint8_t*                     pbRecvData    = NULL;
  CHANNEL_GET_SENDPACKET_REQ_T tGetSendPkt   = {{{{0}}}};
  PMARSH_CHANNEL_INFO_T        ptChannel     = (PMARSH_CHANNEL_INFO_T)hChannel;
  MARSHALLER_ENDPOINT_DATA_T*  ptDLData      = netXTransportGetTLDeviceData( ptChannel->ptnetXTransportDev->hNetXTransportDeviceHandle);

  CHECK_REMOTE_HANDLE( ptChannel);

  GenerateHeader( &tGetSendPkt.tHeader,
                  ptChannel->dwRemoteHandle,
                  MARSHALLER_CHANNEL_METHODID_GETSENDPACKET,
                  1,
                  sizeof(CHANNEL_GET_SENDPACKET_REQ_DATA_T),
                  ptDLData->tDriverInfo.fSequenceNrSupport,
                  ptDLData->tDriverInfo.usSequenceNr);

  tGetSendPkt.tData.ulSize    = ulSize;

  lRet = TransferPacket( ptDLData->hTransport, (PMARSHALLER_DATA_FRAME_HEADER_T)&tGetSendPkt, (PMARSHALLER_DATA_FRAME_HEADER_T*)&pbRecvData, TRANSPORT_TO_TRANSFER);
  if(CIFX_NO_ERROR == lRet)
  {
    PMARSHALLER_DATA_FRAME_HEADER_T ptMarshHeader = (PMARSHALLER_DATA_FRAME_HEADER_T)pbRecvData;
    CIFX_PACKET*                    ptPacket      = (CIFX_PACKET*)(ptMarshHeader + 1);

    uint32_t ulCopySize = ptMarshHeader->ulDataSize;

    if ( CIFX_NO_ERROR == (lRet = VerifyTransferResult(&tGetSendPkt.tHeader, ptMarshHeader)) )
    {
      if(ulSize < ulCopySize)
      {
        ulCopySize = ulSize;
        lRet = CIFX_BUFFER_TOO_SHORT;
      }
      OS_Memcpy(ptRecvPkt, ptPacket, ulCopySize);
    }
    OS_Memfree(pbRecvData);
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
int32_t APIENTRY ChannelWatchdog( CIFXHANDLE hChannel,
                                  uint32_t   ulCmd,
                                  uint32_t*  pulTrigger)
{
  int32_t                     lRet          = CIFX_INVALID_HANDLE;
  PMARSH_CHANNEL_INFO_T       ptChannel     = (PMARSH_CHANNEL_INFO_T)hChannel;
  uint8_t*                    pbRecvData    = NULL;
  CHANNEL_WATCHDOG_REQ_T      tWatchdogREQ  = {{{{0}}}};
  MARSHALLER_ENDPOINT_DATA_T* ptDLData      = netXTransportGetTLDeviceData( ptChannel->ptnetXTransportDev->hNetXTransportDeviceHandle);

  CHECK_REMOTE_HANDLE( ptChannel);

  tWatchdogREQ.tData.ulCmd      = ulCmd;
  tWatchdogREQ.tData.ulTrigger  = *pulTrigger;

  GenerateHeader((PMARSHALLER_DATA_FRAME_HEADER_T) &tWatchdogREQ,
                  ptChannel->dwRemoteHandle,
                  MARSHALLER_CHANNEL_METHODID_WATCHDOG,
                  1,
                  sizeof(CHANNEL_WATCHDOG_REQ_DATA_T),
                  ptDLData->tDriverInfo.fSequenceNrSupport,
                  ptDLData->tDriverInfo.usSequenceNr);

  lRet = TransferPacket( ptDLData->hTransport, (PMARSHALLER_DATA_FRAME_HEADER_T)&tWatchdogREQ, (PMARSHALLER_DATA_FRAME_HEADER_T*)&pbRecvData, TRANSPORT_TO_TRANSFER);

  if(CIFX_NO_ERROR == lRet)
  {
    PCHANNEL_WATCHDOG_CNF_T ptCnf = (PCHANNEL_WATCHDOG_CNF_T)pbRecvData;

    if ( CIFX_NO_ERROR == (lRet = VerifyTransferResult(&tWatchdogREQ.tHeader, &ptCnf->tHeader)) )
    {
      *pulTrigger = ptCnf->tData.ulTrigger;
    }
    OS_Memfree(pbRecvData);
  }
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
int32_t APIENTRY ChannelHostState( CIFXHANDLE hChannel,
                                   uint32_t   ulCmd,
                                   uint32_t*  pulState,
                                   uint32_t   ulTimeout)
{
  int32_t                     lRet          = CIFX_INVALID_HANDLE;
  PMARSH_CHANNEL_INFO_T       ptChannel     = (PMARSH_CHANNEL_INFO_T)hChannel;
  uint8_t*                    pbRecvData    = NULL;
  CHANNEL_HOSTSTATE_REQ_T     tHostStateREQ = {{{{0}}}};
  MARSHALLER_ENDPOINT_DATA_T* ptDLData      = netXTransportGetTLDeviceData( ptChannel->ptnetXTransportDev->hNetXTransportDeviceHandle);

  CHECK_REMOTE_HANDLE( ptChannel);

  tHostStateREQ.tData.ulCmd      = ulCmd;
  tHostStateREQ.tData.ulTimeout  = ulTimeout;
  if(NULL != pulState)
  tHostStateREQ.tData.ulState    = *pulState;

  GenerateHeader((PMARSHALLER_DATA_FRAME_HEADER_T) &tHostStateREQ,
                  ptChannel->dwRemoteHandle,
                  MARSHALLER_CHANNEL_METHODID_HOSTSTATE,
                  1,
                  sizeof(CHANNEL_HOSTSTATE_REQ_DATA_T),
                  ptDLData->tDriverInfo.fSequenceNrSupport,
                  ptDLData->tDriverInfo.usSequenceNr);

  lRet = TransferPacket( ptDLData->hTransport, (PMARSHALLER_DATA_FRAME_HEADER_T)&tHostStateREQ, (PMARSHALLER_DATA_FRAME_HEADER_T*)&pbRecvData, ulTimeout);

  if(CIFX_NO_ERROR == lRet)
  {
    PCHANNEL_HOSTSTATE_CNF_T ptCnf = (PCHANNEL_HOSTSTATE_CNF_T)pbRecvData;

    if ( CIFX_NO_ERROR == (lRet = VerifyTransferResult(&tHostStateREQ.tHeader, &ptCnf->tHeader)) )
    {
      if(NULL != pulState)
      *pulState = ptCnf->tData.ulState;
    }

    OS_Memfree(pbRecvData);
  }
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
int32_t APIENTRY ChannelConfigLock( CIFXHANDLE hChannel,
                                    uint32_t   ulCmd,
                                    uint32_t*  pulState,
                                    uint32_t   ulTimeout)
{
  int32_t                     lRet           = CIFX_INVALID_HANDLE;
  PMARSH_CHANNEL_INFO_T       ptChannel      = (PMARSH_CHANNEL_INFO_T)hChannel;
  uint8_t*                    pbRecvData     = NULL;
  CHANNEL_CONFIG_LOCK_REQ_T   tConfigLockREQ = {{{{0}}}};
  MARSHALLER_ENDPOINT_DATA_T* ptDLData       = netXTransportGetTLDeviceData( ptChannel->ptnetXTransportDev->hNetXTransportDeviceHandle);

  CHECK_REMOTE_HANDLE( ptChannel);

  tConfigLockREQ.tData.ulCmd      = ulCmd;
  tConfigLockREQ.tData.ulState    = *pulState;
  tConfigLockREQ.tData.ulTimeout  = ulTimeout;

  GenerateHeader((PMARSHALLER_DATA_FRAME_HEADER_T) &tConfigLockREQ,
                  ptChannel->dwRemoteHandle,
                  MARSHALLER_CHANNEL_METHODID_CONFIGLOCK,
                  1,
                  sizeof(CHANNEL_CONFIG_LOCK_REQ_DATA_T),
                  ptDLData->tDriverInfo.fSequenceNrSupport,
                  ptDLData->tDriverInfo.usSequenceNr);

  lRet = TransferPacket( ptDLData->hTransport, (PMARSHALLER_DATA_FRAME_HEADER_T)&tConfigLockREQ, (PMARSHALLER_DATA_FRAME_HEADER_T*)&pbRecvData, ulTimeout);

  if(CIFX_NO_ERROR == lRet)
  {
     PCHANNEL_CONFIG_LOCK_CNF_T ptCnf = (PCHANNEL_CONFIG_LOCK_CNF_T)pbRecvData;

    if ( CIFX_NO_ERROR == (lRet = VerifyTransferResult(&tConfigLockREQ.tHeader, &ptCnf->tHeader)) )
    {
      *pulState = ptCnf->tData.ulState;
    }

    OS_Memfree(pbRecvData);
  }
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
int32_t APIENTRY ChannelBusState( CIFXHANDLE hChannel,
                                  uint32_t   ulCmd,
                                  uint32_t*  pulState,
                                  uint32_t   ulTimeout)
{
  int32_t                     lRet          = CIFX_INVALID_HANDLE;
  PMARSH_CHANNEL_INFO_T       ptChannel     = (PMARSH_CHANNEL_INFO_T)hChannel;
  uint8_t*                    pbRecvData    = NULL;
  CHANNEL_BUSSTATE_REQ_T      tBusStateREQ  = {{{{0}}}};
  MARSHALLER_ENDPOINT_DATA_T* ptDLData      = netXTransportGetTLDeviceData( ptChannel->ptnetXTransportDev->hNetXTransportDeviceHandle);

  CHECK_REMOTE_HANDLE( ptChannel);

  tBusStateREQ.tData.ulCmd      = ulCmd;
  tBusStateREQ.tData.ulTimeout  = ulTimeout;
  if(NULL != pulState)
  tBusStateREQ.tData.ulState    = *pulState;

  GenerateHeader((PMARSHALLER_DATA_FRAME_HEADER_T) &tBusStateREQ,
                  ptChannel->dwRemoteHandle,
                  MARSHALLER_CHANNEL_METHODID_BUSSTATE,
                  1,
                  sizeof(CHANNEL_BUSSTATE_REQ_DATA_T),
                  ptDLData->tDriverInfo.fSequenceNrSupport,
                  ptDLData->tDriverInfo.usSequenceNr);

  lRet = TransferPacket( ptDLData->hTransport, (PMARSHALLER_DATA_FRAME_HEADER_T)&tBusStateREQ, (PMARSHALLER_DATA_FRAME_HEADER_T*)&pbRecvData, ulTimeout);

  if(CIFX_NO_ERROR == lRet)
  {
    PCHANNEL_BUSSTATE_CNF_T ptCnf = (PCHANNEL_BUSSTATE_CNF_T)pbRecvData;

    if ( CIFX_NO_ERROR == (lRet = VerifyTransferResult(&tBusStateREQ.tHeader, &ptCnf->tHeader)) )
    {
      if(NULL != pulState)
      *pulState = ptCnf->tData.ulState;
    }
    OS_Memfree(pbRecvData);
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
int32_t APIENTRY ChannelDMAState(CIFXHANDLE hChannel, uint32_t ulCmd, uint32_t* pulState)
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
int32_t APIENTRY ChannelRegisterNotification( CIFXHANDLE  hChannel, uint32_t ulNotification, PFN_NOTIFY_CALLBACK  pfnCallback, void* pvUser)
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
int32_t APIENTRY ChannelUnregisterNotification( CIFXHANDLE  hChannel, uint32_t ulNotification)
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
int32_t APIENTRY ChannelSyncState( CIFXHANDLE  hChannel, uint32_t ulCmd, uint32_t ulTimeout, uint32_t* pulErrorCount)
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
int32_t APIENTRY ChannelIOInfo( CIFXHANDLE hChannel,
                                uint32_t   ulCmd,
                                uint32_t   ulAreaNumber,
                                uint32_t   ulSize,
                                void*      pvData)
{
  int32_t                     lRet          = CIFX_INVALID_HANDLE;
  PMARSH_CHANNEL_INFO_T       ptChannel     = (PMARSH_CHANNEL_INFO_T)hChannel;
  CHANNEL_IOINFO_REQ_T        tIOInfoREQ    = {{{{0}}}};
  uint8_t*                    pbRecvData    = NULL;
  MARSHALLER_ENDPOINT_DATA_T* ptDLData      = netXTransportGetTLDeviceData( ptChannel->ptnetXTransportDev->hNetXTransportDeviceHandle);

  CHECK_REMOTE_HANDLE( ptChannel);

  tIOInfoREQ.tData.ulCmd        = ulCmd;
  tIOInfoREQ.tData.ulArea       = ulAreaNumber;
  tIOInfoREQ.tData.ulDataLen    = ulSize;

  GenerateHeader((PMARSHALLER_DATA_FRAME_HEADER_T)&tIOInfoREQ.tHeader,
                  ptChannel->dwRemoteHandle,
                  MARSHALLER_CHANNEL_METHODID_IOINFO,
                  1,
                  sizeof(CHANNEL_IOINFO_REQ_DATA_T),
                  ptDLData->tDriverInfo.fSequenceNrSupport,
                  ptDLData->tDriverInfo.usSequenceNr);

  lRet = TransferPacket( ptDLData->hTransport, (PMARSHALLER_DATA_FRAME_HEADER_T)&tIOInfoREQ, (PMARSHALLER_DATA_FRAME_HEADER_T*)&pbRecvData, TRANSPORT_TO_TRANSFER);

  if(CIFX_NO_ERROR == lRet)
  {
    PMARSHALLER_DATA_FRAME_HEADER_T ptMarshHeader = (PMARSHALLER_DATA_FRAME_HEADER_T)pbRecvData;

    if ( CIFX_NO_ERROR == (lRet = VerifyTransferResult(&tIOInfoREQ.tHeader, ptMarshHeader)) )
    {
      /* Check the user buffer size and copy at least the length of the user buffer */
      uint32_t ulCopySize = ptMarshHeader->ulDataSize;
      if(ulSize < ulCopySize)
      {
        ulCopySize = ulSize;
        lRet       = CIFX_BUFFER_TOO_SHORT;
      }
      OS_Memcpy( pvData, ptMarshHeader + 1, ulCopySize);
    }
    OS_Memfree(pbRecvData);
  }
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
int32_t APIENTRY ChannelIORead( CIFXHANDLE hChannel,
                                uint32_t   ulAreaNumber,
                                uint32_t   ulOffset,
                                uint32_t   ulDataLen,
                                void*      pvData,
                                uint32_t   ulTimeout)
{
  int32_t                     lRet          = CIFX_INVALID_HANDLE;
  PMARSH_CHANNEL_INFO_T       ptChannel     = (PMARSH_CHANNEL_INFO_T)hChannel;
  CHANNEL_IOREAD_REQ_T        tIOReadREQ    = {{{{0}}}};
  uint8_t*                    pbRecvData    = NULL;
  MARSHALLER_ENDPOINT_DATA_T* ptDLData      = netXTransportGetTLDeviceData( ptChannel->ptnetXTransportDev->hNetXTransportDeviceHandle);

  CHECK_REMOTE_HANDLE( ptChannel);

  tIOReadREQ.tData.ulArea       = ulAreaNumber;
  tIOReadREQ.tData.ulOffset     = ulOffset;
  tIOReadREQ.tData.ulDataLen    = ulDataLen;
  tIOReadREQ.tData.ulTimeout    = ulTimeout;

  GenerateHeader((PMARSHALLER_DATA_FRAME_HEADER_T)&tIOReadREQ.tHeader,
                  ptChannel->dwRemoteHandle,
                  MARSHALLER_CHANNEL_METHODID_IOREAD,
                  1,
                  sizeof(CHANNEL_IOREAD_REQ_DATA_T),
                  ptDLData->tDriverInfo.fSequenceNrSupport,
                  ptDLData->tDriverInfo.usSequenceNr);

  lRet = TransferPacket( ptDLData->hTransport, (PMARSHALLER_DATA_FRAME_HEADER_T)&tIOReadREQ, (PMARSHALLER_DATA_FRAME_HEADER_T*)&pbRecvData, ulTimeout);

  if(CIFX_NO_ERROR == lRet)
  {
    PMARSHALLER_DATA_FRAME_HEADER_T ptMarshHeader = (PMARSHALLER_DATA_FRAME_HEADER_T)pbRecvData;

    lRet = VerifyTransferResult(&tIOReadREQ.tHeader, ptMarshHeader);
    if ( (CIFX_NO_ERROR == lRet) ||
         (CIFX_DEV_NO_COM_FLAG == lRet) )
    {
      /* Check the user buffer size and copy at least the length of the user buffer */
      uint32_t ulCopySize = ptMarshHeader->ulDataSize;
      if(ulDataLen < ulCopySize)
      {
        ulCopySize = ulDataLen;
        lRet       = CIFX_BUFFER_TOO_SHORT;
      }
      OS_Memcpy( pvData, ptMarshHeader + 1, ulCopySize);
    }
    OS_Memfree(pbRecvData);
  }
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
int32_t APIENTRY ChannelIOWrite( CIFXHANDLE hChannel,
                                 uint32_t   ulAreaNumber,
                                 uint32_t   ulOffset,
                                 uint32_t   ulDataLen,
                                 void*      pvWriteData,
                                 uint32_t   ulTimeout)
{
  int32_t                         lRet          = CIFX_INVALID_HANDLE;
  PMARSH_CHANNEL_INFO_T           ptChannel     = (PMARSH_CHANNEL_INFO_T)hChannel;
  PMARSHALLER_DATA_FRAME_HEADER_T ptHeader      = NULL;
  uint32_t                        ulRequestSize = 0;
  uint8_t*                        pbRecvData    = NULL;
  uint32_t                        ulSendOffset  = 0;
  uint8_t*                        pbData        = NULL;
  MARSHALLER_ENDPOINT_DATA_T*     ptDLData      = netXTransportGetTLDeviceData( ptChannel->ptnetXTransportDev->hNetXTransportDeviceHandle);

  CHECK_REMOTE_HANDLE( ptChannel);

  ulRequestSize = sizeof(*ptHeader)     +   /* Transport Header     */
                  sizeof(ulAreaNumber)  +   /* Area number          */
                  sizeof(ulOffset)      +   /* Offset               */
                  sizeof(ulTimeout)     +   /* Timeout in ms        */
                  sizeof(ulDataLen)     +   /* Length of data       */
                  ulDataLen;

  ptHeader = (PMARSHALLER_DATA_FRAME_HEADER_T)OS_Memalloc(ulRequestSize);
  OS_Memset(ptHeader, 0, ulRequestSize);

  GenerateHeader( ptHeader,
                  ptChannel->dwRemoteHandle,
                  MARSHALLER_CHANNEL_METHODID_IOWRITE,
                  1,
                  ulRequestSize - sizeof(*ptHeader),
                  ptDLData->tDriverInfo.fSequenceNrSupport,
                  ptDLData->tDriverInfo.usSequenceNr);


  ulSendOffset = 0;
  pbData   = (uint8_t*)(ptHeader + 1);

  OS_Memcpy(&pbData[ulSendOffset], &ulAreaNumber, sizeof(ulAreaNumber));
  ulSendOffset += sizeof(ulAreaNumber);

  OS_Memcpy(&pbData[ulSendOffset], &ulOffset, sizeof(ulOffset));
  ulSendOffset += sizeof(ulOffset);

  OS_Memcpy(&pbData[ulSendOffset], &ulTimeout, sizeof(ulTimeout));
  ulSendOffset += sizeof(ulTimeout);

  OS_Memcpy(&pbData[ulSendOffset], &ulDataLen, sizeof(ulDataLen));
  ulSendOffset += sizeof(ulDataLen);

  OS_Memcpy(&pbData[ulSendOffset], pvWriteData, ulDataLen);
  ulSendOffset += ulDataLen;

  lRet = TransferPacket( ptDLData->hTransport, (PMARSHALLER_DATA_FRAME_HEADER_T)ptHeader, (PMARSHALLER_DATA_FRAME_HEADER_T*)&pbRecvData, ulTimeout);

  if(CIFX_NO_ERROR == lRet)
  {
    PMARSHALLER_DATA_FRAME_HEADER_T ptMarshHeader = (PMARSHALLER_DATA_FRAME_HEADER_T)pbRecvData;

    if ( CIFX_NO_ERROR == (lRet = VerifyTransferResult(ptHeader, ptMarshHeader)) )
    {
      lRet = ptMarshHeader->ulError;
    }
    OS_Memfree(pbRecvData);
  }
  OS_Memfree(ptHeader);
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
int32_t APIENTRY ChannelIOReadSendData( CIFXHANDLE hChannel,
                                        uint32_t   ulAreaNumber,
                                        uint32_t   ulOffset,
                                        uint32_t   ulDataLen,
                                        void*      pvData)
{
  int32_t                      lRet          = CIFX_INVALID_HANDLE;
  PMARSH_CHANNEL_INFO_T        ptChannel     = (PMARSH_CHANNEL_INFO_T)hChannel;
  uint8_t*                     pbRecvData    = NULL;
  CHANNEL_IOREADSENDDATA_REQ_T tReadSendREQ  = {{{{0}}}};
  MARSHALLER_ENDPOINT_DATA_T*  ptDLData      = netXTransportGetTLDeviceData( ptChannel->ptnetXTransportDev->hNetXTransportDeviceHandle);

  CHECK_REMOTE_HANDLE( ptChannel);

  tReadSendREQ.tData.ulArea       = ulAreaNumber;
  tReadSendREQ.tData.ulOffset     = ulOffset;

  GenerateHeader((PMARSHALLER_DATA_FRAME_HEADER_T) &tReadSendREQ,
                  ptChannel->dwRemoteHandle,
                  MARSHALLER_CHANNEL_METHODID_IOREADSENDDATA,
                  1,
                  sizeof(CHANNEL_IOREADSENDDATA_REQ_T),
                  ptDLData->tDriverInfo.fSequenceNrSupport,
                  ptDLData->tDriverInfo.usSequenceNr);

  lRet = TransferPacket( ptDLData->hTransport, (PMARSHALLER_DATA_FRAME_HEADER_T)&tReadSendREQ, (PMARSHALLER_DATA_FRAME_HEADER_T*)&pbRecvData, TRANSPORT_TO_TRANSFER);

  if(CIFX_NO_ERROR == lRet)
  {
    PMARSHALLER_DATA_FRAME_HEADER_T ptMarshHeader = (PMARSHALLER_DATA_FRAME_HEADER_T)pbRecvData;

    lRet = VerifyTransferResult(&tReadSendREQ.tHeader, ptMarshHeader);
    if ( (CIFX_NO_ERROR == lRet) ||
         (CIFX_DEV_NO_COM_FLAG == lRet) )
    {
      /* Check the user buffer size and copy at least the length of the user buffer */
      uint32_t ulCopySize = ptMarshHeader->ulDataSize;
      if(ulDataLen < ulCopySize)
      {
        ulCopySize = ulDataLen;
        lRet       = CIFX_BUFFER_TOO_SHORT;
      }
      OS_Memcpy( pvData, ptMarshHeader + 1, ulCopySize);
    }
    OS_Memfree(pbRecvData);
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
int32_t APIENTRY ChannelControlBlock( CIFXHANDLE hChannel,
                                      uint32_t   ulCmd,
                                      uint32_t   ulOffset,
                                      uint32_t   ulDataLen,
                                      void*      pvData)
{
  PMARSH_CHANNEL_INFO_T ptChannel = (PMARSH_CHANNEL_INFO_T)hChannel;

  CHECK_REMOTE_HANDLE( ptChannel);

  return ChannelReadWriteBlock(ptChannel, ulCmd, ulOffset, ulDataLen, pvData, MARSHALLER_CHANNEL_METHODID_CONTROLBLOCK);
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
int32_t APIENTRY ChannelCommonStatusBlock( CIFXHANDLE hChannel,
                                           uint32_t   ulCmd,
                                           uint32_t   ulOffset,
                                           uint32_t   ulDataLen,
                                           void*      pvData)
{
  PMARSH_CHANNEL_INFO_T ptChannel = (PMARSH_CHANNEL_INFO_T)hChannel;

  CHECK_REMOTE_HANDLE( ptChannel);

  return ChannelReadWriteBlock(ptChannel, ulCmd, ulOffset, ulDataLen, pvData, MARSHALLER_CHANNEL_METHODID_STATUSBLOCK);
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
int32_t APIENTRY ChannelUserBlock( CIFXHANDLE hChannel,
                                   uint32_t   ulAreaNumber,
                                   uint32_t   ulCmd,
                                   uint32_t   ulOffset,
                                   uint32_t   ulDataLen,
                                   void*      pvData)
{
  int32_t lRet = CIFX_FUNCTION_NOT_AVAILABLE;

  UNREFERENCED_PARAMETER(hChannel);
  UNREFERENCED_PARAMETER(ulAreaNumber);
  UNREFERENCED_PARAMETER(ulCmd);
  UNREFERENCED_PARAMETER(ulOffset);
  UNREFERENCED_PARAMETER(ulDataLen);
  UNREFERENCED_PARAMETER(pvData);

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
int32_t APIENTRY ChannelExtendedStatusBlock( CIFXHANDLE hChannel,
                                             uint32_t   ulCmd,
                                             uint32_t   ulOffset,
                                             uint32_t   ulDataLen,
                                             void*      pvData)
{
  PMARSH_CHANNEL_INFO_T ptChannel = (PMARSH_CHANNEL_INFO_T)hChannel;

  CHECK_REMOTE_HANDLE( ptChannel);

  return ChannelReadWriteBlock(ptChannel, ulCmd, ulOffset, ulDataLen, pvData, MARSHALLER_CHANNEL_METHODID_EXTSTATUSBLOCK);
}

/*! **************************************************************************
* Get a Pointer to the I/O Area
*   \param hChannel       Channel handle acquired by xChannelOpen
*   \param ulCmd          Command
*   \param pvMemoryInfo   Returned I/O Area pointer
*   \return CIFX_NO_ERROR on success
******************************************************************************/
int32_t APIENTRY ChannelPLCMemoryPtr(CIFXHANDLE hChannel, uint32_t ulCmd, void* pvMemoryInfo)
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
int32_t APIENTRY ChannelPLCIsReadReady(CIFXHANDLE hChannel, uint32_t ulAreaNumber, uint32_t* pulReadState)
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
int32_t APIENTRY ChannelPLCIsWriteReady(CIFXHANDLE hChannel, uint32_t ulAreaNumber, uint32_t* pulWriteState)
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
int32_t APIENTRY ChannelPLCActivateWrite(CIFXHANDLE hChannel, uint32_t ulAreaNumber)
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
int32_t APIENTRY ChannelPLCActivateRead(CIFXHANDLE hChannel, uint32_t ulAreaNumber)
{
  int32_t lRet = CIFX_FUNCTION_NOT_AVAILABLE;

  UNREFERENCED_PARAMETER(hChannel);
  UNREFERENCED_PARAMETER(ulAreaNumber);

  return lRet;
}

/*! **************************************************************************
* Download a file (Firmware, Configuratio, etc) to the device
*   \param hChannel             Handle to the channel
*   \param ulMode               Download mode (DOWNLOAD_MODE_FIRMWARE, etc)
*   \param szFileName           Name of the file
*   \param pabFileData          Pointer to the file data
*   \param ulFileSize           Length of the file data
*   \param ptcifXTransportInst  pointer to netXTransport Device Instance
*   \param pfnCallback          Callback for progress indication (NULL for no callback)
*   \param pfnRecvPktCallback   Callback pointer for unhandled receive packets
*   \param pvUser               User parameter on callback.
*   \return CIFX_NO_ERROR on success
******************************************************************************/
int32_t APIENTRY ChannelDownload( CIFXHANDLE                     hChannel,
                                   uint32_t                      ulMode,
                                   char*                         szFileName,
                                   uint8_t*                      pabFileData,
                                   uint32_t                      ulFileSize,
                                   CIFX_TRANSPORT_DEVICE_INST_T* ptcifXTransportInst,
                                   PFN_PROGRESS_CALLBACK         pfnCallback,
                                   PFN_RECV_PKT_CALLBACK         pfnRecvPktCallback,
                                   void*                         pvUser)
{
  int32_t               lRet      = CIFX_NO_ERROR;
  PMARSH_CHANNEL_INFO_T ptChannel = (PMARSH_CHANNEL_INFO_T)hChannel;

  CHECK_REMOTE_HANDLE( ptChannel);

  if(ptChannel->fOpen > 0)
  {
    lRet = SysdeviceDownload( hChannel,
                              ptChannel->ulChannelNr,
                              ulMode,
                              szFileName,
                              pabFileData,
                              ulFileSize,
                              ptcifXTransportInst,
                              pfnCallback,
                              pfnRecvPktCallback,
                              pvUser);
  }
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
int32_t APIENTRY ChannelFindFirstFile( CIFXHANDLE            hChannel,
                                       CIFX_DIRECTORYENTRY*  ptDirectoryInfo,
                                       PFN_RECV_PKT_CALLBACK pfnRecvPktCallback,
                                       void*                 pvUser)
{
  int32_t               lRet      = CIFX_INVALID_HANDLE;
  PMARSH_CHANNEL_INFO_T ptChannel = (PMARSH_CHANNEL_INFO_T)hChannel;

  CHECK_REMOTE_HANDLE( ptChannel);

  if(ptChannel->fOpen > 0)
  {
    lRet = SysdeviceFindFirstFile( hChannel,
                                    ptChannel->ulChannelNr,
                                    ptDirectoryInfo,
                                    pfnRecvPktCallback,
                                    pvUser);
  }

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
int32_t APIENTRY ChannelFindNextFile( CIFXHANDLE             hChannel,
                                      CIFX_DIRECTORYENTRY*   ptDirectoryInfo,
                                      PFN_RECV_PKT_CALLBACK  pfnRecvPktCallback,
                                      void*                  pvUser)
{
  int32_t               lRet      = CIFX_INVALID_HANDLE;
  PMARSH_CHANNEL_INFO_T ptChannel = (PMARSH_CHANNEL_INFO_T)hChannel;

  CHECK_REMOTE_HANDLE( ptChannel);

  if(ptChannel->fOpen > 0)
  {
    lRet = SysdeviceFindNextFile( hChannel,
                                   ptChannel->ulChannelNr,
                                   ptDirectoryInfo,
                                   pfnRecvPktCallback,
                                   pvUser);
  }

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
int32_t APIENTRY ChannelUpload( CIFXHANDLE             hChannel,
                                uint32_t               ulMode,
                                char*                  szFileName,
                                uint8_t*               pabFileData,
                                uint32_t*              pulFileSize,
                                PFN_PROGRESS_CALLBACK  pfnCallback,
                                PFN_RECV_PKT_CALLBACK  pfnRecvPktCallback,
                                void*                  pvUser)
{
  int32_t               lRet      = CIFX_NO_ERROR;
  PMARSH_CHANNEL_INFO_T ptChannel = (PMARSH_CHANNEL_INFO_T)hChannel;

  CHECK_REMOTE_HANDLE( ptChannel);

  if(ptChannel->fOpen > 0)
  {
    lRet = xSysdeviceUpload(hChannel,
                            ptChannel->ulChannelNr,
                            ulMode,
                            szFileName,
                            pabFileData,
                            pulFileSize,
                            pfnCallback,
                            pfnRecvPktCallback,
                            pvUser);
  }else
  {
    lRet = CIFX_INVALID_HANDLE;
  }
  return lRet;
}

/****************************************************************************/
/****** GENERIC FUNCTIONS ***************************************************/
/****************************************************************************/

/*****************************************************************************/
/*! Opens a channel by name (Name can be obtained when enumerating Channels)
*   \param ptChannel  endpoint information
*   \param szBoard    Name of the board
*   \param ulChannel  Channel number
*   \return CIFX_NO_ERROR on success                                         */
/*****************************************************************************/
int32_t OpenChannelRequest( PMARSH_CHANNEL_INFO_T ptChannel, char *szDevice, uint32_t ulChannelNr)
{
  MARSHALLER_ENDPOINT_DATA_T*     ptDLData         = netXTransportGetTLDeviceData( ptChannel->ptnetXTransportDev->hNetXTransportDeviceHandle);
  int32_t                         lRet             = 0;
  uint32_t                        ulSendDataSize   = 0;
  PMARSHALLER_DATA_FRAME_HEADER_T ptHeader         = NULL;
  const uint32_t                  ulDeviceNameLen  = OS_Strlen(szDevice);
  uint8_t*                        pbRecvData       = NULL;

  if(ulChannelNr == CIFX_SYSTEM_DEVICE)
  {
    uint32_t ulOffset;
    uint8_t* pbData;

    ulSendDataSize = sizeof(uint32_t)     +    /* ulBoardNameSize          */
                          ulDeviceNameLen;       /* Boardname (NOT \0 terminated) */

    ptHeader = (PMARSHALLER_DATA_FRAME_HEADER_T)OS_Memalloc(sizeof(*ptHeader) + ulSendDataSize);

    ulOffset = 0;
    pbData   = (uint8_t*)(ptHeader + 1);

    OS_Memcpy( (void*)&pbData[ulOffset], (void*)&ulDeviceNameLen, sizeof(ulDeviceNameLen));
    ulOffset += sizeof(ulDeviceNameLen);

    OS_Memcpy( (void*)&pbData[ulOffset], (void*)szDevice, ulDeviceNameLen);
    ulOffset += ulDeviceNameLen;

  } else
  {
    uint32_t ulOffset;
    uint8_t* pbData;

    ulSendDataSize = sizeof(uint32_t)     +   /* ulBoardNameSize               */
                    ulDeviceNameLen   +       /* Boardname (NOT \0 terminated) */
                    sizeof(uint32_t);         /* Channel number                */

    ptHeader = (PMARSHALLER_DATA_FRAME_HEADER_T)(OS_Memalloc(sizeof(*ptHeader) + ulSendDataSize));

    ulOffset = 0;
    pbData   = (uint8_t*)(ptHeader + 1);

    OS_Memcpy( (void*)&pbData[ulOffset], (void*)&ulDeviceNameLen, sizeof(ulDeviceNameLen));
    ulOffset += sizeof(ulDeviceNameLen);

    OS_Memcpy( (void*)&pbData[ulOffset], (void*)szDevice, ulDeviceNameLen);
    ulOffset += ulDeviceNameLen;

    OS_Memcpy( (void*)&pbData[ulOffset], (void*)&ulChannelNr, sizeof(ulChannelNr));
    ulOffset += sizeof(ulChannelNr);
  }
  GenerateHeader( ptHeader,
                  ptChannel->hDriver,
                  (ulChannelNr == CIFX_SYSTEM_DEVICE) ? MARSHALLER_DRV_METHODID_OPENSYSDEV :
                                                        MARSHALLER_DRV_METHODID_OPENCHANNEL,
                  1,
                  ulSendDataSize,
                  ptDLData->tDriverInfo.fSequenceNrSupport,
                  ptDLData->tDriverInfo.usSequenceNr);

  lRet = TransferPacket( ptDLData->hTransport, ptHeader , (PMARSHALLER_DATA_FRAME_HEADER_T*)&pbRecvData, TRANSPORT_TO_TRANSFER);

  if(CIFX_NO_ERROR == lRet)
  {
    PCF_CREATEINSTANCE_CNF_T ptCnf = (PCF_CREATEINSTANCE_CNF_T)pbRecvData;

    if ( CIFX_NO_ERROR == (lRet = VerifyTransferResult(ptHeader, &ptCnf->tHeader)) )
    {
      ptChannel->dwRemoteHandle = ptCnf->tData.uHandle.ulHandle;
    }
    OS_Memfree(pbRecvData);
  }
  OS_Memfree(ptHeader);

  return lRet;
}

/*****************************************************************************/
/*! Opens a channel by name (Name can be obtained when enumerating Channels)
*   \param hDriver    Driver handle
*   \param szBoard    Name of the board
*   \param ulChannel  Channel number
*   \param phChannel  Returned handle to the channel (Needed for all channel specific operations)
*   \return CIFX_NO_ERROR on success                                         */
/*****************************************************************************/
static int32_t OpenChannel( CIFXHANDLE hDriver, char* szDevice, uint32_t ulChannelNr, CIFXHANDLE* phChannel)
{
  int32_t                       lRet                  = CIFX_FUNCTION_FAILED;
  CIFX_TRANSPORT_DEVICE_INST_T* ptnetXTransportDevice = (CIFX_TRANSPORT_DEVICE_INST_T*)hDriver;
  MARSHALLER_ENDPOINT_DATA_T*   ptDLData              = netXTransportGetTLDeviceData( ptnetXTransportDevice->hNetXTransportDeviceHandle);
  PMARSH_CHANNEL_INFO_T         ptChannel             = OS_Memalloc(sizeof(MARSH_CHANNEL_INFO_T));

  UNREFERENCED_PARAMETER( szDevice);

  if (NULL != ptChannel)
  {
    lRet                          = CIFX_INVALID_BOARD;

    ptChannel->dwRemoteHandle     = 0;
    ptChannel->ulChannelNr        = ulChannelNr;
    ptChannel->hDriver            = ptDLData->tDriverInfo.ulDriverHandle;
    ptChannel->fOpen              = 0;
    ptChannel->ptnetXTransportDev = ptnetXTransportDevice;
    ptChannel->lHandleState       = CIFX_INVALID_HANDLE;

    lRet = OpenChannelRequest( ptChannel, ptDLData->tBoardInfo.tBoardInfo.abBoardName, ulChannelNr);

    if(CIFX_NO_ERROR == lRet)
    {
      *phChannel       = ptChannel;
      ptChannel->fOpen = 1;

      ptChannel->lHandleState = CIFX_NO_ERROR;

      /* store handle for later usage */
      TAILQ_INSERT_TAIL( &s_tChannelHandleList, ptChannel, tList);
    } else
    {
      OS_Memfree(ptChannel);
    }
  }
  return lRet;
}

/*****************************************************************************/
/*! Sends a "close-channel" request
*   \param ptChannel  endpoint information
*   \return CIFX_NO_ERROR on success                                         */
/*****************************************************************************/
int32_t CloseChannelRequest( PMARSH_CHANNEL_INFO_T ptChannel)
{
  MARSHALLER_ENDPOINT_DATA_T*    ptDLData = netXTransportGetTLDeviceData( ptChannel->ptnetXTransportDev->hNetXTransportDeviceHandle);
  int32_t                        lRet     = 0;
  MARSHALLER_DATA_FRAME_HEADER_T tCloseObject = {{{0}}};
  uint8_t*                       pbRecvData   = NULL;

  GenerateHeader( &tCloseObject,
                  ptChannel->dwRemoteHandle,
                  (ptChannel->ulChannelNr == CIFX_SYSTEM_DEVICE) ? MARSHALLER_SYSDEV_METHODID_CLOSE : MARSHALLER_CHANNEL_METHODID_CLOSE,
                  1,
                  0,
                  ptDLData->tDriverInfo.fSequenceNrSupport,
                  ptDLData->tDriverInfo.usSequenceNr);

  lRet = TransferPacket( ptDLData->hTransport, &tCloseObject, (PMARSHALLER_DATA_FRAME_HEADER_T*)&pbRecvData, TRANSPORT_TO_TRANSFER);
  if(CIFX_NO_ERROR == lRet)
  {
    MARSHALLER_DATA_FRAME_HEADER_T* ptCnf = (MARSHALLER_DATA_FRAME_HEADER_T*)pbRecvData;

    lRet = VerifyTransferResult( &tCloseObject, ptCnf);

    OS_Memfree( pbRecvData);
  }
  return lRet;
}

/*****************************************************************************/
/*! Closed an open System device
*   \param ptChannel
*   \param fSendRequest
*   \param fForceRemove
*   \return CIFX_NO_ERROR on success                                         */
/*****************************************************************************/
static int32_t CloseChannel( PMARSH_CHANNEL_INFO_T ptChannel, int fSendRequest /* = true */, int fForceRemove /* = false */)
{
  int32_t lRet = CIFX_INVALID_HANDLE;

  UNREFERENCED_PARAMETER(fForceRemove);

  CHECK_REMOTE_HANDLE( ptChannel);

  if ((ptChannel == NULL) || ptChannel->fOpen == 0)
    return CIFX_INVALID_PARAMETER;

  if (fSendRequest != 0)
    lRet = CloseChannelRequest( ptChannel);

  if (fSendRequest && (lRet == CIFX_NO_ERROR))
  {
    ptChannel->fOpen = 0;

    TAILQ_REMOVE( &s_tChannelHandleList, ptChannel, tList);
    OS_Memfree( ptChannel);
  }
  return lRet;
}

/*****************************************************************************/
/*! Enumerate over all available Boards
*   \param hTransport
*   \param ulBoard      Should be counted from 0 upwards on enumeration
*   \param ulSize       Size of the user buffer
*   \param pvBoardInfo  Returned board information
*   \return CIFX_NO_ERROR on success, CIFX_NO_MORE_ENTRIES when ulBoard is unknown */
/*****************************************************************************/
static int32_t EnumDevice( MARSHALLER_TL_T* ptTLInfo, uint32_t ulBoard, uint32_t ulSize, void* pvBoardInfo)
{

  int32_t                         lRet          = CIFX_NO_ERROR;
  PMARSHALLER_DATA_FRAME_HEADER_T ptHeader      = NULL;
  uint32_t                        ulLocalSize   = ulSize;
  uint32_t                        ulRequestSize = 0;
  uint8_t*                        pbRecvData    = NULL;
  uint32_t                        ulOffset      = 0;
  uint8_t*                        pbData        = NULL;

  ulRequestSize = sizeof(*ptHeader)  +
                  sizeof(uint32_t)      +  /* ulBoard   */
                  sizeof(uint32_t);        /* ulSize    */

  ptHeader = (PMARSHALLER_DATA_FRAME_HEADER_T)OS_Memalloc(ulRequestSize);

  OS_Memset(ptHeader, 0, ulRequestSize);

  pbData   = (uint8_t*)(ptHeader + 1);

  OS_Memcpy((void*)&pbData[ulOffset], (void*)&ulBoard, sizeof(uint32_t));
  ulOffset += sizeof(ulBoard);

  OS_Memcpy((void*)&pbData[ulOffset], (void*)&ulSize, sizeof(uint32_t));
  ulOffset += sizeof(ulSize);

  GenerateHeader(ptHeader,
                 ptTLInfo->tDriverInfo.ulDriverHandle,
                 MARSHALLER_DRV_METHODID_ENUMBOARDS,
                 1,
                 (sizeof(ulBoard) + sizeof(ulLocalSize)),
                 ptTLInfo->tDriverInfo.fSequenceNrSupport,
                 ptTLInfo->tDriverInfo.usSequenceNr);

  lRet = TransferPacket( ptTLInfo->hTransport, ptHeader, (PMARSHALLER_DATA_FRAME_HEADER_T*)&pbRecvData, TRANSPORT_TO_TRANSFER);

  if(CIFX_NO_ERROR == lRet)
  {
    PCHANNEL_GETMBXSTATE_CNF_T ptCnf = (PCHANNEL_GETMBXSTATE_CNF_T)pbRecvData;

    if ( CIFX_NO_ERROR == (lRet = VerifyTransferResult(ptHeader, &ptCnf->tHeader)) )
    {
      OS_Memcpy( pvBoardInfo, &ptCnf->tData, ulSize);
    }

    OS_Memfree(pbRecvData);
  }

  OS_Memfree(ptHeader);

  return lRet;
}

/*****************************************************************************/
/*! Enumerate over all available Channels on a board
*   \param hTransport
*   \param ulBoard        The board number during board enumeration
*   \param ulChannel      Should be counted from 0 upwards on enumeration
*   \param ulSize         Size of the user buffer
*   \param pvChannelInfo  Returned channel information
*   \return CIFX_NO_ERROR on success, CIFX_NO_MORE_ENTRIES when ulChannel is unknown */
/*****************************************************************************/
static int32_t EnumChannel( MARSHALLER_TL_T* ptTLInfo, uint32_t ulBoard, uint32_t ulChannel, uint32_t ulSize, void* pvChannelInfo)
{
  int32_t                         lRet          = CIFX_NO_ERROR;
  PMARSHALLER_DATA_FRAME_HEADER_T ptHeader      = NULL;
  uint32_t                        ulLocalSize   = ulSize;
  uint32_t                        ulRequestSize = 0;
  uint32_t                        ulOffset      = 0;
  uint8_t*                        pbData;
  uint8_t*                        pbRecvData    = NULL;

  ulRequestSize = sizeof(MARSHALLER_DATA_FRAME_HEADER_T)  +
                  sizeof(uint32_t)      +  /* ulBoard   */
                  sizeof(uint32_t)      +  /* ulChannel   */
                  sizeof(uint32_t);        /* ulSize    */

  ptHeader = (PMARSHALLER_DATA_FRAME_HEADER_T)OS_Memalloc(ulRequestSize);

  OS_Memset(ptHeader, 0, ulRequestSize);

  pbData = (uint8_t*)(ptHeader + 1);

  OS_Memcpy((void*)&pbData[ulOffset], (void*)&ulBoard, sizeof(uint32_t));
  ulOffset += sizeof(ulBoard);

  OS_Memcpy((void*)&pbData[ulOffset], (void*)&ulChannel, sizeof(uint32_t));
  ulOffset += sizeof(ulChannel);

  OS_Memcpy((void*)&pbData[ulOffset], (void*)&ulSize, sizeof(uint32_t));
  ulOffset += sizeof(ulSize);

  GenerateHeader(ptHeader,
                 ptTLInfo->tDriverInfo.ulDriverHandle,
                 MARSHALLER_DRV_METHODID_ENUMCHANNELS,
                 1,
                 (sizeof(ulBoard) + sizeof(ulChannel) + sizeof(ulLocalSize)),
                 ptTLInfo->tDriverInfo.fSequenceNrSupport,
                 ptTLInfo->tDriverInfo.usSequenceNr);

  lRet = TransferPacket( ptTLInfo->hTransport, ptHeader, (PMARSHALLER_DATA_FRAME_HEADER_T*)&pbRecvData, TRANSPORT_TO_TRANSFER);

  if(CIFX_NO_ERROR == lRet)
  {
    PMARSHALLER_DATA_FRAME_HEADER_T ptFrame = (PMARSHALLER_DATA_FRAME_HEADER_T)pbRecvData;

    if ( CIFX_NO_ERROR == (lRet = VerifyTransferResult(ptHeader, ptFrame)) )
    {
      CHANNEL_INFORMATION* ptChannelInfo = (CHANNEL_INFORMATION*)(ptFrame + 1);

      OS_Memcpy( pvChannelInfo, ptChannelInfo, ulSize);
    }
    OS_Memfree(pbRecvData);
  }
  OS_Memfree(ptHeader);

  return lRet;
}

/*****************************************************************************/
/*! Gets the information of a system device
*   \param ptDevice
*   \param ulCmd        Command
*   \param ulSize       Size of the passed structure
*   \param pvInfo       Pointer to the structure for returned data
*   \param ulType       channel type
*   \return CIFX_NO_ERROR on success                                         */
/*****************************************************************************/
static int32_t MethodeInfo( PMARSH_CHANNEL_INFO_T ptDevice,
                       uint32_t            ulCmd,
                       uint32_t            ulSize,
                       void*               pvInfo,
                       uint32_t            ulType)
{
  int32_t                         lRet            = CIFX_NO_ERROR;
  int                             fCall           = 0;
  uint32_t                        ulRequestSize   = 0;
  uint32_t                        ulMarshallerCmd = 0;
  PMARSHALLER_DATA_FRAME_HEADER_T ptHeader        = NULL;
  uint32_t                        ulOffset        = 0;
  uint8_t*                        pbData          = NULL;
  MARSHALLER_ENDPOINT_DATA_T*     ptDLData        = netXTransportGetTLDeviceData( ptDevice->ptnetXTransportDev->hNetXTransportDeviceHandle);

  switch(ulType)
  {
    case CHANNEL_TYPE_XSYSDEVICE:
    {
      /* HANDLE xSysdeviceInfo call */
      ulRequestSize = sizeof(*ptHeader) +
                      sizeof(uint32_t)     +    /* ulCmd       */
                      sizeof(uint32_t);         /* ulSize, of the returned structure  */

      ptHeader = (PMARSHALLER_DATA_FRAME_HEADER_T)OS_Memalloc(ulRequestSize);
      /* Clean the allocated memory */
      OS_Memset(ptHeader, 0, ulRequestSize);

      pbData = (uint8_t*)(ptHeader + 1);

      OS_Memcpy((void*)&pbData[ulOffset], (void*)&ulCmd, sizeof(ulCmd));
      ulOffset += sizeof(ulCmd);

      OS_Memcpy((void*)&pbData[ulOffset], (void*)&ulSize, sizeof(ulSize));
      ulOffset += sizeof(ulSize);

      ulMarshallerCmd = MARSHALLER_SYSDEV_METHODID_INFO;
      fCall = 1;
    }
    break;

    case CHANNEL_TYPE_CHANNEL:
    {
      uint32_t ulOffset = 0;
      uint8_t* pbData   = NULL;

      /* HANDLE xChannelInfo call, ignore the ulCmd param */
      ulRequestSize = sizeof(*ptHeader) +
                      sizeof(uint32_t);         /* ulSize, of the returned structure  */

      ptHeader = (PMARSHALLER_DATA_FRAME_HEADER_T)OS_Memalloc(ulRequestSize);
      /* Clean the allocated memory */
      OS_Memset(ptHeader, 0, ulRequestSize);

      pbData = (uint8_t*)(ptHeader + 1);

      OS_Memcpy((void*)&pbData[ulOffset], (void*)&ulSize, sizeof(ulSize));
      ulOffset += sizeof(ulSize);
      ulMarshallerCmd = MARSHALLER_CHANNEL_METHODID_INFO;

      fCall = 1;
    }
    break;

    default:
      lRet = CIFX_INVALID_PARAMETER;
      break;
  }

  if(fCall)
  {
    uint8_t* pbRecvData    = NULL;

    GenerateHeader(ptHeader,
                  ptDevice->dwRemoteHandle,
                  ulMarshallerCmd,
                  1,
                  ulRequestSize - sizeof(*ptHeader),
                  ptDLData->tDriverInfo.fSequenceNrSupport,
                  ptDLData->tDriverInfo.usSequenceNr);

    lRet = TransferPacket( ptDLData->hTransport, ptHeader, (PMARSHALLER_DATA_FRAME_HEADER_T*)&pbRecvData, TRANSPORT_TO_TRANSFER);

    if(CIFX_NO_ERROR == lRet)
    {
      PCF_CREATEINSTANCE_CNF_T ptCnf = (PCF_CREATEINSTANCE_CNF_T)pbRecvData;

      if ( CIFX_NO_ERROR == (lRet = VerifyTransferResult(ptHeader, &ptCnf->tHeader)) )
      {
        OS_Memcpy( pvInfo, &ptCnf->tData , ulSize);
      }

      OS_Memfree(pbRecvData);
    }

    OS_Memfree(ptHeader);

  }
  return lRet;
}

/*****************************************************************************/
/*! Gets the Mailbox state of an open system device
*   \param ptChannel
*   \param pulRecvCnt      Number of packets in receive mailbox
*   \param pulSendCnt      Number of packets the application is able to send at once
*   \return CIFX_NO_ERROR on success                                         */
/*****************************************************************************/
static int32_t GetMbxState( PMARSH_CHANNEL_INFO_T ptChannel, uint32_t* pulRecvCnt, uint32_t* pulSendCnt)
{
  int32_t                        lRet;
  uint8_t*                       pbRecvData    = NULL;
  MARSHALLER_DATA_FRAME_HEADER_T tGetMbxState  = {{{0}}};
  MARSHALLER_ENDPOINT_DATA_T*    ptDLData      = netXTransportGetTLDeviceData( ptChannel->ptnetXTransportDev->hNetXTransportDeviceHandle);

  GenerateHeader(&tGetMbxState,
                 ptChannel->dwRemoteHandle,
                 (ptChannel->ulChannelNr == CIFX_SYSTEM_DEVICE) ? MARSHALLER_SYSDEV_METHODID_GETMBXSTATE :
                                                                  MARSHALLER_CHANNEL_METHODID_GETMBXSTATE,
                 1,
                 0,
                 ptDLData->tDriverInfo.fSequenceNrSupport,
                 ptDLData->tDriverInfo.usSequenceNr);

  lRet = TransferPacket( ptDLData->hTransport, &tGetMbxState, (PMARSHALLER_DATA_FRAME_HEADER_T*)&pbRecvData, TRANSPORT_TO_TRANSFER);

  if(CIFX_NO_ERROR == lRet)
  {
     PCHANNEL_GETMBXSTATE_CNF_T ptCnf = (PCHANNEL_GETMBXSTATE_CNF_T)pbRecvData;

    if ( CIFX_NO_ERROR == (lRet = VerifyTransferResult(&tGetMbxState, &ptCnf->tHeader)) )
    {
      *pulRecvCnt = ptCnf->tData.ulRecvPktCnt;
      *pulSendCnt = ptCnf->tData.ulSendPktCnt;
    }
    OS_Memfree(pbRecvData);
  }
  return lRet;
}

/*****************************************************************************/
/*! Inserts a packet into the Mailbox
*   \param ptChannel
*   \param ptPacket        Packet to send to device
*   \param ulTimeout       maximum time to wait for packet to be accepted by device (in ms)
*   \return CIFX_NO_ERROR on success                                         */
/*****************************************************************************/
static int32_t PutPacket(PMARSH_CHANNEL_INFO_T ptChannel, CIFX_PACKET* ptPacket, uint32_t ulTimeout)
{
  int32_t                         lRet;
  uint32_t                        ulRequestSize  = 0;
  PMARSHALLER_DATA_FRAME_HEADER_T ptHeader       = NULL;
  uint32_t                        ulSendDataSize = ptPacket->tHeader.ulLen + sizeof(ptPacket->tHeader);
  uint32_t                        ulOffset       = 0;
  uint8_t*                        pbData         = NULL;
  uint8_t*                        pbRecvData     = NULL;
  MARSHALLER_ENDPOINT_DATA_T*     ptDLData       = netXTransportGetTLDeviceData( ptChannel->ptnetXTransportDev->hNetXTransportDeviceHandle);

  ulRequestSize = sizeof(*ptHeader) +
                  sizeof(uint32_t)     +    /* ulSendSize  */
                  ulSendDataSize    +    /* abData[]    */
                  sizeof(uint32_t);         /* ulTimeout   */

  ptHeader = (PMARSHALLER_DATA_FRAME_HEADER_T)OS_Memalloc(ulRequestSize);
  OS_Memset(ptHeader, 0, ulRequestSize);

  pbData   = (uint8_t*)(ptHeader + 1);

  OS_Memcpy((void*)&pbData[ulOffset], (void*)&ulSendDataSize, sizeof(ulSendDataSize));
  ulOffset += sizeof(ulSendDataSize);

  OS_Memcpy((void*)&pbData[ulOffset], (void*)ptPacket, ulSendDataSize);
  ulOffset += ulSendDataSize;

  OS_Memcpy((void*)&pbData[ulOffset], (void*)&ulTimeout, sizeof(ulTimeout));
  ulOffset += sizeof(ulTimeout);

  GenerateHeader(ptHeader,
                 ptChannel->dwRemoteHandle,
                 (ptChannel->ulChannelNr == CIFX_SYSTEM_DEVICE) ? MARSHALLER_SYSDEV_METHODID_PUTPACKET :
                                                                  MARSHALLER_CHANNEL_METHODID_PUTPACKET,
                 1,
                 ulRequestSize - sizeof(*ptHeader),
                 ptDLData->tDriverInfo.fSequenceNrSupport,
                 ptDLData->tDriverInfo.usSequenceNr);

  lRet = TransferPacket( ptDLData->hTransport, ptHeader, (PMARSHALLER_DATA_FRAME_HEADER_T*)&pbRecvData, ulTimeout);

  if(CIFX_NO_ERROR == lRet)
  {
    PCF_CREATEINSTANCE_CNF_T ptCnf = (PCF_CREATEINSTANCE_CNF_T)pbRecvData;

    lRet = VerifyTransferResult(ptHeader, &ptCnf->tHeader);

    OS_Memfree(pbRecvData);
  }
  OS_Memfree(ptHeader);

  return lRet;
}

/*****************************************************************************/
/*! Retrieves a packet from the Mailbox
*   \param ptChannel
*   \param ptPacket        Pointer to buffer for received packet
*   \param ulPacketLen     Size of the buffer to retrieve the packet
*   \param ulTimeout       maximum time to wait for packet to be delivered by device (in ms)
*   \return CIFX_NO_ERROR on success                                         */
/*****************************************************************************/
static int32_t GetPacket( PMARSH_CHANNEL_INFO_T ptChannel, CIFX_PACKET* ptPacket, uint32_t ulPacketLen, uint32_t ulTimeout)
{
  int32_t                     lRet          = CIFX_NO_ERROR;
  CHANNEL_GETPACKET_REQ_T     tGetPacket    = {{{{0}}}};
  uint8_t*                    pbRecvData    = NULL;
  MARSHALLER_ENDPOINT_DATA_T* ptDLData      = netXTransportGetTLDeviceData( ptChannel->ptnetXTransportDev->hNetXTransportDeviceHandle);

  GenerateHeader( &tGetPacket.tHeader,
                  ptChannel->dwRemoteHandle,
                 (ptChannel->ulChannelNr == CIFX_SYSTEM_DEVICE) ? MARSHALLER_SYSDEV_METHODID_GETPACKET :
                                                                  MARSHALLER_CHANNEL_METHODID_GETPACKET,
                 1,
                 sizeof(tGetPacket.tData),
                 ptDLData->tDriverInfo.fSequenceNrSupport,
                 ptDLData->tDriverInfo.usSequenceNr);

  tGetPacket.tData.ulSize    = ulPacketLen;
  tGetPacket.tData.ulTimeout = ulTimeout;

  lRet = TransferPacket( ptDLData->hTransport, (PMARSHALLER_DATA_FRAME_HEADER_T)&tGetPacket, (PMARSHALLER_DATA_FRAME_HEADER_T*)&pbRecvData, ulTimeout);

  if(CIFX_NO_ERROR == lRet)
  {
    MARSHALLER_DATA_FRAME_HEADER_T* ptCnf = (MARSHALLER_DATA_FRAME_HEADER_T*)pbRecvData;

    if ( CIFX_NO_ERROR == (lRet = VerifyTransferResult(&tGetPacket.tHeader, ptCnf)) )
    {
      CIFX_PACKET* ptRecvPacket = (CIFX_PACKET*)(ptCnf + 1);

      uint32_t ulCopySize = ptRecvPacket->tHeader.ulLen + sizeof(CIFX_PACKET_HEADER);
      if(ulPacketLen < ulCopySize)
      {
        ulCopySize = ulPacketLen;
        lRet = CIFX_BUFFER_TOO_SHORT;
      }

      OS_Memcpy(ptPacket, ptRecvPacket, ulCopySize);
    }
  }

  OS_Memfree(pbRecvData);
  return lRet;
}

/*****************************************************************************/
/*! Reset
*   \param hSysdevice
*   \param ulResetMode
*   \param ulType
*   \param ulTimeout   Timeout
*   \return CIFX_NO_ERROR on success                                         */
/*****************************************************************************/
static int32_t Reset( PMARSH_CHANNEL_INFO_T ptDevice,
               uint32_t              ulResetMode,
               uint32_t              ulType,
               uint32_t              ulTimeout)
{
  int32_t                         lRet            = CIFX_NO_ERROR;
  int                             fCall           = 0;
  uint32_t                        ulRequestSize   = 0;
  uint32_t                        ulMarshallerCmd = 0;
  PMARSHALLER_DATA_FRAME_HEADER_T ptHeader        = NULL;
  uint32_t                        ulOffset        = 0;
  uint8_t*                        pbData          = NULL;
  MARSHALLER_ENDPOINT_DATA_T*     ptDLData        = netXTransportGetTLDeviceData( ptDevice->ptnetXTransportDev->hNetXTransportDeviceHandle);

  switch(ulType)
  {
    /* xSysdeviceReset */
    case CHANNEL_TYPE_XSYSDEVICE:
    {
      ulRequestSize = sizeof(*ptHeader) +
                      sizeof(uint32_t);        /* ulTimeout in ms to wait for reset to complete */

      ptHeader = (PMARSHALLER_DATA_FRAME_HEADER_T)OS_Memalloc(ulRequestSize);
      /* Clean the allocated memory */
      OS_Memset( ptHeader, 0, ulRequestSize);

      ulOffset = 0;
      pbData = (uint8_t*)(ptHeader + 1);

      OS_Memcpy((void*)&pbData[ulOffset], (void*)&ulTimeout, sizeof(ulTimeout));
      ulOffset += sizeof(ulTimeout);
      ulMarshallerCmd = MARSHALLER_SYSDEV_METHODID_RESET;

      fCall = 1;
    }
    break;

    /* xChannelReset */
    case CHANNEL_TYPE_CHANNEL:
    {
      ulRequestSize = sizeof(*ptHeader) +
                      sizeof(uint32_t)     +   /* ulResetMode, special mode of reset */
                      sizeof(uint32_t);        /* ulTimeout in ms for function call  */

      ptHeader = (PMARSHALLER_DATA_FRAME_HEADER_T)OS_Memalloc(ulRequestSize);
      /* Clean the allocated memory */
      OS_Memset(ptHeader, 0, ulRequestSize);

      ulOffset = 0;
      pbData   = (uint8_t*)(ptHeader + 1);

      OS_Memcpy((void*)&pbData[ulOffset], (void*)&ulResetMode, sizeof(ulResetMode));
      ulOffset += sizeof(ulResetMode);

      OS_Memcpy((void*)&pbData[ulOffset], (void*)&ulTimeout, sizeof(ulTimeout));
      ulOffset += sizeof(ulTimeout);
      ulMarshallerCmd = MARSHALLER_CHANNEL_METHODID_RESET;
      fCall = 1;
    }
    break;

    default:
      lRet = CIFX_INVALID_PARAMETER;
      break;
  }

  if(fCall)
  {
    /* Send reset packet */
    uint8_t* pbRecvData    = NULL;

    GenerateHeader(ptHeader,
                  ptDevice->dwRemoteHandle,
                  ulMarshallerCmd,
                  1,
                  ulRequestSize - sizeof(*ptHeader),
                  ptDLData->tDriverInfo.fSequenceNrSupport,
                  ptDLData->tDriverInfo.usSequenceNr);

    lRet = TransferPacket( ptDLData->hTransport, ptHeader, (PMARSHALLER_DATA_FRAME_HEADER_T*)&pbRecvData, ulTimeout);

    if(NXT_TRANSACTION_CANCELLED == lRet)
    {
      /* The device has dropped the connection after reset, so are not sure if the reset succeeded,
         or if the connection has dropped, so let the upper layer to the reconnect */
      lRet = CIFX_NO_ERROR;
    }else if(CIFX_NO_ERROR == lRet)
    {
      PMARSHALLER_DATA_FRAME_HEADER_T ptMarshHeader = (PMARSHALLER_DATA_FRAME_HEADER_T)pbRecvData;

      lRet = VerifyTransferResult(ptHeader, ptMarshHeader);

      OS_Memfree(pbRecvData);
    }
    OS_Memfree(ptHeader);
  }

  return lRet;
}

/*****************************************************************************/
/*! Acces the channels control block
*   \param hChannel     Channel handle acquired by xChannelOpen
*   \param ulCmd        Command
*   \param ulOffset     Data start offset
*   \param ulDataLen    Data length
*   \param pvData       Data buffer
*   \return CIFX_NO_ERROR on success                                         */
/*****************************************************************************/
static int32_t ChannelReadWriteBlock( PMARSH_CHANNEL_INFO_T ptChannel,
                                      uint32_t             ulCmd,
                                      uint32_t             ulOffset,
                                      uint32_t             ulDataLen,
                                      void*                pvData,
                                      uint32_t             ulMethodID)
{
  int32_t                     lRet;
  MARSHALLER_ENDPOINT_DATA_T* ptDLData = netXTransportGetTLDeviceData( ptChannel->ptnetXTransportDev->hNetXTransportDeviceHandle);

  switch(ulCmd)
  {
    case CIFX_CMD_READ_DATA:
      {
        CHANNEL_BLOCK_READ_REQ_T tBlockReadReq = {{{{0}}}};
        uint8_t* pbRecvData    = NULL;

        GenerateHeader( &tBlockReadReq.tHeader,
                        ptChannel->dwRemoteHandle,
                        ulMethodID,
                        1,
                        sizeof(tBlockReadReq.tData),
                        ptDLData->tDriverInfo.fSequenceNrSupport,
                        ptDLData->tDriverInfo.usSequenceNr);

        tBlockReadReq.tData.ulCmd     = ulCmd;
        tBlockReadReq.tData.ulOffset  = ulOffset;
        tBlockReadReq.tData.ulDatalen = ulDataLen;

        lRet = TransferPacket( ptDLData->hTransport, (PMARSHALLER_DATA_FRAME_HEADER_T)&tBlockReadReq, (PMARSHALLER_DATA_FRAME_HEADER_T*)&pbRecvData, TRANSPORT_TO_TRANSFER);

        if(CIFX_NO_ERROR == lRet)
        {
          PCHANNEL_BLOCK_READ_CNF_T ptReadCnf = (PCHANNEL_BLOCK_READ_CNF_T)pbRecvData;

          if ( CIFX_NO_ERROR == (lRet = VerifyTransferResult(&tBlockReadReq.tHeader, &ptReadCnf->tHeader)) )
          {
            OS_Memcpy(pvData, ptReadCnf->tData.abData, ulDataLen);
          }

          OS_Memfree(pbRecvData);
        }
      }
      break;

    case CIFX_CMD_WRITE_DATA:
      {
        PCHANNEL_BLOCK_WRITE_REQ_T ptBlockWriteReq = {0};
        uint8_t* pbRecvData    = NULL;
        uint32_t ulDataSendLen = sizeof(*ptBlockWriteReq) + ulDataLen;

        ptBlockWriteReq = (PCHANNEL_BLOCK_WRITE_REQ_T)OS_Memalloc(ulDataSendLen);

        GenerateHeader( &ptBlockWriteReq->tHeader,
                        ptChannel->dwRemoteHandle,
                        ulMethodID,
                        1,
                        ulDataSendLen,
                        ptDLData->tDriverInfo.fSequenceNrSupport,
                        ptDLData->tDriverInfo.usSequenceNr);

        ptBlockWriteReq->tData.ulCmd     = ulCmd;
        ptBlockWriteReq->tData.ulOffset  = ulOffset;
        ptBlockWriteReq->tData.ulDatalen = ulDataLen;
        OS_Memcpy(ptBlockWriteReq->tData.abData, pvData, ulDataLen);

        lRet = TransferPacket( ptDLData->hTransport, (PMARSHALLER_DATA_FRAME_HEADER_T)ptBlockWriteReq, (PMARSHALLER_DATA_FRAME_HEADER_T*)&pbRecvData, TRANSPORT_TO_TRANSFER);

        if(CIFX_NO_ERROR == lRet)
        {
          PCHANNEL_BLOCK_READ_CNF_T ptReadCnf = (PCHANNEL_BLOCK_READ_CNF_T)pbRecvData;

          if ( CIFX_NO_ERROR == (lRet = VerifyTransferResult(&ptBlockWriteReq->tHeader, &ptReadCnf->tHeader)) )
          {
            lRet = ptReadCnf->tHeader.ulError;
          }

          OS_Memfree(pbRecvData);
        }
        OS_Memfree(ptBlockWriteReq);
      }
      break;

    default:
      lRet = CIFX_INVALID_COMMAND;
      break;
  }

  return lRet;
}

/*****************************************************************************/
/*! Create Object
*   \param hTransport
*   \param ulDestHandle
*   \param ulObjectType
*   \param pulObjectHandle
*   \return CIFX_NO_ERROR on success                                         */
/*****************************************************************************/
static int32_t CreateObject( NXTHANDLE hTransport,
                      uint32_t  ulDestHandle,
                      uint32_t  ulObjectType,
                      uint32_t* pulObjectHandle)
{
  int32_t                 lRet            = CIFX_NO_ERROR;
  uint8_t*                pbRecvData      = NULL;
  CF_CREATEINSTANCE_REQ_T tCreateInstance = {{{{0}}}};

  GenerateHeader(&tCreateInstance.tHeader,
                 ulDestHandle,
                 MARSHALLER_CF_METHODID_CREATEINSTANCE,
                 1,
                 sizeof(tCreateInstance.tData),
                 0,
                 0);

  tCreateInstance.tData.ulObjectType = ulObjectType;

  lRet = TransferPacket( hTransport, (PMARSHALLER_DATA_FRAME_HEADER_T)&tCreateInstance , (PMARSHALLER_DATA_FRAME_HEADER_T*)&pbRecvData, TRANSPORT_TO_TRANSFER);

  if(CIFX_NO_ERROR == lRet)
  {
    PCF_CREATEINSTANCE_CNF_T ptCnf = (PCF_CREATEINSTANCE_CNF_T)pbRecvData;

    if ( CIFX_NO_ERROR == (lRet = VerifyTransferResult(&tCreateInstance.tHeader, &ptCnf->tHeader)) )
    {
      *pulObjectHandle = ptCnf->tData.uHandle.ulHandle;
    }

    OS_Memfree(pbRecvData);
  }
  return lRet;
}

/*****************************************************************************/
/*! QueryMarshallerVersion
*   \param hTransport
*   \return CIFX_NO_ERROR on success                                          */
/******************************************************************************/
static int32_t QueryMarshallerVersion( NXTHANDLE hTransport)
{
  int32_t                        lRet          = CIFX_NO_ERROR;
  uint8_t*                       pbRecvData    = NULL;
  MARSHALLER_DATA_FRAME_HEADER_T tCFVersion    = {{{0}}};

  GenerateHeader(&tCFVersion, 0, MARSHALLER_CF_METHODID_SERVERVERSION, 1, 0, 0, 0);

  lRet = TransferPacket( hTransport, &tCFVersion , (PMARSHALLER_DATA_FRAME_HEADER_T*)&pbRecvData, TRANSPORT_TO_TRANSFER);

  if(CIFX_NO_ERROR == lRet)
  {
    PCF_SERVERVERSION_CNF_T ptCnf = (PCF_SERVERVERSION_CNF_T)(pbRecvData);

    if ( CIFX_NO_ERROR == (lRet = VerifyTransferResult(&tCFVersion, &ptCnf->tHeader)) )
    {
      g_ulVersion = ptCnf->tData.ulVersion;
    }

    OS_Memfree(pbRecvData);
  }
  return lRet;
}

/*****************************************************************************/
/*! Generate transport header for marshaller
*   \param ptHeader
*   \param ulHandle
*   \param ulMethodId
*   \param fRequest
*   \param ulDataSize
*   \param fSequenceSupport
*   \param usSequenceNr                                                       */
/******************************************************************************/
static void GenerateHeader( PMARSHALLER_DATA_FRAME_HEADER_T ptHeader,
                     uint32_t                        ulHandle,
                     uint32_t                        ulMethodId,
                     int                             fRequest,
                     uint32_t                        ulDataSize,
                     int                             fSequenceSupport,
                     uint16_t                        usSequenceNr)
{
  OS_Memset(ptHeader,0,sizeof(*ptHeader));

  ptHeader->ulMethodID                          = ulMethodId;
  ptHeader->uHandle.ulHandle                    = ulHandle;
  ptHeader->ulDataSize                          = ulDataSize;
  ptHeader->uSequence.tBfSequence.fRequest      = fRequest ? 1 : 0;
  ptHeader->uSequence.tBfSequence.fSeqNrSupport = fSequenceSupport ? 1 : 0;
  ptHeader->uSequence.tBfSequence.usSequenceNr  = fSequenceSupport ? ++usSequenceNr : 0;
}

/*****************************************************************************/
/*! verifies transfer result
*   \param ptReq  Pointer to request header
*   \param ptCnf  Pointer to confirmation header
*   return CIFX_NO_ERROR on success                                           */
/******************************************************************************/
int32_t VerifyTransferResult (PMARSHALLER_DATA_FRAME_HEADER_T ptReq, PMARSHALLER_DATA_FRAME_HEADER_T ptCnf)
{
  int32_t lRet = CIFX_NO_ERROR;

  if ( (ptCnf->ulMethodID       != ptReq->ulMethodID)       ||
       (ptCnf->uHandle.ulHandle != ptReq->uHandle.ulHandle) ||
       (ptCnf->uSequence.tBfSequence.fRequest)                 )
  {
    lRet = CIFX_TRANSPORT_INVALID_RESPONSE;
  } else
  {
    lRet = ptCnf->ulError;
  }

  return lRet;
}

/*****************************************************************************/
/*! TransferPacket
*   \param hTransport
*   \param ptSendPacket
*   \param pptRecvPacket
*   \param ulTimeout
*   return CIFX_NO_ERROR on success                                           */
/******************************************************************************/
static int32_t TransferPacket(NXTHANDLE hTransport, MARSHALLER_DATA_FRAME_HEADER_T* ptSendPacket , MARSHALLER_DATA_FRAME_HEADER_T** pptRecvPacket, uint32_t ulTimeout)
{
  int32_t                 lRet                  = NXT_NO_MEMORY;
  uint32_t                ulDataLen             = 0;
  PHIL_TRANSPORT_PACKET_T ptTransportSendPacket = NULL;
  PHIL_TRANSPORT_PACKET_T ptTransportRecvPacket = NULL;

  ulDataLen = ptSendPacket->ulDataSize + sizeof(MARSHALLER_DATA_FRAME_HEADER_T);

  /* allocate netXTransport send packet */
  ptTransportSendPacket = HilTransportAllocatePacket( hTransport, ulDataLen, HIL_TRANSPORT_TYPE_MARSHALLER);

  if (ptTransportSendPacket != NULL)
  {
    ptTransportSendPacket->ptHeader->ulLength = ulDataLen;
    OS_Memcpy(ptTransportSendPacket->pbData, ptSendPacket, ulDataLen);

    if (NXT_NO_ERROR == (lRet = HilTransportTransferPacket( hTransport,
                                        ptTransportSendPacket->ptHeader,
                                        &ptTransportRecvPacket,
                                        ulTimeout)))
    {
      *pptRecvPacket = (PMARSHALLER_DATA_FRAME_HEADER_T)OS_Memalloc(ptTransportRecvPacket->ptHeader->ulLength);

      if (NULL != *pptRecvPacket)
      {
        OS_Memcpy( *pptRecvPacket, ptTransportRecvPacket->pbData, ptTransportRecvPacket->ptHeader->ulLength);
      }
    }
  }
  HilTransportFreePacket( ptTransportSendPacket);
  HilTransportFreePacket( ptTransportRecvPacket);

  return lRet;
}

/*****************************************************************************/
/*! Translation-Layer receive function. Queues incoming packets
*   \param hTLHandle handle to the translation layer
*   \param ptPacket Packet to process
*   \return NXT_NO_ERROR on success                                         */
/*****************************************************************************/
static void APIENTRY cifXAPI_Receive(void* hTLHandle, PHIL_TRANSPORT_PACKET_T ptPacket)
{
  UNREFERENCED_PARAMETER( hTLHandle);

  HilTransportFreePacket(ptPacket);
}

/*****************************************************************************/
/*! Translation-Layer restore function. Function is called in case of a reconnect to
    restore all active connections.
*   \param hNetXTransportDeviceHandle  Handle to the netXTransport device    */
/*****************************************************************************/
static void APIENTRY cifXAPI_RestoreConnectionStates( NXTHANDLE hNetXTransportDeviceHandle)
{
  PMARSH_CHANNEL_INFO_T ptChannel = NULL;

  /* re-activate all opened connections */
  TAILQ_FOREACH ( ptChannel, &s_tChannelHandleList, tList)
  {
    CIFX_TRANSPORT_DEVICE_INST_T* ptnetXTransportDevice = ptChannel->ptnetXTransportDev;

    /* find the device */
    if (hNetXTransportDeviceHandle == ptnetXTransportDevice->hNetXTransportDeviceHandle)
    {
      /* check if the device was opened before */
      if (ptChannel->fOpen != 0)
      {
        MARSHALLER_ENDPOINT_DATA_T* ptDLData  = netXTransportGetTLDeviceData( hNetXTransportDeviceHandle);

        if ((ptChannel->ulChannelNr != RCX_SYSTEM_CHANNEL) && (ptChannel->ulChannelNr > ptDLData->tBoardInfo.tBoardInfo.ulChannelCnt))
        {
          ptChannel->lHandleState = CIFX_INVALID_CHANNEL;
        } else
        {
          ptChannel->lHandleState = OpenChannelRequest( ptChannel, ptDLData->tBoardInfo.tBoardInfo.abBoardName, ptChannel->ulChannelNr);
        }
      }
    }
  }
}

/*****************************************************************************/
/*! TransferPacket
*   \param hTLHandle
*   \param szDeviceName
*   \param pulDeviceIdentifier
*   \param pvDLDeviceData
*   \param ulDeviceNumber
*   return NXT_NO_ERROR on success                                           */
/******************************************************************************/
static int32_t APIENTRY cifXAPI_DeviceScan( NXTHANDLE hTLHandle, char* szDeviceName, uint32_t* pulDeviceIdentifier, void** pvDLDeviceData, uint32_t ulDeviceNumber)
{
  int32_t                     lRet        = NXT_NO_ERROR;
  MARSHALLER_TL_T*            ptTLInfo    = (MARSHALLER_TL_T*)hTLHandle;
  MARSHALLER_ENDPOINT_DATA_T* ptEndpoint  = OS_Memalloc(sizeof(MARSHALLER_ENDPOINT_DATA_T));

  *pvDLDeviceData = NULL;

  if(NULL != ptEndpoint)
  {
    if(g_ulTraceLevel & TRACE_LEVEL_INFO)
    {
      USER_Trace( s_tcifXMarshallerLayer.hNetXTransport, TRACE_LEVEL_INFO, "Scanning for device %d (Marshaller Translation-Layer -0x200).", ulDeviceNumber);
    }

    /* now create all resources for all available devices */
    /* driver should be already opended */
    /* if(CIFX_NO_ERROR == (lRet = MsDriverOpen( ptTLInfo))) */
    {
      BOARD_INFORMATION tBoardInfo = {0};

      /* Enumerate channels, and create channel objects */
      if (CIFX_NO_ERROR == (lRet = EnumDevice( ptTLInfo, ulDeviceNumber, sizeof(tBoardInfo), &tBoardInfo)))
      {
        uint32_t            ulChannel    = 0;
        CHANNEL_INFORMATION tChannelInfo = {{0}};

        /* Get the necessary names and store the information in the device */
        OS_Memcpy( &ptEndpoint->tBoardInfo.tBoardInfo, &tBoardInfo, sizeof(tBoardInfo));

        /* is it required to do this step here???  */
        while(CIFX_NO_ERROR == EnumChannel( ptTLInfo, ulDeviceNumber, ulChannel, sizeof(tChannelInfo), &tChannelInfo))
        {
          ++ulChannel;
        }
      }
      ptEndpoint->hTransport                 = ptTLInfo->hTransport;
      ptEndpoint->tDriverInfo.ulDriverHandle = ptTLInfo->tDriverInfo.ulDriverHandle;
      /* ptEndpoint->tDriverInfo.ulClassFactory = ptTLInfo->tDriverInfo.ulClassFactory;  */
      *pvDLDeviceData                        = ptEndpoint;

      /* store private data */
      ptTLInfo->atMarshallerEndpoints = OS_Memrealloc( ptTLInfo->atMarshallerEndpoints, (ptTLInfo->ulEndpointCounter+1)*sizeof(ptEndpoint));
      ptTLInfo->atMarshallerEndpoints[ptTLInfo->ulEndpointCounter++] = ptEndpoint;

      OS_Strncpy( szDeviceName, 16, tBoardInfo.abBoardName, 16);

      /* unique device identification -> to be able to identify a reconnected device          */
      /* (the identifier needs to be unique per interface, so we can use the physical address */
      *pulDeviceIdentifier = tBoardInfo.ulPhysicalAddress;

      lRet = NXT_NO_ERROR;
    }
  } else
  {
    lRet = NXT_NO_MEMORY;
  }
  return lRet;
}

/*****************************************************************************/
/*! Function returns pointer to function table (Translation-Layer API interface)
*   \param pfnFunctionPointerTable structure to the function pointer table
*   \return NXT_NO_ERROR on success                                         */
/*****************************************************************************/
static int32_t APIENTRY cifXAPI_Probe(NXTHANDLE hTransport, NXTHANDLE* phTLHandle, uint32_t* pulDeviceNumber, TRANSPORT_SERVER_INFO_T* ptServerInfo)
{
  int32_t          lRet     = NXT_NO_ERROR;
  MARSHALLER_TL_T* ptTLInfo = (MARSHALLER_TL_T*)OS_Memalloc(sizeof(MARSHALLER_TL_T));

  *phTLHandle = NULL;

  if(NULL != ptTLInfo)
  {
    OS_Memset(ptTLInfo, 0, sizeof(*ptTLInfo));
    ptTLInfo->hTransport = hTransport;

    /* setup server configuration */
    ptTLInfo->tDriverInfo.fSequenceNrSupport = ptServerInfo->fIgnoreSeqNr;
    ptTLInfo->tDriverInfo.usSequenceNr       = 0;

    /* initialize connection and store driver specific informations */
    if (CIFX_NO_ERROR != (lRet = QueryMarshallerVersion( hTransport)))
    {
      if(g_ulTraceLevel & TRACE_LEVEL_INFO)
      {
        USER_Trace( s_tcifXMarshallerLayer.hNetXTransport, TRACE_LEVEL_INFO, "Failed to request Marshaller version!");
      }

    } else if (CIFX_NO_ERROR != (lRet = CreateObject( hTransport, 0,
                                            MARSHALLER_OBJECT_TYPE_CLASSFACTORY,
                                            &ptTLInfo->tDriverInfo.ulClassFactory)))
    {
      if(g_ulTraceLevel & TRACE_LEVEL_INFO)
      {
        USER_Trace( s_tcifXMarshallerLayer.hNetXTransport, TRACE_LEVEL_INFO, "Failed to request Class-Factory object type!");
      }

    } else if (CIFX_NO_ERROR == (lRet = CreateObject( hTransport, ptTLInfo->tDriverInfo.ulClassFactory,
                                            MARSHALLER_OBJECT_TYPE_DRIVER,
                                            &ptTLInfo->tDriverInfo.ulDriverHandle)))
    {
      BOARD_INFORMATION tBoardInfo = {0};

      /* at this point we are only interrested in how many boards are there */
      if(CIFX_NO_ERROR == (lRet = MsDriverOpen( ptTLInfo)))
      {
        *pulDeviceNumber = 0;
        while(CIFX_NO_ERROR == (lRet = EnumDevice( ptTLInfo, *pulDeviceNumber, sizeof(tBoardInfo), &tBoardInfo)))
        {
          (*pulDeviceNumber)++;
        }
        if ((lRet == CIFX_INVALID_BOARD) && (*pulDeviceNumber > 0))
        {
          *phTLHandle = ptTLInfo;

          lRet = NXT_NO_ERROR;
        }
      }
    }
    if (lRet != CIFX_NO_ERROR)
      lRet = NXT_FUNCTION_FAILED;

  } else
  {
    lRet = NXT_OUT_OF_MEMORY;
  }

  if(NXT_NO_ERROR != lRet)
  {
    OS_Memfree(ptTLInfo);
  }

  return lRet;
}

/*****************************************************************************/
/*! De-initializes Marshaller Translation-Layer
*   \param hTLHandle  Handle to Translation-Layer instance                   */
/*****************************************************************************/
void APIENTRY cifXAPI_Release( NXTHANDLE hTLHandle)
{
  MARSHALLER_TL_T* ptTLInfo = (MARSHALLER_TL_T*)hTLHandle;

  if (ptTLInfo != NULL)
  {
    while (ptTLInfo->ulEndpointCounter-->0)
    {
      if (ptTLInfo->atMarshallerEndpoints)
      {
        if (ptTLInfo->atMarshallerEndpoints[ptTLInfo->ulEndpointCounter] != NULL)
          OS_Memfree(ptTLInfo->atMarshallerEndpoints[ptTLInfo->ulEndpointCounter]);
      }
    }
    OS_Memfree(ptTLInfo->atMarshallerEndpoints);

    OS_Memfree( ptTLInfo);
  }
}

/*****************************************************************************/
/*! Map the current function pointer table
*   \param pfnFunctionPointerTable pointer to the function pointer table
*   \return CIFX_NO_ERROR on success                                         */
/*****************************************************************************/
int32_t APIENTRY cifXAPI_MapFunctionPointerTable( FUNCTION_POINTER_TABLE_T* pfnFunctionPointerTable)
{
  uint32_t lRet = NXT_NO_ERROR;

  if (pfnFunctionPointerTable)
  {
    CIFX_DRV_FUNCTION_TABLE_T* ptCifXConfig = (CIFX_DRV_FUNCTION_TABLE_T*)OS_Memalloc(sizeof(CIFX_DRV_FUNCTION_TABLE_T));

    OS_Memset(ptCifXConfig, 0, sizeof(CIFX_DRV_FUNCTION_TABLE_T));

    ptCifXConfig->pfnxDriverOpen                 = DriverOpen;
    ptCifXConfig->pfnxDriverClose                = DriverClose;
    ptCifXConfig->pfnxDriverGetInformation       = DriverGetInformation;
    ptCifXConfig->pfnxDriverGetErrorDescription  = DriverGetErrorDescription;
    ptCifXConfig->pfnxDriverEnumBoards           = DriverEnumBoards;
    ptCifXConfig->pfnxDriverEnumChannels         = DriverEnumChannels;
    ptCifXConfig->pfnxDriverMemoryPointer        = DriverMemoryPointer;
    ptCifXConfig->pfnxSysdeviceOpen              = SysdeviceOpen;
    ptCifXConfig->pfnxSysdeviceClose             = SysdeviceClose;
    ptCifXConfig->pfnxSysdeviceReset             = SysdeviceReset;
    ptCifXConfig->pfnxSysdeviceGetMBXState       = SysdeviceGetMBXState;
    ptCifXConfig->pfnxSysdevicePutPacket         = SysdevicePutPacket;
    ptCifXConfig->pfnxSysdeviceGetPacket         = SysdeviceGetPacket;
    ptCifXConfig->pfnxSysdeviceDownload          = SysdeviceDownload;
    ptCifXConfig->pfnxSysdeviceInfo              = SysdeviceInfo;
    ptCifXConfig->pfnxSysdeviceFindFirstFile     = SysdeviceFindFirstFile;
    ptCifXConfig->pfnxSysdeviceFindNextFile      = SysdeviceFindNextFile;
    ptCifXConfig->pfnxChannelOpen                = ChannelOpen;
    ptCifXConfig->pfnxChannelClose               = ChannelClose;
    ptCifXConfig->pfnxChannelDownload            = ChannelDownload;
    ptCifXConfig->pfnxChannelGetMBXState         = ChannelGetMBXState;
    ptCifXConfig->pfnxChannelPutPacket           = ChannelPutPacket;
    ptCifXConfig->pfnxChannelGetPacket           = ChannelGetPacket;
    ptCifXConfig->pfnxChannelGetSendPacket       = ChannelGetSendPacket;
    ptCifXConfig->pfnxChannelConfigLock          = ChannelConfigLock;
    ptCifXConfig->pfnxChannelReset               = ChannelReset;
    ptCifXConfig->pfnxChannelInfo                = ChannelInfo;
    ptCifXConfig->pfnxChannelWatchdog            = ChannelWatchdog;
    ptCifXConfig->pfnxChannelHostState           = ChannelHostState;
    ptCifXConfig->pfnxChannelBusState            = ChannelBusState;
    ptCifXConfig->pfnxChannelIORead              = ChannelIORead;
    ptCifXConfig->pfnxChannelIOWrite             = ChannelIOWrite;
    ptCifXConfig->pfnxChannelIOReadSendData      = ChannelIOReadSendData;
    ptCifXConfig->pfnxChannelControlBlock        = ChannelControlBlock;
    ptCifXConfig->pfnxChannelCommonStatusBlock   = ChannelCommonStatusBlock;
    ptCifXConfig->pfnxChannelExtendedStatusBlock = ChannelExtendedStatusBlock;
    ptCifXConfig->pfnxChannelPLCMemoryPtr        = ChannelPLCMemoryPtr;
    ptCifXConfig->pfnxChannelPLCIsReadReady      = ChannelPLCIsReadReady;
    ptCifXConfig->pfnxChannelPLCIsWriteReady     = ChannelPLCIsWriteReady;
    ptCifXConfig->pfnxChannelPLCActivateWrite    = ChannelPLCActivateWrite;
    ptCifXConfig->pfnxChannelPLCActivateRead     = ChannelPLCActivateRead;
    ptCifXConfig->pfnxChannelFindFirstFile       = ChannelFindFirstFile;
    ptCifXConfig->pfnxChannelFindNextFile        = ChannelFindNextFile;

    pfnFunctionPointerTable->ulVersionMajor     = CIFXAPI_VERSION_MAJOR;
    pfnFunctionPointerTable->ulVersionMinor     = CIFXAPI_VERSION_MINOR;
    pfnFunctionPointerTable->ulVersionRevision  = CIFXAPI_VERSION_REV;
    pfnFunctionPointerTable->ulVersionBuild     = CIFXAPI_VERSION_BUILD;

    pfnFunctionPointerTable->pvTranslationLayerFunctionTable = (void*)ptCifXConfig;
  } else
  {
    lRet = NXT_INVALID_POINTER;
  }
  return lRet;
}

/*****************************************************************************/
/*! Function initializes the translation layer (rcXPacket-> data type = 0x200) and adds
*   the initialized translation layer to the netXTransport layer
*   \param pvParam  currently not used
*   \return NXT_NO_ERROR on success                                         */
/*****************************************************************************/
int32_t APIENTRY cifX_Marshaller_Init( void* pvParam)
{
  int32_t lRet = NXT_NO_ERROR;

  UNREFERENCED_PARAMETER( pvParam);

  s_tcifXMarshallerLayer.usDataType          = HIL_TRANSPORT_TYPE_MARSHALLER;   /* supported data type of the translation layer                */
  s_tcifXMarshallerLayer.pfnProbe            = cifXAPI_Probe;                   /* called to check if data is supported and for initialization */
  s_tcifXMarshallerLayer.pfnRelease          = cifXAPI_Release;
  s_tcifXMarshallerLayer.pfnScan             = cifXAPI_DeviceScan;              /* called to check if data is supported and for initialization   */
  s_tcifXMarshallerLayer.pfnRestoreCon       = cifXAPI_RestoreConnectionStates; /* called to restore all active connections in case of reconnect */
  s_tcifXMarshallerLayer.pfnReceive          = cifXAPI_Receive;                 /* called in case of incoming packets with the packet type HIL_TRANSPORT_TYPE_MARSHALLER */
  s_tcifXMarshallerLayer.pfnMapFunctionTable = cifXAPI_MapFunctionPointerTable; /* table of API functions                                      */

  /* register marshaller translation layer */
  if (NXT_NO_ERROR == (lRet = netXTransportAddTranslationLayer(&s_tcifXMarshallerLayer, &s_tcifXMarshallerLayer.hNetXTransportDevGroup)))
  {
    TAILQ_INIT( &s_tChannelHandleList);
    cifXAPIInit( s_tcifXMarshallerLayer.hNetXTransportDevGroup);
  }
  return lRet;
}

/*****************************************************************************/
/*! Function de-registers translation layer
*   \param pvParam  currently not used
*   \return NXT_NO_ERROR on success                                         */
/*****************************************************************************/
void APIENTRY cifX_Marshaller_DeInit( void* pvParam)
{
  UNREFERENCED_PARAMETER( pvParam);

  /* de-register marshaller translation layer */
  netXTransportRemoveTranslationLayer(s_tcifXMarshallerLayer.hNetXTransportDevGroup);
}
