#ifndef GLCD_HAVR_h
#define GLCD_HAVR_h
#include <Arduino.h>

class GLCD_HAVR 
{
    public: 
        //GLCD_HAVR();
        //~<GLCD_HAVR();
            GLCD_HAVR (uint8_t _GLCD_E, uint8_t _GLCD_RW, uint8_t _GLCD_DI,uint8_t GLCD_CS1, uint8_t _GLCD_RST, uint8_t _GLCD_CS2, uint8_t _GLCD_CS3);
            void glcd_writeByte(uint8_t _side1, byte _data);
            void glcd_init(uint8_t _mode);
            void glcd_pixel(uint8_t _x, uint8_t _y, boolean _color);
            void glcd_fillScreen(boolean _color);
            byte glcd_readByte(uint8_t _side2);
   private: 
       byte data; 
       uint8_t x,y,mode,side1,side2; 
       boolean color; 
       uint8_t GLCD_E, GLCD_RW, GLCD_DI, GLCD_CS1, GLCD_RST, GLCD_CS2, GLCD_CS3;
};
#endif 
