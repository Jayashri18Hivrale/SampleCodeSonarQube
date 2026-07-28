#ifndef NETX90_DDIO_H
#define NETX90_DDIO_H

/**
 * @file netx90_ddio.h
 * @brief Header file for the NetX90 DDIO module.
 *
 * TODO.
 * - Add support for non interrupt operation.
 * - Add other digital input ports (not only PIO)
 *
 */

#include "ddio.h"
#include "netx90_app.h"
#include <stdint.h>

/**
 * @struct netx90_ddio_t
 * @brief Structure for the Netx90 DDIO module.
 */
typedef struct
{
    struct ddio super;           /**< Parent structure */
    pio_app_Type* pioRegister;   /**< PIO register */
    uint32_t pioPin;             /**< PIO pin */
    ddio_callback eventCallback; /**< Event callback */
} netx90_ddio_t;

/**
 * @struct netx90_ddio_conf_t
 * @brief Structure for the Netx90 DDIO configuration.
 */
typedef struct
{
    uint32_t pioPin;            /**< PIO pin */
    ddio_Mode_t mode;           /**< Mode */
    ddio_EventMode_t eventMode; /**<Event mode */
    uint32_t interruptPriority; /**< Rising/Falling interrupt priority */
} netx90_ddio_conf_t;

void netx90_ddio_Init(netx90_ddio_t* me, netx90_ddio_conf_t* conf);

void netx90_ddio_EdgeIrqCallback(netx90_ddio_t* me, ddio_callback callback);

void dPIO0_IRQHandler(void);

#endif // !NETX90_DDIO_H
