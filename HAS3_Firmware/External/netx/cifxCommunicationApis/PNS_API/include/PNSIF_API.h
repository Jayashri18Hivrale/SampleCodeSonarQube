/**************************************************************************************
Copyright (c) Hilscher Gesellschaft fuer Systemautomation mbH. All Rights Reserved.
***************************************************************************************
$Id: PNSIF_API.h 73553 2018-01-17 10:38:50Z Benjamin $:

Description:
 Profinet IO Device Stack public API header
 Defines packet structure definitions of services offered by Profinet IO Device stack

Changes:
 Date          Description
 -----------------------------------------------------------------------------------
 2014-10-22    created
**************************************************************************************/
#ifndef PNSIF_API_H_
#define PNSIF_API_H_

#include <stdint.h>
#include "Hil_Packet.h"
#include "Hil_Types.h"

/**
 * @defgroup pns_api PNS Protocol API
 * @ingroup protocol_api
 *
 * @brief Definition of the API of the Profinet Slave Stack / Firmware
 *
 * As a general rule, every indication/request (even command code number) which is received
 * at the user application shall be returned back to the stack/firmware.  If the user
 * application does not support the indication/request the error code "TLR_E_UNKNOWN_COMMAND"
 * shall be used and the response/confirmation length shall be set to zero.
 *
 */

/** PNSIF Tasks process queue name
 *
 * @ingroup pns_api_lom
 * @deprecated Using this macro together with TLS_QUE_IDENTIFY() is not recommended.
 *
 * If the PNS stack is used as linkable object, the users application
 * has to directly interact with the PNSIF task. The handle of this task's
 * queue is stored in the field PROFINET_IODEVICE_TASK_RESOURCES_T::hQuePnsif when
 * calling PNS_StackInit(). The user application shall read this field in order
 * to get the handle. For compatibility with older user applications, it
 * is still possible to identify the queue handling using this name. */
#define PNS_IF_QUEUE_NAME            "QUE_PNS_IF"

#define PNS_IF_MAX_DEVICE_TYPE_LEN       (25)    /**< Maximum length of Device_Type */
#define PNS_IF_MAX_NAME_OF_STATION       (240)   /**< Maximum length of NameOfStation */
#define PNS_IF_MAX_TYPE_OF_STATION       (240)   /**< Maximum length of TypeOfStation */
#define PNS_IF_MAX_ORDER_ID              (20)    /**< Maximum length of OrderID */

/**
 * Maximum amount of bytes for acyclic services
 * @ingroup pns_api
 **/
#define PNS_IF_MAX_RECORD_DATA_LEN       (1024 * 64)

/**
 * Maximum amount of data which can be attached to an process alarm or
 * Generic Diagnosis.
 *
 * @ingroup pns_api
 *
 * @warning Many Profinet IO Controller do not accept more than 200 Bytes
 *          of Alarm Data.
 **/
#define PNS_IF_MAX_ALARM_DATA_LEN        (1024)



/**
 * Profinet Device Stack commands
 * @ingroup pns_api_packets
 *
 * Command codes for the Requests and Indications supported/used Profinet Device Stack.
 * Ususally the request/confirmation pair is used by the host application to make the
 * stack to do something whereas the indication/response pair is used by the
 * stack to indicate an event to the host application.
 **/
typedef enum PNS_IF_CMD_Etag
{
  PNS_IF_PLUG_MODULE_REQ                    = 0x00001F04, /**< Plug Module Service - Request */
  PNS_IF_PLUG_MODULE_CNF                    = 0x00001F05, /**< Plug Module Service - Confirmation */
  PNS_IF_PULL_MODULE_REQ                    = 0x00001F06, /**< Pull Module Service - Request */
  PNS_IF_PULL_MODULE_CNF                    = 0x00001F07, /**< Pull Module Service - Confirmation */
  PNS_IF_PLUG_SUBMODULE_REQ                 = 0x00001F08, /**< Plug Submodule Service - Request */
  PNS_IF_PLUG_SUBMODULE_CNF                 = 0x00001F09, /**< Plug Submodule Service - Confirmation */
  PNS_IF_PULL_SUBMODULE_REQ                 = 0x00001F0A, /**< Pull Submodule Service - Request */
  PNS_IF_PULL_SUBMODULE_CNF                 = 0x00001F0B, /**< Pull Submodule Service - Confirmation */
  PNS_IF_PARAM_END_IND                      = 0x00001F0E, /**< Parameter End Service - Indication */
  PNS_IF_PARAM_END_RES                      = 0x00001F0F, /**< Parameter End Service - Response */

  PNS_IF_SET_APPL_READY_REQ                 = 0x00001F10, /**< Set Application Ready Service - Request */
  PNS_IF_SET_APPL_READY_CNF                 = 0x00001F11, /**< Set Application Ready Service - Confirmation */
  PNS_IF_AR_SET_STATUS_IND                  = 0x00001F12, /**< SR ARSet status - indication of V4 */
  PNS_IF_AR_SET_STATUS_RSP                  = 0x00001F13, /**< SR ARSet status - Response of V4 */
  PNS_IF_AR_CHECK_IND                       = 0x00001F14, /**< AR Check Service - Indication */
  PNS_IF_AR_CHECK_RES                       = 0x00001F15, /**< AR Check Service - Response */
  PNS_IF_CHECK_IND                          = 0x00001F16, /**< Check Service - Indication */
  PNS_IF_CHECK_RES                          = 0x00001F17, /**< Check Service - Response */
  PNS_IF_RESET_FACTORY_SETTINGS_IND         = 0x00001F18, /**< Reset Factory Settings Service - Indication */
  PNS_IF_RESET_FACTORY_SETTINGS_RES         = 0x00001F19, /**< Reset Factory Settings Service - Response */
  PNS_IF_SAVE_STATION_NAME_IND              = 0x00001F1A, /**< Save Station Name Service - Indication */
  PNS_IF_SAVE_STATION_NAME_RES              = 0x00001F1B, /**< Save Station Name Service - Response */
  PNS_IF_RECONFIG_IND                       = 0x00001F1C, /**< Dynamic reconfiguration Service - Indication */
  PNS_IF_RECONFIG_RES                       = 0x00001F1D, /**< Dynamic reconfiguration Service - Response */
  PNS_IF_START_LED_BLINKING_IND             = 0x00001F1E, /**< Start LED Blinking Service - Indication */
  PNS_IF_START_LED_BLINKING_RES             = 0x00001F1F, /**< Start LED Blinking Service - Response */

  PNS_IF_STOP_LED_BLINKING_IND              = 0x00001F20, /**< Stop LED Blinking Service - Indication */
  PNS_IF_STOP_LED_BLINKING_RES              = 0x00001F21, /**< Stop LED Blinking Service - Response */
  PNS_IF_GET_CONFIGURED_SUBM_REQ            = 0x00001F22, /**< Get list of configured submodules - Request */
  PNS_IF_GET_CONFIGURED_SUBM_CNF            = 0x00001F23, /**< Get list of configured submodules - Confirmation */
  PNS_IF_CONFIG_TRIGGER_EVENT_REQ           = 0x00001F24, /**< Config trigger events - Request */
  PNS_IF_CONFIG_TRIGGER_EVENT_CNF           = 0x00001F25, /**< Config trigger events - Confirmation */
  PNS_IF_AR_INDATA_IND                      = 0x00001F28, /**< AR InData Service - Indication */
  PNS_IF_AR_INDATA_RES                      = 0x00001F29, /**< AR InData Service - Response */
  PNS_IF_AR_ABORT_IND                       = 0x00001F2A, /**< AR Abort Service - Indication */
  PNS_IF_AR_ABORT_RES                       = 0x00001F2B, /**< AR Abort Service - Response */
  PNS_IF_APDU_STATUS_IND                    = 0x00001F2E, /**< APDU Status Service - Indication */
  PNS_IF_APDU_STATUS_RES                    = 0x00001F2F, /**< APDU Status Service - Response */

  PNS_IF_ALARM_IND                          = 0x00001F30, /**< Alarm Service - Indication */
  PNS_IF_ALARM_RES                          = 0x00001F31, /**< Alarm Service - Response */
  PNS_IF_READ_IM_IND                        = 0x00001F32, /**< Read Identification & Maintenance Service - Indication */
  PNS_IF_READ_IM_RES                        = 0x00001F33, /**< Read Identification & Maintenance Service - Response */
  PNS_IF_WRITE_IM_IND                       = 0x00001F34, /**< Write Identification & Maintenance Service - Indication */
  PNS_IF_WRITE_IM_RES                       = 0x00001F35, /**< Write Identification & Maintenance Service - Response */
  PNS_IF_READ_RECORD_IND                    = 0x00001F36, /**< Read Record Service - Indication */
  PNS_IF_READ_RECORD_RES                    = 0x00001F37, /**< Read Record Service - Response */
  PNS_IF_WRITE_RECORD_IND                   = 0x00001F3A, /**< Write Record Service - Indication */
  PNS_IF_WRITE_RECORD_RES                   = 0x00001F3B, /**< Write Record Service - Response */
  PNS_IF_PARAM_BEGIN_IND                    = 0x00001F3C, /**< Parameter Begin Service - Indication */
  PNS_IF_PARAM_BEGIN_RES                    = 0x00001F3D, /**< Parameter Begin Service - Response */
  PNS_IF_GET_ASSET_IND                      = 0x00001F3E, /**< Get asset indication */
  PNS_IF_GET_ASSET_RES                      = 0x00001F3F, /**< Get asset response */

  PNS_IF_ADD_CHANNEL_DIAG_REQ               = 0x00001F46, /**< Add Channel Diagnosis Service - Request */
  PNS_IF_ADD_CHANNEL_DIAG_CNF               = 0x00001F47, /**< Add Channel Diagnosis Service - Confirmation */
  PNS_IF_SEND_DIAG_ALARM_REQ                = 0x00001F4C, /**< Send Diagnosis Alarm Service - Request */
  PNS_IF_SEND_DIAG_ALARM_CNF                = 0x00001F4D, /**< Send Diagnosis Alarm Service - Confirmation */

  PNS_IF_RETURN_OF_SUB_REQ                  = 0x00001F50, /**< Return Of Submodule Service - Request */
  PNS_IF_RETURN_OF_SUB_CNF                  = 0x00001F51, /**< Return Of Submodule Service - Confirmation */
  PNS_IF_SEND_PROCESS_ALARM_REQ             = 0x00001F52, /**< Send Process Alarm Service - Request */
  PNS_IF_SEND_PROCESS_ALARM_CNF             = 0x00001F53, /**< Send Process Alarm Service - Confirmation */
  PNS_IF_ADD_EXTENDED_DIAG_REQ              = 0x00001F54, /**< Add Extended Channel Diagnosis Service - Request */
  PNS_IF_ADD_EXTENDED_DIAG_CNF              = 0x00001F55, /**< Add Extended Channel Diagnosis Service - Confirmation */
  PNS_IF_ADD_GENERIC_DIAG_REQ               = 0x00001F58, /**< Add Generic Diagnosis Service - Request */
  PNS_IF_ADD_GENERIC_DIAG_CNF               = 0x00001F59, /**< Add Generic Diagnosis Service - Confirmation */
  PNS_IF_SEND_ALARM_REQ                     = 0x00001F5C, /**< Send Alarm Service - Request */
  PNS_IF_SEND_ALARM_CNF                     = 0x00001F5D, /**< Send Alarm Service - Confirmation */

  PNS_IF_GET_PARAM_REQ                      = 0x00001F64, /**< Get Parameter Service - Request */
  PNS_IF_GET_PARAM_CNF                      = 0x00001F65, /**< Get Parameter Service - Confirmation */

  PNS_IF_RESERVED1_REQ                      = 0x00001F80, /**< Internal service */
  PNS_IF_RESERVED1_CNF                      = 0x00001F81, /**< Internal service */

  PNS_IF_GET_STATION_NAME_REQ               = 0x00001F8E, /**< Get Station Name Service - Request */
  PNS_IF_GET_STATION_NAME_CNF               = 0x00001F8F, /**< Get Station Name Service - Confirmation */

  PNS_IF_SET_SUBM_STATE_REQ                 = 0x00001F92, /**< Set Submodule State - Request> */
  PNS_IF_SET_SUBM_STATE_CNF                 = 0x00001F93,  /**< Set Submodule State - Confirmation> */

  /** Add a PROFIenergy entity */
  PNS_IF_ADD_PE_ENTITY_REQ                  = 0x00001F94,
  PNS_IF_ADD_PE_ENTITY_CNF                  = 0x00001F95,
  /** Remove a PROFIenergy entity */
  PNS_IF_REMOVE_PE_ENTITY_REQ               = 0x00001F96,
  PNS_IF_REMOVE_PE_ENTITY_CNF               = 0x00001F97,
  /** Update a PROFIenergy entity */
  PNS_IF_UPDATE_PE_ENTITY_REQ               = 0x00001F98,
  PNS_IF_UPDATE_PE_ENTITY_CNF               = 0x00001F99,

  PNS_IF_GET_DIAGNOSIS_REQ                  = 0x00001FB2, /**< Get Diagnosis Service - Request */
  PNS_IF_GET_DIAGNOSIS_CNF                  = 0x00001FB3, /**< Get Diagnosis Service - Confirmation */
  PNS_IF_SAVE_IP_ADDR_IND                   = 0x00001FB8, /**< Save IP Address Service - Indication */
  PNS_IF_SAVE_IP_ADDR_RES                   = 0x00001FB9, /**< Save IP Address Service - Response */
  PNS_IF_GET_IP_ADDR_REQ                    = 0x00001FBC, /**< Get IP Address Service - Request */
  PNS_IF_GET_IP_ADDR_CNF                    = 0x00001FBD, /**< Get IP Address Service - Confirmation */

  PNS_IF_CONNECT_REQ_DONE_IND               = 0x00001FD4, /**< Connect Request Done Service - Indication */
  PNS_IF_CONNECT_REQ_DONE_RES               = 0x00001FD5, /**< Connect Request Done Service - Response */
  PNS_IF_ABORT_CONNECTION_REQ               = 0x00001FD8, /**< Abort Connection Service - Request */
  PNS_IF_ABORT_CONNECTION_CNF               = 0x00001FD9, /**< Abort Connection Service - Confirmation */

  PNS_IF_USER_ERROR_IND                     = 0x00001FDC, /**< User Error Service - Indication */
  PNS_IF_USER_ERROR_RES                     = 0x00001FDD, /**< User Error Service - Response */

  PNS_IF_SET_PORT_MAC_REQ                   = 0x00001FE0, /**< Set Port Mac Address Service - Request */
  PNS_IF_SET_PORT_MAC_CNF                   = 0x00001FE1, /**< Set Port Mac Address Service - Confirmation */
  PNS_IF_SET_CONFIGURATION_REQ              = 0x00001FE2, /**< Set Configuration Service - Request */
  PNS_IF_SET_CONFIGURATION_CNF              = 0x00001FE3, /**< Set Configuration Service - Confirmation */
  PNS_IF_GET_XMAC_DIAGNOSIS_REQ             = 0x00001FE4, /**< Get XMAC Diagnosis Service - Request */
  PNS_IF_GET_XMAC_DIAGNOSIS_CNF             = 0x00001FE5, /**< Get XMAC Diagnosis Service - Confirmation */
  PNS_IF_REMOVE_DIAG_REQ                    = 0x00001FE6, /**< Remove Diagnosis Service - Request */
  PNS_IF_REMOVE_DIAG_CNF                    = 0x00001FE7, /**< Remove Diagnosis Service - Confirmation */
  PNS_IF_SET_OEM_PARAMETERS_REQ             = 0x00001FE8, /**< Set OEM Parameters Service - Request */
  PNS_IF_SET_OEM_PARAMETERS_CNF             = 0x00001FE9, /**< Set OEM Parameters Service - Confirmation */
  PNS_IF_STORE_REMANENT_DATA_IND            = 0x00001FEA, /**< Store Remanent Data Service - Indication */
  PNS_IF_STORE_REMANENT_DATA_RES            = 0x00001FEB, /**< Store Remanent Data Service - Response */
  PNS_IF_LOAD_REMANENT_DATA_REQ             = 0x00001FEC, /**< Load Remanent Data Service - Request */
  PNS_IF_LOAD_REMANENT_DATA_CNF             = 0x00001FED, /**< Load Remanent Data Service - Confirmation */

  PNS_IF_SET_IOXS_CONFIG_REQ                = 0x00001FF2, /**< Set IOxS Config Service - Request */
  PNS_IF_SET_IOXS_CONFIG_CNF                = 0x00001FF3, /**< Set IOxS Config Service - Confirmation */
  PNS_IF_RESET_FACTORY_SETTINGS_REQ         = 0x00001FF4, /**< Reset Factory Settings Service - Request */
  PNS_IF_RESET_FACTORY_SETTINGS_CNF         = 0x00001FF5, /**< Reset Factory Settings Service - Confirmation */
  PNS_IF_PARAMET_SPEEDUP_SUPPORTED_IND      = 0x00001FF8, /**< Parameter Speedup Supported Service - Indication */
  PNS_IF_PARAMET_SPEEDUP_SUPPORTED_RSP      = 0x00001FF9, /**< Parameter Speedup Supported Service - Response */

  PNS_IF_EVENT_IND                          = 0x00001FFE, /**< Event Service - Indication */
  PNS_IF_EVENT_RSP                          = 0x00001FFF, /**< Event Service - Response */

} PNS_IF_CMD_E;

/**
 * @defgroup pns_api_lom PNS LOM Extensions
 * @ingroup pns_api
 *
 * These API functions are only required/available
 * if the PNS stack is used as LOM.
 */



/** Type of the fatal error callback
 * @ingroup pns_api_common
 */
typedef void(*PNS_FATAL_ERROR_CLB)(uint32_t ulErrorCode, void* pvUserParam);


/**
 * @defgroup pns_api_services PNS Services
 * @ingroup pns_api
 *
 * Services to be used for interaction with the PNS stack.
 */

/**
 * @defgroup pns_api_common PNS protocol API common definitions
 * @ingroup pns_api */

#ifdef __HIL_PRAGMA_PACK_ENABLE
  #pragma __HIL_PRAGMA_PACK_1(PNSIF_API_PUBLIC)
#endif


/**
 * @defgroup pns_api_configuration PNS configuration structures
 * @ingroup pns_api_common
 *
 * Structures used by several PNS configuration services
 *
 * @{
 */

/** The stack shall start communication right after issuing service RCX_CHANNEL_INIT_REQ */
#define PNS_IF_SYSTEM_START_AUTO_START                   0x00000000
/** The stack shall wait for explicit bus on by user application before start of communication */
#define PNS_IF_SYSTEM_START_APPL_CONTROLLED              0x00000001

#define PNS_IF_SYSTEM_DATA_STA_DISABLED                  0x00000000
#define PNS_IF_SYSTEM_DATA_STA_ENABLED                   0x00000002

#define PNS_IF_SYSTEM_LONG_DATA_STA_DISABLED             0x00000000
#define PNS_IF_SYSTEM_LONG_DATA_STA_ENABLED              0x00000004

/* leave some entries free for possible further extensions of siemens stack based IO-Device */

/** The stack shall not handle Identification & Maintenance on its own. */
#define PNS_IF_SYSTEM_STACK_HANDLE_I_M_DISABLED          0x00000000
/** The stack shall handle Identification & Maintenance on its own.
 *
 * This will enable basic Identification & Maintenance functionality:
 * I&M0 for all submodules, I&M1-4 read- and writeable for slot 0 subslot 0
 * */
#define PNS_IF_SYSTEM_STACK_HANDLE_I_M_ENABLED           0x00000100

/** The stack shall never send an application ready automatically */
#define PNS_IF_SYSTEM_ARDY_WOUT_APPL_REG_DISABLED        0x00000000
/** The stack shall send application ready even if no application registered.
 *
 * This option makes only sense for very very simple devices.
 *
 * @attention Usage of this flag may break Profinet Conformance
 */
#define PNS_IF_SYSTEM_ARDY_WOUT_APPL_REG_ENABLED         0x00000200

/** The stack shall use serial number from crypto flash and use
 * default values for some fields in Indentification & Maintenance records
 * @deprecated The stack ignores this flag. Usage of OEM parameters
 *             is automatically detected */
#define PNS_IF_SYSTEM_USE_OEM_PARAMETERS_DISABLE         0x00000000
/** The stack shall use serial number and parameters from
 * PNS_IF_SET_OEM_PARAMETERS_REQ.
 * @deprecated The stack ignores this flag. Usage of OEM parameters
 *             is automatically detected */
#define PNS_IF_SYSTEM_USE_OEM_PARAMETERS_ENABLE          0x00000400

/** The stack shall not use the check service for properly
 * configured modules */
#define PNS_IF_SYSTEM_CHECK_IND_ALL_MODULES_DISABLED     0x00000000
/** The stack shall use the check service for properly
 * configured modules */
#define PNS_IF_SYSTEM_CHECK_IND_ALL_MODULES_ENABLED      0x00000800

/** The stack shall not handle link down as fatal error */
#define PNS_IF_SYSTEM_HANDLE_LINK_DOWN_AS_FATAL_DISABLED 0x00000000
/** The stack shall treat link down as fatal error.
 *
 * This can be useful to test the applications implementation of
 * @ref pns_api_service_fatalerrorclb.
 */
#define PNS_IF_SYSTEM_HANDLE_LINK_DOWN_AS_FATAL_ENABLED  0x00001000

/** The stack shall not send check indications for submodules
 * not used by any AR. */
#define PNS_IF_SYSTEM_CHECK_IND_UNUSED_MODULES_DISABLED  0x00000000
/** The stack shall send check indications for submodules
 * not used by any AR. */
#define PNS_IF_SYSTEM_CHECK_IND_UNUSED_MODULES_ENABLED   0x00002000

/** The stack will try to store remanent data on its own */
#define PNS_IF_SYSTEM_DISABLE_STORE_REMANENT_DISABLE     0x00000000
/** The stack will never store remanent dtata on its own but send the
 * data to the user application */
#define PNS_IF_SYSTEM_DISABLE_STORE_REMANENT_ENABLED     0x00004000

/** The stack shall not swap the byte order of the I/O-Data */
#define PNS_IF_SYSTEM_BYTEORDER_BIG_ENDIAN               0x00000000
/** The stack shall swap the byte order of the I/O-Data. If this is enabled
 * the I/O structure information must be appended to the
 * set configuration request after the submodule configuration.
 *
 * @warning Using this feature may consume a lot of CPU power and therefore
 *          limit the minimum i/o update interval
 *
 * @note Little Endian Byteorder is not supported by PNS v4.x and
 *       is only supported by PNS v3.x */
