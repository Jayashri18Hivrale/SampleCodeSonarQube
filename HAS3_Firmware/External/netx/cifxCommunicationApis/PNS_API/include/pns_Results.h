/**************************************************************************************
Copyright (c) Hilscher Gesellschaft fuer Systemautomation mbH. All Rights Reserved.
***************************************************************************************
$Id: pns_Results.h 98001 2021-02-03 09:39:37Z AMesser $:

Description:
  Profinet Stack status codes
**************************************************************************************/

#ifndef PNS_RESULTS_H_
#define PNS_RESULTS_H_

#include<stdint.h>

/////////////////////////////////////////////////////////////////////////////////////
// PROFINET IO-Device Interface Task
/////////////////////////////////////////////////////////////////////////////////////
//
// MessageId: ERR_PNS_IF_COMMAND_INVALID
//
// MessageText:
//
// Invalid command.
//
#define ERR_PNS_IF_COMMAND_INVALID       ((uint32_t)0xC0300001L)

//
// MessageId: ERR_PNS_IF_OS_INIT_FAILED
//
// MessageText:
//
// Initialization of PNS Operating system adaption failed.
//
#define ERR_PNS_IF_OS_INIT_FAILED        ((uint32_t)0xC0300002L)

//
// MessageId: ERR_PNS_IF_SET_INIT_IP_FAILED
//
// MessageText:
//
// Initialization of PNS IP address failed.
//
#define ERR_PNS_IF_SET_INIT_IP_FAILED    ((uint32_t)0xC0300003L)

//
// MessageId: ERR_PNS_IF_PNIO_SETUP_FAILED
//
// MessageText:
//
// PROFINET IO-Device Setup failed.
//
#define ERR_PNS_IF_PNIO_SETUP_FAILED     ((uint32_t)0xC0300004L)

//
// MessageId: ERR_PNS_IF_DEVICE_INFO_ALREADY_SET
//
// MessageText:
//
// Device information set already.
//
#define ERR_PNS_IF_DEVICE_INFO_ALREADY_SET ((uint32_t)0xC0300005L)

//
// MessageId: ERR_PNS_IF_SET_DEVICE_INFO_FAILED
//
// MessageText:
//
// Setting of device information failed.
//
#define ERR_PNS_IF_SET_DEVICE_INFO_FAILED ((uint32_t)0xC0300006L)

//
// MessageId: ERR_PNS_IF_NO_DEVICE_SETUP
//
// MessageText:
//
// PROFINET IO-Device stack is not initialized. Send PNS_IF_SET_DEVICEINFO_REQ before PNS_IF_OPEN_DEVICE_REQ.
//
#define ERR_PNS_IF_NO_DEVICE_SETUP       ((uint32_t)0xC0300007L)

//
// MessageId: ERR_PNS_IF_DEVICE_OPEN_FAILED
//
// MessageText:
//
// Opening a device instance failed.
//
#define ERR_PNS_IF_DEVICE_OPEN_FAILED    ((uint32_t)0xC0300008L)

//
// MessageId: ERR_PNS_IF_NO_DEVICE_INSTANCE
//
// MessageText:
//
// No device instance open.
//
#define ERR_PNS_IF_NO_DEVICE_INSTANCE    ((uint32_t)0xC0300009L)

//
// MessageId: ERR_PNS_IF_PLUG_MODULE_FAILED
//
// MessageText:
//
// Plugging a module failed.
//
#define ERR_PNS_IF_PLUG_MODULE_FAILED    ((uint32_t)0xC030000AL)

//
// MessageId: ERR_PNS_IF_PLUG_SUBMODULE_FAILED
//
// MessageText:
//
// Plugging a submodule failed.
//
#define ERR_PNS_IF_PLUG_SUBMODULE_FAILED ((uint32_t)0xC030000BL)

//
// MessageId: ERR_PNS_IF_DEVICE_START_FAILED
//
// MessageText:
//
// Start of PROFINET IO-Device failed.
//
#define ERR_PNS_IF_DEVICE_START_FAILED   ((uint32_t)0xC030000CL)

//
// MessageId: ERR_PNS_IF_EDD_ENABLE_FAILED
//
// MessageText:
//
// Start of network communication failed.
//
#define ERR_PNS_IF_EDD_ENABLE_FAILED     ((uint32_t)0xC030000DL)

//
// MessageId: ERR_PNS_IF_ALLOC_MNGMNT_BUFFER_FAILED
//
// MessageText:
//
// Allocation of a device instance management buffer failed.
//
#define ERR_PNS_IF_ALLOC_MNGMNT_BUFFER_FAILED ((uint32_t)0xC030000EL)

//
// MessageId: ERR_PNS_IF_DEVICE_HANDLE_NULL
//
// MessageText:
//
// Given device handle is NULL.
//
#define ERR_PNS_IF_DEVICE_HANDLE_NULL    ((uint32_t)0xC030000FL)

//
// MessageId: ERR_PNS_IF_SET_APPL_READY_FAILED
//
// MessageText:
//
// Command PNS_IF_SET_APPL_READY_REQ failed.
//
#define ERR_PNS_IF_SET_APPL_READY_FAILED ((uint32_t)0xC0300010L)

//
// MessageId: ERR_PNS_IF_SET_DEVSTATE_FAILED
//
// MessageText:
//
// Command PNS_IF_SET_DEVSTATE_REQ failed.
//
#define ERR_PNS_IF_SET_DEVSTATE_FAILED   ((uint32_t)0xC0300011L)

//
// MessageId: ERR_PNS_IF_PULL_SUBMODULE_FAILED
//
// MessageText:
//
// Pulling the submodule failed.
//
#define ERR_PNS_IF_PULL_SUBMODULE_FAILED ((uint32_t)0xC0300012L)

//
// MessageId: ERR_PNS_IF_PULL_MODULE_FAILED
//
// MessageText:
//
// Pulling the module failed.
//
#define ERR_PNS_IF_PULL_MODULE_FAILED    ((uint32_t)0xC0300013L)

//
// MessageId: ERR_PNS_IF_WRONG_DEST_ID
//
// MessageText:
//
// Destination ID in command invalid.
//
#define ERR_PNS_IF_WRONG_DEST_ID         ((uint32_t)0xC0300014L)

//
// MessageId: ERR_PNS_IF_DEVICE_HANDLE_INVALID
//
// MessageText:
//
// Device Handle in command invalid.
//
#define ERR_PNS_IF_DEVICE_HANDLE_INVALID ((uint32_t)0xC0300015L)

//
// MessageId: ERR_PNS_IF_CALLBACK_TIMEOUT
//
// MessageText:
//
// PNS stack callback timeout.
//
#define ERR_PNS_IF_CALLBACK_TIMEOUT      ((uint32_t)0xC0300016L)

//
// MessageId: ERR_PNS_IF_PACKET_POOL_EMPTY
//
// MessageText:
//
// PNS_IF packet pool empty.
//
#define ERR_PNS_IF_PACKET_POOL_EMPTY     ((uint32_t)0xC0300017L)

//
// MessageId: ERR_PNS_IF_ADD_API_FAILED
//
// MessageText:
//
// Command PNS_IF_ADD_API_REQ failed.
//
#define ERR_PNS_IF_ADD_API_FAILED        ((uint32_t)0xC0300018L)

//
// MessageId: ERR_PNS_IF_SET_SUB_STATE_FAILED
//
// MessageText:
//
// Setting submodule state failed.
//
#define ERR_PNS_IF_SET_SUB_STATE_FAILED  ((uint32_t)0xC0300019L)

//
// MessageId: ERR_PNS_IF_NO_NW_DBM_ERROR
//
// MessageText:
//
// No network configuration DBM-file.
//
#define ERR_PNS_IF_NO_NW_DBM_ERROR       ((uint32_t)0xC030001AL)

//
// MessageId: ERR_PNS_IF_NW_SETUP_TABLE_ERROR
//
// MessageText:
//
// Error during reading the "SETUP" table of the network configuration DBM-file .
//
#define ERR_PNS_IF_NW_SETUP_TABLE_ERROR  ((uint32_t)0xC030001BL)

//
// MessageId: ERR_PNS_IF_CFG_SETUP_TABLE_ERROR
//
// MessageText:
//
// Error during reading the "SETUP" table of the config.xxx DBM-file .
//
#define ERR_PNS_IF_CFG_SETUP_TABLE_ERROR ((uint32_t)0xC030001CL)

//
// MessageId: ERR_PNS_IF_NO_CFG_DBM_ERROR
//
// MessageText:
//
// No config.xxx DBM-file.
//
#define ERR_PNS_IF_NO_CFG_DBM_ERROR      ((uint32_t)0xC030001DL)

//
// MessageId: ERR_PNS_IF_DBM_DATASET_ERROR
//
// MessageText:
//
// Error getting dataset pointer.
//
#define ERR_PNS_IF_DBM_DATASET_ERROR     ((uint32_t)0xC030001EL)

//
// MessageId: ERR_PNS_IF_SETUPEX_TABLE_ERROR
//
// MessageText:
//
// Error getting dataset pointer(SETUP_EX table).
//
#define ERR_PNS_IF_SETUPEX_TABLE_ERROR   ((uint32_t)0xC030001FL)

//
// MessageId: ERR_PNS_IF_AP_TABLE_ERROR
//
// MessageText:
//
// Error getting either dataset pointer or number of datasets(AP table).
//
#define ERR_PNS_IF_AP_TABLE_ERROR        ((uint32_t)0xC0300020L)

//
// MessageId: ERR_PNS_IF_MODULES_TABLE_ERROR
//
// MessageText:
//
// Error getting either dataset pointer or number of datasets(MODULE table).
//
#define ERR_PNS_IF_MODULES_TABLE_ERROR   ((uint32_t)0xC0300021L)

//
// MessageId: ERR_PNS_IF_SUBMODULES_TABLE_ERROR
//
// MessageText:
//
// Error getting either dataset pointer or number of datasets(SUBMODULE table).
//
#define ERR_PNS_IF_SUBMODULES_TABLE_ERROR ((uint32_t)0xC0300022L)

//
// MessageId: ERR_PNS_IF_PNIO_SETUP_ERROR
//
// MessageText:
//
// Error setting up PNIO configuration(PNIO_setup()).
//
#define ERR_PNS_IF_PNIO_SETUP_ERROR      ((uint32_t)0xC0300023L)

//
// MessageId: ERR_PNS_IF_MODULES_GET_REC
//
// MessageText:
//
// Error getting record of "MODULES" linked table.
//
#define ERR_PNS_IF_MODULES_GET_REC       ((uint32_t)0xC0300024L)

