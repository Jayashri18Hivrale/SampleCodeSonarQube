#include "nex90_duart.h"

#define MASK_USED_BITS_NX90_uartiir 0x0000000fU // Definition from regdef_netx90_arm_app.h Todo replace when possible

#define NETX90_DUART_UART_0 uart_app
#define NETX90_DUART_UART_1 uart_xpic_app
#define NETX90_DUART_UART_2 uart

#define BAUD_RATE_DIVISOR 100ull

// Hilscher's documentation has this "more precise" calculation defined as (((Baud Rate * 16) / System Frequency) * 2^16).
// The (* 2^16) is reordered here to maintain integer math.
// netx90_duart_baudrate values are /100, so SystemCoreClock is /100 here to match.

#define DEV_BAUDRATE_DIV_LO(baud) (((baud * 16ull * 65536ull) / (SystemCoreClock / BAUD_RATE_DIVISOR)) & 0xff)
#define DEV_BAUDRATE_DIV_HI(baud) (((baud * 16ull * 65536ull) / (SystemCoreClock / BAUD_RATE_DIVISOR)) >> 8)

netx90_duart_t* netx90Uarts[3] = { 0 };

/**
 * @brief Executes module specific tasks.
 *
 * This function is to be placed within the Vtable function for supper duart.h
 *
 * @param platform Pointer to the parent structure.
 */
static void tasks(pPlatform_duart_t platform)
{
    netx90_duart_t* me = (netx90_duart_t*)platform;
    if (me->uartRegister->uartrsr & (uart_app_uartrsr_FE_Msk | uart_app_uartrsr_FE_Msk | uart_app_uartrsr_BE_Msk | uart_app_uartrsr_OE_Msk))
    {
        duart_raiseErrorCallback(platform, IrqPlatformError);
        // Clear flags
        me->uartRegister->uartrsr = uart_app_uartrsr_FE_Msk | uart_app_uartrsr_FE_Msk | uart_app_uartrsr_BE_Msk | uart_app_uartrsr_OE_Msk;
    }
}

duart_interface_t netx90_duart_vtable = { tasks };

/**
 * @brief Enables or disables UART0 TX IRQ.
 *
 * To be used by the duart module.
 *
 * @param isEnabled Boolean indicating whether to enable (true) or disable (false) UART0 TX IRQ.
 */
static void enableUART0_TxIrq(bool isEnabled)
{
    NETX90_DUART_UART_0->uartcr_b.TIE = isEnabled;
}

/**
 * @brief Enables or disables UART1 TX IRQ.
 *
 * To be used by the duart module.
 *
 * @param isEnabled Boolean indicating whether to enable (true) or disable (false) UART1 TX IRQ.
 */
static void enableUART1_TxIrq(bool isEnabled)
{
    NETX90_DUART_UART_1->uartcr_b.TIE = isEnabled;
}

/**
 * @brief Enables or disables UART2 TX IRQ.
 *
 * To be used by the duart module.
 *
 * @param isEnabled Boolean indicating whether to enable (true) or disable (false) UART2 TX IRQ.
 */
static void enableUART2_TxIrq(bool isEnabled)
{
    NETX90_DUART_UART_2->uartcr_b.TIE = isEnabled;
}

/**
 * @brief Enables or disables device IRQ.
 *
 * @param isEnabled If true, IRQs are enabled. If false, IRQs are disabled.
 *
 * TODO. Move to a more general module where it can reached by others hardware modules.
 */
static void enableIrq(bool isEnabled)
{
    if (isEnabled)
        __enable_irq();
    else
        __disable_irq();
}

/**
 * @brief Gets the IRQ number for the specified UART.
 * @param netx90Uart The UART to get the IRQ for.
 * @return The IRQ number.
 */
static IRQn_Type GetDuartIRQ(netx90_uart netx90Uart)
{
    switch (netx90Uart)
    {
    case Netx90Uart0:
        return uart_app_IRQn;
    case Netx90Uart1:
        return uart_xpic_app_IRQn;
    case Netx90Uart2:
        return uart_IRQn;
    default:
        return uart_app_IRQn;
    }
}

/**
 * @brief Sets the base register for the specified UART.
 * @param me Pointer to the netx90_duart_t structure.
 */
static void setBaseRegister(netx90_duart_t* me)
{
    switch (me->netx90Uart)
    {
    case Netx90Uart0:
        me->uartRegister = NETX90_DUART_UART_0;
        return;
    case Netx90Uart1:
        me->uartRegister = (uart_app_Type*)NETX90_DUART_UART_1;
        return;
    case Netx90Uart2:
        me->uartRegister = (uart_app_Type*)NETX90_DUART_UART_2;
        return;
    }
}

/**
 * @brief Sets the function to enable TX IRQ for the specified UART.
 * @param me Pointer to the netx90_duart_t structure.
 * @param conf Pointer to the duart_conf_t structure.
 */
