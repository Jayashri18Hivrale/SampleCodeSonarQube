#ifndef NETX90_DUART_H
#define NETX90_DUART_H

#include "netx90_app.h"
#include "d_uart.h"

/**
 * @file netx90_duart.h
 * @brief This file contains the definitions and function prototypes for the NetX90 DUART module.
 *
 * The NetX90 DUART module provides an interface for UART communication.
 */

#define NETX90_DUART_RX_BUFFER_SIZE 128
#define NETX90_DUART_TX_BUFFER_SIZE 4096

/**
 * @brief Enumeration of the available baud rates for the DUART.
 */
typedef enum
{
    NetX90_DUART_BR_300 = 3ul,                     /*!< Baudrate of 300 */
    NetX90_DUART_BR_600 = 6ul,                     /*!< Baudrate of 600 */
    NetX90_DUART_BR_1200 = 12ul,                   /*!< Baudrate of 1200 */
    NetX90_DUART_BR_2400 = 24ul,                   /*!< Baudrate of 2400 */
    NetX90_DUART_BR_4800 = 48ul,                   /*!< Baudrate of 4800 */
    NetX90_DUART_BR_9600 = 96ul,                   /*!< Baudrate of 9600 */
    NetX90_DUART_BR_19200 = 192ul,                 /*!< Baudrate of 19200 */
    NetX90_DUART_BR_38400 = 384ul,                 /*!< Baudrate of 38400 */
    NetX90_DUART_BR_57600 = 576ul,                 /*!< Baudrate of 57600 */
    NetX90_DUART_BR_115200 = 1152ul,               /*!< Baudrate of 115200 */
    NetX90_DUART_BR_460800 = 4608ul,               /*!< Baudrate of 460800 */
    NetX90_DUART_BR_DEFAULT = NetX90_DUART_BR_9600 /*!< Baudrate of 9600 */
} netx90_duart_baudrate;

/**
 * @brief Enumeration of the available UARTs.
 */
typedef enum
{
    Netx90Uart0, /**< Uart App */
    Netx90Uart1, /**< Uart XPIC */
    Netx90Uart2, /**< Shared Uart */
} netx90_uart;

/**
 * @brief Structure for the DUART configuration.
 */
typedef struct
{
    netx90_uart netx90Uart;         /**< The UART to use */
    netx90_duart_baudrate baudRate; /**< The baud rate to use */
    duart_callback rxCallback;      /**< The receive callback function */
    uint32_t uartInterruptPriority; /**< Interrupt priority */
} netx90_duart_conf;

/**
 * @brief Structure for the DUART.
 */
typedef struct
{
    struct duart duart;                            /**< The DUART */
    uint64_t baudRate;                             /**< The baud rate */
    netx90_uart netx90Uart;                        /**< The UART */
    uart_app_Type* uartRegister;                   /**< The UART register */
    uint8_t RxBuffer[NETX90_DUART_RX_BUFFER_SIZE]; /**< The receive buffer */
    uint8_t TxBuffer[NETX90_DUART_TX_BUFFER_SIZE]; /**< The transmit buffer */
} netx90_duart_t;

// Function prototypes
void Netx90Duart_Init(netx90_duart_t* me, netx90_duart_conf* conf);

void dUARTAPP_IRQHandler();

void dUARTXPIC_IRQHandler();

void dUART_IRQHandler();

#endif
