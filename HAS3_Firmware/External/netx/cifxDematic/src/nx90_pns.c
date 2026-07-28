/*
 * nx90_pns.c
 *
 *  Created on: Oct 30, 2024
 *      Author: A0084817
 */
#include "nx90_pns.h"

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "dPacketCommunication.h"
#include "Hil_Applicationcmd.h"
#include "Hil_SystemCmd.h"
#include "HIL_results.h"
#include "nx90_SystemPackets.h"

#define PNS_INPUT_BUFFERS PNS_INPUT_SUBMODULES + PNS_IN_OUT_SUBMODULES
#define PNS_OUTPUT_BUFFERS PNS_OUTPUT_SUBMODULES + PNS_IN_OUT_SUBMODULES

#define NO_WAIT_TIME 0

typedef struct
{
  N90Cifx_t * hNx90Cifx;
  Pkt_Comm packetHandler;
  CIFXHANDLE * channelHandler;
  cyclicDataReceived pfCyclicInputdata;
  void * pCyclicInputUserData;
  cyclicDataReceived pfCyclicOutputdata;
  void * pCyclicOutputUserData;
  pFuDebugCB debugCB;
  uint8_t * inputDataBuffers[PNS_INPUT_BUFFERS];
  uint8_t *outputDataBuffers[PNS_OUTPUT_BUFFERS];
  bool isConfigured;
  bool isCyclicCommunicating;
  bool isControllerConnected;
  bool isEventCyclicActivated;
  uint16_t TriggerPdin;
  uint16_t TriggerPdOut;
  uint16_t TriggerSync;
  uint16_t MinFreeRunUpdate;
}nx90_pns_t;

static nx90_pns_t pns_data = { 0 };

static bool indicationHandler(CIFX_PACKET* ptPacket, void* pvUserData);

static void assemblePnsConfigureCommand(PNS_CONFIGURATION_DATA_T * confData, CIFX_PACKET * packet);

static void pns_HandleParameterEndInd(CIFX_PACKET* ptPacket);

static int32_t pns_GetTriggerType();

static int32_t pns_SetTriggerType();

static int32_t handleInputCyclicData();

static int32_t handleOutputCyclicData();

void nx90PNS_Init(N90Cifx_t * hInitializedN90Cifx, nx90PNS_config_t * conf)
{
     if(NULL != hInitializedN90Cifx)
     {
       pns_data.hNx90Cifx = hInitializedN90Cifx;
       Pkt_Comm_config packet_conf = { 0 };
       packet_conf.fnIndHandler = indicationHandler;
       packet_conf.pvIndHandlerUserData = &pns_data;
       packet_conf.hChannel = pns_data.hNx90Cifx->n90Channels[0];
       packet_conf.setSucessHandler = conf->SendSucessCB;
       packet_conf.getSucessHandler = conf->RecSucessCB;
       dPkt_Init(&pns_data.packetHandler, &packet_conf);
       pns_data.channelHandler = pns_data.hNx90Cifx->n90Channels[0];
       pns_data.pfCyclicInputdata = conf->pfCyclicInputdata;
       pns_data.pCyclicInputUserData = conf->pCyclicInputUserData;
       pns_data.pfCyclicOutputdata = conf->pfCyclicOutputdata;
       pns_data.pCyclicOutputUserData = conf->pCyclicOutputUserData;
       pns_data.debugCB = conf->debugCB;
     }
}

int32_t nx90PNS_RegisterIndicationCallback(PNS_IF_CMD_E indicationId, indicationCallback callback, void * userData)
{
    // ToDO implement indication.
    return -1;
}

int32_t nx90PNS_CommStackTasks()
{
  if(!pns_data.isConfigured)
  {
    return -1;
  }
  CIFX_PACKET Packet;
  int32_t lRet = dPkt_CheckReceiveMailbox(&pns_data.packetHandler, &Packet);
  if( CIFX_DEV_GET_NO_PACKET == lRet || CIFX_DEV_NOT_READY == lRet)
  {
    /* Handle "no packet" and "stack not ready" as success. */
    lRet = CIFX_NO_ERROR;
  }
  return lRet;
}

int32_t nx90PNS_CyclicTasks()
{
     if(!pns_data.isControllerConnected)
     {
       return -1;
     }
     int32_t lRet;
     /** INPUT DATA *********************************************************************/
     lRet = handleInputCyclicData();
     /** OUTPUT DATA ***************************************/
     lRet = handleOutputCyclicData();
     return lRet;
}

