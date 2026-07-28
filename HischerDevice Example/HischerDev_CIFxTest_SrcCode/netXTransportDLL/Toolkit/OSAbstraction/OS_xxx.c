/**************************************************************************************

   Copyright (c) Hilscher GmbH. All Rights Reserved.

 **************************************************************************************

   Filename:
    $Id: OS_xxx.c 3194 2011-12-14 16:36:28Z Robert $
   Last Modification:
    $Author: Robert $
    $Date: 2011-12-14 17:36:28 +0100 (Mi, 14 Dez 2011) $
    $Revision: 3194 $

   Targets:
     xxx

   Description:
    xxx OS abstraction for netXTransport toolkit

   Changes:

     Version   Date        Author   Description
     ----------------------------------------------------------------------------------
      1        23.02.13    MT       initial version

**************************************************************************************/

#include "OS_Dependent.h"

/*****************************************************************************/
/*! Create Lock (Usually same as mutex, but does not support timed waiting)
*     \return Handle to created lock                                         */
/*****************************************************************************/
void* OS_CreateLock(void)
{
  return NULL;
}

/*****************************************************************************/
/*! Acquire a lock
*     \param pvLock Handle to lock                                           */
/*****************************************************************************/
void OS_EnterLock(void* pvLock)
{
  UNREFERENCED_PARAMETER( pvLock);
}

/*****************************************************************************/
/*! Release a lock
*     \param pvLock Handle to lock                                           */
/*****************************************************************************/
void OS_LeaveLock(void* pvLock)
{
  UNREFERENCED_PARAMETER( pvLock);
}

/*****************************************************************************/
/*! Delete a lock
*     \param pvLock Handle to lock                                           */
/*****************************************************************************/
void OS_DeleteLock(void* pvLock)
{
  UNREFERENCED_PARAMETER( pvLock);
}

/*****************************************************************************/
/*! Writes formatted string to szDest
*     \param szDest   Pointer to destiantion string
*     \param ulSize   Size of destination string
*     \param format   Pointer to format string
*     \return Pointer to szDest                                              */
/*****************************************************************************/
char* OS_Strvsprintf(char *szDest, uint32_t ulSize, const char *format, ...)
{
  UNREFERENCED_PARAMETER( szDest);
  UNREFERENCED_PARAMETER( ulSize);
  UNREFERENCED_PARAMETER( format);

  return NULL;
}

/*****************************************************************************/
/*! Compare strings
*     \param pszBuf1  String buffer 1
*     \param pszBuf2  String buffer 2
*     \return 0 if strings are equal                                         */
/*****************************************************************************/
int OS_Strcmp(const char* pszBuf1, const char* pszBuf2)
{
  UNREFERENCED_PARAMETER( pszBuf1);
  UNREFERENCED_PARAMETER( pszBuf2);

  return 0;
}

/*****************************************************************************/
/*! Compare strings (case sensitive)
*     \param pszBuf1  String buffer 1
*     \param pszBuf2  String buffer 2
*     \return 0 if strings are equal                                         */
/*****************************************************************************/
int OS_Strnicmp (const char* pszBuf1, const char* pszBuf2, uint32_t len)
{
  UNREFERENCED_PARAMETER( pszBuf1);
  UNREFERENCED_PARAMETER( pszBuf2);
  UNREFERENCED_PARAMETER( len);

  return 0;
}

/*****************************************************************************/
/*! Copy string to destination buffer
*     \param szText   Destination string
*     \param szSource Source string
*     \param ulLen    Maximum length to copy
*     \return Pointer to szDest                                              */
/*****************************************************************************/
char* OS_Strncpy(char* szDest, uint32_t ulDstSize, const char* szSource, uint32_t ulLen)
{
  UNREFERENCED_PARAMETER( szDest);
  UNREFERENCED_PARAMETER( ulDstSize);
  UNREFERENCED_PARAMETER( szSource);
  UNREFERENCED_PARAMETER( ulLen);

  return NULL;
}

/*****************************************************************************/
/*! Get length of string
*     \param szText  Text buffer
*     \return Length of given string                                         */
/*****************************************************************************/
int OS_Strlen(const char* szText)
{
  UNREFERENCED_PARAMETER( szDest);

  return 0;
}

/*****************************************************************************/
/*! Find string occurrence
*     \param szText  Text buffer
*     \return Length of given string                                         */
/*****************************************************************************/
uint32_t OS_Strcspn( char* szSearchStr, char* szFindStr)
{
  UNREFERENCED_PARAMETER( szSearchStr);
  UNREFERENCED_PARAMETER( szFindStr);

  return 0;
}

/*****************************************************************************/
/*! Concatenate strings
*     \param szDest  Pointer to destiantion string
*     \param szSrc   Pointer to source string
*     \return szDest                                                         */
/*****************************************************************************/
char* OS_Strcat(char* szDest, uint32_t ulDstSize, char* szSrc)
{
  UNREFERENCED_PARAMETER( szDest);
  UNREFERENCED_PARAMETER( ulDstSize);
  UNREFERENCED_PARAMETER( szSrc);

  return NULL;
}

/*****************************************************************************/
/*! Memory allocation
*     \param ulSize Size of block to allocate
*     \return NULL on failure                                                */
/*****************************************************************************/
void* OS_Memalloc(uint32_t ulSize)
{
  UNREFERENCED_PARAMETER( ulSize);

  return NULL;
}

