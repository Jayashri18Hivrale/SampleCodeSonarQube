/**************************************************************************************
 
   Copyright (c) Hilscher GmbH. All Rights Reserved.
 
 **************************************************************************************
 
   Filename:
    $Workfile: OS_Dependent.h $
   Last Modification:
    $Author: stephans $
    $Modtime: 4.05.07 15:17 $
    $Revision: 1136 $
   
   Targets:
     Win32/ANSI   : yes
     Win32/Unicode: yes (define _UNICODE)
     WinCE        : yes
 
   Description:
    Definitions of data structures for marshaller data
       
   Changes:
 
     Version   Date        Author   Description
     ----------------------------------------------------------------------------------
     1         07.07.2006  MT       intitial version
 
**************************************************************************************/

#ifndef __MARSHALLERFRAME__H
#define __MARSHALLERFRAME__H

#define MARSHALLER_OBJECT_TYPE_CLASSFACTORY    0x00
#define MARSHALLER_OBJECT_TYPE_DRIVER          0x01
#define MARSHALLER_OBJECT_TYPE_SYSDEVICE       0x02
#define MARSHALLER_OBJECT_TYPE_CHANNEL         0x03

#define MARSHALLER_SUBIDX_SYSTEMCHANNEL        0xFF

#define MARSHALLER_CF_METHODID_SERVERVERSION        0x00000000 /*!< Class Factory: MethodID for querying ServerVersion                   */
#define MARSHALLER_CF_METHODID_CREATEINSTANCE       0x00000001 /*!< Class Factory: MethodID for Creating/Getting a handle to a new object_Type (DriverObject) */

#define MARSHALLER_DRV_METHODID_OPEN                0x00000001 /*!< Driver Object: xDriverOpen                */
#define MARSHALLER_DRV_METHODID_CLOSE               0x00000002 /*!< Driver Object: xDriverClose               */
#define MARSHALLER_DRV_METHODID_GETINFO             0x00000003 /*!< Driver Object: xDriverGetInformation      */
#define MARSHALLER_DRV_METHODID_ERRORDESCR          0x00000004 /*!< Driver Object: xDriverGetErrorDescription */
#define MARSHALLER_DRV_METHODID_ENUMBOARDS          0x00000005 /*!< Driver Object: xDriverEnumBoard           */
#define MARSHALLER_DRV_METHODID_ENUMCHANNELS        0x00000006 /*!< Driver Object: xDriverEnumChannels        */
#define MARSHALLER_DRV_METHODID_OPENCHANNEL         0x00000008 /*!< Driver Object: xDriverOpenChannel         */
#define MARSHALLER_DRV_METHODID_OPENSYSDEV          0x00000009 /*!< Driver Object: xDriverSysDeviceOpen       */

#define MARSHALLER_SYSDEV_METHODID_CLOSE            0x00000001 /*!< Sysdevice Object: xSysdeviceClose         */
#define MARSHALLER_SYSDEV_METHODID_INFO             0x00000002 /*!< Sysdevice Object: xSysdeviceInfo          */
#define MARSHALLER_SYSDEV_METHODID_RESET            0x00000003 /*!< Sysdevice Object: xSysdeviceReset         */
#define MARSHALLER_SYSDEV_METHODID_GETMBXSTATE      0x00000004 /*!< Sysdevice Object: xSysdeviceGetMBXState   */
#define MARSHALLER_SYSDEV_METHODID_PUTPACKET        0x00000005 /*!< Sysdevice Object: xSysdevicePutPacket     */
#define MARSHALLER_SYSDEV_METHODID_GETPACKET        0x00000006 /*!< Sysdevice Object: xSysdeviceGetPacket     */
#define MARSHALLER_SYSDEV_METHODID_DOWNLOAD         0x00000007 /*!< Sysdevice Object: xSysdeviceDownload      */
#define MARSHALLER_SYSDEV_METHODID_FINDFIRSTFILE    0x00000008 /*!< Sysdevice Object: xSysdeviceFindFirstFile */
#define MARSHALLER_SYSDEV_METHODID_FINDNEXTFILE     0x00000009 /*!< Sysdevice Object: xSysdeviceFindNextFile  */
#define MARSHALLER_SYSDEV_METHODID_UPLOAD           0x00000010 /*!< Sysdevice Object: xSysdeviceUpload        */

#define MARSHALLER_CHANNEL_METHODID_CLOSE           0x00000001 /*!< Channel Object: xChannelClose             */
#define MARSHALLER_CHANNEL_METHODID_DOWNLOAD        0x00000002 /*!< Channel Object: xChannelDownload          */
#define MARSHALLER_CHANNEL_METHODID_GETMBXSTATE     0x00000003 /*!< Channel Object: xChannelGetMBXState       */
#define MARSHALLER_CHANNEL_METHODID_PUTPACKET       0x00000004 /*!< Channel Object: xChannelPutPacket         */
#define MARSHALLER_CHANNEL_METHODID_GETPACKET       0x00000005 /*!< Channel Object: xChannelGetPacket         */
#define MARSHALLER_CHANNEL_METHODID_GETSENDPACKET   0x00000006 /*!< Channel Object: xChannelGetSendPacket     */
#define MARSHALLER_CHANNEL_METHODID_CONFIGLOCK      0x00000007 /*!< Channel Object: xChannelConfigLock        */
#define MARSHALLER_CHANNEL_METHODID_RESET           0x00000008 /*!< Channel Object: xChannelReset             */
#define MARSHALLER_CHANNEL_METHODID_INFO            0x00000009 /*!< Channel Object: xChannelInfo              */
#define MARSHALLER_CHANNEL_METHODID_WATCHDOG        0x00000010 /*!< Channel Object: xChannelWatchdog          */
#define MARSHALLER_CHANNEL_METHODID_HOSTSTATE       0x00000011 /*!< Channel Object: xChannelHostState         */
#define MARSHALLER_CHANNEL_METHODID_IOREAD          0x00000012 /*!< Channel Object: xChannelIORead            */
#define MARSHALLER_CHANNEL_METHODID_IOWRITE         0x00000013 /*!< Channel Object: xChannelIOWrite           */
#define MARSHALLER_CHANNEL_METHODID_IOREADSENDDATA  0x00000014 /*!< Channel Object: xChannelIOReadSendData    */
#define MARSHALLER_CHANNEL_METHODID_BUSSTATE        0x00000015 /*!< Channel Object: xChannelBusState          */
#define MARSHALLER_CHANNEL_METHODID_CONTROLBLOCK    0x00000016 /*!< Channel Object: xChannelControlBlock      */
#define MARSHALLER_CHANNEL_METHODID_STATUSBLOCK     0x00000017 /*!< Channel Object: xChannelStatusBlock       */
#define MARSHALLER_CHANNEL_METHODID_EXTSTATUSBLOCK  0x00000018 /*!< Channel Object: xChannelExtendedStatusBlock */
#define MARSHALLER_CHANNEL_METHODID_USERBLOCK       0x00000019 /*!< Channel Object: xChannelUserBlock          */
#define MARSHALLER_CHANNEL_METHODID_FINDFIRSTFILE   0x00000020 /*!< Channel Object: xChannelFindFirstFile     */
#define MARSHALLER_CHANNEL_METHODID_FINDNEXTFILE    0x00000021 /*!< Channel Object: xChannelFindNextFile      */
#define MARSHALLER_CHANNEL_METHODID_UPLOAD          0x00000022 /*!< Channel Object: xChannelUpload            */
#define MARSHALLER_CHANNEL_METHODID_IOINFO          0x00000023 /*!< Channel Object: xChannelIOInfo            */

