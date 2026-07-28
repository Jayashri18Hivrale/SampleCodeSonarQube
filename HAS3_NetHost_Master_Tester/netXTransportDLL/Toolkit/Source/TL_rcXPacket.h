                       /**************************************************************************************

   Copyright (c) Hilscher GmbH. All Rights Reserved.

 **************************************************************************************

   Filename:
    $Id: TL_rcXPacket.h 165 2013-06-14 13:44:13Z sebastiand $
   Last Modification:
    $Author: sebastiand $
    $Date: 2013-06-14 15:44:13 +0200 (Fr, 14. Jun 2013) $
    $Revision: 165 $

   Targets:
     O/S Independent : yes

   Description:
     Declaration of TL cifX rcXPacket Interface

   Changes:

     Version   Date        Author   Description
     ----------------------------------------------------------------------------------
     1         23.02.13    SD       initial version

**************************************************************************************/

/****************************************************************************/
/*! \file TL_rcXPacket.h
    Declaration of translation layer Interface                                     */
/****************************************************************************/

#ifndef __TL_RCXPACKET__H
#define __TL_RCXPACKET__H

#include "rcX_Public.h"
#include "rcX_User.h"

#ifdef __cplusplus
extern "C"
{
#endif

/* list of receive packets */
TAILQ_HEAD(RCX_PACKET_LIST_T, HIL_TRANSPORT_PACKET_Ttag);

/*****************************************************************************/
/*!  \addtogroup TL_RCXPACKET_STRUCTURE rcXPacket Translation-Layer Structure Definition
*    \{                                                                      */
/*****************************************************************************/
/*****************************************************************************/
/*! Management structure of a packet queue of specific channel               */
/*****************************************************************************/
typedef struct RCX_PACKET_CHANNEL_Ttag
{
  uint32_t                  ulAreaNr;           /*!< Device number                                */
  uint32_t                  ulChannelId;        /*!< Channel number                               */
  RCX_FW_IDENTIFICATION_T   tFirmwareInfo;      /*!< Firmware information structure               */
  void*                     hRecvSemaphore;     /*!< Required for receive process                 */
  struct RCX_PACKET_LIST_T  tRecvPackets;       /*!< List of all receive packets for this channel */
  void*                     pvOpenCloseLock;    /*!< Used to synchronize channel access           */

} RCX_PACKET_CHANNEL_T, *PRCX_PACKET_CHANNEL_T; /*!< Pointer to RCX_PACKET_CHANNEL_T              */

/*****************************************************************************/
/*! Management structure of for the packet queue of a specfic device         */
/*****************************************************************************/
typedef struct RCX_PACKET_DEVICE_Ttag
{
  uint32_t                    ulChannelCount;                           /*!< Number of channels listed in aptChannels */
  PRCX_PACKET_CHANNEL_T       aptChannels[NETX_MAX_SUPPORTED_CHANNELS]; /*!< Channel information structure            */
  char                        szDevicename[16];                         /*!< Name of the device                       */
  RCX_HW_IDENTIFY_CNF_DATA_T  tDeviceInfo;                              /*!< Device info                              */
  NETX_SYSTEM_INFO_BLOCK      tSystemInfo;                              /*!< Device info                              */

} RCX_PACKET_DEVICE_T, *PRCX_PACKET_DEVICE_T /*!< Pointer to RCX_PACKET_DEVICE_T           */;

/*****************************************************************************/
/*! Basic information of the packet managment structure of a device          */
/*! hidden by translation layer handle                                       */
/*****************************************************************************/
typedef struct RCX_PACKET_TL_INFO_Ttag
{
  void*                hTransport;              /*!< Handle to the dedicated transport layer instance */
  RCX_PACKET_DEVICE_T  tDevice;                 /*!< rcX Packet specifc device information            */

} RCX_PACKET_TL_INFO_T, *PRCX_PACKET_TL_INFO_T /*!< Pointer to RCX_PACKET_TL_INFO_T           */;

/*****************************************************************************/
/*! Basic device information                                                 */
/*! used for device managment in rcX-Packet layer                            */
/*****************************************************************************/
typedef struct CIFX_CHANNEL_HANDLE_Ttag
{
  uint32_t                  ulChannelNr;                /*!< Channel number                   */
  void*                     hSysDevice;                 /*!< Handle of the system device      */
  void*                     hChannel;                   /*!< Handle of the channel            */
  void*                     hNetxTransportDeviceHandle; /*!< Handle to netXTransport device   */

} CIFX_CHANNEL_HANDLE_T, *PCIFX_CHANNEL_HANDLE_T /*!< Pointer to CIFX_CHANNEL_HANDLE_T */;
/*****************************************************************************/
/*! \}                                                                       */
/*****************************************************************************/

/*! Translation-Layer specific functions */
int32_t          APIENTRY rcXPacket_Init        ( void* pvParam);  /*!< TL-Marshaller initialization function    */
void             APIENTRY rcXPacket_DeInit      ( void* pvParam);  /*!< TL-Marshaller de-initialization function */

#ifdef __cplusplus
}
#endif

#endif /* __TL_RCXPACKET__H */
