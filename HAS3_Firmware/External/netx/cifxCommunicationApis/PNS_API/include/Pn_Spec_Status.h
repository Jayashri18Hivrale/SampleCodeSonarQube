/**************************************************************************************
Copyright (c) Hilscher Gesellschaft fuer Systemautomation mbH. All Rights Reserved.
***************************************************************************************
$Id: Pn_Spec_Status.h 95962 2020-10-28 13:34:50Z AMesser $:

Description:
 Profinet Core Status Specification Declarations
**************************************************************************************/
#ifndef PN_SPEC_STATUS_H_
#define PN_SPEC_STATUS_H_

#include <stdint.h>

/** Type to be used for Profinet status values */
typedef uint32_t PN_PNIOSTATUS_T;

/** @defgroup spc_pn_status Profinet Status Code definitions
 *
 * @hint We can not use an enumeration here since enumerations are
 *       not typesafe in C. (Produces tons of PC-Lint warnings also)
 *
 * @{
 */

/** success state code */
#define  PNIO_S_OK ((PN_PNIOSTATUS_T)(0x00000000UL))

  /* The following error codes are incomplete and need to be combined with
   * proper ErrorCode value to form a valid profinet error code. They are
   * used in conjunction with common functions used with read or write services.
   * In order to allow for file search matches, the combined error codes
   * are added as comment*/
#define PNIO_E_IOD_PNIORW_ACCESS_INVALIDINDEX                      ((PN_PNIOSTATUS_T)(0x0080B000UL)) /* 0xDE80B000, 0xDF80B000 */
#define PNIO_E_IOD_PNIORW_ACCESS_INVALIDLENGTH                     ((PN_PNIOSTATUS_T)(0x0080B100UL)) /* 0xDE80B100, 0xDF80B100 */
#define PNIO_E_IOD_PNIORW_ACCESS_INVALIDLENGTH_BUFFERTOSMALL       ((PN_PNIOSTATUS_T)(0x0080B101UL)) /* 0xDE80B101, 0xDF80B101 */
#define PNIO_E_IOD_PNIORW_ACCESS_INVALIDLENGTH_LENGTHTOSMALL       ((PN_PNIOSTATUS_T)(0x0080B102UL)) /* 0xDE80B102, 0xDF80B102 */
#define PNIO_E_IOD_PNIORW_ACCESS_INVALIDLENGTH_LENGTHMISMATCH      ((PN_PNIOSTATUS_T)(0x0080B103UL)) /* 0xDE80B103, 0xDF80B103 */
#define PNIO_E_IOD_PNIORW_ACCESS_INVALIDSLOT_SUBSLOT               ((PN_PNIOSTATUS_T)(0x0080B200UL)) /* 0xDE80B200, 0xDF80B200 */
#define PNIO_E_IOD_PNIORW_ACCESS_TYPECONFLICT                      ((PN_PNIOSTATUS_T)(0x0080B300UL)) /* 0xDE80B300, 0xDF80B300 */
#define PNIO_E_IOD_PNIORW_ACCESS_INVALIDAPI_AREA                   ((PN_PNIOSTATUS_T)(0x0080B400UL)) /* 0xDE80B400, 0xDF80B400 */
#define PNIO_E_IOD_PNIORW_ACCESS_STATECONFLICT                     ((PN_PNIOSTATUS_T)(0x0080B500UL)) /* 0xDE80B500, 0xDF80B500 */
#define PNIO_E_IOD_PNIORW_ACCESS_DENIED                            ((PN_PNIOSTATUS_T)(0x0080B600UL)) /* 0xDE80B600, 0xDF80B600 */
#define PNIO_E_IOD_PNIORW_ACCESS_DENIED_OWNER                      ((PN_PNIOSTATUS_T)(0x0080B601UL)) /* 0xDE80B601, 0xDF80B601 */
#define PNIO_E_IOD_PNIORW_ACCESS_DENIED_SOL                        ((PN_PNIOSTATUS_T)(0x0080B602UL)) /* 0xDE80B602, 0xDF80B602 */
#define PNIO_E_IOD_PNIORW_ACCESS_INVALIDRANGE                      ((PN_PNIOSTATUS_T)(0x0080B700UL)) /* 0xDE80B700, 0xDF80B700 */
#define PNIO_E_IOD_PNIORW_ACCESS_INVALIDPARAM                      ((PN_PNIOSTATUS_T)(0x0080B800UL)) /* 0xDE80B800, 0xDF80B800 */
#define PNIO_E_IOD_PNIORW_ACCESS_INVALIDTYPE                       ((PN_PNIOSTATUS_T)(0x0080B900UL)) /* 0xDE80B900, 0xDF80B900 */
#define PNIO_E_IOD_PNIORW_ACCESS_INVALIDTYPE_BLOCKTYPEMISMATCH     ((PN_PNIOSTATUS_T)(0x0080B901UL)) /* 0xDE80B901, 0xDF80B901 */
#define PNIO_E_IOD_PNIORW_ACCESS_INVALIDTYPE_BLOCKVERSIONMISMATCH  ((PN_PNIOSTATUS_T)(0x0080B902UL)) /* 0xDE80B902, 0xDF80B902 */
#define PNIO_E_IOD_PNIORW_ACCESS_BACKUP                            ((PN_PNIOSTATUS_T)(0x0080BA00UL)) /* 0xDE80BA00, 0xDF80BA00 */
#define PNIO_E_IOD_PNIORW_ACCESS_USERSPEC7                         ((PN_PNIOSTATUS_T)(0x0080BB00UL)) /* 0xDE80BB00, 0xDF80BB00 */
#define PNIO_E_IOD_PNIORW_ACCESS_USERSPEC8                         ((PN_PNIOSTATUS_T)(0x0080BC00UL)) /* 0xDE80BC00, 0xDF80BC00 */
#define PNIO_E_IOD_PNIORW_ACCESS_USERSPEC9                         ((PN_PNIOSTATUS_T)(0x0080BD00UL)) /* 0xDE80BD00, 0xDF80BD00 */
#define PNIO_E_IOD_PNIORW_ACCESS_USERSPEC10                        ((PN_PNIOSTATUS_T)(0x0080BE00UL)) /* 0xDE80BE00, 0xDF80BE00 */
#define PNIO_E_IOD_PNIORW_ACCESS_USERSPEC11                        ((PN_PNIOSTATUS_T)(0x0080BF00UL)) /* 0xDE80BF00, 0xDF80BF00 */


  /* NRPM - ErrorCode = 0x81, ErrorDecode = 0x81, ErrorCode1 = 0x3F (63) */
#define PNIO_E_CTLDINA_NO_DCP_ACTIVE                         ((PN_PNIOSTATUS_T)(0x81813F00UL))
#define PNIO_E_CTLDINA_DNS_STATIONNAME_UNKNOWN               ((PN_PNIOSTATUS_T)(0x81813F01UL))
#define PNIO_E_CTLDINA_DCP_STATIONNAME_NO_REAL               ((PN_PNIOSTATUS_T)(0x81813F02UL))
#define PNIO_E_CTLDINA_DCP_STATIONNAME_MULTIPLE              ((PN_PNIOSTATUS_T)(0x81813F03UL))
#define PNIO_E_CTLDINA_DCP_STATIONNAME_FAILED                ((PN_PNIOSTATUS_T)(0x81813F04UL))
#define PNIO_E_CTLDINA_NO_IP_ADDRESS                         ((PN_PNIOSTATUS_T)(0x81813F05UL))
#define PNIO_E_CTLDINA_DCP_ERROR                             ((PN_PNIOSTATUS_T)(0x81813F06UL))
#define PNIO_E_CTLDINA_MULTIPLE_IP_ADDRESS                   ((PN_PNIOSTATUS_T)(0x81813F07UL))

  /** rpc error codes */
#define PNIO_E_CLRPC_ERR_REJECTED                            ((PN_PNIOSTATUS_T)(0x81814501UL))
#define PNIO_E_CLRPC_ERR_FAULTED                             ((PN_PNIOSTATUS_T)(0x81814502UL))
#define PNIO_E_CLRPC_ERR_TIMEOUT                             ((PN_PNIOSTATUS_T)(0x81814503UL))
#define PNIO_E_CLRPC_ERR_IN_ARGS                             ((PN_PNIOSTATUS_T)(0x81814504UL))
#define PNIO_E_CLRPC_ERR_OUT_ARGS                            ((PN_PNIOSTATUS_T)(0x81814505UL))
#define PNIO_E_CLRPC_ERR_DECODE                              ((PN_PNIOSTATUS_T)(0x81814506UL))
#define PNIO_E_CLRPC_ERR_PNIO_OUT_ARGS                       ((PN_PNIOSTATUS_T)(0x81814507UL))
#define PNIO_E_CLRPC_ERR_PNIO_APP_TIMEOUT                    ((PN_PNIOSTATUS_T)(0x81814508UL))

  /* FSPM error codes
   *
   * According to profinet specification, there are FSPM specific error
   * codes. According to specification, usage of these error codes
   * is described somewhere in state machines, but I did not found any
   * description. If we regard FSPM as intermediate layer between
   * Application and Stack we end up it beeing the AP-Task for Hilscher
   * implementations. Thus we hijack this error code range to report
   * application issues to the network. This makes it easy to figure
   * out errors when analyzing a network trace
   */
