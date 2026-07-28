/**************************************************************************************

   Copyright (c) Hilscher GmbH. All Rights Reserved.

 **************************************************************************************

   Filename:
    $Id: netXTransport_Errors.h 165 2013-06-14 13:44:13Z sebastiand $
   Last Modification:
    $Author: sebastiand $
    $Date: 2013-06-14 15:44:13 +0200 (Fr, 14. Jun 2013) $
    $Revision: 165 $

   Targets:
     O/S Independent : yes

   Description:
     netX Transport specific error code definitions

   Changes:

     Version   Date        Author   Description
     ----------------------------------------------------------------------------------
     1         23.02.13    MT       initial version

**************************************************************************************/

/****************************************************************************/
/*! \file netXTransport_Errors.h
    netX Transport specific error code definitions                          */
/****************************************************************************/

#ifndef __NETXTRANSPORT_ERRORS__H
#define __NETXTRANSPORT_ERRORS__H

#include <netXConnectorErrors.h>

#define NXT_NO_ERROR                          (0x00000000L)

/*******************************************************************************
* Generic Errors
*******************************************************************************/
#define NXT_INVALID_POINTER                   (0x80100001L)
#define NXT_INVALID_HANDLE                    (0x80100002L)
#define NXT_INVALID_PARAMETER                 (0x80100003L)
#define NXT_INVALID_COMMAND                   (0x80100004L)
#define NXT_INVALID_BUFFERSIZE                (0x80100005L)
#define NXT_INVALID_ACCESS_SIZE               (0x80100006L)
#define NXT_FUNCTION_FAILED                   (0x80100007L)
#define NXT_FILE_OPEN_FAILED                  (0x80100008L)
#define NXT_FILE_SIZE_ZERO                    (0x80100009L)
#define NXT_FILE_READ_ERROR                   (0x8010000AL)
#define NXT_FILE_WRITE_ERROR                  (0x8010000BL)
#define NXT_FUNCTION_NOT_AVAILABLE            (0x8010000CL)
#define NXT_BUFFER_TOO_SHORT                  (0x8010000DL)
#define NXT_RESOURCE_NOT_FOUND                (0x8010000EL)
#define NXT_NO_MEMORY                         (0x8010000FL)


/*******************************************************************************
* netXTransport API Errors
*******************************************************************************/
#define NXT_TOOLKIT_INIT_ERROR                (0x80200001L)
#define NXT_TL_INIT_ERROR                     (0x80200002L)
#define NXT_CONN_INIT_ERROR                   (0x80200003L)
#define NXT_HIL_INIT_ERROR                    (0x80200004L)
#define NXT_DUPLICATE_CONN_IDENTIFIER         (0x80200005L)
#define NXT_DUPLICATE_CONN_UUID               (0x80200006L)
#define NXT_DUPLICATE_TL_DATATYPE             (0x80200007L)
#define NXT_OUT_OF_MEMORY                     (0x80200008L)
#define NXT_SEND_TIMEOUT                      (0x80200009L)
#define NXT_RECV_TIMEOUT                      (0x8020000AL)
#define NXT_TRANSACTION_CANCELLED             (0x8020000BL)
#define NXT_UNSUPPORTED_DEVICE                (0x8020000CL)

#endif /* __NETXTRANSPORT_ERRORS__H */