#define PNS_IF_SYSTEM_BYTEORDER_LITTLE_ENDIAN            0x00000008

/** The stack shall issue link state indications on any link change */
#define PNS_IF_SYSTEM_ENABLE_LINK_STATE_INDICATION       0x00000000
/** The stack shall not issue link state indications if the link changes */
#define PNS_IF_SYSTEM_DISABLE_LINK_STATE_INDICATION      0x00008000

/* shall the stack check if IO-data offsets in IO-image (or DPM) */
/* provided by application are valid? */
/* ATTENTION: disabling this check may lead to inconsistent IO-data */
/*            in case of faulty application configuration */
/* ATTENTION: disabling this check will lead to invalid stack response */
/*            to service RCX_GET_DPM_IO_INFO_REQ */
#define PNS_IF_SYSTEM_ENABLE_IO_OFFSET_CHECKING           0x00000000
#define PNS_IF_SYSTEM_DISABLE_IO_OFFSET_CHECKING          0x00010000

/** The stack shall not store remanent IP-address and device name on its own. */
/*  An Application should take care to store them on indications "PNS_IF_SAVE_STATION_NAME_IND",
 *  "PNS_IF_SAVE_IP_ADDR_IND". */
#define PNS_IF_SYSTEM_NAME_IP_HANDLING_BY_STACK_DISABLED   0x00000000
/** The stack shall store remanent IP-address and device name on its own. */
/*  In this case the indications "PNS_IF_SAVE_STATION_NAME_IND",
 *  "PNS_IF_SAVE_IP_ADDR_IND" are for application informative only  */
#define PNS_IF_SYSTEM_NAME_IP_HANDLING_BY_STACK_ENABLED    0x00020000

/** if the stack handles Identification & Maintenance on its own.
 *  This will indicates which I&M records shall be supported by the stack.
 * */

#define PNS_IF_STACK_IM_MSK                0x0000000F
#define PNS_IF_STACK_HANDLE_IM_1           0x00000001
#define PNS_IF_STACK_HANDLE_IM_2           0x00000002
#define PNS_IF_STACK_HANDLE_IM_3           0x00000004
#define PNS_IF_STACK_HANDLE_IM_4           0x00000008
#define PNS_IF_STACK_HANDLE_IM_5           0x00000010



/**
 * Basic Device Parameters
 *
 * This structure describes the basic device parameters the
 * PNS stack shall use.
 */
typedef __HIL_PACKED_PRE struct PNS_IF_DEVICE_PARAMETER_Ttag
{
  uint32_t ulSystemFlags;                               /** flags to use are defined in this file */
  uint32_t ulWdgTime;                                   /** DPM watchdog time*/
  uint32_t ulVendorId;                                  /** the Vendor ID */
  uint32_t ulDeviceId;                                  /** the Device ID */
  uint32_t ulMaxAr;                                     /** currently unused */
  uint32_t ulCompleteInputSize;                         /** max. combined amount of Input bytes */
  uint32_t ulCompleteOutputSize;                        /** max. combined amount of Output bytes */
  uint32_t ulNameOfStationLen;                          /** length of NameOfStation */
  uint8_t  abNameOfStation[PNS_IF_MAX_NAME_OF_STATION]; /** the NameOfStation */
  uint32_t ulTypeOfStationLen;                          /** length of TypeOfStation */
  uint8_t  abTypeOfStation[PNS_IF_MAX_TYPE_OF_STATION]; /** the TypeOfStation */
  uint8_t  abDeviceType[PNS_IF_MAX_DEVICE_TYPE_LEN +3]; /** the DeviceType - ignore the last 3 padding bytes ! */
  uint8_t  abOrderId[PNS_IF_MAX_ORDER_ID];              /** the OrderID (pad with spaces (0x20) */

  uint32_t ulIpAddr;                                    /** IP Address, default: 0.0.0.0 */
  uint32_t ulNetMask;                                   /** Netmask, default: 0.0.0.0 */
  uint32_t ulGateway;                                   /** Gateway, default: 0.0.0.0 */
  uint16_t usHwRevision;                                /** Hardware Revision, default: 0 */
  uint16_t usSwRevision1;                               /** Software Revision 1, default: 0 */
  uint16_t usSwRevision2;                               /** Software Revision 2, default: 0 */
  uint16_t usSwRevision3;                               /** Software Revision 3, default: 0 */
  uint8_t  bSwRevisionPrefix;                           /** Software Prefix, default: 0 */
  uint8_t  bReserved;                                   /** reserved, shall be set to 0 */
  uint16_t usMaxDiagRecords;                            /** max. amaount of parallel Diagnosis records, default: 256 */
  uint16_t usInstanceId;                                /** GSDML-parameter ObjectUUID_LocalIndex, default: 1 */
  uint16_t usReserved;                                  /** reserved, shall be set to 0 */
} __HIL_PACKED_POST PNS_IF_DEVICE_PARAMETER_T;


/** Definition of an API */
typedef __HIL_PACKED_PRE struct PNS_IF_API_STRUCT_Ttag
{
  /** The API to define */
  uint32_t                        ulApi;
  /* Number of @ref PNS_IF_SUBMODULE_STRUCT_T following this structure */
  uint32_t                        ulNumSubmodItems;
} __HIL_PACKED_POST PNS_IF_API_STRUCT_T;

/** Definition of an Submodule */
typedef __HIL_PACKED_PRE struct PNS_IF_SUBMODULE_STRUCT_Ttag
{
  /** the slot this submodule belongs to */
  uint16_t                        usSlot;
  /** the subslot of this submodule */
  uint16_t                        usSubslot;
  /** Module ID to which this submodule belongs */
  uint32_t                        ulModuleId;
  /** Submodule ID */
  uint32_t                        ulSubmodId;
  /** length of provider data (sent by IO-Device, received by IO-Controller) */
  uint32_t                        ulProvDataLen;
  /** length of consumer data (sent by IO-Controller, received by IO-Device) */
  uint32_t                        ulConsDataLen;
  /** offset in DPM where consumer data is copied to */
  uint32_t                        ulDPMOffsetIn;
  /** offset in DPM where provider data is taken from */
  uint32_t                        ulDPMOffsetOut;
  /** offset where to take IOPS of this submodule relative to beginning of IOPS block in dpm output area from */
  uint16_t                        usOffsetIOPSProvider;
  /** offset where to put IOPS for this submodule relative to beginning of IOPS block in dpm input area to */
  uint16_t                        usOffsetIOPSConsumer;
  /** offset where to take IOCS  of this submodule relative to beginning of IOPS block in dpm input area from */
  uint16_t                        usOffsetIOCSProvider;
  /** offset where to put IOCS for this submodule relative to beginning of IOPS block in dpm output area to */
  uint16_t                        usOffsetIOCSConsumer;
  /** reserved for future usage */
  uint32_t                        ulReserved;
} __HIL_PACKED_POST PNS_IF_SUBMODULE_STRUCT_T;

/** Submodule configuration part of the Set Configuration Service Request
 *
 * This part of the Set configuration Service Request has a dynamic structure and
 * should be carefully filled in. The stack will refuse any invalid configuration.
 *
 * The structure shall be as follows:
 * @code
 * PNS_IF_MODULE_CFG_REQ_DATA_T tHead;
 * //... first api (mandatory) ...
 * PNS_IF_API_STRUCT_T          tApi1;
 * PNS_IF_SUBMODULE_STRUCT_T    atSubmod[tApi1.ulNumSubmodItems];
 * //... second and more APIs (optional) ...
 * // ... last api ...
 * PNS_IF_API_STRUCT_T          tApiN; // N = tHead.ulNumApi
 * PNS_IF_SUBMODULE_STRUCT_T    atSubmod[tApiN.ulNumSubmodItems];
 * @endcode
 * */
typedef __HIL_PACKED_PRE struct PNS_IF_MODULE_CFG_REQ_DATA_Ttag
{
  /** Number of APIs following this structure */
  uint32_t                   ulNumApi;
} __HIL_PACKED_POST PNS_IF_MODULE_CFG_REQ_DATA_T;

/** Signal configuration part of the Set Configuration Request
 *
 * This part of the set configuration service request describes the signals
 * of each submodule. This is required if the stack is configured
 * to use Little Endian Byte order for the data in DPM.
 *
 * The structure of this part shall be as follows:
 *
 * @code
 * PNS_IF_SIGNAL_CFG_REQ_DATA_T            tHead; // ulNumSignals = Number of submodules!
 * IO_SIGNALS_CONFIGURE_SIGNAL_REQ_DATA_T  atSignals[tHead.ulNumsignals];
 * @endcode
 */
typedef __HIL_PACKED_PRE struct PNS_IF_SIGNAL_CFG_REQ_DATA_Ttag
{
  /** Number of Signal elements following this structure */
  uint32_t                   ulNumSignals;
} __HIL_PACKED_POST PNS_IF_SIGNAL_CFG_REQ_DATA_T;



/** @} */


/** Link status for one port
 * @ingroup pns_api_common
 * */
typedef __HIL_PACKED_PRE struct PNS_IF_LINK_STATUS_DATA_Ttag
{
  /** port the link status is for */
  uint32_t    ulPort;
  /** if a full duplex link is available on this port */
  uint32_t    fIsFullDuplex;
  /** if a link is available on this port */
  uint32_t    fIsLinkUp;
  /** the speed of the link
   *
   * - 0 No Link
   * - 10 for 10MBit
   * - 100 for 100MBit
   */
  uint32_t    ulSpeed;
} __HIL_PACKED_POST PNS_IF_LINK_STATUS_DATA_T;

/** @} */


/**
 * @defgroup pns_api_packets PNS packets
 * @ingroup pns_api
 *
 * A summary of all packets received or send by the
 * PNS stack.
 */


/**
 * @defgroup pns_api_packet_devicehandle Device Handle Packet
 * @ingroup pns_api_packets
 *
 * A common packet used in serveral PNS services
 *
 * @{
 */

/** Data Part of the Device Handle Packet */
typedef __HIL_PACKED_PRE struct PNS_IF_HANDLE_DATA_Ttag
{
  /** The device handle */
  uint32_t hDeviceHandle;
} __HIL_PACKED_POST PNS_IF_HANDLE_DATA_T;

/** Packet structure of the Device Handle Packet */
typedef __HIL_PACKED_PRE struct PNS_IF_HANDLE_PACKET_Ttag
{
  /** packet header */
  HIL_PACKET_HEADER_T                       tHead;
  /** packet data */
  PNS_IF_HANDLE_DATA_T                      tData;
} __HIL_PACKED_POST PNS_IF_HANDLE_PACKET_T;
/** @} */


/** Extended status block Bitlist types
 * @ingroup pns_api_common
 *
 * If the PNS stack is used as LFW the following definitions apply to the
 * extended status bitlist definitions from the extended status block.
 * */
typedef enum PNS_IF_STATUS_TYPE_Etag
{
  /** invalid entry / reserved */
  PNS_IF_STATUS_TYPE_UNDEFINED    = 0,
  /** The list is an IOPS list using one bit for each submodule IOPS 1=good/0=bad */
  PNS_IF_STATUS_TYPE_IOPS_BITLIST,
  /** The list is an IOPS list using one byte for each submodule IOPS 0x80=good/...=bad (see pns specification)*/
  PNS_IF_STATUS_TYPE_IOPS_BYTELIST,
  /** The list is an IOCS list using one bit for each submodule IOCS 1=good/0=bad */
  PNS_IF_STATUS_TYPE_IOCS_BITLIST,
  /** The list is an IOCS list using one byte for each submodule IOCS 0x80=good/...=bad (see pns specification)*/
  PNS_IF_STATUS_TYPE_IOCS_BYTELIST,
} PNS_IF_STATUS_TYPE_E;


/** @defgroup pns_api_packet_setconfiguration_req Set Configuration Request
 * @ingroup pns_api_packets
 *
 * @attention
 * If the request packet defined below is transfered through DPM
 * the case may occur that the packet in total will be bigger than
 * the amount of data the rcX-DPM mailbox can handle at once.
 * For this scenario a mechanism to transfer the packet in a sequence
 * of several smaller packets is defined in Hilscher DualPortMemory Manual.
 * This sequenced way is supported by the firmware.
 *
 * @attention
 * The second and all later sequenced packet shall consist of the
 * HIL_PACKET_HEADER_T directly followed by the content of bytes.
 * The fields ulTotalConfigPckLen, tWarmstart and tModule shall NOT be present.
 * @{ */

/** Data Part */
typedef __HIL_PACKED_PRE struct PNS_IF_SET_CONFIGURATION_REQUEST_DATA_Ttag
{
  /** total length of this configuration request.
   * This length is needed to handle the case that the request is transfered
   * in more than 1 sequence through DPM it shall contain the total length
   * in summ) of all sequenced packets (without packet headers) */
  uint32_t                                ulTotalConfigPckLen;
  /** the device parameters */
  PNS_IF_DEVICE_PARAMETER_T                 tDeviceParameters;
  /** The module configuration */
  PNS_IF_MODULE_CFG_REQ_DATA_T              tModuleConfig;
} __HIL_PACKED_POST PNS_IF_SET_CONFIGURATION_REQUEST_DATA_T;

/** Request Packet */
typedef __HIL_PACKED_PRE struct PNS_IF_SET_CONFIGURATION_REQUEST_REQ_Ttag
{
  /** Packet header */
  HIL_PACKET_HEADER_T                       tHead;
  /** Data part */
  PNS_IF_SET_CONFIGURATION_REQUEST_DATA_T   tData;
} __HIL_PACKED_POST PNS_IF_SET_CONFIGURATION_REQUEST_REQ_T;

/** @} */

/** @defgroup pns_api_packet_setconfiguration_cnf Set Configuration Confirmation
 * @ingroup pns_api_packets
 * @{ */

/** Confirmation Packet */
typedef HIL_EMPTY_PACKET_T                  PNS_IF_SET_CONFIGURATION_REQUEST_CNF_T;

/** @} */



/** PNS Set Configuration Service.
 * @ingroup pns_api_services
 *
 * The set configuration service shall be used to configure the PNS stack if
 * no database (Sycon) or inibatch (netX Configuration Utility) configuration is used.
 *
 */
typedef union PNS_IF_SET_CONFIGURATION_REQUEST_PCK_Ttag
{
  /** Set Configuration Request */
  PNS_IF_SET_CONFIGURATION_REQUEST_REQ_T    tReq;
  /** Set Configuration Confirmation */
  PNS_IF_SET_CONFIGURATION_REQUEST_CNF_T    tCnf;
} PNS_IF_SET_CONFIGURATION_REQUEST_PCK_T;



/*
 * PLUG MODULE SERVICE
 */

enum
{
  PNS_IF_PLUG_CORRECT_MODULE = 0,         /**< the correct module is plugged */
  PNS_IF_PLUG_SUBSTITUTE_MODULE,          /**< a substitute module is plugged */
};

/* Request packet */
typedef __HIL_PACKED_PRE struct PNS_IF_PLUG_MODULE_REQ_DATA_Ttag
{
  /** Obsolete field. set to zero */
  uint32_t ulReserved;
  uint32_t ulApi;
  uint32_t ulSlot;
  uint32_t ulModuleId;
  uint16_t usModuleState; /* module state: 0 = correct module, 1 = substitute module */
} __HIL_PACKED_POST PNS_IF_PLUG_MODULE_REQ_DATA_T;

typedef __HIL_PACKED_PRE struct PNS_IF_PLUG_MODULE_REQ_Ttag
{
  /** packet header */
  HIL_PACKET_HEADER_T                       tHead;
  /** packet data */
  PNS_IF_PLUG_MODULE_REQ_DATA_T             tData;
} __HIL_PACKED_POST PNS_IF_PLUG_MODULE_REQ_T;

/* Confirmation packet */
typedef PNS_IF_PLUG_MODULE_REQ_T            PNS_IF_PLUG_MODULE_CNF_T;

/* union packet */
typedef union PNS_IF_PLUG_MODULE_PCK_Ttag
{
  /** request packet */
  PNS_IF_PLUG_MODULE_REQ_T                  tReq;
  /** confirmation packet */
  PNS_IF_PLUG_MODULE_CNF_T                  tCnf;
} PNS_IF_PLUG_MODULE_PCK_T;


/*
 * PLUG SUBMODULE SERVICE
 */

enum
{
  PNS_IF_PLUG_CORRECT_SUBMODULE = 0,      /**< the correct submodule is plugged */
  PNS_IF_PLUG_SUBSTITUTE_SUBMODULE,       /**< a substitute submodule is plugged */
};

/* Extended plug submodule data structure (since stack versions V3.4) */
typedef __HIL_PACKED_PRE struct PNS_IF_PLUG_SUBMODULE_EXTENDED_REQ_DATA_Ttag
{
  /** Obsolete field. set to zero */
  uint32_t ulReserved1;
  uint32_t ulApi;
  uint32_t ulSlot;
  uint32_t ulSubslot;
  uint32_t ulSubmodId;
  /* provider data is data sent by IO-Device and received by IO-Controller */
  uint32_t ulProvDataLen;
  /* consumer data is data sent by IO-Controller and received by IO-Device */
  uint32_t ulConsDataLen;
  /* offset in DPM where Output data (consumed by IO-Device from IO-Controller) is copied to */
  uint32_t ulDPMOffsetCons;
  /* offset in DPM where Input data (provided by IO-Device to IO-Controller) is taken from */
  uint32_t ulDPMOffsetProv;
  /* offset where to put IOPS provider state for this submodule relative to beginning of IOPS block in dpm output area to */
  uint16_t usOffsetIOPSProvider;
  /* offset where to take IOPS provider state of this submodule relative to beginning of IOPS block in dpm input area from */
  uint16_t usOffsetIOPSConsumer;
  /* offset where to put IOCS provider state for this submodule relative to beginning of IOCS block in dpm output area to */
  uint16_t usOffsetIOCSProvider;
  /* offset where to take IOCS provider state of this submodule relative to beginning of IOCS block in dpm input area from */
  uint16_t usOffsetIOCSConsumer;
  /* reserved for future use - maybe needed for DPM Area later */
  uint32_t ulReserved2;
  /* submodule state: 0 = correct submodule, 1 = substitute submodule */
  uint16_t usSubmodState;
  /* Module identifier (new since V3.4 - Submodules can now be plugged without prior plugging the modules)*/
  uint32_t ulModuleId;
  /* Module state (new since V3.4 - Submodules can now be plugged without prior plugging the modules)*/
  uint16_t usModuleState;

} __HIL_PACKED_POST PNS_IF_PLUG_SUBMODULE_EXTENDED_REQ_DATA_T;

/* Plug submodule data structure. */
typedef __HIL_PACKED_PRE struct PNS_IF_PLUG_SUBMODULE_REQ_DATA_Ttag
{
  /** Obsolete field. set to zero */
  uint32_t ulReserved1;
  uint32_t ulApi;
  uint32_t ulSlot;
  uint32_t ulSubslot;
  uint32_t ulSubmodId;
  /* provider data is data sent by IO-Device and received by IO-Controller */
  uint32_t ulProvDataLen;
  /* consumer data is data sent by IO-Controller and received by IO-Device */
  uint32_t ulConsDataLen;
  /* offset in DPM where Output data (consumed by IO-Device from IO-Controller) is copied to */
  uint32_t ulDPMOffsetCons;
  /* offset in DPM where Input data (provided by IO-Device to IO-Controller) is taken from */
  uint32_t ulDPMOffsetProv;
  /* offset where to put IOPS provider state for this submodule relative to beginning of IOPS block in dpm output area to */
  uint16_t usOffsetIOPSProvider;
  /* offset where to take IOPS consumer state of this submodule relative to beginning of IOPS block in dpm input area from */
  uint16_t usOffsetIOPSConsumer;
  /* offset where to put IOCS provider state for this submodule relative to beginning of IOCS block in dpm output area to */
  uint16_t usOffsetIOCSProvider;
  /* offset where to take IOCS consumer state of this submodule relative to beginning of IOCS block in dpm input area from */
  uint16_t usOffsetIOCSConsumer;
  /* reserved for future use - maybe needed for DPM Area later */
  uint32_t ulReserved2;
  /* submodule state: 0 = correct submodule, 1 = substitute submodule */
  uint16_t usSubmodState;
} __HIL_PACKED_POST PNS_IF_PLUG_SUBMODULE_REQ_DATA_T;



typedef __HIL_PACKED_PRE struct PNS_IF_PLUG_SUBMODULE_REQ_Ttag
{
  /** packet header */
  HIL_PACKET_HEADER_T                         tHead;
  /** packet data */
  PNS_IF_PLUG_SUBMODULE_REQ_DATA_T            tData;
} __HIL_PACKED_POST PNS_IF_PLUG_SUBMODULE_REQ_T;

/* Confirmation packet */
typedef PNS_IF_PLUG_SUBMODULE_REQ_T         PNS_IF_PLUG_SUBMODULE_CNF_T;

/* union packet */
typedef union PNS_IF_PLUG_SUBMODULE_PCK_Ttag
{
  /** request packet */
  PNS_IF_PLUG_SUBMODULE_REQ_T               tReq;
  /** confirmation packet */
  PNS_IF_PLUG_SUBMODULE_CNF_T               tCnf;
} PNS_IF_PLUG_SUBMODULE_PCK_T;

typedef __HIL_PACKED_PRE struct PNS_IF_PLUG_SUBMODULE_EXTENDED_REQ_Ttag
{
  /** packet header */
  HIL_PACKET_HEADER_T                         tHead;
  /** packet data */
  PNS_IF_PLUG_SUBMODULE_EXTENDED_REQ_DATA_T   tData;
} __HIL_PACKED_POST PNS_IF_PLUG_SUBMODULE_EXTENDED_REQ_T;

/* Confirmation packet */
typedef PNS_IF_PLUG_SUBMODULE_EXTENDED_REQ_T         PNS_IF_PLUG_SUBMODULE_EXTENDED_CNF_T;

/* union packet */
typedef union PNS_IF_PLUG_SUBMODULE_EXTENDED_PCK_Ttag
{
  /** request packet */
  PNS_IF_PLUG_SUBMODULE_EXTENDED_REQ_T        tReq;
  /** confirmation packet */
  PNS_IF_PLUG_SUBMODULE_EXTENDED_CNF_T        tCnf;
} PNS_IF_PLUG_SUBMODULE_EXTENDED_PCK_T;


/*
 * PULL MODULE SERVICE
 */

