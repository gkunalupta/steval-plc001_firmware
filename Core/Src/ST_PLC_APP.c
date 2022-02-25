/**
******************************************************************************
* @file    ST_PLC_APP.c
* @brief   All function related to the input & output module.
******************************************************************************
* @attention
*
* Copyright (c) 2020 STMicroelectronics.

*/
#include "main.h"
//#include "ST_PLC_APP.h"

#include "STEVALPLC.h"
#include "ISO8200.h"
#include "W25xxx.h"

extern uint8_t DO_Module_status[];
extern TIM_HandleTypeDef htim2;
extern struct Module_Status IO_ModuleStatus_Fault[5];
extern ST_PLC_ErrorCode ST_PLC_Module_FaultStatus(uint8_t FaultModule, uint8_t *Data);
extern SDRAM_HandleTypeDef hsdram1;
uint8_t GlobalPLCWatchM1, Global_TestErrorCount;
uint32_t GlobalPLCWatch32;

extern void ST_PLC_Blink_One_DebugLed(uint8_t DebugLed, uint32_t delay, uint8_t repeat); 

extern ST_PLC_ErrorCode ST_PLC_Update_ModuleFaultStatus(uint8_t FaultModule);
extern SDRAM_HandleTypeDef hsdram1;

/**
*@brief  This function maps the DODI modules 
*@param  Module_ID & testpattern
*@retval retuns ST_PLC_NO_ERROR
*/

  // the code will map the pattern of input to the output


/**
*@brief  This function invokes the DIDO Usecase. 
*
*@param  None
*
*@retval retuns ST_PLC_NO_ERROR
*/
ST_PLC_ErrorCode ST_PLC_UsecaseDIDO(void)
{
  extern uint8_t GlobalPLCWatch2;
  uint8_t InDataclt1 = 0, InDataclt03 = 0, Outdata;
  //InData=0xff;
  ST_PLC_Read_AllChannels_InModule((uint8_t) Module_ID_CLT01_38SQ7, &InDataclt1);
  Outdata = ST_PLC_Util_ReverseBits(InDataclt1); /*mirroring*/
  //GlobalPLCWatch2 = Outdata;
  ST_PLC_Write_AllChannels_InModule((uint8_t) Module_ID_ISO8200AQ, Outdata);
  ST_PLC_Update_ModuleFaultStatus((uint8_t) Module_ID_ISO8200AQ );  /* update faiult status for display */
  if(InDataclt1= Outdata)  /* dummy check*/
  {
    ST_PLC_Blink_One_DebugLed(DebugLed2,100,2);
  }
  
  ST_PLC_Read_AllChannels_InModule((uint8_t) Module_ID_CLT03_2Q3_X_2, &InDataclt03);
  Outdata = IO_ModuleStatus_Fault[Module_ID_CLT03_2Q3_X_2].IO_status;
  //GlobalPLCWatch2 = Outdata;
  ST_PLC_Write_AllChannels_InModule((uint8_t) Module_ID_IPS4260L, Outdata);
  ST_PLC_Update_ModuleFaultStatus((uint8_t) Module_ID_IPS4260L );
  if(InDataclt03= Outdata)
  {
    ST_PLC_Blink_One_DebugLed(DebugLed2,100,2);
  }
  return (ST_PLC_NO_ERROR);
  
}

/**
*@brief  This function provides information regarding the all the pins 
*        and channels being used.
*
*@param  None
*
*@retval retuns ST_PLC_NO_ERROR
*/
ST_PLC_ErrorCode ST_PLC_UsecaseInfo(void)
{
	 ST_PLC_Blink_One_DebugLed(DebugLed1,100,1);
         HAL_Delay(5000);
         ST_PLC_Blink_One_DebugLed(DebugLed2,250,2);
  //ST_PLC_Read_Device2(2);//Device 2 is CLT03
         return (ST_PLC_NO_ERROR);
/**
*@brief  This function gives the information about all the DODI 
*
*@param  None
*
	return (ST_PLC_NO_ERROR);
*/
}

/**
*@brief for executing the ladder logic 
*
*@note  This function executes the ladder logic as per the 
*       user choice.     
*
*@param  None
*
*@retval retuns ST_PLC_NO_ERROR
*/
ST_PLC_ErrorCode ST_PLC_UsecaseLadderLogic(void)
{
  ST_PLC_ExecuteLadder();
  return (ST_PLC_NO_ERROR);
  
}