/*****************************************************************************/
/*! Marshaller handle definition (not valid for old device implementation)   */
/*****************************************************************************/
typedef union MARSHALLER_HANDLE_Utag
{
  struct MARSHALLER_HANDLE_BF_Ttag
  {
    unsigned int bObjectType    : 8; /*!< Type of the Object see MARSHALLER_OBJECT_TYPE_XXX defines       */
    unsigned int bObjectIdx     : 8; /*!< Idx of the Object, i.e. Board number                            */
    unsigned int bObjectSubIdx  : 8; /*!< SubIdx of the Object (0..6 for Channels, 0 for Sysdevice/Driver */
    unsigned int bReserved      : 7; /*!< SubIdx of the Object (0..6 for Channels, 0 for Sysdevice/Driver */
    unsigned int fValid         : 1; /*!< 1 to show this is a valid handle                                */
    
  } tBfHandle;
  unsigned long ulHandle;   /*!< Returned handle from DriverOpen/ChannelOpen/SysDeviceOpen functions */
  
} MARSHALLER_HANDLE_U;

typedef struct MARSHALLER_DATA_FRAME_HEADER_Ttag
{
  MARSHALLER_HANDLE_U  uHandle;
  unsigned long        ulMethodID; /*!< ID of the method to execute */

  union MARSHALLER_SEQUENCE_Utag
  {
    struct MARSHALLER_SEQUENCE_BF_Ttag
    {
      unsigned int   fRequest       :1;   /*!< 1=Request, 0 = Answer                                  */
      unsigned int   fSeqNrSupport  :1;   /*!< Sequence number supported                              */
      unsigned int   uiReserved     :14;  /*!< Reserved for future use. Set to 0                      */
      unsigned int   usSequenceNr   :16;  /*!< Sequence number, used to associate answer with request */
    } tBfSequence;
    unsigned long ulSequence;   /*!< Sequence number, to associate request/answer                  */
  } uSequence;
  unsigned long ulError;        /*!< Error, used on Answer, to indicate function return value      */
  unsigned long ulDataSize;     /*!< Length of following data                                      */
  
} MARSHALLER_DATA_FRAME_HEADER_T, *PMARSHALLER_DATA_FRAME_HEADER_T;

/*****************************************************************************
   ____   _                         _____                  _                          
  / ___| | |   __ _   ___   ___    |  ___|   __ _    ___  | |_    ___    _ __   _   _ 
 | |     | |  / _` | / __| / __|   | |_     / _` |  / __| | __|  / _ \  | '__| | | | |
 | |___  | | | (_| | \__ \ \__ \   |  _|   | (_| | | (__  | |_  | (_) | | |    | |_| |
  \____| |_|  \__,_| |___/ |___/   |_|      \__,_|  \___|  \__|  \___/  |_|     \__, |
                                                                                |___/ 
*****************************************************************************/


/*****************************************************************************/
/*! Class Factory - Create instance request                                  */
/*****************************************************************************/
typedef struct CF_CREATEINSTANCE_REQ_DATA_Ttag
{
  unsigned long ulObjectType;  /*!< Object type to generate (see MARSHALLER_OBJECT_TYPE_XXX defines */

} CF_CREATEINSTANCE_REQ_DATA_T, *PCF_CREATEINSTANCE_REQ_DATA_T;

typedef struct CF_CREATEINSTANCE_REQ_Ttag
{
  MARSHALLER_DATA_FRAME_HEADER_T tHeader;
  CF_CREATEINSTANCE_REQ_DATA_T   tData;

} CF_CREATEINSTANCE_REQ_T, *PCF_CREATEINSTANCE_REQ_T;

/*****************************************************************************/
/*! Class Factory - Create instance confirmation                             */
/*****************************************************************************/
typedef struct CF_CREATEINSTANCE_CNF_DATA_Ttag
{
  MARSHALLER_HANDLE_U uHandle;  /*!< Handle returned for future use. This handle must be used to communicate with the created object */
  
} CF_CREATEINSTANCE_CNF_DATA_T, *PCF_CREATEINSTANCE_CNF_DATA_T;

typedef struct CF_CREATEINSTANCE_CNF_Ttag
{
  MARSHALLER_DATA_FRAME_HEADER_T  tHeader;
  CF_CREATEINSTANCE_CNF_DATA_T    tData;

} CF_CREATEINSTANCE_CNF_T, *PCF_CREATEINSTANCE_CNF_T;

