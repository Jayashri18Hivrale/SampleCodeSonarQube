#include "ddio.h"

#include "platform_assert.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/**
 * @brief Checks if the pin mode is output.
 * @param me The ddio_t structure.
 * @return True if the pin mode is output, false otherwise.
 */
static bool isOutput(pDdio_t me)
{
    switch (me->mode)
    {
    case ddio_Out_PushPull:
    case ddio_Out_OpenDrain:
        return true;
        break;

    case ddio_In_NoPull:
    case ddio_In_PullUp:
        return false;
        break;

    case ddio_Mode_SIZE:
    default:
        break;
    }
    return false;
}

/**
 * @brief Executes the falling edge callbacks.
 * @param _me The _ddio_t structure.
 * @param me The ddio_t structure.
 */
static void fallingCallbacks(pDdio_t me)
{
    _ddio_t* _me = &me->privateData;
    if (_me->isFallingEdgeDetected)
    {
        _me->isFallingEdgeDetected = false;
        for (uint32_t i = 0; i < _me->nFallingEdgeCBs; i++)
        {
            if (_me->FallingEdgeCBs[i] != NULL)
            {
                _me->FallingEdgeCBs[i](me, _me->FallingEdgeArgs[i]);
            }
        }
    }
}

/**
 * @brief Executes the rising edge callback.
 * @param _me The _ddio_t structure.
 * @param me The ddio_t structure.
 */
static void risingCallbacks(pDdio_t me)
{
    _ddio_t* _me = &me->privateData;
    if (_me->isRisingEdgeDetected)
    {
        _me->isRisingEdgeDetected = false;
        for (uint32_t i = 0; i < _me->nRisingEdgeCBs; i++)
        {
            if (_me->RisingEdgeCBs[i] != NULL)
            {
                _me->RisingEdgeCBs[i](me, _me->RisingEdgeArgs[i]);
            }
        }
    }
}

/**
 * @brief Clears the rising edge callback.
 * @param _me The _ddio_t structure.
 */
static void clearRisingCallbacks(_ddio_t* _me)
{
    _me->nRisingEdgeCBs = 0;
    for (uint32_t i = 0; i < DDIO_MAX_CALLBACKS_ALLOWED; i++)
    {
        _me->RisingEdgeCBs[i] = NULL;
        _me->RisingEdgeArgs[i] = NULL;
    }
}

/**
 * @brief Clears the falling edge callback.
 * @param _me The _ddio_t structure.
 */
static void clearFallingCallbacks(_ddio_t* _me)
{
    _me->nFallingEdgeCBs = 0;
    for (uint32_t i = 0; i < DDIO_MAX_CALLBACKS_ALLOWED; i++)
    {
        _me->FallingEdgeCBs[i] = NULL;
        _me->FallingEdgeArgs[i] = NULL;
    }
}

/**
 * @brief Triggers the rising edge callback.
 *
 * To be used in specific peripheral implementations.
 *
 * @param me The ddio_t structure.
 */
static void ddio_triggerRisingEdgeCallBack(pDdio_t me)
{
    _ddio_t* _me = &me->privateData;
    _me->isFallingEdgeDetected = false;
    _me->isRisingEdgeDetected = true;
}

/**
 * @brief Triggers the falling edge callback.
 *
 * To be used in specific peripheral implementations.
 *
 * @param me The ddio_t structure.
 */
static void ddio_triggerFallingEdgeCallBack(pDdio_t me)
{
    _ddio_t* _me = &me->privateData;
    _me->isRisingEdgeDetected = false;
    _me->isFallingEdgeDetected = true;
}

/**
 * @brief Debounces the input value.
 *
 * This function checks the current value of the input and increments the debounce counter if the value remains stable.
 * If the debounce counter reaches the target count, the function returns true, indicating a stable input.
 *
 * @param me Pointer to the ddio structure.
 * @return true if the input value is stable for the required number of ticks, false otherwise.
 */
