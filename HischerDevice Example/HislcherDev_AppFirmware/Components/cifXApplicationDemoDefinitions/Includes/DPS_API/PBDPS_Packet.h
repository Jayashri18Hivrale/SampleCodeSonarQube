/**************************************************************************************
Copyright (c) Hilscher Gesellschaft fuer Systemautomation mbH. All Rights Reserved.
***************************************************************************************
$Id: PBDPS_Packet.h 104758 2022-02-10 13:49:38Z TGrigorov $

Description:
  This file contains definitions of the packet interface to Profibus FSPMS task

Changes:
  Date          Description
  -----------------------------------------------------------------------------------
  2015-01-13    created
**************************************************************************************/
/*!\file PBDPS_Packet.h */
#ifndef __PBDPS_PACKET_H
#define __PBDPS_PACKET_H

#include "Hil_Packet.h"
#include "PBDPS_Defines.h"

/****************************************************************************************
* Profibus Fspms, command codes  */
/*! \defgroup dpsCorePacketIf DPS Core Packet Interface
 * PROFIBUS DP Slave Core  Packet Interface
 * =====================================
 * Stack specific commands for packet based interaction.
 * @{*/

/*!\name  PROFIBUS DP Slave Core Command codes
 * Commands processed by the DPS Core component.
 * PROFIBUS DP Slave Core commands reserved area from 0x400 - 0x4FF.
 *@{*/
/*! Start of command reserved area. */
#define  PROFIBUS_FSPMS_PACKET_COMMAND_START                0x00000400


/*! Send an Abort Signal Request.
 * In order to request an abort signal. \sa pckAbort*/
#define  PROFIBUS_FSPMS_CMD_ABORT_REQ                       0x00000408
/*! Send an Abort Signal Confirmation.\sa pckAbort*/
#define  PROFIBUS_FSPMS_CMD_ABORT_CNF                       0x00000409


/*! Application ready for Duty Request. \sa pckAppReady*/
#define  PROFIBUS_FSPMS_CMD_APPLICATION_READY_REQ           0x00000412
/*! Application ready for Duty Confirmation.\sa pckAppReady*/
#define  PROFIBUS_FSPMS_CMD_APPLICATION_READY_CNF           0x00000413


/*! Check Configuration Indication. \sa pckChkCfg*/
#define  PROFIBUS_FSPMS_CMD_CHECK_CFG_IND                   0x00000414
/*! Response to Indication Command of a Check Configuration. \sa pckChkCfg*/
#define  PROFIBUS_FSPMS_CMD_CHECK_CFG_RES                   0x00000415


/*! Check Parameter Data Indication. \sa pckChkPrm*/
#define  PROFIBUS_FSPMS_CMD_CHECK_USER_PRM_IND              0x00000416
/*! Response to Indication Check Parameter Data.*/
#define  PROFIBUS_FSPMS_CMD_CHECK_USER_PRM_RES              0x00000417


/*! Global Control Indication. \sa pckGlbCtrl.*/
#define  PROFIBUS_FSPMS_CMD_GLOBAL_CONTROL_IND              0x00000418
/*! Response to Indication Global Control.*/
#define  PROFIBUS_FSPMS_CMD_GLOBAL_CONTROL_RES              0x00000419


/*! Set Slave Address Indication.
 * Indication Command that indicates the Request for changing the Slave Address. \sa pckSetAddr*/
#define  PROFIBUS_FSPMS_CMD_SET_SLAVE_ADD_IND               0x0000041A
/*! Response to Indication Set Slave Address. */
#define  PROFIBUS_FSPMS_CMD_SET_SLAVE_ADD_RES               0x0000041B


/*! Set I&M 0 Parameter Settings Request.
 * Change Identification & Maintenance 0 Parameter Settings Request. \sa pckSetIM0 */
#define  PROFIBUS_FSPMS_CMD_SET_IM0_REQ                     0x00000420
/*! Confirmation to Set I&M0 Parameter Settings Request.*/
#define  PROFIBUS_FSPMS_CMD_SET_IM0_CNF                     0x00000421


/*! Identification & Maintenance Read Indication.
 * The indication is sent by the PBDPS Core if a PROFIBUS DP Master has send a I&M call get request
 * and the Application has registered an I&M unequal I&M0 Slot 0 with the
 * PROFIBUS_FSPMS_CMD_SET_IM0_REQ request. \sa pckIMRead*/
#define  PROFIBUS_FSPMS_CMD_IM_READ_IND                     0x00000422
/*! Response to Indication I&M Read. */
#define  PROFIBUS_FSPMS_CMD_IM_READ_RES                     0x00000423


/*! Identification & Maintenance Write Indication.
 * An indication is sent by the DPS stack if a PROFIBUS DP Master has send a I&M call set request
 * and the Application has registered an I&M unequal I&M0 Slot 0 with the
 * PROFIBUS_FSPMS_CMD_SET_IM0_REQ request.*/
#define  PROFIBUS_FSPMS_CMD_IM_WRITE_IND                    0x00000424
/*! Response to Indication I&M Write. */
#define  PROFIBUS_FSPMS_CMD_IM_WRITE_RES                    0x00000425


/*! Register IO-Link Call Request.  \sa pckIoCallReg*/
#define  PROFIBUS_FSPMS_CMD_IOL_CALL_REGISTER_REQ           0x00000426
/*! Register IO-Link Call Confirmation.*/
#define  PROFIBUS_FSPMS_CMD_IOL_CALL_REGISTER_CNF           0x00000427


/*! IO-Link Call Indication. \sa pckIoCall*/
#define  PROFIBUS_FSPMS_CMD_IOL_CALL_IND                    0x00000428
/*! IO-Link Call Response. \sa pckIoCall*/
#define  PROFIBUS_FSPMS_CMD_IOL_CALL_RES                    0x00000429

/*! Use this packet only when working with linkable object modules.*/
#define  PROFIBUS_FSPMS_CMD_SET_CFG_REQ                     0x0000042E
/*! Use this packet only when working with linkable object modules.*/
#define  PROFIBUS_FSPMS_CMD_SET_CFG_CNF                     0x0000042F


/*! Indication for new Extended Parameter Data. \sa pckExtPrmInd*/
#define  PROFIBUS_FSPMS_CMD_CHECK_EXT_USER_PRM_IND          0x00000438
/*! Response new Extended Parameter Data. \sa pckExtPrmInd*/
#define  PROFIBUS_FSPMS_CMD_CHECK_EXT_USER_PRM_RES          0x00000439


/*! Use this packet only when working with linkable object modules.*/
#define  PROFIBUS_FSPMS_CMD_CONFIGURE_REQ                   0x0000043A
/*! Use this packet only when working with linkable object modules.*/
#define  PROFIBUS_FSPMS_CMD_CONFIGURE_CNF                   0x0000043B


/*! Use this packet only when working with linkable object modules.*/
#define  PROFIBUS_FSPMS_CMD_STATE_CHANGED_IND               0x00000448
/*! Use this packet only when working with linkable object modules.*/
#define  PROFIBUS_FSPMS_CMD_STATE_CHANGED_RES               0x00000449


/*! Use this packet only when working with linkable object modules.*/
#define  PROFIBUS_FSPMS_CMD_RESET_REQ                       0x0000044E
/*! Use this packet only when working with linkable object modules.*/
#define  PROFIBUS_FSPMS_CMD_RESET_CNF                       0x0000044F


/*! Set Static Diagnostic Request. \sa pckSetStatDia*/
#define  PROFIBUS_FSPMS_CMD_SET_STAT_DIAG_REQ               0x00000452
/*! Set Static Diagnostic Confirmation. \sa pckSetStatDia*/
#define  PROFIBUS_FSPMS_CMD_SET_STAT_DIAG_CNF               0x00000453


/*! Set Slave Diagnostic Request. \sa pckSetDiag */
#define  PROFIBUS_FSPMS_CMD_SLAVE_DIAG_REQ                  0x00000454
/*! Set Slave Diagnostic Confirmation. \sa pckSetDiag */
#define  PROFIBUS_FSPMS_CMD_SLAVE_DIAG_CNF                  0x00000455


/*! Use this packet only when working with linkable object modules.*/
#define  PROFIBUS_FSPMS_CMD_SLAVE_DIAG_ISPENDING_REQ        0x00000456
/*! Use this packet only when working with linkable object modules.*/
#define  PROFIBUS_FSPMS_CMD_SLAVE_DIAG_ISPENDING_CNF        0x00000457


/*! Get Diagnostic Information of the stack Request. \sa pckGetDiag */
#define  PROFIBUS_FSPMS_CMD_GET_TASK_DIAG_REQ               0x00000458
/*! Get Diagnostic Information of the stack Confirmation. \sa pckGetDiag*/
#define  PROFIBUS_FSPMS_CMD_GET_TASK_DIAG_CNF               0x00000459


/*! Use this packet only when working with linkable object modules.*/
#define  PROFIBUS_FSPMS_CMD_GET_TIME_REQ                    0x0000045A
/*! Use this packet only when working with linkable object modules.*/
#define  PROFIBUS_FSPMS_CMD_GET_TIME_CNF                    0x0000045B


/*! Alarm Notification Request. \sa pckAlrmNot*/
#define  PROFIBUS_FSPMS_CMD_C1_ALARM_NOTIFICATION_REQ       0x00000480
/*! Alarm Notification Confirmation. \sa pckAlrmNot*/
#define  PROFIBUS_FSPMS_CMD_C1_ALARM_NOTIFICATION_CNF       0x00000481


/*! Alarm Request Indication. \sa pckAlarm*/
#define  PROFIBUS_FSPMS_CMD_C1_ALARM_ACK_IND                0x00000482
 /*! Alarm Request Response. \sa pckAlarm*/
#define  PROFIBUS_FSPMS_CMD_C1_ALARM_ACK_RES                0x00000483


/*! An Acyclic Read Request to a Specific Process Data Object Indication. \sa pckC1Read*/
#define  PROFIBUS_FSPMS_CMD_C1_READ_IND                     0x00000484
/*! An Acyclic Read Request to a Specific Process Data Object Response.\sa pckC1Read*/
#define  PROFIBUS_FSPMS_CMD_C1_READ_RES                     0x00000485


/*! An acyclic write Request to a specific Process Data Object Indication. \sa pckC1Write*/
#define  PROFIBUS_FSPMS_CMD_C1_WRITE_IND                    0x00000486
/*! An acyclic write Request to a specific Process Data Object Response. \sa pckC1Write*/
#define  PROFIBUS_FSPMS_CMD_C1_WRITE_RES                    0x00000487


/*! Request to establish an acyclic Connection to a DP-Master Class 2 Indication. \sa  pckC2Init*/
#define  PROFIBUS_FSPMS_CMD_C2_INITIATE_IND                 0x000004A2
/*! Request to establish an acyclic Connection to a DP-Master Class 2 Confirmation. \sa pckC2Init*/
#define  PROFIBUS_FSPMS_CMD_C2_INITIATE_RES                 0x000004A3


/*! Indication Command of a Process Data Read Request. \sa pckC2Read*/
#define  PROFIBUS_FSPMS_CMD_C2_READ_IND                     0x000004A4
/*! Response Command of a Process Data Read Request. \sa pckC2Read*/
#define  PROFIBUS_FSPMS_CMD_C2_READ_RES                     0x000004A5


/*! Acyclic write Request to a specific Process Data Object Indication. \sa pckC2Write*/
#define  PROFIBUS_FSPMS_CMD_C2_WRITE_IND                    0x000004A6
/*! Acyclic write Request to a specific Process Data Object Response. \sa pckC2Write*/
#define  PROFIBUS_FSPMS_CMD_C2_WRITE_RES                    0x000004A7


/*! Acyclic Data Transport Request to a Single Combined Process Data Object Indication \sa pckC2Trans*/
#define  PROFIBUS_FSPMS_CMD_C2_DATA_TRANSPORT_IND           0x000004A8
/*! Acyclic Data Transport Request to a Single Combined Process Data Object Response \sa pckC2Trans */
#define  PROFIBUS_FSPMS_CMD_C2_DATA_TRANSPORT_RES           0x000004A9


/*! Abort of Class 2 Connection Indication \sa pckC2Abort*/
#define  PROFIBUS_FSPMS_CMD_C2_ABORT_IND                    0x000004AA
/*! Abort of Class 2 Connection Response \sa pckC2Abort*/
#define  PROFIBUS_FSPMS_CMD_C2_ABORT_RES                    0x000004AB
/*!@}*/


/*****************************************************************************/
/*  Profibus Fspms, packets and corresponding structures                                                                         */
/*****************************************************************************/
/* pragma pack */
#ifdef PRAGMA_PACK_ENABLE
#pragma PRAGMA_PACK_1(PROFIBUS_FSPMS_PUBLIC)
#endif

/**/
typedef struct PROFIBUS_FSPMS_PACKET_CONFIGURE_REQ_Ttag
{
  HIL_PACKET_HEADER_T tHead; /*!< Standard Hilscher Header structure. */
  PBDPS_CONFIG_T tData;
} PROFIBUS_FSPMS_PACKET_CONFIGURE_REQ_T;

/* Packet Data Length in bytes. */
#define PROFIBUS_FSPMS_CONFIGURE_REQ_SIZE                   (sizeof(PBDPS_CONFIG_T))

/**/
typedef struct PROFIBUS_FSPMS_PACKET_CONFIGURE_CNF_Ttag
{
  HIL_PACKET_HEADER_T tHead; /*!< Standard Hilscher Header structure. */
} PROFIBUS_FSPMS_PACKET_CONFIGURE_CNF_T;
/* Packet Data Length in bytes.*/
#define PROFIBUS_FSPMS_CONFIGURE_CNF_SIZE                   (0)

/**************************************************************************************************/
/* Structures of all Request/Confirmation commands the task is able to send and receive           */
/*                                                                                                */
/* Request and Confirmation Packets PROFIBUS_PACKET_FSPMS_xx_REQ/CNF                              */
/* (xx = Command)                                                                                 */
/*                                                                                                */
/* Use the same order as the commands in TLR_Commands.h                                           */
/* PROFIBUS_FSPMS_xx_REQ/CNF (xx = Command)!                                                      */
/* incoming request packets                                                                       */
/* structures of all request/confirmation commands the task is able to receive/sent               */
/**************************************************************************************************/

typedef struct PROFIBUS_FSPMS_CMD_RESET_REQ_Ttag
{
  uint32_t  ulMode;
}PROFIBUS_FSPMS_CMD_RESET_REQ_T;

#define PROFIBUS_FSPMS_RESET_MODE_RESET_SETTINGS     0x00000000
#define PROFIBUS_FSPMS_RESET_MODE_KEEP_IM_SETTINGS   0x00000001

#define PROFIBUS_FSPMS_CMD_RESET_REQ_SIZE sizeof(PROFIBUS_FSPMS_CMD_RESET_REQ_T);

/* Request-Packet for the starting the Master-Slave cyclic state machine */
typedef struct PROFIBUS_FSPMS_PACKET_CMD_RESET_REQ_Ttag
{
  HIL_PACKET_HEADER_T tHead; /*!< Standard Hilscher Header structure. */
  PROFIBUS_FSPMS_CMD_RESET_REQ_T tData;
} PROFIBUS_FSPMS_PACKET_CMD_RESET_REQ_T;

/*  typedef __HIL_PACKED_PRE struct PROFIBUS_FSPMS_CMD_RESET_CNF_Ttag {
}__HIL_PACKED_POST PROFIBUS_FSPMS_CMD_RESET_CNF_T;

*/

#define PROFIBUS_FSPMS_CMD_RESET_CNF_SIZE                   (0)