int32_t nx90PNS_ConfigureStack(PNS_CONFIGURATION_DATA_T * confData)
{
  CIFX_PACKET Packet;
  int32_t lRet;

  nx90_SysPkt_AssembleRegisterAppReq(&Packet);
  lRet = dPkt_SendReceivePacket(&pns_data.packetHandler, &Packet);
  if(lRet != CIFX_NO_ERROR)
  {
    return lRet;
  }

  assemblePnsConfigureCommand(confData, &Packet);
  lRet = dPkt_SendReceivePacket(&pns_data.packetHandler, &Packet);
  if(lRet != CIFX_NO_ERROR)
  {
    return lRet;
  }

  // ToDo Validate it is properly configured.

  /** ToDo create a function for this configureIndicationPackets or so **/

  nx90_SysPkt_AssembleChannelInitReq(&Packet);
  lRet = dPkt_SendReceivePacket(&pns_data.packetHandler, &Packet);
  if(lRet != CIFX_NO_ERROR)
  {
    return lRet;
  }
  nx90_SysPkt_AssembleStartStopCommReq(&Packet, true);
  lRet = dPkt_SendReceivePacket(&pns_data.packetHandler, &Packet);
  if(lRet != CIFX_NO_ERROR)
  {
    return lRet;
  }

  for(uint32_t i = 0; i < PNS_INPUT_BUFFERS; i++)
  {
    pns_data.inputDataBuffers[i] = (uint8_t *)malloc(PNSC_GetInputCyclicOffsets()->Lengths[i]);
  }

  for(uint32_t i = 0; i < PNS_OUTPUT_BUFFERS; i++)
  {
    pns_data.outputDataBuffers[i] = (uint8_t *)malloc(PNSC_GetOutputCyclicOffsets()->Lengths[i]);
  }

  pns_data.isConfigured = true;

  while(!pns_data.isControllerConnected)
  {
    nx90PNS_CommStackTasks();
  }
  /** ToDo end of create a function for this configureIndicationPackets or so, report any errors **/


  lRet = pns_SetTriggerType();

  if(lRet == 0)
     pns_data.isEventCyclicActivated = true;
  else
  {
    // ToDo Report it is properly configured.
  }

  return lRet;
}

/**
 * @brief Indication handler function that processes incoming indication packets.
 *
 * This function verifies the indication ID in the packet. If valid, it searches for
 * a registered callback. If a callback is found, it is called with the packet data.
 * If the callback provides a response, this response is sent. If no callback is found
 * or the callback does not provide a response, a default "No Application Registered"
 * response is sent.
 *
 * @param ptPacket Pointer to the packet received for processing.
 * @param pvUserData Pointer to `nx90_pns_t` structure containing indication callbacks.
 * @return true if the packet was processed (either by callback or by default response),
 *         false if the command is not a valid indication.
 */
