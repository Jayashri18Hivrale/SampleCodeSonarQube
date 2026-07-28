#include "CppUTest/TestHarness.h"

extern "C" {
#include "circularBuffer_byte.h"
}

cb_byte_t cb;
uint8_t cbBuf[8];

// clang-format off
TEST_GROUP(cbByte){
    void setup()
    {
    }
    void teardown()
    {
    }
};

// clang-format on

TEST(cbByte, cbCreated)
{

    cb_init(&cb, cbBuf, 8);

    LONGS_EQUAL(cb.read_index, 0);
    LONGS_EQUAL(cb.write_index, 0);
    LONGS_EQUAL(cb.length, 8);
    POINTERS_EQUAL(cb.buffer, cbBuf);
    CHECK_TRUE(cb_is_empty);
}

TEST(cbByte, cbWrites)
{
    cb_init(&cb, cbBuf, 8);

    for (int i = 1; i < 8; i++) CHECK_TRUE(cb_write(&cb, i));

    for (int i = 1; i < 8; i++) LONGS_EQUAL(cbBuf[i - 1], i);
    CHECK_FALSE(cb_write(&cb, 8));
}

TEST(cbByte, cbRead)
{
    cb_init(&cb, cbBuf, 8);

    for (int i = 1; i < 8; i++) cb_write(&cb, i);

    uint8_t rByte = 0xff;
    CHECK_TRUE(cb_read(&cb, &rByte));
    LONGS_EQUAL(rByte, 1);
    CHECK_TRUE(cb_read(&cb, &rByte));
    LONGS_EQUAL(rByte, 2);

    cb_write(&cb, 1);
    cb_write(&cb, 2);

    for (int i = 0; i < 7; i++) CHECK_TRUE(cb_read(&cb, &rByte));

    LONGS_EQUAL(rByte, 2);
    CHECK_FALSE(cb_read(&cb, &rByte));
}
