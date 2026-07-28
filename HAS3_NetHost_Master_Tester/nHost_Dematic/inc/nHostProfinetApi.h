#ifndef NHOST_PROFINET_API_H
#define NHOST_PROFINET_API_H

#if _MSC_VER >= 1000
#define __PNM_PACKED_PRE
#define __PNM_PACKED_POST
#pragma once
#pragma pack(1)            /* Always align structures to 1Byte boundery */
#ifndef STRICT             /* Check Typedefinition */
#define STRICT
#endif
#endif /* _MSC_VER >= 1000 */

/* support for GNU compiler */
#ifdef __GNUC__
#define __PNM_PACKED_PRE
#define __PNM_PACKED_POST   __attribute__((packed))
#endif

#define APIOC_ACTIVE_DEVICE_CONNECT_INFO_STRUCT_ID   0x2245
#define APIOC_INACTIVE_DEVICE_CONNECT_INFO_STRUCT_ID 0x2244

#define PNM_AP_CMD_CFG_GET_NUM_CONFIGURED_OBJECTS_REQ 0x9450
#define PNM_AP_CMD_CFG_GET_NUM_CONFIGURED_OBJECTS_CNF 0x9451

#define PNM_AP_CMD_CFG_GET_SUBMODULE_REQ  0x00009462
#define PNM_AP_CMD_CFG_GET_SUBMODULE_CNF  0x00009463

#include <stdint.h>
#include "cifXUser.h"

typedef __PNM_PACKED_PRE struct
{
    uint32_t ulLenName;
    uint32_t ulLenType;
    uint32_t ulIPAddress;
    uint32_t ulDiagFlags;
    uint16_t usDeviceID;
    uint16_t usVendorID;
    uint8_t  abMac[6];
    uint16_t reserved;
    uint8_t  abName[240];
    uint8_t  abType[240];
} __PNM_PACKED_POST PNM_AP_ACTIVE_SLAVE_CONNECT_INFO_T;

typedef __PNM_PACKED_PRE struct
{
    uint32_t ulLenName;
    uint32_t ulDiagFlags;
    uint16_t usDeviceID;
    uint16_t usVendorID;
    uint8_t  abName[240];
} __PNM_PACKED_POST PNM_AP_INACTIVE_SLAVE_CONNECT_INFO_T;

typedef __PNM_PACKED_PRE struct
{
    uint32_t ulHandle;     // Slave handle
    uint32_t ulStructID;   // Structure ID (e.g., active or inactive)
    union {
        PNM_AP_ACTIVE_SLAVE_CONNECT_INFO_T tActiveInfo;   // Active connection info
        PNM_AP_INACTIVE_SLAVE_CONNECT_INFO_T tInactiveInfo; // Inactive connection info
    } tConnectInfo;
} __PNM_PACKED_POST PNM_AP_SLAVE_CONNECT_INFO_T;

typedef __PNM_PACKED_PRE struct
{
    uint16_t usNumAR;
    uint16_t usNumIocr;
    uint16_t usNumSubmodule;
} __PNM_PACKED_POST PNM_AP_NUM_CONFIGURED_OBJECTS_T;


typedef __PNM_PACKED_PRE struct
{
    uint32_t ulStructVersion;
    uint16_t usSubmoduleHandle;
    uint16_t usDeviceHandle;
    uint16_t usInputIocrHandle;
    uint16_t usOutputIocrHandle;
    uint32_t ulModuleIdentNumber;
    uint32_t ulSubmoduleIdentNumber;
    uint32_t ulApi;
    uint16_t usSlot;
    uint16_t usSubslot;
    uint16_t usSubmoduleProperties;
    uint16_t usDataLenInput;
    uint16_t usDataLenOutput;
} __PNM_PACKED_POST PNM_AP_SUBMODULE_INFO_T;


int32_t NHsPNM_GetSlaveConnecInfo(CIFXHANDLE channel, uint32_t slaveHandle,
                                    PNM_AP_INACTIVE_SLAVE_CONNECT_INFO_T* inactiveInfo,
                                            PNM_AP_ACTIVE_SLAVE_CONNECT_INFO_T* activeInfo);

int32_t NHsPNM_GetNumConfiguredObjects(CIFXHANDLE channel, PNM_AP_NUM_CONFIGURED_OBJECTS_T* confObjects);


int32_t NHsPNM_GetSubmoduleInfo(CIFXHANDLE channel, uint16_t submoduleHandle, PNM_AP_SUBMODULE_INFO_T* submoduleInfo);

#endif