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

#include "dPacketCommunication.h"

#include "cifxToolkit.h"
#include "Hil_Packet.h"
#include "OS_Dependent.h"
#include "cifxUser.h"

#include <string.h>

/* Timeout values for sending (TX) and receiving (RX) packets. */
#define TX_TIMEOUT       500 /* milliseconds */
#define RX_TIMEOUT         0 /* milliseconds */
#define TXRX_TIMEOUT     500 /* milliseconds */

/*****************************************************************************/
/*! PRIVATE FUNCTIONS                                                           */
/*****************************************************************************/

static bool Pkt_QueueConfirmationPacket(Pkt_Comm * me, CIFX_PACKET* ptPacket);

static bool Pkt_GetConfirmationPacketFromQueue(Pkt_Comm * me, CIFX_PACKET* ptPacket);

static bool Pkt_TryGetConfirmationPacketFrom(Pkt_Comm * me, uint32_t ulCmd, CIFX_PACKET* ptPacket);

static void Pkt_DispatchIndication(Pkt_Comm * me, CIFX_PACKET* ptPacket );

/*****************************************************************************/
/*! FUNCTIONS                                                                */
/*****************************************************************************/

void dPkt_Init(Pkt_Comm * me, Pkt_Comm_config * conf) {
    // Copy over the configuration values
    me->hChannel = conf->hChannel;
    me->fnIndHandler = conf->fnIndHandler;
    me->pvIndHandlerUserData = conf->pvIndHandlerUserData;
    me->setSucessHandler = conf->setSucessHandler;
    me->setFailHandler = conf->setFailHandler;
    me->getSucessHandler = conf->getSucessHandler;
    me->getFailHandler = conf->getFailHandler;

    // Initialize other fields
    me->s_ulNextPacketId = 1;  // Start packet ID from 1 (or whatever starting point is appropriate)
    me->ulPacketSendCountSuccess = 0;
    me->ulPacketSendCountError = 0;
    me->ulPacketReceiveCount = 0;

    // Initialize the unhandled packet queue
    for (int i = 0; i < 5; i++) {
        memset(me->unHandledPktQue, 0, 5 * sizeof(CIFX_PACKET)); // Ensure space cleared
        me->unHandledPktQueStatus[i] = false; // Mark each slot as unused
    }
}


uint32_t dPkt_SendPacket(Pkt_Comm * me, CIFX_PACKET* ptPacket)
{
  uint32_t lRet = CIFX_NO_ERROR;

  lRet = xChannelPutPacket(me->hChannel, ptPacket, TX_TIMEOUT);

  if(CIFX_NO_ERROR == lRet)
  {
    me->ulPacketSendCountSuccess++;
    if(me->setSucessHandler)
    {
      me->setSucessHandler(ptPacket);
    }
  }
  else
  {
    me->ulPacketSendCountError++;
    if(me->setSucessHandler)
    {
      me->setFailHandler(ptPacket);
    }
  }

  return lRet;
}


uint32_t dPkt_ReceivePacket(Pkt_Comm * me, CIFX_PACKET* ptPacket)
{
  uint32_t lRet = CIFX_NO_ERROR;

  lRet = xChannelGetPacket(me->hChannel, sizeof(*ptPacket), ptPacket, RX_TIMEOUT);

  if(CIFX_NO_ERROR == lRet)
  {
    if(me->getSucessHandler)
    {
      me->getSucessHandler(ptPacket);
    }
  }
  else
  {
    if(me->getFailHandler)
    {
      me->getFailHandler(ptPacket);
    }
  }

  return lRet;
}


uint32_t dPkt_SendReceivePacket( Pkt_Comm * me, CIFX_PACKET* ptPacket )
{
  uint32_t ulRet = CIFX_NO_ERROR;
  uint32_t ulCmd = ptPacket->tHeader.ulCmd;

  if(!(ptPacket->tHeader.ulExt & (HIL_PACKET_SEQ_MIDDLE | HIL_PACKET_SEQ_LAST)))
  {
    // increment PacketId
    me->s_ulNextPacketId++;
  }

  ptPacket->tHeader.ulId = me->s_ulNextPacketId;

  /* issue the request */
  ulRet = dPkt_SendPacket(me, ptPacket);

  if( CIFX_NO_ERROR == ulRet)
  {
    /* Sending the packet succeeded */

    /* Now, let's wait for the confirmation. All indications coming up in the meantime will be dispatched. */
    uint32_t ellapsedMs = 0;
    while(1)
    {
      ulRet = dPkt_ReceivePacket(me, ptPacket);

      if(CIFX_NO_ERROR == ulRet)
      {
        if((ptPacket->tHeader.ulCmd == (ulCmd | 0x1)) && (ptPacket->tHeader.ulId == me->s_ulNextPacketId))
        {
          /* This is the confirmation we were waiting for. */
          break;
        }
        else if(ptPacket->tHeader.ulCmd & 0x1)
        {
          /* This is a confirmation for another command we have issued, so put it into the receive confirmation queue for now. */
          if (!Pkt_QueueConfirmationPacket(me, ptPacket))
          {
               // Queue, overrun
               // ToDo Add callback?
               ulRet = -1;
               break;
          }
        }
        else
        {
        /* This is a received indication packet, we dispatch it */
          Pkt_DispatchIndication(me, ptPacket);
          /* Then look for our packet in the queue, not sure about this (when it was saved?)*/
          if(Pkt_TryGetConfirmationPacketFrom(me, (ulCmd | 0x1), ptPacket))
          {
            /* The confirmation we are waiting for was queued up during indication handling, we're fine. */
            break;
          }
        }
      }
      else if(CIFX_DEV_GET_NO_PACKET == ulRet)
      {
        /* No packet within the given timeout received. */
        OS_Sleep(1);
        ellapsedMs++;
        if(ellapsedMs >= TXRX_TIMEOUT)
        {
           break;
        }
      }
    }
  }
  return ulRet;
}