/* Request-Packet for the starting the Master-Slave cyclic state machine */
typedef struct PROFIBUS_FSPMS_PACKET_CMD_RESET_CNF_Ttag
{
  HIL_PACKET_HEADER_T tHead; /*!< Standard Hilscher Header structure. */
/*  PROFIBUS_FSPMS_CMD_RESET_CNF_T tData; */
} PROFIBUS_FSPMS_PACKET_CMD_RESET_CNF_T;


/*!\defgroup pckAppReady Declaring the Application ready for Duty.
 * This service has to be used by the AP task to indicate its readiness for the incoming I/O
 * communication.
 * @{*/
/*! Request-Packet to declare the application as ready */
typedef struct PROFIBUS_FSPMS_PACKET_DP_SLAVE_APPLICATION_READY_REQ_Ttag
{
  HIL_PACKET_HEADER_T tHead; /*!< Standard Hilscher Header structure. */
} PROFIBUS_FSPMS_PACKET_DP_SLAVE_APPLICATION_READY_REQ_T;

/*! Confirmation-Packet */
typedef struct PROFIBUS_FSPMS_PACKET_DP_SLAVE_APPLICATION_READY_CNF_Ttag
{
  HIL_PACKET_HEADER_T    tHead; /*!< Standard Hilscher Header structure. */
} PROFIBUS_FSPMS_PACKET_DP_SLAVE_APPLICATION_READY_CNF_T;
/*!@}*/

/*!\defgroup pckSetDiag Set Slave Diagnostic Request/Confirmation.
 *@{*/
#define PROFIBUS_FSPMS_SLAVE_DIAG_CREATE                    (0x01)  /*!< Service: Create Diagnostic. */
#define PROFIBUS_FSPMS_SLAVE_DIAG_MODIFY                    (0x02)  /*!< Service: Modify Diagnostic. */
#define PROFIBUS_FSPMS_SLAVE_DIAG_DELETE                    (0x03)  /*!< Service: Delete Diagnostic. */

/*!Set Slave Diagnostic Request Data structure. */
typedef __HIL_PACKED_PRE struct PROFIBUS_FSPMS_SLAVE_DIAG_REQ_Ttag
{
  uint32_t ulIdentifier;    /*!< Diagnostic Identifier.                                           */
  uint16_t usService;       /*!< The action of doing with diagnostic (create, modify and delete). */
  uint8_t fExtDiagOverflow; /*!< flag for extended diagnostic overflow.                           */
  uint8_t fExtDiagFlag;     /*!< flag for extended diagnostic.                                    */
  uint8_t abExtDiagData[PBDPS_SLAVEDIAG_MAX]; /*!< Diagnostic data that'll be sent.               */
}__HIL_PACKED_POST  PROFIBUS_FSPMS_SLAVE_DIAG_REQ_T;


/* Set Slave Diagnostic Request Packet. */
typedef struct PROFIBUS_FSPMS_PACKET_SLAVE_DIAG_REQ_Ttag
{
  HIL_PACKET_HEADER_T             tHead;  /*!< Standard Hilscher Header structure. */
  PROFIBUS_FSPMS_SLAVE_DIAG_REQ_T tData;  /*!< Set Slave Diagnostic Request Data. */
} PROFIBUS_FSPMS_PACKET_SLAVE_DIAG_REQ_T;

/*! Set Slave Diagnostic Request packet data length in bytes without diagnostic bytes.  */
#define PROFIBUS_FSPMS_SLAVE_DIAG_REQ_SIZE              (sizeof(PROFIBUS_FSPMS_SLAVE_DIAG_REQ_T) - \
                                                         PBDPS_SLAVEDIAG_MAX)

/*! Set Static Diagnostic Confirmation Data structure. */
typedef __HIL_PACKED_PRE struct PROFIBUS_FSPMS_SLAVE_DIAG_CNF_Ttag
{
  uint32_t ulIdentifier;  /*!< Diagnostic Identifier.                                           */
  uint16_t usService;     /*!< The action of doing with diagnostic (create, modify and delete). */
}__HIL_PACKED_POST  PROFIBUS_FSPMS_SLAVE_DIAG_CNF_T;

/* Set Slave Diagnostic Confirmation Packet.
 * Confirmation-Packet that the diagnostic has been sent */
typedef struct PROFIBUS_FSPMS_PACKET_SLAVE_DIAG_CNF_Ttag
{
  HIL_PACKET_HEADER_T             tHead;  /*!< Standard Hilscher Header structure. */
  PROFIBUS_FSPMS_SLAVE_DIAG_CNF_T tData;  /*!< Set Static Diagnostic Confirmation Data.*/
} PROFIBUS_FSPMS_PACKET_SLAVE_DIAG_CNF_T;

/*! Set Slave Diagnostic Confirmation packet data length in bytes.  */
#define PROFIBUS_FSPMS_SLAVE_DIAG_CNF_SIZE                 (sizeof(PROFIBUS_FSPMS_SLAVE_DIAG_CNF_T))
/*!@}*/


/* Request-Packet to set a slave diagnostic */
typedef struct PROFIBUS_FSPMS_PACKET_SLAVE_DIAG_ISPENDING_REQ_Ttag
{
  HIL_PACKET_HEADER_T tHead; /*!< Standard Hilscher Header structure. */
} PROFIBUS_FSPMS_PACKET_SLAVE_DIAG_ISPENDING_REQ_T;

#define PROFIBUS_FSPMS_SLAVE_DIAG_ISPENDING_REQ_SIZE        (0)

typedef __HIL_PACKED_PRE struct PROFIBUS_FSPMS_SLAVE_DIAG_ISPENDING_CNF_Ttag
{
  uint32_t fIsPending;
}__HIL_PACKED_POST  PROFIBUS_FSPMS_SLAVE_DIAG_ISPENDING_CNF_T;

/* Confirmation-Packet that the diagnostic has been sent */
typedef struct PROFIBUS_FSPMS_PACKET_SLAVE_DIAG_ISPENDING_CNF_Ttag
{
  HIL_PACKET_HEADER_T tHead; /*!< Standard Hilscher Header structure. */
  PROFIBUS_FSPMS_SLAVE_DIAG_ISPENDING_CNF_T tData;
} PROFIBUS_FSPMS_PACKET_SLAVE_DIAG_ISPENDING_CNF_T;

#define PROFIBUS_FSPMS_SLAVE_DIAG_ISPENDING_CNF_SIZE  (sizeof(PROFIBUS_FSPMS_SLAVE_DIAG_ISPENDING_CNF_T))


/*!\defgroup pckSetStatDia Set Static Diagnostic Request/Confirmation.
 *@{  */
/*! Set Static Diagnostic Request Data structure.*/
typedef __HIL_PACKED_PRE struct PROFIBUS_FSPMS_SET_STAT_DIAG_REQ_Ttag
{
  uint8_t fStatDiag;  /*!< Static diagnostic flag. */
}__HIL_PACKED_POST PROFIBUS_FSPMS_SET_STAT_DIAG_REQ_T;

/*! Set Static Diagnostic Request Packet.
 * This packet is used for requesting or releasing a static diagnostic. If this packet is sent, the
 * diagnostic data are taken and will remain valid until the packet is sent again.*/
typedef struct PROFIBUS_FSPMS_PACKET_SET_STAT_DIAG_REQ_Ttag
{
  HIL_PACKET_HEADER_T                 tHead;  /*!< Standard Hilscher Header structure. */
  PROFIBUS_FSPMS_SET_STAT_DIAG_REQ_T  tData;  /*!< Set Static Diagnostic Request Data. */
} PROFIBUS_FSPMS_PACKET_SET_STAT_DIAG_REQ_T;

/*! Set Static Diagnostic Request packet data length in bytes. */
#define PROFIBUS_FSPMS_SET_STAT_DIAG_REQ_SIZE           (sizeof(PROFIBUS_FSPMS_SET_STAT_DIAG_REQ_T))

/*!Set Static Diagnostic Confirmation Packet. */
typedef struct PROFIBUS_FSPMS_PACKET_SET_STAT_DIAG_CNF_Ttag
{
  HIL_PACKET_HEADER_T tHead; /*!< Standard Hilscher Header structure. */
} PROFIBUS_FSPMS_PACKET_SET_STAT_DIAG_CNF_T;
/*!@}*/


typedef __HIL_PACKED_PRE struct PROFIBUS_FSPMS_SET_CFG_REQ_Ttag
{
  uint8_t abCfgData[PBDPS_MODULE_CONFIG_MAX]; /* Configuration data that'll be set */
}__HIL_PACKED_POST PROFIBUS_FSPMS_SET_CFG_REQ_T;

/* Request-Packet to set the configuration data */
typedef struct PROFIBUS_FSPMS_PACKET_SET_CFG_REQ_Ttag
{
  HIL_PACKET_HEADER_T tHead; /*!< Standard Hilscher Header structure. */
  PROFIBUS_FSPMS_SET_CFG_REQ_T tData;
} PROFIBUS_FSPMS_PACKET_SET_CFG_REQ_T;

/* Confirmation-Packet that the diagnostic has been sent */
typedef struct PROFIBUS_FSPMS_PACKET_SET_CFG_CNF_Ttag
{
  HIL_PACKET_HEADER_T tHead; /*!< Standard Hilscher Header structure. */
} PROFIBUS_FSPMS_PACKET_SET_CFG_CNF_T;


/*!\defgroup pckGetDiag Get Diagnostic Information of the stack Request/Confirmation
 * This service has to be used by application to request additional diagnostic information from the
 * stack.
 *@{*/
/*! Get Task Diagnostic Request Data structure */
typedef __HIL_PACKED_PRE struct PROFIBUS_FSPMS_GET_TASK_DIAG_REQ_Ttag
{
  /*! Selects the requested diagnostic information. Diagnostic information about the module
   * configuration, parameter data and etc is available. \sa PBDPS_TASK_DIAG */
  uint32_t ulTaskDiagID;
}__HIL_PACKED_POST PROFIBUS_FSPMS_GET_TASK_DIAG_REQ_T;

/*! Get Task Diagnostic Request Packet.
 *  Request-Packet to get internal diagnostic information */
typedef struct PROFIBUS_FSPMS_PACKET_GET_TASK_DIAG_REQ_Ttag
{
  HIL_PACKET_HEADER_T                tHead; /*!< Standard Hilscher Header structure.  */
  PROFIBUS_FSPMS_GET_TASK_DIAG_REQ_T tData; /*!< Get Task Diagnostic Request Data.    */
} PROFIBUS_FSPMS_PACKET_GET_TASK_DIAG_REQ_T;

/*! Get Task Diagnostic Request packet data length in bytes. */
#define PROFIBUS_FSPMS_GET_TASK_DIAG_REQ_SIZE           (sizeof(PROFIBUS_FSPMS_GET_TASK_DIAG_REQ_T))

/*! Get Task Diagnostic Confirmation Data structure.
 * Confirmation-Packet to get internal diagnostic information */
typedef __HIL_PACKED_PRE struct PROFIBUS_FSPMS_GET_TASK_DIAG_CNF_Ttag
{
  uint32_t ulTaskDiagID;  /*!< ID of requested diagnostic buffer. */
  union
  {
    FSPMS_EXTENDED_DIAG_T   tExtDiag;       /*!< Extended Task Diagnostic.            */
    FSPMS_CFG_DATA_T        tLocalConfig;   /*!< Module Configuration of the slave.   */
    FSPMS_PRM_DATA_T        tPrmData;       /*!< Parameter data from the Master.      */
    FSPMS_CFG_DATA_MASTER_T tMasterConfig;  /*!< Module Configuration of the Master.  */

    /*! Unstructured diagnostic data. */
    uint8_t abData[HIL_MAX_PACKET_SIZE - sizeof(HIL_PACKET_HEADER_T) - sizeof(uint32_t)];
  }uDiag;
}__HIL_PACKED_POST PROFIBUS_FSPMS_GET_TASK_DIAG_CNF_T;

/*! Get Task Diagnostic Confirmation Packet.
 * Confirmation-Packet that the diagnostic has been sent */
typedef struct PROFIBUS_FSPMS_PACKET_GET_TASK_DIAG_CNF_Ttag
{
  HIL_PACKET_HEADER_T                 tHead;  /*!< Standard Hilscher Header structure.    */
  PROFIBUS_FSPMS_GET_TASK_DIAG_CNF_T  tData;  /*!< Get Task Diagnostic Confirmation Data. */
} PROFIBUS_FSPMS_PACKET_GET_TASK_DIAG_CNF_T;

/*! Get Task Diagnostic Confirmation packet data length in bytes without diagnostic. */
#define PROFIBUS_FSPMS_GET_TASK_DIAG_CNF_SIZE (sizeof(PROFIBUS_FSPMS_GET_TASK_DIAG_CNF_T) - \
                                               sizeof(((PROFIBUS_FSPMS_GET_TASK_DIAG_CNF_T*)0)->uDiag))
/*!@}*/


/* Request-Packet to set a slave diagnostic */
typedef struct PROFIBUS_FSPMS_PACKET_GET_TIME_REQ_Ttag
{
  HIL_PACKET_HEADER_T tHead; /*!< Standard Hilscher Header structure. */
} PROFIBUS_FSPMS_PACKET_GET_TIME_REQ_T;

#define PROFIBUS_FSPMS_GET_TIME_REQ_SIZE                    (0)

/* Confirmation-Packet that the diagnostic has been sent */
typedef struct PROFIBUS_FSPMS_PACKET_GET_TIME_CNF_Ttag
{
  HIL_PACKET_HEADER_T tHead; /*!< Standard Hilscher Header structure. */
  PBDPS_CV_TIME_T     tData;
} PROFIBUS_FSPMS_PACKET_GET_TIME_CNF_T;

#define PROFIBUS_FSPMS_GET_TIME_CNF_SIZE                    (sizeof(PBDPS_CV_TIME_T))


/*!\defgroup pckAbort Send an Abort Signal.
 * In order to request an abort signal. To abort DP V1 Class 2 connection have to specify which
 * connection to be aborted (Reference number of Application Reference End Point Types) \sa pckC2Init.
 * @{*/
/*! FSPMS Abort request data.*/
typedef struct PROFIBUS_FSPMS_ABORT_REQ_DATA_Ttag
{
  uint32_t ulReference;  /*!< Reference number of DP V1 Class 2 connection. */
}PROFIBUS_FSPMS_ABORT_REQ_DATA_T;


/*! Request-Packet to send an Abort.
 * To abort DP V1 Class 2 connection have to assign reference number of DP V1 Class 2 connection */
typedef struct PROFIBUS_FSPMS_PACKET_ABORT_REQ_Ttag
{
  HIL_PACKET_HEADER_T             tHead;  /*!< Standard Hilscher Header structure. */
  PROFIBUS_FSPMS_ABORT_REQ_DATA_T tData;  /*!< FSPMS request data structure.       */
} PROFIBUS_FSPMS_PACKET_ABORT_REQ_T;

/*! PROFIBUS FSPMS Abort Request Packet data length in bytes. */
#define PROFIBUS_FSPMS_ABORT_REQ_SIZE   (sizeof(PROFIBUS_FSPMS_ABORT_REQ_DATA_T))

/*! FSPMS Abort confirmation data.*/
typedef struct PROFIBUS_FSPMS_ABORT_CNF_Ttag
{
  uint32_t ulReference;  /*!< Connection Reference number (reflection from the request). */
}PROFIBUS_FSPMS_ABORT_CNF_T;

