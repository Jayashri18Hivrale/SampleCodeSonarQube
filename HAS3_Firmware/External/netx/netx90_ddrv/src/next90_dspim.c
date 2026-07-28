#include "netx90_dspim.h"

#define NETX90_SPI_0 spi0_app
#define NETX90_SPI_1 spi1_app
#define NETX90_SPI_2 spi2_app

static void setBaseRegister(nx90_dspim_t* self);
static IRQn_Type GetSpiIRQ(nx90_dspi spi);
static void nxt90_dspim_irq(nx90_dspim_t* self);

static void enableInterrupts(pPlatform_dSpiM_t platform, bool isEnabled);
static void EnableFifoLevelIrq(pPlatform_dSpiM_t platform, bool isEnabled);
static void enableRxByteReceivedIrq(pPlatform_dSpiM_t platform, bool isEnabled);
static bool isTxFifoFull(pPlatform_dSpiM_t platform);
static bool isRxFifoempty(pPlatform_dSpiM_t platform);
static bool isSpiBusy(pPlatform_dSpiM_t platform);

const dSpiM_vTable_t nx90_dspim_vtable = { enableInterrupts, EnableFifoLevelIrq, enableRxByteReceivedIrq, isTxFifoFull, isRxFifoempty, isSpiBusy };
nx90_dspim_t* netx90SpiM[3] = { 0 };

void Nx90Dspim_Init(nx90_dspim_t* self, nx90_spim_conf* conf)
{
    self->speed = conf->speed;
    self->spiMode = conf->spiMode;
    self->spi = conf->netx90Spi;
    setBaseRegister(self);
    // Set default values in configuration registers
    self->spiRegister->spi_cr0 = 0x00080007;
    /* The 0x00080007 configuration sets up the SPI as follows:
         - Motorola SPI frame format with 8-bit transfers.
         - Mode 0 - Clock idle state is low, and data is sampled on the first clock edge.
         - SPI clock is set to 50 MHz (sck_muladd = 0x800).
         - Input filtering and advanced timing features are disabled.
    */
    self->spiRegister->spi_cr1 = 0x00080000;
    /*The configuration spi_cr1 = 0x00080000 sets up the SPI module with the following parameters:
    FIFO Watermarks:
        - Receive FIFO watermark is set to 0, generating an interrupt when the FIFO contains 0 or more items (every received byte).
        - Transmit FIFO watermark is set to 8, generating an interrupt when the FIFO contains 8 or fewer items.
    Chip-Select Behavior:
        - Automatic toggling of SPI chip-select (fss_static = 0), with no active external chip-select signals (fss = 000).
    Mode:
        - Configured as a master SPI device (MS = 0), with the module initially disabled (SSE = 0).
    Loopback:
        - Disabled (LBM = 0).
    Slave Mode Output:
        -MISO can be driven if the device operates in slave mode (SOD = 0).
    */
    self->spiRegister->spi_imsc = 0x00000000;
    self->spiRegister->spi_icr = (unsigned)-1;
    self->spiRegister->spi_dmacr = 0x00000000;
    // Set Speed 
    self->spiRegister->spi_cr0_b.sck_muladd = conf->speed;
    // Set Mode
    self->spiRegister->spi_cr0_b.SPH = (conf->spiMode & 0x1);
    self->spiRegister->spi_cr0_b.SPO = (conf->spiMode & 0x2) >> 1;
    // Set loopBack
    if(conf->loopbackMode == NX90_SPIM_LOOPBACK_ENABLED)
    {
      self->spiRegister->spi_cr1_b.LBM = 1;
      self->spiRegister->spi_cr0_b.filter_in = 1;
    }
    // Set chip-select
      self->spiRegister->spi_cr1_b.fss = conf->hardwareChipEnable;
    // Configure Interrupts
    IRQn_Type irq = GetSpiIRQ(self->spi);
    NVIC_DisableIRQ(irq);
    NVIC_ClearPendingIRQ(irq);
    NVIC_SetPriority(irq, conf->interruptPriority);
    self->spiRegister->spi_imsc_b.RXIM = 1;
    // Configure abstract dSPI
    dSpiM_conf_t config = { 0 };
    config.rxBuffer = self->RxBuffer;
    config.rxSize = sizeof(self->RxBuffer);
    config.txBuffer = self->TxBuffer;
    config.txSize = sizeof(self->TxBuffer);
    config.vTable = &nx90_dspim_vtable;
    dSpiM_Init((pPlatform_dSpiM_t)self, &config);
    // Enable peripheral and Enable interrupts
    self->spiRegister->spi_cr1_b.SSE = 1;
    NVIC_EnableIRQ(irq);
}