//
// MessageId: ERR_PNS_IF_SUBMODULES_GET_REC
//
// MessageText:
//
// Error getting record of "SUBMODULES" linked table.
//
#define ERR_PNS_IF_SUBMODULES_GET_REC    ((uint32_t)0xC0300025L)

//
// MessageId: ERR_PNS_IF_PNIOD_MODULE_ID_TABLE_ERROR
//
// MessageText:
//
// Error accessing "PNIOD_MODULE_ID" table or table record error.
//
#define ERR_PNS_IF_PNIOD_MODULE_ID_TABLE_ERROR ((uint32_t)0xC0300026L)

//
// MessageId: ERR_PNS_IF_SIGNALS_TABLE_ERROR
//
// MessageText:
//
// Error accessing "SIGNALS" table or table record error.
//
#define ERR_PNS_IF_SIGNALS_TABLE_ERROR   ((uint32_t)0xC0300027L)

//
// MessageId: ERR_PNS_IF_MODULES_IO_TABLE_ERROR
//
// MessageText:
//
// Error accessing "MODULES_IO" table or table record error.
//
#define ERR_PNS_IF_MODULES_IO_TABLE_ERROR ((uint32_t)0xC0300028L)

//
// MessageId: ERR_PNS_IF_CHANNEL_SETTING_TABLE_ERROR
//
// MessageText:
//
// Error accessing "CHANNEL_SETTING" table or table record error.
//
#define ERR_PNS_IF_CHANNEL_SETTING_TABLE_ERROR ((uint32_t)0xC0300029L)

//
// MessageId: ERR_PNS_IF_WRITE_DBM
//
// MessageText:
//
// Error writing DBM-file.
//
#define ERR_PNS_IF_WRITE_DBM             ((uint32_t)0xC030002AL)

//
// MessageId: ERR_PNS_IF_DPM_CONFIG
//
// MessageText:
//
// No basic DPM configuration.
//
#define ERR_PNS_IF_DPM_CONFIG            ((uint32_t)0xC030002BL)

//
// MessageId: ERR_PNS_IF_WATCHDOG
//
// MessageText:
//
// Application did not trigger the watchdog.
//
#define ERR_PNS_IF_WATCHDOG              ((uint32_t)0xC030002CL)

//
// MessageId: ERR_PNS_IF_SIGNALS_SUBMODULES
//
// MessageText:
//
// Data length in "SIGNALS" table does not correspond to that in "SUBMODULES" table.
//
#define ERR_PNS_IF_SIGNALS_SUBMODULES    ((uint32_t)0xC030002DL)

//
// MessageId: ERR_PNS_IF_READ_DPM_SUBAREA
//
// MessageText:
//
// Failed to read DPM subarea.
//
#define ERR_PNS_IF_READ_DPM_SUBAREA      ((uint32_t)0xC030002EL)

//
// MessageId: ERR_PNS_IF_MOD_0_SUB_1
//
// MessageText:
//
// Error configuring Module 0 Submodule 1.
//
#define ERR_PNS_IF_MOD_0_SUB_1           ((uint32_t)0xC030002FL)

//
// MessageId: ERR_PNS_IF_SIGNALS_LENGTH
//
// MessageText:
//
// Length of I/O signals is bigger than the size of DPM subarea.
//
#define ERR_PNS_IF_SIGNALS_LENGTH        ((uint32_t)0xC0300030L)

//
// MessageId: ERR_PNS_IF_SUB_TRANSFER_DIRECTION
//
// MessageText:
//
// A submodule cannot have input and outputs at the same time.
//
#define ERR_PNS_IF_SUB_TRANSFER_DIRECTION ((uint32_t)0xC0300031L)

//
// MessageId: ERR_PNS_IF_FORMAT_PNVOLUME
//
// MessageText:
//
// Error while formatting PNVOLUME.
//
#define ERR_PNS_IF_FORMAT_PNVOLUME       ((uint32_t)0xC0300032L)

//
// MessageId: ERR_PNS_IF_MOUNT_PNVOLUME
//
// MessageText:
//
// Error while mounting PNVOLUME.
//
#define ERR_PNS_IF_MOUNT_PNVOLUME        ((uint32_t)0xC0300033L)

//
// MessageId: ERR_PNS_IF_INIT_REMOTE
//
// MessageText:
//
// Error during initialization of the remote resources of the stack.
//
#define ERR_PNS_IF_INIT_REMOTE           ((uint32_t)0xC0300034L)

//
// MessageId: ERR_PNS_IF_WARMSTART_CONFIG_REDUNDANT
//
// MessageText:
//
// Warmstart parameters are redundant. The stack was configured with DBM or packets.
//
#define ERR_PNS_IF_WARMSTART_CONFIG_REDUNDANT ((uint32_t)0xC0300035L)

//
// MessageId: ERR_PNS_IF_WARMSTART_PARAMETER
//
// MessageText:
//
// Incorrect warmstart parameter(s).
//
#define ERR_PNS_IF_WARMSTART_PARAMETER   ((uint32_t)0xC0300036L)

//
// MessageId: ERR_PNS_IF_SET_APPL_STATE_READY
//
// MessageText:
//
// PNIO_set_appl_state_ready() returns error.
//
#define ERR_PNS_IF_SET_APPL_STATE_READY  ((uint32_t)0xC0300037L)

//
// MessageId: ERR_PNS_IF_SET_DEV_STATE
//
// MessageText:
//
// PNIO_set_dev_state() returns error.
//
#define ERR_PNS_IF_SET_DEV_STATE         ((uint32_t)0xC0300038L)

//
// MessageId: ERR_PNS_IF_PROCESS_ALARM_SEND
//
// MessageText:
//
// PNIO_process_alarm_send() returns error.
//
#define ERR_PNS_IF_PROCESS_ALARM_SEND    ((uint32_t)0xC0300039L)

//
// MessageId: ERR_PNS_IF_RET_OF_SUB_ALARM_SEND
//
// MessageText:
//
// PNIO_ret_of_sub_alarm_send() returns error.
//
#define ERR_PNS_IF_RET_OF_SUB_ALARM_SEND ((uint32_t)0xC030003AL)

//
// MessageId: ERR_PNS_IF_DIAG_ALARM_SEND
//
// MessageText:
//
// PNIO_diag_alarm_send() returns error.
//
#define ERR_PNS_IF_DIAG_ALARM_SEND       ((uint32_t)0xC030003BL)

//
// MessageId: ERR_PNS_IF_DIAG_GENERIC_ADD
//
// MessageText:
//
// PNIO_diag_generic_add() returns error.
//
#define ERR_PNS_IF_DIAG_GENERIC_ADD      ((uint32_t)0xC030003CL)

//
// MessageId: ERR_PNS_IF_DIAG_GENERIC_REMOVE
//
// MessageText:
//
// PNIO_diag_generic_remove() returns error.
//
#define ERR_PNS_IF_DIAG_GENERIC_REMOVE   ((uint32_t)0xC030003DL)

//
// MessageId: ERR_PNS_IF_DIAG_CHANNEL_ADD
//
// MessageText:
//
// PNIO_diag_channel_add() returns error.
//
#define ERR_PNS_IF_DIAG_CHANNEL_ADD      ((uint32_t)0xC030003EL)

//
// MessageId: ERR_PNS_IF_DIAG_CHANNEL_REMOVE
//
// MessageText:
//
// PNIO_diag_channel_remove() returns error.
//
#define ERR_PNS_IF_DIAG_CHANNEL_REMOVE   ((uint32_t)0xC030003FL)

//
// MessageId: ERR_PNS_IF_EXT_DIAG_CHANNEL_ADD
//
// MessageText:
//
// PNIO_ext_diag_channel_add() returns error.
//
#define ERR_PNS_IF_EXT_DIAG_CHANNEL_ADD  ((uint32_t)0xC0300040L)

//
// MessageId: ERR_PNS_IF_EXT_DIAG_CHANNEL_REMOVE
//
// MessageText:
//
// PNIO_ext_diag_channel_remove() returns error.
//
#define ERR_PNS_IF_EXT_DIAG_CHANNEL_REMOVE ((uint32_t)0xC0300041L)

//
// MessageId: ERR_PNS_IF_STATION_NAME_LEN
//
// MessageText:
//
// Parameter station name length is incorrect.
//
#define ERR_PNS_IF_STATION_NAME_LEN      ((uint32_t)0xC0300042L)

//
// MessageId: ERR_PNS_IF_STATION_NAME
//
// MessageText:
//
// Parameter station name is incorrect.
//
#define ERR_PNS_IF_STATION_NAME          ((uint32_t)0xC0300043L)

//
// MessageId: ERR_PNS_IF_STATION_TYPE_LEN
//
// MessageText:
//
// Parameter station type length is incorrect.
//
#define ERR_PNS_IF_STATION_TYPE_LEN      ((uint32_t)0xC0300044L)

//
// MessageId: ERR_PNS_IF_DEVICE_TYPE
//
// MessageText:
//
// Parameter device type is incorrect.
//
#define ERR_PNS_IF_DEVICE_TYPE           ((uint32_t)0xC0300045L)

//
// MessageId: ERR_PNS_IF_ORDER_ID
//
// MessageText:
//
// Parameter order id is incorrect.
//
#define ERR_PNS_IF_ORDER_ID              ((uint32_t)0xC0300046L)

//
// MessageId: ERR_PNS_IF_INPUT_STATUS
//
// MessageText:
//
// Parameter input data status bytes length is incorrect.
//
#define ERR_PNS_IF_INPUT_STATUS          ((uint32_t)0xC0300047L)

//
// MessageId: ERR_PNS_IF_OUTPUT_STATUS
//
// MessageText:
//
// Parameter output data status bytes length is incorrect.
//
#define ERR_PNS_IF_OUTPUT_STATUS         ((uint32_t)0xC0300048L)

//
// MessageId: ERR_PNS_IF_WATCHDOG_PARAMETER
//
// MessageText:
//
// Parameter watchdog timing is incorrect(must be >= 10).
//
#define ERR_PNS_IF_WATCHDOG_PARAMETER    ((uint32_t)0xC0300049L)

//
// MessageId: ERR_PNS_IF_OUT_UPDATE
//
// MessageText:
//
// Parameter output data update timing is incorrect.
//
#define ERR_PNS_IF_OUT_UPDATE            ((uint32_t)0xC030004AL)

//
// MessageId: ERR_PNS_IF_IN_UPDATE
//
// MessageText:
//
// Parameter input data update timing is incorrect.
//
#define ERR_PNS_IF_IN_UPDATE             ((uint32_t)0xC030004BL)

//
// MessageId: ERR_PNS_IF_IN_SIZE
//
// MessageText:
//
// Parameter input memory area size is incorrect.
//
#define ERR_PNS_IF_IN_SIZE               ((uint32_t)0xC030004CL)