/* Request packet */
typedef __HIL_PACKED_PRE struct PNS_IF_PULL_MODULE_REQ_DATA_Ttag
{
  /** Obsolete field. set to zero */
  uint32_t ulReserved;
  uint32_t ulApi;
  uint32_t ulSlot;
} __HIL_PACKED_POST PNS_IF_PULL_MODULE_REQ_DATA_T;

typedef __HIL_PACKED_PRE struct PNS_IF_PULL_MODULE_REQ_Ttag
{
  /** packet header */
  HIL_PACKET_HEADER_T                       tHead;
  /** packet data */
  PNS_IF_PULL_MODULE_REQ_DATA_T             tData;
} __HIL_PACKED_POST PNS_IF_PULL_MODULE_REQ_T;

/* Confirmation packet */
typedef PNS_IF_PULL_MODULE_REQ_T            PNS_IF_PULL_MODULE_CNF_T;

/* packet union */
typedef union PNS_IF_PULL_MODULE_PCK_Ttag
{
  /** request packet */
  PNS_IF_PULL_MODULE_REQ_T                  tReq;
  /** confirmation packet */
  PNS_IF_PULL_MODULE_CNF_T                  tCnf;
} PNS_IF_PULL_MODULE_PCK_T;


/*
 * PULL SUBMODULE SERVICE
 */

/* Request packet */
typedef __HIL_PACKED_PRE struct PNS_IF_PULL_SUBMODULE_REQ_DATA_Ttag
{
  /** Obsolete field. set to zero */
  uint32_t ulReserved;
  uint32_t ulApi;
  uint32_t ulSlot;
  uint32_t ulSubslot;
} __HIL_PACKED_POST PNS_IF_PULL_SUBMODULE_REQ_DATA_T;

typedef __HIL_PACKED_PRE struct PNS_IF_PULL_SUBMODULE_REQ_Ttag
{
  /** packet header */
  HIL_PACKET_HEADER_T                       tHead;
  /** packet data */
  PNS_IF_PULL_SUBMODULE_REQ_DATA_T          tData;
} __HIL_PACKED_POST PNS_IF_PULL_SUBMODULE_REQ_T;

/* Confirmation packet */
typedef PNS_IF_PULL_SUBMODULE_REQ_T         PNS_IF_PULL_SUBMODULE_CNF_T;

/* packet union */
typedef union PNS_IF_PULL_SUBMODULE_PCK_Ttag
{
  /** request packet */
  PNS_IF_PULL_SUBMODULE_REQ_T               tReq;
  /** confirmation packet */
  PNS_IF_PULL_SUBMODULE_CNF_T               tCnf;
} PNS_IF_PULL_SUBMODULE_PCK_T;


/*
 * AR CHECK INDICATION SERVICE
 */

/* Indication packet */
typedef __HIL_PACKED_PRE struct PNS_IF_AR_CHECK_IND_DATA_Ttag
{
  uint32_t    hDeviceHandle;
  uint16_t    usARType;
  uint32_t    ulARProperties;
  uint32_t    ulRemoteIpAddr;
  uint16_t    usRemoteNameOfStationLen;
  uint8_t     abRemoteNameOfStation[PNS_IF_MAX_NAME_OF_STATION];
  HIL_UUID_T  tCmInitiatorObjUUID;

  /** The following fields are not supported by PNSv3 implementation and only valid for PNSv4 implementation */
  /** The following fields shall only be evaluated if usARType == 0x20 (IOCARSR: System redundancy or dynamic reconfiguration). */
  /* AR ARUUID */
  HIL_UUID_T  tARUUID;
  /* RedundancyDataHoldTimer of the whole AR-Set */
  uint16_t    usRdhtValue;

} __HIL_PACKED_POST PNS_IF_AR_CHECK_IND_DATA_T;

typedef __HIL_PACKED_PRE struct PNS_IF_AR_CHECK_IND_Ttag
{
  /** packet header */
  HIL_PACKET_HEADER_T                       tHead;
  /** packet data */
  PNS_IF_AR_CHECK_IND_DATA_T                tData;
} __HIL_PACKED_POST PNS_IF_AR_CHECK_IND_T;

/* Confirmation packet */
typedef PNS_IF_HANDLE_PACKET_T              PNS_IF_AR_CHECK_RSP_T;

/* Response union */
typedef union PNS_IF_AR_CHECK_PCK_Ttag
{
  /** indication packet */
  PNS_IF_AR_CHECK_IND_T                     tInd;
  /** response packet */
  PNS_IF_AR_CHECK_RSP_T                     tRsp;
} PNS_IF_AR_CHECK_PCK_T;


/*
 * CHECK INDICATION SERVICE
 */

/** Module state code */
typedef enum PNIO_MODSTATE_Etag          /* Module state */
{
  PNIO_MODSTATE_NO_MODULE = 0,           /**< no module plugged in slot */
  PNIO_MODSTATE_WRONG_MODULE,            /**< wrong module plugged in slot */
  PNIO_MODSTATE_PROPER_MODULE,           /**< proper module */
  PNIO_MODSTATE_SUBSTITUTE_MODULE,       /**< substitute, the wrong module can fulfill requirements */
  PNIO_MODSTATE_UNUSED_MODULE,       /**< module not expected by controller */
  PNIO_MODSTATE_CORRECT_MODULE = 0xFFFF, /**< correct module was plugged by application */
} PNIO_MODSTATE_E;

/** submodule state code */
typedef enum PNIO_SUBSTATE_Etag              /* Submodule state */
{
  PNIO_SUBSTATE_NO_SUBMODULE = 0,            /**< no submodule */
  PNIO_SUBSTATE_WRONG_SUBMODULE,             /**< Wrong submodule */
  PNIO_SUBSTATE_PROPER_SUBMODULE,            /**< locked by IO controller */
  PNIO_SUBSTATE_RESERVED_1,                  /**< reserved */
  PNIO_SUBSTATE_APPL_READY_PENDING,          /**< application ready pending */
  PNIO_SUBSTATE_RESERVED_2,                  /**< reserved */
  PNIO_SUBSTATE_RESERVED_3,                  /**< reserved */
  PNIO_SUBSTATE_SUBSTITUTE_SUBMODULE,        /**< substitute, the wrong submodule can fulfill requirements */
  PNIO_SUBSTATE_UNUSED_SUBMODULE,        /**< submodule not expected by controller */
  PNIO_SUBSTATE_CORRECT_SUBMODULE = 0xFFFF,  /**< correct submodule was plugged by application*/
} PNIO_SUBSTATE_E;

/** @} */


/* Indication packet */
typedef __HIL_PACKED_PRE struct PNS_IF_CHECK_IND_IND_DATA_Ttag
{
  uint32_t hDeviceHandle;
  uint32_t ulApi;
  uint32_t ulSlot;
  uint32_t ulSubslot;
  uint32_t ulModuleId;
  uint16_t usModuleState;
  uint32_t ulSubmodId;
  uint16_t usSubmodState;
  uint16_t usExpInDataLen;
  uint16_t usExpOutDataLen;
} __HIL_PACKED_POST PNS_IF_CHECK_IND_IND_DATA_T;

typedef __HIL_PACKED_PRE struct PNS_IF_CHECK_IND_Ttag
{
  /** packet header */
  HIL_PACKET_HEADER_T                       tHead;
  /** packet data */
  PNS_IF_CHECK_IND_IND_DATA_T               tData;
} __HIL_PACKED_POST PNS_IF_CHECK_IND_T;

/* Response packet */
typedef __HIL_PACKED_PRE struct PNS_IF_CHECK_IND_RSP_DATA_Ttag
{
  uint32_t hDeviceHandle;
  uint32_t ulApi;
  uint32_t ulSlot;
  uint32_t ulSubslot;
  uint32_t ulModuleId;
  uint16_t usModuleState;
  uint32_t ulSubmodId;
  uint16_t usSubmodState;
} __HIL_PACKED_POST PNS_IF_CHECK_IND_RSP_DATA_T;

typedef __HIL_PACKED_PRE struct PNS_IF_CHECK_RSP_Ttag
{
  /** packet header */
  HIL_PACKET_HEADER_T                       tHead;
  /** packet data */
  PNS_IF_CHECK_IND_RSP_DATA_T               tData;
} __HIL_PACKED_POST PNS_IF_CHECK_RSP_T;

/* packet union */
typedef union PNS_IF_CHECK_IND_PCK_Ttag
{
  /** indication packet */
  PNS_IF_CHECK_IND_T                        tInd;
  /** response packet */
  PNS_IF_CHECK_RSP_T                        tRsp;
} PNS_IF_CHECK_IND_PCK_T;


/*
 * CONNECT REQUEST DONE SERVICE
 */

/* Indication packet */
typedef PNS_IF_HANDLE_PACKET_T              PNS_IF_CONNECTREQ_DONE_IND_T;

/* Response packet */
typedef PNS_IF_HANDLE_PACKET_T              PNS_IF_CONNECTREQ_DONE_RSP_T;

/* packet union */
typedef union PNS_IF_CONNECTREQ_DONE_PCK_Ttag
{
  /** indication packet */
  PNS_IF_CONNECTREQ_DONE_IND_T              tInd;
  /** response packet */
  PNS_IF_CONNECTREQ_DONE_RSP_T              tRsp;
} PNS_IF_CONNECTREQ_DONE_PCK_T;


/*
 * APPLICATION READY SERVICE
 */

/* request packet */
typedef PNS_IF_HANDLE_PACKET_T              PNS_IF_APPL_READY_REQ_T;

/* Confirmation packet */
typedef PNS_IF_HANDLE_PACKET_T              PNS_IF_APPL_READY_CNF_T;

/* packet union */
typedef union PNS_IF_APPL_READY_PCK_Ttag
{
  /** request packet */
  PNS_IF_APPL_READY_REQ_T                   tReq;
  /** confirmation packet */
  PNS_IF_APPL_READY_CNF_T                   tCnf;
} PNS_IF_APPL_READY_PCK_T;


/*
 * AR INDATA INDICATION SERVICE
 */

/* Indication packet */
typedef PNS_IF_HANDLE_PACKET_T              PNS_IF_AR_IN_DATA_IND_T;

/* Response packet */
typedef PNS_IF_HANDLE_PACKET_T              PNS_IF_AR_IN_DATA_RSP_T;

/* packet union */
typedef union PNS_IF_AR_IN_DATA_PCK_Ttag
{
  /** indication packet */
  PNS_IF_AR_IN_DATA_IND_T                   tInd;
  /** response packet */
  PNS_IF_AR_IN_DATA_RSP_T                   tRsp;
} PNS_IF_AR_IN_DATA_PCK_T;




/*
 * PARAMETER END SERVICE
 */

/* indication packet */
typedef __HIL_PACKED_PRE struct PNS_IF_PARAM_END_IND_DATA_Ttag
{
  uint32_t    hDeviceHandle;
  uint32_t    ulApi;      /* valid only if usSlot    != 0 */
  uint16_t    usSlot;     /* valid only if usSubslot != 0 */
  uint16_t    usSubslot;  /* 0: for all (sub)modules, != 0: for this specifi submodule */
} __HIL_PACKED_POST PNS_IF_PARAM_END_IND_DATA_T;

typedef __HIL_PACKED_PRE struct PNS_IF_PARAM_END_IND_Ttag
{
  /** packet header */
  HIL_PACKET_HEADER_T                       tHead;
  /** packet data */
  PNS_IF_PARAM_END_IND_DATA_T               tData;
} __HIL_PACKED_POST PNS_IF_PARAM_END_IND_T;

/* Response packet */
typedef __HIL_PACKED_PRE struct PNS_IF_PARAM_END_RSP_DATA_Ttag
{
  uint32_t    hDeviceHandle;
  uint32_t    fSendApplicationReady; /* set to TRUE to send ApplReady automatically */
} __HIL_PACKED_POST PNS_IF_PARAM_END_RSP_DATA_T;

typedef __HIL_PACKED_PRE struct PNS_IF_PARAM_END_RSP_Ttag
{
  /** packet header */
  HIL_PACKET_HEADER_T                       tHead;
  /** packet data */
  PNS_IF_PARAM_END_RSP_DATA_T               tData;
} __HIL_PACKED_POST PNS_IF_PARAM_END_RSP_T;

/* packet union */
typedef union PNS_IF_PARAM_END_PCK_Ttag
{
  /** indication packet */
  PNS_IF_PARAM_END_IND_T                    tInd;
  /** response packet */
  PNS_IF_PARAM_END_RSP_T                    tRsp;
} PNS_IF_PARAM_END_PCK_T;


/*
 * AR ABORT INDICATION SERVICE
 */

/* Request packet */
typedef __HIL_PACKED_PRE struct PNS_IF_AR_ABORT_IND_IND_DATA_Ttag
{
  uint32_t    hDeviceHandle;
  /* Profinet error code, consists of ErrCode, ErrDecode, ErrCode1 and ErrCode2 */
  uint32_t    ulPnio;
} __HIL_PACKED_POST PNS_IF_AR_ABORT_IND_IND_DATA_T;

typedef __HIL_PACKED_PRE struct PNS_IF_AR_ABORT_IND_IND_Ttag
{
  /** packet header */
  HIL_PACKET_HEADER_T                       tHead;
  /** packet data */
  PNS_IF_AR_ABORT_IND_IND_DATA_T            tData;
} __HIL_PACKED_POST PNS_IF_AR_ABORT_IND_IND_T;

/* Confirmation packet */
typedef PNS_IF_HANDLE_PACKET_T              PNS_IF_AR_ABORT_IND_RSP_T;

/* packet union */
typedef union PNS_IF_AR_ABORT_IND_PCK_Ttag
{
  /** indication packet */
  PNS_IF_AR_ABORT_IND_IND_T                 tInd;
  /** response packet */
  PNS_IF_AR_ABORT_IND_RSP_T                 tRsp;

} PNS_IF_AR_ABORT_IND_PCK_T;


/*
 * READ RECORD SERVICE
 */

/* Indication packet */
typedef __HIL_PACKED_PRE struct PNS_IF_READ_RECORD_IND_DATA_Ttag
{
  uint32_t    hRecordHandle;
  uint32_t    hDeviceHandle;
  uint32_t    ulSequenceNum;
  uint32_t    ulApi;
  uint32_t    ulSlot;
  uint32_t    ulSubslot;
  uint32_t    ulIndex;
  uint32_t    ulLenToRead;
} __HIL_PACKED_POST PNS_IF_READ_RECORD_IND_DATA_T;

typedef __HIL_PACKED_PRE struct PNS_IF_READ_RECORD_IND_Ttag
{
  /** packet header */
  HIL_PACKET_HEADER_T                       tHead;
  /** packet data */
  PNS_IF_READ_RECORD_IND_DATA_T             tData;
} __HIL_PACKED_POST PNS_IF_READ_RECORD_IND_T;

/* Response packet */
typedef __HIL_PACKED_PRE struct PNS_IF_READ_RECORD_RSP_DATA_Ttag
{
  uint32_t    hRecordHandle;
  uint32_t    hDeviceHandle;
  uint32_t    ulSequenceNum;
  uint32_t    ulApi;
  uint32_t    ulSlot;
  uint32_t    ulSubslot;
  uint32_t    ulIndex;
  uint32_t    ulReadLen;
  /* Profinet error code, consists of ErrCode, ErrDecode, ErrCode1 and ErrCode2 */
  uint32_t    ulPnio;
  uint16_t    usAddValue1;
  uint16_t    usAddValue2;
  uint8_t     abRecordData[]; /* ATTENTION: in case of LOM the length of abRecordData may be up to PNS_IF_MAX_RECORD_DATA_LEN bytes (BUT never more than ulLenToRead)*/
} __HIL_PACKED_POST PNS_IF_READ_RECORD_RSP_DATA_T;

typedef __HIL_PACKED_PRE struct PNS_IF_READ_RECORD_RSP_Ttag
{
  /** packet header */
  HIL_PACKET_HEADER_T                       tHead;
  /** packet data */
  PNS_IF_READ_RECORD_RSP_DATA_T             tData;
} __HIL_PACKED_POST PNS_IF_READ_RECORD_RSP_T;

/* packet union */
typedef union PNS_IF_READ_RECORD_PCK_Ttag
{
  /** indication packet */
  PNS_IF_READ_RECORD_IND_T                  tInd;
  /** response packet */
  PNS_IF_READ_RECORD_RSP_T                  tRsp;
} PNS_IF_READ_RECORD_PCK_T;


/*
 * WRITE RECORD SERVICE
 */

/* Indication packet */
typedef __HIL_PACKED_PRE struct PNS_IF_WRITE_RECORD_IND_DATA_Ttag
{
  /** Stack internal identifier. Do not evaluate */
  uint32_t    hRecordHandle;
  /** The requests associated device handle */
  uint32_t    hDeviceHandle;
  /** The requests sequence number used by the IO controller */
  uint32_t    ulSequenceNum;
  /** The api the record shall be written to */
  uint32_t    ulApi;
  /** The slot the record shall be written to */
  uint32_t    ulSlot;
  /** The subslot the record shall be written to */
  uint32_t    ulSubslot;
  /** The index the record shall be written to */
  uint32_t    ulIndex;
  /** The length of the record data */
  uint32_t    ulLenToWrite;
  /** The records data */
  uint8_t     abRecordData[]; /* ATTENTION: in case of LOM the length of abRecordData may be up to PNS_IF_MAX_RECORD_DATA_LEN bytes */
} __HIL_PACKED_POST PNS_IF_WRITE_RECORD_IND_DATA_T;

typedef __HIL_PACKED_PRE struct PNS_IF_WRITE_RECORD_IND_Ttag
{
  /** Packet header */
  HIL_PACKET_HEADER_T                       tHead;
  /** Packet data */
  PNS_IF_WRITE_RECORD_IND_DATA_T            tData;
} __HIL_PACKED_POST PNS_IF_WRITE_RECORD_IND_T;

/* Response packet */
typedef __HIL_PACKED_PRE struct PNS_IF_WRITE_RECORD_RSP_DATA_Ttag
{
  /** Stack internal identifier. Use value from indication. */
  uint32_t    hRecordHandle;
  /** The requests associated device handle. Use value from indication. */
  uint32_t    hDeviceHandle;
  /** The requests sequence number used by the IO controller.  Use value from indication. */
  uint32_t    ulSequenceNum;
  /** The api the record shall be written to. Use value from indication. */
  uint32_t    ulApi;
  /** The slot the record shall be written to. Use value from indication. */
  uint32_t    ulSlot;
  /** The subslot the record shall be written to. Use value from indication. */
  uint32_t    ulSubslot;
  /** The index the record shall be written to. Use value from indication. */
  uint32_t    ulIndex;
  /** The length of the written record data. Use value from indication. */
  uint32_t    ulWriteLen;
  /** The profinet status code. This is the status of the write.
   *
   * Use the following values:
   * - PNIO_S_OK: The write was successfull
   * - PNIO_E_IOD_WRITE_ACCESS_INVALIDINDEX: The addressed submodule does not support this index
   */
  uint32_t    ulPnio;
  /** Additional value for write response */
  uint16_t    usAddValue1;
  /** Additional value for write response */
  uint16_t    usAddValue2;
} __HIL_PACKED_POST PNS_IF_WRITE_RECORD_RSP_DATA_T;

typedef __HIL_PACKED_PRE struct PNS_IF_WRITE_RECORD_RSP_Ttag
{
  /** packet header */
  HIL_PACKET_HEADER_T                       tHead;
  /** packet data */
  PNS_IF_WRITE_RECORD_RSP_DATA_T            tData;
} __HIL_PACKED_POST PNS_IF_WRITE_RECORD_RSP_T;

/* packet union */
typedef union PNS_IF_WRITE_RECORD_PCK_Ttag
{
  /** indication packet */
  PNS_IF_WRITE_RECORD_IND_T                tInd;
  /** response packet */
  PNS_IF_WRITE_RECORD_RSP_T                tRsp;
} PNS_IF_WRITE_RECORD_PCK_T;

/*
 * SEND PROCESS ALARM SERVICE
 */

/* Request packet */
typedef __HIL_PACKED_PRE struct PNS_IF_SEND_PROCESS_ALARM_REQ_DATA_Ttag
{
  uint32_t  ulReserved ;
  uint32_t  ulApi;
  uint32_t  ulSlot;
  uint32_t  ulSubslot;
  uint32_t  hAlarmHandle;
  uint16_t  usUserStructId;
  uint16_t  usAlarmDataLen;
  uint8_t   abAlarmData[PNS_IF_MAX_ALARM_DATA_LEN]; /* ATTENTION: Recommended not to use more than 172 byte of data for compatibility reasons */
} __HIL_PACKED_POST PNS_IF_SEND_PROCESS_ALARM_REQ_DATA_T;

typedef __HIL_PACKED_PRE struct PNS_IF_SEND_PROCESS_ALARM_REQ_Ttag
{
  /** packet header */
  HIL_PACKET_HEADER_T                       tHead;
  /** packet data */
  PNS_IF_SEND_PROCESS_ALARM_REQ_DATA_T      tData;
} __HIL_PACKED_POST PNS_IF_SEND_PROCESS_ALARM_REQ_T;

/* Confirmation packet */
typedef __HIL_PACKED_PRE struct PNS_IF_SEND_PROCESS_ALARM_CNF_DATA_Ttag
{
  uint32_t    ulReserved ;
  uint32_t    hAlarmHandle;
  /* Profinet error code, consists of ErrCode, ErrDecode, ErrCode1 and ErrCode2 */
  uint32_t    ulPnio;
} __HIL_PACKED_POST PNS_IF_SEND_PROCESS_ALARM_CNF_DATA_T;

typedef __HIL_PACKED_PRE struct PNS_IF_SEND_PROCESS_ALARM_CNF_Ttag
{
  /** packet header */
  HIL_PACKET_HEADER_T                       tHead;
  /** packet data */
  PNS_IF_SEND_PROCESS_ALARM_CNF_DATA_T      tData;
} __HIL_PACKED_POST PNS_IF_SEND_PROCESS_ALARM_CNF_T;

/* packet union */
typedef union PNS_IF_SEND_PROCESS_ALARM_PCK_Ttag
{
  /** request packet */
  PNS_IF_SEND_PROCESS_ALARM_REQ_T           tReq;
  /** confirmation packet */
  PNS_IF_SEND_PROCESS_ALARM_CNF_T           tCnf;
} PNS_IF_SEND_PROCESS_ALARM_PCK_T;



/** @name Alarm Type Definition
 *
 * .
 * @{
 */
