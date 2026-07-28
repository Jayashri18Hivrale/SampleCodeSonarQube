/**************************************************************************************

   Copyright (c) Hilscher GmbH. All Rights Reserved.

 **************************************************************************************

   Filename:
    $Id: cifXHelper.c 171 2013-06-17 13:27:09Z sebastiand $
   Last Modification:
    $Author: sebastiand $
    $Date: 2013-06-17 15:27:09 +0200 (Mo, 17. Jun 2013) $
    $Revision: 171 $

   Targets:
     OS-Independent : yes

   Description:
    Helper function for cifXAPI (packet type 0x100/0x200)

   Changes:

     Version   Date        Author   Description
     ----------------------------------------------------------------------------------
      1         23.02.13    SD      initial version

**************************************************************************************/

/****************************************************************************/
/*! \file cifXHelper.c
    Auxiliary functions of the cifX Driver API functions                    */
/****************************************************************************/

#include <OS_Includes.h>
#include "OS_Dependent.h"
#include "cifXUser.h"
#include "cifXErrors.h"
#include "rcX_Public.h"
#include "HilFileHeaderV3.h"
#include "cifXAPI.h"
#include "Endianess.h"

/*****************************************************************************/
/*!  \addtogroup CIFX_TL_HELPER cifX Translation-Layer Helper Functions
*    \{                                                                      */
/*****************************************************************************/

/*****************************************************************************/
/*! CRC 32 lookup table                                                      */
/*****************************************************************************/
static uint32_t Crc32Table[256]=
{
  0x00000000UL, 0x77073096UL, 0xee0e612cUL, 0x990951baUL, 0x076dc419UL,
  0x706af48fUL, 0xe963a535UL, 0x9e6495a3UL, 0x0edb8832UL, 0x79dcb8a4UL,
  0xe0d5e91eUL, 0x97d2d988UL, 0x09b64c2bUL, 0x7eb17cbdUL, 0xe7b82d07UL,
  0x90bf1d91UL, 0x1db71064UL, 0x6ab020f2UL, 0xf3b97148UL, 0x84be41deUL,
  0x1adad47dUL, 0x6ddde4ebUL, 0xf4d4b551UL, 0x83d385c7UL, 0x136c9856UL,
  0x646ba8c0UL, 0xfd62f97aUL, 0x8a65c9ecUL, 0x14015c4fUL, 0x63066cd9UL,
  0xfa0f3d63UL, 0x8d080df5UL, 0x3b6e20c8UL, 0x4c69105eUL, 0xd56041e4UL,
  0xa2677172UL, 0x3c03e4d1UL, 0x4b04d447UL, 0xd20d85fdUL, 0xa50ab56bUL,
  0x35b5a8faUL, 0x42b2986cUL, 0xdbbbc9d6UL, 0xacbcf940UL, 0x32d86ce3UL,
  0x45df5c75UL, 0xdcd60dcfUL, 0xabd13d59UL, 0x26d930acUL, 0x51de003aUL,
  0xc8d75180UL, 0xbfd06116UL, 0x21b4f4b5UL, 0x56b3c423UL, 0xcfba9599UL,
  0xb8bda50fUL, 0x2802b89eUL, 0x5f058808UL, 0xc60cd9b2UL, 0xb10be924UL,
  0x2f6f7c87UL, 0x58684c11UL, 0xc1611dabUL, 0xb6662d3dUL, 0x76dc4190UL,
  0x01db7106UL, 0x98d220bcUL, 0xefd5102aUL, 0x71b18589UL, 0x06b6b51fUL,
  0x9fbfe4a5UL, 0xe8b8d433UL, 0x7807c9a2UL, 0x0f00f934UL, 0x9609a88eUL,
  0xe10e9818UL, 0x7f6a0dbbUL, 0x086d3d2dUL, 0x91646c97UL, 0xe6635c01UL,
  0x6b6b51f4UL, 0x1c6c6162UL, 0x856530d8UL, 0xf262004eUL, 0x6c0695edUL,
  0x1b01a57bUL, 0x8208f4c1UL, 0xf50fc457UL, 0x65b0d9c6UL, 0x12b7e950UL,
  0x8bbeb8eaUL, 0xfcb9887cUL, 0x62dd1ddfUL, 0x15da2d49UL, 0x8cd37cf3UL,
  0xfbd44c65UL, 0x4db26158UL, 0x3ab551ceUL, 0xa3bc0074UL, 0xd4bb30e2UL,
  0x4adfa541UL, 0x3dd895d7UL, 0xa4d1c46dUL, 0xd3d6f4fbUL, 0x4369e96aUL,
  0x346ed9fcUL, 0xad678846UL, 0xda60b8d0UL, 0x44042d73UL, 0x33031de5UL,
  0xaa0a4c5fUL, 0xdd0d7cc9UL, 0x5005713cUL, 0x270241aaUL, 0xbe0b1010UL,
  0xc90c2086UL, 0x5768b525UL, 0x206f85b3UL, 0xb966d409UL, 0xce61e49fUL,
  0x5edef90eUL, 0x29d9c998UL, 0xb0d09822UL, 0xc7d7a8b4UL, 0x59b33d17UL,
  0x2eb40d81UL, 0xb7bd5c3bUL, 0xc0ba6cadUL, 0xedb88320UL, 0x9abfb3b6UL,
  0x03b6e20cUL, 0x74b1d29aUL, 0xead54739UL, 0x9dd277afUL, 0x04db2615UL,
  0x73dc1683UL, 0xe3630b12UL, 0x94643b84UL, 0x0d6d6a3eUL, 0x7a6a5aa8UL,
  0xe40ecf0bUL, 0x9309ff9dUL, 0x0a00ae27UL, 0x7d079eb1UL, 0xf00f9344UL,
  0x8708a3d2UL, 0x1e01f268UL, 0x6906c2feUL, 0xf762575dUL, 0x806567cbUL,
  0x196c3671UL, 0x6e6b06e7UL, 0xfed41b76UL, 0x89d32be0UL, 0x10da7a5aUL,
  0x67dd4accUL, 0xf9b9df6fUL, 0x8ebeeff9UL, 0x17b7be43UL, 0x60b08ed5UL,
  0xd6d6a3e8UL, 0xa1d1937eUL, 0x38d8c2c4UL, 0x4fdff252UL, 0xd1bb67f1UL,
  0xa6bc5767UL, 0x3fb506ddUL, 0x48b2364bUL, 0xd80d2bdaUL, 0xaf0a1b4cUL,
  0x36034af6UL, 0x41047a60UL, 0xdf60efc3UL, 0xa867df55UL, 0x316e8eefUL,
  0x4669be79UL, 0xcb61b38cUL, 0xbc66831aUL, 0x256fd2a0UL, 0x5268e236UL,
  0xcc0c7795UL, 0xbb0b4703UL, 0x220216b9UL, 0x5505262fUL, 0xc5ba3bbeUL,
  0xb2bd0b28UL, 0x2bb45a92UL, 0x5cb36a04UL, 0xc2d7ffa7UL, 0xb5d0cf31UL,
  0x2cd99e8bUL, 0x5bdeae1dUL, 0x9b64c2b0UL, 0xec63f226UL, 0x756aa39cUL,
  0x026d930aUL, 0x9c0906a9UL, 0xeb0e363fUL, 0x72076785UL, 0x05005713UL,
  0x95bf4a82UL, 0xe2b87a14UL, 0x7bb12baeUL, 0x0cb61b38UL, 0x92d28e9bUL,
  0xe5d5be0dUL, 0x7cdcefb7UL, 0x0bdbdf21UL, 0x86d3d2d4UL, 0xf1d4e242UL,
  0x68ddb3f8UL, 0x1fda836eUL, 0x81be16cdUL, 0xf6b9265bUL, 0x6fb077e1UL,
  0x18b74777UL, 0x88085ae6UL, 0xff0f6a70UL, 0x66063bcaUL, 0x11010b5cUL,
  0x8f659effUL, 0xf862ae69UL, 0x616bffd3UL, 0x166ccf45UL, 0xa00ae278UL,
  0xd70dd2eeUL, 0x4e048354UL, 0x3903b3c2UL, 0xa7672661UL, 0xd06016f7UL,
  0x4969474dUL, 0x3e6e77dbUL, 0xaed16a4aUL, 0xd9d65adcUL, 0x40df0b66UL,
  0x37d83bf0UL, 0xa9bcae53UL, 0xdebb9ec5UL, 0x47b2cf7fUL, 0x30b5ffe9UL,
  0xbdbdf21cUL, 0xcabac28aUL, 0x53b39330UL, 0x24b4a3a6UL, 0xbad03605UL,
  0xcdd70693UL, 0x54de5729UL, 0x23d967bfUL, 0xb3667a2eUL, 0xc4614ab8UL,
  0x5d681b02UL, 0x2a6f2b94UL, 0xb40bbe37UL, 0xc30c8ea1UL, 0x5a05df1bUL,
  0x2d02ef8d
};

