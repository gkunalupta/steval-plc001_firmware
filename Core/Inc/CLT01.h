#ifndef CLT01_H
#define CLT01_H
#include"main.h"
/*

 Current FW major version 
#define CLT01_FW_MAJOR_VERSION (uint8_t)(1)
 Current FW minor version 
#define CLT01_FW_MINOR_VERSION (uint8_t)(0)
 Current FW patch version 
#define CLT01_FW_PATCH_VERSION (uint8_t)(0)
 Current FW version 
#define CLT01_FW_VERSION       (uint32_t)((CLT01_FW_MAJOR_VERSION<<16)|\
                                              (CLT01_FW_MINOR_VERSION<<8)|\
                                              (CLT01_FW_PATCH_VERSION))*/
#define SpixTimeout 1000
/** CLT-01 Driver Parameters Structure Type */
/*typedef struct {
                           
  SPI frequency in Hz 
  uint32_t spiFreq;                             
  Device parameters array 
  deviceParams_t device[MAX_NUMBER_OF_DEVICES]; 
}CLT01_driverParams_t; 
*/
//******user code for the function declaration*****


uint8_t*          CLT01_38S_GetInpData(void);
static uint8_t    CLT01_38S_GetUvaStatus(uint8_t*);
static uint8_t    CLT01_38S_UvaStatus(uint8_t*);
static uint16_t   InputData(uint16_t* a,uint16_t* b);
static void       GetStatus(uint16_t CLT03_16Bit_Data);
//static void       WriteReadData8Bit(const uint8_t*, uint8_t*, uint16_t);
void SPIx_WriteReadData(const uint8_t *DataIn, uint8_t *DataOut, uint16_t DataLegnth);

//*****private variable declaration*****
//static uint8_t* Input_Data1 = 0x00;
//static uint8_t* Input_Data2 = 0x00;

#endif