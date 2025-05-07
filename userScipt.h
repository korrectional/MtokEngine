#include "GameObject.h"
#include "DataTypes.h"
#include "Renderer.h"
#include <random>
#include <ctime>


void onCollisions();


GameObject* player = instantiate("player");

GameObject* rock1 = instantiate("rock");
GameObject* rock2 = instantiate("rock");
GameObject* rock3 = instantiate("rock");
GameObject* rocks[3] = {rock1, rock2, rock3};

float randomVal(){
    return (float)rand() / RAND_MAX;
}

int overlayMovement = 0;
void showOverlay();
bool gameover = false;
bool r1move = true;
bool r2move = false;
bool r3move = false;



void setupGame(){
    playing = true;
    gameover = false;
    player->setPosition(0.0,-1.5,-6.0);
    player->scaleObject(0.6,0.5,0.5);
    rock1->setPosition(-1,-1.5,-50);
    rock2->setPosition(0,-1.5,-50);
    rock3->setPosition(1,-1.5,-50);
    rocks[0]->scaleObject((randomVal()*0.3)+0.4, (randomVal()*0.3)+0.4, (randomVal()*0.3)+0.4);
    rocks[1]->scaleObject((randomVal()*0.3)+0.4, (randomVal()*0.3)+0.4, (randomVal()*0.3)+0.4);
    rocks[2]->scaleObject((randomVal()*0.3)+0.4, (randomVal()*0.3)+0.4, (randomVal()*0.3)+0.4);
    r1move = true;
    r2move = false;
    r3move = false;
}

void userStart(){
    setupGame();
    onCollisions();
    srand(time(0));
    setRenderUIFunc(showOverlay, &playing);
}



void restart(){
    ClearDisplay();
    playing = true;
    gameover = false;
    setupGame();
}

bool UIenabled = true;
void gameLoop(){
    if(playing){
        if (button1 == HIGH && player->position.x < 5) player->move(0.1, 0, 0); // Move object based on input
        if (button2 == HIGH && player->position.x > -5) player->move(-0.1, 0, 0);
        

        if(r1move) rock1->move(0,0,0.2);
        if(r2move) rock2->move(0,0,0.2); else if(rock1->position.z>-15) r2move = true;
        if(r3move) rock3->move(0,0,0.2); else if(rock2->position.z>-15) r3move = true;
        
        for(int rock = 0; rock < 3; rock++){
            if(rocks[rock]->position.z > 0){
                rocks[rock]->setPosition((randomVal()*8.0)-4.0,-1.5,-50);
                rocks[rock]->scaleObject((randomVal()*0.3)+0.4, (randomVal()*0.3)+0.4, (randomVal()*0.3)+0.4);
            }
        }
        overlayMovement++;
    }
    else if(gameover){
        if(button1==HIGH){
            UIenabled=!UIenabled;
        }
        if(button2==HIGH){
            restart();
        }
    }
}


void loss(){
    playing = false;
    gameover = true;
    ClearDisplay(); // display gameover
    Write(11, 15, 2.5, "GAME OVER");
    Write(0, 43, 1, "R to restart\nL to disable UI");
    Display();
    delay(800);
}
void onCollisions(){ // set what happens when x object collides with object y
    player->onCollisionExecute("rock", loss);
}


void showOverlay(){
    if(!UIenabled) return;
    for(int i = 0; i < 64; i++){
        if(((i+(int)(overlayMovement/10))%20 > 3)){
            DrawPixel(0, i, 1);
        }
    }
    for(int i = 0; i < 64; i++){
        if(((i+(int)((200-overlayMovement)/10))%20 > 3)){
            DrawPixel(127, i, 1);
        }
    }
    overlayMovement++;
    if(overlayMovement>200) overlayMovement = 0;

    Write(2, 1, 1, "AstroBelt");
}