/*****************************************************************************/
/*! Class Factory - Query cifX marshaller version confirmation               */
/*****************************************************************************/
typedef struct CF_SERVERVERSION_CNF_DATA_Ttag
{
  unsigned long ulVersion;  /*!< Returned version */
 
} CF_SERVERVERSION_CNF_DATA_T, *PCF_SERVERVERSION_CNF_DATA_T;

typedef struct CF_SERVERVERSION_CNF_Ttag
{
  MARSHALLER_DATA_FRAME_HEADER_T  tHeader;
  CF_SERVERVERSION_CNF_DATA_T     tData;
  
} CF_SERVERVERSION_CNF_T, *PCF_SERVERVERSION_CNF_T;


/*****************************************************************************
  ____           _                            ___    _         _                 _   
 |  _ \   _ __  (_) __   __   ___   _ __     / _ \  | |__     (_)   ___    ___  | |_ 
 | | | | | '__| | | \ \ / /  / _ \ | '__|   | | | | | '_ \    | |  / _ \  / __| | __|
 | |_| | | |    | |  \ V /  |  __/ | |      | |_| | | |_) |   | | |  __/ | (__  | |_ 
 |____/  |_|    |_|   \_/    \___| |_|       \___/  |_.__/   _/ |  \___|  \___|  \__|
                                                            |__/                     
*****************************************************************************/

/*****************************************************************************/
/*! Driver - xDriverGetInformation request                                   */
/*****************************************************************************/
typedef struct DRV_GETINFORMATION_REQ_DATA_Ttag
{
  unsigned long ulSize; /*!< Number of byte to return in confirmation */
  
} DRV_GETINFORMATION_REQ_DATA_T, *PDRV_GETINFORMATION_REQ_DATA_T;

typedef struct DRV_GETINFORMATION_REQ_Ttag
{
  MARSHALLER_DATA_FRAME_HEADER_T  tHeader;
  DRV_GETINFORMATION_REQ_DATA_T   tData;
  
} DRV_GETINFORMATION_REQ_T, *PDRV_GETINFORMATION_REQ_T;

/*****************************************************************************/
/*! Driver - xDriverEnumBoards request                                       */
/*****************************************************************************/
typedef struct DRV_ENUMBOARD_REQ_DATA_Ttag
{
  unsigned long ulBoard;  /*!< Board number */
  unsigned long ulSize;   /*!< Size of the returned board information (in confirmation) */
  
} DRV_ENUMBOARD_REQ_DATA_T, *PDRV_ENUMBOARD_REQ_DATA_T;

typedef struct DRV_ENUMBOARD_REQ_Ttag
{
  MARSHALLER_DATA_FRAME_HEADER_T  tHeader;
  DRV_ENUMBOARD_REQ_DATA_T        tData;
  
} DRV_ENUMBOARD_REQ_T, *PDRV_ENUMBOARD_REQ_T;


/*****************************************************************************/
/*! Driver - xDriverEnumChannels request                                     */
/*****************************************************************************/
typedef struct DRV_ENUMCHANNELS_REQ_DATA_Ttag
{
  unsigned long ulBoard;    /*!< Board number */
  unsigned long ulChannel;  /*!< Channel number */
  unsigned long ulSize;     /*!< Size of returned channel information */
  
} DRV_ENUMCHANNELS_REQ_DATA_T, *PDRV_ENUMCHANNELS_REQ_DATA_T;

typedef struct DRV_ENUMCHANNELS_REQ_Ttag
{
  MARSHALLER_DATA_FRAME_HEADER_T  tHeader;
  DRV_ENUMCHANNELS_REQ_DATA_T     tData;
  
} DRV_ENUMCHANNELS_REQ_T, *PDRV_ENUMCHANNELS_REQ_T;


/*****************************************************************************
  ____                      _                  _               
 / ___|   _   _   ___    __| |   ___  __   __ (_)   ___    ___ 
 \___ \  | | | | / __|  / _` |  / _ \ \ \ / / | |  / __|  / _ \
  ___) | | |_| | \__ \ | (_| | |  __/  \ V /  | | | (__  |  __/
 |____/   \__, | |___/  \__,_|  \___|   \_/   |_|  \___|  \___|
          |___/                                                
   ___    _         _                 _   
  / _ \  | |__     (_)   ___    ___  | |_ 
 | | | | | '_ \    | |  / _ \  / __| | __|
 | |_| | | |_) |   | | |  __/ | (__  | |_ 
  \___/  |_.__/   _/ |  \___|  \___|  \__|
                 |__/                     
*****************************************************************************/


/*****************************************************************************/
/*! Sysdevice - xSysdeviceFindFirstFile request data                         */
/*****************************************************************************/
typedef struct SYSDEV_FIND_FIRSTFILE_REQ_DATA_Ttag
{
  unsigned long ulChannel;                            /*!< Number of Channel                              */
  unsigned long hList;                                /*!< Handle from Enumeration function, do not touch */
  char          szFilename[CIFx_MAX_INFO_NAME_LENTH]; /*!< File name.                                     */
  unsigned char bFiletype;                            /*!< File type.                                     */
  unsigned long ulFilesize;                           /*!< File size.                                     */
  unsigned long ulRecvPktCallback;                    /*!< Recv-Pkt-Callback value                        */
  unsigned long ulUser;                               /*!< User value                                     */
  
} SYSDEV_FIND_FIRSTFILE_REQ_DATA_T, *PSYSDEV_FIND_FIRSTFILE_REQ_DATA_T;

/*****************************************************************************/
/*! Sysdevice - xSysdeviceFindFirstFile request                              */
/*****************************************************************************/
typedef struct SYSDEV_FIND_FIRSTFILE_REQ_Ttag
{
  MARSHALLER_DATA_FRAME_HEADER_T     tHeader;
  SYSDEV_FIND_FIRSTFILE_REQ_DATA_T   tData;

} SYSDEV_FIND_FIRSTFILE_REQ_T, *PSYSDEV_FIND_FIRSTFILE_REQ_T;