#define PNIO_E_FSPM_SERVICE_FAILED                           ((PN_PNIOSTATUS_T)(0x81814E00UL))
#define PNIO_E_FSPM_SERVICE_PENDING                          ((PN_PNIOSTATUS_T)(0x81814E01UL))
#define PNIO_E_FSPM_SERVICE_LENGTH_ERROR                     ((PN_PNIOSTATUS_T)(0x81814E02UL))
#define PNIO_E_FSPM_SERVICE_INVALID_PARAMETER                ((PN_PNIOSTATUS_T)(0x81814E03UL))


  /* used by profinet device when checking IOCRBlockReq */
#define PNIO_E_CMRPC_IOCRREQ_BLOCKTYPE                       ((PN_PNIOSTATUS_T)(0xDB810200UL))
#define PNIO_E_CMRPC_IOCRREQ_BLOCKLEN                        ((PN_PNIOSTATUS_T)(0xDB810201UL))
#define PNIO_E_CMRPC_IOCRREQ_REF                             ((PN_PNIOSTATUS_T)(0xDB810205UL))
#define PNIO_E_CMRPC_IOCRREQ_LT                              ((PN_PNIOSTATUS_T)(0xDB810206UL))
#define PNIO_E_CMRPC_IOCRREQ_PROPERTY                        ((PN_PNIOSTATUS_T)(0xDB810207UL))
#define PNIO_E_CMRPC_IOCRREQ_DATA_LEN                        ((PN_PNIOSTATUS_T)(0xDB810208UL))
#define PNIO_E_CMRPC_IOCRREQ_FRAME_ID                        ((PN_PNIOSTATUS_T)(0xDB810209UL))
#define PNIO_E_CMRPC_IOCRREQ_SENDCLOCK_FACT                  ((PN_PNIOSTATUS_T)(0xDB81020AUL))
#define PNIO_E_CMRPC_IOCRREQ_REDUCTIONRATIO                  ((PN_PNIOSTATUS_T)(0xDB81020BUL))
#define PNIO_E_CMRPC_IOCRREQ_PHASE                           ((PN_PNIOSTATUS_T)(0xDB81020CUL))
#define PNIO_E_CMRPC_IOCRREQ_FRAME_SEND_OFFSET               ((PN_PNIOSTATUS_T)(0xDB81020EUL))
#define PNIO_E_CMRPC_IOCRREQ_WATCHDOG_FACTOR                 ((PN_PNIOSTATUS_T)(0xDB81020FUL))
#define PNIO_E_CMRPC_IOCRREQ_DATA_HOLD_FACTOR                ((PN_PNIOSTATUS_T)(0xDB810210UL))
#define PNIO_E_CMRPC_IOCRREQ_TAG_HEADER                      ((PN_PNIOSTATUS_T)(0xDB810211UL))
#define PNIO_E_CMRPC_IOCRREQ_MC_ADDR                         ((PN_PNIOSTATUS_T)(0xDB810212UL))
#define PNIO_E_CMRPC_IOCRREQ_NUM_API                         ((PN_PNIOSTATUS_T)(0xDB810213UL))
#define PNIO_E_CMRPC_IOCRREQ_API                             ((PN_PNIOSTATUS_T)(0xDB810214UL))
#define PNIO_E_CMRPC_IOCRREQ_NUM_IO_DATA_OBJ                 ((PN_PNIOSTATUS_T)(0xDB810215UL))
#define PNIO_E_CMRPC_IOCRREQ_SLOT_IODATAOBJ                  ((PN_PNIOSTATUS_T)(0xDB810216UL))
#define PNIO_E_CMRPC_IOCRREQ_SUBSLOT_IODATAOBJ               ((PN_PNIOSTATUS_T)(0xDB810217UL))
#define PNIO_E_CMRPC_IOCRREQ_IODATAOBJ_FRAMEOFFSET           ((PN_PNIOSTATUS_T)(0xDB810218UL))
#define PNIO_E_CMRPC_IOCRREQ_NUM_IOCS                        ((PN_PNIOSTATUS_T)(0xDB810219UL))
#define PNIO_E_CMRPC_IOCRREQ_SLOT_IOCS                       ((PN_PNIOSTATUS_T)(0xDB81021AUL))
#define PNIO_E_CMRPC_IOCRREQ_SUBSLOT_IOCS                    ((PN_PNIOSTATUS_T)(0xDB81021BUL))
#define PNIO_E_CMRPC_IOCRREQ_IOCS_FRAMEOFFSET                ((PN_PNIOSTATUS_T)(0xDB81021CUL))

#define PNIO_E_CMRPC_ARRPCR_SERVER_PORT                      ((PN_PNIOSTATUS_T)(0xDB810704UL))

#define PNIO_E_CMRPC_IRINFO_IRDATAUUID                       ((PN_PNIOSTATUS_T)(0xDB810905UL))
#define PNIO_E_CMRPC_IRINFO_NUMIOCR                          ((PN_PNIOSTATUS_T)(0xDB810906UL))

#define PNIO_E_CMRPC_ARFSU_FASTSTARTUPBLOCK                  ((PN_PNIOSTATUS_T)(0xDB810B05UL))

  /* used by profinet controller when checking the ar server blockres */
#define PNIO_E_CTLRPC_ARSERVERBLOCKRES_BLOCKTYPE               ((PN_PNIOSTATUS_T)(0xDB813700UL))
#define PNIO_E_CTLRPC_ARSERVERBLOCKRES_BLOCKLEN                ((PN_PNIOSTATUS_T)(0xDB813701UL))
#define PNIO_E_CTLRPC_ARSERVERBLOCKRES_CMRESPONDERSTATIONNAME  ((PN_PNIOSTATUS_T)(0xDB813704UL))

  /* Used by profinet controller when checking the ARVendorBlockRes*/
#define PNIO_E_CTLRPC_ARVENDORBLOCKRES_BLOCKTYPE             ((PN_PNIOSTATUS_T)(0xDB813800UL))
#define PNIO_E_CTLRPC_ARVENDORBLOCKRES_BLOCKLEN              ((PN_PNIOSTATUS_T)(0xDB813801UL))
#define PNIO_E_CTLRPC_ARVENDORBLOCKRES_API                   ((PN_PNIOSTATUS_T)(0xDB813805UL))

  /* CMDEV - ErrorCode = 0x81, ErrorDecode = 0x81, ErrorCode1 = 0x3D (61) */
#define PNIO_E_CMDEV_STATE_CONFLICT                          ((PN_PNIOSTATUS_T)(0xDB813D00UL))
#define PNIO_E_CMDEV_CONNECT_RESOURCE                        ((PN_PNIOSTATUS_T)(0xDB813D01UL))
#define PNIO_E_CMDEV_CONTROL_RESOURCE                        ((PN_PNIOSTATUS_T)(0xDD813D01UL))

  /** CTLRPC Error codes */
#define PNIO_E_CTLRPC_INVALID_STATE                          ((PN_PNIOSTATUS_T)(0xDB816B00UL))
#define PNIO_E_CTLRPC_ERROR                                  ((PN_PNIOSTATUS_T)(0xDB816B00UL))


#define PNIO_E_IODCONNECTRES_CMCTL_STATECONFLICT             ((PN_PNIOSTATUS_T)(0xDB813E00UL))
#define PNIO_E_IODCONNECTRES_CMCTL_TIMEOUT                   ((PN_PNIOSTATUS_T)(0xDB813E01UL))
#define PNIO_E_IODCONNECTRES_CMCTL_NODATASEND                ((PN_PNIOSTATUS_T)(0xDB813E02UL))
#define PNIO_E_IODCONNECTRES_CMCTL_OUTOFRESOURECE            ((PN_PNIOSTATUS_T)(0xDB813E03UL))

#define PNIO_E_IODCTRL_SUBMODULELIST_TYPE                    ((PN_PNIOSTATUS_T)(0xDD811900UL))
#define PNIO_E_IODCTRL_SUBMODULELIST_BLOCK_LEN               ((PN_PNIOSTATUS_T)(0xDD811901UL))
#define PNIO_E_IODCTRL_SUBMODULELIST_BLOCK_MAJOR             ((PN_PNIOSTATUS_T)(0xDD811902UL))
#define PNIO_E_IODCTRL_SUBMODULELIST_BLOCK_MINOR             ((PN_PNIOSTATUS_T)(0xDD811903UL))
#define PNIO_E_IODCTRL_SUBMODULELIST_NUMBER_ENTRIES          ((PN_PNIOSTATUS_T)(0xDD811904UL))


  /** read record error codes, the lowest byte can be used for additional
   * user specific information.
   *
   * 0xDE800000 - 0xDE809FFF is reserved for future usage */