static bool debounce_algorithm(pDdio_t me)
{
    _ddio_t* _me = &me->privateData;
    ddio_DebounceData_t* debounce = &_me->debounceData;
    bool ret = false;
    debounce->currentValue = _me->raw_value;
    _me->raw_value = _me->vtable->pin_value_read((pPlatform_ddio_t)me);
    if (debounce->pf_platformIncrement)
    {
        debounce->pf_platformIncrement(me);
    }
    if (debounce->currentValue == _me->raw_value)
    {
        if (debounce->currentCount >= debounce->targetCount)
        {
            me->value = _me->raw_value;
            debounce->currentCount = 0;
            ret = true;
        }
    }
    else
    {
        debounce->currentValue = _me->raw_value;
        debounce->currentCount = 0;
    }
    return ret;
}

/**
 * @brief Handles non-blocking debounce logic.
 *
 * This function manages the debounce process without blocking the execution. It checks for edge detection and
 * initiates the debounce process if necessary. If the debounce is successful, it handles the edge event.
 *
 * @param me Pointer to the Ddio structure.
 */
static void nonBlockingDebounce(pDdio_t me)
{
    _ddio_t* _me = &me->privateData;
    ddio_DebounceData_t* debounce = &_me->debounceData;
    if ((!_me->isRisingEdgeDetected && !_me->isFallingEdgeDetected) && !debounce->isActive)
    {
        return;
    }
    debounce->isActive = true;
    _me->isFallingEdgeDetected = false;
    _me->isRisingEdgeDetected = false;
    if (debounce_algorithm(me))
    {
        ddio_handleEdgeEvent((pPlatform_ddio_t)me, true);
        debounce->isActive = false;
    }
}

/**
 * @brief Handles non-blocking debounce logic.
 *
 * This function manages the debounce process without blocking the execution. It checks for edge detection and
 * initiates the debounce process if necessary. If the debounce is successful, it handles the edge event.
 *
 * @param me Pointer to the ddio structure.
 */
static void blockingDebounce(pDdio_t me)
{
    _ddio_t* _me = &me->privateData;
    if ((!_me->isRisingEdgeDetected && !_me->isFallingEdgeDetected))
    {
        return;
    }
    ddio_DebounceData_t* debounce = &_me->debounceData;
    while (!debounce_algorithm(me))
    {
        if (!debounce->pf_platformIncrement)
        {
            ddio_incrementDebounceTicks(me, 1);
        }
    }
    ddio_handleEdgeEvent((pPlatform_ddio_t)me, true);
}

/**
 * @brief Perform polling tasks for the given DDIO instance.
 *
 * This function checks if the event mode is set to polling. If so, it reads the current pin value
 * and compares it with the previous value. If the value has changed, it handles the edge event.
 *
 * @param me Pointer to the ddio instance.
 */
static void polling_tasks(pDdio_t me)
{
    _ddio_t* _me = &me->privateData;
    if (me->eventMode == ddio_Polling)
    {
        ddio_Value_t prev = _me->raw_value;
        _me->raw_value = _me->vtable->pin_value_read((pPlatform_ddio_t)me);
        if (_me->raw_value != prev)
        {
            ddio_handleEdgeEvent((pPlatform_ddio_t)me, true);
        }
    }
}

/**
 * @brief Perform debounce tasks for the given ddio instance.
 *
 * This function checks if debounce is enabled. If enabled, it performs either blocking or non-blocking
 * debounce based on the debounce mode. If debounce is not enabled, it directly assigns the raw value
 * to the instance's value.
 *
 * @param me Pointer to the ddio instance.
 */
static void debounce_tasks(pDdio_t me)
{
    _ddio_t* _me = &me->privateData;
    ddio_DebounceData_t* debounce = &_me->debounceData;
    if (_me->isDebounceEnabled)
    {
        if (debounce->isBlockingMode)
        {
            blockingDebounce(me);
        }
        else
        {
            nonBlockingDebounce(me);
        }
    }
    else
    {
        me->value = _me->raw_value;
    }
}

/**
 * @brief Sets the pin mode.
 * @param me The ddio_t structure.
 * @param mode The mode to set.
 */
void ddio_setPinMode(pDdio_t me, ddio_Mode_t mode)
{
    _ddio_t* _me = &me->privateData;
    if (_me->vtable->pin_mode_set != NULL)
    {
        _me->vtable->pin_mode_set((pPlatform_ddio_t)me, mode);
    }
    me->mode = mode;
}

/**
 * @brief Sets the value of the pin.
 * @param me The ddio_t structure.
 * @param value The value to set.
 */
