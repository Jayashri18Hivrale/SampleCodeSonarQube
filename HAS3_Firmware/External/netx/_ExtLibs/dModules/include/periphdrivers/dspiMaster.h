/**
 * @file dSpiMaster.h
 * @brief Hardware-abstracted SPI Master Driver with Platform Adaptation Layer
 *
 * This module provides a vendor-agnostic interface for SPI master communication.
 * It is divided into two layers:
 *
 * 1. **Platform Implementation Layer**:
 *    - Vendor-specific code that adapts to the abstract interface
 *    - Implements low-level register access, interrupt handling, and platform-specific configuration
 *
 * 2. **Abstract Application Layer**:
 *    - Hardware-independent API for SPI transactions
 *    - Manages data buffering and callback notifications
 *
 * Design Features:
 * - Full-duplex 8-bit transactions
 * - Interrupt-driven operation
 * - Circular buffering for zero-copy transactions
 * - Chip Select (CS) line management
 *
 * ### Assumptions:
 * - The SPI driver operates in full-duplex mode, handling both RX and TX data simultaneously.
 * - The platform-specific implementation provides accurate handling of FIFO status and interrupt control.
 * - Callback functions must be non-blocking and lightweight to prevent interrupt latency.
 */

#ifndef DSPI_MASTER_H
#define DSPI_MASTER_H

#include "circularBuffer_byte.h"
#include <stddef.h>

 /** @typedef pPlatform_dSpiM_t
  *  @brief Pointer to the platform-specific SPI master structure.
  */
typedef struct platform_dSpiM* pPlatform_dSpiM_t;

/** @typedef pDspiM_t
 *  @brief Pointer to the abstract SPI master structure.
 */
typedef struct dSpiM* pDspiM_t;

/**
 * @typedef dSpiM_callback
 * @brief Function pointer type for transaction completion callbacks.
 *
 * This callback is executed upon completion of an SPI transaction.
 *
 * @param self Pointer to the `dSpiM` instance.
 * @param arg User-defined argument for the callback.
 */
typedef void (*dSpiM_callback)(pDspiM_t self, void* arg);

/**
 * @struct dSpiM_vTable_t
 * @brief Virtual table for platform-specific functions.
 *
 * Contains function pointers to platform-dependent operations. These methods abstract low-level
 * SPI control, including interrupt enable/disable functionality and FIFO status checking.
 *
 * ### Assumptions for Implementations:
 * - `EnableAllInterrupts`: Enables or disables SPI interrupts globally. Implementation must manage all necessary interrupts.
 * - `EnableTxFifoLevelIrq` and `EnableRxByteReceivedIrq`: Enable or disable individual FIFO-related interrupts.
 * - `isTxFifoFull`: Returns `true` if the TX FIFO is full and cannot accept more data.
 * - `isRxFifoEmpty`: Returns `true` if the RX FIFO is empty and no data is available to read.
 * - `isSpiBusy`: Checks whether the SPI peripheral is actively engaged in a transaction.
 */
typedef struct {
    void (*EnableAllInterrupts)(pPlatform_dSpiM_t platform, bool isEnabled); /**< Enables or disables all SPI interrupts */
    void (*EnableTxFifoLevelIrq)(pPlatform_dSpiM_t platform, bool isEnabled); /**< Enables or disables FIFO level interrupts */
    void (*EnableRxByteReceivedIrq)(pPlatform_dSpiM_t platform, bool isEnabled); /**< Enables or disables Rx byte received interrupt */
    bool (*isTxFifoFull)(pPlatform_dSpiM_t platform);
    bool (*isRxFifoEmpty)(pPlatform_dSpiM_t platform);
    bool (*isSpiBusy)(pPlatform_dSpiM_t platform);
} dSpiM_vTable_t;

/**
 * @struct dSpiM
 * @brief Abstract SPI master structure.
 *
 * Encapsulates core elements of the SPI master, including circular buffers for RX and TX,
 * and callback functions for asynchronous operations. The virtual table provides a hardware
 * abstraction for platform-specific interactions.
 */
struct dSpiM
{
    cb_byte_t rxCB; /**< RX circular buffer for incoming data */
    cb_byte_t txCB; /**< TX circular buffer for outgoing data */
    dSpiM_callback completeCallback; /**< Callback for transaction completion */
    void* completeCallbackUserArgs; /**< User-defined argument passed to the callback */
    dSpiM_callback receivedByteCallback; /**< Callback for received completion */
    void* receivedByteCallbackUserArgs; /**< User-defined argument passed to the callback */
    const dSpiM_vTable_t* vTable; /**< Pointer to platform-specific virtual table */
};