//
// MessageId: ERR_PNS_IF_OUT_SIZE
//
// MessageText:
//
// Parameter output memory area size is incorrect.
//
#define ERR_PNS_IF_OUT_SIZE              ((uint32_t)0xC030004DL)

//
// MessageId: ERR_PNS_IF_GLOBAL_RESOURCES
//
// MessageText:
//
// Unable to allocate memory for global access to local resources.
//
#define ERR_PNS_IF_GLOBAL_RESOURCES      ((uint32_t)0xC030004EL)

//
// MessageId: ERR_PNS_IF_DYNAMIC_CFG_PCK
//
// MessageText:
//
// Unable to allocate memory for dynamic configuration packet.
//
#define ERR_PNS_IF_DYNAMIC_CFG_PCK       ((uint32_t)0xC030004FL)

//
// MessageId: ERR_PNS_IF_DEVICE_STOP
//
// MessageText:
//
// Unable to stop device.
//
#define ERR_PNS_IF_DEVICE_STOP           ((uint32_t)0xC0300050L)

//
// MessageId: ERR_PNS_IF_DEVICE_ID
//
// MessageText:
//
// Parameter device id is incorrect.
//
#define ERR_PNS_IF_DEVICE_ID             ((uint32_t)0xC0300051L)

//
// MessageId: ERR_PNS_IF_VENDOR_ID
//
// MessageText:
//
// Parameter vendor id is incorrect.
//
#define ERR_PNS_IF_VENDOR_ID             ((uint32_t)0xC0300052L)

//
// MessageId: ERR_PNS_IF_SYS_START
//
// MessageText:
//
// Parameter system start is incorrect.
//
#define ERR_PNS_IF_SYS_START             ((uint32_t)0xC0300053L)

//
// MessageId: ERR_PNS_IF_DYN_CFG_IO_LENGTH
//
// MessageText:
//
// The length of IO data expected by the controller exceeds the limit specified in warmstart parameters.
//
#define ERR_PNS_IF_DYN_CFG_IO_LENGTH     ((uint32_t)0xC0300054L)

//
// MessageId: ERR_PNS_IF_DYN_CFG_MOD_NUM
//
// MessageText:
//
// The count of the IO modules expected by the controller exceeds the supported by the stack count.
//
#define ERR_PNS_IF_DYN_CFG_MOD_NUM       ((uint32_t)0xC0300055L)

//
// MessageId: ERR_PNS_IF_ACCESS_LOCAL_RSC
//
// MessageText:
//
// No global access to local resources.
//
#define ERR_PNS_IF_ACCESS_LOCAL_RSC      ((uint32_t)0xC0300056L)

//
// MessageId: ERR_PNS_IF_PULL_PLUG
//
// MessageText:
//
// Plugging and pulling modules during creation of communication is not allowed.
//
#define ERR_PNS_IF_PULL_PLUG             ((uint32_t)0xC0300057L)

//
// MessageId: ERR_PNS_IF_AR_NUM
//
// MessageText:
//
// Maximum number of ARs is 1.
//
#define ERR_PNS_IF_AR_NUM                ((uint32_t)0xC0300058L)

//
// MessageId: ERR_PNS_IF_API_NUM
//
// MessageText:
//
// Only API = 0 is supported.
//
#define ERR_PNS_IF_API_NUM               ((uint32_t)0xC0300059L)

//
// MessageId: ERR_PNS_IF_ALREADY_OPEN
//
// MessageText:
//
// Device is already opened.
//
#define ERR_PNS_IF_ALREADY_OPEN          ((uint32_t)0xC030005AL)

//
// MessageId: ERR_PNS_IF_API_ADDED
//
// MessageText:
//
// Application is already added.
//
#define ERR_PNS_IF_API_ADDED             ((uint32_t)0xC030005BL)

//
// MessageId: ERR_PNS_IF_CONFIG_MODE
//
// MessageText:
//
// Configuration modes should not be mixed( DBM-files,application,warmstart message).
//
#define ERR_PNS_IF_CONFIG_MODE           ((uint32_t)0xC030005CL)

//
// MessageId: ERR_PNS_IF_UNK_LED_MODE
//
// MessageText:
//
// Unknown LED mode.
//
#define ERR_PNS_IF_UNK_LED_MODE          ((uint32_t)0xC030005DL)

//
// MessageId: ERR_PNS_IF_PHYSICAL_LINK
//
// MessageText:
//
// Physical link rate is less than 100 Mbit.
//
#define ERR_PNS_IF_PHYSICAL_LINK         ((uint32_t)0xC030005EL)

//
// MessageId: ERR_PNS_IF_MAX_SLOT_SUBSLOT
//
// MessageText:
//
// Number of slots or subslots too big.
//
#define ERR_PNS_IF_MAX_SLOT_SUBSLOT      ((uint32_t)0xC030005FL)

//
// MessageId: ERR_PNS_IF_AR_REASON_MEM
//
// MessageText:
//
// AR error. Out of memory.
//
#define ERR_PNS_IF_AR_REASON_MEM         ((uint32_t)0xC0300060L)

//
// MessageId: ERR_PNS_IF_AR_REASON_FRAME
//
// MessageText:
//
// AR error. Add provider or consumer failed.
//
#define ERR_PNS_IF_AR_REASON_FRAME       ((uint32_t)0xC0300061L)

//
// MessageId: ERR_PNS_IF_AR_REASON_MISS
//
// MessageText:
//
// AR error. Consumer missing.
//
#define ERR_PNS_IF_AR_REASON_MISS        ((uint32_t)0xC0300062L)

//
// MessageId: ERR_PNS_IF_AR_REASON_TIMER
//
// MessageText:
//
// AR error. CMI timeout.
//
#define ERR_PNS_IF_AR_REASON_TIMER       ((uint32_t)0xC0300063L)

//
// MessageId: ERR_PNS_IF_AR_REASON_ALARM
//
// MessageText:
//
// AR error. Alarm open failed.
//
#define ERR_PNS_IF_AR_REASON_ALARM       ((uint32_t)0xC0300064L)

//
// MessageId: ERR_PNS_IF_AR_REASON_ALSND
//
// MessageText:
//
// AR error. Alarm send confirmation failed.
//
#define ERR_PNS_IF_AR_REASON_ALSND       ((uint32_t)0xC0300065L)

//
// MessageId: ERR_PNS_IF_AR_REASON_ALACK
//
// MessageText:
//
// AR error. Alarm acknowledge send confirmation failed.
//
#define ERR_PNS_IF_AR_REASON_ALACK       ((uint32_t)0xC0300066L)

//
// MessageId: ERR_PNS_IF_AR_REASON_ALLEN
//
// MessageText:
//
// AR error. Alarm data too long.
//
#define ERR_PNS_IF_AR_REASON_ALLEN       ((uint32_t)0xC0300067L)

//
// MessageId: ERR_PNS_IF_AR_REASON_ASRT
//
// MessageText:
//
// AR error. Alarm indication error.
//
#define ERR_PNS_IF_AR_REASON_ASRT        ((uint32_t)0xC0300068L)

//
// MessageId: ERR_PNS_IF_AR_REASON_RPC
//
// MessageText:
//
// AR error. RPC client call confirmation failed.
//
#define ERR_PNS_IF_AR_REASON_RPC         ((uint32_t)0xC0300069L)

//
// MessageId: ERR_PNS_IF_AR_REASON_ABORT
//
// MessageText:
//
// AR error. Abort request.
//
#define ERR_PNS_IF_AR_REASON_ABORT       ((uint32_t)0xC030006AL)

//
// MessageId: ERR_PNS_IF_AR_REASON_RERUN
//
// MessageText:
//
// AR error. Re-Run.
//
#define ERR_PNS_IF_AR_REASON_RERUN       ((uint32_t)0xC030006BL)

//
// MessageId: ERR_PNS_IF_AR_REASON_REL
//
// MessageText:
//
// AR error. Release indication received.
//
#define ERR_PNS_IF_AR_REASON_REL         ((uint32_t)0xC030006CL)

//
// MessageId: ERR_PNS_IF_AR_REASON_PAS
//
// MessageText:
//
// AR error. Device deactivated.
//
#define ERR_PNS_IF_AR_REASON_PAS         ((uint32_t)0xC030006DL)

//
// MessageId: ERR_PNS_IF_AR_REASON_RMV
//
// MessageText:
//
// AR error. Device/ar removed.
//
#define ERR_PNS_IF_AR_REASON_RMV         ((uint32_t)0xC030006EL)

//
// MessageId: ERR_PNS_IF_AR_REASON_PROT
//
// MessageText:
//
// AR error. Protocol violation.
//
#define ERR_PNS_IF_AR_REASON_PROT        ((uint32_t)0xC030006FL)

//
// MessageId: ERR_PNS_IF_AR_REASON_NARE
//
// MessageText:
//
// AR error. NARE error.
//
#define ERR_PNS_IF_AR_REASON_NARE        ((uint32_t)0xC0300070L)

//
// MessageId: ERR_PNS_IF_AR_REASON_BIND
//
// MessageText:
//
// AR error. RPC-Bind error.
//
#define ERR_PNS_IF_AR_REASON_BIND        ((uint32_t)0xC0300071L)

//
// MessageId: ERR_PNS_IF_AR_REASON_CONNECT
//
// MessageText:
//
// AR error. RPC-Connect error.
//
#define ERR_PNS_IF_AR_REASON_CONNECT     ((uint32_t)0xC0300072L)

//
// MessageId: ERR_PNS_IF_AR_REASON_READ
//
// MessageText:
//
// AR error. RPC-Read error.
//
#define ERR_PNS_IF_AR_REASON_READ        ((uint32_t)0xC0300073L)

//
// MessageId: ERR_PNS_IF_AR_REASON_WRITE
//
// MessageText:
//
// AR error. RPC-Write error.
//
#define ERR_PNS_IF_AR_REASON_WRITE       ((uint32_t)0xC0300074L)

//
// MessageId: ERR_PNS_IF_AR_REASON_CONTROL
//
// MessageText:
//
// AR error. RPC-Control error.
//
#define ERR_PNS_IF_AR_REASON_CONTROL     ((uint32_t)0xC0300075L)

//
// MessageId: ERR_PNS_IF_AR_REASON_UNKNOWN
//
// MessageText:
//
// AR error. Unknown.
//
#define ERR_PNS_IF_AR_REASON_UNKNOWN     ((uint32_t)0xC0300076L)

//
// MessageId: ERR_PNS_IF_INIT_WATCHDOG
//
// MessageText:
//
// Watchdog initialization failed.
//
#define ERR_PNS_IF_INIT_WATCHDOG         ((uint32_t)0xC0300077L)

