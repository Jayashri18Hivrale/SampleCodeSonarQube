/**************************************************************************************
Copyright (c) Hilscher Gesellschaft fuer Systemautomation mbH. All Rights Reserved.
***************************************************************************************
$Id: PBDPS_Results.h 86777 2019-10-25 13:53:04Z TGrigorov $:

Description:
  This file contains definitions of result codes of Profibus DP Slave FSPMS task.

Changes:
 Date          Description
-----------------------------------------------------------------------------------
 2019-09-15    Created
**************************************************************************************/
/*!\file PBDPS_Results.h */
#ifndef __PROFIBUS_FSPMS_RESULTS_H
#define __PROFIBUS_FSPMS_RESULTS_H

#include<stdint.h>

/////////////////////////////////////////////////////////////////////////////////////
// PROFIBUS FSPMS Result codes (0001 .. 00FF)
/////////////////////////////////////////////////////////////////////////////////////
//
// MessageId: ERR_PROFIBUS_FSPMS_COMMAND_INVALID
//
// MessageText:
//
// Invalid command received.
//
#define ERR_PROFIBUS_FSPMS_COMMAND_INVALID ((uint32_t)0xC0090001L)

//
// MessageId: ERR_PROFIBUS_FSPMS_MAX_EXT_DIAG_SIZE_EXCEEDED
//
// MessageText:
//
// Setting the slave diagnostic failed, because the limit of the maximum number of 238 extended diagnostic bytes is exceeded.
//
#define ERR_PROFIBUS_FSPMS_MAX_EXT_DIAG_SIZE_EXCEEDED ((uint32_t)0xC0090002L)

//
// MessageId: ERR_PROFIBUS_FSPMS_MAX_CFG_DATA_SIZE_EXCEEDED
//
// MessageText:
//
// Setting the slave's configuration data failed, because the limit of the maximum number of 244 configuration bytes is exceeded.
//
#define ERR_PROFIBUS_FSPMS_MAX_CFG_DATA_SIZE_EXCEEDED ((uint32_t)0xC0090003L)

//
// MessageId: ERR_PROFIBUS_FSPMS_MS0_INIT_WRONG_STATE
//
// MessageText:
//
// The cyclic slave state machine cannot be initialized, state machine is not in "POWER-ON" state.
//
#define ERR_PROFIBUS_FSPMS_MS0_INIT_WRONG_STATE ((uint32_t)0xC0090004L)

//
// MessageId: ERR_PROFIBUS_FSPMS_SLAVE_DIAG_POWER_ON
//
// MessageText:
//
// Setting the Slave Diagnostic Data cannot be performed, because slave state machine isn't initialized yet.
//
#define ERR_PROFIBUS_FSPMS_SLAVE_DIAG_POWER_ON ((uint32_t)0xC0090005L)

//
// MessageId: ERR_PROFIBUS_FSPMS_SET_CFG_POWER_ON
//
// MessageText:
//
// Setting the Slave Configuration Data cannot be performed, because slave state machine isn't initialized yet.
//
#define ERR_PROFIBUS_FSPMS_SET_CFG_POWER_ON ((uint32_t)0xC0090006L)

//
// MessageId: ERR_PROFIBUS_FSPMS_GET_OUTPUT_POWER_ON
//
// MessageText:
//
// Getting the Slave Output Data cannot be performed, because slave state machine isn't initialized yet.
//
#define ERR_PROFIBUS_FSPMS_GET_OUTPUT_POWER_ON ((uint32_t)0xC0090007L)

//
// MessageId: ERR_PROFIBUS_FSPMS_GET_OUTPUT_WAIT_PRM
//
// MessageText:
//
// Getting the Slave Output Data cannot be performed, because slave state is currently in state "WAIT-PRM".
//
#define ERR_PROFIBUS_FSPMS_GET_OUTPUT_WAIT_PRM ((uint32_t)0xC0090008L)

//
// MessageId: ERR_PROFIBUS_FSPMS_SET_INPUT_POWER_ON
//
// MessageText:
//
// Setting the Slave Input Data cannot be performed, because slave state machine isn't initialized yet.
//
#define ERR_PROFIBUS_FSPMS_SET_INPUT_POWER_ON ((uint32_t)0xC0090009L)

