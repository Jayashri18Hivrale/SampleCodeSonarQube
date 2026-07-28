/**************************************************************************************
Copyright (c) Hilscher Gesellschaft fuer Systemautomation mbH. All Rights Reserved.
***************************************************************************************
$Id: PBDPS_Defines.h 89663 2020-02-07 16:32:54Z TGrigorov $

Description:
  This file contains definition and data types of the Profibus DP Slave

Changes:
  Date          Description
  -----------------------------------------------------------------------------------
  2015-03-02    created
**************************************************************************************/
/*!\file PBDPS_Defines.h */
#ifndef _PBDPS_DATASTRUCT_H_
#define _PBDPS_DATASTRUCT_H_
#include "Hil_Compiler.h"
/****************************************************************************************
 * Profibus DP Slave, constants
 * !!! add additional public constants
 */
/*!\name Application Reference End Point Types for the Abort service
 * @{*/
#define PBDPS_AREP_TYPE_MS0                         0x00000000  /*!< AREP Types for MS0 Abort service. */
#define PBDPS_AREP_TYPE_MS2                         0x00020000  /*!< AREP Types for MS2 Abort service. */
/*!@}*/

#define PBDPS_SLAVEDIAG_MAX                         (238)   /*!< Maximum supported diagnostic data size, exlusive 6 standard bytes */
#define PBDPS_MODULE_CONFIG_MAX                     (244)   /*!< Maximum supported configuration size  */
#define PBDPS_USER_PRM_MAX                          (237)   /*!< Maximum supported user parameter data size               */
#define PBDPS_EXT_USER_PRM_MAX                      (244)   /*!< Maximum supported extended user parameter data size      */
#define PBDPS_SET_SLAVE_ADD_MAX                     (240)   /*!< Maximum supported set slave address permanent user data.  */

#define PBDPS_INPUT_DATA_MAX                        (244)   /* Maximum supported input data length    */
#define PBDPS_OUTPUT_DATA_MAX                       (244)   /* Maximum supported output data length   */
#define PBDPS_OUTPUT_STATUS_SIZE                    (4)     /* Maximum supported output status length */
#define PBDPS_MODULE_STATE_SIZE                     (31)

#define PBDPS_IOSEGMENT_MAX                         (80)    /* Maximum number of IO Segments.
                                                            For each discontiguous I/O 1 segment is needed */

#define PBDPS_SUBSCRIBER_MAX                        (8)
/* #warning define PBDPS_SUBSCRIBER_MAX as 8 - todo: use HAL define DXB_LINK_NUM
 * the define DXB_LINK_NUM is defined only for HAL Slave */

#define PBDPS_MODUL_STATUS_OFFSET                   (PBDPS_OUTPUT_DATA_MAX)   /* Module status is located behind output data */
#define PBDPS_MODUL_STATUS_MAX                      (31)   /* 244  byte bitfield (1 Bit for each module)  */

#define PBDPS_CLASS1_ACYCLIC_MAX                    (244)  /*!< Maximum supported acyclic data length         */
#define PBDPS_CLASS2_ACYCLIC_MAX                    (240)  /*!< Maximum supported acyclic class2 data length  */

#define PBDPS_READ_MAX                              (240)  /*!< Maximum supported acyclic read data length       */
#define PBDPS_WRITE_MAX                             (240)  /*!< Maximum supported acyclic write data length      */
#define PBDPS_DATATRANSPORT_MAX                     (240)  /*!< Maximum supported acyclic transport data length  */

#define PBDPS_INITIATE_ADD_TABLE_MAX                (234)  /*!< Maximum supported acyclic initiate address data length */

#define PBDPS_ALARM_USERDATA_MAX                    (59)   /*!< Maximum allowed user specific Alarm data */
#define PBDPS_DIAGNOSIS_DEVICE_BLOCK_DATA_MAX       (63)
#define PBDPS_DIAGNOSIS_IDENTIFIER_DATA_MAX         (32)

#define PBDPS_SLOT_NOT_ZERO_MAX                     (255) /*!< Maximum number of not zero slots.  */

#define PBDPS_ALARM_ENABLED_PULL_PLUG               (0x80)
#define PBDPS_ALARM_ENABLED_PROCESS                 (0x40)
#define PBDPS_ALARM_ENABLED_DIAGNOSTIC              (0x20)
#define PBDPS_ALARM_ENABLED_MANUFACTURER_SPECIFIC   (0x10)
#define PBDPS_ALARM_ENABLED_STATUS                  (0x08)
#define PBDPS_ALARM_ENABLED_UPDATE                  (0x04)