//
// MessageId: ERR_PNS_IF_NO_PHYSICAL_LINK
//
// MessageText:
//
// The Device is not connected to a network.
//
#define ERR_PNS_IF_NO_PHYSICAL_LINK      ((uint32_t)0xC0300078L)

//
// MessageId: ERR_PNS_IF_DPM_CYCLIC_IO_RW
//
// MessageText:
//
// Failed to copy from DPM or to DPM the cyclic IO data.
//
#define ERR_PNS_IF_DPM_CYCLIC_IO_RW      ((uint32_t)0xC0300079L)

//
// MessageId: ERR_PNS_IF_SUBMODULE
//
// MessageText:
//
// Submodule number is wrong.
//
#define ERR_PNS_IF_SUBMODULE             ((uint32_t)0xC030007AL)

//
// MessageId: ERR_PNS_IF_MODULE
//
// MessageText:
//
// Module number is wrong.
//
#define ERR_PNS_IF_MODULE                ((uint32_t)0xC030007BL)

//
// MessageId: ERR_PNS_IF_NO_AR
//
// MessageText:
//
// The AR was closed or the AR handle is not valid.
//
#define ERR_PNS_IF_NO_AR                 ((uint32_t)0xC030007CL)

//
// MessageId: ERR_PNS_IF_WRITE_REC_RES_TIMEOUT
//
// MessageText:
//
// Timeout while waiting for response to write_record_indication.
//
#define ERR_PNS_IF_WRITE_REC_RES_TIMEOUT ((uint32_t)0xC030007DL)

//
// MessageId: ERR_PNS_IF_UNREGISTERED_SENDER
//
// MessageText:
//
// The sender of the request in not registered with request PNS_IF_REGISTER_AP_REQ.
//
#define ERR_PNS_IF_UNREGISTERED_SENDER   ((uint32_t)0xC030007EL)

//
// MessageId: ERR_PNS_IF_RECORD_HANDLE_INVALID
//
// MessageText:
//
// Unknown record handle.
//
#define ERR_PNS_IF_RECORD_HANDLE_INVALID ((uint32_t)0xC030007FL)

//
// MessageId: ERR_PNS_IF_REGISTER_AP
//
// MessageText:
//
// Another instance is registered at the moment.
//
#define ERR_PNS_IF_REGISTER_AP           ((uint32_t)0xC0300080L)

//
// MessageId: ERR_PNS_IF_UNREGISTER_AP
//
// MessageText:
//
// One instance cannot unregister another one.
//
#define ERR_PNS_IF_UNREGISTER_AP         ((uint32_t)0xC0300081L)

//
// MessageId: ERR_PNS_IF_CONFIG_DIFFER
//
// MessageText:
//
// The Must-configuration differs from the Is-configuration.
//
#define ERR_PNS_IF_CONFIG_DIFFER         ((uint32_t)0xC0300082L)

//
// MessageId: ERR_PNS_IF_NO_COMMUNICATION
//
// MessageText:
//
// No communication processing.
//
#define ERR_PNS_IF_NO_COMMUNICATION      ((uint32_t)0xC0300083L)

//
// MessageId: ERR_PNS_IF_BAD_PARAMETER
//
// MessageText:
//
// At least one parameter in a packet was wrong or/and did not meet the requirements.
//
#define ERR_PNS_IF_BAD_PARAMETER         ((uint32_t)0xC0300084L)

//
// MessageId: ERR_PNS_IF_AREA_OVERFLOW
//
// MessageText:
//
// Input or Output data requires more space than available.
//
#define ERR_PNS_IF_AREA_OVERFLOW         ((uint32_t)0xC0300085L)

//
// MessageId: ERR_PNS_IF_WRM_PCK_SAVE
//
// MessageText:
//
// Saving Warmstart Configuration for later use was not successful.
//
#define ERR_PNS_IF_WRM_PCK_SAVE          ((uint32_t)0xC0300086L)

//
// MessageId: ERR_PNS_IF_AR_REASON_PULLPLUG
//
// MessageText:
//
// AR error. Pull and Plug are forbidden after check.rsp and before in-data.ind.
//
#define ERR_PNS_IF_AR_REASON_PULLPLUG    ((uint32_t)0xC0300087L)

//
// MessageId: ERR_PNS_IF_AR_REASON_AP_RMV
//
// MessageText:
//
// AR error. AP has been removed.
//
#define ERR_PNS_IF_AR_REASON_AP_RMV      ((uint32_t)0xC0300088L)

//
// MessageId: ERR_PNS_IF_AR_REASON_LNK_DWN
//
// MessageText:
//
// AR error. Link "down".
//
#define ERR_PNS_IF_AR_REASON_LNK_DWN     ((uint32_t)0xC0300089L)

//
// MessageId: ERR_PNS_IF_AR_REASON_MMAC
//
// MessageText:
//
// AR error. Could not register multicast-MAC.
//
#define ERR_PNS_IF_AR_REASON_MMAC        ((uint32_t)0xC030008AL)

//
// MessageId: ERR_PNS_IF_AR_REASON_SYNC
//
// MessageText:
//
// AR error. Not synchronized (Cannot start companion-AR).
//
#define ERR_PNS_IF_AR_REASON_SYNC        ((uint32_t)0xC030008BL)

//
// MessageId: ERR_PNS_IF_AR_REASON_TOPO
//
// MessageText:
//
// AR error. Wrong topology(Cannot start companion-AR).
//
#define ERR_PNS_IF_AR_REASON_TOPO        ((uint32_t)0xC030008CL)

//
// MessageId: ERR_PNS_IF_AR_REASON_DCP_NAME
//
// MessageText:
//
// AR error. DCP. Station Name changed.
//
#define ERR_PNS_IF_AR_REASON_DCP_NAME    ((uint32_t)0xC030008DL)

//
// MessageId: ERR_PNS_IF_AR_REASON_DCP_RESET
//
// MessageText:
//
// AR error. DCP. Reset to factory-settings.
//
#define ERR_PNS_IF_AR_REASON_DCP_RESET   ((uint32_t)0xC030008EL)

//
// MessageId: ERR_PNS_IF_AR_REASON_PRM
//
// MessageText:
//
// AR error. Cannot start companion-AR because a 0x8ipp submodule in the first AR /has appl-ready-pending/ is locked/ is wrong or pulled/ .
//
#define ERR_PNS_IF_AR_REASON_PRM         ((uint32_t)0xC030008FL)

//
// MessageId: ERR_PNS_IF_PACKET_MNGMNT
//
// MessageText:
//
// Packet management error.
//
#define ERR_PNS_IF_PACKET_MNGMNT         ((uint32_t)0xC0300090L)

//
// MessageId: ERR_PNS_IF_WRONG_API_NUM
//
// MessageText:
//
// Invalid parameter API.
//
#define ERR_PNS_IF_WRONG_API_NUM         ((uint32_t)0xC0300091L)

//
// MessageId: ERR_PNS_IF_WRONG_MODULE_ID
//
// MessageText:
//
// Invalid parameter ModuleIdentifier (a module with different ModuleIdentifier is already plugged).
//
#define ERR_PNS_IF_WRONG_MODULE_ID       ((uint32_t)0xC0300092L)

//
// MessageId: ERR_PNS_IF_WRONG_MODULE_NUM
//
// MessageText:
//
// Obsolete, no longer used.
//
#define ERR_PNS_IF_WRONG_MODULE_NUM      ((uint32_t)0xC0300093L)

//
// MessageId: ERR_PNS_IF_UNS_AREA
//
// MessageText:
//
// Obsolete, no longer used.
//
#define ERR_PNS_IF_UNS_AREA              ((uint32_t)0xC0300094L)

//
// MessageId: ERR_PNS_IF_WRONG_SUB_ID
//
// MessageText:
//
// Obsolete, no longer used.
//
#define ERR_PNS_IF_WRONG_SUB_ID          ((uint32_t)0xC0300095L)

//
// MessageId: ERR_PNS_IF_WRONG_SUBMODULE_NUM
//
// MessageText:
//
// Obsolete, no longer used.
//
#define ERR_PNS_IF_WRONG_SUBMODULE_NUM   ((uint32_t)0xC0300096L)

//
// MessageId: ERR_PNS_IF_DEVICE_STOP_FAILED
//
// MessageText:
//
// Obsolete, no longer used.
//
#define ERR_PNS_IF_DEVICE_STOP_FAILED    ((uint32_t)0xC0300097L)

//
// MessageId: ERR_PNS_IF_EDD_DISABLE_FAILED
//
// MessageText:
//
// Obsolete, no longer used.
//
#define ERR_PNS_IF_EDD_DISABLE_FAILED    ((uint32_t)0xC0300098L)

//
// MessageId: ERR_PNS_IF_WRITE_IN
//
// MessageText:
//
// Obsolete, no longer used.
//
#define ERR_PNS_IF_WRITE_IN              ((uint32_t)0xC0300099L)

//
// MessageId: ERR_PNS_IF_READ_OUT
//
// MessageText:
//
// Obsolete, no longer used.
//
#define ERR_PNS_IF_READ_OUT              ((uint32_t)0xC030009AL)

//
// MessageId: ERR_PNS_IF_PNIO_STATUS
//
// MessageText:
//
// Obsolete, no longer used.
//
#define ERR_PNS_IF_PNIO_STATUS           ((uint32_t)0xC030009BL)

//
// MessageId: ERR_PNS_IF_WRONG_MODULE_ADDRESS
//
// MessageText:
//
// Obsolete, no longer used.
//
#define ERR_PNS_IF_WRONG_MODULE_ADDRESS  ((uint32_t)0xC030009CL)

//
// MessageId: ERR_PNS_IF_UNK_DEVICE_STATE
//
// MessageText:
//
// Obsolete, no longer used.
//
#define ERR_PNS_IF_UNK_DEVICE_STATE      ((uint32_t)0xC030009DL)

//
// MessageId: ERR_PNS_IF_ALARM_DATA_LEN
//
// MessageText:
//
// Invalid alarm data length.
//
#define ERR_PNS_IF_ALARM_DATA_LEN        ((uint32_t)0xC030009EL)

//
// MessageId: ERR_PNS_IF_UNK_SUBMODULE_STATE
//
// MessageText:
//
// Obsolete, no longer used.
//
#define ERR_PNS_IF_UNK_SUBMODULE_STATE   ((uint32_t)0xC030009FL)

//
// MessageId: ERR_PNS_IF_BAD_DIAG_HANDLE
//
// MessageText:
//
// Invalid parameter Diagnosis handle.
//
#define ERR_PNS_IF_BAD_DIAG_HANDLE       ((uint32_t)0xC03000A0L)

