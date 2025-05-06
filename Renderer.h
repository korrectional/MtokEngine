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

#define WHITE 1

// Create the OLED display
Adafruit_SH1106G d = Adafruit_SH1106G(128, 64,OLED_MOSI, OLED_CLK, OLED_DC, OLED_RST, OLED_CS);



int pov = 2;

void RenderSetup(void){
    d.begin(0, true);
    d.setCursor(0, 0);
    d.setTextColor(WHITE);
    d.setTextSize(1);
    d.clearDisplay();
}
void ClearDisplay(){
    d.clearDisplay();    
}
void Display(){
    d.display();
}
// if x and y are 1111 then just continue from last point
void Write(int x, int y, int scale, const String text){
    if(x!=1111&&y!=1111) d.setCursor(x, y);
    d.setTextSize(scale);
    d.print(text);
}
void DrawPixel(int x, int y, int color){
    d.drawPixel(x, y, color);
}

void (*renderUI)();
bool *renderUIwhen; 
void setRenderUIFunc(void (*function)(), bool *when){
    renderUI = function;
    renderUIwhen = when;
}
void RenderUI(){
    if(renderUI && renderUIwhen){
        renderUI();
    }
}


#endif