/*****************************************************************************/
/*! Create a CRC32 value from the given buffer data
*   \param ulCRC Continued CRC32 value
*   \param pabBuffer Buffer to create the CRC from
*   \param ulLength Buffer length
*   \return CRC32 value                                                      */
/*****************************************************************************/
static uint32_t CreateCRC32(uint32_t ulCRC, uint8_t* pabBuffer, uint32_t ulLength)
{
  if( (0 == pabBuffer) || (0 == ulLength) ) return ulCRC;
  ulCRC = ulCRC ^ 0xffffffff;
  for(;ulLength > 0; --ulLength)
  {
    ulCRC = (Crc32Table[((ulCRC) ^ (*(pabBuffer++)) ) & 0xff] ^ ((ulCRC) >> 8));
  }
  return ulCRC ^ 0xffffffff;
}

/*****************************************************************************/
/*! Check if the given file is a firmware file
*   \param pszFileName   Name of the file
*   \return 1 on success                                                     */
/*****************************************************************************/
int DEV_IsFWFile( char* pszFileName)
{
  /* Check if we have a .NXO, .NXF,.NXM or .MOD extension */
  int fRet    = 0;
  int iStrlen = OS_Strlen(pszFileName);

  if( iStrlen >= CIFX_MIN_FILE_NAME_LENGTH)  /* At least x.abc */
  {
    if ( (0 == OS_Strnicmp( HIL_FILE_EXTENSION_FIRMWARE, &pszFileName[iStrlen - 4], 4) ) ||
         (0 == OS_Strnicmp( HIL_FILE_EXTENSION_MODULE, &pszFileName[iStrlen - 4], 4) )   ||
         (0 == OS_Strnicmp( HIL_FILE_EXTENSION_OPTION, &pszFileName[iStrlen - 4], 4) )   ||
         (0 == OS_Strnicmp( ".MOD", &pszFileName[iStrlen - 4], 4) )  )
    {
      fRet = 1;
    }
  }

  return fRet;
}

/*****************************************************************************/
/*! Retrieves the firmware transfer type from file name.
*   \param pszFileName      Input file name
*   \param pulTransferType  Buffer for transfer type
*   \return CIFX_NO_ERROR on success                                         */
/*****************************************************************************/
int32_t DEV_GetFWTransferTypeFromFileName( char* pszFileName, uint32_t* pulTransferType)
{
  /* Check if we have a NXF or .NXM / .MOD extension */
  int32_t lRet = CIFX_NO_ERROR;

  int iStrlen = OS_Strlen(pszFileName);
  if( iStrlen < CIFX_MIN_FILE_NAME_LENGTH)  /* At least x.abc */
  {
    lRet = CIFX_FILE_NAME_INVALID;
  } else
  {
    /* Check if we have a valid firmware file */
    if (!DEV_IsFWFile( pszFileName))
    {
      lRet = CIFX_FILE_TYPE_INVALID;
    } else
    {
      /* We have a firmware file, choose the correct download type */
      if ( (0 == OS_Strnicmp( HIL_FILE_EXTENSION_MODULE, &pszFileName[iStrlen - 4], 4) ) ||
           (0 == OS_Strnicmp( HIL_FILE_EXTENSION_OPTION, &pszFileName[iStrlen - 4], 4) ) ||
           (0 == OS_Strnicmp( ".MOD", &pszFileName[iStrlen - 4], 4) )  )
      {
        /* We are using the module file transfer type */
        *pulTransferType = RCX_FILE_XFER_MODULE;
      } else
      {
        /* All other files are downloaded via the file transfer type */
        *pulTransferType = RCX_FILE_XFER_FILE;
      }
    }
  }

  return lRet;
}