//
// MessageId: ERR_PNS_IF_UNS_STRUCT_ID
//
// MessageText:
//
// Obsolete, no longer used.
//
#define ERR_PNS_IF_UNS_STRUCT_ID         ((uint32_t)0xC03000A1L)

//
// MessageId: ERR_PNS_IF_UNK_ALARM_STATE
//
// MessageText:
//
// Obsolete, no longer used.
//
#define ERR_PNS_IF_UNK_ALARM_STATE       ((uint32_t)0xC03000A2L)

//
// MessageId: ERR_PNS_IF_DIAG_DATA_LEN
//
// MessageText:
//
// Obsolete, no longer used.
//
#define ERR_PNS_IF_DIAG_DATA_LEN         ((uint32_t)0xC03000A3L)

//
// MessageId: ERR_PNS_IF_BAD_CHANNEL_ERR_TYPE
//
// MessageText:
//
// Obsolete, no longer used.
//
#define ERR_PNS_IF_BAD_CHANNEL_ERR_TYPE  ((uint32_t)0xC03000A4L)

//
// MessageId: ERR_PNS_IF_BAD_CHANNEL_PROP
//
// MessageText:
//
// Obsolete, no longer used.
//
#define ERR_PNS_IF_BAD_CHANNEL_PROP      ((uint32_t)0xC03000A5L)

//
// MessageId: ERR_PNS_IF_BAD_CHANNEL_NUM
//
// MessageText:
//
// Obsolete, no longer used.
//
#define ERR_PNS_IF_BAD_CHANNEL_NUM       ((uint32_t)0xC03000A6L)

//
// MessageId: ERR_PNS_IF_RCX_RESTART
//
// MessageText:
//
// Obsolete, no longer used.
//
#define ERR_PNS_IF_RCX_RESTART           ((uint32_t)0xC03000A7L)

//
// MessageId: ERR_PNS_IF_CFG_MNGMNT
//
// MessageText:
//
// Obsolete, no longer used.
//
#define ERR_PNS_IF_CFG_MNGMNT            ((uint32_t)0xC03000A8L)

//
// MessageId: ERR_PNS_IF_UNK_INTERN_REQ
//
// MessageText:
//
// Obsolete, no longer used.
//
#define ERR_PNS_IF_UNK_INTERN_REQ        ((uint32_t)0xC03000A9L)

//
// MessageId: ERR_PNS_IF_CFG_STORE
//
// MessageText:
//
// Obsolete, no longer used.
//
#define ERR_PNS_IF_CFG_STORE             ((uint32_t)0xC03000AAL)

//
// MessageId: ERR_PNS_IF_CFG_DELETE_FAILED
//
// MessageText:
//
// An internal error occurred while deleting the configuration.
//
#define ERR_PNS_IF_CFG_DELETE_FAILED     ((uint32_t)0xC03000ABL)

//
// MessageId: ERR_PNS_IF_READ_CFG
//
// MessageText:
//
// Obsolete, no longer used.
//
#define ERR_PNS_IF_READ_CFG              ((uint32_t)0xC03000ACL)

//
// MessageId: ERR_PNS_IF_ACCESS_SYS_VOLUME
//
// MessageText:
//
// Obsolete, no longer used.
//
#define ERR_PNS_IF_ACCESS_SYS_VOLUME     ((uint32_t)0xC03000ADL)

//
// MessageId: ERR_PNS_IF_ACCESS_BCKUP_VOLUME
//
// MessageText:
//
// Obsolete, no longer used.
//
#define ERR_PNS_IF_ACCESS_BCKUP_VOLUME   ((uint32_t)0xC03000AEL)

//
// MessageId: ERR_PNS_IF_CFG_BAD_LEN
//
// MessageText:
//
// Obsolete, no longer used.
//
#define ERR_PNS_IF_CFG_BAD_LEN           ((uint32_t)0xC03000AFL)

//
// MessageId: ERR_PNS_IF_WRM_CFG_MNGMNT
//
// MessageText:
//
// Obsolete, no longer used.
//
#define ERR_PNS_IF_WRM_CFG_MNGMNT        ((uint32_t)0xC03000B0L)

//
// MessageId: ERR_PNS_IF_RESET_FACTORY_IND
//
// MessageText:
//
// No registered application. Reset_to_factory_settings Indication failed.
//
#define ERR_PNS_IF_RESET_FACTORY_IND     ((uint32_t)0xC03000B1L)

//
// MessageId: ERR_PNS_IF_MODULE_ALREADY_PLUGGED
//
// MessageText:
//
// A module was already plugged to the slot.
//
#define ERR_PNS_IF_MODULE_ALREADY_PLUGGED ((uint32_t)0xC03000B2L)

//
// MessageId: ERR_PNS_IF_OSINIT
//
// MessageText:
//
// Failed to init the OS adaptation layer.
//
#define ERR_PNS_IF_OSINIT                ((uint32_t)0xC03000B3L)

//
// MessageId: ERR_PNS_IF_OSSOCKINIT
//
// MessageText:
//
// Failed to init the TCPIP adaptation layer.
//
#define ERR_PNS_IF_OSSOCKINIT            ((uint32_t)0xC03000B4L)

//
// MessageId: ERR_PNS_IF_INVALID_NETMASK
//
// MessageText:
//
// Invalid subnetwork mask.
//
#define ERR_PNS_IF_INVALID_NETMASK       ((uint32_t)0xC03000B5L)

//
// MessageId: ERR_PNS_IF_INVALID_IP_ADDR
//
// MessageText:
//
// Invalid IP address.
//
#define ERR_PNS_IF_INVALID_IP_ADDR       ((uint32_t)0xC03000B6L)

//
// MessageId: ERR_PNS_IF_STA_STARTUP_PARAMETER
//
// MessageText:
//
// Erroneous Task start-up parameters.
//
#define ERR_PNS_IF_STA_STARTUP_PARAMETER ((uint32_t)0xC03000B7L)

//
// MessageId: ERR_PNS_IF_INIT_LOCAL
//
// MessageText:
//
// Failed to initialize the Task local resources.
//
#define ERR_PNS_IF_INIT_LOCAL            ((uint32_t)0xC03000B8L)

//
// MessageId: ERR_PNS_IF_APP_CONFIG_INCOMPLETE
//
// MessageText:
//
// The configuration per packets is incomplete.
//
#define ERR_PNS_IF_APP_CONFIG_INCOMPLETE ((uint32_t)0xC03000B9L)

//
// MessageId: ERR_PNS_IF_INIT_EDD
//
// MessageText:
//
// EDD Initialization failed.
//
#define ERR_PNS_IF_INIT_EDD              ((uint32_t)0xC03000BAL)

//
// MessageId: ERR_PNS_IF_DPM_NOT_ENABLED
//
// MessageText:
//
// DPM is not enabled.
//
#define ERR_PNS_IF_DPM_NOT_ENABLED       ((uint32_t)0xC03000BBL)

//
// MessageId: ERR_PNS_IF_READ_LINK_STATUS
//
// MessageText:
//
// Reading Link Status failed.
//
#define ERR_PNS_IF_READ_LINK_STATUS      ((uint32_t)0xC03000BCL)

//
// MessageId: ERR_PNS_IF_INVALID_GATEWAY
//
// MessageText:
//
// Invalid gateway address (not reachable with configured netmask).
//
#define ERR_PNS_IF_INVALID_GATEWAY       ((uint32_t)0xC03000BDL)

// these #defines are from Benjamin - for use with new PNIO stack (none-Siemens)
//
// MessageId: ERR_PNS_IF_PACKET_SEND_FAILED
//
// MessageText:
//
// Error while sending a packet to another task.
//
#define ERR_PNS_IF_PACKET_SEND_FAILED    ((uint32_t)0xC0300100L)

//
// MessageId: ERR_PNS_IF_RESOURCE_OUT_OF_MEMORY
//
// MessageText:
//
// Insufficient memory to handle the request.
//
#define ERR_PNS_IF_RESOURCE_OUT_OF_MEMORY ((uint32_t)0xC0300101L)

//
// MessageId: ERR_PNS_IF_NO_APPLICATION_REGISTERED
//
// MessageText:
//
// No application to send the indication to is registered.
//
#define ERR_PNS_IF_NO_APPLICATION_REGISTERED ((uint32_t)0xC0300102L)

//
// MessageId: ERR_PNS_IF_INVALID_SOURCE_ID
//
// MessageText:
//
// The host-application returned a packet with invalid (changed) SourceID.
//
#define ERR_PNS_IF_INVALID_SOURCE_ID     ((uint32_t)0xC0300103L)

//
// MessageId: ERR_PNS_IF_PACKET_BUFFER_FULL
//
// MessageText:
//
// The buffer used to store packets exchanged between host-application and stack is full.
//
#define ERR_PNS_IF_PACKET_BUFFER_FULL    ((uint32_t)0xC0300104L)

//
// MessageId: ERR_PNS_IF_PULL_NO_MODULE
//
// MessageText:
//
// Pulling the (sub)module failed because no module is plugged into the slot specified.
//
#define ERR_PNS_IF_PULL_NO_MODULE        ((uint32_t)0xC0300105L)

//
// MessageId: ERR_PNS_IF_PULL_NO_SUBMODULE
//
// MessageText:
//
// Pulling the submodule failed because no submodule is plugged into the subslot specified.
//
#define ERR_PNS_IF_PULL_NO_SUBMODULE     ((uint32_t)0xC0300106L)

//
// MessageId: ERR_PNS_IF_PACKET_BUFFER_RESTORE_ERROR
//
// MessageText:
//
// The packet buffer storing packets exchanged between host-application and stack returned an invalid packet.
//
#define ERR_PNS_IF_PACKET_BUFFER_RESTORE_ERROR ((uint32_t)0xC0300107L)

//
// MessageId: ERR_PNS_IF_DIAG_NO_MODULE
//
// MessageText:
//
// Diagnosis data not accepted because no module is plugged into the slot specified.
//
#define ERR_PNS_IF_DIAG_NO_MODULE        ((uint32_t)0xC0300108L)

//
// MessageId: ERR_PNS_IF_DIAG_NO_SUBMODULE
//
// MessageText:
//
// Diagnosis data not accepted because no submodule is plugged into the subslot specified.
//
#define ERR_PNS_IF_DIAG_NO_SUBMODULE     ((uint32_t)0xC0300109L)

//
// MessageId: ERR_PNS_IF_CYCLIC_EXCHANGE_ACTIVE
//
// MessageText:
//
// The services requested are not available while cyclic communication is running.
//
#define ERR_PNS_IF_CYCLIC_EXCHANGE_ACTIVE ((uint32_t)0xC030010AL)

