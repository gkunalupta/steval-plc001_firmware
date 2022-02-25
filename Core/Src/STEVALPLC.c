/**
  ******************************************************************************
  * @file    STEVALPLC.c
  * @brief   All the function of the board.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.



  */

#include "main.h"
#include "STEVALPLC.h"
#include "stm32f7xx_hal_spi.h"
#include "stm32f7xx_hal.h"
#include "stm32f7xx_it.h"
#include "IPS4260.h"    
    
extern uint8_t GlobalPLCWatchM1;
extern GPIO_TypeDef* tmp_IPS_arr_port[4] = { DO_Q1_0_GPIO_Port, DO_Q1_1_GPIO_Port, DO_Q1_2_GPIO_Port, DO_Q1_3_GPIO_Port};
extern uint16_t tmp_IPS_arr_pin[4] = { DO_Q1_0_Pin, DO_Q1_1_Pin, DO_Q1_2_Pin, DO_Q1_3_Pin};

uint8_t GlobalPLCWatch1, GlobalPLCWatch2;
extern ST_PLC_ErrorCode Iso8200aq_WriteBytes(uint8_t Module_ID , uint8_t *pByteToTransmit);
extern void Ips4260l_WriteNibble(uint8_t Data);

extern SPI_HandleTypeDef hspi2;
extern TIM_OC_InitTypeDef sConfigPWM;
extern TIM_HandleTypeDef htim2;

extern ST_PLC_ErrorCode Clt03_ReadNibble(uint8_t *Data);
extern  void  WriteReadData(const uint8_t *DataIn, uint8_t *DataOut, uint16_t DataLegnth);
extern ST_PLC_ErrorCode ST_PLC_Ipsfaultdetect(uint8_t Module_ID, uint8_t *IPSfaultStatus);
extern ST_PLC_ErrorCode Get_Input_WriteByte(uint8_t *Data);
extern ST_PLC_ErrorCode Clt03_ReadNibble(uint8_t *Data);
extern ST_PLC_ErrorCode ST_PLC_Isofaultdetect(uint8_t ModID, uint8_t *Faultbits);
extern bool Clt03_ReadChannel(uint8_t channelindex);

struct Module_Status IO_ModuleStatus_Fault[5]; /* Covers first Module IDs*/

/********************************/
/**
 *@brief  Writes a byte on the SD.
 *@param  Data: byte to send.
 *@retval returns ST_PLC_NO_ERROR
 ***********************************/


 ST_PLC_ErrorCode  ST_PLC_GetFaultPin_Status(uint32_t Channel_Index)
 {
    
        uint8_t Fault_sequence,ips_fault;
           
        union FaultUnion{
          
          uint32_t FaultDescription;
          struct{ 
              uint8_t faultposition;
              uint8_t ModuleID_fault;
                            
          }Fault;
        };
        union FaultUnion CurrentFaultChannel;

        CurrentFaultChannel.FaultDescription = Channel_Index;
        if(CurrentFaultChannel.Fault.ModuleID_fault == (uint8_t) Module_ID_ISO8200AQ)
        {
         Fault_sequence = CurrentFaultChannel.Fault.faultposition; 
        }
        else if (CurrentFaultChannel.Fault.ModuleID_fault == (uint8_t) Module_ID_IPS4260L)
        {
          for( ips_fault = 0; ips_fault<6; ips_fault++)
          {
            if( Fault_sequence != 1)
           Fault_sequence = CurrentFaultChannel.Fault.faultposition;
          }
        }
          return ST_PLC_ERROR_HW_FLT;
 }



