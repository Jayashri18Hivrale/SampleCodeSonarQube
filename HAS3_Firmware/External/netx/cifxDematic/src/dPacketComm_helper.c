#include "dPacketComm_helper.h"
#include <stdio.h>

#define PRINTF printf
#define NEWLINE "\r\n"
/*****************************************************************************/
/*! Displays a hex dump on the debug console (16 bytes per line)
 *
 *   \param pbData    [in]  Data buffer that shall be dumped
 *   \param ulDataLen [in]  Number of bytes in pbData
 */
/*****************************************************************************/
static void Pkt_PrintFormattedHexData( unsigned char* pbData,
                           unsigned long  ulDataLen )
{
  unsigned long ulIdx;

  if(CIFX_MAX_DATA_SIZE < ulDataLen)
  {
    ulDataLen = CIFX_MAX_DATA_SIZE;
  }

  for(ulIdx = 0; ulIdx < ulDataLen; ++ulIdx)
  {
    if(0 == (ulIdx % 16))
    {
      PRINTF(NEWLINE);
    }

    PRINTF("%02X ", pbData[ulIdx]);
  }

  PRINTF(NEWLINE);
}
/*****************************************************************************/
/*! Dumps a packet to the debug console
 *
 *   \param ptPacket  [in] Packet to be dumped
 */
/*****************************************************************************/
void Pkt_DumpPacket( CIFX_PACKET* ptPacket )
{
  PRINTF("Dest   : 0x%08X      ID   : 0x%08X" NEWLINE, (unsigned int) ptPacket->tHeader.ulDest, (unsigned int) ptPacket->tHeader.ulId);
  PRINTF("Src    : 0x%08X      Sta  : 0x%08X" NEWLINE, (unsigned int) ptPacket->tHeader.ulSrc, (unsigned int) ptPacket->tHeader.ulState);
  PRINTF("DestID : 0x%08X      Cmd  : 0x%08X" NEWLINE, (unsigned int) ptPacket->tHeader.ulDestId, (unsigned int) ptPacket->tHeader.ulCmd);
  PRINTF("SrcID  : 0x%08X      Ext  : 0x%08X" NEWLINE, (unsigned int) ptPacket->tHeader.ulSrcId, (unsigned int) ptPacket->tHeader.ulExt);
  PRINTF("Len    : 0x%08X      Rout : 0x%08X" NEWLINE, (unsigned int) ptPacket->tHeader.ulLen, (unsigned int) ptPacket->tHeader.ulRout);

  if(ptPacket->tHeader.ulLen)
  {
    PRINTF("Data:");

    /** Displays a hex dump on the debug console (16 bytes per line) */
    Pkt_PrintFormattedHexData(ptPacket->abData, ptPacket->tHeader.ulLen);
  }

  PRINTF(NEWLINE);
}