//
// MessageId: ERR_PROFIBUS_FSPMS_SET_INPUT_WAIT_PRM
//
// MessageText:
//
// Setting the Slave Input Data cannot be performed, because slave state is currently in state "WAIT-PRM".
//
#define ERR_PROFIBUS_FSPMS_SET_INPUT_WAIT_PRM ((uint32_t)0xC009000AL)

//
// MessageId: ERR_PROFIBUS_FSPMS_CHECK_USER_PRM_POWER_ON
//
// MessageText:
//
// Confirming the Slave Parameter Data cannot be performed, because slave state machine isn't initialized yet.
//
#define ERR_PROFIBUS_FSPMS_CHECK_USER_PRM_POWER_ON ((uint32_t)0xC009000BL)

//
// MessageId: ERR_PROFIBUS_FSPMS_CHECK_USER_PRM_NOT_PENDING
//
// MessageText:
//
// There is no Parameter Data checking command pending, command ignored.
//
#define ERR_PROFIBUS_FSPMS_CHECK_USER_PRM_NOT_PENDING ((uint32_t)0xC009000CL)

//
// MessageId: ERR_PROFIBUS_FSPMS_CHECK_USER_PRM_NEW_PARAMETER
//
// MessageText:
//
// The confirmation of the Slave Parameter Data is obsolete, there is new Slave Parameter Data available.
//
#define ERR_PROFIBUS_FSPMS_CHECK_USER_PRM_NEW_PARAMETER ((uint32_t)0xC009000DL)

//
// MessageId: ERR_PROFIBUS_FSPMS_CHECK_CFG_POWER_ON
//
// MessageText:
//
// Confirming the Slave Configuration Data cannot be performed, because slave state machine isn't initialized yet.
//
#define ERR_PROFIBUS_FSPMS_CHECK_CFG_POWER_ON ((uint32_t)0xC009000EL)

//
// MessageId: ERR_PROFIBUS_FSPMS_CHECK_CFG_NOT_PENDING
//
// MessageText:
//
// There is no Configuration Data checking command pending, command ignored.
//
#define ERR_PROFIBUS_FSPMS_CHECK_CFG_NOT_PENDING ((uint32_t)0xC009000FL)

//
// MessageId: ERR_PROFIBUS_FSPMS_CHECK_CFG_NEW_CONFIGURATION
//
// MessageText:
//
// The confirmation of the Slave Configruation Data is obsolete, there is new Slave Configuration Data available.
//
#define ERR_PROFIBUS_FSPMS_CHECK_CFG_NEW_CONFIGURATION ((uint32_t)0xC0090010L)

//
// MessageId: ERR_PROFIBUS_FSPMS_CHECK_EXT_USER_PRM_POWER_ON
//
// MessageText:
//
// Confirming the extended Slave Parameter Data cannot be performed, because slave state machine isn't initialized yet.
//
#define ERR_PROFIBUS_FSPMS_CHECK_EXT_USER_PRM_POWER_ON ((uint32_t)0xC0090011L)

//
// MessageId: ERR_PROFIBUS_FSPMS_CHECK_EXT_USER_PRM_NOT_PENDING
//
// MessageText:
//
// There is no extended Parameter Data checking command pending, command ignored.
//
#define ERR_PROFIBUS_FSPMS_CHECK_EXT_USER_PRM_NOT_PENDING ((uint32_t)0xC0090012L)

//
// MessageId: ERR_PROFIBUS_FSPMS_CHECK_EXT_USER_PRM_NEW_PARAMETER
//
// MessageText:
//
// The confirmation of the extended Slave Parameter Data is obsolete, there is new extended Slave Parameter Data available.
//
#define ERR_PROFIBUS_FSPMS_CHECK_EXT_USER_PRM_NEW_PARAMETER ((uint32_t)0xC0090013L)

//
// MessageId: ERR_PROFIBUS_FSPMS_ABORT_IGNORED
//
// MessageText:
//
// The abort command is ignored in the current state of the slave state machine.
//
#define ERR_PROFIBUS_FSPMS_ABORT_IGNORED ((uint32_t)0xC0090014L)

