#ifndef DDIO_H
#define DDIO_H

#include <stdbool.h>
#include <stdint.h>

/**
 * @brief This module provides an interface for a DDIO (Dematic digital Input/Output).
 *        It includes functions for setting and getting values, handling tasks, and managing callbacks.
 */

#define DDIO_MAX_CALLBACKS_ALLOWED 3 /**< Maximum number of callbacks allowed per event.*/

///\cond
/**
 * This typedef won't be documented, and it will be excluded from code analysis.
 */
typedef struct platform_ddio* pPlatform_ddio_t;

typedef struct ddio* pDdio_t;

///\endcond

/**
 * @brief Callback function type.
 */
typedef void (*ddio_callback)(pDdio_t me, void* arg);
typedef void (*ddio_debounceIncrement)(pDdio_t me);

/**
 * @brief Enum for the type of digital Pin Mode.
 */
typedef enum
{
    ddio_In_NoPull,     /**< Digital input (nothing else)*/
    ddio_In_PullUp,     /**< Digital input with internal pull-up resistor*/
    ddio_Out_PushPull,  /**< Digital output self powered*/
    ddio_Out_OpenDrain, /**< Digital output at open collector/drain*/
    ddio_Mode_SIZE
} ddio_Mode_t;

/**
 * @brief Enum for the digital value.
 */
typedef enum
{
    ddio_clear = 0,   /**< Clear value*/
    ddio_set = 1,     /**< Set value*/

    ddio_unknown = 2, /**< Initialization value*/
} ddio_Value_t;

/**
 * @brief debounce handling data.
 */
typedef struct
{
    bool isActive;                               /**< Currenlt debouncing flag */
    bool isBlockingMode;                         /**< Debounce mode flag. */
    ddio_debounceIncrement pf_platformIncrement; /**< Debounce increment function. */
    ddio_Value_t currentValue;                   /**< Debounce value. */
    uint32_t targetCount;                        /**< Debounce set value. */
    uint32_t currentCount;
} ddio_DebounceData_t;

#include "ddio_private.h"

/**
 * @brief DDIO structure.
 */
struct ddio
{
    ddio_Value_t value;         /**< Current status of the instance*/
    ddio_Mode_t mode;           /**< Current operating mode*/
    ddio_EventMode_t eventMode; /**< The event mode for the ddio. */
    _ddio_t privateData;        /**< Private data*/
};

/** @brief Generic platform-specific ddio structure.
 *
 * Only to be used as a parameter type, intended to:
 *   - Define the expected structure of platform-specific ddios (`super` as the first field).
 *   - Allow abstract handling from the ddio superclass, particularly for callbacks.
 */
struct platform_ddio
{
    struct ddio super;
    // Platform-specific fields follow.
};

// Function prototypes
void ddio_setPinMode(pDdio_t me, ddio_Mode_t mode);

void ddio_setVal(pDdio_t me, ddio_Value_t value);

ddio_Value_t ddio_getVal(pDdio_t me);

void ddio_tasks(pDdio_t me);

void ddio_fallingEdgeCallback(pDdio_t me, ddio_callback callback, void* arg);

void ddio_risingEdgeCallback(pDdio_t me, ddio_callback callback, void* arg);

void ddio_enableDebounce(pDdio_t me, bool isBlockingMode, uint32_t targetCount, ddio_debounceIncrement platformIncrementFunction);

void ddio_disableDebounce(pDdio_t me);

void ddio_incrementDebounceTicks(pDdio_t me, uint32_t ticksToincrement);

#endif // !DDIO_H
