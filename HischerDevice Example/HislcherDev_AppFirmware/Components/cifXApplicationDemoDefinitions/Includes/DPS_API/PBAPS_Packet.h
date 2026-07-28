/**************************************************************************************
Copyright (c) Hilscher Gesellschaft fuer Systemautomation mbH. All Rights Reserved.
***************************************************************************************
$Id: PBAPS_Packet.h 86330 2019-10-02 13:42:47Z TGrigorov $:

Description:
 This file contains definitions of the packet interface to Profibus APS task

Changes:
 Date          Description
 -----------------------------------------------------------------------------------
 2019-09-11    created
**************************************************************************************/

/*!\file PBAPS_Packet.h */
#ifndef __DPSGCI_PUBLIC_H_
#define __DPSGCI_PUBLIC_H_

#include "Hil_Packet.h"
#include "Hil_Compiler.h"

/*!\defgroup dpsApsPacketInterface DPS APS Packet Interface
 * DPS Packet Interface
 * =====================================
 * Stack specific commands for packet based interaction.
 * The packet interface allows the user to:
 * - configure the stack,
 * - to exchange data with the stack
 * - to be notified about stack events.
 @{*/

/*!\addtogroup dpsConfiguration
 * @{*/
#define PROFIBUS_APS_MAX_CFG_DATA_SIZE            (244)   /*!< Maximum number of bytes of configuration data. */

/*!\defgroup sysFlags Configuration System Flags
 *@{*/
/*!< System flag - Communication start control mask.<br>The start of the device can be performed
 * either application controlled or automatically.*/
#define PROFIBUS_APS_SYSTEM_FLAG_START_MSK            (0x00000001)

/*! System flag - Application controlled communication start.<br>The channel firmware is forced to
 * wait for the host application. Communication is allowed only <b>with</b> the BUS_ON flag. HIL_START_STOP_COMM_REQ
 * have to be send with data parameter HIL_START_STOP_COMM_PARAM_START.*/
#define PROFIBUS_APS_SYSTEM_FLAG_START_APPLICATION    (0x00000001)

/*! System flag - Autostart controlled communication start.<br>Network connections are opened automatically
 * without taking care of the state of the host application. Communication is allowed <b>without</b> the BUS_ON flag
 * by the host application.*/
#define PROFIBUS_APS_SYSTEM_FLAG_START_AUTO           (0x00000000)

/*! Using this parameter you can set the status of the input or the output
data. For each input and output date the following status information (in
byte) is stored in the dual-port memory.*/
#define PROFIBUS_APS_SYSTEM_FLAG_IOSTAT_ENABLE_MSK    (0x00000002)  /*!< System flag - IO Status enable mask.*/
#define PROFIBUS_APS_SYSTEM_FLAG_IOSTAT_ENABLED_MSK   (0x00000002)  /*!< System flag - Enable IO Status.*/
#define PROFIBUS_APS_SYSTEM_FLAG_IOSTAT_DISABLED_MSK  (0x00000000)  /*!< System flag - Disable IO Status.*/

#define PROFIBUS_APS_SYSTEM_FLAG_IOSTAT_LEN_MSK       (0x00000004)  /*!< System flag - IO Status length mask.*/
#define PROFIBUS_APS_SYSTEM_FLAG_IOSTAT_LEN_8BIT      (0x00000004)  /*!< System flag - IO Status 8 bits length.*/
#define PROFIBUS_APS_SYSTEM_FLAG_IOSTAT_LEN_32BIT     (0x00000000)  /*!< System flag - IO Status 32 bits length.*/

#define PROFIBUS_APS_SYSTEM_FLAG_ADR_SW_MSK           (0x00000010)  /*!< System flag - Address switch mask.*/
#define PROFIBUS_APS_SYSTEM_FLAG_ADR_SW_ENABLE        (0x00000010)  /*!< System flag - Enable address switch.*/
#define PROFIBUS_APS_SYSTEM_FLAG_ADR_SW_DISABLE       (0x00000000)  /*!< System flag - Disable address switch.*/
/*!@}*/

/*!\defgroup cfgFlags Configuration Flags
 * Configuration flags masks.
 *@{*/