/**********************************************************
 *@brief DODI stage One
 *@note  This function executes the first stage of the  
 *            DODI Selftest
 *@param  None
 *@retval retuns Error Count 
 **********************************************************/
ST_PLC_ErrorCode ST_PLC_DODIInitialStage(void)
{
         uint8_t stage1, stage1pattern,stage_out, S1ModuleName,S1INModuleName;
         uint8_t ModIndex, S1bit_position;
         uint32_t DO_module_bit, S1delay=200;
         
         ModIndex=0;
         S1ModuleName = Module_ID_ISO8200AQ;
         S1INModuleName = Module_ID_CLT01_38SQ7;
         for(stage1=0;stage1 < Nb_Total_DO_Channels; stage1++)
         {
           if (stage1 == 0) 
                {  S1bit_position = 0x80;
                }
		 else if (stage1 == 8)
		{
			S1bit_position = 0x08; ModIndex=1;
			S1ModuleName = Module_ID_IPS4260L;
                        S1INModuleName = Module_ID_CLT03_2Q3_X_2;
		}
		else S1bit_position = S1bit_position >> 1;
		
		DO_module_bit = (S1ModuleName << 8) | (S1bit_position);
                
                ST_PLC_SetOne_DO(DO_module_bit);
                stage_out= IO_ModuleStatus_Fault[S1ModuleName].IO_status;
		HAL_Delay(S1delay);
		ST_PLC_Read_AllChannels_InModule(S1INModuleName, &stage1pattern);

                if( S1INModuleName ==Module_ID_CLT01_38SQ7)  /* Connection are in reverese in the board, logic can be made generic */
                {
                  stage1pattern = ST_PLC_Util_ReverseBits(stage1pattern);
                }

		ST_PLC_ClrOne_DO(DO_module_bit);
                
		if (stage1pattern !=  stage_out) 
		   {   
                     Global_TestErrorCount++;
		   	/*return (ST_PLC_ERROR_UnexpectedOutcome); */
                     ST_PLC_Blink_One_DebugLed(DebugLed1,100,2);
                     
                   }
               /*GlobalPLCWatch2=   Global_TestErrorCount;*/
         }
          if ( Global_TestErrorCount !=0) 
            {
              return (ST_PLC_ERROR_UnexpectedOutcome) ;
            }
                  
         return ST_PLC_NO_ERROR;
}


/**********************************************************
 *@brief This function performs the stage1 and stage2 of the DODIselftest
 *@param  DODIStage: takes the stage that needs to be executed.
 *@retval retuns errorcount 
 **********************************************************/
