#include <stdio.h>
#include <stdlib.h>

#include "FieldBus_Master_Common.h"
#include "hilscherDev.h"
#include "cifXErrors.h"
#include "cifXUser.h"

/*****************************************************************************/
/*! Application

     The application initializes the netXTransport toolkit, supporting the
     Translation-Layer rcX-Packet (Data-Type=0x100) and cifXMarshaller
     (Data-Type=0x200).
     The application uses the TCP connector type.
     
     To be able to use the cifX API interface (communicate with NetHost Devices), the following steps are required (see code below):
      1. Initialize netXTransport Toolkit.
      2. Get Cifx Driver.
      3. Get NetHosts Information.
      4. Create Connection handlers (one per netHost)
      5. Verify netHost topology
      6. Create/Get Slaves handlders for each netHost
      7. Verify slaves topology
      8. Start slaves monitoring/testing
                                                                             */
/*****************************************************************************/
int main(void)
{
  
  printf("\n***********************************************************************\n\n");
  printf("  Hilscher (Profibus and Profinet) Device Test Example using CIFx-API\n");
  printf("\n***********************************************************************\n");
  /*****************************************************************************/
  /* 1. Initialize netXTransport Toolkit using TCP IP connectio                */
  /*****************************************************************************/
  int32_t lRet = StartTcpTransportLayer();

  /*****************************************************************************/
  /* 2. Getting the cifx driver and its information                 */
  /*****************************************************************************/
  CIFXHANDLE cifxDriver = NULL;
  DRIVER_INFORMATION tDriverInfo = { 0 };
  int connectedNethosts = 0;
  if (lRet == CIFX_NO_ERROR)
  {
      lRet = GetCifxDriverAndRelatedInfo(&cifxDriver, &tDriverInfo);
      connectedNethosts = tDriverInfo.ulBoardCnt;
  }

  /*****************************************************************************/
  /* 3. Getting connected NetHosts Information (local network)                 */
  /*****************************************************************************/
  // Dinamically get the array of NetHost Boards Info (to get the name)
  BOARD_INFORMATION** NetHostBoardsInfo = malloc(connectedNethosts * sizeof(BOARD_INFORMATION*));
  if (lRet == CIFX_NO_ERROR)
  {
      lRet = GetConnectedNetHostsBoardInformation(cifxDriver, NetHostBoardsInfo, connectedNethosts);
  }

  /*****************************************************************************/
  /* 4. Getting connection handlers (one per Nethost)                      */
  /*****************************************************************************/
  // Dinamically get the array of ConnectionHandlers
  FIELDBUS_CONNECTION_HANDLERS** ConnectionHandlers = malloc(connectedNethosts * sizeof(FIELDBUS_CONNECTION_HANDLERS*));

  for (int i = 0; i < connectedNethosts; i++)
  {
      ConnectionHandlers[i] = malloc(sizeof(FIELDBUS_CONNECTION_HANDLERS));
      ConnectionHandlers[i]->cifxDriver = cifxDriver;   // Asign the cifxdriver to each element
  }
 
  if (lRet == CIFX_NO_ERROR)
  {
      lRet = GetConnectionHandlers(ConnectionHandlers, NetHostBoardsInfo, connectedNethosts);
  }

  /******************************************************************************/
  /* 5. Verify 2 netHost topology one for PN one for PB the board Names for each*/
  /******************************************************************************/

  // ToDo. Add code to end program if two nethost on PB and one PN are not connected.

  // ToDO. GetPointer to Pn and Get Pointer to Pb for now manually done

  FIELDBUS_CONNECTION_HANDLERS* hPnConnection = ConnectionHandlers[0];
  
  FIELDBUS_CONNECTION_HANDLERS* hPbConnection = ConnectionHandlers[1];

  /*****************************************************************************/
  /* 6. Get Slaves Handlers                                                    */
  /*****************************************************************************/

  // To Do. Enclose this in a pn_slaves_module
  
  FIELDBUS_SLAVES_INFO_T pn_slaves_info = { 0 };
  pn_slaves_info.connectionHandler = hPnConnection;
  pn_slaves_info.ulSlaveState = RCX_LIST_ACTV_SLAVES;

  if (lRet == CIFX_NO_ERROR)
  {
      lRet = FieldBusGetActiveSlaves(&pn_slaves_info);
  }

  // To Do. Create Function to get slave info for profibus
 

  /******************************************************************************/
  /* 7. Verify slaves topology one each Nethposts                               */
  /******************************************************************************/
   // To Do. Create get_pb_slave_info function for slave handler as argument
   // To Do. Create get_pn_slave_info for slave handler as argument

  /*****************************************************************************/
  /* 8. Start slaves monitoring/testing           */
  /*****************************************************************************/
   // To Do: Move all functions to hilscherDev.c and define application functionallity
  if (lRet == CIFX_NO_ERROR && pn_slaves_info.pNumSlaves > 0)      
  {
      CIFXHANDLE hChannel = NULL;
      if (CIFX_NO_ERROR != (lRet = xChannelOpen(hPnConnection->cifxDriver, hPnConnection->szBoardName, 0, &hChannel))) {
          printf("Failed to open system device. Error: 0x%X\n", lRet);
      }

      Device_OutputData Out = { 0 };
      Out.LED_1 = 1;
      Out.LED_2 = 1;
      Out.CountSpeed = 2;

      Device_InputData In = { 0 };

      if (lRet == CIFX_NO_ERROR)
      {
          xChannelIOWrite(hChannel, pn_slaves_info.pSlaveHandles[0], 0, 6, &Out, 50); // To access 
      }
      for (int i = 0; i < 1000; i++)
      {
          //xChannelIORead(hChannel, pn_slaves_info.pSlaveHandles[1], 0, 10, &In, 1);
          //xChannelIORead(hChannel, pn_slaves_info.pSlaveHandles[1], 0, 10, &In, 1);
          lRet = xChannelIORead(hChannel, pn_slaves_info.pSlaveHandles[0], 0, 10, &In, 10);
          if (lRet == CIFX_NO_ERROR)
              printf("Count: %d\n", In.CounterValue);
          else
              printf("Error: %d\n", lRet);
          // printf("Actuator 1: %d\n", In.Actuator_1);
          // printf("Actuator 2: %d\n", In.Actuator_2);
          // printf("State 1: %d\n", In.State1);
          // printf("State 2: %d\n", In.State2);
      }

      xChannelClose(hPnConnection->cifxDriver);
  }

  /* Closing the application. */
  StopTcpTransportLayer();

  printf("\n\nEnding Program\n");

  return 0;
}