/*****************************************************************************/
/*! Exchanges a packet with the device
*   \param pvChannel        Channel instance to exchange a packet
*   \param ptSendPkt        Send packet pointer
*   \param ptRecvPkt        Pointer to place received Packet in
*   \param ulRecvBufferSize Length of the receive buffer
*   \param ulTimeout        Maximum time in ms to wait for an empty mailbox
*   \param ptcifXTransportInst pointer to netXTransport Device Instance
*   \param pvPktCallback    Packet callback for unhandled receive packets
*   \param pvUser           User data for callback function
*   \return CIFX_NO_ERROR on success                                         */
/*****************************************************************************/
int32_t DEV_TransferPacket( void*                       pvChannel,
                             CIFX_PACKET*               ptSendPkt,
                             CIFX_PACKET*               ptRecvPkt,
                             uint32_t                   ulRecvBufferSize,
                             uint32_t                   ulTimeout,
                             CIFX_TRANSPORT_DEVICE_INST_T* ptcifXTransportInst,
                             PFN_RECV_PKT_CALLBACK      pvPktCallback,
                             void*                      pvUser)
{
  int32_t lRet = CIFX_NO_ERROR;

  if( (lRet = ptcifXTransportInst->ptCifXFctTable->pfnxSysdevicePutPacket(pvChannel, ptSendPkt, ulTimeout)) == CIFX_NO_ERROR)
  {
    if( (lRet = ptcifXTransportInst->ptCifXFctTable->pfnxSysdeviceGetPacket(pvChannel, ulRecvBufferSize, ptRecvPkt, ulTimeout)) == CIFX_NO_ERROR)
    {
      /* Check if we got the answer */
      if((LE32_TO_HOST(ptRecvPkt->tHeader.ulCmd) & ~RCX_MSK_PACKET_ANSWER) == LE32_TO_HOST(ptSendPkt->tHeader.ulCmd) )
      {
        /* Check rest of packet data */
        if ( (ptRecvPkt->tHeader.ulSrc    == ptSendPkt->tHeader.ulSrc)    &&
              (ptRecvPkt->tHeader.ulId    == ptSendPkt->tHeader.ulId)     &&
              (ptRecvPkt->tHeader.ulSrcId == ptSendPkt->tHeader.ulSrcId)  )
        {
          /* We got the answer message */
          /* lRet = ptRecvPkt->tHeader.ulState; */ /* Do not deliever back this information */

        } else
        {
          /* This is not our packet, check if the user wants it */
          if( NULL != pvPktCallback)
          {
            pvPktCallback(ptRecvPkt, pvUser);
          }
        }
      } else
      {
        lRet = CIFX_DEV_GET_TIMEOUT;
      }
    }
  }
  return lRet;
}