/**
 * @struct platform_dSpiM
 * @brief Platform-specific SPI master structure.
 *
 * Extends the abstract `dSpiM` structure with platform-specific data fields.
 * Expected to handle low-level SPI operations, including register access and interrupts.
 */
struct platform_dSpiM
{
    struct dSpiM super; /**< Base `dSpiM` structure */
    // Platform-specific fields follow.
};

/**
 * @struct dSpiM_conf_t
 * @brief Configuration structure for SPI master initialization.
 *
 * Provides the necessary parameters for configuring and initializing the SPI master.
 */
typedef struct
{
    uint8_t* rxBuffer; /**< Pointer to the RX buffer */
    uint8_t* txBuffer; /**< Pointer to the TX buffer */
    uint32_t rxSize; /**< Size of the RX buffer */
    uint32_t txSize; /**< Size of the TX buffer */
    const dSpiM_vTable_t* vTable; /**< Pointer to platform-specific virtual table */
} dSpiM_conf_t;

/**
 * ## Platform Implementation Functions
 * These functions interact directly with the platform-specific SPI hardware.
 */

 /**
  * @brief Initializes the SPI master module.
  *
  * Configures the SPI master instance using the provided platform-specific structure
  * and initialization parameters. Links the virtual table for platform-specific operations.
  *
  * @param platform Pointer to the platform-specific SPI master structure.
  * @param conf Pointer to the configuration structure containing initialization parameters.
  */
void dSpiM_Init(pPlatform_dSpiM_t platform, dSpiM_conf_t* conf);

/**
 * @brief Handles the SPI TX FIFO level interrupt.
 *
 * Manages transmission in the platform fifo level IRQ.
 *
 * @param platform Pointer to the platform-specific SPI master structure.
 * @param pIsFifoFullFlag Pointer to a flag indicating if the FIFO is full.
 * @param pSpiDataRegister Pointer to the SPI data register.
 */
void dSpiM_HandleTxFifoLevelIrq(pPlatform_dSpiM_t platform, uint8_t* pSpiDataRegister);

/**
 * @brief Handles the SPI RX byte received interrupt.
 *
 * Reads data from the SPI data register and stores it in the RX buffer.
 *
 * @param platform Pointer to the platform-specific SPI master structure.
 * @param pSpiDataRegister Pointer to the SPI data register.
 */
void dSpiM_HandleRxByteReceived(pPlatform_dSpiM_t platform, uint8_t* pSpiDataRegister);

/**
 * ## Hardware-Decoupled Usage Functions
 * These functions provide a hardware-independent API for SPI operations.
 */

 /**
  * @brief Queues data for SPI transmission.
  *
  * Adds the specified data to the TX buffer for transmission through the SPI interface.
  *
  * @param self Pointer to the abstract SPI master structure.
  * @param data Pointer to the data array to be transmitted.
  * @param length Number of bytes to transmit.
  */
void dSpiM_Write(pDspiM_t self, uint8_t* data, size_t length);

/**
 * @brief Sets the transaction complete callback.
 *
 * Registers a callback function that is invoked upon transaction completion.
 *
 * @param self Pointer to the abstract SPI master structure.
 * @param callback Pointer to the callback function.
 * @param userArgs User-defined argument to pass to the callback.
 */
void dSpiM_SetTransactionCompleteCallback(pDspiM_t self, dSpiM_callback callback, void* userArgs);

/**
 * @brief Sets the received byte callback.
 *
 * Registers a callback function that is invoked whenever a byte is received.
 *
 * @param self Pointer to the abstract SPI master structure.
 * @param callback Pointer to the callback function.
 * @param userArgs User-defined argument to pass to the callback.
 */
void dSpiM_SetReceivedByteCallback(pDspiM_t self, dSpiM_callback callback, void* userArgs);

/**
 * @brief Retrieves data from the RX buffer.
 *
 * Reads the next available byte from the RX buffer.
 *
 * @param self Pointer to the abstract SPI master structure.
 * @param recData Pointer to store the received data.
 * @return `true` if data was successfully read, `false` if the buffer was empty.
 */
bool dSpiM_GetRxData(pDspiM_t self, uint8_t* recData);

#endif  /* DSPI_MASTER_H */