/*! Confirmation-Packet that Abort has been performed. */
typedef struct PROFIBUS_FSPMS_PACKET_ABORT_CNF_Ttag
{
  HIL_PACKET_HEADER_T         tHead;  /*!< Standard Hilscher Header structure. */
  PROFIBUS_FSPMS_ABORT_CNF_T  tData;  /*!< Packet Data structure. */
} PROFIBUS_FSPMS_PACKET_ABORT_CNF_T;


/*! PROFIBUS FSPMS Abort Request Packet data length in bytes. */
#define PROFIBUS_FSPMS_ABORT_CNF_SIZE   (sizeof(PROFIBUS_FSPMS_ABORT_CNF_T))
/*!@}*/


/*!\defgroup pckAlrmNot Alarm Notification Request/Confirmation
 * The Slave indicates an alarm to the PROFIBUS Master.
 *@{*/
/*! Alarm Notification Request Data structure. */
typedef __HIL_PACKED_PRE struct PROFIBUS_FSPMS_C1_ALARM_NOTIFICATION_REQ_Ttag
{
  /*! Slot number of the object causing the alarm indicating the source of the alarm. The allowed
   * range here goes from 0 to 254. The value 255 is declared as reserved in the DPV1 specification.*/
  uint8_t bSlotNumber;

  /*! Identifies the alarm type. Value range from 0 to 127. Possible Alarm Types: 0 - Reserved;
   * 1 - Diagnostic Alarm; 2 - Process Alarm; 3 - Pull Alarm; 4 - Plug Alarm; 5 - Status Alarm; 6 -
   * Update Alarm;  7:31 - Reserved; 32:126 - Manufacturer Specific. 127 - Reserved; */
  uint8_t bAlarmType;

  /*! Sequence number for the distinction of alarms that are active at the same time. Value range from
   * 0 to 31. */
  uint8_t bSeqNr;

  /*! TThe additional acknowledgement flag contains the following information: This alarm requires a
   * separate user acknowledge additionally to the state machine MSAL1M_ALARM_RES. This can be done
   * for instance by means of a write service. With this packet the slave confirms that it has
   * previously been notified about an alarm by a PROFIBUS_FSPMS_CMD_C1_ALARM_NOTIFICATION_REQ packet.
   *  True or False(0, 1)*/
  uint8_t fAddAck;

  /*! The alarm specifier parameter gives additional alarm information, e.g. an alarm appears,
   * disappears or no further differentiation is not possible or if the alarm requires an additional
   * user acknowledge.*/
  uint8_t bAlarmSpecifier;

  /*! This area contains alarm specific data.*/
  uint8_t abAlarmData[PBDPS_ALARM_USERDATA_MAX];
}__HIL_PACKED_POST PROFIBUS_FSPMS_C1_ALARM_NOTIFICATION_REQ_T;

/*! Alarm Notification Request Packet.
 * Request-Packet of an Alarm Notification */
typedef struct PROFIBUS_FSPMS_PACKET_C1_ALARM_NOTIFICATION_REQ_Ttag
{
  HIL_PACKET_HEADER_T                         tHead; /*!< Standard Hilscher Header structure. */
  PROFIBUS_FSPMS_C1_ALARM_NOTIFICATION_REQ_T  tData; /*!< Alarm Notification Request Data.*/
} PROFIBUS_FSPMS_PACKET_C1_ALARM_NOTIFICATION_REQ_T;

/*! Alarm Notification Request Packet Data Length in bytes without number of bytes in block abAlarmData. */
#define PROFIBUS_FSPMS_C1_ALARM_NOTIFICATION_REQ_SIZE (sizeof(PROFIBUS_FSPMS_C1_ALARM_NOTIFICATION_REQ_T) - \
                                                       PBDPS_ALARM_USERDATA_MAX)

/*! Alarm Notification Confirmation Packet Data structure. */
typedef __HIL_PACKED_PRE struct PROFIBUS_FSPMS_C1_ALARM_NOTIFICATION_CNF_Ttag
{
  /*! Slot number of the object causing the alarm indicating the source of the alarm. The allowed
   * range here goes from 0 to 254. The value 255 is declared as reserved in the DPV1 specification.*/
  uint8_t bSlotNumber;

  /*! Identifies the alarm type. Value range from 0 to 127. Possible Alarm Types: 0 - Reserved;
   * 1 - Diagnostic Alarm; 2 - Process Alarm; 3 - Pull Alarm; 4 - Plug Alarm; 5 - Status Alarm; 6 -
   * Update Alarm;  7:31 - Reserved; 32:126 - Manufacturer Specific. 127 - Reserved; */
  uint8_t bAlarmType;

  /*! Sequence number for the distinction of alarms that are active at the same time. Value range from
   * 0 to 31. */
  uint8_t bSeqNr;
}__HIL_PACKED_POST PROFIBUS_FSPMS_C1_ALARM_NOTIFICATION_CNF_T;

/*! Alarm Notification Confirmation Packet.
 *  Confirmation-Packet of an Alarm Notification */
typedef struct PROFIBUS_FSPMS_PACKET_C1_ALARM_NOTIFICATION_CNF_Ttag
{
  HIL_PACKET_HEADER_T                         tHead; /*!< Standard Hilscher Header structure. */
  PROFIBUS_FSPMS_C1_ALARM_NOTIFICATION_CNF_T  tData; /*!< Alarm Notification Confirmation Packet Data. */
} PROFIBUS_FSPMS_PACKET_C1_ALARM_NOTIFICATION_CNF_T;
/*!@}*/


/*!\defgroup pckAlarm Alarm Request Indication/Confirmation.
 * All parameters will be checked by the device. In case of an error the alarm will be rejected and an
 * error message will be returned to the host application. Else the alarm will be sent and the device
 * waits for the "Alarm Acknowledge" service from the master. When it is received the device informs
 * the host application of the successful alarm processing by returning an answer message.
 *@{*/
/*! Alarm Request Indication Data structure. */
typedef __HIL_PACKED_PRE struct PROFIBUS_FSPMS_C1_ALARM_ACK_IND_Ttag
{
  /*! Slot number of the object causing the alarm indicating the source of the alarm. The allowed
   * range here goes from 0 to 254. The value 255 is declared as reserved in the DPV1 specification.*/
  uint8_t bSlotNumber;

  /*! Identifies the alarm type. Value range from 0 to 127. Possible Alarm Types: 0 - Reserved;
   * 1 - Diagnostic Alarm; 2 - Process Alarm; 3 - Pull Alarm; 4 - Plug Alarm; 5 - Status Alarm; 6 -
   * Update Alarm;  7:31 - Reserved; 32:126 - Manufacturer Specific. 127 - Reserved; */
  uint8_t bAlarmType;

  /*! Sequence number for the distinction of alarms that are active at the same time. Value range from
   * 0 to 31. */
  uint8_t bSeqNr;
}__HIL_PACKED_POST PROFIBUS_FSPMS_C1_ALARM_ACK_IND_T;

/*! Alarm Request Indication Packet. */
typedef struct PROFIBUS_FSPMS_PACKET_C1_ALARM_ACK_IND_Ttag
{
  HIL_PACKET_HEADER_T               tHead; /*!< Standard Hilscher Header structure. */
  PROFIBUS_FSPMS_C1_ALARM_ACK_IND_T tData; /*!< Alarm Request Indication Data.      */
} PROFIBUS_FSPMS_PACKET_C1_ALARM_ACK_IND_T;

/*! Alarm Request Positive Confirmation Data structure.
 * A positive write response does not have any further parameter data.*/
typedef __HIL_PACKED_PRE struct PROFIBUS_FSPMS_C1_ALARM_ACK_RES_POS_Ttag
{
  /*! Slot number of the object causing the alarm indicating the source of the alarm. The allowed
   * range here goes from 0 to 254. The value 255 is declared as reserved in the DPV1 specification.*/
  uint8_t bSlotNumber;

  /*! Identifies the alarm type. Value range from 0 to 127. Possible Alarm Types: 0 - Reserved;
   * 1 - Diagnostic Alarm; 2 - Process Alarm; 3 - Pull Alarm; 4 - Plug Alarm; 5 - Status Alarm; 6 -
   * Update Alarm;  7:31 - Reserved; 32:126 - Manufacturer Specific. 127 - Reserved; */
  uint8_t bAlarmType;

  /*! Sequence number for the distinction of alarms that are active at the same time. Value range from
   * 0 to 31. */
  uint8_t bSeqNr;
}__HIL_PACKED_POST PROFIBUS_FSPMS_C1_ALARM_ACK_RES_POS_T;

/*! Alarm Request Positive Confirmation Packet.
 *  Positive response packet of an acyclic Alarm Acknowledgment command */
typedef struct PROFIBUS_FSPMS_PACKET_C1_ALARM_ACK_RES_POS_Ttag
{
  HIL_PACKET_HEADER_T                   tHead; /*!< Standard Hilscher Header structure.       */
  PROFIBUS_FSPMS_C1_ALARM_ACK_RES_POS_T tData; /*!< Alarm Request Positive Confirmation Data. */
} PROFIBUS_FSPMS_PACKET_C1_ALARM_ACK_RES_POS_T;

/*! Alarm Request Negative Response Data structure.
 * \sa PBDPS_DPV1_ERROR_T */
typedef __HIL_PACKED_PRE struct PROFIBUS_FSPMS_C1_ALARM_ACK_RES_NEG_Ttag
{
  /*! Error decode value classifying the error 128 indicates DP V1 error handling is applied 254,
   * 255 indicate profile-specific error handling is applied. */
  uint8_t bErrorDecode;

  /*!Detailed error code.  */
  uint8_t bErrorCode1;

  /*! User specific error code. */
  uint8_t bErrorCode2;
}__HIL_PACKED_POST PROFIBUS_FSPMS_C1_ALARM_ACK_RES_NEG_T;

/*! Alarm Request Negative Response Packet.
 * A negative response is issued whenever the indicated request cannot be satisfied by the AP task.
 * 3 variables are specifying the type and source of the error in this event. The applicable error
 * codes of the 3 variables (bErrorDecode, bErrorCode1, bErrorCode2) that are specifying the type
 * and source of the error in this event. */
typedef struct PROFIBUS_FSPMS_PACKET_C1_ALARM_ACK_RES_NEG_Ttag
{
  HIL_PACKET_HEADER_T                   tHead; /*!< Standard Hilscher Header structure.   */
  PROFIBUS_FSPMS_C1_ALARM_ACK_RES_NEG_T tData; /*!< Alarm Request Negative Response Data. */
} PROFIBUS_FSPMS_PACKET_C1_ALARM_ACK_RES_NEG_T;
/*!@}*/


/*!\defgroup pckC1Read An Acyclic Read Request to a Specific Process Data Object Indication/Response
 * This service indicates the AP task that a specific process data object shall be read by a DP-Master
 * (Class 1). The AP task has to take care of the process data objects themselves. This means that it
 * is fully application specific where those process data objects are read from and what purpose they
 * have. To complete the process data read indication, the AP task has to respond and deliver the
 * requested data
 *@{ */
/*! An Acyclic Read Request to a Specific Process Data Object Indication Data structure. */
typedef __HIL_PACKED_PRE struct PROFIBUS_FSPMS_C1_READ_IND_Ttag
  {
  uint8_t bSlotNumber;  /*!< Slot number of the process data object to be read.               */
  uint8_t bIndex;       /*!< Index of the process data object to be read.                     */
  uint8_t bLength;      /*!< Number of bytes to be read in the specified process data object. */
}__HIL_PACKED_POST PROFIBUS_FSPMS_C1_READ_IND_T;

/*! An Acyclic Read Request to a Specific Process Data Object Indication Packet.
 * Packet command: PROFIBUS_FSPMS_CMD_C1_READ_IND.*/
typedef struct PROFIBUS_FSPMS_PACKET_C1_READ_IND_Ttag
{
  HIL_PACKET_HEADER_T           tHead;  /*!< Standard Hilscher Header structure. */
  PROFIBUS_FSPMS_C1_READ_IND_T  tData;  /*!< Indication Data.*/
} PROFIBUS_FSPMS_PACKET_C1_READ_IND_T;

/*! Indication Packet Data Length in bytes. */
#define PROFIBUS_FSPMS_C1_READ_IND_SIZE                     (sizeof(PROFIBUS_FSPMS_C1_READ_IND_T))


/*! Positive response data structure of an acyclic read command.
 * The variable bLength defines how many bytes are read and returned to the FSPMS task. The
 * indicated length and the returned length may differ. It is allowed for example that the indication
 * service requests more data bytes to be read than physically can be returned. In this case the
 * response service just returns the maximum number of bytes that are really returned.*/
typedef __HIL_PACKED_PRE struct PROFIBUS_FSPMS_C1_READ_RES_POS_Ttag
{
  uint8_t bSlotNumber;  /*!< Slot number of the Process Data Object that has been read.         */
  uint8_t bIndex;       /*!< Index of the Process Data object that has been read.               */
  uint8_t bLength;      /*!< Number of data bytes read from the specified Process Data Object.  */
  uint8_t abData[PBDPS_READ_MAX]; /*!< Process Data object data that has been read.             */
}__HIL_PACKED_POST PROFIBUS_FSPMS_C1_READ_RES_POS_T;

/*! An Acyclic Read Request to a Specific Process Data Object Positive Response Packet.
 * The AP task returns the current Process Data object's data A positive response is send
 * when ulSta = SUCCESS_HIL_OK */
typedef struct PROFIBUS_FSPMS_PACKET_C1_READ_RES_POS_Ttag
{
  HIL_PACKET_HEADER_T               tHead;  /*!< Standard Hilscher Header structure.              */
  PROFIBUS_FSPMS_C1_READ_RES_POS_T  tData;  /*!< Positive response data of an acyclic read command.*/
} PROFIBUS_FSPMS_PACKET_C1_READ_RES_POS_T;

/*! Positive Response Packet Data Length in bytes without number of bytes in read data block extended.*/
#define PROFIBUS_FSPMS_C1_READ_RES_POS_SIZE            (sizeof(PROFIBUS_FSPMS_C1_READ_RES_POS_T) - \
                                                        PBDPS_READ_MAX)


/*! Negative response data structure of an acyclic read command.*/
typedef __HIL_PACKED_PRE struct PROFIBUS_FSPMS_C1_READ_RES_NEG_Ttag
{
  /*! Error decode value classifying the error 128 indicates DP V1 error handling is applied 254,
   * 255 indicate profile-specific error handling is applied. */
  uint8_t bErrorDecode;

  uint8_t bErrorCode1;  /*!< Detailed error code.       */
  uint8_t bErrorCode2;  /*!< User specific error code.  */
}__HIL_PACKED_POST PROFIBUS_FSPMS_C1_READ_RES_NEG_T;

/*!  An Acyclic Read Request to a Specific Process Data Object Negative Response Packet
  * A negative response is issued whenever the indicated request cannot be satisfied by the AP task.
  * A negative response is send when ulSta != SUCCESS_HIL_OK (By default: ERR_HIL_FAIL) */
typedef struct PROFIBUS_FSPMS_PACKET_C1_READ_RES_NEG_Ttag
{
  HIL_PACKET_HEADER_T               tHead;  /*!< Standard Hilscher Header structure. */
  PROFIBUS_FSPMS_C1_READ_RES_NEG_T  tData;  /*!< Negative response data of an acyclic read command.*/
} PROFIBUS_FSPMS_PACKET_C1_READ_RES_NEG_T;

/*! Return packet to a previously sent read response packet */
typedef struct PROFIBUS_FSPMS_PACKET_C1_READ_RES_RET_Ttag
{
  HIL_PACKET_HEADER_T tHead; /*!< Standard Hilscher Header structure. */
} PROFIBUS_FSPMS_PACKET_C1_READ_RES_RET_T;
/*!@}*/