/*****************************************************************************/
/*! Download a file to the hardware.
*   \param pvChannel          Channel instance the download is performed on
*   \param ulChannel          Channel number the download is for
*   \param ulMailboxSize      Size of the mailbox
*   \param ulTransferType     Type of transfer (see RCX_FILE_XFER_XXX defines)
*   \param szFileName         Short file name (needed by firmware to create the file by name)
*   \param ulFileLength       Length of the file to download
*   \param pvData             File data being downloaded
*   \param ptcifXTransportInst pointer to netXTransport Device Instance
*   \param pfnCallback        User callback for download progress indications
*   \param pfnRecvPktCallback User callback for unsolicited receive packets
*   \param pvUser             User parameter passed on callback
*   \return CIFX_NO_ERROR on success                                         */
/*****************************************************************************/
int32_t DEV_DownloadFile(void*                       pvChannel,
                          uint32_t                   ulChannel,
                          uint32_t                   ulMailboxSize,
                          uint32_t                   ulTransferType,
                          char*                      szFileName,
                          uint32_t                   ulFileLength,
                          void*                      pvData,
                          CIFX_TRANSPORT_DEVICE_INST_T* ptcifXTransportInst,
                          PFN_PROGRESS_CALLBACK      pfnCallback,
                          PFN_RECV_PKT_CALLBACK      pfnRecvPktCallback,
                          void*                      pvUser)
{
  /* Usually one brace should be enough, but GNU wants to have a second brace
     to initialize the structure. On GCC 4.0.3 the whole structure is initialized
     as described in ISOC90 */
  CIFX_PACKET  tSendPkt  = {{0}};
  CIFX_PACKET  tRecvPkt  = {{0}};

  /* Set download state informations */
  uint32_t  ulMaxDataLength   = ulMailboxSize -  /* Maximum possible user data length */
                                        sizeof(RCX_FILE_DOWNLOAD_DATA_REQ_T);
  uint32_t ulSendLen          = 0;
  uint32_t ulTransferedLength = 0;
  uint8_t* pabActData         = NULL;
  uint32_t ulCRC              = 0;
  uint32_t ulBlockNumber      = 0;
  uint32_t ulState            = RCX_FILE_DOWNLOAD_REQ;
  uint32_t ulCmdDataState     = RCX_PACKET_SEQ_NONE;
  int      fStopDownload      = 0;
  int32_t  lRetAbort          = CIFX_NO_ERROR;
  int32_t  lRet               = CIFX_NO_ERROR;
  uint32_t ulCurrentId        = 0;

  /* Performce download */
  do
  {
    switch (ulState)
    {
      /* Send download request */
      case RCX_FILE_DOWNLOAD_REQ:
      {
        RCX_FILE_DOWNLOAD_REQ_T*  ptSendPkt = (RCX_FILE_DOWNLOAD_REQ_T*)&tSendPkt;
        RCX_FILE_DOWNLOAD_CNF_T*  ptRecvPkt = (RCX_FILE_DOWNLOAD_CNF_T*)&tRecvPkt;

        /* Validate filename length to fit mailbox/packet */
        uint32_t ulFileNameLength = MIN( (uint32_t)OS_Strlen(szFileName) + 1,
                                              ulMailboxSize - sizeof(RCX_FILE_DOWNLOAD_REQ_T));

        /* Insert packet data */
        ++ulCurrentId;
        ptSendPkt->tHead.ulDest   = HOST_TO_LE32(RCX_PACKET_DEST_SYSTEM);
        ptSendPkt->tHead.ulSrc    = HOST_TO_LE32((uint32_t)PTR2LONG(pvChannel));
        ptSendPkt->tHead.ulDestId = HOST_TO_LE32(0);
        ptSendPkt->tHead.ulSrcId  = HOST_TO_LE32(0);
        ptSendPkt->tHead.ulLen    = HOST_TO_LE32((uint32_t)(sizeof(RCX_FILE_DOWNLOAD_REQ_DATA_T) +
                                                                 ulFileNameLength));
        ptSendPkt->tHead.ulId     = HOST_TO_LE32(ulCurrentId);
        ptSendPkt->tHead.ulSta    = HOST_TO_LE32(0);
        ptSendPkt->tHead.ulCmd    = HOST_TO_LE32(RCX_FILE_DOWNLOAD_REQ);
        ptSendPkt->tHead.ulExt    = HOST_TO_LE32(ulCmdDataState);
        ptSendPkt->tHead.ulRout   = HOST_TO_LE32(0);

        /* Insert command data (extended data) */
        ptSendPkt->tData.ulFileLength     = HOST_TO_LE32(ulFileLength);
        ptSendPkt->tData.ulMaxBlockSize   = HOST_TO_LE32(ulMaxDataLength);
        ptSendPkt->tData.ulXferType       = HOST_TO_LE32(ulTransferType);
        ptSendPkt->tData.ulChannelNo      = HOST_TO_LE32(ulChannel);
        ptSendPkt->tData.usFileNameLength = HOST_TO_LE16((uint16_t)ulFileNameLength);
        OS_Strncpy( (char*)(ptSendPkt + 1),
                    ulFileNameLength,
                    szFileName,
                    ulFileNameLength) ;

        /* Transfer packet */
        lRet = DEV_TransferPacket(pvChannel,
                                  &tSendPkt,
                                  &tRecvPkt,
                                  (uint32_t)sizeof(RCX_FILE_DOWNLOAD_CNF_T),
                                  CIFX_TO_SEND_PACKET,
                                  ptcifXTransportInst,
                                  pfnRecvPktCallback,
                                  pvUser);

        if( (CIFX_NO_ERROR != lRet)                                 ||
            (RCX_S_OK      != (lRet = LE32_TO_HOST((int32_t)ptRecvPkt->tHead.ulSta))) )
        {
          /* Error during first packet, end download */
          /* Send progress notification */
          if(pfnCallback)
            pfnCallback(ulTransferedLength, ulFileLength, pvUser, CIFX_CALLBACK_FINISHED, lRet);

          /* Send abort request on unusable data */
          ulState = RCX_FILE_DOWNLOAD_ABORT_REQ;
        } else if( LE32_TO_HOST(ptRecvPkt->tData.ulMaxBlockSize) == 0)
        {
          /* Error in device information, stop download (Device returned illegal block size */
          lRet = CIFX_INVALID_ACCESS_SIZE;

          /* Send progress notification */
          if(pfnCallback)
            pfnCallback(ulTransferedLength, ulFileLength, pvUser, CIFX_CALLBACK_FINISHED, lRet);

          /* Send abort request on unusable data */
          ulState = RCX_FILE_DOWNLOAD_ABORT_REQ;
        } else
        {
          /* Everything went ok, so start transmitting file data now */
          pabActData = (uint8_t*)pvData;

          /* Get download packet size from the device confirmation.
             If the devices packet size is smaller than our size, use the length from the device.
             Otherwise use our length. */
          if( ulMaxDataLength > LE32_TO_HOST(ptRecvPkt->tData.ulMaxBlockSize))
            ulMaxDataLength = LE32_TO_HOST(ptRecvPkt->tData.ulMaxBlockSize);

          /* Check if the file fits into one packet or if we have to send multiple packets */
          ulSendLen = ulMaxDataLength;
          if(ulFileLength <= ulSendLen)
          {
            /* We have only one packet to send */
            ulSendLen       = ulFileLength;
            ulCmdDataState  = RCX_PACKET_SEQ_NONE;
          } else
          {
            /* We have to send multiple packets */
            ulCmdDataState  = RCX_PACKET_SEQ_FIRST;
          }

          /* Goto next state */
          ulState = RCX_FILE_DOWNLOAD_DATA_REQ;
        }
      }
      break;

      /* Data download packets */
      case RCX_FILE_DOWNLOAD_DATA_REQ:
      {
        RCX_FILE_DOWNLOAD_DATA_REQ_T* ptSendPkt = (RCX_FILE_DOWNLOAD_DATA_REQ_T*)&tSendPkt;
        RCX_FILE_DOWNLOAD_DATA_CNF_T* ptRecvPkt = (RCX_FILE_DOWNLOAD_DATA_CNF_T*)&tRecvPkt;

        ++ulCurrentId;
        ptSendPkt->tHead.ulDest = HOST_TO_LE32(RCX_PACKET_DEST_SYSTEM);
        ptSendPkt->tHead.ulSrc  = HOST_TO_LE32((uint32_t)PTR2LONG(pvChannel));
        ptSendPkt->tHead.ulCmd  = HOST_TO_LE32(RCX_FILE_DOWNLOAD_DATA_REQ);
        ptSendPkt->tHead.ulId   = HOST_TO_LE32(ulCurrentId);
        ptSendPkt->tHead.ulExt  = HOST_TO_LE32(ulCmdDataState);

        /* Copy file data to packet */
        OS_Memcpy( &(ptSendPkt->tData) + 1, pabActData, ulSendLen);

        /* Adjust packet length */
        ptSendPkt->tHead.ulLen = HOST_TO_LE32((uint32_t)(sizeof(RCX_FILE_DOWNLOAD_DATA_REQ_DATA_T) +
                                                                   ulSendLen));

        /* Create continued CRC */
        ulCRC = CreateCRC32( ulCRC, pabActData, ulSendLen);
        ptSendPkt->tData.ulChksum   = HOST_TO_LE32(ulCRC);
        ptSendPkt->tData.ulBlockNo  = HOST_TO_LE32(ulBlockNumber);
        ++ulBlockNumber;

        /* Transfer packet */
        lRet = DEV_TransferPacket(pvChannel,
                                  (CIFX_PACKET*)ptSendPkt,
                                  (CIFX_PACKET*)ptRecvPkt,
                                  (uint32_t)sizeof(RCX_FILE_DOWNLOAD_DATA_CNF_T),
                                  CIFX_TO_SEND_PACKET,
                                  ptcifXTransportInst,
                                  pfnRecvPktCallback,
                                  pvUser);

        if( (CIFX_NO_ERROR != lRet)                                   ||
            (RCX_S_OK      != (lRet = LE32_TO_HOST((int32_t)(ptRecvPkt->tHead.ulSta)))) )
        {
          /* Driver error during transfer packet, end download */
          /* Always try to send an abort request */
          if(pfnCallback)
            pfnCallback(ulTransferedLength, ulFileLength, pvUser, CIFX_CALLBACK_FINISHED, lRet);

          ulState = RCX_FILE_DOWNLOAD_ABORT_REQ;
        } else
        {
          /* Add send size to transfered size */
          ulTransferedLength += ulSendLen;

          /* Indicate progress, if user wants a notification */
          if(pfnCallback)
            pfnCallback(ulTransferedLength, ulFileLength, pvUser,
                        (ulTransferedLength == ulFileLength) ? CIFX_CALLBACK_FINISHED : CIFX_CALLBACK_ACTIVE,
                        lRet);

          /* Check if we are done with the download */
          if( (RCX_PACKET_SEQ_LAST == ulCmdDataState) ||
              (RCX_PACKET_SEQ_NONE == ulCmdDataState) )
          {
            /* No more packets to send, end download */
            fStopDownload = 1;
          } else
          {
            /* Move data pointer to next data */
            pabActData += ulSendLen;

            /* Calculate next message length */
            if ( ulFileLength <= (ulSendLen + ulTransferedLength))
            {
              /* Set the send length to rest of data,
                 This will be the last packet */
              ulSendLen = ulFileLength - ulTransferedLength;
              ulCmdDataState = RCX_PACKET_SEQ_LAST;
            } else
            {
              ulCmdDataState = RCX_PACKET_SEQ_MIDDLE;
            }

            /* Goto next state */
            ulState = RCX_FILE_DOWNLOAD_DATA_REQ;
          }
        }
      }
      break;

      /* Abort active download */
      case RCX_FILE_DOWNLOAD_ABORT_REQ:
      {
        RCX_FILE_DOWNLOAD_ABORT_REQ_T*  ptSendPkt = (RCX_FILE_DOWNLOAD_ABORT_REQ_T*)&tSendPkt;
        RCX_FILE_DOWNLOAD_ABORT_CNF_T*  ptRecvPkt = (RCX_FILE_DOWNLOAD_ABORT_CNF_T*)&tRecvPkt;

        ++ulCurrentId;
        ptSendPkt->tHead.ulDest   = HOST_TO_LE32(RCX_PACKET_DEST_SYSTEM);
        ptSendPkt->tHead.ulSrc    = HOST_TO_LE32((uint32_t)PTR2LONG(pvChannel));
        ptSendPkt->tHead.ulDestId = HOST_TO_LE32(0);
        ptSendPkt->tHead.ulSrcId  = HOST_TO_LE32(0);
        ptSendPkt->tHead.ulLen    = HOST_TO_LE32(0);
        ptSendPkt->tHead.ulId     = HOST_TO_LE32(ulCurrentId);
        ptSendPkt->tHead.ulSta    = HOST_TO_LE32(0);
        ptSendPkt->tHead.ulCmd    = HOST_TO_LE32(RCX_FILE_DOWNLOAD_ABORT_REQ);
        ptSendPkt->tHead.ulExt    = HOST_TO_LE32(RCX_PACKET_SEQ_NONE);
        ptSendPkt->tHead.ulRout   = HOST_TO_LE32(0);

        /* Transfer packet */
        lRetAbort = DEV_TransferPacket(pvChannel,
                                       &tSendPkt,
                                       &tRecvPkt,
                                       sizeof(RCX_FILE_DOWNLOAD_ABORT_CNF_T),
                                       CIFX_TO_SEND_PACKET,
                                       ptcifXTransportInst,
                                       pfnRecvPktCallback,
                                       pvUser);

        if( lRetAbort == CIFX_NO_ERROR)
        {
          /* Return packet state if function succeeded */
          lRetAbort = LE32_TO_HOST((int32_t)ptRecvPkt->tHead.ulSta);
        }

        /* End download */
        fStopDownload = 1;
      }
      break;

      default:
        /* unkonwn, leave command */
        lRet = CIFX_FUNCTION_FAILED;

        /* End download */
        fStopDownload = 1;
        break;
    }

  } while(!fStopDownload);

  /* Always return lRet first, thean abort error */
  if( CIFX_NO_ERROR != lRet)
    return lRet;
  else if( CIFX_NO_ERROR != lRetAbort)
    return lRetAbort;
  else
    return CIFX_NO_ERROR;
}

