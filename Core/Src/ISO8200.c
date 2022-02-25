/**
  ******************************************************************************
  * @file    ISO8200.c
  * @brief   All functions related to ISO8200
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

#include "main.h"

#include "STEVALPLC.h"

#include "ISO8200.h"


extern SPI_HandleTypeDef hspi2;
extern uint8_t ST_PLC_UtilsetBitLeft(uint8_t tmp_data, uint8_t pos,bool bit);

typedef struct {
 GPIO_TypeDef* gpio;
 uint16_t pin;
} IsoFault_port_pin_t;

IsoFault_port_pin_t STPLC_IsoFault_GPIOInterfaces[] = {

  {DI_Q0_PGOOD_GPIO_Port, DI_Q0_PGOOD_Pin },
  {DI_Q0_FLT_GPIO_Port,   DI_Q0_FLT_Pin }
};

/* Includes ------------------------------------------------------------------*/

    
/* Private constants  ---------------------------------------------------------*/
    
/* Private constants ---------------------------------------------------------*/    


/****************************************************//**
*@brief this function checks the fault in the ISO module
*
*@param *Isofault: pointer to the data to be read
*@param Module_ID:Calls the ISO module
*
*@retval returns ST_PLC_ErrorCode
***********************************************************/
ST_PLC_ErrorCode ST_PLC_Isofaultdetect(uint8_t ModID, uint8_t *Faultbits)
{
        bool Iso_faultpin_status[2];
        
         //uint16_t* IsoDatacheck;
	uint8_t Isofaultcheck=0x00;
        uint8_t fault = ST_PLC_ERROR_HW_FLT;	
             for(int i = 0 ; i<2 ;i++)
      {
      Iso_faultpin_status[i]=HAL_GPIO_ReadPin(STPLC_IsoFault_GPIOInterfaces[i].gpio, STPLC_IsoFault_GPIOInterfaces[i].pin); //read the fault pin 
      if(Iso_faultpin_status [i] ==0)
      {
      Isofaultcheck= ST_PLC_UtilsetBitLeft(Isofaultcheck, i, Iso_faultpin_status[i]);
           
      }
      Isofaultcheck =0;
      *Faultbits = Isofaultcheck;
     }
     return (ST_PLC_NO_ERROR);      
      //*IPSfaultStatus= faultcheck;
      //*Datacheck= faultcheck;
      
}
/******************************************************//**
 * @brief  Write and receive a byte via SPI
 * @param[in] pByteToTransmit pointer to the byte to transmit
 * @param[in] pReceivedByte pointer to the received byte
 * @retval ST_PLC_ErrorCode
 **********************************************************/
ST_PLC_ErrorCode Iso8200aq_WriteBytes(uint8_t Module_ID , uint8_t *pByteToTransmit)    

{
  //static uint8_t data2; 
   extern uint8_t DO_Module_status[];
   
   DO_Module_status[0]= *pByteToTransmit;
   HAL_GPIO_WritePin( SPI2_NSS_Q0_GPIO_Port, SPI2_NSS_Q0_Pin, GPIO_PIN_RESET);
   HAL_SPI_Transmit(&hspi2,pByteToTransmit,1,10);
   HAL_GPIO_WritePin( SPI2_NSS_Q0_GPIO_Port, SPI2_NSS_Q0_Pin, GPIO_PIN_SET);

   return (ST_PLC_NO_ERROR);
}
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/