/*****************************************************************************/
/*! Sysdevice - xSysdeviceFindFirstFile confirmation data                    */
/*****************************************************************************/
typedef struct SYSDEV_FIND_FIRSTFILE_CNF_DATA_Ttag
{
  unsigned long hList;                                /*!< Handle from Enumeration function, do not touch */
  char          szFilename[CIFx_MAX_INFO_NAME_LENTH]; /*!< File name.                                     */
  unsigned char bFiletype;                            /*!< File type.                                     */
  unsigned long ulFilesize;                           /*!< File size.                                     */
  
} SYSDEV_FIND_FIRSTFILE_CNF_DATA_T, *PSYSDEV_FIND_FIRSTFILE_CNF_DATA_T;

/*****************************************************************************/
/*! Sysdevice - xSysdeviceFindFirstFile confirmation                         */
/*****************************************************************************/
typedef struct SYSDEV_FIND_FIRSTFILE_CNF_Ttag
{
  MARSHALLER_DATA_FRAME_HEADER_T    tHeader;
  SYSDEV_FIND_FIRSTFILE_CNF_DATA_T  tData;

} SYSDEV_FIND_FIRSTFILE_CNF_T, *PSYSDEV_FIND_FIRSTFILE_CNF_T;


/*****************************************************************************/
/*! Sysdevice - xSysdeviceFindNextFile request data                          */
/*****************************************************************************/
typedef struct SYSDEV_FIND_NEXTFILE_REQ_DATA_Ttag
{
  unsigned long ulChannel;                            /*!< Number of Channel                              */
  unsigned long hList;                                /*!< Handle from Enumeration function, do not touch */
  char          szFilename[CIFx_MAX_INFO_NAME_LENTH]; /*!< File name.                                     */
  unsigned char bFiletype;                            /*!< File type.                                     */
  unsigned long ulFilesize;                           /*!< File size.                                     */
  unsigned long ulRecvPktCallback;                    /*!< Recv-Pkt-Callback value                        */
  unsigned long ulUser;                               /*!< User value                                     */
  
} SYSDEV_FIND_NEXTFILE_REQ_DATA_T, *PSYSDEV_FIND_NEXTFILE_REQ_DATA_T;

/*****************************************************************************/
/*! Sysdevice - xSysdeviceFindNextFile request                               */
/*****************************************************************************/
typedef struct SYSDEV_FIND_NEXTFILE_REQ_Ttag
{
  MARSHALLER_DATA_FRAME_HEADER_T    tHeader;
  SYSDEV_FIND_NEXTFILE_REQ_DATA_T   tData;

} SYSDEV_FIND_NEXTFILE_REQ_T, *PSYSDEV_FIND_NEXTFILE_REQ_T;

/*****************************************************************************/
/*! Sysdevice - xSysdeviceFindNextFile confirmation data                     */
/*****************************************************************************/
typedef struct SYSDEV_FIND_NEXTFILE_CNF_DATA_Ttag
{
  unsigned long hList;                                /*!< Handle from Enumeration function, do not touch */
  char          szFilename[CIFx_MAX_INFO_NAME_LENTH]; /*!< File name.                                     */
  unsigned char bFiletype;                            /*!< File type.                                     */
  unsigned long ulFilesize;                           /*!< File size.                                     */
  
} SYSDEV_FIND_NEXTFILE_CNF_DATA_T, *PSYSDEV_FIND_NEXTFILE_CNF_DATA_T;

/*****************************************************************************/
/*! Sysdevice - xSysdeviceFindNextFile confirmation                          */
/*****************************************************************************/
typedef struct SYSDEV_FIND_NEXTFILE_CNF_Ttag
{
  MARSHALLER_DATA_FRAME_HEADER_T    tHeader;
  SYSDEV_FIND_NEXTFILE_CNF_DATA_T   tData;

} SYSDEV_FIND_NEXTFILE_CNF_T, *PSYSDEV_FIND_NEXTFILE_CNF_T;

/*****************************************************************************/
/*! Sysdevice - xSysdeviceInfo request                                       */
/*****************************************************************************/
typedef struct SYSDEV_INFO_REQ_DATA_Ttag
{
  unsigned long ulCmd;    /*!< Command */
  unsigned long ulSize;   /*!< Size of returned information */
  
} SYSDEV_INFO_REQ_DATA_T, *PSYSDEV_INFO_REQ_DATA_T;

typedef struct SYSDEV_INFO_REQ_Ttag
{
  MARSHALLER_DATA_FRAME_HEADER_T  tHeader;
  SYSDEV_INFO_REQ_DATA_T          tData;
  
} SYSDEV_INFO_REQ_T, *PSYSDEV_INFO_REQ_T;

/*****************************************************************************/
/*! Sysdevice - xSysdeviceReset request                                      */
/*****************************************************************************/
typedef struct SYSDEV_RESET_REQ_DATA_Ttag
{
  unsigned long ulTimeout;    /*!< Timeout in ms for reset */
  
} SYSDEV_RESET_REQ_DATA_T, *PSYSDEV_RESET_REQ_DATA_T;

typedef struct SYSDEV_RESET_REQ_Ttag
{
  MARSHALLER_DATA_FRAME_HEADER_T  tHeader;
  SYSDEV_RESET_REQ_DATA_T         tData;
  
} SYSDEV_RESET_REQ_T, *PSYSDEV_RESET_REQ_T;


/*****************************************************************************
   ____   _                                      _ 
  / ___| | |__     __ _   _ __    _ __     ___  | |
 | |     | '_ \   / _` | | '_ \  | '_ \   / _ \ | |
 | |___  | | | | | (_| | | | | | | | | | |  __/ | |
  \____| |_| |_|  \__,_| |_| |_| |_| |_|  \___| |_|
                                                   
   ___    _         _                 _   
  / _ \  | |__     (_)   ___    ___  | |_ 
 | | | | | '_ \    | |  / _ \  / __| | __|
 | |_| | | |_) |   | | |  __/ | (__  | |_ 
  \___/  |_.__/   _/ |  \___|  \___|  \__|
                 |__/                     
*****************************************************************************/

