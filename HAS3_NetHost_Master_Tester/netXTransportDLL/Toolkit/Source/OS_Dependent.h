/**************************************************************************************

   Copyright (c) Hilscher GmbH. All Rights Reserved.

 **************************************************************************************

   Filename:
    $Workfile: OS_Dependent.h $
   Last Modification:
    $Author: Robert $
    $Modtime: 23.06.08 14:48 $
    $Revision: 3151 $

   Targets:
     O/S Independent : yes

   Description:
    OS Dependent function declaration. These functions must be implemented

   Changes:

     Version   Date        Author   Description
     ----------------------------------------------------------------------------------
     1         23.02.2013  MT       intitial version

**************************************************************************************/

#ifndef __OS_DEPENDENT__H
#define __OS_DEPENDENT__H

#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif

#define OS_EVENT_SIGNALLED  0
#define OS_EVENT_TIMEOUT    1

#define OS_SEM_SIGNALLED    0
#define OS_SEM_TIMEOUT      1

void*    OS_Memalloc(uint32_t ulSize);
void*    OS_Memrealloc(void* pvBuffer, uint32_t ulSize);
void     OS_Memfree(void* pvMem);

void     OS_Memset(void* pvMem, unsigned char bFill, uint32_t ulSize);
void     OS_Memcpy(void* pvDest, void* pvSrc, uint32_t ulSize);
int      OS_Memcmp(void* pvBuf1, void* pvBuf2, uint32_t ulSize);
void     OS_Memmove(void* pvDest, void* pvSrc, uint32_t ulSize);

uint32_t OS_GetMilliSecCounter(void);
void     OS_Sleep(uint32_t ulSleepTimeMs);

void*    OS_CreateLock(void);
void     OS_EnterLock(void* pvLock);
void     OS_LeaveLock(void* pvLock);
void     OS_DeleteLock(void* pvLock);

void*    OS_CreateEvent(void);
void     OS_SetEvent(void* pvEvent);
void     OS_ResetEvent(void* pvEvent);
void     OS_DeleteEvent(void* pvEvent);
uint32_t OS_WaitEvent(void* pvEvent, uint32_t ulTimeout);

void*    OS_CreateSemaphore(uint32_t ulInitialCount);
void     OS_PutSemaphore(void* pvSem, uint32_t ulCount);
void     OS_DeleteSemaphore(void* pvSem);
uint32_t OS_WaitSemaphore(void* pvSem, uint32_t ulTimeout);

int      OS_Strcmp(const char* pszBuf1, const char* pszBuf2);
int      OS_Strnicmp(const char* pszBuf1, const char* pszBuf2, uint32_t ulLen);
int      OS_Strlen(const char* szText);
char*    OS_Strncpy(char* szDest, uint32_t ulDstSize, const char* szSource, uint32_t ulLen);
char*    OS_Strvsprintf(char *szDest, uint32_t ulSize, const char *format, ...);
uint32_t OS_Strcspn( char* szSearchStr, char* szFindStr);
char*    OS_Strcat(char* szDest, uint32_t ulDstSize, char* szSrc);

#ifdef __cplusplus
}
#endif


#endif /* __OS_DEPENDENT__H */
