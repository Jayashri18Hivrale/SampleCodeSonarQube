#ifndef DDIO_PRIVATE_H
#define DDIO_PRIVATE_H

#include "ddio.h"

/**
 * @brief An enumeration of the available ddio event modes.
 */
typedef enum
{
    ddio_Interrupt, /**< Interrupt event mode */
    ddio_Polling,   /**< Polling event mode */
    ddio_EventMode_SIZE
} ddio_EventMode_t;

/**
 * @brief Interface for the DDIO.
 *        Contains function pointers for setting pin mode, setting value, and executing tasks.
 */
typedef struct ddio_interface_t
{
    void (*pin_mode_set)(pPlatform_ddio_t me, ddio_Mode_t mode);      /**< Function to set the pin mode. */
    void (*pin_value_write)(pPlatform_ddio_t me, ddio_Value_t value); /**< Function to write the value. */
    ddio_Value_t (*pin_value_read)(pPlatform_ddio_t me);              /**< Function to read value. */
    void (*Tasks)(pPlatform_ddio_t me);                               /**< Function to execute the tasks. */
} ddio_interface_t;

/**
 * @brief Private data for the DDIO.
 *        Contains the virtual table, callbacks for rising and falling edges, and flags for edge detection.
 */
typedef struct _ddio_t
{
    ddio_interface_t* vtable;                                 /**< Virtual table. */
    ddio_callback FallingEdgeCBs[DDIO_MAX_CALLBACKS_ALLOWED]; /**< Falling edge callbacks. */
    ddio_callback RisingEdgeCBs[DDIO_MAX_CALLBACKS_ALLOWED];  /**< Rising edge callbacks. */
    void* FallingEdgeArgs[DDIO_MAX_CALLBACKS_ALLOWED];        /**< Arguments for the falling edge callbacks. */
    void* RisingEdgeArgs[DDIO_MAX_CALLBACKS_ALLOWED];         /**< Arguments for the rising edge callbacks. */
    uint32_t nFallingEdgeCBs;                                 /**< Number of falling edge callbacks. */
    uint32_t nRisingEdgeCBs;                                  /**< Number of rising edge callbacks. */
    bool isRisingEdgeDetected;                                /**< Flag for rising edge detection. */
    bool isFallingEdgeDetected;                               /**< Flag for falling edge detection. */
    bool isDebounceEnabled;                                   /**< Debounce enabled flag. */
    ddio_Value_t raw_value;                                   /**< Newest status value at platform. */
    ddio_DebounceData_t debounceData;                         /**< Debounce data. */
} _ddio_t;

/**
 * @brief Configuration for the DDIO.
 *        Contains the virtual table and the mode.
 */
typedef struct ddio_conf_t
{
    ddio_interface_t* vTable;   /**< Virtual table. */
    ddio_Mode_t mode;           /**< Operating mode. */
    ddio_EventMode_t eventMode; /**< Event mode. */
} ddio_conf_t;

// platform specific implementations
void ddio_init(pPlatform_ddio_t me, ddio_conf_t* c);

void ddio_handleEdgeEvent(pPlatform_ddio_t me, bool skipUpdate);

ddio_Value_t ddio_getRawValue(pPlatform_ddio_t platform);

void ddio_setRawValue(pPlatform_ddio_t platform, ddio_Value_t value);

#endif // !DDIO_PRIVATE_H
