#include "netx90_ddio.h"

#include "netx90_defs.h"

#include <assert.h>

#define PIO pio_app

static uint32_t nConfiguredPio = 0;

static netx90_ddio_t* configuredPio[29] = { 0 };

static void netx90_ddio_setInvRegister(netx90_ddio_t* me);

/**
 * @brief Sets the mode of the pin.
 *
 * Vtable function for super ddio.h
 *
 * If mode is set as any type of output. The default init value of will be cleared.
 *
 * @param platform Pointer to the parent structure.
 * @param mode The mode to set.
 */
static void pin_mode_set(pPlatform_ddio_t platform, ddio_Mode_t mode)
{
    netx90_ddio_t* me = (netx90_ddio_t*)platform;
    assert(mode != ddio_In_PullUp);
    assert(mode != ddio_Out_OpenDrain);
    switch (mode)
    {
    case ddio_In_NoPull:
        CLEAR_BIT_POS(me->pioRegister->pio_out, me->pioPin);
        CLEAR_BIT_POS(me->pioRegister->pio_oe, me->pioPin);
        break;
    case ddio_Out_PushPull:
        CLEAR_BIT_POS(me->pioRegister->pio_out, me->pioPin);
        SET_BIT_POS(me->pioRegister->pio_oe, me->pioPin);
        break;
    default:
        break;
    }
}

/**
 * @brief Sets the value of the pin.
 *
 * Vtable function for supper ddio.h
 *
 * @param platform Pointer to the parent structure.
 * @param value The value to set.
 */
static void pin_value_write(pPlatform_ddio_t platform, ddio_Value_t value)
{
    netx90_ddio_t* me = (netx90_ddio_t*)platform;
    if (value == ddio_set)
    {
        SET_BIT_POS(me->pioRegister->pio_out, me->pioPin);
    }
    else
    {
        CLEAR_BIT_POS(me->pioRegister->pio_out, me->pioPin);
    }
}

/**
 * @brief Executes tasks.
 *
 * Vtable function for supper ddio.h
 *
 * @param platform Pointer to the parent structure.
 */
static void tasks(pPlatform_ddio_t platform)
{
}

/**
 * @brief Updates the value of an input pin.
 *
 * Considers the pio inv register status.
 *
 * @param me pointer to the pin to be updated.
 */
static ddio_Value_t getInputvalue(netx90_ddio_t* me)
{
    bool pinSet = (0 < (me->pioRegister->pio_in & (1u << me->pioPin)));
    bool pinInverted = (0 < (me->pioRegister->pio_in_inv & (1u << me->pioPin)));
    // Get the actual state by accounting for pin inversion.
    ddio_Value_t actual = (pinSet ^ pinInverted) ? ddio_set : ddio_clear;
    // if actual value is different than previous one and we are using interrupt mode
    if (me->super.eventMode == ddio_Interrupt && ddio_getRawValue((pPlatform_ddio_t)me) != actual)
    {
        ddio_setRawValue((pPlatform_ddio_t)me, !(bool)(me->pioRegister->pio_in_inv & (1u << me->pioPin)));
        // Set in_inv register for next edge event
        netx90_ddio_setInvRegister(me);
    }
    return actual;
}

/**
 * @brief Updates the actual value of the pin.
 *
 * Vtable function for super ddio.h
 *
 * @param platform Pointer to the netx90_ddio_t structure.
 */
static ddio_Value_t pin_value_read(pPlatform_ddio_t platform)
{
    netx90_ddio_t* me = (netx90_ddio_t*)platform;
    switch (me->super.mode)
    {
    case ddio_In_NoPull:
        return getInputvalue(me);
        break;
    case ddio_Out_PushPull:
        return (me->pioRegister->pio_out & (1u << me->pioPin)) != 0;
        break;
    default:
        return ddio_set;
        break;
    }
}

/**
 * @brief Table of function pointers for the interface.
 */
ddio_interface_t nex90_ddio_vtable = { pin_mode_set, pin_value_write, pin_value_read, tasks };

/**
 * @brief Sets the inv register value based on the actual
 * state of the ddio element.
 *
 * This function is pertinent only for interrupt operations. It modifies the 'in_inv' register bit,
 * enabling the generation of the appropriate next edge interrupt based on the actual value.
 * The Netx90 PIO peripheral has a single edge interrupt to manage this behavior. This function provides
 * the mechanism to select the interrupt-triggering edge.
 *
 * @param me pointer to the pin.
 */
