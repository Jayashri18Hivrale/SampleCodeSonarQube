/**************************************************************************//**
 * @file    main.c
 * @brief   Main program body
 * $Revision:  $
 * $Date:  $
 * \copyright
 ******************************************************************************/

#include "main.h"

#include "netx90_dspim.h"

nx90_dspim_t spiTest;

pDspiM_t pSpiTest= (pDspiM_t)&spiTest;

static void CompleteTransferCB(pDspiM_t self, void* arg);

uint8_t data[14] = {0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE};

uint8_t recData[14];

int asyncErrorCount = 0;

int completeErorCount = 0;
/*!
 * \brief The main of the template project.
 *
 * Contains an infinity loop. In case it breaks, the function will return
 * properly.
 * \return Will always return 0.
 */
int main(void)
{
  nx90_spim_conf conf = {0};
  conf.interruptPriority = 1;
  conf.netx90Spi = Netx90SPI1;
  conf.speed = NX90_SPI_FREQUENCY_1_56MHz; //NX90_SPI_FREQUENCY_6_25MHz; //NX90_SPI_FREQUENCY_1_56MHz;
  conf.spiMode = NX90_SPI_MODE_3;
  conf.loopbackMode = NX90_SPIM_LOOPBACK_ENABLED;
  conf.hardwareChipEnable = NX90_SPI_CHIP_ENABLE_0;

  Nx90Dspim_Init(&spiTest, &conf);

  dSpiM_SetTransactionCompleteCallback(pSpiTest, CompleteTransferCB, 0);

 dSpiM_Write(pSpiTest, data , sizeof(data));

  while(1)
  {
    // just waiting for Tx Events
  }

  return 0;
}


void CompleteTransferCB(pDspiM_t self, void* arg)
{
  uint8_t dummy;
  for(int i = 0; i < sizeof(data); i++)  // read all received buffer
  {
    if(dSpiM_GetRxData(pSpiTest, &recData[i]))
    {
      if(recData[i] != data[i])
      {
        asyncErrorCount++;
        while(dSpiM_GetRxData(pSpiTest, &dummy))
        {
        }
      }
    }
    else
    {
      completeErorCount++;
      while(dSpiM_GetRxData(pSpiTest, &dummy))
      {
      }
    }
  }
  dSpiM_Write(pSpiTest, data , sizeof(data)); // Write again
}
