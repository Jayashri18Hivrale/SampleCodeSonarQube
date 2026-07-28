#include "platform_FieldBus.h"

#include "dPacketComm_helper.h"
#include "has3_pn_config.h"
#include "libc_support.h"
#include "nx90_pns.h"
#include "nx90_socket.h"

static void inputCyclicHandler(uint32_t submodule, uint8_t * data, uint32_t dataLength, void * userData);
static void outputCyclicHandler(uint32_t submodule, uint8_t * data, uint32_t dataLength, void * userData);

static void pns_sendSucess(CIFX_PACKET* ptPacket);
static void pns_receiveSucess(CIFX_PACKET* ptPacket);

static void debugFunction(uint8_t * data, uint32_t dataLength);

static void handleCyclicTest(has3_inputCyclicData * newDataIn, has3_outputCyclicData * dataOut);


uint32_t fieldBus_Init(N90Cifx_t * cifxDriver,  has3_cyclicData_t * has3Data)
{
    nx90PNS_config_t pns_conf;

    pns_conf.pfCyclicInputdata = inputCyclicHandler;
    pns_conf.pCyclicInputUserData = &has3Data;
    pns_conf.pfCyclicOutputdata = outputCyclicHandler;
    pns_conf.pCyclicOutputUserData = &has3Data;

    pns_conf.SendSucessCB = pns_sendSucess;
    pns_conf.RecSucessCB = pns_receiveSucess;

    pns_conf.debugCB = debugFunction;

    nx90PNS_Init(cifxDriver, &pns_conf);

    return nx90PNS_ConfigureStack(configureHas3SlavePN());
}


void fieldBus_Tasks(void)
{
    nx90PNS_CommStackTasks();
    nx90PNS_CyclicTasks();
}


static void debugFunction(uint8_t * data, uint32_t dataLength)
{
  printf("Count: %d",*((int*)data));
}

static void pns_sendSucess(CIFX_PACKET* ptPacket)
{
     printf("========================================================\r\n");
     printf("Sent packet:\r\n");
     Pkt_DumpPacket(ptPacket);
     printf("========================================================\r\n");
}

static void pns_receiveSucess(CIFX_PACKET* ptPacket)
{
      printf("========================================================\r\n");
      printf("Received packet:\r\n");
      Pkt_DumpPacket(ptPacket);
      printf("========================================================\r\n");
}


static void inputCyclicHandler(uint32_t submodule, uint8_t * data, uint32_t dataLength, void * userData)
{
  has3_cyclicData_t* pHasData = (has3_cyclicData_t *)userData;
  has3_inputCyclicData * in = (has3_inputCyclicData *) data;

  handleCyclicTest(in, &pHasData->out);

}

static void outputCyclicHandler(uint32_t submodule, uint8_t * data, uint32_t dataLength, void * userData)
{
  has3_cyclicData_t* pHasData = (has3_cyclicData_t *)userData;
  has3_outputCyclicData * out = (has3_outputCyclicData *)data;
  out->header = pHasData->out.header;
  out->elementStatus = pHasData->out.elementStatus;
}


static void handleCyclicTest(has3_inputCyclicData * newDataIn, has3_outputCyclicData * dataOut)
{
  static int currentReadCount = 0;
  static int lastTimeTestCount;
  static uint16_t prevTime;
    if(newDataIn->command == 0xA0)
    {
      if(prevTime != newDataIn->registerValue)
      {
        currentReadCount++;
        prevTime = newDataIn->registerValue;
      }
    }else
    {
      if(currentReadCount != 0)
      {
        lastTimeTestCount = currentReadCount;
        currentReadCount = 0;
      }
    }
    if(newDataIn->command == 0xA1)
    {
      dataOut->header = newDataIn->command;
      dataOut->elementStatus = lastTimeTestCount;
    }
}
