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

/*****************************************************************************/
/*! General Inclusion Area                                                   */
/*****************************************************************************/

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "App_SystemPackets.h"
#include "App_TerminalHandler.h"

/*****************************************************************************/
/*! GLOBAL, STATIC OR EXTERN VARIABLES                                       */
/*****************************************************************************/

/** List of acyclic data objects */
APP_TERMINAL_ACYCLIC_DATA_OBJECT_NAMES_T g_atTerminalAcyclicDataObjectDictionary[] =
{
    /* Acyclic data of sensors */
    {APP_TERMINAL_ACYCLIC_DATA_SENSOR_1_MODE,         "Sensor 1 Mode:"          },
    {APP_TERMINAL_ACYCLIC_DATA_SENSOR_1_STATUSCODE,   "Sensor 1 Status Code:"   },
    {APP_TERMINAL_ACYCLIC_DATA_SENSOR_2_MODE,         "Sensor 2 Mode:"          },
    {APP_TERMINAL_ACYCLIC_DATA_SENSOR_2_STATUSCODE,   "Sensor 2 Status Code:"   },

    /* Acyclic data of actuators */
    {APP_TERMINAL_ACYCLIC_DATA_ACTUATOR_1_MODE,       "Actuator 1 Mode:"        },
    {APP_TERMINAL_ACYCLIC_DATA_ACTUATOR_1_STATUSCODE, "Actuator 1 Status Code:" },
    {APP_TERMINAL_ACYCLIC_DATA_ACTUATOR_2_MODE,       "Actuator 2 Mode:"        },
    {APP_TERMINAL_ACYCLIC_DATA_ACTUATOR_2_STATUSCODE, "Actuator 2 Status Code:" },
};

/** Terminal service commands */
APP_TERMINAL_CMD_T g_tTerminalCommandsDictionary[] =
{
  {APP_TERMINAL_HANDLER_COMMAND_DUMP_INPUT_PROCESS_DATA,  "in",           "Display input data image"   },
  {APP_TERMINAL_HANDLER_COMMAND_DUMP_OUTPUT_PROCESS_DATA, "out",          "Display output data image"  },
  {APP_TERMINAL_HANDLER_COMMAND_FIRMWARE_INFO,            "fwinfo",       "Firmware info"              },
  {APP_TERMINAL_HANDLER_COMMAND_HARDWARE_INFO,            "hwinfo",       "Hardware info"              },
  {APP_TERMINAL_HANDLER_COMMAND_CHANNEL_INFO,             "chinfo",       "Channel info" },
  {APP_TERMINAL_HANDLER_COMMAND_HELP,                     "help",         "Display help"               },
  {APP_TERMINAL_HANDLER_COMMAND_ACYCLIC_READ,             "rd",           "Read acyclic data object"   },
  {APP_TERMINAL_HANDLER_COMMAND_ACYCLIC_WRITE,            "wr",           "Write acyclic data object"  },
  {APP_TERMINAL_HANDLER_COMMAND_LIST_OBJECTS,             "list",         "Read and list all objects"  },
  {APP_TERMINAL_HANDLER_COMMAND_UPDATE,                   "update",       "trigger FW update by MFW"   },
  {APP_TERMINAL_HANDLER_COMMAND_PACKET_CONFIG_DELETE,     "deleteconfig", "Send HIL_DELETE_CONFIG_REQ" },
  {APP_TERMINAL_HANDLER_COMMAND_QUIT,                     "quit",         "Returns from main function" },
};

/*****************************************************************************/
/*! FUNCTIONS                                                                */
/*****************************************************************************/

/*****************************************************************************/
/*! Translates the given command name to the appropriate command code
 *
 *   \param szCmd [in] Command name
 *
 *   \return Command code
 */
/*****************************************************************************/
static uint8_t Term_GetCommandCodeByCommandName(char* szCommandName)
{
  uint8_t bIndex = 0;

  for(bIndex = 0; bIndex < APP_TERMINAL_HANDLER_NUMBER_OF_COMMANDS; bIndex++)
  {
    if(0 == strcmp(szCommandName, &g_tTerminalCommandsDictionary[bIndex].szString[0]))
    {
      return g_tTerminalCommandsDictionary[bIndex].bCommandCode;
    }
  }

  return APP_TERMINAL_HANDLER_COMMAND_UNKNOWN;
}