static void SetDuartTxEnableFunction(netx90_duart_t* me, duart_conf_t* conf)
{
    switch (me->netx90Uart)
    {
    case Netx90Uart0:
        conf->enableIsr = enableIrq;
        conf->enableTxIsr = enableUART0_TxIrq;
        netx90Uarts[0] = me;
        return;
    case Netx90Uart1:
        conf->enableIsr = enableIrq;
        conf->enableTxIsr = enableUART1_TxIrq;
        netx90Uarts[1] = me;
        return;
    case Netx90Uart2:
        conf->enableIsr = enableIrq;
        conf->enableTxIsr = enableUART2_TxIrq;
        netx90Uarts[2] = me;
        return;
    }
}

/**
 * @brief Handles interrupts for the specified UART.
 * @param me Pointer to the netx90_duart_t structure.
 */
static void netx90uart_interrupt(netx90_duart_t* me)
{
    if (!me)
        return;
    if (me->uartRegister->uartiir_b.RIS)
    {
        uint8_t data = me->uartRegister->uartdr_b.DATA;
        duart_rx_irq_callback((pPlatform_duart_t)me, data);
    }
    if (me->uartRegister->uartiir_b.TIS && me->uartRegister->uartcr_b.TIE)
    {
        me->uartRegister->uartdr_b.DATA = duart_tx_irq_callback((pPlatform_duart_t)me);
    }
    me->uartRegister->uartiir = MASK_USED_BITS_NX90_uartiir;
}

/**
 * @brief Initializes the specified UART.
 * @param me Pointer to the netx90_duart_t structure.
 * @param conf Pointer to the netx90_duart_conf_t structure.
 */
void Netx90Duart_Init(netx90_duart_t* me, netx90_duart_conf* conf)
{
    duart_conf_t duartConf = { 0 };
    me->baudRate = conf->baudRate;
    me->netx90Uart = conf->netx90Uart;
    duartConf.rx_dataReceivedUserCallback = conf->rxCallback;
    setBaseRegister(me);
    me->uartRegister->uartcr = 0;/* First of all disable everything */
    me->uartRegister->uartcr_2 = 0x1u;
    /* Adjust the baud rate register */
    me->uartRegister->uartlcr_l = (uint32_t)DEV_BAUDRATE_DIV_LO((uint64_t)me->baudRate);
    me->uartRegister->uartlcr_m = (uint32_t)DEV_BAUDRATE_DIV_HI((uint64_t)me->baudRate);
    me->uartRegister->uartcr_b.TX_RX_LOOP = 0;
    /* set UART to 8 bits, 1 stop bit, no parity, FIFO enabled */
    me->uartRegister->uartlcr_h_b.BRK = 0;
    me->uartRegister->uartlcr_h_b.EPS = 0;
    me->uartRegister->uartlcr_h_b.STP2 = 0;
    me->uartRegister->uartlcr_h_b.PEN = 0;
    me->uartRegister->uartlcr_h_b.FEN = 1u;
    me->uartRegister->uartlcr_h_b.WLEN = 0x3u;
    /* Configure FIFO enables and interrupt when one byte received*/
    me->uartRegister->uartrxiflsel_b.RXIFLSEL = 0x1u;
    me->uartRegister->uarttxiflsel_b.TXIFLSEL = 0x1u;
    /* Set TX-Driver to enabled */
    me->uartRegister->uartdrvout_b.DRVTX = 1u;
    /* Configure nvic (activate IRQ, define priority and so on) */
    me->uartRegister->uartcr_b.RIE = 1;
    IRQn_Type irq = GetDuartIRQ(me->netx90Uart);
    NVIC_ClearPendingIRQ(irq);
    NVIC_EnableIRQ(irq);
    NVIC_SetPriority(irq, conf->uartInterruptPriority);
    /* Configure duart*/
    duartConf.rxBuffer = me->RxBuffer;
    duartConf.rxSize = NETX90_DUART_RX_BUFFER_SIZE;
    duartConf.txBuffer = me->TxBuffer;
    duartConf.txSize = NETX90_DUART_TX_BUFFER_SIZE;
    duartConf.vTable = &netx90_duart_vtable;
    SetDuartTxEnableFunction(me, &duartConf);
    duart_Init((pPlatform_duart_t)me, &duartConf);
    /* Finally enable the UART */
    me->uartRegister->uartcr_b.uartEN = 1;
}

/**
 * @brief Handles the interrupt for the UARTAPP.
 *
 * This function is called when an interrupt occurs on the UARTAPP.
 * To be placed within the vector table.
 * It calls the netx90uart_interrupt function with the first UART.
 */
__INLINE void dUARTAPP_IRQHandler()
{
    netx90uart_interrupt(netx90Uarts[0]);
}

/**
 * @brief Handles the interrupt for the UARTXPIC.
 *
 * This function is called when an interrupt occurs on the UARTXPIC.
 * To be placed within the vector table.
 * It calls the netx90uart_interrupt function with the second UART.
 */
__INLINE void dUARTXPIC_IRQHandler()
{
    netx90uart_interrupt(netx90Uarts[1]);
}

/**
 * @brief Handles the interrupt for the UART.
 *
 * This function is called when an interrupt occurs on the UART.
 * To be placed within the vector table.
 * It calls the netx90uart_interrupt function with the third UART.
 */
__INLINE void dUART_IRQHandler()
{
    netx90uart_interrupt(netx90Uarts[2]);
}