#define PNIO_E_IOD_READ_APPL_ERROR                           ((PN_PNIOSTATUS_T)(0xDE80A000UL))
#define PNIO_E_IOD_READ_APPL_MODULEFAIL                      ((PN_PNIOSTATUS_T)(0xDE80A200UL))
#define PNIO_E_IOD_READ_APPL_UNSPEC1                         ((PN_PNIOSTATUS_T)(0xDE80A300UL))
#define PNIO_E_IOD_READ_APPL_UNSPEC2                         ((PN_PNIOSTATUS_T)(0xDE80A400UL))
#define PNIO_E_IOD_READ_APPL_UNSPEC3                         ((PN_PNIOSTATUS_T)(0xDE80A500UL))
#define PNIO_E_IOD_READ_APPL_UNSPEC4                         ((PN_PNIOSTATUS_T)(0xDE80A600UL))
#define PNIO_E_IOD_READ_APPL_BUSY                            ((PN_PNIOSTATUS_T)(0xDE80A700UL))
#define PNIO_E_IOD_READ_APPL_VERSIONCONFLICT                 ((PN_PNIOSTATUS_T)(0xDE80A800UL))
#define PNIO_E_IOD_READ_APPL_UNSUPPORTED                     ((PN_PNIOSTATUS_T)(0xDE80A900UL))
#define PNIO_E_IOD_READ_APPL_USERSPEC1                       ((PN_PNIOSTATUS_T)(0xDE80AA00UL))
#define PNIO_E_IOD_READ_APPL_USERSPEC2                       ((PN_PNIOSTATUS_T)(0xDE80AB00UL))
#define PNIO_E_IOD_READ_APPL_USERSPEC3                       ((PN_PNIOSTATUS_T)(0xDE80AC00UL))
#define PNIO_E_IOD_READ_APPL_USERSPEC4                       ((PN_PNIOSTATUS_T)(0xDE80AD00UL))
#define PNIO_E_IOD_READ_APPL_USERSPEC5                       ((PN_PNIOSTATUS_T)(0xDE80AE00UL))
#define PNIO_E_IOD_READ_APPL_USERSPEC6                       ((PN_PNIOSTATUS_T)(0xDE80AF00UL))

#define PNIO_E_IOD_READ_ACCESS_INVALIDINDEX                  ((PN_PNIOSTATUS_T)(0xDE80B000UL))
#define PNIO_E_IOD_READ_ACCESS_INVALIDSLOT_SUBSLOT           ((PN_PNIOSTATUS_T)(0xDE80B200UL))
#define PNIO_E_IOD_READ_ACCESS_TYPECONFLICT                  ((PN_PNIOSTATUS_T)(0xDE80B300UL))
#define PNIO_E_IOD_READ_ACCESS_INVALIDAPI_AREA               ((PN_PNIOSTATUS_T)(0xDE80B400UL))
#define PNIO_E_IOD_READ_ACCESS_STATECONFLICT                 ((PN_PNIOSTATUS_T)(0xDE80B500UL))
#define PNIO_E_IOD_READ_ACCESS_DENIED                        ((PN_PNIOSTATUS_T)(0xDE80B600UL))
#define PNIO_E_IOD_READ_ACCESS_INVALIDRANGE                  ((PN_PNIOSTATUS_T)(0xDE80B700UL))
#define PNIO_E_IOD_READ_ACCESS_INVALIDRANGE_PADDING          ((PN_PNIOSTATUS_T)(0xDE80B709UL))
#define PNIO_E_IOD_READ_ACCESS_INVALIDPARAM                  ((PN_PNIOSTATUS_T)(0xDE80B800UL))
#define PNIO_E_IOD_READ_ACCESS_INVALIDTYPE                   ((PN_PNIOSTATUS_T)(0xDE80B900UL))
#define PNIO_E_IOD_READ_ACCESS_BACKUP                        ((PN_PNIOSTATUS_T)(0xDE80BA00UL))
#define PNIO_E_IOD_READ_ACCESS_USERSPEC7                     ((PN_PNIOSTATUS_T)(0xDE80BB00UL))
#define PNIO_E_IOD_READ_ACCESS_USERSPEC8                     ((PN_PNIOSTATUS_T)(0xDE80BC00UL))
#define PNIO_E_IOD_READ_ACCESS_USERSPEC9                     ((PN_PNIOSTATUS_T)(0xDE80BD00UL))
#define PNIO_E_IOD_READ_ACCESS_USERSPEC10                    ((PN_PNIOSTATUS_T)(0xDE80BE00UL))
#define PNIO_E_IOD_READ_ACCESS_USERSPEC11                    ((PN_PNIOSTATUS_T)(0xDE80BF00UL))

#define PNIO_E_IOD_READ_RESOURCE_CONSTRAINCONFLICT           ((PN_PNIOSTATUS_T)(0xDE80C000UL))
#define PNIO_E_IOD_READ_RESOURCE_BUSY                        ((PN_PNIOSTATUS_T)(0xDE80C200UL))
#define PNIO_E_IOD_READ_RESOURCE_UNAVAILABLE                 ((PN_PNIOSTATUS_T)(0xDE80C300UL))
#define PNIO_E_IOD_READ_RESOURCE_UNSPEC1                     ((PN_PNIOSTATUS_T)(0xDE80C400UL))
#define PNIO_E_IOD_READ_RESOURCE_UNSPEC2                     ((PN_PNIOSTATUS_T)(0xDE80C500UL))
#define PNIO_E_IOD_READ_RESOURCE_UNSPEC3                     ((PN_PNIOSTATUS_T)(0xDE80C600UL))
#define PNIO_E_IOD_READ_RESOURCE_UNSPEC4                     ((PN_PNIOSTATUS_T)(0xDE80C700UL))
#define PNIO_E_IOD_READ_RESOURCE_USERSPEC12                  ((PN_PNIOSTATUS_T)(0xDE80C800UL))
#define PNIO_E_IOD_READ_RESOURCE_USERSPEC13                  ((PN_PNIOSTATUS_T)(0xDE80C900UL))
#define PNIO_E_IOD_READ_RESOURCE_USERSPEC14                  ((PN_PNIOSTATUS_T)(0xDE80CA00UL))
#define PNIO_E_IOD_READ_RESOURCE_USERSPEC15                  ((PN_PNIOSTATUS_T)(0xDE80CB00UL))
#define PNIO_E_IOD_READ_RESOURCE_USERSPEC16                  ((PN_PNIOSTATUS_T)(0xDE80CC00UL))
#define PNIO_E_IOD_READ_RESOURCE_USERSPEC17                  ((PN_PNIOSTATUS_T)(0xDE80CD00UL))
#define PNIO_E_IOD_READ_RESOURCE_USERSPEC18                  ((PN_PNIOSTATUS_T)(0xDE80CE00UL))
#define PNIO_E_IOD_READ_RESOURCE_USERSPEC19                  ((PN_PNIOSTATUS_T)(0xDE80CF00UL))

  /** 0xDE80D000 - 0xDE80FFFF can be used for user specific read errors */

  /** write record error codes, the lowest byte can be used for additional
   * user specific information.
   *
   * 0xDF800000 - 0xDF809FFF is reserved for future usage */
#define PNIO_E_IOD_WRITE                                      ((PN_PNIOSTATUS_T)(0xDF800000UL))

#define PNIO_E_IOD_WRITE_APPL_ERROR                           ((PN_PNIOSTATUS_T)(0xDF80A100UL))
#define PNIO_E_IOD_WRITE_APPL_MODULEFAIL                      ((PN_PNIOSTATUS_T)(0xDF80A200UL))
#define PNIO_E_IOD_WRITE_APPL_UNSPEC1                         ((PN_PNIOSTATUS_T)(0xDF80A300UL))
#define PNIO_E_IOD_WRITE_APPL_UNSPEC2                         ((PN_PNIOSTATUS_T)(0xDF80A400UL))
#define PNIO_E_IOD_WRITE_APPL_UNSPEC3                         ((PN_PNIOSTATUS_T)(0xDF80A500UL))
#define PNIO_E_IOD_WRITE_APPL_UNSPEC4                         ((PN_PNIOSTATUS_T)(0xDF80A600UL))
#define PNIO_E_IOD_WRITE_APPL_BUSY                            ((PN_PNIOSTATUS_T)(0xDF80A700UL))
#define PNIO_E_IOD_WRITE_APPL_VERSIONCONFLICT                 ((PN_PNIOSTATUS_T)(0xDF80A800UL))
#define PNIO_E_IOD_WRITE_APPL_UNSUPPORTED                     ((PN_PNIOSTATUS_T)(0xDF80A900UL))
#define PNIO_E_IOD_WRITE_APPL_USERSPEC1                       ((PN_PNIOSTATUS_T)(0xDF80AA00UL))
#define PNIO_E_IOD_WRITE_APPL_USERSPEC2                       ((PN_PNIOSTATUS_T)(0xDF80AB00UL))
#define PNIO_E_IOD_WRITE_APPL_USERSPEC3                       ((PN_PNIOSTATUS_T)(0xDF80AC00UL))
#define PNIO_E_IOD_WRITE_APPL_USERSPEC4                       ((PN_PNIOSTATUS_T)(0xDF80AD00UL))
#define PNIO_E_IOD_WRITE_APPL_USERSPEC5                       ((PN_PNIOSTATUS_T)(0xDF80AE00UL))
#define PNIO_E_IOD_WRITE_APPL_USERSPEC6                       ((PN_PNIOSTATUS_T)(0xDF80AF00UL))