static bool indicationHandler(CIFX_PACKET* ptPacket, void* pvUserData)
{
    bool fPacketCouldBeHandled = false;
    nx90_pns_t * pns = (nx90_pns_t *)pvUserData;

      switch (ptPacket->tHeader.ulCmd)
      {
      case HIL_LINK_STATUS_CHANGE_IND:
        (void) nx90_SysPkt_HandleLinkStatusChangeInd(&pns->packetHandler, ptPacket);
        fPacketCouldBeHandled = true;
        break;
      case PNS_IF_PARAM_END_IND:
        //mandatory, if Register Application is used
        // This function is necessary because it returns application ready = 1.
        // If application ready = 0 will be used, the application needs to send application ready request.
        (void) pns_HandleParameterEndInd(ptPacket);
        fPacketCouldBeHandled = true;
        break;
      case PNS_IF_EVENT_IND:;
        PNS_IF_EVENT_IND_DATA_T * eventRes = (PNS_IF_EVENT_IND_DATA_T *)ptPacket->abData;
        if(eventRes->ausEventCnt[PNS_IF_IO_EVENT_CONSUMER_UPDATE_REQUIRED] != 0)
          handleInputCyclicData();
        if(eventRes->ausEventCnt[PNS_IF_IO_EVENT_PROVIDER_UPDATE_REQUIRED] != 0)
          handleOutputCyclicData();
        ptPacket->tHeader.ulCmd  |= CIFX_MSK_PACKET_ANSWER;
        ptPacket->tHeader.ulLen   = 0;
        dPkt_SendPacket(&pns_data.packetHandler, ptPacket);
        fPacketCouldBeHandled = true;
        break;
      case PNS_IF_SAVE_IP_ADDR_IND:
          ptPacket->tHeader.ulCmd  |= CIFX_MSK_PACKET_ANSWER;
          ptPacket->tHeader.ulLen   = 0;
          dPkt_SendPacket(&pns_data.packetHandler, ptPacket);
          fPacketCouldBeHandled = true;
          break;
      case PNS_IF_AR_INDATA_IND:
          pns->isCyclicCommunicating = true;
          ptPacket->tHeader.ulCmd  |= CIFX_MSK_PACKET_ANSWER;
          ptPacket->tHeader.ulLen   = 4;
          dPkt_SendPacket(&pns_data.packetHandler, ptPacket);
          fPacketCouldBeHandled = true;
          break;
      case PNS_IF_APDU_STATUS_IND:;
          PNS_IF_APDU_STATUS_CHANGED_IND_T * res = (PNS_IF_APDU_STATUS_CHANGED_IND_T*)ptPacket;
          if(res->tData.ulStatus | 0x20)
            pns->isControllerConnected = true;
          ptPacket->tHeader.ulCmd  |= CIFX_MSK_PACKET_ANSWER;
          ptPacket->tHeader.ulLen   = 4;
          dPkt_SendPacket(&pns_data.packetHandler, ptPacket);
          fPacketCouldBeHandled = true;
          break;
      case PNS_IF_CONNECT_REQ_DONE_IND:
      case PNS_IF_AR_CHECK_IND:
        ptPacket->tHeader.ulCmd  |= CIFX_MSK_PACKET_ANSWER;
        ptPacket->tHeader.ulLen   = 4;
        dPkt_SendPacket(&pns_data.packetHandler, ptPacket);
        fPacketCouldBeHandled = true;
        break;
      case PNS_IF_READ_IM_IND:
      case PNS_IF_WRITE_IM_IND:
      case PNS_IF_AR_SET_STATUS_IND:
      case PNS_IF_RECONFIG_IND:
      case PNS_IF_AR_ABORT_IND:
      case PNS_IF_ALARM_IND:
      case PNS_IF_PARAM_BEGIN_IND:
      case PNS_IF_GET_ASSET_IND:
      case PNS_IF_USER_ERROR_IND:
      case PNS_IF_STORE_REMANENT_DATA_IND:
      case PNS_IF_PARAMET_SPEEDUP_SUPPORTED_IND:
        ptPacket->tHeader.ulCmd  |= CIFX_MSK_PACKET_ANSWER;
        ptPacket->tHeader.ulLen   = 0;
        ptPacket->tHeader.ulState = ERR_HIL_NO_APPLICATION_REGISTERED;
        (void)dPkt_SendPacket(&pns_data.packetHandler, ptPacket);
        fPacketCouldBeHandled = true;
        break;
      default:   // Disregarded indication packet received!;
        if((ptPacket->tHeader.ulCmd & 0x1) == 0)
        {

          ptPacket->tHeader.ulLen = 0;
          ptPacket->tHeader.ulState = ERR_HIL_UNKNOWN_COMMAND;
          ptPacket->tHeader.ulCmd |= 0x01; /* Make it a response */

          (void) dPkt_SendPacket(&pns->packetHandler, ptPacket);
          fPacketCouldBeHandled = true;
        }
        else
        { /* received a confirmation */
  //        printf("Warning: Disregarded confirmation packet received!");
        }
        break;
      } /*switch*/

      return fPacketCouldBeHandled;
}

static void assemblePnsConfigureCommand(PNS_CONFIGURATION_DATA_T * confData, CIFX_PACKET * packet)
{
    memset(packet, 0, sizeof(CIFX_PACKET));
    packet->tHeader.ulDest = HIL_PACKET_DEST_DEFAULT_CHANNEL;
    packet->tHeader.ulCmd = PNS_IF_SET_CONFIGURATION_REQ;
    packet->tHeader.ulLen = confData->tSetConfigReq.ulTotalConfigPckLen;
    memcpy(packet->abData, confData, packet->tHeader.ulLen);
}

/**************************************************************************************
 function:    ParameterEndInd
 description: Return ParameterEndResponse with application ready.

 global:      none
 input:       APP_DATA_T* ptAppData    - pointer to the packet

 output:      none
 return:      void
 **************************************************************************************/
static void pns_HandleParameterEndInd(CIFX_PACKET* ptPacket)
{
  PNS_IF_PARAM_END_RSP_T* ptParameterEndRes=(PNS_IF_PARAM_END_RSP_T*)ptPacket;
  PNS_IF_PARAM_END_IND_T* ptParameterEndInd=(PNS_IF_PARAM_END_IND_T*)ptPacket;

  if (0 == ptParameterEndInd->tData.usSubslot) {
 //   LogInfo(eLogTypePROFINET, false, "Controller parameterization complete for all submodules.");
  } else {
 //   LogInfo(eLogTypePROFINET, false, "Controller parameterization complete for submodule %lx.%x.%x.", ptParameterEndInd->tData.ulApi, ptParameterEndInd->tData.usSlot, ptParameterEndInd->tData.usSubslot);
  }

  ptParameterEndRes->tHead.ulCmd |= CIFX_MSK_PACKET_ANSWER; //PNS_IF_PARAM_END_RES;
  ptParameterEndRes->tHead.ulLen  = 8;
  ptParameterEndRes->tHead.ulSta  = SUCCESS_HIL_OK;
  ptParameterEndRes->tData.fSendApplicationReady = 1;

  (void)dPkt_SendPacket(&pns_data.packetHandler, ptPacket);

  return;
}

