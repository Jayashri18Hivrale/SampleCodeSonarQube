/**************************************************************************************

   Copyright (c) Hilscher GmbH. All Rights Reserved.

 **************************************************************************************

   Filename:
    $Workfile: OS_Includes.h $
   Last Modification:
    $Author: Sebastiand $
    $Modtime: 20.01.09 13:32 $
    $Revision: 792 $

   Targets:
     Win32/ANSI   : yes

   Description:
    Header file for OS specific functions (-> OS_KMDF.c)

   Changes:

     Version   Date        Author   Description
     ----------------------------------------------------------------------------------
     2        05.02.2013   SD       add file header
     1        -/-          MT       initial version

**************************************************************************************/

#ifndef __OS_INCLUDES__H
#define __OS_INCLUDES__H

#include "Windows.h"
#include <stdio.h>
#include <stdlib.h>

#undef SLIST_ENTRY
#pragma warning(disable : 4127 )  /* Disable conditional expression is constant (used in TAILQ) */
#pragma warning(disable : 4152 )  /* Disable nonstandard extension (thrown by PtrToLong)        */

#define PTR2LONG PtrToLong
#define LONG2PTR LongToPtr

#ifndef MIN
  #define MIN(a,b)   ((a < b)? (a) : (b))
#endif /* MIN */

#endif /*  __OS_INCLUDES__H */