typedef enum {
  PNS_IF_ALARM_TYPE_PROCESS             = 0x0002,
  PNS_IF_ALARM_TYPE_STATUS              = 0x0005,
  PNS_IF_ALARM_TYPE_UPDATE              = 0x0006,
  PNS_IF_ALARM_TYPE_ISO_PROBLEM         = 0x0010,
  PNS_IF_ALARM_TYPE_UPLOAD_RETRIEVAL    = 0x001E,
  PNS_IF_ALARM_TYPE_MANUF_SPEC_START    = 0x0020,
  PNS_IF_ALARM_TYPE_MANUF_SPEC_END      = 0x007F,
} PNS_IF_ALARM_TYPE_E;

typedef enum {
  PNS_IF_ALARM_USER_STRUCTURE_ID_MANUF_SPEC_STAR    = 0x0000,
  PNS_IF_ALARM_USER_STRUCTURE_ID_MANUF_SPEC_END     = 0x7FFF,
  PNS_IF_ALARM_USER_STRUCTURE_ID_PRAL               = 0x8320,
} PNS_IF_ALARM_USER_STRUCTURE_ID_E;

/** Structure definition of Record data objects.
 * These data are commissioning parameters which shall be  permanently stored at the IO Controller or IO Supervisor.
 * Record data objects which are part of the GSDML file and conveyed from the engineering system shall not be part of Upload and retrieval records  */
typedef __HIL_PACKED_PRE struct PNS_IF_UPLOAD_RETRIEVAL_RECORD_DATA_Ttag
{
   uint32_t         ulRecordIndex;
   uint32_t         ulRecordLength;
}__HIL_PACKED_POST PNS_IF_UPLOAD_RETRIEVAL_RECORD_DATA_T;

/* Upload & Retrieval sub-Alarm type */
typedef enum PNS_IF_ALARM_UPLOADT_RETRIEVAL_SUBTYPE_Etag
{
  PNS_IF_ALARM_UPLOAD_SELECTED_RECORDS       = 0x0001,
  PNS_IF_ALARM_RETRIEVAL_SELECTED_RECORDS    = 0x0002,
  PNS_IF_ALARM_RETRIEVAL_ALL_RECORDS         = 0x0003,
} PNS_IF_ALARM_UPLOADT_RETRIEVAL_SUBTYPE_E;

/** Structure definition of  Upload & Retrieval alarm item data .*/
typedef __HIL_PACKED_PRE struct PNS_IF_UPLOAD_RETRIEVAL_DATA_Ttag
{
  /** Alarm subtype see \ref PNS_IF_ALARM_UPLOADT_RETRIEVAL_SUBTYPE_Etag*/
  uint8_t     bAlarmSubtype;
  /** This part of Upload & Retrieval alarm data has a dynamic structure.
   *  It shall contain device specific records data.
   *  The Length of this structure will be derived from usAlarmDataLen */
  PNS_IF_UPLOAD_RETRIEVAL_RECORD_DATA_T  tRecords[];
}__HIL_PACKED_POST PNS_IF_UPLOAD_RETRIEVAL_DATA_T;

typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST PNS_IF_PROCESS_ALARM_CHANNEL_DATA_Ttag
{
  /** Channel number issuing the process alarm */
  uint16_t  usChannelNum;
  /** Channel properties */
  uint16_t  usChannelProp;
  /** Reason for process alarm */
  uint16_t  usReason;
  /** Extended Reason for process alarm */
  uint16_t  usExtReason;
  /** Additional Value
   *
   * @hint If no additional reason is to be encoded,
   *       the field shall be set to four null bytes value 00:00:00:00
   */
  uint8_t   abReasonAddValue[128];
} PNS_IF_PROCESS_ALARM_CHANNEL_DATA_T;

/* alarm data */
typedef union PNS_IF_ALARM_DATA_Ttag
{
  /** Use with vendor specific process alarms
   *
   * @attention: It is strongly recommended to not use more than 172 byte of data
   *             for compatibility reasons
   */
  uint8_t                            abManufacturerData[PNS_IF_MAX_ALARM_DATA_LEN];
  /** Use for upload and retrieval alarms */
  PNS_IF_UPLOAD_RETRIEVAL_DATA_T      tUploadRetrieval;
  /** Use for GSDML defined process alarms with USI 0x8320 */
  PNS_IF_PROCESS_ALARM_CHANNEL_DATA_T tProcessAlarmChannelData;
}PNS_IF_ALARM_DATA_T;

/* Request packet */
typedef __HIL_PACKED_PRE struct PNS_IF_SEND_ALARM_REQ_DATA_Ttag
{
  /* API of the submodule that issues the alarm */
  uint32_t              ulApi;
  /* Slot of the submodule that issues the alarm */
  uint32_t              ulSlot;
  /* Subslot of the submodule that issues the alarm */
  uint32_t              ulSubslot;
  /* AlarmHandle freely chosen by application for identification purpose */
  uint32_t              hAlarmHandle;
  /* AlarmType to generate, see PNS_IF_USER_ALARMTYPES_E */
  uint16_t              usAlarmType;
  /* User structure identifier of the alarm will only be used in case
   * of manufacturer specific alarm type */
  uint16_t              usUserStructId;
  /* length of the alarm data (thus length of content in abAlarmData) */
  uint16_t              usAlarmDataLen;
  /* alarm data */
  PNS_IF_ALARM_DATA_T   tAlarmData;
} __HIL_PACKED_POST PNS_IF_SEND_ALARM_REQ_DATA_T;

typedef __HIL_PACKED_PRE struct PNS_IF_SEND_ALARM_REQ_Ttag
{
  /** packet header */
  HIL_PACKET_HEADER_T               tHead;
  /** packet data */
  PNS_IF_SEND_ALARM_REQ_DATA_T      tData;
} __HIL_PACKED_POST PNS_IF_SEND_ALARM_REQ_T;

/* Confirmation packet */
typedef __HIL_PACKED_PRE struct PNS_IF_SEND_ALARM_CNF_DATA_Ttag
{
  uint32_t    ulReserved ;
  uint32_t    hAlarmHandle;
  /* Profinet error code, consists of ErrCode, ErrDecode, ErrCode1 and ErrCode2 */
  uint32_t    ulPnio;
} __HIL_PACKED_POST PNS_IF_SEND_ALARM_CNF_DATA_T;

typedef __HIL_PACKED_PRE struct PNS_IF_SEND_ALARM_CNF_Ttag
{
  /** packet header */
  HIL_PACKET_HEADER_T               tHead;
  /** packet data */
  PNS_IF_SEND_ALARM_CNF_DATA_T      tData;
} __HIL_PACKED_POST PNS_IF_SEND_ALARM_CNF_T;

/* packet union */
typedef union PNS_IF_SEND_ALARM_PCK_Ttag
{
  /** request packet */
  PNS_IF_SEND_ALARM_REQ_T           tReq;
  /** confirmation packet */
  PNS_IF_SEND_ALARM_CNF_T           tCnf;
} PNS_IF_SEND_ALARM_PCK_T;


/*
 * ADD CHANNEL DIAGNOSIS RECORD SERVICE
 */

/* Request packet */
typedef __HIL_PACKED_PRE struct  PNS_IF_ADD_CHANNEL_DIAG_Ttag
{
  uint32_t  ulApi;
  uint32_t  ulSlot;
  uint32_t  ulSubslot;
  uint32_t  hDiagHandle;
  uint16_t  usChannelNum;
  uint16_t  usChannelProp;
  uint16_t  usChannelErrType;
} __HIL_PACKED_POST PNS_IF_ADD_CHANNEL_DIAG_T;

typedef __HIL_PACKED_PRE struct PNS_IF_ADD_CHANNEL_DIAG_REQ_Ttag
{
  HIL_PACKET_HEADER_T                       tHead;
  PNS_IF_ADD_CHANNEL_DIAG_T                 tData;
} __HIL_PACKED_POST PNS_IF_ADD_CHANNEL_DIAG_REQ_T;

/* Confirmation packet */
typedef PNS_IF_ADD_CHANNEL_DIAG_REQ_T       PNS_IF_ADD_CHANNEL_DIAG_CNF_T;

/* packet union */
typedef union PNS_IF_ADD_CHANNEL_DIAG_PCK_Ttag
{
  PNS_IF_ADD_CHANNEL_DIAG_REQ_T             tReq;
  PNS_IF_ADD_CHANNEL_DIAG_CNF_T             tCnf;
} PNS_IF_ADD_CHANNEL_DIAG_PCK_T;


/*
 * ADD EXTENDED DIAGNOSIS RECORD SERVICE
 */

/* Request packet */
typedef __HIL_PACKED_PRE struct PNS_IF_ADD_EXTENDED_DIAG_Ttag
{
  uint32_t  ulApi;
  uint32_t  ulSlot;
  uint32_t  ulSubslot;
  uint32_t  hDiagHandle;
  uint16_t  usChannelNum;
  uint16_t  usChannelProp;
  uint16_t  usChannelErrType;
  uint16_t  usReserved;
  uint32_t  ulExtChannelAddValue;
  uint16_t  usExtChannelErrType;
} __HIL_PACKED_POST PNS_IF_ADD_EXTENDED_DIAG_T;

typedef __HIL_PACKED_PRE struct PNS_IF_ADD_EXTENDED_DIAG_REQ_Ttag
{
  HIL_PACKET_HEADER_T                       tHead;
  PNS_IF_ADD_EXTENDED_DIAG_T                tData;
} __HIL_PACKED_POST PNS_IF_ADD_EXTENDED_DIAG_REQ_T;

/* Confirmation packet */
typedef PNS_IF_ADD_EXTENDED_DIAG_REQ_T      PNS_IF_ADD_EXTENDED_DIAG_CNF_T;

/* packet union */
typedef union PNS_IF_ADD_EXTENDED_DIAG_PCK_Ttag
{
  PNS_IF_ADD_EXTENDED_DIAG_REQ_T            tReq;
  PNS_IF_ADD_EXTENDED_DIAG_CNF_T            tCnf;
} PNS_IF_ADD_EXTENDED_DIAG_PCK_T;


/*
 * ADD GENERIC DIAGNOSIS RECORD SERVICE
 */

/* Request packet */
typedef __HIL_PACKED_PRE struct PNS_IF_ADD_GENERIC_DIAG_REQ_DATA_Ttag
{
  uint32_t  ulApi;
  uint32_t  ulSlot;
  uint32_t  ulSubslot;
  uint32_t  hDiagHandle;
  uint16_t  usChannelNum;
  uint16_t  usChannelProp;
  uint16_t  usUserStructId;
  uint16_t  usReserved;
  uint16_t  usDiagDataLen;
  uint8_t   abDiagData[PNS_IF_MAX_ALARM_DATA_LEN]; /* ATTENTION: Recommended not to use more than 172 byte of data for compatibility reasons */
} __HIL_PACKED_POST PNS_IF_ADD_GENERIC_DIAG_REQ_DATA_T;

typedef __HIL_PACKED_PRE struct
{
  HIL_PACKET_HEADER_T                 tHead;
  PNS_IF_ADD_GENERIC_DIAG_REQ_DATA_T  tData;
} __HIL_PACKED_POST PNS_IF_ADD_GENERIC_DIAG_REQ_T;

/* Confirmation packet */
typedef __HIL_PACKED_PRE struct PNS_IF_ADD_GENERIC_DIAG_CNF_DATA_Ttag
{
  uint32_t  ulApi;
  uint32_t  ulSlot;
  uint32_t  ulSubslot;
  uint32_t  hDiagHandle;
  uint16_t  usChannelNum;
  uint16_t  usChannelProp;
  uint16_t  usUserStructId;
} __HIL_PACKED_POST PNS_IF_ADD_GENERIC_DIAG_CNF_DATA_T;

typedef __HIL_PACKED_PRE struct
{
  HIL_PACKET_HEADER_T                 tHead;
  PNS_IF_ADD_GENERIC_DIAG_CNF_DATA_T  tData;
} __HIL_PACKED_POST PNS_IF_ADD_GENERIC_DIAG_CNF_T;

/* packet union */
typedef union PNS_IF_ADD_GENERIC_DIAG_PCK_Ttag
{
  PNS_IF_ADD_GENERIC_DIAG_REQ_T             tReq;
  PNS_IF_ADD_GENERIC_DIAG_CNF_T             tCnf;
} PNS_IF_ADD_GENERIC_DIAG_PCK_T;


/*
 * REMOVE DIAGNOSIS RECORD
 */

/* Request packet */
typedef __HIL_PACKED_PRE struct PNS_IF_REMOVE_DIAG_REQ_DATA_Ttag
{
  uint32_t hDiagHandle;
} __HIL_PACKED_POST PNS_IF_REMOVE_DIAG_REQ_DATA_T;

typedef __HIL_PACKED_PRE struct PNS_IF_REMOVE_DIAG_REQ_Ttag
{
  HIL_PACKET_HEADER_T                       tHead;
  PNS_IF_REMOVE_DIAG_REQ_DATA_T             tData;
} __HIL_PACKED_POST PNS_IF_REMOVE_DIAG_REQ_T;

/* Confirmation packet */
typedef PNS_IF_REMOVE_DIAG_REQ_T            PNS_IF_REMOVE_DIAG_CNF_T;

/* packet union */
typedef union PNS_IF_REMOVE_DIAG_PCK_Ttag
{
  PNS_IF_REMOVE_DIAG_REQ_T                  tReq;
  PNS_IF_REMOVE_DIAG_CNF_T                  tCnf;
} PNS_IF_REMOVE_DIAG_PCK_T;


/*
 * SEND DIAGNOSIS ALARM SERVICE
 */

/* Request packet */
typedef __HIL_PACKED_PRE struct PNS_IF_SEND_DIAG_ALARM_REQ_DATA_Ttag
{
  uint32_t    ulReserved;
  uint32_t    hAlarmHandle;
  uint32_t    hDiagHandle;
} __HIL_PACKED_POST PNS_IF_SEND_DIAG_ALARM_REQ_DATA_T;

typedef __HIL_PACKED_PRE struct PNS_IF_SEND_DIAG_ALARM_REQ_Ttag
{
  /** packet header */
  HIL_PACKET_HEADER_T                       tHead;
  /** packet data */
  PNS_IF_SEND_DIAG_ALARM_REQ_DATA_T         tData;
} __HIL_PACKED_POST PNS_IF_SEND_DIAG_ALARM_REQ_T;

/* Confirmation packet */
typedef __HIL_PACKED_PRE struct PNS_IF_SEND_DIAG_ALARM_CNF_DATA_Ttag
{
  uint32_t    ulReserved;
  uint32_t    hAlarmHandle;
  /* Profinet error code, consists of ErrCode, ErrDecode, ErrCode1 and ErrCode2 */
  uint32_t    ulPnio;
} __HIL_PACKED_POST PNS_IF_SEND_DIAG_ALARM_CNF_DATA_T;

typedef __HIL_PACKED_PRE struct PNS_IF_SEND_DIAG_ALARM_CNF_Ttag
{
  /** packet header */
  HIL_PACKET_HEADER_T                       tHead;
  /** packet data */
  PNS_IF_SEND_DIAG_ALARM_CNF_DATA_T         tData;
} __HIL_PACKED_POST PNS_IF_SEND_DIAG_ALARM_CNF_T;

/* packet union */
typedef union PNS_IF_SEND_DIAG_ALARM_PCK_Ttag
{
  /** request packet */
  PNS_IF_SEND_DIAG_ALARM_REQ_T              tReq;
  /** confirmation packet */
  PNS_IF_SEND_DIAG_ALARM_CNF_T              tCnf;
} PNS_IF_SEND_DIAG_ALARM_PCK_T;


/*
 * ALARM INDICATION SERVICE
 */

/* Indication packet */
typedef __HIL_PACKED_PRE struct PNS_IF_ALARM_IND_DATA_Ttag
{
  uint32_t    hDeviceHandle;
  uint32_t    ulApi;
  uint32_t    ulSlot;
  uint32_t    ulSubslot;
  uint32_t    ulModuleId;
  uint32_t    ulSubmodId;
  uint16_t    usAlarmPriority;
  uint16_t    usAlarmType;
  uint16_t    usAlarmSequence;
  uint32_t    fDiagChannelAvailable;
  uint32_t    fDiagGenericAvailable;
  uint32_t    fDiagSubmodAvailable;
  uint32_t    fReserved;
  uint32_t    fArDiagnosisState;
  uint16_t    usUserStructId;
  uint16_t    usAlarmDataLen;
  uint8_t     abAlarmData[PNS_IF_MAX_ALARM_DATA_LEN];
} __HIL_PACKED_POST PNS_IF_ALARM_IND_DATA_T;

typedef __HIL_PACKED_PRE struct PNS_IF_ALARM_IND_Ttag
{
  /** packet header */
  HIL_PACKET_HEADER_T                       tHead;
  /** packet data */
  PNS_IF_ALARM_IND_DATA_T                   tData;
} __HIL_PACKED_POST PNS_IF_ALARM_IND_T;

/* Response packet */
typedef PNS_IF_HANDLE_PACKET_T              PNS_IF_ALARM_RSP_T;

/* packet union */
typedef union PNS_IF_ALARM_PCK_Ttag
{
  /** indication packet */
  PNS_IF_ALARM_IND_T                        tInd;
  /** response packet */
  PNS_IF_ALARM_RSP_T                        tRsp;
} PNS_IF_ALARM_PCK_T;


/*
 * SAVE STATION NAME SERVICE
 */

/* Indication packet */
typedef __HIL_PACKED_PRE struct PNS_IF_SAVE_STATION_NAME_IND_DATA_Ttag
{
  uint16_t usNameLen;
  uint8_t  bRemanent;
  uint8_t  abNameOfStation[PNS_IF_MAX_NAME_OF_STATION];
} __HIL_PACKED_POST PNS_IF_SAVE_STATION_NAME_IND_DATA_T;

typedef __HIL_PACKED_PRE struct PNS_IF_SAVE_STATION_NAME_IND_Ttag
{
  HIL_PACKET_HEADER_T                       tHead;
  PNS_IF_SAVE_STATION_NAME_IND_DATA_T       tData;
} __HIL_PACKED_POST PNS_IF_SAVE_STATION_NAME_IND_T;

/* Response packet */
typedef HIL_EMPTY_PACKET_T                  PNS_IF_SAVE_STATION_NAME_RSP_T;

/* packet union */
typedef union PNS_IF_SAVE_STATION_NAME_PCK_Ttag
{
  PNS_IF_SAVE_STATION_NAME_IND_T            tInd;
  PNS_IF_SAVE_STATION_NAME_RSP_T            tRsp;
} PNS_IF_SAVE_STATION_NAME_PCK_T;


/*
 * SAVE IP ADDRESS SERVICE
 */

/* Indication packet */
typedef __HIL_PACKED_PRE struct PNS_IF_SAVE_IP_ADDR_IND_DATA_Ttag
{
  uint32_t  ulIpAddr;
  uint32_t  ulNetMask;
  uint32_t  ulGateway;
  uint8_t   bRemanent;
} __HIL_PACKED_POST PNS_IF_SAVE_IP_ADDR_IND_DATA_T;

typedef __HIL_PACKED_PRE struct PNS_IF_SAVE_IP_ADDR_IND_Ttag
{
  HIL_PACKET_HEADER_T                       tHead;
  PNS_IF_SAVE_IP_ADDR_IND_DATA_T            tData;
} __HIL_PACKED_POST PNS_IF_SAVE_IP_ADDR_IND_T;

/* Response packet */
typedef HIL_EMPTY_PACKET_T                  PNS_IF_SAVE_IP_ADDR_RSP_T;

/* packet union */
typedef union PNS_IF_SAVE_IP_ADDR_PCK_Ttag
{
  PNS_IF_SAVE_IP_ADDR_IND_T                 tInd;
  PNS_IF_SAVE_IP_ADDR_RSP_T                 tRsp;
} PNS_IF_SAVE_IP_ADDR_PCK_T;


/*
 * APDU STATUS CHANGED SERVICE
 */

/* Indication packet */
typedef __HIL_PACKED_PRE struct PNS_IF_APDU_STATUS_CHANGED_IND_DATA_Ttag
{
  uint32_t hDeviceHandle;
  uint32_t ulStatus;
} __HIL_PACKED_POST PNS_IF_APDU_STATUS_CHANGED_IND_DATA_T;

typedef __HIL_PACKED_PRE struct PNS_IF_APDU_STATUS_CHANGED_IND_Ttag
{
  HIL_PACKET_HEADER_T                       tHead;
  PNS_IF_APDU_STATUS_CHANGED_IND_DATA_T     tData;
} __HIL_PACKED_POST PNS_IF_APDU_STATUS_CHANGED_IND_T;

/* Response packet */
typedef PNS_IF_HANDLE_PACKET_T              PNS_IF_APDU_STATUS_CHANGED_RSP_T;

/* packet union */
typedef union PNS_IF_APDU_STATUS_CHANGED_PCK_Ttag
{
  PNS_IF_APDU_STATUS_CHANGED_IND_T          tInd;
  PNS_IF_APDU_STATUS_CHANGED_RSP_T          tRsp;
} PNS_IF_APDU_STATUS_CHANGED_PCK_T;


/*
 * RETURN OF SUBMODULE SERVICE
 */

/* Request packet */
typedef __HIL_PACKED_PRE struct PNS_IF_RETURN_OF_SUB_ALARM_REQ_DATA_Ttag
{
  uint32_t  ulReserved;
  uint32_t  ulApi;
  uint32_t  ulSlot;
  uint32_t  ulSubslot;
  uint32_t  hAlarmHandle;
} __HIL_PACKED_POST PNS_IF_RETURN_OF_SUB_ALARM_REQ_DATA_T;

typedef __HIL_PACKED_PRE struct PNS_IF_RETURN_OF_SUB_ALARM_REQ_Ttag
{
  HIL_PACKET_HEADER_T                       tHead;
  PNS_IF_RETURN_OF_SUB_ALARM_REQ_DATA_T     tData;
} __HIL_PACKED_POST PNS_IF_RETURN_OF_SUB_ALARM_REQ_T;

/* Confirmation packet */
typedef __HIL_PACKED_PRE struct PNS_IF_RETURN_OF_SUB_ALARM_CNF_DATA_Ttag
{
  uint32_t ulReserved;
  uint32_t hAlarmHandle;
  /* Profinet error code, consists of ErrCode, ErrDecode, ErrCode1 and ErrCode2 */
  uint32_t ulPnio;
} __HIL_PACKED_POST PNS_IF_RETURN_OF_SUB_ALARM_CNF_DATA_T;

