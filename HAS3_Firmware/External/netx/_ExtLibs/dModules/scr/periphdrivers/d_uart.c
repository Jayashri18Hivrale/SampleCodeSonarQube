#include "d_uart.h"

#include <assert.h>
#include <stddef.h>

const char* duartStringErrors[] = { "rx Buffer Overflow", "tx Buffer Overflow", "tx Irq Unhandled", "peripheral Error" };

/**
 * @brief Initializes a platform specific duart.
 *
 * This function should be used by platform specific implementations.
 *
 * @param platform The platform specific duart instance to initialize.
 * @param conf The configuration for the duart.
 */
void duart_Init(pPlatform_duart_t platform, duart_conf_t* conf)
{
    pDuart_t me = &platform->super;
    _duart_t* _me = &me->privateData;

    cb_init(&me->rxCB, conf->rxBuffer, conf->txSize);
    cb_init(&me->txCB, conf->txBuffer, conf->txSize);
    me->userData = conf->userData;
    *_me = _duart_t_INIT;
    _me->vtable = conf->vTable;
    _me->rx_dataReceivedUserCallback = conf->rx_dataReceivedUserCallback;
    _me->enableTxIsr = conf->enableTxIsr;
    _me->enableIsr = conf->enableIsr;
    _me->isInitialized = true;
}

/**
 * @brief Checks if there is data available in the RX buffer of a duart instance.
 * @param me The duart instance.
 * @return True if data is available, false otherwise.
 */
bool duart_isRxDataAvailable(pDuart_t me)
{
    return !cb_is_empty(&me->rxCB);
}

/**
 * @brief Reads data from a duart instance.
 * @param me The duart instance to read from.
 * @param data The buffer to store the read data.
 * @param length The number of bytes to read.
 * @return The number of bytes actually read.
 */
uint32_t duart_read(pDuart_t me, uint8_t* data, uint32_t length)
{
    for (uint32_t i = 0; i < length; i++)
    {
        if (!cb_read(&me->rxCB, &data[i]))
        {
            return i;
        }
    }
    return length;
}

/**
 * @brief performs recurrent tasks for a duart instance.
 *
 * All platform dependent callbacks are triggered from this function.
 *
 * @param me The duart instance to handle tasks for.
 */
void duart_tasks(pDuart_t me)
{
    _duart_t* _me = &me->privateData;

    _me->vtable->Tasks((pPlatform_duart_t)me); // execute configured tasks configured per platform
    if (_me->isRxOverrun)
    {
        _me->isRxOverrun = false;
        duart_raiseErrorCallback((pPlatform_duart_t)me, rxBufferOverflow);
    }
    if (_me->isTxOverrun)
    {
        _me->isTxOverrun = false;
        duart_raiseErrorCallback((pPlatform_duart_t)me, txBufferOverflow);
    }
    if (_me->isTxIrqUnhandled)
    {
        _me->isTxIrqUnhandled = false;
        duart_raiseErrorCallback((pPlatform_duart_t)me, txIrqUnhandled);
    }
}

/**
 * @brief Reads a byte from a duart instance.
 * @param me The duart instance to read from.
 * @return The read byte.
 */
uint8_t duart_readByte(pDuart_t me)
{
    uint8_t byte;
    uint32_t readBytes = duart_read(me, &byte, 1);
    assert(readBytes == 1);
    return byte;
}

/**
 * @brief Writes data to a duart instance.
 * @param me The duart instance to write to.
 * @param data The data to write.
 * @param length The number of bytes to write.
 */
void duart_write(pDuart_t me, uint8_t* data, uint32_t length)
{
    _duart_t* _me = &me->privateData;

    // Disable ISR to avoid race condition from another call of this function within the ISR
    if (_me->enableIsr)
    {
        _me->enableIsr(false);
    }

    for (uint32_t i = 0; i < length; i++)
    {
        if (!cb_write(&me->txCB, data[i]))
        {
            _me->isTxOverrun = true;
            return;
        }
        _me->enableTxIsr(true);
    }

    // Enable ISR
    if (_me->enableIsr)
    {
        _me->enableIsr(true);
    }
    return;
}

uint32_t duart_TxQueueSpaceAvailable(pDuart_t me)
{
    return cb_space_available(&me->txCB);
}

/**
 * @brief Writes a byte to a duart instance.
 * @param me The duart instance to write to.
 * @param data The byte to write.
 */
void duart_writeByte(pDuart_t me, uint8_t data)
{
    duart_write(me, &data, 1);
}

/**
 * @brief Callback function for RX interrupt.
 *
 * To be placed within peripheral irq handling of a platform duart instance.
 *
 * @param platform The platform duart instance to handle the Rx callback for.
 * @param received_byte The received byte.
 */