//
// MessageId: ERR_PROFIBUS_FSPMS_GET_OUTPUT_WAIT_CFG
//
// MessageText:
//
// Getting the Slave Output Data cannot be performed, because slave state is currently in state "WAIT-CFG".
//
#define ERR_PROFIBUS_FSPMS_GET_OUTPUT_WAIT_CFG ((uint32_t)0xC0090015L)

//
// MessageId: ERR_PROFIBUS_FSPMS_SET_INPUT_NOT_PENDING
//
// MessageText:
//
// Setting the Slave Input Data cannot be performed, because input update is not pending.
//
#define ERR_PROFIBUS_FSPMS_SET_INPUT_NOT_PENDING ((uint32_t)0xC0090016L)

//
// MessageId: ERR_PROFIBUS_FSPMS_CHECK_USER_PRM_INVALID_MASTER_ADDRESS
//
// MessageText:
//
// The confirmation of the Slave Parameter Data is obsolete, because meanwhile an other master has parameterized the slave.
//
#define ERR_PROFIBUS_FSPMS_CHECK_USER_PRM_INVALID_MASTER_ADDRESS ((uint32_t)0xC0090017L)

//
// MessageId: ERR_PROFIBUS_FSPMS_CHECK_CFG_INVALID_MASTER_ADDRESS
//
// MessageText:
//
// The confirmation of the Slave Configruation Data is obsolete, because meanwhile an other master has configured the slave.
//
#define ERR_PROFIBUS_FSPMS_CHECK_CFG_INVALID_MASTER_ADDRESS ((uint32_t)0xC0090018L)

//
// MessageId: ERR_PROFIBUS_FSPMS_APPLICATION_READY_IGNORED
//
// MessageText:
//
// The Application ready command is ignored in the current state of the slave state machine.
//
#define ERR_PROFIBUS_FSPMS_APPLICATION_READY_IGNORED ((uint32_t)0xC0090019L)

//
// MessageId: ERR_PROFIBUS_FSPMS_CHECK_EXT_USER_PRM_INVALID_MASTER_ADDRESS
//
// MessageText:
//
// The confirmation of the extended Slave Parameter Data is obsolete, because meanwhile an other master has parameterized the slave.
//
#define ERR_PROFIBUS_FSPMS_CHECK_EXT_USER_PRM_INVALID_MASTER_ADDRESS ((uint32_t)0xC009001AL)

//
// MessageId: ERR_PROFIBUS_FSPMS_GET_OUTPUT_DATA_EXCHANGE_NO_CYCLE
//
// MessageText:
//
// Getting the Slave Output Data cannot be performed, because slave state machine is in state "DATA-EXCH" but no output cycle has been driven yet.
//
#define ERR_PROFIBUS_FSPMS_GET_OUTPUT_DATA_EXCHANGE_NO_CYCLE ((uint32_t)0xC009001BL)

//
// MessageId: ERR_PROFIBUS_FSPMS_APPLICATION_ALREADY_READY
//
// MessageText:
//
// The Application ready command is ignored, because the application has already signaled its readyness.
//
#define ERR_PROFIBUS_FSPMS_APPLICATION_ALREADY_READY ((uint32_t)0xC009001CL)

//
// MessageId: ERR_PROFIBUS_FSPMS_SLAVE_DIAG_PENDING
//
// MessageText:
//
// A new Slave Diagnostic command can not be accepted, while a previous one is pending.
//
#define ERR_PROFIBUS_FSPMS_SLAVE_DIAG_PENDING ((uint32_t)0xC009001DL)

//
// MessageId: ERR_PROFIBUS_FSPMS_READ_RESPONSE_NEG
//
// MessageText:
//
// The read command can not be A new Slave Diagnostic command can not be accepted, while a previous one is pending.
//
#define ERR_PROFIBUS_FSPMS_READ_RESPONSE_NEG ((uint32_t)0xC009001EL)

//
// MessageId: ERR_PROFIBUS_FSPMS_MS1_INIT_WRONG_STATE
//
// MessageText:
//
// The acyclic slave state machine cannot be initialized, state machine is not in "POWER-ON" state.
//
#define ERR_PROFIBUS_FSPMS_MS1_INIT_WRONG_STATE ((uint32_t)0xC009001FL)