/*!\defgroup glbSync Global Control Sync Operation Types
 * Different types of Global Control Sync Operation PROFIBUS_FSPMS_GLOBAL_CONTROL_IND_T.bSyncCommand
 * @{*/
/*! No synchronization action is initiated.
 *  Also available for ROFIBUS_FSPMS_GLOBAL_CONTROL_IND_T.bFreezeCommand .*/
#define PBDPS_GLOBALCONTROL_NOACTION                (0)
#define PBDPS_GLOBALCONTROL_SYNC                    (1) /*!< Outputs has been synchronized once. */
#define PBDPS_GLOBALCONTROL_UNSYNC                  (2) /*!< Outputs are not synchronized. */
/*!@}*/

/*!\defgroup glbFreeze Global Control Freeze Operation Types
 * Different types of Global Control Sync Operation PROFIBUS_FSPMS_GLOBAL_CONTROL_IND_T.bFreezeCommand
 * @{*/
#define PBDPS_GLOBALCONTROL_FREEZE                  (1) /*!< Inputs are frozen. */
#define PBDPS_GLOBALCONTROL_UNFREEZE                (2) /*!< Inputs are no longer frozen. */
/*!@}*/

/*! Error Decode DPV1.
 * Declares the error to be a user specific error.*/
#define PBDPS_ERROR_DECODE_DPV1                     (128)

/*!\name Error Class
 * @{*/
#define PBDPS_ERROR_CLASS_APPLICATION               (0xA0)  /*!< Application error class */
#define PBDPS_ERROR_CLASS_ACCESS                    (0xB0)  /*!< Access error class      */
#define PBDPS_ERROR_CLASS_RESOURCE                  (0xC0)  /*!< Resource error class    */
/*!@}*/

/*!\name Error Codes for Error Class Application
 * @{*/
#define PBDPS_ERROR_CODE_APPL_READ                  (0x00)  /*!< Read error             */
#define PBDPS_ERROR_CODE_APPL_WRITE                 (0x01)  /*!< Write error            */
#define PBDPS_ERROR_CODE_APPL_MODULE                (0x02)  /*!< Module failure         */
#define PBDPS_ERROR_CODE_APPL_VERSION               (0x08)  /*!< Version conflict       */
#define PBDPS_ERROR_CODE_APPL_NOT_SUPPORTED         (0x09)  /*!< Feature not supported  */
/*!@}*/

/*!\name Error Codes for Error Class Access
 *@{*/
#define PBDPS_ERROR_CODE_ACC_INDEX                  (0x00)  /*!< Invalid index      */
#define PBDPS_ERROR_CODE_ACC_WRITE_LENGTH           (0x01)  /*!< Write length error */
#define PBDPS_ERROR_CODE_ACC_SLOT_INVALID           (0x02)  /*!< Invalid slot       */
#define PBDPS_ERROR_CODE_ACC_TYPE_CONFLICT          (0x03)  /*!< Type conflict      */
#define PBDPS_ERROR_CODE_ACC_AREA_INVALID           (0x04)  /*!< Invalid area       */
#define PBDPS_ERROR_CODE_ACC_STATE_CONFLICT         (0x05)  /*!< State conflict     */
#define PBDPS_ERROR_CODE_ACC_ACCESS_DENIED          (0x06)  /*!< Access denied      */
#define PBDPS_ERROR_CODE_ACC_RANGE_INVALID          (0x07)  /*!< Invalid range      */
#define PBDPS_ERROR_CODE_ACC_PARAMETER_INVALID      (0x08)  /*!< Invalid parameter  */
#define PBDPS_ERROR_CODE_ACC_TYPE_INVALID           (0x09)  /*!< Invalid type       */
/*!@}*/

/*!\name Error Codes for Error Class Resources
 *@{*/
#define PBDPS_ERROR_CODE_RSC_READ_CONSTRAIN         (0x00)  /*!< Read constraint conflict   */
#define PBDPS_ERROR_CODE_RSC_WRITE_CONSTRAIN        (0x01)  /*!< Write constraint conflict  */
#define PBDPS_ERROR_CODE_RSC_BUSY                   (0x02)  /*!< Resource busy              */
#define PBDPS_ERROR_CODE_RSC_UNAVAILABLE            (0x03)  /*!< Resource unavailable       */
/*!@}*/

