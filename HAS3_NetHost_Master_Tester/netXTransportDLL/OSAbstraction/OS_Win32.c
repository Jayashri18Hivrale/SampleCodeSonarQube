/**************************************************************************************

   Copyright (c) Hilscher GmbH. All Rights Reserved.

 **************************************************************************************

   Filename:
    $Id: OS_win32.c 3194 2011-12-14 16:36:28Z Robert $
   Last Modification:
    $Author: Robert $
    $Date: 2011-12-14 17:36:28 +0100 (Mi, 14 Dez 2011) $
    $Revision: 3194 $

   Targets:
     win32        : yes

   Description:
    windows OS abstraction for netXTransport toolkit

   Changes:

     Version   Date        Author   Description
     ----------------------------------------------------------------------------------
      1        23.02.13    MT       initial version

**************************************************************************************/

#include "OS_Dependent.h"
#include <OS_Includes.h>

/*****************************************************************************/
/*! Create Lock (Usually same as mutex, but does not support timed waiting)
*     \return Handle to created lock                                         */
/*****************************************************************************/
void* OS_CreateLock(void)
{
  CRITICAL_SECTION* ptSection = malloc(sizeof(*ptSection));

  if(NULL != ptSection)
  {
    InitializeCriticalSection(ptSection);
  }
  return ptSection;
}

/*****************************************************************************/
/*! Acquire a lock
*     \param pvLock Handle to lock                                           */
/*****************************************************************************/
void OS_EnterLock(void* pvLock)
{
  CRITICAL_SECTION* ptSection = (CRITICAL_SECTION*)pvLock;

  EnterCriticalSection(ptSection);
}

/*****************************************************************************/
/*! Release a lock
*     \param pvLock Handle to lock                                           */
/*****************************************************************************/
void OS_LeaveLock(void* pvLock)
{
  CRITICAL_SECTION* ptSection = (CRITICAL_SECTION*)pvLock;

  LeaveCriticalSection(ptSection);
}

/*****************************************************************************/
/*! Delete a lock
*     \param pvLock Handle to lock                                           */
/*****************************************************************************/
void OS_DeleteLock(void* pvLock)
{
  CRITICAL_SECTION* ptSection = (CRITICAL_SECTION*)pvLock;

  DeleteCriticalSection(ptSection);
  free(ptSection);
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
  int     iRet;
  va_list ap;

  va_start(ap, format);
  iRet = vsprintf_s( szDest, ulSize, format, ap);
  va_end(ap);

  return szDest;
}

/*****************************************************************************/
/*! Compare strings
*     \param pszBuf1  String buffer 1
*     \param pszBuf2  String buffer 2
*     \return 0 if strings are equal                                         */
/*****************************************************************************/
int OS_Strcmp(const char* pszBuf1, const char* pszBuf2)
{
  return strcmp(pszBuf1, pszBuf2);
}

/*****************************************************************************/
/*! Compare strings (case sensitive)
*     \param pszBuf1  String buffer 1
*     \param pszBuf2  String buffer 2
*     \return 0 if strings are equal                                         */
/*****************************************************************************/
int OS_Strnicmp (const char* pszBuf1, const char* pszBuf2, uint32_t len)
{
  return _strnicmp(pszBuf1, pszBuf2, len);
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
  strncpy_s(szDest, ulDstSize, szSource, ulLen);

  return szDest;
}

/*****************************************************************************/
/*! Get length of string
*     \param szText  Text buffer
*     \return Length of given string                                         */
/*****************************************************************************/
int OS_Strlen(const char* szText)
{
  return (int)strlen(szText);
}

/*****************************************************************************/
/*! Find string occurrence
*     \param szText  Text buffer
*     \return Length of given string                                         */
/*****************************************************************************/
uint32_t OS_Strcspn( char* szSearchStr, char* szFindStr)
{
  return (uint32_t)strcspn( szSearchStr, szFindStr);
}

/*****************************************************************************/
/*! Concatenate strings
*     \param szDest  Pointer to destiantion string
*     \param szSrc   Pointer to source string
*     \return szDest                                                         */
/*****************************************************************************/
char* OS_Strcat(char* szDest, uint32_t ulDstSize, char* szSrc)
{
  strcat_s( szDest, ulDstSize, szSrc);

  return szDest;
}

/*****************************************************************************/
/*! Memory allocation
*     \param ulSize Size of block to allocate
*     \return NULL on failure                                                */
/*****************************************************************************/
void* OS_Memalloc(uint32_t ulSize)
{
  return malloc(ulSize);
}

/*****************************************************************************/
/*! Memset
*     \param pvMem   Memory to set
*     \param bFill   Fill byte
*     \param ulSize  Size of the fill block                                  */
/*****************************************************************************/
void OS_Memset(void* pvMem, uint8_t bFill, uint32_t ulSize)
{
  memset(pvMem, bFill, ulSize);
}

