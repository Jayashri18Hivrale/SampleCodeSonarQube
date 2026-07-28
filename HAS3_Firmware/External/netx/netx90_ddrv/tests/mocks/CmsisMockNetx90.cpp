extern "C" {
#include "netx90_app.h"
}

uint8_t cortexm4_scs_space[0xF00] = { 0 };

uint8_t netx90_uart_space[0x40] = { 0 };
uint8_t netx90_uart_app_space[0x40] = { 0 };
uint8_t netx90_uart_xpic_app_space[0x40] = { 0 };
uint8_t netx90_pio_app_space[0x100] = { 0 };
