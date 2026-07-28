/*
 * pn_config.c
 *
 *  Created on: Oct 28, 2024
 *      Author: A0084817
 */

#include "nx90_pns_config.h"

#include <string.h>

#define FIXED_GSDML_INSTANCE_ID 1

#define DAP_V3_5_SLOT        0x0000
#define DAP_V3_5_SUBSLOT     0x0001

#define PDEV_PN_IO_SUBSLOT   0x8000


#define PDEV_PORT_1_SUBSLOT  PDEV_PN_IO_SUBSLOT + 1
#define PDEV_PORT_2_SUBSLOT  PDEV_PORT_1_SUBSLOT + 1


#define INITIAL_MODULE_ID 0x01   // Starting module ID for custom modules
#define INITIAL_SLOT 0x0001      // Starting slot ID for custom modules
#define INITIAL_SUBMODULE_ID 0x01  // Starting submodule ID for cyclic data
#define INITIAL_SUBSLOT 0x0001     // Starting subslot for cyclic data


typedef struct
{
  uint32_t totalsubmoduleCount;
  uint32_t currentInputSubmoduleCount;
  uint32_t currentOutputSubmoduleCount;
  uint32_t submoduleCounts[PNS_APIS];         // Track submodule counts per API
  uint32_t currentModuleId;                   // Tracks current module ID
  uint32_t currentSlot;                       // Tracks current slot number
  uint32_t currentSubmoduleId;                // Tracks current submodule ID
  uint32_t currentSubslot;                    // Tracks current subslot number
  uint32_t currentDPMOffsetIn;                // Tracks current offsets
  uint32_t currentDPMOffsetOut;                // Tracks current offsets
  uint32_t submodulesInDataOffsets[PNS_INPUT_SUBMODULES + PNS_IN_OUT_SUBMODULES];
  uint32_t submodulesOutDataOffsets[PNS_OUTPUT_SUBMODULES - PNS_IN_OUT_SUBMODULES];
  uint32_t submodulesInDataLengths[PNS_INPUT_SUBMODULES + PNS_IN_OUT_SUBMODULES];
  uint32_t submodulesOutDataLengths[PNS_OUTPUT_SUBMODULES - PNS_IN_OUT_SUBMODULES];
  PNS_Cyclic_Offsets_t inputCyclic;
  PNS_Cyclic_Offsets_t outputCyclic;
}_PNS_CONFIGURATION_REQUEST_DATA_T;

static PNS_CONFIGURATION_DATA_T pnsConfig;

static PNS_CONFIGURATION_DATA_T* pPnsConfig = &pnsConfig;

static _PNS_CONFIGURATION_REQUEST_DATA_T privateData = { 0 };

static int32_t PNSC_AddDefaultSubmodules(uint32_t apiIndex, uint32_t dapModuleId);

PNS_CONFIGURATION_DATA_T * PNSC_InitData(const uint32_t num_apis, const uint32_t *num_submodules, uint32_t dapModuleId) {
  // Validate inputs
  if (num_apis > PNS_APIS || num_submodules == NULL) {
      return NULL;
  }

  // Calculate the total submodules and ensure within limits
  privateData.totalsubmoduleCount = 0;
  for (uint32_t i = 0; i < num_apis; i++) {
      privateData.totalsubmoduleCount += num_submodules[i];
      privateData.submoduleCounts[i] = 0; // Track submodules per API
  }

  // Initialize static config data and clear buffer
  memset(pPnsConfig, 0, sizeof(pnsConfig));

  pPnsConfig->tSetConfigReq.tModuleConfig.ulNumApi = num_apis;
  pPnsConfig->tSetConfigReq.ulTotalConfigPckLen = sizeof(pnsConfig);

  // Initialize APIs with default settings and add required submodules to API 0
  for (uint32_t i = 0; i < num_apis; i++) {
      pPnsConfig->tAPIs[i].ulApi = i;
      pPnsConfig->tAPIs[i].ulNumSubmodItems = num_submodules[i];
  }

  // Set fixed fields
  pPnsConfig->tSetConfigReq.tDeviceParameters.usInstanceId = FIXED_GSDML_INSTANCE_ID;
  pPnsConfig->tSetConfigReq.tDeviceParameters.usMaxDiagRecords = 16;

  // Ensure the first API has at least 4 submodules
  if (num_apis > 0 && num_submodules[0] >= 4) {
      PNSC_AddDefaultSubmodules(0, dapModuleId); // Add default submodules for API 0
  } else {
      return NULL; // Error: Insufficient submodules for API 0
  }

  privateData.currentModuleId = INITIAL_MODULE_ID;
  privateData.currentSlot = INITIAL_SLOT;
  privateData.currentSubmoduleId = INITIAL_SUBMODULE_ID;
  privateData.currentSubslot = INITIAL_SUBSLOT;

  privateData.inputCyclic.totalSubmodules = PNS_INPUT_SUBMODULES + PNS_IN_OUT_SUBMODULES;
  privateData.inputCyclic.Offsets = privateData.submodulesInDataOffsets;
  privateData.inputCyclic.Lengths = privateData.submodulesInDataLengths;

  privateData.outputCyclic.totalSubmodules = PNS_OUTPUT_SUBMODULES + PNS_IN_OUT_SUBMODULES;
  privateData.outputCyclic.Offsets = privateData.submodulesOutDataOffsets;
  privateData.outputCyclic.Lengths = privateData.submodulesOutDataLengths;

  return pPnsConfig; // Return pointer to the initialized structure
}

