#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H


#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
#include <iostream>
#include <algorithm>
#include "DataTypes.h"
#include "Renderer.h"

int GameRotation = 0;


class GameObject{ // 192 bytes size, so I can only have one object? Lol arduino be like

    public:
    String name = "player";
    bool enabled = true;

    Point position = { 0, 0, -4 };

    Point corners[8] = { // the coords of each point
        { 1, -1, -1},
        { 1, -1, 1},
        { 1,  1, -1},
        { 1,  1, 1},
        {-1, -1, -1},
        {-1, -1, 1},
        {-1,  1, -1},
        {-1,  1, 1}
    };

    Point scale = {1,1,1};
    
    Point r_corners[8];// the position of each point ON screen

    float collisions[6] = {// hitbox
        1.0, -1.0, 1.0, -1.0, 1.0, -1.0 // x x   y y   z z  (ranges) (colliders are all cuboids)
    };

    bool staticObject = false;


    void move(float x, float y, float z){ // moves the object by x y z using 3 floats as input
        if(staticObject) return;
        position.x += x;
        position.y += y;
        position.z += z;
    }
    void moveP(Point point){ // moves the object by x y z using points as input
        if(staticObject) return;
        position.x += point.x;
        position.y += point.y;
        position.z += point.z;
    }

    void setPosition(float x, float y, float z){ // moves the object by x y z
        position.x = x;
        position.y = y;
        position.z = z;
        //Serial.println("Position set to: (" + String(x) + ", " + String(y) + ", " + String(z) + ")");
    }

    void scaleObject(float x, float y, float z) {
        scale.x = x;
        scale.y = y;
        scale.z = z;
    
        for (int i = 0; i < (sizeof(corners) / sizeof(corners[0])); i++) {
            corners[i].x = (corners[i].x > 0) ? x : -x;
            corners[i].y = (corners[i].y > 0) ? y : -y;
            corners[i].z = (corners[i].z > 0) ? z : -z;
        }
    
        collisions[0] = x;
        collisions[1] = -x; 
        collisions[2] = y;
        collisions[3] = -y; 
        collisions[4] = z;
        collisions[5] = -z; 
    
        //Serial.println("Scale updated to: (" + String(x) + ", " + String(y) + ", " + String(z) + ")");
        for (int i = 0; i < 6; i++) {
            Serial.println("collisions [" + String(i) + "] == " + String(collisions[i]));
        }
    }

    void enable(){
        enabled = true;
    }

    //++++++++++++++++++++++++++++++++++++++++++++++++++COLLISION ENGINE++++++++++++++++++++++++++++++++++++++++++++++++++
    CollisionCallback collisionCallbacks[10];
    String collisionCallbacksNames[10];
    int collisionCallbacksLength = 0;

    void onCollision(GameObject* obj, int initial = 1){
        if(initial == 0){
            obj->onCollision(this);
        }
        for(int i = 0; i < collisionCallbacksLength; i++){ // execute OnCollisionExecute
            if(obj->name == collisionCallbacksNames[i]){
                collisionCallbacks[i]();
            }
        }
    }

    void onCollisionExecute(const String& objName, CollisionCallback callback)
    {
        collisionCallbacks[collisionCallbacksLength] = callback;
        collisionCallbacksNames[collisionCallbacksLength] = objName;
        collisionCallbacksLength++;
    }




    // collision related functions
    Point calculateCollision(GameObject* object) { // spagetti
        float posX = position.x;
        float posY = position.y;
        float posZ = position.z;
        float left = collisions[0] + posX;
        float right = collisions[1] + posX;
        float top = collisions[2] + posY;
        float bottom = collisions[3] + posY;
        float front = collisions[4] + posZ;
        float back = collisions[5] + posZ;

        float objLeft = object->collisions[0] + object->position.x;
        float objRight = object->collisions[1] + object->position.x;
        float objTop = object->collisions[2] + object->position.y;
        float objBottom = object->collisions[3] + object->position.y;
        float objFront = object->collisions[4] + object->position.z;
        float objBack = object->collisions[5] + object->position.z;


        bool xCol = (objLeft >= left && left >= objRight) || (objLeft >= right && right >= objRight) || (left >= objLeft && objLeft >= right) || (left >= objRight && objRight >= right);
        bool yCol = (objTop >= top && top >= objBottom) || (objTop >= bottom && bottom >= objBottom) || (top >= objTop && objTop >= bottom) || (top >= objBottom && objBottom >= bottom);
        bool zCol = (objFront >= back && back >= objBack) || (objFront >= front && front >= objBack) || (back >= objFront && objFront >= front) || (back >= objBack && objBack >= front);
        //Serial.println(String(xCol) + " " +  String(yCol)  + " " + String(zCol));

        if(xCol && yCol && zCol){
            //Serial.println("COLLISION!!");
            float xMove = std::min((std::abs(right - objLeft)), (std::abs(left - objRight)));
            float yMove = std::min((std::abs(bottom - objTop)), (std::abs(top - objBottom)));
            float zMove = std::min((std::abs(back - objFront)), (std::abs(front - objBack)));
            
            float min = std::min(xMove, std::min(yMove, zMove));

            if(xMove == min) return {selectDistanceBetweenTwoPoints(right, objLeft, left, objRight), 0, 0};
            if(yMove == min) return {0, selectDistanceBetweenTwoPoints(bottom, objTop, top, objBottom), 0};
            if(zMove == min) return {0, 0, selectDistanceBetweenTwoPoints(back, objFront, front, objBack)}; 
            
            return {xMove, 0, 0};
        }
        return {0,0,0};
    }
        