ST_PLC_ErrorCode ST_PLC_DODIStageProgress(uint8_t DODIStage)
{
        uint8_t Module_In, Module_Out,stagepattern,ModIndex, stageprev, progress_bit, bit_position;
        uint8_t Stage3;
        uint32_t Delay=150;
        
       /*stage1: Set and clear Four DOs and check correct feedback to  corresponding DIs */
        switch(DODIStage)
        {
         case Stage1: 
             bit_position=0xF0;
             Module_Out = Module_ID_ISO8200AQ;
             Module_In = Module_ID_CLT01_38SQ7;
            for(progress_bit=0;progress_bit < 3; progress_bit++)
            {   
                    if (progress_bit <= 1) 
                {  
                      ST_PLC_Write_AllChannels_InModule(Module_Out,bit_position);
                      bit_position = bit_position >> 4;
                }
                  
		   else if (progress_bit == 2)
		{
                     bit_position=0x0F;
                     Module_Out = Module_ID_IPS4260L;
                     Module_In  = Module_ID_CLT03_2Q3_X_2;
                     ST_PLC_Write_AllChannels_InModule(Module_Out,bit_position); 
		}
                    stageprev= IO_ModuleStatus_Fault[Module_Out].IO_status;
                    
	            HAL_Delay(Delay);
		    ST_PLC_Read_AllChannels_InModule(Module_In, &stagepattern);
                    HAL_Delay(Delay);
                    
                  if( Module_In ==Module_ID_CLT01_38SQ7)  /* Connection are in reverese in the board, logic can be made generic */
                {
                     stagepattern = ST_PLC_Util_ReverseBits(stagepattern);
                }
                     ST_PLC_Write_AllChannels_InModule(Module_Out,0x00);  
                     
               
                  
                  if (stagepattern !=  stageprev) 
		   {   
                     Global_TestErrorCount++;
		   	/*return (ST_PLC_ERROR_UnexpectedOutcome); */
                     ST_PLC_Blink_One_DebugLed(DebugLed1,100,2);
                     
                   }
                                    
            }
             ST_PLC_Blink_One_DebugLed(DebugLed1,Delay,2);
            break;
            
        /*stage2: Set All DOs and check correct feedback to corresponding DIs */    
        case Stage2:
                       
              Module_In = Module_ID_CLT01_38SQ7;
              for(Stage3=0;Stage3<2;Stage3++)
                { 
                  Module_Out = Module_ID_ISO8200AQ;
                  bit_position=0xFF;
                 for(progress_bit=0;progress_bit < 2; progress_bit++)
                  {   
                    if (progress_bit ==0) 
                   {  
                      ST_PLC_Write_AllChannels_InModule(Module_Out,bit_position);
                      bit_position = bit_position >> 4;
                   }
                  
		   else if (progress_bit == 1)
		  {
                    
                     Module_Out = Module_ID_IPS4260L;
                     Module_In  = Module_ID_CLT03_2Q3_X_2;
                     ST_PLC_Write_AllChannels_InModule(Module_Out,bit_position);
		  }
                    stageprev= IO_ModuleStatus_Fault[ModIndex].IO_status=stageprev;
                    
	            HAL_Delay(Delay);
		    ST_PLC_Read_AllChannels_InModule(Module_In, &stagepattern);
                      if( Module_In ==Module_ID_CLT01_38SQ7)  /* Connection are in reverese in the board, logic can be made generic */
                {
                     stagepattern = ST_PLC_Util_ReverseBits(stagepattern);
                }
                    HAL_Delay(Delay);                  
                    /*if(ST_PLC_Get_ModuleFaultStatus(Module_Out)!= HAL_OK)*//*Error in the output module is checked,Led blinks on fault detection*/
                  /*{
                     errorcount++; 
                     ST_PLC_Blink_One_DebugLed(DebugLed1,100,2);                     
                  }*/
                    if (stagepattern !=  stageprev) 
		   {   
                     Global_TestErrorCount++;
		   	/*return (ST_PLC_ERROR_UnexpectedOutcome); */
                     ST_PLC_Blink_One_DebugLed(DebugLed1,100,2);
                     
                   }
                    
            }
            ST_PLC_Write_AllChannels_InModule(Module_ID_ISO8200AQ,0x00); 
            ST_PLC_Write_AllChannels_InModule(Module_ID_IPS4260L,0x00);
                 
          }
             
             break;
        }
              
        if ( Global_TestErrorCount !=0) 
            {
              return (ST_PLC_ERROR_UnexpectedOutcome) ;
            }
                
         return (ST_PLC_NO_ERROR);

}


/***********************************************************
*@brief This functions enables each 12 digital output pin 
        one by one and deactivating in the same way
*@param None 
*@retval returns ST_PLC_NO_ERROR
*************************************************************/

