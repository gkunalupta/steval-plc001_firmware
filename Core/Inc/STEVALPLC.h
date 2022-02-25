/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef STEVALPLC_H
#define STEVALPLC_H

#ifdef __cplusplus
extern "C" {
#endif 
  
#include "main.h"
#include "CLT01.h"
//#include "ST_PLC_timerapi.h"
#include "cmsis_os.h"
#include "PLC_timer_api.h"  
#include "ladder.h"  
  
  /** Current FW major version */
#define STEVALPLC_FW_MAJOR_VERSION (uint8_t)(1)
  /** Current FW minor version */
#define STEVALPLC_FW_MINOR_VERSION (uint8_t)(0)
  /** Current FW patch version */
#define STEVALPLC_FW_PATCH_VERSION (uint8_t)(0)
  /** Current FW version */
#define ST_PLC_FW_VERSION       (uint32_t)((STEVALPLC_FW_MAJOR_VERSION<<16)|\
  (STEVALPLC_FW_MINOR_VERSION<<8)|\
    (STEVALPLC_FW_PATCH_VERSION))
//typedef  uint32_t ST_PLC_ErrorCode;

#define STEVAL_PLC_BoardVersion 1

enum ST_PLC_Err_Code { ST_PLC_NO_ERROR,
ST_PLC_ERROR_HW_FLT,
ST_PLC_ERROR_WrongParameter,
ST_PLC_ERROR_UnexpectedOutcome,
ST_PLC_ERROR_SPI,
ST_PLC_ERROR_Undefined,
ST_PLC_ERROR_FeatureNotSupported
};

# define Nb_Total_DI_Channels 12
# define Nb_Total_DO_Channels 12
# define Nb_Total_DI_Module  2
# define Nb_Total_DO_Module 2

enum Debug_Led{
               DebugLed1,
               DebugLed2,
               DebugLed3
               };

enum DebugLed_State{
                    DebugLed_ON,
                    DebugLed_OFF,
                    DebugLed_REVERSE
                   };
enum LCDBackLight_control{    
                          BackLight_Reset,   /* this initializes the LCD Backlight*/
                          BackLight_25,          /* LCD Backlight glows at 25% CCR= 8000 */
                          BackLight_50,         /* LCD Backlight glows at 50% ccr=16000*/
                          BackLight_75,         /* LCD Backlight glows at 75% CCR=24000*/
                          BackLight_100,         /* LCD Backlight glows at 100% CCR=32000*/
                        };
enum DODIStage{
                Stage1,
                Stage2
              };
 struct Module_Status
{
    uint8_t IO_status;
    uint8_t Fault_status;
};

enum Module_ID {                 /* All modules on STEVAL-PLC001 board ;  to be extended in case of additinal boards connected to say Morpho expnader*/
  Module_ID_STM32F746,  /* ARM Cortex-M7 core*/
  Module_ID_CLT01_38SQ7, /*Digital input current limiter*/
  Module_ID_CLT03_2Q3_X_2, /*Self powered D. input current limiter - teo devices clubed toegether to in the mcoulde*/
  Module_ID_ISO8200AQ, /*Galvanic isolated octal high side smart SSR) */
  Module_ID_IPS4260L, /* Digital Inputs two Modules, 8 + 4 channels */
  Moduel_ID_UART_GEN ,
  Module_ID_STLD40DPUR  ,  /*  =  0x80LCD Backlight controller;  kept room for inserting additional IO devices */
  Module_ID_RK035HQ02_CT814B, /* LCD Display */
  Module_ID_MAX     /*=0xFF */
};

/* Digital Inputs two Modules, 8 + 4 channels */
enum DI_Channel_Name { 
  
  /**    [31-][24-][][8-15: 8bit device ID][0-7: bitpattern]*/
  /** bit pattern reflect bit position in device register/pin; not the connections on a board */
  DI_Channel_I0_0 = (Module_ID_CLT01_38SQ7<<8) | (0x01)    ,
  DI_Channel_I0_1 = (Module_ID_CLT01_38SQ7<<8) | (0x01<<1) ,
  DI_Channel_I0_2 = (Module_ID_CLT01_38SQ7<<8) | (0x01<<2) , 
  DI_Channel_I0_3 = (Module_ID_CLT01_38SQ7<<8) | (0x01<<3) , 
  DI_Channel_I0_4 = (Module_ID_CLT01_38SQ7<<8) | (0x01<<4) ,
  DI_Channel_I0_5 = (Module_ID_CLT01_38SQ7<<8) | (0x01<<5) ,
  DI_Channel_I0_6 = (Module_ID_CLT01_38SQ7<<8) | (0x01<<6) , 
  DI_Channel_I0_7 = (Module_ID_CLT01_38SQ7<<8) | (0x01<<7) , 
  
