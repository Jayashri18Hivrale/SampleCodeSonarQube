#include "CppUTest/TestHarness.h"

extern "C" {
#include "d_uart.h"
}

#include "duart_helper.h"

// clang-format off
TEST_GROUP(dUart) {
    void setup()
    {

    }
    void teardown()
    {
    }
};

// clang-format on

TEST(dUart, Init)
{
    pDuart_t duart = getInitializedDummyUart();
    CHECK_TRUE(duart_isInitialized(duart));
}

TEST(dUart, read)
{
    pDuart_t duart = getInitializedDummyUart();
    duart_rx_irq_callback((pPlatform_duart_t)duart, 'a');
    duart_rx_irq_callback((pPlatform_duart_t)duart, 'b');
    duart_rx_irq_callback((pPlatform_duart_t)duart, 'c');
    duart_rx_irq_callback((pPlatform_duart_t)duart, 'd');

    CHECK_TRUE(duart_isRxDataAvailable(duart));

    uint8_t rBytes[8];
    uint32_t readBytes = duart_read(duart, rBytes, 5);

    LONGS_EQUAL(4, readBytes);

    LONGS_EQUAL('a', rBytes[0]);
    LONGS_EQUAL('b', rBytes[1]);
    LONGS_EQUAL('c', rBytes[2]);
    LONGS_EQUAL('d', rBytes[3]);

    LONGS_EQUAL(0, duart_read(duart, rBytes, 2));
}

TEST(dUart, readByte)
{
    pDuart_t duart = getInitializedDummyUart();
    duart_rx_irq_callback((pPlatform_duart_t)duart, 'a');
    duart_rx_irq_callback((pPlatform_duart_t)duart, 'b');
    duart_rx_irq_callback((pPlatform_duart_t)duart, 'c');
    duart_rx_irq_callback((pPlatform_duart_t)duart, 'd');

    LONGS_EQUAL(duart_readByte(duart), 'a');
    LONGS_EQUAL(duart_readByte(duart), 'b');
    LONGS_EQUAL(duart_readByte(duart), 'c');
    LONGS_EQUAL(duart_readByte(duart), 'd');
}

TEST(dUart, write)
{
    uint8_t SentData[4] = { 0 };
    pDuart_t duart = getInitializedDummyUart();

    const char* text = "Hola";

    duart_write(duart, (uint8_t*)text, 4);

    int i = 0;
    while (txIrqEnabled)
    {
        SentData[i] = duart_tx_irq_callback((pPlatform_duart_t)duart);
        i++;
    }
    LONGS_EQUAL(4, i);
    STRCMP_CONTAINS("Hola", (char*)SentData);
}

TEST(dUart, writeByte)
{
    uint8_t SentData[4] = { 0 };
    pDuart_t duart = getInitializedDummyUart();

    duart_writeByte(duart, 1);
    duart_writeByte(duart, 2);
    duart_writeByte(duart, 3);

    int i = 0;
    while (txIrqEnabled)
    {
        SentData[i] = duart_tx_irq_callback((pPlatform_duart_t)duart);
        i++;
    }
    LONGS_EQUAL(3, i);
    LONGS_EQUAL(1, SentData[0]);
    LONGS_EQUAL(2, SentData[1]);
    LONGS_EQUAL(3, SentData[2]);
}

TEST(dUart, isTxDataPending)
{
    uint8_t SentData[4];
    pDuart_t duart = getInitializedDummyUart();
    const char* text = "Hola";

    duart_write(duart, (uint8_t*)text, 4);

    CHECK_TRUE(duart_isTxDataPending(duart));

    int i = 0;
    while (txIrqEnabled)
    {
        SentData[i] = duart_tx_irq_callback((pPlatform_duart_t)duart);
        i++;
    }
    CHECK_FALSE(duart_isTxDataPending(duart));
    LONGS_EQUAL(SentData[0], 'H');
    LONGS_EQUAL(SentData[1], 'o');
    LONGS_EQUAL(SentData[2], 'l');
    LONGS_EQUAL(SentData[3], 'a');
}

TEST(dUart, readRaiseRxBufferOverflowCallBack)
{
    uint8_t overFlowedBuffer[C_BUFFER_SIZE + 1];
    pDuart_t duart = getInitializedDummyUart();
    duart_errorCallback(duart, errCallback);
    initBuffer(overFlowedBuffer, sizeof(overFlowedBuffer));
    callbackError = IrqPlatformError;

    for (size_t i = 0; i < sizeof(overFlowedBuffer); i++)
    {
        duart_rx_irq_callback((pPlatform_duart_t)duart, overFlowedBuffer[i]);
    }
    duart_tasks(duart);

    LONGS_EQUAL(callbackError, rxBufferOverflow);
}

TEST(dUart, writeRaiseTxBufferOverflowCallBack)
{
    uint8_t overFlowedBuffer[C_BUFFER_SIZE + 1];
    pDuart_t duart = getInitializedDummyUart();
    duart_errorCallback(duart, errCallback);
    callbackError = IrqPlatformError;
    initBuffer(overFlowedBuffer, sizeof(overFlowedBuffer));

    duart_write(duart, overFlowedBuffer, sizeof(overFlowedBuffer));
    duart_tasks(duart);

    LONGS_EQUAL(callbackError, txBufferOverflow);
}

TEST(dUart, txIrqRaiseTxIrqUnhandledCallBack)
{
    pDuart_t duart = getInitializedDummyUart();
    duart_errorCallback(duart, errCallback);
    CHECK_FALSE(duart_isTxDataPending(duart));
    callbackError = IrqPlatformError;

    duart_tx_irq_callback((pPlatform_duart_t)duart);
    duart_tasks(duart);

    LONGS_EQUAL(callbackError, txIrqUnhandled);
}

TEST(dUart, ThreeErrorCallbacksCalledInOrder)
{
    uint8_t overFlowedBuffer[C_BUFFER_SIZE + 1];
    pDuart_t duart = getInitializedDummyUart();
    duart_errorCallback(duart, errCallbackOrder1);
    duart_errorCallback(duart, errCallbackOrder2);
    duart_errorCallback(duart, errCallbackOrder3);
    initBuffer(overFlowedBuffer, sizeof(overFlowedBuffer));
    callbackError = IrqPlatformError;
    callback_order = 0;

    duart_write(duart, overFlowedBuffer, sizeof(overFlowedBuffer));
    duart_tasks(duart);

    LONGS_EQUAL(3, callback_order);
}

TEST(dUart, FourthCallbacksItIsNotCalled)
{
    uint8_t overFlowedBuffer[C_BUFFER_SIZE + 1];
    pDuart_t duart = getInitializedDummyUart();
    duart_errorCallback(duart, errCallbackOrder1);
    duart_errorCallback(duart, errCallbackOrder2);
    duart_errorCallback(duart, errCallbackOrder3);
    bool ret = duart_errorCallback(duart, errCallbackOrder4);
    initBuffer(overFlowedBuffer, sizeof(overFlowedBuffer));
    callbackError = IrqPlatformError;
    callback_order = 0;

    duart_write(duart, overFlowedBuffer, sizeof(overFlowedBuffer));
    duart_tasks(duart);

    LONGS_EQUAL(3, callback_order);
    CHECK_FALSE(ret);
}