int32_t PNSC_AddCyclicInputSubmodule(uint32_t apiIndex, uint32_t input_data_length) {
  // Validate API index and submodule count for this API
  if (apiIndex >= pPnsConfig->tSetConfigReq.tModuleConfig.ulNumApi || privateData.submoduleCounts[apiIndex] >= pPnsConfig->tAPIs[apiIndex].ulNumSubmodItems) {
      return -1; // Error: Index out of range or submodule limit reached
  }

  // Get pointer to the next available submodule in the array
  PNS_IF_SUBMODULE_STRUCT_T *ptSubmod = &pPnsConfig->tSubmodules[privateData.submoduleCounts[apiIndex]++];

  // Configure the cyclic input submodule
  ptSubmod->usSlot = privateData.currentSlot;
  ptSubmod->ulModuleId = privateData.currentModuleId;
  ptSubmod->usSubslot = privateData.currentSubslot++;
  ptSubmod->ulSubmodId = privateData.currentSubmoduleId++;
  ptSubmod->ulProvDataLen = input_data_length;
  ptSubmod->ulDPMOffsetIn = privateData.currentDPMOffsetIn;  // Set appropriate DPM offset based on your configuration

  privateData.submodulesInDataLengths[privateData.currentInputSubmoduleCount] = input_data_length - privateData.currentDPMOffsetIn;
  privateData.currentDPMOffsetIn += input_data_length;
  privateData.submodulesInDataOffsets[privateData.currentInputSubmoduleCount++] = privateData.currentDPMOffsetIn;
  return 0; // Success
}

int32_t PNSC_AddCyclicOutputSubmodule(uint32_t apiIndex, uint32_t output_data_length) {
  // Validate API index and submodule count for this API
  if (apiIndex >= pPnsConfig->tSetConfigReq.tModuleConfig.ulNumApi || privateData.submoduleCounts[apiIndex] >= pPnsConfig->tAPIs[apiIndex].ulNumSubmodItems) {
      return -1; // Error: Index out of range or submodule limit reached
  }

  // Get pointer to the next available submodule in the array
  PNS_IF_SUBMODULE_STRUCT_T *ptSubmod = &pPnsConfig->tSubmodules[privateData.submoduleCounts[apiIndex]++];

  // Configure the cyclic output submodule
  ptSubmod->usSlot = privateData.currentSlot;
  ptSubmod->ulModuleId = privateData.currentModuleId;
  ptSubmod->usSubslot = privateData.currentSubslot++;
  ptSubmod->ulSubmodId = privateData.currentSubmoduleId++;
  ptSubmod->ulConsDataLen = output_data_length;
  ptSubmod->ulDPMOffsetIn = privateData.currentDPMOffsetOut;  // Set appropriate DPM offset based on your configuration

  privateData.submodulesOutDataLengths[privateData.currentOutputSubmoduleCount] = output_data_length - privateData.currentDPMOffsetOut;
  privateData.currentDPMOffsetOut += output_data_length;
  privateData.submodulesOutDataOffsets[privateData.currentOutputSubmoduleCount++] = privateData.currentDPMOffsetOut;

  return 0; // Success
}