void ddio_setVal(pDdio_t me, ddio_Value_t value)
{
    _ddio_t* _me = &me->privateData;
    if (!isOutput(me))
    {
        return;
    }
    if (_me->vtable->pin_value_write != NULL)
    {
        _me->vtable->pin_value_write((pPlatform_ddio_t)me, value);
    }
    me->value = value;
}

/**
 * @brief Gets the value of the pin.
 *
 * Perform an update of status variables.
 *
 * @param me The ddio_t structure.
 * @return The value of the pin.
 */
ddio_Value_t ddio_getVal(pDdio_t me)
{
    _ddio_t* _me = &me->privateData;
    _me->raw_value = _me->vtable->pin_value_read((pPlatform_ddio_t)me);
    me->value = _me->raw_value;
    return me->value;
}

/**
 * @brief Executes the tasks for the ddio_t structure.
 *
 * To be placed within an infinite loop or within a RTOS task.
 * Triggers the callback and updates the current value of the
 * ddio_t when no interrupt based implementation is provided.
 *
 *
 * @param me The ddio_t structure.
 */
void ddio_tasks(pDdio_t me)
{
    _ddio_t* _me = &me->privateData;

    _me->vtable->Tasks((pPlatform_ddio_t)me); // runs special configured tasks if configured

    polling_tasks(me);  // generic polling
    debounce_tasks(me); // Debounce functions modify edge flags in `_me` so that we only execute rising/falling callbacks after debouncing

    // for PE signals, ISR sets/clear flag, callbacks below handle the flag
    risingCallbacks(me);
    fallingCallbacks(me);
}

/**
 * @brief Sets the falling edge callback.
 * @param me The ddio_t structure.
 * @param callback The callback function.
 * @param arg The arguments for the callback function.
 */
void ddio_fallingEdgeCallback(pDdio_t me, ddio_callback callback, void* arg)
{
    _ddio_t* _me = &me->privateData;
    if (_me->nFallingEdgeCBs >= DDIO_MAX_CALLBACKS_ALLOWED)
    {
        return;
    }
    _me->FallingEdgeCBs[_me->nFallingEdgeCBs] = callback;
    _me->FallingEdgeArgs[_me->nFallingEdgeCBs] = arg;
    _me->nFallingEdgeCBs++;
}

/**
 * @brief Sets the rising edge callback.
 * @param me The ddio_t structure.
 * @param callback The callback function.
 * @param arg The arguments for the callback function.
 */
void ddio_risingEdgeCallback(pDdio_t me, ddio_callback callback, void* arg)
{
    _ddio_t* _me = &me->privateData;
    if (_me->nRisingEdgeCBs >= DDIO_MAX_CALLBACKS_ALLOWED)
    {
        return;
    }
    _me->RisingEdgeCBs[_me->nRisingEdgeCBs] = callback;
    _me->RisingEdgeArgs[_me->nRisingEdgeCBs] = arg;
    _me->nRisingEdgeCBs++;
}

/**
 * @brief Enables the blockingDebounce algorithm for a specific ddio_t instance.
 *
 * This function enables debouncing for the provided ddio_t instance, setting a
 * targetCount value that determines how many tick increments are required for a stable
 * input state. The actual Tick count has to be incremented by the user by using ddio_incrementDebounceTicks()
 * function, mostlikely within the platformIncrementFunction.
 *
 * @param me The ddio_t structure instance to enable debouncing on.
 * @param targetCount The number of consistent reads required to consider the input stable.
 * @param platformIncrementFunction Platform specific function used to handle the debounce currentCount. If a NULL
 *                                  function is passed, currentCount is incremented once at every call of ddio_Task() function.
 */
void ddio_enableDebounce(pDdio_t me, bool isBlockingMode, uint32_t targetCount, ddio_debounceIncrement platformIncrementFunction)
{
    _ddio_t* _me = &me->privateData;
    ddio_DebounceData_t* debounce = &_me->debounceData;
    _me->isDebounceEnabled = true;
    debounce->isBlockingMode = isBlockingMode;
    debounce->targetCount = targetCount;
    debounce->pf_platformIncrement = platformIncrementFunction;
    debounce->currentCount = 0;
}

