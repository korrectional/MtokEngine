#ifndef INPUT_H
#define INPUT_H




#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
#include <iostream>






int button1, button2; // input


void InputSetup(void){
    Serial.begin(115200); // for debugging purposes
    Serial.println("hii");

    // for input from pin 
    pinMode(21, INPUT);
    digitalWrite (21, HIGH);
    pinMode(19, INPUT);
    digitalWrite (19, HIGH);
}



void InputLoop(void){
    // input
    button1 = digitalRead(21);
    button2 = digitalRead(19);
}



#endif