/**********************************************
* @brief This function return the fault status in a particular module
* @param FaultModule and Data
* @retval ST_PLC_NO_ERROR
***********************************************/
 ST_PLC_ErrorCode ST_PLC_Update_ModuleFaultStatus(uint8_t FaultModule)
{
    uint8_t DIFault, FaultData=0xFF;
    switch (FaultModule)
	{
		case Module_ID_ISO8200AQ:
		  DIFault = ST_PLC_Isofaultdetect( FaultModule, &FaultData);
		/*  Data= DIFault;
			GlobalPLCWatch2 = *Data;
			GlobalPLCWatch2 = FaultData; */  /*210 temp*/
                  break;
		case Module_ID_IPS4260L:
                  DIFault = ST_PLC_Ipsfaultdetect( FaultModule, &FaultData);
			/* *Data = DIFault; */
                   break;
		
                default :
                  return (ST_PLC_ERROR_WrongParameter);
	}
        
    IO_ModuleStatus_Fault[FaultModule].Fault_status= FaultData;

    if (DIFault == ST_PLC_NO_ERROR)
          { return(ST_PLC_NO_ERROR);
          }
     else 
          {return (ST_PLC_ERROR_HW_FLT);
          }

}
/***************

*/

 void Fresh_Start(void)
   
 { uint8_t Localloop;
   for (Localloop =0; Localloop <5; Localloop++)
   {
    IO_ModuleStatus_Fault[Localloop].Fault_status = 0;
   }
    
  ST_PLC_Write_AllChannels_InModule((uint8_t) Module_ID_ISO8200AQ, 0x00);
  ST_PLC_Write_AllChannels_InModule((uint8_t) Module_ID_IPS4260L, 0x00);
  ST_PLC_Update_ModuleFaultStatus((uint8_t) Module_ID_IPS4260L );
   
 }
   
 /**************************************//**
 *@brief Inorder to initialize the entire board
 *@param GPIO State
 *@retval returns ST_PLC_NO_ERROR
 **********************************************************/