  DI_Channel_I1_0 = (Module_ID_CLT03_2Q3_X_2<<8) | (0x01) , 
  DI_Channel_I1_1 = (Module_ID_CLT03_2Q3_X_2<<8) | (0x01 <<1) ,
  DI_Channel_I1_2 = (Module_ID_CLT03_2Q3_X_2<<8) | (0x01 <<2) ,
  DI_Channel_I1_3 = (Module_ID_CLT03_2Q3_X_2<<8) | (0x01 <<3)
};

/* Digital Outputs two Modulres, 8 + 4 channels */ 
enum DO_Channel_Name { 
  DO_Channel_Q0_7 = (Module_ID_ISO8200AQ<<8) | (0x01)    ,
  DO_Channel_Q0_6 = (Module_ID_ISO8200AQ<<8) | (0x01<<1) ,
  DO_Channel_Q0_5 = (Module_ID_ISO8200AQ<<8) | (0x01<<2) , 
  DO_Channel_Q0_4 = (Module_ID_ISO8200AQ<<8) | (0x01<<3) , 
  DO_Channel_Q0_3 = (Module_ID_ISO8200AQ<<8) | (0x01<<4) ,
  DO_Channel_Q0_2 = (Module_ID_ISO8200AQ<<8) | (0x01<<5) ,
  DO_Channel_Q0_1 = (Module_ID_ISO8200AQ<<8) | (0x01<<6) , 
  DO_Channel_Q0_0 = (Module_ID_ISO8200AQ<<8) | (0x01<<7) , 
  
  DO_Channel_Q1_0 = (Module_ID_IPS4260L<<8) | (0x01) , 
  DO_Channel_Q1_1 = (Module_ID_IPS4260L<<8) | (0x01 <<1) ,
  DO_Channel_Q1_2 = (Module_ID_IPS4260L<<8) | (0x01 <<2) ,
  DO_Channel_Q1_3 = (Module_ID_IPS4260L<<8) | (0x01 <<3)
};



#define Module_I1_1stChannel  DO_Channel_I1_0
#define Module_Q1_1stChannel  DO_Channel_Q1_0


enum IO_Devices_Name {                 
  IN_CLT01_38SQ7, /*Digital input current limiter*/
  IN_CLT03_2Q3_Array, /*Self powered D. input current limiter more than one device in a set; No of elements limted by Device  descriptor  bit field size*/
  OUT_ISO8200AQ, /*Galvanic isolated octal high side smart SSR) */
  OUT_IPS4260L, /* Digital Inputs two Modules, 8 + 4 channels */
  IO_UART_GEN ,
  UI_RK035HQ02_CT814B, /* LCD Display */
  XY_NAME_MAX =0xFF
};


typedef struct IODeviceDescriptor
{
  uint32_t  DeviceName_Interfaces;  /* Bit fields with device name , types of all interface for Control and status */
  void *SerialInterface;  /* typically points to a hanlde such as spi handle */
  void *GPIOInterfaces;  /*points to array containg GPIO control  and status pins */
  void *Reserved;
}IODeviceDescriptor_t;


static uint8_t     pos;
static void        STEVALPLC(void);
void  ISO_OutputEnable_Pin(GPIO_PinState state);

bool        STEvalPlcUtilIsNthBitSet(uint8_t ,uint8_t );
ST_PLC_ErrorCode   ST_PLC_Read_AllChannels_InModule(uint8_t ModuleName, uint8_t *InData );
ST_PLC_ErrorCode   ST_PLC_Write_AllChannels_InModule(uint8_t Module_ID, uint8_t Data);


