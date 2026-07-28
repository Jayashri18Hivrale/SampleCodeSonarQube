#include "dspiMaster.h"

void dSpiM_Init(pPlatform_dSpiM_t platform, dSpiM_conf_t* conf)
{
	pDspiM_t self = &platform->super;
	cb_init(&self->rxCB, conf->rxBuffer, conf->rxSize);
	cb_init(&self->txCB, conf->txBuffer, conf->txSize);
	cb_init(&self->txCB, conf->txBuffer, conf->txSize);
	self->vTable = conf->vTable;
    self->completeCallback = 0;
    self->completeCallbackUserArgs = 0;
    self->vTable->EnableTxFifoLevelIrq(platform, false);
    self->vTable->EnableRxByteReceivedIrq(platform, true);
}

void dSpiM_Write(pDspiM_t self, uint8_t* data, size_t length)
{
    if (data == NULL)
    {
        return;
    }
    // Disable ISR to avoid race condition from another call of this function within the ISR
    self->vTable->EnableAllInterrupts((pPlatform_dSpiM_t)self, false);
    for (uint32_t i = 0; i < length; i++)
    {
        if (!cb_write(&self->txCB, data[i]))
        {
            // ToDO add overrun error handling
            return;
        }
    }
    if (!cb_is_empty(&self->txCB))
    {
        self->vTable->EnableTxFifoLevelIrq((pPlatform_dSpiM_t)self, true);
        self->vTable->EnableAllInterrupts((pPlatform_dSpiM_t)self, true);
    }
    return;
}

void dSpiM_SetTransactionCompleteCallback(pDspiM_t self, dSpiM_callback callback, void* userArgs)
{
    self->completeCallback = callback;
    self->completeCallbackUserArgs = userArgs;
}

void dSpiM_SetReceivedByteCallback(pDspiM_t self, dSpiM_callback callback, void* userArgs)
{
    self->receivedByteCallback = callback;
    self->receivedByteCallbackUserArgs = userArgs;
}

bool dSpiM_GetRxData(pDspiM_t self, uint8_t* recData)
{
    return cb_read(&self->txCB, recData);
}

void dSpiM_HandleTxFifoLevelIrq(pPlatform_dSpiM_t platform, uint8_t * pSpiDataRegister)
{
    uint8_t data;
    pDspiM_t self = &platform->super;
    while (!self->vTable->isTxFifoFull(platform) && cb_read(&self->txCB, &data))
    {
        // Write data to the SPI data register
        *pSpiDataRegister = data;
    }
    // If the circular buffer is empty, disable the FIFO watermark interrupt
    if (cb_is_empty(&self->txCB))
    {
        self->vTable->EnableTxFifoLevelIrq(platform, false);
    }
}

void dSpiM_HandleRxByteReceived(pPlatform_dSpiM_t platform, uint8_t* pSpiDataRegister)
{
    pDspiM_t self = &platform->super;
    while(!self->vTable->isRxFifoEmpty(platform))
    {
        if (!cb_write(&self->txCB, *pSpiDataRegister))
        {
            // handle data overflow
            self->vTable->EnableRxByteReceivedIrq(platform, false);
        }
    }
    if (self->receivedByteCallback)
    {
        self->receivedByteCallback(self, self->receivedByteCallbackUserArgs);
    }
    if (!self->vTable->isSpiBusy(platform))
    {
        if (self->completeCallback)
        {
            self->completeCallback(self, self->completeCallbackUserArgs);
        }
    }
}