ST_PLC_ErrorCode ST_PLC_InitiatizeSTEval(uint32_t BoardNameVersion)
{
        uint8_t Localval;
        ST_PLC_Blink_One_DebugLed(DebugLed1,100,1); /*D37 -amber*/
	ISO_OutputEnable_Pin(GPIO_PIN_RESET);
	HAL_GPIO_WritePin(DO_SPI2_NSS_I1_GPIO_Port, DO_SPI2_NSS_I1_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(SPI2_NSS_Q0_GPIO_Port, SPI2_NSS_Q0_Pin, GPIO_PIN_SET);
        Fresh_Start();
        ST_PLC_Read_AllChannels_InModule(Module_ID_CLT01_38SQ7, &Localval);
        ST_PLC_Read_AllChannels_InModule(Module_ID_CLT03_2Q3_X_2, &Localval);
	
        /*ST_PLC_Set_HMI_Backlightintensity(BackLight_50);*/
        ST_PLC_Blink_One_DebugLed(DebugLed2,100,2); /*D36 -Blue*/
	return (ST_PLC_NO_ERROR);
}

/******************************************************//**
 * @brief Debug led blinking when fault is detected.
 * @param DebugLed: Calls the particular debug led.
 * @param delay 
 * @param repeat: The number of times  a led will blink.
 * @retval user will see the Led blinking.
 *********************************************************/

void  ST_PLC_Blink_One_DebugLed(uint8_t DebugLed, uint32_t delay, uint8_t repeat)
{
  uint8_t led1,led2,led3;
  switch (DebugLed)
  { 
    case DebugLed1:
        for(led1= 0; led1< repeat; led1++)
	{
         HAL_GPIO_WritePin(DebugLed1Port, DebugLed1Pin, GPIO_PIN_SET); /*ND@52 LED_ON  GPIO  GPIO_PIN_RESET  */
         HAL_Delay(delay);
         HAL_GPIO_WritePin(DebugLed1Port, DebugLed1Pin, GPIO_PIN_RESET);
         HAL_Delay(delay);
      }
        break;
    case DebugLed2:
        for(led2= 0; led2< repeat; led2++)
     {
		HAL_GPIO_WritePin(DebugLed2Port, DebugLed2Pin, GPIO_PIN_SET); /*ND@52 LED_ON  GPIO  GPIO_PIN_RESET  */
		HAL_Delay(delay);
		HAL_GPIO_WritePin(DebugLed2Port, DebugLed2Pin, GPIO_PIN_RESET);
        HAL_Delay(delay);
     }
       break;
   case DebugLed3:
        for(led3= 0; led3< repeat; led3++)
     {
        HAL_GPIO_WritePin(DebugLed3Port, DebugLed3Pin, GPIO_PIN_SET); /*ND@52 LED_ON  GPIO  GPIO_PIN_RESET  */
		HAL_Delay(delay);
        HAL_GPIO_WritePin(DebugLed3Port, DebugLed3Pin, GPIO_PIN_RESET);
        HAL_Delay(delay);
     }
       break;
  }
}
void  ST_PLC_Control_DebugLed(uint8_t DebugLed, uint8_t DebugState)
{
         uint8_t reversebit;
         switch (DebugState)
   { 
         case DebugLed_ON:
              switch(DebugLed)
              {
              case DebugLed1:
                {
                   HAL_GPIO_WritePin(DebugLed1Port, DebugLed1Pin, GPIO_PIN_SET);
                }
                break;
              case DebugLed2:
                {
                   HAL_GPIO_WritePin(DebugLed2Port, DebugLed2Pin, GPIO_PIN_SET);
                }
                break;
              case DebugLed3:
                {
                   HAL_GPIO_WritePin(DebugLed3Port, DebugLed3Pin, GPIO_PIN_SET);
                }
                break;
              }
        case DebugLed_OFF:
               switch(DebugLed)
              {
              case DebugLed1:
                {
		HAL_GPIO_WritePin(DebugLed1Port, DebugLed1Pin, GPIO_PIN_RESET);
	}
                break;
              case DebugLed2:
                {
                   HAL_GPIO_WritePin(DebugLed2Port, DebugLed2Pin, GPIO_PIN_RESET);
                }
                break;
              case DebugLed3:
                {
                   HAL_GPIO_WritePin(DebugLed3Port, DebugLed3Pin, GPIO_PIN_RESET);
                }
                break;
              }
        case DebugLed_REVERSE: 
              switch(DebugLed)
              {
              case DebugLed1:
                {
                  reversebit=  HAL_GPIO_ReadPin(DebugLed1Port,DebugLed1Pin);
                  if(reversebit ==1)
                  {
                    HAL_GPIO_WritePin(DebugLed1Port, DebugLed1Pin, GPIO_PIN_RESET);
                  }
                  else
                  {
                    HAL_GPIO_WritePin(DebugLed1Port, DebugLed1Pin, GPIO_PIN_SET);
                  }
                }
                break;
              case DebugLed2:
                {
                   reversebit=  HAL_GPIO_ReadPin(DebugLed2Port,DebugLed2Pin);
                  if(reversebit ==1)
                  {
                    HAL_GPIO_WritePin(DebugLed2Port, DebugLed2Pin, GPIO_PIN_RESET);
                  }
                  else
                  {
                    HAL_GPIO_WritePin(DebugLed2Port, DebugLed2Pin, GPIO_PIN_SET);
                  } 
                }
                break;
              case DebugLed3:
                {
                   reversebit=  HAL_GPIO_ReadPin(DebugLed3Port,DebugLed3Pin);
                  if(reversebit ==1)
                  {
                    HAL_GPIO_WritePin(DebugLed3Port, DebugLed3Pin, GPIO_PIN_RESET);
                  }
                  else
                  {
                    HAL_GPIO_WritePin(DebugLed3Port, DebugLed3Pin, GPIO_PIN_SET);
                  } 
                }
                break;
              }
        }
}


/******************************************************//**
 *@brief  Returns the clt01 data
 *@param  ModuleName &Indata to recieve module name and its input data
 *@retval returns ST_PLC_NO_ERROR
 *********************************************************/
ST_PLC_ErrorCode ST_PLC_Read_AllChannels_InModule(uint8_t ModuleName, uint8_t *InData)
{
	static uint8_t Inclt = 0x00, InmoduleData ;
	//static uint8_t *XYZ,tmp_11;

	switch (ModuleName)
	{
		case Module_ID_CLT01_38SQ7:
			/*temp1 = Channel1; */

			// Clt01_38sq7_ReadByte((uint8_t)Module_ID_CLT01_38SQ7, InData);
			Inclt = Get_Input_WriteByte(&InmoduleData);
			*InData = Inclt;
			GlobalPLCWatch2 = *InData;
			GlobalPLCWatch2 = InmoduleData;

			break;
		case Module_ID_CLT03_2Q3_X_2:

			Inclt = Clt03_ReadNibble(&InmoduleData);
			*InData = Inclt;
                    //     IO_ModuleStatus_Fault[Module_ID_CLT03_2Q3_X_2].IO_status=*InData ;
			break;
		default :
                  return (ST_PLC_ERROR_WrongParameter);
	}
        IO_ModuleStatus_Fault[ModuleName].IO_status=InmoduleData;
	return (ST_PLC_NO_ERROR);

}
/******************************************************//**
 *@brief Gets the data returned by clt01
 *
 *@param ModuleId and its input Data is recieved
 *
 * @param ModuleId and its input Data is recieved
 * 
 *@retval returns ST_PLC_NO_ERROR
 *********************************************************/
ST_PLC_ErrorCode ST_PLC_Write_AllChannels_InModule(uint8_t Module_ID, uint8_t Data)
{
	extern uint8_t GlobalPLCWatchM1;
        GlobalPLCWatchM1 = Data;
	switch (Module_ID)
	{
		
		case Module_ID_ISO8200AQ:
			ISO_OutputEnable_Pin(GPIO_PIN_SET);
			Iso8200aq_WriteBytes(Data, &Data);
			break;
		case Module_ID_IPS4260L:
			/*WriteIpsdata(&Data); */
//			Ips4260l_WriteNibble(&Data);  //BMP
                        Ips4260l_WriteNibble(Data);
	}
        IO_ModuleStatus_Fault[Module_ID].IO_status=Data;
	return (ST_PLC_NO_ERROR);

}



/******************************************************/
/**
 *@brief This will Enable the module.
 *
 *@param ModuleID  
 *
 *@retval returns ST_PLC_NO_ERROR
 *******************************************************/
ST_PLC_ErrorCode ST_PLC_Enable(uint8_t Module_ID)
{
	switch (Module_ID)
	{
		case Module_ID_ISO8200AQ:
			//  ISO_OutputEnable_Pin(GPIO_PIN_RESET);
			break;
	}

	return (ST_PLC_NO_ERROR);
}

/******************************************************/
/**
 *@brief This will make the module to the ON State.
 *
 *@param ModuleID 
 *
 *@retval returns ST_PLC_NO_ERROR
 *******************************************************/
ST_PLC_ErrorCode ST_PLC_Disable(uint8_t Module_ID)
{
	switch (Module_ID)
	{
		case Module_ID_ISO8200AQ:
			//  ISO_OutputEnable_Pin(GPIO_PIN_SET);
			break;
	}

	return (ST_PLC_NO_ERROR);
}


/******************************************************//**
 *@brief Set one digital output
 *
 *@param Channel_Name is used to call the particular channel of the module.
 *
 *@retval returns ST_PLC_NO_ERROR
 *********************************************************/

ST_PLC_ErrorCode ST_PLC_SetOne_DO(uint32_t Channel_Name)
{
	uint8_t bit_pattern, testbitpattern = 0x01, index_ips;
	extern uint8_t GlobalPLCWatchM1, DO_Module_status[];

	union ChannelDUnion
	{
		uint32_t ChannelDescriptor;
		struct
		{
			uint8_t bitposition;
			uint8_t Moduleid_8bit;
			uint8_t b3;
			uint8_t b4;	// or number them in reverse order
		}

		CDBytes;
	};
	union ChannelDUnion CurrentChannelDescriptor;
	CurrentChannelDescriptor.ChannelDescriptor = Channel_Name;
	
	bit_pattern = CurrentChannelDescriptor.CDBytes.bitposition; /* change this bit */
	if (CurrentChannelDescriptor.CDBytes.Moduleid_8bit == (uint8_t) Module_ID_ISO8200AQ) /*Module Q0 */
	{
		ISO_OutputEnable_Pin(GPIO_PIN_SET);
		bit_pattern |= DO_Module_status[0]; /* set this DO */
                IO_ModuleStatus_Fault[3].IO_status= bit_pattern;
                DO_Module_status[0] = bit_pattern;
		Iso8200aq_WriteBytes(bit_pattern, &bit_pattern);
	}
	else if (CurrentChannelDescriptor.CDBytes.Moduleid_8bit == (uint8_t) Module_ID_IPS4260L) /*Module Q1 */
	{
                IO_ModuleStatus_Fault[4].IO_status |= bit_pattern;
                DO_Module_status[1]=  IO_ModuleStatus_Fault[4].IO_status;
		for (index_ips = 0; index_ips < 5; index_ips++)
		{
			if (bit_pattern == testbitpattern)
                        break;
			testbitpattern <<= 1;
			GlobalPLCWatchM1 = testbitpattern;

		}

		GlobalPLCWatchM1 = index_ips;
		if (index_ips == 4) return (ST_PLC_ERROR_WrongParameter);
		GlobalPLCWatchM1 = index_ips;
		IPS4260_SetSingleChannel(index_ips);
	}
	else return (ST_PLC_ERROR_WrongParameter);

	return (ST_PLC_NO_ERROR);
}

/******************************************************//**
 *@brief Clear one digital output
 *
 *@param Channel_Name is used to call the particular channel of the Module.
 *
 *@retval returns ST_PLC_NO_ERROR
 *********************************************************/
ST_PLC_ErrorCode ST_PLC_ClrOne_DO(uint32_t Channel_Name)
{
        uint8_t bit_sequence, Initiatebit = 0x01, index_ips;
	extern uint8_t GlobalPLCWatchM1, DO_Module_status[];

	union ChannelDUnion
	{
		uint32_t ChannelDescriptor;
		struct
		{
			uint8_t bitposition;
			uint8_t Moduleid_8bit;
			uint8_t b3;
			uint8_t b4;	// or number them in reverse order
		}

		CDBytes;
	};
	union ChannelDUnion CurrentChannelDescriptor;
	CurrentChannelDescriptor.ChannelDescriptor = Channel_Name;
	
	bit_sequence = CurrentChannelDescriptor.CDBytes.bitposition; /* change this bit */
	if (CurrentChannelDescriptor.CDBytes.Moduleid_8bit == (uint8_t) Module_ID_ISO8200AQ) /*Module Q0 */
	{
		ISO_OutputEnable_Pin(GPIO_PIN_SET);
                bit_sequence = ~bit_sequence; /* Clr this DO */
		bit_sequence &= DO_Module_status[0]; 

                IO_ModuleStatus_Fault[3].IO_status= bit_sequence;
                DO_Module_status[0]= bit_sequence;
		Iso8200aq_WriteBytes(bit_sequence, &bit_sequence);
	}
	else if (CurrentChannelDescriptor.CDBytes.Moduleid_8bit == (uint8_t) Module_ID_IPS4260L) /*Module Q1 */
	{
                IO_ModuleStatus_Fault[4].IO_status &= bit_sequence;
                DO_Module_status[0]=  IO_ModuleStatus_Fault[4].IO_status;
		for (index_ips = 0; index_ips < 5; index_ips++)
		{
			if (bit_sequence == Initiatebit)
				break;
			Initiatebit <<= 1;
			GlobalPLCWatchM1 = Initiatebit;

		}

		GlobalPLCWatchM1 = index_ips;
		if (index_ips == 4)
			return (ST_PLC_ERROR_WrongParameter);
		GlobalPLCWatchM1 = index_ips;
		IPS4260_ClrSingleChannel(index_ips);
	}
	else return (ST_PLC_ERROR_WrongParameter);

	return (ST_PLC_NO_ERROR);
}

/***********************************************************//**
*@brief Get one digital input
*@param Channel_Name is used to inorder to call the specified channel 
*         of a particular module.
*@retval returns ST_PLC_NO_ERROR
***************************************************************/
ST_PLC_ErrorCode  ST_PLC_GetOne_DI (uint32_t Channel_Name, bool *Instatus)
{ 
        uint8_t In_sequence, Inregval , Startbit =0x01, index_clt03  ;
        extern uint8_t GlobalPLCWatchM1 ,DI_Module_status[];

        union InputChannelUnion
   {
            uint32_t  ChannelDescriptor;
            struct 
        { 
            uint8_t bitposition;
            uint8_t Moduleid_8bit;
            uint8_t b1;
            uint8_t b2; // can be in any order
        }  
    
       INData;
    };
      union InputChannelUnion InputChannelDetail;
      InputChannelDetail.ChannelDescriptor= Channel_Name;    
      
      In_sequence = InputChannelDetail.INData.bitposition;                    /*  get the first bit */
           if ( InputChannelDetail.INData.Moduleid_8bit == (uint8_t)Module_ID_CLT01_38SQ7)  /* Module I0 */
            {
                   
               Get_Input_WriteByte(&Inregval);
               IO_ModuleStatus_Fault[Module_ID_CLT01_38SQ7].IO_status=Inregval;
               Inregval &= InputChannelDetail.INData.bitposition;
                           
               Inregval &= InputChannelDetail.INData.bitposition;
                if (Inregval == 0)*Instatus = false;
                else *Instatus = true;
             }
 
            else if (InputChannelDetail.INData.Moduleid_8bit == (uint8_t)Module_ID_CLT03_2Q3_X_2) /* Module I1 */
            {       
                   
                for (index_clt03=0; index_clt03<5; index_clt03++)
                {
                    if (In_sequence == Startbit)
                    break;
         
                    Startbit <<= 1;
                    GlobalPLCWatchM1 = Startbit;
                }
              if (index_clt03 ==4)
             return(ST_PLC_ERROR_WrongParameter);
             GlobalPLCWatchM1 =index_clt03;
             *Instatus= Clt03_ReadChannel(index_clt03);
             
            }          
             else return(ST_PLC_ERROR_WrongParameter);
             
             return(ST_PLC_NO_ERROR);
}

/***********************************************************//**
*@brief Extract status of a specific digital output from bitmap stored in memory
*@param Channel_Name is used to extract specified 
*@retval returns boolean status
***************************************************************/
ST_PLC_ErrorCode   ST_PLC_GetOne_DO_Status (uint32_t Channel_Name, bool *Outstatus )

{ 
        uint8_t bitpos, MaskedVal  ;
        extern uint8_t GlobalPLCWatchM1 ,DI_Module_status[];

        union OutputChannelUnion
   {
            uint32_t  OChannelDescriptor;
            struct 
        { 
            uint8_t bitposition;
            uint8_t Moduleid_8bit;
            uint8_t b1;
            uint8_t b2; // can be in any order
        }  
    
       OutData;
    };
      union OutputChannelUnion OutputChannelDetail;
      OutputChannelDetail.OChannelDescriptor= Channel_Name;  
      MaskedVal  = IO_ModuleStatus_Fault[OutputChannelDetail.OutData.Moduleid_8bit].IO_status ;
      MaskedVal &= OutputChannelDetail.OutData.bitposition;
      if (MaskedVal == 0)*Outstatus = false;
      else *Outstatus = true;
                        
      return(ST_PLC_NO_ERROR);
}

/******************************************************//**
 *@brief Returns the FW version of the library
 *
 *@retval CLT03_FW_VERSION
 **********************************************************/
uint32_t ST_PLC_GetFWVersion(void)
{
	return (ST_PLC_FW_VERSION);
}

/*******************************************
*@brief Displays the HMI on various intensity
*@param LCD_intensity
*@retval None

*******************************************/
void ST_PLC_Set_HMI_Backlightintensity(uint32_t LCD_intensity)
{
   switch(LCD_intensity)
   {
     case BackLight_Reset:
         sConfigPWM.Pulse=0;
         HAL_TIM_Base_Start(&htim2);
         HAL_TIM_PWM_ConfigChannel(&htim2,&sConfigPWM,TIM_CHANNEL_1);
         HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_1);
     break;
    
     case BackLight_25:
        sConfigPWM.Pulse= 125;
        HAL_TIM_Base_Start(&htim2);
        HAL_TIM_PWM_ConfigChannel(&htim2,&sConfigPWM,TIM_CHANNEL_1);
        HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_1);
     break;
     
     case BackLight_50:
         sConfigPWM.Pulse= 250;
         HAL_TIM_Base_Start(&htim2);
         HAL_TIM_PWM_ConfigChannel(&htim2,&sConfigPWM,TIM_CHANNEL_1);
         HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_1);
      break;
    
     case BackLight_75:
         sConfigPWM.Pulse= 375;
         HAL_TIM_Base_Start(&htim2);
         HAL_TIM_PWM_ConfigChannel(&htim2,&sConfigPWM,TIM_CHANNEL_1);
         HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_1);
      break;
     
     case BackLight_100:
         sConfigPWM.Pulse= 500;
         HAL_TIM_Base_Start(&htim2);
         HAL_TIM_PWM_ConfigChannel(&htim2,&sConfigPWM,TIM_CHANNEL_1);
         HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_1);
     break;
   }
}