int32_t PNSC_AddCyclicInputOutputSubmodule(uint32_t apiIndex,uint32_t input_data_length, uint32_t output_data_length) {
  // Validate API index and submodule count for this API
  if (apiIndex >= pPnsConfig->tSetConfigReq.tModuleConfig.ulNumApi || privateData.submoduleCounts[apiIndex] >= pPnsConfig->tAPIs[apiIndex].ulNumSubmodItems) {
      return -1; // Error: Index out of range or submodule limit reached
  }

  // Get pointer to the next available submodule in the array
  PNS_IF_SUBMODULE_STRUCT_T *ptSubmod = &pPnsConfig->tSubmodules[privateData.submoduleCounts[apiIndex]++];

  // Configure the cyclic input-output submodule
  ptSubmod->usSlot = privateData.currentSlot;
  ptSubmod->ulModuleId = privateData.currentModuleId;
  ptSubmod->usSubslot = privateData.currentSubslot++;
  ptSubmod->ulSubmodId = privateData.currentSubmoduleId++;
  ptSubmod->ulProvDataLen = input_data_length;
  ptSubmod->ulConsDataLen = output_data_length;
  ptSubmod->ulDPMOffsetIn = privateData.currentDPMOffsetIn;  // Set appropriate DPM offset based on your configuration
  ptSubmod->ulDPMOffsetOut = privateData.currentDPMOffsetOut;  // Set appropriate DPM offset based on your configuration

  privateData.submodulesInDataLengths[privateData.currentInputSubmoduleCount] = input_data_length - privateData.currentDPMOffsetIn;
  privateData.currentDPMOffsetIn += input_data_length;
  privateData.submodulesInDataOffsets[privateData.currentInputSubmoduleCount++] = privateData.currentDPMOffsetIn;

  privateData.submodulesOutDataLengths[privateData.currentOutputSubmoduleCount] = output_data_length - privateData.currentDPMOffsetOut;
  privateData.currentDPMOffsetOut += output_data_length;
  privateData.submodulesOutDataOffsets[privateData.currentOutputSubmoduleCount++] = privateData.currentDPMOffsetOut;

  return 0; // Success
}

int32_t PNSC_AddModule(uint32_t apiIndex) {
// Validate API index
if (apiIndex >= pPnsConfig->tSetConfigReq.tModuleConfig.ulNumApi) {
    return -1; // Error: API index out of range
}

// Increment the module ID and slot for the specified API
privateData.currentModuleId++;
privateData.currentSlot++;

// Reset submodule and subslot counts for the new module
privateData.currentSubmoduleId = 1;
privateData.currentSubslot = 1;

return 0; // Success
}

int32_t PNSC_SetSystemParameters(uint32_t ulSystemFlags, uint32_t ulWdgTime) {
  pPnsConfig->tSetConfigReq.tDeviceParameters.ulSystemFlags = ulSystemFlags;
  pPnsConfig->tSetConfigReq.tDeviceParameters.ulWdgTime = ulWdgTime;

  return 0;  // Success
}

int32_t PNSC_SetDeviceIdentification(uint32_t vendorId, uint32_t ulDeviceId) {
  pPnsConfig->tSetConfigReq.tDeviceParameters.ulDeviceId = ulDeviceId;
  pPnsConfig->tSetConfigReq.tDeviceParameters.ulVendorId = vendorId;
  return 0;
}

int32_t PNSC_SetNameAndType(const uint8_t *abNameOfStation, const uint8_t *abTypeOfStation) {
  memset(pPnsConfig->tSetConfigReq.tDeviceParameters.abNameOfStation, 0x00, 240); /* The NameOfStation as ASCII char-array  */
  memset(pPnsConfig->tSetConfigReq.tDeviceParameters.abTypeOfStation, 0x00, 240); /* The TypeOfStation as ASCII char-array  */
  memset(pPnsConfig->tSetConfigReq.tDeviceParameters.abDeviceType, 0x00, 28); /* The DeviceType as ASCII char-array     */

  pPnsConfig->tSetConfigReq.tDeviceParameters.ulNameOfStationLen = strlen((char *)abNameOfStation);
  strncpy((char *)pPnsConfig->tSetConfigReq.tDeviceParameters.abNameOfStation, (const char *)abNameOfStation, pPnsConfig->tSetConfigReq.tDeviceParameters.ulNameOfStationLen + 1);

  pPnsConfig->tSetConfigReq.tDeviceParameters.ulTypeOfStationLen = strlen((char *)abTypeOfStation);
  strncpy((char *)pPnsConfig->tSetConfigReq.tDeviceParameters.abTypeOfStation, (const char *)abTypeOfStation, pPnsConfig->tSetConfigReq.tDeviceParameters.ulTypeOfStationLen + 1);

  strncpy((char *)pPnsConfig->tSetConfigReq.tDeviceParameters.abDeviceType, (const char *)abTypeOfStation, pPnsConfig->tSetConfigReq.tDeviceParameters.ulTypeOfStationLen + 1);

  return 0;  // Success
}