/*!\defgroup pckC1Write An acyclic write Request to a specific Process Data Object Indication/Response.
 * This service indicates the AP task that a specific Process Data Object shall be written by a
 * DPMaster Class 1. The AP task has to take care of the process Data objects themselves. This means
 * that it is fully application specific where those process Data objects are stored to and what
 * purpose they have. To complete the Process Data write indication, the AP task has to respond and
 * deliver the requested data by using the service referenced PROFIBUS_FSPMS_CMD_C1_WRITE_RES_POS -
 * Positive Response Command of a Process Data write or PROFIBUS_FSPMS_CMD_C1_WRITE_RES_NEG -
 * Negative Response Command of a Process Data write.
 *@{*/
/*! Indication Data structure of a Process Data Write Request.
 * The variable bSlotNumber has to be used in the AP task for addressing the desired process Data
 * object in the specified slot (typically a module).
 * The variable bIndex has to be used in the AP task for addressing the desired process Data object
 * itself.
 * The variable bLength indicates the number of bytes of the specified process Data object that has
 * to be written. Within the array abData[] the FSPMS task indicates the new process Data
 * to be written.*/
typedef __HIL_PACKED_PRE struct PROFIBUS_FSPMS_C1_WRITE_IND_Ttag
  {
  uint8_t bSlotNumber;  /*!< Slot number of the Process Data object to be written.                */
  uint8_t bIndex;       /*!< Index of the Process Data object to be written.                      */
  uint8_t bLength;      /*!< Number of bytes to be written into the specified Process Data object.*/
  uint8_t abData[PBDPS_WRITE_MAX]; /*!< Process Data object data that shall be written.           */
}__HIL_PACKED_POST PROFIBUS_FSPMS_C1_WRITE_IND_T;

/*! An acyclic write Request to a specific Process Data Object Indication Packet.
 * PROFIBUS_FSPMS_CMD_C1_WRITE_IND - Indication Command of a Process Data Write Request.*/
typedef struct PROFIBUS_FSPMS_PACKET_C1_WRITE_IND_Ttag
{
  HIL_PACKET_HEADER_T           tHead;  /*!< Standard Hilscher Header structure. */
  PROFIBUS_FSPMS_C1_WRITE_IND_T tData;  /*!< Indication Data of a Process Data Write Request.*/
} PROFIBUS_FSPMS_PACKET_C1_WRITE_IND_T;

/*! Indication of a Process Data Write Request packet data length in bytes without number of bytes in write data block.*/
#define PROFIBUS_FSPMS_C1_WRITE_IND_SIZE                 (sizeof(PROFIBUS_FSPMS_C1_WRITE_IND_T) - \
                                                          PBDPS_WRITE_MAX)

/*! Positive Response Data structure of a Process Data write. */
typedef __HIL_PACKED_PRE struct PROFIBUS_FSPMS_C1_WRITE_RES_POS_Ttag
{
  uint8_t bSlotNumber;  /*!< Slot number of the Process Data object to be written.                */
  uint8_t bIndex;       /*!< Index of the Process Data object to be written.                      */
  uint8_t bLength;      /*!< Number of bytes to be written into the specified Process Data object.*/
}__HIL_PACKED_POST PROFIBUS_FSPMS_C1_WRITE_RES_POS_T;

/*!
 * Positive response packet of an acyclic write command does not contain any additional parameter
 * data. ulSta shall be SUCCESS_HIL_OK (0)*/
typedef struct PROFIBUS_FSPMS_PACKET_C1_WRITE_RES_POS_Ttag
{
  HIL_PACKET_HEADER_T               tHead;  /*!< Standard Hilscher Header structure. */
  PROFIBUS_FSPMS_C1_WRITE_RES_POS_T tData;  /*!< Positive Response Data of a Process Data write. */
} PROFIBUS_FSPMS_PACKET_C1_WRITE_RES_POS_T;


/*! Negative Response Data of a Process Data write.*/
typedef __HIL_PACKED_PRE struct PROFIBUS_FSPMS_C1_WRITE_RES_NEG_Ttag
{
  /*! Error decode value classifying the error 128 indicates DP V1 error handling is applied 254,
   * 255 indicate profile-specific error handling is applied. */
  uint8_t bErrorDecode;
  uint8_t bErrorCode1;  /*!< Detailed error code.       */
  uint8_t bErrorCode2;  /*!< User specific error code.  */
}__HIL_PACKED_POST PROFIBUS_FSPMS_C1_WRITE_RES_NEG_T;

/*! Negative Response Packet of a Process Data write.
 *  Negative response packet of an acyclic read command is issued whenever the indicated request
 *  cannot be satisfied by the AP task. 3 variables are specifying the type and source of the error
 *  in this event.*/
typedef struct PROFIBUS_FSPMS_PACKET_C1_WRITE_RES_NEG_Ttag
{
  HIL_PACKET_HEADER_T               tHead;  /*!< Standard Hilscher Header structure. */
  PROFIBUS_FSPMS_C1_WRITE_RES_NEG_T tData;  /*!< Negative Response Data of a Process Data write.*/
} PROFIBUS_FSPMS_PACKET_C1_WRITE_RES_NEG_T;

/*! Return packet to a previously sent write response packet.
 * The PROFIBUS_FSPMS_CMD_C1_WRITE_RES service has to be used by the AP task in order to confirm a
 * previously FSPMS task issued Process Data write indication. The response is required and a must
 * do when having received the indication command PROFIBUS_FSPMS_CMD_C1_WRITE_IND - Indication
 * Command of a Process Data Write Request. */
typedef struct PROFIBUS_FSPMS_PACKET_C1_WRITE_RES_RET_Ttag
{
  HIL_PACKET_HEADER_T tHead; /*!< Standard Hilscher Header structure. */
} PROFIBUS_FSPMS_PACKET_C1_WRITE_RES_RET_T;
/*!@}*/

/* ******************************************************************************************* */
typedef __HIL_PACKED_PRE struct PROFIBUS_FSPMS_REGISTER_DIAG_STRUCT_REQ_Ttag
{
  uint8_t *  pbDiagArea;
  uint32_t  ulLength;
}__HIL_PACKED_POST PROFIBUS_FSPMS_REGISTER_DIAG_STRUCT_REQ_T;

#define PROFIBUS_FSPMS_REGISTER_DIAG_STRUCT_REQ_SIZE  (sizeof(PROFIBUS_FSPMS_REGISTER_DIAG_STRUCT_REQ_T))

typedef struct PROFIBUS_FSPMS_PACKET_REGISTER_DIAG_STRUCT_REQ_Ttag
{
  HIL_PACKET_HEADER_T tHead; /*!< Standard Hilscher Header structure. */
  PROFIBUS_FSPMS_REGISTER_DIAG_STRUCT_REQ_T tData;
} PROFIBUS_FSPMS_PACKET_REGISTER_DIAG_STRUCT_REQ_T;

#if 0
typedef __HIL_PACKED_PRE struct PROFIBUS_FSPMS_REGISTER_DIAG_STRUCT_CNF_Ttag
{
}__HIL_PACKED_POST PROFIBUS_FSPMS_REGISTER_DIAG_STRUCT_CNF_T;
#endif

/* sizeof(PROFIBUS_FSPMS_REGISTER_DIAG_STRUCT_CNF_T) */
#define PROFIBUS_FSPMS_REGISTER_DIAG_STRUCT_CNF_SIZE        (0)

typedef struct PROFIBUS_FSPMS_PACKET_REGISTER_DIAG_STRUCT_CNF_Ttag
{
  HIL_PACKET_HEADER_T tHead; /*!< Standard Hilscher Header structure. */
/*  PROFIBUS_FSPMS_REGISTER_DIAG_STRUCT_CNF_T tData; */
} PROFIBUS_FSPMS_PACKET_REGISTER_DIAG_STRUCT_CNF_T;


/**************************************************************************************************/
/*  Indication packets structures of all indication commands the task is able to sent             */
/**************************************************************************************************/
/*!\defgroup pckChkCfg Indicating the Request for Validation of the assumed I/O Configuration Data
 * \details This service indicates the APS task that a "Check Configuration" command has been received.
 * This service is sent by a DP-Master to a DP-Slave to force a validation of the "Is-Configuration"
 * within the slave and the assumed configuration data of the DP master. The AP task has to compare
 * the received configuration data against its own "Is-Configuration".
 * @{*/
/*! Data structure of Check Configuration indication.*/
typedef __HIL_PACKED_PRE struct PROFIBUS_FSPMS_CHECK_CFG_IND_Ttag
{
  uint8_t abCfgData[PBDPS_MODULE_CONFIG_MAX]; /*!< Assumed Configuration data to be validated. */
}__HIL_PACKED_POST PROFIBUS_FSPMS_CHECK_CFG_IND_T;

/*! Indication - Packet to indicate  a "Check Configuration" command has been received.*/
 typedef struct PROFIBUS_FSPMS_PACKET_CHECK_CFG_IND_Ttag
{
  HIL_PACKET_HEADER_T             tHead; /*!< Standard Hilscher Header structure. */
  PROFIBUS_FSPMS_CHECK_CFG_IND_T  tData; /*!< Indication Data structure.*/
} PROFIBUS_FSPMS_PACKET_CHECK_CFG_IND_T;

/*! Data structure of Check Configuration response. */
typedef __HIL_PACKED_PRE struct PROFIBUS_FSPMS_CHECK_CFG_RES_Ttag
{
  /*! If set to true(1) the AP task has accepted the Configuration Data.
   *  If set to false(0), the Configuration Data are not ok. */
  uint8_t fCfgOk;
  uint8_t bInputDataLen;    /*!< Real length of Application input data, maximum. */
  uint8_t bOutputDataLen;   /*!< Real length of Application output data, maximum. */
}__HIL_PACKED_POST PROFIBUS_FSPMS_CHECK_CFG_RES_T;

/*! Response to Indication Command of a Check Configuration. */
 typedef struct PROFIBUS_FSPMS_PACKET_CHECK_CFG_RES_Ttag
{
  HIL_PACKET_HEADER_T             tHead; /*!< Standard Hilscher Header structure. */
  PROFIBUS_FSPMS_CHECK_CFG_RES_T  tData; /*!< Response Data Structure.*/
} PROFIBUS_FSPMS_PACKET_CHECK_CFG_RES_T;
/*!@}*/


/*!\defgroup pckGlbCtrl Indicating a Global Control Command
 * This service indicates the AP task that a Global Control Command has been received. The broadcast
 * service is sent by a DP-Master via the network for the synchronization of the values of the input
 * data and the output data (Sync/Freeze) of different slaves.
 * \note Use this packet only when working with linkable object modules. It has not been designed for
 * usage in the context of loadable firmware.
 * @{*/
/*! Indication Global Control Data structure */
typedef __HIL_PACKED_PRE struct PROFIBUS_FSPMS_GLOBAL_CONTROL_IND_Ttag
{
  /*! Clear Commnad flag.<br>
   * If set to false(0), the DP-Master is operating in the state OPERATE.  If the flag is set to
   * true(1), the DP-Master is in the state CLEAR and the AP task should set its output data to a
   * safe state.*/
  uint8_t fClearCommand;

  /*! Type of Sync Operation.<br>
   * Range : 0 to 2. A reaction of the AP task based on the value of bSyncCommand is in most cases
   * basically not necessary. The MSCY1S state machine itself has automatically done all necessary
   * reactions. \sa glbSync*/
  uint8_t bSyncCommand;

  /*! Type of Freeze Operation.<br>
   * Range : 0 to 2. A reaction of the AP task based on the value of bFreezeCommand is in most cases
   * basically not  necessary. The MSCY1S state machine itself has automatically done all necessary
   * reactions. \sa glbFreeze*/
  uint8_t bFreezeCommand;

  /*! Group Select.<br>
   * The variable determines what group(s) of assigned slaves is addressed due to the "Global Control"
   * command in the network.
   * - 0 : All slaves are addressed.
   * - 1 to 255 : Specific slave groups are addressed.
   * There is no reaction of the AP task needed depending on the indicated
   * value. The MSCY1S state machine itself has automatically done all necessary reactions. */
  uint8_t bGroupSelect;
}__HIL_PACKED_POST PROFIBUS_FSPMS_GLOBAL_CONTROL_IND_T;

/*! Indication of a Global Control Command. */
typedef struct PROFIBUS_FSPMS_PACKET_GLOBAL_CONTROL_IND_Ttag
{
  HIL_PACKET_HEADER_T                 tHead; /*!< Standard Hilscher Header structure. */
  PROFIBUS_FSPMS_GLOBAL_CONTROL_IND_T tData; /*!< Indication Global Control Data structure. */
} PROFIBUS_FSPMS_PACKET_GLOBAL_CONTROL_IND_T;

/*! Response of a Global Control Command. */
typedef struct PROFIBUS_FSPMS_PACKET_GLOBAL_CONTROL_RES_Ttag
{
  HIL_PACKET_HEADER_T                 tHead; /*!< Standard Hilscher Header structure. */
} PROFIBUS_FSPMS_PACKET_GLOBAL_CONTROL_RES_T;

/*!@}*/

/*!\defgroup pckChkPrm Indicating the Reception of new Parameter Data
 * This service indicates the AP task that a Parameterization Command has been received. This
 * service is sent by a DP-Master to a DP-Slave right before the I/O communication is started, to
 * force validation of the Parameter Data on one hand and to parameterize the DP-Slave on the
 * other. The AP task has to check the received Parameter Data Set if it is consistent and valid, to
 * use it finally in the positive case as parameterization.
 * @{*/
/*! Data structure of "Check Parameter Data" indication.*/
typedef __HIL_PACKED_PRE struct PROFIBUS_FSPMS_CHECK_USER_PRM_IND_Ttag
{
  uint8_t abUserPrmData[PBDPS_USER_PRM_MAX]; /*!< Parameter data to be validated and accepted.*/
}__HIL_PACKED_POST PROFIBUS_FSPMS_CHECK_USER_PRM_IND_T;

/*! Indication - Packet to indicate  a "Check Parameter Data" command has been received.*/
typedef struct PROFIBUS_FSPMS_PACKET_CHECK_USER_PRM_IND_Ttag
{
  HIL_PACKET_HEADER_T                 tHead; /*!< Standard Hilscher Header structure. */
  PROFIBUS_FSPMS_CHECK_USER_PRM_IND_T tData; /*!< Indication Data structure. */
} PROFIBUS_FSPMS_PACKET_CHECK_USER_PRM_IND_T;

/*! Response to Indication Command of a Check Parameter Data. */
typedef __HIL_PACKED_PRE struct PROFIBUS_FSPMS_CHECK_USER_PRM_RES_Ttag
{
  /*! If set to true(1), the Parameter Data is accepted.
   * If set to false(0), the Parameter Data is not ok. */
  uint8_t fPrmOk;
}__HIL_PACKED_POST PROFIBUS_FSPMS_CHECK_USER_PRM_RES_T;

/*! Response to Indication Command of a Check Parameter Data. */
typedef struct PROFIBUS_FSPMS_PACKET_CHECK_USER_PRM_RES_Ttag
{
  HIL_PACKET_HEADER_T                 tHead; /*!< Standard Hilscher Header structure. */
  PROFIBUS_FSPMS_CHECK_USER_PRM_RES_T tData; /*!< Response Data Structure. */
} PROFIBUS_FSPMS_PACKET_CHECK_USER_PRM_RES_T;
/*!@}*/