static void netx90_ddio_setInvRegister(netx90_ddio_t* me)
{
    if (ddio_getRawValue((pPlatform_ddio_t)me) == ddio_set)
    {
        SET_BIT_POS(me->pioRegister->pio_in_inv, me->pioPin);
    }
    else
    {
        CLEAR_BIT_POS(me->pioRegister->pio_in_inv, me->pioPin);
    }
}

/**
 * @brief Handles interrupts for the netx90_ddio_t structure.
 *
 * me->super.value value is updated based solely on pio_in_inv bit value which determines
 * the edge that triggered the interrupt. This allows a reduced interrupt handling time
 * and improves the performance under noisy/bouncing conditions.
 *
 * @param me Pointer to the netx90_ddio_t structure.
 */
static void netx90_ddio_irq(netx90_ddio_t* me)
{
    if ((me->pioRegister->pio_irq0_masked & (1 << me->pioPin)) != 0)
    {
        ddio_setRawValue((pPlatform_ddio_t)me, !(bool)(me->pioRegister->pio_in_inv & (1u << me->pioPin)));
        netx90_ddio_setInvRegister(me);
        me->pioRegister->pio_irq_raw = (1u << me->pioPin);
        ddio_handleEdgeEvent((pPlatform_ddio_t)me, true);
        if (me->eventCallback)
        {
            me->eventCallback(&me->super, 0);
        }
    }
}

/**
 * @brief Initializes the netx90_ddio_t structure.
 *
 * If configuration mode is set as any type of output. The default init value
 * of it will be cleared.
 *
 * This function will raise a callback request on the actual state that
 * will be executed the next time ddio_tasks function is called using @p me
 * as argument.
 *
 * @param me Pointer to the netx90_ddio_t structure.
 * @param conf Pointer to the configuration structure.
 */
void netx90_ddio_Init(netx90_ddio_t* me, netx90_ddio_conf_t* conf)
{
    ddio_conf_t conf_super;
    conf_super.mode = conf->mode;
    conf_super.eventMode = conf->eventMode;
    conf_super.vTable = &nex90_ddio_vtable;
    me->pioPin = conf->pioPin;
    if (!me->pioRegister)
        me->pioRegister = PIO; // TODO. Implement for other IO pins
    ddio_init((pPlatform_ddio_t)(&(me->super)), &conf_super);
    configuredPio[nConfiguredPio] = me;
    nConfiguredPio++;
    if (conf->eventMode == ddio_Interrupt)
    {
        // Configure edge interrupt
        NVIC_DisableIRQ(pio0_app_IRQn);
        SET_BIT_POS(me->pioRegister->pio_edge_event, me->pioPin);
        SET_BIT_POS(me->pioRegister->pio_irq_raw, me->pioPin);
        SET_BIT_POS(me->pioRegister->pio_irq0_msk_set, me->pioPin);
        // Initialize interrupts
        NVIC_SetPriority(pio0_app_IRQn, conf->interruptPriority);
        NVIC_ClearPendingIRQ(pio0_app_IRQn);
        NVIC_EnableIRQ(pio0_app_IRQn);
    }
    // Init callback
    ddio_handleEdgeEvent((pPlatform_ddio_t)me, false);
}

/**
 * @brief Handles PIO0 interrupts.
 *
 * To be placed within the vector table.
 */
void dPIO0_IRQHandler(void)
{
    for (uint32_t i = 0; i < nConfiguredPio; i++)
    {
        netx90_ddio_irq(configuredPio[i]);
    }
}

/**
 * @brief Assigns an interrupt callback handler when an edge transition is detected
 *
 * This function provides a way to execute custom application related code when an edge
 * interrupt is detected. Passed callback function should be as small as possible and
 * care has to be taken, since this code will be executed within the interrupt handler.
 *
 * @param me Pointer to the netx90_ddio_t structure.
 * @param callback Custom callback function.
 */
void netx90_ddio_EdgeIrqCallback(netx90_ddio_t* me, ddio_callback callback)
{
    me->eventCallback = callback;
}
