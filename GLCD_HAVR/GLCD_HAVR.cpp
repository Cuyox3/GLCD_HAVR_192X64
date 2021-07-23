#include "GLCD_HAVR.h"
#include <Arduino.h> 
//GLCD_HAVR: :GLCD_HAVR(){}     Constructor y destructor
//GLCD_HAVR: :~GLCD_HAVR(){}
#define GLCD_LEFT    0 
#define GLCD_MID     1 
#define GLCD_RIGHT   2

         GLCD_HAVR::GLCD_HAVR (uint8_t _GLCD_E, uint8_t _GLCD_RW, uint8_t _GLCD_DI,uint8_t _GLCD_CS1, uint8_t _GLCD_RST, uint8_t _GLCD_CS2, uint8_t _GLCD_CS3)
    {
    GLCD_E=_GLCD_E;
    GLCD_RW=_GLCD_RW;
    GLCD_DI=_GLCD_DI;
    GLCD_CS1=_GLCD_CS1;
    GLCD_RST=_GLCD_RST;
    GLCD_CS2=_GLCD_CS2;
    GLCD_CS3=_GLCD_CS3;

  pinMode(GLCD_DI, OUTPUT);
  pinMode(GLCD_RW, OUTPUT);
  pinMode(GLCD_E,  OUTPUT);
  pinMode(GLCD_CS1, OUTPUT);
  pinMode(GLCD_RST, OUTPUT);
  pinMode(GLCD_CS2, OUTPUT);
  pinMode(GLCD_CS3, OUTPUT);
            }

          void GLCD_HAVR::glcd_fillScreen(boolean _color)
          {
  color=_color;
   int i, j; 
   for(i = 0; i < 8; ++i) 
   { 
      digitalWrite(GLCD_DI,LOW);                      // Establece para instrucciones 
      glcd_writeByte(GLCD_LEFT, 0b01000000);    // Establece el horizontal a  0 
      glcd_writeByte(GLCD_RIGHT, 0b01000000); 
      glcd_writeByte(GLCD_MID, 0b01000000); 
      glcd_writeByte(GLCD_LEFT, i | 0b10111000);// Establece la pagina de acceso  
      glcd_writeByte(GLCD_RIGHT, i | 0b10111000); 
      glcd_writeByte(GLCD_MID, i | 0b10111000); 
      digitalWrite(GLCD_DI,HIGH);                     // Establece para datos  
      // Pin D/I (Datos/instrucciones)
      // Loop por la seccion horizontal 
      for(j = 0; j < 64; ++j) 
      { 
         if(true){
           glcd_writeByte(GLCD_LEFT, 0xFF);  // Cambiar pixeles on a off
           glcd_writeByte(GLCD_MID, 0xFF); // Cambiar pixeles on a off
           glcd_writeByte(GLCD_RIGHT, 0xFF); // Cambiar pixeles on a off
         }else{
           glcd_writeByte(GLCD_LEFT, 0x00);  // Cambiar pixeles on a off
           glcd_writeByte(GLCD_MID, 0x00); // Cambiar pixeles on a off
           glcd_writeByte(GLCD_RIGHT, 0x00); // Cambiar pixeles on a off
         }
      } 
   } 
}

          void GLCD_HAVR::glcd_pixel(uint8_t _x, uint8_t _y, boolean _color) 
          {
  x=_x;
  y=_y;
  color=_color; 
   byte data; 
   int side3 = GLCD_LEFT;  //Almacena que chip utilizar en la lcd 

   if(( x > 63) && (x<128))              // Verifica la primera y segunda parte de la LCD
   { 
      x -= 64; 
      side3=GLCD_MID; 
   } 
   else if ((x>127)&&(x<192)) 
   {                              
      x -=128; 
      side3=GLCD_RIGHT; 
   }                          
   else 
   { 
      side3=GLCD_LEFT; 
   }                        

   digitalWrite(GLCD_DI,LOW);                         // Set for instruction 
   bitClear(x,7);                                     // Clear the MSB. Part of an instruction code 
   bitSet(x,6);                                       // Set bit 6. Also part of an instruction code 
   glcd_writeByte(side1, x);                           // Set the horizontal address 
   glcd_writeByte(side1, (y/8 & 0xBF) | 0xB8);         // Set the vertical page address 
   digitalWrite(GLCD_DI,HIGH);                        // Set for data 
   glcd_readByte(side2);                               // Need two reads to get data 
   data=glcd_readByte(side2);                          //  at new address 
   //Serial.println(data);                   
   if(color == true) 
      bitSet(data, y%8);               // Turn the pixel on 
   else                                // or 
      bitClear(data, y%8);             // turn the pixel off 
      
   digitalWrite(GLCD_DI,LOW);          // Set for instruction 
   glcd_writeByte(side1, x);            // Set the horizontal address 
   digitalWrite(GLCD_DI,HIGH);         // Set for data                                                              
   glcd_writeByte(side1, data);         // Write the pixel data 
}

            void GLCD_HAVR::glcd_init(uint8_t _mode)
            {   // 0 Apaga display, 1 Enciende display
  mode=_mode;
   // Initialze some pins 
   //Serial.println("Inicio de GLCD");
   digitalWrite(GLCD_RST,HIGH); 
   digitalWrite(GLCD_E,LOW); 
   digitalWrite(GLCD_CS1,HIGH);        
   digitalWrite(GLCD_CS2,HIGH); 
   digitalWrite(GLCD_CS3,HIGH); 
   digitalWrite(GLCD_DI,LOW);                 // Set for instruction 
   glcd_writeByte(GLCD_LEFT, 0xC0);    // Specify first RAM line at the top 
   glcd_writeByte(GLCD_RIGHT, 0xC0);    //   of the screen 
   glcd_writeByte(GLCD_MID, 0xC0); 
   glcd_writeByte(GLCD_LEFT, 0x40);    // Set the column address to 0 
   glcd_writeByte(GLCD_RIGHT, 0x40); 
   glcd_writeByte(GLCD_MID, 0x40); 
   glcd_writeByte(GLCD_LEFT, 0xB8);    // Set the page address to 0 
   glcd_writeByte(GLCD_RIGHT, 0xB8); 
   glcd_writeByte(GLCD_MID, 0xB8); 
    
   if(mode == 1) 
   { 
      //Serial.println("Chip ON");
      glcd_writeByte(GLCD_LEFT, 0x3F); // Turn the display on 
      glcd_writeByte(GLCD_MID, 0x3F); 
      glcd_writeByte(GLCD_RIGHT, 0x3F); 
   } 
   else 
   {                      
      glcd_writeByte(GLCD_LEFT, 0x3E); // Turn the display off 
      glcd_writeByte(GLCD_MID, 0x3E); 
      glcd_writeByte(GLCD_RIGHT, 0x3E); 
   } 
   glcd_fillScreen(false);    // Clear the display 
   //Serial.println("Fin de inicio de GLCD");
   delay(100);                
} 

          void GLCD_HAVR::glcd_writeByte(uint8_t _side1, byte _data) { 
  side1=_side1;
  data=_data;
   DDRD = B11111111;           // Usamos todo el puerto como salida
   digitalWrite(GLCD_RW,LOW);       // Set for writing 
                                      
   if(side1 == 0)             // Choose which side to write to 
   {    
      digitalWrite(GLCD_CS1,LOW); 
      digitalWrite(GLCD_CS2,HIGH); 
      digitalWrite(GLCD_CS3,HIGH);  
   } 
   else if(side1==1)              
   {    
      digitalWrite(GLCD_CS2,LOW);        
      digitalWrite(GLCD_CS1,HIGH);  
      digitalWrite(GLCD_CS3,HIGH);  
   } 
   else                                                                                                                                          
   {    
      digitalWrite(GLCD_CS3,LOW);  
      digitalWrite(GLCD_CS2,HIGH);  
      digitalWrite(GLCD_CS1,HIGH); 
   }                    
   delay(1);              
   PORTD = data;
   delay(1);    
   digitalWrite(GLCD_E,HIGH);       // Pulse the enable pin 
   delay(5);                                                              
   digitalWrite(GLCD_E,LOW); 
   digitalWrite(GLCD_CS1,HIGH);       // Reset the chip select lines 
   digitalWrite(GLCD_CS2,HIGH);   
   digitalWrite(GLCD_CS3,HIGH);  
} 

          byte GLCD_HAVR::glcd_readByte(uint8_t _side2) { 
   side2=_side2;
   byte data;                 // Stores the data read from the LCD 
   DDRD = B00000000;           // Usamos todo el puerto como entrada
   digitalWrite(GLCD_RW,HIGH);      // Set for reading 
   if(side2==0)                   // Choose which side to write to 
   {    
      digitalWrite(GLCD_CS1,LOW); 
      digitalWrite(GLCD_CS2,HIGH); 
      digitalWrite(GLCD_CS3,HIGH);  
   }  
   else if(side2==1) 
   {    
      digitalWrite(GLCD_CS2,LOW); 
      digitalWrite(GLCD_CS1,HIGH); 
      digitalWrite(GLCD_CS3,HIGH);  
   }  
   else 
   {    
      digitalWrite(GLCD_CS3,LOW); 
      digitalWrite(GLCD_CS2,HIGH); 
      digitalWrite(GLCD_CS1,HIGH);  
   }                                                    
   delay(1); 
   digitalWrite(GLCD_E,HIGH);       // Pulse the enable pin 
   delay(4); 
   data = PORTD ;          // Get the data from the display's output register 
   digitalWrite(GLCD_E,LOW); 
   digitalWrite(GLCD_CS1,HIGH);      // Reset the chip select lines 
   digitalWrite(GLCD_CS2,HIGH); 
   digitalWrite(GLCD_CS3,HIGH);
   return data;               // Return the read data 
}   