/*****************************************************************************/
/*! Memset
*     \param pvMem   Memory to set
*     \param bFill   Fill byte
*     \param ulSize  Size of the fill block                                  */
/*****************************************************************************/
void OS_Memset(void* pvMem, uint8_t bFill, uint32_t ulSize)
{
  UNREFERENCED_PARAMETER( pvMem);
  UNREFERENCED_PARAMETER( bFill);
  UNREFERENCED_PARAMETER( ulSize);
}

/*****************************************************************************/
/*! Memcopy
*     \param pvDest  Destination pointer
*     \param pvSrc   Source pointer
*     \param ulSize  Size to copy                                            */
/*****************************************************************************/
void OS_Memcpy(void* pvBuf1, void* pvBuf2, uint32_t ulSize)
{
  UNREFERENCED_PARAMETER( pvBuf1);
  UNREFERENCED_PARAMETER( pvBuf2);
  UNREFERENCED_PARAMETER( ulSize);
}

/*****************************************************************************/
/*! Memory resize
*     \param pvMem      Block to resize
*     \param ulNewSize  New size of the block
*     \return NULL on error                                                  */
/*****************************************************************************/
void* OS_Memrealloc(void* pvBuffer, uint32_t ulSize)
{
  UNREFERENCED_PARAMETER( pvBuffer);
  UNREFERENCED_PARAMETER( ulSize);

  return NULL;
}

/*****************************************************************************/
/*! Memcompare wrapper
*     \param pvBuf1  First compare buffer
*     \param pvBuf2  Second compare buffer
*     \param ulSize  Size to compare
*     \return 0 if blocks are equal                                          */
/*****************************************************************************/
int OS_Memcmp(void* pvBuf1, void* pvBuf2, uint32_t ulSize)
{
  UNREFERENCED_PARAMETER( pvBuf1);
  UNREFERENCED_PARAMETER( pvBuf2);
  UNREFERENCED_PARAMETER( ulSize);

  return 0;
}

/*****************************************************************************/
/*! Memory de-allocation
*     \param pvMem  Block to free                                            */
/*****************************************************************************/
void OS_Memfree(void* pvMem)
{
  UNREFERENCED_PARAMETER( pvMem);
}

/*****************************************************************************/
/*! Create event
*     \return Handle to created event                                        */
/*****************************************************************************/
void* OS_CreateEvent(void)
{
  return NULL;
}

/*****************************************************************************/
/*! Signal event
*     \param pvEvent Handle to event                                         */
/*****************************************************************************/
void OS_SetEvent(void* pvEvent)
{
  UNREFERENCED_PARAMETER( pvEvent);
}

/*****************************************************************************/
/*! Reset event
*     \param pvEvent Handle to event                                         */
/*****************************************************************************/
void OS_ResetEvent(void* pvEvent)
{
  UNREFERENCED_PARAMETER( pvEvent);
}

/*****************************************************************************/
/*! Delete event
*     \param pvEvent Handle to event                                         */
/*****************************************************************************/
void OS_DeleteEvent(void* pvEvent)
{
  UNREFERENCED_PARAMETER( pvEvent);
}

/*****************************************************************************/
/*! Wait for event
*     \param pvEvent   Handle to event
*     \param ulTimeout Timeout in ms to wait for event
*     \return CIFX_EVENT_SIGNALLED if event was set, CIFX_EVENT_TIMEOUT otherwise */
/*****************************************************************************/
uint32_t OS_WaitEvent(void* pvEvent, uint32_t ulTimeout)
{
  UNREFERENCED_PARAMETER( ulTimeout);
  UNREFERENCED_PARAMETER( pvEvent);

  return 0;
}

/*****************************************************************************/
/*! Create Semaphore
*     \param ulInitialCount
*     \return Pointer to semaphore object                                    */
/*****************************************************************************/
void* OS_CreateSemaphore(uint32_t ulInitialCount)
{
  UNREFERENCED_PARAMETER( ulInitialCount);

  return NULL;
}

/*****************************************************************************/
/*! Inc Semaphore
*     \param pvSem    Pointer to semaphore object
*     \param ulCount  value to increment                                     */
/*****************************************************************************/
void OS_PutSemaphore(void* pvSem, uint32_t ulCount)
{
  UNREFERENCED_PARAMETER( pvSem);
  UNREFERENCED_PARAMETER( ulCount);
}

/*****************************************************************************/
/*! Deletes Semaphore
*     \param pvSem    Pointer to semaphore object                            */
/*****************************************************************************/
void OS_DeleteSemaphore(void* pvSem)
{
  UNREFERENCED_PARAMETER( pvSem);
}

/*****************************************************************************/
/*! Wait for semaphore
*     \param pvSem   Pointer to semaphore object
*     \param ulTimeout Timeout in ms to wait for semaphore
*     \return OS_SEM_SIGNALLED if semaphore was set, OS_SEM_TIMEOUT otherwise */
/*****************************************************************************/
uint32_t OS_WaitSemaphore(void* pvSem, uint32_t ulTimeout)
{
  UNREFERENCED_PARAMETER( pvSem);
  UNREFERENCED_PARAMETER( ulTimeout);

  return 0;
}

/*****************************************************************************/
/*! Returns currents system timer tick
*     \return timertick                                                      */
/*****************************************************************************/
uint32_t OS_GetMilliSecCounter()
{
  return 0;
}

/*****************************************************************************/
/*! Sleep
*     \param ulSleep Time to sleep in ms                                     */
/*****************************************************************************/
void OS_Sleep(uint32_t ulSleep)
{
  UNREFERENCED_PARAMETER( ulSleep);
}

