/*
 * has3_pn_config.c
 *
 *  Created on: Nov 6, 2024
 *      Author: A0084817
 */
#include "has3_pn_config.h"

PNS_CONFIGURATION_DATA_T * configureHas3SlavePN()
{
   uint32_t numSubmodulesForAPI[] = {6};

   PNS_CONFIGURATION_DATA_T * data = PNSC_InitData(1, numSubmodulesForAPI, 0x00003031);

   // Set system parameters
   PNSC_SetSystemParameters(PNS_IF_SYSTEM_START_AUTO_START |
                            PNS_IF_SYSTEM_STACK_HANDLE_I_M_ENABLED |
                            PNS_IF_SYSTEM_ARDY_WOUT_APPL_REG_ENABLED |
                            PNS_IF_SYSTEM_NAME_IP_HANDLING_BY_STACK_ENABLED, 1000);

   // Set device identification information
   PNSC_SetDeviceIdentification(DEMATIC_VENDOR_ID, 0x000B);

   // Set name, type, and device type for the station
   PNSC_SetNameAndType((const uint8_t *)"has-3", (const uint8_t *)"HAS3");

   // Set firmware version information
   PNSC_SetSoftwareVersionInfo(0, 0, 6, 'V');

   // Add cyclic input and output submodules for API 0
   PNSC_AddCyclicInputSubmodule(0, 5);  // Data From HAS

   PNSC_AddModule(0);

   PNSC_AddCyclicOutputSubmodule(0, 48);  // Data To HAS

   return data;
}


PNS_CONFIGURATION_DATA_T * configureHilscherSlaveExamplePN()
{
  uint32_t numSubmodulesForAPI[] = {6};

     PNS_CONFIGURATION_DATA_T * data = PNSC_InitData(1, numSubmodulesForAPI, 0x00003021);

     // Set system parameters
     PNSC_SetSystemParameters(PNS_IF_SYSTEM_START_AUTO_START |
                              PNS_IF_SYSTEM_STACK_HANDLE_I_M_ENABLED |
                              PNS_IF_SYSTEM_ARDY_WOUT_APPL_REG_ENABLED |
                              PNS_IF_SYSTEM_NAME_IP_HANDLING_BY_STACK_ENABLED, 1000);

     // Set device identification information
     PNSC_SetDeviceIdentification(HILSCHER_VENDOR_ID, 0x1003);

     // Set name, type, and device type for the station
     PNSC_SetNameAndType((const uint8_t *)"dev-example", (const uint8_t *)"NETX V5");

     // Set firmware version information
     PNSC_SetSoftwareVersionInfo(5, 4, 0, 'V');

     // Add cyclic input and output submodules for API 0
     PNSC_AddCyclicInputSubmodule(0, 10);  // Data From HAS

     PNSC_AddModule(0);

     PNSC_AddCyclicOutputSubmodule(0, 6);  // Data To HAS

     return data;
}