//
// MessageId: ERR_PROFIBUS_FSPMS_ALARM_HANDLER_NOT_STARTED
//
// MessageText:
//
// The Alarm Handler state machine isn't started yet, an Alarm cannot be notified.
//
#define ERR_PROFIBUS_FSPMS_ALARM_HANDLER_NOT_STARTED ((uint32_t)0xC0090020L)

//
// MessageId: ERR_PROFIBUS_FSPMS_ALARM_HANDLER_NOT_ENABLED
//
// MessageText:
//
// The requested Type of Alarm is not enabled, this Alarm cannot be notified.
//
#define ERR_PROFIBUS_FSPMS_ALARM_HANDLER_NOT_ENABLED ((uint32_t)0xC0090022L)

//
// MessageId: ERR_PROFIBUS_FSPMS_ALARM_HANDLER_LIMIT_EXPIRED
//
// MessageText:
//
// The limit of parallel running alarms is expired, this Alarm cannot be notified.
//
#define ERR_PROFIBUS_FSPMS_ALARM_HANDLER_LIMIT_EXPIRED ((uint32_t)0xC0090023L)

//
// MessageId: ERR_PROFIBUS_FSPMS_ALARM_HANDLER_PENDING
//
// MessageText:
//
// This requested Type of Alarm is still pending and in operation, this is why the Alarm cannot be notified.
//
#define ERR_PROFIBUS_FSPMS_ALARM_HANDLER_PENDING ((uint32_t)0xC0090024L)

//
// MessageId: ERR_PROFIBUS_FSPMS_NOTREADY_EXPIRED
//
// MessageText:
//
// Application is at not ready state.
//
#define ERR_PROFIBUS_FSPMS_NOTREADY_EXPIRED ((uint32_t)0x80090025L)

//
// MessageId: ERR_PROFIBUS_FSPMS_WATCHDOG_EXPIRED
//
// MessageText:
//
// Watchdog error expired.
//
#define ERR_PROFIBUS_FSPMS_WATCHDOG_EXPIRED ((uint32_t)0xC0090026L)

//
// MessageId: ERR_PROFIBUS_FSPMS_SUBSCRIBER_NOT_CONFIGURED
//
// MessageText:
//
// Subscriber with given address not configured.
//
#define ERR_PROFIBUS_FSPMS_SUBSCRIBER_NOT_CONFIGURED ((uint32_t)0xC0090027L)

//
// MessageId: ERR_PROFIBUS_FSPMS_SUBSCRIBER_NOT_IN_WSTART_STATE
//
// MessageText:
//
// Subscriber in wrong state, state w_start expected.
//
#define ERR_PROFIBUS_FSPMS_SUBSCRIBER_NOT_IN_WSTART_STATE ((uint32_t)0xC0090028L)

//
// MessageId: ERR_PROFIBUS_FSPMS_SUBSCRIBER_NOT_IN_RUN_STATE
//
// MessageText:
//
// Subscriber in wrong state, state run expected.
//
#define ERR_PROFIBUS_FSPMS_SUBSCRIBER_NOT_IN_RUN_STATE ((uint32_t)0xC0090029L)

//
// MessageId: ERR_PROFIBUS_FSPMS_CLOCKSYNC_ALREADY_RUNNING
//
// MessageText:
//
// ClockSync in wrong state, already running.
//
#define ERR_PROFIBUS_FSPMS_CLOCKSYNC_ALREADY_RUNNING ((uint32_t)0xC009002AL)

//
// MessageId: ERR_PROFIBUS_FSPMS_CLOCKSYNC_NOT_RUNNING
//
// MessageText:
//
// ClockSync in wrong state, not running.
//
#define ERR_PROFIBUS_FSPMS_CLOCKSYNC_NOT_RUNNING ((uint32_t)0xC009002BL)

//
// MessageId: ERR_PROFIBUS_FSPMS_RESET_PENDING
//
// MessageText:
//
// Reset of FSPMS task is pending.
//
#define ERR_PROFIBUS_FSPMS_RESET_PENDING ((uint32_t)0xC009002CL)

//
// MessageId: ERR_PROFIBUS_FSPMS_INPUT_UPDATE_REQUIRED
//
// MessageText:
//
// Input update required.
//
#define ERR_PROFIBUS_FSPMS_INPUT_UPDATE_REQUIRED ((uint32_t)0xC009002DL)

