#include "PLC_timer_api.h"

/**
 * Starting ID and initial database index of the first ever created PLC timer
 * should always be = 0
 */
uint32_t ST_PLC_NewTimerID = 0;
ST_PLC_ErrorCodes isSuccess;

/**
 * A database of timers which is an array of ST_PLC_Timer. It stores all the timers
 * created by user with "index = id of timer" and their pointers are returned from
 * ST_PLC_CreateTimer()
 */
struct ST_PLC_Timer ST_PLC_TimerDatabase[ST_PLC_MaxTimers];

/**
  * @brief  Callback on timer expiry. Updates timer enable & done bits.
  * 		Calls the user callback linked when timer is defined. Toggles LED on nucleo board.
  * @param  FreeRTOS timer handle
  */
void ST_PLC_TimerCallback(xTimerHandle xTimer){
	uint32_t id = ( uint32_t ) pvTimerGetTimerID(xTimer); 			 // timer ID

	switch(ST_PLC_TimerDatabase[id].type)
		{
		   case ST_PLC_OffDelayTimer:
			  // Done bit for off delay timer is reset
			  ST_PLC_TimerDatabase[id].done = 0;
			  break;

		   default:
				// Done bit for on delay and RTO timer is set
				ST_PLC_TimerDatabase[id].done = 1;
				break;
		}

	// Make call to user callback function if defined
	if(ST_PLC_TimerDatabase[id].callback != NULL){
		ST_PLC_TimerDatabase[id].callback(&ST_PLC_TimerDatabase[id]);
	}
}


/**
  * @brief  Creates a new one shot software timer.
  * @param  preset (time in milliseconds), type of timer, callback function
  * @retval timer handle
  */
ST_PLC_TimerHandle_t ST_PLC_CreateTimer(int preset, uint8_t type,
										void (*func)(ST_PLC_TimerHandle_t)){

	// Create a temporary timer structure
	struct ST_PLC_Timer timStruct;

	// Set initial properties
	timStruct.handle = xTimerCreate("Timer", pdMS_TO_TICKS(preset), pdFALSE,
									(void *) ST_PLC_NewTimerID, ST_PLC_TimerCallback);
	timStruct.enable = 0;
	timStruct.preset = preset;
	timStruct.id = ST_PLC_NewTimerID;
	timStruct.type = type;
	timStruct.callback = func;
	timStruct.accumulator = 0;
	timStruct.done = 0;

	// Store the created timer in the database array
	ST_PLC_TimerDatabase[ST_PLC_NewTimerID] = timStruct;

	// Return timer handle
	return &ST_PLC_TimerDatabase[ST_PLC_NewTimerID++];
}


/**
  * @brief  Start timer.
  * @param  timer structure
  * @retval 0 for fail and 1 for pass
  */
BaseType_t ST_PLC_StartTimer(ST_PLC_TimerHandle_t timHandle){
	return xTimerStart(timHandle->handle,0);
}


/**
  * @brief  Stop timer.
  * @param  timer structure
  * @retval 0 for fail and 1 for pass
  */
BaseType_t ST_PLC_StopTimer(ST_PLC_TimerHandle_t timHandle){
	return xTimerStop(timHandle->handle,0);
}

/**
  * @brief  Reset timer.
  * @param  timer structure
  * @retval 0 for fail and 1 for pass
  */
BaseType_t ST_PLC_RTOReset(ST_PLC_TimerHandle_t timHandle){
	// Clear timer done bit
	timHandle->done = 0;

	// Error code
	isSuccess = 1;

	//
	if(!xTimerChangePeriod(timHandle->handle, pdMS_TO_TICKS(timHandle->preset), 0) ||
	   !xTimerStop(timHandle->handle,0)){
		isSuccess = 0;
	}
	return isSuccess;
}



/**
  * @brief  Change timer period
  * @param  timer structure
  * @retval 0 for fail and 1 for pass
  */
ST_PLC_ErrorCodes ST_PLC_ChangeTimerPeriod(ST_PLC_TimerHandle_t timHandle, int preset){
	// Error code
	isSuccess = 1;

	// xTimerChangePeriod() starts the timer after period change
	if(!xTimerChangePeriod( timHandle->handle, pdMS_TO_TICKS(preset), 0)){
		isSuccess = 0;
	} else {
	    // Done bit for RTO timer is reset on reset
	    timHandle->done = 0;

	    // Store preset value
		timHandle->preset = preset;
	}

	return isSuccess;
}



/**
  * @brief  Enable timer.
  * @param  timer structure
  * @retval 0 for fail and 1 for pass
  */
ST_PLC_ErrorCodes ST_PLC_EnableTimer(ST_PLC_TimerHandle_t timHandle){
	// Error code
	isSuccess = 1;

	if(timHandle->enable){
		return isSuccess;
	}


	// Store timer preset value for RTO timer
	int RTOpreset = timHandle->preset;

	// Set timer enable bit
	timHandle->enable = 1;

	switch(timHandle->type)
	{
	   case ST_PLC_OnDelayTimer:
		  // Start on delay timer on enable
		  if(!ST_PLC_StartTimer(timHandle)){
			  isSuccess = 0;
		  }
		  break;

	   case ST_PLC_OffDelayTimer:
		   // stop off delay timer on enable
		   if(!ST_PLC_StopTimer(timHandle)){
			  isSuccess = 0;
		  }

		  // Done bit for on delay timer is reset on disable
		  timHandle->done = 1;

		  break;

	  case ST_PLC_RTOTimer:
		  // Check if accumulator value exists and start timer from that value
		  if(timHandle->accumulator != 0){
			  if(!ST_PLC_ChangeTimerPeriod(timHandle,timHandle->accumulator) ||
				 !ST_PLC_StartTimer(timHandle)){
				  isSuccess = 0;
			  }
			  timHandle->accumulator = 0;
			  timHandle->preset = RTOpreset;
		  } else if(!ST_PLC_StartTimer(timHandle)){
			  isSuccess = 0;
		  }
		  break;
	}
    return isSuccess;
}


/**
  * @brief  Disable timer.
  * @param  timer structure
  * @retval 0 for fail and 1 for pass
  */
ST_PLC_ErrorCodes ST_PLC_DisableTimer(ST_PLC_TimerHandle_t timHandle){
	// Error code
	isSuccess = 1;

	if(!timHandle->enable){
		return isSuccess;
	}

	// Clear timer enable bit
	timHandle->enable = 0;

	switch(timHandle->type)
	{
	   case ST_PLC_OnDelayTimer:
		  // On delay timer is stopped on disable
		  if(!ST_PLC_StopTimer(timHandle)){
			  isSuccess = 0;
		  }

		  // Done bit for on delay timer is reset on disable
		  timHandle->done = 0;

		  break;

	   case ST_PLC_OffDelayTimer:
		  // Off delay timer is stopped on disable
		  if(!ST_PLC_StartTimer(timHandle)){
			  isSuccess = 0;
		  }
		  break;

	   case ST_PLC_RTOTimer:
		  // Store timer accumulated count
		  timHandle->accumulator = xTimerGetExpiryTime(timHandle->handle) - xTaskGetTickCount();

		  // On delay timer is stopped on disable and value is stored in accumulator
		  if(!xTimerStop(timHandle->handle,0)){
			  isSuccess = 0;
		  }

		  break;
	}
    return isSuccess;
}