/******************************************************//**
 * @brief Returns the FW version of the library
 *
 * @retval CLT03_FW_VERSION
 **********************************************************/

uint8_t ST_PLC_Util_ReverseBits(uint8_t num8b) 
{ 
    uint8_t count = sizeof(num8b) * 8 - 1; 
    uint8_t reverse_num8b = num8b; 
      
    num8b >>= 1;  
    while(num8b) 
    { 
       reverse_num8b <<= 1;        
       reverse_num8b |= num8b & 1; 
       num8b >>= 1; 
       count--; 
    } 
    reverse_num8b <<= count; 
    return reverse_num8b; 
} 



//*****Function to check if bit is set or not *****
bool ST_PLC_UtilIsNthBitSet(uint8_t data, uint8_t pos)
{
	return ((data &(1 << pos)) ? 1 : 0);
}


//**********function to set bits of a 16 bit data to left********
uint8_t ST_PLC_UtilsetBitLeft(uint8_t tmp_data, uint8_t pos, bool bit)
{
	return (tmp_data | (bit << pos));
}


//**********function to set bits of a 16 bit data to Right********
uint8_t ST_PLC_UtilSetBitRight(uint8_t tmp_data, uint8_t pos)
{
	return (tmp_data | (1 >> pos));
}
   

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

