#include "circularBuffer_byte.h"
#include "dematic_utilities.h"
#include <assert.h>

/**
 * @brief Increments the given index by 1 and limits to < @p limit.
 *
 * @param index The index to increment.
 * @param limit Exclusive upper bound for the index
 * @return The incremented index with the mask applied..
 */
static uint32_t increment_index(uint32_t index, uint32_t limit)
{
    uint32_t incrementedIndex = index;
    incrementIndex_Rollover(incrementedIndex, 0, limit);
    return incrementedIndex;
}

/**
 * @brief Initializes the circular buffer.
 * @param me The circular buffer to initialize.
 * @param staticBuffer The static buffer to use.
 * @param size The size of the buffer.
 */
void cb_init(cb_byte_t* me, uint8_t* staticBuffer, uint32_t size)
{
    assert(0 < size);
    me->buffer = staticBuffer;
    me->length = size;
    me->read_index = 0;
    me->write_index = 0;
}

/**
 * @brief Checks if the circular buffer is empty.
 * @param me The circular buffer to check.
 * @return True if the buffer is empty, false otherwise.
 */
bool cb_is_empty(cb_byte_t* me)
{
    return me->read_index == me->write_index;
}

/**
 * @brief Writes a byte to the circular buffer.
 * @param me The circular buffer to write to.
 * @param byte The byte to write.
 * @return True if the write was successful, false if the buffer is full.
 */
bool cb_write(cb_byte_t* me, uint8_t byte)
{
    uint32_t next_write_index = increment_index(me->write_index, me->length);
    if (next_write_index == me->read_index)
    {
        return false;
    }
    me->buffer[me->write_index] = byte;
    me->write_index = next_write_index;
    return true;
}

/**
 * @brief Reads a byte from the circular buffer.
 * @param me The circular buffer to read from.
 * @param read_byte The location to store the read byte.
 * @return True if the read was successful, false if the buffer is empty.
 */
bool cb_read(cb_byte_t* me, uint8_t* read_byte)
{
    if (cb_is_empty(me))
    {
        return false;
    }
    *read_byte = me->buffer[me->read_index];
    me->read_index = increment_index(me->read_index, me->length);
    return true;
}

uint32_t cb_space_used(cb_byte_t* me)
{
    // Logic patterned on
    // https://github.com/embeddedartistry/embedded-resources/blob/ccc886a92551d1f55f5b9d65f07968361e77a25d/examples/c/circular_buffer/circular_buffer_no_modulo_threadsafe.c
    uint32_t usedSpace;
    if (me->write_index >= me->read_index)
    {
        usedSpace = me->write_index - me->read_index;
    }
    else
    {
        usedSpace = me->length + me->write_index - me->read_index;
    }

    return usedSpace;
}

uint32_t cb_space_available(cb_byte_t* me)
{
    // -1 for the "reserved" space because (write_index == read_index) means empty so we can't write the last space.
    return me->length - cb_space_used(me) - 1;
}