//
// MessageId: ERR_PNS_IF_FATAL_ERROR_CLB_ALREADY_REGISTERED
//
// MessageText:
//
// This fatal error callback function could not be registered because there is already a function registered.
//
#define ERR_PNS_IF_FATAL_ERROR_CLB_ALREADY_REGISTERED ((uint32_t)0xC030010BL)

//
// MessageId: ERR_PNS_IF_ERROR_STACK_WARMSTART_CONFIGURATION
//
// MessageText:
//
// The stack did not accept the warmstart parameters.
//
#define ERR_PNS_IF_ERROR_STACK_WARMSTART_CONFIGURATION ((uint32_t)0xC030010CL)

//
// MessageId: ERR_PNS_IF_ERROR_STACK_MODULE_CONFIGURATION
//
// MessageText:
//
// The stack did not accept the module configuration packet.
//
#define ERR_PNS_IF_ERROR_STACK_MODULE_CONFIGURATION ((uint32_t)0xC030010DL)

//
// MessageId: ERR_PNS_IF_CHECK_IND_FOR_UNEXPECTED_MODULE
//
// MessageText:
//
// The stack sent a Check Indication for an unexpected module. This module was not part of the CR Info Indication.
//
#define ERR_PNS_IF_CHECK_IND_FOR_UNEXPECTED_MODULE ((uint32_t)0xC030010EL)

//
// MessageId: ERR_PNS_IF_CHECK_IND_FOR_UNEXPECTED_SUBMODULE
//
// MessageText:
//
// The stack sent a Check Indication for an unexpected submodule. This submodule was not part of the CR Info Indication.
//
#define ERR_PNS_IF_CHECK_IND_FOR_UNEXPECTED_SUBMODULE ((uint32_t)0xC030010FL)

//
// MessageId: ERR_PNS_DIAG_BUFFER_FULL
//
// MessageText:
//
// No more diagnosis records can be added to the stack because the maximum amount is already reached.
//
#define ERR_PNS_DIAG_BUFFER_FULL         ((uint32_t)0xC0300110L)

//
// MessageId: ERR_PNS_IF_CHECK_IND_FOR_UNEXPECTED_API
//
// MessageText:
//
// The stack sent a Check Indication for an unexpected API. This API was not part of the CR Info Indication.
//
#define ERR_PNS_IF_CHECK_IND_FOR_UNEXPECTED_API ((uint32_t)0xC0300111L)

//
// MessageId: ERR_PNS_IF_DPM_ACCESS_WITH_INVALID_OFFSET
//
// MessageText:
//
// The DPM shall be accessed with an invalid data offset.
//
#define ERR_PNS_IF_DPM_ACCESS_WITH_INVALID_OFFSET ((uint32_t)0xC0300112L)

//
// MessageId: ERR_PNS_IF_DUPLICATE_INPUT_CR_INFO
//
// MessageText:
//
// The stack indicated to CR Info Indications with type input.
//
#define ERR_PNS_IF_DUPLICATE_INPUT_CR_INFO ((uint32_t)0xC0300113L)

//
// MessageId: ERR_PNS_IF_DUPLICATE_OUTPUT_CR_INFO
//
// MessageText:
//
// The stack indicated to CR Info Indications with type output.
//
#define ERR_PNS_IF_DUPLICATE_OUTPUT_CR_INFO ((uint32_t)0xC0300114L)

//
// MessageId: ERR_PNS_IF_FAULTY_CR_INFO_IND_RECEIVED
//
// MessageText:
//
// The stack indicated a faulty CR Info Indications.
//
#define ERR_PNS_IF_FAULTY_CR_INFO_IND_RECEIVED ((uint32_t)0xC0300115L)

//
// MessageId: ERR_PNS_IF_CONFIG_RELOAD_RUNNING
//
// MessageText:
//
// The request cannot be executed because configuration reload or ChannelInit is running.
//
#define ERR_PNS_IF_CONFIG_RELOAD_RUNNING ((uint32_t)0xC0300116L)

//
// MessageId: ERR_PNS_IF_NO_MAC_ADDRESS_SET
//
// MessageText:
//
// There is no valid chassis MAC address set Without MAC address the stack will not work.
//
#define ERR_PNS_IF_NO_MAC_ADDRESS_SET    ((uint32_t)0xC0300117L)

//
// MessageId: ERR_PNS_IF_SET_PORT_MAC_NOT_POSSIBLE
//
// MessageText:
//
// The Port MAC addresses have to be set before sending Set-Configuration Request to the stack.
//
#define ERR_PNS_IF_SET_PORT_MAC_NOT_POSSIBLE ((uint32_t)0xC0300118L)

//
// MessageId: ERR_PNS_IF_INVALID_MODULE_CONFIGURATION
//
// MessageText:
//
// Evaluating the module configuration failed.
//
#define ERR_PNS_IF_INVALID_MODULE_CONFIGURATION ((uint32_t)0xC030011AL)

//
// MessageId: ERR_PNS_IF_CONF_IO_LEN_TO_BIG
//
// MessageText:
//
// The sum of IO-data length exceeds the maximum allowed value.
//
#define ERR_PNS_IF_CONF_IO_LEN_TO_BIG    ((uint32_t)0xC030011BL)

//
// MessageId: ERR_PNS_IF_NO_MODULE_CONFIGURED
//
// MessageText:
//
// The module configuration does not contain at least one module.
//
#define ERR_PNS_IF_NO_MODULE_CONFIGURED  ((uint32_t)0xC030011CL)

//
// MessageId: ERR_PNS_IF_INVALID_SW_REV_PREFIX
//
// MessageText:
//
// The value of bSwRevisionPrefix is invalid.
//
#define ERR_PNS_IF_INVALID_SW_REV_PREFIX ((uint32_t)0xC030011DL)

//
// MessageId: ERR_PNS_IF_RESERVED_VALUE_NOT_ZERO
//
// MessageText:
//
// The value of usReserved it not zero.
//
#define ERR_PNS_IF_RESERVED_VALUE_NOT_ZERO ((uint32_t)0xC030011EL)

//
// MessageId: ERR_PNS_IF_IDENTIFY_CMDEV_QUEUE_FAILED
//
// MessageText:
//
// Identifying the stack message queue CMDEV failed.
//
#define ERR_PNS_IF_IDENTIFY_CMDEV_QUEUE_FAILED ((uint32_t)0xC030011FL)

//
// MessageId: ERR_PNS_IF_CREATE_SYNC_QUEUE_FAILED
//
// MessageText:
//
// Creating the sync message queue failed.
//
#define ERR_PNS_IF_CREATE_SYNC_QUEUE_FAILED ((uint32_t)0xC0300120L)

//
// MessageId: ERR_PNS_IF_CREATE_ALARM_LOW_QUEUE_FAILED
//
// MessageText:
//
// Creating the low alarm message queue failed.
//
#define ERR_PNS_IF_CREATE_ALARM_LOW_QUEUE_FAILED ((uint32_t)0xC0300121L)

//
// MessageId: ERR_PNS_IF_CREATE_ALARM_HIGH_QUEUE_FAILED
//
// MessageText:
//
// Creating the high alarm message queue failed.
//
#define ERR_PNS_IF_CREATE_ALARM_HIGH_QUEUE_FAILED ((uint32_t)0xC0300122L)

//
// MessageId: ERR_PNS_IF_CFG_PACKET_TO_SMALL
//
// MessageText:
//
// While evaluating SetConfiguration packet the packet length was found smaller than amount of configured modules needs.
//
#define ERR_PNS_IF_CFG_PACKET_TO_SMALL   ((uint32_t)0xC0300123L)

//
// MessageId: ERR_PNS_IF_FATAL_ERROR_OCCURRED
//
// MessageText:
//
// A fatal error occurred prior to this request. Therefore this request cannot be fulfilled.
//
#define ERR_PNS_IF_FATAL_ERROR_OCCURRED  ((uint32_t)0xC0300124L)

//
// MessageId: ERR_PNS_IF_SUBMODULE_NOT_IN_CYCLIC_EXCHANGE
//
// MessageText:
//
// The request cannot be executed because the submodule is not in cyclic data exchange.
//
#define ERR_PNS_IF_SUBMODULE_NOT_IN_CYCLIC_EXCHANGE ((uint32_t)0xC0300125L)

//
// MessageId: ERR_PNS_IF_SERVICE_NOT_AVAILABLE_THROUGH_DPM
//
// MessageText:
//
// This service is not available through DPM.
//
#define ERR_PNS_IF_SERVICE_NOT_AVAILABLE_THROUGH_DPM ((uint32_t)0xC0300126L)

//
// MessageId: ERR_PNS_IF_INVALID_PARAMETER_VERSION
//
// MessageText:
//
// The version of parameters is invalid (most likely too old).
//
#define ERR_PNS_IF_INVALID_PARAMETER_VERSION ((uint32_t)0xC0300127L)

//
// MessageId: ERR_PNS_IF_DATABASE_USAGE_IS_FORBIDDEN
//
// MessageText:
//
// The usage of database is forbidden by startup parameters of task.
//
#define ERR_PNS_IF_DATABASE_USAGE_IS_FORBIDDEN ((uint32_t)0xC0300128L)

//
// MessageId: ERR_PNS_IF_RECORD_LENGTH_TOO_BIG
//
// MessageText:
//
// The amount of record data is too big.
//
#define ERR_PNS_IF_RECORD_LENGTH_TOO_BIG ((uint32_t)0xC0300129L)

//
// MessageId: ERR_PNS_IF_IDENTIFY_LLDP_QUEUE_FAILED
//
// MessageText:
//
// Identifying the stack message queue LLDP failed.
//
#define ERR_PNS_IF_IDENTIFY_LLDP_QUEUE_FAILED ((uint32_t)0xC030012AL)

//
// MessageId: ERR_PNS_IF_INVALID_TOTAL_PACKET_LENGTH
//
// MessageText:
//
// SetConfiguration Requests total packet length is invalid.
//
#define ERR_PNS_IF_INVALID_TOTAL_PACKET_LENGTH ((uint32_t)0xC030012BL)

//
// MessageId: ERR_PNS_IF_APPLICATION_TIMEOUT
//
// MessageText:
//
// The application needed too much time to respond to an indication.
//
#define ERR_PNS_IF_APPLICATION_TIMEOUT   ((uint32_t)0xC030012CL)

//
// MessageId: ERR_PNS_IF_PACKET_BUFFER_INVALID_PACKET
//
// MessageText:
//
// The packet buffer storing packets exchanged between host-application and stack returned a faulty packet.
//
#define ERR_PNS_IF_PACKET_BUFFER_INVALID_PACKET ((uint32_t)0xC030012DL)

//
// MessageId: ERR_PNS_IF_NO_IO_IMAGE_CONFIGURATION_AVAILABLE
//
// MessageText:
//
// The request cannot be handled until a valid IO Image configuration is available.
//
#define ERR_PNS_IF_NO_IO_IMAGE_CONFIGURATION_AVAILABLE ((uint32_t)0xC030012EL)