/*!\defgroup pckExtPrmInd New Extended Parameter Data Indication/Response.
 * This service indicates the AP task that an extended Parameterization Command has been received.
 * This service is sent by a DP-Master to a DP-Slave between sending the standard parameter data and
 * the configuration information. The AP task has to check the received Parameter Dataset if it is
 * consistent and valid, to use it finally in the positive case as extended parameterization.
 * @{ */
/*! Extended Parameter Data Indication Data structure. */
typedef __HIL_PACKED_PRE struct PROFIBUS_FSPMS_CHECK_EXT_USER_PRM_IND_Ttag
{
  /*! extended parameter data that needs to be checked */
  uint8_t abExtUserPrmData[PBDPS_EXT_USER_PRM_MAX];
}__HIL_PACKED_POST PROFIBUS_FSPMS_CHECK_EXT_USER_PRM_IND_T;

/*! Extended Parameter Data Indication Packet. */
typedef struct PROFIBUS_FSPMS_PACKET_CHECK_EXT_USER_PRM_IND_Ttag
{
  HIL_PACKET_HEADER_T                     tHead;  /*!< Standard Hilscher Header structure.      */
  PROFIBUS_FSPMS_CHECK_EXT_USER_PRM_IND_T tData;  /*!< Extended Parameter Data Indication Data. */
} PROFIBUS_FSPMS_PACKET_CHECK_EXT_USER_PRM_IND_T;

/*! Extended Parameter Data Response Data structure. */
typedef __HIL_PACKED_PRE struct PROFIBUS_FSPMS_CHECK_EXT_USER_PRM_RES_Ttag
{
  /*! If set to TRUE(1) the AP task has accepted the extended Parameter Data. If set to FALSE(0),
   * the extended Parameter Data is not ok.*/
  uint8_t fExtPrmOk;
}__HIL_PACKED_POST PROFIBUS_FSPMS_CHECK_EXT_USER_PRM_RES_T;

/*! Extended Parameter Data Response Packet.*/
typedef struct PROFIBUS_FSPMS_PACKET_CHECK_EXT_USER_PRM_RES_Ttag
{
  HIL_PACKET_HEADER_T                     tHead;  /*!< Standard Hilscher Header structure. */
  PROFIBUS_FSPMS_CHECK_EXT_USER_PRM_RES_T tData;  /*!< Extended Parameter Data Response Data.*/
} PROFIBUS_FSPMS_PACKET_CHECK_EXT_USER_PRM_RES_T;
/*!@}*/


/*!\defgroup pckSetAddr Indicating the Reception of a Change Slave Address Request
 * This service indicates to the AP task that a DP-Master requests for changing the slave's current
 * address. Next to the new slave address itself the service may include also further slave specific
 * data which are usually stored in a permanent memory. This has to be done by the AP task. When
 * the service is indicated the MSCY1S state machine has already accepted the new station address
 * and it is still operational.
 * @{*/
/*! Data structure of Set Slave Address Indication.*/
typedef __HIL_PACKED_PRE struct PROFIBUS_FSPMS_SET_SLAVE_ADD_IND_Ttag
 {
  /*! New Slave Address.<br>Contains the new station address the DP-Master wants to change the
   *  slave's address to. Range: 0 - 126.*/
  uint8_t bNewSlaveAdd;

  /*! No Address Change.<br>Indicates whether there is an address change. If set to false(0), indicates
   * changing the address, otherwise (true(1)) no changing the address.*/
  uint8_t bNoAddChange;

  /*! Remenant Slave Data.<br>The array conveys the permanent parameter for this slave. It contains
   * parameters to be stored permanently and its use is fully user specific. This means it is not defined
   * how to behave with this data and what the contents are.*/
  uint8_t abRemSlaveData[PBDPS_SET_SLAVE_ADD_MAX];
}__HIL_PACKED_POST PROFIBUS_FSPMS_SET_SLAVE_ADD_IND_T;

/*! Set Slave Address Indication Packet Data Length in bytes without Remenant Slave Data.*/
#define PROFIBUS_FSPMS_SET_SLAVE_ADD_IND_SIZE    (sizeof(PROFIBUS_FSPMS_SET_SLAVE_ADD_IND_T) - \
                                                  PBDPS_SET_SLAVE_ADD_MAX)

/*! Indication - Packet that indicates the Request for changing the Slave Address.*/
typedef struct PROFIBUS_FSPMS_PACKET_SET_SLAVE_ADD_IND_Ttag
{
  HIL_PACKET_HEADER_T                 tHead; /*!< Standard Hilscher Header structure. */
  PROFIBUS_FSPMS_SET_SLAVE_ADD_IND_T  tData; /*!< Indication Set Slave Address Data structure.*/
} PROFIBUS_FSPMS_PACKET_SET_SLAVE_ADD_IND_T;


/*! Response - Packet that indicates the Request for changing the Slave Address.*/
typedef struct PROFIBUS_FSPMS_PACKET_SET_SLAVE_ADD_RES_Ttag
{
  HIL_PACKET_HEADER_T                 tHead; /*!< Standard Hilscher Header structure. */
} PROFIBUS_FSPMS_PACKET_SET_SLAVE_ADD_RES_T;
/*!@}*/

typedef __HIL_PACKED_PRE struct PROFIBUS_FSPMS_STARTED_IND_Ttag
{
  uint8_t bActualEnabledAlarms;
  uint8_t fAlarmSequence;
  uint8_t bAlarmLimit;
}__HIL_PACKED_POST PROFIBUS_FSPMS_STARTED_IND_T;

typedef struct PROFIBUS_FSPMS_PACKET_STARTED_IND_Ttag
{
  HIL_PACKET_HEADER_T tHead; /*!< Standard Hilscher Header structure. */
  PROFIBUS_FSPMS_STARTED_IND_T tData;
} PROFIBUS_FSPMS_PACKET_STARTED_IND_T;



/* ******************************************************************************************* */
typedef __HIL_PACKED_PRE struct PROFIBUS_FSPMS_STATE_CHANGED_IND_Ttag
{
  uint32_t  ulState;
  uint32_t  ulError;
  uint32_t  ulReason;
}__HIL_PACKED_POST PROFIBUS_FSPMS_STATE_CHANGED_IND_T;

#define PROFIBUS_FSPMS_STATE_CHANGED_IND_DATA_SIZE          (sizeof(PROFIBUS_FSPMS_STATE_CHANGED_IND_T))

typedef struct PROFIBUS_FSPMS_PACKET_STATE_CHANGED_IND_Ttag
{
  HIL_PACKET_HEADER_T tHead; /*!< Standard Hilscher Header structure. */
  PROFIBUS_FSPMS_STATE_CHANGED_IND_T tData;
} PROFIBUS_FSPMS_PACKET_STATE_CHANGED_IND_T;


/*****************************************************************************/
/*  DPV1 Class 2 packets                                                     */
/*****************************************************************************/

typedef __HIL_PACKED_PRE struct PROFIBUS_FSPMS_INITIATE_ADDR_Ttag
{
  uint8_t bAPI;
  uint8_t bSCL;
  uint8_t abNetworkAddress[6];
  uint8_t abMACAddress[];
}__HIL_PACKED_POST PROFIBUS_FSPMS_INITIATE_ADDR_T;


/*!\defgroup pckC2Init Request to establish an acyclic Connection to a DP-Master Class 2 Indication/Response
 * This PROFIBUS_FSPMS_CMD_C2_INITIATE_IND indicates the AP-Task that a connection to a PROFIBUS
 * DP-Master Class 2 shall be established.
 * To complete the connection initialization, the AP-Task has to respond and deliver the requested
 * data by using the service PROFIBUS_FSPMS_CMD_C2_INITIATE_RES.
 *@{*/
#define PBDP_DPV1C2_INIT_FEATURE_RW                         (0x0001) /*!< Feature support read & write.*/
#define PBDP_DPV1C2_INIT_FEATURE_RESERVED                   (0xFFFE) /*!< Reserved bit fields. */
#define PBDP_DPV1C2_INIT_FEATURE_PROFILE_RESERVED           (0xFFFF) /*!< Reserved bit fields. */

/*! Indication Data structure of a Request to establish a DP-Master Class 2 Connection.*/
typedef __HIL_PACKED_PRE struct PROFIBUS_FSPMS_C2_INITIATE_IND_Ttag
{
  /*! Reference number of DP V1 Class 2 connection. */
  uint32_t ulReference;

  /*! Requested features of the DP-Master Class 2 connection. The bit field informs the AP-Task
   * about the requested service functionality. The AP-Task has the possibility to adjust its
   * functionality to the DP-Master's requirements or to reject if it cannot fulfill them. Possible
   * values are 0 and 1. 1 means DPV1 Read and Write is supported. */
  uint16_t usFeaturesSupported;

  /*! Reserved, set to 0*/
  uint16_t usProfileFeaturesSupported;

  /*! Corresponding Profile Ident Number. The variable identifies a profile definition uniquely. All
   * devices using the same profile definition have to use the same Profile Ident Number. The
   * Profile Ident number will be taken from the pool of Ident Numbers for vendor specific or
   * authorized profiles. 0 indicates that no profile is in use. */
  uint16_t usProfileIdentNumber;

  /*! The variable  indicates the presence if TRUE = 1 of the optional Network and MAC address.*/
  uint8_t  fSType;

  /*! Length of the Source Address length parameter. */
  uint8_t  bSLen;

  /*! Decides on the presence of the optional destination Network/MAC address in abSAddrDAddr*/
  uint8_t  fDType;

  /*! Length of the Destination Address length parameter in abSAddrDAddr. */
  uint8_t  bDLen;

  /*! Array the contains the description of the source and destination address.*/
  uint8_t  abSAddrDAddr[PBDPS_INITIATE_ADD_TABLE_MAX];
}__HIL_PACKED_POST PROFIBUS_FSPMS_C2_INITIATE_IND_T;

/*! Indication Packet of a Request to establish a DP-Master Class 2 Connection.
 * PROFIBUS_FSPMS_CMD_C2_INITIATE_IND - Indication Command of a Request to establish a DP-Master
 * Class 2 Connection.
 * If the requested profile is supported by the AP-Task, the Profile Ident Number is mirrored in the
 * response. If the requested profile is not supported by the AP-Task, the AP-Task has to respond
 * negatively or with the Profile Ident Number the AP-Task supports.*/
typedef struct PROFIBUS_FSPMS_PACKET_C2_INITIATE_IND_Ttag
{
  HIL_PACKET_HEADER_T               tHead; /*!< Standard Hilscher Header structure. */
  PROFIBUS_FSPMS_C2_INITIATE_IND_T  tData; /*!< Indication Data. */
} PROFIBUS_FSPMS_PACKET_C2_INITIATE_IND_T;

/*! Indication of a Request to establish a DP-Master Class 2 Connection packet data length in bytes
 * without number of bytes in Source/Destination Data block. */
#define PROFIBUS_FSPMS_C2_INITIATE_IND_DATA_SIZE      (sizeof(PROFIBUS_FSPMS_C2_INITIATE_IND_T) - \
                                                       PBDPS_INITIATE_ADD_TABLE_MAX)

/*! Positive Response Data structure of a DP-Master Class 2 Connection Request.
 * */
typedef __HIL_PACKED_PRE struct PROFIBUS_FSPMS_C2_INITIATE_RES_POS_Ttag
{
  /*! Reference number of DP V1 Class 2 connection. */
  uint32_t ulReference;

  /*! Maximum number of bytes allowed to be transported via the DP-Master Class 2 connection. The
   * value has a range of 1 to 240.*/
  uint16_t usMaxLenDataUnit;

  /*! Supported features of the DP-Master Class 2 connection. */
  uint16_t usFeaturesSupported;

  /*! Supported profile features of the DP-Master Class 2 connection*/
  uint16_t usProfileFeaturesSupported;

  /*! Corresponding Profile Ident Number. 0 indicates that no profile is supported.*/
  uint16_t usProfileIdentNumber;

  /*! Decides on the presence of the optional source Network/MAC address. */
  uint8_t  fSType;

  /*! Length of the Source Address length parameter in abSAddrDAddr. */
  uint8_t  bSLen;

  /*! Decides on the presence of the optional destination Network/MAC address. */
  uint8_t  fDType;

  /*! Length of the Destination Address length parameter in abSAddrDAddr.*/
  uint8_t  bDLen;

  /*! Array containing the description of the source and destination address. */
  uint8_t  abSAddrDAddr[PBDPS_INITIATE_ADD_TABLE_MAX];
}__HIL_PACKED_POST PROFIBUS_FSPMS_C2_INITIATE_RES_POS_T;

 /*! Positive Response Packet of a DP-Master Class 2 Connection Request.
  * PROFIBUS_FSPMS_CMD_C2_INITIATE_RES_POS - Positive Response Command of a DP-Master Class 2
  * Connection Request.  tHead.ulSta = SUCCESS_HIL_OK */
typedef struct PROFIBUS_FSPMS_PACKET_C2_INITIATE_RES_POS_Ttag
{
  HIL_PACKET_HEADER_T                   tHead; /*!< Standard Hilscher Header structure. */
  PROFIBUS_FSPMS_C2_INITIATE_RES_POS_T  tData; /*!< Positive Response Data.             */
} PROFIBUS_FSPMS_PACKET_C2_INITIATE_RES_POS_T;

/*! Positive Response of a DP-Master Class 2 Connection Request Packet Data Length in bytes  without
 * number of byte in the Source/Destination Address field. */
#define PROFIBUS_FSPMS_C2_INITIATE_RES_DATA_SIZE  (sizeof(PROFIBUS_FSPMS_C2_INITIATE_RES_POS_T) - \
                                                   PBDPS_INITIATE_ADD_TABLE_MAX)

/*! Negative Response Data structure of a DP-Master Class 2 initialization request. */
typedef __HIL_PACKED_PRE struct PROFIBUS_FSPMS_C2_INITIATE_RES_NEG_Ttag
{
  uint32_t  ulReference;  /*!< Reference number of DP V1 Class 2 connection. */

  /*! Error decode value classifying the error 128 indicates DP V1 error handling is applied 254,
   * 255 indicate profile-specific error handling is applied. */
  uint8_t   bErrorDecode;
  uint8_t   bErrorCode1;  /*!< Detailed error code.       */
  uint8_t   bErrorCode2;  /*!< User specific error code.  */
}__HIL_PACKED_POST PROFIBUS_FSPMS_C2_INITIATE_RES_NEG_T;

 /*! Negative Response Packet of a DP-Master Class 2 initialization request.
  * PROFIBUS_FSPMS_CMD_C2_INITIATE_RES_NEG - Negative Response Command of a DP-Master Class 2
  * initialization request. */
typedef struct PROFIBUS_FSPMS_PACKET_C2_INITIATE_RES_NEG_Ttag
{
  HIL_PACKET_HEADER_T                   tHead;  /*!< Standard Hilscher Header structure. */
  PROFIBUS_FSPMS_C2_INITIATE_RES_NEG_T  tData;  /*!< Negative Response Data. */
} PROFIBUS_FSPMS_PACKET_C2_INITIATE_RES_NEG_T;
/*!@}*/


/*!\defgroup pckC2Read An Acyclic Read Request (Class 2) to a Specific Process Data Object Indication/Response
 * This service indicates the AP-Task that a specific Process Data Object shall be read by a DPMaster
 * Class 2. The AP-Task has to take care of the Process Data objects themselves. This means that it
 * is fully application specific where those Process Data objects are read from and what purpose
 * they have. To complete the Process Data read indication, the AP-Task has to respond and deliver
 * the requested data by using the service PROFIBUS_FSPMS_CMD_C2_READ_RES_POS - Positive Response
 * Command of a Process Data read or PROFIBUS_FSPMS_CMD_C2_READ_RES_NEG - Negative Response Command
 * of a Process Data read.
 *@{*/
