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

/**************************************************************************************/
/*! General Inclusion Area                                                            */
/**************************************************************************************/

#include <stdbool.h>
#include <malloc.h>

#include "App_DemoApplication.h"
#include "App_EventHandler.h"
#include "App_PacketCommunication.h"
#include "App_TerminalHandler.h"
#include "App_SystemPackets.h"
#include "Hil_DualPortMemory.h"
#include "Hil_ApplicationCmd.h"

extern PROTOCOL_DESCRIPTOR_T g_tRealtimeProtocolHandlers;
extern PROTOCOL_DESCRIPTOR_T g_tNetworkServicesHandlers;
extern PROTOCOL_DESCRIPTOR_T g_tNetProxyHandlers;

/* application data is kept global for use by multiples modules */
APP_DATA_T g_tAppData;

/**************************************************************************************/
/*! FUNCTIONS                                                                         */
/**************************************************************************************/

/**************************************************************************************/
/*! Entry point of cifX Application Demo.
 *
 * \param szBoardName Name of the cifX device, e.g., "cifX0"
 *
 * \return CIFX_NO_ERROR on success
 */
/**************************************************************************************/
int App_CifXApplicationDemo(char *szBoardName)
{
  CIFXHANDLE hDriver  = NULL;  /** Handle of cifX driver               */
  int32_t   lRet      = 0;     /** Return value for common error codes */
  uint32_t  ulState   = 0;     /** Actual state returned               */
  uint32_t  ulTimeout = 1000;  /** Timeout in milliseconds             */

  PRINTF("---------- cifX Application Demo ----------" NEWLINE);

  g_tAppData.fRunning = true;

  /* We know the firmware being executed, so we map our different protocol modules directly
     to the channels here. Each protocol module brings a descriptor, providing distinct entry points for
     configuration and packet handling.
  */
  /* communication channel 0 is used by default, it must be explicitly disabled */
#ifndef DEMO_DONT_USE_CHANNEL0
  g_tAppData.aptChannels[0] = (APP_COMM_CHANNEL_T*)calloc(1, sizeof(APP_COMM_CHANNEL_T));
  g_tAppData.aptChannels[0]->tProtocol = g_tRealtimeProtocolHandlers;     /* This is our realtime EtherNet protocol stack */
#endif

  /* usage of communication channel 1 must be explicitly enabled */
#ifdef DEMO_USE_CHANNEL1
  g_tAppData.aptChannels[1] = (APP_COMM_CHANNEL_T*)calloc(1, sizeof(APP_COMM_CHANNEL_T));
  g_tAppData.aptChannels[1]->tProtocol = g_tNetworkServicesHandlers;     /* Communication Channel 1 - Network Services (SocketIF, WebIF, NDIS) */
#endif

  /* usage of communication channel 2 must be explicitly enabled */
#ifdef DEMO_USE_CHANNEL2
  g_tAppData.aptChannels[2] = (APP_COMM_CHANNEL_T*)calloc(1, sizeof(APP_COMM_CHANNEL_T));
  g_tAppData.aptChannels[2]->tProtocol = g_tNetProxyHandlers;            /* Communication Channel 2 - NetProxy */
#endif


  if (HOSTAL_OK != (lRet = HOSTAL_Init()))
  {
    PRINTF("ERROR: HOSTAL_INIT failed: 0x%08x" NEWLINE, (unsigned int)lRet);
    return lRet;
  }

  App_Selftest();

  /* open driver */
  if (CIFX_NO_ERROR != (lRet = xDriverOpen(&hDriver)))
  {
    PRINTF("ERROR: xDriverOpen failed: 0x%08x" NEWLINE, (unsigned int)lRet);
    return lRet;
  }

  App_ReadBoardInfo(hDriver, szBoardName, &g_tAppData.tBoardInfo);

  /** Open all communication channels */
  if (CIFX_NO_ERROR != App_AllChannels_Open(&g_tAppData, hDriver, szBoardName))
  {
    PRINTF("ERROR: Failed to open communication channels: 0x%08x" NEWLINE, (unsigned int)lRet);
    goto error_exit;
  }

  /** Wait for HIL_COMM_COS_READY set in all relevant channels */
  App_AllChannels_GetChannelInfo_WaitReady(&g_tAppData);

  /** Set the Application state flag in the application COS flags */
  if (CIFX_NO_ERROR != (lRet = xChannelHostState(g_tAppData.aptChannels[0]->hChannel, CIFX_HOST_STATE_READY, &ulState, ulTimeout)))
  {
    PRINTF("ERROR: xChannelHostState failed: 0x%08X" NEWLINE, (unsigned int)lRet);
    goto error_exit;
  }

/* in some rare cases no cyclic access to the IO data area of the DPM is required,
  * in these cases only DPM mailbox packets are used for communication between host application and protocol stack
  * the macro DEMO_ONLY_MAILBOX can be set in the wscript in order to remove the code related to cyclic IO data access*/
#ifndef DEMO_ONLY_MAILBOX

  /** register App_IODataHandler as timer callback function */
  if(HOSTAL_OK == (lRet = HOSTAL_Callback_Register(HOSTAL_CB_TIMER, App_IODataHandler, (void*)&g_tAppData)))
  {
    /** enable callback functions only after timer callback function is registered */
    lRet = HOSTAL_Callbacks_Enable();
  }
  if (lRet != HOSTAL_OK)
  {
    PRINTF("Error: HOSTAL_Callback_Register/HOSTAL_Callbacks_Enable failed: 0x%08X" NEWLINE, (unsigned int)lRet);
    goto error_exit;
  }
#endif

  if (CIFX_NO_ERROR != (lRet = App_AllChannels_Configure(&g_tAppData)))
  {
    PRINTF("Error: A channel failed to configure: 0x%08X" NEWLINE, (unsigned int)lRet);
    goto error_exit;
  }

  /** now the bus is running */
  while(g_tAppData.fRunning && lRet == CIFX_NO_ERROR)
  {
    /** check and process incoming packets */
    lRet = App_AllChannels_PacketHandler(&g_tAppData);

    /** check and process input from terminal (UART console) */
    App_TerminalHandler(&g_tAppData);

    /** check for events (DIP switches, changed variable values, etc.) */
    App_EventHandler(&g_tAppData);

    HOSTAL_Sleep(1);
  }

error_exit:

#ifndef DEMO_ONLY_MAILBOX
  /** remove calling of App_IODataHandler, because we don't have valid IO Data any more */
  HOSTAL_Callbacks_Disable();
  HOSTAL_Callback_Register(HOSTAL_CB_TIMER, NULL, NULL);
#endif

  if ((g_tAppData.aptChannels[0] != NULL) &&
      (g_tAppData.aptChannels[0]->hChannel != NULL))
  {
    /** Set the bus state flag in the application COS state flags, to stop communication */
    xChannelBusState(g_tAppData.aptChannels[0]->hChannel, CIFX_BUS_STATE_OFF, &ulState, 10);

    /** Set Host not ready to stop bus communication */
    xChannelHostState(g_tAppData.aptChannels[0]->hChannel, CIFX_HOST_STATE_NOT_READY, &ulState, ulTimeout);
  }

  /** close all communication channels */
  App_AllChannels_Close(&g_tAppData);

  {
    /* free all channel memory */
    int iChannelIdx = 0;
    for (iChannelIdx = 0; iChannelIdx < MAX_COMMUNICATION_CHANNEL_COUNT; iChannelIdx++)
    {
      if (g_tAppData.aptChannels[iChannelIdx] != NULL)
      {
        free(g_tAppData.aptChannels[iChannelIdx]);
      }
    }
  }

  Pkt_Deinit();

  g_tAppData.fRunning = false;

  /** Close driver */
  xDriverClose(hDriver);
  hDriver = NULL;

  PRINTF(" Application terminated: Status = 0x%08X !" NEWLINE, (unsigned int)lRet);
  PRINTF("----------------------------------------------------" NEWLINE);

  return lRet;
}