/*!\name Class 2 Connection Subnet Codding
 *@{*/
#define PBDPS_SUBNET_NO                             (0x00)  /*!< No specific source.  */
#define PBDPS_SUBNET_LOCAL                          (0x01)  /*!< Local source.        */
#define PBDPS_SUBNET_REMOTE                         (0x02)  /*!< Remote source.       */
/*!@}*/

/*!\name Class 2 Connection Instance Codes
 *@{ */
#define PBDPS_INSTANCE_DLL                          (0x00)  /*!< FDL\DLL */
#define PBDPS_INSTANCE_MSAC2                        (0x10)  /*!< MSAC_C2 */
#define PBDPS_INSTANCE_USER                         (0x20)  /*!< User */
/*@}*/

/*!\name Possible Reason Codes caused by DDLM/MSAC_C2.
 * @{*/
#define PBDPS_REASON_CODE_SE                        (0x01)  /*!< Sequence Error                   */
#define PBDPS_REASON_CODE_FE                        (0x02)  /*!< Invalid request PDU received     */
#define PBDPS_REASON_CODE_TO                        (0x03)  /*!< Timeout of the connection        */
#define PBDPS_REASON_CODE_RE                        (0x04)  /*!< Invalid response PDU received    */
#define PBDPS_REASON_CODE_IA                        (0x07)  /*!< Additional address information is not valid  */
/*!@}*/


/*! Profibus DP Slave Baud Rate */
typedef enum PBDPS_DATA_RATEtag
{
  PBDPS_DATA_RATE_96                                =  0, /*!< Baud rate 9.6 kBit/s   */
  PBDPS_DATA_RATE_19_2                              =  1, /*!< Baud rate 19.2 kBit/s  */
  PBDPS_DATA_RATE_93_75                             =  2, /*!< Baud rate 93.75 kBit/s */
  PBDPS_DATA_RATE_187_5                             =  3, /*!< Baud rate 187.5 kBit/s */
  PBDPS_DATA_RATE_500                               =  4, /*!< Baud rate 500 kBit/s   */
  PBDPS_DATA_RATE_1500                              =  6, /*!< Baud rate 1.5 MBit/s   */
  PBDPS_DATA_RATE_3000                              =  7, /*!< Baud rate 3 MBit/s     */
  PBDPS_DATA_RATE_6000                              =  8, /*!< Baud rate 6 MBit/s     */
  PBDPS_DATA_RATE_12000                             =  9, /*!< Baud rate 12 MBit/s    */
  PBDPS_DATA_RATE_31_25                             = 10, /*!< Baud rate 31.25 kBit/s */
  PBDPS_DATA_RATE_45_45                             = 11, /*!< Baud rate 45.45 kBit/s */
  PBDPS_DATA_RATE_AUTO                              = 15  /*!< Baud rate Auto detect  */
} PBDPS_DATA_RATE;

#define PBDPS_IOSTATUS_CLEARCMD                     (0x01)  /* Received clear from Global Control */
#define PBDPS_IOSTATUS_CLEAR                        (0x02)  /* Output data cleared by stack
                                                               Set based on error or empty data exchange (fail safe) */
#define PBDPS_IOSTATUS_SAFESTATE                    (0x04)  /* No valid values available */
#define PBDPS_IOSTATUS_LENGTH                       (0x08)  /* Invalid data length received */

/*! PROFIBUS DP Slave Diagnostic ID numbers.
 * Diagnostic information about the module configuration, parameter data and internal information is
 * available*/
typedef enum PBDPS_TASK_DIAGtag
{
  PBDPS_TASK_DIAG_EXTENDED_DIAG                     = 0x00000401, /*!< Extended diagnostics.      */
  PBDPS_TASK_DIAG_MODULECFG                         = 0x00000402, /*!< Module configuration.      */
  PBDPS_TASK_DIAG_MODULECFG_REQ                     = 0x00000403, /*!< Requested module configuration.*/
  PBDPS_TASK_DIAG_PRMDATA                           = 0x00000404, /*!< Parameter data.            */
  PBDPS_TASK_DIAG_RESERVED1                         = 0x00000405, /*!< Reserved.                  */
  PBDPS_TASK_DIAG_RESERVED2                         = 0x00000406, /*!< Reserved.                  */
  PBDPS_TASK_DIAG_SLAVEDIAGNOSTIC                   = 0x00000407, /*!< Slave diagnostic.          */
  PBDPS_TASK_DIAG_CYCLEDATA                         = 0x00000408, /*!< Cycle data diagnostic.     */
}PBDPS_TASK_DIAG;


