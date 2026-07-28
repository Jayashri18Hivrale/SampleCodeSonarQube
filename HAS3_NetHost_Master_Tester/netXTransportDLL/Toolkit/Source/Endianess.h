/**************************************************************************************

   Copyright (c) Hilscher GmbH. All Rights Reserved.

 **************************************************************************************

   Filename:
    $Workfile: Endianess.h $
   Last Modification:
    $Author: MichaelT $
    $Modtime: 20.01.09 16:08 $
    $Revision: 792 $

   Targets:
     Win32/ANSI   : yes
     Win32/Unicode: yes (define _UNICODE)
     WinCE        : yes

   Description:
    Little/Big Endian conversion

   Changes:

     Version   Date        Author   Description
     ----------------------------------------------------------------------------------
     1        10.06.2013   SD       initial version

**************************************************************************************/

/*****************************************************************************/
/*! \file Endianess.h
*   Little/Big Endian conversion                                             */
/*****************************************************************************/

#ifndef __ENDIANESS__H
#define __ENDIANESS__H

/* Give the user the possibility to use own macros for
   endianess conversion */
#ifndef BIGENDIAN_MACROS_PROVIDED
  #ifndef NETX_TRANSPORT_BIGENDIAN
    /* Little endian, so we don't need a conversion */
    #define LE16_TO_HOST(a)   (a)
    #define LE32_TO_HOST(a)   (a)
    #define LE64_TO_HOST(a)   (a)
    #define HOST_TO_LE16(a)   (a)
    #define HOST_TO_LE32(a)   (a)
    #define HOST_TO_LE64(a)   (a)
  #else
    /* BIG endian, so we DO need a conversion */
    #define LE16_TO_HOST(a)   ( (((a) & 0x00FF) << 8) | \
                                (((a) & 0xFF00) >> 8) )

    #define LE32_TO_HOST(a)   ( (((a) & 0x000000FFUL) << 24) | \
                                (((a) & 0x0000FF00UL) << 8)  | \
                                (((a) & 0x00FF0000UL) >> 8)  | \
                                (((a) & 0xFF000000UL) >> 24) )

    #define LE64_TO_HOST(a)   ( (((a) & 0x00000000000000FFULL) << 56) | \
                                (((a) & 0x000000000000FF00ULL) << 40) | \
                                (((a) & 0x0000000000FF0000ULL) << 24) | \
                                (((a) & 0x00000000FF000000ULL) << 8)  | \
                                (((a) & 0x000000FF00000000ULL) >> 8)  | \
                                (((a) & 0x0000FF0000000000ULL) >> 24) | \
                                (((a) & 0x00FF000000000000ULL) >> 40) | \
                                (((a) & 0xFF00000000000000ULL) >> 56) )

    #define HOST_TO_LE16(a)   LE16_TO_HOST(a)
    #define HOST_TO_LE32(a)   LE32_TO_HOST(a)
    #define HOST_TO_LE64(a)   LE64_TO_HOST(a)
  #endif
#endif

#endif /* __ENDIANESS__H */