/*****************************************************************************/
/*! Displays a list of supported terminal commands
 */
/*****************************************************************************/
static void Term_PrintHelp(void)
{
  uint8_t bIndex;

  for(bIndex = 0; bIndex < APP_TERMINAL_HANDLER_NUMBER_OF_COMMANDS; bIndex++)
  {
    int indent = APP_TERMINAL_MAX_CMD_LEN + 1 - strlen(g_tTerminalCommandsDictionary[bIndex].szString);
    if (indent < 0)
    {
      indent = 0;
    }
    PRINTF("%s", g_tTerminalCommandsDictionary[bIndex].szString);
    while (indent > 0)
    {
      indent--;
      PRINTF("%c", ' ');
    }

    PRINTF("%s" NEWLINE, g_tTerminalCommandsDictionary[bIndex].szHelp);
  }
  PRINTF(NEWLINE);
}

/*****************************************************************************/
/*! Read acyclic data object
 *
 *   \param  ptAppData           [in]   Pointer to application data
 *   \param  eAcyclicDataObject  [in]   Data object number that shall be read
 *   \param  pulData             [out]  Object data
 *
 *   \return CIFX_NO_ERROR on success, otherwise CIFX_INVALID_PARAMETER
 */
/*****************************************************************************/
static uint32_t Term_ReadAcyclicData(APP_DATA_T*                        ptAppData,
                                      APP_TERMINAL_ACYCLIC_DATA_OBJECT_E eAcyclicDataObject,
                                      uint32_t*                          pulData)
{
  uint32_t ulRslt = CIFX_NO_ERROR;

  switch(eAcyclicDataObject)
  {
    case APP_TERMINAL_ACYCLIC_DATA_SENSOR_1_MODE:
      *pulData = (uint32_t)ptAppData->tAcyclicData.bSensor1_Mode;
      break;

    case APP_TERMINAL_ACYCLIC_DATA_SENSOR_1_STATUSCODE:
      *pulData = (uint32_t)ptAppData->tAcyclicData.usSensor1_StatusCode;
      break;

    case APP_TERMINAL_ACYCLIC_DATA_SENSOR_2_MODE:
      *pulData = (uint32_t)ptAppData->tAcyclicData.bSensor2_Mode;
      break;

    case APP_TERMINAL_ACYCLIC_DATA_SENSOR_2_STATUSCODE:
      *pulData = (uint32_t)ptAppData->tAcyclicData.usSensor2_StatusCode;
      break;

    case  APP_TERMINAL_ACYCLIC_DATA_ACTUATOR_1_MODE:
      *pulData = (uint32_t)ptAppData->tAcyclicData.bActuator1_Mode;
      break;

    case APP_TERMINAL_ACYCLIC_DATA_ACTUATOR_1_STATUSCODE:
      *pulData = (uint32_t)ptAppData->tAcyclicData.usActuator1_StatusCode;
      break;

    case  APP_TERMINAL_ACYCLIC_DATA_ACTUATOR_2_MODE:
      *pulData = (uint32_t)ptAppData->tAcyclicData.bActuator2_Mode;
      break;

    case APP_TERMINAL_ACYCLIC_DATA_ACTUATOR_2_STATUSCODE:
      *pulData = (uint32_t)ptAppData->tAcyclicData.usActuator2_StatusCode;
      break;

    default:
      ulRslt = CIFX_INVALID_PARAMETER;
      break;
  }

  return ulRslt;
}


/*****************************************************************************/
/*! Write acyclic data object
 *
 *   \param ptAppData           [in]   Pointer to application data
 *   \param eAcyclicDataObject  [in]   Data object number that shall be written
 *   \param ulData              [in]   Object data
 *
 *   \return CIFX_NO_ERROR on success.
 *           Other return codes: CIFX_INVALID_PARAMETER --> Unknown acyclic data object
 *                               CIFX_INVALID_COMMAND   --> Acyclic data object cannot be written (is read only)
 */