#define PROFIBUS_APS_FLAG_DPV1_ENABLE                 (0x01)  /*!< Configuration flag enable DPV1.*/
#define PROFIBUS_APS_FLAG_SYNC_SUPPORT                (0x02)  /*!< Configuration flag enable Sync support.*/
#define PROFIBUS_APS_FLAG_FREEZE_SUPPORT              (0x04)  /*!< Configuration flag enable Freeze command.*/
#define PROFIBUS_APS_FLAG_FAILSAFE_SUPPORT            (0x08)  /*!< Configuration flag enable Failsafe support.*/
#define PROFIBUS_APS_FLAG_NO_ALARM_SUPPORT            (0x10)  /*!< Configuration flag disable SAP50 alarm.*/
#define PROFIBUS_APS_FLAG_IO_SWAP                     (0x20)  /*!< Configuration flag enable Motorola format.*/
#define PROFIBUS_APS_FLAG_AUTOCONFIG                  (0x40)  /*!< Configuration flag enable Host to check configuration parameter data.*/
#define PROFIBUS_APS_FLAG_NO_ADDR_CHANGE              (0x80)  /*!< Configuration flag disable the support of Set Slave Address command.*/
/*!@}*/

/*!\defgroup cfgExtFlags Extended Configuration Flags
 *@{*/
#define PROFIBUS_APS_EXTFLAG_ALARM_MODE_DEFAULT       (0x00)  /*!< Support all alarm modes.             */
#define PROFIBUS_APS_EXTFLAG_ALARM_MODE_NO_SEQ        (0x02)  /*!< Only support unsequenced alarm mode  */
#define PROFIBUS_APS_EXTFLAG_EXTPRM_SUPPORT           (0x04)  /*!< Enable Extended user parameter       */
#define PROFIBUS_APS_EXTFLAG_PRMCMD_SUPPORT           (0x08)  /*!< Enable Parameter command switched on */
#define PROFIBUS_APS_EXTFLAG_ISOM_SUPPORT             (0x10)  /*!< Enable Isochronous operation.        */
#define PROFIBUS_APS_EXTFLAG_STRUCTPRM_SUPPORT        (0x20)  /*!< Enable Structured parameters support.*/
#define PROFIBUS_APS_EXTFLAG_PUBLISHER_SUPPORT        (0x40)  /*!< Enable Publisher support.            */
/*!@}*/
/*!@}*/

/*! \name Packet commands for GCI component
 * Packets processed by the GCI component.
 * @{*/
#define PROFIBUS_APS_PACKET_COMMAND_START         (0x00003100)

  /*! Set Configuration Parameters request.
   * The packet below is used to provide configuration to the PROFIBUS stack.
   * It holds values for the system flags, watchdog time, network parameter and the
   * current IO data lengths, respectively its data modules (type and size).*/
#define PROFIBUS_APS_SET_CONFIGURATION_REQ        (0x00003102)
#define PROFIBUS_APS_SET_CONFIGURATION_CNF        (0x00003103)

  /*! Check User Parameter Data indication/response.
   * This service indicates the AP task that a check of parameterization is necessary.
   * The AP task has to check the received Parameter Data Set whether it is consistent and valid,
   * to use it finally in the positive case as parameterization.*/
#define PROFIBUS_APS_CHECK_USER_PRM_IND           (0x00003104)
#define PROFIBUS_APS_CHECK_USER_PRM_RES           (0x00003105)

  /*! Check Configuration Data indication/response.
   * This indication signals that new configuration data are available.
   * The configuration data are stored in variable abCfgData.*/
#define PROFIBUS_APS_CHECK_CFG_IND                (0x00003106)
#define PROFIBUS_APS_CHECK_CFG_RES                (0x00003107)

  /*! Request User Parameter Data request/confirmation.
   * This packet is used to request current user parameter data of the PROFIBUS DP slave.
   * The request packet does not have any parameters.*/
#define PROFIBUS_APS_GET_USER_PRM_REQ             (0x00003108)
#define PROFIBUS_APS_GET_USER_PRM_CNF             (0x00003109)

  /*! Request Configuration Data request/confirmation.
   * This packet is used to request current configuration information about the PROFIBUS DP slave.
   * The request packet does not have any parameters.*/
