#include "CppUTest/TestHarness.h"

extern "C" {
#include "nex90_duart.h"
}

netx90_duart_t netx90_duart_td;

uint32_t SystemCoreClock = 100000000;

// clang-format off
TEST_GROUP(netx90Duart) {
    void setup()
    {
    }
    void teardown()
    {
    }
};

// clang-format on

TEST(netx90Duart, Init)
{
    netx90_duart_conf conf;
    conf.baudRate = NetX90_DUART_BR_115200;
    conf.netx90Uart = Netx90Uart0;
    conf.uartInterruptPriority = 2;

    Netx90Duart_Init(&netx90_duart_td, &conf);

    LONGS_EQUAL(uart_app->uartcr, 0x11);       // UART Enabled, RXInterrupt Enabled
    LONGS_EQUAL(uart_app->uartlcr_h, 0x70);    // 8 bits, no parity, 1 stop bit, no break, FIFO enabled
    LONGS_EQUAL(uart_app->uartlcr_m, 0x04);    // 115200 bauds/sec
    LONGS_EQUAL(uart_app->uartlcr_l, 0xb7);    // 115200 bauds/sec
    LONGS_EQUAL(uart_app->uartdrvout, 0x01);   // Enable TX Driver
    LONGS_EQUAL(uart_app->uartrxiflsel, 0x01); // 1 byte waterlevel
    LONGS_EQUAL(uart_app->uarttxiflsel, 0x01); // 1 byte waterlevel
}