//
// MessageId: ERR_PROFIBUS_FSPMS_DIAG_TYPE_EXISTING
//
// MessageText:
//
// A diagnosis from the same type already exists.
//
#define ERR_PROFIBUS_FSPMS_DIAG_TYPE_EXISTING ((uint32_t)0xC009002EL)

//
// MessageId: ERR_PROFIBUS_FSPMS_DIAG_ALARM_NOT_ALLOWED
//
// MessageText:
//
// It is not allowed to send alarms within diagnosis (the alarm send service shall be used instead).
//
#define ERR_PROFIBUS_FSPMS_DIAG_ALARM_NOT_ALLOWED ((uint32_t)0xC009002FL)

/////////////////////////////////////////////////////////////////////////////////////
// PROFIBUS FSPMS-MSCY1S Result codes (0100 .. 01FF)
/////////////////////////////////////////////////////////////////////////////////////
//
// MessageId: ERR_PROFIBUS_FSPMS_MSCY1S_NO_SLAVE_ADD_CHG_IND_PACKET
//
// MessageText:
//
// Getting a packet from the pool to send the received Slave Set Slave Address command to the application failed, service cannot be performed.
//
#define ERR_PROFIBUS_FSPMS_MSCY1S_NO_SLAVE_ADD_CHG_IND_PACKET ((uint32_t)0xC0090100L)

//
// MessageId: ERR_PROFIBUS_FSPMS_MSCY1S_SEND_PACKET_FAILED
//
// MessageText:
//
// Sending a packet failed, service cannot be performed.
//
#define ERR_PROFIBUS_FSPMS_MSCY1S_SEND_PACKET_FAILED ((uint32_t)0xC0090101L)

//
// MessageId: ERR_PROFIBUS_FSPMS_MSCY1S_NO_CFG_IND_PACKET
//
// MessageText:
//
// Getting a packet from the pool to send the received Slave Configuration Data to the application failed, service cannot be performed.
//
#define ERR_PROFIBUS_FSPMS_MSCY1S_NO_CFG_IND_PACKET ((uint32_t)0xC0090102L)

//
// MessageId: ERR_PROFIBUS_FSPMS_MSCY1S_NO_PRM_IND_PACKET
//
// MessageText:
//
// Getting a packet from the pool to send the received Slave Parameter Data to the application failed, service cannot be performed.
//
#define ERR_PROFIBUS_FSPMS_MSCY1S_NO_PRM_IND_PACKET ((uint32_t)0xC0090103L)

/////////////////////////////////////////////////////////////////////////////////////
// PROFIBUS FSPMS-MSAC1S Result codes (0200 .. 02FF)
/////////////////////////////////////////////////////////////////////////////////////
//
// MessageId: ERR_PROFIBUS_FSPMS_MSAC1S_ALARM_RES_UNEXPECTED
//
// MessageText:
//
// An Alarm response has been received while the ayclic State Machine is in "POWER-ON" state and has not been initialized yet.
//
#define ERR_PROFIBUS_FSPMS_MSAC1S_ALARM_RES_UNEXPECTED ((uint32_t)0xC0090200L)

//
// MessageId: ERR_PROFIBUS_FSPMS_MSAC1S_ALREADY_STARTED
//
// MessageText:
//
// A conflict has been detected within the acyclic MSAC1S state machine. In state "OPEN" a new request to start has been received.
//
#define ERR_PROFIBUS_FSPMS_MSAC1S_ALREADY_STARTED ((uint32_t)0xC0090201L)

//
// MessageId: ERR_PROFIBUS_FSPMS_MSAC1S_RSAP_ACTIVATION_DENIED
//
// MessageText:
//
// The activation of a RSAP in the MSAC1S state machine denied by the Data Link Layer.
//
#define ERR_PROFIBUS_FSPMS_MSAC1S_RSAP_ACTIVATION_DENIED ((uint32_t)0xC0090202L)

//
// MessageId: ERR_PROFIBUS_FSPMS_MSAC1S_STATE_CONFLICT_VS_SRES_IND
//
// MessageText:
//
// A conflict has been detected within the acyclic MSAC1S state machine. In state "VS-SRES" an unexpected reply indication has been received.
//
#define ERR_PROFIBUS_FSPMS_MSAC1S_STATE_CONFLICT_VS_SRES_IND ((uint32_t)0xC0090203L)