/*****************************************************************************/
/*! Memcopy
*     \param pvDest  Destination pointer
*     \param pvSrc   Source pointer
*     \param ulSize  Size to copy                                            */
/*****************************************************************************/
void OS_Memcpy(void* pvBuf1, void* pvBuf2, uint32_t ulSize)
{
  memcpy(pvBuf1, pvBuf2, ulSize);
}

/*****************************************************************************/
/*! Memory resize
*     \param pvMem      Block to resize
*     \param ulNewSize  New size of the block
*     \return NULL on error                                                  */
/*****************************************************************************/
void* OS_Memrealloc(void* pvBuffer, uint32_t ulSize)
{
  return realloc(pvBuffer,ulSize);
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
  return memcmp(pvBuf1, pvBuf2, ulSize);
}

/*****************************************************************************/
/*! Memory de-allocation
*     \param pvMem  Block to free                                            */
/*****************************************************************************/
void OS_Memfree(void* pvMem)
{
  free(pvMem);
}

/*****************************************************************************/
/*! Create event
*     \return Handle to created event                                        */
/*****************************************************************************/
void* OS_CreateEvent(void)
{
  return CreateEvent(NULL, FALSE, FALSE, NULL);
}

/*****************************************************************************/
/*! Signal event
*     \param pvEvent Handle to event                                         */
/*****************************************************************************/
void OS_SetEvent(void* pvEvent)
{
  SetEvent(pvEvent);
}

/*****************************************************************************/
/*! Reset event
*     \param pvEvent Handle to event                                         */
/*****************************************************************************/
void OS_ResetEvent(void* pvEvent)
{
  ResetEvent(pvEvent);
}

/*****************************************************************************/
/*! Delete event
*     \param pvEvent Handle to event                                         */
/*****************************************************************************/
void OS_DeleteEvent(void* pvEvent)
{
  CloseHandle(pvEvent);
}

/*****************************************************************************/
/*! Wait for event
*     \param pvEvent   Handle to event
*     \param ulTimeout Timeout in ms to wait for event
*     \return CIFX_EVENT_SIGNALLED if event was set, CIFX_EVENT_TIMEOUT otherwise */
/*****************************************************************************/
uint32_t OS_WaitEvent(void* pvEvent, uint32_t ulTimeout)
{
  uint32_t ulRet = OS_EVENT_TIMEOUT;

  if(WAIT_OBJECT_0 == WaitForSingleObject(pvEvent, ulTimeout))
    ulRet = OS_EVENT_SIGNALLED;

  return ulRet;
}

/*****************************************************************************/
/*! Create Semaphore
*     \param ulInitialCount
*     \return Pointer to semaphore object                                    */
/*****************************************************************************/
void* OS_CreateSemaphore(uint32_t ulInitialCount)
{
  return CreateSemaphore(NULL, ulInitialCount, MAXLONG, NULL);
}

/*****************************************************************************/
/*! Inc Semaphore
*     \param pvSem    Pointer to semaphore object
*     \param ulCount  value to increment                                     */
/*****************************************************************************/
void OS_PutSemaphore(void* pvSem, uint32_t ulCount)
{
  ReleaseSemaphore(pvSem, ulCount, NULL);
}

/*****************************************************************************/
/*! Deletes Semaphore
*     \param pvSem    Pointer to semaphore object                            */
/*****************************************************************************/
void OS_DeleteSemaphore(void* pvSem)
{
  CloseHandle(pvSem);
}

/*****************************************************************************/
/*! Wait for semaphore
*     \param pvSem   Pointer to semaphore object
*     \param ulTimeout Timeout in ms to wait for semaphore
*     \return OS_SEM_SIGNALLED if semaphore was set, OS_SEM_TIMEOUT otherwise */
/*****************************************************************************/
uint32_t OS_WaitSemaphore(void* pvSem, uint32_t ulTimeout)
{
  uint32_t ulRet = OS_SEM_TIMEOUT;

  if(WAIT_OBJECT_0 == WaitForSingleObject(pvSem, ulTimeout))
    ulRet = OS_SEM_SIGNALLED;

  return ulRet;
}

/*****************************************************************************/
/*! Returns currents system timer tick
*     \return timertick                                                      */
/*****************************************************************************/
uint32_t OS_GetMilliSecCounter()
{
  return GetTickCount();
}

/*****************************************************************************/
/*! Sleep
*     \param ulSleep Time to sleep in ms                                     */
/*****************************************************************************/
void OS_Sleep(uint32_t ulSleep)
{
  Sleep( ulSleep);
}