    float selectDistanceBetweenTwoPoints(float r, float oL, float l, float oR){ // corrected parameter order
        if(std::abs(r - oL) < std::abs(l - oR)){
            return r - oL;
        }
        return l - oR;
    }

    ///////////// rendering related functions
    void calculateSC(){ // calculate r_corners
        for(int i = 0; i < 8; i++){
            float x = (corners[i].x + position.x) / (corners[i].z + position.z)*pov;
            float y = (corners[i].y + position.y) / (corners[i].z + position.z)*pov;
            float end_x;
            float end_y;
            if(GameRotation == 0){
                end_x = (int)((x*64.0+128.0)/2);
                end_y = (int)((y*64.0+64.0)/2);
            }
            else if(GameRotation == 3){
                end_x = (int)((x*64.0+64.0)/2);
                end_y = (int)((y*64.0+128.0)/2);
            }
            r_corners[i].x = end_x;
            r_corners[i].y = end_y;
            //Serial.println(String(end_x) + " " + String(end_y));
        }
    }
    void drawLine(int i1, int i2){
        if((corners[i1].z + position.z) >= 0 || (corners[i2].z + position.z) >= 0) return;

        if(r_corners[i1].x < 0) r_corners[i1].x = 0;
        if(r_corners[i1].y < 0) r_corners[i1].y = 0;
        if(r_corners[i2].x < 0) r_corners[i2].x = 0;
        if(r_corners[i2].y < 0) r_corners[i2].y = 0;

        d.drawLine(r_corners[i1].x, r_corners[i1].y, r_corners[i2].x, r_corners[i2].y, 1);
    }

    void render(){
        drawLine(0, 2); // back square
        drawLine(2, 6);
        drawLine(6, 4);
        drawLine(4, 0);

        drawLine(1, 3); // front square
        drawLine(3, 7);
        drawLine(7, 5);
        drawLine(5, 1);

        drawLine(3, 2); // connectors
        drawLine(7, 6);
        drawLine(5, 4);
        drawLine(1, 0);
    }


};
///////////////////////////////////////////////////////////////////////////////




#define MAX_GAMEOBJECTS 100
GameObject* GameObjectArray[MAX_GAMEOBJECTS];
int GameObjectCount = 0;



GameObject* instantiate(const String& name = "object"){
    if(GameObjectCount>=MAX_GAMEOBJECTS){
        Serial.println("Sorry man, too many objects");
        return nullptr;
    }

    GameObject* newObject = new GameObject();
    newObject->name = name;
    GameObjectArray[GameObjectCount++] = newObject; // Add the new object to the array

    Serial.println("GameObject created: " + newObject->name);
    return newObject; // Return a pointer to the new GameObject
}


GameObject* instantiateAt(float x, float y, float z, const String& name = "object"){
    if(GameObjectCount>=MAX_GAMEOBJECTS){
        Serial.println("Sorry man, too many objects");
        return nullptr;
    }

    GameObject* newObject = new GameObject();
    newObject->name = name + String(GameObjectCount); // Assign a unique name
    GameObjectArray[GameObjectCount++] = newObject; // Add the new object to the array

    Serial.println("GameObject created: " + newObject->name);

    newObject->setPosition(x, y, z);
    newObject->calculateSC();
    return newObject; // Return a pointer to the new GameObject
}

void destroy(GameObject* object) {
    for (int i = 0; i < GameObjectCount; i++) {
        if (GameObjectArray[i] == object) {
            delete GameObjectArray[i]; // Free the memory
            GameObjectArray[i] = GameObjectArray[--GameObjectCount]; // Replace with the last object
            GameObjectArray[GameObjectCount] = nullptr; // Nullify the last slot
            Serial.println("GameObject destroyed.");
            return;
        }
    }
    Serial.println("Error: GameObject not found!");
}


bool playing = true;
void GameObjectRenderLoop(){

    for(int i = 0; i < GameObjectCount; i++){
        if(!GameObjectArray[i]->enabled) continue; // if disabled then just
        for(int j = (i+1); j < GameObjectCount; j++){
            if(!GameObjectArray[j]->enabled) continue; // skip it
            Point move = GameObjectArray[j]->calculateCollision(GameObjectArray[i]);
            if(move.x == 0 && move.y == 0 && move.z == 0){
                continue;    
            }
            GameObjectArray[i]->onCollision(GameObjectArray[j], 0);
            GameObjectArray[i]->moveP(move);
        }
    }

    if(!playing) return;
    for(int i = 0; i < GameObjectCount; i++){
        if(!GameObjectArray[i]->enabled) continue;
        GameObjectArray[i]->calculateSC();
        GameObjectArray[i]->render();
    }
}


#endif