#define PNIO_E_IOD_WRITE_ACCESS_INVALIDINDEX                  ((PN_PNIOSTATUS_T)(0xDF80B000UL))
#define PNIO_E_IOD_WRITE_ACCESS_INVALIDLENGTH                 ((PN_PNIOSTATUS_T)(0xDF80B100UL))
#define PNIO_E_IOD_WRITE_ACCESS_INVALIDSLOT_SUBSLOT           ((PN_PNIOSTATUS_T)(0xDF80B200UL))
#define PNIO_E_IOD_WRITE_ACCESS_TYPECONFLICT                  ((PN_PNIOSTATUS_T)(0xDF80B300UL))
#define PNIO_E_IOD_WRITE_ACCESS_INVALIDAPI_AREA               ((PN_PNIOSTATUS_T)(0xDF80B400UL))
#define PNIO_E_IOD_WRITE_ACCESS_STATECONFLICT                 ((PN_PNIOSTATUS_T)(0xDF80B500UL))
#define PNIO_E_IOD_WRITE_ACCESS_DENIED                        ((PN_PNIOSTATUS_T)(0xDF80B600UL))
#define PNIO_E_IOD_WRITE_ACCESS_DENIED_OWNER                  ((PN_PNIOSTATUS_T)(0xDF80B601UL))
#define PNIO_E_IOD_WRITE_ACCESS_DENIED_SOL                    ((PN_PNIOSTATUS_T)(0xDF80B602UL))
#define PNIO_E_IOD_WRITE_ACCESS_INVALIDRANGE                  ((PN_PNIOSTATUS_T)(0xDF80B700UL))
#define PNIO_E_IOD_WRITE_ACCESS_INVALIDPARAM                  ((PN_PNIOSTATUS_T)(0xDF80B800UL))
#define PNIO_E_IOD_WRITE_ACCESS_INVALIDTYPE                   ((PN_PNIOSTATUS_T)(0xDF80B900UL))
#define PNIO_E_IOD_WRITE_ACCESS_BACKUP                        ((PN_PNIOSTATUS_T)(0xDF80BA00UL))
#define PNIO_E_IOD_WRITE_ACCESS_USERSPEC7                     ((PN_PNIOSTATUS_T)(0xDF80BB00UL))
#define PNIO_E_IOD_WRITE_ACCESS_USERSPEC8                     ((PN_PNIOSTATUS_T)(0xDF80BC00UL))
#define PNIO_E_IOD_WRITE_ACCESS_USERSPEC9                     ((PN_PNIOSTATUS_T)(0xDF80BD00UL))
#define PNIO_E_IOD_WRITE_ACCESS_USERSPEC10                    ((PN_PNIOSTATUS_T)(0xDF80BE00UL))
#define PNIO_E_IOD_WRITE_ACCESS_USERSPEC11                    ((PN_PNIOSTATUS_T)(0xDF80BF00UL))

#define PNIO_E_IOD_WRITE_RESOURCE_CONSTRAINCONFLICT           ((PN_PNIOSTATUS_T)(0xDF80C100UL))
#define PNIO_E_IOD_WRITE_RESOURCE_BUSY                        ((PN_PNIOSTATUS_T)(0xDF80C200UL))
#define PNIO_E_IOD_WRITE_RESOURCE_UNAVAILABLE                 ((PN_PNIOSTATUS_T)(0xDF80C300UL))
#define PNIO_E_IOD_WRITE_RESOURCE_UNSPEC1                     ((PN_PNIOSTATUS_T)(0xDF80C400UL))
#define PNIO_E_IOD_WRITE_RESOURCE_UNSPEC2                     ((PN_PNIOSTATUS_T)(0xDF80C500UL))
#define PNIO_E_IOD_WRITE_RESOURCE_UNSPEC3                     ((PN_PNIOSTATUS_T)(0xDF80C600UL))
#define PNIO_E_IOD_WRITE_RESOURCE_UNSPEC4                     ((PN_PNIOSTATUS_T)(0xDF80C700UL))
#define PNIO_E_IOD_WRITE_RESOURCE_USERSPEC12                  ((PN_PNIOSTATUS_T)(0xDF80C800UL))
#define PNIO_E_IOD_WRITE_RESOURCE_USERSPEC13                  ((PN_PNIOSTATUS_T)(0xDF80C900UL))
#define PNIO_E_IOD_WRITE_RESOURCE_USERSPEC14                  ((PN_PNIOSTATUS_T)(0xDF80CA00UL))
#define PNIO_E_IOD_WRITE_RESOURCE_USERSPEC15                  ((PN_PNIOSTATUS_T)(0xDF80CB00UL))
#define PNIO_E_IOD_WRITE_RESOURCE_USERSPEC16                  ((PN_PNIOSTATUS_T)(0xDF80CC00UL))
#define PNIO_E_IOD_WRITE_RESOURCE_USERSPEC17                  ((PN_PNIOSTATUS_T)(0xDF80CD00UL))
#define PNIO_E_IOD_WRITE_RESOURCE_USERSPEC18                  ((PN_PNIOSTATUS_T)(0xDF80CE00UL))
#define PNIO_E_IOD_WRITE_RESOURCE_USERSPEC19                  ((PN_PNIOSTATUS_T)(0xDF80CF00UL))

  /** 0xDF80D000 - 0xDF80FFFF can be used for user specific write errors */


  /* the following error codes shall NOT be used by a user application */
  /* they shall only be used by the stack itself */

  /** IOX decode */
#define PNIO_E_DECODE_IOX                                     ((PN_PNIOSTATUS_T)(0xDB810000UL))

  /* AlarmAck - ErrorCode = 0xDA, ErrorDecode = 0x81, ErrorCode1 = 0x3C (60) */
  /** alarm acknowledge, alarm type is not supported */
#define PNIO_E_ALARM_ACK_TYPE_NOT_SUPPORTED                  ((PN_PNIOSTATUS_T)(0xDA813C00UL))
  /** alarm acknowledge, submodule has wrong state */
#define PNIO_E_ALARM_ACK_WRONG_SUBMODULE_STATUS              ((PN_PNIOSTATUS_T)(0xDA813C01UL))
  /** alarm acknowledged but shall be retransmitted
   * as soon as switch over is performed  */
#define PNIO_E_ALARM_ACK_BACKUP_ALARM_NOT_EXECUTED           ((PN_PNIOSTATUS_T)(0xDA813C02UL))

  /*
   * IOXControlRes (IODControlRes, IOCControlRes, IOSControlRes) - connection establishment
   * ErrorCode = 0xDD, ErrorDecode = 0x81, ErrorCode1 = 0x16 (22)
   */
#define PNIO_E_IOX_CONN                                      ((PN_PNIOSTATUS_T)(0x00001600UL))
#define PNIO_E_IOX_CONN_BLOCK_TYPE                           ((PN_PNIOSTATUS_T)(0xDD811600UL))
#define PNIO_E_IOX_CONN_BLOCK_LEN                            ((PN_PNIOSTATUS_T)(0xDD811601UL))
#define PNIO_E_IOX_CONN_BLOCK_MAJOR                          ((PN_PNIOSTATUS_T)(0xDD811602UL))
#define PNIO_E_IOX_CONN_BLOCK_MINOR                          ((PN_PNIOSTATUS_T)(0xDD811603UL))
#define PNIO_E_IOX_CONN_ARUUID                               ((PN_PNIOSTATUS_T)(0xDD811605UL))
#define PNIO_E_IOX_CONN_SESSION_KEY                          ((PN_PNIOSTATUS_T)(0xDD811606UL))
#define PNIO_E_IOX_CONN_CONTROL_COMMAND                      ((PN_PNIOSTATUS_T)(0xDD811608UL))
#define PNIO_E_IOX_CONN_CONTROL_PROPERTY                     ((PN_PNIOSTATUS_T)(0xDD811609UL))

  /*
   * IOXControlRes (IODControlRes, IOCControlRes, IOSControlRes) - a plug alarm
   * ErrorCode = 0xDD, ErrorDecode = 0x81, ErrorCode1 = 0x17 (23)
   */