void duart_rx_irq_callback(pPlatform_duart_t platform, uint8_t received_byte)
{
    pDuart_t me = &platform->super;
    _duart_t* _me = &me->privateData;

    if (!cb_write(&me->rxCB, received_byte))
    {
        _me->isRxOverrun = true;
        return;
    }
    if (_me->rx_dataReceivedUserCallback)
    {
        _me->rx_dataReceivedUserCallback((pDuart_t)platform, _me->rx_dataArgs);
    }
}

/**
 * @brief Callback function for TX interrupt.
 *
 * To be placed within the peripheral irq handling of a platform duart instance.
 *
 * @param platform The platform duart instance to handle the Tx callback for.
 * @return The byte to transmit.
 */
uint8_t duart_tx_irq_callback(pPlatform_duart_t platform)
{
    pDuart_t me = &platform->super;
    _duart_t* _me = &me->privateData;
    uint8_t txByte;
    if (!cb_read(&me->txCB, &txByte))
    {
        _me->isTxIrqUnhandled = true; // data available in cb to transmit
    }
    if (cb_is_empty(&me->txCB))
    {
        _me->enableTxIsr(false);
    }
    return txByte;
}

/**
 * @brief Returns the TX status of a duart instance.
 *
 * @param me The duart instance to check.
 * @return true if there is data pending to be sent, false otherwise.
 */
bool duart_isTxDataPending(pDuart_t me)
{
    return !cb_is_empty(&me->txCB);
}

/**
 * @brief Registers a callback function to handle errors of a duart instance.
 *
 * This function assigns a user-defined error callback function to the duart instance.
 * The callback will be invoked when an error occurs. Callback functions are
 * invoked from `duart_tasks()`. Multiple callbacks (defined in DUART_MAX_CALLBACKS_ALLOWED)
 * can be registered to this function. Callbacks are executed in the order they are registered.
 *
 * @param me The duart instance to register an error callback on.
 * @param callback Function pointer to the error callback.
 *
 * @return true if callback was set to trigger, false otherwise.
 */
bool duart_errorCallback(pDuart_t me, duart_errCallback callback)
{
    _duart_t* _me = &me->privateData;

    if (_me->configuredErrorCallbacks < DUART_MAX_CALLBACKS_ALLOWED)
    {
        _me->errorCallback[_me->configuredErrorCallbacks] = callback;
        _me->configuredErrorCallbacks++;
        return true;
    }
    return false;
}

/**
 * @brief Invokes the registered error callback function.
 *
 * This function is called by implementations when an error is detected in a platform duart instance.
 * If an error callback is registered, it will be called with the associated error code.
 *
 * @param platform The platform duart instance to raise error callbacks on.
 * @param error The error code encountered by the UART.
 */
void duart_raiseErrorCallback(pPlatform_duart_t platform, duart_error error)
{
    pDuart_t me = &platform->super;
    _duart_t* _me = &me->privateData;

    for (uint32_t i = 0; i < _me->configuredErrorCallbacks; i++)
    {
        if (_me->errorCallback[i])
        {

            _me->errorCallback[i](me, error);
        }
    }
}

/**
 * @brief Retrieves a string that describes a duart instance error code.
 *
 * This function returns a human-readable string that corresponds to a duart instance error code.
 * If the error code is within the range of defined errors, the corresponding string
 * from an array of error strings is returned. Otherwise, "UNKNOWN" is returned.
 *
 * @param error The error code to convert to a string.
 * @return A pointer to a string that describes the UART error code.
 */
const char* duart_getStringError(duart_error error)
{
    if (error >= 0 && error < duart_errors_count)
    {
        return duartStringErrors[error];
    }
    else
    {
        return "UNKNOWN";
    }
}

/** @brief Register a callback for rx_dataReceivedUserCallback.
 *
 * @param me The duart instance to register data received callback on.
 * @param callback Callback function to register.
 * @param arg Argument to provide to the callback.
 */
void duart_registerDataReceivedUserCallback(pDuart_t me, duart_callback callback, void* arg)
{
    _duart_t* _me = &me->privateData;

    // Clear any existing assignment so we don't get an invalid callback call if data comes in while configuring
    _me->rx_dataReceivedUserCallback = NULL;
    _me->rx_dataArgs = arg;
    _me->rx_dataReceivedUserCallback = callback;
}

/** @brief Get initialization status of a duart instance.
 *
 * @param me duart instance to get status for.
 * @return true duart instance is initialized.
 * @return false duart instance is not initialized.
 */
bool duart_isInitialized(pDuart_t me)
{
    _duart_t* _me = &me->privateData;

    return _me->isInitialized;
}