/*****************************************************************************/
static uint32_t Term_WriteAcyclicData(APP_DATA_T*                        ptAppData,
                                      APP_TERMINAL_ACYCLIC_DATA_OBJECT_E eAcyclicDataObject,
                                      uint32_t                           ulData)
{
  uint32_t ulRslt = CIFX_NO_ERROR;

  switch(eAcyclicDataObject)
  {
    case APP_TERMINAL_ACYCLIC_DATA_SENSOR_1_MODE:
      ptAppData->tAcyclicData.bSensor1_Mode = (uint8_t)ulData;
      break;

    case APP_TERMINAL_ACYCLIC_DATA_SENSOR_1_STATUSCODE:
      /* Status is read-only! */
      ulRslt = CIFX_INVALID_COMMAND;
      break;

    case APP_TERMINAL_ACYCLIC_DATA_SENSOR_2_MODE:
      ptAppData->tAcyclicData.bSensor2_Mode = (uint8_t)ulData;
      break;

    case APP_TERMINAL_ACYCLIC_DATA_SENSOR_2_STATUSCODE:
      /* Status is read-only! */
      ulRslt = CIFX_INVALID_COMMAND;
      break;

    case  APP_TERMINAL_ACYCLIC_DATA_ACTUATOR_1_MODE:
      ptAppData->tAcyclicData.bActuator1_Mode = (uint8_t)ulData;
      break;

    case APP_TERMINAL_ACYCLIC_DATA_ACTUATOR_1_STATUSCODE:
      /* Status is read-only! */
      ulRslt = CIFX_INVALID_COMMAND;
      break;

    case  APP_TERMINAL_ACYCLIC_DATA_ACTUATOR_2_MODE:
      ptAppData->tAcyclicData.bActuator2_Mode = (uint8_t)ulData;
      break;

    case APP_TERMINAL_ACYCLIC_DATA_ACTUATOR_2_STATUSCODE:
      /* Status is read-only! */
      ulRslt = CIFX_INVALID_COMMAND;
      break;

    default:
      ulRslt = CIFX_INVALID_PARAMETER;
      break;
  }

  return ulRslt;
}

/*****************************************************************************/
/*! Displays a list of all acyclic data objects and their values
 *
 *   \param  ptAppData  [in]  Pointer to application data
 */
/*****************************************************************************/
static void Term_ListAcyclicDataObejcts(APP_DATA_T* ptAppData)
{
  uint8_t  bIndex;
  uint32_t ulValue = 0;

  for(bIndex = 0; bIndex < APP_TERMINAL_ACYCLIC_DATA_NUMBER_OF_ACYCLIC_DATA_ENTITIES; bIndex++)
  {
    if(CIFX_NO_ERROR == Term_ReadAcyclicData(ptAppData, bIndex, &ulValue))
    {
      PRINTF(g_atTerminalAcyclicDataObjectDictionary[bIndex].szName);
      PRINTF("value 0x%04x " NEWLINE, (int)ulValue);
    }
  }
} /** Term_ListVars */


/*****************************************************************************/
/*! Handler for incoming commands
 *
 *   \param ptAppData             [in] pointer to APP_DATA_T structure
 *   \param uiTerminalCommandCode [in] terminal command code
 *   \param iNumberOfArguments    [in] Number of arguments in auiArgumentList
 *   \param auiArgumentList       [in] Argument list
 */