typedef __HIL_PACKED_PRE struct PNS_IF_RETURN_OF_SUB_ALARM_CNF_Ttag
{
  HIL_PACKET_HEADER_T                       tHead;
  PNS_IF_RETURN_OF_SUB_ALARM_CNF_DATA_T     tData;
} __HIL_PACKED_POST PNS_IF_RETURN_OF_SUB_ALARM_CNF_T;

/* packet union */
typedef union PNS_IF_RETURN_OF_SUB_ALARM_PCK_Ttag
{
  PNS_IF_RETURN_OF_SUB_ALARM_REQ_T          tReq;
  PNS_IF_RETURN_OF_SUB_ALARM_CNF_T          tCnf;
} PNS_IF_RETURN_OF_SUB_ALARM_PCK_T;


/*
 * LINK STATUS CHANGED SERVICE
 */

/* Indication packet */
typedef __HIL_PACKED_PRE struct PNS_IF_LINK_STATUS_CHANGED_IND_DATA_Ttag
{
  PNS_IF_LINK_STATUS_DATA_T  atLinkData[2];
} __HIL_PACKED_POST PNS_IF_LINK_STATUS_CHANGED_IND_DATA_T;

typedef __HIL_PACKED_PRE struct PNS_IF_LINK_STATUS_CHANGED_IND_Ttag
{
  HIL_PACKET_HEADER_T                       tHead;
  PNS_IF_LINK_STATUS_CHANGED_IND_DATA_T     tData;
} __HIL_PACKED_POST PNS_IF_LINK_STATUS_CHANGED_IND_T;

/* Response packet */
typedef HIL_EMPTY_PACKET_T                  PNS_IF_LINK_STATUS_CHANGED_RSP_T;

/* packet union */
typedef union PNS_IF_LINK_STATUS_CHANGED_PCK_Ttag
{
  PNS_IF_LINK_STATUS_CHANGED_IND_T          tInd;
  PNS_IF_LINK_STATUS_CHANGED_RSP_T          tRsp;
} PNS_IF_LINK_STATUS_CHANGED_PCK_T;


/*
 * START LED BLINKING SERVICE
 */

/* Indication packet */
typedef __HIL_PACKED_PRE struct PNS_IF_START_LED_BLINKING_IND_DATA_Ttag
{
  uint32_t ulFrequency;
} __HIL_PACKED_POST PNS_IF_START_LED_BLINKING_IND_DATA_T;

typedef __HIL_PACKED_PRE struct PNS_IF_START_LED_BLINKING_IND_Ttag
{
  HIL_PACKET_HEADER_T                       tHead;
  PNS_IF_START_LED_BLINKING_IND_DATA_T      tData;
} __HIL_PACKED_POST PNS_IF_START_LED_BLINKING_IND_T;

/* Response packet */
typedef HIL_EMPTY_PACKET_T                  PNS_IF_START_LED_BLINKING_RSP_T;

/* packet union */
typedef union PNS_IF_START_LED_BLINKING_PCK_Ttag
{
  PNS_IF_START_LED_BLINKING_IND_T           tInd;
  PNS_IF_START_LED_BLINKING_RSP_T           tRsp;
} PNS_IF_START_LED_BLINKING_PCK_T;


/*
 * STOP LED BLINKING SERVICE
 */

/* Indication packet */
typedef HIL_EMPTY_PACKET_T                  PNS_IF_STOP_LED_BLINKING_IND_T;

/* Response packet */
typedef HIL_EMPTY_PACKET_T                  PNS_IF_STOP_LED_BLINKING_RSP_T;

/* packet union */
typedef union PNS_IF_STOP_LED_BLINKING_PCK_Ttag
{
  PNS_IF_STOP_LED_BLINKING_IND_T            tInd;
  PNS_IF_STOP_LED_BLINKING_RSP_T            tRsp;
} PNS_IF_STOP_LED_BLINKING_PCK_T;


/*
 * GET STATION NAME SERVICE
 */

/* Request packet */
typedef HIL_EMPTY_PACKET_T                  PNS_IF_GET_STATION_NAME_REQ_T;

/* Confirmation packet */
typedef __HIL_PACKED_PRE struct PNS_IF_GET_STATION_NAME_CNF_DATA_Ttag
{
  uint16_t  usNameLen;
  uint8_t   abNameOfStation[PNS_IF_MAX_NAME_OF_STATION];
} __HIL_PACKED_POST PNS_IF_GET_STATION_NAME_CNF_DATA_T;

typedef __HIL_PACKED_PRE struct PNS_IF_GET_STATION_NAME_CNF_Ttag
{
  HIL_PACKET_HEADER_T                       tHead;
  PNS_IF_GET_STATION_NAME_CNF_DATA_T        tData;
} __HIL_PACKED_POST PNS_IF_GET_STATION_NAME_CNF_T;

/* packet union */
typedef union PNS_IF_GET_STATION_NAME_PCK_Ttag
{
  PNS_IF_GET_STATION_NAME_REQ_T             tReq;
  PNS_IF_GET_STATION_NAME_CNF_T             tCnf;
} PNS_IF_GET_STATION_NAME_PCK_T;


/*
 * GET IP ADDRESS SERVICE
 */

/* Request packet */
typedef HIL_EMPTY_PACKET_T                  PNS_IF_GET_IP_ADDR_REQ_T;

/* Confirmation packet */
typedef __HIL_PACKED_PRE struct PNS_IF_GET_IP_ADDR_CNF_DATA_Ttag
{
  uint32_t ulIpAddr;
  uint32_t ulNetMask;
  uint32_t ulGateway;
} __HIL_PACKED_POST PNS_IF_GET_IP_ADDR_CNF_DATA_T;

typedef __HIL_PACKED_PRE struct PNS_IF_GET_IP_ADDR_CNF_Ttag
{
  HIL_PACKET_HEADER_T                       tHead;
  PNS_IF_GET_IP_ADDR_CNF_DATA_T             tData;
} __HIL_PACKED_POST PNS_IF_GET_IP_ADDR_CNF_T;

/* packet union */
typedef union PNS_IF_GET_IP_ADDR_PCK_Ttag
{
  PNS_IF_GET_IP_ADDR_REQ_T                  tReq;
  PNS_IF_GET_IP_ADDR_CNF_T                  tCnf;
} PNS_IF_GET_IP_ADDR_PCK_T;


/*
 * USER ERROR INDICATION SERVICE
 */

/* Indication packet */
typedef __HIL_PACKED_PRE struct PNS_IF_USER_ERROR_IND_DATA_Ttag
{
  uint32_t ulErrorCode;
  uint32_t ulCommand;
} __HIL_PACKED_POST PNS_IF_USER_ERROR_IND_DATA_T;

typedef __HIL_PACKED_PRE struct PNS_IF_USER_ERROR_IND_Ttag
{
  HIL_PACKET_HEADER_T                       tHead;
  PNS_IF_USER_ERROR_IND_DATA_T              tData;
} __HIL_PACKED_POST PNS_IF_USER_ERROR_IND_T;

/* Response packet */
typedef HIL_EMPTY_PACKET_T                  PNS_IF_USER_ERROR_RSP_T;

/* packet union */
typedef union PNS_IF_USER_ERROR_PCK_Ttag
{
  PNS_IF_USER_ERROR_IND_T                   tInd;
  PNS_IF_USER_ERROR_RSP_T                   tRsp;
} PNS_IF_USER_ERROR_PCK_T;


/*
 * GET STACK DIAGNOSIS SERVICE
 */
#define  PNS_IF_STATE_UNCONFIGURED          ((uint32_t)(0x00000000UL))
#define  PNS_IF_STATE_DEVICEINFO_SET        ((uint32_t)(0x00000001UL))     /* basic information set inside the stack? */
#define  PNS_IF_STATE_STACK_STARTED         ((uint32_t)(0x00000002UL))     /* stack was started and is running? */
#define  PNS_IF_STATE_RESERVED1             ((uint32_t)(0x00000004UL))
#define  PNS_IF_STATE_API_PRESENT           ((uint32_t)(0x00000008UL))     /* at least one api present */
#define  PNS_IF_STATE_MOD0_PRESENT          ((uint32_t)(0x00000010UL))
#define  PNS_IF_STATE_MOD0SUB1_PRESENT      ((uint32_t)(0x00000020UL))
#define  PNS_IF_STATE_BUS_ON                ((uint32_t)(0x00000040UL))     /* Bus ON was set by application? */
#define  PNS_IF_STATE_COMMUNICATING         ((uint32_t)(0x00000080UL))     /* stack is in cyclic communication? */
#define  PNS_IF_STATE_CONFIG_LOCKED         ((uint32_t)(0x00000100UL))     /* the configuration was locked by application */
#define  PNS_IF_STATE_FATAL_ERROR_OCCURRED  ((uint32_t)(0x00000200UL))     /* a fatal error occurred */
#define  PNS_IF_STATE_DIAGNOSIS_PRESENT     ((uint32_t)(0x00000400UL))     /* a Profinet diagnosis record exists at stack level */
#define  PNS_IF_STATE_MAINTENANCE_REQUIRED  ((uint32_t)(0x00000800UL))     /* a Profinet maintenance required record is pending */
#define  PNS_IF_STATE_MAINTENANCE_DEMANDED  ((uint32_t)(0x00001000UL))     /* a Profinet maintenance demanded record is pending */
#define  PNS_IF_STATE_FO_MAINT_DEM_PORT_0   ((uint32_t)(0x00002000UL))     /* a Profinet FiberOptic Maintenance demanded record exists for Port 0 */
#define  PNS_IF_STATE_FO_MAINT_REQ_PORT_0   ((uint32_t)(0x00004000UL))     /* a Profinet FiberOptic Maintenance required record exists for Port 0 */
#define  PNS_IF_STATE_FO_MAINT_DEM_PORT_1   ((uint32_t)(0x00008000UL))     /* a Profinet FiberOptic Maintenance demanded record exists for Port 1 */
#define  PNS_IF_STATE_FO_MAINT_REQ_PORT_1   ((uint32_t)(0x00010000UL))     /* a Profinet FiberOptic Maintenance required record exists for Port 1 */

typedef enum PNS_IF_LINKSTATES_Ttag {
  PNS_IF_LINK_STATE_LINK             = 0x00000001,
  PNS_IF_LINK_STATE_LOWSPEED         = 0x00000002,
  PNS_IF_LINK_STATE_NOLINK           = 0x00000003
} PNS_IF_LINKSTATES_T;

typedef enum PNS_IF_CONFIG_STATES_Ttag {
  PNS_IF_CONFIG_STATE_UNCONFIGURED   = 0,
  PNS_IF_CONFIG_STATE_DBM,
  PNS_IF_CONFIG_STATE_DBM_ERROR,
  PNS_IF_CONFIG_STATE_APPL,
  PNS_IF_CONFIG_STATE_APPL_RUNNING,
  PNS_IF_CONFIG_STATE_APPL_ERROR,
  PNS_IF_CONFIG_STATE_WARMSTART,
  PNS_IF_CONFIG_STATE_WARMSTART_RUNNING,
  PNS_IF_CONFIG_STATE_WARMSTART_ERROR
} PNS_IF_CONFIG_STATES_T;


/* Request packet */
typedef HIL_EMPTY_PACKET_T                  PNS_IF_GET_DIAGNOSIS_REQ_T;

/* Confirmation packet */
typedef __HIL_PACKED_PRE struct PNS_IF_STATUS_Ttag
{
  uint32_t      ulPnsState;
  uint32_t      ulLastRslt;
  uint32_t      ulLinkState;
  uint32_t      ulConfigState;
  uint32_t      ulCommunicationState;
  uint32_t      ulCommunicationError;
  uint32_t      aulLineDelay[2];
} __HIL_PACKED_POST PNS_IF_STATUS_T;

typedef __HIL_PACKED_PRE struct PNS_IF_GET_DIAGNOSIS_CNF_Ttag
{
  HIL_PACKET_HEADER_T                       tHead;
  PNS_IF_STATUS_T                           tData;
} __HIL_PACKED_POST PNS_IF_GET_DIAGNOSIS_CNF_T;

/* packet union */
typedef union PNS_IF_GET_DIAGNOSIS_PCK_Ttag
{
  PNS_IF_GET_DIAGNOSIS_REQ_T                tReq;
  PNS_IF_GET_DIAGNOSIS_CNF_T                tCnf;
} PNS_IF_GET_DIAGNOSIS_PCK_T;


/* confirmation packet */
typedef __HIL_PACKED_PRE struct PNS_IF_GET_XMAC_DIAGNOSIS_DATA_Ttag
{

  uint8_t   bMajor;
  uint8_t   bMinor;
  uint8_t   bBuild;
  uint8_t   bRevision;

  /* placeholder for real data */
  uint32_t  aulBuffer[];
} __HIL_PACKED_POST PNS_IF_GET_XMAC_DIAGNOSIS_DATA_T;

typedef __HIL_PACKED_PRE struct PNS_IF_GET_XMAC_DIAGNOSIS_REQ_Ttag
{
  HIL_PACKET_HEADER_T                       tHead;

  uint8_t                                 abReserved[sizeof(PNS_IF_GET_XMAC_DIAGNOSIS_DATA_T)];
} __HIL_PACKED_POST PNS_IF_GET_XMAC_DIAGNOSIS_REQ_T;

typedef __HIL_PACKED_PRE struct PNS_IF_GET_XMAC_DIAGNOSIS_CNF_Ttag
{
  HIL_PACKET_HEADER_T                       tHead;
  PNS_IF_GET_XMAC_DIAGNOSIS_DATA_T          tData;
} __HIL_PACKED_POST PNS_IF_GET_XMAC_DIAGNOSIS_CNF_T;

/* packet union */

typedef union
{
  PNS_IF_GET_XMAC_DIAGNOSIS_REQ_T tReq;
  PNS_IF_GET_XMAC_DIAGNOSIS_CNF_T tCnf;
} PNS_IF_GET_XMAC_DIAGNOSIS_PCK_T ;

/*
 * SET PORT MAC ADDRESS SERVICE
 */

/* Request packet */
typedef uint8_t PORT_MAC_ADDR_T[6];

typedef __HIL_PACKED_PRE struct PNS_IF_SET_PORT_MAC_REQ_DATA_Ttag
{
  PORT_MAC_ADDR_T atPortMacAddr[2];
} __HIL_PACKED_POST PNS_IF_SET_PORT_MAC_REQ_DATA_T;

typedef __HIL_PACKED_PRE struct PNS_IF_SET_PORT_MAC_REQ_Ttag
{
  HIL_PACKET_HEADER_T                       tHead;
  PNS_IF_SET_PORT_MAC_REQ_DATA_T            tData;
} __HIL_PACKED_POST PNS_IF_SET_PORT_MAC_REQ_T;

/* Confirmation packet */
typedef HIL_EMPTY_PACKET_T                  PNS_IF_SET_PORT_MAC_CNF_T;

/* packet union */
typedef union PNS_IF_SET_PORT_MAC_PCK_Ttag
{
  PNS_IF_SET_PORT_MAC_REQ_T                tReq;
  PNS_IF_SET_PORT_MAC_CNF_T                tCnf;
} PNS_IF_SET_PORT_MAC_PCK_T;


/*
 * RESET  FACTORY SETTINGS SERVICE FROM STACK
 */

/* possible values for usResetCode */
#define PNS_IF_RESET_FACTORY_SETTINGS_IF_APPLICATION     2
#define PNS_IF_RESET_FACTORY_SETTINGS_IF_COMMUNICATION   4
#define PNS_IF_RESET_FACTORY_SETTINGS_IF_ENGINEERING     6
#define PNS_IF_RESET_FACTORY_SETTINGS_IF_ALL             8
#define PNS_IF_RESET_FACTORY_SETTINGS_DEVICE_ALL        16
#define PNS_IF_RESET_FACTORY_SETTINGS_DEVICE_RESTORE    18

typedef __HIL_PACKED_PRE struct
{
  /** This field has the same coding as the DCP BlockQualifier with option ControlOption and suboption SuboptionResetToFactory.*/
  uint16_t usResetCode;
} __HIL_PACKED_POST PNS_IF_RESET_FACTORY_SETTINGS_IND_DATA_T;

/* Indication packet */
typedef __HIL_PACKED_PRE struct
{
  HIL_PACKET_HEADER_T                       tHead;
  PNS_IF_RESET_FACTORY_SETTINGS_IND_DATA_T  tData;
} __HIL_PACKED_POST PNS_IF_RESET_FACTORY_SETTINGS_IND_T;


/* Response packet */
typedef HIL_EMPTY_PACKET_T                  PNS_IF_RESET_FACTORY_SETTINGS_RSP_T;

/* packet union */
typedef union PNS_IF_RESET_FACTORY_SETTINGS_PCK_Ttag
{
  PNS_IF_RESET_FACTORY_SETTINGS_IND_T       tInd;
  PNS_IF_RESET_FACTORY_SETTINGS_RSP_T       tRsp;
} PNS_IF_RESET_FACTORY_SETTINGS_PCK_T;


/*
 * RESET  FACTORY SETTINGS SERVICE FROM APPLICATION
 */

/* Request packet */
typedef HIL_EMPTY_PACKET_T                  PNS_IF_RESET_FACTORY_SETTINGS_REQ_T;

/* Confirmation packet */
typedef HIL_EMPTY_PACKET_T                  PNS_IF_RESET_FACTORY_SETTINGS_CNF_T;

/* packet union */
typedef union PNS_IF_RESET_FACTORY_SETTINGS_APP_PCK_Ttag
{
  PNS_IF_RESET_FACTORY_SETTINGS_REQ_T       tReq;
  PNS_IF_RESET_FACTORY_SETTINGS_CNF_T       tCnf;
} PNS_IF_RESET_FACTORY_SETTINGS_APP_PCK_T;


/*
 * ABORT CONNECTION SERVICE
 */

typedef __HIL_PACKED_PRE struct
{
  uint32_t hDeviceHandle;
  /** the error status to report on bus. This field is ignored if the packet is smaller
   *  than 8 Bytes. Stack versions < 3.5.1.1 also ignore this field */
  uint32_t ulPnio;
} __HIL_PACKED_POST PNS_IF_ABORT_CONNECTION_REQ_DATA_T;

/* Request packet */
typedef __HIL_PACKED_PRE struct
{
  HIL_PACKET_HEADER_T                       tHead;
  PNS_IF_ABORT_CONNECTION_REQ_DATA_T        tData;
} __HIL_PACKED_POST PNS_IF_ABORT_CONNECTION_REQ_T;

/* Confirmation packet */
typedef PNS_IF_HANDLE_PACKET_T              PNS_IF_ABORT_CONNECTION_CNF_T;

/* packet union */
typedef union PNS_IF_ABORT_CONNECTION_PCK_Ttag
{
  PNS_IF_ABORT_CONNECTION_REQ_T             tReq;
  PNS_IF_ABORT_CONNECTION_CNF_T             tCnf;
} PNS_IF_ABORT_CONNECTION_PCK_T;



/*
 * STORE REMANENT DATA SERVICE -> used to indicate data to be stored
 */

/* Indication packet */
typedef __HIL_PACKED_PRE struct PNS_IF_STORE_REMANENT_DATA_IND_DATA_Ttag
{
  /** this is only the first byte, many others may follow */
  uint8_t  abData[1];
} __HIL_PACKED_POST PNS_IF_STORE_REMANENT_DATA_IND_DATA_T;

typedef __HIL_PACKED_PRE struct PNS_IF_STORE_REMANENT_DATA_IND_Ttag
{
  HIL_PACKET_HEADER_T                       tHead;
  PNS_IF_STORE_REMANENT_DATA_IND_DATA_T     tData;
} __HIL_PACKED_POST PNS_IF_STORE_REMANENT_DATA_IND_T;

/* Response packet */
typedef HIL_EMPTY_PACKET_T                  PNS_IF_STORE_REMANENT_DATA_RES_T;

/* packet union */
typedef union PNS_IF_STORE_REMANENT_DATA_PCK_Ttag
{
  PNS_IF_STORE_REMANENT_DATA_IND_T          tInd;
  PNS_IF_STORE_REMANENT_DATA_RES_T          tRes;
} PNS_IF_STORE_REMANENT_DATA_PCK_T;

/*
 * LOAD REMANENT DATA SERVICE -> used to load remanent data back into the stack
 */

/* request packet */
typedef __HIL_PACKED_PRE struct
{
  /** this is only the first byte, many others may follow */
  uint8_t  abData[1];
} __HIL_PACKED_POST PNS_IF_LOAD_REMANENT_DATA_REQ_DATA_T;

typedef __HIL_PACKED_PRE struct
{
  HIL_PACKET_HEADER_T                       tHead;
  PNS_IF_LOAD_REMANENT_DATA_REQ_DATA_T      tData;
} __HIL_PACKED_POST PNS_IF_LOAD_REMANENT_DATA_REQ_T;

/* Response packet */
typedef HIL_EMPTY_PACKET_T                  PNS_IF_LOAD_REMANENT_DATA_CNF_T;

/* packet union */
typedef union PNS_IF_LOAD_REMANENT_DATA_PCK_Ttag
{
  PNS_IF_LOAD_REMANENT_DATA_REQ_T          tReq;
  PNS_IF_LOAD_REMANENT_DATA_CNF_T          tCnf;
} PNS_IF_LOAD_REMANENT_DATA_PCK_T;


/*
 * WARMSTART SERVICE  with autoconfiguration
 */

/* ATTENTION: It is NOT recommended to use this service */
/* Using it will make the stack automatically adapt its module configuration */
/* to the configuration expected by IO-Controller. */
/* The stack will internally plug all modules and calculate the DPM-offsets */
/* starting at offset 0 the offsets will be ascending according to the slot and subslot */
/* the user will NOT have any information about the modules and datatypes behind them */
/* the user will only see the total amount of IO-Data */

#define PNS_IF_GSDML_SLOT0_MODULEID                  0x11
#define PNS_IF_GSDML_SLOT0_SUBSLOT1_SUBMODULEID      0x10
#define PNS_IF_GSDML_SLOT0_SUBSLOT8000_SUBMODULEID   0x11
#define PNS_IF_GSDML_SLOT0_SUBSLOT8001_SUBMODULEID   0x12
#define PNS_IF_GSDML_SLOT0_SUBSLOT8002_SUBMODULEID   0x13
#define PNS_IF_GSDML_INSTANCE_ID                     1

#define PNS_IF_DEFAULT_DIAG_RECORD_AMOUNT            256


/**
 * Set IO Image Service
 */
