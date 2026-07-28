/**************************************************************************************
Copyright (c) Hilscher Gesellschaft fuer Systemautomation mbH. All Rights Reserved.
***************************************************************************************
$Id: PBAPS_Results.h 86804 2019-10-28 10:45:22Z TGrigorov $:

Description:
  ProfibusAps_Results.h
**************************************************************************************/
/*!\file PBAPS_Results.h */
#ifndef __PROFIBUS_APS_RESULTS_H
#define __PROFIBUS_APS_RESULTS_H

#include<stdint.h>

/////////////////////////////////////////////////////////////////////////////////////
// PROFIBUS APS Packet Status codes
/////////////////////////////////////////////////////////////////////////////////////
//
// MessageId: ERR_PROFIBUS_APS_COMMAND_INVALID
//
// MessageText:
//
// Invalid command received.
//
#define ERR_PROFIBUS_APS_COMMAND_INVALID ((uint32_t)0xC01D0001L)

//
// MessageId: ERR_PROFIBUS_APS_ALREADY_CONFIGURED
//
// MessageText:
//
// Device is already configured. The new configuration is discarded.
//
#define ERR_PROFIBUS_APS_ALREADY_CONFIGURED ((uint32_t)0xC01D0002L)

//
// MessageId: ERR_PROFIBUS_APS_NO_CONFIG_DBM
//
// MessageText:
//
// No database available.
//
#define ERR_PROFIBUS_APS_NO_CONFIG_DBM   ((uint32_t)0xC01D0003L)

//
// MessageId: ERR_PROFIBUS_APS_CONFIG_DBM_INVALID
//
// MessageText:
//
// Database is invalid.
//
#define ERR_PROFIBUS_APS_CONFIG_DBM_INVALID ((uint32_t)0xC01D0004L)

//
// MessageId: ERR_PROFIBUS_APS_CONFIG_MODULE_LENGTH
//
// MessageText:
//
// Module configuration consists invalid length.
//
#define ERR_PROFIBUS_APS_CONFIG_MODULE_LENGTH ((uint32_t)0xC01D0005L)

//
// MessageId: ERR_PROFIBUS_APS_CFG_DATA_INVALID_LENGTH
//
// MessageText:
//
// Configuration data invalid length.
//
#define ERR_PROFIBUS_APS_CFG_DATA_INVALID_LENGTH ((uint32_t)0xC01D0006L)

//
// MessageId: ERR_PROFIBUS_APS_CFG_DATA_INCONSISTENT
//
// MessageText:
//
// Configuration data inconsistent.
//
#define ERR_PROFIBUS_APS_CFG_DATA_INCONSISTENT ((uint32_t)0xC01D0007L)

//
// MessageId: ERR_PROFIBUS_APS_CFG_DATA_MAX_IO_LEN_EXCEEDED
//
// MessageText:
//
// Configuration data maximum I/O length exceeded.
//
#define ERR_PROFIBUS_APS_CFG_DATA_MAX_IO_LEN_EXCEEDED ((uint32_t)0xC01D0008L)

//
// MessageId: ERR_PROFIBUS_APS_RESET_PENDING
//
// MessageText:
//
// Reset of APS task is pending.
//
#define ERR_PROFIBUS_APS_RESET_PENDING   ((uint32_t)0xC01D0009L)

//
// MessageId: ERR_PROFIBUS_APS_BAUDRATE_INVALID
//
// MessageText:
//
// Configuration data consists an invalid baudrate parameter.
//
#define ERR_PROFIBUS_APS_BAUDRATE_INVALID ((uint32_t)0xC01D000AL)

//
// MessageId: ERR_PROFIBUS_APS_ADDR_INVALID
//
// MessageText:
//
// Configuration data consists an invalid address parameter.
//
#define ERR_PROFIBUS_APS_ADDR_INVALID    ((uint32_t)0xC01D000BL)

//
// MessageId: ERR_PROFIBUS_APS_WDG_INVALID
//
// MessageText:
//
// Configuration data consists an invalid watchdog parameter.
//
#define ERR_PROFIBUS_APS_WDG_INVALID     ((uint32_t)0xC01D000CL)

#endif  /* __PROFIBUS_APS_RESULTS_H */