#define PROFIBUS_APS_GET_CFG_REQ                  (0x0000310A)
#define PROFIBUS_APS_GET_CFG_CNF                  (0x0000310B)
/*!@}*/
/*!@}*/


/* pragma pack */
#ifdef PRAGMA_PACK_ENABLE
#pragma PRAGMA_PACK_1(PROFIBUS_GCI_PUBLIC)
#endif
/*!\defgroup  dpsConfiguration Stack Configuration
 * \ingroup dpsApsPacketInterface
Stack Configuration
=====================================
@{*/
/*! \brief Set Configuration Parameters Request Data.*/
typedef __HIL_PACKED_PRE struct PROFIBUS_APS_SET_CONFIGURATION_REQ_Ttag
{
  /*!System Flags.<br>
   * BIT 0 - AUTOSTART/APPLICATION CONTROLLED<br>BIT 1:2 - I/O STATUS <b><i>(Not supported yet)</i></b><br>
   * BIT 3: - Reserved<br>BIT 4 -  ADDRESS_SWITCH<br>BIT 5: - BAUD_SWITCH<br>BIT 6:31 - Reserved<br>
   * \copydetails sysFlagTable
   * \sa sysFlags */
  uint32_t  ulSystemFlags;

  /*!Watchdog Time.<br>Watchdog time within which the device watchdog must be retriggered
   * from the application program while the application program monitoring
   * is activated. When the watchdog time value is equal to 0 respectively
   * the application program monitoring is deactivated.<br>Range of Value: 0, 20 - 65535 [ms]<br>
   * default = 1000 ms, 0 = Off*/
  uint32_t  ulWdgTime;

  /*! Own PROFIBUS Identification Number.<br>PROFIBUS-system specific identification number.<br>
   *Generally allowed values: <i> 0x0000 – 0xFFFF </i>, Default value: 0x0A12 (indicating netX).*/
  uint16_t  usIdentNumber;

  uint8_t   bBusAddr;   /*!<Bus Address.<br>Bus address (own network station address). Allowed values:0 – 126*/

  /*! Network Baud Rate.<br>Network transmission rate (Baudrate) of the PROFIBUS connection.
   * Available Baud Rate Values: 9.6 kBit/s to 12 MBit/s.<br>The applicable baud rates are coded with the values.
   * Default value: Auto detect.<br> \copydetails tableBaudrate
   * see also ...*/
  uint8_t   bBaudRate;

  /*! Configuration parameters.<br>Configuration flags:
   * - Bit0 - DPV1 Enable :Flag that indicates whether DPV1 is supported. If set, DPV1 functions are activated.
   *  Otherwise, DPV1 functions will not be available.
   *
   * - Bit1 - Sync supported : Flag that indicates if set to (1) that the slave stack shall
   * support the SYNC command and the SYNC mode is activated. Otherwise, if set to (0), the slave stack
   * will not support the SYNC command.
   *
   * - Bit2 - Freeze supported :Flag that indicates if set to (1) that the slave stack shall support
   * the FREEZE command and the FREEZE mode is activated. Otherwise, if set to (0), the slave stack
   * will not support the FREEZE command.
   *
   * - Bit3 - Fail safe supported : Flag that indicates whether ‘Fail safe’ operation is supported.
   * If set, FAILSAFE mode is activated. Otherwise, FAILSAFE mode will not be available.
   *
   * - Bit4 - Alarm SAP 50 deactivate : Flag that indicates if set to (1) that the alarm SAP 50 is deactivated.
   * If the flag is set to (0), the stack supports the alarm SAP 50.
   *
   * - Bit5 - I/O data swap : Flag that indicates if the I/O Data at the Dual Port Memory is shown
   * at Motorola or Intel format.
   *
   * - Bit6 - Auto configuration : Flag that indicates if set to (1) that the slave stack requests
   * the host application for check configuration and user parameter data. If set to (0), the stack
   * handles configuration and parameter data.
   *
   * - Bit7 - Address change not allowed : Flag that indicates if set to (1) that the slave stack
   * does not support the “Set Slave Address” command. If set to (0), changing the bus address via
   * the master is activated and the slave stack does support the “Set Slave Address” command.
   *
   *  Default value: 0. \sa cfgFlags*/
  uint8_t   bFlags;

  /*! Extended configuration parameter.
   * - Bit0 - Reserved.
   * - Bit1 - Flag indicates that alarm sequence mode is supported or not. If 0 (default) alarm sequence mode is supported.
   * if set 1, no alarms are supported to prevent master configuration with alarm sequence mode enabled.
   * - Bit2 - Flag indicates that extended user parameter is supported or not. If 0 is not support, otherwise supported.
   * - Bit3 - Flag indicates that parameter command is supported or not. If 0 is not support, otherwise supported.
   * - Bit4 - Flag indicates that Isochronous operation is supported or not. If 0 is not support, otherwise supported.
   * - Bit5 - Flag indicates that Structured parameters is supported or not. If 0 is not support, otherwise supported.
   * - Bit6 - Flag indicates that publisher support is supported or not. If 0 is not support, otherwise supported.
   * - Bit7 - Reserved. Should be 0.
   *
   *  Default value: 0. \sa cfgExtFlags*/
  uint8_t   bExtFlags;

  uint8_t   bRes;     /*!< Reserved */

  uint8_t   bCfgLen;  /*!< Number of configuration bytes n (for each module one or more bytes are needed)*/

  /*! Identifier Bytes <i>(can be specified in two alternative forms)</i>.
   * - General Identifier Byte <i>(coded according to the PROFIBUS standard, also see)</i>
   * - Special Identifier Byte Format <i>(SIF, also see)</i>.*/
  uint8_t   abCfgData[PROFIBUS_APS_MAX_CFG_DATA_SIZE];
}__HIL_PACKED_POST PROFIBUS_APS_SET_CONFIGURATION_REQ_T;

