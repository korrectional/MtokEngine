#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
#include <iostream>
#include <algorithm>
#include "GameObject.h"
#include "DataTypes.h"
#include "Input.h"
#include "U_AstroBelt.h"


void (*currentGameLoop)();
void GameManager_showOverlay();

void GameManager_start(){
    setRenderUIFunc(GameManager_showOverlay, &playing);
}

void GameManager_loop(){
    if(currentGameLoop){
        currentGameLoop();
        return;
    }
    if(button1 == HIGH){
        currentGameLoop = AstroBelt_gameLoop;
        AstroBelt_userStart();
    }
}


int GameManager_overlayMovement = 0;
void GameManager_showOverlay() {
    for (int i = 0; i < 64; i++) {
        if (((i + (int)(GameManager_overlayMovement / 10)) % 20 > 3)) {
            DrawPixel(0, i, 1);
        }
    }
    for (int i = 0; i < 64; i++) {
        if (((i + (int)((200 - GameManager_overlayMovement) / 10)) % 20 > 3)) {
            DrawPixel(127, i, 1);
        }
    }
    GameManager_overlayMovement++;
    if (GameManager_overlayMovement > 200) GameManager_overlayMovement = 0;

    Write(2, 1, 1, "GameManager");
}



#endif