#define PNIO_E_IOX_PLUG                                      ((PN_PNIOSTATUS_T)(0x00001700UL))
#define PNIO_E_IOX_PLUG_BLOCK_TYPE                           ((PN_PNIOSTATUS_T)(0xDD811700UL))
#define PNIO_E_IOX_PLUG_BLOCK_LEN                            ((PN_PNIOSTATUS_T)(0xDD811701UL))
#define PNIO_E_IOX_PLUG_BLOCK_MAJOR                          ((PN_PNIOSTATUS_T)(0xDD811702UL))
#define PNIO_E_IOX_PLUG_BLOCK_MINOR                          ((PN_PNIOSTATUS_T)(0xDD811703UL))
#define PNIO_E_IOX_PLUG_ARUUID                               ((PN_PNIOSTATUS_T)(0xDD811705UL))
#define PNIO_E_IOX_PLUG_SESSION_KEY                          ((PN_PNIOSTATUS_T)(0xDD811706UL))
#define PNIO_E_IOX_PLUG_CONTROL_COMMAND                      ((PN_PNIOSTATUS_T)(0xDD811708UL))
#define PNIO_E_IOX_PLUG_CONTROL_PROPERTY                     ((PN_PNIOSTATUS_T)(0xDD811709UL))


  /* CMCTL - ErrorCode = 0x81, ErrorDecode = 0x81, ErrorCode1 = 0x3E (62) */
#define PNIO_E_CMCTL_STATE_CONFLICT                          ((PN_PNIOSTATUS_T)(0x81813E01UL))
#define PNIO_E_CMCTL_TIME_OUT                                ((PN_PNIOSTATUS_T)(0x81813E02UL))
#define PNIO_E_CMCTL_NO_DATA_SEND                            ((PN_PNIOSTATUS_T)(0x81813E03UL))


  /* RMPM - ErrorCode = Service, ErrorDecode = 0x81, ErrorCode1 = 0x40 (64) */
#define PNIO_E_RMPM_CONNECT_ARGS_LEN_INVALID                 ((PN_PNIOSTATUS_T)(0xDB814000UL))
#define PNIO_E_RMPM_CONNECT_UNKNOWN_BLOCKS                   ((PN_PNIOSTATUS_T)(0xDB814001UL))
#define PNIO_E_RMPM_CONNECT_IOCR_MISSING                     ((PN_PNIOSTATUS_T)(0xDB814002UL))
#define PNIO_E_RMPM_CONNECT_WRONG_ALCR_BLOCK_COUNT           ((PN_PNIOSTATUS_T)(0xDB814003UL))
#define PNIO_E_RMPM_CONNECT_OUT_OF_AR_RESOURCES              ((PN_PNIOSTATUS_T)(0xDB814004UL))
#define PNIO_E_RMPM_STATE_CONFLICT                           ((PN_PNIOSTATUS_T)(0xDB814006UL))
#define PNIO_E_RMPM_OUT_OF_PROV_CONS_OR_ALARM_RESOURCES      ((PN_PNIOSTATUS_T)(0xDB814007UL))
#define PNIO_E_RMPM_OUT_OF_MEMORY                            ((PN_PNIOSTATUS_T)(0xDB814008UL))
#define PNIO_E_CMRPC_PDEV_ALREADY_OWNED                      ((PN_PNIOSTATUS_T)(0xDB814009UL))
#define PNIO_E_CMRPC_ARSET_STATE_CONFLICT_DURING_CONNECT     ((PN_PNIOSTATUS_T)(0xDB81400AUL))
#define PNIO_E_CMRPC_ARSET_PARAMETER_CONFLICT_DURING_CONNECT ((PN_PNIOSTATUS_T)(0xDB81400BUL))
#define PNIO_E_RMPM_WRONG_SRINFO_BLOCK_COUNT_HILSCHER_VALUE  ((PN_PNIOSTATUS_T)(0xDB8140F0UL))
#define PNIO_E_RMPM_UNSPECIFIC_ERROR                         ((PN_PNIOSTATUS_T)(0xDB8140FFUL))

#define PNIO_E_RMPM_CONTROL_ARGS_LEN_INVALID                 ((PN_PNIOSTATUS_T)(0xDD814000UL))
#define PNIO_E_RMPM_CONTROL_UNKNOWN_BLOCKS                   ((PN_PNIOSTATUS_T)(0xDD814001UL))
#define PNIO_E_RMPM_CONTROL_AR_UUID_UNKNOWN                  ((PN_PNIOSTATUS_T)(0xDD814005UL))

#define PNIO_E_RMPM_RELEASE_ARGS_LEN_INVALID                 ((PN_PNIOSTATUS_T)(0xDC814000UL))
#define PNIO_E_RMPM_RELEASE_AR_UUID_UNKNOWN                  ((PN_PNIOSTATUS_T)(0xDC814005UL))

#define PNIO_E_RWRECORD_PARAM_ERROR_TARGETARUUID             ((PN_PNIOSTATUS_T)(0xDE81080CUL))

#define PNIO_E_RMPM_READ_ARGS_LEN_INVALID                    ((PN_PNIOSTATUS_T)(0xDE814000UL))
#define PNIO_E_RMPM_READ_AR_UUID_UNKNOWN                     ((PN_PNIOSTATUS_T)(0xDE814005UL))

#define PNIO_E_RMPM_WRITE_ARGS_LEN_INVALID                   ((PN_PNIOSTATUS_T)(0xDF814000UL))
#define PNIO_E_RMPM_WRITE_AR_UUID_UNKNOWN                    ((PN_PNIOSTATUS_T)(0xDF814005UL))


  /* ALPMI - ErrorCode = 0xCF, ErrorDecode = 0x81, ErrorCode1 = 0x41 (65) */
#define PNIO_E_ALPMI_INVALID_STATE                           ((PN_PNIOSTATUS_T)(0xCF814100UL))
#define PNIO_E_ALPMI_WRONG_ACK_PDU                           ((PN_PNIOSTATUS_T)(0xCF814101UL))
#define PNIO_E_ALPMI_INVALID                                 ((PN_PNIOSTATUS_T)(0xCF814102UL))

  /* ALPMR - ErrorCode = 0xCF, ErrorDecode = 0x81, ErrorCode1 = 0x42 (66) */
#define PNIO_E_ALPMR_INVALID_STATE                           ((PN_PNIOSTATUS_T)(0xCF814200UL))
#define PNIO_E_ALPMR_WRONG_NOTIFICATION_PDU                  ((PN_PNIOSTATUS_T)(0xCF814201UL))

  /* APMR - ErrorCode = 0x81, ErrorDecode = 0x81, ErrorCode1 = 0x46 (70) */
#define PNIO_E_APMR_STATE_CONFLICT                           ((PN_PNIOSTATUS_T)(0xCF814600UL))
#define PNIO_E_APMR_LMPM_SIGNALED_ERROR                      ((PN_PNIOSTATUS_T)(0xCF814601UL))
#define PNIO_E_APMR_UNSPECIFIC_ERROR                         ((PN_PNIOSTATUS_T)(0xCF8146FFUL))

  /* APMS - ErrorCode = 0x81, ErrorDecode = 0x81, ErrorCode1 = 0x47 (71) */
#define PNIO_E_APMS_STATE_CONFLICT                           ((PN_PNIOSTATUS_T)(0xCF814700UL))
#define PNIO_E_APMS_LMPM_SIGNALED_ERROR                      ((PN_PNIOSTATUS_T)(0xCF814701UL))
#define PNIO_E_APMS_PROTOCOL_TIMEOUT                         ((PN_PNIOSTATUS_T)(0xCF814702UL))
#define PNIO_E_APMS_UNSPECIFIC_ERROR                         ((PN_PNIOSTATUS_T)(0xCF8147FFUL))

  /* CPM - ErrorCode = 0x81, ErrorDecode = 0x81, ErrorCode1 = 0x48 (72) */
#define PNIO_E_CPM_STATE_CONFLICT                            ((PN_PNIOSTATUS_T)(0xCF814800UL))

  /* PPM - ErrorCode = 0x81, ErrorDecode = 0x81, ErrorCode1 = 0x49 (73) */
#define PNIO_E_PPM_STATE_CONFLICT                            ((PN_PNIOSTATUS_T)(0xCF814900UL))

  /* DCPUCS - ErrorCode = 0x81, ErrorDecode = 0x81, ErrorCode1 = 0x4A (74) */
#define PNIO_E_DCPUCS_STATE_CONFLICT                         ((PN_PNIOSTATUS_T)(0xCF814A00UL))
#define PNIO_E_DCPUCS_LMPM_SIGNALED_ERROR                    ((PN_PNIOSTATUS_T)(0xCF814A01UL))
#define PNIO_E_DCPUCS_TIMEOUT                                ((PN_PNIOSTATUS_T)(0xCF814A02UL))
#define PNIO_E_DCPUCS_UNSPECIFIC_ERROR                       ((PN_PNIOSTATUS_T)(0xCF814AFFUL))

  /* DCPUCR - ErrorCode = 0x81, ErrorDecode = 0x81, ErrorCode1 = 0x4B (75) */