ST_PLC_ErrorCode ST_PLC_DODIsequecetest(void)
{
         uint8_t stage1,stage1pattern, S1ModuleName,S1INModuleName;
         uint8_t Oneprev, ModIndex, S1bit_position;
         uint32_t DO_module_bit, S1delay=100;
         ModIndex=0;
         S1ModuleName = Module_ID_ISO8200AQ;
         S1INModuleName = Module_ID_CLT01_38SQ7;
         
         for(stage1=0;stage1 < Nb_Total_DO_Channels; stage1++)
         {
           if (stage1 == 0) 
                {  S1bit_position = 0x80;
                }
		 else if (stage1 == 8)
		{
			S1bit_position = 0x08; ModIndex=1;
			S1ModuleName = Module_ID_IPS4260L;
                        S1INModuleName = Module_ID_CLT03_2Q3_X_2;
		}
		else S1bit_position = S1bit_position >> 1;
		DO_module_bit = (S1ModuleName << 8) | (S1bit_position);
                
                HAL_Delay(S1delay);
                ST_PLC_SetOne_DO(DO_module_bit);
                Oneprev=DO_Module_status[ModIndex];
//                IO_ModuleStatus_Fault[ModIndex].IO_status=stageoneprev;
		HAL_Delay(S1delay);
		ST_PLC_Read_AllChannels_InModule(S1INModuleName, &stage1pattern);
                if( S1INModuleName ==Module_ID_CLT01_38SQ7)  /* Connection are in reverese in the board, logic can be made generic */
                {
                  stage1pattern = ST_PLC_Util_ReverseBits(stage1pattern);
                }
         }
            ModIndex=0;
            S1ModuleName = Module_ID_ISO8200AQ;
            S1INModuleName = Module_ID_CLT01_38SQ7;
         for(stage1=0; stage1 < Nb_Total_DO_Channels; stage1++)
         {
           if (stage1 == 0) 
                {  S1bit_position = 0x80;
                }
		 else if (stage1 == 8)
		{
			S1bit_position = 0x08; ModIndex=1;
			S1ModuleName = Module_ID_IPS4260L;
                        S1INModuleName = Module_ID_CLT03_2Q3_X_2;
		}
		else S1bit_position = S1bit_position >> 1;
                                      
		DO_module_bit = (S1ModuleName << 8) | (S1bit_position);
                ST_PLC_ClrOne_DO(DO_module_bit);                
                Oneprev=DO_Module_status[ModIndex];

		HAL_Delay(S1delay);
		ST_PLC_Read_AllChannels_InModule(S1INModuleName, &stage1pattern);
                if( S1INModuleName ==Module_ID_CLT01_38SQ7)  /* Connection are in reverese in the board, logic can be made generic */
                {
                  stage1pattern = ST_PLC_Util_ReverseBits(stage1pattern);
                }

         }
      return(ST_PLC_NO_ERROR); 
}

 /********************************************************************
 *@brief  Celar output and do nothing else  
 *        if all the pins are mapped correctly then corresponding output 
 *        gives result.
 *@param  None
 *@retval retuns ST_PLC_NO_ERROR
 ******************************************************************/

ST_PLC_ErrorCode  ST_PLC_UsecaseNone(void)
{       
  
  Fresh_Start();
  ST_PLC_Blink_One_DebugLed(DebugLed2,75,2);
  HAL_Delay(100);
  return (ST_PLC_NO_ERROR);
  
  
}

   
   
/********************************************************************
 *@brief  Intended for Loopback self test.DO is fedback to corresponding DI
 *@note   This function takes the input and maps it to the output pins 
 *        if all the pins are mapped correctly then corresponding output 
 *        gives result.
 *@param  None
 *@retval retuns ST_PLC_NO_ERROR
 ******************************************************************/

ST_PLC_ErrorCode ST_PLC_UsecaseDODI_Selftest(void)
{       uint8_t ErrorCount=0, numblink; 
        Global_TestErrorCount =0; 
        ErrorCount += ST_PLC_DODIInitialStage(); /* Check One DO >DI at a time */
        Global_TestErrorCount =0; 
        ErrorCount += ST_PLC_DODIStageProgress((uint8_t)Stage1); /* Check four DO >DI at a time */
        Global_TestErrorCount =0; 
        ErrorCount += ST_PLC_DODIStageProgress((uint8_t)Stage2);/* Check all DO >DI at a time */
        
        if (ErrorCount != 0 )
        {   Global_TestErrorCount =0; 
            for ( numblink =0; numblink <25 ; numblink++)
                {
                ST_PLC_Blink_One_DebugLed(DebugLed1,100,1);
                ST_PLC_Blink_One_DebugLed(DebugLed2,100,1);
                }
          return (ST_PLC_ERROR_UnexpectedOutcome);
        }
        ST_PLC_Blink_One_DebugLed(DebugLed2,1000,7);
        return (ST_PLC_NO_ERROR);
}



/**
*@brief For the user to test there own program and test it
*
*@param None
*
*@retval returns ST_PLC_NO_ERROR
*/
ST_PLC_ErrorCode ST_PLC_UsecaseUserdefined(void)
{
#ifdef 0
  uint8_t ipsFault;
  uint8_t Data=0x05;
//  if(ST_PLC_Ipsfaultdetect(Module_ID_IPS4260L)!= HAL_ERROR)  //Org
  if(ST_PLC_Ipsfaultdetect(Module_ID_IPS4260L, &ipsFault)!= HAL_ERROR)  //BMP
  {
   ST_PLC_Blink_One_DebugLed(DebugLed2,100,2); /*D36 -Blue*/
  }
  else return (ST_PLC_ERROR_HW_FLT);
  ST_PLC_Write_AllChannels_InModule((uint8_t)Module_ID_IPS4260L , Data);
#endif  
  return(ST_PLC_NO_ERROR);
  

}