/**
 * @brief Disables the blockingDebounce algorithm for a specific ddio_t instance.
 *
 * This function disables debouncing for the provided ddio_t instance. After calling
 * this function, input changes will be immediately reflected without waiting for
 * blockingDebounce stabilization. Use this function to bypass debouncing when instantaneous
 * input response is required.
 *
 * @param me The ddio_t structure instance to disable debouncing on.
 */
void ddio_disableDebounce(pDdio_t me)
{
    _ddio_t* _me = &me->privateData;
    _me->isDebounceEnabled = false;
}

/**
 * @brief Increments the debounce tick count for a specific ddio_t instance.
 *
 * This function increments the current debounce tick count by the specified number of ticks.
 * It is typically called within the platformIncrementFunction to update the debounce status.
 *
 * @param me The ddio_t structure instance whose debounce tick count is to be incremented.
 * @param ticksToincrement The number of ticks to add to the current debounce count.
 */
void ddio_incrementDebounceTicks(pDdio_t me, uint32_t ticksToincrement)
{
    assert(ticksToincrement > 0);
    _ddio_t* _me = &me->privateData;
    ddio_DebounceData_t* debounce = &_me->debounceData;
    debounce->currentCount += ticksToincrement;
}

/**
 * @brief Initializes the ddio_t structure.
 *
 * To be used in specific peripheral implementations.
 *
 * @param me The ddio_t structure to initialize.
 * @param c The configuration for the ddio_t structure.
 */
void ddio_init(pPlatform_ddio_t platform, ddio_conf_t* c)
{
    assert(platform);
    assert(c);
    assert(c->vTable);

    pDdio_t me = &platform->super;
    _ddio_t* _me = &(me->privateData);

    // pDdio_t.privateData
    // vtable needs to be set before we can call ddio_setPinMode()
    _me->vtable = c->vTable;
    clearRisingCallbacks(_me);
    clearFallingCallbacks(_me);
    ddio_disableDebounce(me);

    // pDdio_t.value
    _me->raw_value = ddio_unknown;
    // pDdio_t.mode
    assert(_me->vtable);
    ddio_setPinMode(me, c->mode);

    // pDdio_t.eventMode
    me->eventMode = c->eventMode;
}

/**
 * @brief Handles the edge (rising/falling) events.
 *
 * This function has to be used in specific peripheral implementations.
 *
 * @param me The ddio_t where the edge event is detected.
 * @param skipUpdate If true, current _me->raw_value has already the proper state after the edge event.
 * If false, _me->raw_value is updated within this function and current _me->raw_value has the state
 * before the edge event. The pDdio_t me->value is updated within the task function before trigger the
 * rising/falling interrupts.
 */
void ddio_handleEdgeEvent(pPlatform_ddio_t platform, bool skipUpdate)
{
    pDdio_t me = &platform->super;
    _ddio_t* _me = &(me->privateData);
    if (!skipUpdate)
    {
        _me->raw_value = _me->vtable->pin_value_read(platform);
    }
    switch (_me->raw_value)
    {
    case ddio_set:
        ddio_triggerRisingEdgeCallBack(me);
        break;
    case ddio_clear:
        ddio_triggerFallingEdgeCallBack(me);
        break;
    default:
        break;
    }
}

/**
 * @brief Retrieves the raw value from a specific ddio_t instance.
 *
 * This function returns the current raw value stored in the ddio_t instance's private data.
 *
 * @param platform The platform-specific ddio_t structure instance from which to retrieve the raw value.
 * @return The raw value stored in the ddio_t instance.
 */
ddio_Value_t ddio_getRawValue(pPlatform_ddio_t platform)
{
    pDdio_t me = &platform->super;
    _ddio_t* _me = &(me->privateData);
    return _me->raw_value;
}

/**
 * @brief Sets the raw value for a specific ddio_t instance.
 *
 * This function updates the raw value stored in the ddio_t instance's private data.
 *
 * @param platform The platform-specific ddio_t structure instance for which to set the raw value.
 * @param value The raw value to be set in the ddio_t instance.
 */
void ddio_setRawValue(pPlatform_ddio_t platform, ddio_Value_t value)
{
    pDdio_t me = &platform->super;
    _ddio_t* _me = &(me->privateData);
    _me->raw_value = value;
}
