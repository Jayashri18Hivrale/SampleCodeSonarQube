#include <windows.h>

#include "pnTests.h"
#include "has3_device.h"

#define SENT_DATA_BUFFER_SIZE 2000

#define CYCLIC_TEST_START_COMMAND     0xA0
#define CYCLIC_TEST_GET_READS_COMMAND 0xA1
#define CYCLIC_TEST_TIME_TEST_MS 2000
#define CYCLIC_TEST_HETHOST_TIMEOUT 0

#define NO_OFFSET 0
#define DEFAULT_SLAVE_NUMBER 0

void print_cyclicTest_results(float result);

uint16_t sentdata[SENT_DATA_BUFFER_SIZE] = { 0 };
has3_cyclicData_t data = { 0 };

void run_cyclic_test(NHost_Profinet_t* profinetNethost)
{
    LARGE_INTEGER start, ellapsed;
    LARGE_INTEGER frequency;
    QueryPerformanceFrequency(&frequency);
    float elapsed;
    int i = 0;
    CIFXHANDLE hChannel = profinetNethost->common.hCyclicChannel;
    uint32_t slaveHandle = profinetNethost->common.configuredSlavesHandlers[DEFAULT_SLAVE_NUMBER];

    printf("=== Cyclic Speed Test ===\n");
    
    data.out.command = CYCLIC_TEST_START_COMMAND;
    QueryPerformanceCounter(&start);
    while (1)
    {
        QueryPerformanceCounter(&ellapsed);
        elapsed = (double)(ellapsed.QuadPart - start.QuadPart) * 1000 / frequency.QuadPart;
        data.out.registerValue = (uint16_t)(elapsed * 10);
        xChannelIOWrite(hChannel, slaveHandle, NO_OFFSET, HAS3_OUT_DATA_SIZE, &data.out, CYCLIC_TEST_HETHOST_TIMEOUT);
        sentdata[i] = data.out.registerValue;
        data.out.registerId = CYCLIC_TEST_GET_READS_COMMAND;
        if (elapsed > CYCLIC_TEST_TIME_TEST_MS)
        {
            break;
        }
    }
    data.out.command = CYCLIC_TEST_GET_READS_COMMAND;
    xChannelIOWrite(hChannel, slaveHandle, NO_OFFSET, HAS3_OUT_DATA_SIZE, &data.out, CYCLIC_TEST_HETHOST_TIMEOUT);
    do
    {
        xChannelIORead(hChannel, slaveHandle, NO_OFFSET, HAS3_IN_DATA_SIZE, &data.in, CYCLIC_TEST_HETHOST_TIMEOUT);
    } while (data.in.header != CYCLIC_TEST_GET_READS_COMMAND);
    print_cyclicTest_results(elapsed / data.in.elementStatus);
}

void print_cyclicTest_results(float result) {
    printf("\nTest Results for PN Slave Cyclic time\n");
    printf("---------------------------------------\n");
    printf("Average cycle time: %.2f ms\n", result);
    printf("---------------------------------------\n");
    printf("Press any key to return to menu...");
    _getch();
    system("cls");
}