/*****************************************************************************/
/*! Uploads a file from the hardware. It is required to list the files
* on the hardware, to know the file length for creating the buffer.
*   \param pvChannel          Channel instance the upload is performed on
*   \param ulChannel          Channel number the upload made is for
*   \param ulMailboxSize      Size of the mailbox
*   \param ulTransferType     Type of transfer (see RCX_FILE_XFER_XXX defines)
*   \param szFileName         Short file name
*   \param pulDataBufferLen   Length of the provided buffer, returned length of data
*   \param pvData             Buffer for storing upload. This buffer must be allocated by the caller.
*   \param ptcifXTransportInst pointer to netXTransport Device Instance
*   \param pfnCallback        User callback for upload progress indications
*   \param pfnRecvPktCallback User callback for unsolicited receive packets
*   \param pvUser             User parameter passed on callback
*   \return CIFX_NO_ERROR on success                                         */
/*****************************************************************************/
int32_t DEV_UploadFile(void*                       pvChannel,
                        uint32_t                   ulChannel,
                        uint32_t                   ulMailboxSize,
                        uint32_t                   ulTransferType,
                        char*                      szFileName,
                        uint32_t*                  pulDataBufferLen,
                        void*                      pvData,
                        CIFX_TRANSPORT_DEVICE_INST_T* ptcifXTransportInst,
                        PFN_PROGRESS_CALLBACK      pfnCallback,
                        PFN_RECV_PKT_CALLBACK      pfnRecvPktCallback,
                        void*                      pvUser)
{
  /* Usually one brace should be enough, but GNU wants to have a second brace
     to initialize the structure. On GCC 4.0.3 the whole structure is initialized
     as described in ISOC90 */
  CIFX_PACKET                 tSendPacket     = {{0}};
  CIFX_PACKET                 tRecvPacket     = {{0}};
  RCX_FILE_UPLOAD_REQ_T*      ptUploadReq     = (RCX_FILE_UPLOAD_REQ_T*)&tSendPacket;
  RCX_FILE_UPLOAD_CNF_T*      ptUploadCnf     = (RCX_FILE_UPLOAD_CNF_T*)&tRecvPacket;
  RCX_FILE_UPLOAD_DATA_REQ_T* ptUploadDataReq = (RCX_FILE_UPLOAD_DATA_REQ_T*)&tSendPacket;
  RCX_FILE_UPLOAD_DATA_CNF_T* ptUploadDataCnf = (RCX_FILE_UPLOAD_DATA_CNF_T*)&tRecvPacket;
  uint16_t                    usFilenameLen   = (uint16_t)(OS_Strlen(szFileName) + 1); /*Firmware expects length including terminating NULL */
  uint32_t                    ulBlockSize     = ulMailboxSize -
                                                    sizeof(*ptUploadDataCnf); /* maximum size of each file block */
  int                         fSendAbort      = 0;
  int32_t                     lRet            = CIFX_NO_ERROR;

  /* Check parameters */
  if( (NULL == pvData) || (NULL == pulDataBufferLen) )
    return CIFX_INVALID_POINTER;

  if( ulMailboxSize < NETX_SYSTEM_MAILBOX_MIN_SIZE)
    return CIFX_DEV_MAILBOX_TOO_SHORT;

  ptUploadReq->tHead.ulDest           = HOST_TO_LE32(RCX_PACKET_DEST_SYSTEM);
  ptUploadReq->tHead.ulSrc            = HOST_TO_LE32((uint32_t)PTR2LONG(pvChannel));
  ptUploadReq->tHead.ulCmd            = HOST_TO_LE32(RCX_FILE_UPLOAD_REQ);
  ptUploadReq->tHead.ulExt            = HOST_TO_LE32(RCX_PACKET_SEQ_NONE);
  ptUploadReq->tHead.ulLen            = HOST_TO_LE32((uint32_t)(sizeof(ptUploadReq->tData) + usFilenameLen));
  ptUploadReq->tHead.ulId             = HOST_TO_LE32(0);
  ptUploadReq->tData.usFileNameLength = HOST_TO_LE16(usFilenameLen);
  ptUploadReq->tData.ulXferType       = HOST_TO_LE32(ulTransferType);
  ptUploadReq->tData.ulMaxBlockSize   = HOST_TO_LE32(ulBlockSize);
  ptUploadReq->tData.ulChannelNo      = HOST_TO_LE32(ulChannel);

  OS_Strncpy((char*)(ptUploadReq + 1), usFilenameLen, szFileName, usFilenameLen);

  lRet = DEV_TransferPacket(pvChannel,
                            &tSendPacket,
                            &tRecvPacket,
                            sizeof(tRecvPacket),
                            CIFX_TO_SEND_PACKET,
                            ptcifXTransportInst,
                            pfnRecvPktCallback,
                            pvUser);
  if( (CIFX_NO_ERROR != lRet)                               ||
      (RCX_S_OK      != (lRet = LE32_TO_HOST(tRecvPacket.tHeader.ulState))) )
  {
    /* Send progress notification */
    if(pfnCallback)
      pfnCallback( 0, 0, pvUser, CIFX_CALLBACK_FINISHED, lRet);

    fSendAbort = 1;
  } else
  {
    uint32_t ulFileLength = LE32_TO_HOST(ptUploadCnf->tData.ulFileLength);

    if(ulFileLength > *pulDataBufferLen)
    {
      fSendAbort = 1;
      lRet = CIFX_INVALID_BUFFERSIZE;
    } else
    {
      uint32_t  ulCRC              = 0;
      uint8_t* pbData             = (uint8_t*)pvData; /* pointer to return buffer */
      uint32_t  ulTransferredBytes = 0;
      uint32_t  ulTotalBytes       = ulFileLength;
      uint32_t  ulCurrentId        = 0;

      /* Set user file read data to 0 */
      *pulDataBufferLen = 0;

      /* Create upload data packet */
      OS_Memset( ptUploadDataReq,0, sizeof(*ptUploadDataReq));
      ptUploadDataReq->tHead.ulDest         = HOST_TO_LE32(RCX_PACKET_DEST_SYSTEM);
      ptUploadDataReq->tHead.ulSrc          = HOST_TO_LE32((uint32_t)PTR2LONG(pvChannel));
      ptUploadDataReq->tHead.ulCmd          = HOST_TO_LE32(RCX_FILE_UPLOAD_DATA_REQ);
      ptUploadDataReq->tHead.ulExt          = HOST_TO_LE32(RCX_PACKET_SEQ_NONE);
      ptUploadDataReq->tHead.ulId           = HOST_TO_LE32(ulCurrentId);
      ptUploadDataReq->tHead.ulLen          = HOST_TO_LE32(0);

      /* Adjust block size to the size of the system */
      if( LE32_TO_HOST(ptUploadCnf->tData.ulMaxBlockSize) < ulBlockSize)
         ulBlockSize = LE32_TO_HOST(ptUploadCnf->tData.ulMaxBlockSize);

      /* Check size we have to send */
      /* If this is only one packet, set extension to NONE */
      ptUploadReq->tHead.ulExt = HOST_TO_LE32(RCX_PACKET_SEQ_FIRST);
      if( ulTotalBytes <= ulBlockSize)
        ptUploadReq->tHead.ulExt = HOST_TO_LE32(RCX_PACKET_SEQ_NONE);    /* We can send all in one packet */

      /* Perform upload */
      while( (ulFileLength > 0) && (CIFX_NO_ERROR == lRet) )
      {
        /* Send and receive data */
        lRet = DEV_TransferPacket(pvChannel,
                                  &tSendPacket,
                                  &tRecvPacket,
                                  sizeof(tRecvPacket),
                                  CIFX_TO_SEND_PACKET,
                                  ptcifXTransportInst,
                                  pfnRecvPktCallback,
                                  pvUser);
        /* Check for errors */
        if( (CIFX_NO_ERROR != lRet)                                ||
            (RCX_S_OK      != (lRet = LE32_TO_HOST(tRecvPacket.tHeader.ulState))) )
        {
          /* This is a packet error from the hardware */
          /* - Inform application */
          /* - Leave upload and send abort */
          if(pfnCallback)
            pfnCallback(ulTransferredBytes, ulTotalBytes, pvUser, CIFX_CALLBACK_FINISHED, lRet);

          fSendAbort = 1;
          break;
        } else
        {
          uint32_t  ulCurrentDataLen = LE32_TO_HOST(ptUploadCnf->tHead.ulLen) - sizeof(ptUploadCnf->tData);
          uint8_t* pbRecvData       = (uint8_t*)(ptUploadCnf + 1);
          uint32_t  ulPacketCrc      = LE32_TO_HOST(ptUploadDataCnf->tData.ulChksum);

          /* Create own checksum and compare with it */
          ulCRC = CreateCRC32( ulCRC, pbRecvData, ulCurrentDataLen);

          if(ulCRC != ulPacketCrc)
          {
            /* Abort, as a CRC32 error occured */
            lRet = CIFX_FILE_CHECKSUM_ERROR;

            /* Send progress notification */
            if(pfnCallback)
              pfnCallback(ulTransferredBytes, ulTotalBytes, pvUser, CIFX_CALLBACK_FINISHED, lRet);

            fSendAbort = 1;
            break;
          } else
          {
            /* Next packet */
            ++ulCurrentId;
            ptUploadDataReq->tHead.ulId = HOST_TO_LE32(ulCurrentId);

            /* Calculate outstanding size */
            ulFileLength        -= ulCurrentDataLen;
            OS_Memcpy(pbData, pbRecvData, ulCurrentDataLen);
            pbData              += ulCurrentDataLen;
            ulTransferredBytes  += ulCurrentDataLen;
            *pulDataBufferLen   = ulTransferredBytes;

            /* Send progress notification */
            if(pfnCallback)
              pfnCallback(ulTransferredBytes, ulTotalBytes, pvUser,
                          (ulTransferredBytes == ulTotalBytes)? CIFX_CALLBACK_FINISHED : CIFX_CALLBACK_ACTIVE,
                          lRet);

            /* Calculate next packet length and packet extension */
            if(ulFileLength != 0)
            {
              if(ulFileLength <= ulBlockSize)
                ptUploadReq->tHead.ulExt = HOST_TO_LE32(RCX_PACKET_SEQ_LAST);
              else
                ptUploadReq->tHead.ulExt = HOST_TO_LE32(RCX_PACKET_SEQ_MIDDLE);
            }
          }
        }
      }
    }
  }

  /* If anything failed during upload, send an abort request */
  if( fSendAbort)
  {
    /* Usually one brace should be enough, but GNU wants to have a second brace
      to initialize the structure. On GCC 4.0.3 the whole structure is initialized
      as described in ISOC90 */
    RCX_FILE_UPLOAD_ABORT_REQ_T tFileAbortReq = {{0}};
    RCX_FILE_UPLOAD_ABORT_CNF_T tFileAbortCnf = {{0}};

    tFileAbortReq.tHead.ulSrc  = HOST_TO_LE32((uint32_t)PTR2LONG(pvChannel));
    tFileAbortReq.tHead.ulDest = HOST_TO_LE32(RCX_PACKET_DEST_SYSTEM);
    tFileAbortReq.tHead.ulCmd  = HOST_TO_LE32(RCX_FILE_UPLOAD_ABORT_REQ);

    DEV_TransferPacket(pvChannel,
                       (CIFX_PACKET*)&tFileAbortReq,
                       (CIFX_PACKET*)&tFileAbortCnf,
                       sizeof(tFileAbortCnf),
                       CIFX_TO_SEND_PACKET,
                       ptcifXTransportInst,
                       pfnRecvPktCallback,
                       pvUser);
  }

  return lRet;
}

