#ifndef U_TETRIS_H
#define U_TETRIS_H

#include "GameObject.h"
#include "DataTypes.h"
#include "Renderer.h"
#include <random>
#include <ctime>

void Tetris_onCollisions();
void Tetris_showOverlay();




void Tetris_userStart() {
    playing = true;
    Tetris_onCollisions();
    setRenderUIFunc(Tetris_showOverlay, &playing);
}


int Tetris_overlayMovement = 0;

void Tetris_gameLoop() {
    if (playing) {
        Tetris_overlayMovement++;
    }
}


void Tetris_onCollisions() { // tetris doesnt really use collisions
    playing = true; // temp
    //AstroBelt_player->onCollisionExecute("rock", Tetris_loss);
}


void Tetris_showOverlay() {
    for (int i = 0; i < 64; i++) {
        if (((i + (int)(Tetris_overlayMovement / 10)) % 20 > 3)) {
            DrawPixel(0, i, 1);
        }
    }
    for (int i = 0; i < 64; i++) {
        if (((i + (int)((200 - Tetris_overlayMovement) / 10)) % 20 > 3)) {
            DrawPixel(127, i, 1);
        }
    }
    Tetris_overlayMovement++;
    if (Tetris_overlayMovement > 200) Tetris_overlayMovement = 0;

    Write(2, 1, 1, "Tetris");
}

#endif