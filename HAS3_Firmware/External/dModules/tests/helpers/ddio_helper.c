#include "ddio_helper.h"

struct ddio td;
pDdio_t pDummy_ddio = &td;
ddio_conf_t conf;
ddio_interface_t vtable = { .pin_mode_set = pin_mode_set, .pin_value_write = pin_value_write, .pin_value_read = updateValue, .Tasks = tasks };

int setPinCallCount;

void pin_mode_set(pPlatform_ddio_t me, ddio_Mode_t mode)
{
    setPinCallCount++;
}

int setValCallCount;

void pin_value_write(pPlatform_ddio_t me, ddio_Value_t value)
{
    setValCallCount++;
}

int setTaskCallCount;

void tasks(pPlatform_ddio_t me)
{
    setTaskCallCount++;
}

int setUpdateValueCallCount;

ddio_Value_t updateValue(pPlatform_ddio_t me)
{
    setUpdateValueCallCount++;
    return ddio_getRawValue(me);
}

void* dummyEdgeCbArg;
int dummyEdgeCbCallCount;

void dummyEdgeCb(pDdio_t me, void* arg)
{
    dummyEdgeCbCallCount++;
    dummyEdgeCbArg = arg;
}

void ddioTest_EmulateRisingEdgeEvent(pDdio_t me)
{
    _ddio_t* _me = &me->privateData;
    _me->raw_value = ddio_set;
    ddio_handleEdgeEvent((pPlatform_ddio_t)me, true);
}

void ddioTest_EmulateFallingEdgeEvent(pDdio_t me)
{
    _ddio_t* _me = &me->privateData;
    _me->raw_value = ddio_clear;
    ddio_handleEdgeEvent((pPlatform_ddio_t)me, true);
}

int debounceTickCallCount;

void debounceTick(pDdio_t me)
{
    ddio_incrementDebounceTicks(me, 1);
    debounceTickCallCount++;
}