typedef struct PBDPS_NETWORK_TIME_Ttag
{
  uint32_t ulSeconds;
  uint32_t ulSecondsFraction; /* Unit: 1/(2^32)s */
}PBDPS_NETWORK_TIME_T;

/*! Profibus DPV1 Error Structure */
typedef struct PBDPS_DPV1_ERROR_Ttag
{
  /*! Error decode value classifying the error.
   * - 128 : indicates DP V1 error, the error to be a user specific
   * - 254, 255 : indicate profile-specific error, in this case error code 1 and 2 are also profile-specific*/
  uint8_t bErrorDecode;

  /*! Detailed error code
   * The variable includes information for Error Class and Error Code. Error Class is high nibble
   * and Error Code is low nibble. Error Class:
   * - 0 to 9 : Reserved
   * - 10 : Application
   * - 11 : Access
   * - 12 : Resources
   * - 13 to 15 : User specific*/
  uint8_t bErrorCode1;

  /*! User specific error code
   * The variable is fully user specific and may contain any value. Its handling is
   * transparent and reported to the requesting DP-Master without any change. */
  uint8_t bErrorCode2;
}PBDPS_DPV1_ERROR_T;

/*! IO-Link Acyclic Info Header. */
typedef struct PBDPS_ACYCLIC_INFO_HEADER_Ttag
{
  uint8_t bSAPIdx;  /*!< SAP Index              */
  uint8_t bSlot;    /*!< Slot of IO-Link call.  Value range: 0 - 254 */
  uint8_t bIndex;   /*!< Index of IO-Link call. Value range: 0 - 254 */
  uint8_t bRes;     /*!< Reserved.              */
} PBDPS_ACYCLIC_INFO_HEADER_T;

/*! IO-Link Call Header. */
typedef struct __HIL_PACKED_PRE PBDPS_CALL_HEADER_Ttag
{
  #define PBDPS_PARAMETER_COMMAND_PRIMARY           (0x02)  /*!< Parameter command. */
  #define PBDPS_PARAMETER_COMMAND_START_MS1         (0x04)  /*!< Parameter command. */
  #define PBDPS_PARAMETER_COMMAND_STOP_MS1          (0x08)  /*!< Parameter command. */
  #define PBDPS_PARAMETER_COMMAND_CHECK_PROPERTIES  (0x10)  /*!< Parameter command. */
  #define PBDPS_PARAMETER_COMMAND_MASTERSTATECLEAR  (0x40)  /*!< Parameter command. */

  uint8_t  bFunction;   /*!< Extended function number - CALL */
  uint8_t  bEntityPort; /*!< IO-Link Master Port number. Value range: 0 - 63 */
  uint16_t usFIIndex;   /*!< function invocation index - IO-Link call */
} __HIL_PACKED_POST PBDPS_CALL_HEADER_T;

/*! IO-Link Header. */
typedef struct __HIL_PACKED_PRE PBDPS_IOL_HEADER_Ttag
{
  /*! Control: Value range 0 -3, 128. 0: Cancel/Release; 1: Idle; 2: Write; 3: Read; 128: IOL Error PDU. */
  uint8_t  bControl;

  /*! IO-Link Index . Value range: 0 - 32767 - IO-Link Device Data Index, 65535 - Port function invocation.*/
  uint16_t usIOLIndex;

  /*! IO-Link Device Data subindex or port function, Value range 0 - 255. */
  uint8_t  bIOLSubindex;
} __HIL_PACKED_POST PBDPS_IOL_HEADER_T;