ST_PLC_ErrorCode ST_PLC_UsecaseBoardConfig(void)
{
  
  uint8_t *Data=0;
  //uint32_t Channel_Index=0;
  /*ST_PLC_Module_FaultStatus((uint8_t) Module_ID_ISO8200AQ , Data);
  ST_PLC_Module_FaultStatus((uint8_t) Module_ID_IPS4260L, Data);*/
  
  
  return (ST_PLC_NO_ERROR);
}



/**
*@brief  Selects the usecase for calling various function
*
*@note   The function calls various functions to execute different operations 
*        in a PLC
*            
*@param Choice that takes the value and performs the functions
*
*@retval return ST_PLC_NO_ERROR
*/
ST_PLC_ErrorCode ST_PLC_SelectUsecase(ST_PLC_Usecase Choice)
{
  switch (Choice)
  {
  case ST_PLC_DIDO:	//Dido usecase
    ST_PLC_UsecaseDIDO();
    break;
    
  case ST_PLC_Info:	//Read data of a particular channel
    ST_PLC_UsecaseInfo();
    break;
     
  case ST_PLC_LadderLogic:	// usecase Ladderlogic 
    ST_PLC_UsecaseLadderLogic();
    break;
    
  case ST_PLC_DODI_Selftest:	//usecase Selftest
    ST_PLC_UsecaseDODI_Selftest();
    break;
    
  case ST_PLC_Userdefined:	// usecase Userdefined
    ST_PLC_UsecaseUserdefined();
    break;
    
  case ST_PLC_BoardConfig:       // Fault test
    ST_PLC_UsecaseBoardConfig(); 
    break;       
    
  case ST_PLC_NONE:
    ST_PLC_UsecaseNone();
    break;
    
  }
  
  return (ST_PLC_ERROR_FeatureNotSupported);
}

extern ST_PLC_Usecase CurrentUsecase;



void ST_PLC_Select_DIDO(void)
{
  CurrentUsecase = ST_PLC_DIDO;    
}


void ST_PLC_Select_InfoDIDO_(void)
{
  CurrentUsecase = ST_PLC_Info_DIDO;    
}

void ST_PLC_Select_PLC_Info(void)
{
  CurrentUsecase = ST_PLC_Info;    
}

void ST_PLC_Select_LadderLogic(void)
{
  CurrentUsecase = ST_PLC_LadderLogic;    
}

void ST_PLC_Select_DODI_Selftest(void)
{
  CurrentUsecase = ST_PLC_DODI_Selftest;    
}

void ST_PLC_Select_UserDefined(void)
{
  CurrentUsecase = ST_PLC_Userdefined;    
}

void ST_PLC_Select_BoardConfig(void)
{
  CurrentUsecase = ST_PLC_BoardConfig;    
}


void ST_PLC_Select_None(void)
{
  CurrentUsecase = ST_PLC_NONE;
}


ST_PLC_Usecase ST_PLC_Get_Current_State(void)
{
  return (CurrentUsecase);
}


uint8_t SelfTestFlag = 0;
uint8_t IOPlayFlag = 0;
uint8_t Toggleflag = 0;


void SetSelfTestFlag(void)
{
  SelfTestFlag = 1;
}

uint8_t GetSelfTestFlag(void)
{
  return(SelfTestFlag);
}


void SetIOPlayFlag(void)
{
  IOPlayFlag = 1;
}

void ResetSelfTestFlag(void)
{
  SelfTestFlag = 0;
}

void ResetIOPlayFlag(void)
{
  IOPlayFlag = 0;
}

void SetToggleFlag(void)
{
  Toggleflag = 1;
}

void ResetToggleFlag(void)
{
  Toggleflag = 0;
}




void ST_PLC_Toggle_UseCase(void)
{
//  static uint8_t flag = 0;
  
  if(Toggleflag == 0)
  {
    Toggleflag = 1;
    
    if(SelfTestFlag == 1)
      CurrentUsecase = ST_PLC_DODI_Selftest;
    if(IOPlayFlag == 1)
      CurrentUsecase = ST_PLC_DIDO;
  }
  else
  {
    Toggleflag = 0;
   // IOPlayFlag = 0;
   // SelfTestFlag = 0;
    CurrentUsecase = ST_PLC_NONE;
  }
}