void dSPI0_IRQHandler()
{
    nxt90_dspim_irq(netx90SpiM[0]);
}

void dSPI1_IRQHandler()
{
    nxt90_dspim_irq(netx90SpiM[1]);
}

void dSPI2_IRQHandler()
{
    nxt90_dspim_irq(netx90SpiM[2]);
}

static void setBaseRegister(nx90_dspim_t* self)
{
    switch (self->spi)
    {
    case Netx90SPI0:
        self->spiRegister = (spi0_app_Type*)NETX90_SPI_0;
        netx90SpiM[0] = self;
        return;
    case Netx90SPI1:
        self->spiRegister = (spi0_app_Type*)NETX90_SPI_1;
        netx90SpiM[1] = self;
        return;
    case Netx90SPI2:
        self->spiRegister = (spi0_app_Type*)NETX90_SPI_2;
        netx90SpiM[2] = self;
        return;
    }
}

static IRQn_Type GetSpiIRQ(nx90_dspi spi)
{
    switch (spi)
    {
    case Netx90SPI0:
        return spi0_app_IRQn;
    case Netx90SPI1:
        return spi1_app_IRQn;
    case Netx90SPI2:
        return spi2_app_IRQn;
    default:
        return spi0_app_IRQn;
    }
}

static void enableInterrupts(pPlatform_dSpiM_t platform, bool isEnabled)
{
    if (isEnabled)
        __enable_irq();
    else
        __disable_irq();
}

static void EnableFifoLevelIrq(pPlatform_dSpiM_t platform, bool isEnabled)
{
    nx90_dspim_t* self = (nx90_dspim_t*)platform;
    self->spiRegister->spi_imsc_b.TXIM = isEnabled;
}

static void enableRxByteReceivedIrq(pPlatform_dSpiM_t platform, bool isEnabled)
{
    nx90_dspim_t* self = (nx90_dspim_t*)platform;
    self->spiRegister->spi_imsc_b.RXIM = isEnabled;
}

static bool isTxFifoFull(pPlatform_dSpiM_t platform)
{
  nx90_dspim_t* self = (nx90_dspim_t*)platform;
  return !((bool)self->spiRegister->spi_sr_b.TNF);
}


static bool isRxFifoempty(pPlatform_dSpiM_t platform)
{
    nx90_dspim_t* self = (nx90_dspim_t*)platform;
    return !((bool)self->spiRegister->spi_sr_b.RNE);
}

static bool isSpiBusy(pPlatform_dSpiM_t platform)
{
    nx90_dspim_t* self = (nx90_dspim_t*)platform;
    return self->spiRegister->spi_sr_b.BSY;
}

static void nxt90_dspim_irq(nx90_dspim_t* self)
{ 
    if (!self)
        return;
    if (self->spiRegister->spi_mis_b.RXMIS)
    {
        dSpiM_HandleRxByteReceived((pPlatform_dSpiM_t)self, (uint8_t*)&self->spiRegister->spi_dr);
        self->spiRegister->spi_icr_b.RXIC = 1;
    }
    if (self->spiRegister->spi_mis_b.TXMIS)
    {
        dSpiM_HandleTxFifoLevelIrq((pPlatform_dSpiM_t)self, (uint8_t*)&self->spiRegister->spi_dr);
        self->spiRegister->spi_icr_b.TXIC = 1;
    }
}
