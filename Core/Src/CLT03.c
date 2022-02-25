/**
  ******************************************************************************
  * @file    CLT03.c
  * @brief   All function related to CLT03
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

#include "CLT03.h"

typedef struct {
 GPIO_TypeDef* gpio;
 uint16_t pin;
} CLT03_port_pin_t;

CLT03_port_pin_t STPLC_Module_CLT03_GPIOInterfaces[] = {
	
        {DI_I1_0_GPIO_Port, DI_I1_0_Pin	},
	{DI_I1_1_GPIO_Port, DI_I1_1_Pin	},
	{DI_I1_2_GPIO_Port, DI_I1_2_Pin	},
	{DI_I1_3_GPIO_Port, DI_I1_3_Pin	}
};

 uint8_t Clt03In(void);

/** Function pointer to error handler call back */
/*static void (*pCLT03ErrorHandlerCallback)(uint16_t);*/

extern uint8_t ST_PLC_UtilsetBitLeft(uint8_t tmp_data, uint8_t pos,bool bit);
extern struct Module_Status IO_ModuleStatus_Fault[5];




/**********************************************************
  *@brief  Clt03 Moduile data read
  *
  *@param  pointer to Data to be fetched into
  *
   **********************************************************/

ST_PLC_ErrorCode Clt03_ReadNibble(uint8_t *Data)
{
	uint8_t tmp_03= 0;
        extern uint8_t GlobalPLCWatch2;

	//static bool CLT03[4];
        bool Cltread=0;
	for (int i = 0; i < 4; i++)
	{
                
		Cltread =HAL_GPIO_ReadPin(STPLC_Module_CLT03_GPIOInterfaces[i].gpio, STPLC_Module_CLT03_GPIOInterfaces[i].pin);
		 tmp_03 = ST_PLC_UtilsetBitLeft(tmp_03, i, Cltread);
                 GlobalPLCWatch2 = tmp_03;
	}
       	*Data = tmp_03;
	return tmp_03;
}

bool Clt03_ReadChannel(uint8_t channelindex)
{
	bool Cltreadchl=0;
	            
	Cltreadchl =HAL_GPIO_ReadPin(STPLC_Module_CLT03_GPIOInterfaces[channelindex].gpio, STPLC_Module_CLT03_GPIOInterfaces[channelindex].pin);
	
       	return Cltreadchl;
}


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/