static int32_t pns_SetTriggerType()
{
      CIFX_PACKET Packet;
      CIFX_PACKET * ptPkt = &Packet;

      memset( &ptPkt->tHeader, 0, sizeof(ptPkt->tHeader));

      HIL_SET_TRIGGER_TYPE_REQ_DATA_T * sent = (HIL_SET_TRIGGER_TYPE_REQ_DATA_T *)ptPkt->abData;

      ptPkt->tHeader.ulDest = HIL_PACKET_DEST_DEFAULT_CHANNEL;
      ptPkt->tHeader.ulCmd = HIL_SET_TRIGGER_TYPE_REQ;
      ptPkt->tHeader.ulLen  = 6;
      sent->usPdInHskTriggerType = HIL_TRIGGER_TYPE_PDIN_RX_DATA_RECEIVED;
      sent->usPdOutHskTriggerType = HIL_TRIGGER_TYPE_PDOUT_READY_FOR_TX_DATA;
      sent->usSyncHskTriggerType = HIL_TRIGGER_TYPE_SYNC_TIMED_ACTIVATION;

      int32_t lRet = dPkt_SendReceivePacket(&pns_data.packetHandler, ptPkt);
      if(lRet != 0)
        return lRet;
      if(ptPkt->tHeader.ulState != 0)
        return lRet;
      return pns_GetTriggerType();
}

static int32_t pns_GetTriggerType()
{
    CIFX_PACKET Packet;
    CIFX_PACKET * ptPkt = &Packet;
    int32_t lRet;

    memset( &ptPkt->tHeader, 0, sizeof(ptPkt->tHeader));

    ptPkt->tHeader.ulDest = HIL_PACKET_DEST_DEFAULT_CHANNEL;
    ptPkt->tHeader.ulCmd  = HIL_GET_TRIGGER_TYPE_REQ;
    ptPkt->tHeader.ulLen  = 0;

    lRet = dPkt_SendReceivePacket(&pns_data.packetHandler, ptPkt);

    HIL_GET_TRIGGER_TYPE_CNF_DATA_T * resp = (HIL_GET_TRIGGER_TYPE_CNF_DATA_T *)ptPkt->abData;

    pns_data.TriggerPdin  = resp->usPdInHskTriggerType;
    pns_data.TriggerPdOut = resp->usPdOutHskTriggerType;
    pns_data.TriggerSync  = resp->usSyncHskTriggerType;
    pns_data.MinFreeRunUpdate = resp->usMinFreeRunUpdateInterval;

    return lRet;
}

static int32_t handleInputCyclicData()
{
  PNS_Cyclic_Offsets_t * inputOff = PNSC_GetInputCyclicOffsets();
  int32_t lRet;
  for(uint32_t i = 0; i < inputOff->totalSubmodules; i++)
  {
    lRet = xChannelIORead(pns_data.channelHandler, 0, 0, inputOff->Lengths[i], pns_data.inputDataBuffers[i], NO_WAIT_TIME);
    if(lRet == CIFX_NO_ERROR)
    {
      if(NULL != pns_data.pfCyclicInputdata)
      {
        pns_data.pfCyclicInputdata(i, pns_data.inputDataBuffers[i], inputOff->Lengths[i], pns_data.pCyclicInputUserData);
      }
    }
  }
  return lRet;
}

static int32_t handleOutputCyclicData()
{
  int32_t lRet;
  PNS_Cyclic_Offsets_t * outputOff = PNSC_GetOutputCyclicOffsets();
      for(uint32_t i = 0; i < outputOff->totalSubmodules ;i++)
      {
          /** update output data image to be sent in this cycle */
          if(NULL != pns_data.pfCyclicOutputdata)
          {
              pns_data.pfCyclicOutputdata(i,pns_data.outputDataBuffers[i], outputOff->Lengths[i], pns_data.pCyclicOutputUserData);
          }
          lRet = xChannelIOWrite(pns_data.channelHandler, 0, 0, outputOff->Lengths[i], pns_data.outputDataBuffers[i], NO_WAIT_TIME);
      }
      return lRet;
}