#define PNIO_E_DCPUCR_STATE_CONFLICT                         ((PN_PNIOSTATUS_T)(0x81814B00UL))
#define PNIO_E_DCPUCR_LMPM_SIGNALED_ERROR                    ((PN_PNIOSTATUS_T)(0x81814A01UL))
#define PNIO_E_DCPUCR_UNSPECIFIC_ERROR                       ((PN_PNIOSTATUS_T)(0x81814BFFUL))

  /* DCPMCS - ErrorCode = 0x81, ErrorDecode = 0x81, ErrorCode1 = 0x4C (76) */
#define PNIO_E_DCPMCS_STATE_CONFLICT                         ((PN_PNIOSTATUS_T)(0x81814C00UL))
#define PNIO_E_DCPMCS_LMPM_SIGNALED_ERROR                    ((PN_PNIOSTATUS_T)(0x81814A01UL))
#define PNIO_E_DCPMCS_UNSPECIFIC_ERROR                       ((PN_PNIOSTATUS_T)(0x81814CFFUL))

  /* DCPMCR - ErrorCode = 0x81, ErrorDecode = 0x81, ErrorCode1 = 0x4D (77) */
#define PNIO_E_DCPMCR_STATE_CONFLICT                         ((PN_PNIOSTATUS_T)(0x81814D00UL))
#define PNIO_E_DCPMCR_LMPM_SIGNALED_ERROR                    ((PN_PNIOSTATUS_T)(0x81814A01UL))
#define PNIO_E_DCPMCR_UNSPECIFIC_ERROR                       ((PN_PNIOSTATUS_T)(0x81814DFFUL))

  /* RTA - ErrorCode = 0xCF, ErrorDecode = 0x81, ErrorCode1 = 0xFD (253) */
#define PNIO_E_RTA_PROTOCOL_RESERVED_0                       ((PN_PNIOSTATUS_T)(0xCF81FD00UL))
#define PNIO_E_RTA_PROTOCOL_SEQUENCE_NUMBER                  ((PN_PNIOSTATUS_T)(0xCF81FD01UL))
#define PNIO_E_RTA_PROTOCOL_INSTANCE_CLOSED                  ((PN_PNIOSTATUS_T)(0xCF81FD02UL))
#define PNIO_E_RTA_PROTOCOL_OUT_OF_MEMORY                    ((PN_PNIOSTATUS_T)(0xCF81FD03UL))
#define PNIO_E_RTA_PROTOCOL_ADD_PM_FAILED                    ((PN_PNIOSTATUS_T)(0xCF81FD04UL))
#define PNIO_E_RTA_PROTOCOL_DHT_WDT_EXPIRED                  ((PN_PNIOSTATUS_T)(0xCF81FD05UL))
#define PNIO_E_RTA_PROTOCOL_CMI_TIMEOUT                      ((PN_PNIOSTATUS_T)(0xCF81FD06UL))
#define PNIO_E_RTA_PROTOCOL_ALARM_OPEN_FAILED                ((PN_PNIOSTATUS_T)(0xCF81FD07UL))
#define PNIO_E_RTA_PROTOCOL_ALARM_SEND_NEG                   ((PN_PNIOSTATUS_T)(0xCF81FD08UL))
#define PNIO_E_RTA_PROTOCOL_ALARM_SEND_NEG_ACK               ((PN_PNIOSTATUS_T)(0xCF81FD09UL))
#define PNIO_E_RTA_PROTOCOL_ALARM_DATA_TOO_LONG              ((PN_PNIOSTATUS_T)(0xCF81FD0AUL))
#define PNIO_E_RTA_PROTOCOL_ALARM_IND_ERR                    ((PN_PNIOSTATUS_T)(0xCF81FD0BUL))
#define PNIO_E_RTA_PROTOCOL_RPC_CLIENT_CALL_NEG              ((PN_PNIOSTATUS_T)(0xCF81FD0CUL))
#define PNIO_E_RTA_PROTOCOL_ABORT_REQ                        ((PN_PNIOSTATUS_T)(0xCF81FD0DUL))
#define PNIO_E_RTA_PROTOCOL_RERUN_ABORT_EXISTING             ((PN_PNIOSTATUS_T)(0xCF81FD0EUL))
#define PNIO_E_RTA_PROTOCOL_RELEASE_IND_RECEIVED             ((PN_PNIOSTATUS_T)(0xCF81FD0FUL))
#define PNIO_E_RTA_PROTOCOL_DEVICE_DEACTIVATED               ((PN_PNIOSTATUS_T)(0xCF81FD10UL))
#define PNIO_E_RTA_PROTOCOL_REMOVED                          ((PN_PNIOSTATUS_T)(0xCF81FD11UL))
#define PNIO_E_RTA_PROTOCOL_PROTOCOL_VIOLATION               ((PN_PNIOSTATUS_T)(0xCF81FD12UL))
#define PNIO_E_RTA_PROTOCOL_NAME_RESOLUTION_ERR              ((PN_PNIOSTATUS_T)(0xCF81FD13UL))
#define PNIO_E_RTA_PROTOCOL_RPC_BIND_ERR                     ((PN_PNIOSTATUS_T)(0xCF81FD14UL))
#define PNIO_E_RTA_PROTOCOL_RPC_CONNECT_ERR                  ((PN_PNIOSTATUS_T)(0xCF81FD15UL))
#define PNIO_E_RTA_PROTOCOL_RPC_READ_ERR                     ((PN_PNIOSTATUS_T)(0xCF81FD16UL))
#define PNIO_E_RTA_PROTOCOL_RPC_WRITE_ERR                    ((PN_PNIOSTATUS_T)(0xCF81FD17UL))
#define PNIO_E_RTA_PROTOCOL_RPC_CONTROL_ERR                  ((PN_PNIOSTATUS_T)(0xCF81FD18UL))
#define PNIO_E_RTA_PROTOCOL_FORBIDDEN_PULL_OR_PLUG           ((PN_PNIOSTATUS_T)(0xCF81FD19UL))
#define PNIO_E_RTA_PROTOCOL_AP_REMOVED                       ((PN_PNIOSTATUS_T)(0xCF81FD1AUL))
#define PNIO_E_RTA_PROTOCOL_LINK_DOWN                        ((PN_PNIOSTATUS_T)(0xCF81FD1BUL))
#define PNIO_E_RTA_PROTOCOL_REGISTER_MC_MAC_FAILED           ((PN_PNIOSTATUS_T)(0xCF81FD1CUL))
#define PNIO_E_RTA_PROTOCOL_NOT_SYNCHRONIZED                 ((PN_PNIOSTATUS_T)(0xCF81FD1DUL))
#define PNIO_E_RTA_PROTOCOL_WRONG_TOPOLOGY                   ((PN_PNIOSTATUS_T)(0xCF81FD1EUL))
#define PNIO_E_RTA_PROTOCOL_DCP_NAMEOFSTATION_CHANGED        ((PN_PNIOSTATUS_T)(0xCF81FD1FUL))
#define PNIO_E_RTA_PROTOCOL_DCP_RESET_FACTORY_SETTINGS       ((PN_PNIOSTATUS_T)(0xCF81FD20UL))
#define PNIO_E_RTA_PROTOCOL_COMP_AR_0X8IPP_SUBMOD_FIRST_AR   ((PN_PNIOSTATUS_T)(0xCF81FD21UL))
#define PNIO_E_RTA_PROTOCOL_NO_IR_DATA_RECORD_YET            ((PN_PNIOSTATUS_T)(0xCF81FD22UL))
#define PNIO_E_RTA_PROTOCOL_PDEV                             ((PN_PNIOSTATUS_T)(0xCF81FD23UL))
#define PNIO_E_RTA_PROTOCOL_PDEV_NO_PORT_OFFERS_REQ_SPEED_DUPLEXITY  ((PN_PNIOSTATUS_T)(0xCF81FD24UL))
#define PNIO_E_RTA_PROTOCOL_IP_SUITE_OF_IOC_CHANGED          ((PN_PNIOSTATUS_T)(0xCF81FD25UL))
#define PNIO_E_RTA_PROTOCOL_IOCARSR_RDHT_EXPIRED             ((PN_PNIOSTATUS_T)(0xCF81FD26UL))
#define PNIO_E_RTA_PROTOCOL_IOCARSR_PDEV_PARAM_IMPOSIIBLE    ((PN_PNIOSTATUS_T)(0xCF81FD27UL))
#define PNIO_E_RTA_PROTOCOL_IOCARSR_REMOTE_APP_TIMEOUT       ((PN_PNIOSTATUS_T)(0xCF81FD28UL))
#define PNIO_E_RTA_PROTOCOL_IOCARSR_MTOT_EXPIRED             ((PN_PNIOSTATUS_T)(0xCF81FD2AUL))
#define PNIO_E_RTA_PROTOCOL_IOCARSR_VIOLATION                ((PN_PNIOSTATUS_T)(0xCF81FD2BUL))