/*****************************************************************************/
/*! Channel - xChannelReset request                                          */
/*****************************************************************************/
typedef struct CHANNEL_RESET_REQ_DATA_Ttag
{
  unsigned long ulMode;    /*!< Reset mode */
  unsigned long ulTimeout; /*!< Timeout in ms for reset */
  
} CHANNEL_RESET_REQ_DATA_T, *PCHANNEL_RESET_REQ_DATA_T;

typedef struct CHANNEL_RESET_REQ_Ttag
{
  MARSHALLER_DATA_FRAME_HEADER_T  tHeader;
  CHANNEL_RESET_REQ_DATA_T        tData;
  
} CHANNEL_RESET_REQ_T, *PCHANNEL_RESET_REQ_T;

/*****************************************************************************/
/*! Channel - xChannelInfo request                                           */
/*****************************************************************************/
typedef struct CHANNEL_INFO_REQ_DATA_Ttag
{
  unsigned long ulSize;    /*!< Size of returned data */
  
} CHANNEL_INFO_REQ_DATA_T, *PCHANNEL_INFO_REQ_DATA_T;

typedef struct CHANNEL_INFO_REQ_Ttag
{
  MARSHALLER_DATA_FRAME_HEADER_T  tHeader;
  CHANNEL_INFO_REQ_DATA_T         tData;
  
} CHANNEL_INFO_REQ_T, *PCHANNEL_INFO_REQ_T;

/*****************************************************************************/
/*! Channel - xChannelWatchdog request                                       */
/*****************************************************************************/
typedef struct CHANNEL_WATCHDOG_REQ_DATA_Ttag
{
  unsigned long ulCmd;     /*!< Command       */
  unsigned long ulTrigger; /*!< Trigger value */
  
} CHANNEL_WATCHDOG_REQ_DATA_T, *PCHANNEL_WATCHDOG_REQ_DATA_T;

typedef struct CHANNEL_WATCHDOG_REQ_Ttag
{
  MARSHALLER_DATA_FRAME_HEADER_T  tHeader;
  CHANNEL_WATCHDOG_REQ_DATA_T     tData;
  
} CHANNEL_WATCHDOG_REQ_T, *PCHANNEL_WATCHDOG_REQ_T;

/*****************************************************************************/
/*! Channel - xChannelWatchdog confirmation                                  */
/*****************************************************************************/
typedef struct CHANNEL_WATCHDOG_CNF_DATA_Ttag
{
  unsigned long ulTrigger; /*!< Trigger value */
  
} CHANNEL_WATCHDOG_CNF_DATA_T, *PCHANNEL_WATCHDOG_CNF_DATA_T;

typedef struct CHANNEL_WATCHDOG_CNF_Ttag
{
  MARSHALLER_DATA_FRAME_HEADER_T  tHeader;
  CHANNEL_WATCHDOG_CNF_DATA_T     tData;
  
} CHANNEL_WATCHDOG_CNF_T, *PCHANNEL_WATCHDOG_CNF_T;

/*****************************************************************************/
/*! Channel - xChannelHoststate request                                      */
/*****************************************************************************/
typedef struct CHANNEL_HOSTSTATE_REQ_DATA_Ttag
{
  unsigned long ulCmd;     /*!< Command       */
  unsigned long ulState;   /*!< State         */
  unsigned long ulTimeout; /*!< Timeout in ms */
  
} CHANNEL_HOSTSTATE_REQ_DATA_T, *PCHANNEL_HOSTSTATE_REQ_DATA_T;

typedef struct CHANNEL_HOSTSTATE_REQ_Ttag
{
  MARSHALLER_DATA_FRAME_HEADER_T  tHeader;
  CHANNEL_HOSTSTATE_REQ_DATA_T    tData;
  
} CHANNEL_HOSTSTATE_REQ_T, *PCHANNEL_HOSTSTATE_REQ_T;

/*****************************************************************************/
/*! Channel - xChannelWatchdog confirmation                                  */
/*****************************************************************************/
typedef struct CHANNEL_HOSTSTATE_CNF_DATA_Ttag
{
  unsigned long ulState; /*!< Returned state */
  
} CHANNEL_HOSTSTATE_CNF_DATA_T, *PCHANNEL_HOSTSTATE_CNF_DATA_T;

typedef struct CHANNEL_HOSTSTATE_CNF_Ttag
{
  MARSHALLER_DATA_FRAME_HEADER_T  tHeader;
  CHANNEL_HOSTSTATE_CNF_DATA_T    tData;
  
} CHANNEL_HOSTSTATE_CNF_T, *PCHANNEL_HOSTSTATE_CNF_T;

/*****************************************************************************/
/*! Channel - xChannelConfigLock request data                                */
/*****************************************************************************/
typedef struct CHANNEL_CONFIG_LOCK_REQ_DATA_Ttag
{
  unsigned long ulCmd;                                    /*!< Command code               */
  unsigned long ulState;                                  /*!< State to set               */
  unsigned long ulTimeout;                                /*!< Timeout on ms              */
  
} CHANNEL_CONFIG_LOCK_REQ_DATA_T, *PCHANNEL_CONFIG_LOCK_REQ_DATA_T;

/*****************************************************************************/
/*! Channel - xChannelConfigLock request                                     */
/*****************************************************************************/
typedef struct CHANNEL_CONFIG_LOCK_REQ_Ttag
{
  MARSHALLER_DATA_FRAME_HEADER_T              tHeader;
  CHANNEL_CONFIG_LOCK_REQ_DATA_T              tData;

} CHANNEL_CONFIG_LOCK_REQ_T, *PCHANNEL_CONFIG_LOCK_REQ_T;
 
