#ifndef CRC_8_BIT_H
#define CRC_8_BIT_H

/**
 * @file CRC_8_BIT_H
 * @brief This module provides an interface for calculating 8-bit CRC.
 *
 *  "CRC-8, CRC-9, and CRC-10 all perform better than Fletcher-16
 *   for code word lengths less than 128, 256, and 512 bits, respectively."
 * https://users.ece.cmu.edu/~koopman/pubs/maxino09_checksums.pdf
 *
 * The selected polynomial for this module is 0xEA which is one of the most effective,
 * according to https://users.ece.cmu.edu/~koopman/roses/dsn04/koopman04_crc_poly_embedded.pdf
 *
 */

#include <stdint.h>

// Function prototypes
uint8_t crc8bits(uint8_t* data, uint32_t len);

uint8_t crc8_twoBytes(uint8_t byte1, uint8_t byte2);

uint8_t crc8_bytePointer(uint8_t byte1, uint8_t* pbyte, uint32_t lenpByte);

#endif // !CRC_8_BIT_H