/*! Command of a Process Data Read Request Indication Data structure. */
typedef __HIL_PACKED_PRE struct PROFIBUS_FSPMS_C2_READ_IND_Ttag
{
  uint32_t  ulReference;  /*!< Reference number of DP V1 Class 2 connection. */

  /*! Slot number of the Process Data object to be read. The variable has to be used in the AP-Task
   * for addressing the desired Process Data object in the specified slot. */
  uint8_t   bSlotNumber;

  /*! Index of the Process Data object to be read. The variable has to be used in the AP-Task for
   * addressing the desired Process Data object itself.*/
  uint8_t   bIndex;

  /*! Number of bytes to be read in the specified Process Data object. Value range: 0 - 240 bytes.*/
  uint8_t   bLength;
}__HIL_PACKED_POST PROFIBUS_FSPMS_C2_READ_IND_T;

/*! Process Data Read Request Indication Packet.
 * PROFIBUS_FSPMS_CMD_C2_READ_IND - Indication Command of a Process Data Read Request. */
typedef struct PROFIBUS_FSPMS_PACKET_C2_READ_IND_Ttag
{
  HIL_PACKET_HEADER_T           tHead;  /*!< Standard Hilscher Header structure. */
  PROFIBUS_FSPMS_C2_READ_IND_T  tData;  /*!< Command of a Process Data Read Request Indication Data.*/
} PROFIBUS_FSPMS_PACKET_C2_READ_IND_T;

/*! Command of a Process Data Read Request Indication Packet Data Length in bytes. */
#define PROFIBUS_FSPMS_C2_READ_IND_SIZE                     (sizeof(PROFIBUS_FSPMS_C2_READ_IND_T))

/*! Process Data Read Positive Data structure.
 * The variable bLength defines how many bytes are read and returned to the FSPMS-Task. The
 * indicated length and the returned length may differ. It is allowed for example that the indication
 * service requests more data bytes to be read than physically can be returned. In this case the
 * response service just returns the maximum number of bytes that are really returned. Within the
 * array abData[] the AP-Task returns the current process data object's data*/
typedef __HIL_PACKED_PRE struct PROFIBUS_FSPMS_C2_READ_RES_POS_Ttag
{
  uint32_t  ulReference;    /*!< Reference number of DP V1 Class 2 connection. */
  uint8_t   bSlotNumber;    /*!< Slot number of the Process Data Object that has been read. */
  uint8_t   bIndex;         /*!< Index of the Process Data object that has been read. */
  uint8_t   bLength;        /*!< Number of data bytes read from the specified Process Data Object. */
  uint8_t   abData[PBDPS_READ_MAX]; /*!< Process Data object data that has been read. */
}__HIL_PACKED_POST PROFIBUS_FSPMS_C2_READ_RES_POS_T;


/*! Process Data Read Request Positive Response Packet.
 * PROFIBUS_FSPMS_CMD_C2_READ_RES - Positive Response Command of a Process Data read
 * tHead.ulSta shall be SUCCESS_HIL_OK (0).*/
typedef struct PROFIBUS_FSPMS_PACKET_C2_READ_RES_POS_Ttag
{
  HIL_PACKET_HEADER_T               tHead;  /*!< Standard Hilscher Header structure. */
  PROFIBUS_FSPMS_C2_READ_RES_POS_T  tData;  /*!< Process Data Read Positive Data. */
} PROFIBUS_FSPMS_PACKET_C2_READ_RES_POS_T;

/*! */
#define PROFIBUS_FSPMS_C2_READ_RES_POS_SIZE           (sizeof(PROFIBUS_FSPMS_C2_READ_RES_POS_T) - \
                                                       PBDPS_READ_MAX)

/*! Process Data Read Request Negative Data structure.
 * A negative response is issued whenever the indicated request cannot be satisfied by the AP-Task.
 * 3 variables are specifying the type and source of the error in this event.*/
typedef __HIL_PACKED_PRE struct PROFIBUS_FSPMS_C2_READ_RES_NEG_Ttag
{
  uint32_t  ulReference;    /*!< Reference number of DP V1 Class 2 connection. */

  /*! Error decode value classifying the error 128 indicates DP V1 error handling is applied 254,
   * 255 indicate profile-specific error handling is applied. */
  uint8_t   bErrorDecode;

  uint8_t   bErrorCode1;  /*!< Detailed error code.       */
  uint8_t   bErrorCode2;  /*!< User specific error code.  */
}__HIL_PACKED_POST PROFIBUS_FSPMS_C2_READ_RES_NEG_T;

 /*! Process Data Read Negative Response Packet.
  *  PROFIBUS_FSPMS_CMD_C2_READ_RES - Negative Response Command of a Process Data read
 * tHead.ulSta shall be ERR_HIL_FAIL (or  =! SUCCESS_HIL_OK ) */
typedef struct PROFIBUS_FSPMS_PACKET_C2_READ_RES_NEG_Ttag
{
  HIL_PACKET_HEADER_T               tHead;  /*!< Standard Hilscher Header structure. */
  PROFIBUS_FSPMS_C2_READ_RES_NEG_T  tData;  /*!< Process Data Read Negative Data.*/
} PROFIBUS_FSPMS_PACKET_C2_READ_RES_NEG_T;
/*!@}*/


/*!\defgroup pckC2Write Acyclic Write Request to a Specific Process Data Object Indication/Response
 * This service indicates the AP-Task that a specific Process Data Object shall be written by a DPMaster
 * Class 2. The AP-Task has to take care of the Process Data objects themselves. This means that it
 * is fully application specific where those Process Data objects are stored to and what purpose
 * they have. To complete the process data write indication, the AP-Task has to respond and deliver
 * the requested data by using the service PROFIBUS_FSPMS_C2_WRITE_RES_POS_T - Positive Response
 * Command of a Process Data Write or PROFIBUS_FSPMS_C2_WRITE_RES_NEG_T - Negative Response
 * Command of a Process Data write.
 * @{*/
/*! Process Data Write Request Indication Data structure. */
typedef __HIL_PACKED_PRE struct PROFIBUS_FSPMS_C2_WRITE_IND_Ttag
{
  uint32_t  ulReference;    /*!< Reference number of DP V1 Class 2 connection. */
  uint8_t   bSlotNumber;    /*!< Slot number of the Process Data object to be written. */
  uint8_t   bIndex;         /*!< Index of the Process Data object to be written. */
  uint8_t   bLength;   /*!< Number of bytes to be written into the specified Process Data object.*/
  uint8_t   abData[PBDPS_WRITE_MAX]; /*!< Process Data object data intended to be written. */
}__HIL_PACKED_POST PROFIBUS_FSPMS_C2_WRITE_IND_T;

/*! Process Data Write Request Indication Packet.
 * PROFIBUS_FSPMS_CMD_C2_WRITE_IND - Indication Command of a Process Data Write Request.
 * */
typedef struct PROFIBUS_FSPMS_PACKET_C2_WRITE_IND_Ttag
{
  HIL_PACKET_HEADER_T           tHead;  /*!< Standard Hilscher Header structure.        */
  PROFIBUS_FSPMS_C2_WRITE_IND_T tData;  /*!< Process Data Write Request Indication Data.*/
} PROFIBUS_FSPMS_PACKET_C2_WRITE_IND_T;

/*! Process Data Write Request Indication Packet Data Length in bytes without number of bytes
 * in write data block.*/
#define PROFIBUS_FSPMS_C2_WRITE_IND_SIZE                 (sizeof(PROFIBUS_FSPMS_C2_WRITE_IND_T) - \
                                                          PBDPS_WRITE_MAX)


/*! Process Data Write Request Positive Response Data structure. */
typedef __HIL_PACKED_PRE struct PROFIBUS_FSPMS_C2_WRITE_RES_POS_Ttag
{
  uint32_t  ulReference;  /*! Reference number of DP V1 Class 2 connection.                 */
  uint8_t   bSlotNumber;  /*! Slot number of the Process Data object that has been written. */
  uint8_t   bIndex;       /*! Index of the Process Data object that has been written.       */
  uint8_t   bLength;      /*! Number of real written process data bytes.                    */
}__HIL_PACKED_POST PROFIBUS_FSPMS_C2_WRITE_RES_POS_T;

/*! Process Data Write Request Positive Response Packet.
 * PROFIBUS_FSPMS_CMD_C2_WRITE_RES - Positive Response Command of a Process Data Write*.
 * A positive write response does not have any additional parameter data. ulSta = SUCCESS_HIL_OK */
typedef struct PROFIBUS_FSPMS_PACKET_C2_WRITE_RES_POS_Ttag
{
  HIL_PACKET_HEADER_T               tHead;  /*!< Standard Hilscher Header structure. */
  PROFIBUS_FSPMS_C2_WRITE_RES_POS_T tData;  /*!< Process Data Write Request Positive Response Data.*/
} PROFIBUS_FSPMS_PACKET_C2_WRITE_RES_POS_T;

/*! Process Data Write Request Positive Response Packet Data Length in bytes. */
#define PROFIBUS_FSPMS_C2_WRITE_RES_POS_SIZE            (sizeof(PROFIBUS_FSPMS_C2_WRITE_RES_POS_T))


/*! Process Data Write Negative Response Data structure.*/
typedef __HIL_PACKED_PRE struct PROFIBUS_FSPMS_C2_WRITE_RES_NEG_Ttag
{
  uint32_t  ulReference;  /*!< Reference number of DP V1 Class 2 connection.*/

  /*! Error decode value classifying the error 128 indicates DP V1 error handling is applied 254,
   * 255 indicate profile-specific error handling is applied. */
  uint8_t   bErrorDecode;

  uint8_t   bErrorCode1;  /*!< Detailed error code.       */
  uint8_t   bErrorCode2;  /*!< User specific error code.  */
}__HIL_PACKED_POST PROFIBUS_FSPMS_C2_WRITE_RES_NEG_T;

 /*! Process Data Write Negative Response Packet.
  * PROFIBUS_FSPMS_CMD_C2_WRITE_RES - Negative Response Command of a Process Data write.
  * tHad.ulSta = ERR_HIL_FAIL  (notSUCCESS_HIL_OK). A negative response is issued whenever the
  * indicated request cannot be satisfied by the AP-Task. 3 variables are specifying the type and
  * source of the error in this event. */
typedef struct PROFIBUS_FSPMS_PACKET_C2_WRITE_RES_NEG_Ttag
{
  HIL_PACKET_HEADER_T               tHead;  /*!< Standard Hilscher Header structure. */
  PROFIBUS_FSPMS_C2_WRITE_RES_NEG_T tData;  /*! Process Data Write Negative Response Data.*/
} PROFIBUS_FSPMS_PACKET_C2_WRITE_RES_NEG_T;
/*!@}*/


/*!\defgroup pckC2Trans Indicating an acyclic Data Transport Request to a Single Combined Process Data Object
 * This service indicates the AP-Task that a specific process data object shall accessed in a
 * client/server manner by a DP-Master Class 2. The AP-Task has to take care of the process data
 * objects themselves. This means that it is fully application specific where those Process Data
 * objects are stored to and what purpose they have.
 *@{*/
/*! Process Data Transport Request Indication Data structure. */
typedef __HIL_PACKED_PRE struct PROFIBUS_FSPMS_C2_DATA_TRANSPORT_IND_Ttag
{
  uint32_t  ulReference;  /*!< Reference number of DP V1 Class 2 connection.            */
  uint8_t   bSlotNumber;  /*!< Slot number of the Process Data object to be addressed.  */
  uint8_t   bIndex;       /*!< Index of the Process Data object to be addressed.        */
  uint8_t   bLength;      /*!< Number of bytes to be transported in the specified Process Data object. */
  uint8_t   abData[PBDPS_DATATRANSPORT_MAX];  /*!< Process data object: data that is transported in the request. */
}__HIL_PACKED_POST PROFIBUS_FSPMS_C2_DATA_TRANSPORT_IND_T;

/*! Process Data Transport Request Indication Packet.
 * PROFIBUS_FSPMS_CMD_C2_DATA_TRANSPORT_IND - Indication Command of a Process Data Transport Request.
 * */
typedef struct PROFIBUS_FSPMS_PACKET_C2_DATA_TRANSPORT_IND_Ttag
{
  HIL_PACKET_HEADER_T                     tHead;  /*!< Standard Hilscher Header structure. */
  PROFIBUS_FSPMS_C2_DATA_TRANSPORT_IND_T  tData;  /*!< Process Data Transport Request Indication Data*/
} PROFIBUS_FSPMS_PACKET_C2_DATA_TRANSPORT_IND_T;

/*! Process Data Transport Request Indication Packet Data Length in bytes without number of bytes in
 * data transport data block.*/
 #define PROFIBUS_FSPMS_C2_DATA_TRANSPORT_IND_SIZE (sizeof(PROFIBUS_FSPMS_C2_DATA_TRANSPORT_IND_T) - \
                                                    PBDPS_DATATRANSPORT_MAX)


/*! Process Data Transport Request Response Data structure. */
typedef __HIL_PACKED_PRE struct PROFIBUS_FSPMS_C2_DATA_TRANSPORT_RES_POS_Ttag
{
  uint32_t  ulReference;  /*!< Reference number of DP V1 Class 2 connection.*/
  uint8_t   bSlotNumber;  /*!< Slot number of the process data object to be written. */
  uint8_t   bIndex;       /*!< Index of the Process Data object to be written. */
  uint8_t   bLength;      /*!< Number of transported data bytes.*/
  uint8_t abData[PBDPS_DATATRANSPORT_MAX];  /*!< Process Data object data that are transported.*/
}__HIL_PACKED_POST PROFIBUS_FSPMS_C2_DATA_TRANSPORT_RES_POS_T;

/*! Process Data Transport Request Response Packet.
 * The variable bLength defines how many bytes are transported back to the FSPMS-Task. Within the
 * array tData.abData[] the AP-Task returns the current Process Data object's transport data
 * tHead.ulSta = SUCCESS_HIL_OK*/
typedef struct PROFIBUS_FSPMS_PACKET_C2_DATA_TRANSPORT_RES_POS_Ttag
{
  HIL_PACKET_HEADER_T                         tHead;  /*!< Standard Hilscher Header structure. */
  PROFIBUS_FSPMS_C2_DATA_TRANSPORT_RES_POS_T  tData;  /*!< Process Data Transport Request Response Data.*/
} PROFIBUS_FSPMS_PACKET_C2_DATA_TRANSPORT_RES_POS_T;

/*Packet Data Length in bytes*/
#define PROFIBUS_FSPMS_C2_DATA_TRANSPORT_RES_POS_SIZE (sizeof(PROFIBUS_FSPMS_C2_DATA_TRANSPORT_RES_POS_T) - \
                                                       PBDPS_DATATRANSPORT_MAX)


/*! Process Data Transport Request Negative Response Data structure. */
typedef __HIL_PACKED_PRE struct PROFIBUS_FSPMS_C2_DATA_TRANSPORT_RES_NEG_Ttag
{
  uint32_t  ulReference;  /*!< Reference number of DP V1 Class 2 connection.*/

  /*! Error decode value classifying the error 128 indicates DP V1 error handling is applied 254,
   * 255 indicate profile-specific error handling is applied. */
  uint8_t   bErrorDecode;

  uint8_t   bErrorCode1;  /*!< Detailed error code.       */
  uint8_t   bErrorCode2;  /*!< User specific error code.  */
}__HIL_PACKED_POST PROFIBUS_FSPMS_C2_DATA_TRANSPORT_RES_NEG_T;

 /*! Process Data Transport Request Negative Response Packet.
  * Negative response packet of an acyclic read command */
