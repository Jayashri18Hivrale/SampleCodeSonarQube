#include "CppUTest/TestHarness.h"
#include <string.h>

extern "C" {
#include "ddio.h"
#include "ddio_helper.h"
}

// clang-format off
TEST_GROUP(ddioTest) {
    void setup()
    {
        conf.vTable = &vtable;
        setValCallCount = 0;
        setPinCallCount = 0;
        setTaskCallCount = 0;
        dummyEdgeCbArg = 0;
        dummyEdgeCbCallCount = 0;
    }
    void teardown()
    {
    }
};

// clang-format on

TEST(ddioTest, Init)
{
    uint8_t expectedSetPinCalls = 0;
    for (uint8_t mode = 0; mode < ddio_Mode_SIZE; mode++)
    {
        for (uint8_t eventMode = 0; eventMode < ddio_EventMode_SIZE; eventMode++)
        {
            conf.mode = (ddio_Mode_t)mode;
            conf.eventMode = (ddio_EventMode_t)eventMode;

            // Fill with garbage so we know the init clears things
            memset(pDummy_ddio, 0xFF, sizeof(*pDummy_ddio));

            ddio_init((pPlatform_ddio_t)pDummy_ddio, &conf);
            expectedSetPinCalls++;

            // ddio_Value_t value;
            // Not initialized

            // ddio_Mode_t mode;
            ENUMS_EQUAL_INT(conf.mode, pDummy_ddio->mode);

            // ddio_EventMode_t eventMode;
            ENUMS_EQUAL_INT(conf.eventMode, pDummy_ddio->eventMode);

            // _ddio_t privateData;
            FUNCTIONPOINTERS_EQUAL(vtable.pin_mode_set, pDummy_ddio->privateData.vtable->pin_mode_set);
            FUNCTIONPOINTERS_EQUAL(vtable.pin_value_write, pDummy_ddio->privateData.vtable->pin_value_write);
            FUNCTIONPOINTERS_EQUAL(vtable.pin_value_read, pDummy_ddio->privateData.vtable->pin_value_read);
            FUNCTIONPOINTERS_EQUAL(vtable.Tasks, pDummy_ddio->privateData.vtable->Tasks);

            LONGS_EQUAL(0, pDummy_ddio->privateData.nRisingEdgeCBs);
            LONGS_EQUAL(0, pDummy_ddio->privateData.nFallingEdgeCBs);
            for (uint8_t i = 0; i < DDIO_MAX_CALLBACKS_ALLOWED; i++)
            {
                FUNCTIONPOINTERS_EQUAL(NULL, pDummy_ddio->privateData.RisingEdgeCBs[i]);
                FUNCTIONPOINTERS_EQUAL(NULL, pDummy_ddio->privateData.RisingEdgeArgs[i]);
                FUNCTIONPOINTERS_EQUAL(NULL, pDummy_ddio->privateData.FallingEdgeCBs[i]);
                FUNCTIONPOINTERS_EQUAL(NULL, pDummy_ddio->privateData.FallingEdgeArgs[i]);
            }

            CHECK_FALSE(pDummy_ddio->privateData.isDebounceEnabled);

            LONGS_EQUAL(expectedSetPinCalls, setPinCallCount);
        }
    }
}

TEST(ddioTest, setVal)
{
    conf.mode = ddio_Out_PushPull;
    ddio_init((pPlatform_ddio_t)pDummy_ddio, &conf);

    ddio_setVal(pDummy_ddio, ddio_set);

    LONGS_EQUAL(pDummy_ddio->value, ddio_set);
    LONGS_EQUAL(setValCallCount, 1);
}

TEST(ddioTest, setValOnInput)
{
    conf.mode = ddio_In_NoPull;
    ddio_init((pPlatform_ddio_t)pDummy_ddio, &conf);

    ddio_setVal(pDummy_ddio, ddio_set);

    LONGS_EQUAL(setValCallCount, 0);
}

TEST(ddioTest, tasksCallvtable)
{
    conf.mode = ddio_In_NoPull;
    ddio_init((pPlatform_ddio_t)pDummy_ddio, &conf);

    ddio_tasks(pDummy_ddio);

    LONGS_EQUAL(setTaskCallCount, 1);
}

TEST(ddioTest, handleEgdeEventOnFallingEdgeCallBack)
{
    conf.mode = ddio_In_NoPull;
    ddio_init((pPlatform_ddio_t)pDummy_ddio, &conf);
    ddio_fallingEdgeCallback(pDummy_ddio, dummyEdgeCb, (void*)&conf);

    ddioTest_EmulateFallingEdgeEvent(pDummy_ddio);
    ddio_tasks(pDummy_ddio);
    ddio_tasks(pDummy_ddio);
    ddio_tasks(pDummy_ddio);

    LONGS_EQUAL(dummyEdgeCbCallCount, 1);
    POINTERS_EQUAL(&conf, dummyEdgeCbArg);
}

