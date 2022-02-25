/**
  ******************************************************************************
  * @file    IPS4260.c
  * @brief   All the function of IPS4260
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

#include "IPS4260.h"

#include "STEVALPLC.h"

extern bool tmp_id[4];   
extern uint8_t GlobalPLCWatch1, GlobalPLCWatch2;

extern uint8_t ST_PLC_UtilsetBitLeft(uint8_t tmp_data, uint8_t pos,bool bit);

extern uint8_t DO_Module_status[];
//static uint16_t* value; 

 GPIO_port_pin_t STPLC_Module_IPS4260L_GPIOInterfaces[] = {

        
        {DO_Q1_0_GPIO_Port, DO_Q1_0_Pin	},
	{DO_Q1_1_GPIO_Port, DO_Q1_1_Pin	},
	{DO_Q1_2_GPIO_Port, DO_Q1_2_Pin	},
	{DO_Q1_3_GPIO_Port, DO_Q1_3_Pin	}
};

GPIO_port_pin_t STPLC_Module_IPS4260LFault_GPIOInterfaces[]={
        {DI_FLT_Q1_0_GPIO_Port, DI_FLT_Q1_0_Pin	},
	{DI_FLT_Q1_1_GPIO_Port, DI_FLT_Q1_1_Pin	},
	{DI_FLT_Q1_2_GPIO_Port, DI_FLT_Q1_2_Pin	},
	{DI_FLT_Q1_3_GPIO_Port, DI_FLT_Q1_3_Pin	},
        {DI_FLT_Q1_FLT_GPIO_Port, DI_FLT_Q1_FLT_Pin },
        {DI_FLT_Q1_OL_GPIO_Port, DI_FLT_Q1_OL_Pin }
        
        
};
/**********************************************************
  * @brief  array for the pin and the prt of the the selection of IPS Outpout
  * @param  tmp_IPS_arr_port: Port array of IPS
  * @param  tmp_IPS_arr_pin: Pin array of IPS
  
  */

ST_PLC_ErrorCode ST_PLC_Ipsfaultdetect(uint8_t Module_ID, uint8_t *IPSfaultStatus)
{
        bool DI_faultpin_status[6];
        
	uint8_t faultcheck=0x00 ;
	        
        for(int i = 0 ; i<6 ;i++)
      {
      DI_faultpin_status[i]=HAL_GPIO_ReadPin(STPLC_Module_IPS4260LFault_GPIOInterfaces[i].gpio, STPLC_Module_IPS4260LFault_GPIOInterfaces[i].pin); //read the fault pin 
      if(DI_faultpin_status [i] ==0)
      {
      faultcheck= ST_PLC_UtilsetBitLeft(faultcheck, i, 1);
           
      }
      
      *IPSfaultStatus = faultcheck;
      }
     if (faultcheck == 0)
          { return(ST_PLC_NO_ERROR);
          }
     else 
          {return (ST_PLC_ERROR_HW_FLT);
          }

}


/**********************************************************
  * @brief  IPS  data write
  * @param  tmp_IPS_arr_port: Port array of IPS
  * @param  tmp_IPS_arr_pin: Pin array of IPS
  
  */

void Ips42601_WriteNibble(uint8_t *Data)
{
  
uint8_t datanibble;
bool ChannelState;
datanibble = *Data;
  
        	
  for(int i = 0 ; i< 4 ;i++)
  {
      
     ChannelState= ((datanibble &(1<<i)?1 : 0));
     extern GPIO_port_pin_t STPLC_Module_IPS4260L_GPIOInterfaces[];   
     HAL_GPIO_WritePin(STPLC_Module_IPS4260L_GPIOInterfaces[i].gpio,STPLC_Module_IPS4260L_GPIOInterfaces[i].pin, (GPIO_PinState)ChannelState);
       /*void HAL_GPIO_WritePin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, GPIO_PinState PinState)*/
      //HAL_GPIO_WritePin(STPLC_Module_IPS4260L_Port[i], STPLC_Module_IPS4260L_Pin[i], (GPIO_PinState)ChannelState);
     
      
  } 
}

/**********************************************************
*@brief  IPS  data single channel write
*@param  tmp_IPS_arr_port: Port array of IPS
*@param  tmp_IPS_arr_pin: Pin array of IPS

*/
void IPS4260_SetSingleChannel(uint8_t Channel_Index)
{
  extern GPIO_port_pin_t STPLC_Module_IPS4260L_GPIOInterfaces[4];

	HAL_GPIO_WritePin(STPLC_Module_IPS4260L_GPIOInterfaces[Channel_Index].gpio,STPLC_Module_IPS4260L_GPIOInterfaces[Channel_Index].pin, GPIO_PIN_SET); 

}

/*****************************************************//****
*@brief IPS is updates with the data
*
*@param  Module_ID and Data
*
*@retval None
**********************************************************/
void IPS4260_ClrSingleChannel(uint8_t Channel_Index)
  {  extern GPIO_port_pin_t STPLC_Module_IPS4260L_GPIOInterfaces[4];
     DO_Module_status[Channel_Index] &= ((~1) << Channel_Index);
     //HAL_GPIO_WritePin(STPLC_Module_IPS4260L_Port[Channel_Index], STPLC_Module_IPS4260L_Pin[Channel_Index], GPIO_PIN_RESET);
     HAL_GPIO_WritePin(STPLC_Module_IPS4260L_GPIOInterfaces[Channel_Index].gpio,STPLC_Module_IPS4260L_GPIOInterfaces[Channel_Index].pin, GPIO_PIN_RESET);         
   }
  

void Ips4260l_WriteNibble(uint8_t Data)
 {
        uint8_t datanibble;
		bool ChannelState;
	    datanibble= Data;
	for (int i=0; i<4; i++)
	{       
 
     ChannelState= ((datanibble &(1<<i)));

                extern GPIO_port_pin_t STPLC_Module_IPS4260L_GPIOInterfaces[4];
                if (ChannelState == 1)
                {
                  IPS4260_SetSingleChannel(i );
		}
                else 
                {
                  IPS4260_ClrSingleChannel(i );
                }
				
	}
}

