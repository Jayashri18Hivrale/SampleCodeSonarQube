/**************************************************************************************

   Copyright (c) Hilscher GmbH. All Rights Reserved.

 **************************************************************************************

   Filename:
    $Id: TranslationLayer.h 165 2013-06-14 13:44:13Z sebastiand $
   Last Modification:
    $Author: sebastiand $
    $Date: 2013-06-14 15:44:13 +0200 (Fr, 14. Jun 2013) $
    $Revision: 165 $

   Targets:
     O/S Independent : yes

   Description:
     Declaration of translation layer Interface

   Changes:

     Version   Date        Author   Description
     ----------------------------------------------------------------------------------
     1         23.02.13    SD       initial version

**************************************************************************************/

/****************************************************************************/
/*! \file TranslationLayer.h
    Declaration of translation layer Interface                              */
/****************************************************************************/

#ifndef __TRANSLATION_LAYER__H
#define __TRANSLATION_LAYER__H

#include <stdint.h>
#include <sys/queue.h>
#include <OS_Includes.h>
#include "HilTransportLayer.h"
#include "cifXUser.h"

/*****************************************************************************/
/*! Structure definition of TRANSPORT_SERVER_INFO_T                          */
/*****************************************************************************/
typedef struct TRANSPORT_SERVER_INFO_Ttag
{
  PHIL_TRANSPORT_ADMIN_QUERYSERVER_DATA_T ptQueryServerData;    /*!< Information of the server request      */
  uint32_t                                ulBufferSize;        /*!< Max. buffer size of endpoint           */
  uint32_t                                ulParallelServices;  /*!< Number of supported parallel services  */
  uint32_t                                ulFeatures;          /*!< Feature information flags              */
  int                                     fIgnoreSeqNr;        /*!< Sequence number support                */

} TRANSPORT_SERVER_INFO_T, *PTRANSPORT_SERVER_INFO_T;         /*!< Pointer to TRANSPORT_SERVER_INFO_T     */

typedef struct FUNCTION_POINTER_TABLE_Ttag FUNCTION_POINTER_TABLE_T;

/*****************************************************************************/
/*!  \addtogroup TL_FUNCTION_POINTER Translation-Layer Functions Pointer Definition
*    \{                                                                      */
/*****************************************************************************/
typedef int32_t (APIENTRY *PFN_TL_PROBE)       ( NXTHANDLE hTransport, NXTHANDLE* phTLHandle, uint32_t* ulDeviceNumber, TRANSPORT_SERVER_INFO_T* ptServerInfo);           /*!< Translation-Layer Probe function              */
typedef void    (APIENTRY *PFN_TL_RELEASE)     ( NXTHANDLE hTLHandle);                                                                                                    /*!< Translation-Layer Release function            */
typedef int32_t (APIENTRY *PFN_TL_SCAN)        ( NXTHANDLE hTLHandle, char* szDeviceName, uint32_t* ulDeviceIdentifier, void** ppvTLDeviceData, uint32_t ulDeviceNumber); /*!< Translation-Layer Scan function               */
typedef void    (APIENTRY *PFN_TL_RESTORECON)  ( NXTHANDLE hTLHandle);                                                                                                    /*!< Translation-Layer connection restore function */
typedef void    (APIENTRY *PFN_TL_RECEIVE)     ( NXTHANDLE hTLHandle, PHIL_TRANSPORT_PACKET_T ptPacket);                                                                  /*!< Translation-Layer Receive function            */
typedef int32_t (APIENTRY *PFN_TL_MAPPFNTABLE) ( FUNCTION_POINTER_TABLE_T* pfnFunctionTable);                                                                             /*!< Translation-Layer Functionpointer table       */
/*****************************************************************************/
/*! \}                                                                       */
/*****************************************************************************/

/* Init information for the cifX Transport */
/*****************************************************************************/
/*! TL Function Pointer structure definition                                 */
/*****************************************************************************/
struct FUNCTION_POINTER_TABLE_Ttag
{
  void*    pvTranslationLayerFunctionTable;  /*!< Pointer to Translation-Layer specific API */
  uint32_t ulVersionMajor;                   /*!< Version major                             */
  uint32_t ulVersionMinor;                   /*!< Version minor                             */
  uint32_t ulVersionBuild;                   /*!< Version build                             */
  uint32_t ulVersionRevision;                /*!< Version revision                          */

};

/*****************************************************************************/
/*! Generic Translation-Layer interface                                      */
/*****************************************************************************/
typedef struct NETX_TL_INTERFACE_Ttag
{
  TAILQ_ENTRY(NETX_TL_INTERFACE_Ttag) tList;           /*!< List entry                              */
  uint16_t                            usDataType;      /*!< Supported data type                     */

  PFN_TL_PROBE                 pfnProbe;               /*!< see PFN_TL_PROBE                        */
  PFN_TL_RELEASE               pfnRelease;             /*!< see PFN_TL_RELEASE                      */
  PFN_TL_SCAN                  pfnScan;                /*!< see PFN_TL_SCAN                         */
  PFN_TL_RESTORECON            pfnRestoreCon;          /*!< see PFN_TL_RESTORECON                   */
  PFN_TL_RECEIVE               pfnReceive;             /*!< see PFN_TL_RECEIVE                      */
  PFN_TL_MAPPFNTABLE           pfnMapFunctionTable;    /*!< see PFN_TL_MAPPFNTABLE                  */

  NXTHANDLE                    hNetXTransportDevGroup; /*!< Handle to netXTransport instance        */
  NXTHANDLE                    hNetXTransport;         /*!< Handle to netXTransport instance        */

} NETX_TL_INTERFACE_T, *PNETX_TL_INTERFACE_T;          /*!< Pointer to NETX_TL_INTERFACE_T          */

#endif /* __TRANSLATION_LAYER__H */
