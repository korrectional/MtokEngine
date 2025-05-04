#include <SPI.h> // ignore these errors !!!
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
#include <iostream>
#include <algorithm>
#include "GameObject.h"
#include "DataTypes.h"



#define NUMFLAKES 10
#define XPOS 0
#define YPOS 1
#define DELTAY 2


#define DISPLAY_HEIGHT 64
#define DISPLAY_WIDTH  128









GameObject* object = instantiate();
GameObject* object2 = instantiate();


int button1, button2; // input


void setup(void){
    d.begin(0, true);
    d.clearDisplay();
    Serial.begin(115200); // for debugging purposes
    Serial.println("hii");

    // for input from pin 
    pinMode(21, INPUT);
    digitalWrite (21, HIGH);
    pinMode(19, INPUT);
    digitalWrite (19, HIGH);





    // temporary
    object2->setPosition(-3.0,0.0,-4.0);
    object2->staticObject = true;
    object->scaleObject(0.5,0.5,0.5);
}





void gameLoop(){
    if (button1 == HIGH) object->move(0.1, 0, 0); // Move object based on input
    if (button2 == HIGH) object->move(-0.1, 0, 0);

    //if (button1 == HIGH) object2.move(0, 0, 0.1); // Move object based on input
    //if (button2 == HIGH) object2.move(0, 0, -0.1);
    //if (object2.position.y > 3) direction2 = -0.1;
    //if (object2.position.y < -3) direction2 = 0.1;
    //if (button2 == HIGH) object2.move(0, direction2, 0); // Move object2 based on input
}

void renderLoop(){

    object->calculateSC();

    object->render();

    for(int i = 0; i < GameObjectCount; i++){
        if(GameObjectArray[i]->staticObject) break;
        for(int j = (i+1); j < GameObjectCount; j++){
            GameObjectArray[i]->moveP(GameObjectArray[j]->calculateCollision(GameObjectArray[i]));
        }
    }
    
    object2->calculateSC();

    object2->render();
}



void loop(void){
    d.clearDisplay();
    
    gameLoop();
    renderLoop();

    d.display();


    // input
    button1 = digitalRead(21);
    button2 = digitalRead(19);


    delay(8);
}

