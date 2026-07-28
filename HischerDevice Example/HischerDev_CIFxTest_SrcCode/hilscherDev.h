#ifndef HILSCHER_EXAMPLE_DEVICE_H
#define HILSCHER_EXAMPLE_DEVICE_H

#include <stdint.h>

typedef struct {
    uint16_t LED_1;
    uint16_t LED_2;
    uint8_t CountDirection;
    uint8_t CountSpeed;
}Device_OutputData;

typedef struct {
    uint16_t CounterValue;
    uint16_t TempCelcius;
    uint8_t State1;
    uint16_t TempVolts;
    uint8_t State2;
    uint8_t Actuator_1;
    uint8_t Actuator_2;
}Device_InputData;


#endif