/*****************************************************************************/
/*! Channel - xChannelConfigLock confirmation data                           */
/*****************************************************************************/
typedef struct CHANNEL_CONFIG_LOCK_CNF_DATA_Ttag
{
  unsigned long ulState;                      /*!< Returned state            */
  
} CHANNEL_CONFIG_LOCK_CNF_DATA_T, *PCHANNEL_CONFIG_LOCK_CNF_DATA_T;

/*****************************************************************************/
/*! Channel - xChannelConfigLock confirmation                                */
/*****************************************************************************/
typedef struct CHANNEL_CONFIG_LOCK_CNF_Ttag
{
  MARSHALLER_DATA_FRAME_HEADER_T              tHeader;
  CHANNEL_CONFIG_LOCK_CNF_DATA_T              tData;

} CHANNEL_CONFIG_LOCK_CNF_T, *PCHANNEL_CONFIG_LOCK_CNF_T;

/*****************************************************************************/
/*! Channel - xChannelIOInfo request data                                    */
/*****************************************************************************/
typedef struct CHANNEL_IOINFO_REQ_DATA_Ttag
{
  unsigned long   ulCmd;                         /*!< Command                     */
  unsigned long   ulArea;                        /*!< ulAreaNumber                */
  unsigned long   ulDataLen;                     /*!< Length to read              */
} CHANNEL_IOINFO_REQ_DATA_T, *PCHANNEL_IOINFO_REQ_DATA_T;

/*****************************************************************************/
/*! Channel - xChannelIOInfo request                                         */
/*****************************************************************************/
typedef struct CHANNEL_IOINFO_REQ_Ttag
{
  MARSHALLER_DATA_FRAME_HEADER_T              tHeader;
  CHANNEL_IOINFO_REQ_DATA_T                   tData;

} CHANNEL_IOINFO_REQ_T, *PCHANNEL_IOINFO_REQ_T;

/*****************************************************************************/
/*! Channel - xChannelIORead request                                         */
/*****************************************************************************/
typedef struct CHANNEL_IOREAD_REQ_DATA_Ttag
{
  unsigned long ulArea;
  unsigned long ulOffset;
  unsigned long ulTimeout;
  unsigned long ulDataLen;
  
} CHANNEL_IOREAD_REQ_DATA_T, *PCHANNEL_IOREAD_REQ_DATA_T;

typedef struct CHANNEL_IOREAD_REQ_Ttag
{
  MARSHALLER_DATA_FRAME_HEADER_T  tHeader;
  CHANNEL_IOREAD_REQ_DATA_T       tData;
  
} CHANNEL_IOREAD_REQ_T, *PCHANNEL_IOREAD_REQ_T;

/*****************************************************************************/
/*! Channel - xChannelIOWrite request                                        */
/*****************************************************************************/
typedef struct CHANNEL_IOWRITE_REQ_DATA_Ttag
{
  unsigned long ulArea;
  unsigned long ulOffset;
  unsigned long ulTimeout;
  unsigned long ulDataLen;
  unsigned char abData[1];
  
} CHANNEL_IOWRITE_REQ_DATA_T, *PCHANNEL_IOWRITE_REQ_DATA_T;

typedef struct CHANNEL_IOWRITE_REQ_Ttag
{
  MARSHALLER_DATA_FRAME_HEADER_T  tHeader;
  CHANNEL_IOWRITE_REQ_DATA_T      tData;
  
} CHANNEL_IOWRITE_REQ_T, *PCHANNEL_IOWRITE_REQ_T;

/*****************************************************************************/
/*! Channel - xChannelGetMBXState request (also user for sysdevice)          */
/*****************************************************************************/
typedef struct CHANNEL_GETMBXSTATE_CNF_DATA_Ttag
{
  unsigned long ulRecvPktCnt; /*!< Number of messages pending in receive mailbox  */
  unsigned long ulSendPktCnt; /*!< Number of messages that the channel can handle */
  
} CHANNEL_GETMBXSTATE_CNF_DATA_T, *PCHANNEL_GETMBXSTATE_CNF_DATA_T;

typedef struct CHANNEL_GETMBXSTATE_CNF_Ttag
{
  MARSHALLER_DATA_FRAME_HEADER_T tHeader;
  CHANNEL_GETMBXSTATE_CNF_DATA_T tData;
  
} CHANNEL_GETMBXSTATE_CNF_T, *PCHANNEL_GETMBXSTATE_CNF_T;

/*****************************************************************************/
/*! Channel - xChannelIOReadSendData request                                 */
/*****************************************************************************/
typedef struct CHANNEL_IOREADSENDDATA_REQ_DATA_Ttag
{
  unsigned long ulArea;
  unsigned long ulOffset;
  unsigned long ulDataLen;
  
} CHANNEL_IOREADSENDDATA_REQ_DATA_T, *PCHANNEL_IOREADSENDDATA_REQ_DATA_T;

typedef struct CHANNEL_IOREADSENDDATA_REQ_Ttag
{
  MARSHALLER_DATA_FRAME_HEADER_T    tHeader;
  CHANNEL_IOREADSENDDATA_REQ_DATA_T tData;
  
} CHANNEL_IOREADSENDDATA_REQ_T, *PCHANNEL_IOREADSENDDATA_REQ_T;

/*****************************************************************************/
/*! Channel - xChannelBusState request                                       */
/*****************************************************************************/
typedef struct CHANNEL_BUSSTATE_REQ_DATA_Ttag
{
  unsigned long ulCmd;     /*!< Command       */
  unsigned long ulState;   /*!< State         */
  unsigned long ulTimeout; /*!< Timeout in ms */
  
} CHANNEL_BUSSTATE_REQ_DATA_T, *PCHANNEL_BUSSTATE_REQ_DATA_T;

typedef struct CHANNEL_BUSSTATE_REQ_Ttag
{
  MARSHALLER_DATA_FRAME_HEADER_T  tHeader;
  CHANNEL_BUSSTATE_REQ_DATA_T     tData;
  
} CHANNEL_BUSSTATE_REQ_T, *PCHANNEL_BUSSTATE_REQ_T;

