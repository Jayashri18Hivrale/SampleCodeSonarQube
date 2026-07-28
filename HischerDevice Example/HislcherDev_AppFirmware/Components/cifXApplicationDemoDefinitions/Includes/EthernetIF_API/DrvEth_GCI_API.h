/**************************************************************************************
Copyright (c) Hilscher Gesellschaft fuer Systemautomation mbH. All Rights Reserved.
***************************************************************************************
  $Id:  $: *//*!

  \file DrvEth_GCI_API.h
  DrvEth GCI Application Programming Interface

Changes:
  Date          Description
  -----------------------------------------------------------------------------------
  2018-08-8     created
**************************************************************************************/

#ifndef __DRVETH_GCI_API_H
#define __DRVETH_GCI_API_H

#include <stdint.h>
#include "Hil_Compiler.h"
#include "EthernetIF_API.h"

/*! \defgroup drveth_gci_public_api Raw Ethernet GCI Public API
 *
 * This section describes the API between the host application and
 * DrvEth GCI Adapter.
 * @{
 */

/*! \defgroup drveth_gci_extended_state Extended State
 * @{
 */
/*! Extended Status Area content. */
typedef __HIL_PACKED_PRE struct DRVETH_GCI_EXTENDED_STATE_Ttag
{
  /*!The mac address of the ethernet interface
   * This is set to zero if ethernet mac is not available. */
  uint8_t abMacAddress[ETHERNET_IF_ETH_ADDR_SIZE];
  /*! The current mau type according IANA */
  uint8_t bMautype;
  /*! alignment */
  uint8_t  bPadding;
  /*! Reserved. */
  uint32_t   ulReserved0;
  /*! Reserved. */
  uint32_t   ulReserved1;
  /*! Reserved. */
  uint32_t   ulReserved2;
  /*! The number of received packets passed to the host. */
  uint64_t   ullIfInPkts;
  /*! The number of received packets not passed to the host because of
   *  packet queue overflow. */
  uint64_t   ullIfInDiscards;
  /*! The number of sent ethernet frames. */
  uint64_t   ullIfOutPkts;
  /*! The number of ethernet frames dropped because of no
   *  ethernet frame buffer available. */
  uint64_t   ullIfOutDiscards;
  /*! The number of bytes received. */
  uint64_t   ullIfInBytes;
  /*! The number of bytes transmitted. */
  uint64_t   ullIfOutBytes;
} __HIL_PACKED_POST DRVETH_GCI_EXTENDED_STATE_T;

/*! @} */

/*! \defgroup drveth_gci_linkstatus_indication Link Status Change
 *
 *  This service is used by the DrvEth GCI Adapter to notify the host application
 *  about occurring link status changes. The service will be enabled after the
 *  host application used the HIL_REGISTER_APP_REQ.
 *
 *  The service uses a locking mechanism to avoid flooding the host application
 *  with every link change. After an link change indication has been generated,
 *  the DrvEth GCI Adapter will send the latest link status change indication
 *  after the host returned the response back.
 *
 * @{
 */

/*! @} */

/*! @} */

#endif /* #ifndef __DRVETH_GCI_API_H */