ST_PLC_ErrorCode   ST_PLC_SetOne_DO (uint32_t Channel_Name);
ST_PLC_ErrorCode   ST_PLC_ClrOne_DO (uint32_t Channel_Name);
ST_PLC_ErrorCode  ST_PLC_GetOne_DI (uint32_t Channel_Name, bool *Instatus);
ST_PLC_ErrorCode   ST_PLC_GetOne_DO_Status (uint32_t Channel_Name, bool *Outstatus );
        

ST_PLC_ErrorCode  ST_PLC_GetFaultPin_Status(uint32_t Channel_Index);
ST_PLC_ErrorCode ST_PLC_Update_ModuleFaultStatus(uint8_t FaultModule);

bool ST_PlC_UtilIsNthBitSet(uint8_t ,uint8_t );
uint8_t ST_PLC_UtilsetBitLeft(uint8_t tmp_data, uint8_t pos,bool bit);
uint8_t ST_PLC_UtilSetBitRight(uint8_t tmp_data, uint8_t pos);
uint8_t  ST_PLC_Util_ReverseBits(uint8_t num8b) ; 

extern ST_PLC_ErrorCode Clt01_38sq7_ReadByte(uint8_t Module_ID , uint8_t* Data);
//void        StevalPLC_Setdata(uint8_t);     
ST_PLC_ErrorCode     Clt03_ReadNibble(uint8_t* Data);
void        ST_PLC_Write_Device1(uint8_t);
void        ST_PLC_Write_Device2(uint8_t);
ST_PLC_ErrorCode ST_PLC_InitiatizeSTEval(uint32_t BoardNameVersion);
uint8_t ST_PLC_util_ReverseCLT03(uint8_t num8b);

uint8_t ST_PLC_Util_ReverseBits(uint8_t num8b) ;
void ST_PLC_Set_HMI_Backlightintensity(uint32_t LCD_intensity);
void ST_PLC_Control_DebugLed(uint8_t DebugLed, uint8_t DebugState);
void ST_PLC_Blink_One_DebugLed(uint8_t DebugLed, uint32_t delay, uint8_t repeat);

ST_PLC_ErrorCode ST_PLC_SelectUsecase(ST_PLC_Usecase Choice);
ST_PLC_ErrorCode ST_PLC_Ipsfaultdetect(uint8_t Module_ID, uint8_t *IPSfaultStatus);
void Ips4260l_WriteNibble(uint8_t Data);
void IPS4260_SetSingleChannel(uint8_t Channel_Index);
void IPS4260_ClrSingleChannel(uint8_t Channel_Index);
/*ST_PLC_ErrorCode  ST_PLC_UsecaseNone(void)*/
void ST_PLC_Select_DIDO(void);
void ST_PLC_Select_InfoDIDO_(void);
void ST_PLC_Select_PLC_Info(void);
void ST_PLC_Select_LadderLogic(void);
void ST_PLC_Select_DODI_Selftest(void);
void ST_PLC_Select_UserDefined(void);
void ST_PLC_Select_BoardConfig(void);
void ST_PLC_Select_None(void);
void ST_PLC_Toggle_UseCase(void);
uint16_t  ST_PLC_Set_Progress(void);
void  ST_PLC_Manage_Progress_Bar(void);
void  ST_PLC_Set_HOME_Button_Actions(void);
uint8_t  ST_PLC_GetHexIOValue(void);
void  ST_PLC_Reset_HOME_Button_Flag(void);
uint8_t  ST_PLC_Get_HOME_Button_Flag(void);
uint8_t  ST_PLC_ResetHexIOValue(void);
uint8_t  ST_PLC_ResetHexIOValue(void);
uint8_t ST_PLC_GetSFLASHFlag(void);
uint8_t ST_PLC_GetSRAMFlag(void);
void ST_PLC_PerformSFLASHTest(void);
void ST_PLC_PerformSRAMTest(void);
void SetIOPlayFlag(void);
void SetSelfTestFlag(void);
void ResetSelfTestFlag(void);
void ResetIOPlayFlag(void);
void SetToggleFlag(void);
void ResetToggleFlag(void);
uint8_t GetSelfTestFlag(void);
ST_PLC_Usecase ST_PLC_Get_Current_State(void);
void Fresh_Start(void);

#ifdef __cplusplus
}
#endif

#endif /* STEVALPLC_TARGET_CONFIG_H */