typedef struct PROFIBUS_FSPMS_PACKET_C2_DATA_TRANSPORT_RES_NEG_Ttag
{
  HIL_PACKET_HEADER_T                         tHead; /*!< Standard Hilscher Header structure. */
  PROFIBUS_FSPMS_C2_DATA_TRANSPORT_RES_NEG_T  tData; /*!< Process Data Transport Request Negative Response Data.*/
} PROFIBUS_FSPMS_PACKET_C2_DATA_TRANSPORT_RES_NEG_T;
/*!@}*/


/*!\defgroup pckC2Abort Indicating the Abort of Class 2 Connection
 * The PROFIBUS DP Master Class 2 may send an abort request for the MSAC_C2 connection to the
 * PROFIBUS DP Slave. If such a request arrives at the PROFIBUS DP Slave, this indication is received.
 *@{ */
/*! Abort of Class 2 Connection Indication Data structure. */
typedef __HIL_PACKED_PRE struct PROFIBUS_FSPMS_C2_ABORT_IND_Ttag
{
  uint32_t  ulReference;  /*!< Reference number of DP V1 Class 2 connection.*/

  /*! Indicates whether the source of the abort is local:
   * - 0: Abort occurred in remote partner
   * - 1: Abort occurred in local station.*/
  uint32_t  ulLocal;

  /*! Subnet. The variable indicates the source of the abort, i.e.. whether it originates from the
   * local or a remote subnet or no specific source information should be given. The coding is as
   * follows:
   * - 0: No
   * - 1: Local
   * - 2: Remote
   * - 3-255: Reserved*/
  uint32_t  ulSubnet;

  /*! Instance Code.The variable causing the abort is specified here. Possible values are FDL, USER, MSAC_C2.
   * The coding is as follows:
   * - 0x00: FDL - PBDPS_INSTANCE_DLL
   * - 0x10: MSAC_C2 - PBDPS_INSTANCE_MSAC2
   * - 0x20: User - PBDPS_INSTANCE_USER
   * Bits 0 to 3 are always 0.*/
  uint32_t  ulInstance;

  /*! Reason code. The reason code is a 4 bit value (between 0 and 15) whose meaning depends from the
   * instance. It contains information of the cause why the MSAC_C2 connection is to be aborted.*/
  uint32_t  ulReasonCode;
}__HIL_PACKED_POST PROFIBUS_FSPMS_C2_ABORT_IND_T;

/*! Abort of Class 2 Connection Indication Packet. */
typedef struct PROFIBUS_FSPMS_PACKET_C2_ABORT_IND_Ttag
{
  HIL_PACKET_HEADER_T           tHead;  /*!< Standard Hilscher Header structure. */
  PROFIBUS_FSPMS_C2_ABORT_IND_T tData;  /*!< Abort of Class 2 Connection Indication Data.*/
} PROFIBUS_FSPMS_PACKET_C2_ABORT_IND_T;

/*! Abort of Class 2 Connection Indication Packet Data Length in bytes. */
 #define PROFIBUS_FSPMS_C2_ABORT_IND_SIZE                   (sizeof(PROFIBUS_FSPMS_C2_ABORT_IND_T))


/*! Abort of Class 2 Connection Response Data structure.*/
typedef __HIL_PACKED_PRE struct PROFIBUS_FSPMS_C2_ABORT_RES_Ttag
{
  uint32_t  ulReference;  /*!< Reference number of DP V1 Class 2 connection.*/
}__HIL_PACKED_POST PROFIBUS_FSPMS_C2_ABORT_RES_T;

/*! Abort of Class 2 Connection Response Packet. */
typedef struct PROFIBUS_FSPMS_PACKET_C2_ABORT_RES_Ttag
{
  HIL_PACKET_HEADER_T           tHead;  /*!< Standard Hilscher Header structure. */
  PROFIBUS_FSPMS_C2_ABORT_RES_T tData;  /*!< Abort of Class 2 Connection Response Data.*/
} PROFIBUS_FSPMS_PACKET_C2_ABORT_RES_T;

/*! Abort of Class 2 Connection Response Packet Data Length in bytes*/
#define PROFIBUS_FSPMS_C2_ABORT_RES_SIZE                    (sizeof(PROFIBUS_FSPMS_C2_ABORT_RES_T))
/*!@}*/


typedef __HIL_PACKED_PRE struct PROFIBUS_FSPMS_IO_LENGTH_DATA_Ttag
{
  uint32_t ulInputLen;
  uint32_t ulOutputLen;
}__HIL_PACKED_POST PROFIBUS_FSPMS_UPDATE_IO_LENGTH_DIAG_DATA_T;

typedef struct PROFIBUS_FSPMS_PACKET_IO_LENGTH_DATA_REQ_Ttag
{
  HIL_PACKET_HEADER_T                         tHead; /*!< Standard Hilscher Header structure. */
  PROFIBUS_FSPMS_UPDATE_IO_LENGTH_DIAG_DATA_T tData;
}PROFIBUS_FSPMS_PACKET_UPDATE_IO_LENGTH_DIAG_DATA_REQ_T;

/* I&M packets */

/*!\defgroup pckSetIM0 Change I&M0 Parameter Settings
 * The PROFIBUS DP Slave stack handles the I&M0 index slot 0 on its own. If a write request is
 * successful, the stack increments the revision counter variable of I&M0 slot 0 structure. The stack
 *  supports the slots from 0 up to ulSlotsNotZeroSupp. The I&M0 of a slot different from 0 has to
 *  be handled by the application.
 * @{*/
/*! Set I&M0 Parameter Settings Request.<br>In order to change the default (listed below) I&M0
 * parameter and/or dis-/enable the other I&M indices the command PROFIBUS_FSPMS_CMD_SET_IM0_REQ has
 * to be send. To receive I&M calls (not including I&M0 slot 0) the variables usIMSupported,
 * fProfSpecSupp and fManuSpecSupp of I&M0 index have to be changed for slot 0 requests.
 * The I&M0 slot 0 has no retain data. To ensure the correctness of variable PBDPS_IM0_T.usRevisionCounter,
 * the increment has to be done at application level and rewritten to retain value after channel init (power on)*/
typedef struct PROFIBUS_FSPMS_PACKET_SET_IM0_REQ_Ttag
{
  HIL_PACKET_HEADER_T tHead; /*!< Standard Hilscher Header structure. */
  PBDPS_IM_DATA_T     tData; /*!< Profibus DPS IM Data structure. */
}PROFIBUS_FSPMS_PACKET_SET_IM0_REQ_T;

#define PROFIBUS_FSPMS_SET_IM0_REQ_SIZE       (sizeof(PBDPS_IM_DATA_T))

/*! Confirmation to Change I&M0 Parameter Settings Request. */
typedef struct PROFIBUS_FSPMS_PACKET_SET_IM0_CNF_Ttag
{
  HIL_PACKET_HEADER_T tHead; /*!< Standard Hilscher Header structure. */
}PROFIBUS_FSPMS_PACKET_SET_IM0_CNF_T;
/*!@}*/

/*! Profibus FSPMS Identification & Maintenance Index */
typedef __HIL_PACKED_PRE struct PROFIBUS_FSPMS_IM_INDEX_Ttag
{
  uint32_t ulSAPIdx;    /*!< SAP Index.*/
  uint32_t ulSlot;      /*!< Slot Number.*/
  uint32_t ulIM_Index;  /*!< Identification & Maintenance Index.*/
  uint32_t ulLen;       /*!< Length. */
}__HIL_PACKED_POST PROFIBUS_FSPMS_IM_INDEX_T;

/*! Profibus FSPMS Identification & Maintenance Read Response Data Structure
 * \ingroup pckIMRead
 * @{ */
typedef __HIL_PACKED_PRE struct PROFIBUS_FSPMS_IM_READ_RES_Ttag
{
  PROFIBUS_FSPMS_IM_INDEX_T tInfo;  /*!< Index Structure*/

  /*! Contains read data in case of success and error structure in case of failure */
  __HIL_PACKED_PRE union
  {
    uint8_t  abData[244];         /*!< Read data.*/
    PBDPS_DPV1_ERROR_T tError;    /*!< Error structure.*/
  }__HIL_PACKED_POST un;
}__HIL_PACKED_POST PROFIBUS_FSPMS_IM_READ_RES_T;
/*!@}*/

/*!\defgroup pckIMWrite Identification & Maintenance Write Indication/Response
 *@{*/
/*! I&M Write Indication Data. */
typedef __HIL_PACKED_PRE struct PROFIBUS_FSPMS_IM_WRITE_IND_Ttag
{
  PROFIBUS_FSPMS_IM_INDEX_T tInfo;  /*!< I&M Index structure. */
  uint8_t abData[238];              /*!< I&M Data.            */
}__HIL_PACKED_POST PROFIBUS_FSPMS_IM_WRITE_IND_T;

/*!< &M Write Response Data. */
typedef __HIL_PACKED_PRE struct PROFIBUS_FSPMS_IM_WRITE_RES_Ttag
{
  PROFIBUS_FSPMS_IM_INDEX_T tInfo;    /*!< I&M Index structure. */
  PBDPS_DPV1_ERROR_T        tError;   /*!< Error Structure.     */
}__HIL_PACKED_POST PROFIBUS_FSPMS_IM_WRITE_RES_T;

/*! I&M Write Indication Packet.
 * The indication is sent by the DPS stack if a PROFIBUS DP Master has send a I&M call set request
 * and the Application has registered an I&M unequal I&M0 Slot 0 with the PROFIBUS_FSPMS_CMD_SET_IM0_REQ request.*/
typedef struct PROFIBUS_FSPMS_PACKET_IM_WRITE_IND_Ttag
{
  HIL_PACKET_HEADER_T           tHead; /*!< Standard Hilscher Header structure. */
  PROFIBUS_FSPMS_IM_WRITE_IND_T tData; /*!< Indication Data. */
}PROFIBUS_FSPMS_PACKET_IM_WRITE_IND_T;

/*! I&M Write Response.
 * If the slot number, I&M index or data are incorrect, tHead.ulSta has to be set to ERR_HIL_FAIL
 * and the corresponding error must be entered in tData.tError. The data tData.tInfo.ulSAPIdx,
 * tData.tInfo.ulSlot and tData.tInfo.ulIM_Index must not be changed.*/
typedef struct PROFIBUS_FSPMS_PACKET_IM_WRITE_RES_Ttag
{
  HIL_PACKET_HEADER_T           tHead;  /*!< Standard Hilscher Header structure. */
  PROFIBUS_FSPMS_IM_WRITE_RES_T tData;  /*!< I&M Write Response Data structure.  */
}PROFIBUS_FSPMS_PACKET_IM_WRITE_RES_T;
/*!@}*/

/*!\defgroup pckIMRead Identification & Maintenance Read Indication/Response
 *@{*/
/*! I&M Read Indication Packet.
 * The indication is sent if a PROFIBUS DP Master has send a I&M call get request and the
 * Application has registered an I&M unequal I&M0 Slot 0. */
typedef struct PROFIBUS_FSPMS_PACKET_IM_READ_IND_Ttag
{
  HIL_PACKET_HEADER_T       tHead; /*!< Standard Hilscher Header structure. */
  PROFIBUS_FSPMS_IM_INDEX_T tData; /*!< Indication data - Profibus FSPMS I&M Index structure.*/
}PROFIBUS_FSPMS_PACKET_IM_READ_IND_T;

/*! Response packet to I&M Read Indication
 * If the slot number and I&M index are correct, the complete I&M structure has to be copied in the
 * tData.un.abData field and the length has to be set in tData.ulLen of PROFIBUS_FSPMS_PACKET_IM_READ_RES.
 * If the verification is invalid tHead.ulSta has to be set to ERR_HIL_FAIL and the corresponding
 * error must be entered in tData.un.tError of PROFIBUS_FSPMS_PACKET_IM_READ_RES. The data
 * tData.tInfo.ulSAPIdx, tData.tInfo.ulSlot and tData.tInfo.ulIM_Index must not be changed.
 * The variable tData.tInfo.ulLen of indication package is not used.*/
typedef struct PROFIBUS_FSPMS_PACKET_IM_READ_RES_Ttag
{
  HIL_PACKET_HEADER_T           tHead; /*!< Standard Hilscher Header structure. */
  PROFIBUS_FSPMS_IM_READ_RES_T  tData; /*!< Response data */
}PROFIBUS_FSPMS_PACKET_IM_READ_RES_T;
/*!@}*/

/*!\defgroup pckIoCallReg Register IO-Link Call Request/Confirmation
 * To enable the IO-Link Call feature in the PROFIBUS-DP Slave stack, the command
 * PROFIBUS_FSPMS_CMD_IOL_CALL_REGISTER_REQ has to be sent to the DPS Stack. If an IO-Link Call is
 * received on the registered slot/index, the handling is completely done by the stack.
 * On a registered slot/index no other acyclic read/write is possible except for I&M calls, if set
 * up by PROFIBUS_FSPMS_CMD SET_IM0_REQ command. If a registered IO-Link Call is on an enabled I&M
 * slot/index combination, the I&M0 of that combination must set bit 0 of usIMSupported.
 *@{ */

#define PROFIBUS_FSPMS_IOL_CALL_REGISTER                    (0x00000000L) /*!< IO Link call is registered.         */
#define PROFIBUS_FSPMS_IOL_CALL_UNREGISTER                  (0x00000001L) /*!< IO Link call is unregistered.       */
#define PROFIBUS_FSPMS_IOL_CALL_UNREGISTER_ALL              (0xFFFFFFFFL) /*!< All IO Link calls are unregistered. */

/*! Register IO-Link Call Request Data structure. */
typedef struct __HIL_PACKED_PRE PROFIBUS_FSPMS_IOL_CALL_REGISTER_REQ_Ttag
{
  uint32_t ulSlot;        /*!< Slot for IO-Link call. Value range 0 to 254. */
  uint32_t ulIndex;       /*!< Index for IO-Link call. Value range 0 to 254.*/
  uint32_t ulUnregister;  /*!< It is possible to unregister a previously registered slot/index
                               combination the variable must be set:
                               to PROFIBUS_FSPMS_IOL_CALL_UNREGISTER     to unregister,
                               to PROFIBUS_FSPMS_IOL_CALL_REGISTER       to register and
                               to PROFIBUS_FSPMS_IOL_CALL_UNREGISTER_ALL to unregister all IO-Link calls.*/
}__HIL_PACKED_POST PROFIBUS_FSPMS_IOL_CALL_REGISTER_REQ_T;

/*! Register IO-Link Call Request Packet. */
typedef struct PROFIBUS_FSPMS_PACKET_IOL_CALL_REGISTER_REQ_Ttag
{
  HIL_PACKET_HEADER_T                     tHead;  /*!< Standard Hilscher Header structure. */
  PROFIBUS_FSPMS_IOL_CALL_REGISTER_REQ_T  tData;  /*!< Register IO-Link Call Request Data.*/
}PROFIBUS_FSPMS_PACKET_IOL_CALL_REGISTER_REQ_T;

/*! Register IO-Link Call Request packet data length in bytes. */
#define PROFIBUS_FSPMS_IOL_CALL_REGISTER_SIZE   (sizeof(PROFIBUS_FSPMS_IOL_CALL_REGISTER_REQ_T))

/*! Register IO-Link Call Confirmation Packet. */
typedef struct PROFIBUS_FSPMS_PACKET_IOL_CALL_REGISTER_CNF_Ttag
{
  HIL_PACKET_HEADER_T tHead; /*!< Standard Hilscher Header structure. */
}PROFIBUS_FSPMS_PACKET_IOL_CALL_REGISTER_CNF_T;
/*!@}*/