/*****************************************************************************/
/*! Delete all existing files in a channel, from the file system.
*   \param pvChannel           Channel instance
*   \param ulChannel           Channel number
*   \param ptcifXTransportInst Pointer to netXTransport Device Instance
*   \param pfnRecvPacket       User callback for unsolicited receive packets
*   \param pvUser              User parameter passed on callback
*   \param szExceptFile
*   \return CIFX_NO_ERROR on success                                         */
/*****************************************************************************/
int DEV_RemoveChannelFiles(void*                      pvChannel,
                           uint32_t                   ulChannel,
                           CIFX_TRANSPORT_DEVICE_INST_T* ptcifXTransportInst,
                           PFN_RECV_PKT_CALLBACK      pfnRecvPacket,
                           void*                      pvUser,
                           char*                      szExceptFile)
{
  /* Try to find file with the extension *.nxm, *.nxf, *.mod and remove it */
  CIFX_DIRECTORYENTRY tDirectoryEntry = {0};
  int32_t             lRet            = CIFX_NO_ERROR;
  int                 fFindFirst      = 1;

  /* Search for all firmware files. If one is found. delete it an start with find first again, */
  /* because we can't store a directory list in here */
  do
  {
    if ( fFindFirst)
    {
      /* Clear directory structure to enumerate whole directory */
      OS_Memset(&tDirectoryEntry, 0x00, sizeof(tDirectoryEntry));

      lRet = ptcifXTransportInst->ptCifXFctTable->pfnxSysdeviceFindFirstFile( pvChannel, ulChannel, &tDirectoryEntry, pfnRecvPacket, pvUser);

      /* Search first file */
      if ( CIFX_NO_ERROR != lRet)
      {
        /* No more files, or error during find first */
        break;
      } else
      {
        /* Is this a valid file name */
        int iStrlen = OS_Strlen(tDirectoryEntry.szFilename);
        if( iStrlen >= CIFX_MIN_FILE_NAME_LENGTH)  /* At least x.abc */
        {
          if( !((NULL != szExceptFile)                                                          &&
                (4 == OS_Strlen(szExceptFile))                                                  &&
                (0 == OS_Strnicmp( szExceptFile, &tDirectoryEntry.szFilename[iStrlen - 4], 4)))   )
          {
            /* Delete file and continue with find first file again */
            if (CIFX_NO_ERROR !=  DEV_DeleteFile( pvChannel, ulChannel, tDirectoryEntry.szFilename, ptcifXTransportInst, pfnRecvPacket, pvUser))
            {
              /* Delete file failed. Stop searching for files */
              break;
            }
          }
        } else
        {
          /* Not a valid file, search next file */
          fFindFirst = 0;
        }
      }
    } else
    {
      lRet = ptcifXTransportInst->ptCifXFctTable->pfnxSysdeviceFindNextFile( pvChannel, ulChannel, &tDirectoryEntry, pfnRecvPacket, pvUser);

      /* Search for more files */
      if ( CIFX_NO_ERROR != lRet)
      {
        /* No more files, or error during find next */
        break;
      } else
      {
        /* Is this a valid file name */
        int iStrlen = OS_Strlen(tDirectoryEntry.szFilename);
        if( iStrlen >= CIFX_MIN_FILE_NAME_LENGTH)  /* At least x.abc */
        {
          /* If firmware file, delete it, else search until all files checked */
          if( !((NULL != szExceptFile)                                            &&
                (4 == OS_Strlen(szExceptFile))                                    &&
                (0 == OS_Strnicmp( szExceptFile, &tDirectoryEntry.szFilename[iStrlen - 4], 4)))   )
          {
            /* Delete the file and start with find first again */
            if (CIFX_NO_ERROR != DEV_DeleteFile( pvChannel, ulChannel, tDirectoryEntry.szFilename, ptcifXTransportInst, pfnRecvPacket, pvUser))
            {
              /* Delete file failed. Stop searching for files */
              break;
            } else
            {
              fFindFirst = 1;
            }
          }
        }
      }
    }
  } while ( CIFX_NO_ERROR == lRet);

  return 1;
}