/*! Profibus DP Slave Identification & Maintenance 0 Data.*/
typedef struct PBDPS_IM0_Ttag
{
  /*! Manufacturer of Device.<br>Default Value for I&M0:" " (10 blanks)*/
  uint8_t     abManufactor[10];

  /*! Manufacturer ID.<br>Default Value for I&M0: 0x011E
   * (indicating"HilscherGesellschaft fürSystemautomationmbH" )*/
  uint16_t    usManufactorId;

  uint8_t     abOrder_Id[20];     /*!< Order ID.<br>Default Value for I&M0:" " (20 blanks)*/
  uint8_t     abSerialNumber[16]; /*!< Serial Number.<br>Default Value for I&M0:"1 " (15 blanks)*/
  uint16_t    usHwRevision;       /*!< Hardware Revision.<br>Default Value for I&M0: 1*/

  /*! Software Revision.<br>First byte interpreted as ASCII character, valid are :
   * - 'V' released version
   * - 'R' revision
   * - 'P' prototype
   * - 'U' under test
   * - 'T' test device
   * The 3 remaining bytes are interpreted as unsigned integer.<br>Default Value for I&M0: Default
   * value is always current stack version.*/
  uint8_t     abSwRevision[4];

  /*! Revision Counter.<br>Initial value 0, increments on every set of I&M data, must not be 0 after
   * overrun. <br>Default Value for I&M0: 0 (= Init State)*/
  uint16_t    usRevisionCounter;

  uint16_t    usProfileId;  /*!< Profile ID.<br>Default Value for I&M0: 0x0000 (= Non-Profile Device)*/
  uint16_t    usProfileSpecType; /*!< Profile Specific Type.<br>Default Value for I&M0:0x0004(= Communication Module)*/
  uint16_t    usIMVersion;  /*!< I&M Version.<br>Default Value for I&M0: 0x0102.*/

  /*! I&M supported.<br>The variable usIMSupported is interpreted as bit array which represents the
   * supported I&M records:
   * - bit 0: Profile Specific I&M
   * - bit 1: I&M1
   * - bit 2: I&M2
   * - ...
   * - bit 15: I&M15 <p>
   * The bit 0 is used for signaling that profile specific I&M calls are supported and must be set
   * corresponding to variable PBDPS_IM_DATA_T.fProfSpecSupp.
   * Default Value for I&M0: 0x0000.*/
  uint16_t    usIMSupported;
} PBDPS_IM0_T;

/*! Profibus DP Slave Identification & Maintenance Data. */
typedef struct PBDPS_IM_DATA_Ttag
{
  PBDPS_IM0_T tIM0;   /*!< Profibus DP Slave I&M0 data structure.*/

  /*!Profile Specific Support.<br>Indicates, whether Profile Specific I&Ms are supported.*/
  uint32_t fProfSpecSupp;

  /*! Manufacturer Specific Support.<br> Indicates, whether Manufacturer Specific I&Ms are supported.*/
  uint32_t  fManuSpecSupp;

  /*! Slots No Zero Support.<br>Indicates, whether there are slots other than zero supported. If slots
   * different from zero should be supported the variable must be set to the corresponding value. The
   * stack supports the slots from 0 up to ulSlotsNotZeroSupp.*/
  uint32_t  ulSlotsNotZeroSupp;
} PBDPS_IM_DATA_T;

/*! IO Link Maximum data size in bytes. */
#define PBDPS_IOLCALL_DATA_MAX                      (PBDPS_CLASS1_ACYCLIC_MAX    - \
                                                     sizeof(PBDPS_CALL_HEADER_T) - \
                                                     sizeof(PBDPS_IOL_HEADER_T))

typedef __HIL_PACKED_PRE struct PBDPS_STRUCTURED_PARAMETER_HEADER_Ttag
{
  uint8_t     bLength;

  #define PBDPS_STRUCTURED_PARAMETER_TYPE_PRM_CMD              (0x02)
  #define PBDPS_STRUCTURED_PARAMETER_TYPE_DXB_LINKTABLE        (0x03)
  #define PBDPS_STRUCTURED_PARAMETER_TYPE_ISOM_PARAMETER       (0x04)
  #define PBDPS_STRUCTURED_PARAMETER_TYPE_F_PARAMETER          (0x05)
  #define PBDPS_STRUCTURED_PARAMETER_TYPE_DXB_SUBSCRIBERTABLE  (0x07)
  #define PBDPS_STRUCTURED_PARAMETER_TYPE_TIME_AR_PARAMETER    (0x08)
  #define PBDPS_STRUCTURED_PARAMETER_TYPE_USER_PARAMETER_DATA  (0x81)

  uint8_t     bType;

  uint8_t     bSlotNo;
  uint8_t     bSequenceNo;
}__HIL_PACKED_POST PBDPS_STRUCTURED_PARAMETER_HEADER_T;