#define PNIO_E_RTA_PROTOCOL_RESERVED_39                      ((PN_PNIOSTATUS_T)(0xCF81FD2CUL))
#define PNIO_E_RTA_PROTOCOL_RESERVED_200                     ((PN_PNIOSTATUS_T)(0xCF81FDC8UL))
#define PNIO_E_RTA_PROTOCOL_MANUF_SPECIFIC_START             ((PN_PNIOSTATUS_T)(0xCF81FDC9UL))

#define PNIO_E_RTA_PROTOCOL_REMOVED_AP_WATCHDOG_TIMEOUT      ((PN_PNIOSTATUS_T)(0xCF81FDC9UL))
#define PNIO_E_RTA_PROTOCOL_REMOVED_AP_POOL_EMPTY            ((PN_PNIOSTATUS_T)(0xCF81FDCAUL))
#define PNIO_E_RTA_PROTOCOL_REMOVED_AP_QUE_SENDPACKET_ERROR  ((PN_PNIOSTATUS_T)(0xCF81FDCBUL))
#define PNIO_E_RTA_PROTOCOL_REMOVED_AP_OUTOFMEMORY           ((PN_PNIOSTATUS_T)(0xCF81FDCCUL))
#define PNIO_E_RTA_PROTOCOL_CTLACD_ADDRESSCONFLICT           ((PN_PNIOSTATUS_T)(0xCF81FDCDUL))

#define PNIO_E_RTA_PROTOCOL_MANUF_SPECIFIC_END               ((PN_PNIOSTATUS_T)(0xCF81FDFFUL))

/** PNIOStatus IOX */
#define PNIO_E_DECODE_IOC                                    ((PN_PNIOSTATUS_T)(0xDD810000UL))
#define PNIO_E_DECODE_IOD                                    ((PN_PNIOSTATUS_T)(0xDB810000UL))
#define PNIO_E_DECODE_IOX_RW_PN                              ((PN_PNIOSTATUS_T)(0xDF810000UL))
#define PNIO_E_DECODE_IOX_RW                                 ((PN_PNIOSTATUS_T)(0xDF800000UL))
#define PNIO_E_DECODE_IOX_REL                                ((PN_PNIOSTATUS_T)(0xDC810000UL))
#define PNIO_E_DECODE_IOX_RR_PN                              ((PN_PNIOSTATUS_T)(0xDE810000UL))
#define PNIO_E_DECODE_IOX_RR                                 ((PN_PNIOSTATUS_T)(0xDE800000UL))

/** PNIOStatus IOX BLOCK */
#define PNIO_E_DECODE_IOX_AR                                 ((PN_PNIOSTATUS_T)(0x00000100UL))
#define PNIO_E_DECODE_IOX_IOCR                               ((PN_PNIOSTATUS_T)(0x00000200UL))
#define PNIO_E_DECODE_IOX_EXPS                               ((PN_PNIOSTATUS_T)(0x00000300UL))
#define PNIO_E_DECODE_IOX_ALCR                               ((PN_PNIOSTATUS_T)(0x00000400UL))
#define PNIO_E_DECODE_IOX_ARRPC                              ((PN_PNIOSTATUS_T)(0x00000700UL))
#define PNIO_E_DECODE_IOX_RWREQ                              ((PN_PNIOSTATUS_T)(0x00000800UL))
#define PNIO_E_DECODE_IOX_IRINFO                             ((PN_PNIOSTATUS_T)(0x00000900UL))
#define PNIO_E_DECODE_IOX_SRINFO                             ((PN_PNIOSTATUS_T)(0x00000A00UL))
#define PNIO_E_DECODE_IOX_ARFSU                              ((PN_PNIOSTATUS_T)(0x00000B00UL))
#define PNIO_E_DECODE_IOX_IODCTRL                            ((PN_PNIOSTATUS_T)(0x00001400UL))
#define PNIO_E_DECODE_IOX_IODCTRL_PLUG                       ((PN_PNIOSTATUS_T)(0x00001500UL))
#define PNIO_E_DECODE_IOX_IODCTRL_PRMBEGIN                   ((PN_PNIOSTATUS_T)(0x00001800UL))
#define PNIO_E_DECODE_IOX_IODCTRL_SUBMODULELIST              ((PN_PNIOSTATUS_T)(0x00001900UL))
#define PNIO_E_DECODE_IOX_RELEASE                            ((PN_PNIOSTATUS_T)(0x00002800UL))
#define PNIO_E_DECODE_RTA_PROTOCOL                           ((PN_PNIOSTATUS_T)(0x0000FD00UL))

/** PNIOStatus IOX BLOCK AR Req */
  /* IOX code */
#define PNIO_E_CODE_IOX_BLOCK_TYPE                           ((PN_PNIOSTATUS_T)(0x00000000UL))
#define PNIO_E_CODE_IOX_BLOCK_LEN                            ((PN_PNIOSTATUS_T)(0x00000001UL))
#define PNIO_E_CODE_IOX_BLOCK_MAJOR                          ((PN_PNIOSTATUS_T)(0x00000002UL))
#define PNIO_E_CODE_IOX_BLOCK_MINOR                          ((PN_PNIOSTATUS_T)(0x00000003UL))
#define PNIO_E_CODE_IOX_TYPE                                 ((PN_PNIOSTATUS_T)(0x00000004UL))
#define PNIO_E_CODE_IOX_ARUUID                               ((PN_PNIOSTATUS_T)(0x00000005UL))
#define PNIO_E_CODE_IOX_SESSION_KEY                          ((PN_PNIOSTATUS_T)(0x00000006UL))
#define PNIO_E_CODE_IOX_MAC                                  ((PN_PNIOSTATUS_T)(0x00000007UL))
#define PNIO_E_CODE_IOX_CMI_OBJECTUUID                       ((PN_PNIOSTATUS_T)(0x00000008UL))
#define PNIO_E_CODE_IOX_AR_PROPERTY                          ((PN_PNIOSTATUS_T)(0x00000009UL))
#define PNIO_E_CODE_IOX_CLOCK_FACTOR                         ((PN_PNIOSTATUS_T)(0x0000000AUL))
#define PNIO_E_CODE_IOX_UDP_RTPORT                           ((PN_PNIOSTATUS_T)(0x0000000BUL))
#define PNIO_E_CODE_IOX_STATION_NAME_LEN                     ((PN_PNIOSTATUS_T)(0x0000000CUL))
#define PNIO_E_CODE_IOX_STATION_NAME                         ((PN_PNIOSTATUS_T)(0x0000000DUL))

/** PNIOStatus IOX BLOCK IOCR Req */
#define PNIO_E_CODE_IOX_IOCR_REF                             ((PN_PNIOSTATUS_T)(0x00000005UL))
#define PNIO_E_CODE_IOX_IOCR_LT                              ((PN_PNIOSTATUS_T)(0x00000006UL))
#define PNIO_E_CODE_IOX_IOCR_PROPERTY                        ((PN_PNIOSTATUS_T)(0x00000007UL))
#define PNIO_E_CODE_IOX_IOCR_DATA_LEN                        ((PN_PNIOSTATUS_T)(0x00000008UL))
#define PNIO_E_CODE_IOX_IOCR_FRAME_ID                        ((PN_PNIOSTATUS_T)(0x00000009UL))
#define PNIO_E_CODE_IOX_IOCR_SENDCLOCK_FACT                  ((PN_PNIOSTATUS_T)(0x0000000AUL))
#define PNIO_E_CODE_IOX_IOCR_REDUCTION_RATIO                 ((PN_PNIOSTATUS_T)(0x0000000BUL))
#define PNIO_E_CODE_IOX_IOCR_PHASE                           ((PN_PNIOSTATUS_T)(0x0000000CUL))
#define PNIO_E_CODE_IOX_IOCR_FRAME_SEND_OFFSET               ((PN_PNIOSTATUS_T)(0x0000000EUL))
#define PNIO_E_CODE_IOX_IOCR_WATCHDOG_FACTOR                 ((PN_PNIOSTATUS_T)(0x0000000FUL))
#define PNIO_E_CODE_IOX_IOCR_DATA_HOLD_FACTOR                ((PN_PNIOSTATUS_T)(0x00000010UL))
#define PNIO_E_CODE_IOX_IOCR_TAG_HEADER                      ((PN_PNIOSTATUS_T)(0x00000011UL))
#define PNIO_E_CODE_IOX_IOCR_MC_ADDR                         ((PN_PNIOSTATUS_T)(0x00000012UL))
#define PNIO_E_CODE_IOX_IOCR_NUM_API                         ((PN_PNIOSTATUS_T)(0x00000013UL))
#define PNIO_E_CODE_IOX_IOCR_API                             ((PN_PNIOSTATUS_T)(0x00000014UL))
#define PNIO_E_CODE_IOX_IOCR_NUM_IO_DATA_OBJ                 ((PN_PNIOSTATUS_T)(0x00000015UL))
#define PNIO_E_CODE_IOX_IOCR_SLOT_IO_DATA_OBJ                ((PN_PNIOSTATUS_T)(0x00000016UL))
#define PNIO_E_CODE_IOX_IOCR_SUBSLOT_IO_DATA_OBJ             ((PN_PNIOSTATUS_T)(0x00000017UL))
#define PNIO_E_CODE_IOX_IOCR_IO_DATA_OBJ_FRAME_OFFSET        ((PN_PNIOSTATUS_T)(0x00000018UL))
#define PNIO_E_CODE_IOX_IOCR_NUM_IOCS                        ((PN_PNIOSTATUS_T)(0x00000019UL))
#define PNIO_E_CODE_IOX_IOCR_SLOT_IOCS                       ((PN_PNIOSTATUS_T)(0x0000001AUL))
#define PNIO_E_CODE_IOX_IOCR_SUBSLOT_IOCS                    ((PN_PNIOSTATUS_T)(0x0000001BUL))
#define PNIO_E_CODE_IOX_IOCR_IOCS_FRAME_OFFSET               ((PN_PNIOSTATUS_T)(0x0000001CUL))