/*****************************************************************************/
static void Term_TerminalCommandHandler(APP_DATA_T*   ptAppData,
                                            int           uiTerminalCommandCode,
                                            int           iNumberOfArguments,
                                            unsigned int* auiArgumentList)
{
  uint32_t  ulRslt = CIFX_NO_ERROR;
  uint32_t  ulVal  = 0;

  if(iNumberOfArguments == 0)
  {
    PRINTF("Not enough arguments provided" NEWLINE);
  }
  else
  {
    switch(uiTerminalCommandCode)
    {
      case APP_TERMINAL_HANDLER_COMMAND_HELP:
      {
        PRINTF("Help:" NEWLINE);
        Term_PrintHelp();
        break;
      }

      case APP_TERMINAL_HANDLER_COMMAND_ACYCLIC_READ:
      {
        if(iNumberOfArguments < 2)
        {
          PRINTF("ERROR, READ wrong number of arguments" NEWLINE);
          PRINTF("Use as \"READ <any_variable_idx>\"" NEWLINE);
          break;
        }
        ulRslt = Term_ReadAcyclicData(ptAppData, auiArgumentList[0], &ulVal);
        switch (ulRslt)
        {
          case CIFX_NO_ERROR:
            PRINTF("OK, read var 0x%02x value 0x%08x" NEWLINE, auiArgumentList[0], (int)ulVal);
            break;
          case CIFX_INVALID_PARAMETER:
            PRINTF("ERROR, var 0x%02x is unknown" NEWLINE, auiArgumentList[0]);
            break;
          default:
            break;
        }
        break;
      }

      case APP_TERMINAL_HANDLER_COMMAND_ACYCLIC_WRITE:
      {
        if(iNumberOfArguments < 3)
        {
          PRINTF("ERROR, WRITE wrong number of arguments" NEWLINE);
          PRINTF("Use as \"WRITE <any_variable_idx> <value>\"" NEWLINE);
          break;
        }
        ulRslt = Term_WriteAcyclicData(ptAppData, auiArgumentList[0], (uint32_t)auiArgumentList[1]);

        switch(ulRslt)
        {
          case CIFX_NO_ERROR:
            PRINTF("OK, wrote var 0x%02x value 0x%08x" NEWLINE, auiArgumentList[0], auiArgumentList[1]);
            break;
          case CIFX_INVALID_COMMAND:
            PRINTF("ERROR, var 0x%02x is READ ONLY" NEWLINE, auiArgumentList[0]);
            break;
          case CIFX_INVALID_PARAMETER:
            PRINTF("ERROR, var 0x%02x is unknown" NEWLINE, auiArgumentList[0]);
            break;
          default:
            break;
        }
        break;
      }

      case APP_TERMINAL_HANDLER_COMMAND_FIRMWARE_INFO:
      {
        App_SysPkt_AssembleFirmwareIdentifyReq(&ptAppData->aptChannels[0]->tPacket, 0);
        ulRslt = Pkt_SendReceivePacket(ptAppData, 0, &ptAppData->aptChannels[0]->tPacket, TXRX_TIMEOUT);
        if((CIFX_NO_ERROR == ulRslt) && (CIFX_NO_ERROR == ptAppData->aptChannels[0]->tPacket.tHeader.ulState))
        {
          App_SysPkt_HandleFirmwareIdentifyCnf(&ptAppData->aptChannels[0]->tPacket);
        }
        break;
      }

      case APP_TERMINAL_HANDLER_COMMAND_UPDATE:
      {
        App_SysPkt_AssembleFirmwareResetReq(&ptAppData->aptChannels[0]->tPacket, HIL_SYS_CONTROL_RESET_MODE_UPDATESTART, 0, 1);
        ulRslt = Pkt_SendReceivePacket(ptAppData, 0, &ptAppData->aptChannels[0]->tPacket, TXRX_TIMEOUT);
        if((CIFX_NO_ERROR == ulRslt) && (CIFX_NO_ERROR == ptAppData->aptChannels[0]->tPacket.tHeader.ulState))
        {
        }
        break;
      }

      case APP_TERMINAL_HANDLER_COMMAND_HARDWARE_INFO:
      {
        App_SysPkt_AssembleHardwareInfoReq(&ptAppData->aptChannels[0]->tPacket);
        ulRslt = Pkt_SendReceivePacket(ptAppData, 0, &ptAppData->aptChannels[0]->tPacket, TXRX_TIMEOUT);
        if((CIFX_NO_ERROR == ulRslt) && (CIFX_NO_ERROR == ptAppData->aptChannels[0]->tPacket.tHeader.ulState))
        {
          App_SysPkt_HandleHardwareInfoCnf(&ptAppData->aptChannels[0]->tPacket);
        }
        break;
      }

      case APP_TERMINAL_HANDLER_COMMAND_CHANNEL_INFO:
      {
        if (iNumberOfArguments != 2)
        {
          PRINTF("ERROR, CHINFO wrong number of arguments" NEWLINE);
          PRINTF("Use as \"CHINFO <channel_index>\"" NEWLINE);
          break;
        }

        if ((auiArgumentList[0] < 0) || (auiArgumentList[0] > MAX_COMMUNICATION_CHANNEL_COUNT))
        {
          PRINTF("ERROR, CHINFO invalid channel index" NEWLINE);
          break;
        }

        if ((ptAppData->aptChannels[auiArgumentList[0]] == NULL) ||
            (ptAppData->aptChannels[auiArgumentList[0]]->hChannel == NULL))
        {
          PRINTF("ERROR, CHINFO channel not used by demo project" NEWLINE);
          break;
        }

        App_ReadChannelInfo(ptAppData->aptChannels[auiArgumentList[0]]->hChannel,
                           &ptAppData->aptChannels[auiArgumentList[0]]->tChannelInfo);
        break;
      }

      case APP_TERMINAL_HANDLER_COMMAND_DUMP_INPUT_PROCESS_DATA:
      {
        int iIdx;
        PRINTF("Input Data Image:" NEWLINE);
        for(iIdx = 0; iIdx < sizeof(ptAppData->tInputData); iIdx++)
        {
          PRINTF("%02d 0x%02x" NEWLINE, iIdx, (int)((uint8_t*)&ptAppData->tInputData)[iIdx]);
        }
        break;
      }

      case APP_TERMINAL_HANDLER_COMMAND_DUMP_OUTPUT_PROCESS_DATA:
      {
        int iIdx;
        PRINTF("Output Data Image:" NEWLINE);
        for(iIdx = 0; iIdx < sizeof(ptAppData->tOutputData); iIdx++)
        {
          PRINTF("%02d 0x%02x" NEWLINE, iIdx, (int)((uint8_t*)&ptAppData->tOutputData)[iIdx]);
        }
        break;
      }

      case APP_TERMINAL_HANDLER_COMMAND_LIST_OBJECTS:
      {
        PRINTF("Objects:" NEWLINE);
        Term_ListAcyclicDataObejcts(ptAppData);
        break;
      }

      case APP_TERMINAL_HANDLER_COMMAND_QUIT:
      {
        PRINTF("Quit now!" NEWLINE);
        ptAppData->fRunning = false;
        break;
      }

      case APP_TERMINAL_HANDLER_COMMAND_PACKET_CONFIG_DELETE:
      {
        PRINTF(NEWLINE "Send HIL_CONFIG_DELETE_REQ" NEWLINE);
        App_SysPkt_DeleteConfig(0);
        break;
      }

      case APP_TERMINAL_HANDLER_COMMAND_UNKNOWN:
      default:
      {
        PRINTF("Unknown Command!" NEWLINE);
        break;
      }
    }
  }
}

