#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

#include "cifXErrors.h"
#include "has3_device.h"
#include "nHost_pnm.h"
#include "pnTests.h"

void display_menu();
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
      6. Create/Get configuredSlavesHandlers handlders for each netHost
      7. Verify slaves topology
      8. Start slaves monitoring/testing
                                                                             */
/*****************************************************************************/
int main(void)
{
    hnHostCifx nHosts = NHsCifx_Init();
    if (NULL == nHosts || nHosts->cifxStatus != CIFX_NO_ERROR)
    { 
        printf("Failed to initialize Cifx Toolkit");
        NHsCifx_DeInit();
        return -1;
    }

    if (nHosts->nHostConnected == 0)
    {
        printf("No NetHost Connected");
        NHsCifx_DeInit();
        return -1;
    }

    hHNost_Profinet pnHandler = NHsPNM_Init(nHosts);
    
    if (pnHandler->PN_NetHostsCount != 1)
    {
        printf("Incorrect number of PN Nethost connected, there are %d but should be only 1", pnHandler->PN_NetHostsCount);
        return -1;
    }

    NHost_Profinet_t* profinetNethost = &pnHandler->PN_NetHosts[0];

    

   // NHost_PNSlave_t* has3 = &(pnHandler->PN_NetHosts[0].pnSlaves[0]);

    
  ///*****************************************************************************/
  ///* 8. Start slaves monitoring/testing           */
  ///*****************************************************************************/
    char choice = 0;

    system("cls");  // Clear screen on Windows
    printf("HAS 3 ProfiNet Device Test Program\n");
    printf("==================================\n");
    display_menu();
    while (1) {
        if (_kbhit()) {
            choice = _getch();
            system("cls");  // Clear screen on Windows
            switch (toupper(choice)) {
            case '1':
                run_cyclic_test(profinetNethost);
                display_menu();
                break;
            case '2':
                NHsPNM_DumpSlavesInfo(profinetNethost);
                display_menu();
                break;
            case '3':
                // run_other_test2(netHost);
                break;
            case 'Q':
                printf("Exiting test program...\n");
                //shutdown_profinet_connection(netHost);  // Your cleanup function
                return 0;
            default:
                printf("Invalid choice! Please try again.\n");
                break;
            }
        }
        Sleep(100);  // Reduce CPU usage
    }

  return 0;
}

void display_menu() {
    printf("\nMain Test Menu:\n");
    printf("1) Run Cyclic Speed Test\n");
    printf("2) Dump connected slaves info\n");
    //printf("3) Run Other Test 2\n");
    printf("Q) Exit Program\n");
    printf("Enter choice: ");
    fflush(stdout);
}