typedef __HIL_PACKED_PRE struct PBDPS_STRUCTURED_PARAMETER_USER_Ttag
{
  PBDPS_STRUCTURED_PARAMETER_HEADER_T tHeader;
  uint8_t abUserData[1];
} __HIL_PACKED_POST PBDPS_STRUCTURED_PARAMETER_USER_T;

typedef __HIL_PACKED_PRE struct PBDPS_STRUCTURED_PARAMETER_COMMAND_Ttag
{
  PBDPS_STRUCTURED_PARAMETER_HEADER_T tHeader;
  uint8_t  bFunction;
  uint8_t  bProperties;
  uint16_t usOutputHoldTime;
}__HIL_PACKED_POST PBDPS_STRUCTURED_PARAMETER_COMMAND_T;

typedef __HIL_PACKED_PRE struct PBDPS_STRUCTURED_PARAMETER_ISOM_Ttag
{
  PBDPS_STRUCTURED_PARAMETER_HEADER_T tHeader;
  uint8_t  bVersion;
  uint32_t ulTBase_Dp;
  uint16_t usTDp;
  uint8_t  bTMapc;
  uint32_t ulTBase_Io;
  uint16_t usTI;
  uint16_t usTO;
  uint32_t ulTDx;
  uint16_t usTPll_W;
  uint16_t usTPll_D;
}__HIL_PACKED_POST PBDPS_STRUCTURED_PARAMETER_ISOM_T;

typedef __HIL_PACKED_PRE struct PBDPS_STRUCTURED_PARAMETER_DXB_LINKTABLE_ENTRY_Ttag
{
  uint8_t bPublisher_Addr;
  uint8_t bPublisher_Length;
  uint8_t bSample_Offset;
  uint8_t bSample_Length;
}__HIL_PACKED_POST PBDPS_STRUCTURED_PARAMETER_DXB_LINKTABLE_ENTRY_T;

typedef __HIL_PACKED_PRE struct PBDPS_STRUCTURED_PARAMETER_DXB_LINKTABLE_Ttag
{
  PBDPS_STRUCTURED_PARAMETER_HEADER_T tHeader;
  uint8_t bVersion;
  PBDPS_STRUCTURED_PARAMETER_DXB_LINKTABLE_ENTRY_T atEntry[1];
}__HIL_PACKED_POST PBDPS_STRUCTURED_PARAMETER_DXB_LINKTABLE_T;

typedef __HIL_PACKED_PRE struct PBDPS_STRUCTURED_PARAMETER_DXB_SUBSCRIBERTABLE_ENTRY_Ttag
{
  uint8_t bPublisher_Addr;
  uint8_t bPublisher_Length;
  uint8_t bSample_Offset;
  uint8_t bDest_Slot_Number;
  uint8_t bOffset_Data_Area;
  uint8_t bSample_Length;
}__HIL_PACKED_POST PBDPS_STRUCTURED_PARAMETER_DXB_SUBSCRIBERTABLE_ENTRY_T;

typedef __HIL_PACKED_PRE struct PBDPS_STRUCTURED_PARAMETER_DXB_SUBSCRIBERTABLE_Ttag
{
  PBDPS_STRUCTURED_PARAMETER_HEADER_T tHeader;
  uint8_t bVersion;
  PBDPS_STRUCTURED_PARAMETER_DXB_SUBSCRIBERTABLE_ENTRY_T atEntry[1];
}__HIL_PACKED_POST PBDPS_STRUCTURED_PARAMETER_DXB_SUBSCRIBERTABLE_T;

typedef __HIL_PACKED_PRE struct PBDPS_STRUCTURED_PARAMETER_TIME_AR_PARAMTER_Ttag
{
  PBDPS_STRUCTURED_PARAMETER_HEADER_T tHeader;
  uint16_t usClockSyncInterval;
  PBDPS_NETWORK_TIME_T tCSDelayTime;
}__HIL_PACKED_POST PBDPS_STRUCTURED_PARAMETER_TIME_AR_PARAMTER_T;

