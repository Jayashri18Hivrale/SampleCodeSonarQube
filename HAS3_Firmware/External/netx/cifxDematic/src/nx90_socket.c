/*
 * nx90_socket.c
 *
 *  Created on: Nov 12, 2024
 *      Author: A0084817
 */
#include "nx90_socket.h"

#include <stdbool.h>
#include <string.h>

#include "HIL_results.h"
#include "nx90_SystemPackets.h"
#include "SOCK_API.h"

typedef struct
{
  SOCK_H socket;
  uint16_t port;
  uint16_t length;
  int32_t status;
  socketData pfReceivedCallback;
  nx90Socket_data_t recData;
}nx90_Socket_t;

typedef struct{
  CIFXHANDLE channelHandler;
  Pkt_Comm packetHandler;
  nx90_Socket_t sockets[NX90_MAX_SOCKETS];
  uint32_t initSocketCount;
  bool isInitialized;
}_nx90_socket_t;

static _nx90_socket_t socket_data = {0};

static bool  AppSock_PacketHandler( CIFX_PACKET* ptPacket,  void* pvUserData);

static int32_t nx90Socket_UdpStartListening(nx90_Socket_t * socket);

int32_t nx90Socket_Init(CIFXHANDLE initializedChannelHandler)
{
  if(NULL == initializedChannelHandler)
    return -1;
 CIFX_PACKET Packet;
 Pkt_Comm_config packet_conf = { 0 };
 socket_data.channelHandler = initializedChannelHandler;
 packet_conf.fnIndHandler = AppSock_PacketHandler;
 packet_conf.pvIndHandlerUserData = &socket_data;
 packet_conf.hChannel = socket_data.channelHandler;
 dPkt_Init(&socket_data.packetHandler, &packet_conf);
 nx90_SysPkt_AssembleRegisterAppReq(&Packet);
 int32_t lRet = dPkt_SendReceivePacket(&socket_data.packetHandler, &Packet);
 if(lRet == CIFX_NO_ERROR)
 {
   socket_data.isInitialized = true;
 }
 return lRet;
}

int32_t nx90Socket_Tasks()
{
  CIFX_PACKET Packet = {  { 0 }  };
  int32_t lRet = dPkt_CheckReceiveMailbox(&socket_data.packetHandler, &Packet);
  if( CIFX_DEV_GET_NO_PACKET == lRet || CIFX_DEV_NOT_READY == lRet)
  {
    lRet = CIFX_NO_ERROR;
  }
  return lRet;
}

nx90_hSocket nx90Socket_CreateUDPServerSocket(uint16_t port, uint16_t length, socketData receivedCallback)
{
    nx90_Socket_t * socket = (nx90_Socket_t *)nx90Socket_CreateUdpSocket();
    if(NULL == socket)
    {
      return NULL;
    }

    socket->port = port;
    socket->length = length;
    socket->pfReceivedCallback = receivedCallback;

    CIFX_PACKET Packet = { { 0 } };
    SOCK_BIND_REQ_T* ptSocketBind = (SOCK_BIND_REQ_T*) &Packet;

    ptSocketBind->tHead.ulCmd = SOCK_CMD_BIND_REQ;
    ptSocketBind->tHead.ulLen = sizeof(SOCK_ADDR_DATA_T);

    ptSocketBind->tData.hSocket = socket->socket;
    ptSocketBind->tData.tSa.tIp.bFamily = SOCK_AF_INET; // SOCK_AF_NET
    ptSocketBind->tData.tSa.tIp.usPort = socket->port;
    int32_t lRet = dPkt_SendReceivePacket(&socket_data.packetHandler, &Packet);
    if(CIFX_NO_ERROR != lRet)
    {
      return NULL;
    }
    if(CIFX_NO_ERROR != nx90Socket_UdpStartListening(socket))
    {
      return NULL;
    }
    return (uint32_t *) socket;
}

