/**
  ******************************************************************************
  * File Name          : STM32TouchController.cpp
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */

/* USER CODE BEGIN STM32TouchController */

#include <STM32TouchController.hpp>
#include <TouchGFXHAL.hpp>
#include <gt911.hpp>
#include <stm32f7xx_hal.h>
#include <touchgfx/hal/OSWrappers.hpp>
#include "gt911.hpp"
#include <iostream>

//static TS_DrvTypeDef* tsDriver;
extern I2C_HandleTypeDef hi2c4;
extern GT911_Dev Dev_Now,Dev_Backup;


void STM32TouchController::init()
{
  
//  /* Initialize the TS driver structure */
//    tsDriver = &gt911_ts_drv;
//
//    /* Initialize the TS driver */
//    tsDriver->Start();
//    tsDriver->Init();
//    
//    tsDriver->ReadID();
//  GT911_TEST();
  
  GT911_TEST();  //BMP
}


//static uint8_t TestFlag = 0;

int32_t TestX, TestY;

uint8_t TouchFlag = 0;
uint16_t x_Value =0;
uint16_t y_value =0;


    /**
     * By default sampleTouch returns false,
     * return true if a touch has been detected, otherwise false.
     *
     * Coordinates are passed to the caller by reference by x and y.
     *
     * This function is called by the TouchGFX framework.
     * By default sampleTouch is called every tick, this can be adjusted by HAL::setTouchSampleRate(int8_t);
     *
     */
bool STM32TouchController::sampleTouch(int32_t& x, int32_t& y)
{
  static uint16_t prevX = 0, prevY = 0;
//if(Touch == 1)
//{
///* Get each touch coordinates */
////tsDriver->GetXY(TS_I2C_ADDRESS, (uint16_t*)&y, (uint16_t*)&x);
//*(&x)= x_Value;
//*(&y) = y_value;
//Touch =0;
////return true;
//}
//else
//{
//
//// return false;
//}
//
//return true;
  
  GT911_Scan();
  
if ((prevX != Dev_Now.X[0]) ||(prevY != Dev_Now.Y[0]))
{
  *(&y) = (uint16_t)(3*Dev_Now.X[0])/10;
  *(&x) = (uint16_t)(2*Dev_Now.Y[0])/3;   
  prevX = Dev_Now.X[0];
  prevY = Dev_Now.Y[0];
  return true;
} 
else
{
  return false;
}

}




///**
//  * @brief  Writes a single data.
//  * @param  Addr: I2C address
//  * @param  Reg: Reg address
//  * @param  Value: Data to be written
//  * @retval None
//  */
//void TS_IO_Write(uint8_t Addr, uint8_t Reg, uint8_t Value)
//{
//    I2Cx_WriteMultiple(&hi2c4, Addr, (uint16_t)Reg, I2C_MEMADD_SIZE_8BIT, (uint8_t*)&Value, 1);
//}

///**
//  * @brief  Reads a single data.
//  * @param  Addr: I2C address
//  * @param  Reg: Reg address
//  * @retval Data to be read
//  */
//uint8_t TS_IO_Read(uint8_t Addr, uint8_t Reg)
//{
//    uint8_t read_value = 0;
//
//    I2Cx_ReadMultiple(&hi2c4, Addr, Reg, I2C_MEMADD_SIZE_8BIT, (uint8_t*)&read_value, 1);
//
//    return read_value;
//}

/**
  * @brief  TS delay
  * @param  Delay: Delay in ms
  * @retval None
  */
void TS_IO_Delay(uint32_t Delay)
{
    HAL_Delay(Delay);
}

/* USER CODE END STM32TouchController */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
