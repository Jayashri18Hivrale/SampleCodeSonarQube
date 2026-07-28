/**************************************************************************************

   Copyright (c) Hilscher GmbH. All Rights Reserved.

 **************************************************************************************

   Filename:
    $Id: cifXAPI.h 167 2013-06-17 07:59:33Z sebastiand $
   Last Modification:
    $Author: sebastiand $
    $Date: 2013-06-17 09:59:33 +0200 (Mo, 17. Jun 2013) $
    $Revision: 167 $

   Targets:
     O/S Independent : yes

   Description:
    Declaration of function codes and structures for datalayer execute

   Changes:

     Version   Date        Author   Description
     ----------------------------------------------------------------------------------
      1         23.02.13    MT       initial version

**************************************************************************************/

/****************************************************************************/
/*! \file cifXAPI.h
    Declaration of function codes and structures for datalyer execute       */
/****************************************************************************/

#ifndef __CIFX_API__H
#define __CIFX_API__H

#include "cifXUser.h"
#include "netXTransport.h"

/*****************************************************************************/
/*! cifX API version information                                             */
/*****************************************************************************/
#define CIFX_DRV_VERSION_MAJOR 1  /*!< Version major    */
#define CIFX_DRV_VERSION_MINOR 0  /*!< Version minor    */
#define CIFX_DRV_VERSION_BUILD 0  /*!< Version build    */
#define CIFX_DRV_VERSION_REV   0  /*!< Version revision */

#define CIFX_DRIVER_NAME       "cifX netXTransport Device Driver"  /*!< Version name */

/*****************************************************************************/
/*! cifX-Translation-Layer defines                                           */
/*****************************************************************************/
#define CIFX_DEVICE_DEFAULT_NAME "cifX"                                /*!< Suffix to be appended at the inferface name */

/*****************************************************************************/
/*! cifX-Translation-Layer information                                       */
/*****************************************************************************/
TAILQ_HEAD(CIFX_DEVICE_LIST, CIFX_TRANSPORT_DEVICE_INST_Ttag);         /*!< List of registered devices                  */

/*****************************************************************************/
/*!  \addtogroup CIFX_TL_STRUCTURE cifX Translation-Layer Structure Definitions
*    \{                                                                      */
/*****************************************************************************/
typedef struct CIFX_DRV_FUNCTION_TABLE_Ttag CIFX_DRV_FUNCTION_TABLE_T; /*!< cifX Function Pointer Table                 */

/*****************************************************************************/
/*! cifX-Translation-Layer information                                       */
/*****************************************************************************/
typedef struct DRV_TL_INFO_Ttag
{
  NXTHANDLE                  hNetXTransportDevGroup; /*!< handle of the netXTransport device group     */
  uint16_t                   usDataType;             /*!< Data type supported by the Translation-Layer */

  FUNCTION_POINTER_TABLE_T   tTLFctTable;            /*!< Generic function pointer table               */
  CIFX_DRV_FUNCTION_TABLE_T* ptCifXFctTable;         /*!< Translation-Layer specific function table    */

} DRV_TL_INFO_T, *PDRV_TL_INFO_T;                    /*!< Pointer to DRV_TL_INFO_T                     */

/*****************************************************************************/
/*! cifX API specific information                                            */
/*****************************************************************************/
typedef struct CIFX_DRIVER_INFO_Ttag
{
  int                     fInitialized;
  uint32_t                ulRefCount;
  NXTHANDLE               hNetXTransport;            /*!< Handle to netXTransport                      */
  uint32_t                ulTLInfoSize;              /*!< Size of atTLInfo                             */
  PDRV_TL_INFO_T          atTLInfo;                  /*!< Array of DRV_TL_INFO_T                       */

  struct CIFX_DEVICE_LIST tSysDeviceList;            /*!< List of handles to active systemdevices      */
  struct CIFX_DEVICE_LIST tChannelDeviceList;        /*!< List of handles to active devices            */
  void*                   pvDevListLock;             /*!< Lock for tSysDeviceList, tChannelDeviceList  */

} CIFX_DRIVER_INFO_T;

/*****************************************************************************/
/*! cifX device specific information                                         */
/*****************************************************************************/
typedef struct CIFX_TRANSPORT_DEVICE_INST_Ttag
{
  TAILQ_ENTRY(CIFX_TRANSPORT_DEVICE_INST_Ttag) tList;      /*!< List entry                               */

  NXTHANDLE                  hNetXTransportDeviceHandle;   /*!< Handle to the netXTransportDevice         */
  NXTHANDLE                  hSysDevice;                   /*!< Handle to the system device               */
  NXTHANDLE                  hChannelHandle;               /*!< Handle to the channel device              */
  FUNCTION_POINTER_TABLE_T   tTLFctTable;                  /*!< Generic function pointer table            */
  CIFX_DRV_FUNCTION_TABLE_T* ptCifXFctTable;               /*!< Handle to the channel device              */
  void*                      pvOpenCloseLock;              /*!< Translation-Layer specific function table */
  uint32_t                   ulRefCounter;                 /*!< Reference counter                         */

} CIFX_TRANSPORT_DEVICE_INST_T;
/*****************************************************************************/
/*! \}                                                                       */
/*****************************************************************************/