volatile uint16_t ProgressBar = 0;
volatile uint8_t ProgBarStartFlag = 0;





uint16_t  ST_PLC_Set_Progress(void)
{
  static uint16_t tmp = 0;
  //    HAL_Delay(200);
  
  tmp++;
  
  if(ProgBarStartFlag == 1)
  {
    if(ProgressBar < 100)
    {
      if(tmp>10)
      {
        ProgressBar += 10;
        tmp = 0;
      }
      return(ProgressBar);
    }
    else
    {
      if(tmp >10)
      {
        ProgressBar = 0;
        tmp = 0;
      }
      return(ProgressBar);
    } 
  }
  
  else
  {
    tmp = 0;
    return(ProgressBar);
  }
}

 static uint8_t ProgBarFlag = 0;

void  ST_PLC_Manage_Progress_Bar(void)
{
//  static uint8_t ProgBarFlag = 0;
  
  if(ProgBarFlag == 0)
  {
    ProgBarStartFlag = 1;
    ProgBarFlag = 1;
  }
  
  else if(ProgBarFlag == 1)
  {
    ProgBarStartFlag = 0;
    ProgBarFlag = 0;
  }
}

uint8_t HomeButtonFlag = 1;


void  ST_PLC_Set_HOME_Button_Actions(void)
{
 ProgBarFlag = 0; 
 ProgressBar = 0;
 ProgBarStartFlag = 0;
 ST_PLC_Select_None();
 
 HomeButtonFlag  = 1;
 ST_PLC_ResetHexIOValue();
}


void  ST_PLC_Reset_HOME_Button_Flag(void)
{
 HomeButtonFlag  = 0;
}

uint8_t  ST_PLC_Get_HOME_Button_Flag(void)
{
  return HomeButtonFlag;
  
}

  static uint8_t hex = 0;


uint8_t  ST_PLC_GetHexIOValue(void)
{
  
  uint8_t tst = ST_PLC_Get_HOME_Button_Flag();
  if(tst == 0)
  {
    //  static uint8_t hex = 0;
    static uint16_t tmp = 0 ;
    
    if(tmp == 50)
    {
      tmp = 0;
      hex++;
    }
    else
    {
      tmp++;
    }
  }
  else
  {
    hex = 0;
  }
  
  return hex;
}




uint8_t  ST_PLC_ResetHexIOValue(void)
{

  hex  = 0;
  return hex;
  
}


uint8_t SFlashTestFlag = 0xFF;
uint8_t SRAMTestFlag = 0xFF;


void ST_PLC_PerformSFLASHTest(void)
{
  uint8_t wrbuff = 0xAA;
  uint8_t rdbuff;
  uint8_t frdbuff;
  
  W25xxx_ReadBytes(&frdbuff, 0x00, 1);
  
  W25xxx_WriteByte(wrbuff, 0x00);
  W25xxx_ReadBytes(&rdbuff, 0x00, 1);
  
  if(wrbuff == rdbuff)
    SFlashTestFlag = 1;  //SET 1 ,If Tested Ok
  else
    SFlashTestFlag = 0;
  
  W25xxx_WriteByte(frdbuff, 0x00);
  
}


void ST_PLC_PerformSRAMTest(void)
{
  uint8_t wrbuff = 0xAA;
  uint8_t rdbuff;
  uint8_t frdbuff;
  
  uint32_t addr = 0;
  
  HAL_SDRAM_Read_8b(&hsdram1, &addr, &frdbuff, 1);
  
  HAL_SDRAM_Write_8b(&hsdram1, &addr, &wrbuff, 1);
  HAL_SDRAM_Read_8b(&hsdram1, &addr, &rdbuff, 1);
  
  if (rdbuff == wrbuff)
  {
    SRAMTestFlag = 1;       //SET 1, If Tested Ok
  }
  else
  {
    SRAMTestFlag = 0;
  }
  
}





uint8_t ST_PLC_GetSFLASHFlag(void)
{
//  uint8_t tmp = 1;
  
//  if(1)  //If Tested Ok
  
  return SFlashTestFlag;
}



uint8_t ST_PLC_GetSRAMFlag(void)
{
//  uint8_t tmp = 1;
  
//  if(1)  //If Tested Ok
  
  return SRAMTestFlag;
}




/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/