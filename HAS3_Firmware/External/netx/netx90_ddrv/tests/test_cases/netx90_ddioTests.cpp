#include "CppUTest/TestHarness.h"

extern "C" {
#include "netx90_ddio.h"
}

netx90_ddio_t netx90_ddio_td;
bool isNetx90callBackCalled;

// clang-format off
TEST_GROUP(netx90Ddio) {
    void setup()
    {
        isNetx90callBackCalled = false;
    }
    void teardown()
    {
        pio_app->pio_in_inv = 0;
    }
};

// clang-format on

TEST(netx90Ddio, InitInput)
{
    netx90_ddio_conf_t conf;
    conf.mode = ddio_In_NoPull;
    conf.eventMode = ddio_Interrupt;
    conf.pioPin = 8;
    conf.interruptPriority = 3;

    netx90_ddio_Init(&netx90_ddio_td, &conf);

    POINTERS_EQUAL(&netx90_ddio_td, &netx90_ddio_td.super); // Super is me
    LONGS_EQUAL(netx90_ddio_td.super.mode, conf.mode);
    LONGS_EQUAL(pio_app->pio_irq_raw, (1u << conf.pioPin));
    LONGS_EQUAL(pio_app->pio_irq0_msk_set, (1u << conf.pioPin));
    LONGS_EQUAL(pio_app->pio_out, 0);
    LONGS_EQUAL(pio_app->pio_oe, 0);
    LONGS_EQUAL(pio_app->pio_in, 0);
}

TEST(netx90Ddio, InitOutput)
{
    netx90_ddio_conf_t conf;
    conf.mode = ddio_Out_PushPull;
    conf.eventMode = ddio_Interrupt;
    conf.pioPin = 8;

    netx90_ddio_Init(&netx90_ddio_td, &conf);

    POINTERS_EQUAL(&netx90_ddio_td, &netx90_ddio_td.super); // Super is me
    LONGS_EQUAL(netx90_ddio_td.super.mode, conf.mode);
    LONGS_EQUAL(pio_app->pio_irq_raw, (1u << conf.pioPin));
    LONGS_EQUAL(pio_app->pio_irq0_msk_set, (1u << conf.pioPin));
    LONGS_EQUAL(pio_app->pio_out, 0);
    LONGS_EQUAL(pio_app->pio_oe, (1u << conf.pioPin));
    LONGS_EQUAL(pio_app->pio_in, 0);
}

static void callback(pDdio_t me, void* arg)
{
    isNetx90callBackCalled = true;
}

TEST(netx90Ddio, InitClearRaisesFallingEventCallBack)
{
    netx90_ddio_conf_t conf;
    conf.mode = ddio_In_NoPull;
    conf.pioPin = 8;

    netx90_ddio_Init(&netx90_ddio_td, &conf);
    ddio_fallingEdgeCallback((pDdio_t)&netx90_ddio_td, callback, 0);
    ddio_tasks((pDdio_t)&netx90_ddio_td);

    CHECK_TRUE(isNetx90callBackCalled);
}

TEST(netx90Ddio, InitSetRaisesRaisingEventCallBack)
{
    netx90_ddio_conf_t conf;
    conf.mode = ddio_In_NoPull;
    conf.pioPin = 8;
    pio_app->pio_in_inv = 1u << 8; // start as set

    netx90_ddio_Init(&netx90_ddio_td, &conf);
    ddio_risingEdgeCallback((pDdio_t)&netx90_ddio_td, callback, 0);
    ddio_tasks((pDdio_t)&netx90_ddio_td);

    CHECK_TRUE(isNetx90callBackCalled);
}
