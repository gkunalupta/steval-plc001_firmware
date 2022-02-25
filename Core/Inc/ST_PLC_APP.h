/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef ST_PLC_APP_H
#define STEVALPLC_H      

#ifdef __cplusplus
 extern "C" {
#endif 
   
#include "main.h"
#define STEVAL_PLC_BoardVersion 1
ST_PLC_ErrorCode ST_PLC_SelectUsecase(uint8_t Choice);   
ST_PLC_ErrorCode ST_PLC_DODIInitialStage(void);
ST_PLC_ErrorCode ST_PLC_DODIStageProgress(uint8_t Stage);
ST_PLC_ErrorCode ST_PLC_UsecaseDIDO(void);
ST_PLC_ErrorCode ST_PLC_UsecaseInfo(void);
ST_PLC_ErrorCode ST_PLC_UsecaseInfo_DIDO(void);
ST_PLC_ErrorCode ST_PLC_UsecaseLadderLogic(void);
ST_PLC_ErrorCode ST_PLC_UsecaseDODI_Selftest(void);
ST_PLC_ErrorCode ST_PLC_UsecaseUserdefined(void);
ST_PLC_ErrorCode ST_PLC_Module_functionaltestPhase1(void);
ST_PLC_ErrorCode ST_PLC_Module_functionaltestphase2(void);
ST_PLC_ErrorCode ST_PLC_Module_functionaltestphase3(void);
ST_PLC_ErrorCode ST_PLC_Module_functionaltestphase3_off(void);
ST_PLC_ErrorCode ST_PLC_Module_functionaltestPhase2_Deactive(void);
ST_PLC_ErrorCode ST_PLC_Module_functionaltestPhase1_Deactive(void);
ST_PLC_ErrorCode ST_PLC_DODIsequecetest(void);



#ifdef __cplusplus
  }
#endif

#endif