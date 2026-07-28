                       /**************************************************************************************

   Copyright (c) Hilscher GmbH. All Rights Reserved.

 **************************************************************************************

   Filename:
    $Id: TL_Marshaller.h 167 2013-06-17 07:59:33Z sebastiand $
   Last Modification:
    $Author: sebastiand $
    $Date: 2013-06-17 09:59:33 +0200 (Mo, 17. Jun 2013) $
    $Revision: 167 $

   Targets:
     O/S Independent : yes

   Description:
     Declaration of TL cifX Marshaller Interface

   Changes:

     Version   Date        Author   Description
     ----------------------------------------------------------------------------------
     1         23.02.13    SD       initial version

**************************************************************************************/

/****************************************************************************/
/*! \file TL_Marshaller.h
    Declaration of translation layer Interface                                     */
/****************************************************************************/

#ifndef __TL_MARSHALLER__H
#define __TL_MARSHALLER__H

#include "cifXAPI.h"

#ifdef __cplusplus
extern "C"
{
#endif

/*****************************************************************************/
/*!  \addtogroup TL_CIFX_STRUCTURE cifX Translation-Layer Structure Definition
*    \{                                                                      */
/*****************************************************************************/
/*****************************************************************************/
/*! Marshaller driver information (host side)                                */
/*****************************************************************************/
typedef struct MARSH_DRIVER_INFO_Ttag
{
  uint32_t  ulDriverHandle;     /*!< Remote driver handle    */
  uint32_t  ulClassFactory;     /*!< Remote class factory    */
  int       fSequenceNrSupport; /*!< Sequence number support */
  uint16_t  usSequenceNr;       /*!< Current squence number  */

} MARSH_DRIVER_INFO_T;

typedef struct MARSHALLER_ENDPOINT_DATA_Ttag  MARSHALLER_ENDPOINT_DATA_T;    /*!< - */
typedef struct MARSHALLER_ENDPOINT_DATA_Ttag  *PMARSHALLER_ENDPOINT_DATA_T;  /*!< - */

/*****************************************************************************/
/*! Marshaller information (one per connection)                              */
/*****************************************************************************/
typedef struct MARSHALLER_TL_Ttag
{
  void*                        hTransport;               /*!< Handle of corresponding netXTransport instance */
  MARSH_DRIVER_INFO_T          tDriverInfo;              /*!< Driver information (host side)                 */
  uint32_t                     ulEndpointCounter;        /*!< Number of elements in atMarshallerEndpoints    */
  MARSHALLER_ENDPOINT_DATA_T** atMarshallerEndpoints;    /*!< List of all detected endpoints                 */

} MARSHALLER_TL_T;

/*****************************************************************************/
/*! Base information of a device                                             */
/*****************************************************************************/
typedef struct MARSH_CHANNEL_INFO_Ttag
{
  TAILQ_ENTRY(MARSH_CHANNEL_INFO_Ttag) tList;         /*!< List entry                   */
  int                           fOpen;               /*!< indicates if channel is opened                     */
  uint32_t                      ulChannelNr;         /*!< 0xFFFFFFFF == RCX_SYSTEM_CHANNEL === system device */
  uint32_t                      dwRemoteHandle;      /*!< Remote handle (on target system)                   */
  uint32_t                      hDriver;             /*!< Driver handle (on target system)                   */
  int32_t                       lHandleState;        /*!< State of remote handle                             */
  CIFX_TRANSPORT_DEVICE_INST_T* ptnetXTransportDev;  /*!< Pointer to the Marshaller endpoint device          */

} MARSH_CHANNEL_INFO_T, *PMARSH_CHANNEL_INFO_T;      /*!< Pointer to MARSH_CHANNEL_INFO_T */

/*****************************************************************************/
/*! Board information structure                                              */
/*****************************************************************************/
typedef struct MARSHALLER_BOARD_INFO_Ttag
{
  BOARD_INFORMATION    tBoardInfo;                    /*!< cifX board information structure */
  MARSH_CHANNEL_INFO_T tDevice;                       /*!< Marshaller channel information   */

} MARSHALLER_BOARD_INFO_T, *PMARSHALLER_BOARD_INFO_T; /*!< Pointer to MARSHALLER_BOARD_INFO_T */

/*****************************************************************************/
/*! Marshaller endpoint information (one per device)                         */
/*****************************************************************************/
struct MARSHALLER_ENDPOINT_DATA_Ttag
{
  MARSHALLER_BOARD_INFO_T tBoardInfo;                /*!< Marshaller board information            */
  MARSH_DRIVER_INFO_T     tDriverInfo;               /*!< Marshaller driver information           */
  void*                   hTransport;                /*!< Handle to corresponding transport layer */

};
/*****************************************************************************/
/*! \}                                                                       */
/*****************************************************************************/

/*! Translation-Layer specific functions */
int32_t          APIENTRY cifX_Marshaller_Init  ( void* pvParam);  /*!< TL-rcXPacket initialization function    */
void             APIENTRY cifX_Marshaller_DeInit( void* pvParam);  /*!< TL-rcXPacket de-initialization function */

#ifdef __cplusplus
}
#endif

#endif /* __TL_MARSHALLER__H */