void cifXAPIInit  ( NXTHANDLE hNetXTransport);
void cifXAPIDeInit( NXTHANDLE hNetXTransport);

/*****************************************************************************/
/*!  \addtogroup CIFX_TL_FUNCTION_POINTER cifX Translation-Layer Functions Pointer Definition
*    \{                                                                      */
/*****************************************************************************/
typedef int32_t(APIENTRY *PFN_XDSDEVICEDOWNLOAD_TL     )( CIFXHANDLE hSysdevice, uint32_t ulChannel, uint32_t ulMode, char* szFileName, uint8_t* pabFileData, uint32_t ulFileSize,
                                                                          CIFX_TRANSPORT_DEVICE_INST_T* ptcifXTransportInst, PFN_PROGRESS_CALLBACK pfnCallback, PFN_RECV_PKT_CALLBACK pfnRecvPktCallback, void* pvUser);
typedef int32_t(APIENTRY *PFN_XSYSDEVICEUPLOAD_TL       )( CIFXHANDLE  hSysdevice, uint32_t ulChannel, uint32_t ulMode, char* szFileName, uint8_t* pabFileData, uint32_t* pulFileSize,
                                                            CIFX_TRANSPORT_DEVICE_INST_T* ptcifXTransportInst, PFN_PROGRESS_CALLBACK pfnCallback, PFN_RECV_PKT_CALLBACK pfnRecvPktCallback, void* pvUser);
typedef int32_t(APIENTRY *PFN_XCHANNELDOWNLOAD_TL         )( CIFXHANDLE hChannel, uint32_t ulMode, char* szFileName, uint8_t* pabFileData, uint32_t ulFileSize,
                                                                          CIFX_TRANSPORT_DEVICE_INST_T* ptcifXTransportInst, PFN_PROGRESS_CALLBACK pfnCallback, PFN_RECV_PKT_CALLBACK pfnRecvPktCallback, void* pvUser);
typedef int32_t(APIENTRY *PFN_XCHANNELUPLOAD_TL           )( CIFXHANDLE hChannel, uint32_t ulMode, char* szFileName, uint8_t* pabFileData, uint32_t* pulFileSize,
                                                                          CIFX_TRANSPORT_DEVICE_INST_T* ptcifXTransportInst, PFN_PROGRESS_CALLBACK pfnCallback, PFN_RECV_PKT_CALLBACK pfnRecvPktCallback, void* pvUser);

