#include <SPI.h> // ignore these errors !!!
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
#include <iostream>
#include <algorithm>
#include "GameObject.h"
#include "DataTypes.h"
#include "Input.h"
#include "userScipt.h"



#define NUMFLAKES 10
#define XPOS 0
#define YPOS 1
#define DELTAY 2


#define DISPLAY_HEIGHT 64
#define DISPLAY_WIDTH  128





void setup(void){
    RenderSetup();
    InputSetup();
}









void loop(void){
    InputLoop();
    gameLoop();
    ClearDisplay();
    GameObjectRenderLoop();
    Display();
    delay(8);
}