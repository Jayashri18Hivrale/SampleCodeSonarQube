#ifndef D_UART_HELPER_H
#define D_UART_HELPER_H
#include "d_uart.h"

#define C_BUFFER_SIZE 32

extern duart_error callbackError;
extern bool txIrqEnabled;
extern int callback_order;

void enableTXirq(bool i);

pDuart_t getInitializedDummyUart();

void errCallback(pDuart_t duart, duart_error error);

void errCallbackOrder1(pDuart_t duart, duart_error error);
void errCallbackOrder2(pDuart_t duart, duart_error error);
void errCallbackOrder3(pDuart_t duart, duart_error error);
void errCallbackOrder4(pDuart_t duart, duart_error error);

void initBuffer(uint8_t* buffer, int size);

#endif // !D_UART_HELPER_H
