#ifndef RENDERER_H
#define RENDERER_H


#include <SPI.h> // ignore these errors !!!
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
#include <iostream>
#include <algorithm>


#define NUMFLAKES 10
#define XPOS 0
#define YPOS 1
#define DELTAY 2


#define DISPLAY_HEIGHT 64
#define DISPLAY_WIDTH  128



#define OLED_MOSI     23
#define OLED_CLK      18
#define OLED_DC       16
#define OLED_CS       5
#define OLED_RST      17


// Create the OLED display
Adafruit_SH1106G d = Adafruit_SH1106G(128, 64,OLED_MOSI, OLED_CLK, OLED_DC, OLED_RST, OLED_CS);





void RenderSetup(void){
    d.begin(0, true);
    d.clearDisplay();
}
void ClearDisplay(){
    d.clearDisplay();    
}
void Display(){
    d.display();
}


#endif