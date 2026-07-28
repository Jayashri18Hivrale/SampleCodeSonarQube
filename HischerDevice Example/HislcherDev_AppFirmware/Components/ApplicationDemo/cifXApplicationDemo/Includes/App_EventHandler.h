/**************************************************************************************
Exclusion of Liability for this demo software:
  The following software is intended for and must only be used for reference and in an
  evaluation laboratory environment. It is provided without charge and is subject to
  alterations. There is no warranty for the software, to the extent permitted by
  applicable law. Except when otherwise stated in writing the copyright holders and/or
  other parties provide the software "as is" without warranty of any kind, either
  expressed or implied.
  Please refer to the Agreement in README_DISCLAIMER.txt, provided together with this file!
  By installing or otherwise using the software, you accept the terms of this Agreement.
  If you do not agree to the terms of this Agreement, then do not install or use the
  Software!
**************************************************************************************/

/**************************************************************************************

Copyright (c) Hilscher Gesellschaft fuer Systemautomation mbH. All Rights Reserved.

***************************************************************************************/

/*****************************************************************************/
/*! \file  EventHandler.h
*   Event Handler header file                                                */
/*****************************************************************************/

#ifndef COMPONENTS_CIFXAPPLICATIONDEMO_INCLUDES_APP_EVENTHANDLER_H_
#define COMPONENTS_CIFXAPPLICATIONDEMO_INCLUDES_APP_EVENTHANDLER_H_

/*****************************************************************************/
/*! General Inclusion Area                                                   */
/*****************************************************************************/

#include "App_DemoApplication.h"

/*****************************************************************************/
/*! FORWARD DECLARATIONS                                                     */
/*****************************************************************************/

struct APP_DATA_Ttag;

/*****************************************************************************/
/*! ENUMERATIONS                                                             */
/*****************************************************************************/
typedef enum APP_EVENT_Etag
{
  APP_EVENT_NONE = 0,

  /* Alarms 1 - 4 */
  APP_EVENT_SET_ALARM1,
  APP_EVENT_CLEAR_ALARM1,

  APP_EVENT_SET_ALARM2,
  APP_EVENT_CLEAR_ALARM2,

  APP_EVENT_SET_ALARM3,
  APP_EVENT_CLEAR_ALARM3,

  APP_EVENT_SET_ALARM4,
  APP_EVENT_CLEAR_ALARM4,

  /* Errors 1 - 4 */
  APP_EVENT_SET_ERROR1,
  APP_EVENT_CLEAR_ERROR1,

  APP_EVENT_SET_ERROR2,
  APP_EVENT_CLEAR_ERROR2,

  APP_EVENT_SET_ERROR3,
  APP_EVENT_CLEAR_ERROR3,

  APP_EVENT_SET_ERROR4,
  APP_EVENT_CLEAR_ERROR4,
} APP_EVENT_E;

/*****************************************************************************/
/*! FUNCTION PROTOTYPES                                                      */
/*****************************************************************************/
void        App_EventHandler     ( struct APP_DATA_Ttag* ptAppData );

#endif /** COMPONENTS_CIFXAPPLICATIONDEMO_INCLUDES_APP_EVENTHANDLER_H_ */
