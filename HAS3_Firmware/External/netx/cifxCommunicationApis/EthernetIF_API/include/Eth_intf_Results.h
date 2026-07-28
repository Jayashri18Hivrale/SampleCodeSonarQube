/**************************************************************************************
Copyright (c) Hilscher Gesellschaft fuer Systemautomation mbH. All Rights Reserved.
***************************************************************************************
$Id:  $:

Description:
  Eth_intf_Results.h
**************************************************************************************/

#ifndef __ETH_INTF_RESULTS_H
#define __ETH_INTF_RESULTS_H

#include<stdint.h>

/////////////////////////////////////////////////////////////////////////////////////
// Ethernet Interface Task
/////////////////////////////////////////////////////////////////////////////////////
//
// MessageId: ERR_ETH_INTF_COMMAND_INVALID
//
// MessageText:
//
// Invalid command received.
//
#define ERR_ETH_INTF_COMMAND_INVALID     ((uint32_t)0xC05D0001L)

//
// MessageId: ERR_ETH_INTF_CONFIG_LOCK
//
// MessageText:
//
// Configuration is locked.
//
#define ERR_ETH_INTF_CONFIG_LOCK         ((uint32_t)0xC05D0002L)

//
// MessageId: ERR_ETH_INTF_INVALID_PACKET_LENGHT
//
// MessageText:
//
// Invalid packet length.
//
#define ERR_ETH_INTF_INVALID_PACKET_LENGHT ((uint32_t)0xC05D0003L)

//
// MessageId: ERR_ETH_INTF_INVALID_MODE
//
// MessageText:
//
// Invalid mode in request.
//
#define ERR_ETH_INTF_INVALID_MODE        ((uint32_t)0xC05D0004L)

//
// MessageId: ERR_ETH_INTF_PARAM_AUTO_NEGOTIATION_PORT_0
//
// MessageText:
//
// Invalid parameter for auto-negotiation port 0.
//
#define ERR_ETH_INTF_PARAM_AUTO_NEGOTIATION_PORT_0 ((uint32_t)0xC05D0005L)

//
// MessageId: ERR_ETH_INTF_PARAM_AUTO_NEGOTIATION_PORT_1
//
// MessageText:
//
// Invalid parameter for auto-negotiation port 1.
//
#define ERR_ETH_INTF_PARAM_AUTO_NEGOTIATION_PORT_1 ((uint32_t)0xC05D0006L)

//
// MessageId: ERR_ETH_INTF_PARAM_DUPLEX_MODE_PORT_0
//
// MessageText:
//
// Invalid parameter for duplex mode port 0.
//
#define ERR_ETH_INTF_PARAM_DUPLEX_MODE_PORT_0 ((uint32_t)0xC05D0007L)

//
// MessageId: ERR_ETH_INTF_PARAM_DUPLEX_MODE_PORT_1
//
// MessageText:
//
// Invalid parameter for duplex mode port 1.
//
#define ERR_ETH_INTF_PARAM_DUPLEX_MODE_PORT_1 ((uint32_t)0xC05D0008L)

//
// MessageId: ERR_ETH_INTF_PARAM_TRANSMISSION_RATE_PORT_0
//
// MessageText:
//
// Invalid parameter for transmission rate port 0.
//
#define ERR_ETH_INTF_PARAM_TRANSMISSION_RATE_PORT_0 ((uint32_t)0xC05D0009L)

//
// MessageId: ERR_ETH_INTF_PARAM_TRANSMISSION_RATE_PORT_1
//
// MessageText:
//
// Invalid parameter for transmission rate port 1.
//
#define ERR_ETH_INTF_PARAM_TRANSMISSION_RATE_PORT_1 ((uint32_t)0xC05D000AL)

//
// MessageId: ERR_ETH_INTF_PARAM_AUTO_CROSSOVER_PORT_0
//
// MessageText:
//
// Invalid parameter for auto cross-over port 0.
//
#define ERR_ETH_INTF_PARAM_AUTO_CROSSOVER_PORT_0 ((uint32_t)0xC05D000BL)

//
// MessageId: ERR_ETH_INTF_PARAM_AUTO_CROSSOVER_PORT_1
//
// MessageText:
//
// Invalid parameter for auto cross-over port 1.
//
#define ERR_ETH_INTF_PARAM_AUTO_CROSSOVER_PORT_1 ((uint32_t)0xC05D000CL)

//
// MessageId: ERR_ETH_INTF_NO_CONFIGURATION
//
// MessageText:
//
// Task is not configured.
//
#define ERR_ETH_INTF_NO_CONFIGURATION    ((uint32_t)0xC05D000DL)

//
// MessageId: ERR_ETH_INTF_APP_NOT_REGISTERED
//
// MessageText:
//
// No application registered.
//
#define ERR_ETH_INTF_APP_NOT_REGISTERED  ((uint32_t)0xC05D000EL)

//
// MessageId: ERR_ETH_INTF_APP_SET_NOT_READY
//
// MessageText:
//
// Application set not ready.
//
#define ERR_ETH_INTF_APP_SET_NOT_READY   ((uint32_t)0xC05D000FL)

//
// MessageId: ERR_ETH_INTF_LINK_DOWN
//
// MessageText:
//
// No Ethernet link.
//
#define ERR_ETH_INTF_LINK_DOWN           ((uint32_t)0xC05D0010L)

//
// MessageId: ERR_ETH_INTF_GET_SEND_BUFFER
//
// MessageText:
//
// Failed to get send buffer.
//
#define ERR_ETH_INTF_GET_SEND_BUFFER     ((uint32_t)0xC05D0011L)

//
// MessageId: ERR_ETH_INTF_SEND_FRAME
//
// MessageText:
//
// Failed to send Ethernet frame.
//
#define ERR_ETH_INTF_SEND_FRAME          ((uint32_t)0xC05D0012L)

//
// MessageId: ERR_ETH_INTF_SET_DRV_EDD_CFG
//
// MessageText:
//
// Failed to set driver EDD configuration.
//
#define ERR_ETH_INTF_SET_DRV_EDD_CFG     ((uint32_t)0xC05D0013L)

//
// MessageId: ERR_ETH_INTF_INVALID_ETH_PORT
//
// MessageText:
//
// Invalid parameter for Ethernet port.
//
#define ERR_ETH_INTF_INVALID_ETH_PORT    ((uint32_t)0xC05D0014L)

//
// MessageId: ERR_ETH_INTF_UNKNOWN_ERROR
//
// MessageText:
//
// Unknown error detected.
//
#define ERR_ETH_INTF_UNKNOWN_ERROR       ((uint32_t)0xC05DFFFFL)

#endif  /* __ETH_INTF_RESULTS_H */
