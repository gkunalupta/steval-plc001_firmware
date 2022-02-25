/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f7xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
/** boolean for false condition */
#ifndef FALSE
#define FALSE (0)
#endif
/** boolean for true condition */
#ifndef TRUE
#define TRUE  (1)
#endif
#include "stdbool.h"
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */
typedef struct {
 GPIO_TypeDef* gpio;
 uint16_t pin;
} GPIO_port_pin_t;

typedef uint32_t ST_PLC_ErrorCode;

typedef enum {ST_PLC_NONE,
ST_PLC_DIDO, 
ST_PLC_Info,
ST_PLC_Info_DIDO,
ST_PLC_LadderLogic,
ST_PLC_DODI_Selftest,
ST_PLC_Userdefined,
ST_PLC_BoardConfig
}ST_PLC_Usecase;

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define DI_I1_0_Pin GPIO_PIN_2
#define DI_I1_0_GPIO_Port GPIOE
#define DI_I1_1_Pin GPIO_PIN_3
#define DI_I1_1_GPIO_Port GPIOE
#define DI_I1_2_Pin GPIO_PIN_4
#define DI_I1_2_GPIO_Port GPIOE
#define DI_I1_3_Pin GPIO_PIN_5
#define DI_I1_3_GPIO_Port GPIOE
#define DI_FLT_Q1_0_Pin GPIO_PIN_6
#define DI_FLT_Q1_0_GPIO_Port GPIOE
#define DI_FLT_Q1_1_Pin GPIO_PIN_13
#define DI_FLT_Q1_1_GPIO_Port GPIOC
#define SPI5_GPIO_NSS_Pin GPIO_PIN_6
#define SPI5_GPIO_NSS_GPIO_Port GPIOF
#define SPI5_HOLD_Pin GPIO_PIN_1
#define SPI5_HOLD_GPIO_Port GPIOA
#define DO_Q0_EN_Pin GPIO_PIN_5
#define DO_Q0_EN_GPIO_Port GPIOC
#define LED2_or_L_ON_Pin GPIO_PIN_2
#define LED2_or_L_ON_GPIO_Port GPIOB
#define SPI2_NSS_Q0_Pin GPIO_PIN_12
#define SPI2_NSS_Q0_GPIO_Port GPIOB
#define LED1_or_OL_OFF_Pin GPIO_PIN_14
#define LED1_or_OL_OFF_GPIO_Port GPIOB
#define DO_Q1_0_Pin GPIO_PIN_15
#define DO_Q1_0_GPIO_Port GPIOB
#define DO_Q1_1_Pin GPIO_PIN_11
#define DO_Q1_1_GPIO_Port GPIOD
#define DO_Q1_2_Pin GPIO_PIN_8
#define DO_Q1_2_GPIO_Port GPIOC
#define DI_FLT_Q1_OL_Pin GPIO_PIN_9
#define DI_FLT_Q1_OL_GPIO_Port GPIOC
#define DI_FLT_Q1_FLT_Pin GPIO_PIN_8
#define DI_FLT_Q1_FLT_GPIO_Port GPIOA
#define DO_Q1_3_Pin GPIO_PIN_12
#define DO_Q1_3_GPIO_Port GPIOC
#define DI_FLT_Q1_2_Pin GPIO_PIN_4
#define DI_FLT_Q1_2_GPIO_Port GPIOD
#define DO_LCD_DISP_Pin GPIO_PIN_5
#define DO_LCD_DISP_GPIO_Port GPIOD
#define DO_SPI2_NSS_I1_Pin GPIO_PIN_6
#define DO_SPI2_NSS_I1_GPIO_Port GPIOD
#define DI_Q0_PGOOD_Pin GPIO_PIN_13
#define DI_Q0_PGOOD_GPIO_Port GPIOG
#define DI_Q0_FLT_Pin GPIO_PIN_14
#define DI_Q0_FLT_GPIO_Port GPIOG
#define DI_FLT_Q1_3_Pin GPIO_PIN_5
#define DI_FLT_Q1_3_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */
//#define  Channel1 Get_Input_WriteByte(&Data)
#define DebugLed1Port GPIOB
#define DebugLed1Pin GPIO_PIN_2 
#define DebugLed2Port GPIOB
#define DebugLed2Pin GPIO_PIN_14
#define DebugLed3Port GPIOG
#define DebugLed3Pin GPIO_PIN_2


void       SPIx_Error(void);
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __ST_PLC_TIMERAPI_H */





/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
