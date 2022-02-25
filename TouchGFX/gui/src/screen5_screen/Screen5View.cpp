#include <gui/screen5_screen/Screen5View.hpp>
#include <touchgfx/Color.hpp>
#include <iostream>
Screen5View::Screen5View()
{

}


 static uint8_t hexValue = 88;
extern struct Module_Status IO_ModuleStatus_Fault[5];

void Screen5View::setupScreen()
{
  //  textArea7.setTypedText(touchgfx::TypedText(T_SINGLEUSEID27));
  //Screen5ViewBase::setupScreen();
  //textArea7.setXY(6, 132);
  textArea7.setWildcard(textArea7Buffer);
  textArea7.setColor(Color::getColorFrom24BitRGB(48, 117, 8));
  
  
  textArea8.setWildcard(textArea8Buffer);
  textArea8.setColor(Color::getColorFrom24BitRGB(48, 117, 8));
  
  
  textArea9.setWildcard(textArea9Buffer);
  textArea9.setColor(Color::getColorFrom24BitRGB(48, 117, 8));
  
  
  textArea10.setWildcard(textArea10Buffer);
  textArea10.setColor(Color::getColorFrom24BitRGB(48, 117, 8));
  
  
  //add(textArea7);
  Screen5View::updateTxt(hexValue);
  
}

void Screen5View::tearDownScreen()
{
    Screen5ViewBase::tearDownScreen();
}

    touchgfx::Unicode::UnicodeChar TestBuffer[30];

void Screen5View::updateTxt(uint8_t newValue)
{
  //sprintf((char *)TestBuffer,"%2X\n", newValue); //BMP
  
  newValue = IO_ModuleStatus_Fault[3].IO_status;
  Unicode::snprintf(TestBuffer, 20, "%2X\n", newValue);
  Unicode::snprintf(textArea7Buffer, 3, "%2X\n", newValue);
  //  Unicode::snprintf(textArea7Buffer, 3, "%s", TestBuffer);
  textArea7.invalidate();
  
//  if(1)//newValue & 0xF0)
//  {
//        toggleButton1.setVisible(false);
//        toggleButton1.invalidate();
//  }
  
  
  
  newValue = IO_ModuleStatus_Fault[4].IO_status;  
  Unicode::snprintf(textArea8Buffer, 3, "%2X\n", newValue);
  textArea8.invalidate();
  
  newValue = IO_ModuleStatus_Fault[1].IO_status;
  newValue = ST_PLC_Util_ReverseBits(newValue);
  Unicode::snprintf(textArea9Buffer, 3, "%2X\n", newValue);
  textArea9.invalidate();
  
  newValue = IO_ModuleStatus_Fault[2].IO_status;
  Unicode::snprintf(textArea10Buffer, 3, "%2X\n", newValue);
  textArea10.invalidate();
  
  newValue = IO_ModuleStatus_Fault[1].Fault_status;
  Unicode::snprintf(textArea11Buffer, 3, "%2X\n", newValue);
  textArea11.invalidate();
  
  newValue = IO_ModuleStatus_Fault[3].Fault_status;
  Unicode::snprintf(textArea11_1Buffer, 3, "%2X\n", newValue);
  textArea11_1.invalidate();
  
  newValue = IO_ModuleStatus_Fault[4].Fault_status;
  Unicode::snprintf(textArea11_1_1Buffer, 3, "%2X\n", newValue);
  textArea11_1_1.invalidate();
  
  
  
}




void Screen5View::handleTickEvent()
{
  
  static uint16_t cnt = 0;
  
 
//   static uint8_t hexValue = 0;
//   hexValue = ST_PLC_GetHexIOValue();
  //  textArea7.setColor(Color::getColorFrom24BitRGB(48, 117, 8));
   //     textArea7.setWildcard(textArea7Buffer);
//   Screen5View::updateTxt(hexValue);
   
   
   
   if(cnt< 1)
     cnt++;
   else
   {
        hexValue = ST_PLC_GetHexIOValue();
     cnt =0;
        Screen5View::updateTxt(hexValue);
   }
       
}