/** PNIOStatus IOX BLOCK EXPS Req */
#define PNIO_E_CODE_IOX_EXPS_API                             ((PN_PNIOSTATUS_T)(0x00000005UL))
#define PNIO_E_CODE_IOX_EXPS_SLOT                            ((PN_PNIOSTATUS_T)(0x00000006UL))
#define PNIO_E_CODE_IOX_EXPS_MODULE_ID                       ((PN_PNIOSTATUS_T)(0x00000007UL))
#define PNIO_E_CODE_IOX_EXPS_PROP                            ((PN_PNIOSTATUS_T)(0x00000008UL))
#define PNIO_E_CODE_IOX_EXPS_NR_SUBS                         ((PN_PNIOSTATUS_T)(0x00000009UL))
#define PNIO_E_CODE_IOX_EXPS_SUBSLOT                         ((PN_PNIOSTATUS_T)(0x0000000AUL))
#define PNIO_E_CODE_IOX_EXPS_UNUSED_1                        ((PN_PNIOSTATUS_T)(0x0000000BUL))
#define PNIO_E_CODE_IOX_EXPS_SUBMODULE_PROP                  ((PN_PNIOSTATUS_T)(0x0000000CUL))
#define PNIO_E_CODE_IOX_EXPS_DATA_DESCRIPTION                ((PN_PNIOSTATUS_T)(0x0000000DUL))
#define PNIO_E_CODE_IOX_EXPS_LENGTH_SUBMODULE_DATA           ((PN_PNIOSTATUS_T)(0x0000000EUL))
#define PNIO_E_CODE_IOX_EXPS_LENGTH_IOPS                     ((PN_PNIOSTATUS_T)(0x0000000FUL))
#define PNIO_E_CODE_IOX_EXPS_LENGTH_IOCS                     ((PN_PNIOSTATUS_T)(0x00000010UL))

/** PNIOStatus IOX BLOCK ALCR Req */
  /* IOX code */
#define PNIO_E_CODE_IOX_ALCR_TYPE                            ((PN_PNIOSTATUS_T)(0x00000004UL))
#define PNIO_E_CODE_IOX_ALCR_LT                              ((PN_PNIOSTATUS_T)(0x00000005UL))
#define PNIO_E_CODE_IOX_ALCR_PROPERTIES                      ((PN_PNIOSTATUS_T)(0x00000006UL))
#define PNIO_E_CODE_IOX_ALCR_RTA_FACTOR                      ((PN_PNIOSTATUS_T)(0x00000007UL))
#define PNIO_E_CODE_IOX_ALCR_RTA_RETRIES                     ((PN_PNIOSTATUS_T)(0x00000008UL))
#define PNIO_E_CODE_IOX_ALCR_NOT_CHECKED_1                   ((PN_PNIOSTATUS_T)(0x00000009UL))
#define PNIO_E_CODE_IOX_ALCR_MAX_ALARM_DATA_LEN              ((PN_PNIOSTATUS_T)(0x0000000AUL))
#define PNIO_E_CODE_IOX_ALCR_ALARM_PRIO_HIGH                 ((PN_PNIOSTATUS_T)(0x0000000BUL))
#define PNIO_E_CODE_IOX_ALCR_ALARM_PRIO_LOW                  ((PN_PNIOSTATUS_T)(0x0000000CUL))

/** PNIOStatus IOX BLOCK IORW Req */
#define PNIO_E_CODE_IOX_IORW_ARUUID                          ((PN_PNIOSTATUS_T)(0x00000005UL))
#define PNIO_E_CODE_IOX_IORW_API                             ((PN_PNIOSTATUS_T)(0x00000006UL))
#define PNIO_E_CODE_IOX_IORW_PADDING                         ((PN_PNIOSTATUS_T)(0x00000009UL))

/** PNIOStatus IOX BLOCK IORW Req */
#define PNIO_E_DECODE_IOX_SRINFO_RDHT                        ((PN_PNIOSTATUS_T)(0x00000004UL))
#define PNIO_E_DECODE_IOX_SRINFO_SR_PROP                     ((PN_PNIOSTATUS_T)(0x00000005UL))

/** PNIOStatus PrmBegin - Submodule list Block */
  /* IOX code */
#define PNIO_E_CODE_IOX_PRMBEGIN_BLOCK_TYPE                   ((PN_PNIOSTATUS_T)(0x00000000UL))
#define PNIO_E_CODE_IOX_PRMBEGIN_BLOCK_LEN                    ((PN_PNIOSTATUS_T)(0x00000001UL))
#define PNIO_E_CODE_IOX_PRMBEGIN_BLOCK_MAJOR                  ((PN_PNIOSTATUS_T)(0x00000002UL))
#define PNIO_E_CODE_IOX_PRMBEGIN_BLOCK_MINOR                  ((PN_PNIOSTATUS_T)(0x00000003UL))
#define PNIO_E_CODE_IOX_PRMBEGIN_RESERVED_1                   ((PN_PNIOSTATUS_T)(0x00000004UL))
#define PNIO_E_CODE_IOX_PRMBEGIN_ARUUID                       ((PN_PNIOSTATUS_T)(0x00000005UL))
#define PNIO_E_CODE_IOX_PRMBEGIN_SESSION_KEY                  ((PN_PNIOSTATUS_T)(0x00000006UL))
#define PNIO_E_CODE_IOX_PRMBEGIN_RESERVED_2                   ((PN_PNIOSTATUS_T)(0x00000007UL))
#define PNIO_E_CODE_IOX_PRMBEGIN_CTRL_COMMAND                 ((PN_PNIOSTATUS_T)(0x00000008UL))
#define PNIO_E_CODE_IOX_PRMBEGIN_RESERVED_3                   ((PN_PNIOSTATUS_T)(0x00000009UL))

/** PNIOStatus PrmBegin - Submodule list Block */
  /* IOX code */
#define PNIO_E_CODE_IOX_SUBMODULELIST_BLOCK_TYPE             ((PN_PNIOSTATUS_T)(0x00000000UL))
#define PNIO_E_CODE_IOX_SUBMODULELIST_BLOCK_LEN              ((PN_PNIOSTATUS_T)(0x00000001UL))
#define PNIO_E_CODE_IOX_SUBMODULELIST_BLOCK_MAJOR            ((PN_PNIOSTATUS_T)(0x00000002UL))
#define PNIO_E_CODE_IOX_SUBMODULELIST_BLOCK_MINOR            ((PN_PNIOSTATUS_T)(0x00000003UL))
#define PNIO_E_CODE_IOX_SUBMODULELIST_NUM_ENTRIES            ((PN_PNIOSTATUS_T)(0x00000004UL))
#define PNIO_E_CODE_IOX_SUBMODULELIST_API                    ((PN_PNIOSTATUS_T)(0x00000005UL))
#define PNIO_E_CODE_IOX_SUBMODULELIST_SLOT                   ((PN_PNIOSTATUS_T)(0x00000006UL))
#define PNIO_E_CODE_IOX_SUBMODULELIST_SUBSLOT                ((PN_PNIOSTATUS_T)(0x00000007UL))


static inline PN_PNIOSTATUS_T Pn_Status_MaskErrorCode(PN_PNIOSTATUS_T ulStatus)
{
  return ulStatus & 0xFF000000;
}

static inline PN_PNIOSTATUS_T Pn_Status_MaskErrorDecode(PN_PNIOSTATUS_T ulStatus)
{
  return ulStatus & 0x00FF0000;
}

static inline PN_PNIOSTATUS_T Pn_Status_MaskErrorCode1(PN_PNIOSTATUS_T ulStatus)
{
  return ulStatus & 0x0000FF00;
}

static inline PN_PNIOSTATUS_T Pn_Status_MaskErrorCode2(PN_PNIOSTATUS_T ulStatus)
{
  return ulStatus & 0x000000FF;
}

/** @} */


/*************************************************************************************/
#endif /* #ifndef __PNIO_STATE_H */
