/**************************************************************************************
Exclusion of Liability for this demo software:
  The following software is intended for and must only be used for reference and in an
  evaluation laboratory environment. It is provided without charge and is subject to
  alterations. There is no warranty for the software, to the extent permitted by
  applicable law. Except when otherwise stated in writing the copyright holders and/or
  other parties provide the software "as is" without warranty of any kind, either
  expressed or implied.
  Please refer to the Agreement in README_DISCLAIMER.txt, provided together with this file!
  By installing or otherwise using the software, you accept the terms of this Agreement.
  If you do not agree to the terms of this Agreement, then do not install or use the
  Software!
**************************************************************************************/

/**************************************************************************************

Copyright (c) Hilscher Gesellschaft fuer Systemautomation mbH. All Rights Reserved.

***************************************************************************************/

#ifndef COMPONENTS_CIFXAPPLICATIONDEMO_INCLUDES_APP_TERMINALHANDLER_H_
#define COMPONENTS_CIFXAPPLICATIONDEMO_INCLUDES_APP_TERMINALHANDLER_H_

/*****************************************************************************/
/*! General Inclusion Area                                                   */
/*****************************************************************************/

#include "App_DemoApplication.h"
#include "Hil_DualPortMemory.h"
#include "hostAbstractionLayer.h"

/*****************************************************************************/
/*! DEFINITIONS                                                              */
/*****************************************************************************/

#define APP_TERMINAL_MAX_CMDLINE_LEN   64 /* Terminal buffer size */
#define APP_TERMINAL_MAX_CMD_LEN       20 /* Terminal command buffer size */

/*****************************************************************************/
/*! ENUMERATIONS of terminal commands                                        */
/*****************************************************************************/
typedef enum APP_TERMINAL_HANDLER_COMMANDS_Etag
{
  APP_TERMINAL_HANDLER_COMMAND_DUMP_INPUT_PROCESS_DATA = 0,
  APP_TERMINAL_HANDLER_COMMAND_DUMP_OUTPUT_PROCESS_DATA,
  APP_TERMINAL_HANDLER_COMMAND_FIRMWARE_INFO,
  APP_TERMINAL_HANDLER_COMMAND_HARDWARE_INFO,
  APP_TERMINAL_HANDLER_COMMAND_CHANNEL_INFO,
  APP_TERMINAL_HANDLER_COMMAND_HELP,
  APP_TERMINAL_HANDLER_COMMAND_ACYCLIC_READ,
  APP_TERMINAL_HANDLER_COMMAND_ACYCLIC_WRITE,
  APP_TERMINAL_HANDLER_COMMAND_LIST_OBJECTS,
  APP_TERMINAL_HANDLER_COMMAND_UPDATE,
  APP_TERMINAL_HANDLER_COMMAND_PACKET_CONFIG_DELETE,
  APP_TERMINAL_HANDLER_COMMAND_QUIT,

  APP_TERMINAL_HANDLER_NUMBER_OF_COMMANDS,
  APP_TERMINAL_HANDLER_COMMAND_UNKNOWN,
} APP_TERMINAL_HANDLER_COMMANDS_E;

/*****************************************************************************/
/*! ENUMERATIONS of terminal variables code                                  */
/*****************************************************************************/

typedef enum APP_TERMINAL_ACYCLIC_DATA_OBJECT_Etag
{
  /* Acyclic data of sensors */
  APP_TERMINAL_ACYCLIC_DATA_SENSOR_1_MODE = 0,
  APP_TERMINAL_ACYCLIC_DATA_SENSOR_1_STATUSCODE,

  APP_TERMINAL_ACYCLIC_DATA_SENSOR_2_MODE,
  APP_TERMINAL_ACYCLIC_DATA_SENSOR_2_STATUSCODE,

  /* Acyclic data of actuators */
  APP_TERMINAL_ACYCLIC_DATA_ACTUATOR_1_MODE,
  APP_TERMINAL_ACYCLIC_DATA_ACTUATOR_1_STATUSCODE,

  APP_TERMINAL_ACYCLIC_DATA_ACTUATOR_2_MODE,
  APP_TERMINAL_ACYCLIC_DATA_ACTUATOR_2_STATUSCODE,

  APP_TERMINAL_ACYCLIC_DATA_NUMBER_OF_ACYCLIC_DATA_ENTITIES,
} APP_TERMINAL_ACYCLIC_DATA_OBJECT_E;

typedef struct APP_TERMINAL_ACYCLIC_DATA_OBJECT_NAMES_Ttag
{
  APP_TERMINAL_ACYCLIC_DATA_OBJECT_E eAcyclicDataObject;
  char*                              szName;
} APP_TERMINAL_ACYCLIC_DATA_OBJECT_NAMES_T;


/*****************************************************************************/
/*! typedef terminal command structure                                       */
/*****************************************************************************/
typedef struct APP_TERMINAL_CMD_Ttag
{
  uint8_t bCommandCode;
  const char    *szString;
  const char    *szHelp;
} APP_TERMINAL_CMD_T;


/*****************************************************************************/
/*! FUNCTION PROTOTYPES                                                      */
/*****************************************************************************/
void App_TerminalHandler(APP_DATA_T *ptAppData);

#endif /* COMPONENTS_CIFXAPPLICATIONDEMO_INCLUDES_APP_TERMINALHANDLER_H_ */
