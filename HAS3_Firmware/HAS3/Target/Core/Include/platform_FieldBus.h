/*
 * platform_FieldBus.h
 *
 *  Created on: 13/02/2025
 *      Author: A0084817
 */

#ifndef TARGET_CORE_INCLUDE_PLATFORM_FIELDBUS_H_
#define TARGET_CORE_INCLUDE_PLATFORM_FIELDBUS_H_

#include "has3_defs.h"
#include "nx90Cifx.h"

uint32_t fieldBus_Init(N90Cifx_t * cifxDriver, has3_cyclicData_t * has3Data);

void fieldBus_Tasks(void);

#endif /* TARGET_CORE_INCLUDE_PLATFORM_FIELDBUS_H_ */