typedef __HIL_PACKED_PRE union PBDPS_STRUCTURED_PRM_DATA_Ttag
{
  PBDPS_STRUCTURED_PARAMETER_HEADER_T               tHeader;

  /* Structure Type     0,1: reserved             */

  /* Structure Type       2: PrmCmd               */
  PBDPS_STRUCTURED_PARAMETER_COMMAND_T              tPrmCmd;

  /* Structure Type       3: DXB Linktable        */
  PBDPS_STRUCTURED_PARAMETER_DXB_LINKTABLE_T        tDXBLinktable;

  /* Structure Type       4: IsoM Parameter       */
  PBDPS_STRUCTURED_PARAMETER_ISOM_T                 tIsoMParameter;

  /* Structure Type       5: F-Parameter          */
  PBDPS_STRUCTURED_PARAMETER_USER_T                 tFParameter;

  /* Structure Type       6: reserved             */

  /* Structure Type       7: DXB Subscribertable  */
  PBDPS_STRUCTURED_PARAMETER_DXB_SUBSCRIBERTABLE_T  tDXBSubscribertable;

  /* Structure Type       8: Time AR Parameter    */
  PBDPS_STRUCTURED_PARAMETER_TIME_AR_PARAMTER_T     tTimeARParameter;

  /* Structure Type    9-31: reserved             */

  /* Structure Type  32-128: Manufacture Specific */
  PBDPS_STRUCTURED_PARAMETER_USER_T                 tManufactureSpecific;

  /* Structure Type     129: User Prm Data        */
  PBDPS_STRUCTURED_PARAMETER_USER_T                 tUser;

  /* Structure Type 130-255: reserved             */
}__HIL_PACKED_POST PBDPS_STRUCTURED_PRM_DATA_T;

typedef __HIL_PACKED_PRE struct PBDPS_CV_TIME_Ttag
{
  uint32_t  ulTimeValueSec;
  uint32_t  ulTimeValueNSec;
  int32_t   lTimeDiffMin;
  uint32_t  ulAccuracyMSec;
  uint8_t   fSummerTime;
  uint8_t   fSyncActive;
  uint8_t   fAnnouncementHour;
}__HIL_PACKED_POST PBDPS_CV_TIME_T;

typedef struct PBDPS_MS0_CONFIG_Ttag
{

  #define PBDPS_SYNC_SUPPORTED                      (0x00000001)
  #define PBDPS_FREEZE_SUPPORTED                    (0x00000002)
  #define PBDPS_SETADDR_SUPPORTED                   (0x00000004)
  #define PBDPS_FAILSAFE_SUPPORTED                  (0x00000008)
  #define PBDPS_DPV1_SUPPORTED                      (0x00000010)
  #define PBDPS_EXTPRM_SUPPORTED                    (0x00000020)
  #define PBDPS_PRMCMD_SUPPORTED                    (0x00000040)
  #define PBDPS_ISOM_SUPPORTED                      (0x00000080)
  #define PBDPS_PUBLISHER_SUPPORTED                 (0x00000100)
  #define PBDPS_STRUCTPRM_SUPPORTED                 (0x00000200)

  struct
  {
    uint16_t usOutputLength;  /*!< Length of Output in bytes.  */
    uint16_t usInputLength;   /*!< length of Input in bytes.   */
  }tDiag;

  uint32_t ulFeaturesSupported;
  uint16_t usIdentNumber;
  uint8_t bSlaveAddr;
  uint8_t bDataRate;
  uint8_t bCfgLen;
  uint8_t abRealCfgData[PBDPS_MODULE_CONFIG_MAX];
} PBDPS_MS0_CONFIG_T;

typedef struct PBDPS_MS1_CONFIG_Ttag
{
  uint8_t bAlarmModeSlave;    /* Specifies the number of maximum possible active alarms */
  uint8_t bAlarmsSupported;   /* Variable that indicates the types of alarms which shall be supported */
} PBDPS_MS1_CONFIG_T;

typedef __HIL_PACKED_PRE struct PBDPS_CONFIG_Ttag
{

  #define PBDPS_CONFIG_VERSION_V1                   (1)

  uint32_t ulVersion;

  PBDPS_MS0_CONFIG_T tMS0;
  PBDPS_MS1_CONFIG_T tMS1;
}__HIL_PACKED_POST PBDPS_CONFIG_T;