TEST(ddioTest, handleEgdeEventOnRisingEdgeCallBack)
{
    conf.mode = ddio_In_NoPull;
    ddio_init((pPlatform_ddio_t)pDummy_ddio, &conf);
    ddio_risingEdgeCallback(pDummy_ddio, dummyEdgeCb, (void*)&conf);

    ddioTest_EmulateRisingEdgeEvent(pDummy_ddio);
    ddio_tasks(pDummy_ddio);
    ddio_tasks(pDummy_ddio);
    ddio_tasks(pDummy_ddio);

    LONGS_EQUAL(dummyEdgeCbCallCount, 1);
    POINTERS_EQUAL(&conf, dummyEdgeCbArg);
}

TEST(ddioTest, passIntegerFromForLoop)
{
    conf.mode = ddio_In_NoPull;
    ddio_init((pPlatform_ddio_t)pDummy_ddio, &conf);
    for (int i = 0; i < 2; i++)
    {
        ddio_fallingEdgeCallback(pDummy_ddio, dummyEdgeCb, (void*)i);
    }

    ddioTest_EmulateFallingEdgeEvent(pDummy_ddio);
    ddio_tasks(pDummy_ddio);

    LONGS_EQUAL(2, dummyEdgeCbCallCount);
    LONGS_EQUAL(1, (int)(intptr_t)dummyEdgeCbArg);
}

TEST(ddioTest, taskHandleBlockingBouncingInput)
{
    const int BOUNCE_TICK_CALLS = 100;
    conf.mode = ddio_In_NoPull;
    debounceTickCallCount = 0;
    ddio_init((pPlatform_ddio_t)pDummy_ddio, &conf);
    ddio_enableDebounce(pDummy_ddio, true, BOUNCE_TICK_CALLS, debounceTick);
    ddio_risingEdgeCallback(pDummy_ddio, dummyEdgeCb, (void*)&conf);

    ddioTest_EmulateRisingEdgeEvent(pDummy_ddio);
    ddio_tasks(pDummy_ddio);

    LONGS_EQUAL(1, dummyEdgeCbCallCount);
    LONGS_EQUAL(BOUNCE_TICK_CALLS, debounceTickCallCount);
}

TEST(ddioTest, taskHandleNonBlockingBouncingInput)
{
    const int BOUNCE_TICK_CALLS = 100;
    debounceTickCallCount = 0;
    conf.mode = ddio_In_NoPull;
    ddio_init((pPlatform_ddio_t)pDummy_ddio, &conf);
    ddio_enableDebounce(pDummy_ddio, false, BOUNCE_TICK_CALLS, debounceTick);
    ddio_risingEdgeCallback(pDummy_ddio, dummyEdgeCb, (void*)&conf);

    ddioTest_EmulateRisingEdgeEvent(pDummy_ddio);
    ddio_tasks(pDummy_ddio);

    LONGS_EQUAL(0, dummyEdgeCbCallCount);
    LONGS_EQUAL(1, debounceTickCallCount);
}

TEST(ddioTest, nonBlockingBouncingInputTriggersCallback)
{
    const int BOUNCE_TICK_CALLS = 100;
    debounceTickCallCount = 0;
    conf.mode = ddio_In_NoPull;
    ddio_init((pPlatform_ddio_t)pDummy_ddio, &conf);
    ddio_enableDebounce(pDummy_ddio, false, BOUNCE_TICK_CALLS, debounceTick);
    ddio_risingEdgeCallback(pDummy_ddio, dummyEdgeCb, (void*)&conf);

    ddioTest_EmulateRisingEdgeEvent(pDummy_ddio);
    // Added an aditional ddio_task() call (BOUNCE_TICK_CALLS + 1) to verify that after finalizing
    // the debounce algorithm no further side effects are noticed and algorithm is ready for starting again.
    for (int i = 0; i < BOUNCE_TICK_CALLS + 1; i++)
    {
        ddio_tasks(pDummy_ddio);
    }

    LONGS_EQUAL(1, dummyEdgeCbCallCount);
    LONGS_EQUAL(100, debounceTickCallCount);
}

TEST(ddioTest, nonBlockingBouncingTwoInputs)
{
    const int BOUNCE_TICK_CALLS = 100;
    debounceTickCallCount = 0;
    conf.mode = ddio_In_NoPull;
    ddio_init((pPlatform_ddio_t)pDummy_ddio, &conf);
    ddio_enableDebounce(pDummy_ddio, false, BOUNCE_TICK_CALLS, debounceTick);
    ddio_risingEdgeCallback(pDummy_ddio, dummyEdgeCb, (void*)&conf);

    ddioTest_EmulateRisingEdgeEvent(pDummy_ddio);
    for (int i = 0; i < BOUNCE_TICK_CALLS + 1; i++)
    {
        ddio_tasks(pDummy_ddio);
    }

    ddioTest_EmulateRisingEdgeEvent(pDummy_ddio);
    for (int i = 0; i < BOUNCE_TICK_CALLS + 1; i++)
    {
        ddio_tasks(pDummy_ddio);
    }

    LONGS_EQUAL(2, dummyEdgeCbCallCount);
    LONGS_EQUAL(200, debounceTickCallCount);
}
