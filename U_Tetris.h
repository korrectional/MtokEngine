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
    instantiate("block"),
    instantiate("block"),
    instantiate("block"),
    instantiate("block"),
    instantiate("block"),
    instantiate("block"),
    instantiate("block"),
    instantiate("block"),
    instantiate("block"),
    instantiate("block"),
    instantiate("block"),
    instantiate("block"),
    instantiate("block"),
    instantiate("block"),
    instantiate("block"),
    instantiate("block"),
    instantiate("block"),
    instantiate("block"),
    instantiate("block"),
    instantiate("block"),
    instantiate("block"),
};



void Tetris_userStart() {
    d.setRotation(3);
    GameRotation = 3;
    playing = true;
    Tetris_onCollisions();
    setRenderUIFunc(Tetris_showOverlay, &playing);

    for(int i = 0; i < 20; i++) {
        tetrisBlocks[i]->enabled = true;
        tetrisBlocks[i]->staticObject = true;
        tetrisBlocks[i]->setPosition(50, 50, -20);
    }
    tetrisBlocks[5]->setPosition(10, -10,-15);


    // temp
    tetris[0][0] = 1;
    tetris[0][1] = 1;
    tetris[1][0] = 1;
    tetris[1][1] = 1;
    tetris[1][2] = 1;
    tetris[2][1] = 1;
}


int Tetris_overlayMovement = 0;

void Tetris_gameLoop() {
    if (!playing) return;
    
    Tetris_overlayMovement++;
    
    // test vert
    tetrisBlocks[5]->staticObject = false;
    tetrisBlocks[5]->move(-0.1,-0.1,0);


    


    // set block position according to tetris array
    int blockNum = 0;
    for(int i = 0; i < 10; i++){
        for(int j = 0; j < 5; j++){
            if(tetris[i][j] == 1){
                tetrisBlocks[blockNum]->setPosition(j*2.5-7, i*2.5-5, -20);
                blockNum++;
                if(blockNum>19) return;
            }
        }
    }


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

    Write(2, 1, 1, "Tetris");
}

#endif