//
// MessageId: ERR_PROFIBUS_FSPMS_MSAC1S_STATE_CONFLICT_AA_SRES
//
// MessageText:
//
// A conflict has been detected within the acyclic MSAC1S state machine. In state "AA-SRES" an erroneous reply update confirmation has been received.
//
#define ERR_PROFIBUS_FSPMS_MSAC1S_STATE_CONFLICT_AA_SRES ((uint32_t)0xC0090204L)

//
// MessageId: ERR_PROFIBUS_FSPMS_MSAC1S_REPLY_UPDATE_ERROR
//
// MessageText:
//
// Updating a response buffer within the Data Link Layer entity failed. Servce has been rejected.
//
#define ERR_PROFIBUS_FSPMS_MSAC1S_REPLY_UPDATE_ERROR ((uint32_t)0xC0090205L)

//
// MessageId: ERR_PROFIBUS_FSPMS_MSAC1S_REPLY_UPDATE_CONFIRMATION
//
// MessageText:
//
// A conflict has been detected within the acyclic MSAC1S state machine. In state "OPEN" a reply update confirmation has been received.
//
#define ERR_PROFIBUS_FSPMS_MSAC1S_REPLY_UPDATE_CONFIRMATION ((uint32_t)0xC0090206L)

//
// MessageId: ERR_PROFIBUS_FSPMS_MSAC1S_STATE_CONFLICT_VS_WRES
//
// MessageText:
//
// A conflict has been detected within the acyclic MSAC1S state machine. In state "VS-WRES" an unexpected reply update confirmation has been received.
//
#define ERR_PROFIBUS_FSPMS_MSAC1S_STATE_CONFLICT_VS_WRES ((uint32_t)0xC0090207L)

//
// MessageId: ERR_PROFIBUS_FSPMS_MSAC1S_STATE_CONFLICT_VS_SRES
//
// MessageText:
//
// A conflict has been detected within the acyclic MSAC1S state machine. In state "VS-SRES" an unexpected reply update confirmation has been received.
//
#define ERR_PROFIBUS_FSPMS_MSAC1S_STATE_CONFLICT_VS_SRES ((uint32_t)0xC0090208L)

//
// MessageId: ERR_PROFIBUS_FSPMS_MSAC1S_SAP_DEACTIVATION_FAILED
//
// MessageText:
//
// A conflict has been detected within the acyclic MSAC1S state machine. A deactivation of an Service Access Point failed.
//
#define ERR_PROFIBUS_FSPMS_MSAC1S_SAP_DEACTIVATION_FAILED ((uint32_t)0xC0090209L)

//
// MessageId: ERR_PROFIBUS_FSPMS_MSAC1S_SET_SLAVE_DIAG_FAILED
//
// MessageText:
//
// A conflict has been detected within the acyclic MSAC1S state machine. Requesting a Slave diagnostic failed.
//
#define ERR_PROFIBUS_FSPMS_MSAC1S_SET_SLAVE_DIAG_FAILED ((uint32_t)0xC009020AL)

//
// MessageId: ERR_PROFIBUS_FSPMS_MSAC1S_ALREADY_STOPPED
//
// MessageText:
//
// A conflict has been detected within the acyclic MSAC1S state machine. In state "STOP" a new request to stop has been received.
//
#define ERR_PROFIBUS_FSPMS_MSAC1S_ALREADY_STOPPED ((uint32_t)0xC009020BL)

/////////////////////////////////////////////////////////////////////////////////////
// PROFIBUS FSPMS-MSAC2S Result codes (0300 .. 03FF)
/////////////////////////////////////////////////////////////////////////////////////
//
// MessageId: ERR_PROFIBUS_FSPMS_MSAC2S_NO_RSAP_PACKET
//
// MessageText:
//
// Getting a packet from the pool failed, service cannot be performed.
//
#define ERR_PROFIBUS_FSPMS_MSAC2S_NO_RSAP_PACKET ((uint32_t)0xC0090300L)

//
// MessageId: ERR_PROFIBUS_FSPMS_MSAC2S_RSAP_ACTIVATION_DENIED
//
// MessageText:
//
// Activation of RSAP is denied.
//
#define ERR_PROFIBUS_FSPMS_MSAC2S_RSAP_ACTIVATION_DENIED ((uint32_t)0xC0090301L)

