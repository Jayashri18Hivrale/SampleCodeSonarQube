#ifndef CIRCULAR_BUFFER_BYTE_H
#define CIRCULAR_BUFFER_BYTE_H

/**
 * @file CIRCULAR_BUFFER_BYTE_H
 * @brief This module provides an interface for handling circular buffers of bytes.
 */

#include <stdint.h>
#include <stdbool.h>

/**
 * @brief A structure for a circular buffer of bytes.
 */
typedef struct cb_byte_t
{
    uint8_t* buffer;      /**< Pointer to the buffer. The size of the buffer must be a power of 2. */
    uint32_t length;      /**< Size of buffer.. */
    uint32_t read_index;  /**< The read index for the buffer. */
    uint32_t write_index; /**< The write index for the buffer. */
} cb_byte_t;

// Function prototypes
void cb_init(cb_byte_t* me, uint8_t* staticBuffer, uint32_t size);
bool cb_is_empty(cb_byte_t* me);
bool cb_write(cb_byte_t* me, uint8_t byte);
bool cb_read(cb_byte_t* me, uint8_t* read_byte);
uint32_t cb_space_used(cb_byte_t* me);
uint32_t cb_space_available(cb_byte_t* me);

#endif // !CIRCULAR_BUFFER_BYTE_H