/*!\defgroup pckIoCall IO-Link Call Indication/Response
 * If on a previously registered slot/index an IO-Link call is requested, the command
 * PROFIBUS_FSPMS_CMD_IOL_CALL_IND is sent by the stack.
 * The following answers are possible of the indication:
 * - positive response with IO-Link call done,
 * - response with IO-Link error PDU,
 * - error response with IO-Link busy,
 * - other error response.
 *@{*/
/*! IO-Link Call Indication Data structure.
 * The data in tData.tAcyc must not be changed, because it contains the reference for stack to handle
 * the IO-Link call.*/
typedef struct PROFIBUS_FSPMS_IOL_CALL_IND_Ttag
{
  PBDPS_ACYCLIC_INFO_HEADER_T tAcyc;                              /*!< Acyclic header.  */
  PBDPS_CALL_HEADER_T         tCall;                              /*!< Call header.     */
  PBDPS_IOL_HEADER_T          tIOL;                               /*!< IO-Link header.  */
  uint8_t                     abIOLData[PBDPS_IOLCALL_DATA_MAX];  /*!< IO-Link data.    */
}PROFIBUS_FSPMS_IOL_CALL_IND_T;

/*! IO-Link Call Positive Response Data structure. */
typedef struct __HIL_PACKED_PRE PROFIBUS_FSPMS_IOL_CALL_RES_POS_Ttag
{
  PBDPS_ACYCLIC_INFO_HEADER_T tAcyc;                              /*!< Acyclic header.  */
  PBDPS_CALL_HEADER_T         tCall;                              /*!< Call header.     */
  PBDPS_IOL_HEADER_T          tIOL;                               /*!< IO-Link header.  */
  uint8_t                     abIOLData[PBDPS_IOLCALL_DATA_MAX];  /*!< IO-Link data.    */
}__HIL_PACKED_POST PROFIBUS_FSPMS_IOL_CALL_RES_POS_T;

/*! IO-Link Call Negative Response Data structure. */
typedef struct __HIL_PACKED_PRE PROFIBUS_FSPMS_IOL_CALL_RES_NEG_Ttag
{
  PBDPS_ACYCLIC_INFO_HEADER_T tAcyc;  /*!< Acyclic header.  */
  PBDPS_DPV1_ERROR_T          tError; /*!< Profibus DPV1 Error data.*/
}PROFIBUS_FSPMS_IOL_CALL_RES_NEG_T;

/*! IO-Link Call Indication Packet.
 * The indication contains the full data which is sent on bus.*/
typedef struct PROFIBUS_FSPMS_PACKET_IOL_CALL_IND_Ttag
{
  HIL_PACKET_HEADER_T           tHead;  /*!< Standard Hilscher Header structure. */
  PROFIBUS_FSPMS_IOL_CALL_IND_T tData;  /*!< IO-Link Call Indication Data.*/
}PROFIBUS_FSPMS_PACKET_IOL_CALL_IND_T;

/*! IO-Link Call Indication packet data length in bytes without bytes to write. */
#define PROFIBUS_FSPMS_IOL_CALL_IND_SIZE                    (sizeof(PROFIBUS_FSPMS_IOL_CALL_IND_T) - \
                                                             PBDPS_IOLCALL_DATA_MAX)
/*! IO-Link Call Positive Response Packet.
 * - If the IO-Link call request is successfully executed the control byte in tData.tIOL.bControl must
 * be set to zero to indicate Done/Transfer terminated. On a read response the requested data should
 * be filled in tData.abIOLData[] and the tHead.ulLen to the positive response length plus requested
 * data.
 * - If the IO-Link call request results in an error the control byte in tData.tIOL.bControl must be
 * set to 128 to indicate an IO-Link error PDU which is filled in tData.abIOLData[]. The tHead.ulLen
 * must be set to the positive response length plus error PDU data length. */
typedef struct PROFIBUS_FSPMS_PACKET_IOL_CALL_RES_POS_Ttag
{
  HIL_PACKET_HEADER_T               tHead; /*!< Standard Hilscher Header structure. */
  PROFIBUS_FSPMS_IOL_CALL_RES_POS_T tData; /*!< IO-Link Call Positive Response Data.*/
}PROFIBUS_FSPMS_PACKET_IOL_CALL_RES_POS_T;

/*! IO-Link Call Positive Response packet data length in bytes without bytes read / IO-link error PDU. */
#define PROFIBUS_FSPMS_IOL_CALL_RES_POS_SIZE                (sizeof(PBDPS_ACYCLIC_INFO_HEADER_T) + \
                                                             sizeof(PBDPS_CALL_HEADER_T)         + \
                                                             sizeof(PBDPS_IOL_HEADER_T))

/*! IO-Link Call Negative Response Packet.
 * - If the IO-Link call request needs more time, then an error response with IO-Link busy must be sent.
 * The error codes to be applied are:
 * tData.tError.bErrorDecode = PBDPS_ERROR_DECODE_DPV1 (0x80)
 * tData.tError.bErrorCode1  = PBDPS_ERROR_CLASS_RESOURCE | PBDPS_ERROR_CODE_APPL_MODULE (0xC2)
 * tData.tError.bErrorCode2  = 0x00
 * The length is set to negative response length and the status to ERR_HIL_FAIL. On the next update
 * request from the PROFIBUS-DP master, the complete IO-Link call indication to the application is repeated.
 *
 * - If the IO-Link call request is wrong on application/access/resource level the appropriate error
 * codes have to be used. The length is set to negative response length and the status to ERR_HIL_FAIL.*/
typedef struct PROFIBUS_FSPMS_PACKET_IOL_CALL_RES_NEG_Ttag
{
  HIL_PACKET_HEADER_T               tHead; /*!< Standard Hilscher Header structure. */
  PROFIBUS_FSPMS_IOL_CALL_RES_NEG_T tData; /*!< IO-Link Call Negative Response Data.*/
}PROFIBUS_FSPMS_PACKET_IOL_CALL_RES_NEG_T;

/*! IO-Link Call Negative packet data length in bytes.*/
#define PROFIBUS_FSPMS_IOL_CALL_RES_NEG_SIZE                (sizeof(PBDPS_ACYCLIC_INFO_HEADER_T) + \
                                                             sizeof(PBDPS_DPV1_ERROR_T))
/*!@}*/
/* pragma unpack */
#ifdef PRAGMA_PACK_ENABLE
#pragma PRAGMA_UNPACK_1(PROFIBUS_FSPMS_PUBLIC)
#endif

typedef union PROFIBUS_FSPMS_PACKETtag                      PROFIBUS_FSPMS_PACKET_T;

union PROFIBUS_FSPMS_PACKETtag
{
  HIL_PACKET_HEADER_T                                   tHead;              /* Hilscher header */

  PROFIBUS_FSPMS_PACKET_SET_STAT_DIAG_REQ_T             tSetStatDiagReq;    /* Set the Static diagnostic Request */
  PROFIBUS_FSPMS_PACKET_SET_STAT_DIAG_CNF_T             tSetStatDiagCnf;    /* Set the Static diagnostic Confirmation */

  PROFIBUS_FSPMS_PACKET_SET_CFG_REQ_T                   tSetCfgDataReq;     /* Set the Slave's configuration Request */
  PROFIBUS_FSPMS_PACKET_SET_CFG_CNF_T                   tSetCfgDataCnf;     /* Set the Slave's configuration Confirmation */

  PROFIBUS_FSPMS_PACKET_ABORT_REQ_T                     tAbtReq;            /* Abort the Master-Slave connection Request */
  PROFIBUS_FSPMS_PACKET_ABORT_CNF_T                     tAbtCnf;            /* Abort the Master-Slave connection Confirmation */

  PROFIBUS_FSPMS_PACKET_DP_SLAVE_APPLICATION_READY_REQ_T tAppRdyReq;        /* Set the Application to ready Request */
  PROFIBUS_FSPMS_PACKET_DP_SLAVE_APPLICATION_READY_CNF_T tAppRdyCnf;        /* Set the Application to ready Confirmation */

  PROFIBUS_FSPMS_PACKET_C1_READ_IND_T                   tReadInd;           /* Indication read command */
  PROFIBUS_FSPMS_PACKET_C1_READ_RES_POS_T               tReadResPos;        /* Positive Read response */
  PROFIBUS_FSPMS_PACKET_C1_READ_RES_NEG_T               tReadResNeg;        /* Negative Read response */

  PROFIBUS_FSPMS_PACKET_C1_WRITE_IND_T                  tWriteInd;          /* Indication write command */
  PROFIBUS_FSPMS_PACKET_C1_WRITE_RES_POS_T              tWriteResPos;       /* Positive Write response */
  PROFIBUS_FSPMS_PACKET_C1_WRITE_RES_NEG_T              tWriteResNeg;       /* Negative Write response */

  PROFIBUS_FSPMS_PACKET_C1_ALARM_ACK_RES_POS_T          tAlarmAckResPos;    /* Positive Alarm acknowledging response */
  PROFIBUS_FSPMS_PACKET_C1_ALARM_ACK_RES_NEG_T          tAlarmAckResNeg;    /* Negative Alarm acknowledging response */
  PROFIBUS_FSPMS_PACKET_C1_ALARM_NOTIFICATION_REQ_T     tAlarmNotifyReq;    /* Alarm Notification Request */
  PROFIBUS_FSPMS_PACKET_C1_ALARM_NOTIFICATION_CNF_T     tAlarmNotifyCnf;    /* Alarm Notification Confirmation */

  PROFIBUS_FSPMS_PACKET_C2_INITIATE_IND_T               tC2InitiateInd;
  PROFIBUS_FSPMS_PACKET_C2_INITIATE_RES_POS_T           tC2InitiateResPos;
  PROFIBUS_FSPMS_PACKET_C2_INITIATE_RES_NEG_T           tC2InitiateResNeg;

  PROFIBUS_FSPMS_PACKET_C2_READ_IND_T                   tC2ReadInd;
  PROFIBUS_FSPMS_PACKET_C2_READ_RES_POS_T               tC2ReadResPos;
  PROFIBUS_FSPMS_PACKET_C2_READ_RES_NEG_T               tC2ReadResNeg;

  PROFIBUS_FSPMS_PACKET_C2_WRITE_IND_T                  tC2WriteInd;
  PROFIBUS_FSPMS_PACKET_C2_WRITE_RES_POS_T              tC2WriteResPos;
  PROFIBUS_FSPMS_PACKET_C2_WRITE_RES_NEG_T              tC2WriteResNeg;

  PROFIBUS_FSPMS_PACKET_C2_DATA_TRANSPORT_IND_T         tC2DataTransportInd;
  PROFIBUS_FSPMS_PACKET_C2_DATA_TRANSPORT_RES_POS_T     tC2DataTransportResPos;
  PROFIBUS_FSPMS_PACKET_C2_DATA_TRANSPORT_RES_NEG_T     tC2DataTransportResNeg;

  PROFIBUS_FSPMS_PACKET_C2_ABORT_IND_T                  tC2AbortInd;
  PROFIBUS_FSPMS_PACKET_C2_ABORT_RES_T                  tC2AbortRes;

  PROFIBUS_FSPMS_PACKET_CHECK_CFG_IND_T                 tChkCfgInd;         /* Indicate new Configuration Data */
  PROFIBUS_FSPMS_PACKET_CHECK_USER_PRM_IND_T            tChkUsrPrmInd;      /* Indicate new User Parameter Data */
  PROFIBUS_FSPMS_PACKET_CHECK_EXT_USER_PRM_IND_T        tChkExtUsrPrmInd;   /* Indicate new extended User Parameter Data */
  PROFIBUS_FSPMS_PACKET_CHECK_CFG_RES_T                 tChkCfgRes;         /* Indicate new Configuration Data */
  PROFIBUS_FSPMS_PACKET_CHECK_USER_PRM_RES_T            tChkUsrPrmRes;      /* Indicate new User Parameter Data */
  PROFIBUS_FSPMS_PACKET_CHECK_EXT_USER_PRM_RES_T        tChkExtUsrPrmRes;   /* Indicate new extended User Parameter Data */

  PROFIBUS_FSPMS_PACKET_SET_SLAVE_ADD_IND_T             tSetSlaveAddInd;    /* Indicate a Set Slave Address */
  PROFIBUS_FSPMS_PACKET_GLOBAL_CONTROL_IND_T            tGlobalControlInd;  /* Indicate a Global Control reception */
  PROFIBUS_FSPMS_PACKET_C1_ALARM_ACK_IND_T              tAlarmAckInd;       /* Indicate an Alarm Acknowledgement */

  PROFIBUS_FSPMS_PACKET_STATE_CHANGED_IND_T             tStateChangedInd;   /* Changed operation state */

  PROFIBUS_FSPMS_PACKET_REGISTER_DIAG_STRUCT_REQ_T      tRegDiagStructReq;  /* Register the diagnostic area */
  PROFIBUS_FSPMS_PACKET_REGISTER_DIAG_STRUCT_CNF_T      tRegDiagStructCnf;

  PROFIBUS_FSPMS_PACKET_CMD_RESET_REQ_T                 tResetReq;          /* Reset the DPS Stack */
  PROFIBUS_FSPMS_PACKET_CMD_RESET_CNF_T                 tResetCnf;

  PROFIBUS_FSPMS_PACKET_SET_IM0_REQ_T                   tSetIM0Req;         /* Set I&M 0 Object */
  PROFIBUS_FSPMS_PACKET_SET_IM0_CNF_T                   tSetIM0Cnf;

  PROFIBUS_FSPMS_PACKET_IM_READ_IND_T                   tIMReadInd;         /* I&M Read indication */
  PROFIBUS_FSPMS_PACKET_IM_READ_RES_T                   tIMReadRes;         /* I&M Read response */

  PROFIBUS_FSPMS_PACKET_IM_WRITE_IND_T                  tIMWriteInd;        /* I&M Write indication */
  PROFIBUS_FSPMS_PACKET_IM_WRITE_RES_T                  tIMWriteRes;        /* I&M Write response */

  PROFIBUS_FSPMS_PACKET_IOL_CALL_REGISTER_REQ_T         tIOLCallRegisterReq;  /* IO-Link Call register request */
  PROFIBUS_FSPMS_PACKET_IOL_CALL_REGISTER_CNF_T         tIOLCallRegisterCnf;  /* IO-Link Call register confirmation */

  PROFIBUS_FSPMS_PACKET_IOL_CALL_IND_T                  tIOLCallInd;        /* IO-Link Call indication */
  PROFIBUS_FSPMS_PACKET_IOL_CALL_RES_POS_T              tIOLCallResPos;     /* IO-Link Call response positive */
  PROFIBUS_FSPMS_PACKET_IOL_CALL_RES_NEG_T              tIOLCallResNeg;     /* IO-Link Call response negative */

  PROFIBUS_FSPMS_PACKET_GET_TASK_DIAG_REQ_T             tGetTaskDiagReq;    /* Read Task Diagnostic request       */
  PROFIBUS_FSPMS_PACKET_GET_TASK_DIAG_CNF_T             tGetTaskDiagCnf;    /* Read Task Diagnostic confirmation  */

  PROFIBUS_FSPMS_PACKET_GET_TIME_REQ_T                  tGetTimeReq;
  PROFIBUS_FSPMS_PACKET_GET_TIME_CNF_T                  tGetTimeCnf;
};
/*!@}*/


/***************************************************************************************/
#endif /* #ifndef __PBDPS_PACKET_H */
