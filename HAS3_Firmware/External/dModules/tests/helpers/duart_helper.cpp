extern "C" {
#include "d_uart.h"
}

#include "duart_helper.h"

void dummy_tasks(pPlatform_duart_t platform);

duart_error callbackError;
struct duart dummy_uart;
pDuart_t pDummy_duart = &dummy_uart;
uint8_t rxBuffer[C_BUFFER_SIZE];
uint8_t txBuffer[C_BUFFER_SIZE];
duart_conf_t uart_conf;
bool txIrqEnabled;
int callback_order = 0;
duart_interface_t dummyVtable = { dummy_tasks };

void enableTXirq(bool i)
{
    txIrqEnabled = i;
    return;
}

void uart_config_default()
{
    uart_conf.rxBuffer = rxBuffer;
    uart_conf.txBuffer = txBuffer;
    uart_conf.rxSize = C_BUFFER_SIZE;
    uart_conf.txSize = C_BUFFER_SIZE;
    uart_conf.enableTxIsr = enableTXirq;
    uart_conf.vTable = &dummyVtable;
}

pDuart_t getInitializedDummyUart()
{
    uart_config_default();
    duart_Init((pPlatform_duart_t)pDummy_duart, &uart_conf);
    return pDummy_duart;
}

void errCallback(pDuart_t duart, duart_error error)
{
    callbackError = error;
}

void errCallbackOrder1(pDuart_t duart, duart_error error)
{
    if (callback_order == 0)
    {
        callback_order++;
    }
}

void errCallbackOrder2(pDuart_t duart, duart_error error)
{

    if (callback_order == 1)
    {
        callback_order++;
    }
}

void errCallbackOrder3(pDuart_t duart, duart_error error)
{

    if (callback_order == 2)
    {
        callback_order++;
    }
}

void errCallbackOrder4(pDuart_t duart, duart_error error)
{

    if (callback_order == 4)
    {
        callback_order++;
    }
}

void initBuffer(uint8_t* buffer, int size)
{

    for (int i = 0; i < size; i++)
    {
        buffer[i] = i;
    }
}

void dummy_tasks(pPlatform_duart_t platform)
{
}