/*****************************************************************************/
/*! Channel - xChannelWatchdog confirmation                                  */
/*****************************************************************************/
typedef struct CHANNEL_BUSSTATE_CNF_DATA_Ttag
{
  unsigned long ulState; /*!< Returned state */
  
} CHANNEL_BUSSTATE_CNF_DATA_T, *PCHANNEL_BUSSTATE_CNF_DATA_T;

typedef struct CHANNEL_BUSSTATE_CNF_Ttag
{
  MARSHALLER_DATA_FRAME_HEADER_T  tHeader;
  CHANNEL_BUSSTATE_CNF_DATA_T     tData;
  
} CHANNEL_BUSSTATE_CNF_T, *PCHANNEL_BUSSTATE_CNF_T;



/*****************************************************************************/
/*! Channel - xChannelGetPacket request (also user for sysdevice)            */
/*****************************************************************************/
typedef struct CHANNEL_GETPACKET_REQ_DATA_Ttag
{
  unsigned long ulSize;     /*!< Number of bytes to return */
  unsigned long ulTimeout;  /*!< Maximum time to wait for packet [ms] */
  
} CHANNEL_GETPACKET_REQ_DATA_T, *PCHANNEL_GETPACKET_REQ_DATA_T;

typedef struct CHANNEL_GETPACKET_REQ_Ttag
{
  MARSHALLER_DATA_FRAME_HEADER_T tHeader;
  CHANNEL_GETPACKET_REQ_DATA_T   tData;

} CHANNEL_GETPACKET_REQ_T, *PCHANNEL_GETPACKET_REQ_T;

/*****************************************************************************/
/*! Channel - xChannelGetSendPacket request data (also user for sysdevice)   */
/*****************************************************************************/
typedef struct CHANNEL_GET_SENDPACKET_REQ_DATA_Ttag
{
  unsigned long ulSize;    /*!< Size of buffer              */
} CHANNEL_GET_SENDPACKET_REQ_DATA_T, *PCHANNEL_GET_SENDPACKET_REQ_DATA_T;

/*****************************************************************************/
/*! Channel - xChannelGetSendPacket request (also user for sysdevice)        */
/*****************************************************************************/
typedef struct CHANNEL_GET_SENDPACKET_REQ_Ttag
{
  MARSHALLER_DATA_FRAME_HEADER_T              tHeader;
  CHANNEL_GET_SENDPACKET_REQ_DATA_T           tData;

} CHANNEL_GET_SENDPACKET_REQ_T, *PCHANNEL_GET_SENDPACKET_REQ_T;

/*****************************************************************************/
/*! Channel - xChannelConfigLock request                                     */
/*****************************************************************************/
typedef struct CHANNEL_CONFIGLOCK_REQ_DATA_Ttag
{
  unsigned long ulCmd;      /*!< Command */
  unsigned long ulState;    /*!< State to set */
  unsigned long ulTimeout;  /*!< Timeout to wait for state being reached */
  
} CHANNEL_CONFIGLOCK_REQ_DATA_T, *PCHANNEL_CONFIGLOCK_REQ_DATA_T;

typedef struct CHANNEL_CONFIGLOCK_REQ_Ttag
{
  MARSHALLER_DATA_FRAME_HEADER_T tHeader;
  CHANNEL_CONFIGLOCK_REQ_DATA_T  tData;

} CHANNEL_CONFIGLOCK_REQ_T, *PCHANNEL_CONFIGLOCK_REQ_T;

/*****************************************************************************/
/*! Channel - xChannelConfigLock confirmation                                */
/*****************************************************************************/
typedef struct CHANNEL_CONFIGLOCK_CNF_DATA_Ttag
{
  unsigned long ulState;  /*!< Returned state information */
  
} CHANNEL_CONFIGLOCK_CNF_DATA_T, *PCHANNEL_CONFIGLOCK_CNF_DATA_T;

typedef struct CHANNEL_CONFIGLOCK_CNF_Ttag
{
  MARSHALLER_DATA_FRAME_HEADER_T tHeader;
  CHANNEL_CONFIGLOCK_CNF_DATA_T  tData;

} CHANNEL_CONFIGLOCK_CNF_T, *PCHANNEL_CONFIGLOCK_CNF_T;


/*****************************************************************************/
/*! Channel - Control-, Common Status- and Extended Statusblock 
*             Read command (request)                                         */
/*****************************************************************************/
typedef struct CHANNEL_BLOCK_READ_REQ_DATA_Ttag
{
  unsigned long ulCmd;      /*!< Command */
  unsigned long ulOffset;   /*!< Offset inside block */
  unsigned long ulDatalen;  /*!< Returned data length */
  
} CHANNEL_BLOCK_READ_REQ_DATA_T, *PCHANNEL_BLOCK_READ_REQ_DATA_T;

typedef struct CHANNEL_BLOCK_READ_REQ_Ttag
{
  MARSHALLER_DATA_FRAME_HEADER_T tHeader;
  CHANNEL_BLOCK_READ_REQ_DATA_T  tData;

} CHANNEL_BLOCK_READ_REQ_T, *PCHANNEL_BLOCK_READ_REQ_T;

/*****************************************************************************/
/*! Channel - Control-, Common Status- and Extended Statusblock 
*             Read command (confirmation)                                    */
/*****************************************************************************/
typedef struct CHANNEL_BLOCK_READ_CNF_DATA_Ttag
{
  unsigned long abData[1];  /*!< Returned data */
} CHANNEL_BLOCK_READ_CNF_DATA_T, *PCHANNEL_BLOCK_READ_CNF_DATA_T;

typedef struct CHANNEL_BLOCK_READ_CNF_Ttag
{
  MARSHALLER_DATA_FRAME_HEADER_T tHeader;
  CHANNEL_BLOCK_READ_CNF_DATA_T  tData;

} CHANNEL_BLOCK_READ_CNF_T, *PCHANNEL_BLOCK_READ_CNF_T;

