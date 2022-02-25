/**
  ******************************************************************************
  * @file    CLT01.c
  * @brief   All the function of the CLT01
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */


#include "CLT01.h"

#include "STEVALPLC.h"

/** Function pointer to error handler call back */
//static void (*pErrorHandlerCallback)(uint16_t);
/** Function pointer to fault interrupt call back */
//static void (*pFaultInterruptCallback)(void);
/** ISR flag : set when code an ISR is running */
/*ST_PLC_ErrorCode Clt01_38sq7_ReadByte(uint8_t Module_ID , uint8_t* Data);
extern uint16_t STEvalPlcUtilSetBit(uint16_t tmp_data, uint8_t pos);*/
extern SPI_HandleTypeDef hspi2;
//extern  void  SPIx_Error(void);
extern  uint8_t GlobalPLCWatch2;

/***************************************
 *@brief  SPI Write a byte to device 16 bit data
 *@param  DataIn: pointer to the value to be read
 *@param  DataOut: pointer to the value to be written
 *@param  DataLegnth: length of data
 ***************************************/
void SPIx_WriteReadData(const uint8_t *DataIn, uint8_t *DataOut, uint16_t DataLegnth)
{
	HAL_StatusTypeDef status = HAL_OK;
	status = HAL_SPI_TransmitReceive(&hspi2, (uint8_t*) DataIn, DataOut, DataLegnth, SpixTimeout);
	if (status != HAL_OK)
	{
	}
}

 ST_PLC_ErrorCode Get_Input_WriteByte(uint8_t *Data)
{
	static uint8_t tmp2;
	uint8_t Global_Q1_Status;
	*Data = 0x75; /*was 0xff ndg*/
	/*Send the byte*/

	
	HAL_GPIO_WritePin(DO_SPI2_NSS_I1_GPIO_Port, DO_SPI2_NSS_I1_Pin, GPIO_PIN_RESET);
	SPIx_WriteReadData((uint8_t*) &Global_Q1_Status, (uint8_t*) &tmp2, GPIO_PIN_SET);
	HAL_GPIO_WritePin(DO_SPI2_NSS_I1_GPIO_Port, DO_SPI2_NSS_I1_Pin, GPIO_PIN_SET);
	
	*Data = tmp2;
	GlobalPLCWatch2 = tmp2;
	return tmp2;
}







/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