#define STR(tok) #tok
#define XSTR(tok) STR(tok)

/*****************************************************************************/
/*! Entry point for terminal handle. Needs to be called cyclically in order to
 *  process incoming terminal commands.
 *
 *   \param ptAppData [in]  Pointer to application data
 */
/*****************************************************************************/
void App_TerminalHandler(APP_DATA_T* ptAppData)
{
  static char   s_szInBuffer[APP_TERMINAL_MAX_CMDLINE_LEN];
  static int    s_iKeyCnt = 0;
  int           iKey      = 0;
  int           iNumberOfArguments     = 0;
  char          szCmd[APP_TERMINAL_MAX_CMD_LEN + 1];   /** plus one for the terminating NUL character */
  unsigned int  auiArgumentList[3];

  if(HOSTAL_kbhit())
  {
    iKey = HOSTAL_getchar();

    if(iKey == '\r' || iKey == '\n')
    {
      s_szInBuffer[s_iKeyCnt] = '\0';
      s_iKeyCnt               = 0;
      iNumberOfArguments = sscanf( &s_szInBuffer[0],
                                   "%" XSTR(APP_TERMINAL_MAX_CMD_LEN) "s %x %x %x",
                                   &szCmd[0],
                                   &auiArgumentList[0],
                                   &auiArgumentList[1],
                                   &auiArgumentList[2] ); /** command length is hardcoded here*/


      int iTerminalCommandCode = Term_GetCommandCodeByCommandName(&szCmd[0]);
      if (iTerminalCommandCode == APP_TERMINAL_HANDLER_COMMAND_UNKNOWN)
      {
        Term_PrintHelp();
      }
      else
      {
        Term_TerminalCommandHandler(ptAppData,
                                    iTerminalCommandCode,
                                    iNumberOfArguments,
                                    &auiArgumentList[0]);
      }

      App_AllChannels_TerminalHandler(ptAppData, s_szInBuffer);
    }
    else
    {
      s_szInBuffer[s_iKeyCnt] = (char)iKey;
      s_iKeyCnt++;

      if(s_iKeyCnt >= APP_TERMINAL_MAX_CMDLINE_LEN)
      {
        s_iKeyCnt = 0;
      }
    }
  }
}