/* in some rare cases no cyclic access to the IO data area of the DPM is required,
 * in these cases only DPM mailbox packets are used for communication between host application and protocol stack
 * the macro DEMO_ONLY_MAILBOX can be set in the wscript in order to remove the code related to cyclic IO data access*/
#ifndef DEMO_ONLY_MAILBOX
/**************************************************************************************/
/*! Cyclic IO data handler: fast data exchange with netX via DPM called from a timer ISR
 *
 * \param ptAppData  [in]  Pointer to application data
 */
/**************************************************************************************/
void App_IODataHandler(void* ptAppResources)
{
  long            lRet      = CIFX_NO_ERROR; /** Return value for common error codes  */
  APP_DATA_T*     ptAppData = (APP_DATA_T*)ptAppResources;

  if(ptAppData->aptChannels[0]->hChannel != NULL)
  {
    App_IO_UpdateCycleCounter(ptAppData);

    /** INPUT DATA *********************************************************************/
    lRet = xChannelIORead(ptAppData->aptChannels[0]->hChannel, 0, 0, sizeof(ptAppData->tInputData), &ptAppData->tInputData, 0);
    if(lRet != CIFX_NO_ERROR)
    {
      /** Something failed?
       * Reason for error could be:
       * 1) netX is not "ready" yet. May happen during startup.
       * 2) netX is not "running" yet. May happen during startup in case the netX is not fully configured yet.
       * 3) netX has not yet established an IO connection. */

      ptAppData->fInputDataValid = false;
    }
    else
    {
      /** process newly received input data image */
      ptAppData->fInputDataValid = true;

      HOSTAL_Actuator_SetData(0, ptAppData->tInputData.usActuator_1_Input);
      HOSTAL_Actuator_SetData(1, ptAppData->tInputData.usActuator_2_Input);
    }

    /** OUTPUT DATA ***************************************/
    /** update output data image to be sent in this cycle */

    HOSTAL_Sensor_GetData(0, &ptAppData->tOutputData.usSensor_1_Output);
    ptAppData->tOutputData.bSensor_1_State = HOSTAL_Sensor_GetState(0, &ptAppData->tAcyclicData.usSensor1_StatusCode);

    HOSTAL_Sensor_GetData(1, &ptAppData->tOutputData.usSensor_2_Output);
    ptAppData->tOutputData.bSensor_2_State = HOSTAL_Sensor_GetState(1, &ptAppData->tAcyclicData.usSensor2_StatusCode);

    ptAppData->tOutputData.bActuator_1_State = HOSTAL_Actuator_GetState(0, &ptAppData->tAcyclicData.usActuator1_StatusCode);
    ptAppData->tOutputData.bActuator_2_State = HOSTAL_Actuator_GetState(1, &ptAppData->tAcyclicData.usActuator2_StatusCode);

    lRet = xChannelIOWrite(ptAppData->aptChannels[0]->hChannel, 0, 0, sizeof(ptAppData->tOutputData), &ptAppData->tOutputData, 0);
    if(lRet != CIFX_NO_ERROR)
    {
      /** Something failed?
       * Reason for error could be:
       * 1) netX is not "ready" yet. May happen during startup.
       * 2) netX is not "running" yet. May happen during startup in case the netX is not fully configured yet.
       * 3) netX has not yet established an IO connection. */
    }
  }
}
#endif
