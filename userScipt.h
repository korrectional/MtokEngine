#include "GameObject.h"
#include "DataTypes.h"



GameObject* player = instantiate("player");

GameObject* rock = instantiate("rock");


void userStart(){
    player->scaleObject(0.7,0.6,0.6);
    player->setPosition(0,0,-4);

    rock->setPosition(0,0,-50);
}


void gameLoop(){
    if (button1 == HIGH) player->move(0.1, 0, 0); // Move object based on input
    if (button2 == HIGH) player->move(-0.1, 0, 0);

    rock->move(0,0,0.2);
    if(rock->position.z > 0){
        rock->setPosition(0,0,-50);
    }
}



