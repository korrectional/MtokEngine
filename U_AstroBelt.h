#ifndef U_ASTROBELT_H
#define U_ASTROBELT_H

#include "GameObject.h"
#include "DataTypes.h"
#include "Renderer.h"
#include <random>
#include <ctime>

void AstroBelt_onCollisions();

GameObject* AstroBelt_player = instantiate("player");
GameObject* AstroBelt_rock1 = instantiate("rock");
GameObject* AstroBelt_rock2 = instantiate("rock");
GameObject* AstroBelt_rock3 = instantiate("rock");
GameObject* AstroBelt_rocks[3] = {AstroBelt_rock1, AstroBelt_rock2, AstroBelt_rock3};

float AstroBelt_randomVal() {
    return (float)rand() / RAND_MAX;
}

int AstroBelt_overlayMovement = 0;
void AstroBelt_showOverlay();
bool AstroBelt_gameover = false;
bool AstroBelt_r1move = true;
bool AstroBelt_r2move = false;
bool AstroBelt_r3move = false;

void AstroBelt_setupGame() {
    playing = true;
    AstroBelt_gameover = false;
    AstroBelt_player->setPosition(0.0, -1.5, -6.0);
    AstroBelt_player->scaleObject(0.6, 0.5, 0.5);
    AstroBelt_rock1->setPosition(-1, -1.5, -50);
    AstroBelt_rock2->setPosition(0, -1.5, -50);
    AstroBelt_rock3->setPosition(1, -1.5, -50);
    AstroBelt_rocks[0]->scaleObject((AstroBelt_randomVal() * 0.3) + 0.4, (AstroBelt_randomVal() * 0.3) + 0.4, (AstroBelt_randomVal() * 0.3) + 0.4);
    AstroBelt_rocks[1]->scaleObject((AstroBelt_randomVal() * 0.3) + 0.4, (AstroBelt_randomVal() * 0.3) + 0.4, (AstroBelt_randomVal() * 0.3) + 0.4);
    AstroBelt_rocks[2]->scaleObject((AstroBelt_randomVal() * 0.3) + 0.4, (AstroBelt_randomVal() * 0.3) + 0.4, (AstroBelt_randomVal() * 0.3) + 0.4);
    AstroBelt_r1move = true;
    AstroBelt_r2move = false;
    AstroBelt_r3move = false;
}

void AstroBelt_userStart() {
    AstroBelt_setupGame();
    AstroBelt_onCollisions();
    srand(time(0));
    setRenderUIFunc(AstroBelt_showOverlay, &playing);
}

void AstroBelt_restart() {
    ClearDisplay();
    playing = true;
    AstroBelt_gameover = false;
    AstroBelt_setupGame();
}

bool AstroBelt_UIenabled = true;
void AstroBelt_gameLoop() {
    if (playing) {
        if (button1 == HIGH && AstroBelt_player->position.x < 5) AstroBelt_player->move(0.1, 0, 0); // Move object based on input
        if (button2 == HIGH && AstroBelt_player->position.x > -5) AstroBelt_player->move(-0.1, 0, 0);

        if (AstroBelt_r1move) AstroBelt_rock1->move(0, 0, 0.2);
        if (AstroBelt_r2move) AstroBelt_rock2->move(0, 0, 0.2); else if (AstroBelt_rock1->position.z > -15) AstroBelt_r2move = true;
        if (AstroBelt_r3move) AstroBelt_rock3->move(0, 0, 0.2); else if (AstroBelt_rock2->position.z > -15) AstroBelt_r3move = true;

        for (int rock = 0; rock < 3; rock++) {
            if (AstroBelt_rocks[rock]->position.z > 0) {
                AstroBelt_rocks[rock]->setPosition((AstroBelt_randomVal() * 8.0) - 4.0, -1.5, -50);
                AstroBelt_rocks[rock]->scaleObject((AstroBelt_randomVal() * 0.3) + 0.4, (AstroBelt_randomVal() * 0.3) + 0.4, (AstroBelt_randomVal() * 0.3) + 0.4);
            }
        }
        AstroBelt_overlayMovement++;
    } else if (AstroBelt_gameover) {
        if (button1 == HIGH) {
            AstroBelt_UIenabled = !AstroBelt_UIenabled;
        }
        if (button2 == HIGH) {
            AstroBelt_restart();
        }
    }
}

void AstroBelt_loss() {
    playing = false;
    AstroBelt_gameover = true;
    ClearDisplay(); // display gameover
    Write(11, 15, 2.5, "GAME OVER");
    Write(0, 43, 1, "R to restart\nL to disable UI");
    Display();
    delay(800);
}

void AstroBelt_onCollisions() { // set what happens when x object collides with object y
    AstroBelt_player->onCollisionExecute("rock", AstroBelt_loss);
}

void AstroBelt_showOverlay() {
    if (!AstroBelt_UIenabled) return;
    for (int i = 0; i < 64; i++) {
        if (((i + (int)(AstroBelt_overlayMovement / 10)) % 20 > 3)) {
            DrawPixel(0, i, 1);
        }
    }
    for (int i = 0; i < 64; i++) {
        if (((i + (int)((200 - AstroBelt_overlayMovement) / 10)) % 20 > 3)) {
            DrawPixel(127, i, 1);
        }
    }
    AstroBelt_overlayMovement++;
    if (AstroBelt_overlayMovement > 200) AstroBelt_overlayMovement = 0;

    Write(2, 1, 1, "AstroBelt");
}

#endif