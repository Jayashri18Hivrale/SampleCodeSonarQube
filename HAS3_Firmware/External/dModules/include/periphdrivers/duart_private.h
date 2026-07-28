#ifndef DUART_PRIVATE_H
#define DUART_PRIVATE_H

#include "d_uart.h"

typedef void (*duart_callback)(pDuart_t duart, void* arg);
typedef void (*duart_enableBit)(bool isEnabled);

/**
 * @brief Interface for the duart.
 *        Contains function pointers for executing tasks.
 */
typedef struct
{
    void (*Tasks)(pPlatform_duart_t platform); /**< Function to execute the tasks. */
} duart_interface_t;

typedef struct
{
    uint32_t configuredErrorCallbacks;
    void* rx_dataArgs;                                            /**< Arguments for the RX data received callback function */
    bool isInitialized;                                           /**< Initialization status */
    bool isRxOverrun;                                             /** Rx buffer was full when we attempted to queue Rx data. */
    bool isTxOverrun;                                             /** Tx buffer was full when we attempted to queue Tx data. */
    bool isTxIrqUnhandled;                                        /** Tx buffer was empty when we attempted to dequeue Tx data. */
    duart_callback rx_dataReceivedUserCallback;                   /**< Callback function for RX data received */
    duart_enableBit enableTxIsr;                                  /**< Function to enable TX interrupt */
    duart_enableBit enableIsr;                                    /**< Function to enable RX interrupt */
    duart_errCallback errorCallback[DUART_MAX_CALLBACKS_ALLOWED]; /**< Error callback function */
    duart_interface_t* vtable;                                    /**< Virtual table. */
} _duart_t;

/**
 * @brief duart configuration structure.
 */
typedef struct
{
    duart_interface_t* vTable;                  /**< Virtual table. */
    uint8_t* rxBuffer;                          /**< RX buffer */
    uint8_t* txBuffer;                          /**< TX buffer */
    uint32_t rxSize;                            /**< RX buffer size */
    uint32_t txSize;                            /**< TX buffer size */
    duart_callback rx_dataReceivedUserCallback; /**< Callback function for RX data received */
    duart_enableBit enableTxIsr;                /**< Function to enable TX interrupt */
    duart_enableBit enableIsr;                  /**< Function to enable RX interrupt */
    void* userData;                             // User-specified data, intended to be used as callback context
} duart_conf_t;

// platform specific functions
void duart_Init(pPlatform_duart_t platform, duart_conf_t* conf);

void duart_rx_irq_callback(pPlatform_duart_t platform, uint8_t received_data);

uint8_t duart_tx_irq_callback(pPlatform_duart_t platform);

void duart_raiseErrorCallback(pPlatform_duart_t platform, duart_error error);

static const _duart_t _duart_t_INIT = { 0 };
#endif
