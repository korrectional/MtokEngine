#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
#include <iostream>
#include <algorithm>
#include "GameObject.h"
#include "DataTypes.h"
#include "Input.h"
#include "GameManager.h"
#include "U_AstroBelt.h"
#include "U_Tetromino.h"



#define NUMFLAKES 10
#define XPOS 0
#define YPOS 1
#define DELTAY 2


#define DISPLAY_HEIGHT 64
#define DISPLAY_WIDTH  128



void setup(void){
    RenderSetup();
    InputSetup();
    GameManager_start();
}




void loop(void){
    


    InputLoop();
    GameManager_loop();
    if(playing) { // clearing, display and such are all handled by the game's built in UI features
        ClearDisplay(); // so in practice, playing means that the 3d renderer is running
        GameObjectRenderLoop();
        RenderUI();
        // AAAHHH THE CATS ASSASINATION OF THE SCREEN!
        //for(int i = 0; i < 128; i++){
        //    for(int j = 0; j < 64; j++){
        //        DrawPixel(i, j, 1);
        //    }
        //}

        Display();
    }
    delay(8);
}