/*! Set Configuration Parameters request packet data length in bytes. */
#define PROFIBUS_APS_SET_CONFIGURATION_REQ_SIZE   (sizeof(PROFIBUS_APS_SET_CONFIGURATION_REQ_T) - PROFIBUS_APS_MAX_CFG_DATA_SIZE)

/*! \brief Set Configuration Parameters Request Packet.
 *  The packet is used to provide configuration to the PROFIBUS stack.
 *  \details
      It holds values for: the system flags, watchdog time, network parameter, the current IO data lengths,
      data modules (type and size). Configuration parameters will be stored internally, in case of any error no data will be stored at all.
      A channel init is required to activate the parameterized data. This packet does not perform any registration at the stack automatically.
      Registering must be performed with a separate packet. This request will be denied if the configuration lock flag is set. */
typedef struct PROFIBUS_APS_PACKET_SET_CONFIGURATION_REQ_Ttag
{
  HIL_PACKET_HEADER_T                   tHead; /*!< Standard Hilscher Header structure. */
  PROFIBUS_APS_SET_CONFIGURATION_REQ_T  tData; /*!< Configuration Data structure.*/
} PROFIBUS_APS_PACKET_SET_CONFIGURATION_REQ_T;

#define PROFIBUS_APS_SET_CONFIGURATION_CNF_SIZE   (0) /*!< Set Configuration Parameters confirmation packet data length in bytes. */

/* Indication Packet for acknowledged connectionless data transfer */
typedef struct PROFIBUS_APS_PACKET_SET_CONFIGURATION_CNF_Ttag
{
  HIL_PACKET_HEADER_T tHead;
} PROFIBUS_APS_PACKET_SET_CONFIGURATION_CNF_T;
/*!@}*/


/*!\addtogroup dpsUserPrmInd
 *@{ */
#define PROFIBUS_APS_MAX_USER_PRM_DATA_SIZE       (237) /*!< Maximum User Parameter data in bytes. */

/*! Indication Check User Parameter Data.*/
typedef __HIL_PACKED_PRE struct PROFIBUS_APS_CHECK_USER_PRM_IND_Ttag
{
  uint8_t abUserPrmData[PROFIBUS_APS_MAX_USER_PRM_DATA_SIZE]; /*!< User parameter data. */
}__HIL_PACKED_POST PROFIBUS_APS_CHECK_USER_PRM_IND_T;


