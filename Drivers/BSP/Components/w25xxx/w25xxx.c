
#include "w25xxx.h"
#include "w25xxxConf.h"

#if (_W25XXX_DEBUG==1)
#include <stdio.h>
#endif

#define W25XXX_DUMMY_BYTE         0xA5

w25qxx_t	w25xxx;

#define	W25xxx_Delay(delay)		HAL_Delay(delay)

//###################################################################################################################
uint8_t	W25xxx_Spi(uint8_t	Data)
{
  uint8_t	ret;
  HAL_SPI_TransmitReceive(&_W25XXX_SPI,&Data,&ret,1,100);
  return ret;	
}
//###################################################################################################################
uint32_t W25xxx_ReadID(void)
{
  uint32_t Temp = 0, Temp0 = 0, Temp1 = 0, Temp2 = 0;
  HAL_GPIO_WritePin(SPI5_GPIO_NSS_GPIO_Port,SPI5_GPIO_NSS_Pin,GPIO_PIN_RESET);
  W25xxx_Spi(0x9F);
  Temp0 = W25xxx_Spi(W25XXX_DUMMY_BYTE);
  Temp1 = W25xxx_Spi(W25XXX_DUMMY_BYTE);
  Temp2 = W25xxx_Spi(W25XXX_DUMMY_BYTE);
  HAL_GPIO_WritePin(SPI5_GPIO_NSS_GPIO_Port,SPI5_GPIO_NSS_Pin,GPIO_PIN_SET);
  Temp = (Temp0 << 16) | (Temp1 << 8) | Temp2;
  return Temp;
}
//###################################################################################################################
void W25xxx_ReadUniqID(void)
{
  HAL_GPIO_WritePin(SPI5_GPIO_NSS_GPIO_Port,SPI5_GPIO_NSS_Pin,GPIO_PIN_RESET);
  W25xxx_Spi(0x4B);
  for(uint8_t	i=0;i<4;i++)
    W25xxx_Spi(W25XXX_DUMMY_BYTE);
  for(uint8_t	i=0;i<8;i++)
    w25xxx.UniqID[i] = W25xxx_Spi(W25XXX_DUMMY_BYTE);
  HAL_GPIO_WritePin(SPI5_GPIO_NSS_GPIO_Port,SPI5_GPIO_NSS_Pin,GPIO_PIN_SET);
}
//###################################################################################################################
void W25xxx_WriteEnable(void)
{
  HAL_GPIO_WritePin(SPI5_GPIO_NSS_GPIO_Port,SPI5_GPIO_NSS_Pin,GPIO_PIN_RESET);
  W25xxx_Spi(0x06);
  HAL_GPIO_WritePin(SPI5_GPIO_NSS_GPIO_Port,SPI5_GPIO_NSS_Pin,GPIO_PIN_SET);
  W25xxx_Delay(1);
}
//###################################################################################################################
void W25xxx_WriteDisable(void)
{
  HAL_GPIO_WritePin(SPI5_GPIO_NSS_GPIO_Port,SPI5_GPIO_NSS_Pin,GPIO_PIN_RESET);
  W25xxx_Spi(0x04);
  HAL_GPIO_WritePin(SPI5_GPIO_NSS_GPIO_Port,SPI5_GPIO_NSS_Pin,GPIO_PIN_SET);
  W25xxx_Delay(1);
}
//###################################################################################################################
uint8_t W25xxx_ReadStatusRegister(uint8_t	SelectStatusRegister_1_2_3)
{
  uint8_t	status=0;
  HAL_GPIO_WritePin(SPI5_GPIO_NSS_GPIO_Port,SPI5_GPIO_NSS_Pin,GPIO_PIN_RESET);
  if(SelectStatusRegister_1_2_3==1)
  {
    W25xxx_Spi(0x05);
    status=W25xxx_Spi(W25XXX_DUMMY_BYTE);	
    w25xxx.StatusRegister1 = status;
  }
  else if(SelectStatusRegister_1_2_3==2)
  {
    W25xxx_Spi(0x35);
    status=W25xxx_Spi(W25XXX_DUMMY_BYTE);	
    w25xxx.StatusRegister2 = status;
  }
  else
  {
    W25xxx_Spi(0x15);
    status=W25xxx_Spi(W25XXX_DUMMY_BYTE);	
    w25xxx.StatusRegister3 = status;
  }	
  HAL_GPIO_WritePin(SPI5_GPIO_NSS_GPIO_Port,SPI5_GPIO_NSS_Pin,GPIO_PIN_SET);
  return status;
}
//###################################################################################################################
void W25xxx_WriteStatusRegister(uint8_t	SelectStatusRegister_1_2_3,uint8_t Data)
{
  HAL_GPIO_WritePin(SPI5_GPIO_NSS_GPIO_Port,SPI5_GPIO_NSS_Pin,GPIO_PIN_RESET);
  if(SelectStatusRegister_1_2_3==1)
  {
    W25xxx_Spi(0x01);
    w25xxx.StatusRegister1 = Data;
  }
  else if(SelectStatusRegister_1_2_3==2)
  {
    W25xxx_Spi(0x31);
    w25xxx.StatusRegister2 = Data;
  }
  else
  {
    W25xxx_Spi(0x11);
    w25xxx.StatusRegister3 = Data;
  }
  W25xxx_Spi(Data);
  HAL_GPIO_WritePin(SPI5_GPIO_NSS_GPIO_Port,SPI5_GPIO_NSS_Pin,GPIO_PIN_SET);
}
//###################################################################################################################
void W25xxx_WaitForWriteEnd(void)
{
  W25xxx_Delay(1);
  HAL_GPIO_WritePin(SPI5_GPIO_NSS_GPIO_Port,SPI5_GPIO_NSS_Pin,GPIO_PIN_RESET);
  W25xxx_Spi(0x05);
  do
  {
    w25xxx.StatusRegister1 = W25xxx_Spi(W25XXX_DUMMY_BYTE);
    W25xxx_Delay(1);
  }
  while ((w25xxx.StatusRegister1 & 0x01) == 0x01);
  HAL_GPIO_WritePin(SPI5_GPIO_NSS_GPIO_Port,SPI5_GPIO_NSS_Pin,GPIO_PIN_SET);
}
//###################################################################################################################
bool	W25xxx_Init(void)
{
  uint16_t devID;
  w25xxx.Lock=1;	
  while(HAL_GetTick()<100)
    W25xxx_Delay(1);
  HAL_GPIO_WritePin(SPI5_GPIO_NSS_GPIO_Port,SPI5_GPIO_NSS_Pin,GPIO_PIN_SET);
  W25xxx_Delay(1000);
  uint32_t	id;
#if (_W25XXX_DEBUG==1)
  printf("w25xxx Init Begin...\r\n");
#endif
  id=W25xxx_ReadID();
  devID = id&0x0000FFFF;
#if (_W25XXX_DEBUG==1)
  printf("w25xxx ID:0x%X\r\n",id);
#endif
  switch(devID)
  {
  case 0x3014:	//	w25x80
    w25xxx.ID=W25X80;
    w25xxx.BlockCount=16;
#if (_W25XXX_DEBUG==1)
    printf("w25xxx Chip: w25x80\r\n");
#endif
    break;
  case 0x3013:	//	w25x40
    w25xxx.ID=W25X40;
    w25xxx.BlockCount=8;
#if (_W25XXX_DEBUG==1)
    printf("w25xxx Chip: w25x40\r\n");
#endif
    break;
  case 0x3012:	//	w25x20
    w25xxx.ID=W25X20;
    w25xxx.BlockCount=4;
#if (_W25XXX_DEBUG==1)
    printf("w25xxx Chip: w25x20\r\n");
#endif
    break;
  case 0x3011:	//	w25x10
    w25xxx.ID=W25X10;
    w25xxx.BlockCount=2;
#if (_W25XXX_DEBUG==1)
    printf("w25xxx Chip: w25x10\r\n");
#endif
    break;
  default:
#if (_W25XXX_DEBUG==1)
    printf("w25xxx Unknown ID\r\n");
#endif
    w25xxx.Lock=0;	
    return false;
    
  }		
  w25xxx.PageSize=256;
  w25xxx.SectorSize=0x1000;
  w25xxx.SectorCount=w25xxx.BlockCount*16;
  w25xxx.PageCount=(w25xxx.SectorCount*w25xxx.SectorSize)/w25xxx.PageSize;
  w25xxx.BlockSize=w25xxx.SectorSize*16;
  w25xxx.CapacityInKiloByte=(w25xxx.SectorCount*w25xxx.SectorSize)/1024;
  W25xxx_ReadUniqID();
  W25xxx_ReadStatusRegister(1);
  W25xxx_ReadStatusRegister(2);
  W25xxx_ReadStatusRegister(3);
#if (_W25XXX_DEBUG==1)
  printf("w25xxx Page Size: %d Bytes\r\n",w25xxx.PageSize);
  printf("w25xxx Page Count: %d\r\n",w25xxx.PageCount);
  printf("w25xxx Sector Size: %d Bytes\r\n",w25xxx.SectorSize);
  printf("w25xxx Sector Count: %d\r\n",w25xxx.SectorCount);
  printf("w25xxx Block Size: %d Bytes\r\n",w25xxx.BlockSize);
  printf("w25xxx Block Count: %d\r\n",w25xxx.BlockCount);
  printf("w25xxx Capacity: %d KiloBytes\r\n",w25xxx.CapacityInKiloByte);
  printf("w25xxx Init Done\r\n");
#endif
  w25xxx.Lock=0;	
  return true;
}	
//###################################################################################################################
void	W25xxx_EraseChip(void)
{
  while(w25xxx.Lock==1)
    W25xxx_Delay(1);
  w25xxx.Lock=1;	
#if (_W25XXX_DEBUG==1)
  uint32_t	StartTime=HAL_GetTick();	
  printf("w25xxx EraseChip Begin...\r\n");
#endif
  W25xxx_WriteEnable();
  HAL_GPIO_WritePin(SPI5_GPIO_NSS_GPIO_Port,SPI5_GPIO_NSS_Pin,GPIO_PIN_RESET);
  W25xxx_Spi(0xC7);
  HAL_GPIO_WritePin(SPI5_GPIO_NSS_GPIO_Port,SPI5_GPIO_NSS_Pin,GPIO_PIN_SET);
  W25xxx_WaitForWriteEnd();
#if (_W25XXX_DEBUG==1)
  printf("w25xxx EraseBlock done after %d ms!\r\n",HAL_GetTick()-StartTime);
#endif
  W25xxx_Delay(10);
  w25xxx.Lock=0;	
}
//###################################################################################################################
void W25xxx_EraseSector(uint32_t SectorAddr)
{
  while(w25xxx.Lock==1)
    W25xxx_Delay(1);
  w25xxx.Lock=1;	
#if (_W25XXX_DEBUG==1)
  uint32_t	StartTime=HAL_GetTick();	
  printf("w25xxx EraseSector %d Begin...\r\n",SectorAddr);
#endif
  W25xxx_WaitForWriteEnd();
  SectorAddr = SectorAddr * w25xxx.SectorSize;
  W25xxx_WriteEnable();
  HAL_GPIO_WritePin(SPI5_GPIO_NSS_GPIO_Port,SPI5_GPIO_NSS_Pin,GPIO_PIN_RESET);
  W25xxx_Spi(0x20);
  if(w25xxx.ID>=W25X80)
    W25xxx_Spi((SectorAddr & 0xFF000000) >> 24);
  W25xxx_Spi((SectorAddr & 0xFF0000) >> 16);
  W25xxx_Spi((SectorAddr & 0xFF00) >> 8);
  W25xxx_Spi(SectorAddr & 0xFF);
  HAL_GPIO_WritePin(SPI5_GPIO_NSS_GPIO_Port,SPI5_GPIO_NSS_Pin,GPIO_PIN_SET);
  W25xxx_WaitForWriteEnd();
#if (_W25XXX_DEBUG==1)
  printf("w25xxx EraseSector done after %d ms\r\n",HAL_GetTick()-StartTime);
#endif
  W25xxx_Delay(1);
  w25xxx.Lock=0;
}
//###################################################################################################################
void W25xxx_EraseBlock(uint32_t BlockAddr)
{
  while(w25xxx.Lock==1)
    W25xxx_Delay(1);
  w25xxx.Lock=1;	
#if (_W25XXX_DEBUG==1)
  printf("w25xxx EraseBlock %d Begin...\r\n",BlockAddr);
  W25xxx_Delay(100);
  uint32_t	StartTime=HAL_GetTick();	
#endif
  W25xxx_WaitForWriteEnd();
  BlockAddr = BlockAddr * w25xxx.SectorSize*16;
  W25xxx_WriteEnable();
  HAL_GPIO_WritePin(SPI5_GPIO_NSS_GPIO_Port,SPI5_GPIO_NSS_Pin,GPIO_PIN_RESET);
  W25xxx_Spi(0xD8);
  if(w25xxx.ID>=W25X80)
    W25xxx_Spi((BlockAddr & 0xFF000000) >> 24);
  W25xxx_Spi((BlockAddr & 0xFF0000) >> 16);
  W25xxx_Spi((BlockAddr & 0xFF00) >> 8);
  W25xxx_Spi(BlockAddr & 0xFF);
  HAL_GPIO_WritePin(SPI5_GPIO_NSS_GPIO_Port,SPI5_GPIO_NSS_Pin,GPIO_PIN_SET);
  W25xxx_WaitForWriteEnd();
#if (_W25XXX_DEBUG==1)
  printf("w25xxx EraseBlock done after %d ms\r\n",HAL_GetTick()-StartTime);
  W25xxx_Delay(100);
#endif
  W25xxx_Delay(1);
  w25xxx.Lock=0;
}
//###################################################################################################################
uint32_t	W25xxx_PageToSector(uint32_t	PageAddress)
{
  return ((PageAddress*w25xxx.PageSize)/w25xxx.SectorSize);
}
//###################################################################################################################
uint32_t	W25xxx_PageToBlock(uint32_t	PageAddress)
{
  return ((PageAddress*w25xxx.PageSize)/w25xxx.BlockSize);
}
//###################################################################################################################
uint32_t	W25xxx_SectorToBlock(uint32_t	SectorAddress)
{
  return ((SectorAddress*w25xxx.SectorSize)/w25xxx.BlockSize);
}
//###################################################################################################################
uint32_t	W25xxx_SectorToPage(uint32_t	SectorAddress)
{
  return (SectorAddress*w25xxx.SectorSize)/w25xxx.PageSize;
}
//###################################################################################################################
uint32_t	W25xxx_BlockToPage(uint32_t	BlockAddress)
{
  return (BlockAddress*w25xxx.BlockSize)/w25xxx.PageSize;
}
//###################################################################################################################
bool 	W25xxx_IsEmptyPage(uint32_t Page_Address,uint32_t OffsetInByte,uint32_t NumByteToCheck_up_to_PageSize)
{
  while(w25xxx.Lock==1)
    W25xxx_Delay(1);
  w25xxx.Lock=1;	
  if(((NumByteToCheck_up_to_PageSize+OffsetInByte)>w25xxx.PageSize)||(NumByteToCheck_up_to_PageSize==0))
    NumByteToCheck_up_to_PageSize=w25xxx.PageSize-OffsetInByte;
#if (_W25XXX_DEBUG==1)
  printf("w25xxx CheckPage:%d, Offset:%d, Bytes:%d begin...\r\n",Page_Address,OffsetInByte,NumByteToCheck_up_to_PageSize);
  W25xxx_Delay(100);
  uint32_t	StartTime=HAL_GetTick();
#endif		
  uint8_t	pBuffer[32];
  uint32_t	WorkAddress;
  uint32_t	i;
  for(i=OffsetInByte; i<w25xxx.PageSize; i+=sizeof(pBuffer))
  {
    HAL_GPIO_WritePin(SPI5_GPIO_NSS_GPIO_Port,SPI5_GPIO_NSS_Pin,GPIO_PIN_RESET);
    WorkAddress=(i+Page_Address*w25xxx.PageSize);
    W25xxx_Spi(0x0B);
    if(w25xxx.ID>=W25X80)
      W25xxx_Spi((WorkAddress & 0xFF000000) >> 24);
    W25xxx_Spi((WorkAddress & 0xFF0000) >> 16);
    W25xxx_Spi((WorkAddress & 0xFF00) >> 8);
    W25xxx_Spi(WorkAddress & 0xFF);
    W25xxx_Spi(0);
    HAL_SPI_Receive(&_W25XXX_SPI,pBuffer,sizeof(pBuffer),100);	
    HAL_GPIO_WritePin(SPI5_GPIO_NSS_GPIO_Port,SPI5_GPIO_NSS_Pin,GPIO_PIN_SET);	
    for(uint8_t x=0;x<sizeof(pBuffer);x++)
    {
      if(pBuffer[x]!=0xFF)
        goto NOT_EMPTY;		
    }			
  }	
  if((w25xxx.PageSize+OffsetInByte)%sizeof(pBuffer)!=0)
  {
    i-=sizeof(pBuffer);
    for( ; i<w25xxx.PageSize; i++)
    {
      HAL_GPIO_WritePin(SPI5_GPIO_NSS_GPIO_Port,SPI5_GPIO_NSS_Pin,GPIO_PIN_RESET);
      WorkAddress=(i+Page_Address*w25xxx.PageSize);
      W25xxx_Spi(0x0B);
      if(w25xxx.ID>=W25X80)
        W25xxx_Spi((WorkAddress & 0xFF000000) >> 24);
      W25xxx_Spi((WorkAddress & 0xFF0000) >> 16);
      W25xxx_Spi((WorkAddress & 0xFF00) >> 8);
      W25xxx_Spi(WorkAddress & 0xFF);
      W25xxx_Spi(0);
      HAL_SPI_Receive(&_W25XXX_SPI,pBuffer,1,100);	
      HAL_GPIO_WritePin(SPI5_GPIO_NSS_GPIO_Port,SPI5_GPIO_NSS_Pin,GPIO_PIN_SET);	
      if(pBuffer[0]!=0xFF)
        goto NOT_EMPTY;
    }
  }	
#if (_W25XXX_DEBUG==1)
  printf("w25xxx CheckPage is Empty in %d ms\r\n",HAL_GetTick()-StartTime);
  W25xxx_Delay(100);
#endif	
  w25xxx.Lock=0;
  return true;	
NOT_EMPTY:
#if (_W25XXX_DEBUG==1)
  printf("w25xxx CheckPage is Not Empty in %d ms\r\n",HAL_GetTick()-StartTime);
  W25xxx_Delay(100);
#endif	
  w25xxx.Lock=0;
  return false;
}
//###################################################################################################################
bool 	W25xxx_IsEmptySector(uint32_t Sector_Address,uint32_t OffsetInByte,uint32_t NumByteToCheck_up_to_SectorSize)
{
  while(w25xxx.Lock==1)
    W25xxx_Delay(1);
  w25xxx.Lock=1;	
  if((NumByteToCheck_up_to_SectorSize>w25xxx.SectorSize)||(NumByteToCheck_up_to_SectorSize==0))
    NumByteToCheck_up_to_SectorSize=w25xxx.SectorSize;
#if (_W25XXX_DEBUG==1)
  printf("w25xxx CheckSector:%d, Offset:%d, Bytes:%d begin...\r\n",Sector_Address,OffsetInByte,NumByteToCheck_up_to_SectorSize);
  W25xxx_Delay(100);
  uint32_t	StartTime=HAL_GetTick();
#endif		
  uint8_t	pBuffer[32];
  uint32_t	WorkAddress;
  uint32_t	i;
  for(i=OffsetInByte; i<w25xxx.SectorSize; i+=sizeof(pBuffer))
  {
    HAL_GPIO_WritePin(SPI5_GPIO_NSS_GPIO_Port,SPI5_GPIO_NSS_Pin,GPIO_PIN_RESET);
    WorkAddress=(i+Sector_Address*w25xxx.SectorSize);
    W25xxx_Spi(0x0B);
    if(w25xxx.ID>=W25X80)
      W25xxx_Spi((WorkAddress & 0xFF000000) >> 24);
    W25xxx_Spi((WorkAddress & 0xFF0000) >> 16);
    W25xxx_Spi((WorkAddress & 0xFF00) >> 8);
    W25xxx_Spi(WorkAddress & 0xFF);
    W25xxx_Spi(0);
    HAL_SPI_Receive(&_W25XXX_SPI,pBuffer,sizeof(pBuffer),100);	
    HAL_GPIO_WritePin(SPI5_GPIO_NSS_GPIO_Port,SPI5_GPIO_NSS_Pin,GPIO_PIN_SET);	
    for(uint8_t x=0;x<sizeof(pBuffer);x++)
    {
      if(pBuffer[x]!=0xFF)
        goto NOT_EMPTY;		
    }			
  }	
  if((w25xxx.SectorSize+OffsetInByte)%sizeof(pBuffer)!=0)
  {
    i-=sizeof(pBuffer);
    for( ; i<w25xxx.SectorSize; i++)
    {
      HAL_GPIO_WritePin(SPI5_GPIO_NSS_GPIO_Port,SPI5_GPIO_NSS_Pin,GPIO_PIN_RESET);
      WorkAddress=(i+Sector_Address*w25xxx.SectorSize);
      W25xxx_Spi(0x0B);
      if(w25xxx.ID>=W25X80)
        W25xxx_Spi((WorkAddress & 0xFF000000) >> 24);
      W25xxx_Spi((WorkAddress & 0xFF0000) >> 16);
      W25xxx_Spi((WorkAddress & 0xFF00) >> 8);
      W25xxx_Spi(WorkAddress & 0xFF);
      W25xxx_Spi(0);
      HAL_SPI_Receive(&_W25XXX_SPI,pBuffer,1,100);	
      HAL_GPIO_WritePin(SPI5_GPIO_NSS_GPIO_Port,SPI5_GPIO_NSS_Pin,GPIO_PIN_SET);	
      if(pBuffer[0]!=0xFF)
        goto NOT_EMPTY;
    }
  }	
#if (_W25XXX_DEBUG==1)
  printf("w25xxx CheckSector is Empty in %d ms\r\n",HAL_GetTick()-StartTime);
  W25xxx_Delay(100);
#endif	
  w25xxx.Lock=0;
  return true;	
NOT_EMPTY:
#if (_W25XXX_DEBUG==1)
  printf("w25xxx CheckSector is Not Empty in %d ms\r\n",HAL_GetTick()-StartTime);
  W25xxx_Delay(100);
#endif	
  w25xxx.Lock=0;
  return false;
}
//###################################################################################################################
bool 	W25xxx_IsEmptyBlock(uint32_t Block_Address,uint32_t OffsetInByte,uint32_t NumByteToCheck_up_to_BlockSize)
{
  while(w25xxx.Lock==1)
    W25xxx_Delay(1);
  w25xxx.Lock=1;	
  if((NumByteToCheck_up_to_BlockSize>w25xxx.BlockSize)||(NumByteToCheck_up_to_BlockSize==0))
    NumByteToCheck_up_to_BlockSize=w25xxx.BlockSize;
#if (_W25XXX_DEBUG==1)
  printf("w25xxx CheckBlock:%d, Offset:%d, Bytes:%d begin...\r\n",Block_Address,OffsetInByte,NumByteToCheck_up_to_BlockSize);
  W25xxx_Delay(100);
  uint32_t	StartTime=HAL_GetTick();
#endif		
  uint8_t	pBuffer[32];
  uint32_t	WorkAddress;
  uint32_t	i;
  for(i=OffsetInByte; i<w25xxx.BlockSize; i+=sizeof(pBuffer))
  {
    HAL_GPIO_WritePin(SPI5_GPIO_NSS_GPIO_Port,SPI5_GPIO_NSS_Pin,GPIO_PIN_RESET);
    WorkAddress=(i+Block_Address*w25xxx.BlockSize);
    W25xxx_Spi(0x0B);
    if(w25xxx.ID>=W25X80)
      W25xxx_Spi((WorkAddress & 0xFF000000) >> 24);
    W25xxx_Spi((WorkAddress & 0xFF0000) >> 16);
    W25xxx_Spi((WorkAddress & 0xFF00) >> 8);
    W25xxx_Spi(WorkAddress & 0xFF);
    W25xxx_Spi(0);
    HAL_SPI_Receive(&_W25XXX_SPI,pBuffer,sizeof(pBuffer),100);	
    HAL_GPIO_WritePin(SPI5_GPIO_NSS_GPIO_Port,SPI5_GPIO_NSS_Pin,GPIO_PIN_SET);	
    for(uint8_t x=0;x<sizeof(pBuffer);x++)
    {
      if(pBuffer[x]!=0xFF)
        goto NOT_EMPTY;		
    }			
  }	
  if((w25xxx.BlockSize+OffsetInByte)%sizeof(pBuffer)!=0)
  {
    i-=sizeof(pBuffer);
    for( ; i<w25xxx.BlockSize; i++)
    {
      HAL_GPIO_WritePin(SPI5_GPIO_NSS_GPIO_Port,SPI5_GPIO_NSS_Pin,GPIO_PIN_RESET);
      WorkAddress=(i+Block_Address*w25xxx.BlockSize);
      W25xxx_Spi(0x0B);
      if(w25xxx.ID>=W25X80)
        W25xxx_Spi((WorkAddress & 0xFF000000) >> 24);
      W25xxx_Spi((WorkAddress & 0xFF0000) >> 16);
      W25xxx_Spi((WorkAddress & 0xFF00) >> 8);
      W25xxx_Spi(WorkAddress & 0xFF);
      W25xxx_Spi(0);
      HAL_SPI_Receive(&_W25XXX_SPI,pBuffer,1,100);	
      HAL_GPIO_WritePin(SPI5_GPIO_NSS_GPIO_Port,SPI5_GPIO_NSS_Pin,GPIO_PIN_SET);	
      if(pBuffer[0]!=0xFF)
        goto NOT_EMPTY;
    }
  }	
#if (_W25XXX_DEBUG==1)
  printf("w25xxx CheckBlock is Empty in %d ms\r\n",HAL_GetTick()-StartTime);
  W25xxx_Delay(100);
#endif	
  w25xxx.Lock=0;
  return true;	
NOT_EMPTY:
#if (_W25XXX_DEBUG==1)
  printf("w25xxx CheckBlock is Not Empty in %d ms\r\n",HAL_GetTick()-StartTime);
  W25xxx_Delay(100);
#endif	
  w25xxx.Lock=0;
  return false;
}
//###################################################################################################################
void W25xxx_WriteByte(uint8_t pBuffer, uint32_t WriteAddr_inBytes)
{
  while(w25xxx.Lock==1)
    W25xxx_Delay(1);
  w25xxx.Lock=1;
#if (_W25XXX_DEBUG==1)
  uint32_t	StartTime=HAL_GetTick();
  printf("w25xxx WriteByte 0x%02X at address %d begin...",pBuffer,WriteAddr_inBytes);
#endif
  W25xxx_WaitForWriteEnd();
  W25xxx_WriteEnable();
  HAL_GPIO_WritePin(SPI5_GPIO_NSS_GPIO_Port,SPI5_GPIO_NSS_Pin,GPIO_PIN_RESET);
  W25xxx_Spi(0x02);
  if(w25xxx.ID>=W25X80)
    W25xxx_Spi((WriteAddr_inBytes & 0xFF000000) >> 24);
  W25xxx_Spi((WriteAddr_inBytes & 0xFF0000) >> 16);
  W25xxx_Spi((WriteAddr_inBytes & 0xFF00) >> 8);
  W25xxx_Spi(WriteAddr_inBytes & 0xFF);
  W25xxx_Spi(pBuffer);
  HAL_GPIO_WritePin(SPI5_GPIO_NSS_GPIO_Port,SPI5_GPIO_NSS_Pin,GPIO_PIN_SET);
  W25xxx_WaitForWriteEnd();
#if (_W25XXX_DEBUG==1)
  printf("w25xxx WriteByte done after %d ms\r\n",HAL_GetTick()-StartTime);
#endif
  w25xxx.Lock=0;
}
//###################################################################################################################
void 	W25xxx_WritePage(uint8_t *pBuffer	,uint32_t Page_Address,uint32_t OffsetInByte,uint32_t NumByteToWrite_up_to_PageSize)
{
  while(w25xxx.Lock==1)
    W25xxx_Delay(1);
  w25xxx.Lock=1;
  if(((NumByteToWrite_up_to_PageSize+OffsetInByte)>w25xxx.PageSize)||(NumByteToWrite_up_to_PageSize==0))
    NumByteToWrite_up_to_PageSize=w25xxx.PageSize-OffsetInByte;
  if((OffsetInByte+NumByteToWrite_up_to_PageSize) > w25xxx.PageSize)
    NumByteToWrite_up_to_PageSize = w25xxx.PageSize-OffsetInByte;
#if (_W25XXX_DEBUG==1)
  printf("w25xxx WritePage:%d, Offset:%d ,Writes %d Bytes, begin...\r\n",Page_Address,OffsetInByte,NumByteToWrite_up_to_PageSize);
  W25xxx_Delay(100);
  uint32_t	StartTime=HAL_GetTick();
#endif	
  W25xxx_WaitForWriteEnd();
  W25xxx_WriteEnable();
  HAL_GPIO_WritePin(SPI5_GPIO_NSS_GPIO_Port,SPI5_GPIO_NSS_Pin,GPIO_PIN_RESET);
  W25xxx_Spi(0x02);
  Page_Address = (Page_Address*w25xxx.PageSize)+OffsetInByte;	
  if(w25xxx.ID>=W25X80)
    W25xxx_Spi((Page_Address & 0xFF000000) >> 24);
  W25xxx_Spi((Page_Address & 0xFF0000) >> 16);
  W25xxx_Spi((Page_Address & 0xFF00) >> 8);
  W25xxx_Spi(Page_Address&0xFF);
  HAL_SPI_Transmit(&_W25XXX_SPI,pBuffer,NumByteToWrite_up_to_PageSize,100);	
  HAL_GPIO_WritePin(SPI5_GPIO_NSS_GPIO_Port,SPI5_GPIO_NSS_Pin,GPIO_PIN_SET);
  W25xxx_WaitForWriteEnd();
#if (_W25XXX_DEBUG==1)
  StartTime = HAL_GetTick()-StartTime; 
  for(uint32_t i=0;i<NumByteToWrite_up_to_PageSize ; i++)
  {
    if((i%8==0)&&(i>2))
    {
      printf("\r\n");
      W25xxx_Delay(10);			
    }
    printf("0x%02X,",pBuffer[i]);		
  }	
  printf("\r\n");
  printf("w25xxx WritePage done after %d ms\r\n",StartTime);
  W25xxx_Delay(100);
#endif	
  W25xxx_Delay(1);
  w25xxx.Lock=0;
}
//###################################################################################################################
void 	W25xxx_WriteSector(uint8_t *pBuffer	,uint32_t Sector_Address,uint32_t OffsetInByte	,uint32_t NumByteToWrite_up_to_SectorSize)
{
  if((NumByteToWrite_up_to_SectorSize>w25xxx.SectorSize)||(NumByteToWrite_up_to_SectorSize==0))
    NumByteToWrite_up_to_SectorSize=w25xxx.SectorSize;
#if (_W25XXX_DEBUG==1)
  printf("+++w25xxx WriteSector:%d, Offset:%d ,Write %d Bytes, begin...\r\n",Sector_Address,OffsetInByte,NumByteToWrite_up_to_SectorSize);
  W25xxx_Delay(100);
#endif	
  if(OffsetInByte>=w25xxx.SectorSize)
  {
#if (_W25XXX_DEBUG==1)
    printf("---w25xxx WriteSector Faild!\r\n");
    W25xxx_Delay(100);
#endif	
    return;
  }	
  uint32_t	StartPage;
  int32_t		BytesToWrite;
  uint32_t	LocalOffset;	
  if((OffsetInByte+NumByteToWrite_up_to_SectorSize) > w25xxx.SectorSize)
    BytesToWrite = w25xxx.SectorSize-OffsetInByte;
  else
    BytesToWrite = NumByteToWrite_up_to_SectorSize;	
  StartPage = W25xxx_SectorToPage(Sector_Address)+(OffsetInByte/w25xxx.PageSize);
  LocalOffset = OffsetInByte%w25xxx.PageSize;	
  do
  {		
    W25xxx_WritePage(pBuffer,StartPage,LocalOffset,BytesToWrite);
    StartPage++;
    BytesToWrite-=w25xxx.PageSize-LocalOffset;
    pBuffer += w25xxx.PageSize - LocalOffset;
    LocalOffset=0;
  }while(BytesToWrite>0);		
#if (_W25XXX_DEBUG==1)
  printf("---w25xxx WriteSector Done\r\n");
  W25xxx_Delay(100);
#endif	
}
//###################################################################################################################
void 	W25xxx_WriteBlock	(uint8_t* pBuffer ,uint32_t Block_Address	,uint32_t OffsetInByte	,uint32_t	NumByteToWrite_up_to_BlockSize)
{
  if((NumByteToWrite_up_to_BlockSize>w25xxx.BlockSize)||(NumByteToWrite_up_to_BlockSize==0))
    NumByteToWrite_up_to_BlockSize=w25xxx.BlockSize;
#if (_W25XXX_DEBUG==1)
  printf("+++w25xxx WriteBlock:%d, Offset:%d ,Write %d Bytes, begin...\r\n",Block_Address,OffsetInByte,NumByteToWrite_up_to_BlockSize);
  W25xxx_Delay(100);
#endif	
  if(OffsetInByte>=w25xxx.BlockSize)
  {
#if (_W25XXX_DEBUG==1)
    printf("---w25xxx WriteBlock Faild!\r\n");
    W25xxx_Delay(100);
#endif	
    return;
  }	
  uint32_t	StartPage;
  int32_t		BytesToWrite;
  uint32_t	LocalOffset;	
  if((OffsetInByte+NumByteToWrite_up_to_BlockSize) > w25xxx.BlockSize)
    BytesToWrite = w25xxx.BlockSize-OffsetInByte;
  else
    BytesToWrite = NumByteToWrite_up_to_BlockSize;	
  StartPage = W25xxx_BlockToPage(Block_Address)+(OffsetInByte/w25xxx.PageSize);
  LocalOffset = OffsetInByte%w25xxx.PageSize;	
  do
  {		
    W25xxx_WritePage(pBuffer,StartPage,LocalOffset,BytesToWrite);
    StartPage++;
    BytesToWrite-=w25xxx.PageSize-LocalOffset;
    pBuffer += w25xxx.PageSize - LocalOffset;
    LocalOffset=0;
  }while(BytesToWrite>0);		
#if (_W25XXX_DEBUG==1)
  printf("---w25xxx WriteBlock Done\r\n");
  W25xxx_Delay(100);
#endif	
}
//###################################################################################################################
void 	W25xxx_ReadByte(uint8_t *pBuffer,uint32_t Bytes_Address)
{
  while(w25xxx.Lock==1)
    W25xxx_Delay(1);
  w25xxx.Lock=1;
#if (_W25XXX_DEBUG==1)
  uint32_t	StartTime=HAL_GetTick();
  printf("w25xxx ReadByte at address %d begin...\r\n",Bytes_Address);
#endif
  HAL_GPIO_WritePin(SPI5_GPIO_NSS_GPIO_Port,SPI5_GPIO_NSS_Pin,GPIO_PIN_RESET);
  W25xxx_Spi(0x0B);
  if(w25xxx.ID>=W25X80)
    W25xxx_Spi((Bytes_Address & 0xFF000000) >> 24);
  W25xxx_Spi((Bytes_Address & 0xFF0000) >> 16);
  W25xxx_Spi((Bytes_Address& 0xFF00) >> 8);
  W25xxx_Spi(Bytes_Address & 0xFF);
  W25xxx_Spi(0);
  *pBuffer = W25xxx_Spi(W25XXX_DUMMY_BYTE);
  HAL_GPIO_WritePin(SPI5_GPIO_NSS_GPIO_Port,SPI5_GPIO_NSS_Pin,GPIO_PIN_SET);	
#if (_W25XXX_DEBUG==1)
  printf("w25xxx ReadByte 0x%02X done after %d ms\r\n",*pBuffer,HAL_GetTick()-StartTime);
#endif
  w25xxx.Lock=0;
}
//###################################################################################################################
void W25xxx_ReadBytes(uint8_t* pBuffer, uint32_t ReadAddr, uint32_t NumByteToRead)
{
  while(w25xxx.Lock==1)
    W25xxx_Delay(1);
  w25xxx.Lock=1;
#if (_W25XXX_DEBUG==1)
  uint32_t	StartTime=HAL_GetTick();
  printf("w25xxx ReadBytes at Address:%d, %d Bytes  begin...\r\n",ReadAddr,NumByteToRead);
#endif	
  HAL_GPIO_WritePin(SPI5_GPIO_NSS_GPIO_Port,SPI5_GPIO_NSS_Pin,GPIO_PIN_RESET);
  W25xxx_Spi(0x0B);
  if(w25xxx.ID>=W25X80)
    W25xxx_Spi((ReadAddr & 0xFF000000) >> 24);
  W25xxx_Spi((ReadAddr & 0xFF0000) >> 16);
  W25xxx_Spi((ReadAddr& 0xFF00) >> 8);
  W25xxx_Spi(ReadAddr & 0xFF);
  W25xxx_Spi(0);
  HAL_SPI_Receive(&_W25XXX_SPI,pBuffer,NumByteToRead,2000);	
  HAL_GPIO_WritePin(SPI5_GPIO_NSS_GPIO_Port,SPI5_GPIO_NSS_Pin,GPIO_PIN_SET);
#if (_W25XXX_DEBUG==1)
  StartTime = HAL_GetTick()-StartTime; 
  for(uint32_t i=0;i<NumByteToRead ; i++)
  {
    if((i%8==0)&&(i>2))
    {
      printf("\r\n");
      W25xxx_Delay(10);
    }
    printf("0x%02X,",pBuffer[i]);		
  }
  printf("\r\n");
  printf("w25xxx ReadBytes done after %d ms\r\n",StartTime);
  W25xxx_Delay(100);
#endif	
  W25xxx_Delay(1);
  w25xxx.Lock=0;
}
//###################################################################################################################
void 	W25xxx_ReadPage(uint8_t *pBuffer,uint32_t Page_Address,uint32_t OffsetInByte,uint32_t NumByteToRead_up_to_PageSize)
{
  while(w25xxx.Lock==1)
    W25xxx_Delay(1);
  w25xxx.Lock=1;
  if((NumByteToRead_up_to_PageSize>w25xxx.PageSize)||(NumByteToRead_up_to_PageSize==0))
    NumByteToRead_up_to_PageSize=w25xxx.PageSize;
  if((OffsetInByte+NumByteToRead_up_to_PageSize) > w25xxx.PageSize)
    NumByteToRead_up_to_PageSize = w25xxx.PageSize-OffsetInByte;
#if (_W25XXX_DEBUG==1)
  printf("w25xxx ReadPage:%d, Offset:%d ,Read %d Bytes, begin...\r\n",Page_Address,OffsetInByte,NumByteToRead_up_to_PageSize);
  W25xxx_Delay(100);
  uint32_t	StartTime=HAL_GetTick();
#endif	
  Page_Address = Page_Address*w25xxx.PageSize+OffsetInByte;
  HAL_GPIO_WritePin(SPI5_GPIO_NSS_GPIO_Port,SPI5_GPIO_NSS_Pin,GPIO_PIN_RESET);
  W25xxx_Spi(0x0B);
  if(w25xxx.ID>=W25X80)
    W25xxx_Spi((Page_Address & 0xFF000000) >> 24);
  W25xxx_Spi((Page_Address & 0xFF0000) >> 16);
  W25xxx_Spi((Page_Address& 0xFF00) >> 8);
  W25xxx_Spi(Page_Address & 0xFF);
  W25xxx_Spi(0);
  HAL_SPI_Receive(&_W25XXX_SPI,pBuffer,NumByteToRead_up_to_PageSize,100);	
  HAL_GPIO_WritePin(SPI5_GPIO_NSS_GPIO_Port,SPI5_GPIO_NSS_Pin,GPIO_PIN_SET);
#if (_W25XXX_DEBUG==1)
  StartTime = HAL_GetTick()-StartTime; 
  for(uint32_t i=0;i<NumByteToRead_up_to_PageSize ; i++)
  {
    if((i%8==0)&&(i>2))
    {
      printf("\r\n");
      W25xxx_Delay(10);
    }
    printf("0x%02X,",pBuffer[i]);		
  }	
  printf("\r\n");
  printf("w25xxx ReadPage done after %d ms\r\n",StartTime);
  W25xxx_Delay(100);
#endif	
  W25xxx_Delay(1);
  w25xxx.Lock=0;
}
//###################################################################################################################
void 	W25xxx_ReadSector(uint8_t *pBuffer,uint32_t Sector_Address,uint32_t OffsetInByte,uint32_t NumByteToRead_up_to_SectorSize)
{	
  if((NumByteToRead_up_to_SectorSize>w25xxx.SectorSize)||(NumByteToRead_up_to_SectorSize==0))
    NumByteToRead_up_to_SectorSize=w25xxx.SectorSize;
#if (_W25XXX_DEBUG==1)
  printf("+++w25xxx ReadSector:%d, Offset:%d ,Read %d Bytes, begin...\r\n",Sector_Address,OffsetInByte,NumByteToRead_up_to_SectorSize);
  W25xxx_Delay(100);
#endif	
  if(OffsetInByte>=w25xxx.SectorSize)
  {
#if (_W25XXX_DEBUG==1)
    printf("---w25xxx ReadSector Faild!\r\n");
    W25xxx_Delay(100);
#endif	
    return;
  }	
  uint32_t	StartPage;
  int32_t		BytesToRead;
  uint32_t	LocalOffset;	
  if((OffsetInByte+NumByteToRead_up_to_SectorSize) > w25xxx.SectorSize)
    BytesToRead = w25xxx.SectorSize-OffsetInByte;
  else
    BytesToRead = NumByteToRead_up_to_SectorSize;	
  StartPage = W25xxx_SectorToPage(Sector_Address)+(OffsetInByte/w25xxx.PageSize);
  LocalOffset = OffsetInByte%w25xxx.PageSize;	
  do
  {		
    W25xxx_ReadPage(pBuffer,StartPage,LocalOffset,BytesToRead);
    StartPage++;
    BytesToRead-=w25xxx.PageSize-LocalOffset;
    pBuffer += w25xxx.PageSize - LocalOffset;
    LocalOffset=0;
  }while(BytesToRead>0);		
#if (_W25XXX_DEBUG==1)
  printf("---w25xxx ReadSector Done\r\n");
  W25xxx_Delay(100);
#endif	
}
//###################################################################################################################
void 	W25xxx_ReadBlock(uint8_t* pBuffer,uint32_t Block_Address,uint32_t OffsetInByte,uint32_t	NumByteToRead_up_to_BlockSize)
{
  if((NumByteToRead_up_to_BlockSize>w25xxx.BlockSize)||(NumByteToRead_up_to_BlockSize==0))
    NumByteToRead_up_to_BlockSize=w25xxx.BlockSize;
#if (_W25XXX_DEBUG==1)
  printf("+++w25xxx ReadBlock:%d, Offset:%d ,Read %d Bytes, begin...\r\n",Block_Address,OffsetInByte,NumByteToRead_up_to_BlockSize);
  W25xxx_Delay(100);
#endif	
  if(OffsetInByte>=w25xxx.BlockSize)
  {
#if (_W25XXX_DEBUG==1)
    printf("w25xxx ReadBlock Faild!\r\n");
    W25xxx_Delay(100);
#endif	
    return;
  }	
  uint32_t	StartPage;
  int32_t		BytesToRead;
  uint32_t	LocalOffset;	
  if((OffsetInByte+NumByteToRead_up_to_BlockSize) > w25xxx.BlockSize)
    BytesToRead = w25xxx.BlockSize-OffsetInByte;
  else
    BytesToRead = NumByteToRead_up_to_BlockSize;	
  StartPage = W25xxx_BlockToPage(Block_Address)+(OffsetInByte/w25xxx.PageSize);
  LocalOffset = OffsetInByte%w25xxx.PageSize;	
  do
  {		
    W25xxx_ReadPage(pBuffer,StartPage,LocalOffset,BytesToRead);
    StartPage++;
    BytesToRead-=w25xxx.PageSize-LocalOffset;
    pBuffer += w25xxx.PageSize - LocalOffset;
    LocalOffset=0;
  }while(BytesToRead>0);		
#if (_W25XXX_DEBUG==1)
  printf("---w25xxx ReadBlock Done\r\n");
  W25xxx_Delay(100);
#endif	
}
//###################################################################################################################