/*****************************************************************************/
/*! Delete the given file
*   \param pvChannel           Channel instance
*   \param ulChannelNumber     Channel number
*   \param pszFileName         Input file name
*   \param ptcifXTransportInst pointer to netXTransport Device Instance
*   \param pfnRecvPacket       User callback for unsolicited receive packets
*   \param pvUser              User parameter passed on callback
*   \return 1 on success                                                     */
/*****************************************************************************/
int32_t DEV_DeleteFile(void*                       pvChannel,
                        uint32_t                   ulChannelNumber,
                        char*                      pszFileName,
                        CIFX_TRANSPORT_DEVICE_INST_T* ptcifXTransportInst,
                        PFN_RECV_PKT_CALLBACK      pfnRecvPacket,
                        void*                      pvUser)
{
/* Create delete packet */
  CIFX_PACKET            tSendPkt      = {{0}};
  RCX_FILE_DELETE_REQ_T* ptRequest     = (RCX_FILE_DELETE_REQ_T*)&tSendPkt;
  RCX_FILE_DELETE_CNF_T  tConf         = {{0}};
  uint16_t               usFileNameLen = (uint16_t)OS_Strlen(pszFileName);
  int32_t                lRet          = CIFX_NO_ERROR;

  /* Initialize the message */
  ptRequest->tHead.ulSrc  = HOST_TO_LE32((uint32_t)PTR2LONG(pvChannel));
  ptRequest->tHead.ulDest = HOST_TO_LE32(RCX_PACKET_DEST_SYSTEM);
  ptRequest->tHead.ulCmd  = HOST_TO_LE32(RCX_FILE_DELETE_REQ);
  ptRequest->tHead.ulExt  = HOST_TO_LE32(RCX_PACKET_SEQ_NONE);
  ptRequest->tHead.ulLen  = HOST_TO_LE32((uint32_t)(sizeof(ptRequest->tData) + usFileNameLen + 1));

  /* Insert file data */
  ptRequest->tData.ulChannelNo      = HOST_TO_LE32(ulChannelNumber);
  ptRequest->tData.usFileNameLength = HOST_TO_LE16( (usFileNameLen + 1) );

  /* Insert file name */
  OS_Strncpy( (char*)(ptRequest + 1), usFileNameLen, pszFileName, usFileNameLen);

  /* Send delete packet */
  lRet = DEV_TransferPacket( pvChannel,
                            (CIFX_PACKET*)ptRequest,
                            (CIFX_PACKET*)&tConf,
                            sizeof(tConf),
                            CIFX_TO_FIRMWARE_START,       /* Could take a little while */
                            ptcifXTransportInst,
                            pfnRecvPacket,
                            pvUser);

  if(CIFX_NO_ERROR == lRet)
    lRet = LE32_TO_HOST(tConf.tHead.ulSta);

  return lRet;
}

/*****************************************************************************/
/*! \}                                                                       */
/*****************************************************************************/