/*! \brief Check User Parameter Data Indication Packet.
 * This service indicates the AP task that a check of parameterization is necessary.
 * \details
 *  The AP task has to check the received Parameter Data Set whether it is consistent and valid, to use it finally in the
 *  positive case as parameterization. This indication will only be sent if the following conditions are fulfilled:
 *  - The application has registered itself at the PROFIBUS-DP protocol stack formerly.
 *  - Auto-configuration mode has been enabled by setting the auto-config flag(::PROFIBUS_APS_SET_CONFIGURATION_REQ_T.bFlags bit 6)
 *  in the configuration parameters. */
typedef struct PROFIBUS_APS_PACKET_CHECK_USER_PRM_IND_Ttag
{
  HIL_PACKET_HEADER_T               tHead; /*!< Standard Hilscher Header structure. */
  PROFIBUS_APS_CHECK_USER_PRM_IND_T tData; /*!< Indication User parameter data. */
} PROFIBUS_APS_PACKET_CHECK_USER_PRM_IND_T;

/*! Response Check User Parameter Data. */
typedef __HIL_PACKED_PRE struct PROFIBUS_APS_CHECK_USER_PRM_RES_Ttag
{
  uint8_t fPrmOk; /*!< This variable indicates whether the user parameter data are ok or not.*/
}__HIL_PACKED_POST PROFIBUS_APS_CHECK_USER_PRM_RES_T;

/*! APS Check User parameter response packet data length in bytes.*/
#define PROFIBUS_APS_CHECK_USER_PRM_RES_SIZE      (sizeof(PROFIBUS_APS_CHECK_USER_PRM_RES_T))

/*! Response packet to Check User Parameter Indication. */
typedef struct PROFIBUS_APS_PACKET_CHECK_USER_PRM_RES_Ttag
{
  HIL_PACKET_HEADER_T               tHead; /*!< Standard Hilscher Header structure. */
  PROFIBUS_APS_CHECK_USER_PRM_RES_T tData; /*!< User parameter indication data. */
} PROFIBUS_APS_PACKET_CHECK_USER_PRM_RES_T;
/*!@}*/


/*! \addtogroup dpsChkCfgInd
 * @{
 */
/*! Check Configuration Indication Data. */
typedef __HIL_PACKED_PRE struct PROFIBUS_APS_CHECK_CFG_IND_Ttag
{
  uint8_t abCfgData[PROFIBUS_APS_MAX_CFG_DATA_SIZE]; /*!< Configuration data that needs to be checked. */
}__HIL_PACKED_POST PROFIBUS_APS_CHECK_CFG_IND_T;

/*! \brief Check Configuration Data Indication Packet.
 * This indication signals that new configuration data are available.
 * \details
 *  The configuration data are stored in variable abCfgData. The length is limited to 244 bytes by
 *  the PROFIBUS DP specification. This indication will only be sent if the following conditions are fulfilled:
 *  - The application has registered itself at the PROFIBUS-DP protocol stack formerly.
 *  - Auto-configuration mode has been enabled by setting the auto-config flag(::PROFIBUS_APS_SET_CONFIGURATION_REQ_T.bFlags bit 6)
 *  in the configuration parameters. */
typedef struct PROFIBUS_APS_PACKET_CHECK_CFG_IND_Ttag
{
  HIL_PACKET_HEADER_T           tHead; /*!< Standard Hilscher Header structure. */
  PROFIBUS_APS_CHECK_CFG_IND_T  tData; /*!< Check configuration indication data.*/
} PROFIBUS_APS_PACKET_CHECK_CFG_IND_T;

/*! Check Configuration Response Data.*/
typedef __HIL_PACKED_PRE struct PROFIBUS_APS_CHECK_CFG_RES_Ttag
{
  uint8_t fCfgOk; /*!< This variable indicates whether the configuration data are ok or not.*/
}__HIL_PACKED_POST PROFIBUS_APS_CHECK_CFG_RES_T;

/*! APS Check configuration response packet data length in bytes.*/
#define PROFIBUS_APS_CHECK_CFG_RES_SIZE           (sizeof(PROFIBUS_APS_CHECK_CFG_RES_T))

