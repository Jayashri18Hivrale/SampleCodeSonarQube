#ifndef D_UART_H
#define D_UART_H

/**
 * @file D_UART_H
 * @brief This module provides an interface for a DUART (Dematic Universal Asynchronous Receiver/Transmitter).
 *        It includes functions for initialization, reading and writing data, and handling interrupts.
 */

#include "circularBuffer_byte.h"

#define DUART_MAX_CALLBACKS_ALLOWED 3 /**< Maximum number of callbacks allowed per event.*/

/** @brief Represents a concrete implementation of pDuart.
 *
 */
typedef struct platform_duart* pPlatform_duart_t;

/** @brief Represents the pointer to an "abstract" base type that is used as the parameter for d_uart functions.
 *
 * It is expected that the functions will actually be passed a pPlatform_duart_t, which should itself point
 * to a concrete implementation e.g. netx90_duart_t.
 */
typedef struct duart* pDuart_t;

/**
 * @brief Enum for errors.
 */
typedef enum
{
    rxBufferOverflow,
    txBufferOverflow,
    txIrqUnhandled,
    IrqPlatformError,
    duart_errors_count
} duart_error;

/**
 * @brief Callback function types for DUART.
 */
typedef void (*duart_errCallback)(pDuart_t me, duart_error error);

#include "duart_private.h"

/**
 * @brief duart structure.
 */
struct duart
{
    cb_byte_t rxCB; /**< RX circular buffer */
    cb_byte_t txCB; /**< TX circular buffer */
    void* userData; // User-specified data, intended to be used as callback context
    _duart_t privateData;
};

/** @brief platform duart structure.
 *
 * Represents an "abstract" base type for platform implementations.
 * Shows the required structure to make a concrete implementation compatible with pDuart_t.
 */
struct platform_duart
{
    struct duart super; // Base `struct duart` should always be first in any platform-specific structure.
    // Platform-specific fields follow.
};

// Function prototypes
void duart_tasks(pDuart_t me);
uint32_t duart_read(pDuart_t me, uint8_t* data, uint32_t length);
uint8_t duart_readByte(pDuart_t me);
bool duart_isRxDataAvailable(pDuart_t me);
bool duart_isTxDataPending(pDuart_t me);
void duart_write(pDuart_t me, uint8_t* data, uint32_t length);
void duart_writeByte(pDuart_t me, uint8_t data);
bool duart_errorCallback(pDuart_t me, duart_errCallback callback);
const char* duart_getStringError(duart_error error);
void duart_registerDataReceivedUserCallback(pDuart_t me, duart_callback callback, void* arg);
bool duart_isInitialized(pDuart_t me);
uint32_t duart_TxQueueSpaceAvailable(pDuart_t me);

#endif // !D_UART_H