nx90_hSocket nx90Socket_CreateUdpSocket()
{
  if(socket_data.initSocketCount >= NX90_MAX_SOCKETS)
    return 0;

  nx90_Socket_t * ret = &socket_data.sockets[socket_data.initSocketCount];
  socket_data.initSocketCount ++;
  CIFX_PACKET Packet = { { 0 } };
  SOCK_SOCKET_REQ_T* ptSocketReq = (SOCK_SOCKET_REQ_T*) &Packet;
  ptSocketReq->tHead.ulCmd = SOCK_CMD_SOCKET_REQ;
  ptSocketReq->tHead.ulLen = sizeof(SOCK_SOCKET_REQ_DATA_T);

  ptSocketReq->tData.ulDomain = SOCK_AF_INET;
  ptSocketReq->tData.ulType = SOCK_SOCKET_DGRAM;
  ptSocketReq->tData.ulProtocol = SOCK_IPPROTO_UDP;
  int32_t lRet = dPkt_SendReceivePacket(&socket_data.packetHandler, &Packet);
  if(CIFX_NO_ERROR != lRet  || CIFX_NO_ERROR != Packet.tHeader.ulState)
  {
    return 0;
  }
  SOCK_SOCKET_CNF_DATA_T* ptSocketCnf = (SOCK_SOCKET_CNF_DATA_T*)&(Packet.abData);
  ret->socket = ptSocketCnf->hSocket;
  return (nx90_hSocket)ret;
}

int32_t nx90Socket_SendData(nx90_hSocket _socket, nx90Socket_data_t * sentData)
{
  nx90_Socket_t * socket = (nx90_Socket_t *)_socket;
  if(socket->length < sentData->length)
    return -1;
  CIFX_PACKET Packet = { { 0 } };
  SOCK_SENDTO_REQ_T * ptSockRecTo = (SOCK_SENDTO_REQ_T *)&Packet;
  ptSockRecTo->tHead.ulCmd = SOCK_CMD_SENDTO_REQ;
  ptSockRecTo->tHead.ulLen = 0x14 + sentData->length;
  ptSockRecTo->tHead.ulId = (uint32_t)socket;

  ptSockRecTo->tData.hSocket = socket->socket;
  ptSockRecTo->tData.tSa.tCommon.bFamily = SOCK_AF_INET;
  ptSockRecTo->tData.tSa.tIp.bFamily = SOCK_AF_INET;
  ptSockRecTo->tData.tSa.tIp.ulIpAddr = sentData->ip;
  ptSockRecTo->tData.tSa.tIp.usPort = sentData->port;
  memcpy(ptSockRecTo->tData.abBuffer, sentData->data, sentData->length);

  int32_t lRet = dPkt_SendReceivePacket(&socket_data.packetHandler, &Packet);
  if(CIFX_NO_ERROR != lRet)
  {
    return lRet;
  }
  return Packet.tHeader.ulState;
}

static bool  AppSock_PacketHandler( CIFX_PACKET* ptPacket,  void* pvUserData)
{
  bool fPacketCouldBeHandled = false;
  switch(ptPacket->tHeader.ulCmd )
  {
    case SOCK_CMD_RECVFROM_CNF:
      if(SUCCESS_HIL_OK == ptPacket->tHeader.ulState)
      {
        SOCK_RECVFROM_CNF_T* ptSockRecFrom = (SOCK_RECVFROM_CNF_T*) ptPacket;
        nx90_Socket_t * socket = (nx90_Socket_t *)ptSockRecFrom->tHead.ulId;
        socket->recData.ip = ptSockRecFrom->tData.tSa.tIp.ulIpAddr;
        socket->recData.port = ptSockRecFrom->tData.tSa.tIp.usPort;
        socket->recData.data = ptSockRecFrom->tData.abPayload;
        socket->recData.length = ptSockRecFrom->tHead.ulLen - 0x10;
        if(NULL != socket->pfReceivedCallback)
        {
          socket->pfReceivedCallback((nx90_hSocket)socket, &socket->recData);
        }
        nx90Socket_UdpStartListening(socket);
        fPacketCouldBeHandled = true;
      }
      break;
    default:
      break;
  }
  return fPacketCouldBeHandled;
}

static int32_t nx90Socket_UdpStartListening(nx90_Socket_t * socket)
{
  CIFX_PACKET Packet = { { 0 } };
  SOCK_RECVFROM_REQ_T* ptSockRecFrom = (SOCK_RECVFROM_REQ_T*) &Packet;

  ptSockRecFrom->tHead.ulCmd = SOCK_CMD_RECVFROM_REQ;
  ptSockRecFrom->tHead.ulLen = 6;
  ptSockRecFrom->tHead.ulId = (uint32_t)socket;

  ptSockRecFrom->tData.hSocket = socket->socket;
  ptSockRecFrom->tData.usMaxLen = socket->length;

  return dPkt_SendPacket(&socket_data.packetHandler, &Packet);
}