//
// MessageId: ERR_PNS_IF_IO_IMAGE_ALREADY_CONFIGURED
//
// MessageText:
//
// A valid IO Image configuration is already available.
//
#define ERR_PNS_IF_IO_IMAGE_ALREADY_CONFIGURED ((uint32_t)0xC030012FL)

//
// MessageId: ERR_PNS_IF_INVALID_PDEV_SUBSLOT
//
// MessageText:
//
// A submodule may only be plugged into a PDEV-subslot which does not exceed the number of supported interfaces and port numbers.
//
#define ERR_PNS_IF_INVALID_PDEV_SUBSLOT  ((uint32_t)0xC0300130L)

//
// MessageId: ERR_PNS_IF_NO_DAP_PRESENT
//
// MessageText:
//
// The module configuration does not contain a Device Access Point DAP-submodule in slot 0 subslot 1.
//
#define ERR_PNS_IF_NO_DAP_PRESENT        ((uint32_t)0xC0300131L)

//
// MessageId: ERR_PNS_IF_PLUG_SUBMOD_OUTPUT_SIZE_EXCEEDED
//
// MessageText:
//
// Output size of the submodule exceeded. Configured value of ulCompleteOutputSize is smaller than the Output size of all plugged input modules. Upgrade ulCompleteOutputSize.
//
#define ERR_PNS_IF_PLUG_SUBMOD_OUTPUT_SIZE_EXCEEDED ((uint32_t)0xC0300132L)

//
// MessageId: ERR_PNS_IF_PLUG_SUBMOD_INPUT_SIZE_EXCEEDED
//
// MessageText:
//
// Input size of the submodule exceeded. Configured value of ulCompleteInputSize is smaller than the Input size of all plugged input modules. Upgrade ulCompleteInputSize.
//
#define ERR_PNS_IF_PLUG_SUBMOD_INPUT_SIZE_EXCEEDED ((uint32_t)0xC0300133L)

//
// MessageId: ERR_PNS_IF_PLUG_SUBMOD_NO_MODULE_ATTACHED_TO_ADD_TO
//
// MessageText:
//
// No module attached to add the submodule to.
//
#define ERR_PNS_IF_PLUG_SUBMOD_NO_MODULE_ATTACHED_TO_ADD_TO ((uint32_t)0xC0300134L)

//
// MessageId: ERR_PNS_IF_PLUG_SUBMOD_ALREADY_PLUGGED_THIS_SUBMOD
//
// MessageText:
//
// Submodule already plugged.
//
#define ERR_PNS_IF_PLUG_SUBMOD_ALREADY_PLUGGED_THIS_SUBMOD ((uint32_t)0xC0300135L)

//
// MessageId: ERR_PNS_IF_SETIOXS_INVALID_PROV_IMAGE
//
// MessageText:
//
// Invalid IOXS provider image.
//
#define ERR_PNS_IF_SETIOXS_INVALID_PROV_IMAGE ((uint32_t)0xC0300136L)

//
// MessageId: ERR_PNS_IF_SETIOXS_INVALID_CONS_IMAGE
//
// MessageText:
//
// Invalid IOXS consumer image.
//
#define ERR_PNS_IF_SETIOXS_INVALID_CONS_IMAGE ((uint32_t)0xC0300137L)

//
// MessageId: ERR_PNS_IF_INVALID_IOPS_MODE
//
// MessageText:
//
// Invalid IOPS mode.
//
#define ERR_PNS_IF_INVALID_IOPS_MODE     ((uint32_t)0xC0300138L)

//
// MessageId: ERR_PNS_IF_INVALID_IOCS_MODE
//
// MessageText:
//
// Invalid IOCS mode.
//
#define ERR_PNS_IF_INVALID_IOCS_MODE     ((uint32_t)0xC0300139L)

//
// MessageId: ERR_PNS_IF_INVALID_API
//
// MessageText:
//
// Invalid API.
//
#define ERR_PNS_IF_INVALID_API           ((uint32_t)0xC030013AL)

//
// MessageId: ERR_PNS_IF_INVALID_SLOT
//
// MessageText:
//
// Invalid slot.
//
#define ERR_PNS_IF_INVALID_SLOT          ((uint32_t)0xC030013BL)

//
// MessageId: ERR_PNS_IF_INVALID_SUBSLOT
//
// MessageText:
//
// Invalid subslot.
//
#define ERR_PNS_IF_INVALID_SUBSLOT       ((uint32_t)0xC030013CL)

//
// MessageId: ERR_PNS_IF_INVALID_CHANNEL_NUMBER
//
// MessageText:
//
// Invalid channel number.
//
#define ERR_PNS_IF_INVALID_CHANNEL_NUMBER ((uint32_t)0xC030013DL)

//
// MessageId: ERR_PNS_IF_INVALID_CHANNEL_PROPERTIES
//
// MessageText:
//
// Invalid channel properties.
//
#define ERR_PNS_IF_INVALID_CHANNEL_PROPERTIES ((uint32_t)0xC030013EL)

//
// MessageId: ERR_PNS_IF_CHANNEL_ERRORTYPE_NOT_ALLOWED
//
// MessageText:
//
// Invalid channel error type not allowed.
//
#define ERR_PNS_IF_CHANNEL_ERRORTYPE_NOT_ALLOWED ((uint32_t)0xC030013FL)

//
// MessageId: ERR_PNS_IF_EXT_CHANNEL_ERRORTYPE_NOT_ALLOWED
//
// MessageText:
//
// Invalid channel EXT error type not allowed.
//
#define ERR_PNS_IF_EXT_CHANNEL_ERRORTYPE_NOT_ALLOWED ((uint32_t)0xC0300140L)

//
// MessageId: ERR_PNS_IF_INVALID_USER_STRUCT_IDENTIFIER
//
// MessageText:
//
// Invalid user struct identifier.
//
#define ERR_PNS_IF_INVALID_USER_STRUCT_IDENTIFIER ((uint32_t)0xC0300141L)

//
// MessageId: ERR_PNS_IF_INVALID_SUBMODULE
//
// MessageText:
//
// Invalid submodule.
//
#define ERR_PNS_IF_INVALID_SUBMODULE     ((uint32_t)0xC0300142L)

//
// MessageId: ERR_PNS_IF_INVALID_IM_TYPE
//
// MessageText:
//
// Invalid IM type.
//
#define ERR_PNS_IF_INVALID_IM_TYPE       ((uint32_t)0xC0300143L)

//
// MessageId: ERR_PNS_IF_IDENTIFY_FODMI_QUEUE_FAILED
//
// MessageText:
//
// Failed to identify the FODMI Queue.
//
#define ERR_PNS_IF_IDENTIFY_FODMI_QUEUE_FAILED ((uint32_t)0xC0300144L)

//
// MessageId: ERR_PNS_IF_DPM_MAILBOX_OVERFLOW
//
// MessageText:
//
// The DPM Receive Mailbox Queue run out of space. Most likely the host did not fetch the packets.
//
#define ERR_PNS_IF_DPM_MAILBOX_OVERFLOW  ((uint32_t)0xC0300145L)

//
// MessageId: ERR_PNS_IF_APPL_IM_ACCESS_DENIED
//
// MessageText:
//
// The application denied read/write access to I&M record object.
//
#define ERR_PNS_IF_APPL_IM_ACCESS_DENIED ((uint32_t)0xC0300146L)

//
// MessageId: ERR_PNS_IF_APPL_IM_INVALID_INDEX
//
// MessageText:
//
// The application does not implement the requested I&M record object.
//
#define ERR_PNS_IF_APPL_IM_INVALID_INDEX ((uint32_t)0xC0300147L)

//
// MessageId: ERR_PNS_IF_TAGLIST_INVALID_SUBMODULE_NUMBER
//
// MessageText:
//
// Invalid number of max supported submodules.
//
#define ERR_PNS_IF_TAGLIST_INVALID_SUBMODULE_NUMBER ((uint32_t)0xC0300148L)

//
// MessageId: ERR_PNS_IF_TAGLIST_INVALID_ADDITIONAL_AR_NUMBER
//
// MessageText:
//
// Invalid number of max supported additional IO ARs.
//
#define ERR_PNS_IF_TAGLIST_INVALID_ADDITIONAL_AR_NUMBER ((uint32_t)0xC0300149L)

//
// MessageId: ERR_PNS_IF_TAGLIST_INVALID_IMPLICIT_AR_NUMBER
//
// MessageText:
//
// Invalid number of max supported implicit IO ARs.
//
#define ERR_PNS_IF_TAGLIST_INVALID_IMPLICIT_AR_NUMBER ((uint32_t)0xC030014AL)

//
// MessageId: ERR_PNS_IF_TAGLIST_INVALID_DAAR_NUMBER
//
// MessageText:
//
// Invalid number of max supported Device Access ARs.
//
#define ERR_PNS_IF_TAGLIST_INVALID_DAAR_NUMBER ((uint32_t)0xC030014BL)

//
// MessageId: ERR_PNS_IF_TAGLIST_INVALID_MIN_RPC_BUFFER_SIZE
//
// MessageText:
//
// Invalid RPC buffer size.
//
#define ERR_PNS_IF_TAGLIST_INVALID_MIN_RPC_BUFFER_SIZE ((uint32_t)0xC030014CL)

//
// MessageId: ERR_PNS_IF_TAGLIST_INVALID_DIAGNOSIS_ENTRIES_NUM
//
// MessageText:
//
// Invalid number of max supported diagnosis entries .
//
#define ERR_PNS_IF_TAGLIST_INVALID_DIAGNOSIS_ENTRIES_NUM ((uint32_t)0xC030014DL)

//
// MessageId: ERR_PNS_IF_TAGLIST_INVALID_ARSET_NUM
//
// MessageText:
//
// Invalid number of max supported AR sets.
//
#define ERR_PNS_IF_TAGLIST_INVALID_ARSET_NUM ((uint32_t)0xC030014EL)

//
// MessageId: ERR_PNS_IF_PE_ENTITY_EXISTS
//
// MessageText:
//
// A PE Entity was already added to this submodule.
//
#define ERR_PNS_IF_PE_ENTITY_EXISTS      ((uint32_t)0xC030014FL)

//
// MessageId: ERR_PNS_IF_NO_PE_ENTITY
//
// MessageText:
//
// The submodule does not have a PE Entity.
//
#define ERR_PNS_IF_NO_PE_ENTITY          ((uint32_t)0xC0300150L)