/*****************************************************************************/
/*! cifX API function pointer table                                          */
/*****************************************************************************/
struct CIFX_DRV_FUNCTION_TABLE_Ttag
{
  PFN_XDRIVEROPEN                 pfnxDriverOpen;
  PFN_XDRIVERCLOSE                pfnxDriverClose;
  PFN_XDRIVERGETINFORMATION       pfnxDriverGetInformation;
  PFN_XDRIVERGETERRORDESCRIPTION  pfnxDriverGetErrorDescription;
  PFN_XDRIVERENUMBOARDS           pfnxDriverEnumBoards;
  PFN_XDRIVERENUMCHANNELS         pfnxDriverEnumChannels;
  PFN_XDRIVERMEMORYPOINTER        pfnxDriverMemoryPointer;
  PFN_XSYSDEVICEOPEN              pfnxSysdeviceOpen;
  PFN_XSYSDEVICECLOSE             pfnxSysdeviceClose;
  PFN_XSYSDEVICERESET             pfnxSysdeviceReset;
  PFN_XSYSDEVICEGETMBXSTATE       pfnxSysdeviceGetMBXState;
  PFN_XSYSDEVICEPUTPACKET         pfnxSysdevicePutPacket;
  PFN_XSYSDEVICEGETPACKET         pfnxSysdeviceGetPacket;
  PFN_XDSDEVICEDOWNLOAD_TL        pfnxSysdeviceDownload;
  PFN_XSYSDEVICEUPLOAD_TL         pfnxSysdeviceUpload;
  PFN_XSYSDEVICEINFO              pfnxSysdeviceInfo;
  PFN_XSYSDEVICEFINDFIRSTFILE     pfnxSysdeviceFindFirstFile;
  PFN_XSYSDEVICEFINDNEXTFILE      pfnxSysdeviceFindNextFile;
  PFN_XCHANNELOPEN                pfnxChannelOpen;
  PFN_XCHANNELCLOSE               pfnxChannelClose;
  PFN_XCHANNELDOWNLOAD_TL         pfnxChannelDownload;
  PFN_XCHANNELUPLOAD_TL           pfnxChannelUpload;
  PFN_XCHANNELGETMBXSTATE         pfnxChannelGetMBXState;
  PFN_XCHANNELPUTPACKET           pfnxChannelPutPacket;
  PFN_XCHANNELGETPACKET           pfnxChannelGetPacket;
  PFN_XCHANNELGETSENDPACKET       pfnxChannelGetSendPacket;
  PFN_XCHANNELCONFIGLOCK          pfnxChannelConfigLock;
  PFN_XCHANNELRESET               pfnxChannelReset;
  PFN_XCHANNELINFO                pfnxChannelInfo;
  PFN_XCHANNELFINDFIRSTFILE       pfnxChannelFindFirstFile;
  PFN_XCHANNELFINDNEXTFILE        pfnxChannelFindNextFile;
  PFN_XCHANNELWATCHDOG            pfnxChannelWatchdog;
  PFN_XCHANNELHOSTSTATE           pfnxChannelHostState;
  PFN_XCHANNELBUSSTATE            pfnxChannelBusState;
  PFN_XCHANNELIOINFO              pfnxChannelIOInfo;
  PFN_XCHANNELIOREAD              pfnxChannelIORead;
  PFN_XCHANNELIOWRITE             pfnxChannelIOWrite;
  PFN_XCHANNELIOREADSENDDATA      pfnxChannelIOReadSendData;
  PFN_XCHANNELCONTROLBLOCK        pfnxChannelControlBlock;
  PFN_XCHANNELCOMMONSTATUSBLOCK   pfnxChannelCommonStatusBlock;
  PFN_XCHANNELEXTENDEDSTATUSBLOCK pfnxChannelExtendedStatusBlock;
  PFN_XCHANNELUSERBLOCK           pfnxChannelUserBlock;
  PFN_XCHANNELPLCMEMORYPTR        pfnxChannelPLCMemoryPtr;
  PFN_XCHANNELPLCISREADREADY      pfnxChannelPLCIsReadReady;
  PFN_XCHANNELPLCISWRITEREADY     pfnxChannelPLCIsWriteReady;
  PFN_XCHANNELPLCACTIVATEWRITE    pfnxChannelPLCActivateWrite;
  PFN_XCHANNELPLCACTIVATEREAD     pfnxChannelPLCActivateRead;

};
/*****************************************************************************/
/*! \}                                                                       */
/*****************************************************************************/

/*****************************************************************************/
/*!  \addtogroup CIFX_TL_HELPER Helper Functions
*    \{                                                                      */
/*****************************************************************************/
int32_t DEV_GetFWTransferTypeFromFileName( char* pszFileName, uint32_t* pulTransferType);

int     DEV_RemoveChannelFiles( void*                         pvChannel,
                                uint32_t                      ulChannel,
                                CIFX_TRANSPORT_DEVICE_INST_T* ptcifXTransportInst,
                                PFN_RECV_PKT_CALLBACK         pfnRecvPacket,
                                void*                         pvUser,
                                char*                         szExceptFile);

int32_t DEV_DownloadFile( void*                         pvChannel,
                          uint32_t                      ulChannel,
                          uint32_t                      ulMailboxSize,
                          uint32_t                      ulTransferType,
                          char*                         szFileName,
                          uint32_t                      ulFileLength,
                          void*                         pvData,
                          CIFX_TRANSPORT_DEVICE_INST_T* ptcifXTransportInst,
                          PFN_PROGRESS_CALLBACK         pfnCallback,
                          PFN_RECV_PKT_CALLBACK         pfnRecvPktCallback,
                          void*                         pvUser);

int32_t DEV_UploadFile( void*                         pvChannel,
                        uint32_t                      ulChannel,
                        uint32_t                      ulMailboxSize,
                        uint32_t                      ulTransferType,
                        char*                         szFileName,
                        uint32_t*                     pulDataBufferLen,
                        void*                         pvData,
                        CIFX_TRANSPORT_DEVICE_INST_T* ptcifXTransportInst,
                        PFN_PROGRESS_CALLBACK         pfnCallback,
                        PFN_RECV_PKT_CALLBACK         pfnRecvPktCallback,
                        void*                         pvUser);

int32_t DEV_DeleteFile( void*                         pvChannel,
                        uint32_t                      ulChannelNumber,
                        char*                         pszFileName,
                        CIFX_TRANSPORT_DEVICE_INST_T* ptcifXTransportInst,
                        PFN_RECV_PKT_CALLBACK         pfnRecvPacket,
                        void*                         pvUser);
/*****************************************************************************/
/*! \}                                                                       */
/*****************************************************************************/


#endif /* __CIFX_API__H */
