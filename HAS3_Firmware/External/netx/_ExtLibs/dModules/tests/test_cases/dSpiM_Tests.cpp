#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"

extern "C" {
   #include "dSpiMaster.h"
}

// Define a test master SPI
typedef struct
{
    struct dSpiM super;
    // Additional and dummy test (platform) specific fields
    uint32_t frequency;
    uint32_t mode;
    bool isTxFifoLevelEnabled;
}test_dSpiM;

void Mock_EnableAllInterrupts(pPlatform_dSpiM_t platform, bool isEnabled)
{
    test_dSpiM* emulated = (test_dSpiM*)platform;
    emulated->isTxFifoLevelEnabled = isEnabled;
}

void Mock_EnableTxFifoLevelIrq(pPlatform_dSpiM_t platform, bool isEnabled)
{
    
}

void Mock_EnableRxByteReceivedIrq(pPlatform_dSpiM_t platform, bool isEnabled)
{

}


bool Mock_IsTxFifoFull(pPlatform_dSpiM_t platform)
{
    return true;
}

bool Mock_IsRxFifoEmpty(pPlatform_dSpiM_t platform)
{
    return true;
}

bool Mock_IsSpiBusy(pPlatform_dSpiM_t platform)
{
    return true;
}

TEST_GROUP(dspiTests) {

    test_dSpiM testSpi;
    pPlatform_dSpiM_t pPlatformTestSpi = (pPlatform_dSpiM_t)&testSpi;
    pDspiM_t pTestSpi = (pDspiM_t)&testSpi;
   
    uint8_t rxBuffer[16];
    uint8_t rxBuffer1[16];
    uint8_t txBuffer[16];
    
    
    const dSpiM_vTable_t test_vTable = {
     Mock_EnableAllInterrupts,
     Mock_EnableTxFifoLevelIrq,
     Mock_EnableRxByteReceivedIrq,
     Mock_IsTxFifoFull,
     Mock_IsRxFifoEmpty,
     Mock_IsSpiBusy
    };

    dSpiM_conf_t config = { 0 };
    
    void setup()
    {  
        config.rxBuffer = rxBuffer;
        config.txBuffer = txBuffer;
        config.rxSize = sizeof(rxBuffer);
        config.txSize = sizeof(txBuffer);

        config.vTable = &test_vTable;
        dSpiM_Init(pPlatformTestSpi, &config);
    }

    void teardown() 
    {
       /* mock().clear();*/
    }
};

TEST(dspiTests, InitInitializesRxTxCircularBuffers)
{   
    POINTERS_EQUAL(rxBuffer, testSpi.super.rxCB.buffer);
    CHECK(cb_is_empty(&testSpi.super.rxCB));

    POINTERS_EQUAL(txBuffer, testSpi.super.txCB.buffer);
    CHECK(cb_is_empty(&testSpi.super.txCB));
}

TEST(dspiTests, InitAsignsVTable)
{
    POINTERS_EQUAL(&test_vTable, pPlatformTestSpi->super.vTable);
    POINTERS_EQUAL(test_vTable.EnableAllInterrupts, pPlatformTestSpi->super.vTable->EnableAllInterrupts);
    POINTERS_EQUAL(test_vTable.EnableTxFifoLevelIrq, pPlatformTestSpi->super.vTable->EnableTxFifoLevelIrq);
    POINTERS_EQUAL(test_vTable.EnableRxByteReceivedIrq, pPlatformTestSpi->super.vTable->EnableRxByteReceivedIrq);
    POINTERS_EQUAL(test_vTable.isTxFifoFull, pPlatformTestSpi->super.vTable->isTxFifoFull);
    POINTERS_EQUAL(test_vTable.isRxFifoEmpty, pPlatformTestSpi->super.vTable->isRxFifoEmpty);
    POINTERS_EQUAL(test_vTable.isSpiBusy, pPlatformTestSpi->super.vTable->isSpiBusy);
}

//TEST(dspiTests, WriteDataEnablesInterruptsAndBuffersData)
//{
//    uint8_t testData[] = { 0xAA, 0xBB, 0xCC };
//    uint8_t bufferData;
// 
//    // Call the function under test
//    dSpiM_Write(pTestSpi, testData, sizeof(testData));
//
//    // 
//    CHECK_TRUE(testSpi.isTxFifoLevelEnabled);
//    cb_read(&pTestSpi->txCB, &bufferData);
//    LONGS_EQUAL(0xAA, testData[0]);
//    cb_read(&pTestSpi->txCB, &bufferData);
//    LONGS_EQUAL(0xBB, testData[1]);
//    cb_read(&pTestSpi->txCB, &bufferData);
//    LONGS_EQUAL(0xCC, testData[2]);
//}
//
//TEST(dspiTests, WriteWithNullDataPointer)
//{
//    // Call the function with a NULL pointer
//    dSpiM_Write(pTestSpi, NULL, 5);
//
//    // Verify that no data was added to the buffer
//    CHECK(cb_is_empty(&pTestSpi->txCB));
//
//    // Verify that the interrupt was not enabled
//    CHECK_FALSE(testSpi.isTxFifoLevelEnabled);
//}
//
//TEST(dspiTests, WriteWithZeroLength)
//{
//    uint8_t testData[] = { 0xAA };
//
//    // Call the function with zero length
//    dSpiM_Write(pTestSpi, testData, 0);
//
//    // Verify that no data was added to the buffer
//    CHECK(cb_is_empty(&pTestSpi->txCB));
//
//    // Verify that the interrupt was not enabled
//    CHECK_FALSE(testSpi.isTxFifoLevelEnabled);
//}