//
// MessageId: ERR_PROFIBUS_FSPMS_MSAC2S_REPLY_UPDATE_FAILED_PACKET
//
// MessageText:
//
// Data of the SAP could not be update.
//
#define ERR_PROFIBUS_FSPMS_MSAC2S_REPLY_UPDATE_FAILED_PACKET ((uint32_t)0xC0090302L)

//
// MessageId: ERR_PROFIBUS_FSPMS_MSAC2S_SAP_DEACTIVATION_FAILED
//
// MessageText:
//
// Closing the SAP failed.
//
#define ERR_PROFIBUS_FSPMS_MSAC2S_SAP_DEACTIVATION_FAILED ((uint32_t)0xC0090304L)

/////////////////////////////////////////////////////////////////////////////////////
// PROFIBUS FSPMS-MSRM2S Result codes (0400 .. 04FF)
/////////////////////////////////////////////////////////////////////////////////////
//
// MessageId: ERR_PROFIBUS_FSPMS_MSRM2S_RSAP_ACTIVATION_DENIED
//
// MessageText:
//
// Activation of RSAP failed.
//
#define ERR_PROFIBUS_FSPMS_MSRM2S_RSAP_ACTIVATION_DENIED ((uint32_t)0xC0090400L)

//
// MessageId: ERR_PROFIBUS_FSPMS_MSRM2S_REPLY_UPDATE_FAILED_PACKET
//
// MessageText:
//
// Update data of resource manager failed.
//
#define ERR_PROFIBUS_FSPMS_MSRM2S_REPLY_UPDATE_FAILED_PACKET ((uint32_t)0xC0090401L)

//
// MessageId: ERR_PROFIBUS_FSPMS_MSRM2S_FAIL_SAP_CLOSED
//
// MessageText:
//
// Class 2 connection could not be closed.
//
#define ERR_PROFIBUS_FSPMS_MSRM2S_FAIL_SAP_CLOSED ((uint32_t)0xC0090402L)

//
// MessageId: ERR_PROFIBUS_FSPMS_MSRM2S_SAP_DEACTIVATION_FAILED
//
// MessageText:
//
// SAP of the resource manager could not be closed.
//
#define ERR_PROFIBUS_FSPMS_MSRM2S_SAP_DEACTIVATION_FAILED ((uint32_t)0xC0090403L)

/////////////////////////////////////////////////////////////////////////////////////
// PROFIBUS FSPMS-DMPMS Result codes (0500 .. 05FF)
/////////////////////////////////////////////////////////////////////////////////////
//
// MessageId: ERR_PROFIBUS_FSPMS_DMPMS_SEND_PACKET_FAILED
//
// MessageText:
//
// Sending a packet failed, service cannot be performed.
//
#define ERR_PROFIBUS_FSPMS_DMPMS_SEND_PACKET_FAILED ((uint32_t)0xC0090500L)

//
// MessageId: ERR_PROFIBUS_FSPMS_DMPMS_RSAP_ACTIVATION_DENIED
//
// MessageText:
//
// The activation of a RSAP in the DMPMS state machine denied by the Data Link Layer.
//
#define ERR_PROFIBUS_FSPMS_DMPMS_RSAP_ACTIVATION_DENIED ((uint32_t)0xC0090501L)

//
// MessageId: ERR_PROFIBUS_FSPMS_DMPMS_SAP_ACTIVATION_DENIED
//
// MessageText:
//
// The activation of a SAP in the DMPMS state machine denied by the Data Link Layer.
//
#define ERR_PROFIBUS_FSPMS_DMPMS_SAP_ACTIVATION_DENIED ((uint32_t)0xC0090502L)

//
// MessageId: ERR_PROFIBUS_FSPMS_DMPMS_SAP_DEACTIVATION_DENIED
//
// MessageText:
//
// The deactivation of a SAP in the DMPMS state machine denied by the Data Link Layer.
//
#define ERR_PROFIBUS_FSPMS_DMPMS_SAP_DEACTIVATION_DENIED ((uint32_t)0xC0090503L)




#endif  //__PROFIBUS_FSPMS_RESULT_H

