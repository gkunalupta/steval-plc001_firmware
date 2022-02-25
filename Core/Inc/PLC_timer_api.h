#ifndef timer_H
#define timer_H

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"
#include "cmsis_os.h"
  
/* Limit for maximum timers that can be created by user */
#define ST_PLC_MaxTimers 10
  
/**
 * Type by which PLC software timers are referenced.  For example, a call to
 * ST_PLC_CreateTimer() returns a timer handle that can be used by
 * ST_PLC_StartTimer(),ST_PLC_StopTimer(), ST_PLC_ResetTimer()
 */
typedef struct ST_PLC_Timer *ST_PLC_TimerHandle_t;


/**
 * Error code variable shared by ST_PLC API functions
 * 0 = Failure, 1=Success
 * */
typedef uint8_t ST_PLC_ErrorCodes;



/*PLC timer types*/
enum ST_PLC_TimerType {ST_PLC_OnDelayTimer,
                       ST_PLC_OffDelayTimer,
                       ST_PLC_RTOTimer};


/**
 * PLC Timer structure
 * User defined callback function will be passed a timer handle ST_PLC_TimerHandle_t as an argument
 * FreeRTOS timer handle is also stored in struct when timer is created
 */
struct ST_PLC_Timer {
	TimerHandle_t handle;
	int id;										// ID used by FreeRTOS kernel
	int preset;									// Timer preset value in milliseconds
	uint8_t type;								// Timer type
	bool done;								// Timer one bit
	bool enable;								// Timer enable bit
	TickType_t accumulator;						// Accumulated value for RTO when it is stopped
	void (*callback)(ST_PLC_TimerHandle_t);		// User defined callback function
};


/**
  * @brief  Callback on timer expiry. Updates timer enable & done bits.
  * 		Calls the user callback linked when timer is defined. Toggles LED on nucleo board.
  * @param  FreeRTOS timer handle
  */
void ST_PLC_TimerCallback(xTimerHandle xTimer);


/**
  * @brief  Creates a new one shot software timer.
  * @param  preset (time in milliseconds), type of timer, callback function
  * @retval timer handle
  */
ST_PLC_TimerHandle_t ST_PLC_CreateTimer(int preset, uint8_t type, void (*func)(ST_PLC_TimerHandle_t));


/**
  * @brief  Start timer.
  * @param  timer structure
  * @retval 0 for fail and 1 for pass
  */
BaseType_t ST_PLC_StartTimer(ST_PLC_TimerHandle_t timHandle);


/**
  * @brief  Stop timer.
  * @param  timer structure
  * @retval 0 for fail and 1 for pass
  */
BaseType_t ST_PLC_StopTimer(ST_PLC_TimerHandle_t timHandle);

/**
  * @brief  Reset timer.
  * @param  timer structure
  * @retval 0 for fail and 1 for pass
  */
BaseType_t ST_PLC_RTOReset(ST_PLC_TimerHandle_t timHandle);



/**
  * @brief  Change timer period
  * @param  timer structure
  * @retval 0 for fail and 1 for pass
  */
ST_PLC_ErrorCodes ST_PLC_ChangeTimerPeriod(ST_PLC_TimerHandle_t timHandle, int preset);



/**
  * @brief  Enable timer.
  * @param  timer structure
  * @retval 0 for fail and 1 for pass
  */
ST_PLC_ErrorCodes ST_PLC_EnableTimer(ST_PLC_TimerHandle_t timHandle);


/**
  * @brief  Disable timer.
  * @param  timer structure
  * @retval 0 for fail and 1 for pass
  */
ST_PLC_ErrorCodes ST_PLC_DisableTimer(ST_PLC_TimerHandle_t timHandle);


#ifdef __cplusplus
}
#endif

#endif