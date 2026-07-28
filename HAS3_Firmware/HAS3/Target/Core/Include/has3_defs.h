/*
 * has3_defs.h
 *
 *  Created on: 13/02/2025
 *      Author: A0084817
 */

#ifndef TARGET_CORE_INCLUDE_HAS3_DEFS_H_
#define TARGET_CORE_INCLUDE_HAS3_DEFS_H_

#include <stdint.h>

typedef struct
{
  uint8_t header;
  uint16_t scanSequence;
  uint16_t elementStatus;
  uint8_t scanData[43];
}__attribute__((__packed__)) has3_outputCyclicData;

typedef struct
{
  uint8_t command;
  uint16_t registerId;
  uint16_t registerValue;
}__attribute__((__packed__)) has3_inputCyclicData;

typedef struct
{
  has3_inputCyclicData in;
  has3_outputCyclicData out;
}__attribute__((__packed__)) has3_cyclicData_t;

#endif /* TARGET_CORE_INCLUDE_HAS3_DEFS_H_ */