//
// MessageId: ERR_PNS_IF_INVALID_HW_REVISION
//
// MessageText:
//
// The hardware revision field was set to an invalid value.
//
#define ERR_PNS_IF_INVALID_HW_REVISION   ((uint32_t)0xC0300151L)

//
// MessageId: ERR_PNS_IF_INVALID_SERIAL_NUMBER
//
// MessageText:
//
// The serial number field was set to an invalid value.
//
#define ERR_PNS_IF_INVALID_SERIAL_NUMBER ((uint32_t)0xC0300152L)

//
// MessageId: ERR_PNS_IF_INCONSISTENT_DDP_CONTENT
//
// MessageText:
//
// The dataprovider OEM fields are incomplete regarding Profinet protocol.
//
#define ERR_PNS_IF_INCONSISTENT_DDP_CONTENT ((uint32_t)0xC0300153L)

// START: On 12.11.08 Konstantin added: Area for Abort- and Offline- Reason error codes to support Siemens Stack
// Message IDs 0x0A00 - 0x0AFF
//
// MessageId: ERR_PNS_IF_CM_AR_REASON_NONE
//
// MessageText:
//
// None. Unused.
//
#define ERR_PNS_IF_CM_AR_REASON_NONE     ((uint32_t)0xC0300A00L)

//
// MessageId: 0xC0300A01L (No symbolic name defined)
//
// MessageText:
//
// This error code is obsolete.
//


//
// MessageId: 0xC0300A02L (No symbolic name defined)
//
// MessageText:
//
// This error code is obsolete.
//


//
// MessageId: ERR_PNS_IF_CM_AR_REASON_MEM
//
// MessageText:
//
// AR Out of memory.
//
#define ERR_PNS_IF_CM_AR_REASON_MEM      ((uint32_t)0xC0300A03L)

//
// MessageId: ERR_PNS_IF_CM_AR_REASON_FRAME
//
// MessageText:
//
// AR add provider or consumer failed.
//
#define ERR_PNS_IF_CM_AR_REASON_FRAME    ((uint32_t)0xC0300A04L)

//
// MessageId: ERR_PNS_IF_CM_AR_REASON_MISS
//
// MessageText:
//
// AR consumer DHT/WDT expired.
//
#define ERR_PNS_IF_CM_AR_REASON_MISS     ((uint32_t)0xC0300A05L)

//
// MessageId: ERR_PNS_IF_CM_AR_REASON_TIMER
//
// MessageText:
//
// AR cmi timeout.
//
#define ERR_PNS_IF_CM_AR_REASON_TIMER    ((uint32_t)0xC0300A06L)

//
// MessageId: ERR_PNS_IF_CM_AR_REASON_ALARM
//
// MessageText:
//
// AR alarm-open failed.
//
#define ERR_PNS_IF_CM_AR_REASON_ALARM    ((uint32_t)0xC0300A07L)

//
// MessageId: ERR_PNS_IF_CM_AR_REASON_ALSND
//
// MessageText:
//
// AR alarm-send.cnf(-).
//
#define ERR_PNS_IF_CM_AR_REASON_ALSND    ((uint32_t)0xC0300A08L)

//
// MessageId: ERR_PNS_IF_CM_AR_REASON_ALACK
//
// MessageText:
//
// AR alarm-ack-send.cnf(-).
//
#define ERR_PNS_IF_CM_AR_REASON_ALACK    ((uint32_t)0xC0300A09L)

//
// MessageId: ERR_PNS_IF_CM_AR_REASON_ALLEN
//
// MessageText:
//
// AR alarm data too long.
//
#define ERR_PNS_IF_CM_AR_REASON_ALLEN    ((uint32_t)0xC0300A0AL)

//
// MessageId: ERR_PNS_IF_CM_AR_REASON_ASRT
//
// MessageText:
//
// AR alarm.ind(err).
//
#define ERR_PNS_IF_CM_AR_REASON_ASRT     ((uint32_t)0xC0300A0BL)

//
// MessageId: ERR_PNS_IF_CM_AR_REASON_RPC
//
// MessageText:
//
// AR rpc-client call.cnf(-).
//
#define ERR_PNS_IF_CM_AR_REASON_RPC      ((uint32_t)0xC0300A0CL)

//
// MessageId: ERR_PNS_IF_CM_AR_REASON_ABORT
//
// MessageText:
//
// AR abort.req.
//
#define ERR_PNS_IF_CM_AR_REASON_ABORT    ((uint32_t)0xC0300A0DL)

//
// MessageId: ERR_PNS_IF_CM_AR_REASON_RERUN
//
// MessageText:
//
// AR re-run aborts existing AR.
//
#define ERR_PNS_IF_CM_AR_REASON_RERUN    ((uint32_t)0xC0300A0EL)

//
// MessageId: ERR_PNS_IF_CM_AR_REASON_REL
//
// MessageText:
//
// AR release.ind received.
//
#define ERR_PNS_IF_CM_AR_REASON_REL      ((uint32_t)0xC0300A0FL)

//
// MessageId: ERR_PNS_IF_CM_AR_REASON_PAS
//
// MessageText:
//
// AR device deactivated.
//
#define ERR_PNS_IF_CM_AR_REASON_PAS      ((uint32_t)0xC0300A10L)

//
// MessageId: ERR_PNS_IF_CM_AR_REASON_RMV
//
// MessageText:
//
// AR removed.
//
#define ERR_PNS_IF_CM_AR_REASON_RMV      ((uint32_t)0xC0300A11L)

//
// MessageId: ERR_PNS_IF_CM_AR_REASON_PROT
//
// MessageText:
//
// AR protocol violation.
//
#define ERR_PNS_IF_CM_AR_REASON_PROT     ((uint32_t)0xC0300A12L)

//
// MessageId: ERR_PNS_IF_CM_AR_REASON_NARE
//
// MessageText:
//
// AR name resolution error.
//
#define ERR_PNS_IF_CM_AR_REASON_NARE     ((uint32_t)0xC0300A13L)

//
// MessageId: ERR_PNS_IF_CM_AR_REASON_BIND
//
// MessageText:
//
// AR RPC-Bind error.
//
#define ERR_PNS_IF_CM_AR_REASON_BIND     ((uint32_t)0xC0300A14L)

//
// MessageId: ERR_PNS_IF_CM_AR_REASON_CONNECT
//
// MessageText:
//
// AR RPC-Connect error.
//
#define ERR_PNS_IF_CM_AR_REASON_CONNECT  ((uint32_t)0xC0300A15L)

//
// MessageId: ERR_PNS_IF_CM_AR_REASON_READ
//
// MessageText:
//
// AR RPC-Read error.
//
#define ERR_PNS_IF_CM_AR_REASON_READ     ((uint32_t)0xC0300A16L)

//
// MessageId: ERR_PNS_IF_CM_AR_REASON_WRITE
//
// MessageText:
//
// AR RPC-Write error.
//
#define ERR_PNS_IF_CM_AR_REASON_WRITE    ((uint32_t)0xC0300A17L)

//
// MessageId: ERR_PNS_IF_CM_AR_REASON_CONTROL
//
// MessageText:
//
// AR RPC-Control error.
//
#define ERR_PNS_IF_CM_AR_REASON_CONTROL  ((uint32_t)0xC0300A18L)

//
// MessageId: ERR_PNS_IF_CM_AR_REASON_PULLPLUG
//
// MessageText:
//
// AR forbidden pull or plug after check.rsp and before in-data.ind.
//
#define ERR_PNS_IF_CM_AR_REASON_PULLPLUG ((uint32_t)0xC0300A19L)

//
// MessageId: ERR_PNS_IF_CM_AR_REASON_AP_RMV
//
// MessageText:
//
// AR AP removed.
//
#define ERR_PNS_IF_CM_AR_REASON_AP_RMV   ((uint32_t)0xC0300A1AL)

//
// MessageId: ERR_PNS_IF_CM_AR_REASON_LNK_DWN
//
// MessageText:
//
// AR link down.
//
#define ERR_PNS_IF_CM_AR_REASON_LNK_DWN  ((uint32_t)0xC0300A1BL)

//
// MessageId: ERR_PNS_IF_CM_AR_REASON_MMAC
//
// MessageText:
//
// AR could not register multicast-mac address.
//
#define ERR_PNS_IF_CM_AR_REASON_MMAC     ((uint32_t)0xC0300A1CL)

//
// MessageId: ERR_PNS_IF_CM_AR_REASON_SYNC
//
// MessageText:
//
// Not synchronized (cannot start companion-ar).
//
#define ERR_PNS_IF_CM_AR_REASON_SYNC     ((uint32_t)0xC0300A1DL)

//
// MessageId: ERR_PNS_IF_CM_AR_REASON_TOPO
//
// MessageText:
//
// Wrong topology (cannot start companion-ar).
//
#define ERR_PNS_IF_CM_AR_REASON_TOPO     ((uint32_t)0xC0300A1EL)

//
// MessageId: ERR_PNS_IF_CM_AR_REASON_DCP_NAME
//
// MessageText:
//
// DCP, station-name changed.
//
#define ERR_PNS_IF_CM_AR_REASON_DCP_NAME ((uint32_t)0xC0300A1FL)

//
// MessageId: ERR_PNS_IF_CM_AR_REASON_DCP_RESET
//
// MessageText:
//
// DCP, reset to factory-settings.
//
#define ERR_PNS_IF_CM_AR_REASON_DCP_RESET ((uint32_t)0xC0300A20L)

//
// MessageId: ERR_PNS_IF_CM_AR_REASON_PRM
//
// MessageText:
//
// 0x8ipp submodule in the first AR has either an appl-ready-pending (erroneous parameterization) or is locked (no parameterization) or is wrong or pulled (no parameterization).
//
#define ERR_PNS_IF_CM_AR_REASON_PRM      ((uint32_t)0xC0300A21L)

//
// MessageId: ERR_PNS_IF_CM_AR_REASON_IRDATA
//
// MessageText:
//
// No irdata record yet.
//
#define ERR_PNS_IF_CM_AR_REASON_IRDATA   ((uint32_t)0xC0300A22L)

//
// MessageId: ERR_PNS_IF_CM_AR_REASON_PDEV
//
// MessageText:
//
// Ownership of PDEV.
//
#define ERR_PNS_IF_CM_AR_REASON_PDEV     ((uint32_t)0xC0300A23L)

//
// MessageId: ERR_PNS_IF_CM_AR_REASON_MAX
//
// MessageText:
//
// Max. Unused.
//
#define ERR_PNS_IF_CM_AR_REASON_MAX      ((uint32_t)0xC0300AFFL)

// END: On 12.11.08 Konstantin added: Area for Abort- and Offline- Reason error codes to support Siemens Stack
// Message IDs 0x0A00 - 0x0AFF
#endif  /* PNS_RESULTS_H_ */
