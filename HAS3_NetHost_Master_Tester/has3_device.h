#ifndef HAS3_DEVICE_H
#define HAS3_DEVICE_H

#include <stdint.h>

#define HAS3_OUT_DATA_SIZE 5
#define HAS3_IN_DATA_SIZE 48

typedef struct
{
	uint8_t header;
	uint16_t scanSequence;
	uint16_t elementStatus;
	uint8_t scanData[43];
}has3_inputCyclicData;

typedef struct
{
	uint8_t command;
	uint16_t registerId;
	uint16_t registerValue;
}has3_outputCyclicData;

typedef struct
{
	has3_inputCyclicData in;
	has3_outputCyclicData out;
}has3_cyclicData_t;

#endif