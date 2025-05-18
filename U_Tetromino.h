#ifndef U_TETRIS_H
#define U_TETRIS_H

#include "GameObject.h"
#include "DataTypes.h"
#include "Renderer.h"
#include <random>
#include <ctime>

void Tetris_onCollisions();
void Tetris_showOverlay();

int tetris[10][5] = {};

GameObject* tetrisBlocks[] = {
    instantiate("block"), instantiate("block"), instantiate("block"), instantiate("block"),
    instantiate("block"), instantiate("block"), instantiate("block"), instantiate("block"),
    instantiate("block"), instantiate("block"), instantiate("block"), instantiate("block"),
    instantiate("block"), instantiate("block"), instantiate("block"), instantiate("block"),
    instantiate("block"), instantiate("block"), instantiate("block"), instantiate("block"),
    instantiate("block"), instantiate("block"), instantiate("block"), instantiate("block"),
    instantiate("block"), instantiate("block"), instantiate("block"), instantiate("block"),
    instantiate("block"), instantiate("block"), instantiate("block"), instantiate("block"),
    instantiate("block"), instantiate("block"), instantiate("block"), instantiate("block"),
    instantiate("block"), instantiate("block"), instantiate("block"), instantiate("block")
};



void Tetris_userStart() {
    pov = 3;
    d.setRotation(3);
    GameRotation = 3;
    playing = true;
    Tetris_onCollisions();
    setRenderUIFunc(Tetris_showOverlay, &playing);

    for(int i = 0; i < 40; i++) {
        tetrisBlocks[i]->enabled = true;
        tetrisBlocks[i]->staticObject = true;
        tetrisBlocks[i]->setPosition(50, 50, +20);
    }
}


int Tetris_overlayMovement = 0;

int currentBlock[] = {9, 2};
int oldCurrentBlock[] = {0, 0};
int currentBlockId = 0;
bool currentBlockMoving = false;

int iterations = 0;
int frames = 0; // count of how many frames passed
float frameTime = 40.0; // how many frames to calculate gravity
float frameChange = 0.4; // how much to decrease frameTime as each frame takes longer as more 3d objects are rendered


bool pressed1 = false; // player wont be able to press the button continuously
bool pressed2 = false;


void Tetris_gameLoop() {
    if (!playing) return;
    Tetris_overlayMovement++;
    frames++;


    if(!currentBlockMoving){
        if(tetris[9][2] == 0){
            currentBlock[0] = 9;currentBlock[1] = 2;
            if(currentBlockId == 0){
                tetris[9][2] = 1;
            }
        }
        currentBlockMoving = true;
    }
    else{
        if(frames > frameTime){
            frames = 0;
            iterations ++;
            //frameTime -= frameChange;
            //frameChange += 0.4;
            Serial.println(String(frameTime));
            if(currentBlockId == 0){
                if(tetris[currentBlock[0]-1][currentBlock[1]] == 0){
                    tetris[currentBlock[0]][currentBlock[1]] = 0;
                    tetris[currentBlock[0]-1][currentBlock[1]] = 1;
                    
                    currentBlock[0] = currentBlock[0]-1;
                    currentBlock[1] = currentBlock[1];
                }
                else{
                    currentBlockMoving = false;
                }
                
            }
            
        }
        if(button2 == HIGH && !pressed2){
            pressed2 = true;

            if(currentBlockId == 0){
                if(tetris[currentBlock[0]][currentBlock[1]-1] == 0 && currentBlock[1]!=0){
                    tetris[currentBlock[0]][currentBlock[1]] = 0;
                    tetris[currentBlock[0]][currentBlock[1]-1] = 1;
                    
                    currentBlock[0] = currentBlock[0];
                    currentBlock[1] = currentBlock[1]-1;        
                }
            }
        }
        if(button1 == HIGH && !pressed1){
            pressed1 = true;

            if(currentBlockId == 0){
                if(tetris[currentBlock[0]][currentBlock[1]+1] == 0 && currentBlock[1]!=4){
                    tetris[currentBlock[0]][currentBlock[1]] = 0;
                    tetris[currentBlock[0]][currentBlock[1]+1] = 1;
                    
                    currentBlock[0] = currentBlock[0];
                    currentBlock[1] = currentBlock[1]+1;
                }
            }
        }
        if(button2 == LOW) pressed2 = false;
        if(button1 == LOW) pressed1 = false;
    } 
    

    // set block position according to tetris array
    if(oldCurrentBlock[0] != currentBlock[0] || oldCurrentBlock[1] != currentBlock[1]){
        int blockNum = 0;
        for(int i = 0; i < 10; i++){
            for(int j = 0; j < 5; j++){
                if(tetris[i][j] == 1){
                    tetrisBlocks[blockNum]->setPosition(j*2.5-5, i*2.7-14, -25);
                    blockNum++;
                    if(blockNum>=40) return;
                }
            }
        }
    }
    oldCurrentBlock[0] = currentBlock[0];
    oldCurrentBlock[1] = currentBlock[1];


}


void Tetris_onCollisions() { // tetris doesnt really use collisions
    playing = true; // temp
    //AstroBelt_player->onCollisionExecute("rock", Tetris_loss);
}


void Tetris_showOverlay() {
    for (int i = 0; i < 128; i++) {
        if (((i + (int)(Tetris_overlayMovement / 10)) % 20 > 3)) {
            DrawPixel(0, i, 1);
        }
    }
    for (int i = 0; i < 128; i++) {
        if (((i + (int)((200 - Tetris_overlayMovement) / 10)) % 20 > 3)) {
            DrawPixel(63, i, 1);
        }
    }
    Tetris_overlayMovement++;
    if (Tetris_overlayMovement > 200) Tetris_overlayMovement = 0;

    Write(2, 1, 1, "Tetromino");
}

#endif