typedef enum PNS_IF_IO_EVENT_Etag
{
  PNS_IF_IO_EVENT_RESERVED                    = 0x00000000,
  PNS_IF_IO_EVENT_NEW_FRAME                   = 0x00000001,
  PNS_IF_IO_EVENT_CONSUMER_UPDATE_REQUIRED    = 0x00000002,
  PNS_IF_IO_EVENT_PROVIDER_UPDATE_REQUIRED    = 0x00000003,
  PNS_IF_IO_EVENT_FRAME_SENT                  = 0x00000004,
  PNS_IF_IO_EVENT_CONSUMER_UPDATE_DONE        = 0x00000005,
  PNS_IF_IO_EVENT_PROVIDER_UPDATE_DONE        = 0x00000006,
  PNS_IF_IO_EVENT_MAX,                                      /**< Number of defined events **/
} PNS_IF_IO_EVENT_E;

/**
 * Set IOXS CONFIG request
 */

typedef enum PNS_IF_IOPS_MODE_Etag
{
  PNS_IF_IOPS_DISABLED = 0,
  PNS_IF_IOPS_BITWISE,
  PNS_IF_IOPS_BYTEWISE,
} PNS_IF_IOPS_MODE_E;

typedef enum PNS_IF_IOCS_MODE_Etag
{
  PNS_IF_IOCS_DISABLED = 0,
  PNS_IF_IOCS_BITWISE,
  PNS_IF_IOCS_BYTEWISE,
} PNS_IF_IOCS_MODE_E;

typedef __HIL_PACKED_PRE struct PNS_IF_SET_IOXS_CONFIG_DATA_Ttag
{
  uint32_t    ulIOPSMode;
  uint32_t    ulConsImageIOPSOffset;
  uint32_t    ulReserved1;
  uint32_t    ulProvImageIOPSOffset;

  uint32_t    ulIOCSMode;
  uint32_t    ulConsImageIOCSOffset;
  uint32_t    ulReserved2;
  uint32_t    ulProvImageIOCSOffset;
} __HIL_PACKED_POST PNS_IF_SET_IOXS_CONFIG_DATA_T;

typedef __HIL_PACKED_PRE struct PNS_IF_SET_IOXS_CONFIG_REQ_Ttag
{
  HIL_PACKET_HEADER_T             tHead;
  PNS_IF_SET_IOXS_CONFIG_DATA_T   tData;
} __HIL_PACKED_POST PNS_IF_SET_IOXS_CONFIG_REQ_T ;

typedef __HIL_PACKED_PRE struct PNS_IF_SET_IOXS_CONFIG_CNF_Ttag
{
  HIL_PACKET_HEADER_T             tHead;
} __HIL_PACKED_POST PNS_IF_SET_IOXS_CONFIG_CNF_T ;

typedef union PNS_IF_SET_IOXS_CONFIG_PCK_Ttag
{
  PNS_IF_SET_IOXS_CONFIG_REQ_T           tReq;
  PNS_IF_SET_IOXS_CONFIG_CNF_T           tCnf;
} PNS_IF_SET_IOXS_CONFIG_PCK_T ;


/**
 * Configure Signal request
 */

/** Only define the fieldbus specific part of tData here.
 * See IOSignals_Public for the whole packet
 */
typedef __HIL_PACKED_PRE struct PNS_IF_CONFIGURE_SIGNAL_REQ_DATA_HEAD_Ttag
{
  /** Usually here is the device handle, we dont need it for this request,
   *  just to have the same layout. Set to Zero */
  uint32_t  ulReserved1;
  /** Api of the submodule the io signal configuration is for. */
  uint32_t  ulApi;
  /** Slot of the submodule the io signal configuration is for. */
  uint32_t  ulSlot;
  /** Subslot of the submodule the io signal configuration is for. */
  uint32_t  ulSubslot;
  /** Unused for Profinet device stack. Set to Zero.*/
  uint32_t  ulReserved2;
  /** Unused for Profinet device stack. Set to Zero.*/
  uint32_t  ulReserved3;
  /** Unused for Profinet device stack. Set to Zero.*/
  uint32_t  ulReserved4;
  /** Unused for Profinet device stack. Set to Zero.*/
  uint32_t  ulReserved5;
} __HIL_PACKED_POST PNS_IF_CONFIGURE_SIGNAL_REQ_DATA_HEAD_T;


/*
 * @defgroup im_requests I&M Related Requests
 *
 * @{
 */

/**
 * @brief I&M0 Filter Data Structure
 */
#define PNS_IF_IM0_FILTER_DATA_HAS_IM_DATA ((uint32_t)0x00000001) /**> The submodule has discrete I&M Data */
#define PNS_IF_IM0_FILTER_DATA_MODULE_REF  ((uint32_t)0x00000002) /**> The submodule represents the whole module in terms of I&M */
#define PNS_IF_IM0_FILTER_DATA_DEVICE_REF  ((uint32_t)0x00000004) /**> The submodule represents the whole device in terms of I&M */

typedef __HIL_PACKED_PRE struct PNS_IF_IM0_FILTER_DATA_Ttag
{
  /** api of the module */
  uint32_t    ulApi;
  /** slot of the module */
  uint16_t    usSlot;
  /** subslot of the module */
  uint16_t    usSubslot;
  /** Flags (if ulFlags == 0 module shall be omitted) */
  uint32_t    ulFlags;
} __HIL_PACKED_POST PNS_IF_IM0_FILTER_DATA_T;

/**
 * @brief I&M0 Data Structure
 *
 */
typedef __HIL_PACKED_PRE struct PNS_IF_IM0_DATA_Ttag
{
  /** Unused on Profinet, set to zero */
  uint8_t   abManufacturerSpecific[10];
  /** Manufacturer ID as assigned by PNIO/PROFIBUS office */
  uint16_t  usManufacturerId;
  /** Order ID. (Space(0x20) padded) */
  uint8_t   abOrderId[20];
  /** Serial number (Space(0x20) padded) */
  uint8_t   abSerialNumber[16];
  /** Hardware revision */
  uint16_t  usHardwareRevision;
  /** Software revision PrefixX.Y.Z*/
  struct {
    /** Type Recogninition / Prefix
     *
     * Allowed values:
     * -# 'V' officially released version
     * -# 'R' Revision
     * -# 'P' Prototype
     * -# 'U' Under Test (field test)
     * -# 'T' Test Device
     */
    uint8_t bPrefix;
    /** Functional enhancement */
    uint8_t bX;
    /** Bug fix */
    uint8_t bY;
    /** Internal change */
    uint8_t bZ;
  } tSoftwareRevision;
  /** Revision counter
   *
   * To be incremented on each change of hardware or
   * parameter change*/
  uint16_t  usRevisionCounter;
  /** Profile ID (default 0xF600)*/
  uint16_t  usProfileId;
  /** Profile specific type (optional)*/
  uint16_t  usProfileSpecificType;
  /** I&M Version */
  uint16_t  usIMVersion;
  /** I&M Supported
   *
   * -# Bit 0: Profile specific I&M
   * -# Bit 1: I&M 1 supported
   * -# Bit 2: I&M 2 supported
   * -# Bit 3: I&M 3 supported
   * -# Bit 4: I&M 4 supported
   * -# .... (Remaining I&M reserved)
   * */
  uint16_t  usIMSupported;
} __HIL_PACKED_POST PNS_IF_IM0_DATA_T;

/**
 * @brief I&M1 Data Structure
 */
typedef __HIL_PACKED_PRE struct PNS_IF_IM1_DATA_Ttag
{
  /** Unused on Profinet, set to zero */
  uint8_t   abManufacturerSpecific[10];
  /** TAG-Function. (space(0x20) padded)
   * Default: Filled with 0x20 (space)
   * */
  uint8_t   abTagFunction[32];
  /** TAG-Location. (space(0x20) padded)
   * Default: Filled with 0x20 (space)
   * */
  uint8_t   abTagLocation[22];
}__HIL_PACKED_POST PNS_IF_IM1_DATA_T;

/**
 * @brief I&M2 Data Structure
 */
typedef __HIL_PACKED_PRE struct PNS_IF_IM2_DATA_Ttag
{
  /** Unused on Profinet, set to zero */
  uint8_t   abManufacturerSpecific[10];
  /** Installation Date (space(0x20) padded)
   *
   * Default: Filled with 0x20 (space)
   * Format:
   * -# YYYY-MM-DD or
   * -# YYYY-MM-DD hh::mm
   * */
  uint8_t   abInstallationDate[16];
  /** Reserved, set to zero */
  uint8_t   abReserved[38];

}__HIL_PACKED_POST PNS_IF_IM2_DATA_T;

/**
 * @brief I&M3 Data Structure
 */
typedef __HIL_PACKED_PRE struct PNS_IF_IM3_DATA_Ttag
{
  /** Unused on Profinet, set to zero */
  uint8_t   abManufacturerSpecific[10];
  /** Descriptor (Comment). (space(0x20) padded)
   * Default: Filled with 0x20 (space)
   * */
  uint8_t   abDescriptor[54];
}__HIL_PACKED_POST PNS_IF_IM3_DATA_T;

/**
 * @brief I&M4 Data Structure
 */
typedef __HIL_PACKED_PRE struct PNS_IF_IM4_DATA_Ttag
{
  /** Unused on Profinet, set to zero */
  uint8_t   abManufacturerSpecific[10];
  /** Signature. (space(0x20) padded)
   * Default: Filled with 0x00 (zero)
   * */
  uint8_t   abSignature[54];
}__HIL_PACKED_POST PNS_IF_IM4_DATA_T;

typedef __HIL_PACKED_PRE struct PNS_IF_IM_SWVERSION_Ttag
{
  uint8_t bPrefix;
  uint8_t bX;
  uint8_t bY;
  uint8_t bZ;
} __HIL_PACKED_POST PNS_IF_IM_SWVERSION_T;

/** I&M5 packet data structure */
typedef __HIL_PACKED_PRE struct PNS_IF_IM5_DATA_Ttag
{
  uint8_t               abAnnotation[64];
  uint8_t               abOrderId[64];
  uint16_t              usVendorId;
  uint8_t               abSerialNumber[16];
  uint16_t              usHardwareRevision;
  PNS_IF_IM_SWVERSION_T tSoftwareRevision;
} __HIL_PACKED_POST PNS_IF_IM5_DATA_T;


/**
 * Read I&M service
 */
typedef enum
{
  PNS_IF_IM_TYPE_IM0       = 0,
  PNS_IF_IM_TYPE_IM1       = 1,
  PNS_IF_IM_TYPE_IM2       = 2,
  PNS_IF_IM_TYPE_IM3       = 3,
  PNS_IF_IM_TYPE_IM4       = 4,
  PNS_IF_IM_TYPE_IM5       = 5,
  PNS_IF_IM_TYPE_IM0FILTER = 255,
} PNS_IF_IM_TYPE_E;

typedef __HIL_PACKED_PRE struct PNS_IF_READ_IM_IND_DATA_Ttag
{
  /** api of the submodule the i&m data shall be read from */
  uint32_t    ulApi;
  /** slot of the submodule the i&m data shall be read from */
  uint16_t    usSlot;
  /** subslot of the submodule the i&m data shall be read from */
  uint16_t    usSubslot;
  /** type of i&m to read */
  uint8_t     bIMType;
  /** unused */
  uint8_t     abReserved[3];
} __HIL_PACKED_POST PNS_IF_READ_IM_IND_DATA_T;

typedef union
{
  /** Array of submodules describing I&M state of submodules
   * ( grow Array as required)*/
  PNS_IF_IM0_FILTER_DATA_T  atIM0FilterData[1];
  /** I&M0 Data */
  PNS_IF_IM0_DATA_T         tIM0;
  /** I&M1 Data */
  PNS_IF_IM1_DATA_T         tIM1;
  /** I&M2 Data */
  PNS_IF_IM2_DATA_T         tIM2;
  /** I&M3 Data */
  PNS_IF_IM3_DATA_T         tIM3;
  /** I&M4 Data */
  PNS_IF_IM4_DATA_T         tIM4;
  /** I&M5 Data */
  PNS_IF_IM5_DATA_T         tIM5;
} PNS_IF_READ_IM_RES_DATA_ITEMS_T;

typedef __HIL_PACKED_PRE struct PNS_IF_READ_IM_RES_DATA_Ttag
{
  /** api of the submodule the i&m data shall be read from */
  uint32_t    ulApi;
  /** slot of the submodule the i&m data shall be read from */
  uint16_t    usSlot;
  /** subslot of the submodule the i&m data shall be read from */
  uint16_t    usSubslot;
  /** type of i&m read */
  uint8_t     bIMType;
  /* unused, set to zero */
  uint8_t     abReserved[3];
  /** union of I&M Data */
  PNS_IF_READ_IM_RES_DATA_ITEMS_T tData;
} __HIL_PACKED_POST PNS_IF_READ_IM_RES_DATA_T;


typedef __HIL_PACKED_PRE struct PNS_IF_READ_IM_IND_Ttag
{
  /** packet header */
  HIL_PACKET_HEADER_T         tHead;
  /** packet data */
  PNS_IF_READ_IM_IND_DATA_T   tData;
} __HIL_PACKED_POST PNS_IF_READ_IM_IND_T;

typedef __HIL_PACKED_PRE struct PNS_IF_READ_IM_RES_Ttag
{
  /** packet header */
  HIL_PACKET_HEADER_T         tHead;
  /** packet data */
  PNS_IF_READ_IM_RES_DATA_T   tData;
} __HIL_PACKED_POST PNS_IF_READ_IM_RES_T;

typedef union PNS_IF_READ_IM_PCK_Ttag
{
  /** indication */
  PNS_IF_READ_IM_IND_T        tInd;
  /** response */
  PNS_IF_READ_IM_RES_T        tRsp;
} PNS_IF_READ_IM_PCK_T;

/*
 * Write I&M Service
 */
typedef __HIL_PACKED_PRE struct PNS_IF_WRITE_IM_IND_DATA_Ttag
{
  /** api of the submodule the i&m data shall be read from */
  uint32_t    ulApi;
  /** slot of the submodule the i&m data shall be read from */
  uint16_t    usSlot;
  /** subslot of the submodule the i&m data shall be read from */
  uint16_t    usSubslot;
  /** type of i&m to write */
  uint8_t     bIMType;
  /* unused, set to zero */
  uint8_t     abReserved[3];
  /** union of I&M Data */
  union {
    /** I&M1 Data */
    PNS_IF_IM1_DATA_T         tIM1;
    /** I&M2 Data */
    PNS_IF_IM2_DATA_T         tIM2;
    /** I&M3 Data */
    PNS_IF_IM3_DATA_T         tIM3;
    /** I&M4 Data */
    PNS_IF_IM4_DATA_T         tIM4;
  } tData;
} __HIL_PACKED_POST PNS_IF_WRITE_IM_IND_DATA_T;

typedef __HIL_PACKED_PRE struct PNS_IF_WRITE_IM_RES_DATA_Ttag
{
  /** api of the submodule the i&m data shall be read from */
  uint32_t    ulApi;
  /** slot of the submodule the i&m data shall be read from */
  uint16_t    usSlot;
  /** subslot of the submodule the i&m data shall be read from */
  uint16_t    usSubslot;
  /** type of i&m written */
  uint8_t     bIMType;
  /* unused, set to zero */
  uint8_t     abReserved[3];
} __HIL_PACKED_POST PNS_IF_WRITE_IM_RES_DATA_T;


typedef __HIL_PACKED_PRE struct PNS_IF_WRITE_IM_IND_Ttag
{
  /** packet header */
  HIL_PACKET_HEADER_T         tHead;
  /** packet data */
  PNS_IF_WRITE_IM_IND_DATA_T  tData;
} __HIL_PACKED_POST PNS_IF_WRITE_IM_IND_T;

typedef __HIL_PACKED_PRE struct PNS_IF_WRITE_IM_RES_Ttag
{
  /** packet header */
  HIL_PACKET_HEADER_T         tHead;
  /** packet data */
  PNS_IF_WRITE_IM_RES_DATA_T  tData;
} __HIL_PACKED_POST PNS_IF_WRITE_IM_RES_T;

typedef union PNS_IF_WRITE_IM_PCK_Ttag
{
  /** indication */
  PNS_IF_WRITE_IM_IND_T       tInd;
  /** response */
  PNS_IF_WRITE_IM_RES_T       tRsp;
} PNS_IF_WRITE_IM_PCK_T;



/**
 * Parameterization Speedup Supported Indication to application
 */
typedef __HIL_PACKED_PRE struct PNS_IF_PARAMET_SPEEDUP_SUPPORTED_IND_DATA_Ttag
{
  /** received UUID send to application */
  HIL_UUID_T tFSUuid;

} __HIL_PACKED_POST PNS_IF_PARAMET_SPEEDUP_SUPPORTED_IND_DATA_T;


typedef __HIL_PACKED_PRE struct PNS_IF_PARAMET_SPEEDUP_SUPPORTED_IND_Ttag
{
  /** packet header */
  HIL_PACKET_HEADER_T                           tHead;
  /** packet data */
  PNS_IF_PARAMET_SPEEDUP_SUPPORTED_IND_DATA_T   tData;
} __HIL_PACKED_POST PNS_IF_PARAMET_SPEEDUP_SUPPORTED_IND_T;

typedef HIL_EMPTY_PACKET_T   PNS_IF_PARAMET_SPEEDUP_SUPPORTED_RES_T;


typedef union PNS_IF_PARAMET_SPEEDUP_SUPPORTED_PCK_Ttag
{
  /** indication */
  PNS_IF_PARAMET_SPEEDUP_SUPPORTED_IND_T        tInd;
  /** response */
  PNS_IF_PARAMET_SPEEDUP_SUPPORTED_RES_T        tRsp;
} PNS_IF_PARAMET_SPEEDUP_SUPPORTED_PCK_T;

/**
 * Parameter Begin Indication Service
 */
typedef __HIL_PACKED_PRE struct
{
  /** API of the submodule */
  uint32_t ulApi;
  /** slot the submodules resides */
  uint16_t usSlot;
  /** subslot the submodules resides */
  uint16_t usSubslot;
} __HIL_PACKED_POST PNS_IF_PARAM_BEGIN_SUBMODULE_ELEM_T;

typedef __HIL_PACKED_PRE struct
{
  /** the device handle of AR issued the ParamBegin-Service */
  uint32_t                            hDeviceHandle;
  /** the amount of submodules contained in atSubm  */
  uint32_t                            ulSubmCnt;
  /** the submodules that are affected by this ParamBegin sequence */
  PNS_IF_PARAM_BEGIN_SUBMODULE_ELEM_T   atSubm[];
} __HIL_PACKED_POST PNS_IF_PARAM_BEGIN_IND_DATA_T;

typedef __HIL_PACKED_PRE struct
{
  /** packet header */
  HIL_PACKET_HEADER_T             tHead;
  /** packet data */
  PNS_IF_PARAM_BEGIN_IND_DATA_T   tData;
} __HIL_PACKED_POST PNS_IF_PARAM_BEGIN_IND_T;

typedef PNS_IF_HANDLE_PACKET_T    PNS_IF_PARAM_BEGIN_RES_T;

typedef union
{
  /** indication */
  PNS_IF_PARAM_BEGIN_IND_T  tInd;
  /** response */
  PNS_IF_PARAM_BEGIN_RES_T  tRsp;
} PNS_IF_PARAM_BEGIN_PCK_T;

/**
 * Set Submodule state Service
 */
/* the submodule is locked by application */
/* not yet supported by the stack, for future use */
#define PNS_IF_SET_SUBM_STATE_SUBM_SUPERORD_LOCKED     (1)
/* the submodule is not yet ready for data exchange but not locked */
#define PNS_IF_SET_SUBM_STATE_SUBM_APPL_READY_PENDING  (2)
/* the submodule is no longer locked */
#define PNS_IF_SET_SUBM_STATE_SUBM_OKAY                (0)

typedef __HIL_PACKED_PRE struct PNS_IF_SET_SUBM_STATE_SUBMBLOCK_Ttag
{
  /* the API the submodule belongs to */
  uint32_t ulApi;
  /* the slot the submodule resides */
  uint16_t usSlot;
  /* the subslot the submodule resides */
  uint16_t usSubslot;
  /* the submodule state (see above) */
  uint16_t usSubmState;
  /* the module state, reserved for future use! */
  uint16_t usModuleState;
} __HIL_PACKED_POST PNS_IF_SET_SUBM_STATE_SUBMBLOCK_T;

typedef __HIL_PACKED_PRE struct PNS_IF_SET_SUBM_STATE_DATA_REQ_Ttag
{
  /* amount of submodules contained in this packet */
  uint32_t                         ulSubmCnt;
  /* the first of ulSubmCnt submodule datasets */
  PNS_IF_SET_SUBM_STATE_SUBMBLOCK_T  atSubm[98];
} __HIL_PACKED_POST PNS_IF_SET_SUBM_STATE_DATA_REQ_T;

typedef __HIL_PACKED_PRE struct PNS_IF_SET_SUBM_STATE_REQ_Ttag
{
  HIL_PACKET_HEADER_T                tHead;
  PNS_IF_SET_SUBM_STATE_DATA_REQ_T   tData;
} __HIL_PACKED_POST PNS_IF_SET_SUBM_STATE_REQ_T;

typedef HIL_EMPTY_PACKET_T   PNS_IF_SET_SUBM_STATE_CNF_T;


typedef union PNS_IF_SET_SUBM_STATE_PCK_Ttag
{
  /** request */
  PNS_IF_SET_SUBM_STATE_REQ_T        tReq;
  /** response */
  PNS_IF_SET_SUBM_STATE_CNF_T        tCnf;
} PNS_IF_SET_SUBM_STATE_PCK_T;
/** @} */


/**
 * @defgroup pns_api_event_ind Event Service
 *
 * @brief This service is used to indicate events reported by the event callback
 *        to the host application across the DPM.
 *
 * If an event indication is pending at the host application (no event response
 * returned back to firmware yet) new events are not reported using another event
 * indication but counted. These unreported events will be reported after the
 * event response has been sent to the firmware. This prevents flooding the
 * host application with events.
 *
 * Furthermore events for which other signaling methods exist will not generate
 * an explicit event indication. These are:
 * * PNS_IF_IO_EVENT_CONSUMER_UPDATE_DONE: Reported by DPM Input Handshake
 * * PNS_IF_IO_EVENT_PROVIDER_UPDATE_DONE: Reported by DPM Output Handshake
 *
 * @{
 */

/**
 * @brief Event Indication Data
 */
typedef __HIL_PACKED_PRE struct
{
  /** For each event the number of events occurred since the last event
   * indication. */
  uint16_t  ausEventCnt[PNS_IF_IO_EVENT_MAX];
} __HIL_PACKED_POST PNS_IF_EVENT_IND_DATA_T;

