#ifndef __GT911_H_
#define __GT911_H_

#include "stm32f7xx_hal.h"

#define GT911_MAX_WIDTH		 800    	//Touchscreen pad max width
#define GT911_MAX_HEIGHT	 480			//Touchscreen pad max height

#define CT_CMD_WR			0XBA					//?????
#define CT_CMD_RD   	0XBB					//?????

#define CT_MAX_TOUCH    5						//????????????

#define GT911_COMMAND_REG   				0x8040	/* ???? */
#define GT911_CONFIG_REG						0x8047	/* ??????? */
#define GT911_PRODUCT_ID_REG 				0x8140 	/* Product ID */
#define GT911_FIRMWARE_VERSION_REG  0x8144  /* ????? */
#define GT911_READ_XY_REG 					0x814E	/* ????? */

#define GT911_RST_CLK()			__GPIOD_CLK_ENABLE()
#define GT911_RST_PORT			GPIOD
#define GT911_RST_PIN				GPIO_PIN_11

#define GT911_INT_CLK()			__GPIOG_CLK_ENABLE() //__GPIOD_CLK_ENABLE()
#define GT911_INT_PORT			GPIOG                //GPIOD
#define GT911_INT_PIN			GPIO_PIN_9    //	GPIO_PIN_7

typedef struct
{
	uint8_t Touch;
	uint8_t TouchpointFlag;
	uint8_t TouchCount;
 
	uint8_t Touchkeytrackid[CT_MAX_TOUCH];
	uint16_t X[CT_MAX_TOUCH];
	uint16_t Y[CT_MAX_TOUCH];
	uint16_t S[CT_MAX_TOUCH];
}GT911_Dev;


	  HAL_StatusTypeDef I2Cx_ReadMultiple(I2C_HandleTypeDef* i2c_handler,
                                           uint8_t Addr,
                                           uint16_t Reg,
                                           uint16_t MemAddress, 
                                           uint8_t* Buffer,
                                           uint16_t Length);
          
           HAL_StatusTypeDef I2Cx_WriteMultiple(I2C_HandleTypeDef* i2c_handler,
                                            uint8_t Addr,
                                            uint16_t Reg,
                                            uint16_t MemAddress, 
                                            uint8_t* Buffer,
                                            uint16_t Length);
void GT911_Scan(void);
void GT911_TEST(void);
   
#endif /*__GT911_H_*/