int32_t PNSC_SetNetworkConfig(uint32_t ulIpAddr, uint32_t ulNetMask, uint32_t ulGateway) {
  pPnsConfig->tSetConfigReq.tDeviceParameters.ulIpAddr = ulIpAddr;
  pPnsConfig->tSetConfigReq.tDeviceParameters.ulNetMask = ulNetMask;
  pPnsConfig->tSetConfigReq.tDeviceParameters.ulGateway = ulGateway;

  return 0;  // Success
}

int32_t PNSC_SetSoftwareVersionInfo(uint16_t usSwRevision1, uint16_t usSwRevision2, uint16_t usSwRevision3, uint8_t bSwRevisionPrefix) {
  pPnsConfig->tSetConfigReq.tDeviceParameters.usSwRevision1 = usSwRevision1;
  pPnsConfig->tSetConfigReq.tDeviceParameters.usSwRevision2 = usSwRevision2;
  pPnsConfig->tSetConfigReq.tDeviceParameters.usSwRevision3 = usSwRevision3;
  pPnsConfig->tSetConfigReq.tDeviceParameters.bSwRevisionPrefix = bSwRevisionPrefix;

  return 0;  // Success
}

static int32_t PNSC_AddDefaultSubmodules(uint32_t apiIndex, uint32_t dapModuleId) {
  // Validate API index and ensure space for 4 default submodules
  if (apiIndex >= pPnsConfig->tSetConfigReq.tModuleConfig.ulNumApi ||
      privateData.submoduleCounts[apiIndex] + 4 > pPnsConfig->tAPIs[apiIndex].ulNumSubmodItems) {
      return -1;  // Error: Index out of range or insufficient submodules
  }

  // Set pointers for the current API and starting submodule
  PNS_IF_SUBMODULE_STRUCT_T *ptSubmod = &pPnsConfig->tSubmodules[privateData.submoduleCounts[apiIndex]];

  uint32_t dapSubmoduleId = dapModuleId - 1;
  // Configure DAP V3.5 Submodule
  ptSubmod->usSlot = DAP_V3_5_SLOT;
  ptSubmod->usSubslot = 0x0001;
  ptSubmod->ulModuleId = dapModuleId;
  ptSubmod->ulSubmodId = dapSubmoduleId;
  ptSubmod->ulProvDataLen = 0;
  ptSubmod->ulConsDataLen = 0;
  ptSubmod->ulDPMOffsetOut = 0;
  ptSubmod->ulDPMOffsetIn = 0;
  ptSubmod++;  // Move to the next submodule

  // Configure PDEV PN-IO Submodule
  ptSubmod->usSlot = DAP_V3_5_SLOT;
  ptSubmod->usSubslot = PDEV_PN_IO_SUBSLOT;
  ptSubmod->ulModuleId = dapModuleId;
  ptSubmod->ulSubmodId = dapSubmoduleId + 1;
  ptSubmod->ulProvDataLen = 0;
  ptSubmod->ulConsDataLen = 0;
  ptSubmod->ulDPMOffsetOut = 0;
  ptSubmod->ulDPMOffsetIn = 0;
  ptSubmod++;  // Move to the next submodule

  // Configure PDEV Port 1 Submodule
  ptSubmod->usSlot = DAP_V3_5_SLOT;
  ptSubmod->usSubslot = PDEV_PORT_1_SUBSLOT;
  ptSubmod->ulModuleId = dapModuleId;
  ptSubmod->ulSubmodId = dapSubmoduleId + 2;
  ptSubmod->ulProvDataLen = 0;
  ptSubmod->ulConsDataLen = 0;
  ptSubmod->ulDPMOffsetOut = 0;
  ptSubmod->ulDPMOffsetIn = 0;
  ptSubmod++;  // Move to the next submodule

  // Configure PDEV Port 2 Submodule
  ptSubmod->usSlot = DAP_V3_5_SLOT;
  ptSubmod->usSubslot = PDEV_PORT_2_SUBSLOT;
  ptSubmod->ulModuleId = dapModuleId;
  ptSubmod->ulSubmodId = dapSubmoduleId + 3;
  ptSubmod->ulProvDataLen = 0;
  ptSubmod->ulConsDataLen = 0;
  ptSubmod->ulDPMOffsetOut = 0;
  ptSubmod->ulDPMOffsetIn = 0;

  // Update internal tracking counts
  privateData.submoduleCounts[apiIndex] += 4;

  return 0;  // Success
}

PNS_Cyclic_Offsets_t * PNSC_GetInputCyclicOffsets()
{
      return &privateData.inputCyclic;
}

PNS_Cyclic_Offsets_t * PNSC_GetOutputCyclicOffsets()
{
     return &privateData.outputCyclic;
}
