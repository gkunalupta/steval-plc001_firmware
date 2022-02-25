#ifndef _W25XXX_H
#define _W25XXX_H



#ifdef __cplusplus
 extern "C" {
#endif

#include <stdbool.h>
#include "main.h"

   
extern SPI_HandleTypeDef hspi5;


typedef enum
{
	W25X10=1,
	W25X20,
	W25X40,
	W25X80,
	
}W25XXX_ID_t;

typedef struct
{
	W25XXX_ID_t	ID;
	uint8_t		UniqID[8];
	uint16_t	PageSize;
	uint32_t	PageCount;
	uint32_t	SectorSize;
	uint32_t	SectorCount;
	uint32_t	BlockSize;
	uint32_t	BlockCount;
	uint32_t	CapacityInKiloByte;
	uint8_t		StatusRegister1;
	uint8_t		StatusRegister2;
	uint8_t		StatusRegister3;	
	uint8_t		Lock;
	
}w25qxx_t;

extern w25qxx_t	w25qxx;
//############################################################################
// in Page,Sector and block read/write functions, can put 0 to read maximum bytes 
//############################################################################
bool		W25xxx_Init(void);

void		W25xxx_EraseChip(void);
void 		W25xxx_EraseSector(uint32_t SectorAddr);
void 		W25xxx_EraseBlock(uint32_t BlockAddr);

uint32_t	W25xxx_PageToSector(uint32_t PageAddress);
uint32_t	W25xxx_PageToBlock(uint32_t PageAddress);
uint32_t	W25xxx_SectorToBlock(uint32_t SectorAddress);
uint32_t	W25xxx_SectorToPage(uint32_t SectorAddress);
uint32_t	W25xxx_BlockToPage(uint32_t BlockAddress);

bool 		W25xxx_IsEmptyPage(uint32_t Page_Address, uint32_t OffsetInByte, uint32_t NumByteToCheck_up_to_PageSize);
bool 		W25xxx_IsEmptySector(uint32_t Sector_Address, uint32_t OffsetInByte, uint32_t NumByteToCheck_up_to_SectorSize);
bool 		W25xxx_IsEmptyBlock(uint32_t Block_Address, uint32_t OffsetInByte, uint32_t NumByteToCheck_up_to_BlockSize);

void 		W25xxx_WriteByte(uint8_t pBuffer, uint32_t Bytes_Address);
void 		W25xxx_WritePage(uint8_t *pBuffer, uint32_t Page_Address, uint32_t OffsetInByte, uint32_t NumByteToWrite_up_to_PageSize);
void 		W25xxx_WriteSector(uint8_t *pBuffer, uint32_t Sector_Address, uint32_t OffsetInByte, uint32_t NumByteToWrite_up_to_SectorSize);
void 		W25xxx_WriteBlock(uint8_t* pBuffer, uint32_t Block_Address, uint32_t OffsetInByte, uint32_t NumByteToWrite_up_to_BlockSize);

void 		W25xxx_ReadByte(uint8_t *pBuffer, uint32_t Bytes_Address);
void 		W25xxx_ReadBytes(uint8_t *pBuffer, uint32_t ReadAddr, uint32_t NumByteToRead);
void 		W25xxx_ReadPage(uint8_t *pBuffer, uint32_t Page_Address,uint32_t OffsetInByte, uint32_t NumByteToRead_up_to_PageSize);
void 		W25xxx_ReadSector(uint8_t *pBuffer, uint32_t Sector_Address,uint32_t OffsetInByte, uint32_t NumByteToRead_up_to_SectorSize);
void 		W25xxx_ReadBlock(uint8_t* pBuffer, uint32_t Block_Address, uint32_t OffsetInByte, uint32_t NumByteToRead_up_to_BlockSize);
//############################################################################
#ifdef __cplusplus
}
#endif

#endif