#ifndef __STRUCT_FSPMS_EXTENDED_DIAG_T__
#define __STRUCT_FSPMS_EXTENDED_DIAG_T__
/*! Extended diagnosis structure. */
typedef __HIL_PACKED_PRE struct FSPMS_EXTENDED_DIAG_Ttag {
  uint32_t ulBusAddresse;   /*!< Bus Addres.    */
  uint32_t ulIdentNumber;   /*!< Ident Number.  */
  uint32_t ulBaudrate;      /*!< Buadrate.      */
  uint16_t usOutputLength;  /*!< Length of Output in bytes.  */
  uint16_t usInputLength;   /*!< length of Input in bytes.   */
} __HIL_PACKED_POST FSPMS_EXTENDED_DIAG_T;
#endif
#ifndef __STRUCT_FSPMS_CFG_DATA_T__
#define __STRUCT_FSPMS_CFG_DATA_T__
/*! The configuration data structure. */
typedef __HIL_PACKED_PRE struct FSPMS_CFG_DATA_Ttag {
  uint32_t uCfgDataLen;   /*!< Length  of config data entries. */
  uint8_t abCfgData[244]; /*!< Array of config data elements. */
} __HIL_PACKED_POST FSPMS_CFG_DATA_T;
#endif
#ifndef __STRUCT_FSPMS_PRM_DATA_T__
#define __STRUCT_FSPMS_PRM_DATA_T__
/*! The parameter data structure.*/
typedef __HIL_PACKED_PRE struct FSPMS_PRM_DATA_Ttag {
  uint32_t uPrmDataLen;   /*!< Length  of parameter data entries.  */
  uint8_t abPrmData[244]; /*!< Array of parameter data elements. */
} __HIL_PACKED_POST FSPMS_PRM_DATA_T;
#endif
#ifndef __STRUCT_FSPMS_CFG_DATA_MASTER_T__
#define __STRUCT_FSPMS_CFG_DATA_MASTER_T__
/*! The Module Configuration of the Master. */
typedef __HIL_PACKED_PRE struct FSPMS_CFG_DATA_MASTER_Ttag {
  uint32_t uCfgDataLen;   /*!< Length  of config data entries. */
  uint8_t abCfgData[244]; /*!< Array of config data elements. */
} __HIL_PACKED_POST FSPMS_CFG_DATA_MASTER_T;
#endif
#ifndef __STRUCT_PBDPS_SLAVE_DIAGNOSIS_T__
#define __STRUCT_PBDPS_SLAVE_DIAGNOSIS_T__
typedef __HIL_PACKED_PRE struct PBDPS_SLAVE_DIAGNOSIS_Ttag {
  uint8_t bStationstatus_1;
  uint8_t bStationstatus_2;
  uint8_t bStationstatus_3;
  uint8_t bMaster_Add;
  uint16_t usIdent_Number;
  uint8_t abExt_Diag_Data[238];
} __HIL_PACKED_POST PBDPS_SLAVE_DIAGNOSIS_T;
#endif
#ifndef __STRUCT_PBDPS_CYCLE_DIAGNOSIS_T__
#define __STRUCT_PBDPS_CYCLE_DIAGNOSIS_T__
typedef __HIL_PACKED_PRE struct PBDPS_CYCLE_DIAGNOSIS_Ttag {
  uint32_t ulState;
  uint32_t ulTBase_Dp;
  uint16_t usTDp;
  uint16_t usTMapc;
  uint32_t ulTBase_Io;
  uint16_t usTI;
  uint16_t usTO;
  uint32_t ulTDx;
  uint16_t usTPll_W;
  uint16_t usTPll_D;
  uint32_t ulTDP_n;
  uint32_t ulTDX_n;
  uint32_t ulTPll_W_n;
  uint32_t ulTi_n;
  uint32_t ulTo_n;
  uint32_t ulErrorCnt;
  uint32_t ulSuccessCnt;
  int32_t iDiff;
  uint32_t fRun;
} __HIL_PACKED_POST PBDPS_CYCLE_DIAGNOSIS_T;
#endif
#ifndef __STRUCT_PBDPS_CLASS2_CONNECTION_T__
#define __STRUCT_PBDPS_CLASS2_CONNECTION_T__
typedef __HIL_PACKED_PRE struct PBDPS_CLASS2_CONNECTION_Ttag {
  uint32_t eState;
  uint32_t uRemoteAddr;
  uint32_t uServer_Sap;
  uint32_t uService_Header;
  uint32_t uStored_Slot_Number;
  uint32_t uStored_Index;
  uint32_t uStored_Instance;
  uint32_t uStored_Reason;
  uint32_t uStored_Max_Len_Data_Unit;
  uint32_t uReturn_State;
} __HIL_PACKED_POST PBDPS_CLASS2_CONNECTION_T;
#endif


#endif /* _PBDPS_DATASTRUCT_H_ */
