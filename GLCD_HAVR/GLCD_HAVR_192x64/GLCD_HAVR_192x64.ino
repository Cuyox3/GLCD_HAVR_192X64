/*
 * H-AVR
 * Antonio Valerio
 * GLCD V 1.0
 * https://www.taobao.com/list/item/wap/592432481922.htm
 * https://forum.arduino.cc/t/192x64-slg19264b-lcd-display/620322/6
 */
#include "GLCD_HAVR.h"

GLCD_HAVR Glcd=GLCD_HAVR (A2,A1,A0,8,9,10,11);
  void setup()
  {
  DDRD = 0XFF;  
  Glcd.glcd_init(1);
}
  void loop() 
  {
   for(int jj = 0; jj< 64; jj++){
      Glcd.glcd_pixel(jj,jj,false);
   }  
}
