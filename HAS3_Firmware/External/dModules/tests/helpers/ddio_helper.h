#ifndef DDIO_HELPER_H
#define DDIO_HELPER_H

#include "ddio.h"

extern int setPinCallCount;
void pin_mode_set(pPlatform_ddio_t me, ddio_Mode_t mode);

extern int setValCallCount;
void pin_value_write(pPlatform_ddio_t me, ddio_Value_t value);

extern int setTaskCallCount;
void tasks(pPlatform_ddio_t me);

extern int setUpdateValueCallCount;
ddio_Value_t updateValue(pPlatform_ddio_t me);

extern int dummyEdgeCbCallCount;
extern void* dummyEdgeCbArg;
void dummyEdgeCb(pDdio_t me, void* arg);

extern int debounceTickCallCount;
void debounceTick(pDdio_t me);

void ddioTest_EmulateRisingEdgeEvent(pDdio_t me);
void ddioTest_EmulateFallingEdgeEvent(pDdio_t me);

extern ddio_interface_t vtable;
extern pDdio_t pDummy_ddio;
extern ddio_conf_t conf;

#endif // DDIO_HELPER_H