typedef struct PROFIBUS_APS_PACKET_CHECK_CFG_RES_Ttag
{
  HIL_PACKET_HEADER_T           tHead; /*!< Standard Hilscher Header structure. */
  PROFIBUS_APS_CHECK_CFG_RES_T  tData; /*!< Check configuration response data.*/
} PROFIBUS_APS_PACKET_CHECK_CFG_RES_T;
/*!@}*/


/*! \addtogroup dpsGetUserPrm
 * @{
 */
#define PROFIBUS_APS_GET_USER_PRM_REQ_SIZE        (0) /*!< Get User Parameter Data request packet data length in bytes.*/

/*! \brief Get User Parameter Data Request Packet.
 * This packet is used to request current user parameter data of the PROFIBUS DP slave.
 *  \details
 *   The request packet does not have any parameters.*/
typedef struct PROFIBUS_APS_PACKET_GET_USER_PRM_REQ_Ttag
{
  HIL_PACKET_HEADER_T tHead; /*!< Standard Hilscher Header structure. */
} PROFIBUS_APS_PACKET_GET_USER_PRM_REQ_T;

/*! Get User Parameter Confirmation Data. */
typedef __HIL_PACKED_PRE struct PROFIBUS_APS_GET_USER_PRM_CNF_Ttag
{
  uint8_t abUserPrmData[PROFIBUS_APS_MAX_USER_PRM_DATA_SIZE]; /*!< User parameter data.*/
}__HIL_PACKED_POST PROFIBUS_APS_GET_USER_PRM_CNF_T;

/*! \brief Get User Parameter Data Confirmation Packet.
 * Confirmation of Get User Parameter Data Request.
 *  \details
 *   The confirmation packet contains the requested user parameter data. Their length is limited to 237 bytes.*/
typedef struct PROFIBUS_APS_PACKET_GET_USER_PRM_CNF_Ttag
{
  HIL_PACKET_HEADER_T             tHead; /*!< Standard Hilscher Header structure. */
  PROFIBUS_APS_GET_USER_PRM_CNF_T tData; /*!< Get user parameter confirmation data.*/
} PROFIBUS_APS_PACKET_GET_USER_PRM_CNF_T;
/*!@}*/


/*! \addtogroup dpsGetCfgData
 * @{
 */
/* get configuration data */
#define PROFIBUS_APS_GET_CFG_REQ_SIZE (0) /*!<Get Configuration Data request packet data length in bytes.*/

/*! \brief Get Configuration Data Request Packet.
 *  \details
 *  This packet is used to request current configuration information about the PROFIBUS DP slave.
 *  The request packet does not have any parameters.*/
typedef struct PROFIBUS_APS_PACKET_GET_CFG_REQ_Ttag
{
  HIL_PACKET_HEADER_T tHead; /*!< Standard Hilscher Header structure. */
} PROFIBUS_APS_PACKET_GET_CFG_REQ_T;

/*! Get Configuration Confirmation Packet Data.*/
typedef __HIL_PACKED_PRE struct PROFIBUS_APS_GET_CFG_CNF_Ttag
{
  uint8_t abCfgData[PROFIBUS_APS_MAX_CFG_DATA_SIZE]; /*!< Configuration data.*/
}__HIL_PACKED_POST PROFIBUS_APS_GET_CFG_CNF_T;

/*! \brief Get Configuration Data Confirmation Packet.
 * \details
 * Confirmation to Get Configuration Data Request packet contains the requested configuration data
 * which are returned and sometimes denominated as ‘real configuration data’. Their length is limited
 * to 244 bytes by the PROFIBUS DP specification.*/
typedef struct PROFIBUS_APS_PACKET_GET_CFG_CNF_Ttag
{
  HIL_PACKET_HEADER_T         tHead; /*!< Standard Hilscher Header structure. */
  PROFIBUS_APS_GET_CFG_CNF_T  tData; /*!< Get Configuration confirmation data.*/
} PROFIBUS_APS_PACKET_GET_CFG_CNF_T;
/*!@}*/

/* pragma unpack */
#ifdef PRAGMA_PACK_ENABLE
#pragma PRAGMA_UNPACK_1(PROFIBUS_GCI_PUBLIC)
#endif

#endif /* __DPSGCI_PUBLIC_H_ */