/**
 * @brief Event Indication
 */
typedef __HIL_PACKED_PRE struct
{
  /** packet header */
  HIL_PACKET_HEADER_T         tHead;
  /** packet data */
  PNS_IF_EVENT_IND_DATA_T   tData;
} __HIL_PACKED_POST PNS_IF_EVENT_IND_T;

/**
 * @brief Event Response
 */
typedef HIL_EMPTY_PACKET_T PNS_IF_EVENT_RSP_T;

/**
 * @brief Event Service Packet Union
 */
typedef union
{
  /** indication */
  PNS_IF_EVENT_IND_T        tInd;
  /** response */
  PNS_IF_EVENT_RSP_T        tRsp;
} PNS_IF_EVENT_PCK_T;

/** @} */





/**
 * @defgroup pns_api_get_configured_submodules Get Configured Submodules service
 *
 * @{
 */

/** Request packet for Get Configured Submodules service */
typedef HIL_EMPTY_PACKET_T  PNS_IF_GET_CONFIGURED_SUBM_REQ_T;

/** Information entry in Get Configured Submodules confirmation packet */
typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST
{
  uint32_t ulApi;
  uint16_t usSlot;
  uint16_t usSubslot;
  uint32_t ulModuleId;
  uint32_t ulSubmoduleId;
} PNS_IF_CONFIGURED_SUBM_STRUCT_T;

#define   MAX_SUBMODULE_CNT     95     /* max. count of submodules */

/** Data part of confirmation packet of Get Configured Submodules service */
typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST
{
  uint32_t                       ulSubmCnt;
  PNS_IF_CONFIGURED_SUBM_STRUCT_T  atSubm[MAX_SUBMODULE_CNT];
}  PNS_IF_GET_CONFIGURED_SUBM_CNF_DATA_T;

/** Confirmation packet of Get Configured Submodules service */
typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST
{
  HIL_PACKET_HEADER_T                   tHead;
  PNS_IF_GET_CONFIGURED_SUBM_CNF_DATA_T tData;
} PNS_IF_GET_CONFIGURED_SUBM_CNF_T;

/** Get Configured Submodules service packet union
 */
typedef union
{
  /** Request packet */
  PNS_IF_GET_CONFIGURED_SUBM_REQ_T        tReq;
  /** Confirmation packet */
  PNS_IF_GET_CONFIGURED_SUBM_CNF_T        tCnf;
} PNS_IF_GET_CONFIGURED_SUBM_PCK_T;

/** @} */

/**
 * @brief Apply Isochronous Mode Data Request
 */
typedef __HIL_PACKED_PRE struct
{
  /** the time to configure "T_IO_InputEvent" in ns */
  uint32_t  ulT_IO_InputEvent;
  /** the time to configure "T_IO_OutputEvent" in ns */
  uint32_t  ulT_IO_OutputEvent;
  /**  the time "T_ImpLen" of the activity on the Sync-pins in ns */
  uint32_t  ulT_ImpLen;
} __HIL_PACKED_POST PNS_IF_SET_ISOCHRONOUS_MODE_DATA_DATA_T;

typedef __HIL_PACKED_PRE struct
{
  /** packet header */
  HIL_PACKET_HEADER_T                       tHead;
  /** packet data */
  PNS_IF_SET_ISOCHRONOUS_MODE_DATA_DATA_T   tData;
} __HIL_PACKED_POST PNS_IF_SET_ISOCHRONOUS_MODE_DATA_REQ_T;

/**
 * @brief Apply Isochronous Mode Data Confirmation
 */
typedef HIL_EMPTY_PACKET_T PNS_IF_SET_ISOCHRONOUS_MODE_DATA_CNF_T;

/**
 * @brief Apply Isochronous Mode Data Packet Union
 */
typedef union
{
  /** indication */
  PNS_IF_SET_ISOCHRONOUS_MODE_DATA_REQ_T        tInd;
  /** response */
  PNS_IF_SET_ISOCHRONOUS_MODE_DATA_CNF_T        tRsp;
} PNS_IF_SET_ISOCHRONOUS_MODE_DATA_PCK_T;

/*
 * SET OEM PARAMETER SERVICE
 */

#define PNS_IF_SET_OEM_PARAMETERS_TYPE_1_UNSUPPORTED          0x01
#define PNS_IF_SET_OEM_PARAMETERS_TYPE_2_UNSUPPORTED          0x02
#define PNS_IF_SET_OEM_PARAMETERS_TYPE_3          0x03
#define PNS_IF_SET_OEM_PARAMETERS_TYPE_4_UNSUPPORTED          0x04
#define PNS_IF_SET_OEM_PARAMETERS_TYPE_5          0x05
#define PNS_IF_SET_OEM_PARAMETERS_TYPE_6          0x06
#define PNS_IF_SET_OEM_PARAMETERS_TYPE_7_UNSUPPORTED          0x07
#define PNS_IF_SET_OEM_PARAMETERS_TYPE_8          0x08
#define PNS_IF_SET_OEM_PARAMETERS_TYPE_9          0x09
#define PNS_IF_SET_OEM_PARAMETERS_TYPE_10         0x0A
#define PNS_IF_SET_OEM_PARAMETERS_TYPE_11         0x0B
#define PNS_IF_SET_OEM_PARAMETERS_TYPE_12         0x0C
#define PNS_IF_SET_OEM_PARAMETERS_TYPE_13         0x0D
#define PNS_IF_SET_OEM_PARAMETERS_TYPE_14_UNSUPPORTED         0x0E
#define PNS_IF_SET_OEM_PARAMETERS_TYPE_15_UNSUPPORTED         0x0F
#define PNS_IF_SET_OEM_PARAMETERS_TYPE_16         0x10
#define PNS_IF_SET_OEM_PARAMETERS_TYPE_17_UNSUPPORTED         0x11
#define PNS_IF_SET_OEM_PARAMETERS_TYPE_18         0x12
#define PNS_IF_SET_OEM_PARAMETERS_TYPE_19         0x13
#define PNS_IF_SET_OEM_PARAMETERS_TYPE_20_UNSUPPORTED         0x14

/* Request packet */

typedef __HIL_PACKED_PRE struct PNS_IF_SET_OEM_PARAMETERS_TYPE_3_Ttag
{
  uint32_t ulTimeout;
} __HIL_PACKED_POST PNS_IF_SET_OEM_PARAMETERS_TYPE_3_T;

typedef __HIL_PACKED_PRE struct PNS_IF_SET_OEM_PARAMETERS_TYPE_5_Ttag
{
  uint32_t ulIMFlag;
} __HIL_PACKED_POST PNS_IF_SET_OEM_PARAMETERS_TYPE_5_T;

typedef __HIL_PACKED_PRE struct PNS_IF_SET_OEM_PARAMETERS_TYPE_6_Ttag
{
  uint16_t  usIRTCycleCounterOffset;
} __HIL_PACKED_POST PNS_IF_SET_OEM_PARAMETERS_TYPE_6_T;


typedef __HIL_PACKED_PRE struct PNS_IF_SET_OEM_PARAMETERS_TYPE_8_Ttag
{
  uint32_t  fDisableFirmwareIMHandling; /* if set firmware does not handle I&M requests in case of database configuration */
} __HIL_PACKED_POST PNS_IF_SET_OEM_PARAMETERS_TYPE_8_T;

typedef PNS_IF_IM5_DATA_T PNS_IF_SET_OEM_PARAMETERS_TYPE_9_T;

typedef __HIL_PACKED_PRE struct PNS_IF_SET_OEM_PARAMETERS_TYPE_10_Ttag
{
  uint32_t  fSupportProfiEnergyFunction; /* if set firmware supports ProfiEnergy functionality */
} __HIL_PACKED_POST PNS_IF_SET_OEM_PARAMETERS_TYPE_10_T;

typedef __HIL_PACKED_PRE struct PNS_IF_SET_OEM_PARAMETERS_TYPE_11_Ttag
{
  /* if set firmware supports IRT communication
   * @Note IRT support is enabled by default */
  uint32_t  fSupportIRT;
} __HIL_PACKED_POST PNS_IF_SET_OEM_PARAMETERS_TYPE_11_T;

typedef __HIL_PACKED_PRE struct PNS_IF_SET_OEM_PARAMETERS_TYPE_12_Ttag
{
  /* if set firmware supports IO Supervisor communication
   * @Note IO Supervisor communication support is disabled by default.  */
  uint32_t  fSupportIOSupervisorAR;
} __HIL_PACKED_POST PNS_IF_SET_OEM_PARAMETERS_TYPE_12_T;

#define PNS_IF_SET_OEM_MINDEVICE_INTERVAL_MIN      8
#define PNS_IF_SET_OEM_MINDEVICE_INTERVAL_MAX      4096
#define PNS_IF_SET_OEM_MINDEVICE_INTERVAL_IRT_MAX  32

typedef __HIL_PACKED_PRE struct PNS_IF_SET_OEM_PARAMETERS_TYPE_13_Ttag
{
  /* The minimum supported cycle time in base of 31,25us (allowed values: Power of two in range [8 - 4096]).
   * It must correspond with MinDeviceInterval from GSDML file
   * @Note By default the firmware supports a usMinDeviceInterval of 8 is supported
   * this means that a cycle time of 250us will be supported (8 * 31,25 = 250us)
   * If IRT supported MinDeviceInterval must not be greater than 32 */
  uint16_t  usMinDeviceInterval;
} __HIL_PACKED_POST PNS_IF_SET_OEM_PARAMETERS_TYPE_13_T;

typedef __HIL_PACKED_PRE struct PNS_IF_SET_OEM_PARAMETERS_TYPE_14_Ttag
{
  /* HardwareRevision for usage in I&M0, RPC EndPointMapper and SNMP sysDescr */
  uint16_t              usHardwareRevision;
  /* SoftwareVersion for usage in I&M0, RPC EndPointMapper and SNMP sysDescr */
  PNS_IF_IM_SWVERSION_T tSoftwareRevision;
} __HIL_PACKED_POST PNS_IF_SET_OEM_PARAMETERS_TYPE_14_T;

typedef __HIL_PACKED_PRE struct PNS_IF_SET_OEM_PARAMETERS_TYPE_15_Ttag
{
  /* VendorID to be used (instead of the one contained in configuration database file) */
  uint16_t usVendorId;
  /* DeviceID to be used (instead of the one contained in configuration database file) */
  uint16_t usDeviceId;
  /* OrderID to be used (instead of the one contained in configuration database file) - (pad with spaces (0x20) */
  uint8_t  abOrderId[PNS_IF_MAX_ORDER_ID];
  /* DeviceType to be used (instead of the one contained in configuration database file) - (ignore the last 3 padding bytes */
  uint8_t  abDeviceType[PNS_IF_MAX_DEVICE_TYPE_LEN +3];
} __HIL_PACKED_POST PNS_IF_SET_OEM_PARAMETERS_TYPE_15_T;

typedef __HIL_PACKED_PRE struct PNS_IF_SET_OEM_PARAMETERS_TYPE_16_Ttag
{
  /* if set firmware supports dynamic reconfiguration feature
   * @Note dynamic reconfiguration support is disabled by default.  */
  uint32_t  fSupportDynamicReconfiguration;
} __HIL_PACKED_POST PNS_IF_SET_OEM_PARAMETERS_TYPE_16_T;

typedef __HIL_PACKED_PRE struct PNS_IF_SET_OEM_PARAMETERS_TYPE_18_Ttag
{

  /* VendorID to be used (instead of the one contained in configuration database file) */
  uint16_t usVendorId;
  /* DeviceID to be used (instead of the one contained in configuration database file) */
  uint16_t usDeviceId;
  /* SoftwareVersion for usage in I&M0, RPC EndPointMapper and SNMP sysDescr */
  PNS_IF_IM_SWVERSION_T tSoftwareRevision;
  /* DeviceType to be used (instead of the one contained in configuration database file) - (ignore the last 3 padding bytes */
  uint8_t  abDeviceType[PNS_IF_MAX_DEVICE_TYPE_LEN +3];
} __HIL_PACKED_POST PNS_IF_SET_OEM_PARAMETERS_TYPE_18_T;


typedef __HIL_PACKED_PRE struct PNS_IF_SET_OEM_PARAMETERS_TYPE_19_Ttag
{
  /* Default(0): security class is disabled, 1 Stack supports SecurityClassOne functionality */
  uint32_t  ulEnableSecurityClass; /* set security class, 0 (disable, def. ) or 1  */
} __HIL_PACKED_POST PNS_IF_SET_OEM_PARAMETERS_TYPE_19_T;

typedef __HIL_PACKED_PRE struct PNS_IF_SET_OEM_PARAMETERS_DATA_Ttag
{
    uint32_t ulParameterType;
} __HIL_PACKED_POST PNS_IF_SET_OEM_PARAMETERS_DATA_T;

typedef union PNS_IF_SET_OEM_PARAMETERS_UNION_Ttag
{
  PNS_IF_SET_OEM_PARAMETERS_TYPE_3_T      tType3Param;
  PNS_IF_SET_OEM_PARAMETERS_TYPE_5_T      tType5Param;
  PNS_IF_SET_OEM_PARAMETERS_TYPE_6_T      tType6Param;
  PNS_IF_SET_OEM_PARAMETERS_TYPE_8_T      tType8Param;
  PNS_IF_SET_OEM_PARAMETERS_TYPE_9_T      tType9Param;
  PNS_IF_SET_OEM_PARAMETERS_TYPE_10_T     tType10Param;
  PNS_IF_SET_OEM_PARAMETERS_TYPE_11_T     tType11Param;
  PNS_IF_SET_OEM_PARAMETERS_TYPE_12_T     tType12Param;
  PNS_IF_SET_OEM_PARAMETERS_TYPE_13_T     tType13Param;
  PNS_IF_SET_OEM_PARAMETERS_TYPE_14_T     tType14Param;
  PNS_IF_SET_OEM_PARAMETERS_TYPE_15_T     tType15Param;
  PNS_IF_SET_OEM_PARAMETERS_TYPE_16_T     tType16Param;
  PNS_IF_SET_OEM_PARAMETERS_TYPE_18_T     tType18Param;
  PNS_IF_SET_OEM_PARAMETERS_TYPE_19_T     tType19Param;
} PNS_IF_SET_OEM_PARAMETERS_UNION_T;

typedef __HIL_PACKED_PRE struct PNS_IF_SET_OEM_PARAMETERS_REQ_Ttag
{
  HIL_PACKET_HEADER_T                       tHead;
  PNS_IF_SET_OEM_PARAMETERS_DATA_T          tData;
  PNS_IF_SET_OEM_PARAMETERS_UNION_T         tParam;
} __HIL_PACKED_POST PNS_IF_SET_OEM_PARAMETERS_REQ_T;

/* Confirmation packet */
typedef __HIL_PACKED_PRE struct PNS_IF_SET_OEM_PARAMETERS_CNF_Ttag{
  HIL_PACKET_HEADER_T                     tHead;
}__HIL_PACKED_POST PNS_IF_SET_OEM_PARAMETERS_CNF_T;

/* packet union */
typedef union PNS_IF_SET_OEM_PARAMETERS_PCK_Ttag
{
  PNS_IF_SET_OEM_PARAMETERS_REQ_T           tReq;
  PNS_IF_SET_OEM_PARAMETERS_CNF_T           tCnf;
} PNS_IF_SET_OEM_PARAMETERS_PCK_T;

/**
 * @defgroup pns_getparam_service Get Parameter Service
 * @{
 */

enum PNS_IF_PARAM_Etag
{
  PNS_IF_PARAM_MRP              = 1,
  PNS_IF_PARAM_SUBMODULE_CYCLE  = 2,
  PNS_IF_PARAM_ETHERNET         = 3,
  PNS_IF_PARAM_DIAGNOSIS        = 4, /* only useable for port- and interface submodules */
  PNS_IF_PARAM_IM0_DATA         = 5,
  PNS_IF_PARAM_IM5_DATA         = 6,
  PNS_IF_PARAM_PORT_STATISTIC   = 7, /* only useable for port-submodules */
};

typedef enum PNS_IF_PARAM_Etag PNS_IF_PARAM_E;

typedef __HIL_PACKED_PRE struct PNS_IF_PARAM_COMMON_Ttag
{
  /** mandatory */
  uint16_t  usPrmType;
  /** following parameters are optional */
  uint16_t  usPadding;
  /** api of the associated submodule */
  uint32_t  ulApi;
  /** slot of the associated submodule */
  uint16_t  usSlot;
  /** subslot of the associated submodule */
  uint16_t  usSubslot;
} __HIL_PACKED_POST PNS_IF_PARAM_COMMON_T;


/** MRP was disabled by protocol stack itself */
#define PNS_IF_MRP_STATE_DISABLED        (0)
/** MRP was enabled by protocol with default values.
 * That happens if no explicit MRP parameterization took place */
#define PNS_IF_MRP_STATE_ENABLED_DEFAULT  (1)
/** MRP was enabled explicitly by parameterization
 * The controller enabled MRP explicitly by means of MRP parameter record */
#define PNS_IF_MRP_STATE_ENABLED_PRM      (2)
/** MRP was disabled explicitly by parameterization
 * The controller disabled MRP explicitly by means of MRP parameter record */
#define PNS_IF_MRP_STATE_DISABLED_PRM     (3)

#define PNS_IF_MRP_ROLE_NONE         (0)
#define PNS_IF_MRP_ROLE_MRP_CLIENT   (1)
#define PNS_IF_MRP_ROLE_MRP_MANAGER  (2)

typedef __HIL_PACKED_PRE struct PNS_IF_PARAM_MRP_Ttag
{
  uint16_t      usPrmType;

  uint8_t       bState;

  uint8_t       bRole;

  HIL_UUID_T    tUUID;

  uint8_t       szDomainName[PNS_IF_MAX_NAME_OF_STATION];
} __HIL_PACKED_POST PNS_IF_PARAM_MRP_T;


typedef __HIL_PACKED_PRE struct PNS_IF_PARAM_SUBMODULE_CYCLE_Ttag
{
  uint16_t  usPrmType;

  uint16_t  usPadding;
  /** api of the associated submodule */
  uint32_t  ulApi;
  /** slot of the associated submodule */
  uint16_t  usSlot;
  /** subslot of the associated submodule */
  uint16_t  usSubslot;
  /** update interval of this submodules process data in nanoseconds */
  uint32_t  ulUpdateInterval;
  /** send base clock for the associated iocr */
  uint16_t  usSendClock;
  /** send clock reduction for the associated iocr */
  uint16_t  usReductionRatio;
  /** missing frames until timeout of the associated iocr */
  uint16_t  usDataHoldFactor;
} __HIL_PACKED_POST PNS_IF_PARAM_SUBMODULE_CYCLE_T;

typedef __HIL_PACKED_PRE struct PNS_IF_PARAM_ETHERNET_Ttag
{
  uint16_t       usPrmType;
  /* real Mutype */
  uint16_t       usMauType;
  /* measured power budget in dB */
  uint32_t       ulPowerBudget;
  /* measured cable delay in ns */
  uint32_t       ulCableDelay;
}  __HIL_PACKED_POST PNS_IF_PARAM_ETHERNET_T;

typedef __HIL_PACKED_PRE struct PNS_IF_DIAGNOSIS_ENTRY_Ttag
{
  /* subslot the diagnosis alarm belongs to */
  uint16_t       usSubslot;
  /* Channel properties  */
  uint16_t       usChannelProp;
  /* Channel error type */
  uint16_t       usChannelErrType;
  /* Channel extended error type */
  uint16_t       usExtChannelErrType;
} __HIL_PACKED_POST PNS_IF_DIAGNOSIS_ENTRY_T;

#define MAX_DIAGNOSIS_ENTRIES_CNT   32

typedef __HIL_PACKED_PRE struct PNS_IF_DIAGNOSIS_Ttag
{
  uint16_t       usPrmType;
  /* The amount of diagnosis entries contained in the confirmation */
  uint16_t       usDiagnosisCnt;
  /** Diagnosis data, shall contain usDiagnosisCnt entries */
  PNS_IF_DIAGNOSIS_ENTRY_T atDiagnosis[MAX_DIAGNOSIS_ENTRIES_CNT];
} __HIL_PACKED_POST PNS_IF_DIAGNOSIS_T;

typedef __HIL_PACKED_PRE struct PNS_IF_PARAM_IM0_DATA_Ttag
{
  uint16_t  usPrmType;

  uint16_t  usPadding;

  PNS_IF_IM0_DATA_T              tIM0Data;
} __HIL_PACKED_POST PNS_IF_PARAM_IM0_DATA_T;

typedef __HIL_PACKED_PRE struct PNS_IF_PARAM_IM5_DATA_Ttag
{
  uint16_t  usPrmType;

  uint16_t  usPadding;

  PNS_IF_IM5_DATA_T              tIM5Data;
} __HIL_PACKED_POST PNS_IF_PARAM_IM5_DATA_T;


#define PNS_IF_VALID_MASK_PORT_STATISTIC_INOCTETS      (0x0001)
#define PNS_IF_VALID_MASK_PORT_STATISTIC_OUTOCTETS     (0x0002)
#define PNS_IF_VALID_MASK_PORT_STATISTIC_INDISCARDS    (0x0004)
#define PNS_IF_VALID_MASK_PORT_STATISTIC_OUTDISCARDS   (0x0008)
#define PNS_IF_VALID_MASK_PORT_STATISTIC_INERRORS      (0x0010)
#define PNS_IF_VALID_MASK_PORT_STATISTIC_OUTERRORS     (0x0020)

typedef __HIL_PACKED_PRE struct PNS_IF_PARAM_PORT_STATISTIC_DATA_Ttag
{
  uint16_t   usPrmType;
  uint16_t   usPadding;
  uint32_t   ulValidMask;   /* indicator which of the fields in this structure contain valid data */
  uint32_t   ulInOctets;    /* number of octets received by the port */
  uint32_t   ulOutOctets;   /* number of octets sent by the port */
  uint32_t   ulInDiscards;  /* number of frames discarded or dropped due to low resources on reception */
  uint32_t   ulOutDiscards; /* number of frames discarded before sending */
  uint32_t   ulInErrors;    /* number of frames received with detected error */
  uint32_t   ulOutErrors;   /* number of frames sent with detected error */
} __HIL_PACKED_POST PNS_IF_PARAM_PORT_STATISTIC_DATA_T;

typedef union PNS_IF_PARAM_Ttag PNS_IF_PARAM_T;