uint32_t dPkt_CheckReceiveMailbox(Pkt_Comm * me, CIFX_PACKET* ptPacket )
{
  uint32_t ulRet = CIFX_NO_ERROR;

  /* First, let's see if there are some unprocessed confirmations in our queue. */
  if( false == Pkt_GetConfirmationPacketFromQueue(me, ptPacket))
  {
    /* No packets in the confirmation queue --> check the receive mailbox. */
    ulRet = dPkt_ReceivePacket(me, ptPacket);
  }
  if( CIFX_NO_ERROR == ulRet)
  {
    // If we got a queued pkt or just received on we dispach it.
    Pkt_DispatchIndication(me, ptPacket);
  }

  return ulRet;
}

/**************************************************************************************/
///*! Put a packet into the confirmation queue
//*
//*   \param ptPacket   [in]  Packet to be put into the queue
//*
//*   \return true:  Packet could be queued
//*           false: Packet could not be queued
//*/
/**************************************************************************************/
static bool Pkt_QueueConfirmationPacket(Pkt_Comm * me, CIFX_PACKET* ptPacket )
{
  int i;
  for(i = 0; i < 5; i++)
  {
    if(me->unHandledPktQueStatus[i] == false)
    {
      me->unHandledPktQueStatus[i] = true;
      memcpy(&me->unHandledPktQue[i], ptPacket, sizeof(CIFX_PACKET));
      return true;
    }
  }

  return false;
}

/**************************************************************************************/
///*! Get a packet from the confirmation queue
//*
//* Gets the first unhandled packet from the queue
//*
//*   \param ptPacket [out]  Packet dequeued from the confirmation queue.
//*
//*   \return true:  Packet could be queued
//*           false: Packet could not be queued
//*/
/**************************************************************************************/
static bool Pkt_GetConfirmationPacketFromQueue(Pkt_Comm * me, CIFX_PACKET* ptPacket)
{
  int i;
  for(i = 0; i < 5; i++)
  {
    if(me->unHandledPktQueStatus[i] == true)
    {
      ptPacket = &me->unHandledPktQue[i];
      me->unHandledPktQueStatus[i] = false;
      return true;
    }
  }
  return false;
}

/**************************************************************************************/
///*! Get a specific packet (with matching ulCmd and ulId) from the confirmation queue.
// *
// *   \param ulCmd    [in]   ulCmd value of the wanted packet
// *   \param ulId     [in]   ulId value  of the wanted packet
// *   \param ptPacket [out]  Packet dequeued from the confirmation queue.
// *
// *   \return true:  Packet was found
// *           false: Packet could not be found
// */
/**************************************************************************************/
static bool Pkt_TryGetConfirmationPacketFrom(Pkt_Comm * me, uint32_t ulCmd, CIFX_PACKET* ptPacket)
{
  int i;
  for(i = 0; i < 5; i++)
  {
    if((me->unHandledPktQueStatus[i] == true) && (me->unHandledPktQue[i].tHeader.ulCmd == ulCmd)
      && (me->unHandledPktQue[i].tHeader.ulId == me->s_ulNextPacketId))
    {
      ptPacket = &me->unHandledPktQue[i];
      me->unHandledPktQueStatus[i] = false;
      return true;
    }
  }
  return false;
}

///**************************************************************************************/
///*! Dispatch a packet to the registered indication handler
// *
// *   \param ptPacket [out]  Packet that shall be dispatched
// *
// *   \return true:  Packet could be dispatched
// *           false: Packet could not be dispatched (no registered indication handler available)
// */
///**************************************************************************************/
static void Pkt_DispatchIndication(Pkt_Comm * me, CIFX_PACKET* ptPacket )
{
  /* Dispatch this indication */
  if(me->fnIndHandler)
  {
    if(!me->fnIndHandler(ptPacket, me->pvIndHandlerUserData))
    {
      ptPacket->tHeader.ulCmd |= 0x01; /* Make it a response */
      ptPacket->tHeader.ulLen = 0;
      ptPacket->tHeader.ulState = CIFX_INVALID_COMMAND;
      dPkt_SendPacket(me, ptPacket);
    }
  }
}
