/**************************************************************************************
Copyright (c) Hilscher Gesellschaft fuer Systemautomation mbH. All Rights Reserved.
***************************************************************************************
$Id:  $:

Description:
  Profinet Core (PSPNCORE) result codes
**************************************************************************************/

#ifndef PNCORE_RESULTS_H_
#define PNCORE_RESULTS_H_

#include<stdint.h>

/////////////////////////////////////////////////////////////////////////////////////
// PROFINET CORE ERROR codes
/////////////////////////////////////////////////////////////////////////////////////
//
// MessageId: ERR_PROFINET_CORE_TAGLIST_INVALID_MIN_RPC_BUFFER_SIZE
//
// MessageText:
//
// Taglist parameter Minimum size of each RPC Buffer is invalid.
//
#define ERR_PROFINET_CORE_TAGLIST_INVALID_MIN_RPC_BUFFER_SIZE ((uint32_t)0xC1120001L)

//
// MessageId: ERR_PROFINET_CORE_TAGLIST_INVALID_ADDITIONAL_AR_NUMBER
//
// MessageText:
//
// Taglist parameter Number of additional IO Connections is invalid.
//
#define ERR_PROFINET_CORE_TAGLIST_INVALID_ADDITIONAL_AR_NUMBER ((uint32_t)0xC1120002L)

//
// MessageId: ERR_PROFINET_CORE_TAGLIST_INVALID_IMPLICIT_AR_NUMBER
//
// MessageText:
//
// Taglist parameter Number of parallel Read Implicit Requests is invalid.
//
#define ERR_PROFINET_CORE_TAGLIST_INVALID_IMPLICIT_AR_NUMBER ((uint32_t)0xC1120003L)

//
// MessageId: ERR_PROFINET_CORE_TAGLIST_INVALID_DAAR_NUMBER
//
// MessageText:
//
// Taglist parameter Number of parallel DeviceAccess ARs is invalid.
//
#define ERR_PROFINET_CORE_TAGLIST_INVALID_DAAR_NUMBER ((uint32_t)0xC1120004L)

//
// MessageId: ERR_PROFINET_CORE_TAGLIST_INVALID_SUBMODULE_NUMBER
//
// MessageText:
//
// Taglist parameter Number of configurable submodules is invalid.
//
#define ERR_PROFINET_CORE_TAGLIST_INVALID_SUBMODULE_NUMBER ((uint32_t)0xC1120005L)

//
// MessageId: ERR_PROFINET_CORE_TAGLIST_INVALID_ARSET_NUMBER
//
// MessageText:
//
// Taglist parameter Number of parallel ARSets is invalid.
//
#define ERR_PROFINET_CORE_TAGLIST_INVALID_ARSET_NUMBER ((uint32_t)0xC1120006L)

//
// MessageId: ERR_PROFINET_CORE_TAGLIST_INVALID_DIAGNOSIS_BUFFERS_NUMBER
//
// MessageText:
//
// Taglist parameter Number of available diagnosis buffers is invalid.
//
#define ERR_PROFINET_CORE_TAGLIST_INVALID_DIAGNOSIS_BUFFERS_NUMBER ((uint32_t)0xC1120007L)

//
// MessageId: INFO_PROFINET_CORE_IP4_NOT_CHANGED
//
// MessageText:
//
// The IPv4 address was not changed since it already had the requested value.
//
#define INFO_PROFINET_CORE_IP4_NOT_CHANGED ((uint32_t)0x41120008L)

#endif  /* PNCORE_RESULTS_H_ */
