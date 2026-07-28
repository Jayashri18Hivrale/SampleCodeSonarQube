#include "crc8bit.h"
#define CRC_POLINOMIAL 0xEA

/**
 * @brief Calculates the 8-bit CRC of the given data.
 * @param data The data to calculate the CRC for.
 * @param len The length of the data.
 * @return The calculated 8-bit CRC.
 */
uint8_t crc8bits(uint8_t* data, uint32_t len)
{
    uint8_t crc = 0xff;
    uint32_t i, j;
    for (i = 0; i < len; i++)
    {
        crc ^= data[i];
        for (j = 0; j < 8; j++)
        {
            if ((crc & 0x80) != 0)
                crc = (uint8_t)((crc << 1) ^ CRC_POLINOMIAL);
            else
                crc <<= 1;
        }
    }
    return crc;
}

/**
 * @brief Calculates the 8-bit CRC for a byte and a byte array.
 *
 * Used to ease the calculation when the crc is spread in a byte and in a pointer.
 *
 * @param byte1 The first byte.
 * @param pbyte The byte array.
 * @param lengthByte The length of the byte array.
 * @return The calculated 8-bit CRC. Returns 0xFF if the length of the byte array is greater than 10.
 */
uint8_t crc8_bytePointer(uint8_t byte1, uint8_t* pbyte, uint32_t lengthByte)
{
    if (lengthByte > 10)
        return 0xFF;
    uint8_t data[11] = { byte1 };
    for (uint32_t i = 0; i < lengthByte; i++) data[i + 1] = *(pbyte + i);
    return crc8bits(data, lengthByte + 1);
}

/**
 * @brief Calculates the 8-bit CRC for two bytes.
 *
 * Function mainly developed for TDD purposes.
 *
 * @param byte1 The first byte.
 * @param byte2 The second byte.
 * @return The calculated 8-bit CRC.
 */
uint8_t crc8_twoBytes(uint8_t byte1, uint8_t byte2)
{
    uint8_t data[2] = { byte1, byte2 };
    return crc8bits(data, 2);
}
