#include <gui/screen6_screen/Screen6View.hpp>
#include <touchgfx/Color.hpp>
Screen6View::Screen6View()
{

}

void Screen6View::setupScreen()
{
  Screen6ViewBase::setupScreen();
  
  textArea3.setWildcard(textArea3Buffer);
  textArea3.setColor(Color::getColorFrom24BitRGB(19, 202, 15));
  
  
  textArea5.setWildcard(textArea5Buffer);
  textArea5.setColor(Color::getColorFrom24BitRGB(19, 202, 15));
  
  
  Screen6View::updateTxt(0);
}

void Screen6View::tearDownScreen()
{
    Screen6ViewBase::tearDownScreen();
}


void Screen6View::updateTxt(uint8_t newValue)
{  
  static uint8_t first = 1;
  uint8_t ftmp = 0xFF, rtmp = 0xFF;
  
  if( first == 1)
  {
    first = 0;
  }
  else
  {
    ftmp = ST_PLC_GetSFLASHFlag();
    rtmp = ST_PLC_GetSRAMFlag();
  }
  
  ////
  if(ftmp == 1)
  {
    
    Unicode::snprintf(textArea5Buffer, 5, "PASS\n");
    textArea5.invalidate();  //Text area 6 :FLASH
  }
  else if (ftmp == 0xFF)
  {
    Unicode::snprintf(textArea5Buffer, 5, "----\n");
    textArea5.invalidate();  //Text area 6 :FLASH
  }
  else
  {
    Unicode::snprintf(textArea5Buffer, 5, "FAIL\n");
    textArea5.invalidate();
  }
  
  
  if(rtmp == 1)
  {
    Unicode::snprintf(textArea3Buffer, 5, "PASS\n");
    textArea3.invalidate();  //SRAM
  }
  else if(rtmp == 0xFF)
  {
    Unicode::snprintf(textArea3Buffer, 5, "----\n");
    textArea3.invalidate();  //SRAM
  }
  else
  {
    Unicode::snprintf(textArea3Buffer, 5, "FAIL\n");
    textArea3.invalidate();  //SRAM
  }
 
}


void Screen6View::handleTickEvent()
{  
  static uint16_t cnt = 0;
  
   uint16_t progVal;
   progVal = ST_PLC_Set_Progress();
    
//   boxProgress1.setValue(progVal); //BMP

   
   if(cnt < 100)
   {
     cnt++;

   }
   else
   {
          Screen6View::updateTxt(0);
     cnt  = 0;
   }
}