union PNS_IF_PARAM_Ttag
{
  PNS_IF_PARAM_COMMON_T          tCommon;

  PNS_IF_PARAM_MRP_T             tMrp;

  PNS_IF_PARAM_SUBMODULE_CYCLE_T tSubmoduleCycle;

  PNS_IF_PARAM_ETHERNET_T        tEthernetPrm;

  PNS_IF_DIAGNOSIS_T             tDiagnosisData;

  PNS_IF_PARAM_IM0_DATA_T        tIM0Prm;

  PNS_IF_PARAM_IM5_DATA_T        tIM5Prm;

  PNS_IF_PARAM_PORT_STATISTIC_DATA_T tPortStatistic;
};

typedef __HIL_PACKED_PRE struct PNS_IF_GET_PARAM_REQ_Ttag
{
  HIL_PACKET_HEADER_T   tHead;

  PNS_IF_PARAM_COMMON_T tData;
} __HIL_PACKED_POST PNS_IF_GET_PARAM_REQ_T;

typedef __HIL_PACKED_PRE struct PNS_IF_GET_PARAM_CNF_Ttag
{
  HIL_PACKET_HEADER_T   tHead;
  PNS_IF_PARAM_T        tData;
} __HIL_PACKED_POST PNS_IF_GET_PARAM_CNF_T;


/* packet union */
typedef union PNS_IF_GET_PARAM_PCK_Ttag PNS_IF_GET_PARAM_PCK_T;

union PNS_IF_GET_PARAM_PCK_Ttag
{
  PNS_IF_GET_PARAM_REQ_T tReq;
  PNS_IF_GET_PARAM_CNF_T tCnf;
};


typedef __HIL_PACKED_PRE struct ISOCHRONOUS_DATA_Ttag
{
  /** 00:02, BlockType */
  uint16_t        usType;
  /** 02:02, BlockLength */
  uint16_t        usLen;
  /** 04:01, BlockVersionHigh */
  uint8_t         bMajor;
  /** 05:01, BlockVersionLow */
  uint8_t         bMinor;
  /** 06:02, Padding */
  uint8_t         abPadding1[2];
  /** 08:2, SlotNumber */
  uint16_t     usSlot;
  /** 10:2, SubslotNumber */
  uint16_t     usSubSlot;
  /** 12:02, ControllerApplicationCycleFactor */
  uint16_t     usCACF;
  /** 14:02, TimeDataCycle */
  uint16_t     usDataCycle;
  /** 16:04, TimeIOInput */
  uint32_t     ulIO_Input;
  /** 20:04, TimeIOOutput */
  uint32_t     ulIO_Output;
  /** 24:04, TimeIOInputValid */
  uint32_t     ulIO_InputValid;
  /** 28:04, TimeIOOutputValid */
  uint32_t     ulIO_OutputValid;
}__HIL_PACKED_POST PNS_IF_PDU_ISOCHRONOUS_DATA_T;

/*
 * Get Asset Entry Service
 */
typedef __HIL_PACKED_PRE struct PNS_IF_GET_ASSET_IND_DATA_Ttag
{
  /** Number of first requested asset management entry */
  uint16_t    usEntryNumber;
} __HIL_PACKED_POST PNS_IF_GET_ASSET_IND_DATA_T;

typedef __HIL_PACKED_PRE struct PNS_IF_GET_ASSET_IND_Ttag
{
  /** packet header */
  HIL_PACKET_HEADER_T                tHead;
  /** Indication data */
  PNS_IF_GET_ASSET_IND_DATA_T        tData;
} __HIL_PACKED_POST PNS_IF_GET_ASSET_IND_T;

enum
{
  PNS_IF_ASSET_TYPE_FULLINFORMATION         = (0x0001),
  PNS_IF_ASSET_TYPE_FIRMWAREONLYINFORMATION = (0x0002),
  PNS_IF_ASSET_TYPE_HARDWAREONLYINFORMATION = (0x0003),
};

enum
{
  PNS_IF_ASSET_LOCATION_STRUCTURE_LEVEL       = 0x01,
  PNS_IF_ASSET_LOCATION_STRUCTURE_SLOTSUBSLOT = 0x02,
};

typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST PNS_IF_ASSET_LOCATION_LEVEL_Ttag
{
  uint8_t       bStructureId;

  uint8_t       abPadding[3];
  /** Set to 0x3FF for level and subsequent levels if not used*/
  uint16_t      ausLevel[12];
} __HIL_PACKED_POST PNS_IF_ASSET_LOCATION_LEVEL_T;

typedef __HIL_PACKED_PRE struct PNS_IF_ASSET_LOCATION_SLOTSUBSLOT_Ttag
{
  uint8_t       bStructureId;

  uint8_t      abPadding[7];

  uint16_t      usSlotBegin;

  uint16_t      usSubslotBegin;

  uint16_t      usSlotEnd;

  uint16_t      usSubslotEnd;
} __HIL_PACKED_POST PNS_IF_ASSET_LOCATION_SLOTSUBSLOT_T;

typedef union PNS_IF_ASSET_LOCATION_Ttag PNS_IF_ASSET_LOCATION_T;

union PNS_IF_ASSET_LOCATION_Ttag
{
  PNS_IF_ASSET_LOCATION_LEVEL_T       tLevel;
  PNS_IF_ASSET_LOCATION_SLOTSUBSLOT_T tSlotSubslot;
};

typedef __HIL_PACKED_PRE struct PNS_IF_ASSET_DEVICEIDENTIFICATION_Ttag
{
  /** Reserved for future usage. Set to 0x0000 */
  uint16_t usDeviceSubID;
  /** Device ID associated with asset */
  uint16_t usDeviceID;
  /** VendorID associated with asset */
  uint16_t usVendorID;
  /** Organization associated with asset */
  uint16_t usOrganization;
} __HIL_PACKED_POST PNS_IF_ASSET_DEVICEIDENTIFICATION_T;

typedef __HIL_PACKED_PRE struct PNS_IF_ASSET_ENTRY_Ttag
{
  uint16_t                usEntryNumber;

  uint8_t                 bEntryType;

  uint8_t                 bPadding;

  HIL_UUID_T              tIM_UniqueIdentifier;

  PNS_IF_ASSET_LOCATION_T tAM_Location;

  uint8_t                 abIM_Annotation[64];

  uint8_t                 abIM_OrderID[64];

  uint8_t                 abAM_SoftwareRevision[64];

  uint8_t                 abAM_HardwareRevision[64];

  uint8_t                 abIM_Serial_Number[16];

  uint8_t                 abIM_Software_Revision[4];

  PNS_IF_ASSET_DEVICEIDENTIFICATION_T tAM_DeviceIdentification;

  uint16_t                usAM_TypeIdentification;

  uint16_t                usIM_Hardware_Revision;

} __HIL_PACKED_POST PNS_IF_ASSET_ENTRY_T;

typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST PNS_IF_GET_ASSET_RSP_DATA_Ttag
{
  PNS_IF_ASSET_ENTRY_T atEntries[4];
} __HIL_PACKED_POST PNS_IF_GET_ASSET_RSP_DATA_T;

typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST PNS_IF_GET_ASSET_RSP_Ttag
{
  /** packet header */
  HIL_PACKET_HEADER_T                tHead;
  /** Indication data */
  PNS_IF_GET_ASSET_RSP_DATA_T        tData;
} __HIL_PACKED_POST PNS_IF_GET_ASSET_RSP_T;

typedef union PNS_IF_GET_ASSET_PCK_Ttag PNS_IF_GET_ASSET_PCK_T;

union PNS_IF_GET_ASSET_PCK_Ttag
{
  /** indication */
  PNS_IF_GET_ASSET_IND_T       tInd;
  /** response */
  PNS_IF_GET_ASSET_RSP_T       tRsp;
};


/** Add PROFI energy entity service */
typedef __HIL_PACKED_PRE struct PNS_IF_ADD_PE_ENTITY_REQ_DATA_Ttag
{
  uint32_t ulAPI;
  uint16_t usSlot;
  uint16_t usSubslot;
} __HIL_PACKED_POST PNS_IF_ADD_PE_ENTITY_REQ_DATA_T;

typedef __HIL_PACKED_PRE struct PNS_IF_ADD_PE_ENTITY_REQ_Ttag
{
  HIL_PACKET_HEADER_T             tHead;
  PNS_IF_ADD_PE_ENTITY_REQ_DATA_T tData;
} __HIL_PACKED_POST PNS_IF_ADD_PE_ENTITY_REQ_T;

typedef HIL_EMPTY_PACKET_T PNS_IF_ADD_PE_ENTITY_CNF_T;

union PNS_IF_ADD_PE_ENTITY_PCK_Ttag
{
  PNS_IF_ADD_PE_ENTITY_REQ_T tReq;
  PNS_IF_ADD_PE_ENTITY_CNF_T tCnf;
};

typedef union PNS_IF_ADD_PE_ENTITY_PCK_Ttag PNS_IF_ADD_PE_ENTITY_PCK_T;

/** Remove PROFIenergy entity service */
typedef PNS_IF_ADD_PE_ENTITY_REQ_DATA_T PNS_IF_REMOVE_PE_ENTITY_REQ_DATA_T;

typedef __HIL_PACKED_PRE struct PNS_IF_REMOVE_PE_ENTITY_REQ_Ttag
{
  HIL_PACKET_HEADER_T                 tHead;
  PNS_IF_REMOVE_PE_ENTITY_REQ_DATA_T  tData;
} __HIL_PACKED_POST PNS_IF_REMOVE_PE_ENTITY_REQ_T;

typedef HIL_EMPTY_PACKET_T PNS_IF_REMOVE_PE_ENTITY_CNF_T;

union PNS_IF_REMOVE_PE_ENTITY_PCK_Ttag
{
  PNS_IF_REMOVE_PE_ENTITY_REQ_T tReq;
  PNS_IF_REMOVE_PE_ENTITY_CNF_T tCnf;
};

typedef union PNS_IF_REMOVE_PE_ENTITY_PCK_Ttag PNS_IF_REMOVE_PE_ENTITY_PCK_T;

/** Add PROFI energy entity service */
typedef __HIL_PACKED_PRE struct PNS_IF_UPDATE_PE_ENTITY_REQ_DATA_Ttag
{
  uint32_t ulAPI;
  uint16_t usSlot;
  uint16_t usSubslot;
  uint8_t  bOperationalMode;
} __HIL_PACKED_POST PNS_IF_UPDATE_PE_ENTITY_REQ_DATA_T;

typedef __HIL_PACKED_PRE struct PNS_IF_UPDATE_PE_ENTITY_REQ_Ttag
{
  HIL_PACKET_HEADER_T                tHead;
  PNS_IF_UPDATE_PE_ENTITY_REQ_DATA_T tData;
} __HIL_PACKED_POST PNS_IF_UPDATE_PE_ENTITY_REQ_T;

typedef HIL_EMPTY_PACKET_T PNS_IF_UPDATE_PE_ENTITY_CNF_T;

union PNS_IF_UPDATE_PE_ENTITY_PCK_Ttag
{
  PNS_IF_UPDATE_PE_ENTITY_REQ_T tReq;
  PNS_IF_UPDATE_PE_ENTITY_CNF_T tCnf;
};

typedef union PNS_IF_UPDATE_PE_ENTITY_PCK_Ttag PNS_IF_UPDATE_PE_ENTITY_PCK_T;


/**
 * SystemRedundancy Info Indication
 *
 * @note This indication is not supported by PNS v3 and is only
 *       supported by PNSv4
 */
/** SR AR Status used in bStatusFlags */
enum PNS_IF_AR_SET_STATUS_Etag
{
  PNS_IF_AR_SET_STATUS_DISCONNECTED        = 0, /* no active ar's*/
  PNS_IF_AR_SET_STATUS_CONNECTED_BACKUP    = 1, /* no primary AR exists */
  PNS_IF_AR_SET_STATUS_CONNECTED_PRIMARY   = 2, /* primary AR exists */
};

typedef __HIL_PACKED_PRE struct PNS_IF_AR_SET_STATUS_IND_DATA_Ttag
{
  /** the handle associated with the SR-AR-set */
  uint32_t            hARsetHandle;
  /* Indicate the SR-AR-set status (see above PNS_IF_AR_SET_STATUS_Etag) */
  uint8_t             bARsetStatus;
  /** amount of active ARs in the AR-set */
  uint8_t             bARCount;
} __HIL_PACKED_POST PNS_IF_AR_SET_STATUS_IND_DATA_T;


typedef __HIL_PACKED_PRE struct PNS_IF_AR_SET_STATUS_IND_Ttag
{
  /** packet header */
  HIL_PACKET_HEADER_T               tHead;
  /** packet data */
  PNS_IF_AR_SET_STATUS_IND_DATA_T   tData;
} __HIL_PACKED_POST PNS_IF_AR_SET_STATUS_IND_T;

typedef HIL_EMPTY_PACKET_T    PNS_IF_AR_SET_STATUS_RSP_T;

typedef union
{
  /** indication */
  PNS_IF_AR_SET_STATUS_IND_T  tInd;
  /** response */
  PNS_IF_AR_SET_STATUS_RSP_T  tRsp;
} PNS_IF_AR_SET_STATUS_PCK_T;


/*
 * Dynamic Reconfiguration Service
 */

/* Indication packet */
typedef PNS_IF_AR_CHECK_IND_T               PNS_IF_RECONFIG_IND_T;
/* Confirmation packet */
typedef PNS_IF_HANDLE_PACKET_T              PNS_IF_RECONFIG_RSP_T;

/* Response union */
typedef union PNS_IF_RECONFIG_PCK_Ttag
{
  /** indication packet */
  PNS_IF_RECONFIG_IND_T                     tInd;
  /** response packet */
  PNS_IF_RECONFIG_RSP_T                     tRsp;
} PNS_IF_RECONFIG_PCK_T;
/**@}*/



/*! Set data exchange trigger data. */
typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST PNS_IF_CONFIG_TRIGGER_EVENT_Ttag
{
  /*! The period in which the event should be reported in factor of  31,25 us.
   *  e.g a value of 32 --> min data cycle is 32 * 31,25 =  1ms */
  uint32_t ulCycleTime;
  /*! The time related to the beginning of the cycle at which the event is to be reported
   *  Time base:  1 ns */
  uint32_t ulOffset;
} PNS_IF_CONFIG_TRIGGER_EVENT_T;

/*! Set data exchange trigger data. */
typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST PNS_IF_CONFIG_TRIGGER_EVENT_REQ_DATA_Ttag
{
  PNS_IF_CONFIG_TRIGGER_EVENT_T  atTriggerEvent[2];
} PNS_IF_CONFIG_TRIGGER_EVENT_REQ_DATA_T;

/*! Set data exchange trigger request. */
typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST PNS_IF_CONFIG_TRIGGER_EVENT_REQ_Ttag
{
  HIL_PACKET_HEADER_T                      tHead;  /*!< Packet header. */
  PNS_IF_CONFIG_TRIGGER_EVENT_REQ_DATA_T   tData;  /*!< Packet data. */
} PNS_IF_CONFIG_TRIGGER_EVENT_REQ_T;

#define PNS_IF_CONFIG_TRIGGER_TYPE_REQ_SIZE (sizeof(PNS_IF_CONFIG_TRIGGER_EVENT_REQ_DATA_T))

/*! Set data exchange trigger confirmation structure. */
typedef HIL_EMPTY_PACKET_T      PNS_IF_CONFIG_TRIGGER_EVENT_CNF_T;

/* Response union */
typedef union PNS_IF_CONFIG_TRIGGER_EVENT_PCK_Ttag
{
  /** request packet */
  PNS_IF_CONFIG_TRIGGER_EVENT_REQ_T         tReq;
  /** confirmation packet */
  PNS_IF_CONFIG_TRIGGER_EVENT_CNF_T         tCnf;
} PNS_IF_CONFIG_TRIGGER_EVENT_PCK_T;


/*
 * UNION  OF  ALL  PNS_IF  PACKETS
 */
typedef union PROFINET_PNS_IF_PACKET_Ttag
{
   PNS_IF_SET_CONFIGURATION_REQUEST_PCK_T  tSetConfigPck;          // stack configuration packet
   PNS_IF_PLUG_MODULE_PCK_T                tPlugModulePck;         // Module Plug package
   PNS_IF_PLUG_SUBMODULE_PCK_T             tPlugSubmodulePck;      // Submodule Plug package
   PNS_IF_PULL_MODULE_PCK_T                tPullModulePck;         // Module Pull package
   PNS_IF_PULL_SUBMODULE_PCK_T             tPullSubmodulePck;      // Submodule Pull package
   PNS_IF_AR_CHECK_PCK_T                   tARCheckPck;            // AR check package
   PNS_IF_CHECK_IND_PCK_T                  tCheckIndPck;           // Check indication package
   PNS_IF_CONNECTREQ_DONE_PCK_T            tConDonePck;            // Connection done package
   PNS_IF_PARAM_END_PCK_T                  tParamEndPck;           // Parameter end indication package
   PNS_IF_APPL_READY_PCK_T                 tApplReadyPck;          // Application ready indication package
   PNS_IF_AR_IN_DATA_PCK_T                 tARInDataPck;           // AR 1st InData indication package
   PNS_IF_READ_RECORD_PCK_T                tReadRecordPck;         // Read record package
   PNS_IF_WRITE_RECORD_PCK_T               tWriteRecordPck;        // Write record package
   PNS_IF_AR_ABORT_IND_PCK_T               tARAbortIndPck;         // AR abort indication package
   PNS_IF_SAVE_STATION_NAME_PCK_T          tSaveStationNamePck;    // Save station name package
   PNS_IF_SAVE_IP_ADDR_PCK_T               tSaveIPAddrPck;         // Save IP address package
   PNS_IF_START_LED_BLINKING_PCK_T         tStartBlinkPck;         // Start LED blinking package
   PNS_IF_STOP_LED_BLINKING_PCK_T          tStopBlinkPck;          // Start LED blinking package
   PNS_IF_RESET_FACTORY_SETTINGS_PCK_T     tResetFactPck;          // reset to factory settings package
   PNS_IF_APDU_STATUS_CHANGED_PCK_T        tAPDUStatusPck;         // APDU staus changed indication package
   PNS_IF_ALARM_PCK_T                      tAlarmPck;              // Alarm package
   PNS_IF_LINK_STATUS_CHANGED_PCK_T        tLinkStatPck;           // Link status changed package
   PNS_IF_SEND_PROCESS_ALARM_PCK_T         tSendProcAlarmPck;      // Send process alarm package
   PNS_IF_SEND_DIAG_ALARM_PCK_T            tSendDiagAlarmPck;      // Send diagnostic alarm package
   PNS_IF_RETURN_OF_SUB_ALARM_PCK_T        tRetOfSubAlarmPck;      // Return of submodule alarm package
   PNS_IF_ABORT_CONNECTION_PCK_T           tAbortConPck;           // Abort connection package
   PNS_IF_GET_STATION_NAME_PCK_T           tGetStationNamePck;     // Get station name package
   PNS_IF_GET_IP_ADDR_PCK_T                tGetIPAdrPck;           // Get IP address package;
   PNS_IF_USER_ERROR_PCK_T                 tUserErrorPck;          // User error package
   PNS_IF_SET_OEM_PARAMETERS_PCK_T         tSetOemParamPck;        // set OEM parameter request packet
   PNS_IF_STORE_REMANENT_DATA_PCK_T        tStoreRemaDataPck;      // store remanent data packet
   PNS_IF_LOAD_REMANENT_DATA_PCK_T         tLoadRemaDataPck;       // load remanent data packet
   PNS_IF_SET_IOXS_CONFIG_PCK_T            tSetIOxSConfigPck;      // set IOxS config
   PNS_IF_PLUG_SUBMODULE_EXTENDED_PCK_T    tPlugSubmodExtPck;      // extended plug submodule
   PNS_IF_SET_PORT_MAC_PCK_T               tSetPortMacPck;         // set port MAC addresses
   PNS_IF_GET_XMAC_DIAGNOSIS_PCK_T         tGetXMACDiagPck;        // get XMAC diagnosis
   PNS_IF_GET_DIAGNOSIS_PCK_T              tGetStackDiagPck;       // get diagnosis
   PNS_IF_ADD_CHANNEL_DIAG_PCK_T           tAddChannelDiagPck;     // add Channel Diagnosis
   PNS_IF_ADD_EXTENDED_DIAG_PCK_T          tAddExtChannelDiagPck;  // add Extended Channel Diagnosis
   PNS_IF_ADD_GENERIC_DIAG_PCK_T           tAddGenericDiagPck;     // add Generic Diagnosis
   PNS_IF_REMOVE_DIAG_PCK_T                tRemoveDiagPck;         // remove diagnosis
   PNS_IF_HANDLE_PACKET_T                  tHandlePck;             // handle packet
   PNS_IF_READ_IM_PCK_T                    tReadIMPck;             // read i&m packet
   PNS_IF_WRITE_IM_PCK_T                   tWriteIMPck;            // write i&m packet
   PNS_IF_PARAMET_SPEEDUP_SUPPORTED_PCK_T  tParSpeedSuppPck;       // parameter speedup supported indication
   PNS_IF_EVENT_PCK_T                      tEvent;
   PNS_IF_PARAM_BEGIN_PCK_T                tPrmBegin;              // Parameter begin
   PNS_IF_SET_ISOCHRONOUS_MODE_DATA_PCK_T  tSetIschrModeData;      // Apply Isochronous Mode Data
   PNS_IF_GET_PARAM_PCK_T                  tGetParam;
   PNS_IF_GET_ASSET_PCK_T                  tGetAsset;
   PNS_IF_ADD_PE_ENTITY_PCK_T              tAddPEEntity;
   PNS_IF_REMOVE_PE_ENTITY_PCK_T           tRemovePEEntity;
   PNS_IF_UPDATE_PE_ENTITY_PCK_T           tUpdatePEEntity;
   PNS_IF_AR_SET_STATUS_PCK_T              tArSetPck;
   PNS_IF_GET_CONFIGURED_SUBM_PCK_T        tGetConfiguredSubmPck;
   PNS_IF_CONFIG_TRIGGER_EVENT_PCK_T       tConfigTrigerEventPck;
} PROFINET_PNS_IF_PACKET_T;


/* pragma unpack */
#ifdef __HIL_PRAGMA_PACK_ENABLE
  #pragma __HIL_PRAGMA_UNPACK_1(PNSIF_API_PUBLIC)
#endif


/*************************************************************************************/
#endif /* #ifndef PNSIF_API_H_ */
