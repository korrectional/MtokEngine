#ifndef DATATYPES_H
#define DATATYPES_H


#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
#include <iostream>
#include <algorithm>

struct Point {
    float x, y, z;
};
typedef void (*CollisionCallback)();


#endif