/*****************************************************************************/
/*! Channel - Control-, Common Status- and Extended Statusblock 
*             Write command (request)                                        */
/*****************************************************************************/
typedef struct CHANNEL_BLOCK_WRITE_REQ_DATA_Ttag
{
  unsigned long ulCmd;      /*!< Command */
  unsigned long ulOffset;   /*!< Offset inside block */
  unsigned long ulDatalen;  /*!< Datalength in abData [] */
  unsigned char abData[1];  /*!< Data to write */
  
} CHANNEL_BLOCK_WRITE_REQ_DATA_T, *PCHANNEL_BLOCK_WRITE_REQ_DATA_T;

typedef struct CHANNEL_BLOCK_WRITE_REQ_Ttag
{
  MARSHALLER_DATA_FRAME_HEADER_T tHeader;
  CHANNEL_BLOCK_WRITE_REQ_DATA_T tData;

} CHANNEL_BLOCK_WRITE_REQ_T, *PCHANNEL_BLOCK_WRITE_REQ_T;

/*****************************************************************************/
/*! xChannelFindFirstFile request data                                       */
/*****************************************************************************/
typedef struct CHANNEL_FIND_FIRSTFILE_REQ_DATA_Ttag
{
  unsigned long hList;                                /*!< Handle from Enumeration function, do not touch */
  char          szFilename[CIFx_MAX_INFO_NAME_LENTH]; /*!< File name.                                     */
  unsigned char bFiletype;                            /*!< File type.                                     */
  unsigned long ulFilesize;                           /*!< File size.                                     */
  unsigned long ulRecvPktCallback;                    /*!< Recv-Pkt-Callback value                        */
  unsigned long ulUser;                               /*!< User value                                     */
  
} CHANNEL_FIND_FIRSTFILE_REQ_DATA_T, *PCHANNEL_FIND_FIRSTFILE_REQ_DATA_T;

/*****************************************************************************/
/*! xChannelFindFirstFile request                                            */
/*****************************************************************************/
typedef struct CHANNEL_FIND_FIRSTFILE_REQ_Ttag
{
  MARSHALLER_DATA_FRAME_HEADER_T          tHeader;
  CHANNEL_FIND_FIRSTFILE_REQ_DATA_T       tData;

} CHANNEL_FIND_FIRSTFILE_REQ_T, *PCHANNEL_FIND_FIRSTFILE_REQ_T;

/*****************************************************************************/
/*! xChannelFindFirstFile confirmation data                                  */
/*****************************************************************************/
typedef struct CHANNEL_FIND_FIRSTFILE_CNF_DATA_Ttag
{
  unsigned long hList;                                /*!< Handle from Enumeration function, do not touch */
  char          szFilename[CIFx_MAX_INFO_NAME_LENTH]; /*!< File name.                                     */
  unsigned char bFiletype;                            /*!< File type.                                     */
  unsigned long ulFilesize;                           /*!< File size.                                     */
  
} CHANNEL_FIND_FIRSTFILE_CNF_DATA_T, *PCHANNEL_FIND_FIRSTFILE_CNF_DATA_T;

/*****************************************************************************/
/*! xChannelFindFirstFile confirmation                                       */
/*****************************************************************************/
typedef struct CHANNEL_FIND_FIRSTFILE_CNF_Ttag
{
  MARSHALLER_DATA_FRAME_HEADER_T          tHeader;
  CHANNEL_FIND_FIRSTFILE_CNF_DATA_T       tData;

} CHANNEL_FIND_FIRSTFILE_CNF_T, *PCHANNEL_FIND_FIRSTFILE_CNF_T;


/*****************************************************************************/
/*! xChannelFindNextFile request data                                        */
/*****************************************************************************/
typedef struct CHANNEL_FIND_NEXTFILE_REQ_DATA_Ttag
{
  unsigned long hList;                                /*!< Handle from Enumeration function, do not touch */
  char          szFilename[CIFx_MAX_INFO_NAME_LENTH]; /*!< File name.                                     */
  unsigned char bFiletype;                            /*!< File type.                                     */
  unsigned long ulFilesize;                           /*!< File size.                                     */
  unsigned long ulRecvPktCallback;                    /*!< Recv-Pkt-Callback value                        */
  unsigned long ulUser;                               /*!< User value                                     */
  
} CHANNEL_FIND_NEXTFILE_REQ_DATA_T, *PCHANNEL_FIND_NEXTFILE_REQ_DATA_T;

/*****************************************************************************/
/*! xChannelFindNextFile request                                             */
/*****************************************************************************/
typedef struct CHANNEL_FIND_NEXTFILE_REQ_Ttag
{
  MARSHALLER_DATA_FRAME_HEADER_T    tHeader;
  CHANNEL_FIND_NEXTFILE_REQ_DATA_T  tData;

} CHANNEL_FIND_NEXTFILE_REQ_T, *PCHANNEL_FIND_NEXTFILE_REQ_T;

/*****************************************************************************/
/*! xChannelFindNextFile confirmation data                                   */
/*****************************************************************************/
typedef struct CHANNEL_FIND_NEXTFILE_CNF_DATA_Ttag
{
  unsigned long hList;                                /*!< Handle from Enumeration function, do not touch */
  char          szFilename[CIFx_MAX_INFO_NAME_LENTH]; /*!< File name.                                     */
  unsigned char bFiletype;                            /*!< File type.                                     */
  unsigned long ulFilesize;                           /*!< File size.                                     */
  
} CHANNEL_FIND_NEXTFILE_CNF_DATA_T, *PCHANNEL_FIND_NEXTFILE_CNF_DATA_T;

/*****************************************************************************/
/*! xChannelFindNextFile confirmation                                        */
/*****************************************************************************/
typedef struct CHANNEL_FIND_NEXTFILE_CNF_Ttag
{
  MARSHALLER_DATA_FRAME_HEADER_T          tHeader;
  CHANNEL_FIND_NEXTFILE_CNF_DATA_T       tData;

} CHANNEL_FIND_NEXTFILE_CNF_T, *PCHANNEL_FIND_NEXTFILE_CNF_T;

#endif /* __MARSHALLERFRAME__H */
